/*
 *  NDBaseRole.mm
 *  DragonDrive
 *
 *  Created by jhzheng on 11-1-7.
 *  Copyright 2011 (����)DeNA. All rights reserved.
 *
 */

#include "NDBaseRole.h"
#include "NDConstant.h"
#include "NDPath.h"
#include "EnumDef.h"
#include "NDManualRole.h"
#include "NDNpc.h"
#include "NDRidePet.h"
#include "CCPointExtension.h"
#include "AnimationList.h"
#include "NDNode.h"
#include "NDDirector.h"
#include "NDMapLayer.h"
#include "Item.h"
#include "GameScene.h"
//#include "NDUtility.h"
#include "NDUtil.h"
#include "define.h"
#include "CCString.h"
#include "NDDebugOpt.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "NDConsole.h"
#endif
#include "ObjectTracker.h"

using namespace NDEngine;

IMPLEMENT_CLASS(NDBaseRole, NDSprite)

bool NDBaseRole::ms_bGameSceneRelease = false;

NDBaseRole::NDBaseRole()
{
	INC_NDOBJ_RTCLS
	m_nMasterWeaponType = WEAPON_NONE;
	m_nSecondWeaponType = WEAPON_NONE;
	m_bIsRide = false;
	m_nSex = -1;
	m_nSkinColor = -1;
	m_nHairColor = -1;
	m_nHair = -1;

	m_nDirect = -1;
	m_nExpresstion = -1;
	m_nModel = -1;
	m_nWeapon = -1;
	m_nCap = -1;
	m_nArmor = -1;

	m_nLife = 0;
	m_nMaxLife = 0;
	m_nMana = 0;
	m_nMaxMana = 0;
	m_nLevel = 0;
	m_eCamp = CAMP_TYPE_NONE;

	m_bFaceRight = true;

	m_pkRidePet = NULL;

	m_bFocus = false;

	m_pkSubNode = NDNode::Node();
	m_pkSubNode->SetContentSize(
			CCDirector::sharedDirector()->getWinSizeInPixels());

	m_kScreenPosition = CCPointZero;
	m_pkRingPic = NULL;
	m_pkPicShadow = NULL;
	m_pkPicShadowBig = NULL;
	m_iShadowOffsetX = 0;
	m_iShadowOffsetY = 10;
	m_bBigShadow = false;
	m_bShowShadow = true;

	m_nID = 0;

	m_pkEffectFlagAniGroup = NULL;

	//m_pkTalkBox = NULL;

	SetDelegate(this);

	m_pkEffectRidePetAniGroup = NULL;
	m_nPetStandAction = 9;
	m_nPetRunAction = 9;
	m_nAccLevel = 0;
	m_nPetLookface = 0;
}

NDBaseRole::~NDBaseRole()
{
	DEC_NDOBJ_RTCLS
	SAFE_DELETE (m_pkSubNode);
	SAFE_DELETE (m_pkRingPic);
	SAFE_DELETE (m_pkPicShadow);
	SAFE_DELETE (m_pkPicShadowBig);
}

CCRect NDBaseRole::GetFocusRect()
{
	CCPoint point;

	if (m_pkRidePet)
	{
		point = m_pkRidePet->GetPosition();
	}
	else
	{
		point = GetPosition();
	}

	if (m_pkRingPic == NULL)
	{
		m_pkRingPic = NDPicturePool::DefaultPool()->AddPicture(RING_IMAGE);
	}

	CCSize sizeRing = m_pkRingPic->GetSize();

	return CCRectMake(point.x - 8 - 13, point.y - 16 - 5, sizeRing.width,
			sizeRing.height);
}

void NDBaseRole::DirectRight(bool bRight)
{
	if (bRight)
	{
		SetSpriteDir(1);
		if (m_pkRidePet)
		{
			m_pkRidePet->SetSpriteDir(1);
		}
	}
	else
	{
		SetSpriteDir(2);
		if (m_pkRidePet)
		{
			m_pkRidePet->SetSpriteDir(2);
		}
	}
}

int NDBaseRole::getFlagId(int index)
{
	int iid = -1;

	switch (index)
	{
	case CAMP_NEUTRAL:
		iid = -1;
		break;
	case CAMP_SUI:
		iid = FLAG_SUI_DYNASTY_1;
		break;
	case CAMP_TANG:
		iid = FLAG_TAN_DYNASTY_1;
		break;
	case CAMP_TUJUE:
		iid = FLAG_TUJUE_DYNASTY_1;
		break;
	case CAMP_FOR_ESCORT:
		iid = FLAG_ESCORT_1;
		break;	//����
	case 5:
		iid = FLAG_TEAM_1;
		break; //����
	}

	return iid;
}

void NDBaseRole::DrawRingImage(bool bDraw)
{
	return;
#if 0
	if (IsKindOfClass (RUNTIME_CLASS(NDNpc)))
	{
		NDNpc* npc = (NDNpc*) this;
		if (!npc->IsActionOnRing())
		{
			return;
		}
	}

	if (m_bFocus && bDraw)
	{
		if (m_pkRingPic == NULL)
		{
			m_pkRingPic = NDPicturePool::DefaultPool()->AddPicture(RING_IMAGE);
		}
		CCSize sizeRing = m_pkRingPic->GetSize();

		if (GetParent())
		{
			NDLayer *layer = (NDLayer*) GetParent();
			CCSize sizemap = layer->GetContentSize();
			if (!m_pkRidePet)
			{
				//m_picRing->DrawInRect(CCRectMake(GetPosition().x-13-8, GetPosition().y-5-16+320-sizemap.height, sizeRing.width, sizeRing.height));
			}
			else
			{
				//m_picRing->DrawInRect(CCRectMake(ridepet->GetPosition().x-13-8, ridepet->GetPosition().y-5-16+320-sizemap.height, sizeRing.width, sizeRing.height));
			}
		}
	}
#endif
}

bool NDBaseRole::OnDrawBegin(bool bDraw)
{
	if (!NDDebugOpt::getDrawRoleEnabled())
		return false;

	NDNode *node = GetParent();
	CCSize sizemap;

	if (node)
	{

		if (node->IsKindOfClass(RUNTIME_CLASS(NDMapLayer)))
		{
			//��baserole����ת����Ļ����
			NDMapLayer *layer = (NDMapLayer*) node;
			CCPoint screen = layer->GetScreenCenter();
			CCSize winSize = CCDirector::sharedDirector()->getWinSizeInPixels();
			m_kScreenPosition = ccpSub(GetPosition(),
					ccpSub(screen,
							CCPointMake(winSize.width / 2,
									winSize.height / 2)));
		}
		else
		{
			m_kScreenPosition = GetPosition();
		}

		sizemap = node->GetContentSize();
	}
	else
	{
		return true;
	}

	m_pkSubNode->SetContentSize(sizemap);

	HandleShadow(sizemap);

	DrawRingImage(bDraw);

	if (m_pkEffectFlagAniGroup)
	{
		if (!m_pkEffectFlagAniGroup->GetParent())
		{
			m_pkSubNode->AddChild(m_pkEffectFlagAniGroup);
		}
	}

	drawEffects(bDraw);

	return true;
}

void NDBaseRole::OnDrawEnd(bool bDraw)
{

}

CCPoint NDBaseRole::GetScreenPoint()
{
	return m_kScreenPosition;
}

void NDBaseRole::SetAction(bool bMove)
{
	if (bMove)
	{
		if (AssuredRidePet())
		{
			setMoveActionWithRidePet();
		}
		else
		{	// ������ͨ�ƶ�
			AnimationListObj.moveAction(TYPE_MANUALROLE, this, m_bFaceRight);
		}
	}
	else
	{
		if (AssuredRidePet())
		{
			setStandActionWithRidePet();
		}
		else
		{
			AnimationListObj.standAction(TYPE_MANUALROLE, this, m_bFaceRight);
		}
	}
}

bool NDBaseRole::AssuredRidePet()
{
	return m_pkRidePet != NULL && m_bIsRide;
}

void NDBaseRole::setMoveActionWithRidePet()
{
	if (!m_pkRidePet)
	{
		return;
	}

	AnimationListObj.moveAction(TYPE_RIDEPET, m_pkRidePet, m_bFaceRight);// ����ƶ�
	if (FACE_LEFT == m_bFaceRight)
	{
		SetCurrentAnimation(m_nPetRunAction, false);
	}
	else if (FACE_RIGHT == m_bFaceRight)
	{
		SetCurrentAnimation(m_nPetRunAction, true);
	}
}

void NDBaseRole::setStandActionWithRidePet()
{
	if (!m_pkRidePet)
	{
		return;
	}

	AnimationListObj.standAction(TYPE_RIDEPET, m_pkRidePet, m_bFaceRight);
	if (FACE_LEFT == m_bFaceRight)
	{
		SetCurrentAnimation(m_nPetStandAction, false);
	}
	else if (FACE_RIGHT == m_bFaceRight)
	{
		SetCurrentAnimation(m_nPetStandAction, true);
	}
}

///////////////////////////////////////////////////////////
void NDBaseRole::InitRoleLookFace(int lookface)
{
}

void NDBaseRole::InitNonRoleData( const std::string& name, int lookface, int lev )
{
	m_strName = name;
	m_nLevel = lev;

	int model_id = lookface % 1000;
	if (0 == model_id)
		model_id = lookface / 1000000;

	char sprFilePath[256];
	_snprintf(sprFilePath, 256, "%smodel_%d.spr",
			NDPath::GetAnimationPath().c_str(), model_id);
	Initialization(sprFilePath);

	m_bFaceRight = m_nDirect == 2;
	SetCurrentAnimation(MANUELROLE_STAND, m_bFaceRight);
}

void NDBaseRole::SetEquipment(int equipmentId, int quality)
{
}

void NDBaseRole::SetRidePet(int lookface, int stand_action, int run_action,
		int acc)
{
	SAFE_DELETE_NODE (m_pkRidePet);
	m_nPetStandAction = stand_action;
	m_nPetRunAction = run_action;
	m_nPetLookface = lookface;
	m_nAccLevel = acc;
	if (lookface != 0)
	{
		m_pkRidePet = new NDRidePet;
		m_pkRidePet->Initialization(lookface);

		m_pkRidePet->SetPositionEx(this->GetPosition());
		m_bIsRide = true;
		setStandActionWithRidePet();
	}
	else
	{
		m_bIsRide = false;
	}
}

/*
 * �����ιֵ�lookface�н��������õ��������ؼף�ͷ����lookface
 */
int NDBaseRole::getEquipmentLookFace(int lookface, int type)
{
	int nID = 0;
	switch (type)
	{
	case 0:
	{	//����
		int nIndex = lookface / 100000 % 100;
		if (nIndex == 99)
		{
			return 0;
		}
		if (nIndex < 10)
		{
			nID = 1000 + nIndex;
		}
		else if (nIndex < 20)
		{
			nID = 1600 + nIndex - 10;
		}
		else if (nIndex < 30)
		{
			nID = 2800 + nIndex - 20;
		}
		else if (nIndex < 40)
		{
			nID = 1200 + nIndex - 30;
		}
		else if (nIndex < 50)
		{
			nID = 1800 + nIndex - 40;
		}
		else if (nIndex < 60)
		{
			nID = 2200 + nIndex - 50;
		}
		else if (nIndex < 70)
		{
			nID = 2400 + nIndex - 60;
		}
		else if (nIndex < 80)
		{
			nID = 5000 + nIndex - 70;
		}
		else if (nIndex < 90)
		{
			nID = 2600 + nIndex - 80;
		}
		break;
	}
	case 1:
	{ //ͷ��
		int index = lookface / 1000 % 100;
		if (index == 99)
		{
			return 0;
		}
		if (index < 5)
		{
			nID = 10600 + index;
		}
		else if (index < 21)
		{
			nID = 10650 + index - 5;
		}
		else if (index < 26)
		{
			nID = 10700 + index - 21;
		}
		else if (index < 28)
		{
			nID = 10750 + index - 26;
		}
		else
		{
			nID = 10800 + index - 28;
		};
		break;
	}
	case 2:
	{ //�ؼ�
		int index = lookface / 10 % 100;

		if (index == 99)
		{
			return 0;
		}
		if (index < 50)
		{
			if (index < 5)
			{
				nID = 11200 + index;
			}
			else if (index < 9)
			{
				nID = 11250 + index - 5;
			}
			else if (index < 14)
			{
				nID = 11300 + index - 9;
			}
			else
			{
				nID = 11400 + index - 14;
			}
		}
		else
		{ //����
			nID = (index - 50) * 8 + 30000;
		}
		break;
	}
	}
	return nID;
}

void NDBaseRole::defaultDeal()
{
	if (m_nExpresstion == -1)
	{
		if (m_nSex % 2 == SpriteSexMale)
		{
			m_nExpresstion = 10400;
		}
		else
		{
			m_nExpresstion = 10401;
		}

		SetExpressionImageWithEquipmentId (m_nExpresstion);
	}
}

void NDBaseRole::SetHairImageWithEquipmentId(int equipmentId)
{
	if (equipmentId >= 10000 && equipmentId < 10400)
	{
		char hairImageName[256] = {0};
		char hairImageNameTmp1[128] = {0};
		char hairImageNameTmp2[128] = {0};
		_snprintf(hairImageNameTmp1, 128, "%s%d",
				NDPath::GetImagePath().c_str(), equipmentId);
		if (SpriteSexMale == m_nSex % 2)
		{
			_snprintf(hairImageNameTmp2, 128, "%s_1", hairImageNameTmp1);
		}
		else
		{
			_snprintf(hairImageNameTmp2, 128, "%s_2", hairImageNameTmp1);
		}
		_snprintf(hairImageName, 256, "%s.png", hairImageNameTmp2);
		SetHairImage(hairImageName, m_nHairColor);
	}
}

void NDBaseRole::SetFaceImageWithEquipmentId(int equipmentId)
{
	char faceImageName[256];
	_snprintf(faceImageName, 256, "%sskin.png", NDPath::GetImagePath().c_str());
	SetFaceImage(faceImageName);
}

void NDBaseRole::SetExpressionImageWithEquipmentId(int equipmentId)
{
	if (equipmentId >= 10400 && equipmentId < 10600)
	{
		char expressionImageName[256];
		_snprintf(expressionImageName, 256, "%s%d.png",
				NDPath::GetImagePath().c_str(), equipmentId);
		SetExpressionImage(expressionImageName);
	}
}

void NDBaseRole::SetCapImageWithEquipmentId(int equipmentId)
{
	if (equipmentId >= 10600 && equipmentId < 11200)
	{
// 		NSString* capImageName = [NSString stringWithUTF8String:NDPath::GetImagePath().c_str()];
// 		capImageName = [NSString stringWithFormat:@"%@%d.png", capImageName, equipmentId];	
// 		SetCapImage([capImageName UTF8String]);

		char capImageName[256] = {0};
		_snprintf(capImageName, 256, "%s%d.png", NDPath::GetImagePath().c_str(),
				equipmentId);
		SetCapImage(capImageName);
	}
}

void NDBaseRole::SetArmorImageWithEquipmentId(int equipmentId)
{
	if (equipmentId >= 11200 && equipmentId < 11800)
	{
		char armorImageName[256] = {0};
		_snprintf(armorImageName, 256, "%s%d.png",
				NDPath::GetImagePath().c_str(), equipmentId);
		SetArmorImage(armorImageName);
	}
}

void NDBaseRole::SetCloakImageWithEquipmentId(int equipmentId)
{
	if (equipmentId >= 30000 && equipmentId < 40000)
	{
		char cloakName[256] =
		{ 0 };
		_snprintf(cloakName, 256, "%s%d.png", NDPath::GetImagePath().c_str(),
				equipmentId + 7);
		SetCloakImage(cloakName);

		char leftShoulderName[256] =
		{ 0 };
		_snprintf(leftShoulderName, 256, "%s%d.png",
				NDPath::GetImagePath().c_str(), equipmentId + 1);
		SetLeftShoulderImage(leftShoulderName);

		char rightShoulderName[256] =
		{ 0 };
		_snprintf(rightShoulderName, 256, "%s%d.png",
				NDPath::GetImagePath().c_str(), equipmentId + 2);
		SetRightShoulderImage(rightShoulderName);

		char skirtStandName[256] =
		{ 0 };
		_snprintf(skirtStandName, 256, "%s%d.png",
				NDPath::GetImagePath().c_str(), equipmentId + 3);
		SetSkirtStandImage(skirtStandName);

		char skirtWalkName[256] =
		{ 0 };
		_snprintf(skirtWalkName, 256, "%s%d.png",
				NDPath::GetImagePath().c_str(), equipmentId + 4);
		SetSkirtWalkImage(skirtWalkName);

		char skirtSitName[256] =
		{ 0 };
		_snprintf(skirtSitName, 256, "%s%d.png", NDPath::GetImagePath().c_str(),
				equipmentId + 5);
		SetSkirtSitImage(skirtSitName);

		char skirtLiftLegName[256] =
		{ 0 };
		_snprintf(skirtLiftLegName, 256, "%s%d.png",
				NDPath::GetImagePath().c_str(), equipmentId + 6);
		SetSkirtLiftLegImage(skirtLiftLegName);
	}
}

void NDBaseRole::DrawHead(const CCPoint& pos)
{
	m_pkAniGroup->setRuningSprite(this);
	m_pkAniGroup->drawHeadAt(pos);
}

void NDBaseRole::SetWeaponType(int weaponType)
{
	m_nMasterWeaponType = weaponType;
}

int NDBaseRole::GetWeaponType()
{
	return m_nMasterWeaponType;
}

void NDBaseRole::SetSecWeaponType(int secWeaponType)
{
	m_nSecondWeaponType = secWeaponType;
}

int NDBaseRole::GetSecWeaponType()
{
	return m_nSecondWeaponType;
}

void NDBaseRole::SetWeaponQuality(int quality)
{
	m_nMasterWeaponQuality = quality;
}

int NDBaseRole::GetWeaponQuality()
{
	return m_nMasterWeaponQuality;
}

void NDBaseRole::SetSecWeaponQuality(int quality)
{
	m_nSecondWeaponQuality = quality;
}

int NDBaseRole::GetSecWeaponQuality()
{
	return m_nSecondWeaponQuality;
}

void NDBaseRole::SetCapQuality(int quality)
{
	m_nCapQuality = quality;
}

int NDBaseRole::GetCapQuality()
{
	return m_nCapQuality;
}

void NDBaseRole::SetArmorQuality(int quality)
{
	m_nArmorQuality = quality;
}

int NDBaseRole::GetArmorQuality()
{
	return m_nArmorQuality;
}

void NDBaseRole::SetCloakQuality(int quality)
{
	m_nCloakQuality = quality;
}

int NDBaseRole::GetCloakQuality()
{
	return m_nCloakQuality;
}

void NDBaseRole::SetHair(int style, int color)
{

	switch (style)
	{
	case 1:
	case 0:
		m_nHair = 10000;
		break;
	case 2:
		m_nHair = 10001;
		break;
	case 3:
		m_nHair = 10002;
		break;
	}
	m_nHairColor = color;

	char hairImageName[256] = {0};
	if (SpriteSexMale == m_nSex % 2)
	{
		_snprintf(hairImageName, 256, "%s%d_1.png",
				NDPath::GetImagePath().c_str(), m_nHair);
	}
	else
	{
		_snprintf(hairImageName, 256, "%s%d_2.png",
				NDPath::GetImagePath().c_str(), m_nHair);
	}
	SetHairImage(hairImageName, m_nHairColor);
}

void NDBaseRole::SetMaxLife(int nMaxLife)
{
	m_nMaxLife = nMaxLife;

	if (m_nLife > nMaxLife)
	{
		m_nLife = nMaxLife;
	}
}

void NDBaseRole::SetMaxMana(int nMaxMana)
{
	m_nMaxMana = nMaxMana;
	if (m_nMaxMana > nMaxMana)
	{
		m_nMaxMana = nMaxMana;
	}
}

void NDBaseRole::SetCamp(CAMP_TYPE btCamp)
{
	m_eCamp = btCamp;
	if (btCamp == CAMP_TYPE_NONE)
	{
		m_strRank.clear();
	}
}

void NDBaseRole::SetPositionEx(CCPoint newPosition)
{
	NDSprite::SetPosition(newPosition);
}

NDRidePet* NDBaseRole::GetRidePet()
{
	if (m_pkRidePet == NULL)
	{
		m_pkRidePet = new NDRidePet;
	}
	return m_pkRidePet;
}

void NDBaseRole::unpackEquip(int iEquipPos)
{
	if (iEquipPos < Item::eEP_Begin || iEquipPos >= Item::eEP_End)
	{
		return;
	}

	switch (iEquipPos)
	{
	case Item::eEP_MainArmor:
		SetWeaponType (WEAPON_NONE);
		SetRightHandWeaponImage("");
		SetDoubleHandWeaponImage("");
		SetDoubleHandWandImage("");
		SetDoubleHandBowImage("");
		SetDoubleHandSpearImage("");
		SetWeaponQuality(0);
		break;
	case Item::eEP_FuArmor:
		SetSecWeaponType(WEAPON_NONE);
		SetLeftHandWeaponImage("");
		SetShieldImage("");
		SetSecWeaponQuality(0);
		break;
	case Item::eEP_Head:
		SetCapImage("");
		SetCapQuality(0);
		break;
	case Item::eEP_Armor:
		SetArmorImage("");
		SetArmorQuality(0);
		break;
	case Item::eEP_YaoDai:
		SetCloakImage("");
		SetLeftShoulderImage("");
		SetRightShoulderImage("");
		SetSkirtStandImage("");
		SetSkirtWalkImage("");
		SetSkirtSitImage("");
		SetSkirtLiftLegImage("");
		SetCloakQuality(0);
		m_nCloak = -1;
		break;
// 	case Item::eEP_Ride:
// 		SAFE_DELETE_NODE (m_pkRidePet);
// 		break;
	default:
		break;
	}
}

void NDBaseRole::unpakcAllEquip()
{
	for (int i = Item::eEP_Begin; i < Item::eEP_End; i++)
	{
		unpackEquip(i);
	}
}

void NDBaseRole::addTalkMsg( const std::string& msg, int timeForTalkMsg )
{
	NDScene *pkScene = NDDirector::DefaultDirector()->GetRunningScene();
	if (!pkScene || !pkScene->IsKindOfClass(RUNTIME_CLASS(GameScene)))
	{
		return;
	}

	//if (!m_pkTalkBox && m_pkSubNode) 
	//{
	//	m_pkTalkBox = new TalkBox;
	//	m_pkTalkBox->Initialization();
	//	((GameScene*)scene)->AddUIChild(m_pkTalkBox);
	//	m_pkTalkBox->SetDelegate(this);
	//	m_pkTalkBox->SetVisible(false);
	//}
	//
	//if (timeForTalkMsg == 0) m_pkTalkBox->SetFix();
	//m_pkTalkBox->addTalkMsg(msg, timeForTalkMsg);
}

void NDBaseRole::drawEffects(bool bDraw)
{
	if (m_pkEffectRidePetAniGroup != NULL
			&& m_pkEffectRidePetAniGroup->GetParent())
	{
		m_pkEffectRidePetAniGroup->SetPosition(GetPosition());
		m_pkEffectRidePetAniGroup->RunAnimation(bDraw);
	}
}

void NDBaseRole::updateRidePetEffect()
{
	if (AssuredRidePet() && m_pkRidePet->quality > 8)
	{
		SafeAddEffect(m_pkEffectRidePetAniGroup, "effect_3001.spr");
	}
	else
	{
		SafeClearEffect (m_pkEffectRidePetAniGroup);
	}
}

void NDBaseRole::SafeClearEffect(NDSprite*& sprite)
{
	if (sprite != NULL)
	{
		if (sprite->GetParent())
		{
			sprite->RemoveFromParent(true);
		}
		else
		{
			delete sprite;
		}

		sprite = NULL;
	}
}

void NDBaseRole::SafeAddEffect(NDSprite*& sprite, const std::string& file)
{
	if (sprite == NULL && !file.empty())
	{
		sprite = new NDSprite;
		sprite->Initialization(NDPath::GetAniPath(file.c_str()).c_str());
		sprite->SetCurrentAnimation(0, false);
	}
}

void NDBaseRole::ShowShadow(bool bShow, bool bBig /*= false*/)
{
	m_bShowShadow = bShow;
	m_bBigShadow = bBig;
}

void NDBaseRole::SetShadowOffset(int iX, int iY)
{
	m_iShadowOffsetX = iX;
	m_iShadowOffsetY = iY;
}

void NDBaseRole::HandleShadow(CCSize parentsize)
{
	//change by tangziqin ������shadow.png��ͼƬ
	//return;

#if 0
	if (!m_bShowShadow)
	{
		return;
	}

	NDPicture *pic = NULL;
	if (!m_bBigShadow)
	{
		if (m_pkPicShadow == NULL)
		{
			m_pkPicShadow = NDPicturePool::DefaultPool()->AddPicture(
					SHADOW_IMAGE);
		}
		pic = m_pkPicShadow;
	}
	else
	{
		if (m_pkPicShadowBig == NULL)
		{
			m_pkPicShadowBig = NDPicturePool::DefaultPool()->AddPicture(
					BIG_SHADOW_IMAGE);
		}
		pic = m_pkPicShadowBig;
	}

	CCSize sizeShadow = pic->GetSize();

	int x = m_kPosition.x - DISPLAY_POS_X_OFFSET;
	int y = m_kPosition.y - DISPLAY_POS_Y_OFFSET;

//@check
// 	pic->DrawInRect(
// 			CCRectMake(x + m_iShadowOffsetX,
// 					y + m_iShadowOffsetY
// 							+ CCDirector::sharedDirector()->getWinSizeInPixels().height
// 							- parentsize.height, sizeShadow.width,
// 					sizeShadow.height));

	pic->DrawInRect(
			CCRectMake(x + m_iShadowOffsetX, y + m_iShadowOffsetY,
					sizeShadow.width, sizeShadow.height));
#endif
}

// void NDBaseRole::SetNormalAniGroup(int nLookface)
// {
// 	if (nLookface <= 0)
// 	{
// 		return;
// 	}
// 
// 	Initialization(
// 			tq::CString("%smodel_%d%s",
// 					NDEngine::NDPath::GetAnimationPath().c_str(),
// 					nLookface / 100, ".spr"));
// 
// 	m_bFaceRight = true;
// 	SetCurrentAnimation(MANUELROLE_STAND, m_bFaceRight);
// }
// 
// void NDEngine::NDBaseRole::RunBattleSubAnimation(Fighter* pkFighter)
// {
// 	BattleUILayer* pkBattle = 0;
// 
// 	if (!pkBattle)
// 	{
// 		return;
// 	}
// 
// 	// 1.��ȡ��ǰ֡
// 	NDFrame* curFrame = m_pkCurrentAnimation->getFrames()->getObjectAtIndex(
// 			m_pkFrameRunRecord->getCurrentFrameIndex());
// 
// 	// 2.ȡ��ǰ֡���Ӷ������鲢����ս��������Ӷ�������
// 	if (curFrame && curFrame->getSubAnimationGroups())
// 	{
// 		for (NSUInteger i = 0; i < curFrame->getSubAnimationGroups()->count();
// 				i++)
// 		{
// 			NDAnimationGroup *pkAnimationGroup =
// 					curFrame->getSubAnimationGroups()->getObjectAtIndex(i);
// 			pkAnimationGroup->setReverse(
// 					pkFighter->getFighterInfo().group == BATTLE_GROUP_DEFENCE ?
// 							false : true);
// 
// 			if (pkAnimationGroup->getIdentifer() == 0)
// 			{ // ��ħ����Ч
// 				if (pkAnimationGroup->getType() == SUB_ANI_TYPE_SELF
// 						|| pkAnimationGroup->getType() == SUB_ANI_TYPE_NONE)
// 				{
// 					pkBattle->addSubAniGroup(this, pkAnimationGroup, pkFighter);
// 				}
// 			}
// 			else
// 			{ // ħ����Ч
// 				if (pkAnimationGroup->getIdentifer()
// 						== pkFighter->getUseSkill()->getSubAniID())
// 				{
// 					if (pkAnimationGroup->getType() == SUB_ANI_TYPE_SELF)
// 					{
// 						pkBattle->addSubAniGroup(this, pkAnimationGroup,
// 								pkFighter);
// 					}
// 					else if (pkAnimationGroup->getType() == SUB_ANI_TYPE_TARGET)
// 					{
// 
// 						VEC_FIGHTER& array = pkFighter->getArrayTarget();
// 						if (array.size() == 0)
// 						{ // ���û��Ŀ�����飬���ƶ�Ŀ��ΪmainTarget
// 							pkBattle->addSubAniGroup(this, pkAnimationGroup,
// 									pkFighter->m_pkMainTarget);
// 						}
// 						else
// 						{
// 							for (size_t j = 0; j < array.size(); j++)
// 							{
// 								pkBattle->addSubAniGroup(this, pkAnimationGroup,
// 										array.at(j));
// 							}
// 						}
// 					}
// 					else if (pkAnimationGroup->getType() == SUB_ANI_TYPE_NONE)
// 					{
// 						pkBattle->addSubAniGroup(this, pkAnimationGroup,
// 								pkFighter);
// 					}
// 				}
// 			}
// 		}
// 	}
// }
// 
// bool NDEngine::NDBaseRole::DrawSubAnimation(NDSubAniGroup& kSag)
// {
// 	NDNode* pkLayer = GetParent();
// 
// 	if (!pkLayer)
// 	{
// 		return true;
// 	}
// 
// 	NDFrameRunRecord* pkRecord = kSag.frameRec;
// 
// 	if (!pkRecord)
// 	{
// 		return true;
// 	}
// 
// 	NDAnimationGroup* pkAnimationGroup = kSag.aniGroup;
// 
// 	if (!pkAnimationGroup)
// 	{
// 		return true;
// 	}
// 
// 	CCPoint kPosition = pkAnimationGroup->getPosition();
// 	pkAnimationGroup->setRunningMapSize(pkLayer->GetContentSize());
// 
// 	NDAnimation* pkAnimation = nil;
// 	if (pkAnimationGroup->getAnimations()->count() > 0)
// 	{
// 		pkAnimation =
// 				(NDAnimation*) pkAnimationGroup->getAnimations()->objectAtIndex(
// 						0);
// 	}
// 
// 	if (!pkAnimation)
// 	{
// 		return true;
// 	}
// 
// 	CCPoint kTargetPos = ccp(0, 0);
// 	if (pkAnimationGroup->getType() == SUB_ANI_TYPE_NONE)
// 	{
// 		if (kSag.reverse)
// 		{
// 			//����ת++Guosen 2012.6.28
// 			pkAnimationGroup->setReverse(
// 					kSag.fighter->m_kInfo.group == BATTLE_GROUP_DEFENCE ?
// 							false : true);
// 		}
// 		else
// 		{
// 			pkAnimationGroup->setReverse(false);
// 		}
// 
// 		int nCoordX = 0;
// 
// 		if (pkAnimationGroup->getReverse())
// 		{
// 			// �����ͷż���
// 			nCoordX += (240
// 					- (pkAnimationGroup->getPosition().x + pkAnimation->getX()))
// 					* 2;
// 		}
// 
// 		kTargetPos.x = kPosition.x + pkAnimation->getW() / 2 + nCoordX + 20;
// 		kTargetPos.y = kPosition.y + pkAnimation->getH() / 2 + 45;
// 	}
// 	else if (pkAnimationGroup->getType() == SUB_ANI_TYPE_TARGET
// 			|| pkAnimationGroup->getType() == SUB_ANI_TYPE_SELF)
// 	{
// 		kTargetPos.x = kSag.fighter->getX();
// 		int nPosY = kSag.fighter->getY();
// 		if (kSag.pos == 0)
// 		{
// 			nPosY -= FIGHTER_HEIGHT;
// 		}
// 		else if (kSag.pos == 2)
// 		{
// 			nPosY -= FIGHTER_HEIGHT / 2;
// 		}
// 		kTargetPos.y = nPosY;
// 		if (kSag.reverse)
// 		{
// 			//����ת++Guosen 2012.6.28
// 			pkAnimationGroup->setReverse(
// 					kSag.fighter->getFighterInfo().group == BATTLE_GROUP_DEFENCE ?
// 							true : false);
// 		}
// 		else
// 		{
// 			pkAnimationGroup->setReverse(false);
// 		}
// 	}
// 
// 	// �Ӷ�������λ������
// 	pkAnimationGroup->setPosition(kTargetPos);
// 	pkAnimation->runWithRunFrameRecord(pkRecord, true, m_fScale);
// 	pkAnimationGroup->setPosition(kPosition);
// 
// 	return pkRecord->getCurrentFrameIndex() != 0
// 			&& pkRecord->getNextFrameIndex() == 0;
// }

//����+��Ⱦ���������������ط������
void NDBaseRole::RunAnimation(bool bDraw)
{
	if (!NDDebugOpt::getRunAnimRoleEnabled()) return;

	NDSprite::RunAnimation(bDraw);

	this->debugDraw();
}

CCPoint NDBaseRole::getFootPos()
{
	return GetPosition();
}

CCPoint NDBaseRole::getHeadPos()
{
	CCPoint posFoot = GetPosition();

	NDAnimation* anim =
			(NDAnimation*) m_pkAniGroup->getAnimations()->objectAtIndex(0);
	if (anim)
	{
		CCRect animRect = anim->getRect();
		CCPoint posHead = ccp(GetPosition().x,
				GetPosition().y - RESOURCE_SCALE_960*animRect.size.height);
		return posHead;
	}

	return posFoot;
}

void NDBaseRole::debugDraw()
{
	if (!NDDebugOpt::getDrawDebugEnabled())
		return;

	drawCoord( getFootPos() );
	drawCoord( getHeadPos() );
	drawCoord( GetPosition(), true, false );
}

// for debug purpose.
// if bRightUp is false, the coord goes right-down.
void NDBaseRole::drawCoord(const CCPoint& posScreen, bool bRightUp /*= true*/,
		bool blue /*= true*/, const float ofs /*= 10.f*/)
{
	CCPoint pos = posScreen;
	ConvertUtil::convertToPointCoord(pos);
	pos = SCREEN2GL(pos);

	if (blue)
		ccDrawColor4F(0, 0, 1, 1);	//blue
	else
		ccDrawColor4F(1, 0, 0, 1);	//red

	ccDrawLine(pos, ccpAdd(pos, ccp(ofs, 0)));
	ccDrawLine(pos, ccpAdd(pos, ccp(0, ofs * (bRightUp ? 1 : -1))));
}

void NDBaseRole::SetName( const std::string& strName )
{
	m_strName = strName;
}

void NDBaseRole::SetRank( const std::string& strRank )
{
	m_strRank = strRank;
}