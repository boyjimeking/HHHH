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
//#include "NDRidePet.h"
#include "CCPointExtension.h"
#include "AnimationList.h"
#include "NDNode.h"
#include "NDDirector.h"
#include "NDMapLayer.h"
#include "Item.h"
#include "GameScene.h"
#include "NDUtility.h"
#include "define.h"
#include "NDRidePet.h"

using namespace NDEngine;

IMPLEMENT_CLASS(NDBaseRole, NDSprite)

bool NDBaseRole::ms_bGameSceneRelease = false;

NDBaseRole::NDBaseRole()
{
	m_weaponType = WEAPON_NONE;
	m_secWeaponType = WEAPON_NONE;

	sex = -1;
	skinColor = -1;
	hairColor = -1;
	hair = -1;

	direct = -1;
	expresstion = -1;
	model = -1;
	weapon = -1;
	cap = -1;
	armor = -1;

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
	m_pkSubNode->SetContentSize(NDDirector::DefaultDirector()->GetWinSize());

	m_posScreen = CGPointZero;
	m_picRing = NULL;
	m_pkPicShadow = NULL;
	m_pkPicShadowBig = NULL;
	m_iShadowOffsetX = 0;
	m_iShadowOffsetY = 10;
	m_bBigShadow = false;
	m_bShowShadow = true;

	m_nID = 0;

	m_pkEffectFlagAniGroup = NULL;

	//m_talkBox = NULL;

	this->SetDelegate(this);

	m_pkEffectRidePetAniGroup = NULL;
}

NDBaseRole::~NDBaseRole()
{
	SAFE_DELETE (m_pkSubNode);
	SAFE_DELETE (m_picRing);
	SAFE_DELETE (m_pkPicShadow);
	SAFE_DELETE (m_pkPicShadowBig);
	//if (ridepet)
//	{
//		delete ridepet;
//		ridepet = NULL;
//	}

//if (m_talkBox)
//	{
//		if (m_talkBox->GetParent()) 
//		{
//			m_talkBox->RemoveFromParent(true);
//			m_talkBox = NULL;
//		}
//		else 
//		{
//			delete m_talkBox;
//			m_talkBox = NULL;
//		}
//	}
	//if (!s_bGameSceneRelease && m_talkBox) 
	//{
	//	SAFE_DELETE_NODE(m_talkBox);
	//}
}

CGRect NDBaseRole::GetFocusRect()
{
	/*
	 if (!IsFocus())
	 {
	 return CGRectZero;
	 }
	 */

	CGPoint point;

	if (m_pkRidePet)
	{
		point = m_pkRidePet->GetPosition();
	}
	else
	{
		point = GetPosition();
	}

	if (m_picRing == NULL)
	{
		m_picRing = NDPicturePool::DefaultPool()->AddPicture(RING_IMAGE);
	}

	CGSize sizeRing = m_picRing->GetSize();

	return CGRectMake(point.x - 8 - 13, point.y - 16 - 5, sizeRing.width,
			sizeRing.height);
}

void NDBaseRole::DirectRight(bool bRight)
{
	if (bRight)
	{
		this->SetSpriteDir(1);
		if (m_pkRidePet)
		{
			m_pkRidePet->SetSpriteDir(1);
		}
	}
	else
	{
		this->SetSpriteDir(2);
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
	if (this->IsKindOfClass(RUNTIME_CLASS(NDNpc)))
	{
		NDNpc* npc = (NDNpc*) this;
		if (!npc->IsActionOnRing())
		{
			return;
		}
	}

	if (m_bFocus && bDraw)
	{
		if (m_picRing == NULL)
		{
			m_picRing = NDPicturePool::DefaultPool()->AddPicture(RING_IMAGE);
		}
		CGSize sizeRing = m_picRing->GetSize();

		if (this->GetParent())
		{
			NDLayer *layer = (NDLayer*) this->GetParent();
			CGSize sizemap = layer->GetContentSize();
			if (!m_pkRidePet)
			{
				//m_picRing->DrawInRect(CGRectMake(GetPosition().x-13-8, GetPosition().y-5-16+320-sizemap.height, sizeRing.width, sizeRing.height));
			}
			else
			{
				//m_picRing->DrawInRect(CGRectMake(ridepet->GetPosition().x-13-8, ridepet->GetPosition().y-5-16+320-sizemap.height, sizeRing.width, sizeRing.height));
			}
		}
	}
}

bool NDBaseRole::OnDrawBegin(bool bDraw)
{
	NDNode *node = this->GetParent();
	CGSize sizemap;

	if (node)
	{

		if (node->IsKindOfClass(RUNTIME_CLASS(NDMapLayer)))
		{
			//��baserole����ת����Ļ����
			NDMapLayer *layer = (NDMapLayer*) node;
			CGPoint screen = layer->GetScreenCenter();
			CGSize winSize = NDDirector::DefaultDirector()->GetWinSize();
			m_posScreen = ccpSub(this->GetPosition(),
					ccpSub(screen,
							CGPointMake(winSize.width / 2,
									winSize.height / 2)));
		}
		else
		{
			m_posScreen = GetPosition();
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

//	updateRidePetEffect(); ///<��ʱ��ע�� ����

	if (m_pkEffectRidePetAniGroup)
	{
		if (!m_pkEffectRidePetAniGroup->GetParent())
		{
			m_pkSubNode->AddChild(m_pkEffectRidePetAniGroup);
		}
	}

	drawEffects(bDraw);

	//if (m_talkBox) 
	//{
	//	m_talkBox->draw();
	//}

	return true;
}

///< ��ʱ��ע�� --����
// void NDBaseRole::OnDrawEnd(bool bDraw)
// {
// }
// 
// void NDBaseRole::OnBeforeNodeRemoveFromParent(NDNode* node, bool bCleanUp)
// {
// 	if (node == m_talkBox) 
// 	{
// 		m_talkBox = NULL;
// 	}
// 	// ��������.....
// }

CGPoint NDBaseRole::GetScreenPoint()
{
	return m_posScreen;
}

void NDBaseRole::SetAction(bool bMove)
{
	//if (GameScreen.getInstance().getBattle() != null) {
//		return;
//	}
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
		{	// ���վ��
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
//	return ridepet != NULL && ridepet->canRide(); ///< ��ʱ��ע�� ����
	return true;
}

void NDBaseRole::setMoveActionWithRidePet()
{
	if (!m_pkRidePet)
	{
		return;
	}

	AnimationListObj.moveAction(TYPE_RIDEPET, m_pkRidePet, m_bFaceRight);// ����ƶ�
	switch (m_pkRidePet->iType)
	{
	case TYPE_RIDE:	// ��������������ƶ�
		AnimationListObj.ridePetMoveAction(TYPE_MANUALROLE, this, m_bFaceRight);
		break;
	case TYPE_STAND:	// ����վ��������ƶ�
		AnimationListObj.standPetMoveAction(TYPE_MANUALROLE, this,
				m_bFaceRight);
		break;
	case TYPE_RIDE_BIRD:
		AnimationListObj.moveAction(TYPE_RIDEPET, m_pkRidePet,
				1 - m_bFaceRight);
		AnimationListObj.setAction(TYPE_MANUALROLE, this, m_bFaceRight,
				MANUELROLE_RIDE_BIRD_WALK);
		break;
	case TYPE_RIDE_FLY:
		AnimationListObj.setAction(TYPE_MANUALROLE, this, m_bFaceRight,
				MANUELROLE_FLY_PET_WALK);
		break;
	case TYPE_RIDE_YFSH:
		AnimationListObj.setAction(TYPE_MANUALROLE, this, m_bFaceRight,
				MANUELROLE_FLY_PET_WALK);
		break;
	case TYPE_RIDE_QL:
		AnimationListObj.setAction(TYPE_MANUALROLE, this, m_bFaceRight,
				MANUELROLE_RIDE_QL);
		break;
	}
}

void NDBaseRole::setStandActionWithRidePet()
{
	if (!m_pkRidePet)
	{
		return;
	}

	AnimationListObj.standAction(TYPE_RIDEPET, m_pkRidePet, m_bFaceRight);

	// װ�����桢���Խ��桢ս���У���Ҫվ��״̬
	//if (EquipUIScreen.instance == null
//	&& Attribute.instance == null
//	&& GameScreen.getInstance().getBattle() == null
//	&& NpcStore.instance == null/*
//	* && VipStore . instance ==
//	* null
//	*/) 
//	{
	switch (m_pkRidePet->iType)
	{
	case TYPE_RIDE:
		AnimationListObj.ridePetStandAction(TYPE_MANUALROLE, this,
				m_bFaceRight);
		break;
	case TYPE_STAND:
		AnimationListObj.standPetStandAction(TYPE_MANUALROLE, this,
				m_bFaceRight);
		break;
	case TYPE_RIDE_BIRD:
		AnimationListObj.standAction(TYPE_RIDEPET, m_pkRidePet,
				1 - m_bFaceRight);
		AnimationListObj.setAction(TYPE_MANUALROLE, this, m_bFaceRight,
				MANUELROLE_RIDE_BIRD_STAND);
		break;
	case TYPE_RIDE_FLY:
		AnimationListObj.setAction(TYPE_MANUALROLE, this, m_bFaceRight,
				MANUELROLE_FLY_PET_STAND);
		break;
	case TYPE_RIDE_YFSH:
		AnimationListObj.setAction(TYPE_MANUALROLE, this, m_bFaceRight,
				MANUELROLE_FLY_PET_WALK);
		break;
	case TYPE_RIDE_QL:
		AnimationListObj.setAction(TYPE_MANUALROLE, this, m_bFaceRight,
				MANUELROLE_RIDE_QL);
		break;
	}
//	} 
//	else 
//	{
//	 AnimationListObj.standAction(TYPE_MANUALROLE, this, m_faceRight);
//	}
}

///////////////////////////////////////////////////////////
void NDBaseRole::InitRoleLookFace(int lookface)
{
//	sex = lookface / 100000000 % 10;
//	
//	skinColor = lookface / 100000 % 10 - 1;				// ��ɫ
//	hairColor = lookface / 1000000 % 10-1;			// ��ɫ
//	hair = lookface / 10000000 % 10;					// ����
//	
//	SetHair(hair, hairColor);
//	SetHairImageWithEquipmentId(hair);
//	SetFaceImageWithEquipmentId(skinColor);
}

void NDBaseRole::InitNonRoleData(std::string name, int lookface, int lev)
{
	this->m_name = name;
	m_nLevel = lev;
	//m_id = 0; // �û�id
//	sex = lookface / 100000000 % 10; // �����Ա�1-���ԣ�2-Ů�ԣ�
//	direct = lookface % 10;
//	hairColor = lookface / 1000000 % 10;
//	int tmpsex = (sex - 1) / 2 - 1;
//	if (tmpsex < 0 || tmpsex > 2) {
//		tmpsex = 0;
//	}
//	this->SetHair(tmpsex+1); // ����
//	
//	SetHairImageWithEquipmentId(hair);

//	int flagOrRidePet = lookface / 10000000 % 10;
//	if (flagOrRidePet > 0) {
//		if (flagOrRidePet < 5) {
//			camp = CAMP_TYPE(flagOrRidePet);
//		}else {				
//			int id = (flagOrRidePet + 1995)*10;
//			SetEquipment(id,0);
//		}
//	}

//	weapon	= getEquipmentLookFace(lookface, 0);
//	cap			= getEquipmentLookFace(lookface, 1);
//	armor		= getEquipmentLookFace(lookface, 2);
//	SetEquipment(weapon, 0);//����
//	SetEquipment(cap, 0);//ͷ��
//	SetEquipment(armor, 0);//�ؼ�

	//Load Animation Group
	int model_id = lookface / 1000000;
//	if (sex % 2 == SpriteSexMale) 
	//NSString* aniPath = [NSString stringWithUTF8String:NDPath::GetAnimationPath().c_str()];  ///<��ʱ��ע�� ����
//	Initialization([[NSString stringWithFormat:@"%@model_%d.spr",aniPath,model_id] UTF8String] ); ///<��ʱ��ע�� ����
//	else 
//		Initialization(MANUELROLE_HUMAN_FEMALE);

	m_bFaceRight = direct == 2;
	SetFaceImageWithEquipmentId (m_bFaceRight);
	SetCurrentAnimation(MANUELROLE_STAND, m_bFaceRight);

	defaultDeal();
}

void NDBaseRole::SetEquipment(int equipmentId, int quality)
{
	/***
	 * ��ʱ��ע�� ����
	 */
//	if (equipmentId <= 0 ) 
//		return;
//	
//	NSString* imagePath = [NSString stringWithUTF8String:NDPath::GetImagePath().c_str()];
//	CCString* imagePath = new CCString("");
//	
//	if (equipmentId >= 200 && equipmentId < 10000) //����
//	{
//		if ((equipmentId >= 1000 && equipmentId < 1200) || (equipmentId >= 1600 && equipmentId < 1800) || (equipmentId >= 2800 && equipmentId < 3000)) 
//		{
//			if (this->GetRightHandWeaponImage() == NULL) 
//			{
//				this->SetWeaponType(ONE_HAND_WEAPON);
//				this->SetRightHandWeaponImage([[NSString stringWithFormat:@"%@%d.png", imagePath, equipmentId] UTF8String]);
//				this->SetWeaponQuality(quality);
//			}
//			else 
//			{				
//				this->SetSecWeaponType(ONE_HAND_WEAPON);
//				this->SetLeftHandWeaponImage([[NSString stringWithFormat:@"%@%d.png", imagePath, equipmentId] UTF8String]);
//				this->SetSecWeaponQuality(quality);
//			}			
//		}
//		else if ((equipmentId >= 1200 && equipmentId < 1400) || (equipmentId >= 1800 && equipmentId < 2000))
//		{
//			this->SetWeaponType(TWO_HAND_WEAPON);
//			this->SetDoubleHandWeaponImage([[NSString stringWithFormat:@"%@%d.png", imagePath, equipmentId] UTF8String]);
//			this->SetWeaponQuality(quality);
//		}
//		else if (equipmentId >= 2200 && equipmentId < 2400)
//		{
//			this->SetWeaponType(TWO_HAND_WAND);
//			this->SetDoubleHandWandImage([[NSString stringWithFormat:@"%@%d.png", imagePath, equipmentId] UTF8String]);
//			this->SetWeaponQuality(quality);
//		}
//		else if (equipmentId >= 2400 && equipmentId < 2600)
//		{
//			this->SetWeaponType(TWO_HAND_BOW);
//			this->SetDoubleHandBowImage([[NSString stringWithFormat:@"%@%d.png", imagePath, equipmentId] UTF8String]);
//			this->SetWeaponQuality(quality);
//		}
//		else if (equipmentId >= 2600 && equipmentId < 2800)
//		{
//			this->SetWeaponType(TWO_HAND_SPEAR);
//			this->SetDoubleHandSpearImage([[NSString stringWithFormat:@"%@%d.png", imagePath, equipmentId] UTF8String]);
//			this->SetWeaponQuality(quality);
//		}
//		else if (equipmentId >= 5000 && equipmentId < 5200)
//		{
//			this->SetSecWeaponType(SEC_SHIELD);
//			this->SetShieldImage([[NSString stringWithFormat:@"%@%d.png", imagePath, equipmentId] UTF8String]);
//			this->SetSecWeaponQuality(quality);
//		}		
//	}
//	else if (equipmentId >= 10000 && equipmentId < 11800) //����
//	{
//		if (equipmentId > 10000 && equipmentId < 10200) 
//		{
//			hair = equipmentId;
//			this->SetHairImageWithEquipmentId(equipmentId);
//		}
//		else if (equipmentId >= 10200 && equipmentId < 10400)
//		{
//			// do nothing
//		}
//		else if (equipmentId >= 10400 && equipmentId < 10600)
//		{
//			expresstion = equipmentId;
//			this->SetExpressionImageWithEquipmentId(equipmentId);
//		}
//		else if (equipmentId >= 10600 && equipmentId < 11200)
//		{
//			cap = equipmentId;
//			this->SetCapImageWithEquipmentId(equipmentId);
//			this->SetCapQuality(quality);
//		}
//		else if (equipmentId >= 11200 && equipmentId < 11800)
//		{
//			armor = equipmentId;
//			this->SetArmorImageWithEquipmentId(equipmentId);
//			this->SetArmorQuality(quality);
//		}
//	}
//	else if (equipmentId >= 20000 && equipmentId < 30000 || equipmentId > 100000) //���
//	{
//		SAFE_DELETE_NODE(ridepet);
//		ridepet = new NDRidePet;
//		ridepet->Initialization(equipmentId);
//		ridepet->quality = quality;
//		ridepet->SetPositionEx(this->GetPosition());
//		
//		if (this->IsKindOfClass(RUNTIME_CLASS(NDManualRole))) 
//		{
////			if (this->GetParent() && this->GetParent()->IsKindOfClass(RUNTIME_CLASS(NDMapLayer)))
////			{
////				this->SetAction(false);
////			}
//			
//			ridepet->SetOwner(this);
//		}
//		
//		/*
//		if (this->IsKindOfClass(RUNTIME_CLASS(NDNpc))) 
//		{
//			NDRidePet *ridepet = ((NDNpc*)this)->GetRidePet();
//			ridepet->Initialization(equipmentId);
//			ridepet->quality = quality;
//			ridepet->SetPositionEx(this->GetPosition());
//		}
//		*/
//	}
//	else if (equipmentId >= 30000 && equipmentId < 40000) //����
//	{
//		cloak = equipmentId;
//		this->SetCloakImageWithEquipmentId(equipmentId);
//		this->SetCloakQuality(quality);
//	}	
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
		int index = lookface / 100000 % 100;
		if (index == 99)
		{
			return 0;
		}
		if (index < 10)
		{
			nID = 1000 + index;
		}
		else if (index < 20)
		{
			nID = 1600 + index - 10;
		}
		else if (index < 30)
		{
			nID = 2800 + index - 20;
		}
		else if (index < 40)
		{
			nID = 1200 + index - 30;
		}
		else if (index < 50)
		{
			nID = 1800 + index - 40;
		}
		else if (index < 60)
		{
			nID = 2200 + index - 50;
		}
		else if (index < 70)
		{
			nID = 2400 + index - 60;
		}
		else if (index < 80)
		{
			nID = 5000 + index - 70;
		}
		else if (index < 90)
		{
			nID = 2600 + index - 80;
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
	if (expresstion == -1)
	{
		if (sex % 2 == SpriteSexMale)
		{
			expresstion = 10400;
		}
		else
		{
			expresstion = 10401;
		}

		SetExpressionImageWithEquipmentId (expresstion);
	}
}

void NDBaseRole::SetHairImageWithEquipmentId(int equipmentId)
{
	if (equipmentId >= 10000 && equipmentId < 10400)
	{
		/**
		 * ��ʱ��ע�� ����
		 */
// 		NSString* hairImageName = [NSString stringWithUTF8String:NDPath::GetImagePath().c_str()];
// 		hairImageName = [NSString stringWithFormat:@"%@%d", hairImageName, equipmentId];
// 		if (sex % 2 == SpriteSexMale) 
// 		{
// 			hairImageName = [NSString stringWithFormat:@"%@_1", hairImageName];
// 		}
// 		else 
// 		{
// 			hairImageName = [NSString stringWithFormat:@"%@_2", hairImageName];
// 		}
// 		hairImageName = [NSString stringWithFormat:@"%@.png", hairImageName];
// 		this->SetHairImage([hairImageName UTF8String], this->hairColor);
	}
}

void NDBaseRole::SetFaceImageWithEquipmentId(int equipmentId)
{
	/**
	 * ��ʱ��ע�� ����
	 */

// 	NSString* faceImageName = [NSString stringWithUTF8String:NDPath::GetImagePath().c_str()];
// 	faceImageName = [NSString stringWithFormat:@"%@skin.png", faceImageName];	
// 	//faceImageName = [NSString stringWithFormat:@"%@skin@%d.png", faceImageName, skinColor];
// 	this->SetFaceImage([faceImageName UTF8String]);
}

void NDBaseRole::SetExpressionImageWithEquipmentId(int equipmentId)
{
	if (equipmentId >= 10400 && equipmentId < 10600)
	{
		/**
		 * ��ʱ��ע�� ����
		 */

// 		NSString* expressionImageName = [NSString stringWithUTF8String:NDPath::GetImagePath().c_str()];
// 		expressionImageName = [NSString stringWithFormat:@"%@%d.png", expressionImageName, equipmentId];	
// 		this->SetExpressionImage([expressionImageName UTF8String]);
	}
}

void NDBaseRole::SetCapImageWithEquipmentId(int equipmentId)
{
	if (equipmentId >= 10600 && equipmentId < 11200)
	{
		/**
		 * ��ʱ��ע�� ����
		 * begin
		 */
// 		NSString* capImageName = [NSString stringWithUTF8String:NDPath::GetImagePath().c_str()];
// 		capImageName = [NSString stringWithFormat:@"%@%d.png", capImageName, equipmentId];	
// 		this->SetCapImage([capImageName UTF8String]);
		/**
		 * ��ʱ��ע�� ����
		 * end
		 */
	}
}

void NDBaseRole::SetArmorImageWithEquipmentId(int equipmentId)
{
	if (equipmentId >= 11200 && equipmentId < 11800)
	{
		/**
		 * ��ʱ��ע�� ����
		 * begin
		 */
// 		NSString* armorImageName = [NSString stringWithUTF8String:NDPath::GetImagePath().c_str()];
// 		armorImageName = [NSString stringWithFormat:@"%@%d.png", armorImageName, equipmentId];	
// 		this->SetArmorImage([armorImageName UTF8String]);
		/**
		 * ��ʱ��ע�� ����
		 * end
		 */
	}
}

void NDBaseRole::SetCloakImageWithEquipmentId(int equipmentId)
{
	/*
	 if (equipmentId >= 11200 && equipmentId < 11800) 
	 {
	 NSString* cloakImageName = [NSString stringWithUTF8String:NDPath::GetImagePath().c_str()];
	 cloakImageName = [NSString stringWithFormat:@"%@%d.png", cloakImageName, equipmentId];	
	 this->SetCloakImage([cloakImageName UTF8String]);
	 }
	 */
	if (equipmentId >= 30000 && equipmentId < 40000)
	{
		/**
		 * ��ʱ��ע�� ����
		 * begin
		 */

// 		NSString* cloakName = [NSString stringWithUTF8String:NDPath::GetImagePath().c_str()];
// 		cloakName = [NSString stringWithFormat:@"%@%d.png", cloakName, equipmentId+7];	
// 		this->SetCloakImage([cloakName UTF8String]);
// 		
// 		
// 		NSString* leftShoulderName = [NSString stringWithUTF8String:NDPath::GetImagePath().c_str()];
// 		leftShoulderName = [NSString stringWithFormat:@"%@%d.png", leftShoulderName, equipmentId+1];	
// 		this->SetLeftShoulderImage([leftShoulderName UTF8String]);
// 		
// 		NSString* rightShoulderName = [NSString stringWithUTF8String:NDPath::GetImagePath().c_str()];
// 		rightShoulderName = [NSString stringWithFormat:@"%@%d.png", rightShoulderName, equipmentId+2];	
// 		this->SetRightShoulderImage([rightShoulderName UTF8String]);
// 		
// 		NSString* skirtStandName = [NSString stringWithUTF8String:NDPath::GetImagePath().c_str()];
// 		skirtStandName = [NSString stringWithFormat:@"%@%d.png", skirtStandName, equipmentId+3];	
// 		this->SetSkirtStandImage([skirtStandName UTF8String]);
// 		
// 		NSString* skirtWalkName = [NSString stringWithUTF8String:NDPath::GetImagePath().c_str()];
// 		skirtWalkName = [NSString stringWithFormat:@"%@%d.png", skirtWalkName, equipmentId+4];	
// 		this->SetSkirtWalkImage([skirtWalkName UTF8String]);
// 		
// 		NSString* skirtSitName = [NSString stringWithUTF8String:NDPath::GetImagePath().c_str()];
// 		skirtSitName = [NSString stringWithFormat:@"%@%d.png", skirtSitName, equipmentId+5];	
// 		this->SetSkirtSitImage([skirtSitName UTF8String]);
// 		
// 		NSString* skirtLiftLegName = [NSString stringWithUTF8String:NDPath::GetImagePath().c_str()];
// 		skirtLiftLegName = [NSString stringWithFormat:@"%@%d.png", skirtLiftLegName, equipmentId+6];	
// 		this->SetSkirtLiftLegImage([skirtLiftLegName UTF8String]);
		/**
		 * ��ʱ��ע�� ����
		 * end
		 */
	}
}

void NDBaseRole::DrawHead(const CGPoint& pos)
{
	m_pkAniGroup->setRuningSprite(this);
	m_pkAniGroup->drawHeadAt(pos);
}

void NDBaseRole::SetWeaponType(int weaponType)
{
	m_weaponType = weaponType;
}

int NDBaseRole::GetWeaponType()
{
	return m_weaponType;
}

void NDBaseRole::SetSecWeaponType(int secWeaponType)
{
	m_secWeaponType = secWeaponType;
}

int NDBaseRole::GetSecWeaponType()
{
	return m_secWeaponType;
}

void NDBaseRole::SetWeaponQuality(int quality)
{
	m_weaponQuality = quality;
}

int NDBaseRole::GetWeaponQuality()
{
	return m_weaponQuality;
}

void NDBaseRole::SetSecWeaponQuality(int quality)
{
	m_secWeaponQuality = quality;
}

int NDBaseRole::GetSecWeaponQuality()
{
	return m_secWeaponQuality;
}

void NDBaseRole::SetCapQuality(int quality)
{
	m_capQuality = quality;
}

int NDBaseRole::GetCapQuality()
{
	return m_capQuality;
}

void NDBaseRole::SetArmorQuality(int quality)
{
	m_armorQuality = quality;
}

int NDBaseRole::GetArmorQuality()
{
	return m_armorQuality;
}

void NDBaseRole::SetCloakQuality(int quality)
{
	m_cloakQuality = quality;
}

int NDBaseRole::GetCloakQuality()
{
	return m_cloakQuality;
}

void NDBaseRole::SetHair(int style, int color)
{

	switch (style)
	{
	case 1:
	case 0:
		this->hair = 10000;
		break;
	case 2:
		this->hair = 10001;
		break;
	case 3:
		this->hair = 10002;
		break;
	}
	this->hairColor = color;

	/**
	 * ��ʱ��ע�� ����
	 * begin
	 */
// 	NSString* hairImageName = [NSString stringWithUTF8String:NDPath::GetImagePath().c_str()];
// 	hairImageName = [NSString stringWithFormat:@"%@%d", hairImageName, this->hair];
// 	if (this->sex % 2 == SpriteSexMale) 
// 	{
// 		hairImageName = [NSString stringWithFormat:@"%@_1", hairImageName];
// 	}
// 	else 
// 	{
// 		hairImageName = [NSString stringWithFormat:@"%@_2", hairImageName];
// 	}
// 	hairImageName = [NSString stringWithFormat:@"%@.png", hairImageName];
// 	this->SetHairImage([hairImageName UTF8String], this->hairColor);
}

void NDBaseRole::SetMaxLife(int nMaxLife)
{
	this->m_nMaxLife = nMaxLife;

	if (this->m_nLife > nMaxLife)
	{
		this->m_nLife = nMaxLife;
	}
}

void NDBaseRole::SetMaxMana(int nMaxMana)
{
	this->m_nMaxMana = nMaxMana;
	if (this->m_nMaxMana > nMaxMana)
	{
		this->m_nMaxMana = nMaxMana;
	}
}

void NDBaseRole::SetCamp(CAMP_TYPE btCamp)
{
	this->m_eCamp = btCamp;
	if (btCamp == CAMP_TYPE_NONE)
	{
		this->m_strRank.clear();
	}
}

void NDBaseRole::SetPositionEx(CGPoint newPosition)
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
		this->SetWeaponType(WEAPON_NONE);
		this->SetRightHandWeaponImage("");
		this->SetDoubleHandWeaponImage("");
		this->SetDoubleHandWandImage("");
		this->SetDoubleHandBowImage("");
		this->SetDoubleHandSpearImage("");
		this->SetWeaponQuality(0);
		break;
	case Item::eEP_FuArmor:
		this->SetSecWeaponType(WEAPON_NONE);
		this->SetLeftHandWeaponImage("");
		this->SetShieldImage("");
		this->SetSecWeaponQuality(0);
		break;
	case Item::eEP_Head:
		this->SetCapImage("");
		this->SetCapQuality(0);
		break;
	case Item::eEP_Armor:
		this->SetArmorImage("");
		this->SetArmorQuality(0);
		break;
	case Item::eEP_YaoDai:
		this->SetCloakImage("");
		this->SetLeftShoulderImage("");
		this->SetRightShoulderImage("");
		this->SetSkirtStandImage("");
		this->SetSkirtWalkImage("");
		this->SetSkirtSitImage("");
		this->SetSkirtLiftLegImage("");
		this->SetCloakQuality(0);
		this->m_nCloak = -1;
		break;
	case Item::eEP_Ride:
		SAFE_DELETE_NODE (m_pkRidePet);
		break;
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

void NDBaseRole::addTalkMsg(std::string msg, int timeForTalkMsg)
{
	NDScene *pkScene = NDDirector::DefaultDirector()->GetRunningScene();
	if (!pkScene || !pkScene->IsKindOfClass(RUNTIME_CLASS(GameScene)))
	{
		return;
	}

	//if (!m_talkBox && subnode) 
	//{
	//	m_talkBox = new TalkBox;
	//	m_talkBox->Initialization();
	//	((GameScene*)scene)->AddUIChild(m_talkBox);
	//	m_talkBox->SetDelegate(this);
	//	m_talkBox->SetVisible(false);
	//}
	//
	//if (timeForTalkMsg == 0) m_talkBox->SetFix();
	//m_talkBox->addTalkMsg(msg, timeForTalkMsg);
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

void NDBaseRole::SafeAddEffect(NDSprite*& sprite, std::string file)
{
	if (sprite == NULL && !file.empty())
	{
		sprite = new NDSprite;
		sprite->Initialization(NDPath::GetAniPath(file.c_str()));
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

void NDBaseRole::HandleShadow(CGSize parentsize)
{
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

	CGSize sizeShadow = pic->GetSize();

	int x = m_position.x - DISPLAY_POS_X_OFFSET;
	int y = m_position.y - DISPLAY_POS_Y_OFFSET;

	pic->DrawInRect(
			CGRectMake(x + m_iShadowOffsetX,
					y + m_iShadowOffsetY
							+ NDDirector::DefaultDirector()->GetWinSize().height
							- parentsize.height, sizeShadow.width,
					sizeShadow.height));
}

void NDBaseRole::SetNormalAniGroup(int lookface)
{
	if (lookface <= 0)
	{
		return;
	}

	Initialization(
			tq::CString("%smodel_%d%s",
					NDEngine::NDPath::GetAnimationPath().c_str(),
					lookface / 100, ".spr"));

	m_bFaceRight = true;
	SetCurrentAnimation(MANUELROLE_STAND, m_bFaceRight);
}
