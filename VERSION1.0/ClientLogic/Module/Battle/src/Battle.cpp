/*
 *  Battle.mm
 *  DragonDrive
 *
 *  Created by wq on 11-1-17.
 *  Copyright 2011 (����)DeNA. All rights reserved.
 *
 */

#include "Battle.h"
#include "Fighter.h"
#include "NDPlayer.h"
#include "NDConstant.h"
#include "CCPointExtension.h"
#include "ImageNumber.h"
#include "NDUtility.h"
#include "NDDirector.h"
#include "NDMsgDefine.h"
#include "NDDataTransThread.h"
#include "BattleUtil.h"
#include "NDMapLayer.h"
///< #include "NDMapMgr.h" ��ʱ��ע�� ����

#include "ItemMgr.h"
#include <sstream>
//#include "NDUIMemo.h"
#include "SMGameScene.h"
#include "NDAnimationGroup.h"
#include "NDUISynLayer.h"
//#include "ChatInput.h"
//#include "ChatRecordManager.h"
#include "StatusDialog.h"
//#include "NDDataPersist.h"
#include "ItemImage.h"
//#include "BattleFieldScene.h"
#include "CPet.h"
//#include "NewChatScene.h"
#include "GameScene.h"
#include "NDPath.h"
#include "GlobalDialog.h"

IMPLEMENT_CLASS(QuickTalkCell, NDUINode)

QuickTalkCell::QuickTalkCell()
{
	m_clrFocus = ccc4(9, 54, 55, 255);
	m_clrText = ccc4(155, 255, 255, 255);
	m_clrFocusText = ccc4(249, 229, 64, 255);
	m_lbText = NULL;
}

QuickTalkCell::~QuickTalkCell()
{

}

void QuickTalkCell::Initialization(const char* pszText, const CGSize& size)
{
	NDUINode::Initialization();

	NDUIImage* img = new NDUIImage;
	img->Initialization();
	NDPicture* pic = new NDPicture;
	pic->Initialization(NDPath::GetImgPathBattleUI("chat_icon_sys.png").c_str());
	img->SetPicture(pic, true);
	img->SetFrameRect(CGRectMake(3.0f, 9.0f, 9.0f, 9.0f));
	AddChild(img);

	img = new NDUIImage;
	img->Initialization();
	pic = new NDPicture;
	pic->Initialization(NDPath::GetImgPathBattleUI("battle_chat_line.png").c_str(), 10,1);
	img->SetPicture(pic, true);
	img->SetFrameRect(CGRectMake(0.0f, size.height - 6.0f, size.width, 2.0f));
	AddChild(img);

	CGRect rectText = CGRectMake(15.0f, 5.0f, size.width - 15.0f,
			size.height - 7.0f);

	m_lbText = new NDUILabel;
	m_lbText->Initialization();
	m_lbText->SetFontSize(13);
	m_lbText->SetFontColor(m_clrText);
	m_lbText->SetText(pszText);
	m_lbText->SetTextAlignment(LabelTextAlignmentLeft);
	m_lbText->SetFrameRect(rectText);
	AddChild(m_lbText);
}

void QuickTalkCell::draw()
{
	NDNode* parentNode = GetParent();

	if (parentNode && parentNode->IsKindOfClass(RUNTIME_CLASS(NDUILayer)))
	{
		NDUILayer* uiLayer = (NDUILayer*) parentNode;

		if (uiLayer->GetFocus() == this)
		{ // ��ǰ���ڽ���,���ƽ���ɫ
		  //DrawRecttangle(GetScreenRect(), m_clrFocus); ///< ��ʱ��ע�� ����
			m_lbText->SetFontColor(m_clrFocusText);
		}
		else
		{
			m_lbText->SetFontColor(m_clrText);
		}
	}

	NDUINode::draw();
}

string QuickTalkCell::GetText()
{
	if (m_lbText)
	{
		return m_lbText->GetText();
	}

	return "";
}

IMPLEMENT_CLASS(HighlightTipStatusBar, NDUINode)
IMPLEMENT_CLASS(HighlightTip, NDUILayer)

HighlightTip::HighlightTip()
{
	m_pkPicBubble = NULL;
	m_hpBar = NULL;
	m_mpBar = NULL;
}
HighlightTip::~HighlightTip()
{
	CC_SAFE_DELETE (m_pkPicBubble);
}

void HighlightTip::Initialization()
{
	NDUILayer::Initialization();

	m_pkPicBubble = new NDPicture;
	m_pkPicBubble->Initialization(NDPath::GetImgPath("ui_map.png").c_str(), 70, 60);

	NDUIImage* pImgBubble = new NDUIImage;
	pImgBubble->Initialization();
	pImgBubble->SetFrameRect(
			CGRectMake(0, 0, m_pkPicBubble->GetSize().width,
					m_pkPicBubble->GetSize().height));
	pImgBubble->SetPicture(m_pkPicBubble, false);
	AddChild(pImgBubble);

	NDUIImage* imgHp = new NDUIImage;
	imgHp->Initialization();
	NDPicture* pic = NDPicturePool::DefaultPool()->AddPicture(
			NDPath::GetImgPath("hp.png"));
	imgHp->SetPicture(pic);
	imgHp->SetFrameRect(
			CGRectMake(2, 20, pic->GetSize().width, pic->GetSize().height));
	AddChild(imgHp);

	NDUIImage* imgMp = new NDUIImage;
	imgMp->Initialization();
	pic = NDPicturePool::DefaultPool()->AddPicture(
			NDPath::GetImgPath("mp.png"));
	imgMp->SetPicture(pic);
	imgMp->SetFrameRect(
			CGRectMake(2, 35, pic->GetSize().width, pic->GetSize().height));
	AddChild(imgMp);

	m_hpBar = new HighlightTipStatusBar(0xC7321A);
	m_hpBar->Initialization();
	AddChild(m_hpBar);

	m_mpBar = new HighlightTipStatusBar(0x3C4ACF);
	m_mpBar->Initialization();
	AddChild(m_mpBar);

	m_imgNumHp = new ImageNumber;
	m_imgNumHp->Initialization();
	m_imgNumHp->SetFrameRect(CGRectMake(16, 25, 40, 10));
	AddChild(m_imgNumHp);

	m_imgNumMp = new ImageNumber;
	m_imgNumMp->Initialization();
	m_imgNumMp->SetFrameRect(CGRectMake(16, 43, 40, 10));
	AddChild(m_imgNumMp);

	EnableEvent(false);
}

void HighlightTip::SetFighter(Fighter* pkFighter)
{
	if (!pkFighter)
	{
		return;
	}

	NDBaseRole* pkRole = pkFighter->GetRole();
	CGPoint kPoint = CGPointMake(pkFighter->getX(), pkFighter->getY());

	CGRect kRect = CGRectMake(kPoint.x, kPoint.y, m_pkPicBubble->GetSize().width,
			m_pkPicBubble->GetSize().height);

	kRect.origin.x -= kRect.size.width / 2;

	if (kRect.origin.x < 0)
	{
		kRect.origin.x = 0;
	}

	kRect.origin.y = kRect.origin.y - pkRole->GetHeight() - kRect.size.height;

	if (kRect.origin.y < 0)
	{
		kRect.origin.y = 0;
	}

	SetFrameRect(kRect);

	m_hpBar->SetNum(pkFighter->m_kInfo.nLife, pkFighter->m_kInfo.nLifeMax);
	m_hpBar->SetFrameRect(
			CGRectMake(kRect.origin.x + 16, kRect.origin.y + 20, 40, 10));
	m_imgNumHp->SetSmallRedTwoNumber(pkFighter->m_kInfo.nLife, pkFighter->m_kInfo.nLifeMax);

	m_mpBar->SetNum(pkFighter->m_kInfo.nMana, pkFighter->m_kInfo.nManaMax);
	m_mpBar->SetFrameRect(
			CGRectMake(kRect.origin.x + 16, kRect.origin.y + 38, 40, 10));
	m_imgNumMp->SetSmallRedTwoNumber(pkFighter->m_kInfo.nMana, pkFighter->m_kInfo.nManaMax);

	NDUILabel* pkNameLabel = (NDUILabel*) GetChild(TAG_NAME);

	if (!pkNameLabel)
	{
		pkNameLabel = new NDUILabel;
		pkNameLabel->SetFontColor(ccc4(255, 255, 255, 255));
		pkNameLabel->Initialization();
		pkNameLabel->SetTag(TAG_NAME);
		AddChild(pkNameLabel);
	}
	std::stringstream kStringStream;
	kStringStream << pkRole->m_strName << "Lv" << pkRole->m_nLevel;
	//	if (f->m_kInfo.fighterType == Fighter_TYPE_RARE_MONSTER)
	//	{
	//		ss << "��" << NDCommonCString("xiyou") << "��"; 
	//	}
	pkNameLabel->SetText(kStringStream.str().c_str());
	CGSize sizeName = getStringSize(kStringStream.str().c_str(), 15);
	pkNameLabel->SetFrameRect(
			CGRectMake((kRect.size.width - sizeName.width) / 2, 0,
					sizeName.width, sizeName.height));
}

enum
{
	MAX_TURN = 30,	// ���غ���
	AUTO_COUNT = 0,	// �Զ�����ʱʱ��

	TEXT_BTN_HEIGHT = 29, // ��ť�߶�
};

#define TIMER_TIMELEFT 1
#define TIMER_BACKTOGAME 2
#define TIMER_AUTOCOUNT 3
#define TIMER_AUTOFIGHT 4
#define MAX_SKILL_NUM (20)

const char* TEXT_VIEW_STATUS = NDCommonCString("ViewState");

bool Battle::ms_bAuto = false;
BattleAction Battle::ms_kLastTurnActionUser(BATTLE_ACT_PHY_ATK);
BattleAction Battle::ms_kLastTurnActionEudemon(BATTLE_ACT_PET_PHY_ATK);

IMPLEMENT_CLASS(Battle, NDUILayer)

void Battle::ResetLastTurnBattleAction()
{
	ms_bAuto = false;
	ms_kLastTurnActionUser.btAction = BATTLE_ACT_PHY_ATK;
	ms_kLastTurnActionUser.vData.clear();
	ms_kLastTurnActionEudemon.btAction = BATTLE_ACT_PET_PHY_ATK;
	ms_kLastTurnActionEudemon.vData.clear();
}

Battle::Battle()
{
	Init();
}

Battle::Battle(Byte btType)
{
	Init();
	m_battleType = BATTLE_TYPE(btType);

}

void Battle::CloseChatInput()
{
	//if (NULL != m_chatDelegate.tfChat.subviews) {
	//	[m_chatDelegate.tfChat removeFromSuperview];
	//}
}

Battle::~Battle()
{
	//	if (m_imgTurn) {
	//		m_imgTurn->RemoveFromParent(false);
	//		SAFE_DELETE(m_imgTurn);
	//	}
	//	if (m_imgTimer) {
	//		m_imgTimer->RemoveFromParent(false);
	//		SAFE_DELETE(m_imgTimer);
	//	}
	//	if (m_imgQuickTalkBg) {
	//		m_imgQuickTalkBg->RemoveFromParent(false);
	//		SAFE_DELETE(m_imgQuickTalkBg);
	//	}
	//	if (m_tlQuickTalk) {
	//		m_tlQuickTalk->RemoveFromParent(false);
	//		SAFE_DELETE(m_tlQuickTalk);
	//	}
	//	if (m_imgChat) {
	//		m_imgChat->RemoveFromParent(false);
	//		SAFE_DELETE(m_imgChat);
	//	}
	//	if (m_btnSendChat) {
	//		m_btnSendChat->RemoveFromParent(false);
	//		SAFE_DELETE(m_btnSendChat);
	//	}

	CloseChatInput();
	//[m_chatDelegate release];
	if (eraseInOutEffect)
	{
		NDEraseInOutEffect *eioEffect = eraseInOutEffect.Pointer();
		SAFE_DELETE_NODE(eioEffect);
	}
	CC_SAFE_DELETE (m_picActionWordDef);
	CC_SAFE_DELETE (m_picActionWordDodge);
	CC_SAFE_DELETE (m_picActionWordFlee);
	CC_SAFE_DELETE (m_battleBg);

	//	SAFE_DELETE(m_picTalk);
	//	SAFE_DELETE(m_picQuickTalk);
	//	SAFE_DELETE(m_picAuto);
	//	SAFE_DELETE(m_picAutoCancel);
	//	SAFE_DELETE(m_picLeave);

	// ����ս��״̬
	NDPlayer::defaultHero().UpdateState(USERSTATE_FIGHTING, false);

	CC_SAFE_DELETE (m_curBattleAction);

	if (m_dlgBattleResult)
	{
		m_dlgBattleResult->RemoveFromParent(true);
	}
	//	if(m_orignalMapId!=NDMapMgrObj.m_iMapID){
	//		NDMapMgrObj.loadSceneByMapID(m_orignalMapId);
	//		NDMapMgrObj.AddAllNpcToMap();
	//		NDMapMgrObj.AddAllMonsterToMap();
	//	}
	NDDirector* director = NDDirector::DefaultDirector();
//	director->PopScene(true);
	GameScene* gameScene = (GameScene*) director->GetRunningScene();

	/***
	 * ��ʱ��ע�� ���� begin
	 */
// 	NDMapLayer* mapLayer = NDMapMgrObj.getMapLayerOfScene(gameScene);
// 	if (mapLayer) {
// 		//		mapLayer->SetScreenCenter(m_orignalPos);
// 		//mapLayer->SetBattleBackground(false);
// 		mapLayer->replaceMapData(sceneMapId, sceneCenterX, sceneCenterY);
// 		ScriptMgrObj.excuteLuaFunc("SetUIVisible", "",1);
// 		int theId=NDMapMgrObj.GetMotherMapID();
// 		if(theId/100000000==9){
// 			ScriptMgrObj.excuteLuaFunc("showDynMapUI", "",0);
// 		}else{
// 			ScriptMgrObj.excuteLuaFunc("showCityMapUI", "",0);
// 		}
// 		//		mapLayer->AddChild(&(NDPlayer::defaultHero()));
// 	}
	/***
	 * ��ʱ��ע�� ����
	 * end
	 */

	gameScene->OnBattleEnd();
	//	if (m_btnLeave) {
	//		RemoveChild(m_btnLeave, false);
	//		SAFE_DELETE(m_btnLeave);
	//	}

	//	if (m_battleOpt) {
	//		RemoveChild(m_battleOpt, false);
	//		SAFE_DELETE(m_battleOpt);
	//	}
	//	if (m_eudemonOpt) {
	//		RemoveChild(m_eudemonOpt, false);
	//		SAFE_DELETE(m_eudemonOpt);
	//	}

	//	SAFE_DELETE(m_picWhoAmI);
	//	SAFE_DELETE(m_picLingPai);
	CC_SAFE_DELETE (m_picBaoJi);
	//	SAFE_DELETE(m_picBoji);

	//	if (s_bAuto) {
	//		RemoveChild(m_lbAuto, false);
	//		RemoveChild(m_imgAutoCount, false);
	//		SAFE_DELETE(m_imgAutoCount);
	//		SAFE_DELETE(m_lbAuto);
	//	}

	ReleaseCommandList();

	for (VEC_FIGHTER_IT it = m_vAttaker.begin(); it != m_vAttaker.end(); it++)
	{
		Fighter* fighter = *it;
		fighter->GetRole()->RemoveFromParent(false);
	}

	for (VEC_FIGHTER_IT it = m_vDefencer.begin(); it != m_vDefencer.end(); it++)
	{
		Fighter* fighter = *it;
		fighter->GetRole()->RemoveFromParent(false);
	}

	for (VEC_SUB_ANI_GROUP_IT it = m_vSubAniGroup.begin();
			it != m_vSubAniGroup.end(); it++)
	{
		it->frameRec->release();
	}

	NDPlayer& player = NDPlayer::defaultHero();

	if (player.IsInState(USERSTATE_DEAD))
	{
		NDScene* gameScene = NDDirector::DefaultDirector()->GetRunningScene();
		if (gameScene && gameScene->IsKindOfClass(RUNTIME_CLASS(GameScene)))
		{
			((GameScene*) gameScene)->ShowRelieve(true);
		}
	}
	else
	{
		// ʹ���Զ��ָ�ҩ
		if (player.m_nLife < player.m_nMaxLife
				|| player.m_nMana < player.m_nMaxMana)
		{
			ItemMgr& items = ItemMgrObj;
			Item* pkRecoverItem = items.GetBagItemByType(IT_RECOVER);

			if (pkRecoverItem && pkRecoverItem->m_bIsActive)
			{
				sendItemUse(*pkRecoverItem);
			}
		}
	}

	//if (player.IsInState(USERSTATE_BF_WAIT_RELIVE))
	//{
	//	BattleFieldRelive::Show();
	//}
	//
	if (m_rewardContent.size() > 2)
	{
		GlobalShowDlg(NDCommonCString("BattleRes"), m_rewardContent.c_str(),3.0f);
	}
}

void Battle::OnDialogClose(NDUIDialog* dialog)
{
	if (m_dlgBattleResult == dialog)
	{
		// ����ͷ�dialog
		m_dlgBattleResult->SetDelegate(NULL);
		m_dlgBattleResult = NULL;
		BattleMgrObj.quitBattle();
		//	} else if (m_dlgHint == dialog) {
		//		m_dlgHint = NULL;
		//		switch (m_battleStatus) {
		//			case BS_USE_ITEM_MENU:
		//			case BS_USER_SKILL_MENU:
		//				m_battleStatus = BS_USER_MENU;
		//				//AddChild(m_battleOpt);
		//				break;
		//			case BS_EUDEMON_SKILL_MENU:
		//				m_battleStatus = BS_EUDEMON_MENU;
		//				//AddChild(m_eudemonOpt);
		//				break;
		//			default:
		//				break;
		//		}
	}
}

void Battle::AddFighter(Fighter* f)
{
	BATTLE_GROUP group = f->GetGroup();
	NDBaseRole* role = f->GetRole();
	if (!role)
	{
		CC_SAFE_DELETE(f);
		return;
	}

	AddChild(role);
	if (role->IsKindOfClass(RUNTIME_CLASS(NDPlayer)))
	{
		//		m_playerHead->SetBattlePlayer(f);
		m_ourGroup = group;
	}

	//	if (role->IsKindOfClass(RUNTIME_CLASS(NDBattlePet))) {
	//		if (f->m_kInfo.line == BATTLE_LINE_FRONT && f->m_kInfo.posIdx == 0) { // ���ս��
	//			if (NULL == m_petHead) {
	//				m_petHead = new PetHead;
	//				m_petHead->Initialization(f);
	//				AddChild(m_petHead);
	//			}
	//		}
	//	}

	if (BATTLE_GROUP_DEFENCE == group)
	{
		role->m_bFaceRight = false;
		m_vAttaker.push_back(f);
	}
	else
	{
		role->m_bFaceRight = true;
		m_vDefencer.push_back(f);
	}
	f->setPosition(m_teamAmout);
	f->showFighterName(true);
	//	if (f->m_kInfo.bRoleMonster) {
	//		role->m_faceRight = !role->m_faceRight;
	//	}

	battleStandAction(*f);
}

void Battle::clearHighlight()
{
	RemoveChild(TAG_LINGPAI, true);
	RemoveChild(TAG_NAME, true);
	clearAllWillBeAtk();
	//if (m_highlightFighter)
	//		m_highlightFighter->setWillBeAtk(false);
	m_highlightFighter = NULL;
}

void Battle::CloseViewStatus()
{
	RemoveChild(TAG_VIEW_FIGHTER_STATUS, true);
	clearHighlight();

	//	if (m_dlgHint) {
	//		m_dlgHint->RemoveFromParent(true);
	//		m_dlgHint = NULL;
	//	}

	if (m_dlgStatus)
	{
		m_dlgStatus->RemoveFromParent(true);
		m_dlgStatus = NULL;
	}
}

//void Battle::CloseSkillMenu()
//{
//	if (m_dlgHint) {
//		m_dlgHint->RemoveFromParent(true);
//		m_dlgHint = NULL;
//	} else if (m_skillOpt) {
//		RemoveChild(m_skillOpt, true);
//		m_skillOpt = NULL;
//		RemoveChild(TAG_SKILL_MEMO, true);
//	}
//}

//void Battle::CloseItemMenu()
//{
//	if (m_dlgHint) {
//		m_dlgHint->RemoveFromParent(true);
//		m_dlgHint = NULL;
//	} else if (m_itemOpt) {
//		m_mapUseItem.clear();
//		RemoveChild(m_itemOpt, true);
//		m_itemOpt = NULL;
//	}
//}

void Battle::ShowQuickChat(bool bShow)
{
	//	if (bShow) {
	//		AddChild(m_imgQuickTalkBg);
	//		AddChild(m_tlQuickTalk);
	//		m_layerBtnQuitTalk->SetFrameRect(CGRectMake(35, 136, 48, 20));
	//	} else {
	//		RemoveChild(m_imgQuickTalkBg, false);
	//		RemoveChild(m_tlQuickTalk, false);
	//		m_layerBtnQuitTalk->SetFrameRect(CGRectMake(35, 300, 48, 20));
	//	}
}

void Battle::ShowChatTextField(bool bShow)
{
	//	if (m_bShowChatTextField == bShow) {
	//		return;
	//	}
	//	
	//	m_bShowChatTextField = bShow;
	//	
	//	if (m_bShowChatTextField) {
	//		AddChild(m_imgChat);
	//		AddChild(m_btnSendChat);
	//		
	//		if (NULL == m_chatDelegate.tfChat) {
	//			UITextField* tfChat = [[UITextField alloc] init];
	//			tfChat.transform = CGAffineTransformMakeRotation(3.141592f/2.0f);
	//			tfChat.frame = CGRectMake(290.0f, 54.0f, 25.0f, 326.0f);
	//			tfChat.textColor = [UIColor whiteColor];
	//			tfChat.returnKeyType = UIReturnKeyDone;
	//			tfChat.delegate = m_chatDelegate;
	//			m_chatDelegate.tfChat = tfChat;
	//			[tfChat release];
	//		}
	//		
	//		if (NULL == [m_chatDelegate.tfChat superview]) {
	//			[[[CCDirector sharedDirector] openGLView] addSubview:m_chatDelegate.tfChat];
	//		}
	//	} else {
	//		RemoveChild(m_imgChat, false);
	//		RemoveChild(m_btnSendChat, false);
	//		[m_chatDelegate.tfChat removeFromSuperview];
	//	}
}

void Battle::OnButtonClick(NDUIButton* button)
{
	//	if (button == m_btnLeave) {
	//		if (m_bWatch) {
	//			ShowProgressBar;
	//			NDTransData bao(_MSG_BATTLEACT);
	//			bao << BATTLE_ACT_LEAVE << Byte(0) << Byte(0);
	//			// SEND_DATA(bao);
	//			return;
	//		}
	//		
	//	} else if (button == m_btnAuto) {
	//		if (s_bAuto) {
	//			stopAuto();
	//		} else {
	//			OnBtnAuto(m_battleStatus < BS_SET_FIGHTER);
	//		}
	//		
	//		/*switch (m_battleStatus) {
	//			case BS_USER_MENU:
	//			case BS_EUDEMON_MENU:
	//			{
	//				stopAuto();
	//				VEC_FIGHTER& enemyList = GetEnemySideList();
	//				for (VEC_FIGHTER_IT it = enemyList.begin(); it != enemyList.end(); it++) {
	//					if ((*it)->isVisiable()) {
	//						if (m_battleStatus == BS_EUDEMON_MENU) {
	//							m_battleStatus = BS_CHOOSE_VIEW_FIGHTER_STATUS_PET;
	//							RemoveChild(m_eudemonOpt, false);
	//						} else {
	//							m_battleStatus = BS_CHOOSE_VIEW_FIGHTER_STATUS;
	//							RemoveChild(m_battleOpt, false);
	//						}
	//						
	//						HighlightFighter(*it);
	//						
	//						CGSize size = NDDirector::DefaultDirector()->GetWinSize();
	//						NDUILabel* lbViewFighter = new NDUILabel;
	//						lbViewFighter->Initialization();
	//						lbViewFighter->SetText(TEXT_VIEW_STATUS);
	//						lbViewFighter->SetFontColor(ccc4(255, 255, 255, 255));
	//						CGSize sizeText = getStringSize(TEXT_VIEW_STATUS, 15);
	//						lbViewFighter->SetTag(TAG_VIEW_FIGHTER_STATUS);
	//						lbViewFighter->SetFrameRect(CGRectMake((size.width - sizeText.width) / 2
	//										       , 260, sizeText.width, sizeText.height));
	//						AddChild(lbViewFighter);
	//						return;
	//					}
	//				}
	//			}
	//				break;
	//			case BS_CHOOSE_VIEW_FIGHTER_STATUS:
	//				CloseViewStatus();
	//				AddChild(m_battleOpt);
	//				m_battleStatus = BS_USER_MENU;
	//				break;
	//			case BS_CHOOSE_VIEW_FIGHTER_STATUS_PET:
	//				CloseViewStatus();
	//				AddChild(m_eudemonOpt);
	//				m_battleStatus = BS_EUDEMON_MENU;
	//				break;
	//			case BS_USE_ITEM_MENU:
	//				CloseItemMenu();
	//				m_battleStatus = BS_USER_MENU;
	//				AddChild(m_battleOpt);
	//				break;
	//			case BS_EUDEMON_USE_ITEM_MENU:
	//				CloseItemMenu();
	//				m_battleStatus = BS_EUDEMON_MENU;
	//				AddChild(m_eudemonOpt);
	//				break;
	//			case BS_USER_SKILL_MENU:
	//				CloseSkillMenu();
	//				m_battleStatus = BS_USER_MENU;
	//				AddChild(m_battleOpt);
	//				break;
	//			case BS_EUDEMON_SKILL_MENU:
	//				CloseSkillMenu();
	//				m_battleStatus = BS_EUDEMON_MENU;
	//				AddChild(m_eudemonOpt);
	//				break;
	//			case BS_CHOOSE_ENEMY_CATCH:
	//			case BS_CHOOSE_ENEMY_PHY_ATK:
	//			case BS_CHOOSE_ENEMY_MAG_ATK:
	//			case BS_CHOOSE_OUR_SIDE_MAG_ATK:
	//				clearHighlight();
	//				AddChild(m_battleOpt);
	//				m_battleStatus = BS_USER_MENU;
	//				break;
	//			case BS_CHOOSE_OUR_SIDE_USE_ITEM_USER:
	//				clearHighlight();
	//				OnBtnUseItem(BS_USE_ITEM_MENU);
	//				break;
	//			case BS_CHOOSE_OUR_SIDE_USE_ITEM_EUDEMON:
	//				clearHighlight();
	//				OnBtnUseItem(BS_EUDEMON_USE_ITEM_MENU);
	//				break;
	//			case BS_CHOOSE_ENEMY_PHY_ATK_EUDEMON:
	//			case BS_CHOOSE_ENEMY_MAG_ATK_EUDEMON:
	//			case BS_CHOOSE_OUR_SIDE_MAG_ATK_EUDEMON:
	//				clearHighlight();
	//				AddChild(m_eudemonOpt);
	//				m_battleStatus = BS_EUDEMON_MENU;
	//				break;
	//			default:
	//				break;
	//		}*/
	//	} else if (button == m_btnTalk) 
	//	{
	//		//ChatRecordManager::DefaultManager()->Show();
	//		// ֱ����ʾ���������
	//		//ShowChatTextField(!m_bShowChatTextField);
	//		NewChatScene::DefaultManager()->Show();
	//	} else if (button == m_btnQuickTalk) {
	//		// �������
	//		ShowQuickChat(m_imgQuickTalkBg->GetParent() == NULL);
	//		
	//	} else if (button == m_btnSendChat) {
	//		NSString* msg = m_chatDelegate.tfChat.text;
	//		if ([msg length] > 0) {
	//			ChatInput::SendChatDataToServer(ChatTypeSection, [msg UTF8String]);
	//			m_chatDelegate.tfChat.text = @"";
	//			string speaker = NDPlayer::defaultHero().m_name;
	//			speaker += "��";
	//			Chat::DefaultChat()->AddMessage(ChatTypeSection, [msg UTF8String], speaker.c_str());
	//		}
	//	}
	//	else if (button == m_btnCancleAutoFight)
	//	{
	//		//ȡ���Զ�ս��
	//		RemoveCancleAutoFightButton();
	//		s_bAuto = false;
	//	}
}

void Battle::Initialization(int action)
{
	NDUILayer::Initialization();

	SetScrollEnabled(true);

	if (action == BATTLE_STAGE_WATCH)
	{
		m_bWatch = true;
	}

	CGSize winSize = NDDirector::DefaultDirector()->GetWinSize();

	m_battleBg = new NDUILayer();
	m_battleBg->Initialization();
	m_battleBg->SetBackgroundColor(ccc4(0, 0, 0, 188));
	m_battleBg->SetFrameRect(CGRectMake(0, 0, winSize.width, winSize.height));

	//	m_picTalk = new NDPicture();
	//	m_picTalk->Initialization(GetImgPathBattleUI("battle_chat.png"));
	//	
	//	m_picQuickTalk = new NDPicture;
	//	m_picQuickTalk->Initialization(GetImgPathBattleUI("battle_fast_handle.png"));
	//	
	//	m_btnTalk = new NDUIButton();
	//	m_btnTalk->Initialization();
	//	m_btnTalk->SetImage(m_picTalk);
	//	m_btnTalk->SetFrameRect(CGRectMake(0, 263, 47, 57));
	//	m_btnTalk->SetTag(BTN_TALK);
	//	m_btnTalk->SetDelegate(this);
	//	AddChild(m_btnTalk);
	//	
	//	m_layerBtnQuitTalk = new NDUILayer;
	//	m_layerBtnQuitTalk->Initialization();
	//	m_layerBtnQuitTalk->SetBackgroundColor(ccc4(0, 0, 0, 0));
	//	m_layerBtnQuitTalk->SetFrameRect(CGRectMake(40, 300, 48, 20));
	//	AddChild(m_layerBtnQuitTalk, 1);
	//	
	//	m_btnQuickTalk = new NDUIButton();
	//	m_btnQuickTalk->Initialization();
	//	m_btnQuickTalk->SetImage(m_picQuickTalk);
	//	m_btnQuickTalk->SetFrameRect(CGRectMake(0, 0, 48, 20));
	//	m_btnQuickTalk->SetDelegate(this);
	//	m_layerBtnQuitTalk->AddChild(m_btnQuickTalk);
	//	
	//	
	//	if (m_bWatch) {
	//		m_picLeave = new NDPicture();
	//		m_picLeave->Initialization(GetImgPathBattleUI("battle_cancel.png"));
	//		
	//		m_btnLeave = new NDUIButton();
	//		m_btnLeave->Initialization();
	//		m_btnLeave->SetImage(m_picLeave);
	//		m_btnLeave->SetDelegate(this);
	//		m_btnLeave->SetFrameRect(CGRectMake(420, 260, 60, 60));
	//		AddChild(m_btnLeave);
	//	} else {
	//		m_picAuto = new NDPicture();
	//		m_picAuto->Initialization(GetImgPathBattleUI("battle_auto.png"));
	//		
	//		m_picAutoCancel = new NDPicture();
	//		m_picAutoCancel->Initialization(GetImgPathBattleUI("battle_auto_cancel.png"));
	//		
	//		m_btnAuto = new NDUIButton();
	//		m_btnAuto->Initialization();
	//		m_btnAuto->SetImage(s_bAuto ? m_picAutoCancel : m_picAuto);
	//		m_btnAuto->SetFrameRect(CGRectMake(420, 260, 60, 60));
	//		m_btnAuto->SetDelegate(this);
	//		AddChild(m_btnAuto);
	//	}
	//	
	//	m_imgTimer = new NDUIImage;
	//	m_imgTimer->Initialization();
	//	NDPicture* pic = new NDPicture;
	//	pic->Initialization(GetImgPathBattleUI("timerback.png"));
	//	m_imgTimer->SetPicture(pic, true);
	//	m_imgTimer->SetFrameRect(CGRectMake(185.0f, 75.0f, 111.0f, 46.0f));
	//	AddChild(m_imgTimer);
	//	
	//	m_lbTimerTitle = new NDUILabel;
	//	m_lbTimerTitle->?ialization();
	//	m_lbTimerTitle->SetFontSize(14);
	//	m_lbTimerTitle->SetFontColor(ccc4(255, 255, 255, 255));
	//	m_lbTimerTitle->SetText(NDCommonCString("DaoJiShi"));
	//	m_lbTimerTitle->SetFrameRect(CGRectMake(218.0f, 81.0f, 60.0f, 20.0f));
	//	AddChild(m_lbTimerTitle);
	//	
	//	m_lbTimer = new NDUILabel;
	//	m_lbTimer->Initialization();
	//	m_lbTimer->SetFontSize(14);
	//	m_lbTimer->SetFontColor(ccc4(255, 255, 255, 255));
	//	m_lbTimer->SetText("30");
	//	m_lbTimer->SetFrameRect(CGRectMake(233.0f, 103.0f, 60.0f, 20.0f));
	//	AddChild(m_lbTimer);
	//	
	//	m_imgTurn = new NDUIImage;
	//	m_imgTurn->Initialization();
	//	pic = new NDPicture;
	//	pic->Initialization(GetImgPathBattleUI("turnback.png"));
	//	m_imgTurn->SetPicture(pic, true);
	//	m_imgTurn->SetFrameRect(CGRectMake(185.0f, 155.0f, 111.0f, 46.0f));
	//	AddChild(m_imgTurn);
	//	
	//	m_lbTurnTitle = new NDUILabel;
	//	m_lbTurnTitle->Initialization();
	//	m_lbTurnTitle->SetFontSize(14);
	//	m_lbTurnTitle->SetFontColor(ccc4(255, 255, 255, 255));
	//	m_lbTurnTitle->SetText(NDCommonCString("HuiHe"));
	//	m_lbTurnTitle->SetFrameRect(CGRectMake(225.0f, 160.0f, 60.0f, 20.0f));
	//	AddChild(m_lbTurnTitle);
	//	
	//	m_lbTurn = new NDUILabel;
	//	m_lbTurn->Initialization();
	//	m_lbTurn->SetFontSize(14);
	//	m_lbTurn->SetFontColor(ccc4(255, 255, 255, 255));
	//	m_lbTurn->SetText("1/30");
	//	m_lbTurn->SetFrameRect(CGRectMake(226.0f, 177.0f, 60.0f, 20.0f));
	//	AddChild(m_lbTurn);
	//	
	//	m_timer.SetTimer(this, TIMER_TIMELEFT, 1);
	//	
//		NDEraseInOutEffect *eioEffect = new NDEraseInOutEffect;
//		eioEffect->Initialization();
//		eraseInOutEffect = eioEffect->QueryLink();
	//	
	//	m_imgChat = new NDUIImage;
	//	m_imgChat->Initialization();
	//	pic = new NDPicture;
	//	pic->Initialization(GetImgPathBattleUI("input_edit.png"));
	//	m_imgChat->SetPicture(pic, true);
	//	m_imgChat->SetFrameRect(CGRectMake(44.0f, 0.0f, 392.0f, 38.0f));
	//	//AddChild(m_imgChat);
	//	
	//	m_btnSendChat = new NDUIButton;
	//	m_btnSendChat->Initialization();
	//	pic = new NDPicture;
	//	pic->Initialization(GetImgPathBattleUI("chat_btn_normal.png"));
	//	m_btnSendChat->SetImage(pic, false, CGRectMake(0, 0, 0, 0), true);
	//	m_btnSendChat->SetFrameRect(CGRectMake(386.0f, 3.0f, 25.0f, 26.0f));
	//	m_btnSendChat->SetDelegate(this);
	//	//AddChild(m_btnSendChat);
	//	
	//	m_imgQuickTalkBg = new NDUIImage;
	//	m_imgQuickTalkBg->Initialization();
	//	pic = new NDPicture;
	//	pic->Initialization(GetImgPathBattleUI("battle_fast_bg.png"), 10, 10);
	//	m_imgQuickTalkBg->SetPicture(pic, true);
	//	m_imgQuickTalkBg->SetFrameRect(CGRectMake(1.0f, 154.0f, 210.0f, 166.0f));
	//	//AddChild(m_imgQuickTalkBg);
	//	
	//	m_tlQuickTalk = new NDUITableLayer;
	//	m_tlQuickTalk->Initialization();
	//	m_tlQuickTalk->VisibleSectionTitles(false);
	//	
	//	m_tlQuickTalk->SetBackgroundColor(ccc4(0, 0, 0, 0));
	//	NDDataSource *ds = new NDDataSource;
	//	NDSection *sec = new NDSection;
	//	
	//	//NDQuickTalkDataPersist& quickTalk = NDQuickTalkDataPersist::DefaultInstance();
	//	//vector<string> vTalk;
	//	//quickTalk.GetAllQuickTalkString(NDPlayer::defaultHero().m_id, vTalk);
	//	
	//	QuickTalkCell *talk = new QuickTalkCell;
	//	talk->Initialization([kSysQuickTalk1 UTF8String], CGSizeMake(195.0f, 32.0f));
	//	sec->AddCell(talk);
	//	
	//	talk = new QuickTalkCell;
	//	talk->Initialization([kSysQuickTalk2 UTF8String], CGSizeMake(195.0f, 32.0f));
	//	sec->AddCell(talk);
	//	
	//	talk = new QuickTalkCell;
	//	talk->Initialization([kSysQuickTalk3 UTF8String], CGSizeMake(195.0f, 32.0f));
	//	sec->AddCell(talk);
	//	
	//	talk = new QuickTalkCell;
	//	talk->Initialization([kSysQuickTalk4 UTF8String], CGSizeMake(195.0f, 32.0f));
	//	sec->AddCell(talk);
	//	
	//	talk = new QuickTalkCell;
	//	talk->Initialization([kSysQuickTalk5 UTF8String], CGSizeMake(195.0f, 32.0f));
	//	sec->AddCell(talk);
	//	
	//	sec->SetFocusOnCell(0);
	//	m_tlQuickTalk->SetFrameRect(CGRectMake(3.0f, 160.0f, 204.0f, 165.0f));
	//	
	//	ds->AddSection(sec);
	//	m_tlQuickTalk->SetDelegate(this);
	//	m_tlQuickTalk->SetDataSource(ds);
	//AddChild(m_tlQuickTalk);

	if (m_bWatch)
	{ // ��ս����Ҫ��������
		return;
	}

	//	m_imgWhoAmI = new NDUIImage();
	//	m_imgWhoAmI->Initialization();
	//	m_picWhoAmI = NDPicturePool::DefaultPool()->AddPicture(GetImgPath("whoami.png"));
	//	m_picLingPai = NDPicturePool::DefaultPool()->AddPicture(GetImgPath("lingpai.png"));
	//	m_imgWhoAmI->SetFrameRect(CGRectMake(0, 0, m_picWhoAmI->GetSize().width, m_picWhoAmI->GetSize().height));
	//	m_imgWhoAmI->SetPicture(m_picWhoAmI);
	//	AddChild(m_imgWhoAmI);

	/*m_battleOpt = new NDUITableLayer;
	 m_battleOpt->Initialization();
	 m_battleOpt->VisibleSectionTitles(false);
	 NDDataSource *ds = new NDDataSource;
	 NDSection *sec = new NDSection;
	 
	 // ����
	 NDUIButton  *btnAttack = new NDUIButton;
	 btnAttack->Initialization();
	 btnAttack->SetTag(BTN_ATTATCK);
	 btnAttack->SetFocusColor(ccc4(253, 253, 253, 255));
	 btnAttack->SetTitle("��  ��");
	 sec->AddCell(btnAttack);
	 
	 // ��Ʒ
	 NDUIButton  *btnUseItem = new NDUIButton;
	 btnUseItem->Initialization();
	 btnUseItem->SetTitle("��  Ʒ");
	 btnUseItem->SetTag(BTN_ITEM);
	 btnUseItem->SetFocusColor(ccc4(253, 253, 253, 255));
	 sec->AddCell(btnUseItem);
	 
	 // ����
	 NDUIButton  *btnSkill = new NDUIButton;
	 btnSkill->Initialization();
	 btnSkill->SetTitle("��  ��");
	 btnSkill->SetTag(BTN_SKILL);
	 btnSkill->SetFocusColor(ccc4(253, 253, 253, 255));
	 sec->AddCell(btnSkill);
	 
	 // ����
	 NDUIButton  *btnDefence = new NDUIButton;
	 btnDefence->Initialization();
	 btnDefence->SetTitle("��  ��");
	 btnDefence->SetTag(BTN_DEFENCE);
	 btnDefence->SetFocusColor(ccc4(253, 253, 253, 255));
	 sec->AddCell(btnDefence);
	 
	 // �Զ�
	 NDUIButton  *btnAuto = new NDUIButton;
	 btnAuto->Initialization();
	 btnAuto->SetTitle("��  ��");
	 btnAuto->SetTag(BTN_AUTO);
	 btnAuto->SetFocusColor(ccc4(253, 253, 253, 255));
	 sec->AddCell(btnAuto);
	 
	 // ����
	 NDUIButton  *btnRun = new NDUIButton;
	 btnRun->Initialization();
	 if (m_battleType != BATTLE_TYPE_PRACTICE) {
	 btnRun->SetTitle("��  ��");
	 } else {
	 btnRun->SetTitle("��  ��");
	 }
	 
	 btnRun->SetTag(BTN_FLEE);
	 btnRun->SetFocusColor(ccc4(253, 253, 253, 255));
	 sec->AddCell(btnRun);
	 
	 // ��׽
	 if (m_battleType != BATTLE_TYPE_PRACTICE) {
	 NDUIButton  *btnCatch = new NDUIButton;
	 btnCatch->Initialization();
	 btnCatch->SetTitle("��  ׽");
	 btnCatch->SetTag(BTN_CATCH);
	 btnCatch->SetFocusColor(ccc4(253, 253, 253, 255));
	 sec->AddCell(btnCatch);
	 }
	 
	 sec->SetFocusOnCell(0);
	 m_battleOpt->SetFrameRect(CGRectMake((winSize.width / 2) - 30, winSize.height / 2 - 115, 60, TEXT_BTN_HEIGHT * sec->Count()));
	 
	 ds->AddSection(sec);
	 m_battleOpt->SetDelegate(this);
	 m_battleOpt->SetDataSource(ds);
	 AddChild(m_battleOpt);*/

	if (ms_bAuto)
	{
		SetAutoCount();
	}

	//	m_playerHead = new PlayerHead(&NDPlayer::defaultHero());
	//	m_playerHead->Initialization(true);
	//	AddChild(m_playerHead);

	if (ms_bAuto)
	{
		//RemoveChild(m_battleOpt, false);
		m_timer.SetTimer(this, TIMER_AUTOFIGHT, 0.5f);
	}
}

void Battle::InitEudemonOpt()
{
	// ��ս��
	getMainEudemon();

	/*if (getMainEudemon()) {
	 CGSize winSize = NDDirector::DefaultDirector()->GetWinSize();
	 m_eudemonOpt = new NDUITableLayer;
	 m_eudemonOpt->Initialization();
	 m_eudemonOpt->VisibleSectionTitles(false);
	 m_eudemonOpt->SetFrameRect(CGRectMake((winSize.width / 2) - 30, winSize.height / 2 - 115, 60, TEXT_BTN_HEIGHT * 5));
	 NDDataSource *ds = new NDDataSource;
	 NDSection *sec = new NDSection;
	 
	 // ����
	 NDUIButton *attack = new NDUIButton;
	 attack->Initialization();
	 attack->SetTag(BTN_EUD_ATK);
	 attack->SetTitle("��  ��");
	 attack->SetFocusColor(ccc4(253, 253, 253, 255));
	 sec->AddCell(attack);
	 
	 // ��Ʒ
	 NDUIButton  *btnUseItem = new NDUIButton;
	 btnUseItem->Initialization();
	 btnUseItem->SetTitle("��  Ʒ");
	 btnUseItem->SetTag(BTN_EUD_ITEM);
	 btnUseItem->SetFocusColor(ccc4(253, 253, 253, 255));
	 sec->AddCell(btnUseItem);
	 
	 NDUIButton *skill = new NDUIButton;
	 skill->Initialization();
	 skill->SetTag(BTN_EUD_SKILL);
	 skill->SetTitle("��  ��");
	 skill->SetFocusColor(ccc4(253, 253, 253, 255));
	 sec->AddCell(skill);
	 
	 NDUIButton *def = new NDUIButton;
	 def->Initialization();
	 def->SetTag(BTN_EUD_DEF);
	 def->SetTitle("��  ��");
	 def->SetFocusColor(ccc4(253, 253, 253, 255));
	 sec->AddCell(def);
	 
	 NDUIButton *flee = new NDUIButton;
	 flee->Initialization();
	 flee->SetTag(BTN_EUD_FLEE);
	 flee->SetTitle("��  ��");
	 flee->SetFocusColor(ccc4(253, 253, 253, 255));
	 sec->AddCell(flee);
	 
	 sec->SetFocusOnCell(0);
	 
	 ds->AddSection(sec);
	 m_eudemonOpt->SetDelegate(this);
	 m_eudemonOpt->SetDataSource(ds);
	 }*/
}

/*void Battle::OnTableLayerCellFocused(NDUITableLayer* table, NDUINode* cell, unsigned int cellIndex, NDSection* section)
 {
 if (m_skillOpt == table) {
 // ����˵��
 NDUIMemo* skillMemo = (NDUIMemo*)GetChild(TAG_SKILL_MEMO);
 
 if (m_battleStatus == BS_USER_SKILL_MENU) {
 m_curBattleAction->btAction = BATTLE_ACT_MAG_ATK;
 s_lastTurnActionUser.btAction = BATTLE_ACT_MAG_ATK;
 s_lastTurnActionUser.vData.clear();
 
 SET_BATTLE_SKILL_LIST_IT itSkill = m_setBattleSkillList.begin();
 size_t n = 0;
 for (; n < cellIndex; n++) {
 itSkill++;
 }
 
 if (n < m_setBattleSkillList.size()) {
 m_curBattleAction->vData.push_back(*itSkill);
 s_lastTurnActionUser.vData.push_back(*itSkill);
 s_lastTurnActionUser.vData.push_back(0);
 }
 BattleSkill* skill = BattleMgrObj.GetBattleSkill(*itSkill);
 if (!skill) {
 return;
 }
 
 if (skillMemo) {
 skillMemo->SetText(skill->getSimpleDes(true).c_str());
 }
 } else if (m_battleStatus == BS_EUDEMON_SKILL_MENU) {
 
 NDBattlePet* pet = NDPlayer::defaultHero().battlepet;
 NDAsssert(pet != NULL);
 SET_BATTLE_SKILL_LIST petBattleSkillList = pet->GetSkillList(SKILL_TYPE_ATTACK);
 
 m_curBattleAction->btAction = BATTLE_ACT_PET_MAG_ATK;
 
 s_lastTurnActionEudemon.btAction = BATTLE_ACT_PET_MAG_ATK;
 s_lastTurnActionEudemon.vData.clear();
 
 SET_BATTLE_SKILL_LIST_IT itSkill = petBattleSkillList.begin();
 size_t n = 0;
 for (; n < cellIndex; n++) {
 itSkill++;
 }
 
 if (n < petBattleSkillList.size()) {
 m_curBattleAction->vData.push_back(*itSkill);
 s_lastTurnActionEudemon.vData.push_back(*itSkill);
 s_lastTurnActionEudemon.vData.push_back(0);
 }
 BattleSkill* skill = BattleMgrObj.GetBattleSkill(*itSkill);
 if (!skill) {
 return;
 }
 
 if (skillMemo) {
 skillMemo->SetText(skill->getSimpleDes(true).c_str());
 }
 }
 }
 }*/

void Battle::OnTableLayerCellSelected(NDUITableLayer* table, NDUINode* cell,
		unsigned int cellIndex, NDSection* section)
{
	//	if (table == m_tlQuickTalk) {
	//		if (cell->IsKindOfClass(RUNTIME_CLASS(QuickTalkCell))) {
	//			QuickTalkCell* quickTalk = (QuickTalkCell*)cell;
	//			string strText = quickTalk->GetText();
	//			ChatInput::SendChatDataToServer(ChatTypeSection, strText.c_str());
	//			string speaker = NDPlayer::defaultHero().m_name;
	//			speaker += "��";
	//			Chat::DefaultChat()->AddMessage(ChatTypeSection, strText.c_str(), speaker.c_str());
	//			ShowQuickChat(false);
	//		}
	//	}
}
/*void Battle::OnTableLayerCellSelected(NDUITableLayer* table, NDUINode* cell, unsigned int cellIndex, NDSection* section)
 {	
 if (m_battleOpt == table) {
 switch ((cellIndex + 1))
 {
 case BTN_ATTATCK:
 stopAuto();
 OnBtnAttack();
 break;
 case BTN_FLEE:
 stopAuto();
 OnBtnRun();
 break;
 case BTN_DEFENCE:
 stopAuto();
 OnBtnDefence();
 break;
 case BTN_CATCH:
 stopAuto();
 OnBtnCatch();
 break;
 case BTN_AUTO:
 OnBtnAuto(true);
 break;
 //			case BTN_ITEM:
 //				stopAuto();
 //				OnBtnUseItem(BS_USE_ITEM_MENU);
 //				break;
 case BTN_SKILL:
 stopAuto();
 //OnBtnSkill();
 break;
 default:
 break;
 }
 } else if (m_itemOpt == table) {
 if (m_battleStatus == BS_USE_ITEM_MENU) {
 m_curBattleAction->btAction = BATTLE_ACT_USEITEM;
 m_battleStatus = BS_CHOOSE_OUR_SIDE_USE_ITEM_USER;
 } else {
 m_curBattleAction->btAction = BATTLE_ACT_PET_USEITEM;
 m_battleStatus = BS_CHOOSE_OUR_SIDE_USE_ITEM_EUDEMON;
 }
 
 m_curBattleAction->vData.clear();
 
 MAP_USEITEM_IT it = m_mapUseItem.begin();
 for (unsigned int i = 0; it != m_mapUseItem.end() && i < cellIndex; it++, i++) {
 }
 
 if (it != m_mapUseItem.end()) {
 m_curBattleAction->vData.push_back(it->second.first);
 }
 
 VEC_FIGHTER& ourSideList = GetOurSideList();
 Fighter* f;
 for (size_t i = 0; i < ourSideList.size(); i++) {
 f = ourSideList.at(i);
 if (f->isVisiable()) {
 HighlightFighter(f);
 break;
 }
 }
 
 RemoveChild(m_itemOpt, true);
 m_itemOpt = NULL;
 } else if (m_eudemonOpt == table) {
 switch (cell->GetTag()) {
 case BTN_EUD_ATK:
 OnEudemonAttack();
 break;
 case BTN_EUD_SKILL:
 //OnEudemonSkill();
 break;
 case BTN_EUD_DEF:
 {
 BattleAction actioin(BATTLE_ACT_PET_PHY_DEF);
 s_lastTurnActionEudemon.btAction = BATTLE_ACT_PET_PHY_DEF;
 SendBattleAction(actioin);
 }
 break;
 case BTN_EUD_FLEE:
 {
 BattleAction actioin(BATTLE_ACT_PET_ESCAPE);
 SendBattleAction(actioin);
 }
 break;
 //			case BTN_EUD_ITEM:
 //				OnBtnUseItem(BS_EUDEMON_USE_ITEM_MENU);
 //				break;
 default:
 break;
 }
 } else if (m_skillOpt == table) {
 m_curBattleAction->vData.clear();
 
 if (m_battleStatus == BS_USER_SKILL_MENU) {
 m_curBattleAction->btAction = BATTLE_ACT_MAG_ATK;
 s_lastTurnActionUser.btAction = BATTLE_ACT_MAG_ATK;
 s_lastTurnActionUser.vData.clear();
 
 SET_BATTLE_SKILL_LIST_IT itSkill = m_setBattleSkillList.begin();
 size_t n = 0;
 for (; n < cellIndex; n++) {
 itSkill++;
 }
 
 if (n < m_setBattleSkillList.size()) {
 m_curBattleAction->vData.push_back(*itSkill);
 s_lastTurnActionUser.vData.push_back(*itSkill);
 s_lastTurnActionUser.vData.push_back(0);
 }
 BattleSkill* skill = BattleMgrObj.GetBattleSkill(*itSkill);
 if (!skill) {
 return;
 }
 
 int targetType = skill->getAtkType();
 GetMainUser()->setUseSkill(skill);
 
 if ((targetType & SKILL_ATK_TYPE_ENEMY) == SKILL_ATK_TYPE_ENEMY) {
 
 m_battleStatus = BS_CHOOSE_ENEMY_MAG_ATK;
 
 VEC_FIGHTER& enemyList = GetEnemySideList();
 Fighter* f;
 for (size_t i = 0; i < enemyList.size(); i++) {
 f = enemyList.at(i);
 if (f->isVisiable()) {
 HighlightFighter(f);
 break;
 }
 }
 
 } else if ((targetType & SKILL_ATK_TYPE_FRIEND) == SKILL_ATK_TYPE_FRIEND) {
 
 m_battleStatus = BS_CHOOSE_OUR_SIDE_MAG_ATK;
 
 VEC_FIGHTER& ourList = GetOurSideList();
 Fighter* f;
 for (size_t i = 0; i < ourList.size(); i++) {
 f = ourList.at(i);
 if (f->isVisiable()) {
 HighlightFighter(f);
 break;
 }
 }
 
 } else if ((targetType & SKILL_ATK_TYPE_SELF) == SKILL_ATK_TYPE_SELF) {
 m_curBattleAction->vData.push_back(GetMainUser()->m_kInfo.idObj);
 SendBattleAction(*m_curBattleAction);
 
 }
 } else if (m_battleStatus == BS_EUDEMON_SKILL_MENU) {
 
 NDBattlePet* pet = NDPlayer::defaultHero().battlepet;
 NDAsssert(pet != NULL);
 SET_BATTLE_SKILL_LIST petBattleSkillList = pet->GetSkillList(SKILL_TYPE_ATTACK);
 
 m_curBattleAction->btAction = BATTLE_ACT_PET_MAG_ATK;
 
 s_lastTurnActionEudemon.btAction = BATTLE_ACT_PET_MAG_ATK;
 s_lastTurnActionEudemon.vData.clear();
 
 SET_BATTLE_SKILL_LIST_IT itSkill = petBattleSkillList.begin();
 size_t n = 0;
 for (; n < cellIndex; n++) {
 itSkill++;
 }
 
 if (n < petBattleSkillList.size()) {
 m_curBattleAction->vData.push_back(*itSkill);
 s_lastTurnActionEudemon.vData.push_back(*itSkill);
 s_lastTurnActionEudemon.vData.push_back(0);
 }
 BattleSkill* skill = BattleMgrObj.GetBattleSkill(*itSkill);
 if (!skill) {
 return;
 }
 
 // ����mp������ʾ
 if (getMainEudemon()->m_kInfo.nMana < skill->getMpRequire()) {
 m_dlgHint = new NDUIDialog;
 m_dlgHint->Initialization();
 m_dlgHint->SetDelegate(this);
 m_dlgHint->Show("��������", "���ı���������������", NULL, NULL);
 } else {
 int targetType = skill->getAtkType();
 
 if ((targetType & SKILL_ATK_TYPE_ENEMY) == SKILL_ATK_TYPE_ENEMY) {
 
 m_battleStatus = BS_CHOOSE_ENEMY_MAG_ATK_EUDEMON;
 
 VEC_FIGHTER& enemyList = GetEnemySideList();
 Fighter* f;
 for (size_t i = 0; i < enemyList.size(); i++) {
 f = enemyList.at(i);
 if (f->isVisiable()) {
 HighlightFighter(f);
 break;
 }
 }
 
 } else if ((targetType & SKILL_ATK_TYPE_FRIEND) == SKILL_ATK_TYPE_FRIEND) {
 
 m_battleStatus = BS_CHOOSE_OUR_SIDE_MAG_ATK_EUDEMON;
 
 VEC_FIGHTER& ourList = GetOurSideList();
 Fighter* f;
 for (size_t i = 0; i < ourList.size(); i++) {
 f = ourList.at(i);
 if (f->isVisiable()) {
 HighlightFighter(f);
 break;
 }
 }
 
 } else if ((targetType & SKILL_ATK_TYPE_SELF) == SKILL_ATK_TYPE_SELF) {
 m_curBattleAction->vData.push_back(getMainEudemon()->m_kInfo.idObj);
 SendBattleAction(*m_curBattleAction);
 
 }
 }
 }
 RemoveChild(m_skillOpt, true);
 m_skillOpt = NULL;
 RemoveChild(TAG_SKILL_MEMO, true);
 }
 }*/

void Battle::processBattleSkillList(NDTransData& data, int len)
{
	//	m_setBattleSkillList.clear();
	//	int nCount = data.ReadShort();
	//	for (int i = 0; i < nCount; i++) {
	//		m_setBattleSkillList.insert(data.ReadInt());
	//	}
	//	
	//	if (s_bAuto) {
	//		RefreshSkillBar();
	//		m_fighterLeft->SetGray(true);
	//	}
}

/*void Battle::OnBtnSkill()
 {
 m_battleStatus = BS_USER_SKILL_MENU;
 RemoveChild(m_battleOpt, false);
 
 // û�м���
 if (m_setBattleSkillList.size() <= 0) {
 m_dlgHint = new NDUIDialog;
 m_dlgHint->Initialization();
 m_dlgHint->SetDelegate(this);
 m_dlgHint->Show(NULL, "���غ��޿��ü���", NULL, NULL);
 return;
 }
 
 // �򿪼����б�
 NDAsssert (m_skillOpt == NULL);
 
 CGSize winSize = NDDirector::DefaultDirector()->GetWinSize();
 int height = 0;
 
 m_skillOpt = new NDUITableLayer;
 m_skillOpt->Initialization();
 m_skillOpt->VisibleSectionTitles(false);
 
 NDDataSource *ds = new NDDataSource;
 NDSection *sec = new NDSection;
 
 BattleMgr& bm = BattleMgrObj;
 BattleSkill* bs = NULL;
 
 SET_BATTLE_SKILL_LIST_IT itSkill = m_setBattleSkillList.begin();
 BattleSkill* firstSkill = bm.GetBattleSkill(*itSkill);
 for (; itSkill != m_setBattleSkillList.end(); itSkill++) {
 bs = bm.GetBattleSkill(*itSkill);
 if (!bs) {
 continue;
 }
 height += TEXT_BTN_HEIGHT;
 
 NDUIButton *skill = new NDUIButton;
 skill->Initialization();
 skill->SetFocusColor(ccc4(253, 253, 253, 255));
 skill->SetTitle(bs->getName().c_str());
 sec->AddCell(skill);
 }
 
 if (height > 120) {
 height = 120;
 }
 
 sec->SetFocusOnCell(0);
 
 ds->AddSection(sec);
 m_skillOpt->SetDelegate(this);
 m_skillOpt->SetDataSource(ds);
 m_skillOpt->SetFrameRect(CGRectMake((winSize.width / 2) - 80, winSize.height / 2, 160, height));
 AddChild(m_skillOpt);
 
 // ����˵��
 NDUIMemo* skillMemo = new NDUIMemo();
 skillMemo->Initialization();
 skillMemo->SetTag(TAG_SKILL_MEMO);
 skillMemo->SetBackgroundColor(ccc4(228, 219, 169, 255));
 skillMemo->SetText(firstSkill->getSimpleDes(true).c_str());
 skillMemo->SetFrameRect(CGRectMake((winSize.width / 2) - 80, 30, 160, winSize.height / 2 - 35));
 AddChild(skillMemo);
 }*/

/*void Battle::OnBtnUseItem(BATTLE_STATUS bs)
 {
 m_battleStatus = bs;
 if (m_battleOpt) 
 {
 RemoveChild(m_battleOpt, false);
 }	
 if (m_eudemonOpt) 
 {
 RemoveChild(m_eudemonOpt, false);
 }
 
 m_mapUseItem.clear();
 
 ItemMgr& itemsObj = ItemMgrObj;
 
 vector<Item*> vItemList;
 itemsObj.GetBattleUsableItem(vItemList);
 
 if (vItemList.size() == 0) {
 return;
 }
 
 MAP_USEITEM_IT itUseItem;
 Item* item = NULL;
 for (vector<Item*>::iterator it = vItemList.begin(); it != vItemList.end(); it++) {
 item = *it;
 if (!item) {
 continue;
 }
 
 itUseItem = m_mapUseItem.find(item->iItemType);
 if (itUseItem != m_mapUseItem.end()) {
 itUseItem->second.second += item->iAmount;
 } else {
 m_mapUseItem[item->iItemType] = make_pair(item->iID, item->iAmount);
 }
 }
 
 CGSize winSize = NDDirector::DefaultDirector()->GetWinSize();
 int height = 0;
 
 m_itemOpt = new NDUITableLayer;
 m_itemOpt->Initialization();
 m_itemOpt->VisibleSectionTitles(false);
 
 NDDataSource *ds = new NDDataSource;
 NDSection *sec = new NDSection;
 
 NDItemType* itemType = NULL;
 char szItem[256] = "";
 for (itUseItem = m_mapUseItem.begin(); itUseItem != m_mapUseItem.end(); itUseItem++) {
 itemType = itemsObj.QueryItemType(itUseItem->first);
 if (!itemType) {
 continue;
 }
 
 sprintf(szItem, "%s X %d", itemType->m_name.c_str(), itUseItem->second.second);
 
 height += TEXT_BTN_HEIGHT;
 
 NDUIButton *item = new NDUIButton;
 item->Initialization();
 item->SetFocusColor(ccc4(253, 253, 253, 255));
 item->SetTitle(szItem);
 sec->AddCell(item);
 }
 
 if (height > 210) {
 height = 210;
 }
 
 sec->SetFocusOnCell(0);
 
 ds->AddSection(sec);
 m_itemOpt->SetDelegate(this);
 m_itemOpt->SetDataSource(ds);
 m_itemOpt->SetFrameRect(CGRectMake((winSize.width / 2) - 80, winSize.height / 2 - 115, 160, height));
 AddChild(m_itemOpt);
 }*/

void Battle::OnBtnCatch()
{
	//m_battleOpt->RemoveFromParent(false);
//	setBattleStatus(BS_CHOOSE_ENEMY_CATCH);
//	
//	if (ItemMgrObj.IsBagFull()) {
//		Chat::DefaultChat()->AddMessage(ChatTypeSystem,NDCommonCString("BagFullCantCatch"));
//		return;
//	}
//	
//	VEC_FIGHTER& enemyList = GetEnemySideList();
//	Fighter* f;
//	for (size_t i = 0; i < enemyList.size(); i++) {
//		f = enemyList.at(i);
//		if (f->isCatchable()) {
//			HighlightFighter(f);
//			return;
//		}
//	}
//	
//	Chat::DefaultChat()->AddMessage(ChatTypeSystem, NDCommonCString("CantCatch"));
}

void Battle::OnBtnDefence()
{
	//m_battleOpt->RemoveFromParent(false);

	ms_kLastTurnActionUser.btAction = BATTLE_ACT_PHY_DEF;

	BattleAction actioin(BATTLE_ACT_PHY_DEF);
	SendBattleAction(actioin);
}

void Battle::OnBtnRun()
{
	//m_battleOpt->RemoveFromParent(false);

	BattleAction actioin(BATTLE_ACT_ESCAPE);
	SendBattleAction(actioin);
}

/*void Battle::OnEudemonSkill()
 {
 m_battleStatus = BS_EUDEMON_SKILL_MENU;
 RemoveChild(m_eudemonOpt, false);
 
 // ��ȡ���＼��
 NDBattlePet* pet = NDPlayer::defaultHero().battlepet;
 if (!pet) {
 return;
 }
 
 SET_BATTLE_SKILL_LIST& petSkillList = pet->GetSkillList(SKILL_TYPE_ATTACK);
 if (petSkillList.size() <= 0) {
 m_dlgHint = new NDUIDialog;
 m_dlgHint->Initialization();
 m_dlgHint->SetDelegate(this);
 m_dlgHint->Show(NULL, "���غ��޿��ü���", NULL, NULL);
 return;
 }
 
 // �򿪼����б�
 NDAsssert (m_skillOpt == NULL);
 
 CGSize winSize = NDDirector::DefaultDirector()->GetWinSize();
 int height = 0;
 
 m_skillOpt = new NDUITableLayer;
 m_skillOpt->Initialization();
 m_skillOpt->VisibleSectionTitles(false);
 
 NDDataSource *ds = new NDDataSource;
 NDSection *sec = new NDSection;
 
 BattleMgr& bm = BattleMgrObj;
 BattleSkill* bs = NULL;
 
 SET_BATTLE_SKILL_LIST_IT itSkill = petSkillList.begin();
 BattleSkill* firstSkill = bm.GetBattleSkill(*itSkill);
 for (; itSkill != petSkillList.end(); itSkill++) {
 bs = bm.GetBattleSkill(*itSkill);
 if (!bs) {
 continue;
 }
 height += TEXT_BTN_HEIGHT;
 
 NDUIButton *skill = new NDUIButton;
 skill->Initialization();
 skill->SetFocusColor(ccc4(253, 253, 253, 255));
 skill->SetTitle(bs->getName().c_str());
 sec->AddCell(skill);
 }
 
 sec->SetFocusOnCell(0);
 
 if (height > 120) {
 height = 120;
 }
 
 ds->AddSection(sec);
 m_skillOpt->SetDelegate(this);
 m_skillOpt->SetDataSource(ds);
 m_skillOpt->SetFrameRect(CGRectMake((winSize.width / 2) - 80, winSize.height / 2, 160, height));
 AddChild(m_skillOpt);
 
 // ����˵��
 NDUIMemo* skillMemo = new NDUIMemo();
 skillMemo->Initialization();
 skillMemo->SetTag(TAG_SKILL_MEMO);
 skillMemo->SetBackgroundColor(ccc4(228, 219, 169, 255));
 skillMemo->SetText(firstSkill->getSimpleDes(true).c_str());
 skillMemo->SetFrameRect(CGRectMake((winSize.width / 2) - 80, 30, 160, winSize.height / 2 - 35));
 AddChild(skillMemo);
 }*/

void Battle::OnEudemonAttack()
{
//	if (BS_CHOOSE_ENEMY_PHY_ATK_EUDEMON == m_battleStatus) {
//		return;
//	}
//	
//	//m_eudemonOpt->RemoveFromParent(false);
//	setBattleStatus(BS_CHOOSE_ENEMY_PHY_ATK_EUDEMON);
//	//	m_fighterRight->SetFoucusByIndex(3);
//	//	m_fighterLeft->defocus();
//	//	m_fighterBottom->defocus();
//	
//	m_curBattleAction->btAction = BATTLE_ACT_PET_PHY_ATK;
//	m_curBattleAction->vData.clear();
//	
//	VEC_FIGHTER& enemyList = GetEnemySideList();
//	
//	Fighter* f;
//	for (size_t i = 0; i < enemyList.size(); i++) {
//		f = enemyList.at(i);
//		if (f->isVisiable()) {
//			s_lastTurnActionEudemon.btAction = BATTLE_ACT_PET_PHY_ATK;
//			s_lastTurnActionEudemon.vData.clear();
//			HighlightFighter(f);
//			return;
//		}
//	}
}

void Battle::OnBtnAttack()
{
//	if (m_battleStatus == BS_CHOOSE_ENEMY_PHY_ATK) {
//		return;
//	}
//	
//	//m_battleOpt->RemoveFromParent(false);
//	setBattleStatus(BS_CHOOSE_ENEMY_PHY_ATK);
//	//	m_fighterRight->SetFoucusByIndex(3);
//	//	m_fighterLeft->defocus();
//	//	m_fighterBottom->defocus();
//	
//	m_curBattleAction->btAction = BATTLE_ACT_PHY_ATK;
//	m_curBattleAction->vData.clear();
//	
//	VEC_FIGHTER& enemyList = GetEnemySideList();
//	
//	Fighter* f;
//	for (size_t i = 0; i < enemyList.size(); i++) {
//		f = enemyList.at(i);
//		if (f->isVisiable()) {
//			s_lastTurnActionUser.btAction = BATTLE_ACT_PHY_ATK;
//			HighlightFighter(f);
//			return;
//		}
//	}
}

void Battle::stopAuto()
{
	if (ms_bAuto)
	{
		//		m_timer.KillTimer(this, TIMER_AUTOCOUNT);
		//		RemoveChild(m_lbAuto, false);
		//		RemoveChild(m_imgAutoCount, false);
		//		SAFE_DELETE(m_lbAuto);
		//		SAFE_DELETE(m_imgAutoCount);
		//		m_autoCount = AUTO_COUNT;
		ms_bAuto = false;
		//		m_btnAuto->SetImage(m_picAuto);
	}
}

void Battle::SetAutoCount()
{
	//	m_lbAuto = new NDUILabel;
	//	m_lbAuto->Initialization();
	//	m_lbAuto->SetText("�Զ�");
	//	m_lbAuto->SetFontColor(ccc4(255, 255, 255, 255));
	//	CGSize sizeText = getStringSize("�Զ�", 15);
	//	m_lbAuto->SetFrameRect(CGRectMake(2, 2, sizeText.width, sizeText.height));
	//	AddChild(m_lbAuto);
	//	
	//	m_imgAutoCount = new ImageNumber();
	//	m_imgAutoCount->Initialization();
	//	m_imgAutoCount->SetBigRedNumber(m_autoCount, false);
	//	m_imgAutoCount->SetFrameRect(CGRectMake(40, 2, m_imgAutoCount->GetNumberSize().width, m_imgAutoCount->GetNumberSize().height));
	//	AddChild(m_imgAutoCount);
	//	
	//	m_timer.SetTimer(this, TIMER_AUTOCOUNT, 1);
}

void Battle::OnBtnAuto(bool bSendAction)
{
	//RemoveChild(m_battleOpt, false);

	//CreateCancleAutoFightButton();

	m_autoCount = 0;

	if (!ms_bAuto)
	{
		ms_bAuto = true;
		//SetAutoCount();
		//		m_btnAuto->SetImage(m_picAutoCancel);
		//		m_fighterRight->SetShrink(true);
		//		m_fighterLeft->SetShrink(true);
		//		m_fighterBottom->SetShrink(true);
	}

	if (!bSendAction)
	{
		return;
	}

	if (!m_bSendCurTurnUserAction && GetMainUser()
			&& GetMainUser()->isAlive())
	{
		bool bUserActionSend = false;
		switch (ms_kLastTurnActionUser.btAction)
		{
		case BATTLE_ACT_PHY_ATK:
			ms_kLastTurnActionUser.vData.clear();
			ms_kLastTurnActionUser.vData.push_back(0);
			break;
		case BATTLE_ACT_PHY_DEF:
			ms_kLastTurnActionUser.vData.clear();
			break;
		case BATTLE_ACT_MAG_ATK:
		{
			if (m_setBattleSkillList.count(
					ms_kLastTurnActionUser.vData.at(0)) == 0)
			{ // �ü��ܱ��غ��Ѳ���ʹ��
				BattleAction atk(BATTLE_ACT_PHY_ATK);
				atk.vData.push_back(0);
				SendBattleAction(atk);
				bUserActionSend = true;
			}
		}
			break;
		default:
			ms_kLastTurnActionUser.btAction = BATTLE_ACT_PHY_ATK;
			ms_kLastTurnActionUser.vData.clear();
			ms_kLastTurnActionUser.vData.push_back(0);
			break;
		}

		if (!bUserActionSend)
		{
			SendBattleAction(ms_kLastTurnActionUser);
		}
	}

	// �����Զ�ս��
	if (m_mainEudemon && m_mainEudemon->isAlive()
			&& !m_mainEudemon->isEscape())
	{
		switch (ms_kLastTurnActionEudemon.btAction)
		{
		case BATTLE_ACT_PET_PHY_DEF:
			ms_kLastTurnActionEudemon.vData.clear();
			break;
		case BATTLE_ACT_PET_MAG_ATK:
			break;
		default:
			ms_kLastTurnActionEudemon.btAction = BATTLE_ACT_PET_PHY_ATK;
			ms_kLastTurnActionEudemon.vData.clear();
			ms_kLastTurnActionEudemon.vData.push_back(0);
			break;
		}
		SendBattleAction(ms_kLastTurnActionEudemon);
	}
}

void Battle::HighlightFighter(Fighter* f)
{
	m_highlightFighter = f;

	NDUILabel *name = (NDUILabel*) GetChild(TAG_NAME);
	NDUIImage *lingpai = (NDUIImage*) GetChild(TAG_LINGPAI);

	NDBaseRole* role = f->GetRole();
	CGPoint pt = CGPointMake(f->getX(), f->getY());

	if (!lingpai)
	{
		//		lingpai = new NDUIImage;
		//		lingpai->Initialization();
		//		lingpai->SetTag(TAG_LINGPAI);
		//		lingpai->SetPicture(m_picBoji);
		//		AddChild(lingpai);
	}

	//	if (m_picBoji) m_picBoji->SetReverse(f->GetGroup() == BATTLE_GROUP_DEFENCE);
	//	
	//	CGRect rect = CGRectMake(f->getX() + role->GetWidth() / 2 + 5,
	//							 f->getY() - 15,
	//							 m_picBoji->GetSize().width,
	//							 m_picBoji->GetSize().height);
	//							 
	//	if (f->GetGroup() == BATTLE_GROUP_DEFENCE) {
	//		rect.origin.x = rect.origin.x - role->GetWidth() - 10;
	//	}
	//	
	//	lingpai->SetFrameRect(rect);

	//CGRect frameLingpai = lingpai->GetFrameRect();
	if (!name)
	{
		name = new NDUILabel;
		name->SetFontColor(ccc4(255, 255, 255, 255));
		name->Initialization();
		name->SetTag(TAG_NAME);
		AddChild(name);
	}
	std::stringstream ss;
	ss << role->m_strName << "Lv" << role->m_nLevel;
	//	if (f->m_kInfo.fighterType == Fighter_TYPE_RARE_MONSTER)
	//	{
	//		ss << "��" << NDCommonCString("xiyou") << "��"; 
	//	}
	name->SetText(ss.str().c_str());
	CGSize sizeName = getStringSize(ss.str().c_str(), 15);
	name->SetFrameRect(
			CGRectMake(pt.x - sizeName.width / 2,
					pt.y - role->getGravityY() - sizeName.height,
					sizeName.width, sizeName.height));

	setWillBeAtk(getHighlightList());

	//f->setWillBeAtk(true);	
}

void Battle::OnTimer(OBJID tag)
{
	NDUILayer::OnTimer(tag);

	if (tag == TIMER_TIMELEFT)
	{
		if (m_timeLeft > 0)
		{
			if (m_bWatch)
			{
				if (m_battleStatus != BS_SHOW_FIGHT
						&& m_battleStatus != BS_FIGHTER_SHOW_PAS)
				{
					m_timeLeft--;
					stringstream ss;
					ss << m_timeLeft;
					//					m_lbTimer->SetText(ss.str().c_str());
					//m_imgTimer->SetBigRedNumber(m_timeLeft, false);
				}
			}
			else
			{
				if (IsUserOperating() || IsEudemonOperating())
				{
					m_timeLeft--;
					stringstream ss;
					ss << m_timeLeft;
					//					m_lbTimer->SetText(ss.str().c_str());
					//m_imgTimer->SetBigRedNumber(m_timeLeft, false);
				}
			}
		}
	}
	else if (tag == TIMER_BACKTOGAME)
	{
		BattleMgrObj.quitBattle();
	}
	//	else if (tag == TIMER_AUTOCOUNT) 
	//	{
	//		if (m_autoCount > 0) {
	//			m_autoCount--;
	//			m_imgAutoCount->SetBigRedNumber(m_autoCount, false);
	//			if (m_autoCount == 0) {
	//				OnBtnAuto();
	//			}
	//		}
	//	}
	else if (tag == TIMER_AUTOFIGHT)
	{
		OnBtnAuto(true);
		m_timer.KillTimer(this, TIMER_AUTOFIGHT);
	}
}

void Battle::Init()
{
	// ����ս��״̬
	int sceneMapId;
	int sceneCenterX;
	int sceneCenterY;
	NDPlayer::defaultHero().UpdateState(USERSTATE_FIGHTING, true);
//	dieAniGroup=NULL;
	m_bChatTextFieldShouldShow = false;
	//	m_imgTurn = NULL;
	//	m_imgTimer = NULL;
	//	m_imgQuickTalkBg = NULL;
	//	m_tlQuickTalk = NULL;
	m_orignalMapId = 0;
	m_nLastSkillPageUser = 0;
	m_nLastSkillPageEudemon = 0;
	//m_chatDelegate = [[ChatTextFieldDelegate alloc] init];
	m_bShowChatTextField = false;
	//	m_imgChat = NULL;
	//	m_btnSendChat = NULL;
	m_mapLayer = NULL;
	m_bSendCurTurnUserAction = false;

	//m_btnCancleAutoFight = NULL;

	m_dlgStatus = NULL;
	theActor = NULL;
	theTarget = NULL;
	m_bWatch = false;
	//m_battleOpt = NULL;
	//	m_playerHead = NULL;
	//	m_petHead = NULL;
	m_battleBg = NULL;
	//m_eudemonOpt = NULL;
	m_picActionWordDef = new NDPicture;
	m_picActionWordDef->Initialization(NDPath::GetImgPath("actionWord.png").c_str());
	m_picActionWordDef->Cut(CGRectMake(0.0f, 0.0f, 37.0f, 18.0f));
	m_picActionWordFlee = new NDPicture;
	m_picActionWordDodge = new NDPicture;
	m_picActionWordDodge->Initialization(NDPath::GetImgPath("actionWord.png").c_str());
	m_picActionWordDodge->Cut(CGRectMake(0.0f, 18.0f, 37.0f, 18.0f));
	m_picActionWordFlee->Initialization(NDPath::GetImgPath("actionWord.png").c_str());
	m_picActionWordFlee->Cut(CGRectMake(0.0f, 36.0f, 37.0f, 18.0f));

	//	m_picTalk = NULL;
	//	m_picQuickTalk = NULL;
	//	m_picAuto = NULL;
	//	m_picAutoCancel = NULL;
	//	m_picLeave = NULL;
	//	
	//	m_btnTalk = NULL;
	//	m_btnQuickTalk = NULL;
	//	m_layerBtnQuitTalk = NULL;
	//	m_btnAuto = NULL;
	//	m_btnLeave = NULL;

	m_curBattleAction = new BattleAction(BATTLE_ACT_PHY_ATK);
	//	m_dlgHint = NULL;
	//	m_itemOpt = NULL;
	//	m_skillOpt = NULL;
	m_autoCount = AUTO_COUNT;
	//m_lbAuto = NULL;
	//m_imgAutoCount = NULL;
	bActionSet = false;
	m_dlgBattleResult = NULL;
	m_mainEudemon = NULL;
	serverBattleResult = 2;
	battleCompleteResult = BATTLE_COMPLETE_NO;
	m_battleType = BATTLE_TYPE_NONE;
	m_ourGroup = BATTLE_GROUP_ATTACK;
	m_turn = 0;
	//	m_lbTurnTitle = NULL;
	//	m_lbTurn = NULL;
	//	m_lbTimerTitle = NULL;
	//	m_lbTimer = NULL;
	//m_imgTurn = NULL;
	m_timeLeft = 30;
	//	m_picWhoAmI = NULL;
	//	m_imgWhoAmI = NULL;
	m_mainFighter = NULL;
	m_battleStatus = BS_BATTLE_WAIT;
	//	m_picLingPai = NULL;
	m_highlightFighter = NULL;
	m_timeLeftMax = 29;
	m_actionFighterPoint = 0;
	m_foreBattleStatus = 0;
	watchBattle = false;

	m_picBaoJi = new NDPicture;
	m_picBaoJi->Initialization(NDPath::GetImgPath("bo.png").c_str());
	//	
	//	m_picBoji = new NDPicture;
	//	m_picBoji->Initialization(GetImgPath("boji.png"));

	m_defaultActionUser = BATTLE_ACT_PHY_ATK;
	m_pkDefaultTargetUser = NULL;
	m_nDefaultSkillID = ID_NONE;

	m_defaultActionEudemon = BATTLE_ACT_PET_PHY_ATK;
	m_defaultTargetEudemon = NULL;
	m_defaultSkillIDEudemon = ID_NONE;

	m_bTurnStart = true;
	m_bTurnStartPet = false;

	// ս�������
	//	m_fighterBottom = NULL;
	//	m_fighterLeft = NULL;
	//	m_fighterRight = NULL;

	m_bShrinkRight = false;
	m_bShrinkLeft = false;
	m_bShrinkBottom = false;
}

Fighter* Battle::GetTouchedFighter(VEC_FIGHTER& fighterList, CGPoint pt)
{
	VEC_FIGHTER_IT itBegin = fighterList.begin();
	VEC_FIGHTER_IT itEnd = fighterList.end();

	Fighter* pkFighter;
	for (; itBegin != itEnd; itBegin++)
	{
		pkFighter = (*itBegin);
		if (!(pkFighter->isVisiable() && pkFighter->isVisibleStatus))
		{
			continue;
		}

		NDBaseRole* pkRole = pkFighter->GetRole();
		CGPoint fPos = pkRole->GetPosition();

		int w = pkRole->GetWidth();
		int h = pkRole->GetHeight();

		fPos.x -= (w >> 1);
		fPos.y -= h;

		if (IsPointInside(pt, CGRectMake(fPos.x, fPos.y, w, h)))
		{
			return pkFighter;
		}
	}

	return NULL;
}

bool Battle::TouchEnd(NDTouch* touch)
{
	/***
	 * ��ʱ��ע�� ����
	 * begin
	 */
// 	if (m_dlgStatus)
// 	{
// 		CloseStatusDlg();
// 	}
// 	
// 	if (touch && touch->GetLocation().x > 0.0001f && touch->GetLocation().y > 0.0001f) {
// 		if (NDUILayer::TouchEnd(touch)) {
// 			return false;
// 		}
// 	}
// 	
// 	Fighter* f = GetTouchedFighter(GetOurSideList(), touch->GetLocation());
// 	if(!f){
// 		f=GetTouchedFighter(GetEnemySideList(), touch->GetLocation());
// 	}
// 	
// 	if(f)
// 	{
// 		NDLog("touch fighter");
// 		if(currentShowFighter>0)
// 		{
// 			ScriptMgrObj.excuteLuaFunc("CloseFighterInfo","FighterInfo",0);
// 		}
// 		currentShowFighter=f->m_kInfo.idObj;
// 		
// 		int idType=f->m_kInfo.idType;
// 		int skillId=-1;
// 		if(f->m_kInfo.fighterType==FIGHTER_TYPE_PET)
// 		{
// 			skillId=ScriptDBObj.GetN("pet_config",idType,DB_PET_CONFIG_SKILL);
// 		}else if(f->m_kInfo.fighterType==FIGHTER_TYPE_MONSTER)
// 		{
// 			skillId=ScriptDBObj.GetN("monstertype",idType,DB_MONSTERTYPE_SKILL);
// 		}
// 		std::string skillName=ScriptDBObj.GetS("skill_config",skillId,DB_SKILL_CONFIG_NAME);
// 		ScriptMgrObj.excuteLuaFunc("LoadUI", "FighterInfo",f->getOriginX(),f->getOriginY());
// 		ScriptMgrObj.excuteLuaFunc<bool>("SetFighterInfo","FighterInfo",f->GetRole()->m_name,skillName);
// 		ScriptMgrObj.excuteLuaFunc("UpdateHp","FighterInfo",f->m_kInfo.nLife,f->m_kInfo.nLifeMax);
// 		ScriptMgrObj.excuteLuaFunc("UpdateMp","FighterInfo",f->m_kInfo.nMana,f->m_kInfo.nManaMax);
// 	}
	/***
	 * ��ʱ��ע�� ����
	 * end
	 */

//	switch (m_battleStatus)
//	{
//		case BS_CHOOSE_VIEW_FIGHTER_STATUS:
//		case BS_CHOOSE_VIEW_FIGHTER_STATUS_PET:
//		{
//			Fighter* f = GetTouchedFighter(GetEnemySideList(), touch->GetLocation());
//			if (!f) {
//				f = GetTouchedFighter(GetOurSideList(), touch->GetLocation());
//			}
//			
//			if (f) {
//				if (m_highlightFighter != f) {
//					HighlightFighter(f);
//					break;
//				}
//			}
//			
//			if (m_highlightFighter) {
//				m_dlgStatus = new StatusDialog;
//				m_dlgStatus->Initialization(m_highlightFighter);
//				m_dlgStatus->SetFrameRect(CGRectMake(0, 0, NDDirector::DefaultDirector()->GetWinSize().width,
//													 NDDirector::DefaultDirector()->GetWinSize().height));
//				AddChild(m_dlgStatus);
//			}
//		}
//			break;
//		case BS_CHOOSE_ENEMY_PHY_ATK:
//		case BS_CHOOSE_ENEMY_PHY_ATK_EUDEMON:
//		case BS_CHOOSE_ENEMY_MAG_ATK:
//		case BS_CHOOSE_ENEMY_MAG_ATK_EUDEMON:
//		{
//			Fighter* f = GetTouchedFighter(GetEnemySideList(), touch->GetLocation());
//			if (f)
//			{
//				if (m_highlightFighter == f)
//				{
//					// ���͹���ָ��
//					m_curBattleAction->vData.push_back(m_highlightFighter->m_kInfo.idObj);
//					SendBattleAction(*m_curBattleAction);
//				}
//				else
//				{
//					if (m_battleStatus == BS_CHOOSE_ENEMY_PHY_ATK || m_battleStatus == BS_CHOOSE_ENEMY_MAG_ATK) {
//						m_defaultTargetUser = f;
//					} else {
//						m_defaultTargetEudemon = f;
//					}
//					HighlightFighter(f);
//				}
//			} else {
//				if (m_highlightFighter) {
//					// ���͹���ָ��
//					m_curBattleAction->vData.push_back(m_highlightFighter->m_kInfo.idObj);
//					SendBattleAction(*m_curBattleAction);
//				}
//			}
//			
//		}
//			break;
//		case BS_CHOOSE_ENEMY_CATCH:
//		{
//			Fighter* f = GetTouchedFighter(GetEnemySideList(), touch->GetLocation());
//			if (f)
//			{
//				if (f->isCatchable()) {
//					// ���Ͳ�׽ָ��
//					BattleAction actioin(BATTLE_ACT_CATCH);
//					actioin.vData.push_back(f->m_kInfo.idObj);
//					SendBattleAction(actioin);
//				} else {
//					Chat::DefaultChat()->AddMessage(ChatTypeSystem, NDCommonCString("DestCantCatch"));
//				}
//				
//			} else {
//				if (m_highlightFighter) {
//					// ���Ͳ�׽ָ��
//					BattleAction actioin(BATTLE_ACT_CATCH);
//					actioin.vData.push_back(m_highlightFighter->m_kInfo.idObj);
//					SendBattleAction(actioin);
//				}
//			}
//			
//		}
//			break;
//		case BS_CHOOSE_OUR_SIDE_USE_ITEM_USER:
//		case BS_CHOOSE_OUR_SIDE_USE_ITEM_EUDEMON:
//		case BS_CHOOSE_OUR_SIDE_MAG_ATK:
//		case BS_CHOOSE_OUR_SIDE_MAG_ATK_EUDEMON:
//		{
//			Fighter* f = GetTouchedFighter(GetOurSideList(), touch->GetLocation());
//			if (f)
//			{
//				if (m_highlightFighter == f)
//				{
//					m_curBattleAction->vData.push_back(m_highlightFighter->m_kInfo.idObj);
//					SendBattleAction(*m_curBattleAction);
//				}
//				else
//				{
//					if (m_battleStatus == BS_CHOOSE_OUR_SIDE_MAG_ATK) {
//						m_defaultTargetUser = f;
//					} else if (m_battleStatus == BS_CHOOSE_OUR_SIDE_MAG_ATK_EUDEMON) {
//						m_defaultTargetEudemon = f;
//					}
//					HighlightFighter(f);
//				}
//			} else {
//				if (m_highlightFighter) {
//					m_curBattleAction->vData.push_back(m_highlightFighter->m_kInfo.idObj);
//					SendBattleAction(*m_curBattleAction);
//				}
//			}
//		}
//			break;
//		case BS_CHOOSE_SELF_MAG_ATK:
//		case BS_CHOOSE_SELF_MAG_ATK_EUDEMON:
//			if (m_highlightFighter) {
//				m_curBattleAction->vData.push_back(m_highlightFighter->m_kInfo.idObj);
//				SendBattleAction(*m_curBattleAction);
//			}
//		default:
//			break;
//	}
	return true;
}

void Battle::SendBattleAction(const BattleAction& action)
{
	// �Ƴ����ƺ�������ʾ������ȴ���ʾ
	clearHighlight();

	if (!GetChild(TAG_WAITING) && m_mainFighter)
	{
		CGPoint pt = m_mainFighter->GetRole()->GetPosition();
		NDUILabel* waiting = new NDUILabel;
		waiting->Initialization();
		waiting->SetFontColor(ccc4(255, 255, 255, 255));
		waiting->SetTag(TAG_WAITING);
		waiting->SetText(NDCommonCString("wait"));
		CGSize sizeText = getStringSize(NDCommonCString("wait"), 15);
		waiting->SetFrameRect(
				CGRectMake(pt.x - sizeText.width / 2, pt.y, sizeText.width,
						sizeText.height));
		AddChild(waiting);
	}

	NDTransData data(_MSG_BATTLEACT);
	Byte btDataCount = action.vData.size();
	data << action.btAction << Byte(m_turn - 1) << btDataCount;

	for (int i = 0; i < btDataCount; i++)
	{
		data << action.vData.at(i);
	}

	NDDataTransThread::DefaultThread()->GetSocket()->Send(&data);

	if ((action.btAction == BATTLE_ACT_MAG_ATK
			|| action.btAction == BATTLE_ACT_PET_MAG_ATK) && btDataCount > 0)
	{
		if (!((action.btAction == BATTLE_ACT_PET_MAG_ATK
				|| action.btAction == BATTLE_ACT_MAG_ATK)
				&& CanPetFreeUseSkill()))
		{
			int useSkillID = action.vData[0];
			UseSkillDealOfCooldown(useSkillID);
		}
	}

	// �г���ҷ��Զ�ս��
//	if (m_mainEudemon && m_mainEudemon->isAlive() &&
//	    !m_mainEudemon->isEscape() && !s_bAuto) {
//		switch (m_battleStatus) {
//			case BS_USER_MENU:
//			case BS_USER_SKILL_MENU:
//			case BS_CHOOSE_ENEMY_PHY_ATK:
//			case BS_CHOOSE_ENEMY_CATCH:
//			case BS_CHOOSE_OUR_SIDE_USE_ITEM_USER:
//			case BS_CHOOSE_ENEMY_MAG_ATK:
//			case BS_CHOOSE_OUR_SIDE_MAG_ATK:
//				//m_battleStatus = BS_EUDEMON_MENU;
//				//AddChild(m_eudemonOpt);
//				m_bSendCurTurnUserAction = true;
//				m_bTurnStartPet = true;
//				return;
//			default:
//				//RemoveChild(m_eudemonOpt, false);
//				break;
//		}
//	}

	setBattleStatus (BS_WAITING_SERVER_MESSAGE);

	//	m_fighterRight->SetShrink(true);
	//	m_fighterLeft->SetShrink(true);
	//	m_fighterBottom->SetShrink(true);
	//	
	//	m_fighterRight->SetGray(true);
	//	m_fighterLeft->SetGray(true);
	//	m_fighterBottom->SetGray(true);
}

void Battle::setBattleMap(int mapId, int posX, int posY)
{
	//	m_orignalMapId=NDMapMgrObj.m_iMapID;
	//	m_orignalPos=NDMapMgrObj.getMapLayerOfScene(NDDirector::DefaultDirector()->GetRunningScene())->GetScreenCenter();
	//	NDMapMgrObj.ClearNpc();
	//	NDMapMgrObj.ClearMonster();
	//	NDMapMgrObj.ClearGP();

	/***
	 * ��ʱ��ע�� ����
	 * begin
	 */
	// 	NDMapLayer* mapLayer = NDMapMgrObj.getMapLayerOfScene(NDDirector::DefaultDirector()->GetRunningScene());
// 	if(mapLayer){
// 		sceneMapId = mapLayer->GetMapIndex();
// 		sceneCenterX = mapLayer->GetScreenCenter().x;
// 		sceneCenterY = mapLayer->GetScreenCenter().y;
// 		//mapLayer->SetBattleBackground(true);
// 		mapLayer->replaceMapData(mapId, posX, posY);
	//mapLayer->SetNeedShowBackground(false);
	//}
	/***
	 * ��ʱ��ע�� ����
	 * end
	 ***/

//	return NDMapMgrObj.loadBattleSceneByMapID(mapId,posX*MAP_UNITSIZE,posY*MAP_UNITSIZE);
	//	mapLayer = NDMapMgrObj.getBattleMapLayerOfScene(NDDirector::DefaultDirector()->GetRunningScene());
	//	mapLayer->SetScreenCenter(ccp(posX*MAP_UNITSIZE,posY*MAP_UNITSIZE));
}

void Battle::drawSubAniGroup()
{
	bool bErase = false;

	for (VEC_SUB_ANI_GROUP_IT it = m_vSubAniGroup.begin();
			it != m_vSubAniGroup.end(); it++)
	{
		NDSprite* role = it->role;
		if (!role)
		{
			continue;
		}

		if (!(it->isCanStart))
		{
			it->isCanStart = true;
		}
		if (!(it->isCanStart))
		{
			continue;
		}
//		if(it->startFrame>0){
//			it->startFrame--;
//			continue;
//		}
		//NDLog("draw subanigroup");
//		it->bComplete = NDEngine::DrawSubAnimation(role, *it); ///< ��ʱ��ע�� ����
		if (it->bComplete)
		{
			NDLog("subanigroup complete");
			bErase = true;
			if (it->isFromOut)
			{
				it->aniGroup->release();
			}
			it->frameRec->release();
			it->frameRec = NULL;
		}
	}

	if (bErase)
	{
		m_vSubAniGroup.erase(
				remove_if(m_vSubAniGroup.begin(), m_vSubAniGroup.end(),
						IsSubAniGroupComplete()), m_vSubAniGroup.end());
	}
}

void Battle::sortFighterList(VEC_FIGHTER& fighterList)
{
	Fighter* fTemp = NULL;
	for (VEC_FIGHTER_IT itHead = fighterList.begin();
			itHead != fighterList.end(); itHead++)
	{
		for (VEC_FIGHTER_IT itMin = itHead + 1; itMin != fighterList.end();
				itMin++)
		{
			if ((*itMin)->getOriginY() < (*itHead)->getOriginY())
			{
				fTemp = *itMin;
				*itMin = *itHead;
				*itHead = fTemp;
			}
		}
	}
}

void Battle::sortFighterList()
{
	sortFighterList(m_vDefencer);
	sortFighterList(m_vAttaker);

	for (VEC_FIGHTER_IT it = m_vDefencer.begin(); it != m_vDefencer.end(); it++)
	{
		Fighter* fighter = *it;
		fighter = NULL;
	}
}

void Battle::drawFighter()
{
	Fighter* f = NULL;
	for (VEC_FIGHTER_IT it = m_vAttaker.begin(); it != m_vAttaker.end(); it++)
	{
		f = *it;
		if (f->isVisiable())
		{
			f->updatePos();
			f->draw();
			f->drawActionWord();
		}
	}

	for (VEC_FIGHTER_IT it = m_vDefencer.begin(); it != m_vDefencer.end(); it++)
	{
		f = *it;
		if (f->isVisiable())
		{
			f->updatePos();
			f->draw();
			f->drawActionWord();
		}
	}

	//	if (m_mainFighter && !watchBattle)
	//	{
	//		NDBaseRole* role = m_mainFighter->GetRole();
	//		CGPoint pt = role->GetPosition();
	//		m_imgWhoAmI->SetFrameRect(CGRectMake(pt.x - 6, pt.y - role->GetHeight(), m_picWhoAmI->GetSize().width, m_picWhoAmI->GetSize().height));
	//	}
}

void Battle::draw()
{
	if (eraseInOutEffect && !eraseInOutEffect->isChangeComplete())
	{
		return;
	}

	if (!IsVisibled())
	{
		SetVisible(true);
		//		NDMapLayer* mapLayer = NDMapMgrObj.getMapLayerOfScene(NDDirector::DefaultDirector()->GetRunningScene());
		//		mapLayer->SetBattleBackground(true);
		//		mapLayer->SetNeedShowBackground(false);
	}

	NDDirector::DefaultDirector()->EnableDispatchEvent(true);
	//	
	//	if(m_mapLayer){
	//		m_mapLayer->draw();
	//	}

	//	m_battleBg->draw();

	//if (eraseInOutEffect->isChangeComplete())
	battleRefresh();

	// ���Ʋ�ս��λ
	drawFighter();

	//	if (!eraseInOutEffect->isChangeComplete()) {
	//		return;
	//	}

	// ����ȥѪ
	drawAllFighterHurtNumber();

//	switch (m_battleStatus) {
//		case BS_CHOOSE_ENEMY_PHY_ATK_EUDEMON:
//		case BS_CHOOSE_ENEMY_PHY_ATK:
//		case BS_CHOOSE_ENEMY_CATCH:
//		case BS_CHOOSE_OUR_SIDE_USE_ITEM_USER:
//		case BS_CHOOSE_OUR_SIDE_USE_ITEM_EUDEMON:
//		case BS_CHOOSE_ENEMY_MAG_ATK:
//			if (m_highlightFighter) {
//				m_highlightFighter->drawHPMP();
//			}
//			break;
//		default:
//			break;
//	}

	drawSubAniGroup();

	//	if (m_mainFighter && m_battleOpt) 
	//	{
	//		m_battleOpt->SetVisible(!m_mainFighter->isEscape());
	//	}
	//	
	//	if (m_mainEudemon && m_eudemonOpt) 
	//	{
	//		m_eudemonOpt->SetVisible(!m_mainEudemon->isEscape());
	//	}

	//	NDUILayer::draw();
}

Fighter* Battle::GetMainUser()
{
	if (!m_mainFighter)
	{
		NDPlayer& player = NDPlayer::defaultHero();

		if (m_ourGroup == BATTLE_GROUP_ATTACK)
		{
			for (VEC_FIGHTER_IT it = m_vAttaker.begin(); it != m_vAttaker.end();
					it++)
			{
				if ((*it)->m_kInfo.idObj == player.m_nID)
				{
					m_mainFighter = (*it);
				}
			}
		}
		else
		{
			for (VEC_FIGHTER_IT it = m_vDefencer.begin();
					it != m_vDefencer.end(); it++)
			{
				if ((*it)->m_kInfo.idObj == player.m_nID)
				{
					m_mainFighter = (*it);
				}
			}
		}
	}

	return m_mainFighter;
}

void Battle::SetTurn(int turn)
{
	m_turn = turn + 1;
	stringstream ss;
	ss << m_turn << "/" << MAX_TURN;
	//	m_lbTurn->SetText(ss.str().c_str());
	//m_imgTurn->SetBigRedTwoNumber(m_turn - 1, MAX_TURN);

	AddTurnDealOfCooldown();
}

void Battle::AddCommand(Command* cmd)
{
	m_vCmdList.push_back(cmd);
}

void Battle::AddActionCommand(FightAction* action)
{
	Fighter* f = action->m_pkActor;
	int team = 0;
	if (f)
	{
		team = (f->m_kInfo.btBattleTeam - 1) % 3 + 1;
	}
	else
	{
		team = (action->m_nTeamAttack - 1) % 3 + 1;
	}

	switch (team)
	{
	case 1:
		BattleMgrObj.m_vActionList1.push_back(action);
		break;
	case 2:
		BattleMgrObj.m_vActionList2.push_back(action);
		break;
	case 3:
		BattleMgrObj.m_vActionList3.push_back(action);
		break;
	default:
		NDLog("can not addAction %d", team);
		break;
	}

	//	m_vActionList.push_back(action);
}

void Battle::ReleaseCommandList()
{
	VEC_COMMAND_IT it = m_vCmdList.begin();

	for (; it != m_vCmdList.end(); it++)
	{
		//SAFE_DELETE((*it)->skill);
		Command* cmdNext = (*it)->cmdNext;
		while (cmdNext)
		{
			Command* cmd = cmdNext;
			cmdNext = cmd->cmdNext;
			//SAFE_DELETE(cmd->skill);
			CC_SAFE_DELETE(cmd);
		}
		CC_SAFE_DELETE(*it);
	}

	m_vCmdList.clear();
}

Fighter* Battle::GetFighter(int idFighter)
{
	VEC_FIGHTER_IT it = m_vAttaker.begin();
	for (; it != m_vAttaker.end(); it++)
	{
		if ((*it)->m_kInfo.idObj == idFighter)
		{
			return *it;
		}
	}

	for (it = m_vDefencer.begin(); it != m_vDefencer.end(); it++)
	{
		if ((*it)->m_kInfo.idObj == idFighter)
		{
			return *it;
		}
	}

	return NULL;
}

void Battle::AddAnActionFighter(Fighter* fAction)
{
	for (VEC_FIGHTER_IT it = m_vActionFighterList.begin();
			it != m_vActionFighterList.end(); it++)
	{
		if ((*it) == fAction)
		{
			return;
		}
	}

	m_vActionFighterList.push_back(fAction);
}

void Battle::showBattleComplete()
{
	const char* psz = NULL;

	switch (battleCompleteResult)
	{
	case BATTLE_COMPLETE_WIN:
		psz = NDCommonCString("BattleSucc");
		break;
	case BATTLE_COMPLETE_LOSE:
		psz = NDCommonCString("BattleFail");
		break;
	case BATTLE_COMPLETE_NO:
		psz = NDCommonCString("BattleEnd");
		break;
//		case BATTLE_COMPLETE_END:
//			psz = NDCommonCString("BattleEnd");
//			break;
	default:
		psz = NDCommonCString("BattleShouGong");
		break;
	}

	m_dlgBattleResult = new NDUIDialog();
	m_dlgBattleResult->Initialization();
	m_dlgBattleResult->SetDelegate(this);
	//m_dlgBattleResult->SetWidth(100);
	//m_dlgBattleResult->SetContextHeight(20);
	m_dlgBattleResult->Show(NULL, psz, NULL, NULL);

	// �����ص���Ϸ����
	m_timer.SetTimer(this, TIMER_BACKTOGAME, 2);
}

void Battle::battleRefresh()
{
	fighterSomeActionChangeToWait (m_vAttaker);
	fighterSomeActionChangeToWait (m_vDefencer);

	if (m_battleStatus == BS_SHOW_FIGHT && m_startWait <= 0)
	{

		ShowFight();

	}
	else if (m_battleStatus == BS_FIGHTER_SHOW_PAS)
	{
		if (AllFighterActionOK())
		{
			ShowPas();
		}

	}
//	else {
//		if (!m_bWatch) {
//			if (m_bTurnStart) {
//				TurnStart();
//			} else if (m_bTurnStartPet) {
//				TurnStartPet();
//			}
//		}
//	}

	if (m_startWait > 0)
	{
		m_startWait--;
	}

	//refreshFighterData();
}

bool Battle::sideRightAtk()
{
	bool result = false;

	for (size_t i = 0; i < m_vActionFighterList.size(); i++)
	{
		Fighter& f = *m_vActionFighterList.at(i);
		if (f.m_kInfo.group == BATTLE_GROUP_ATTACK)
		{
			if (f.m_action == Fighter::ATTACK
					|| f.m_action == Fighter::SKILLATTACK)
			{
				result = true;
				break;
			}
		}
	}
	return result;
}

bool Battle::sideAttakerAtk()
{
	bool bResult = false;

	for (size_t i = 0; i < m_vActionFighterList.size(); i++)
	{
		Fighter& kFighter = *m_vActionFighterList.at(i);
		if (kFighter.m_kInfo.group == BATTLE_GROUP_ATTACK)
		{
			if (kFighter.m_action == Fighter::ATTACK
					|| kFighter.m_action == Fighter::SKILLATTACK)
			{
				bResult = true;
				break;
			}
		}
	}
	return bResult;
}

void Battle::ShowTimerAndTurn(bool bShow)
{
	//	if (bShow) {
	//		AddChild(m_imgTimer);
	//		AddChild(m_lbTimer);
	//		AddChild(m_lbTimerTitle);
	//		
	//		AddChild(m_imgTurn);
	//		AddChild(m_lbTurn);
	//		AddChild(m_lbTurnTitle);
	//	} else {
	//		m_lbTimer->RemoveFromParent(false);
	//		m_imgTimer->RemoveFromParent(false);
	//		m_lbTimerTitle->RemoveFromParent(false);
	//		
	//		m_lbTurn->RemoveFromParent(false);
	//		m_lbTurnTitle->RemoveFromParent(false);
	//		m_imgTurn->RemoveFromParent(false);
	//	}

}

void Battle::dealWithCommand()
{
	if (m_vCmdList.size() > 0 && m_battleStatus != BS_SHOW_FIGHT)
	{
		setBattleStatus (BS_SET_FIGHTER);

		for (int i = 0; i < m_vCmdList.size(); i++)
		{
			NDLog("%d action", i);
			Command* pkCommand = m_vCmdList.at(i);

			if (pkCommand->complete)
			{	// �������cmd
				continue;
			}
			FightAction* pkAction = NULL;
			switch (pkCommand->btEffectType)
			{
			case BATTLE_EFFECT_TYPE_ATK:	//����
				NDLog("%d effect_atk", pkCommand->idActor);
				//					Fighter* theTarget =
				//					theActor->m_effectType=BATTLE_EFFECT_TYPE(cmd->btEffectType);
				//					theActor->m_mainTarget=GetFighter(cmd->idTarget);
				//					if (theActor->GetNormalAtkType() == ATKTYPE_NEAR) {
				//						theActor->m_action = (Fighter::MOVETOTARGET);
				//					} else if (theActor->GetNormalAtkType() == ATKTYPE_DISTANCE) {
				//						theActor->m_action = (Fighter::AIMTARGET);
				//					}
				//					theActor->m_actionType = (Fighter::ACTION_TYPE_NORMALATK);
				//					AddAnActionFighter(theActor);
				pkAction = new FightAction(GetFighter(pkCommand->idActor),
						GetFighter(pkCommand->idTarget),
						BATTLE_EFFECT_TYPE(pkCommand->btEffectType));
				break;
			case BATTLE_EFFECT_TYPE_SKILL:					//����
				NDLog("%d effect_skill", pkCommand->idActor);
				pkAction = new FightAction(GetFighter(pkCommand->idActor),
						GetFighter(pkCommand->idTarget),
						BATTLE_EFFECT_TYPE(pkCommand->btEffectType));
				pkAction->m_pkSkill = pkCommand->skill;
				break;
			case EFFECT_TYPE_TURN_END:
				NDLog("turn end");
				continue;
			case EFFECT_TYPE_BATTLE_BEGIN:
				NDLog("%d battle_begin", pkCommand->idTarget);
				pkAction = new FightAction(pkCommand->idActor, pkCommand->idTarget,
						EFFECT_TYPE_BATTLE_BEGIN);
				break;
			case EFFECT_TYPE_BATTLE_END:
				NDLog("%d battle_end", pkCommand->idActor);
				pkAction = new FightAction(pkCommand->idActor, pkCommand->idTarget,
						EFFECT_TYPE_BATTLE_END);
				break;
			case BATTLE_EFFECT_TYPE_STATUS_LIFE:
				NDLog("%d status_life", pkCommand->idActor);
				pkAction = new FightAction(GetFighter(pkCommand->idActor),
						GetFighter(pkCommand->idTarget),
						BATTLE_EFFECT_TYPE(pkCommand->btEffectType));
				pkAction->m_nData = pkCommand->nHpLost;
				break;
				//				case BATTLE_EFFECT_TYPE_DODGE:// ����
				//					if (!theActor || !theTarget) {
				//						return;
				//					}
				//
				//					theActor->m_effectType = EFFECT_TYPE(cmd->btEffectType);
				//					theActor->m_mainTarget = theTarget;
				//					theTarget->m_actor = (theActor);
				//					theActor->m_bMissAtk = (true);
				//					if (theActor->GetNormalAtkType() == ATKTYPE_NEAR) {
				//						theActor->m_action = (Fighter::MOVETOTARGET);
				//					} else if (theActor->GetNormalAtkType() == ATKTYPE_DISTANCE) {
				//						theActor->m_action = (Fighter::AIMTARGET);
				//					}
				//					theActor->m_actionType = (Fighter::ACTION_TYPE_NORMALATK);
				//					theActor->m_changeLifeType = EFFECT_CHANGE_LIFE_TYPE(cmd->btType);
				//					AddAnActionFighter(theActor);
				//					break;
				//
				//				case EFFECT_TYPE_CHANGELIFE: // ȥѪ
				//					if (cmd->btType == EFFECT_CHANGE_LIFE_TYPE_PHY_ATK || cmd->btType == EFFECT_CHANGE_LIFE_TYPE_PHY_HARDATK) {
				//						if (!theActor || !theTarget) {
				//							return;
				//						}
				//
				//						theActor->m_changeLifeType = EFFECT_CHANGE_LIFE_TYPE(cmd->btType);
				//						theActor->m_effectType = EFFECT_TYPE(cmd->btEffectType);
				//						theActor->m_actionType = (Fighter::ACTION_TYPE_NORMALATK);
				//						theActor->m_mainTarget = theTarget;
				//						theTarget->m_actor = (theActor);
				//
				//						theTarget->AddAHurt(theActor, cmd->btType, cmd->nHpLost, cmd->nMpLost, cmd->dwData, HURT_TYPE_ACTIVE);
				//						AddAnActionFighter(theActor);
				//
				//						theTarget->m_bHardAtk = cmd->btType == EFFECT_CHANGE_LIFE_TYPE_PHY_HARDATK;
				//
				//						if (cmd->triggerProtect && theTarget->protector) {
				//							theTarget->protector->m_action = (Fighter::MOVETOTARGET);
				//							theTarget->protector->m_actionType = Fighter::ACTION_TYPE_PROTECT;
				//						}
				//
				//						// ״̬Ϊ����Ŀ�꣨���������߹�����Զ�̣�
				//						if (theActor->GetNormalAtkType() == ATKTYPE_NEAR) {
				//							theActor->m_action = (Fighter::MOVETOTARGET);
				//						} else if (theActor->GetNormalAtkType() == ATKTYPE_DISTANCE) {
				//							theActor->m_action = (Fighter::AIMTARGET);
				//						}
				//					} else if (cmd->btType == EFFECT_CHANGE_LIFE_TYPE_USE_SKILL) {
				//						theActor->m_kInfo.nMana += cmd->nMpLost;
				//						theActor->m_kInfo.nLife += cmd->nHpLost;
				//
				//					} else if (cmd->btType == EFFECT_CHANGE_LIFE_TYPE_SKILL_ATK || cmd->btType == EFFECT_CHANGE_LIFE_TYPE_SKILL_HARDATK) {
				//						theActor->m_changeLifeType = EFFECT_CHANGE_LIFE_TYPE(cmd->btType);
				//						theActor->m_effectType = EFFECT_TYPE(cmd->btEffectType);
				//						theActor->m_actionType = (Fighter::ACTION_TYPE_SKILLATK);
				//
				//						theActor->m_mainTarget = theTarget;
				//						theActor->AddATarget(theTarget);
				//						theTarget->m_actor = (theActor);
				//
				//						AddAnActionFighter(theActor);
				//
				//						if (cmd->triggerProtect && theTarget->protector) {
				//							theTarget->protector->m_action = (Fighter::MOVETOTARGET);
				//							theTarget->protector->m_actionType = (Fighter::ACTION_TYPE_PROTECT);
				//						}
				//
				//						theTarget->AddAHurt(theActor, cmd->btType, cmd->nHpLost, cmd->nMpLost, cmd->dwData, HURT_TYPE_ACTIVE);
				//
				//						BattleSkill* skill = cmd->skill;
				//						if (skill) {
				//							theActor->setUseSkill(skill);
				//							if ((theActor->getUseSkill()->getAtkType() & SKILL_ATK_TYPE_NEAR) == SKILL_ATK_TYPE_NEAR) {
				//								theActor->m_action = (Fighter::MOVETOTARGET);
				//								theActor->setSkillAtkType(ATKTYPE_NEAR);
				//							} else if ((theActor->getUseSkill()->getAtkType() & SKILL_ATK_TYPE_REMOTE) == SKILL_ATK_TYPE_REMOTE) {
				//								theActor->m_action = (Fighter::AIMTARGET);
				//								theActor->setSkillAtkType(ATKTYPE_DISTANCE);
				//							}
				//						}
				//
				//						theTarget->m_bHardAtk = cmd->btType == EFFECT_CHANGE_LIFE_TYPE_SKILL_HARDATK;
				//
				//						Command* nextCmd = cmd->cmdNext;
				//						while (nextCmd) {
				//							if ((nextCmd->btType == EFFECT_CHANGE_LIFE_TYPE_SKILL_HARDATK
				//							     || nextCmd->btType == EFFECT_CHANGE_LIFE_TYPE_SKILL_ATK)
				//							    && nextCmd->idActor == cmd->idActor) {
				//								// ��������
				//								theTarget = GetFighter(nextCmd->idTarget);
				//								theTarget->AddAHurt(theActor, cmd->btType,
				//										   nextCmd->nHpLost, nextCmd->nMpLost,
				//										   nextCmd->dwData, HURT_TYPE_ACTIVE);
				//								theActor->AddATarget(theTarget);
				//								theTarget->m_actor = (theActor);
				//								nextCmd->complete = (true);
				//							}
				//							theTarget->m_bHardAtk = cmd->btType == EFFECT_CHANGE_LIFE_TYPE_SKILL_HARDATK;
				//							nextCmd = nextCmd->cmdNext;
				//						}
				//					} else if (cmd->btType == EFFECT_CHANGE_LIFE_TYPE_SKILL_STATUS
				//						   || cmd->btType == EFFECT_CHANGE_LIFE_TYPE_CHG_MAX) {
				//
				//						theTarget->m_changeLifeTypePas = EFFECT_CHANGE_LIFE_TYPE(cmd->btType);
				//						theTarget->AddPasStatus(cmd->dwData);
				//
				//						theTarget->AddAHurt(NULL, cmd->btType, cmd->nHpLost, cmd->nMpLost, cmd->dwData, HURT_TYPE_PASSIVE);// ״̬ȥѪ��Ҫָ���Ǳ�˭���
				//						AddAnActionFighter(theTarget);
				//					} else if (cmd->btType == EFFECT_CHANGE_LIFE_TYPE_USE_ITEM) {
				//
				//						theActor->m_idUsedItem = (cmd->dwData);
				//						theActor->m_changeLifeType = EFFECT_CHANGE_LIFE_TYPE(cmd->btType);
				//						theActor->m_effectType = EFFECT_TYPE(cmd->btEffectType);
				//
				//						theActor->m_actionType = (Fighter::ACTION_TYPE_USEITEM);
				//						theActor->m_action = (Fighter::AIMTARGET);
				//						theActor->m_mainTarget = theTarget;
				//						theTarget->m_actor = (theActor);
				//
				//						theTarget->AddAHurt(theActor, cmd->btType, cmd->nHpLost, cmd->nMpLost, cmd->dwData, HURT_TYPE_ACTIVE);
				//						AddAnActionFighter(theActor);
				//					} else if (cmd->btType == EFFECT_CHANGE_LIFE_TYPE_SKILL_STATUS_ADD
				//						   || cmd->btType == EFFECT_CHANGE_LIFE_TYPE_SKILL_STATUS_LOST) {
				//						// �ڻغϳ��յ�״̬��Ϣ�������ϴ���
				//						Fighter* target = GetFighter(cmd->idTarget);
				//
				//						target->addAStatus(cmd->status);
				//						Command* c = cmd->cmdNext;
				//						while (c) {
				//							target = GetFighter(c->idTarget);
				//							if (c->btType == EFFECT_CHANGE_LIFE_TYPE_SKILL_STATUS_ADD) {
				//								target->addAStatus(c->status);
				//							} else if (c->btType == EFFECT_CHANGE_LIFE_TYPE_SKILL_STATUS_LOST) {
				//								target->removeAStatusAniGroup(c->dwData);
				//							}
				//							c = c->cmdNext;
				//						}
				//					}
				//					if (cmd->btType == EFFECT_CHANGE_LIFE_TYPE_PROTECTED) {
				//						theActor->m_changeLifeType = EFFECT_CHANGE_LIFE_TYPE(cmd->btType);
				//						// ����
				//						theActor->protectTarget = theActor->m_mainTarget;
				//						theActor->m_mainTarget->protector = theActor;
				//						theActor->hurtInprotect = cmd->nHpLost;
				//					}
				//					break;
				//
				//				case EFFECT_TYPE_ESCAPE:
				//					if (!theActor) {
				//						return;
				//					}
				//					theActor->m_effectType = EFFECT_TYPE(cmd->btEffectType);
				//
				//					theActor->m_action = (Fighter::FLEE_SUCCESS);
				//					AddAnActionFighter(theActor);
				//					theActor->m_bFleeNoDie = (true);
				//					break;
				//
				//				case EFFECT_TYPE_ESCAPE_FAIL:
				//					if (!theActor) {
				//						return;
				//					}
				//					theActor->m_effectType = EFFECT_TYPE(cmd->btEffectType);
				//
				//					theActor->m_action = (Fighter::FLEE_FAIL);
				//					AddAnActionFighter(theActor);
				//					break;
				//
				//				case EFFECT_TYPE_DEFENCE:
				//					if (!theActor) {
				//						return;
				//					}
				//					theActor->m_effectType = EFFECT_TYPE(cmd->btEffectType);
				//
				//					theActor->m_bDefenceOK = (true);
				//					theActor->m_action = (Fighter::DEFENCE);
				//					defenceAction(*theActor);
				//					AddAnActionFighter(theActor);
				//					break;
				//
				//				case EFFECT_TYPE_LEFT:
				//					m_timeLeftMax = 29;
				//					break;
				//				case EFFECT_TYPE_CATCH:
				//					theActor->m_effectType = (EFFECT_TYPE_CATCH);
				//					theActor->m_actionType = (Fighter::ACTION_TYPE_CATCH);
				//					theActor->m_action = (Fighter::AIMTARGET);
				//					theActor->m_mainTarget = (theTarget);
				//					theActor->AddATarget(theTarget);
				//					theTarget->m_actor = (theActor);
				//					AddAnActionFighter(theActor);
				//					break;
				//				case EFFECT_TYPE_CATCH_FAIL:
				//					theActor->m_effectType = (EFFECT_TYPE_CATCH_FAIL);
				//					theActor->m_actionType = (Fighter::ACTION_TYPE_CATCH);
				//					theActor->m_action = (Fighter::AIMTARGET);
				//					theActor->m_mainTarget = (theTarget);
				//					theActor->AddATarget(theTarget);
				//					theTarget->m_actor = (theActor);
				//					AddAnActionFighter(theActor);
				//					break;
			default:
				break;
			}
			//������Ҫ���������������
			if (pkAction)
			{
				Command* nextCmd = pkCommand->cmdNext;
				while (nextCmd)
				{

					FIGHTER_CMD* fcmd = new FIGHTER_CMD();

					switch (nextCmd->btEffectType)
					{
					case BATTLE_EFFECT_TYPE_LIFE:
						NDLog("%d chageLife", nextCmd->idActor);
						fcmd->actor = nextCmd->idActor;
						fcmd->effect_type = BATTLE_EFFECT_TYPE_LIFE;
						fcmd->data = (int) nextCmd->nHpLost;
						break;
					case BATTLE_EFFECT_TYPE_MANA:
						NDLog("%d chageMana", nextCmd->idActor);
						fcmd->actor = nextCmd->idActor;
						fcmd->effect_type = BATTLE_EFFECT_TYPE_MANA;
						fcmd->data = (int) nextCmd->nMpLost;
						break;
					case BATTLE_EFFECT_TYPE_SKILL_TARGET:
						NDLog("%d skill_target", nextCmd->idTarget);
						if (pkAction->m_eEffectType == BATTLE_EFFECT_TYPE_SKILL)
						{
							pkAction->m_kFighterList.push_back(
									GetFighter(nextCmd->idTarget));
							if (pkAction->m_pkTarget == NULL)
							{
								pkAction->m_pkTarget = GetFighter(
										nextCmd->idTarget);
							}
						}
						nextCmd = nextCmd->cmdNext;
						continue;
					case BATTLE_EFFECT_TYPE_DODGE:
						NDLog("%d dodge", nextCmd->idActor);
						fcmd->actor = nextCmd->idActor;
						fcmd->effect_type = BATTLE_EFFECT_TYPE_DODGE;
						break;
					case BATTLE_EFFECT_TYPE_DRITICAL:
						NDLog("%d dritical", nextCmd->idActor);
						fcmd->actor = nextCmd->idActor;
						fcmd->effect_type = BATTLE_EFFECT_TYPE_DRITICAL;
						//							fcmd->data=(int)nextCmd->nHpLost;
						break;
					case BATTLE_EFFECT_TYPE_BLOCK:
						NDLog("%d block", nextCmd->idActor);
						fcmd->actor = nextCmd->idActor;
						fcmd->effect_type = BATTLE_EFFECT_TYPE_BLOCK;
						break;
					case BATTLE_EFFECT_TYPE_COMBO:
						NDLog("%d combo", nextCmd->idActor);
						pkAction->m_bIsCombo = true;
						AddActionCommand(pkAction);
						pkAction = new FightAction(GetFighter(pkCommand->idActor),
								GetFighter(pkCommand->idTarget),
								BATTLE_EFFECT_TYPE(pkCommand->btEffectType));
						fcmd->actor = pkCommand->idTarget;
						fcmd->effect_type = BATTLE_EFFECT_TYPE_LIFE;
						fcmd->data = (int) nextCmd->nHpLost;
						pkAction->addCommand(fcmd);
						nextCmd = nextCmd->cmdNext;
						continue;
					case BATTLE_EFFECT_TYPE_STATUS_ADD:
						fcmd->actor = nextCmd->idActor;
						fcmd->effect_type = BATTLE_EFFECT_TYPE_STATUS_ADD;
						fcmd->status = nextCmd->status;
						break;
					case BATTLE_EFFECT_TYPE_STATUS_LOST:
						fcmd->actor = nextCmd->idActor;
						fcmd->effect_type = BATTLE_EFFECT_TYPE_STATUS_LOST;
						fcmd->status = nextCmd->status;
						break;
					case BATTLE_EFFECT_TYPE_DEAD:
						fcmd->actor = nextCmd->idActor;
						fcmd->effect_type = BATTLE_EFFECT_TYPE_DEAD;
						break;
					default:
						break;
					}
					pkAction->addCommand(fcmd);
					//					theTarget = GetFighter(nextCmd->idActor);

					//					if (nextCmd->btType == EFFECT_CHANGE_LIFE_TYPE_SKILL_STATUS_ADD) {
					//						// �����󣬸���״̬
					//						theTarget = GetFighter(nextCmd->idTarget);
					//						theTarget->AddAHurt(NULL, nextCmd->btType,
					//								   nextCmd->nHpLost, nextCmd->nMpLost,
					//								   nextCmd->dwData, HURT_TYPE_PASSIVE);
					//						StatusAction sa(StatusAction::ADD_STATUS,
					//								nextCmd->status,
					//								nextCmd->idTarget);
					//						theActor->AddAStatusTarget(sa);
					//						nextCmd->complete = (true);
					//						
					//					} else if (nextCmd->btType == EFFECT_CHANGE_LIFE_TYPE_SKILL_STATUS_LOST) {
					//						// ������ȡ������״̬
					//						theTarget = GetFighter(nextCmd->idTarget);
					//						theTarget->AddAHurt(NULL, nextCmd->btType,
					//								   nextCmd->nHpLost, nextCmd->nMpLost,
					//								   nextCmd->dwData, HURT_TYPE_PASSIVE);
					//						StatusAction sa(StatusAction::CANCEL_STATUS,
					//								nextCmd->status,
					//								nextCmd->idTarget);
					//						theActor->AddAStatusTarget(sa);						
					//						nextCmd->complete = (true);
					//					}

					nextCmd = nextCmd->cmdNext;
				}
			}
			NDLog("addAction");
			AddActionCommand(pkAction);
		}

		//		if (m_vActionFighterList.size() > 0) {
		//			Fighter* f = m_vActionFighterList.at(0);
		//			f->setBeginAction(true);
		//		}

		m_actionFighterPoint = 0;

		m_foreBattleStatus = 0;

		setBattleStatus (BS_SHOW_FIGHT);
		m_startWait = 25;
	}
}

void Battle::RestartFight()
{
	ShowTimerAndTurn(false);

	RemoveChild(TAG_WAITING, true);
	clearHighlight();

	m_currentActionIndex1 = 0;
	m_currentActionIndex2 = 0;
	m_currentActionIndex3 = 0;
	m_Team1_status = TEAM_WAIT;
	m_Team2_status = TEAM_WAIT;
	m_Team3_status = TEAM_WAIT;

	setBattleStatus (BS_SHOW_FIGHT);
	m_startWait = 25;
}

void Battle::StartFight()
{
	ShowTimerAndTurn(false);
	//RemoveChild(TAG_TIMER, false);
	if (!m_bWatch)
	{
		RemoveChild(TAG_WAITING, true);
		clearHighlight();
		if (ms_bAuto)
		{
			//			RemoveChild(m_imgAutoCount, false);
			//			RemoveChild(m_lbAuto, false);
		}

//		switch (m_battleStatus) {
//			case BS_USER_MENU:
//				//RemoveChild(m_battleOpt, false);
//				break;
//			case BS_USE_ITEM_MENU:
//				//CloseItemMenu();
//				break;
//			case BS_CHOOSE_VIEW_FIGHTER_STATUS:
//				CloseViewStatus();
//				break;
//			case BS_USER_SKILL_MENU:
//				//CloseSkillMenu();
//				break;
//			default:
//				break;
//		}
	}
	dealWithCommand();
	m_currentActionIndex1 = 0;
	m_currentActionIndex2 = 0;
	m_currentActionIndex3 = 0;
	m_Team1_status = TEAM_WAIT;
	m_Team2_status = TEAM_WAIT;
	m_Team3_status = TEAM_WAIT;
}

bool Battle::AllFighterActionOK()
{
	bool result = true;

	//test Ԥ��actionOK���˳�ʱ�����⡣
	for (size_t i = 0; i < m_vActionFighterList.size(); i++)
	{
		Fighter* f = m_vActionFighterList.at(i);
		if (f->getActionTime() > 200)
		{
			f->setActionOK(true);
		}
	}

	// �ж�action�Ƿ�ok
	for (size_t i = 0; i < m_vActionFighterList.size(); i++)
	{
		Fighter* f = m_vActionFighterList.at(i);
		if (!f->isVisiable() || f->isActionOK()
				|| (!f->isEscape() && !f->isAlive()))
		{
			continue;
		}
		else
		{
			result = false;
			break;
		}
	}
	// �ж�״̬�����Ƿ���ɡ�
	if (result == true)
	{
		return fighterStatusOK(m_vAttaker) && fighterStatusOK(m_vDefencer);
	}
	return result;
}

bool Battle::fighterStatusOK(VEC_FIGHTER& fighterList)
{
	bool result = true;
	for (size_t i = 0; i < fighterList.size(); i++)
	{
		Fighter* f = fighterList.at(i);
		if (!f->isHurtOK() && !f->isDodgeOK() && !f->isDieOK())
		{
			continue;
		}
		else
		{
			result = false;
			break;
		}
	}
	return result;
}

void Battle::notifyNextFighterBeginAction()
{
	size_t idx = m_actionFighterPoint;

	if (idx >= m_vActionFighterList.size())
		return;

	Fighter* f = m_vActionFighterList.at(idx);
	Fighter* fNext = NULL;
	size_t i = 0;
	if (idx < m_vActionFighterList.size() - 1)
	{
		for (i = idx + 1; i < m_vActionFighterList.size(); i++)
		{
			Fighter* ftmp = m_vActionFighterList.at(i);
			if (ftmp->isAlive() && ftmp->isVisiable() && !ftmp->isBeginAction())
			{
				fNext = ftmp;
				break;
			}
		}
	}
	if (fNext)
	{
		if (f->m_kInfo.group == fNext->m_kInfo.group)
		{			// ͬ�ߵĺܿ�Ϳ�ʼ����
			if (f->completeOneAction())
			{
				fNext->setBeginAction(true);
				m_actionFighterPoint = i;
			}
		}
		else
		{
			if (f->isActionOK() && !fNext->isHurtOK() && !fNext->isDodgeOK()
					&& !fNext->isDieOK()
					|| f->m_actionType == Fighter::ACTION_TYPE_PROTECT)
			{
				fNext->setBeginAction(true);
				m_actionFighterPoint = i;
			}
		}
	}
}

bool Battle::isPasClear()
{
	bool result = true;
	//	for (size_t i = 0; i < m_vActionFighterList.size(); i++) {
	//		Fighter& f = *m_vActionFighterList.at(i);
	//		if (f.m_changeLifeTypePas != EFFECT_CHANGE_LIFE_TYPE_NONE) {
	//			result = false;
	//			break;
	//		}
	//	}
	return result;
}

bool Battle::noOneCanAct(VEC_FIGHTER& fighterList)
{
	bool result = false;
	if (fighterList.size() == 0)
	{
		result = true;
	}
	for (size_t i = 0; i < fighterList.size(); i++)
	{
		Fighter& f = *fighterList.at(i);
		if (f.isVisiable() && f.isAlive())
		{
			break;
		}
		else if (i == fighterList.size() - 1)
		{
			result = true;
		}
	}
	return result;
}

BATTLE_COMPLETE Battle::battleComplete()
{
	BATTLE_COMPLETE result = BATTLE_COMPLETE(serverBattleResult);
//	if (noOneCanAct(GetEnemySideList())) {
//		result = BATTLE_COMPLETE_WIN;
//		if (GetMainUser() == NULL || GetMainUser()->m_kInfo.nLife > 0) {
//			monsterResult(GetEnemySideList());
//		}
//	}else if (noOneCanAct(GetOurSideList())) {
//		result = BATTLE_COMPLETE_LOSE;
//	}
//	
//	if(getServerBattleResult() != -2) {
//		result = BATTLE_COMPLETE(getServerBattleResult());
//	}
	monsterResult(GetEnemySideList());
	monsterResult(GetOurSideList());
	return result;
}

VEC_FIGHTER& Battle::GetOurSideList()
{
	if (m_ourGroup == BATTLE_GROUP_ATTACK)
	{
		return m_vAttaker;
	}
	else
	{
		return m_vDefencer;
	}
}

VEC_FIGHTER& Battle::GetEnemySideList()
{
	if (m_ourGroup == BATTLE_GROUP_DEFENCE)
	{
		return m_vAttaker;
	}
	else
	{
		return m_vDefencer;
	}
}

Fighter* Battle::getMainEudemon()
{
	if (!m_mainEudemon)
	{
		//Fighter* user = GetMainUser();
		//		if (user) {
		//PetInfo* petInfo = PetMgrObj.GetMainPet(NDPlayer::defaultHero().m_id);
		//if (petInfo) {
		//	VEC_FIGHTER& euList = GetOurSideList();
		//	for (size_t i = 0; i < euList.size(); i++) {
		//		Fighter* f =  euList.at(i);
		//		//					if (f->m_kInfo.idPet == petInfo->data.int_PET_ID) {
		//		//						m_mainEudemon = f;
		//		//						break;
		//		//					}
		//		
		//	}
		//}

		//		}
	}

	return m_mainEudemon;
}

void Battle::setFighterToWait(VEC_FIGHTER& fighterList)
{
	for (size_t i = 0; i < fighterList.size(); i++)
	{
		Fighter& f = *fighterList.at(i);
		if (!f.isVisiable())
		{
			continue;
		}
		f.m_action = (Fighter::WAIT);
		if (f.isAlive())
		{
			battleStandAction(f);
		}
	}
}

void Battle::setAllFightersToWait()
{
	setFighterToWait (m_vAttaker);
	setFighterToWait (m_vDefencer);
}

void Battle::clearFighterStatus(Fighter& f)
{
	f.clearFighterStatus();
	f.setBeginAction(false);
	f.m_bMissAtk = false;
	f.setHurtOK(false);
	f.setDodgeOK(false);
	f.setActionOK(false);
	f.setDefenceOK(false);
	f.setActionTime(0);
}

void Battle::clearActionFighterStatus()
{
	for (size_t i = 0; i < m_vActionFighterList.size(); i++)
	{
		Fighter& f = *m_vActionFighterList.at(i);
		clearFighterStatus(f);
	}
	m_vActionFighterList.clear();
}

void Battle::FinishBattle()
{
	/***
	 * ��ʱ��ע�� ����
	 */
// 	NDMapMgrObj.BattleEnd(BattleMgrObj.GetBattleReward()->battleResult);
// 	//m_timer.SetTimer(this, TIMER_BACKTOGAME, 1);
// 	BattleMgrObj.quitBattle();
}

void Battle::ShowPas()
{
	setBattleStatus (BS_BATTLE_COMPLETE);
	battleCompleteResult = battleComplete();
	//showBattleComplete();
	// �˳�ս��,��ͼ�߼�����

	if (currentShowFighter > 0)
	{
		//ScriptMgrObj.excuteLuaFunc("CloseFighterInfo","FighterInfo",0); ///< ��ʱ��ע�� ����
	}
	BattleMgrObj.showBattleResult();
	//	if (isPasClear() && AllFighterActionOK()) {
	//		battleCompleteResult = battleComplete();
	//		
	//		if (battleCompleteResult != BATTLE_COMPLETE_NO) {
	//			setBattleStatus(BS_BATTLE_COMPLETE);
	//			showBattleComplete();
	//		} else {// ս����δ����,�غϽ���
	//			if (m_vCmdList.size() > 0) {// �Ѿ����˲��ٷ������ǰ������ָ������ռ����ָ���ˡ�
	//				setBattleStatus(BS_SET_FIGHTER);
	//				
	//			} else {
	//				if (GetMainUser()) {
	//					setBattleStatus(BS_USER_MENU);// �Ƚ������ָ��״̬���ڸ��ݾ�������ı䡣
	//					
	//					if (!GetMainUser()->isAlive()) {// ���������ˣ��г���͵��������ָ��
	//						if (getMainEudemon()) {// �������г���
	//							if (getMainEudemon()->isVisiable()) {
	//								setBattleStatus(BS_EUDEMON_MENU);
	//							}
	//						} else {// �����û�г���
	//							setBattleStatus(BS_USER_MENU);// �����״̬��ӦΪ��ҵ�����״̬�ȣ�ָ��򲻻ᵯ��
	//						}
	//					} else {// �����һ���
	//						if (GetMainUser()->isVisiable()) {// �������û�����ܣ����������ָ��
	//							setBattleStatus(BS_USER_MENU);
	//							m_bSendCurTurnUserAction = false;
	//							
	//						} else {// ����������ˣ��г�����ó���ָ�
	//							if (getMainEudemon() != NULL && getMainEudemon()->isVisiable()) {
	//								setBattleStatus(BS_EUDEMON_MENU);
	//							}
	//						}
	//					}
	//					
	//					if (m_battleStatus == BS_USER_MENU) 
	//					{
	//						if (s_bAuto) 
	//						{
	//							OnBtnAuto(true);
	//						}
	//						else 
	//						{
	//							//AddChild(m_battleOpt);
	//							m_bTurnStart = true;
	//						}
	//					} 
	//					else if (m_battleStatus == BS_EUDEMON_MENU) 
	//					{
	//						if (s_bAuto) {
	//							OnBtnAuto(true);
	//						} else {
	//							//AddChild(m_eudemonOpt);
	//							m_bTurnStartPet = true;
	//						}
	//
	//					}
	//				} else {
	//					if (m_bWatch) {
	//						setBattleStatus(BS_WAITING_SERVER_MESSAGE);
	//					}
	//					else
	//					{
	//						if (getMainEudemon()) {// �������г���
	//							if (getMainEudemon()->isVisiable()) {
	//								setBattleStatus(BS_EUDEMON_MENU);
	//								if (s_bAuto) {
	//									OnBtnAuto(true);
	//								} else {
	//									//AddChild(m_eudemonOpt);
	//									m_bTurnStartPet = true;
	//								}
	//							}
	//						}
	//						else
	//						{
	//							setBattleStatus(BS_WAITING_SERVER_MESSAGE);
	//						} 
	//					}
	//				}
	//			}
	//			
	//			setAllFightersToWait();
	//			
	//			// ���»غ����͵���ʱ
	//			//m_turn++;
	//			{
	//				stringstream ss;
	//				ss << m_turn << "/" << MAX_TURN;
	////				m_lbTurn->SetText(ss.str().c_str());
	//			}
	//			//m_imgTurn->SetBigRedTwoNumber(m_turn, MAX_TURN);
	//			m_timeLeft = m_timeLeftMax;
	//			{
	//				stringstream ss;
	//				ss << m_timeLeft;
	////				m_lbTimer->SetText(ss.str().c_str());
	//			}
	//			ShowTimerAndTurn(true);
	//			//m_imgTimer->SetBigRedNumber(, false);
	////			if (m_lbTimer->GetParent() == NULL) {
	////				AddChild(m_lbTimer);
	////			}
	//			
	//			// ���µ���ʱ
	////			if (s_bAuto && m_mainFighter->isAlive()) {
	////				AddChild(m_imgAutoCount);
	////				AddChild(m_lbAuto);
	////				m_autoCount = AUTO_COUNT;
	////				m_imgAutoCount->SetBigRedNumber(m_autoCount, false);
	////			}
	//			
	//			clearActionFighterStatus();
	//		}
	//	} else {
	////		for (size_t i = 0; i < m_vActionFighterList.size(); i++) {
	////			Fighter& f = *m_vActionFighterList.at(i);
	////			if (f.m_changeLifeTypePas == EFFECT_CHANGE_LIFE_TYPE_SKILL_STATUS || f.m_changeLifeTypePas == EFFECT_CHANGE_LIFE_TYPE_CHG_MAX
	////			    || f.m_changeLifeTypePas == EFFECT_CHANGE_LIFE_TYPE_SKILL_STATUS_LOST) {
	////				
	////				performStatus(f);
	////				
	////			}
	////		}
	//	}
}

void Battle::performStatus(Fighter& theTarget)
{
	//	if (theTarget.m_changeLifeTypePas != EFFECT_CHANGE_LIFE_TYPE_NONE) {
	//		int status = theTarget.getAPasStatus();
	//		
	//		Hurt hurt = theTarget.getHurt(NULL, theTarget.m_changeLifeTypePas, status, HURT_TYPE_PASSIVE).second;
	//		
	//		int hurtHP = hurt.hurtHP;
	//		int hurtMP = hurt.hurtMP;
	//		int addHP = 0;
	//		int addMP = 0;
	//		int currentHP = 0;
	//		int currentMP = 0;
	//		if (hurtHP != 0) {
	//			if (theTarget.m_changeLifeTypePas == EFFECT_CHANGE_LIFE_TYPE_CHG_MAX) {
	//				addHP = hurtHP - theTarget.m_kInfo.nLifeMax;
	//				theTarget.m_kInfo.nLifeMax = (hurtHP);
	//				currentHP = theTarget.m_kInfo.nLife;
	//				currentHP += addHP;
	//				theTarget.hurted(addHP);
	//				
	//			} else {
	//				currentHP = theTarget.m_kInfo.nLife;
	//				currentHP += hurtHP;
	//				theTarget.hurted(hurtHP);
	//			}
	//			if (!IsPracticeBattle()) {
	//				theTarget.setCurrentHP(currentHP);
	//			}
	//			currentHP = theTarget.m_kInfo.nLife;
	//			if (hurtHP < 0) {
	//				
	//				if (currentHP > 0) {// hurt
	//					theTarget.setHurtOK(true);
	//					hurtAction(theTarget);
	//				} else {// die
	//					theTarget.setDieOK(true);
	//					dieAction(theTarget);
	//				}
	//				
	//			}
	//		}
	//		if (hurtMP != 0) {
	//			if (theTarget.m_changeLifeTypePas == EFFECT_CHANGE_LIFE_TYPE_CHG_MAX) {
	//				addMP = hurtMP - theTarget.m_kInfo.nManaMax;
	//				theTarget.m_kInfo.nManaMax = (hurtMP);
	//				currentMP = theTarget.m_kInfo.nMana;
	//				currentMP += addMP;
	//				theTarget.hurted(addMP);
	//				theTarget.setCurrentMP(currentMP);
	//			} else {
	//				currentMP = theTarget.m_kInfo.nMana;
	//				currentMP += hurtMP;
	//				theTarget.hurted(hurtMP);
	//				theTarget.setCurrentMP(currentMP);
	//			}
	//
	//			if (hurtMP < 0) {
	//				hurtAction(theTarget);
	//			}
	//
	//		}
	//
	//		if (theTarget.m_changeLifeTypePas == EFFECT_CHANGE_LIFE_TYPE_SKILL_STATUS_LOST) {
	//			// �ͷ�״̬����
	//			theTarget.removeAStatusAniGroup(hurt.dwData);
	//		}
	//
	//		if (!theTarget.hasMorePasStatus()) {
	//			theTarget.m_changeLifeTypePas = (EFFECT_CHANGE_LIFE_TYPE_NONE);
	//		}
	//	}
}

bool Battle::isActionCanBegin(FightAction* action)
{
	Fighter* fighter = action->m_pkActor;
	if (fighter)
	{
		if (!fighter->isAlive())
		{
			action->m_eActionStatus = ACTION_STATUS_FINISH;
			NDLog("fighter already dead,action skip");
			return false;
		}
		if (fighter->isActionOK() && !fighter->isHurtOK()
				&& !fighter->isDodgeOK() && !fighter->isDefenceOK()
				&& !fighter->isDieOK())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (action->m_eEffectType == EFFECT_TYPE_BATTLE_BEGIN)
		{
			TEAM_STATUS status;
			switch (action->m_nTeamAttack)
			{
			case 1:
				status = m_Team1_status;
				break;
			case 2:
				status = m_Team2_status;
				break;
			case 3:
				status = m_Team3_status;
				break;
			default:
				break;
			}
			if (status == TEAM_WAIT)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		return true;
	}
}

VEC_FIGHTER& Battle::getDefFightersByTeam(int team)
{
	VEC_FIGHTER v_team;
	for (VEC_FIGHTER::iterator it = m_vDefencer.begin();
			it != m_vDefencer.end(); it++)
	{
		Fighter* f = *it;
		if (f->m_kInfo.btBattleTeam == team)
		{

			v_team.push_back(f);
		}
	}
	return v_team;
}

void Battle::startAction(FightAction* pkFighterAction)
{
	switch (pkFighterAction->m_eEffectType)
	{
	case BATTLE_EFFECT_TYPE_ATK:
		if (pkFighterAction->m_pkActor->GetNormalAtkType() == ATKTYPE_NEAR)
		{
			pkFighterAction->m_pkActor->m_action = Fighter::MOVETOTARGET;
		}
		else if (pkFighterAction->m_pkActor->GetNormalAtkType() == ATKTYPE_DISTANCE)
		{
			pkFighterAction->m_pkActor->m_action = Fighter::AIMTARGET;
		}
		break;
	case BATTLE_EFFECT_TYPE_SKILL:
		if (pkFighterAction->m_pkSkill->getAtkType() == SKILL_ATK_TYPE_REMOTE)
		{
			pkFighterAction->m_pkActor->m_action = Fighter::AIMTARGET;
		}
		else
		{
			pkFighterAction->m_pkActor->m_action = Fighter::MOVETOTARGET;
		}
		break;
	case EFFECT_TYPE_BATTLE_BEGIN:
		//			NDLog("START_MOVE_TEAM");
		for (VEC_FIGHTER_IT it = m_vAttaker.begin(); it != m_vAttaker.end();
				it++)
		{
			Fighter* pkFighter = *it;
			if (pkFighter->m_kInfo.btBattleTeam == pkFighterAction->m_nTeamDefense)
			{
				pkFighter->m_action = Fighter::MOVETOTARGET;
				pkFighter->m_nTargetX = countX(m_teamAmout, pkFighter->m_kInfo.group,
						(pkFighterAction->m_nTeamDefense - 1) % 3 + 1, pkFighter->m_kInfo.btStations);
				pkFighter->m_nTargetY = countY(m_teamAmout, pkFighter->m_kInfo.group,
						(pkFighterAction->m_nTeamDefense - 1) % 3 + 1, pkFighter->m_kInfo.btStations);
				pkFighterAction->m_kFighterList.push_back(pkFighter);
			}
		}
		switch (pkFighterAction->m_nTeamAttack)
		{
		case 1:
			m_Team1_status = TEAM_FIGHT;
			break;
		case 2:
			m_Team2_status = TEAM_FIGHT;
			break;
		case 3:
			m_Team3_status = TEAM_FIGHT;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	pkFighterAction->m_eActionStatus = ACTION_STATUS_PLAY;
}

void Battle::runAction(int nTeamID)
{
	int nCurrentIndex = 0;
	VEC_FIGHTACTION* pkActionList;
	switch (nTeamID)
	{
	case 1:
		nCurrentIndex = m_currentActionIndex1;
		pkActionList = &(BattleMgrObj.m_vActionList1);
		break;
	case 2:
		nCurrentIndex = m_currentActionIndex2;
		pkActionList = &(BattleMgrObj.m_vActionList2);
		break;
	case 3:
		nCurrentIndex = m_currentActionIndex3;
		pkActionList = &(BattleMgrObj.m_vActionList3);
		break;
	default:
		break;
	}

	FightAction* fa = NULL;
	if (nCurrentIndex < pkActionList->size())
	{
		fa = pkActionList->at(nCurrentIndex);
	}
	else
	{	//actio�б���ȡ��
		//NDLog("%d no more action,battle end",teamId);
		switch (nTeamID)
		{
		case 1:
			m_Team1_status = TEAM_OVER;
			break;
		case 2:
			m_Team2_status = TEAM_OVER;
			break;
		case 3:
			m_Team3_status = TEAM_OVER;
			break;
		default:
			break;
		}
		return;
	}

	notifyNextFighterBeginAction();

	if (fa)
	{
		if (fa->m_pkActor)
		{
			fa->m_pkActor->actionTimeIncrease();
		}

		if (fa->m_eActionStatus == ACTION_STATUS_FINISH)
		{
			nCurrentIndex++;
			switch (nTeamID)
			{
			case 1:
				m_currentActionIndex1 = nCurrentIndex;
				break;
			case 2:
				m_currentActionIndex2 = nCurrentIndex;
				break;
			case 3:
				m_currentActionIndex3 = nCurrentIndex;
				break;
			default:
				break;
			}
			if (nCurrentIndex < pkActionList->size())
			{
				fa = pkActionList->at(nCurrentIndex);
				//NDLog("team:%d index %d",teamId,currentIndex);
				if (isActionCanBegin(fa))
				{
					startAction(fa);
				}
				else
				{
					return;
				}
			}
			else
			{
				return;
			}
		}
		else if (fa->m_eActionStatus == ACTION_STATUS_WAIT)
		{
			if (isActionCanBegin(fa))
			{
				startAction(fa);
			}
			else
			{
				return;
			}
		}

		switch (fa->m_eEffectType)
		{
		case BATTLE_EFFECT_TYPE_ATK:
			if (fa->m_pkActor->GetNormalAtkType() == ATKTYPE_NEAR)
			{
				moveToTarget(fa);
				//						if (f->m_mainTarget->protector) {
				//							moveToTarget(*f->m_mainTarget->protector);
				//						}
				normalAttack(fa);
				moveBack(fa);
				//						if (f->m_mainTarget->protector) {
				//							moveBack(*f->m_mainTarget->protector);
				//						}
			}
			else if (fa->m_pkActor->GetNormalAtkType() == ATKTYPE_DISTANCE)
			{
				aimTarget(fa);
				//						if (f->m_mainTarget->protector) {
				//							moveToTarget(fa->m_mainTarget->protector);
				//						}
				normalDistanceAttack(fa);
				distanceAttackOver(fa);
				//						if (f->m_mainTarget->protector) {
				//							moveBack(*fa->m_mainTarget->protector);
				//						}
			}
			break;
		case BATTLE_EFFECT_TYPE_SKILL:
			if (fa->m_pkSkill->getAtkType() == SKILL_ATK_TYPE_REMOTE)
			{
				aimTarget(fa);
				//						if (f->m_mainTarget->protector) {
				//							moveToTarget(*f->m_mainTarget->protector);
				//						}
				distanceSkillAttack(fa);
				distanceAttackOver(fa);
			}
			else
			{
				moveToTarget(fa);
				skillAttack(fa);
				moveBack(fa);
			}
			//						if (f->m_mainTarget->protector) {
			//							moveBack(*f->m_mainTarget->protector);
			//						}
			//					}
			break;
		case EFFECT_TYPE_BATTLE_BEGIN:				//ս����ʼʱ���Ѷ����ƶ���ս��λ
			moveTeam(fa);
			break;
			//			case EFFECT_TYPE_DEFENCE:
			//				defence(*f);
			//				break;
			//			case EFFECT_TYPE_ESCAPE:
			//				fleeSuccess(*f);
			//				break;
			//			case EFFECT_TYPE_ESCAPE_FAIL:
			//				fleeFail(*f);
			//				break;
			//			case EFFECT_TYPE_CATCH:
			//			case EFFECT_TYPE_CATCH_FAIL: {
			//				aimTarget(*f);
			//				catchPet(*f);
			//				distanceAttackOver(*f);
			//				break;
			//			}
		case EFFECT_TYPE_BATTLE_END:
			switch (fa->m_nTeamAttack)
			{
			case 1:
				m_Team1_status = TEAM_WAIT;
				break;
			case 2:
				m_Team2_status = TEAM_WAIT;
				break;
			case 3:
				m_Team3_status = TEAM_WAIT;
				break;
			default:
				break;
			}
			fa->m_eActionStatus = ACTION_STATUS_FINISH;
			break;
		case BATTLE_EFFECT_TYPE_STATUS_LIFE:				//״̬ȥѪ
			fa->m_pkActor->hurted(fa->m_nData);
			fa->m_pkActor->setCurrentHP((fa->m_pkActor->m_kInfo.nLife) + (fa->m_nData));
			if (fa->m_pkActor->m_kInfo.nLife > 0)
			{				// hurt
				fa->m_pkActor->setHurtOK(true);
				hurtAction(*(fa->m_pkActor));
			}
			else
			{				// die
				fa->m_pkActor->setDieOK(true);
				stringstream ss;
				ss << "die_action.spr";
				const char* file = NDPath::GetAniPath(ss.str().c_str()).c_str();
				NDAnimationGroup* dieAniGroup = new NDAnimationGroup;
				dieAniGroup->initWithSprFile(file);
				addSkillEffectToFighter(fa->m_pkActor, dieAniGroup, 0);
				fa->m_pkActor->showFighterName(false);
				dieAction(*(fa->m_pkActor));
			}
			fa->m_eActionStatus = ACTION_STATUS_FINISH;
			break;
		default:
			break;
		}
	}

}

void Battle::ShowFight()
{
	if (m_Team1_status != TEAM_OVER)
	{
		runAction(1);
	}
	if (m_Team2_status != TEAM_OVER)
	{
		runAction(2);
	}
	if (m_Team3_status != TEAM_OVER)
	{
		runAction(3);
	}
	if (m_Team1_status == TEAM_OVER && m_Team2_status == TEAM_OVER
			&& m_Team3_status == TEAM_OVER)
	{
		setBattleStatus (BS_FIGHTER_SHOW_PAS);
		//ReleaseActionList();
		ReleaseCommandList();

	}
	//	if (AllFighterActionOK()) {
	//		setBattleStatus(BS_FIGHTER_SHOW_PAS);
	//	}
}

void Battle::useItem(Fighter& theActor)
{
	if (theActor.m_action == Fighter::USEITEM)
	{	// ��ʱû��ʹ�õ��ߵĶ����������Ƕ��Լ�ʹ�ò�����Ʒ
		if (theActor.GetRole()->IsAnimationComplete())
		{
			Fighter& target = *theActor.m_pkMainTarget;

			Hurt hurt = target.getHurt(&theActor, 0, theActor.m_uiUsedItem,
					HURT_TYPE_ACTIVE).second;

			int hurtHP = hurt.hurtHP;

			if (hurtHP > 0)
			{
				int currentHP = target.m_kInfo.nLife;
				int addHP = hurtHP;
				currentHP += addHP;
				if (currentHP > target.m_kInfo.nLifeMax)
				{
					currentHP = target.m_kInfo.nLifeMax;
				}
				target.hurted(addHP);
				if (!IsPracticeBattle())
				{
					target.setCurrentHP(currentHP);
				}
				//������
				if (!target.isAlive())
				{
					target.setAlive(true);
					clearFighterStatus(target);
					battleStandAction(target);
					watchBattle = false;
				}
			}

			int hurtMP = hurt.hurtMP;

			if (hurtMP > 0)
			{
				int currentMP = target.m_kInfo.nMana;
				int addMP = hurtMP;
				currentMP += addMP;
				if (currentMP > target.m_kInfo.nManaMax)
				{
					currentMP = target.m_kInfo.nManaMax;
				}
				target.hurted(addMP);
				target.setCurrentMP(currentMP);
			}
			// �����󴥷�״̬�仯
			handleStatusActions(theActor.getArrayStatusTarget());
			theActor.m_action = (Fighter::DISTANCEATTACKOVER);
			battleStandAction(theActor);
		}
	}
}

void Battle::catchPet(Fighter& f)
{
	//	if (f.m_action == Fighter::CATCH_PET) {
	//		if (f.GetRole()->IsAnimationComplete()) {
	//			// ��ɲ�׽,�˴������ ����׽�Ķ��󶯻�
	//			if (f.m_effectType == EFFECT_TYPE_CATCH) {
	//				f.m_mainTarget->setAlive(false);
	//				Chat::DefaultChat()->AddMessage(ChatTypeSystem, NDCommonCString("CatchSucc"));
	//			} else if (f.m_effectType == EFFECT_TYPE_CATCH_FAIL) {
	//				Chat::DefaultChat()->AddMessage(ChatTypeSystem, NDCommonCString("CatchFail"));
	//			}
	//			// �����󴥷�״̬�仯
	//			handleStatusActions(theActor->getArrayStatusTarget());
	//			// ������һ״̬
	//			f.m_action = (Fighter::DISTANCEATTACKOVER);
	//			battleStandAction(f);
	//		}
	//	}
}

void Battle::defence(Fighter& theActor)
{
	if (theActor.m_action == Fighter::DEFENCE)
	{
		if (theActor.GetRole()->IsAnimationComplete())
		{
			theActor.setActionOK(true);
			// �����󴥷�״̬�仯
			handleStatusActions(theActor.getArrayStatusTarget());
		}
	}
}

void Battle::fleeFail(Fighter& theActor)
{
	if (theActor.m_action == Fighter::FLEE_FAIL)
	{
		if (!bActionSet)
		{
			fleeFailAction(theActor);
			bActionSet = true;
		}
		if (theActor.GetRole()->IsAnimationComplete())
		{
			theActor.setActionOK(true);
			// �����󴥷�״̬�仯
			handleStatusActions(theActor.getArrayStatusTarget());
			battleStandAction(theActor);
			bActionSet = false;
		}
	}
}

void Battle::fleeSuccess(Fighter& theActor)
{
	if (theActor.m_action == Fighter::FLEE_SUCCESS)
	{
		if (!bActionSet)
		{
			bActionSet = true;
			fleeSuccessAction(theActor);
		}
		if (theActor.GetRole()->IsAnimationComplete())
		{
			if (theActor.m_kInfo.idObj == NDPlayer::defaultHero().m_nID)
			{	// ������Լ������˻�������
				// ���͹�ս
				watchBattle = true;
				//				m_imgWhoAmI->RemoveFromParent(true);
				//				m_imgWhoAmI = NULL;
			}
			// �����󴥷�״̬�仯
			handleStatusActions(theActor.getArrayStatusTarget());
			theActor.setActionOK(true);
			theActor.setEscape(true);
			bActionSet = false;
		}
	}
}

void Battle::moveTeam(FightAction* action)
{

	VEC_FIGHTER* v_fighters = &(action->m_kFighterList);
	bool isOk = true;
	if (v_fighters)
	{
		for (int i = 0; i < v_fighters->size(); i++)
		{
			Fighter* f = v_fighters->at(i);
			if (f->StandInOrigin())
			{
				moveToTargetAction(*f);
			}
			if (f->moveTo(f->m_nTargetX, f->m_nTargetY))
			{
				f->m_action = (Fighter::WAIT);
				battleStandAction(*f);
				f->setOriginPos(f->m_nTargetX, f->m_nTargetY);
				f->setActionOK(true);
			}
			else
			{
				isOk = false;
			}
		}
	}
	if (isOk)
	{
		action->m_eActionStatus = ACTION_STATUS_FINISH;
		for (int i = 0; i < action->m_vCmdList.size(); i++)
		{
			FIGHTER_CMD* cmd = action->m_vCmdList.at(i);
			dealWithFighterCmd(cmd);
		}
	}
}

void Battle::moveToTarget(FightAction* action)
{
	Fighter* theActor = action->m_pkActor;
	if (theActor->m_action == Fighter::MOVETOTARGET)
	{

		NDBaseRole* role = theActor->GetRole();

		int coordw = role->GetWidth() >> 1;
		//		if (theActor.m_kInfo.group != m_ourGroup) {
		//			coordw = -coordw;
		//		}
		if (theActor->m_kInfo.group == BATTLE_GROUP_ATTACK)
		{
			coordw = -coordw;
		}

		if (theActor->m_actionType == Fighter::ACTION_TYPE_PROTECT)
		{
			coordw = -coordw;
		}

		if (theActor->StandInOrigin())
		{
			moveToTargetAction(*(action->m_pkActor));
		}

		int roleOffset = 0;

		//		if (role->IsKindOfClass(RUNTIME_CLASS(NDManualRole)) &&
		//		    theActor.m_mainTarget->GetRole()->IsKindOfClass(RUNTIME_CLASS(NDManualRole))) {
		//			roleOffset = theActor.m_kInfo.group == BATTLE_GROUP_ATTACK ? 60 : -60;
		//		}
		if (action->m_pkTarget)
		{
			if (theActor->moveTo(
					action->m_pkTarget->getOriginX() + coordw + roleOffset,
					action->m_pkTarget->getOriginY()))
			{		// ������ص���Ŀ�ĵ�
				if (action->m_eEffectType == BATTLE_EFFECT_TYPE_ATK)
				{
					theActor->m_action = (Fighter::ATTACK);
					attackAction(*theActor);
				}
				else if (action->m_eEffectType == BATTLE_EFFECT_TYPE_SKILL)
				{

					theActor->m_action = (Fighter::SKILLATTACK);
					//�����ܶ���
					BattleSkill* skill = action->m_pkSkill;
					theActor->setSkillName(skill->getName());
					theActor->showSkillName(true);
					int actId = skill->GetActId();
//					if(theActor->m_lookfaceType==LOOKFACE_MANUAL){
					roleAction(*theActor, MANUELROLE_ATTACK);
//					}else{
//						petAction(*theActor, actId);
//					}
					//�����ܹ�Ч
//					int effectId=skill->GetLookfaceID()/100;
//					if(effectId!=0){//��Ч��������������
//						int delay=skill->GetLookfaceID()%100;
//						stringstream ss;
//						ss << "effect_" << effectId << ".spr";
//						NSString* file = [NSString stringWithUTF8String:GetAniPath(ss.str().c_str())];
//						NDAnimationGroup* effect = [[NDAnimationGroup alloc] initWithSprFile:file];
//						NDLog("add self effect");
//						addSkillEffectToFighter(theActor,effect,delay);
//					}
//					effectId=skill->GetLookfaceTargetID()/100;//��Ч������Ŀ������
//					if(effectId!=0){
//						int delay=skill->GetLookfaceTargetID()%100;
//						stringstream ss;
//						ss << "effect_" << effectId << ".spr";
//						NSString* file = [NSString stringWithUTF8String:GetAniPath(ss.str().c_str())];
//						NDAnimationGroup* effect = [[NDAnimationGroup alloc] initWithSprFile:file];
//						for (int i = 0; i < action->m_FighterList.size(); i++)
//						{	
//							Fighter* f=action->m_FighterList.at(i);
//							addSkillEffectToFighter(f, effect,delay);
//						}
//					}
				}
			}
		}
		else
		{
			if (action->m_eEffectType == BATTLE_EFFECT_TYPE_ATK)
			{
				theActor->m_action = (Fighter::ATTACK);
				attackAction(*theActor);
			}
			else if (action->m_eEffectType == BATTLE_EFFECT_TYPE_SKILL)
			{

				theActor->showSkillName(true);
				theActor->m_action = (Fighter::SKILLATTACK);
				//�����ܶ���
				BattleSkill* skill = action->m_pkSkill;
				int actId = skill->GetActId();
//				if(theActor->m_lookfaceType==LOOKFACE_MANUAL){
				roleAction(*theActor, MANUELROLE_ATTACK);
//				}else{
//					petAction(*theActor, 0);
//				}
				//�����ܹ�Ч
//				int effectId=skill->GetLookfaceID()/100;
//				if(effectId!=0){//��Ч��������������
//					int delay=skill->GetLookfaceID()%100;
//					stringstream ss;
//					ss << "effect_" << effectId << ".spr";
//					NSString* file = [NSString stringWithUTF8String:GetAniPath(ss.str().c_str())];
//					NDAnimationGroup* effect = [[NDAnimationGroup alloc] initWithSprFile:file];
//					
//					addSkillEffectToFighter(theActor,effect,delay);
//				}
//				effectId=skill->GetLookfaceTargetID()/100;//��Ч������Ŀ������
//				if(effectId!=0){
//					int delay=skill->GetLookfaceTargetID()%100;
//					stringstream ss;
//					ss << "effect_" << effectId << ".spr";
//					NSString* file = [NSString stringWithUTF8String:GetAniPath(ss.str().c_str())];
//					NDAnimationGroup* effect = [[NDAnimationGroup alloc] initWithSprFile:file];
//					for (int i = 0; i < action->m_FighterList.size(); i++)
//					{	
//						NDLog("add effect");
//						Fighter* f=action->m_FighterList.at(i);
//						addSkillEffectToFighter(f, effect,delay);
//					}
//				}
			}
		}
	}
}

void Battle::dealWithFighterCmd(FIGHTER_CMD* cmd)
{
	Fighter* fighter = GetFighter(cmd->actor);
	NDLog("%d cmd:%d", cmd->actor, cmd->effect_type);
	if (fighter)
	{
		switch (cmd->effect_type)
		{
		case BATTLE_EFFECT_TYPE_LIFE:
			fighter->m_bHardAtk = false;
			fighter->hurted(cmd->data);
			NDLog("hurt %d", cmd->data);
			fighter->setCurrentHP((fighter->m_kInfo.nLife) + (cmd->data));
			if (fighter->m_kInfo.nLife > 0)
			{				// hurt
				fighter->setHurtOK(true);
				if (cmd->data < 0)
				{
					hurtAction(*fighter);
				}
			}
			else
			{				// die
				fighter->setDieOK(true);
				dieAction(*fighter);
				stringstream ss;
				ss << "die_action.spr";
				const char* file = NDPath::GetAniPath(ss.str().c_str()).c_str();
				NDAnimationGroup* dieAniGroup = new NDAnimationGroup;
				dieAniGroup->initWithSprFile(file);
				fighter->showFighterName(false);
				addSkillEffectToFighter(fighter, dieAniGroup, 0);
			}
			break;
		case BATTLE_EFFECT_TYPE_MANA:
			fighter->setCurrentMP((fighter->m_kInfo.nMana) + (cmd->data));
			break;
		case BATTLE_EFFECT_TYPE_DODGE:
			fighter->setDodgeOK(true);
			dodgeAction(*fighter);
			break;
		case BATTLE_EFFECT_TYPE_DRITICAL:
			fighter->m_bHardAtk = true;
			//				fighter->hurted(cmd->data);
			//				fighter->setCurrentHP((fighter->m_kInfo.nLife)+(cmd->data));
			//				if (fighter->m_kInfo.nLife > 0) {// hurt
			//					fighter->setHurtOK(true);
			//					if(cmd->data<0){
			//						hurtAction(*fighter);
			//					}
			//				} else {// die
			//					fighter->setDieOK(true);
			//					dieAction(*fighter);
			//				}
			break;
		case BATTLE_EFFECT_TYPE_BLOCK:
			fighter->setDefenceOK(true);
			defenceAction(*fighter);
			break;
		case BATTLE_EFFECT_TYPE_STATUS_ADD:
//				if (cmd->status->m_LastEffectID  != 999) {
//					stringstream ss;
//					ss << "effect_" << cmd->status->m_LastEffectID  << ".spr";
//					NDAsssert(cmd->status->m_aniGroup == NULL);
//					//				ss << "effect_" << cmd->status->m_StartEffectID << ".spr";
//					NSString* file = [NSString stringWithUTF8String:GetAniPath(ss.str().c_str())];
//					cmd->status->m_aniGroup=new NDSubAniGroup;
//					NDAnimation* effect=[[NDAnimationGroup alloc] initWithSprFile:file];
//					cmd->status->m_aniGroup->aniGroup = [effect retain];
//					//subAniGroup.aniGroup.position=CGPointMake(x, y);
//					
//					cmd->status->m_aniGroup->role = fighter->GetRole();
//					
//					cmd->status->m_aniGroup->fighter = fighter;
//					cmd->status->m_aniGroup->frameRec = [[NDFrameRunRecord alloc] init];
//					[effect release];
//				}
//				fighter->addAStatus(cmd->status);
//				if(cmd->status->m_StartEffectID!=0){
//					stringstream ss;
//					ss << "effect_" << cmd->status->m_StartEffectID << ".spr";
//					NSString* file = [NSString stringWithUTF8String:GetAniPath(ss.str().c_str())];
//					NDAnimationGroup* effect = [[NDAnimationGroup alloc] initWithSprFile:file];
//					
//					addSkillEffectToFighter(fighter,effect,0);
//				}
			break;
		case BATTLE_EFFECT_TYPE_STATUS_LOST:
//				fighter->removeAStatusAniGroup(cmd->status);
			break;
		default:
			break;
		}
	}
}

void Battle::normalAttack(FightAction* action)
{
	Fighter* theActor = action->m_pkActor;
	if (theActor->m_action == Fighter::ATTACK)
	{
		NDBaseRole* role = theActor->GetRole();
		if (role->IsAnimationComplete())
		{
			NDLog("%d:normalATK", theActor->m_kInfo.idObj);
			for (int i = 0; i < action->m_vCmdList.size(); i++)
			{
				FIGHTER_CMD* cmd = action->m_vCmdList.at(i);
				dealWithFighterCmd(cmd);
			}
			//			theActor.m_vCmdList.clear();
			//			Fighter* theTarget = theActor.m_mainTarget;
			//			if (theActor.m_bMissAtk) {
			//				theTarget->setDodgeOK(true);
			//				dodgeAction(*theTarget);
			//				
			//				if (role->IsAnimationComplete()) {
			//					theActor.m_action = (Fighter::MOVEBACK);
			//					moveBackAction(theActor);
			//				}
			//			} else {
			//				int currentHP = theTarget->m_kInfo.nLife;
			//				int hurtHP = theTarget->getHurt(&theActor, 1, 0, HURT_TYPE_ACTIVE).second.hurtHP;
			//				currentHP += hurtHP;
			//				theTarget->hurted(hurtHP);
			//				if (!IsPracticeBattle()) {
			//					theTarget->setCurrentHP(currentHP);
			//				}
			//				if (theTarget->m_kInfo.nLife > 0) {// hurt
			//					theTarget->setHurtOK(true);
			//					hurtAction(*theTarget);
			//				} else {// die
			//					theTarget->setDieOK(true);
			//					dieAction(*theTarget);
			//				}
			//				theActor.m_action = (Fighter::MOVEBACK);
			//				moveBackAction(theActor);
			//				if (theTarget->protector) {
			//					theTarget->protector->m_action = (Fighter::MOVEBACK);
			//					moveBackAction(*theTarget->protector);
			//					theTarget->protector->hurted(theTarget->protector->hurtInprotect);
			//					theTarget->protector->hurtInprotect = 0;
			//				}
			//			}
			//			// �����󴥷�״̬�仯
			//			handleStatusActions(theActor.getArrayStatusTarget());
			if (!action->m_bIsCombo)
			{
				theActor->m_action = (Fighter::MOVEBACK);
				moveBackAction(*theActor);
			}
			else
			{
				theActor->m_action = (Fighter::WAIT);
				theActor->setActionOK(true);
				battleStandAction(*theActor);
				action->m_eActionStatus = ACTION_STATUS_FINISH;
			}
		}
	}
}

void Battle::moveBack(FightAction* action)
{
	Fighter* theActor = action->m_pkActor;
	if (theActor->m_action == Fighter::MOVEBACK)
	{
		if (theActor->moveTo(theActor->getOriginX(), theActor->getOriginY()))
		{
			theActor->m_action = (Fighter::WAIT);
			theActor->setActionOK(true);
			battleStandAction(*theActor);
			action->m_eActionStatus = ACTION_STATUS_FINISH;
			//			if (theActor.protectTarget) {
			//				theActor.protectTarget->protector = NULL;
			//				theActor.protectTarget = NULL;
			//			}
		}
	}
}

void Battle::addSkillEffectToFighter(Fighter* fighter, NDAnimationGroup* effect,
		int delay)
{
	//	Fighter* theActor=action->m_Actor;

	//	//�����ܶ���
	//	int actId=0;
	//	if(fighter>m_role->IsKindOfClass(RUNTIME_CLASS(NDManualRole))){
	//		roleAction(*fighter, actId);
	//	}else{
	//		petAction(*fighter, actId);
	//	}
	//	   
	//	//�����Ч
	//	int effectId=0;
	//	int effectDelay=0;
	//	if (effectId > 899) return;

	//	stringstream ss;
	//	ss << "effect_" << effectId << ".spr";
	//	NSString* file = [NSString stringWithUTF8String:GetAniPath(ss.str().c_str())];
	//	NDAnimationGroup* effect = [[NDAnimationGroup alloc] initWithSprFile:file];

	NDLog("add skill effect");
	NDSubAniGroup sa;
	sa.role = fighter->GetRole();
	sa.fighter = fighter;
	sa.aniGroup = effect;
	sa.frameRec = new NDFrameRunRecord;
	sa.isFromOut = true;
	sa.startFrame = delay;
	m_vSubAniGroup.push_back(sa);

	//effect->release();
}

void Battle::addSkillEffect(Fighter& theActor, bool user/*=false*/)
{
//	int skillId = theActor.getUseSkill()->getId();
//	// ����
//	int actId = skillId / 1000000;
//	
//	NDBaseRole *role = theActor.GetRole();
//	
//	if( user && role && (actId == 24 || actId == 22) )
//	{
//		actId = role->GetWeaponType() == ONE_HAND_WEAPON ? 22 : 24;
//	}
//	
//	if (user)
//		roleAction(theActor, actId);
//	else
//		petAction(theActor, actId);
//	// ��ЧidΪ999��ʱ��Ϊ�޹�Ч
//	if (skillId != 999) {
//		int effectId = skillId / 1000 % 1000;
//		int effectDelay = skillId / 10 % 100;
//		
//		if (effectId > 899) return;
//		
//		stringstream ss;
//		ss << "effect_" << effectId << ".spr";
//		NSString* file = [NSString stringWithUTF8String:GetAniPath(ss.str().c_str())];
//		NDAnimationGroup* effect = [[NDAnimationGroup alloc] initWithSprFile:file];
//		
//		int target = skillId % 10;
//		switch (target) {
//			case 0:// 0��Ч��Ŀ������
//			{
//				VEC_FIGHTER& allTarget = theActor.getArrayTarget();
//				int size = allTarget.size();
//				for (int i = 0; i < size; i++) {
//					if (allTarget.at(i)->m_kInfo.group == theActor.m_mainTarget->m_kInfo.group) {
//						NDSubAniGroup sa;
//						sa.role = theActor.GetRole();
//						sa.fighter = allTarget.at(i);
//						sa.aniGroup = [effect retain];
//						sa.frameRec = [[NDFrameRunRecord alloc] init];
//						sa.isFromOut = true;
//						sa.startFrame = effectDelay;
//						m_vSubAniGroup.push_back(sa);
//					}
//				}
//			}
//				break;
//			case 1: // 1��Ч��ʩ��������
//			{
//				NDSubAniGroup sa;
//				sa.role = theActor.GetRole(); // add by jhzheng 2011.10.13
//				sa.fighter = &theActor;
//				sa.aniGroup = [effect retain];
//				sa.isFromOut = true;
//				sa.frameRec = [[NDFrameRunRecord alloc] init];
//				m_vSubAniGroup.push_back(sa);
//				sa.startFrame = effectDelay;
//				break;
//			}
//			default:
//				break;
//		}
//		
//		[effect release];
//	}
}

void Battle::aimTarget(FightAction* action)
{
	Fighter* theActor = action->m_pkActor;
	if (theActor->m_action == Fighter::AIMTARGET)
	{
		if (action->m_eEffectType == BATTLE_EFFECT_TYPE_ATK)
		{
			theActor->m_action = (Fighter::DISTANCEATTACK);
			attackAction(*theActor);
		}
		else if (action->m_eEffectType == BATTLE_EFFECT_TYPE_SKILL)
		{
			theActor->m_action = (Fighter::DISTANCESKILLATTACK);
			//�����ܶ���
			BattleSkill* skill = action->m_pkSkill;
			theActor->setSkillName(skill->getName());
			theActor->showSkillName(true);
			int actId = skill->GetActId();
//			if(theActor->m_lookfaceType==LOOKFACE_MANUAL){
			roleAction(*theActor, MANUELROLE_ATTACK);
//			}else{
//				petAction(*theActor, 0);
//			}
			//�����ܹ�Ч
//			int effectId=skill->GetLookfaceID()/100;
//			if(effectId!=0){//��Ч��������������
//				int delay=skill->GetLookfaceID()%100;
//				stringstream ss;
//				ss << "effect_" << effectId << ".spr";
//				NSString* file = [NSString stringWithUTF8String:GetAniPath(ss.str().c_str())];
//				NDAnimationGroup* effect = [[NDAnimationGroup alloc] initWithSprFile:file];
//				
//				addSkillEffectToFighter(theActor,effect,delay);
//			}
//			effectId=skill->GetLookfaceTargetID()/100;//��Ч������Ŀ������
//			if(effectId!=0){
//				int delay=skill->GetLookfaceTargetID()%100;
//				stringstream ss;
//				ss << "effect_" << effectId << ".spr";
//				NSString* file = [NSString stringWithUTF8String:GetAniPath(ss.str().c_str())];
//				NDAnimationGroup* effect = [[NDAnimationGroup alloc] initWithSprFile:file];
//				for (int i = 0; i < action->m_FighterList.size(); i++)
//				{	
//					Fighter* f=action->m_FighterList.at(i);
//					addSkillEffectToFighter(f, effect,delay);
//				}
//			}

			//			if (theActor.m_kInfo.fighterType == FIGHTER_TYPE_PET) { // ���
			//				/*
			//				int skillId = theActor.getUseSkill()->getSkillTypeID();
			//				while (skillId >= 100) {
			//					skillId = skillId /10;
			//				}
			//				int prof = skillId % 10;
			//				if (prof == PROFESSION_TYPE_SOLDIER) {
			//					warriorSkillAction(theActor);
			//				} else if (prof == PROFESSION_TYPE_ASN) {
			//					assasinSkillAction(theActor);
			//				} else if (prof == PROFESSION_TYPE_MAGE) {
			//					wizzardSkillAction(theActor);
			//				}
			//				*/
			//				// �µ���Ҽ��ܱ���(������)
			//				addSkillEffect(theActor, true);
			//			} else { // ��������
			//				addSkillEffect(theActor);
			//			}
		}
		//		else if (theActor.m_actionType == Fighter::ACTION_TYPE_USEITEM) {
		////			theActor.m_action = (Fighter::USEITEM);
		////			useItemAction(theActor);
		//		} else if (theActor.m_actionType == Fighter::ACTION_TYPE_CATCH) { //��Ӳ�׽����
		//			theActor.m_action = (Fighter::CATCH_PET);
		//			catchPetAction(theActor);
		//		}
	}
}

void Battle::normalDistanceAttack(FightAction* action)
{
	Fighter* theActor = action->m_pkActor;
	if (theActor->m_action == Fighter::DISTANCEATTACK)
	{
		NDBaseRole* role = theActor->GetRole();
		if (role->IsAnimationComplete())
		{
			NDLog("%d:normalDistanceATK", theActor->m_kInfo.idObj);
			for (int i = 0; i < action->m_vCmdList.size(); i++)
			{
				FIGHTER_CMD* cmd = action->m_vCmdList.at(i);
				dealWithFighterCmd(cmd);
			}
			//			if (theActor.m_bMissAtk) {
			//				target->setDodgeOK(true);
			//				dodgeAction(*target);
			//				if (role->IsAnimationComplete()) {
			//					theActor.m_action = (Fighter::DISTANCEATTACKOVER);
			//					battleStandAction(theActor);
			//				}
			//			} else {
			//				int currentHP = target->m_kInfo.nLife;
			//				int hurtHP = target->getHurt(&theActor, 1, 0, HURT_TYPE_ACTIVE).second.hurtHP;
			//				currentHP += hurtHP;
			//				target->hurted(hurtHP);
			//				if (!IsPracticeBattle()) {
			//					target->setCurrentHP(currentHP);
			//				}
			//				if (target->m_kInfo.nLife > 0) {// hurt
			//					target->setHurtOK(true);
			//					hurtAction(*target);
			//					battleStandAction(theActor);
			//				} else {// die
			//					target->setDieOK(true);
			//					dieAction(*target);
			//				}
			//				theActor.m_action = (Fighter::DISTANCEATTACKOVER);
			//				if (target->protector) {
			//					target->protector->m_action = (Fighter::MOVEBACK);
			//					moveBackAction(*target->protector);
			//					target->protector->hurted(target->protector->hurtInprotect);
			//					target->protector->hurtInprotect = 0;
			//				}
			//			}
			// �����󴥷�״̬�仯
			//			handleStatusActions(theActor.getArrayStatusTarget());
			if (!action->m_bIsCombo)
			{
				theActor->m_action = (Fighter::DISTANCEATTACKOVER);
				//				moveBackAction(*theActor);
			}
			else
			{
				theActor->m_action = (Fighter::WAIT);
				theActor->setActionOK(true);
				battleStandAction(*theActor);
				action->m_eActionStatus = ACTION_STATUS_FINISH;
			}
		}
	}
}

void Battle::distanceAttackOver(FightAction* action)
{
	Fighter* theActor = action->m_pkActor;
	if (theActor->m_action == Fighter::DISTANCEATTACKOVER)
	{
		if (theActor->GetRole()->IsAnimationComplete())
		{
			theActor->m_action = (Fighter::WAIT);
			theActor->setActionOK(true);
			battleStandAction(*theActor);
			action->m_eActionStatus = ACTION_STATUS_FINISH;
		}
	}
}

void Battle::fighterSomeActionChangeToWait(VEC_FIGHTER& fighterList)
{
	for (size_t i = 0; i < fighterList.size(); i++)
	{
		Fighter& f = *(fighterList.at(i));
		if (!f.isAlive() || !f.isVisiable())
		{
			f.setHurtOK(false);
			f.setDieOK(false);
			f.setDodgeOK(false);
			f.setDefenceOK(false);
			continue;
		}
		if (f.isHurtOK())
		{ //����������
			if (f.GetRole()->IsAnimationComplete())
			{
				if (f.m_action == Fighter::DEFENCE)
				{
					defenceAction(f);
				}
				else
				{
					battleStandAction(f);
				}
				f.setHurtOK(false);
			}
		}
		if (f.isDieOK())
		{ //��������
			if (f.GetRole()->IsAnimationComplete())
			{
				if (f.m_kInfo.idObj == NDPlayer::defaultHero().m_nID)
				{ // ������Լ�����
					// ���͹�ս
					watchBattle = true;
				}
				f.setActionOK(true);					//���鶯������
				f.setAlive(false);
				f.setDieOK(false);
			}
		}

		if (f.isDodgeOK())
		{ //���ܹ���
			if (f.GetRole()->IsAnimationComplete())
			{

				if (f.m_action == Fighter::DEFENCE)
				{
					defenceAction(f);
				}
				else
				{
					battleStandAction(f);
				}
				f.setDodgeOK(false);
			}
		}

		if (f.isDefenceOK())
		{
			if (f.GetRole()->IsAnimationComplete())
			{
				f.m_bDefenceAtk = false;
				if (f.m_action == Fighter::DEFENCE)
				{
					defenceAction(f);
				}
				else
				{
					battleStandAction(f);
				}
				f.setDefenceOK(false);
			}
		}
	}
}

void Battle::drawFighterHurt(VEC_FIGHTER& fighterList)
{
	for (size_t i = 0; i < fighterList.size(); i++)
	{
		Fighter& f = *fighterList.at(i);
		f.drawHurtNumber();
	}
}

void Battle::drawAllFighterHurtNumber()
{
	drawFighterHurt (m_vAttaker);
	drawFighterHurt (m_vDefencer);
}

NDPicture* Battle::getActionWord(ACTION_WORD index)
{
	switch (index)
	{
	case AW_DEF:
		return m_picActionWordDef;
	case AW_FLEE:
		return m_picActionWordFlee;
	case AW_DODGE:
		return m_picActionWordDodge;
	default:
		return NULL;
	}
}

void Battle::skillAttack(FightAction* action)
{
	Fighter* theActor = action->m_pkActor;
	BattleSkill* skill = action->m_pkSkill;
	if (theActor->m_action == Fighter::SKILLATTACK)
	{
		if (theActor->GetRole()->IsAnimationComplete())
		{
			NDLog("%d:SkillATK", theActor->m_kInfo.idObj);
			for (int i = 0; i < action->m_vCmdList.size(); i++)
			{
				FIGHTER_CMD* cmd = action->m_vCmdList.at(i);
				dealWithFighterCmd(cmd);
			}
			//			VEC_FIGHTER& arrayTarget = theActor.getArrayTarget();
			//			
			//			for (size_t i = 0; i < arrayTarget.size(); i++) {
			//				Fighter& target = *(arrayTarget.at(i));
			//				if (target.protector) {
			//					target.protector->m_action = (Fighter::MOVEBACK);
			//					moveBackAction(*target.protector);
			//					target.protector->hurted(target.protector->hurtInprotect);
			//					target.protector->hurtInprotect = 0;
			//				}
			//				int currentHP = target.m_kInfo.nLife;
			//				
			//				Hurt hurt = target.getHurt(&theActor, 0, theActor.getUseSkill()->getId(), HURT_TYPE_ACTIVE).second;
			//				int hurtHP = hurt.hurtHP;
			//				currentHP += hurtHP;
			//				target.hurted(hurtHP);
			//				if (!IsPracticeBattle()) {
			//					target.setCurrentHP(currentHP);
			//				}
			//				currentHP = target.m_kInfo.nLife;
			//				if (hurtHP <= 0) {
			//					if (currentHP > 0) {// hurt
			//						target.setHurtOK(true);
			//						hurtAction(target);
			//					} else {// die
			//						target.setDieOK(true);
			//						dieAction(target);
			//					}
			//				}
			//			}
			//			
			//			// �����󴥷�״̬�仯
			//			handleStatusActions(theActor.getArrayStatusTarget());

			theActor->m_action = (Fighter::MOVEBACK);
			moveBackAction(*theActor);
		}
	}
}

void Battle::distanceSkillAttack(FightAction* action)
{
	Fighter* theActor = action->m_pkActor;
	if (theActor->m_action == Fighter::DISTANCESKILLATTACK)
	{
		if (theActor->GetRole()->IsAnimationComplete())
		{
			NDLog("%d:distanceSkillATK", theActor->m_kInfo.idObj);
			for (int i = 0; i < action->m_vCmdList.size(); i++)
			{
				FIGHTER_CMD* cmd = action->m_vCmdList.at(i);
				dealWithFighterCmd(cmd);
			}

			//			VEC_FIGHTER& arrayTarget = theActor.getArrayTarget();
			//			
			//			for (size_t i = 0; i < arrayTarget.size(); i++) {
			//				Fighter& target = *(arrayTarget.at(i));
			//				if (target.protector) {
			//					target.protector->m_action = (Fighter::MOVEBACK);
			//					moveBackAction(*target.protector);
			//					target.protector->hurted(target.protector->hurtInprotect);
			//					target.protector->hurtInprotect = 0;
			//				}
			//				int currentHP = target.m_kInfo.nLife;
			//				
			//				Hurt hurt = target.getHurt(&theActor, 0, theActor.getUseSkill()->getId(), HURT_TYPE_ACTIVE).second;
			//				int hurtHP = hurt.hurtHP;
			//				currentHP += hurtHP;
			//				target.hurted(hurtHP);
			//				if (!IsPracticeBattle()) {
			//					target.setCurrentHP(currentHP);
			//				}
			//				currentHP = target.m_kInfo.nLife;
			//				if (hurtHP < 0) {
			//					if (currentHP > 0) {// hurt
			//						target.setHurtOK(true);
			//						hurtAction(target);
			//					} else {// die
			//						target.setDieOK(true);
			//						dieAction(target);
			//					}
			//				}
			//				
			//				// ������
			//				if (!target.isAlive() && hurtHP > 0) {
			//					target.setAlive(true);
			//					clearFighterStatus(target);
			//					battleStandAction(target);
			//					watchBattle = false;
			//				}
			//			}
			//			
			//			// �����󴥷�״̬�仯
			//			handleStatusActions(theActor.getArrayStatusTarget());

			theActor->m_action = (Fighter::DISTANCEATTACKOVER);
			battleStandAction(*theActor);
		}
	}
}

void Battle::clearWillBeAtk(VEC_FIGHTER& fighterList)
{
	for (size_t i = 0; i < fighterList.size(); i++)
	{
		Fighter& f = *fighterList.at(i);
		f.setWillBeAtk(false);
	}
}

void Battle::clearAllWillBeAtk()
{
	clearWillBeAtk(GetOurSideList());
	clearWillBeAtk(GetEnemySideList());
}

Fighter* Battle::getFighterByPos(VEC_FIGHTER& fighterList, int pos, int line)
{
	Fighter* result = NULL;
	for (size_t i = 0; i < fighterList.size(); i++)
	{
		Fighter* f = fighterList.at(i);
		if (f->m_kInfo.btStations == pos)
		{
			result = f;
			break;
		}
	}
	return result;
}

Fighter* Battle::getUpNearFighter(VEC_FIGHTER& fighterList, Fighter* f)
{			// ���������ŵ�һ��fighter
	Fighter* result = NULL;
	if (!f)
	{
		return NULL;
	}

	if (f->m_kInfo.btStations == 1 || f->m_kInfo.btStations == 4
			|| f->m_kInfo.btStations == 7)
	{			// ������
		result = NULL;
	}
	else
	{
		Byte pos = f->m_kInfo.btStations;
		result = getFighterByPos(fighterList, pos - 1, 0);
	}
	return result;
}

Fighter* Battle::getDownNearFighter(VEC_FIGHTER& fighterList, Fighter* f)
{			// ���������ŵ�һ��fighter
	Fighter* result = NULL;
	if (f == NULL)
	{
		return NULL;
	}
	if (f->m_kInfo.btStations == 3 || f->m_kInfo.btStations == 6
			|| f->m_kInfo.btStations == 9)
	{			// ������
		result = NULL;
	}
	else
	{
		Byte pos = f->m_kInfo.btStations;
		result = getFighterByPos(fighterList, pos + 1, 0);
	}
	return result;
}

void Battle::setWillBeAtk(VEC_FIGHTER& fighterList)
{
//	clearAllWillBeAtk();
//	int area = 0;
//	
//	switch (m_battleStatus) {
//		case BS_CHOOSE_ENEMY_MAG_ATK: // ��Ҽ��ܹ���
//		case BS_CHOOSE_OUR_SIDE_MAG_ATK:
//		{
//			if (GetMainUser() == NULL) return;
//			
//			BattleSkill* useSkill = GetMainUser()->getUseSkill();
//			if (!useSkill) {
//				return;
//			}
//			area = useSkill->getArea();
//		}
//			break;
//		case BS_CHOOSE_ENEMY_MAG_ATK_EUDEMON:
//		{
//			BattleSkill* useSkill = getMainEudemon()->getUseSkill();
//			if (!useSkill) {
//				return;
//			}
//			area = useSkill->getArea();
//		}
//			break;
//		default:
//			return;
//	}
//	
//	if (!m_highlightFighter) {
//		return;
//	}
//	
//	Fighter* f = m_highlightFighter;
//	m_highlightFighter->setWillBeAtk(true);
//	
//	
//	if (area == 2) {
//		Fighter* nextF = getUpNearFighter(fighterList, f);
//		if (nextF == NULL || (nextF != NULL && !nextF->isVisiable())) {
//			nextF = getDownNearFighter(fighterList, f);
//		}
//		if (nextF != NULL && nextF->isVisiable()) {
//			nextF->setWillBeAtk(true);
//		}
//	} else if (area == 3) {
//		Fighter* upF = getUpNearFighter(fighterList, f);
//		if (upF != NULL && upF->isVisiable()) {
//			upF->setWillBeAtk(true);
//		}
//		Fighter* downF = getDownNearFighter(fighterList, f);
//		if (downF != NULL && downF->isVisiable()) {
//			downF->setWillBeAtk(true);
//		}
//	} else if (area == 4) {
//		Fighter* upF1 = getUpNearFighter(fighterList, f);
//		if (upF1 != NULL && upF1->isVisiable()) {
//			upF1->setWillBeAtk(true);
//		}
//		Fighter* upF2 = getUpNearFighter(fighterList, upF1);
//		if (upF2 != NULL && upF2->isVisiable()) {
//			upF2->setWillBeAtk(true);
//		}
//		Fighter* downF1 = getDownNearFighter(fighterList, f);
//		if (downF1 != NULL && downF1->isVisiable()) {
//			downF1->setWillBeAtk(true);
//		}
//		if (upF1 == NULL) {
//			Fighter* downF2 = getDownNearFighter(fighterList, downF1);
//			if (downF2 != NULL && downF2->isVisiable()) {
//				downF2->setWillBeAtk(true);
//			}
//		}
//	} else if (area == 5) {
//		Fighter* upF1 = getUpNearFighter(fighterList, f);
//		if (upF1 != NULL && upF1->isVisiable()) {
//			upF1->setWillBeAtk(true);
//		}
//		Fighter* upF2 = getUpNearFighter(fighterList, upF1);
//		if (upF2 != NULL && upF2->isVisiable()) {
//			upF2->setWillBeAtk(true);
//		}
//		Fighter* downF1 = getDownNearFighter(fighterList, f);
//		if (downF1 != NULL && downF1->isVisiable()) {
//			downF1->setWillBeAtk(true);
//		}
//		Fighter* downF2 = getDownNearFighter(fighterList, downF1);
//		if (downF2 != NULL && downF2->isVisiable()) {
//			downF2->setWillBeAtk(true);
//		}
//	} else if (area == 10) {// ȫ�����Ǳ���������
//		VEC_FIGHTER& list = getHighlightList();
//		for (size_t i = 0; i < list.size(); i++) {
//			Fighter* fighter = list.at(i);
//			fighter->setWillBeAtk(true);
//		}
//	}
}

VEC_FIGHTER& Battle::getHighlightList()
{
	for (VEC_FIGHTER_IT it = m_vAttaker.begin(); it != m_vAttaker.end();
			it++)
	{
		if (m_highlightFighter == *it)
		{
			return m_vAttaker;
		}
	}
	return m_vDefencer;
}

void Battle::addSubAniGroup(NDSprite* role, NDAnimationGroup* group, Fighter* f)
{
	NDSubAniGroup subAniGroup;
	subAniGroup.role = role;
	subAniGroup.aniGroup = group;
	subAniGroup.fighter = f;
	subAniGroup.frameRec = new NDFrameRunRecord;

	m_vSubAniGroup.push_back(subAniGroup);
}

void Battle::handleStatusActions(VEC_STATUS_ACTION& statusActions)
{
	//	for (size_t i = 0; i < statusActions.size(); i++) {
	//		StatusAction& sa = statusActions.at(i);
	//		Fighter* target = GetFighter(sa.idTarget);
	//		PAIR_GET_HURT pairHurt;
	//		if (sa.action == StatusAction::ADD_STATUS) {
	//			pairHurt = target->getHurt(NULL,
	//					      EFFECT_CHANGE_LIFE_TYPE_SKILL_STATUS_ADD,
	//					      sa.status->m_idStatus, HURT_TYPE_PASSIVE);
	//		} else {
	//			pairHurt = target->getHurt(NULL,
	//					      EFFECT_CHANGE_LIFE_TYPE_SKILL_STATUS_LOST,
	//					      sa.status->m_idStatus, HURT_TYPE_PASSIVE);
	//		}
	//		// ״̬ȥѪ
	//		if (pairHurt.first) {
	//			Hurt& hurt = pairHurt.second;
	//			int currentHP = target->m_kInfo.nLife;
	//			if (currentHP > 0) {
	//				int hurtHP = hurt.hurtHP;
	//				currentHP += hurtHP;
	//				
	//				target->hurted(hurtHP);
	//				if (hurtHP < 0) {
	//					if (currentHP > 0) {// hurt
	//						target->setHurtOK(true);
	//						hurtAction(*target);
	//					} else {// die
	//						target->setDieOK(true);
	//						dieAction(*target);
	//					}
	//				}
	//				target->setCurrentHP(currentHP);
	//			}
	//		}
	//		if (sa.action == StatusAction::ADD_STATUS) {
	//			target->addAStatus(sa.status);
	//		} else {
	//			target->removeAStatusAniGroup(sa.status->m_idStatus);
	//		}
	//	}
}

void Battle::SetFighterOnline(int idFighter, bool bOnline)
{
	Fighter* f = GetFighter(idFighter);
	if (f)
	{
		f->setOnline(bOnline);
	}
}

/*void Battle::CreateCancleAutoFightButton()
 {
 if (!m_btnCancleAutoFight) 
 {
 m_btnCancleAutoFight = new NDUIButton();
 m_btnCancleAutoFight->Initialization();
 m_btnCancleAutoFight->SetFrameRect(CGRectMake(0, 0, 60, 30));
 m_btnCancleAutoFight->SetDelegate(this);
 m_btnCancleAutoFight->SetTag(BTN_CANCLE_AUTO);
 m_btnCancleAutoFight->SetBackgroundColor(ccc4(107, 158, 156, 255));
 m_btnCancleAutoFight->SetTitle("ȡ��");
 AddChild(m_btnCancleAutoFight);
 }
 else 
 {
 m_btnCancleAutoFight->SetVisible(true);
 }
 }
 
 void Battle::RemoveCancleAutoFightButton()
 {
 if (m_btnCancleAutoFight) 
 {
 m_btnCancleAutoFight->SetVisible(false);
 }	
 }*/

void Battle::TurnStart()
{
//	// �Զ�ս��������
//	if (m_bWatch || s_bAuto || m_bTurnStart == false) {
//		m_bTurnStart = false;
//		m_bTurnStartPet = true;
//		return;
//	}
//	
//	//	m_fighterRight->SetGray(false);
//	//	
//	//	m_fighterLeft->SetGray(false);
//	Fighter* mainFighter = GetMainUser();
//	if (mainFighter == NULL || mainFighter->isEscape() || !mainFighter->isAlive() || mainFighter->isDieOK()) 
//	{
//		m_bSendCurTurnUserAction = true;
//		m_bTurnStartPet = true;
//		m_bTurnStart = false;
//		return;
//	}
//	
//	//	RefreshSkillBar();
//	//	
//	//	m_fighterLeft->SetPage(m_lastSkillPageUser);
//	//	
//	////	m_fighterBottom->SetGray(false);
//	//	
//	//	RefreshItemBar();
//	//	
//	//	// ��������ϻغϵĲ����޸�
//	//	m_fighterRight->SetShrink(m_bShrinkRight);
//	//	m_fighterLeft->SetShrink(m_bShrinkLeft);
//	//	m_fighterBottom->SetShrink(m_bShrinkBottom);
//	
//	// �޿ɲ�׽Ŀ�꣬��׽���
//	VEC_FIGHTER& enemyList = GetEnemySideList();
//	Fighter* f;
//	bool bCatchGray = true;
//	for (size_t i = 0; i < enemyList.size(); i++) {
//		f = enemyList.at(i);
//		if (f->isVisiable() && f->isCatchable()) {
//			bCatchGray = false;
//			break;
//		}
//	}
//	
//	//	if (bCatchGray) {
//	//		m_fighterRight->SetGray(bCatchGray, 0);
//	//	}
//	
//	m_bTurnStart = false;
//	
//	// Ĭ��ѡ�񻺴���һ�غϹ����ͼ���ָ���Լ�Ŀ��
//	// ����ϻغ�Ŀ��������Ĭ��һ����Ŀ��
//	// ���ܱ��غϲ����ã����Ϊ��ͨ����
//	if (BATTLE_ACT_PHY_ATK == m_defaultActionUser) {
//		if (m_defaultTargetUser == NULL || !m_defaultTargetUser->isVisiable()) {
//			OnBtnAttack();
//			m_defaultTargetUser = m_highlightFighter;
//		} else {
//			OnBtnAttack();
//			HighlightFighter(m_defaultTargetUser);
//		}
//		
//	} else if (BATTLE_ACT_MAG_ATK == m_defaultActionUser) {
//		if (m_setBattleSkillList.count(m_defaultSkillID) <= 0) {
//			m_defaultActionUser = BATTLE_ACT_PHY_ATK;
//			OnBtnAttack();
//			m_defaultTargetUser = m_highlightFighter;
//		} else {
//			m_curBattleAction->btAction = BATTLE_ACT_MAG_ATK;
//			m_curBattleAction->vData.clear();
//			
//			s_lastTurnActionUser.btAction = BATTLE_ACT_MAG_ATK;
//			s_lastTurnActionUser.vData.clear();
//			
//			m_curBattleAction->vData.push_back(m_defaultSkillID);
//			s_lastTurnActionUser.vData.push_back(m_defaultSkillID);
//			s_lastTurnActionUser.vData.push_back(0);
//			
//			BattleSkill* skill = BattleMgrObj.GetBattleSkill(m_defaultSkillID);
//			
//			if (skill) {
//				int targetType = skill->getAtkType();
//				GetMainUser()->setUseSkill(skill);
//				
//				if (m_defaultTargetUser == NULL || !m_defaultTargetUser->isVisiable()) {
//					if ((targetType & SKILL_ATK_TYPE_ENEMY) == SKILL_ATK_TYPE_ENEMY) {
//						setBattleStatus(BS_CHOOSE_ENEMY_MAG_ATK);
//						
//						VEC_FIGHTER& enemyList = GetEnemySideList();
//						Fighter* f;
//						for (size_t i = 0; i < enemyList.size(); i++) {
//							f = enemyList.at(i);
//							if (f->isVisiable()) {
//								m_defaultTargetUser = f;
//								HighlightFighter(f);
//								break;
//							}
//						}
//						
//					} else if ((targetType & SKILL_ATK_TYPE_FRIEND) == SKILL_ATK_TYPE_FRIEND) {
//						setBattleStatus(BS_CHOOSE_OUR_SIDE_MAG_ATK);
//						
//						VEC_FIGHTER& ourList = GetOurSideList();
//						Fighter* f;
//						for (size_t i = 0; i < ourList.size(); i++) {
//							f = ourList.at(i);
//							if (f->isVisiable()) {
//								m_defaultTargetUser = f;
//								HighlightFighter(f);
//								break;
//							}
//						}
//					}
//				} else {
//					if ((targetType & SKILL_ATK_TYPE_ENEMY) == SKILL_ATK_TYPE_ENEMY) {
//						setBattleStatus(BS_CHOOSE_ENEMY_MAG_ATK);
//						HighlightFighter(m_defaultTargetUser);
//						
//					} else if ((targetType & SKILL_ATK_TYPE_FRIEND) == SKILL_ATK_TYPE_FRIEND) {
//						setBattleStatus(BS_CHOOSE_OUR_SIDE_MAG_ATK);
//						HighlightFighter(m_defaultTargetUser);
//						
//					} else if ((targetType & SKILL_ATK_TYPE_SELF) == SKILL_ATK_TYPE_SELF) {
//						setBattleStatus(BS_CHOOSE_SELF_MAG_ATK);
//						HighlightFighter(m_defaultTargetUser);
//					}
//				}
//			}
//		}
//	}
}

void Battle::TurnStartPet()
{
//	if (m_bWatch || s_bAuto || m_bTurnStartPet == false) {
//		m_bTurnStartPet = false;
//		return;
//	}
//	
//	m_bTurnStartPet = false;
//	
//	Fighter* mainEudemon = getMainEudemon();
//	if (mainEudemon == NULL || mainEudemon->isEscape() || !mainEudemon->isAlive() || mainEudemon->isDieOK())
//	{
//		//		m_fighterRight->SetGray(true);
//		//		m_fighterLeft->SetGray(true);
//		return;
//	}
//	
//	// ��׽��ť���
//	//	m_fighterRight->SetGray(true, 0);
//	
//	RefreshSkillBarPet();
//	
//	//	m_fighterLeft->SetPage(m_lastSkillPageEudemon);
//	
//	if (BATTLE_ACT_PET_PHY_ATK == m_defaultActionEudemon) {
//		if (m_defaultTargetEudemon == NULL || !m_defaultTargetEudemon->isVisiable()) {
//			OnEudemonAttack();
//			m_defaultTargetEudemon = m_highlightFighter;
//		} else {
//			OnEudemonAttack();
//			HighlightFighter(m_defaultTargetEudemon);
//		}
//		
//	} else if (BATTLE_ACT_PET_MAG_ATK == m_defaultActionEudemon) {
//		// �ϻغϳ��＼�ܿ���
//		if (m_defaultSkillIDEudemon > ID_NONE) {
//			m_curBattleAction->btAction = BATTLE_ACT_PET_MAG_ATK;
//			m_curBattleAction->vData.clear();
//			
//			s_lastTurnActionEudemon.btAction = BATTLE_ACT_PET_MAG_ATK;
//			s_lastTurnActionEudemon.vData.clear();
//			
//			m_curBattleAction->vData.push_back(m_defaultSkillIDEudemon);
//			s_lastTurnActionEudemon.vData.push_back(m_defaultSkillIDEudemon);
//			s_lastTurnActionEudemon.vData.push_back(0);
//			
//			BattleSkill* skill = BattleMgrObj.GetBattleSkill(m_defaultSkillIDEudemon);
//			if (skill) {
//				int targetType = skill->getAtkType();
//				
//				// �ϻغ�Ŀ������
//				if (m_defaultTargetUser == NULL || !m_defaultTargetUser->isVisiable()) {
//					if ((targetType & SKILL_ATK_TYPE_ENEMY) == SKILL_ATK_TYPE_ENEMY) {
//						setBattleStatus(BS_CHOOSE_ENEMY_MAG_ATK_EUDEMON);
//						
//						VEC_FIGHTER& enemyList = GetEnemySideList();
//						Fighter* f;
//						for (size_t i = 0; i < enemyList.size(); i++) {
//							f = enemyList.at(i);
//							if (f->isVisiable()) {
//								m_defaultTargetEudemon = f;
//								HighlightFighter(f);
//								break;
//							}
//						}
//						
//					} else if ((targetType & SKILL_ATK_TYPE_FRIEND) == SKILL_ATK_TYPE_FRIEND) {
//						setBattleStatus(BS_CHOOSE_OUR_SIDE_MAG_ATK_EUDEMON);
//						
//						VEC_FIGHTER& ourList = GetOurSideList();
//						Fighter* f;
//						for (size_t i = 0; i < ourList.size(); i++) {
//							f = ourList.at(i);
//							if (f->isVisiable()) {
//								m_defaultTargetEudemon = f;
//								HighlightFighter(f);
//								break;
//							}
//						}
//						
//					}
//				} else {
//					if ((targetType & SKILL_ATK_TYPE_ENEMY) == SKILL_ATK_TYPE_ENEMY) {
//						setBattleStatus(BS_CHOOSE_ENEMY_MAG_ATK_EUDEMON);
//						HighlightFighter(m_defaultTargetEudemon);
//						
//					} else if ((targetType & SKILL_ATK_TYPE_FRIEND) == SKILL_ATK_TYPE_FRIEND) {
//						setBattleStatus(BS_CHOOSE_OUR_SIDE_MAG_ATK_EUDEMON);
//						HighlightFighter(m_defaultTargetEudemon);
//						
//					} else if ((targetType & SKILL_ATK_TYPE_SELF) == SKILL_ATK_TYPE_SELF) {
//						setBattleStatus(BS_CHOOSE_SELF_MAG_ATK_EUDEMON);
//						HighlightFighter(m_defaultTargetEudemon);
//						
//					}
//				}
//			}
//		} else {
//			m_defaultActionEudemon = BATTLE_ACT_PET_PHY_ATK;
//			OnEudemonAttack();
//			m_defaultTargetEudemon = m_highlightFighter;
//		}
//	}
}

void Battle::RefreshSkillBarPet()
{
	// ��ȡ���＼��
	PetInfo* petInfo = PetMgrObj.GetMainPet(NDPlayer::defaultHero().m_nID);
	if (petInfo)
		return;

	//SET_BATTLE_SKILL_LIST& petSkillList = PetMgrObj.GetSkillList(SKILL_TYPE_ATTACK, petInfo->data.int_PET_ID);
	//SpeedBarInfo skillInfo;
	//BattleMgr& bm = BattleMgrObj;
	//BattleSkill* bs = NULL;
	//SET_BATTLE_SKILL_LIST_IT itSkill = petSkillList.begin();
	//
	//int nPetMp = getMainEudemon()->m_kInfo.nMana;
	//
	//int nFocus = -1;
	//for (int i = 0; i < MAX_SKILL_NUM; i++) {
	//	if (itSkill != petSkillList.end()) {
	//		skillInfo.push_back(SpeedBarCellInfo());
	//		SpeedBarCellInfo& ci = skillInfo.back();
	//		bs = bm.GetBattleSkill(*itSkill);
	//		if (bs) {
	//			ci.foreground = GetSkillIconByIconIndex(bs->getIconIndex(), true);;
	//			
	//			ci.gray = bs->getMpRequire() > nPetMp;
	//			ci.param1 = bs->getId();
	//			ci.param2 = bs->getCd();
	//			
	//			if (ci.param2 != 0 && !ci.gray)
	//			{
	//				CoolDownRecord_IT it = m_recordCoolDown.find(ci.param1);
	//				if (it != m_recordCoolDown.end())
	//					ci.gray = (it->second < ci.param2);
	//			}
	//			
	//			// �ϻغ�ʹ�õļ���
	//			if (m_defaultSkillIDEudemon == ci.param1 && !ci.gray) {
	//				nFocus = i;
	//			} else {
	//				// �ϻغϼ������㣬��ʾ�ü���ʧЧ
	//				m_defaultSkillIDEudemon = ID_NONE;
	//			}
	//			
	//		}
	//		itSkill++;
	//	}
	//}

	//	m_fighterLeft->refresh(skillInfo);
	//	if (nFocus > -1 && !s_bAuto) {
	//		m_fighterLeft->SetFoucusByIndex(nFocus);
	//		m_fighterRight->defocus();
	//		m_fighterBottom->defocus();
	//	}

	//	if (m_fighterLeft)
	//		m_fighterLeft->DealSkillTurn();
}

void Battle::RefreshSkillBar()
{
	//SpeedBarInfo skillInfo;
	//
	//BattleMgr& bm = BattleMgrObj;
	//BattleSkill* bs = NULL;
	//SET_BATTLE_SKILL_LIST_IT itSkill = m_setBattleSkillList.begin();
	//
	//if (GetMainUser() == NULL) return;
	//
	//int nPlayerMp = GetMainUser()->m_kInfo.nMana;
	//
	//int nFocus = -1;
	//int i = 0;
	//for (; i < MAX_SKILL_NUM; i++) {
	//	if (itSkill != m_setBattleSkillList.end()) {
	//		skillInfo.push_back(SpeedBarCellInfo());
	//		SpeedBarCellInfo& ci = skillInfo.back();
	//		bs = bm.GetBattleSkill(*itSkill);
	//		if (bs && bs->getType() == SKILL_TYPE_ATTACK) {
	//			ci.foreground = GetSkillIconByIconIndex(bs->getIconIndex(), true);
	//			
	//			ci.gray = bs->getMpRequire() > nPlayerMp;
	//			ci.param1 = bs->getId();
	//			ci.param2 = bs->getCd();
	//			// �ϻغ�ʹ�õļ���
	//			if (m_defaultSkillID == ci.param1 && !ci.gray) {
	//				nFocus = i;
	//			} else {
	//				m_defaultSkillID = ID_NONE;
	//			}
	//			
	//		}
	//		itSkill++;
	//	}
	//	else
	//	{
	//		break;
	//	}
	//}
	//
	//size_t size = skillInfo.size();
	//SET_BATTLE_SKILL_LIST& skills =  NDPlayer::defaultHero().GetSkillList(SKILL_TYPE_ATTACK);
	//if (size < size_t(MAX_SKILL_NUM) && skills.size() > 0)
	//{
	//	
	//	SET_BATTLE_SKILL_LIST_IT itSkill = skills.begin();
	//	for (; itSkill != skills.end(); itSkill++) {
	//		OBJID skillID = *itSkill;
	//		BattleSkill* bs = BattleMgrObj.GetBattleSkill(skillID);
	//		
	//		if (!bs || !bs->IsSkillOwnByPlayer()) continue;
	//		
	//		if (bs->getType() == SKILL_TYPE_PASSIVE) continue;
	//		
	//		if (m_setBattleSkillList.count(skillID) == 0 && size < size_t(MAX_SKILL_NUM)) {
	//			skillInfo.push_back(SpeedBarCellInfo());
	//			SpeedBarCellInfo& ci = skillInfo.back();
	//			ci.foreground = GetSkillIconByIconIndex(bs->getIconIndex(), true);
	//			
	//			ci.gray = true;
	//			ci.param1 = bs->getId();
	//			ci.param2 = bs->getCd();
	//			// �ϻغ�ʹ�õļ���
	//			if (m_defaultSkillID == ci.param1) {
	//				m_defaultSkillID = ID_NONE;
	//			}
	//			
	//			size++;
	//		}
	//	}
	//}
	//
	//	m_fighterLeft->refresh(skillInfo);
	//	if (nFocus > -1 && !s_bAuto) {
	//		m_fighterLeft->SetFoucusByIndex(nFocus);
	//		m_fighterRight->defocus();
	//		m_fighterBottom->defocus();
	//	}
	//	
	//	if (m_recordCoolDown.size())
	//		m_fighterLeft->DealSkillTurn();
}

void Battle::RefreshItemBar()
{
	//SpeedBarInfo speedbarBottom;
	//
	//// �Ѿ�����Ʒ���е���Ʒ����
	//set<int> itemSet;
	//
	//// ս���ڿ�����Ʒ����
	//ItemMgr& rItemMgr = ItemMgrObj;
	//VEC_ITEM vBagItems;
	//rItemMgr.GetBattleUsableItem(vBagItems);
	//
	//// ս������Ʒ������
	//NDItemBarDataPersist& itemBarSetting = NDItemBarDataPersist::DefaultInstance();
	//VEC_ITEM_BAR_CELL vItemSetting;
	//itemBarSetting.GetItemBarConfigInBattle(NDPlayer::defaultHero().m_id, vItemSetting);
	//
	//Item* pItem = NULL;
	//
	//VEC_ITEM_BAR_CELL::iterator itItemSetting = vItemSetting.begin();
	//for (int i = 0; i < 15; i++) {
	//	speedbarBottom.push_back(SpeedBarCellInfo());
	//	SpeedBarCellInfo& cellinfo = speedbarBottom.back();
	//	//cellinfo.index = i;
	//	
	//	// ����ø������ã��������õ���Ʒͼ��
	//	if (itItemSetting != vItemSetting.end()) {
	//		ItemBarCellInfo& cell = *itItemSetting;
	//		if (cell.nPos == i && cell.idItemType > 0) {
	//			cellinfo.foreground = ItemImage::GetItemPicByType(cell.idItemType, true, true);
	//			cellinfo.gray = (rItemMgr.GetBagItemByType(cell.idItemType) == NULL);
	//			cellinfo.param1 = cell.idItemType;
	//			itemSet.insert(cell.idItemType);
	//		}
	//		itItemSetting++;
	//	}
	//}
	//
	//// ����ս���ڿ�ʹ����Ʒ
	//m_mapUseItem.clear();
	//MAP_USEITEM_IT itUseItem;
	//
	//SpeedBarInfo::iterator it = speedbarBottom.begin();
	//for (VEC_ITEM::iterator itItem = vBagItems.begin(); itItem != vBagItems.end(); itItem++) {
	//	pItem = *itItem;
	//	if (!pItem) {
	//		continue;
	//	}
	//	
	//	itUseItem = m_mapUseItem.find(pItem->iItemType);
	//	if (itUseItem != m_mapUseItem.end()) {
	//		itUseItem->second.second += pItem->iAmount;
	//	} else {
	//		m_mapUseItem[pItem->iItemType] = make_pair(pItem->iID, pItem->iAmount);
	//	}
	//	
	//	if (itemSet.count(pItem->iItemType) == 0) {
	//		// ���ҿհ���λ
	//		for (; it != speedbarBottom.end(); it++) {
	//			if ((*it).foreground == NULL) {
	//				itemSet.insert(pItem->iItemType);
	//				(*it).param1 = pItem->iItemType;
	//				(*it).foreground = ItemImage::GetItemPicByType(pItem->iItemType, true, true);
	//				(*it).gray = false;
	//				it++;
	//				break;
	//			}
	//		}
	//	}
	//}

	//	m_fighterBottom->refresh(speedbarBottom);
}

// Battle::speed bar
void Battle::InitSpeedBar()
{
	if (m_bWatch)
	{
		return;
	}

	//	m_fighterBottom = new FighterBottom;
	//	m_fighterBottom->Initialization();
	//	m_fighterBottom->SetDelegate(this);
	//	
	//	if (s_bAuto) {
	//		RefreshItemBar();
	//	}
	//	
	//	AddChild(m_fighterBottom);

	//	m_fighterLeft = new FighterLeft;
	//	m_fighterLeft->Initialization();
	//	m_fighterLeft->SetDelegate(this);
	//	
	//	if (s_bAuto) {
	//		RefreshSkillBar();
	//	}
	//	
	//	AddChild(m_fighterLeft);
	//	
	//	//AddChild(m_btnQuickTalk);
	//
	//	m_fighterRight = new FighterRight;
	//	m_fighterRight->SetDelegate(this);
	//	m_fighterRight->Initialization();

	//SpeedBarInfo speedbarRight;
	//
	//// ��׽
	//speedbarRight.push_back(SpeedBarCellInfo());
	//SpeedBarCellInfo& ciCatch = speedbarRight.back();
	//NDPicture* pic = new NDPicture(true);
	//pic->Initialization(GetImgPathBattleUI("menucatch.png"));
	//ciCatch.foreground = pic;
	//ciCatch.param1 = CELL_TAG_CATCH;
	//
	//// ����
	//speedbarRight.push_back(SpeedBarCellInfo());
	//SpeedBarCellInfo& ciFlee = speedbarRight.back();
	//pic = new NDPicture(true);
	//pic->Initialization(GetImgPathBattleUI("menuescape.png"));
	//ciFlee.foreground = pic;
	//ciFlee.param1 = CELL_TAG_FLEE;
	//
	//// ����
	//speedbarRight.push_back(SpeedBarCellInfo());
	//SpeedBarCellInfo& ciDef = speedbarRight.back();
	//pic = new NDPicture(true);
	//pic->Initialization(GetImgPathBattleUI("menudefence.png"));
	//ciDef.foreground = pic;
	//ciDef.param1 = CELL_TAG_DEF;
	//
	//// ����
	//speedbarRight.push_back(SpeedBarCellInfo());
	//SpeedBarCellInfo& ciAtk = speedbarRight.back();
	//pic = new NDPicture(true);
	//pic->Initialization(GetImgPathBattleUI("menuattack.png"));
	//ciAtk.foreground = pic;
	//ciAtk.param1 = CELL_TAG_ATK;
	//
	//// �鿴
	//speedbarRight.push_back(SpeedBarCellInfo());
	//SpeedBarCellInfo& ciViewStatus = speedbarRight.back();
	//pic = new NDPicture(true);
	//pic->Initialization(GetImgPathBattleUI("menuwatch.png"));
	//ciViewStatus.foreground = pic;
	//ciViewStatus.param1 = CELL_TAG_VIEWSTATUS;

	//	m_fighterRight->refresh(speedbarRight);
	//	
	//	AddChild(m_fighterRight);
}
//
//void Battle::OnNDUISpeedBarEvent(NDUISpeedBar* speedbar, const SpeedBarCellInfo& info, bool focused)
//{
//	static double begin = [NSDate timeIntervalSinceReferenceDate];
//	static bool   first = true;
//	
//	double end = [NSDate timeIntervalSinceReferenceDate];
//	
//	if (!first)
//	{
//		if (int((end-begin)*1000) < 300)
//		{
//#if ND_DEBUG_STATE == 1
//			std::stringstream ss;
//			ss << "ս��gui̫����[" << int((end-begin)*1000) << "]����";
//			Chat::DefaultChat()->AddMessage(ChatTypeSystem, ss.str().c_str());
//#endif
//			begin = end;
//			return;
//		}
//	}
//	else
//	{
//		first = false;
//	}
//	
//	begin = end;
//	
//	//	if (speedbar == m_fighterLeft) 
//	//	{
//	//		if (info.param1 > 0) {
//	//			if (IsUserOperating()) {
//	//				if (focused) {
//	//					NDTouch touch;
//	//					TouchEnd(&touch);
//	//					
//	//				} else {
//	//					m_curBattleAction->vData.clear();
//	//					m_curBattleAction->btAction = BATTLE_ACT_MAG_ATK;
//	//					
//	//					s_lastTurnActionUser.btAction = BATTLE_ACT_MAG_ATK;
//	//					s_lastTurnActionUser.vData.clear();
//	//					
//	//					m_defaultActionUser = BATTLE_ACT_MAG_ATK;
//	//					m_defaultSkillID = info.param1;
//	//					
//	//					m_curBattleAction->vData.push_back(info.param1);
//	//					s_lastTurnActionUser.vData.push_back(info.param1);
//	//					s_lastTurnActionUser.vData.push_back(0);
//	//					
//	//					BattleSkill* skill = BattleMgrObj.GetBattleSkill(info.param1);
//	//					if (skill && GetMainUser()) {
//	//						int targetType = skill->getAtkType();
//	//						GetMainUser()->setUseSkill(skill);
//	//						
//	//						if ((targetType & SKILL_ATK_TYPE_ENEMY) == SKILL_ATK_TYPE_ENEMY) {
//	//							setBattleStatus(BS_CHOOSE_ENEMY_MAG_ATK);
//	//							VEC_FIGHTER& enemyList = GetEnemySideList();
//	//							Fighter* f;
//	//							for (size_t i = 0; i < enemyList.size(); i++) {
//	//								f = enemyList.at(i);
//	//								if (f->isVisiable()) {
//	//									m_defaultTargetUser = f;
//	//									HighlightFighter(f);
//	//									break;
//	//								}
//	//							}
//	//							
//	//						} else if ((targetType & SKILL_ATK_TYPE_FRIEND) == SKILL_ATK_TYPE_FRIEND) {
//	//							setBattleStatus(BS_CHOOSE_OUR_SIDE_MAG_ATK);
//	//							
//	//							VEC_FIGHTER& ourList = GetOurSideList();
//	//							Fighter* f;
//	//							for (size_t i = 0; i < ourList.size(); i++) {
//	//								f = ourList.at(i);
//	//								if (f->isVisiable()) {
//	//									m_defaultTargetUser = f;
//	//									HighlightFighter(f);
//	//									break;
//	//								}
//	//							}
//	//							
//	//						} else if ((targetType & SKILL_ATK_TYPE_SELF) == SKILL_ATK_TYPE_SELF) {
//	//							m_defaultTargetUser = GetMainUser();
//	//							m_curBattleAction->vData.push_back(GetMainUser()->m_kInfo.idObj);
//	//							SendBattleAction(*m_curBattleAction);
//	//							
//	//						}
//	//					}
//	//				}
//	//				
//	//			} else if (IsEudemonOperating()) {
//	//				if (focused) {
//	//					NDTouch touch;
//	//					TouchEnd(&touch);
//	//					
//	//				} else {
//	//					m_curBattleAction->btAction = BATTLE_ACT_PET_MAG_ATK;
//	//					
//	//					s_lastTurnActionEudemon.btAction = BATTLE_ACT_PET_MAG_ATK;
//	//					s_lastTurnActionEudemon.vData.clear();
//	//					
//	//					m_defaultActionEudemon = BATTLE_ACT_PET_MAG_ATK;
//	//					m_defaultSkillIDEudemon = info.param1;
//	//					
//	//					m_curBattleAction->vData.push_back(info.param1);
//	//					s_lastTurnActionEudemon.vData.push_back(info.param1);
//	//					s_lastTurnActionEudemon.vData.push_back(0);
//	//					
//	//					BattleSkill* skill = BattleMgrObj.GetBattleSkill(info.param1);
//	//					if (skill) {
//	//						if (getMainEudemon()->m_kInfo.nMana >= skill->getMpRequire()) {
//	//							int targetType = skill->getAtkType();
//	//							
//	//							if ((targetType & SKILL_ATK_TYPE_ENEMY) == SKILL_ATK_TYPE_ENEMY) {
//	//								setBattleStatus(BS_CHOOSE_ENEMY_MAG_ATK_EUDEMON);
//	//								
//	//								VEC_FIGHTER& enemyList = GetEnemySideList();
//	//								Fighter* f;
//	//								for (size_t i = 0; i < enemyList.size(); i++) {
//	//									f = enemyList.at(i);
//	//									if (f->isVisiable()) {
//	//										m_defaultTargetEudemon = f;
//	//										HighlightFighter(f);
//	//										break;
//	//									}
//	//								}
//	//								
//	//							} else if ((targetType & SKILL_ATK_TYPE_FRIEND) == SKILL_ATK_TYPE_FRIEND) {
//	//								setBattleStatus(BS_CHOOSE_OUR_SIDE_MAG_ATK_EUDEMON);
//	//								
//	//								VEC_FIGHTER& ourList = GetOurSideList();
//	//								Fighter* f;
//	//								for (size_t i = 0; i < ourList.size(); i++) {
//	//									f = ourList.at(i);
//	//									if (f->isVisiable()) {
//	//										m_defaultTargetEudemon = f;
//	//										HighlightFighter(f);
//	//										break;
//	//									}
//	//								}
//	//								
//	//							} else if ((targetType & SKILL_ATK_TYPE_SELF) == SKILL_ATK_TYPE_SELF) {
//	//								m_defaultTargetEudemon = getMainEudemon();
//	//								m_curBattleAction->vData.push_back(getMainEudemon()->m_kInfo.idObj);
//	//								SendBattleAction(*m_curBattleAction);
//	//								
//	//							}
//	//						}
//	//					}
//	//				}
//	//			}
//	//		}
//	//	}
//	//	else if (speedbar == m_fighterBottom) 
//	//	{
//	//		if (info.param1 > 0) {
//	//			if (IsUserOperating()) {
//	//				if (focused) {
//	//					NDTouch touch;
//	//					TouchEnd(&touch);
//	//				} else {
//	//					m_curBattleAction->btAction = BATTLE_ACT_USEITEM;
//	//					setBattleStatus(BS_CHOOSE_OUR_SIDE_USE_ITEM_USER);
//	//				}
//	//				
//	//			} else if (IsEudemonOperating()) {
//	//				if (focused) {
//	//					NDTouch touch;
//	//					TouchEnd(&touch);
//	//				} else {
//	//					m_curBattleAction->btAction = BATTLE_ACT_PET_USEITEM;
//	//					setBattleStatus(BS_CHOOSE_OUR_SIDE_USE_ITEM_EUDEMON);
//	//				}
//	//			}
//	//			
//	//			if (m_battleStatus == BS_CHOOSE_OUR_SIDE_USE_ITEM_USER || m_battleStatus == BS_CHOOSE_OUR_SIDE_USE_ITEM_EUDEMON) {
//	//				m_curBattleAction->vData.clear();
//	//				
//	//				MAP_USEITEM_IT it = m_mapUseItem.find(info.param1);
//	//				
//	//				if (it != m_mapUseItem.end()) {
//	//					m_curBattleAction->vData.push_back(it->second.first);
//	//				}
//	//				
//	//				VEC_FIGHTER& ourSideList = GetOurSideList();
//	//				Fighter* f;
//	//				for (size_t i = 0; i < ourSideList.size(); i++) {
//	//					f = ourSideList.at(i);
//	//					if (f->isVisiable()) {
//	//						HighlightFighter(f);
//	//						break;
//	//					}
//	//				}
//	//			}
//	//		}
//	//	}
//	//	else if (speedbar == m_fighterRight) 
//	//	{
//	//		switch (info.param1) {
//	//			case CELL_TAG_ATK:
//	//				if (IsUserOperating()) {
//	//					if (focused) {
//	//						NDTouch touch;
//	//						TouchEnd(&touch);
//	//					} else {
//	//						stopAuto();
//	//						OnBtnAttack();
//	//						m_defaultActionUser = BATTLE_ACT_PHY_ATK;
//	//						m_defaultTargetUser = m_highlightFighter;
//	//					}
//	//				} else if (IsEudemonOperating()) {
//	//					if (focused) {
//	//						NDTouch touch;
//	//						TouchEnd(&touch);
//	//					} else {
//	//						OnEudemonAttack();
//	//						m_defaultActionEudemon = BATTLE_ACT_PET_PHY_ATK;
//	//						m_defaultTargetEudemon = m_highlightFighter;
//	//					}
//	//				}
//	//				break;
//	//			case CELL_TAG_DEF:
//	//				if (IsUserOperating()) {
//	//					stopAuto();
//	//					OnBtnDefence();
//	//				} else if (IsEudemonOperating()) {
//	//					BattleAction actioin(BATTLE_ACT_PET_PHY_DEF);
//	//					s_lastTurnActionEudemon.btAction = BATTLE_ACT_PET_PHY_DEF;
//	//					SendBattleAction(actioin);
//	//				}
//	//				break;
//	//			case CELL_TAG_CATCH:
//	//				if (IsUserOperating()) {
//	//					if (focused) {
//	//						NDTouch touch;
//	//						TouchEnd(&touch);
//	//					} else {
//	//						stopAuto();
//	//						OnBtnCatch();
//	//					}
//	//				}
//	//				break;
//	//			case CELL_TAG_FLEE:
//	//				if (IsUserOperating()) {
//	//					stopAuto();
//	//					OnBtnRun();
//	//				} else if (IsEudemonOperating()) {
//	//					BattleAction actioin(BATTLE_ACT_PET_ESCAPE);
//	//					SendBattleAction(actioin);
//	//				}
//	//				break;
//	//			case CELL_TAG_VIEWSTATUS:
//	//			{
//	//				if (focused) {
//	//					NDTouch touch;
//	//					TouchEnd(&touch);
//	//				} else {
//	//					Fighter* f = NULL;
//	//					VEC_FIGHTER& enemyList = GetEnemySideList();
//	//					for (VEC_FIGHTER_IT it = enemyList.begin(); it != enemyList.end(); it++) {
//	//						if ((*it)->isVisiable()) {
//	//							f = *it;
//	//							break;
//	//						}
//	//					}
//	//					
//	//					if (IsUserOperating()) {
//	//						setBattleStatus(BS_CHOOSE_VIEW_FIGHTER_STATUS);
//	//						HighlightFighter(f);
//	//					} else if (IsEudemonOperating()) {
//	//						setBattleStatus(BS_CHOOSE_VIEW_FIGHTER_STATUS_PET);
//	//						HighlightFighter(f);
//	//					}
//	//				}
//	//
//	//			}
//	//				break;
//	//			default:
//	//				break;
//	//		}
//	//	}
//	
//	//	if(speedbar != m_fighterLeft)
//	//	{
//	//		m_fighterLeft->defocus();
//	//	}
//	//	
//	//	if(speedbar != m_fighterRight)
//	//	{
//	//		m_fighterRight->defocus();
//	//	}
//	//	
//	//	if(speedbar != m_fighterBottom)
//	//	{
//	//		m_fighterBottom->defocus();
//	//	}
//}
//
//void Battle::OnNDUISpeedBarSet(NDUISpeedBar* speedbar)
//{
//	//	if (speedbar == m_fighterBottom) 
//	//	{
//	//		if (m_bShowChatTextField) {
//	//			ShowChatTextField(false);
//	//			m_bChatTextFieldShouldShow = true;
//	//		}
//	//		
//	//		// ��Ʒ���ú��л�Ϊ��ͨ����
//	//		if (IsUserOperating()) {
//	//			OnBtnAttack();
//	//		} else if (IsEudemonOperating()) {
//	//			OnEudemonAttack();
//	//		}
//	//		
//	//		GameUIItemConfig *config = new GameUIItemConfig;
//	//		config->Initialization();
//	//		config->SetDelegate(this);
//	//		AddChild(config, 97);
//	//	}
//}
//
//void Battle::OnNDUISpeedBarShrinkClick(NDUISpeedBar* speedbar, bool fromShrnk)
//{
//	//	if (m_fighterRight == speedbar) {
//	//		m_bShrinkRight = !fromShrnk;
//	//	} else if (m_fighterLeft == speedbar) {
//	//		m_bShrinkLeft = !fromShrnk;
//	//	} else if (m_fighterBottom == speedbar) {
//	//		m_bShrinkBottom = !fromShrnk;
//	//	}
//}
//
//
//void Battle::OnRefreshFinish(NDUISpeedBar* speedbar, unsigned int page)
//{
//	//	if (speedbar == m_fighterLeft) {
//	//		if (IsUserOperating()) {
//	//			m_lastSkillPageUser = page;
//	//		} else if (IsEudemonOperating()) {
//	//			m_lastSkillPageEudemon = page;
//	//		}
//	//	}
//}
//
//void Battle::OnNDUISpeedBarEventLongTouch(NDUISpeedBar* speedbar, const SpeedBarCellInfo& info)
//{
//	//	if (speedbar == m_fighterLeft) 
//	//	{
//	//		BattleSkill*bs = BattleMgrObj.GetBattleSkill(info.param1);
//	//		
//	//		if (bs)
//	//			showDialog("", bs->getFullDes().c_str());
//	//	}
//	//	else if (speedbar == m_fighterBottom)
//	//	{
//	//		NDItemType* itemtype = ItemMgrObj.QueryItemType(info.param1);
//	//		
//	//		if (!itemtype) return;
//	//		
//	//		Item item(info.param1);
//	//		
//	//		showDialog(item.getItemNameWithAdd().c_str(), item.makeItemDes(false, true).c_str());
//	//	}
//}

void Battle::OnItemConfigFinish()
{
	RefreshItemBar();
	if (m_bChatTextFieldShouldShow)
	{
		ShowChatTextField(true);
	}
}

bool Battle::IsUserOperating()
{
//	switch (m_battleStatus) {
//		case BS_CHOOSE_VIEW_FIGHTER_STATUS:
//		case BS_CHOOSE_ENEMY_PHY_ATK:
//		case BS_CHOOSE_ENEMY_MAG_ATK:
//		case BS_CHOOSE_OUR_SIDE_MAG_ATK:
//		case BS_CHOOSE_ENEMY_CATCH:
//		case BS_CHOOSE_OUR_SIDE_USE_ITEM_USER:
//			return true;
//			break;
//		default:
//			return false;
//			break;
//	}
	return false;
}

bool Battle::IsEudemonOperating()
{
//	switch (m_battleStatus) {
//		case BS_CHOOSE_VIEW_FIGHTER_STATUS_PET:
//		case BS_CHOOSE_ENEMY_PHY_ATK_EUDEMON:
//		case BS_CHOOSE_ENEMY_MAG_ATK_EUDEMON:
//		case BS_CHOOSE_OUR_SIDE_MAG_ATK_EUDEMON:
//		case BS_CHOOSE_OUR_SIDE_USE_ITEM_EUDEMON:
//			return true;
//			break;
//		default:
//			return false;
//			break;
//	}
	return false;
}

bool Battle::OnRoleDisapper(int iRoleID)
{
	bool ret = false;

	VEC_FIGHTER_IT it = m_vActionFighterList.begin();

	for (size_t index = 0; it != m_vActionFighterList.end(); it++, index++)
	{
		Fighter* f = *it;

		if (f->GetRole() && f->GetRole()->m_nID == iRoleID)
		{

			size_t idx = m_actionFighterPoint;

			if (idx == index)
			{
				m_vActionFighterList.erase(it);

				if (m_vActionFighterList.size())
				{
					m_actionFighterPoint = (index + 1)
							% m_vActionFighterList.size();

					if (m_actionFighterPoint
							< int(m_vActionFighterList.size()))
					{
						m_vActionFighterList[m_actionFighterPoint]->setActionOK(
								true);
					}
				}
			}
			else
			{
				m_vActionFighterList.erase(it);
			}

			ret = true;

			break;
		}
	}

	for (it = m_vAttaker.begin(); it != m_vAttaker.end(); it++)
	{
		Fighter* f = *it;
		if (f->GetRole() && f->GetRole()->m_nID == iRoleID)
		{
			CC_SAFE_DELETE(*it);
			m_vAttaker.erase(it);
			return true;
		}
	}

	for (it = m_vDefencer.begin(); it != m_vDefencer.end(); it++)
	{
		Fighter* f = *it;
		if (f->GetRole() && f->GetRole()->m_nID == iRoleID)
		{
			CC_SAFE_DELETE(*it);
			m_vDefencer.erase(it);
			return true;
		}
	}

	if (m_mainFighter && m_mainFighter->GetRole()
			&& m_mainFighter->GetRole()->m_nID == iRoleID)
	{
		m_mainFighter = NULL;
		ret = true;
	}

	return ret;
}

bool Battle::CanPetFreeUseSkill()
{
	if (m_battleType == BATTLE_TYPE_MONSTER
			|| m_battleType == BATTLE_TYPE_ELITE_MONSTER
			|| m_battleType == BATTLE_TYPE_NPC
			|| m_battleType == BATTLE_TYPE_PRACTICE)
		return true;

	return false;
}

void Battle::UseSkillDealOfCooldown(int skillID)
{
	BattleSkill *bs = BattleMgrObj.GetBattleSkill(skillID);

	if (!bs || bs->getCd() == 0)
		return;

	CoolDownRecord_IT it = m_recordCoolDown.find(skillID);

	m_recordCoolDown[skillID] = -1;
}

void Battle::AddTurnDealOfCooldown()
{
	/*
	 MAP_BATTLE_SKILL& skills = BattleMgrObj.GetBattleSkills();
	 
	 for (MAP_BATTLE_SKILL_IT it = skills.begin(); it != skills.end(); it++) {
	 BattleSkill* bs = it->second;
	 CoolDownID skillID = CoolDownID(it->first);
	 if (bs->getCd() != 0 && 
	 m_recordCoolDown.find(skillID) == m_recordCoolDown.end()) {
	 m_recordCoolDown[skillID] = 0;
	 }
	 }
	 */

	CoolDownRecord_IT it = m_recordCoolDown.begin();

	for (; it != m_recordCoolDown.end(); it++)
	{
		if (m_turn > 1)
			m_recordCoolDown[it->first] = m_recordCoolDown[it->first] + 1;
	}

	//if (m_fighterLeft && m_recordCoolDown.size())
	//m_fighterLeft->DealSkillTurn();
}

void Battle::setBattleStatus(BATTLE_STATUS status)
{
	if (status == m_battleStatus)
		return;

	m_battleStatus = status;
}
