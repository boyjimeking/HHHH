/*
 *  NDMapMgr.mm
 *  DragonDrive
 *
 *  Created by wq on 10-12-28.
 *  Copyright 2010 (����)DeNA. All rights reserved.
 *
 */

///< #include "NDMapMgr.h" ��ʱ��ע�� ����
#include "NDTransData.h"
#include "NDMapLayer.h"
#include "NDDirector.h"
#include "NDItemType.h"
#include "NDPath.h"
#include "JavaMethod.h"
#include "NDNetMsg.h"
#include "NDConstant.h"
#include "NDMapLayerLogic.h"
#include "NDPlayer.h"
#include "CCPointExtension.h"
#include "NDMsgDefine.h"
#include "GameSceneLoading.h"
#include "NDNpc.h"
#include "NDMonster.h"
#include "NDDataTransThread.h"
#include "ItemMgr.h"
#include "GameUIAttrib.h"
//#include "NDBattlePet.h"
//#include "NDRidePet.h"
#include "GameScene.h"
#include "EnumDef.h"
#include "GameScene.h"
#include <sstream>
#include "BattleMgr.h"
#include "NDUISynLayer.h"
#include "NDBaseRole.h"
#include "NDUtility.h"
#include "NDString.h"
#include "GamePlayerBagScene.h"
#include "TaskListener.h"
//#include "GatherPoint.h"
#include "GlobalDialog.h"
#include "TradeUILayer.h"
#include "GameUIPaiHang.h"
//#include "FriendElement.h"
#include "GoodFriendUILayer.h"
#include "Chat.h"
#include "GameUINpcStore.h"
#include "MasterUILayer.h"
#include "ManualRoleEquipScene.h"
#include "TutorUILayer.h"
#include "UserStateUILayer.h"
#include "GameStorageScene.h"
#include "VendorUILayer.h"
//#include "LifeSkillScene.h"
#include "VendorBuyUILayer.h"
#include "Battle.h"
//#include "LifeSkillRandomScene.h"
#include "BattleSkill.h"
#include "LearnSkillUILayer.h"
#include "ForgetSkillUILayer.h"
#include "CreateSynDialog.h"
#include "VipStoreScene.h"
#include "PetSkillScene.h"
#include "SyndicateRegList.h"
#include "SyndicateInfoLayer.h"
//#include "EmailData.h"
//#include "EmailListener.h"
#include "SyndicateCommon.h"
#include "SyndicateList.h"
#include "SyndicateInviteList.h"
#include "SyndicateElectionList.h"
#include "Chat.h"
//#include "NDDataPersist.h"
#include "SyndicateVoteList.h"
#include "SyndicateNote.h"
#include "EquipUpgradeScene.h"
#include "OpenHoleScene.h"
#include "RemoveStoneScene.h"
#include "SyndicateStorage.h"
#include "SyndicateApprove.h"
#include "SyndicateMbrList.h"
//#include "cpLog.h"
#include "NDSprite.h"
#include "GameUIPetAttrib.h"
#include "AuctionSearchLayer.h"
#include "PetSkillCompose.h"
#include "AuctionDef.h"
#include "AuctionUILayer.h"
#include "CompeteListScene.h"
#include "CampDonateScene.h"
#include "RechargeScene.h"
#include "GeneralListLayer.h"
#include "FarmMgr.h"
#include "SelectBagScene.h"
#include "ActivityListScene.h"
#include "HamletListScene.h"
#include "BeatHeart.h"
#include "NDBeforeGameMgr.h"
#include "NewGamePlayerBag.h"
#include "NewGameUIPetAttrib.h"
#include "OtherPlayerInfoScene.h"
#include "QuickInteraction.h"
#include "NewFriendList.h"
#include "PetSkillInfo.h"
#include "SystemAndCustomLayer.h"
#include "SyndicateUILayer.h"
#include "NewPaiHangScene.h"
#include "SynInfoUILayer.h"
#include "SynMbrListUILayer.h"
#include "SynElectionUILayer.h"
#include "SyndicateInfoScene.h"
#include "SynVoteUILayer.h"
#include "SynApproveUILayer.h"
#include "SynUpgradeUILayer.h"
#include "SynDonateUILayer.h"
#include "SocialScene.h"
#include "EquipForgeScene.h"
#include "PetInfoScene.h"
#include "NewChatScene.h"
#include "NewEquipRepair.h"
//#include "Performance.h"
#include "TreasureHunt.h"
//#include "NDCrashUpload.h"
#include "BattleFieldScene.h"
#include "NewVipStoreScene.h"
#include "NewRecharge.h"
#include "CPet.h"
#include "PlayerInfoScene.h"
#include "AutoPathTip.h"
//#include "SimpleAudioEngine_objc.h"

// smys begin
#include "SMGameScene.h"

#include "ScriptGlobalEvent.h"
#include "ScriptMgr.h"
// ...
// smys end

using namespace std;

namespace NDEngine
{
	
#define TAG_TIMER_KICK_OUT_TIP (1021) 
	
	const char* pszChannelID = "IPHONE_BYWX";
	
	enum {
		TEXT_TEXT = 1,
	};
	
	int SHENGWANGMAX[7] = { 5000, 1500, 500,
		100, -100, -500, -1500 };
	int discount[7] = { 70, 80, 90, 95, 100, 110, 120 };
	
	int getShengWangLevel(int v)
	{
		int result = 7;
		for (int i = 0; i < 7; i++) {
			if (v > SHENGWANGMAX[i]) {
				result = i;
				break;
			}
		}
		return result;
	}
	
	int getDiscount(int v)
	{
		return discount[getShengWangLevel(v)];
	}
	
	///////////////////////////////////////////////
	IMPLEMENT_CLASS(NDMapMgr, NDObject)
	
	bool NDMapMgr::bFirstCreate = false;
	bool NDMapMgr::bVerifyVersion = true;
	
	NDMapMgr::NDMapMgr()
	{
		onlineNum = 0;
		this->m_idTradeRole = ID_NONE;
		this->m_idTradeDlg = ID_NONE;
		this->m_idAuctionDlg = ID_NONE;
		NDNetMsgPool& pool = NDNetMsgPoolObj;
		pool.RegMsg(_MSG_USERINFO, this);
		pool.RegMsg(_MSG_USERATTRIB, this);
		pool.RegMsg(_MSG_ROOM, this);
		pool.RegMsg(_MSG_PLAYER, this);
		pool.RegMsg(_MSG_PLAYER_EXT, this);
		pool.RegMsg(_MSG_NPCINFO_LIST, this);
		pool.RegMsg(_MSG_NPC_STATUS, this);
		pool.RegMsg(_MSG_MONSTER_INFO_LIST, this);
		pool.RegMsg(MB_MSG_DISAPPEAR, this);
		pool.RegMsg(_MSG_WALK, this);
		pool.RegMsg(_MSG_KICK_BACK, this);
		pool.RegMsg(_MSG_CHGPOINT, this);
		pool.RegMsg(_MSG_PETINFO, this);
		pool.RegMsg(_MSG_DIALOG, this);
		//pool.RegMsg(_MSG_LIFESKILL, this);
		pool.RegMsg(_MSG_SYNTHESIZE, this);
		pool.RegMsg(_MSG_COLLECTION, this);
		
		//������� �����������
		pool.RegMsg(_MSG_REHEARSE, this);
		pool.RegMsg(_MSG_TEAM, this);
		pool.RegMsg(_MSG_GOODFRIEND, this);
		
		//��Ϸ�˳�
		pool.RegMsg(_MSG_GAME_QUIT, this);
		
		//�������
		pool.RegMsg(_MSG_TASKINFO, this);
		pool.RegMsg(_MSG_DOING_TASK_LIST, this);
		pool.RegMsg(_MSG_QUERY_TASK_LIST, this);
		pool.RegMsg(_MSG_TASK_ITEM_OPT, this);
		pool.RegMsg(_MSG_QUERY_TASK_LIST_EX, this);
		
		pool.RegMsg(_MSG_NPCINFO, this);
		
		//pool.RegMsg(_MSG_TALK, this);
		
		// ����
		pool.RegMsg(_MSG_TRADE, this);
		
		//  ���а�
		pool.RegMsg(_MSG_BILLBOARD, this);
		pool.RegMsg(_MSG_BILLBOARD_FIELD, this);
		pool.RegMsg(_MSG_BILLBOARD_LIST, this);
		pool.RegMsg(_MSG_BILLBOARD_USER, this);
		
		// �̵�
		pool.RegMsg(_MSG_SHOPINFO, this);
		pool.RegMsg(_MSG_SHOP, this);
		
		// ʦͽ���
		pool.RegMsg(_MSG_REG_TUTOR_INFO, this);  
		pool.RegMsg(_MSG_TUTOR_INFO, this);
		pool.RegMsg(_MSG_USER_POS, this);
		pool.RegMsg(_MSG_CHG_MAP_FAIL, this);
		pool.RegMsg(_MSG_TUTOR, this);
		
		// �鿴���������Ϣ��װ��
		pool.RegMsg(_MSG_USERINFO_SEE, this); 
		pool.RegMsg(_MSG_EQUIP_INFO, this); 
		
		// ����״̬
		pool.RegMsg(_MSG_USER_STATE, this);
		pool.RegMsg(_MSG_USER_STATE_CHG, this);
		
		// ������Ϣ
		pool.RegMsg(_MSG_POS_TEXT, this);
		
		//������Լ��䷽
		pool.RegMsg(_MSG_EQUIPIMPROVE, this);
		pool.RegMsg(_MSG_FORMULA, this); // �䷽��Ϣ
		
		// ��̯
		pool.RegMsg(_MSG_BOOTH, this);
		pool.RegMsg(_MSG_BOOTH_GOODS, this);
		
		// ����
		pool.RegMsg(_MSG_MAGIC_GOODS, this);
		pool.RegMsg(_MSG_SKILL, this);
		
		pool.RegMsg(_MSG_PET_SKILL, this);
		
		pool.RegMsg(_MSG_MBR_LIST, this);
		pool.RegMsg(_MSG_APPLY_LIST, this);
		pool.RegMsg(_MSG_SYN_ANNOUNCE, this);
		pool.RegMsg(_MSG_SYN_INFO, this);
		pool.RegMsg(_MSG_QUERY_REG_SYN_LIST, this);
		pool.RegMsg(_MSG_SYNDICATE, this);
		pool.RegMsg(_MSG_SYN_LIST, this);
		pool.RegMsg(_MSG_INVITE_LIST, this);
		pool.RegMsg(_MSG_INVITE_LIST_EX, this);
		pool.RegMsg(_MSG_SYN_INVITE, this);
		pool.RegMsg(_MSG_SYN_LIST_EX, this);
		
		// �ʼ����
		pool.RegMsg(_MSG_LETTER, this);
		pool.RegMsg(_MSG_LETTER_INFO, this);
		pool.RegMsg(_MSG_LETTER_REQUEST, this);
		
		pool.RegMsg(_MSG_AUCTION, this);
		pool.RegMsg(_MSG_AUCTIONINFO, this);
		
		pool.RegMsg(_MSG_DIGOUT, this);
		pool.RegMsg(_MSG_CUSTOMER_SERVICE, this);
		pool.RegMsg(_MSG_SYSTEM_DIALOG, this);
		pool.RegMsg(MB_MSG_CHANGE_PASS, this);
		pool.RegMsg(_MSG_NAME, this);
		pool.RegMsg(_MSG_PLAYERLEVELUP, this);
		pool.RegMsg(_MSG_NPC, this);
		pool.RegMsg(_MSG_SEE, this);
		pool.RegMsg(_MSG_REPUTE_ATTR, this);
		pool.RegMsg(_MSG_CAMP_STORAGE, this);
		pool.RegMsg(_MSG_NPC_POSITION, this);
		pool.RegMsg(_MSG_NPC_TALK, this);
		pool.RegMsg(_MSG_ITEM_TYPE_INFO, this);
		pool.RegMsg(_MSG_COMPETITION, this);
		pool.RegMsg(_MSG_WISH, this);
		pool.RegMsg(_MSG_TIP, this);
		pool.RegMsg(_MSG_WALK_TO, this);
		pool.RegMsg(_MSG_REPUTE, this);
		pool.RegMsg(_MSG_COMMON_LIST, this);
		pool.RegMsg(_MSG_COMMON_LIST_RECORD, this);
		pool.RegMsg(_MSG_LIGHT_EFFECT, this);
		pool.RegMsg(_MSG_CHG_PET_POINT, this);
		
		pool.RegMsg(MB_MSG_RECHARGE, this); //��ֵ
		pool.RegMsg(MB_MSG_RECHARGE_RETURN, this); //��ֵ����
		
		pool.RegMsg(_MSG_VERSION, this);
		
		//pool.RegMsg(_MSG_FARM_LIST, this);
		
		pool.RegMsg(_MSG_ACTIVITY, this);
		
		pool.RegMsg(_MSG_DELETEROLE, this);
		
		pool.RegMsg(_MSG_PORTAL, this);
		
		//	pool.RegMsg(_MSG_EQUIP_SET_CFG, this);
		
		pool.RegMsg(_MSG_QUERY_SOCIAL_INFO, this);
		
		pool.RegMsg(MB_MSG_MOBILE_PWD, this);
		
		pool.RegMsg(_MSG_MARRIAGE, this);
		
		// Ѱ�����
		pool.RegMsg(_MSG_RESPOND_TREASURE_HUNT_INFO, this);
		
		pool.RegMsg(_MSG_RESPOND_TREASURE_HUNT_PR0B, this);
		
		pool.RegMsg(_MSG_SHOW_TREASURE_HUNT_AWARD, this);
		
		pool.RegMsg(_MSG_KICK_OUT_TIP, this);
		
		pool.RegMsg(_MSG_CHARGE_GIFT_INFO, this);
		
		pool.RegMsg(_MSG_QUERY_PETCKILL, this);
		
		//BOSSս���
		pool.RegMsg(_MSG_ROADBLOCK,this);
//		pool.RegMsg(_MSG_BOSS_BATTLE_INFO,this);
//		pool.RegMsg(_MSG_BOSS_BATTLE_SELF,this);
		
		usData = 0;
		m_iMapDocID = -1;
		
		mapType = MAPTYPE_NORMAL;
		
		memset(zhengYing, 0, sizeof(zhengYing));
		
		
		m_iCurDlgNpcID = 0;
		
		bRootItemZhangKai = false;
		bRootMenuZhangKai = false;
		
		id1 = id2 = 0; // ��������id;
		m_nCurrentMonsterRound=0;
	}
	
	NDMapMgr::~NDMapMgr()
	{
	}
	
	void NDMapMgr::Update(unsigned long ulDiff)
	{
		// ��ͼ�߼�����
		NDPlayer& player = NDPlayer::defaultHero();
		player.Update(ulDiff);
		
		// �����������
		do 
		{
			map_manualrole_it it = m_mapManualrole.begin();
			while (it != m_mapManualrole.end()) 
			{
				NDManualRole *role = it->second;
				if (role->bClear) 
				{
					updateTeamListDelPlayer(*role);
					SAFE_DELETE_NODE(role->ridepet);
					//SAFE_DELETE_NODE(role->battlepet);
					SAFE_DELETE_NODE(role);
					m_mapManualrole.erase(it++);
				}
				else
				{
					it++;
				}
			};
			
			it = m_mapManualrole.begin();
			for(;it != m_mapManualrole.end();it++)
			{
				NDManualRole* role = it->second;
				
				if (role == &player) continue;
				
				if (role) 
				{
					NDNode* parent = role->GetParent();
					if (parent && !parent->IsKindOfClass(RUNTIME_CLASS(Battle))) {
						role->Update(ulDiff);
					}
				}
			}
		} while (0);	
		
		
		// ���е�ͼ�ϵĹ�
		do 
		{
			if (NDPlayer::defaultHero().IsInState(USERSTATE_FIGHTING)
				|| NDPlayer::defaultHero().IsGathering() 
				|| NDUISynLayer::IsShown())
			{
				break;
			}
			
			NDScene *scene = NDDirector::DefaultDirector()->GetRunningScene();
			if (scene && scene->IsKindOfClass(RUNTIME_CLASS(GameScene)))
			{
				GameScene *gamescene = (GameScene*)scene;
				if (gamescene->IsUIShow())
				{
					break;
				}
			}
			
			vec_monster_it it = m_vMonster.begin();
			while (it != m_vMonster.end()) 
			{
				NDMonster *tmp = *it;
				if (!tmp->bClear) 
				{
					it++;
					continue;
				}
				
				if (GetBattleMonster() == tmp) 
				{
					tmp = NULL;
				}
				
				SAFE_DELETE_NODE(tmp);
				it = m_vMonster.erase(it);
			}
			
			
			it = m_vMonster.begin();
			for (; it != m_vMonster.end(); it++) 
			{
				NDMonster* monster = *it;
				if (monster) 
				{
					monster->Update(ulDiff);
				}
			}			
		} while (0);
		
	}
	
	void NDMapMgr::OnMsgNpcList(NDTransData* data)
	{
		
	}
	
	void NDMapMgr::OnMsgMonsterList(NDTransData* data)
	{
		
	}
	
	void NDMapMgr::ClearNpc()
	{
		if (m_vNpc.empty())
		{
			return;
		}
		
		vec_npc_it it = m_vNpc.begin();
		for (; it != m_vNpc.end(); it++)
		{
			NDNpc* tmp = *it;
			if (!tmp) 
			{
				continue;
			}
			tmp->HandleNpcMask(false);
			SAFE_DELETE_NODE(tmp->ridepet);
			SAFE_DELETE_NODE(tmp);
		}
		m_vNpc.clear();
		NDPlayer::defaultHero().InvalidNPC();
	}
	
	void NDMapMgr::setNpcTaskStateById(int npcId, int state)
	{
		if (m_vNpc.empty())
		{
			return;
		}
		
		vec_npc_it it = m_vNpc.begin();
		for (; it != m_vNpc.end(); it++)
		{
			NDNpc* tmp = *it;
			
			if (tmp->m_id != npcId) 
			{
				continue;
			}
			
			tmp->SetNpcState(NPC_STATE(state));
			break;
		}
	}
	
	void NDMapMgr::DelNpc(int iID)
	{
		if (m_vNpc.empty())
		{
			return;
		}
		
		vec_npc_it it = m_vNpc.begin();
		for (; it != m_vNpc.end(); it++)
		{
			NDNpc* tmp = *it;
			
			if (tmp->m_id != iID) 
			{
				continue;
			}
			
			if (tmp->m_id == NDPlayer::defaultHero().GetFocusNpcID()) 
			{
				NDPlayer::defaultHero().InvalidNPC();
			}
			
			tmp->HandleNpcMask(false);
			
			SAFE_DELETE_NODE(tmp->ridepet);
			
			SAFE_DELETE_NODE(tmp);
			
			m_vNpc.erase(it);
			break;
		}
	}
	
	void NDMapMgr::AddAllNpcToMap()
	{
		NDMapLayer *layer = getMapLayerOfScene( NDDirector::DefaultDirector()->GetRunningScene());
		if (!layer)
		{
			return;
		}	
		
		vec_npc_it it = m_vNpc.begin();
		for (; it != m_vNpc.end(); it++)
		{
			NDNpc *npc = *it;
			
			if (layer->ContainChild(npc)) 
				continue;
			layer->AddChild((NDNode*)npc);
			
			// ���
			if (npc->ridepet) 
			{				
				npc->ridepet->stopMoving();
				
				npc->ridepet->SetPositionEx(npc->GetPosition());
				npc->ridepet->SetCurrentAnimation(RIDEPET_STAND, npc->m_faceRight);
				npc->SetCurrentAnimation(MANUELROLE_RIDE_PET_STAND, npc->m_faceRight);
				//layer->AddChild(npc->ridepet);
			}
			
			npc->HandleNpcMask(true);
		}
		
		NDPlayer::defaultHero().UpdateFocus();
	}
	
	void NDMapMgr::AddOneNPC(NDNpc *npc)
	{
		NDMapLayer *layer = getMapLayerOfScene( NDDirector::DefaultDirector()->GetScene(RUNTIME_CLASS(GameScene)));
		if (!layer || !npc)
		{
			return;
		}	
		
		for_vec(m_vNpc, vec_npc_it)
		{
			if (npc->m_id == (*it)->m_id)
			{
				return;
			}
		}
		
		if (layer->ContainChild(npc)) 
			return;
		
		m_vNpc.push_back(npc);
		layer->AddChild((NDNode*)npc);
		
		// ���
		if (npc->ridepet) 
		{				
			npc->ridepet->stopMoving();
			
			npc->ridepet->SetPositionEx(npc->GetPosition());
			npc->ridepet->SetCurrentAnimation(RIDEPET_STAND, npc->m_faceRight);
			npc->SetCurrentAnimation(MANUELROLE_RIDE_PET_STAND, npc->m_faceRight);
			//layer->AddChild(npc->ridepet);
		}
		
		npc->HandleNpcMask(true);
		
		NDPlayer::defaultHero().UpdateFocus();
	}
	
	NDNpc* NDMapMgr::GetNpc(int iID)
	{
		vec_npc_it it = m_vNpc.begin();
		for (; it != m_vNpc.end(); it++)
		{
			NDNpc* tmp = *it;
			
			if (tmp->m_id != iID) 
			{
				continue;
			}
			
			
			return tmp;
		}
		
		return NULL;
	}
	
	void NDMapMgr::UpdateNpcLookface(unsigned int idNpc, unsigned int newLookface)
	{
		NDNpc* oldNpc = this->GetNpc(idNpc);
		if (oldNpc) 
		{
			NDNpc* newNpc = new NDNpc();
			newNpc->m_name = oldNpc->m_name;
			newNpc->dataStr = oldNpc->dataStr;	
			newNpc->Initialization(newLookface);
			newNpc->m_id = idNpc;
			newNpc->SetPosition(oldNpc->GetPosition());
			newNpc->SetNpcState(oldNpc->GetNpcState());
			newNpc->SetType(oldNpc->GetType());
			newNpc->col = oldNpc->col;
			newNpc->row = oldNpc->row;
			newNpc->initUnpassPoint();
			//remove the old npc first
			DelNpc(oldNpc->m_id);
			
			//add the new npc to game scene
			AddOneNPC(newNpc);
		}		
	}
	
	bool NDMapMgr::isMonsterClear()
	{
		if (m_vMonster.empty())
		{
			return true;
		}
		bool ret=true;
		vec_monster_it it = m_vMonster.begin();
		for (; it != m_vMonster.end(); it++)
		{
			NDMonster *tmp = *it;
			
			if (tmp->state!=MONSTER_STATE_DEAD)
			{
				ret=false;
			}
		}
		return ret;
	}
	
	void NDMapMgr::ClearMonster()
	{
		if (m_vMonster.empty())
		{
			return;
		}
		
		vec_monster_it it = m_vMonster.begin();
		for (; it != m_vMonster.end(); it++)
		{
			NDMonster *tmp = *it;
			
			SAFE_DELETE_NODE(tmp);
		}
		m_vMonster.clear();
	}
	
	void NDMapMgr::ClearOneMonster(NDMonster* monster)
	{
		vec_monster_it it = m_vMonster.begin();
		for (; it != m_vMonster.end(); it++)
		{
			NDMonster *tmp = *it;
			
			if (tmp == monster) 
			{
				SAFE_DELETE_NODE(tmp);
				m_vMonster.erase(it);
				break;
			}
		}
	}
	
	//void NDMapMgr::ClearOneGP(GatherPoint* gp)
	//{
	//	if (!gp)
	//	{
	//		return;
	//	}
	//	map_gather_point_it it = m_mapGP.find(gp->getId());
	//	if (it != m_mapGP.end())
	//	{
	//		SAFE_DELETE_NODE(gp);
	//		m_mapGP.erase(it);
	//	}
	//}
	//
	//void NDMapMgr::ClearGP()
	//{
	//	map_gather_point_it it = m_mapGP.begin();
	//	for (; it != m_mapGP.end(); it++)
	//	{
	//		GatherPoint *gp = it->second;
	//		SAFE_DELETE_NODE(gp);
	//	}
	//	m_mapGP.clear();
	//}
	//
	NDMonster* NDMapMgr::GetMonster(int iID)
	{
		vec_monster_it it = m_vMonster.begin();
		for (; it != m_vMonster.end(); it++)
		{
			NDMonster *tmp = *it;
			
			if (tmp->m_id == iID) 
			{
				return tmp;
			}
		}
		
		return NULL;
	}
	
	void NDMapMgr::AddOneMonster(NDMonster* monster)
	{
		NDLayer *layer = getMapLayerOfScene( NDDirector::DefaultDirector()->GetScene(RUNTIME_CLASS(GameScene)));
		if (!layer)
		{
			return;
		}
		
		layer->AddChild((NDNode*)monster);
		m_vMonster.push_back(monster);
	}
	
	NDMonster* NDMapMgr::GetBoss()
	{
		vec_monster_it it = m_vMonster.begin();
		for (; it != m_vMonster.end(); it++)
		{
			NDMonster *monster = *it;
			if ( monster && monster->GetType() == MONSTER_BOSS && monster->getState() != MONSTER_STATE_DEAD )
			{
				return monster;
			}
		}
		return NULL;
	}
	
	void NDMapMgr::AddAllMonsterToMap()
	{
		NDLayer *layer = getMapLayerOfScene( NDDirector::DefaultDirector()->GetRunningScene());
		if (!layer)
		{
			return;
		}
		
		vec_monster_it it = m_vMonster.begin();
		for (; it != m_vMonster.end(); it++)
		{
			NDMonster *monster = *it;
			if ( monster && monster->m_id > 0 && monster->getState() == MONSTER_STATE_DEAD )
			{
				continue;
			}
			
			if (monster->GetParent() == NULL)
				layer->AddChild((NDNode*)monster);
		}
		
		//map_gather_point_it it2 = m_mapGP.begin();
		//for (; it2 != m_mapGP.end(); it2++) 
		//{
		//	GatherPoint *gp = it2->second;
		//	if (gp) 
		//	{
		//		if (gp->GetParent() == NULL)
		//			layer->AddChild((NDNode*)gp);
		//	}
		//}
	}
	
	bool NDMapMgr::GetMonsterInfo(monster_type_info& info, int nType)
	{
		monster_info_it it = m_mapMonsterInfo.find(nType);
		if (it == m_mapMonsterInfo.end())
		{
			return false;
		}
		info = it->second;
		return true;
	}
	
	void NDMapMgr::BattleStart()
	{
		/*
		 if (waitbatteleMonster) 
		 {
		 waitbatteleMonster->startBattle();
		 }
		 */
		
		NDScene *scene = NDDirector::DefaultDirector()->GetScene(RUNTIME_CLASS(GameScene));
		if (scene) 
		{
			DirectKey* dk = ((GameScene*)scene)->GetDirectKey();
			if (dk) 
			{
				dk->OnButtonUp(NULL);
			}
		}

		NDPlayer::defaultHero().BattleStart();
	}
	
	void NDMapMgr::BattleEnd(int iResult)
	{
		NDMonster* monster = GetBattleMonster();
		if (monster&&monster->state!=MONSTER_STATE_DEAD) 
		{
			monster->endBattle();
			monster->setMonsterStateFromBattle(iResult);
			SetBattleMonster(NULL);
		}
		
		if (BATTLE_COMPLETE_WIN==iResult)
		{
			this->m_nCurrentMonsterRound++;
		}else{
			if(BattleMgrObj.GetBattle()->GetBattleType()==BATTLE_TYPE_MONSTER)
			{
				if(monster)
				{
					monster->restorePosition();

					NDPlayer& player = NDPlayer::defaultHero();
					
					player.SetPosition(ccp(monster->selfMoveRectX-64, monster->GetPosition().y));
					player.SetServerPositon((monster->selfMoveRectX-64)/MAP_UNITSIZE, (monster->GetPosition().y)/MAP_UNITSIZE);
					
					Battle* battle = BattleMgrObj.GetBattle();
					if (battle)
					{
						battle->setSceneCetner(monster->selfMoveRectX-64, monster->GetPosition().y);
						player.stopMoving();
					}else{
						NDMapLayer *layer = NDMapMgrObj.getMapLayerOfScene( NDDirector::DefaultDirector()->GetRunningScene());

						if (layer) 
						{
							layer->SetScreenCenter(ccp(monster->selfMoveRectX-64, monster->GetPosition().y));
						}
						player.stopMoving();
					}
				}
			}
		}

		NDPlayer::defaultHero().BattleEnd(iResult);
	}
	
	NDManualRole* NDMapMgr::GetManualRole(int nID)
	{
		NDPlayer& role = NDPlayer::defaultHero();
		if (role.m_id == nID) {
			return &role;
		}
		
		if (m_mapManualrole.empty()) 
		{
			return NULL;
		}
		
		map_manualrole_it it = m_mapManualrole.find(nID);
		if (it != m_mapManualrole.end()) 
		{
			return it->second;
		}
		
		return NULL;
	}
	
	NDManualRole* NDMapMgr::GetManualRole(const char* name)
	{
		NDManualRole* result = NULL;
		
		map_manualrole_it it;
		for	(it = m_mapManualrole.begin(); it != m_mapManualrole.end(); it++) 
		{
			NDManualRole* role = it->second;
			if (strcmp(role->m_name.c_str(), name) == 0) 
			{
				result = it->second;
				break;
			}
		}
		return result;
	}
	
	void NDMapMgr::AddManualRole(int nID, NDManualRole* role)
	{
		if (!role || nID == -1) 
		{
			return;
		}
		
		m_mapManualrole.insert(map_manualrole_pair(nID, role));
		
		//if (!role->isTeamLeader() && role->isTeamMember()) 
		//		{
		//			return;
		//		}
		
		NDLayer *layer = getMapLayerOfScene( NDDirector::DefaultDirector()->GetRunningScene());
		if (!layer)
		{
			return;
		}
		
		layer->AddChild((NDNode*)role);
		
		if (role->m_id == NDPlayer::defaultHero().m_iFocusManuRoleID)
		{
			GameScene* gs = GameScene::GetCurGameScene();
			if (gs) {
				gs->SetTargetHead(role);
			}
			role->SetFocus(true);
		}
	}
	
	void NDMapMgr::DelManualRole(int nID)
	{
		if (nID == -1) 
		{
			return;
		}
		
		map_manualrole_it it = m_mapManualrole.find(nID);
		if (it != m_mapManualrole.end()) 
		{
			NDManualRole* role = it->second;
			if (role) 
			{
				SAFE_DELETE_NODE(role->ridepet);
				//SAFE_DELETE_NODE(role->battlepet);
				SAFE_DELETE_NODE(role);
			}
			m_mapManualrole.erase(it);
		}
	}
	
	void NDMapMgr::ClearManualRole()
	{
		if (m_mapManualrole.empty())
		{
			return;
		}
		
		map_manualrole_it it = m_mapManualrole.begin();
		for (; it != m_mapManualrole.end(); it++)
		{
			NDManualRole *role = it->second;
			SAFE_DELETE_NODE(role);
		}
		m_mapManualrole.clear();
	}
	
	NDManualRole* NDMapMgr::NearestBattleFieldManualrole(NDManualRole& role, int iDis)
	{
		int minDist = iDis;
		
		NDManualRole * resrole = NULL;
		
		map_manualrole_it it = m_mapManualrole.begin();
		for (; it != m_mapManualrole.end(); it++)
		{
			NDManualRole *manualrole = it->second;
			
			if (role.m_id == manualrole->m_id) continue;
			
			if (!manualrole->IsInState(USERSTATE_BATTLEFIELD)) 
			{
				continue;
			}
			
			
			if (manualrole->IsInState(USERSTATE_BF_WAIT_RELIVE))
			{
				continue;
			}
			
			if (manualrole->IsInState(USERSTATE_FIGHTING) || manualrole->IsInState(USERSTATE_DEAD)) {
				continue;
			}
			
			if (manualrole->camp == role.camp) {
				continue;
			}
			
			int dis = getDistBetweenRole(manualrole, &role);
			
			if ( dis <= minDist )
			{
				resrole = manualrole;
				minDist = dis;
			}
		}
		
		return resrole;
	}
	
	NDManualRole* NDMapMgr::NearestDacoityManualrole(NDManualRole& role, int iDis)
	{
		int minDist = iDis;
		
		NDManualRole * resrole = NULL;
		
		map_manualrole_it it = m_mapManualrole.begin();
		for (; it != m_mapManualrole.end(); it++)
		{
			NDManualRole *manualrole = it->second;
			
			if (role.m_id == manualrole->m_id) continue;
			
			if (!manualrole->IsInDacoity()) continue;
			
			if (manualrole->IsInState(USERSTATE_FIGHTING) 
				|| manualrole->IsInState(USERSTATE_DEAD)
				|| manualrole->IsInState(USERSTATE_PVE)
				) 
			{
				continue;
			}
			
			if ( !(role.IsInState(USERSTATE_BATTLE_POSITIVE) && manualrole->IsInState(USERSTATE_BATTLE_NEGATIVE) ||
				   role.IsInState(USERSTATE_BATTLE_NEGATIVE) && manualrole->IsInState(USERSTATE_BATTLE_POSITIVE))
				)
				continue;
			
			int dis = getDistBetweenRole(manualrole, &role);
			
			if ( dis <= minDist )
			{
				resrole = manualrole;
				minDist = dis;
			}
		}
		
		return resrole;
	}
	
	void NDMapMgr::OnMsgItemType(NDTransData* data)
	{
		
	}
	
	CSMBattleScene* NDMapMgr::loadBattleSceneByMapID(int mapid,int x,int y)
	{
		//m_iMapDocID = mapid;
		
		NDDirector::DefaultDirector()->PurgeCachedData();		
		//GameSceneReleaseHelper::Begin();
		
		CSMBattleScene* scene = CSMBattleScene::Scene();
		scene->Initialization(mapid);
		scene->SetTag(SMBATTLESCENE_TAG);
		NDDirector::DefaultDirector()->PushScene(scene,true);
		
		NDMapLayer* maplayer = getBattleMapLayerOfScene(scene);
		if (maplayer) 
		{
			m_sizeMap = maplayer->GetContentSize();
			maplayer->SetScreenCenter(ccp(x,y));
		}
		else 
		{
			m_sizeMap = CGSizeZero;
		}
		
		//ScriptGlobalEvent::OnEvent(GE_GENERATE_GAMESCENE);
		
		return scene;
	}
	
	NDMapLayer* NDMapMgr::getBattleMapLayerOfScene(NDScene* scene)
	{
		if (!scene) {
			return NULL;
		}
		
		NDNode *node = scene->GetChild(BATTLEMAPLAYER_TAG);
		
		
		if (!node || !node->IsKindOfClass(RUNTIME_CLASS(NDMapLayer))) {
			return NULL;
		}
		
		return (NDMapLayer*)node;
	}
	
	
	bool NDMapMgr::loadSceneByMapDocID(int mapid)
	{
		m_iMapDocID = mapid;
		
		NDDirector::DefaultDirector()->ReplaceScene(NDScene::Scene());
		NDPicturePool::DefaultPool()->Recyle();
		NDPicturePool::DefaultPool()->Recyle();
		NDDirector::DefaultDirector()->PurgeCachedData();		
	
		//GameSceneReleaseHelper::Begin();
		
		CSMGameScene* scene = CSMGameScene::Scene();
		scene->Initialization(mapid);
		scene->SetTag(SMGAMESCENE_TAG);
		NDDirector::DefaultDirector()->ReplaceScene(scene);
		NDPicturePool::DefaultPool()->Recyle();
		
		NDMapLayer* maplayer = getMapLayerOfScene(scene);
		if (maplayer) 
		{
			m_sizeMap = maplayer->GetContentSize();
		}
		else 
		{
			m_sizeMap = CGSizeZero;
		}
		
		AddSwitch();
		
		return true;
	}
	
	int NDMapMgr::GetMotherMapID()
	{
		int theId=m_mapID;
		if(m_mapID/100000000>0)
		{
			theId=m_mapID/100000*100000;
		}
		return theId;
	}
	
	void NDMapMgr::LoadMapMusic()
	{
		int theId=GetMotherMapID();
		
		int music_id=ScriptDBObj.GetN("map", theId, DB_MAP_MUSIC);
		
		SimpleAudioEngine *audioEngine=[SimpleAudioEngine sharedEngine];
		NSString *musicPath = [NSString stringWithUTF8String:NDPath::GetSoundPath().c_str()]; 
		NSString *musicFile = [NSString stringWithFormat:@"%@music_%d.mp3", musicPath, music_id];
		[audioEngine playBackgroundMusic:musicFile loop:YES];
	}
	
	void NDMapMgr::LoadSceneMonster()
	{
		int theId=GetMotherMapID();
		ID_VEC idList;
		ScriptDBObj.GetIdList("mapzone", idList);
		
		for(unsigned int i=0;i<idList.size();i++){
			int m_id=ScriptDBObj.GetN("mapzone", idList.at(i),DB_MAPZONE_MAPID);
			if(m_id==theId){
				
				NDLayer *layer = NDMapMgrObj.getMapLayerOfScene( NDDirector::DefaultDirector()->GetRunningScene());
				if (!layer)
				{
					return;
				}
				NDMonster *monster = new NDMonster();
				int elite_flag=ScriptDBObj.GetN("mapzone", idList.at(i),DB_MAPZONE_ELITE_FLAG);
				if(elite_flag!=0){//��Ӣ��ͨ���������·������ӱ��ض�ȡ
					continue;
				}
				monster->m_id =ScriptDBObj.GetN("mapzone", idList.at(i),DB_MAPZONE_ID);
				int col=ScriptDBObj.GetN("mapzone", idList.at(i),DB_MAPZONE_POS_X);
				int row=ScriptDBObj.GetN("mapzone", idList.at(i),DB_MAPZONE_POS_Y);
				int idType=ScriptDBObj.GetN("mapzone", idList.at(i),DB_MAPZONE_MONSTER_TYPE);
				int atk_area=ScriptDBObj.GetN("mapzone",idList.at(i),DB_MAPZONE_ATK_AREA);
				monster->SetPosition(ccp(col*MAP_UNITSIZE, row*MAP_UNITSIZE));
				monster->setOriginalPosition(col*MAP_UNITSIZE, row*MAP_UNITSIZE);
				monster->Initialization(idType);
				monster->SetMoveRect(ccp(col*MAP_UNITSIZE, row*MAP_UNITSIZE), 7);
				monster->setState(MONSTER_STATE_NORMAL);
				NDMapMgrObj.m_vMonster.push_back(monster);
			}
		}
		
		NDMapMgrObj.AddAllMonsterToMap();
	}
	
	NDMapLayer* NDMapMgr::getMapLayerOfScene(NDScene* scene)
	{
		if (!scene) {
			return NULL;
		}
		
		NDNode *node = scene->GetChild(MAPLAYER_TAG);
		
		
		if (!node || !node->IsKindOfClass(RUNTIME_CLASS(NDMapLayer))) {
			return NULL;
		}
		
		return (NDMapLayer*)node;
	}
	
	NDUILayer* NDMapMgr::getUILayerOfRunningScene(int iTag)
	{
		NDScene *scene = NDDirector::DefaultDirector()->GetRunningScene();
		if (!scene) 
		{
			return NULL;
		}
		
		NDNode *node = scene->GetChild(iTag);
		
		if (!node || !node->IsKindOfClass(RUNTIME_CLASS(NDUILayer)) )
		{
			return NULL;
		}
		
		return (NDUILayer*)node;
	}
	
	void NDMapMgr::processWalkTo(NDTransData& data)
	{
		int playerId = data.ReadInt();
		int amount = data.ReadByte();
		NDPlayer& player = NDPlayer::defaultHero();
		for (int i = 0; i < amount; i++) {
			int posX = data.ReadShort();
			int posY = data.ReadShort();
			int idNpc	= data.ReadInt();
			if (playerId == player.m_id) {
				CloseProgressBar;
				if (idNpc) {
					NDNpc* pNpc = this->GetNpc(idNpc);
					if (!pNpc) {
						continue;
					}
					
					CGPoint dstPoint;
					if (pNpc->getNearestPoint(player.GetPosition(), dstPoint))
					{
						CGPoint disPos = ccpSub(dstPoint, player.GetPosition());
						
						if (abs(int(disPos.x)) <= 32 && abs(int(disPos.y)) <= 32)
						{
							if (pNpc->GetType() != 6) 
							{
								player.SendNpcInteractionMessage(pNpc->m_id);
//								if (pNpc->IsDirectOnTalk()) 
//								{
//									//npc�����޸�	
//									if (player.GetPosition().x > pNpc->GetPosition().x) 
//										pNpc->DirectRight(true);
//									else 
//										pNpc->DirectRight(false);
//								}
							}
						}
						else
						{
							AutoPathTipObj.work(pNpc->m_name);
							player.Walk(dstPoint, SpriteSpeedStep4, true);
						}
					}
				}
				else {
					player.Walk(CGPointMake(posX << 4, posY << 4), SpriteSpeedStep4, true);
				}
			}
		}
	}
	
	bool NDMapMgr::process(MSGID msgID, NDTransData* data, int len)
	{
		switch (msgID) 
		{
			case _MSG_CHG_PET_POINT:
			{
				/*OBJID idPet = */data->ReadInt();
				int btAnswer = data->ReadByte();
				if (btAnswer == 1) {// �޸ĳ�������
					//NewGameUIPetAttrib* petattr = NewGameUIPetAttrib::GetInstance();
					//					if (petattr)
					//						petattr->UpdateGameUIPetAttrib();
				}
			}
				break;
			case _MSG_COMMON_LIST:
				//GeneralListLayer::processMsgCommonList(*data);
				processMsgCommonList(*data);
				break;
			case _MSG_COMMON_LIST_RECORD:
				//GeneralListLayer::processMsgCommonListRecord(*data);
				processMsgCommonListRecord(*data);
				break;
			case _MSG_LIGHT_EFFECT:
			{
				GameScene* scene = (GameScene*)NDDirector::DefaultDirector()->GetScene(RUNTIME_CLASS(GameScene));
				if (scene) {
					scene->processMsgLightEffect(*data);
				}
			}
				break;
			case _MSG_REPUTE:
			{
				NDPlayer& role = NDPlayer::defaultHero();
				role.swGuojia = data->ReadInt();
				role.swCamp = data->ReadInt();
				role.honour = data->ReadInt();
				role.expendHonour = data->ReadInt();
				role.rank = data->ReadUnicodeString();
				updateTaskRepData(role.swCamp, false);
				updateTaskHrData(role.honour, false);
			}
				break;
			case _MSG_WALK_TO:
				this->processWalkTo(*data);
				break;
			case _MSG_USERINFO:
				processUserInfo(data, len);
				break;
			case _MSG_USERATTRIB:
				processUserAttrib(data, len);
				break;
			case _MSG_ROOM:
				processChangeRoom(data, len);
				break;
			case _MSG_PLAYER:
				processPlayer(data, len);
				break;
			case _MSG_PLAYER_EXT:
				processPlayerExt(data, len);
				break;
			case _MSG_NPCINFO_LIST:
				processNpcInfoList(data, len);
				break;
			case _MSG_NPC_STATUS:
				processNpcStatus(data, len);
				break;
			case _MSG_MONSTER_INFO_LIST:
				processMonsterInfo(data, len);
				break;
			case MB_MSG_DISAPPEAR:
				processDisappear(data, len);
				break;
			case _MSG_WALK:
				processWalk(data, len);
				break;
			case _MSG_KICK_BACK:
				processKickBack(data, len);
				break;
			case _MSG_CHGPOINT:
				processChgPoint(data, len);
				break;
			case _MSG_PETINFO:
				processPetInfo(data, len);
				break;
			case _MSG_DIALOG:
				this->processMsgDlg(*data);
				break;
			//case _MSG_LIFESKILL:
			//	this->processLifeSkill(*data);
			//	break;
			//case _MSG_SYNTHESIZE:
			//	this->processLifeSkillSynthesize(*data);
			//	break;
			case _MSG_COLLECTION:
				this->processCollection(*data);
				break;
			case _MSG_GAME_QUIT:
				this->processGameQuit(data, len);
				break;
			case _MSG_TASKINFO:
			case _MSG_DOING_TASK_LIST:
			case _MSG_QUERY_TASK_LIST:
			case _MSG_TASK_ITEM_OPT:
			case _MSG_QUERY_TASK_LIST_EX:
				processTask(msgID,data);
				break;
			case _MSG_NPCINFO:
				processNPCInfo(*data);
				break;
			case _MSG_TALK:
				processTalk(*data);
				break;
			case _MSG_REHEARSE:
				processRehearse(*data);
				break;
			case _MSG_TEAM:
				processTeam(*data);
				break;
			case _MSG_GOODFRIEND:
				processGoodFriend(*data);
				break;
			case _MSG_TUTOR:
				TutorUILayer::processMsgTutor(*data);
				break;
			case _MSG_TRADE:
				this->processTrade(*data);
				break;
			case _MSG_BILLBOARD:
				this->processBillBoard(*data);
				break;
			case _MSG_BILLBOARD_FIELD:
				this->processBillBoardField(*data);
				break;
			case _MSG_BILLBOARD_LIST:
				this->processBillBoardList(*data);
				break;
			case _MSG_BILLBOARD_USER:
				this->processBillBoardUser(*data);
				break;
			case _MSG_SHOPINFO:
				this->processShopInfo(*data);
				break;
			case _MSG_SHOP:
				this->processShop(*data);
				break;
			case _MSG_REG_TUTOR_INFO:
				MasterUILayer::refreshScroll(*data);
				break;
			case _MSG_TUTOR_INFO:
				TutorUILayer::processTutorList(*data);
				break;
			case _MSG_USER_POS:
				TutorUILayer::processUserPos(*data);
				break;
			case _MSG_CHG_MAP_FAIL:
			{
				NDScene* scene = NDDirector::DefaultDirector()->GetRunningScene();
				if (scene->IsKindOfClass(RUNTIME_CLASS(GameSceneLoading))) 
				{
					NDDirector::DefaultDirector()->PopScene();
				}
			}
				break;
			case _MSG_USERINFO_SEE:
				this->processUserInfoSee(*data);
				break;
			case _MSG_EQUIP_INFO:
				this->processEquipInfo(*data);
				break;
			case _MSG_USER_STATE:
				UserStateUILayer::processMsgUserState(*data);
				break;
			case _MSG_USER_STATE_CHG:
				UserStateUILayer::processMsgUserStateChg(*data);
				break;
			case _MSG_POS_TEXT:
			{
				GameScene* gs = GameScene::GetCurGameScene();
				if (gs) {
					gs->processMsgPosText(*data);
				}
			}
				break;
			case _MSG_EQUIPIMPROVE:
				processEquipImprove(*data);
				break;
			case _MSG_FORMULA:
				processFormula(*data);
				break;
			case _MSG_BOOTH:
				VendorUILayer::processMsgBooth(*data);
				break;
			case _MSG_BOOTH_GOODS:
				VendorBuyUILayer::Show(*data);
				break;
			case _MSG_MAGIC_GOODS:
				this->processSkillGoods(*data);
				break;
			case _MSG_SKILL:
				this->processMsgSkill(*data);
				break;
			case _MSG_PET_SKILL:
				this->processMsgPetSkill(*data);
				break;
			case _MSG_LETTER:
			case _MSG_LETTER_INFO:
			case _MSG_LETTER_REQUEST:
				EmailListener::processMsg(msgID, *data);
				break;
			case _MSG_QUERY_REG_SYN_LIST:
				SyndicateRegListUILayer::refreshScroll(*data);
				break;
			case _MSG_SYNDICATE:
				this->processSyndicate(*data);
				break;
			case _MSG_SYN_LIST:
				SyndicateListUILayer::refreshScroll(*data);
				break;
			case _MSG_SYN_LIST_EX:
				SyndicateUILayer::OnAllSynList(*data);
				break;
			//case _MSG_INVITE_LIST:
			//case _MSG_INVITE_LIST_EX:
			//	// TODO: �ӵ������б�
			//{
			//	int btCount = data->ReadByte();
			//	for (int i = 0; i < btCount; i++) {
			//		int idSyn = data->ReadInt();
			//		string strSynName = data->ReadUnicodeString();
			//		RequsetInfo info;
			//		info.set(idSyn , strSynName, RequsetInfo::ACTION_SYNDICATE);
			//		this->addRequst(info);
			//	}
			//}
			//	//SyndicateInviteList::refreshScroll(*data);
			//	break;
			case _MSG_SYN_INVITE:
				SyndicateInviteList::processSynInvite(*data);
				break;
			case _MSG_SYN_INFO:
			{
				int rank = data->ReadByte(); // �����ڰ����е�ְλ
				string synName = data->ReadUnicodeString();// ��������
				NDPlayer& role = NDPlayer::defaultHero();
				role.setSynRank(rank);
				role.synName = (synName);
				break;
			}
			case _MSG_SYN_ANNOUNCE:
			{
				SynInfoUILayer* synInfo = SynInfoUILayer::GetCurInstance();
				if (synInfo) {
					synInfo->processSynBraodcast(*data);
				}
			}
				//SyndicateNote::Show(data->ReadUnicodeString());
				break;
			case _MSG_APPLY_LIST:
			{
				SynApproveUILayer* approve = SynApproveUILayer::GetCurInstance();
				if (approve) {
					approve->processApproveList(*data);
				}
			}
				//SyndicateApprove::refreshScroll(*data);
				break;
			case _MSG_DIGOUT:
				processDigout(*data);
				break;
			case _MSG_MBR_LIST:
			{
				SynMbrListUILayer* mbrList = SynMbrListUILayer::GetCurInstance();
				if (mbrList) {
					mbrList->processMbrList(*data);
				}
			}
				//SyndicateMbrList::refreshScroll(*data);
				break;
			case _MSG_NAME:
				showDialog(NDCommonCString("tip"), NDCommonCString("RenameSucc"));
				break;
			case _MSG_PLAYERLEVELUP:
				processPlayerLevelUp(*data);
				break;
			case _MSG_NPC:
				processNPC(*data);
				break;
			case _MSG_SYSTEM_DIALOG:
			{
				this->noteTitle = data->ReadUnicodeString();
				this->noteContent = data->ReadUnicodeString();
				GlobalDialogObj.Show(NULL, noteTitle.c_str(), noteContent.c_str(), NULL, NULL);
			}
				break;
			case _MSG_SEE:
				processSee(*data);
				break;
			case _MSG_REPUTE_ATTR:
			{
				NDPlayer& player = NDPlayer::defaultHero();
				int action = data->ReadByte();
				int val = data->ReadInt();
				switch (action) {
					case 1: // ��������
						player.swGuojia = val;
						updateTaskRepData(player.swGuojia + player.swCamp, true);
						break;
					case 2: // ��
						player.swCamp = val;
						updateTaskRepData(player.swGuojia + player.swCamp, true);
						break;
					case 3: // ����
						player.honour = val;
						updateTaskHrData(val, true);
						break;
					case 4: // soldier todo
						break;
					case 5: // ����������ֵ
						player.expendHonour = val;
						BattleFieldScene::UpdateShop();
						break;
					default:
						break;
				}
			}
				break;
			case _MSG_TIP:
			{
				NDScene* scene = NDDirector::DefaultDirector()->GetRunningScene();
				if (scene->IsKindOfClass(RUNTIME_CLASS(GameSceneLoading))) 
				{
					GameSceneLoading* gsl = (GameSceneLoading*)scene;
					gsl->UpdateTitle(data->ReadUnicodeString());
				}
			}
				break;
			case _MSG_CAMP_STORAGE:
				processCampStorage(*data);
				break;
			case _MSG_NPC_POSITION:
				processNpcPosition(*data);
				break;
			case _MSG_NPC_TALK:
				processNpcTalk(*data);
				break;
			case _MSG_ITEM_TYPE_INFO:
				processItemTypeInfo(*data);
				break;
			case _MSG_CUSTOMER_SERVICE:
			{
				CloseProgressBar;
				
				string callNum = data->ReadUnicodeString();
				string domainName = data->ReadUnicodeString();
				
				//NSString *resPath = [NSString stringWithUTF8String:NDPath::GetResourcePath().c_str()];
				NSString *helpiniTable = [NSString stringWithFormat:@"%s", GetResPath("help.ini")];
				NSInputStream *stream  = [NSInputStream inputStreamWithFileAtPath:helpiniTable];
				[stream open];
				string text = cutBytesToString(stream, 3);
				
				text += "<cff0000" + callNum + "/e"
				+ cutBytesToString(stream, 4) + "<cff0000" + domainName + "/e";
				//GlobalShowDlg("�ͷ�����", text);
				
				CustomDeclaration::processDeclareData(text.c_str());
			}
				break;
			case MB_MSG_CHANGE_PASS:
			{
				CloseProgressBar;
				GlobalShowDlg(NDCommonCString("SysTip"), data->ReadUnicodeString());
			}
				break;
			case _MSG_AUCTION:
				AuctionUILayer::processAuction(*data);
				break;
			case _MSG_AUCTIONINFO:
				AuctionUILayer::processAuctionInfo(*data);
				break;
			case _MSG_COMPETITION:
				processCompetition(*data);
				break;
			case _MSG_WISH:
				processWish(*data);
				break;
			case MB_MSG_RECHARGE:
				processReCharge(*data);
				break;
			case MB_MSG_RECHARGE_RETURN:
				processRechargeReturn(*data);
				break;
			case _MSG_VERSION:
				processVersionMsg(*data);
				break;
				//case _MSG_FARM_LIST:
				//				NDFarmMgrObj.processFarmList(*data);
				//				break;
			case _MSG_ACTIVITY:
				processActivity(*data);
			case _MSG_DELETEROLE:
				processDeleteRole(*data);
				break;
			case _MSG_PORTAL:
				processPortal(*data);
				break;
			case _MSG_QUERY_SOCIAL_INFO:
				SocialScene::ProcessSocicalInfoData(*data);
				break;
			case MB_MSG_MOBILE_PWD:
			{
				int btAction = data->ReadByte();
				if (btAction == 3) 
				{
					std::string tip = data->ReadUnicodeString();
					showDialog(NDCommonCString("tip"), tip.c_str());
				}
			}
			case _MSG_MARRIAGE:
				processMarriage(*data);
				break;
			case _MSG_SHOW_TREASURE_HUNT_AWARD:
				processShowTreasureHuntAward(*data);
				break;
			case _MSG_RESPOND_TREASURE_HUNT_PR0B:
				processRespondTreasureHuntProb(*data);
				break;
			case _MSG_RESPOND_TREASURE_HUNT_INFO:
				processRespondTreasureHuntInfo(*data);
				break;
			case _MSG_KICK_OUT_TIP:
				processKickOutTip(*data);
				break;
			case _MSG_CHARGE_GIFT_INFO:
				RechargeUI::ProcessGiftInfo(*data);
				break;
			case _MSG_QUERY_PETCKILL:
				this->processQueryPetSkill(*data);
				break;
			case _MSG_ROADBLOCK:
				this->processRoadBlock(*data);
				break;
//			case _MSG_BOSS_BATTLE_INFO:
//				this->processBossInfo(*data);
//				break;
//			case _MSG_BOSS_BATTLE_SELF:
//				this->processBossSelfInfo(*data);
//				break;
			default:
				break;
		}
		
		return true;
	}
	
	void NDMapMgr::processRoadBlock(NDTransData& data)
	{
		int x=data.ReadInt();
		int y=data.ReadInt();
		unsigned int time=data.ReadInt();

		NDScene* scene = NDDirector::DefaultDirector()->GetScene(RUNTIME_CLASS(CSMGameScene));
		if(!scene)
		{
			return;
		}
		NDMapLayer* layer = getMapLayerOfScene(scene);
		if(!layer)
		{
			return;
		}
		//[layer->GetMapData() setRoadBlock:x roadBlockY:y];
		layer->setStartRoadBlockTimer(time,x,y);
		//NDLog("x:%d,y:%d,time:%d",x,y,time);
	}
	
//	void NDMapMgr::processBossInfo(NDTransData& data)
//	{
//		int btFlag = data.ReadByte();
//		unsigned int life = data.ReadInt();
//		unsigned int lifeLimit= data.ReadInt();
//		int count = data.ReadByte();
//		for(int i=0;i<count;i++)
//		{
//			OBJID id = data.ReadInt();
//			unsigned short rank = data.ReadShort();
//			unsigned int damage = data.ReadInt();
//			std::string name = data.ReadUnicodeString();
//		}
//	}
//	
//	void NDMapMgr::processBossSelfInfo(NDTransData& data)
//	{
//		unsigned int time = data.ReadInt();
//		int x = data.ReadInt();
//		unsigned int damage = data.ReadInt();
//	}
//	
	void NDMapMgr::processVersionMsg(NDTransData& data)
	{
		int updateFlag = data.ReadByte();
		std::string content = data.ReadUnicodeString();
		//std::string url = data.ReadUnicodeString();
		NDScene* scene = NDDirector::DefaultDirector()->GetScene(RUNTIME_CLASS(GameScene));
		if (scene) 
		{
			GameScene* gameScene = (GameScene*)scene;
			gameScene->processVersionMsg(content.c_str(), updateFlag, GetUpdateUrl().c_str());
		}
		
	}
	
	void NDMapMgr::processActivity(NDTransData& data)
	{
		CloseProgressBar;
		int flag = data.ReadByte();
		int amount = data.ReadByte();
		if (flag == 1) {
			//ActivityListScene::ClearData();
			CustomActivity::ClearData();
		}
		for (int i = 0; i < amount; i++) {
			std::string str = data.ReadUnicodeString();
			//ActivityListScene::AddData(str);
			CustomActivity::AddData(str);
		}
		if (flag == 2 || flag == 3) {
			//NDDirector::DefaultDirector()->PushScene(ActivityListScene::Scene());
			CustomActivity::refresh();
		}
		
	}
	
	void NDMapMgr::processDeleteRole(NDTransData& data)
	{
		CloseProgressBar;
		quitGame();
	}
	
	void NDMapMgr::AddSwitch()
	{
		NDScene *scene = NDDirector::DefaultDirector()->GetScene(RUNTIME_CLASS(CSMGameScene));
		NDMapLayer *layer = NULL;
		NDMapData* mapdata = NULL;
		if (!scene 
			|| (layer = getMapLayerOfScene(scene)) == NULL
			|| (mapdata = layer->GetMapData()) == NULL)
		{
			return;
		}
		
		ScriptDB& db	= ScriptDBObj;
		ID_VEC idlist;
		db.GetIdList("portal", idlist);
		for (ID_VEC::iterator it = idlist.begin(); it != idlist.end(); it++) 
		{
			int nMapId	= GetMapID();
			if (db.GetN("portal", *it, DB_PORTAL_MAPID) == nMapId)
			{
				int nIndex	= db.GetN("portal", *it, DB_PORTAL_PORTAL_INDEX);
				int nX		= db.GetN("portal", *it, DB_PORTAL_PORTALX);
				int nY		= db.GetN("portal", *it, DB_PORTAL_PORTALY);
				
				std::string desc = "��ͨ����";
				if  (1 == nIndex)
				{
					desc	= "��Ӣ����";
				}
				
				[mapdata addMapSwitch:nX
									Y:nY
								Index:nIndex
							 DesMapID:nMapId
						   DesMapName:[NSString stringWithUTF8String:desc.c_str()]
						   DesMapDesc:[NSString stringWithUTF8String:""]
				 ];
			}
		}
		
		layer->MapSwitchRefresh();
	}
	
	void NDMapMgr::processPortal(NDTransData& data)
	{
		CloseProgressBar;
		
		return;
		
		NDScene *scene = NDDirector::DefaultDirector()->GetScene(RUNTIME_CLASS(CSMGameScene));
		NDMapLayer *layer = NULL;
		NDMapData* mapdata = NULL;
		if (!scene 
			|| (layer = getMapLayerOfScene(scene)) == NULL
			|| (mapdata = layer->GetMapData()) == NULL)
		{
			return;
		}
		
		int amount = data.ReadByte();// ���������
		for (int i = 0; i < amount; i++) {
			short c = data.ReadShort();// ������ x
			short r = data.ReadShort();// ������ y
			int passwayIndex = data.ReadByte();// ����������
			int targetId = data.ReadInt(); // Ŀ���ͼid
			std::string targetName = data.ReadUnicodeString();// Ŀ���ͼ����
			std::string des = data.ReadUnicodeString();// Ŀ���ͼ����
			[mapdata addMapSwitch:c
								Y:r
							Index:passwayIndex
						 DesMapID:targetId
					   DesMapName:[NSString stringWithUTF8String:targetName.c_str()]
					   DesMapDesc:[NSString stringWithUTF8String:des.c_str()]
			 ];
		}
		
		if (amount > 0) 
		{
			layer->MapSwitchRefresh();
		}
		
	}
	
	void NDMapMgr::processSyndicate(NDTransData& data)
	{
		int answer = data.ReadByte();
		
		switch (answer) {
			case ANS_SYN_PANEL_INFO:
			{
				SynInfoUILayer* infoLayer = SynInfoUILayer::GetCurInstance();
				if (infoLayer) {
					infoLayer->processSynInfo(data);
				}
			}
				break;
			case ANS_REG_SYN_INFO:
			case ANS_SYN_LIST_INFO:
			case ANS_QUERY_VOTE_INFO:
			{
				CloseProgressBar;
				string t_wndTitle = data.ReadUnicodeString();
				string t_wndDetail = data.ReadUnicodeString();
				
				SyndicateInfoScene* synInfoScene = new SyndicateInfoScene;
				synInfoScene->Initialization(t_wndTitle, t_wndDetail);
				NDDirector::DefaultDirector()->PushScene(synInfoScene);
				//SyndicateInfoLayer::refresh(t_wndTitle, t_wndDetail);
				return;
			}
			case ANS_QUERY_OFFICER:
			{
				SynElectionUILayer* election = SynElectionUILayer::GetCurInstance();
				if (election) {
					election->processQueryOfficer(data);
				}
				//SyndicateElectionList::refreshScroll(data);
				return;
			}
			case ANS_QUERY_VOTE_LIST:{// �·�ͶƱ�б�
				SynVoteUILayer* voteLayer = SynVoteUILayer::GetCurInstance();
				if (voteLayer) {
					voteLayer->processVoteList(data);
				}
				//SyndicateVoteList::refreshScroll(data);
				return;
			}
			case ANS_QUERY_SYN_STORAGE:{
				SynDonateUILayer* donate = SynDonateUILayer::GetCurInstance();
				if (donate) {
					donate->processSynDonate(data);
				}
				//SyndicateStorage::refreshScroll(data);
				return;
			}
			case ANS_SYN_UPGRADE_INFO:{// ����������Ϣ
				SynUpgradeUILayer* upgrade = SynUpgradeUILayer::GetCurInstance();
				if (upgrade) {
					upgrade->processSynUpgrade(data);
				}
				//SyndicateUpgrade::refresh(data);
				return;
			}
			case APPROVE_ACCEPT_OK:
			case APPROVE_ACCEPT_FAIL:
			case APPROVE_REFUSE_OK: {
				CloseProgressBar;
				SynApproveUILayer* approve = SynApproveUILayer::GetCurInstance();
				if (approve) {
					approve->delCurSelCell();
				}
				//SyndicateApprove::delCurSelEle();
				break;
			}
			case ANS_UPDATE_SYN_MBR_RANK:
				NDPlayer::defaultHero().setSynRank(data.ReadByte());
				return;
			case QUIT_SYN_OK: {
				NDPlayer& role = NDPlayer::defaultHero();
				role.synName = "";
				role.setSynRank(SYNRANK_NONE);
				return;
			}
			default:
				break;
		}
	}
	
	void NDMapMgr::processDigout(NDTransData& data)
	{
		int itemID = data.ReadInt();
		data.ReadByte();// reserved
		int num = data.ReadByte();
		std::vector<int> stoneItemTypes;
		for (int i = 0; i < num; i++) {
			stoneItemTypes.push_back(data.ReadInt());
		}
		Item *item = NULL;
		VEC_ITEM& itemList = ItemMgrObj.GetPlayerBagItems();
		for (int i = 0; i < int(itemList.size()); i++) {
			item = itemList[i];
			if (item->iID == itemID) {
				break;
			}
		}
		if (item != NULL) {
			for (int i = 0; i < int(stoneItemTypes.size()); i++) {
				for (int j = 0; j < int(item->vecStone.size()); j++) {
					Item *itemStone = item->vecStone[j];
					if (itemStone->iItemType == stoneItemTypes[i]) {
						item->DelStone(itemStone->iID);
						break;
					}
				}
			}
		}
		RemoveStoneScene::Refresh();
		showDialog(NDCommonCString("tip"), NDCommonCString("WaChuBaoShiSucc"));
	}
	
	void NDMapMgr::processPlayerLevelUp(NDTransData& data)
	{
		std::stringstream msg;
		msg << NDCommonCString("up") << "������";
		int idPlayer = data.ReadInt();
		msg << " " << NDCommonCString("up") << NDCommonCString("object") << "��" << idPlayer;
		int dwNewExp = data.ReadInt();
		msg << " " << NDCommonCString("NewExpVal") << dwNewExp;
		int usNewLevel = data.ReadShort();
		msg << " " << NDCommonCString("NewLev") << usNewLevel;
		int usAddPoint = data.ReadShort();
		msg << " " << NDCommonCString("NewRestDian") << usAddPoint;
		
		NDPlayer& role = NDPlayer::defaultHero();
		if (idPlayer == role.m_id) {
			role.exp = dwNewExp;
			role.level = usNewLevel;
			role.restPoint = usAddPoint;
			
			role.levelup = true;
			role.playerLevelUp();
			
			// Npc.refreshNpcStateInMap(); // ����ˢһ�ε�ͼ������
			
			Chat::DefaultChat()->AddMessage(ChatTypeSystem, NDCommonCString("UpTip"));
			
			if (usNewLevel == 20) {
				Chat::DefaultChat()->AddMessage(ChatTypeSystem, NDCommonCString("UpTip20"));
			}
			
		} else {// �����������
			map_manualrole_it it = m_mapManualrole.begin();
			for (; it != m_mapManualrole.end(); it++)
			{
				NDManualRole& player = *(it->second);
				if (player.m_id == idPlayer) {
					player.exp = dwNewExp;
					player.level = usNewLevel;
					player.restPoint = usAddPoint;
					
					player.levelup = true;
					NDPlayer& player = NDPlayer::defaultHero();
					if (player.GetParent() && player.GetParent()->IsKindOfClass(RUNTIME_CLASS(GameScene))) 
					{
						player.playerLevelUp();
					}
					
					break;
				}
			}
		}
	}
	
	void NDMapMgr::processNPC(NDTransData& data)
	{
		CloseProgressBar;
		int iid = data.ReadInt(); // 4���ֽ� npc id
		int btAction = data.ReadByte(); // 1���ֽ� �������ͣ�EVENT_DELNPC = 3;
		int btType = data.ReadByte(); // 1���ֽ� NPC����
		
		if (btAction == 3) { // ɾ��NPC
			DelNpc(iid);
		} else if (btAction == 10) { // ������ʾ
			setNpcTaskStateById(iid, btType);
		}
	}
	
	void NDMapMgr::processSee(NDTransData& data)
	{
		CloseProgressBar;
		
		enum
		{
			SEE_USER_INFO		= 0,	// �鿴�����Ϣ
			SEE_EQUIP_INFO		= 1,	// �鿴���װ��
			SEE_PET_INFO		= 2,	// ��ѯ������Ϣ
			SEE_USER_PET_INFO	= 3,	// ��ѯ��ҳ�����Ϣ
			//��ѯ���
			SEE_OK				= 4,	// ��ѯ�ɹ�
			SEE_FAIL			= 5,	// ��ѯʧ��
			
			SEE_TUTOR_POS		= 6,	// ��ѯ��ʦͽ��ϵ���˵�λ��,
			SEE_PET_INFO_OK		= 7,
			SEE_USER_INFO_OK	= 8,
		};
		
		int action = data.ReadByte();// action
		int idTarget = data.ReadInt();// idtarget
		
		if (action == SEE_PET_INFO_OK)
		{
			// ��ѯ��ֻ����
			PetInfo* petInfo = PetMgrObj.GetPet(idTarget);
			if (petInfo)
			{
				NDDirector::DefaultDirector()->PushScene(NewPetInfoScene::Scene(idTarget));
			}
		}
		else if (action == SEE_USER_INFO_OK)
		{
			OtherPlayerInfoScene::ShowPlayerPet(idTarget);
		}
		
		std::string petName = data.ReadUnicodeString();
		
		//GameUIPetAttrib::ownerName = petName;
		//		NewGameUIPetAttrib::ownerName = petName;
		
		//GameUIPetAttrib *petattrib = new GameUIPetAttrib;
		//		petattrib->Initialization(false);
		//		NDScene *scene = NDDirector::DefaultDirector()->GetRunningScene();
		//		if (scene) 
		//		{
		//			scene->AddChild(petattrib, UILAYER_Z, UILAYER_PET_ATTRIB_TAG);
		//		}
		
		
		//		GamePetAttribScene *scene = new GamePetAttribScene;
		//		scene->Initialization(false);
		//		NDDirector::DefaultDirector()->PushScene(scene);
		//NDDirector::DefaultDirector()->PushScene(PetInfoScene::Scene(true));
		
		//PetAttr.ownerName = petName;
		//		
		//		T.addToTop(new PetAttr(false));
	}
	
	void NDMapMgr::processCampStorage(NDTransData& data)
	{
		int btCamp = data.ReadByte();
		// long reputeTotal = in.readLong();
		// long campReputeTotal = in.readLong();
		data.ReadLong();
		data.ReadLong();
		
		/*VEC_SOCIAL_ELEMENT elements;
		SocialElement *se = new SocialElement;
		se->m_text1 = NDCommonCString("money");
		std::stringstream money;
		money << data.ReadLong();
		se->m_text2 = money.str();
		elements.push_back(se);
		
		se = new SocialElement;
		se->m_text1 = NDCommonCString("emoney");
		std::stringstream emoney;
		emoney << data.ReadLong();
		se->m_text2 = emoney.str();
		elements.push_back(se);
		
		se = new SocialElement;
		se->m_text1 = NDCommonCString("WuQi");
		std::stringstream weapon;
		weapon << data.ReadInt();
		se->m_text2 = weapon.str();
		elements.push_back(se);
		
		se = new SocialElement;
		se->m_text1 = NDCommonCString("FangJu");
		std::stringstream def;
		def<< data.ReadInt();
		se->m_text2 = def.str();
		elements.push_back(se);
		
		se = new SocialElement;
		se->m_text1 = NDCommonCString("CaoYao");
		std::stringstream yao;
		yao << data.ReadInt();
		se->m_text2 = yao.str();
		elements.push_back(se);
		
		std::string strName = "";
		switch (btCamp) {
			case CAMP_NEUTRAL:
				strName = NDCommonCString("CountryStorage");
				break;
			case CAMP_TANG:
				strName = NDCommonCString("TangCampStorage");
				break;
			case CAMP_SUI:
				strName = NDCommonCString("SuiCampStorage");
				break;
			case CAMP_TUJUE:
				strName = NDCommonCString("TuCampStorage");
				break;
		}
		CloseProgressBar;
		CampDonateUpdate(btCamp, strName, elements);*/
		//if (ListScreen.instance != null) {
		//			ListScreen.instance.update(strName, listData,
		//									   ListScreen.LIST_FOR_DONATE);
		//		} else {
		//			T.addToTop(new ListScreen(strName, listData,
		//									  ListScreen.LIST_FOR_DONATE));
		//		}
		//		ListScreen.instance.setParam(btCamp);
		// ListScreen.instance.setListTitle("����������" + reputeTotal, "��Ӫ������" +
		// campReputeTotal);
	}
	
	void NDMapMgr::processNpcPosition(NDTransData& data)
	{
		int npcId = data.ReadInt();
		short col = data.ReadShort();
		short row = data.ReadShort();
		
		NDNpc *npc = GetNpc(npcId);
		if (npc != NULL) 
		{
			npc->AddWalkPoint(col, row);
		}
		
	}
	
	void NDMapMgr::processNpcTalk(NDTransData& data)
	{
		int act=data.ReadByte();
		int iid = data.ReadInt();
		int time=data.ReadInt();
		std::string msg = data.ReadUnicodeString2();
		switch(act){
			case 0:
			{
				NDManualRole *player=GetManualRole(iid);
				if(player!=NULL){
					player->addTalkMsg(msg,time);
				}
				break;
			}
			case 1:
			{
				CloseProgressBar;
				NDNpc *npc = GetNpc(iid);
				if (npc != NULL) {
					npc->addTalkMsg(msg,time);
				}
				break;
			}
			case 2:
			{
				NDMonster *monster=GetMonster(iid);
				if (monster != NULL) {
					monster->addTalkMsg(msg,time);
				}
			}
				break;
		}
	}
	
	void NDMapMgr::processItemTypeInfo(NDTransData& data)
	{
		int cnt = data.ReadByte();
		
		for (int i = 0; i < cnt; i++) {
			NDItemType *itemtype = new NDItemType;
			itemtype->m_data.m_id = data.ReadInt();
			itemtype->m_data.m_level =  data.ReadByte();
			itemtype->m_data.m_req_profession = data.ReadInt();
			itemtype->m_data.m_req_level = data.ReadShort();
			itemtype->m_data.m_req_sex = data.ReadShort();
			itemtype->m_data.m_req_phy = data.ReadShort();
			itemtype->m_data.m_req_dex = data.ReadShort();
			itemtype->m_data.m_req_mag = data.ReadShort();
			itemtype->m_data.m_req_def = data.ReadShort();
			itemtype->m_data.m_price = data.ReadInt();
			itemtype->m_data.m_emoney = data.ReadInt();
			itemtype->m_data.m_save_time = data.ReadInt();
			itemtype->m_data.m_life = data.ReadShort();
			itemtype->m_data.m_mana = data.ReadInt();
			itemtype->m_data.m_amount_limit = data.ReadShort();
			itemtype->m_data.m_hard_hitrate = data.ReadShort();
			itemtype->m_data.m_mana_limit = data.ReadShort();
			itemtype->m_data.m_atk_point_add = data.ReadShort();
			itemtype->m_data.m_def_point_add = data.ReadShort();
			itemtype->m_data.m_mag_point_add = data.ReadShort();
			itemtype->m_data.m_dex_point_add = data.ReadShort();
			itemtype->m_data.m_atk = data.ReadShort();
			itemtype->m_data.m_mag_atk = data.ReadShort();
			itemtype->m_data.m_def = data.ReadShort();
			itemtype->m_data.m_mag_def = data.ReadShort();
			itemtype->m_data.m_hitrate = data.ReadShort();
			itemtype->m_data.m_atk_speed = data.ReadShort();
			itemtype->m_data.m_dodge = data.ReadShort();
			itemtype->m_data.m_monopoly = data.ReadShort();
			itemtype->m_data.m_lookface = data.ReadInt();
			itemtype->m_data.m_iconIndex = data.ReadInt();
			itemtype->m_data.m_holeNum =  data.ReadByte();
			itemtype->m_data.m_suitData =data.ReadInt();
			itemtype->m_data.m_idUplev = data.ReadInt();
			itemtype->m_data.m_enhancedId = data.ReadInt();
			itemtype->m_data.m_enhancedStatus = data.ReadInt();
			itemtype->m_data.m_recycle_time = data.ReadInt();
			
			itemtype->m_name = data.ReadUnicodeString();
			itemtype->m_des = data.ReadUnicodeString();
			
			// ����Ϊ"��"�Ĳ���ʾ
			if (itemtype->m_des == NDCommonCString("wu")) {
				itemtype->m_des.clear();
			}
			// �Է����Ϊ׼,�洢������ڴ�����
			ItemMgrObj.ReplaceItemType(itemtype);
		}
	}
	
	void NDMapMgr::processWish(NDTransData& data)
	{
		CloseProgressBar;
		
		int act = data.ReadByte();
		int amount = data.ReadByte();
		switch(act){
			case 0:
			{
				data.ReadInt();
				std::string name=data.ReadUnicodeString();
				std::string date=data.ReadUnicodeString();
				std::string content=data.ReadUnicodeString();
				
				std::stringstream ss; 
				ss << content << "\n" << NDCommonCString("WishRen") << "��" << name << "\n" << date;
				GlobalShowDlg(NDCommonCString("WishContent"), ss.str().c_str());
			}
				break;
			case 1:
			{
				short curPage = data.ReadShort();
				short allPage = data.ReadShort();
				/*VEC_SOCIAL_ELEMENT wishes;
				for(int i=0;i<amount;i++){
					SocialElement *se = new SocialElement;
					se->m_id = data.ReadInt();
					se->m_text1 = data.ReadUnicodeString();
					se->m_text2 = data.ReadUnicodeString();
					wishes.push_back(se);
				}
				CompetelistUpdate(Competelist_Wish, curPage, allPage, wishes);*/
			}
				break;
		}
	}
	
	void NDMapMgr::processCompetition(NDTransData& data)
	{
		CloseProgressBar;
		int act = data.ReadByte();
		short curPage = data.ReadShort();
		short allPage = data.ReadShort();
		int amount =  data.ReadByte();
		/*VEC_SOCIAL_ELEMENT roles;
		for (int i = 0; i < amount;) 
		{
			SocialElement *se = new SocialElement;
			se->m_id = data.ReadInt();
			se->m_text1 = data.ReadUnicodeString();
			
			if (act == Competelist_VS) 
			{
				se->m_text2 = "VS";
				se->m_param = data.ReadInt();
				se->m_text3 = data.ReadUnicodeString();
			}
			else 
			{
				se->m_text2 = "   ";
			}
			
			
			roles.push_back(se);
			
			act == Competelist_Joins ? i++ : i+=2;
		}
		
		CompetelistUpdate(act, curPage, allPage, roles);*/
	}
	
	void NDMapMgr::processMsgSkill(NDTransData& data)
	{
		CloseProgressBar;
		int action = data.ReadByte();
		int skillID = data.ReadInt();
		if (action == 3) { // ɾ������
			NDPlayer& role = NDPlayer::defaultHero();
			role.DelSkill(skillID);
			ForgetSkillUILayer::RefreshSkillList();
		}
	}
	
	void NDMapMgr::processMsgPetSkill(NDTransData& data)
	{
		CloseProgressBar;
		int act = data.ReadByte();
		int petId = data.ReadInt();
		int skillId = data.ReadInt();
		NDPlayer& player = NDPlayer::defaultHero();
		//if (!player.battlepet) 
		//		{
		//			return;
		//		}
		//NDBattlePet& pet = *player.battlepet;
		if (act == 2) 
		{
			PetMgrObj.DelSkill(player.m_id, petId, skillId);
			//PetSkillSceneUpdate();
		}
		
		CUIPet *uiPet = PlayerInfoScene::QueryPetScene();
		if (uiPet)
		{
			uiPet->UpdateUI(petId);
		}
	}
	
	void NDMapMgr::processSkillGoods(NDTransData& data)
	{
		int idNpc = data.ReadInt();
		int count = data.ReadByte();
		
		VEC_BATTLE_SKILL& vSkill = this->m_mapNpcSkillStore[idNpc];
		
		for (int i = 0; i < count; i++) {
			vSkill.push_back(data.ReadInt());
		}
		
		LearnSkillUILayer::Show(vSkill);
	}
	
	void NDMapMgr::processReCharge(NDTransData& data)
	{
		int amount = data.ReadShort();
		int isEnd = data.ReadShort();
		for (int i = 0; i < amount; i++) 
		{
			int b1 = data.ReadInt();// �˵�ID
			int b2 = data.ReadByte();// �˵�����
			std::string s = data.ReadUnicodeString();// ����
			NDLog("\n===================[%d][%d][%@]", b1, b2, [NSString stringWithUTF8String:s.c_str()]);
			int id2 = b1 % 100;
			if (id2 == 0) 
			{
				//RechargeButton *btn = new  RechargeButton;
				//				btn->Initialization();
				//				btn->textPadding = b1 * 10 + b2;
				//				btn->SetTitle(s.c_str());
				//				btn->CloseFrame();
				//				RechargeScene::datas.push_back(btn);
				/* comment by jhzheng
				 RechargeData chargedata;
				 chargedata.title = s;
				 chargedata.textPadding = b1 * 10 + b2;
				 RechargeScene::datas.push_back(chargedata);
				 */
				
				//NDAsssert(b2 == RechargeData_MainType);
				
				RechargeUI::s_data.push_back(NewRechargeData());
				NewRechargeData& data = RechargeUI::s_data.back();
				data.mainData = NewRechargeSubData(b1, b2, s);
			} 
			else 
			{
				//if (!RechargeScene::datas.empty()) 
				//				{
				//					RechargeButton *btn = new  RechargeButton;
				//					btn->Initialization();
				//					btn->textPadding = b1 * 10 + b2;
				//					btn->SetTitle(s.c_str());
				//					
				//					RechargeScene*& parent = RechargeScene::datas.back();
				//					parent->AddChildRechargeButton(btn);
				//				}
				
				/* comment by jhzheng
				 if (!RechargeScene::datas.empty())
				 {
				 RechargeData& parentdata = RechargeScene::datas.back();
				 RechargeData childdata;
				 childdata.title = s;
				 childdata.textPadding = b1 * 10 + b2;
				 parentdata.children.push_back(childdata);
				 }
				 */
				
				if (!RechargeUI::s_data.empty())
				{
					NewRechargeData& data = RechargeUI::s_data.back();
					NDAsssert(b2 == RechargeData_Tip 
							  || b2 == RechargeData_Card
							  || b2 == RechargeData_Message);
					switch (b2) {
						case RechargeData_Tip:
							data.tipData = NewRechargeSubData(b1, b2, s);
							break;
						case RechargeData_Card:
						case RechargeData_Message:
							data.vSubData.push_back(NewRechargeSubData(b1, b2, s));
							break;
						default:
							break;
					}
				}
			}
		}
		
		if (isEnd == 1) 
		{
			/*
			 CloseProgressBar;
			 NDDirector::DefaultDirector()->PushScene(RechargeScene::Scene());
			 */
			CloseProgressBar;
			NDScene *scene = NDDirector::DefaultDirector()->GetRunningScene();
			if (scene && scene->IsKindOfClass(RUNTIME_CLASS(NewVipStoreScene)))
			{
				NewVipStoreScene *vipscene = (NewVipStoreScene*)scene;
				
				vipscene->ShowRechare();
			}
		}
	}
	
	void NDMapMgr::processRechargeReturn(NDTransData& data)
	{
		CloseProgressBar;
		int act = data.ReadByte();
		switch (act) 
		{
			case RECHARGE_RETURN_ACT_QUERY:
			{
				RechargeUI::ProcessQueryTip(data);
				/*
				 RechargeScene::rechargeInfoTitle=data.ReadUnicodeString();
				 RechargeScene::rechargeInfo=data.ReadUnicodeString();
				 NDScene *scene = NDDirector::DefaultDirector()->GetRunningScene();
				 if (scene && scene->IsKindOfClass(RUNTIME_CLASS(RechargeScene))) 
				 {
				 ((RechargeScene*)scene)->dealRecharge();
				 }*/
			}
				break;
			case RECHARGE_RETURN_ACT_TRUE:
			case RECHARGE_RETURN_ACT_FALSE:
			{
				std::string title = data.ReadUnicodeString();
				std::string content = data.ReadUnicodeString();
				GlobalShowDlg(title.c_str(), content.c_str());
			}
				break;
			case RECHARGE_RETURN_ACT_QUERY_RECORD:
			{
				RechargeRecordUI::prcessRecord(data);
				/* comment by jhzheng
				 int size = data.ReadByte();
				 VEC_SOCIAL_ELEMENT sociallist;
				 for (int i = 0; i < size; i++) 
				 {
				 std::string text1 = data.ReadUnicodeString();
				 int iValue = data.ReadByte();
				 std::stringstream ss; ss << iValue;
				 SocialElement *ele = new SocialElement;
				 ele->m_text1 = text1;
				 ele->m_text2 = ss.str();
				 sociallist.push_back(ele);
				 }
				 ReChargelistScene *scene = new ReChargelistScene;
				 scene->Initialization(sociallist);
				 NDDirector::DefaultDirector()->PushScene(scene);
				 */
			}
				break;
				
		}
	}
	
	void NDMapMgr::OnDialogClose(NDUIDialog* dialog)
	{
		OBJID idDlg = dialog->GetTag();
		
		if (idDlg == m_idDialogMarry)
		{ // ��ͬ����
			sendMarry(id2, id1, _MARRIAGE_REFUSE, 0);
		}
		
		this->m_idTradeDlg = ID_NONE;
		
		this->m_idAuctionDlg = ID_NONE;
		
		this->m_idDeMarry = ID_NONE;
		
		this->m_idDialogDemarry = ID_NONE;
		
		this->m_idDialogMarry = ID_NONE;
	}
	
	void NDMapMgr::OnDialogButtonClick(NDUIDialog* dialog, unsigned int buttonIndex)
	{
		OBJID idDlg = dialog->GetTag();
		if (idDlg == this->m_idTradeDlg) {
			m_idTradeDlg = ID_NONE;
			if (buttonIndex == 0) { // ͬ�⽻��
				NDManualRole* tradeRole = this->GetManualRole(this->m_idTradeRole);
				if (tradeRole) {
					NDPlayer& role = NDPlayer::defaultHero();
					role.stopMoving();
					NDUISynLayer::Show();
					NewTradeLayer::SendTrade(m_idTradeRole, 1);
				} else {
					NewTradeLayer::SendTrade(m_idTradeRole, 2);
				}
			} else if (buttonIndex == 1) { // �ܾ�����
				NewTradeLayer::SendTrade(this->m_idTradeRole, 2);
			}
		} else if (idDlg == this->m_idAuctionDlg) {
			m_idAuctionDlg = ID_NONE;
			switch (buttonIndex) {
				case 0: // �ҵ�����
				{
					AuctionUILayer::Show(1);
					ShowProgressBar;
					NDTransData bao(_MSG_AUCTION);
					bao << Byte(AUCTION_USER) << 0 << 1;
					// SEND_DATA(bao);
				}
					break;
				case 1: // ������Ʒ
				{
					AuctionSearchLayer::Show();
				}
					break;
				default:
					break;
			}
		}
		else if (idDlg == this->m_idDeMarry) {
			m_idDeMarry = ID_NONE;
			NDPlayer& player = NDPlayer::defaultHero();
			sendMarry(player.m_id, player.marriageID, _DIVORCE_APPLY, 0);
		}
		else if (idDlg == this->m_idDeMarry) {
			if (buttonIndex == 0)
			{ //ͬ�����
				sendMarry(id1, id2, _DIVORCE_AGREE, 0);
			}
		}
		else if (idDlg == this->m_idDialogMarry) {
			if (buttonIndex == 0)
			{ // ͬ����
				this->m_idDialogMarry = ID_NONE;
				sendMarry(id2, id1, _MARRIAGE_AGREE, 0);
				NDPlayer::defaultHero().marriageID = id1;
			}
			else if (buttonIndex == 1)
			{ // ��ͬ����
				
			}
		}
		
		dialog->Close();
	}
	
	//bool NDMapMgr::OnCustomViewConfirm(NDUICustomView* customView)
	//{
	//	std::string stramount =	customView->GetEditText(0);
	//	int iTag = customView->GetTag();
	//	if (iTag == eCVOP_GiftNPC) 
	//	{
	//		if (stramount.size() != KEY_LENGTH) 
	//		{
	//			std::stringstream ss; 
	//			ss << NDCommonCString("XuLieHaoLen") << int(KEY_LENGTH) << NDCommonCString("wei") << "��";
	//			customView->ShowAlert(ss.str().c_str());
	//			return false;
	//		}
	//		
	//		NDTransData bao(_MSG_GIFT);
	//		bao.WriteUnicodeString(stramount);
	//		// SEND_DATA(bao);
	//	}
	//	else if (iTag == eCVOP_Wish) 
	//	{
	//		if (stramount.empty())
	//		{
	//			customView->ShowAlert(NDCommonCString("FailInputWish"));
	//			return false;
	//		}
	//		
	//		ShowProgressBar;
	//		NDTransData bao(_MSG_WISH);
	//		bao << (unsigned char)2 << (unsigned char)0;
	//		bao.WriteUnicodeString(stramount);
	//		// SEND_DATA(bao);
	//	}
	//	else if (iTag == eCVOP_FarmName ||
	//			 iTag == eCVOP_FarmWelcomeName ||
	//			 iTag == eCVOP_FarmHarmletName)
	//	{
	//		NDTransData bao(_MSG_SET_FARM_INFO);
	//		int para = iTag == eCVOP_FarmName ? 0 : (iTag == eCVOP_FarmWelcomeName ? 1 : 2);
	//		bao << (unsigned char)para;
	//		if (iTag == eCVOP_FarmHarmletName) bao << int(m_iCurDlgNpcID);
	//		bao.WriteUnicodeString(stramount);
	//		// SEND_DATA(bao);
	//	}
	//	else if (iTag == eCVOP_FarmBuildingName)
	//	{
	//		NDTransData bao(_MSG_SET_FARM_ENTITY_NAME);
	//		bao << int(m_iCurDlgNpcID);
	//		bao.WriteUnicodeString(stramount);
	//		// SEND_DATA(bao);
	//	}
	//	
	//	return true;
	//}
	//
	void NDMapMgr::processTrade(NDTransData& data)
	{
		this->m_idTradeRole = data.ReadInt(); // 4���ֽ�
		int action = data.ReadByte(); // 1���ֽ�
		
		NDManualRole* tradeRole = this->GetManualRole(m_idTradeRole);
		if (action == 0) { // ������
			if (tradeRole) {
				string strContent = tradeRole->m_name + " " + NDCommonCString("ReqTrade");
				this->m_idTradeDlg = GlobalDialogObj.Show(this, NDCommonCString("trade"), strContent.c_str(), NULL, NDCommonCString("agree"), NDCommonCString("reject"), NULL);
				
			} else { // ȡ������
				NewTradeLayer::SendTrade(this->m_idTradeRole, 2);
			}
		} else {
			NewTradeLayer::processTrade(tradeRole, m_idTradeRole, action);
		}
	}
	
	void NDMapMgr::processBillBoard(NDTransData& data)
	{
		CloseProgressBar;
		int count = 0; data >> count;
		std::vector<int> paihangIDS;
		std::vector<std::string> paihangNames;
		for (int i = 0; i < count; i++) 
		{
			paihangIDS.push_back(data.ReadInt());
			paihangNames.push_back(data.ReadUnicodeString());
		}
		
		NDDirector::DefaultDirector()->PushScene(NewPaiHangScene::Scene(paihangNames, paihangIDS));
		/*
		 NDScene *scene = NDDirector::DefaultDirector()->GetRunningScene();
		 if (scene && scene->IsKindOfClass(RUNTIME_CLASS(GameScene))) 
		 {
		 ((GameScene*)scene)->ShowPaiHang(paihangNames, paihangIDS);
		 }
		 */
	}
	
	void NDMapMgr::processMsgCommonList(NDTransData& data)
	{
		int iID = data.ReadInt();
		int field_count = data.ReadInt();
		int button_count = data.ReadInt();
		string title = data.ReadUnicodeString();
		
		NewPaiHangScene::curPaiHangType = iID;
		
		std::vector<int>& fildTypes = NewPaiHangScene::s_PaiHangTitle[iID].fildTypes;
		std::vector<std::string>& fildNames = NewPaiHangScene::s_PaiHangTitle[iID].fildNames;
		
		fildTypes.clear();
		fildNames.clear();
		
		for (int i = 0; i < field_count; i++) {
			fildTypes.push_back(data.ReadByte());
			fildNames.push_back(data.ReadUnicodeString());
		}
		
		for (int i = 0; i < button_count; i++) {
			data.ReadUnicodeString();
		}
	}
	
	void NDMapMgr::processBillBoardField(NDTransData& data)
	{
		//GameUIPaiHang::DataInit();
		
		unsigned char count = 0; data >> count;
		
		PaiHangTitle phTitle;
		//std::vector<int> fildTypes; = NewPaiHangScene::fildTypes;
		//std::vector<std::string> fildNames; = NewPaiHangScene::fildNames;
		
		//fildTypes.clear();
		//fildNames.clear();
		
		for (int i = 0; i < count; i++) 
		{
			phTitle.fildTypes.push_back(data.ReadByte());
			phTitle.fildNames.push_back(data.ReadUnicodeString());
		}
		
		int iid = data.ReadInt();
		int itype = data.ReadByte();
		
		NewPaiHangScene::s_PaiHangTitle[iid] = phTitle;
		//NewPaiHangScene::fieldId.push_back(iid);
		
		NewPaiHangScene::curPaiHangType = iid;
		NewPaiHangScene::itype = itype;
	}
	
	void NDMapMgr::processMsgCommonListRecord(NDTransData& data)
	{
		int packageFlag = data.ReadByte();
		int curCount = data.ReadByte();
		data.ReadShort();
		
		
		std::vector<int>& fildTypes = NewPaiHangScene::s_PaiHangTitle[NewPaiHangScene::curPaiHangType].fildTypes;
		std::vector<std::string>& values = NewPaiHangScene::values[NewPaiHangScene::curPaiHangType];
		values.clear();
		for (int i = 0; i < curCount; i++) {
			data.ReadInt(); // button id
			for (int j = 0; j < int(fildTypes.size()); j++) {
				if (fildTypes[j] == 0) {
					std::stringstream ss; ss << (data.ReadInt());
					values.push_back(ss.str());
				} else if (fildTypes[j] == 1) {
					values.push_back(data.ReadUnicodeString());				
				}
			}
		}
		
		if ((packageFlag & 2) > 0) {
			NewPaiHangScene::refresh();
			CloseProgressBar;
		}
	}
	
	void NDMapMgr::processBillBoardList(NDTransData& data)
	{
		int flag = data.ReadByte();
		int curCount = data.ReadByte();
		if (flag == 0) 
		{// continue
		}
		else if (flag == 1) 
		{// start
			NewPaiHangScene::totalPages = data.ReadShort();
		} 
		else if (flag == 2) 
		{// end
			
		} 
		else if (flag == 3) 
		{// single package
			NewPaiHangScene::totalPages = data.ReadShort();
		}
		std::vector<int>& fildTypes = NewPaiHangScene::s_PaiHangTitle[NewPaiHangScene::curPaiHangType].fildTypes;
		std::vector<std::string>& values = NewPaiHangScene::values[NewPaiHangScene::curPaiHangType];
		values.clear();
		for (int i = 0; i < curCount; i++) {
			for (int j = 0; j < int(fildTypes.size()); j++) {
				if (fildTypes[j] == 0) {
					std::stringstream ss; ss << (data.ReadInt());
					values.push_back(ss.str());
				} else if (fildTypes[j] == 1) {
					values.push_back(data.ReadUnicodeString());				
				}
			}
		}
		if (flag == 3 || flag == 2) {
			//String values[] = new String[paihangRecords.size()];
			//			for (int i = 0; i < values.length; i++) {
			//				values[i] = (String) paihangRecords.elementAt(i);
			//			}
			//			PaiHang.getInstance().setallValue(values);
			//			T.closeSynDialog();
			//			PaiHang.getInstance().makeContent();
			//			Array dialog = T.getDialogList();
			//			if (!dialog.contain(PaiHang.getInstance())) {
			//				T.addDialog(PaiHang.getInstance());
			//			}
			//GameScene::ShowUIPaiHang();
			NewPaiHangScene::refresh();
			CloseProgressBar;
		}
	}
	
	void NDMapMgr::processBillBoardUser(NDTransData& data)
	{
		int rank = data.ReadInt();
		std::stringstream ss;
		if (rank == 0) {
			ss << NDCommonCString("YouNotOnBang");
		} else {
			ss << NDCommonCString("YouMingCi") << rank;
		}
		GlobalShowDlg(NDCommonCString("PaiMing"), ss.str().c_str());
	}
	
	void NDMapMgr::processShopInfo(NDTransData& data)
	{
		int shopID = data.ReadInt();
		int itemNum = data.ReadByte();
		
		vector<ShopItemInfo>& vShopItemInfo = m_mapNpcStore[shopID];
		
		vShopItemInfo.clear();
		
		std::stringstream sb; sb << "�̵�ID" << shopID;
		sb << (" ��ƷID�б�Ϊ");
		for (int i = 0; i < itemNum; i++) {
			int itemID = data.ReadInt();
			int payType = data.ReadByte();
			
			vShopItemInfo.push_back(ShopItemInfo(itemID, payType));
			
			sb << itemID;
			sb << ";";
		}
		
		//m_mapNpcStore.insert(map_npc_store_pair(shopID, idList));
		//if (DepolyCfg.debug) {
		//			ChatUI.addChatRecodeChatList(new ChatRecord(1, "�̵���Ʒ", sb
		//														.toString()));
		//		}
		
		GameUINpcStore::GenerateNpcItems(shopID);
		
		CloseProgressBar;
		if (shopID == 0) {
			//T.addDialog(new VipStore(itemList));
		}
		else if (shopID == 99998)
		{
			GameScene::ShowShop(shopID);
		} 
		else 
		{
			GameScene::ShowShop();
		}
	}
	
	void NDMapMgr::processShop(NDTransData& data)
	{
		/*int itemID = */data.ReadInt(); // ��Ʒid
		data.ReadInt();
		int action = data.ReadByte();
		/*int amount = */data.ReadByte();
		
		CloseProgressBar;
		
		switch (action) 
		{
			case Item::_SHOPACT_BUY: {
				
				NpcStoreUpdateBag();
				
				//if (VipStore.instance != null) {
				//					VipStore.instance.refreshBag();
				//					if (DepolyCfg.debug) {
				//						System.out.println("bag store refresh");
				//					}
				//				}
				break;
			}
				
			case Item::_SHOPACT_SELL: {
				NpcStoreUpdateBag();
				break;
			}
				
		}
	}
	
	void NDMapMgr::processUserInfoSee(NDTransData& data)
	{
		std::deque<string> deqStrings;
		int targeId = data.ReadInt();
		
		// �罻�õ������ݲɼ�
		SocialData social;
		social.iId = targeId;
		
		std::stringstream sb;
		int bSex = data.ReadByte(); // 1��2Ů
		sb << NDCommonCString("sex") << "       " << NDCommonCString("bie") << ": ";
		if (bSex == USER_SEX_MALE) {
			social.sex = NDCommonCString("male");
			sb << NDCommonCString("male");
		} else {
			social.sex = NDCommonCString("female");
			sb << NDCommonCString("female");
		}
		deqStrings.push_back(sb.str());
		sb.str("");
		int bLevel = data.ReadByte(); // �ȼ�
		sb << NDCommonCString("deng") << "       " << NDCommonCString("Ji") << ": " << bLevel;
		deqStrings.push_back(sb.str());
		sb.str("");
		
		social.lvl = bLevel;
		/* byte bProfession = (byte) */data.ReadByte(); // ְҵ
		// sb.append("ְҵ: ");
		// switch (bProfession) {
		// case ManualRole.PRO_WARRIOR: {
		// sb.append("սʿ\n");
		// break;
		// }
		// case ManualRole.PRO_WIZARD: {
		// sb.append("��ʦ\n");
		// break;
		// }
		// case ManualRole.PRO_ASSASIN: {
		// sb.append("�̿�\n");
		// break;
		// }
		// }
		int iPkPoint = data.ReadInt(); // PKֵ
		int iHornor = data.ReadInt(); // ����ֵ
		
		sb << "PK" << "      " << NDCommonCString("val") << ": " << iPkPoint;
		deqStrings.push_back(sb.str());
		sb.str("");
		
		sb << NDCommonCString("HonurVal") << ": " << iHornor;
		deqStrings.push_back(sb.str());
		sb.str("");
		
		int bShow = data.ReadByte(); // ָ���Ƿ��а���,��ż,��λȡ
		std::string name = data.ReadUnicodeString(); // �������
		sb << NDCommonCString("PlayerXingMing") << ": " << name; // ��������
		deqStrings.push_front(sb.str());
		sb.str("");
		
		bool isHaveSyn = false; // �Ƿ��а���
		bool isMarry = false; // �Ƿ���
		for (int i = 0; i < 2; i++) {
			int a = bShow & 0x1;
			if (a == 1) {
				if (i == 0) {
					isHaveSyn = true;
				} else {
					isMarry = true;
				}
			}
			bShow = bShow >> 1;
		}
		if (isHaveSyn) {
			std::string synName = data.ReadUnicodeString(); // ��������
			int bRank = data.ReadByte(); // ���ɵȼ�
			sb << NDCommonCString("JunTuanName") << ": " << synName;
			deqStrings.push_back(sb.str());
			sb.str("");
			
			sb << NDCommonCString("JunTuanPost") << ": " << getRankStr(bRank);
			deqStrings.push_back(sb.str());
			sb.str("");
			
			social.SynName = synName;
			social.rank = getRankStr(bRank);
		} else {
			sb << NDCommonCString("jun") << "       " << NDCommonCString("tuan") << ": " << NDCommonCString("wu");
			deqStrings.push_back(sb.str());
			sb.str("");
		}
		if (isMarry) {
			std::string marryName = data.ReadUnicodeString(); // ��ż����
			sb << NDCommonCString("pei") << "       " << NDCommonCString("ou") << ": " << marryName;
			deqStrings.push_back(sb.str());
			sb.str("");
		} else {
			sb << NDCommonCString("pei") << "       " << NDCommonCString("ou") << ": " << NDCommonCString("wu");
			deqStrings.push_back(sb.str());
			sb.str("");
		}
		
		CloseProgressBar;
		OtherPlayerInfoScene::showPlayerInfo(deqStrings);
		
		//TutorUILayer::processSocialData(social);
		//NewGoodFriendUILayer::processSocialData(social);
		SynMbrListUILayer* mbrList = SynMbrListUILayer::GetCurInstance();
		if (mbrList) {
			mbrList->processSocialData(social);
		}
		//GlobalShowDlg("�����Ϣ", content.str());
	}
	
	void NDMapMgr::processEquipInfo(NDTransData& data)
	{
		int idTarget = data.ReadInt();
		int lookface = data.ReadInt();
		
		NewGoodFriendUILayer::processSeeEquip(idTarget, lookface);
		TutorUILayer::processSeeEquip(idTarget, lookface);
		OtherPlayerInfoScene::showPlayerEquip();
		CloseProgressBar;
		/*ManualRoleEquipScene *scene = new ManualRoleEquipScene;
		 scene->Initialization();
		 scene->SetRole(lookface, idTarget);
		 scene->LoadRoleEquip(ItemMgrObj.GetOtherItem());
		 NDDirector::DefaultDirector()->PushScene(scene);*/
	}
	
	void NDMapMgr::processEquipImprove(NDTransData& data)
	{
		int btAction = data.ReadByte();
		
		switch (btAction) {
			case EQUIP_IM_QUALITY_FALSE: {
				Chat::DefaultChat()->AddMessage(ChatTypeSystem, NDCommonCString("EquipUpFail"));
				//if (GameScreen.getInstance() != null) {
				//					GameScreen.getInstance().initNewChat(new ChatRecord(5, "ϵͳ", "�ܱ�Ǹ��װ������ʧ����"));
				//				}
				break;
			}
			case EQUIP_IM_QUALITY: {
				Chat::DefaultChat()->AddMessage(ChatTypeSystem, NDCommonCString("EquipUpSucess"));
				//if (GameScreen.getInstance() != null) {
				//					GameScreen.getInstance().initNewChat(new ChatRecord(5, "ϵͳ", "��ϲ����װ�������ɹ�"));
				//				}
				break;
			}
			case EQUIP_IM_ENHANCE_FALSE: {
				Chat::DefaultChat()->AddMessage(ChatTypeSystem, NDCommonCString("QiangHuaFail"));
				//if (GameScreen.getInstance() != null) {
				//					GameScreen.getInstance().initNewChat(new ChatRecord(5, "ϵͳ", "�ܱ�Ǹ��װ��ǿ��ʧ����"));
				//				}
				break;
			}
			case EQUIP_IM_ENHANCE: {
				Chat::DefaultChat()->AddMessage(ChatTypeSystem, NDCommonCString("QiangHuaSucess"));
				//if (GameScreen.getInstance() != null) {
				//					GameScreen.getInstance().initNewChat(new ChatRecord(5, "ϵͳ", "��ϲ����װ��ǿ���ɹ�"));
				//				}
				break;
			}
			case EQUIP_IM_ADD_HOLE: {
				Chat::DefaultChat()->AddMessage(ChatTypeSystem, NDCommonCString("KaiDonSucess"));
				//if (GameScreen.getInstance() != null) {
				//					GameScreen.getInstance().initNewChat(new ChatRecord(5, "ϵͳ", "��ϲ����װ�������ɹ�"));
				//				}
				break;
			}
			case EQUIP_IM_QUALITY_DISTROY: {
				Chat::DefaultChat()->AddMessage(ChatTypeSystem, NDCommonCString("KaiDongFail"));
				//if (GameScreen.getInstance() != null) {
				//					GameScreen.getInstance().initNewChat(new ChatRecord(5, "ϵͳ", "���ź�������װ��������"), true);
				//				}
				break;
			}
			case EQUIP_IM_UPLEV:{
				Chat::DefaultChat()->AddMessage(ChatTypeSystem, NDCommonCString("EquipUpSucess"));
				break;
			}
		}
		
		EquipUpgradeScene::Refresh();
		OpenHoleScene::Refresh();
		
		if (btAction == EQUIP_IM_QUALITY_FALSE) {
			
		} else {
			// int idEquip = input.readInt();
			// int newEquipItemtype = input.readInt();
			//
			// for (int i = 0; i < T.itemList.size(); i++) {
			// Item item = (Item) T.itemList.elementAt(i);
			// if (item.itemID == idEquip) {
			// item.itemType = newEquipItemtype;
			// break;
			// }
			// }
			
		}
		CloseProgressBar;
		EquipForgeScene* equipForge = EquipForgeScene::GetCurInstance();
		if (equipForge) {
			equipForge->processEquipImprove();
		}
	}
	
	void NDMapMgr::processFormula(NDTransData& data)
	{
		int byStudyType = data.ReadByte();//0���·���1��ѧϰ
		int btSkillCount = data.ReadByte();
		for(int i = 0;i<btSkillCount;i++){
			int t_idFormula = data.ReadInt();
			int t_lev = data.ReadByte();
			int t_matID_1 = data.ReadInt();
			int t_matCount_1 = data.ReadByte();
			int t_matID_2 = data.ReadInt();
			int t_matCount_2 = data.ReadByte();
			int t_matID_3 = data.ReadInt();
			int t_matCount_3 = data.ReadByte();
			int t_producID = data.ReadInt();
			int t_money = data.ReadInt();
			int t_emoney = data.ReadInt();
			// TODO: ����ͼ�������ֶ�
			int iconIndex = data.ReadInt();
			std::string s_formulaName = data.ReadUnicodeString();
			std::string strIconName = data.ReadUnicodeString();
			FormulaMaterialData *t_formula = getFormulaData(t_idFormula);
			if(t_formula != NULL){
				t_formula->init(t_lev,t_matID_1,t_matCount_1,t_matID_2,t_matCount_2,t_matID_3,t_matCount_3,t_producID,t_money,t_emoney,iconIndex);
			}else{
				m_mapFomula.insert( map_fomula_pair(t_idFormula,
													new FormulaMaterialData(t_idFormula,t_lev,t_matID_1,t_matCount_1,t_matID_2,t_matCount_2,t_matID_3,t_matCount_3,t_producID,t_money,t_emoney,s_formulaName,iconIndex) ) 
								   );
			}
			if(byStudyType == 1){//0���·���1��ѧϰ
				//					String formulaName = new Item(t_idFormula).getItemName();
				Item item(t_idFormula);
				std::stringstream ss; ss << NDCommonCString("Common_LearnedTip") << item.getItemName();
				GlobalShowDlg(NDCommonCString("OperateSucess"), ss.str());
			}
		}
		CloseProgressBar;
	}
	
	//////////////////////////////////////////////////////////////////////////
	//��Ϣ����
	void NDMapMgr::processUserInfo(NDTransData* data, int len)
	{
		int id = 0; (*data) >> id; // �û�id 4���ֽ�
		int dwLookFace = 0; (*data) >> dwLookFace; // ���������ʱ����6����ۿɹ�ѡ����� ����
		int nLife = 0; (*data) >> nLife; // ����ֵ4���ֽ�
		int nMaxLife = 0; (*data) >> nMaxLife;// �������ֵ4���ֽ�
		int nMana = 0; (*data) >> nMana; // ħ��ֵ
		
		int nMaxMana = 0; (*data) >> nMaxMana; // ���ħ��ֵ
		
		int nMoney = 0; (*data) >> nMoney; // ���� 4���ֽ�
		unsigned char btLevel = 0; (*data) >> btLevel; // �û��ȼ�
		unsigned short usRecordX = 0; (*data) >> usRecordX; // ��¼����ϢX
		unsigned short usRecordY = 0; (*data) >> usRecordY; // ��¼����ϢY
		int idRecordMap = 0; (*data) >> idRecordMap; // ��¼��ĵ�ͼ id
		int dwStorageLimit = 0; (*data) >> dwStorageLimit; // �ֿ���� 4���ֽ�
		int dwPackLimit = 0; (*data) >> dwPackLimit;; // �������� 4���ֽ�
		unsigned char btProfession = 0; (*data) >> btProfession; // ��ҵ�ְҵ 1���ֽ�
		int dwEMoney = 0; (*data) >> dwEMoney; // Ԫ�� 4���ֽ�
		int storageMoney = 0; (*data) >> storageMoney; // �ֿ�����4���ֽ�
		int storageEMoney = 0; (*data) >> storageEMoney; // �ֿ�Ԫ��4���ֽ�
		int dwPhyPoint = 0; (*data) >> dwPhyPoint; // ������4���ֽ�
		int dwDexPoint = 0; (*data) >> dwDexPoint; // ���ݵ�4���ֽ�
		int dwMagPoint = 0; (*data) >> dwMagPoint; // ������4���ֽ�
		int dwDefPoint = 0; (*data) >> dwDefPoint; // ������4���ֽ�
		int dwExp = 0; (*data) >> dwExp;// ����ֵ4���ֽ�
		int dwLevelUpExp = 0; (*data) >> dwLevelUpExp;// �´��������辭��
		int dwResetPoint = 0; (*data) >> dwResetPoint; // ʣ����� 4���ֽ�
		int dwSkillPoint = 0; (*data) >> dwSkillPoint;// ����ֵ
		int dwPkPoint = 0; (*data) >> dwPkPoint; // pkֵ
		unsigned char btCamp = 0;(*data) >> btCamp;  // ��Ӫ
		
		NDPlayer::pugeHero();
		NDPlayer& player = NDPlayer::defaultHero(dwLookFace, true);
		player.m_caclData.extra_phyPoint=data->ReadInt();
		player.m_caclData.extra_dexPoint=data->ReadInt();
		player.m_caclData.extra_magPoint=data->ReadInt();
		player.m_caclData.extra_defPoint=data->ReadInt();
		player.m_caclData.phy_atk=data->ReadInt();
		player.m_caclData.phy_def=data->ReadInt();
		player.m_caclData.mag_atk=data->ReadInt();
		
		player.m_caclData.mag_def=data->ReadInt();
		player.m_caclData.atk_speed=data->ReadInt();
		player.m_caclData.hitrate=data->ReadInt();
		player.m_caclData.dodge=data->ReadInt();
		player.m_caclData.hardhit=data->ReadInt();
		player.jifeng = data->ReadInt();
		player.activityValue = data->ReadShort();
		player.activityValueMax = data->ReadShort();
		
		std::string name = data->ReadUnicodeString();
		player.m_id = id;
		player.InitRoleLookFace(dwLookFace);
		player.dwLookFace = dwLookFace;
		player.life = nLife;
		player.maxLife = nMaxLife;
		player.mana = nMana;
		player.maxMana = nMaxMana;
		player.money = nMoney;
		player.level = btLevel;
		player.iRecordMap = idRecordMap;
		player.iProfesstion = btProfession;
		player.eMoney = dwEMoney;
		player.iStorgeMoney = storageMoney;
		player.iStorgeEmoney = storageEMoney;
		player.phyPoint = dwPhyPoint;
		player.dexPoint = dwDexPoint;
		player.magPoint = dwMagPoint;
		player.defPoint = dwDefPoint;
		player.exp = dwExp;
		player.lvUpExp = dwLevelUpExp;
		player.restPoint = dwResetPoint;
		player.iSkillPoint = dwSkillPoint;
		player.pkPoint = dwPkPoint;
		player.SetCamp((CAMP_TYPE)btCamp);
		player.m_name = name;
		player.setSafeProtected(true);
		// ���ܲ�����
		player.m_nMaxSlot	= data->ReadByte();
		player.m_nVipLev	= data->ReadByte();
		player.m_nPeerage	= data->ReadByte(); 
		player.m_nRank		= data->ReadByte(); 
		
		
		ItemMgrObj.SetBagLitmit(dwPackLimit);
		ItemMgrObj.SetStorageLitmit(dwStorageLimit);
		//NDLog("%d", layer->GetChildren().size()); 
		
		player.stopMoving();
		player.SetPositionEx(ccp(usRecordX*MAP_UNITSIZE+DISPLAY_POS_X_OFFSET, usRecordY*MAP_UNITSIZE+DISPLAY_POS_Y_OFFSET));
		player.SetServerPositon(usRecordX, usRecordY);
		
		BeatHeartMgrObj.Start();
		//NDLog("�������:%@", [NSString stringWithUTF8String:name.c_str()]);
	}
	void NDMapMgr::processUserAttrib(NDTransData* data, int len)
	{
		int idUser = 0; (*data) >> idUser;
		
		NDPlayer& player = NDPlayer::defaultHero();
		if (idUser != player.m_id) // ��������Լ���ҵ�id������
		{
			return;
		}
		
		unsigned char dwAttributeNum = 0; (*data) >> dwAttributeNum; // 1�ֽ�
		
		for (int i = 0; i < dwAttributeNum; i++)
		{
			int ucAttributeType = 0; (*data) >> ucAttributeType;
			int dwAttributeData = 0; (*data) >> dwAttributeData;
			
			switch (ucAttributeType) {
				case USERATTRIB_MONEY: {
					player.money = dwAttributeData;
					NpcStoreUpdateMoney();
					GameStorageUpdateMoney();
					VendorUILayer::UpdateMoney();
					VendorBuyUILayer::UpdateMoney();
					GamePlayerBagUpdateMoney();
					NewGamePlayerBagUpdateMoney();
					updateTaskMoneyData(dwAttributeData, true);
					break;
				}
				case USERATTRIB_EMONEY: {
					NpcStoreUpdateMoney();
					player.eMoney = dwAttributeData;
					GameStorageUpdateMoney();
					//VipStoreUpdateEmoney();
					NewVipStoreScene::UpdateShop();
					VendorUILayer::UpdateMoney();
					VendorBuyUILayer::UpdateMoney();
					GamePlayerBagUpdateMoney();
					NewGamePlayerBagUpdateMoney();
					break;
				}
				case USERATTRIB_MAXLIFE: {
					player.SetMaxLife(dwAttributeData);
					break;
				}
				case USERATTRIB_LIFE: {
					player.life = dwAttributeData;
					break;
				}
				case USERATTRIB_MAXMANA: {
					player.SetMaxMana(dwAttributeData);
					break;
				}
				case USERATTRIB_MANA: {
					player.mana = dwAttributeData;
					break;
				}
				case USERATTRIB_LEV: {
					player.level = dwAttributeData;
					break;
				}
				case USERATTRIB_EXP: {
					player.exp = dwAttributeData;
					break;
				}
				case USERATTRIB_PK: {
					player.pkPoint = dwAttributeData;
					break;
				}
				case USERATTRIB_PHY_POINT: {
					player.phyPoint = dwAttributeData;
					break;
				}
				case USERATTRIB_DEX_POINT: {
					player.dexPoint = dwAttributeData;
					break;
				}
				case USERATTRIB_MAG_POINT: {
					player.magPoint = dwAttributeData;
					break;
				}
				case USERATTRIB_DEF_POINT: {
					player.defPoint = dwAttributeData;
					break;
				}
				case USERATTRIB_ADDPOINT: {
					player.restPoint = dwAttributeData;
					break;
				}
				case USERATTRIB_EXPMEDICAL_LEVEL: {
					player.lvUpExp = dwAttributeData;
					break;
				}
				case USERATTRIB_REIKI: {
					player.iSkillPoint = dwAttributeData;
					break;
				}
				case USERATTRIB_STATUS: {
					bool bDead = player.IsInState(USERSTATE_DEAD);
					player.SetState(dwAttributeData);
					if (bDead && !player.IsInState(USERSTATE_DEAD) ) {
						player.setSafeProtected(true);
						NDUISynLayer::Close();
						NDScene* gameScene = NDDirector::DefaultDirector()->GetRunningScene();
						if (gameScene && gameScene->IsKindOfClass(RUNTIME_CLASS(GameScene))) {
							((GameScene*)gameScene)->ShowRelieve(false);
						}
						
					} else if (player.IsInState(USERSTATE_DEAD)) {
						NDScene* gameScene = NDDirector::DefaultDirector()->GetRunningScene();
						if (gameScene && gameScene->IsKindOfClass(RUNTIME_CLASS(GameScene))) {
							if (!BattleMgrObj.GetBattle()) { // ����������״̬
								((GameScene*)gameScene)->ShowRelieve(true);
							}
						}
					}
					
					bool bWaitRelive = player.IsInState(USERSTATE_BF_WAIT_RELIVE);
					if (!bWaitRelive) 
						BattleFieldRelive::Hide();
					else if (!BattleMgrObj.GetBattle())
					{
						BattleFieldRelive::Show();
					}
					
					
					// ��Ч
					int effectAmount = data->ReadByte();
					std::vector<int> vEffect;
					for (int i = 0; i < effectAmount; i++) 
					{
						vEffect.push_back(data->ReadInt());
					}
					player.SetServerEffect(vEffect);
					
					break;
				}
				case USERATTRIB_CAMP: {
					player.SetCamp(CAMP_TYPE(dwAttributeData));
					break;
				}
				case USERATTRIB_EXTRA_PHY_POINT:{
					player.m_caclData.extra_phyPoint = (dwAttributeData);
					break;
				}
				case USERATTRIB_EXTRA_DEX_POINT:{
					player.m_caclData.extra_dexPoint = (dwAttributeData);
					break;
				}
				case USERATTRIB_EXTRA_MAG_POINT:{
					player.m_caclData.extra_magPoint = (dwAttributeData);
					break;
				}
				case USERATTRIB_EXTRA_DEF_POINT:{
					player.m_caclData.extra_defPoint = (dwAttributeData);
					break;
				}
				case USERATTRIB_PHY_ATK:{
					player.m_caclData.phy_atk = (dwAttributeData);
					break;
				}
				case USERATTRIB_PHY_DEF:{
					player.m_caclData.phy_def = (dwAttributeData);
					break;
				}
				case USERATTRIB_MAGIC_ATK:{
					player.m_caclData.mag_atk = (dwAttributeData);
					break;
				}
				case USERATTRIB_MAGIC_DEF:{
					player.m_caclData.mag_def = (dwAttributeData);
					break;
				}
				case USERATTRIB_ATK_SPEED:{
					player.m_caclData.atk_speed = (dwAttributeData);
					break;
				}
				case USERATTRIB_HITRATE:{
					player.m_caclData.hitrate = (dwAttributeData);
					break;
				}
				case USERATTRIB_DODGE:{
					player.m_caclData.dodge = (dwAttributeData);
					break;
				}
				case USERATTRIB_HARDHIT:{
					player.m_caclData.hardhit = (dwAttributeData);
					break;
				}
				case USERATTRIB_SAVED2:{
					player.jifeng = (dwAttributeData);
					break;
				}
				case USER_ATTRIB_ACTIVITY_POINT:{
					player.activityValue = (dwAttributeData);
					break;
				}
				case USER_ATTRIB_MAX_ACTIVITY:{
					player.activityValueMax = (dwAttributeData);
					break;
				}
				case USERATTRIB_MAX_SKILL_SLOT:{
					player.m_nMaxSlot = (dwAttributeData);
					break;
				}
				case USERATTRIB_VIP_RANK:{
					player.m_nVipLev	= dwAttributeData;
					break;
				}
				case USERATTRIB_VIP_EXP:{
					
					break;
				}
				case USERATTRIB_PEERAGE:{
					player.m_nPeerage	= dwAttributeData;
					updateTaskPrData(dwAttributeData, true);
					break;
				}
			}
		}
		
		GameUIAttrib *attr = GameUIAttrib::GetInstance();
		if (attr)
		{
			attr->UpdateGameUIAttrib();
		}
	}
	
	void NDMapMgr::processChangeRoom(NDTransData* data, int len)
	{
		// �½���ɫ�����ͼʱ��Ҫ��������id
		/*
		 if (bFirstCreate)
		 {
		 bFirstCreate = false;
		 
		 NDTransData data(_MSG_RECORD_CHANNEL);
		 data.WriteUnicodeString(pszChannelID);
		 NDDataTransThread::DefaultThread()->GetSocket()->Send(&data);
		 }
		 */
		m_nCurrentMonsterRound=0;
		if (bVerifyVersion)
		{
			bVerifyVersion = false;
			
			// �汾��֤
			NDBeforeGameMgrObj.VerifyVesion();
		}
		
		DealBugReport();
		
		BattleMgrObj.quitBattle(false);
		
		data->ReadShort(); // û���õ�
		data->ReadInt(); // û���õ�
		int mapId =data->ReadInt();//ʵ��MAP ID
		
		int idMapDoc=data->ReadInt(); // MAP��ԴID
		int dwPortalX = data->ReadShort();
		int dwPortalY = data->ReadShort();
		data->ReadShort(); // û���õ�
		data->ReadShort(); // û���õ�
		data->ReadShort(); // û���õ�
		data->ReadShort(); // û���õ�
		this->mapType = data->ReadInt();
		this->mapName = data->ReadUnicodeString();
		
		NDPlayer& player = NDPlayer::defaultHero();
		
		if (player.IsInState(USERSTATE_DEAD)) {
			NDUISynLayer::Close(SYN_RELIEVE);
		}
		
		NDUISynLayer::Close(SYN_CREATE_ROLE);
		
		m_vecTeamList.clear();
		if (player.isTeamLeader()) 
		{
			s_team_info info;
			info.team[0] = player.m_id;
			m_vecTeamList.push_back(info);
		}
		else if (player.isTeamMember())
		{
			s_team_info info;
			info.team[0] = player.teamId;
			info.team[1] = player.m_id;
			m_vecTeamList.push_back(info);
		}
		
		NDMapMgrObj.ClearManualRole();
		this->m_mapID=mapId;
		if (player.idCurMap == idMapDoc)
		{ // ����ͼ����
			while (NDDirector::DefaultDirector()->PopScene());
			
			player.idCurMap = idMapDoc;
			NDMapLayer *layer = NDMapMgrObj.getMapLayerOfScene( NDDirector::DefaultDirector()->GetRunningScene());
			if (!layer) 
			{
				return;
			}
			
			player.SetPositionEx(ccp(dwPortalX*MAP_UNITSIZE+DISPLAY_POS_X_OFFSET, dwPortalY*MAP_UNITSIZE+DISPLAY_POS_Y_OFFSET));
			player.SetServerPositon(dwPortalX, dwPortalY);
			player.stopMoving();
			
			layer->SetScreenCenter(ccp(dwPortalX*MAP_UNITSIZE+DISPLAY_POS_X_OFFSET, dwPortalY*MAP_UNITSIZE+DISPLAY_POS_Y_OFFSET));
			
			player.SetAction(false);
			
			CloseProgressBar;
			
			return;
		}
		
		player.idCurMap = idMapDoc;
		
		
		ShowPetInfo petInfoRerserve;
		player.GetShowPetInfo(petInfoRerserve);
		
		player.ResetShowPet();
		
		if (player.GetParent() != NULL) 
		{
			//NDBattlePet *pet = NDPlayer::defaultHero().battlepet;
			NDRidePet	*ridepet = NDPlayer::defaultHero().ridepet;
			//			if (pet && pet->GetParent()) 
			//			{
			//				pet->RemoveFromParent(false);
			//			}
			if (ridepet && ridepet->GetParent()) 
			{
				ridepet->RemoveFromParent(false);
			}
			player.RemoveFromParent(false);
		}
		
		while (NDDirector::DefaultDirector()->PopScene());
		
		NDMapMgrObj.ClearNpc();
		NDMapMgrObj.ClearMonster();
		NDMapMgrObj.ClearGP();
		NDMapMgrObj.loadSceneByMapDocID(idMapDoc);
		
		NDMapLayer *layer = NDMapMgrObj.getMapLayerOfScene( NDDirector::DefaultDirector()->GetRunningScene());
		int theId=GetMotherMapID();
		int title_id=ScriptDBObj.GetN("map", theId, DB_MAP_TITLE);
		layer->ShowTitle(title_id/100-1,title_id%100-1);
		if (!layer) 
		{
			ScriptMgrObj.DebugOutPut("!layer...");
			return;
		}

		//NDLog("%d", layer->GetChildren().size()); 
		NDMapMgrObj.LoadSceneMonster();
#ifndef DEBUG
		NDMapMgrObj.LoadMapMusic();
#endif
		layer->AddChild(&player);
		
		player.SetPositionEx(ccp(dwPortalX*MAP_UNITSIZE+DISPLAY_POS_X_OFFSET, dwPortalY*MAP_UNITSIZE+DISPLAY_POS_Y_OFFSET));
		player.SetServerPositon(dwPortalX, dwPortalY);
		
		player.SetShowPet(petInfoRerserve);
		
		player.stopMoving();
		
		//		SimpleAudioEngine *audioEngine=[SimpleAudioEngine sharedEngine];
		//		NSString *effectFile = [NSString stringWithUTF8String:NDPath::GetSoundPath().append("press.wav").c_str()];
		//		[audioEngine preloadEffect:effectFile];
		//		NSString *soundFile = [NSString stringWithUTF8String:NDPath::GetSoundPath().append("jay.mp3").c_str()];
		//		[audioEngine playBackgroundMusic:soundFile];
		
		/*
		 NDBattlePet *pet = NDPlayer::defaultHero().battlepet;
		 if (pet && !pet->GetParent()) 
		 {
		 pet->stopMoving();
		 pet->m_faceRight = !NDPlayer::defaultHero().m_faceRight;
		 //pet->SetPosition(ccp(dwPortalX*16+8, dwPortalY*16+8));
		 pet->SetPosition(NDPlayer::defaultHero().GetPosition());
		 pet->SetAction(false);
		 //pet->SetCurrentAnimation(MONSTER_STAND, !NDPlayer::defaultHero().m_faceRight); 
		 layer->AddChild(pet);
		 }
		 */
		
		NDRidePet *ridepet = player.ridepet;
		if (ridepet) 
		{
			ridepet->stopMoving();
			ridepet->SetPositionEx(ccp(dwPortalX*MAP_UNITSIZE+DISPLAY_POS_X_OFFSET, dwPortalY*MAP_UNITSIZE+DISPLAY_POS_Y_OFFSET));
			//ridepet->SetCurrentAnimation(RIDEPET_STAND, NDPlayer::defaultHero().m_faceRight);
			//NDPlayer::defaultHero().SetCurrentAnimation(MANUELROLE_RIDE_PET_STAND, NDPlayer::defaultHero().m_faceRight);
			//NDPlayer::defaultHero().SetAction(false);
			//layer->AddChild(ridepet);
		}
		
		
		layer->SetScreenCenter(ccp(dwPortalX*MAP_UNITSIZE+DISPLAY_POS_X_OFFSET, dwPortalY*MAP_UNITSIZE+DISPLAY_POS_Y_OFFSET));
		
		player.SetAction(false);
		player.SetLoadMapComplete();
		
		//������ʱ����Ʒ����
		ItemMgrObj.SortBag();
		
		ScriptGlobalEvent::OnEvent(GE_GENERATE_GAMESCENE);
		
		CloseProgressBar;
		
		if(theId/100000000>0){
			ScriptMgrObj.excuteLuaFunc("showDynMapUI", "",0);
		}else{
			ScriptMgrObj.excuteLuaFunc("showCityMapUI", "",0);
		}
		
#ifdef VIEW_PERFORMACE
		PerformanceEnable;
#endif 
	}
	
	void NDMapMgr::processPlayer(NDTransData* data, int len)
	{
		if (!data || len == 0) return ;
		
		int nUserID = 0; (*data) >> nUserID; // �û�id 4���ֽ�
		int nLife = 0; (*data) >> nLife; // ����ֵ4���ֽ�
		int nMaxLife = 0; (*data) >> nMaxLife; // ����ֵ4���ֽ�
		int nMana = 0; (*data) >> nMana; // ħ��ֵ
		int nMoney = 0; (*data) >> nMoney; // ���� 4���ֽ�
		int dwLookFace = 0; (*data) >> dwLookFace; // ���������ʱ����6����ۿɹ�ѡ����� ���� 4���ֽ�
		unsigned short usRecordX = 0; (*data) >> usRecordX; // ��¼����ϢX
		unsigned short usRecordY = 0; (*data) >> usRecordY; // ��¼����ϢY
		unsigned char btDir = 0; (*data) >> btDir; // ����沿���������������� 1���ֽ�
		unsigned char btProfession = 0; (*data) >> btProfession; // ��ҵ�ְҵ 1���ֽ�
		unsigned char btLevel = 0; (*data) >> btLevel; // �û��ȼ�
		int dwState = 0; (*data) >> dwState; // ״̬λ
		// System.out.println(" ״̬ " + dwState);
		int synRank = 0; (*data) >> synRank; // ���ɵȼ�
		int dwArmorType = 0; (*data) >> dwArmorType; // ����id 4���ֽ�
		int dwPkPoint = 0; (*data) >> dwPkPoint;// pkֵ
		unsigned char btCamp = 0;
		(*data) >> btCamp; // ��Ӫ
		std::string name = "";
		std::string rank = "";
		std::string synName = ""; // ��������
		
		unsigned char n = 0; (*data) >> n; // TQMB ���� 1�ֽ�
		for (int i = 0; i < n; i++) 
		{
			std::string str = data->ReadUnicodeString();
			if (i == 0) 
			{
				name = str;
			} else if (i == 1 && str.length() > 2) 
			{
				rank = str;
			} else if (i == 2) 
			{
				synName = str;
			}
		}
		
		if (synName.empty()) {
			synRank = SYNRANK_NONE;
		}
		
		NDPlayer& player = NDPlayer::defaultHero();
		if(nUserID == player.m_id)
		{
			if (dwState & USERSTATE_SHAPESHIFT) {
				player.updateTransform(data->ReadInt());
			} else {
				player.updateTransform(0);
			}
			
			// ��Ч
			int effectAmount = data->ReadByte();
			std::vector<int> vEffect;
			for (int i = 0; i < effectAmount; i++) 
			{
				vEffect.push_back(data->ReadInt());
			}
			
			player.SetPosition(ccp(usRecordX*MAP_UNITSIZE+DISPLAY_POS_X_OFFSET, usRecordY*MAP_UNITSIZE+DISPLAY_POS_Y_OFFSET));
			player.SetState(dwState);
			player.SetServerPositon(usRecordX, usRecordY);
			player.SetAction(false);
			return;
		}
		
		NDManualRole *role = NULL;
		bool bAdd = true;
		role = NDMapMgrObj.GetManualRole(nUserID);
		if ( !role ) 
		{
			role = new NDManualRole;
			role->m_id = nUserID;
			role->Initialization(dwLookFace, true);
			bAdd = false;
		}
		
		if (dwState & USERSTATE_SHAPESHIFT) {
			role->updateTransform(data->ReadInt());
		} else {
			role->updateTransform(0);
		}
		
		// ��Ч
		int effectAmount = data->ReadByte();
		std::vector<int> vEffect;
		for (int i = 0; i < effectAmount; i++) 
		{
			vEffect.push_back(data->ReadInt());
		}
		
		
		role->life = nLife;
		role->maxLife = nMaxLife;
		role->mana = nMana;
		role->money = nMoney;
		role->dwLookFace = dwLookFace;
		role->iProfesstion = btProfession;
		role->level = btLevel;
		role->SetState(dwState);
		role->setSynRank(synRank);
		role->pkPoint = dwPkPoint;
		role->SetCamp((CAMP_TYPE)btCamp);
		role->m_name = name;
		role->rank = rank;
		role->synName = synName;
		role->teamId = dwArmorType;
		role->SetPosition(ccp(usRecordX*MAP_UNITSIZE+DISPLAY_POS_X_OFFSET, usRecordY*MAP_UNITSIZE+DISPLAY_POS_Y_OFFSET));
		role->SetSpriteDir(btDir);
		role->SetServerPositon(usRecordX, usRecordY);
		
		if (role->isTeamMember()) 
		{
			updateTeamListAddPlayer(*role);
		}
		
		if (!bAdd) 
		{
			NDMapMgrObj.AddManualRole(nUserID, role);
		}
		
		role->SetAction(false);
		role->SetServerEffect(vEffect);
		
		// �������
		//PetInfo* petInfo = PetMgrObj.GetMainPet(role->m_id);
		//		role->SetShowPet(petInfo ? petInfo->data.int_PET_ID : 0);
		
		// int dwTransFormLookFace = 0; (*data) >> dwLookFace; // ���γɹ���
		
		// msg end
		
		// T.DEBUG("******* name = " + name);
		// T.DEBUG("******* synName = " + synName);
		// T.DEBUG("******* synRank = " + synRank);
		//
		// T.writeLog("dwLookFace " + dwLookFace);
		// T.writeLog("����ֵ " + nLife);
		// T.writeLog("�������ֵ " + nMaxLife);
		
		//int sex = ManualRole.getSexByLookface(dwLookFace); // �Ա�
		//int skinColor = ManualRole.getSkinColorByLookface(dwLookFace);// ��ɫ
		//int hairColor = ManualRole.getHairColorByLookface(dwLookFace);// ��ɫ
		//int hair = ManualRole.getHairByLookface(dwLookFace);// ͷ��
		
		// T.writeLog(" �����Ա�(1-���ԣ�2-Ů��): " + sex);
		// T.writeLog(" ��ɫ: " + skinColor);
		// T.writeLog(" ��ɫ: " + hairColor);
		// T.writeLog(" hair: " + hair);
		//
		// T.writeLog("  ������� name:" + name + " id:" + id + " col:" + usRecordX
		// + " row:" + usRecordY + " dir:" + btDir);
		/*
		 if (id != role.getId()) {
		 
		 // ����ȫ
		 ManualRole player = Scene.getRole(id);
		 
		 if (player == null) {
		 player = new ManualRole();
		 player.setSex(sex);
		 player.playerInit();
		 }
		 
		 player.setVisible(true);
		 player.setPosition(usRecordX, usRecordY);
		 
		 // player.setMaxMana(maxMana)
		 player.setMaxLife(nMaxLife);
		 player.setId(id);
		 player.setLife(nLife);
		 player.setMana(nMana);
		 player.setMoney(nMoney);
		 player.setDirect(btDir);
		 player.setProfession(btProfession);
		 player.setLevel(btLevel);
		 player.setName(name);
		 player.setRank(rank);
		 
		 player.setSex(sex);
		 player.setHairColor(hairColor - 1); // ��ɫ
		 player.setHair(hair); // ����
		 player.setSkinColor(skinColor - 1); // ��ɫ
		 
		 player.teamId = dwArmorType;
		 if (DepolyCfg.debug) {
		 System.out.println(" team id :" + dwArmorType);
		 }
		 player.setPkPoint(dwPkPoint); // pkֵ
		 player.setCamp(btCamp); // ��Ӫ 1,2
		 player.setState(dwState);
		 if (synName != null) {
		 player.setSynName(synName);
		 player.setSynRank((byte) synRank);
		 } else {
		 player.setSynName(null);
		 player.setSynRank(SyndicateElement.RANK_NONE);
		 }
		 scene.addPlayer(player);
		 // scene.addPlayerToList(player);
		 
		 if ((dwState & ManualRole.USERSTATE_TRANSFORM) > 0) {
		 player.updateTransform(data.readInt());
		 }
		 
		 } else {
		 role.setPosition(usRecordX, usRecordY);
		 role.setState(dwState);
		 }
		 */
	}
	
	void NDMapMgr::processPlayerExt(NDTransData* data, int len)
	{
		if (!data || len == 0) return ;
		
		// �����û�״̬
		int idUser = 0; (*data) >> idUser;
		
		int dwStatus = 0; (*data) >> dwStatus;
		
		NDManualRole *role = NULL;
		role = NDMapMgrObj.GetManualRole(idUser);
		if ( !role ) 
		{
			return;
		}
		role->SetState(dwStatus);
		
		//ж������װ��
		role->unpakcAllEquip();
		
		role->ResetShowPet();
		
		// �����˵�װ����Ϣ
		unsigned char btAmount = 0; (*data) >> btAmount; //һ�ֽ�
		
		for (int i = 0; i < btAmount; i++) 
		{
			int equipTypeId = 0; (*data) >> equipTypeId; //����װ��id,�ж������ĸ���λ
			NDItemType* item = ItemMgrObj.QueryItemType(equipTypeId);
			
			if (!item) 
			{
				continue;
			}
			
			role->m_vEquipsID.push_back(equipTypeId);
			int nID = item->m_data.m_lookface;
			int quality = equipTypeId % 10;
			
			if (nID == 0) 
			{
				continue;
			}
			int aniId = 0;
			if (nID > 100000) 
			{
				aniId = (nID % 100000) / 10;
			}
			if (aniId >= 1900 && aniId < 2000 || nID >= 19000 && nID < 20000) 
			{// ս��
				//NDBattlePet*& pet = role->battlepet;
				//				pet = new NDBattlePet();
				//				pet->Initialization(nID);
				//				pet->quality = quality;
				//				pet->m_faceRight = !role->m_faceRight;
				//				pet->SetPosition(role->GetPosition());
				//				pet->SetCurrentAnimation(MONSTER_STAND, !role->m_faceRight);
				//				pet->m_id = equipTypeId;
				//				pet->SetOwnerID(idUser);
				//				NDScene *scene = NDDirector::DefaultDirector()->GetScene(RUNTIME_CLASS(GameScene));
				//				if (scene && getMapLayerOfScene(scene)) 
				//				{
				//					getMapLayerOfScene(scene)->AddChild(pet);
				//				}
				//				else 
				//				{
				//					SAFE_DELETE_NODE(role->battlepet);
				//				}
				ShowPetInfo showPetInfo(
										equipTypeId,
										nID,
										quality);
				role->SetShowPet(showPetInfo);
				// �������
				//PetInfo* petInfo = PetMgrObj.GetMainPet(role->m_id);
				//				role->SetShowPet(petInfo ? petInfo->data.int_PET_ID : 0);
				
			} else 
			{
				role->SetEquipment(nID, quality);
			}
			
			//Item item = new Item(equipTypeId);
			// int lookface = new Item(equipTypeId).getLookface();
			
			// scene.updatePlayerEqpuipment(idUser, item);
			//player.setEquipmentLookFace(item);
			
			//
			//int lookface = 0;
			//role->SetEquipment(int equipmentId, int quality);
			
		}
		
		if (role->IsInState(USERSTATE_SHAPESHIFT)) {
			role->updateTransform(data->ReadInt());
		} else {
			role->updateTransform(0);
		}
		
		if (role->GetParent() != NULL && !role->GetParent()->IsKindOfClass(RUNTIME_CLASS(Battle))) {
			role->SetAction(role->isMoving(), true);
		}
		
		// msg end
	}
	
	void NDMapMgr::processWalk(NDTransData* data, int len)
	{
		if (!data || len == 0) return ;
		
		int nID = 0; (*data) >> nID;
		unsigned char dir = 0; (*data) >> dir;
		
		if ( nID != NDPlayer::defaultHero().m_id) 
		{
			NDManualRole *role = NULL;
			role = NDMapMgrObj.GetManualRole(nID);
			if ( role && (!role->isTeamMember() || role->isTeamLeader())) 
			{
				role->AddWalkDir(dir);
				role->SetServerDir(dir);
				if (role->isTeamLeader()) 
				{
					role->teamSetServerDir(dir);
				}
			}
			
			return;
		}
	}
	
	void NDMapMgr::processKickBack(NDTransData* data, int len)
	{
		if (!data || len == 0) return ;
		
		unsigned short usRecordX = 0; (*data) >> usRecordX;
		unsigned short usRecordY = 0; (*data) >> usRecordY;
		
		NDPlayer& player = NDPlayer::defaultHero();
	
		player.SetPosition(ccp(usRecordX*MAP_UNITSIZE+DISPLAY_POS_X_OFFSET, usRecordY*MAP_UNITSIZE+DISPLAY_POS_Y_OFFSET));
		player.SetServerPositon(usRecordX, usRecordY);
		if (player.isTeamLeader()) 
		{
			player.teamSetServerPosition(usRecordX, usRecordY);
		}
		
		Battle* battle = BattleMgrObj.GetBattle();
		NDMapLayer *layer = NDMapMgrObj.getMapLayerOfScene( NDDirector::DefaultDirector()->GetRunningScene());
		player.stopMoving();
		if (battle) {
			//NDLog("x,y=%d,%d",usRecordX,us);
			battle->setSceneCetner(usRecordX*32+DISPLAY_POS_X_OFFSET, usRecordY*32+DISPLAY_POS_X_OFFSET);
		}
		else{

			if (!layer) 
			{
				return;
			}
			layer->SetScreenCenter(ccp(usRecordX*32+DISPLAY_POS_X_OFFSET, usRecordY*32+DISPLAY_POS_Y_OFFSET));
		}

	}
	
	void NDMapMgr::processDisappear(NDTransData* data, int len)
	{
		if (!data || len == 0) return ;
		
		int idUser = 0; (*data) >> idUser;
		
		NDManualRole *role = GetManualRole(idUser);
		if (role)//&& role->isTeamMember()) 
		{
			//updateTeamListDelPlayer(*role);
			role->bClear = true;
			
			BattleMgr& mgr = BattleMgrObj;
			
			if (mgr.GetBattle()
				&& mgr.GetBattle()->OnRoleDisapper(role->m_id)) 
			{
				role->RemoveFromParent(false);
			}
			
			NDPlayer::defaultHero().UpdateFocusPlayer();
		}
		
		//NDMapMgrObj.DelManualRole(idUser);
	}
	
	void NDMapMgr::processNpcInfoList(NDTransData* data, int len)
	{
		const int LIST_ACTION_END = 1;
		//NDLog("����NPC��Ϣ-----------");
		unsigned char btAction = 0; (*data) >> btAction;
		unsigned char btCount = 0; (*data) >> btCount;
		for (int i = 0; i < btCount; i++)
		{
			int id = 0; (*data) >> id; // 4���ֽ� npc id
			unsigned char uitype = 0; (*data) >> uitype; // ���ֶ����ڹ���Ѱ·��npc�б�
			int usLook = 0; (*data) >> usLook; // 4���ֽ�
			unsigned char  btSort = 0; (*data) >> btSort;
			unsigned short usCellX = 0; (*data) >> usCellX; // 2���ֽ�
			unsigned short usCellY = 0; (*data) >> usCellY; // 2���ֽ�
			unsigned char btState = 0;(*data) >>btState; // 1���ֽڱ�״̬
			unsigned char btCamp = 0; (*data) >>btCamp;
			std::string name = data->ReadUnicodeString(); 
			//NDLog("%@", [NSString stringWithUTF8String:name.c_str()]);
			
			//Npc npc = new Npc(id, name, usLook, usCellX, usCellY);
			//npc.mapId = mapId;
			//npc.setState(btState);
			//npc.setCamp(btCamp);
			//npc.dataStr = data.readString2();
			std::string dataStr = data->ReadUnicodeString(); 
			
			
			std::string talkStr = data->ReadUnicodeString(); 
			//NDLog("%@", [NSString stringWithUTF8String:dataStr.c_str()]);
			//dynamicNpcArray.addElement(npc);
			//if (name == "��ʯ��") {
			//				continue;
			//			}
			
			NDNpc *npc = new NDNpc;
			npc->m_id = id;
			npc->col	= usCellX;
			npc->row	= usCellY;
			npc->look	= usLook;
			npc->SetCamp(CAMP_TYPE(btCamp));
			//npc->SetNpcState(NPC_STATE(btState));
			if (uitype == 6) 
			{
				npc->m_name = "";
			}
			else 
			{
				npc->m_name = name;
			}			
			npc->SetPosition(ccp(usCellX*MAP_UNITSIZE+DISPLAY_POS_X_OFFSET, usCellY*MAP_UNITSIZE+DISPLAY_POS_Y_OFFSET));
			npc->dataStr = dataStr;
			npc->talkStr = talkStr;
			npc->SetType(uitype);
			npc->Initialization(usLook);
			if (btSort != 0) 
			{
				npc->SetActionOnRing(false);
				npc->SetDirectOnTalk(false);
			}
			npc->initUnpassPoint();
			NDMapMgrObj.m_vNpc.push_back(npc);
		}
		
		if (btAction == LIST_ACTION_END)
		{ // �շ�����
			NDMapMgrObj.AddAllNpcToMap();
			// Array staticNpcArray = Npc.getNpcsByMapId(mapId);
			// scene.addNpc(staticNpcArray); // ���Ӿ�̬npc
			
			//scene.addNpc(dynamicNpcArray); // ���Ӷ�̬npc
			//scene.initialAutoFindPath();
			// staticNpcArray.removeAllElements();
			//dynamicNpcArray.removeAllElements();
			// if (T.isTaskHasReceived) { // ��ֹû���յ�Task_Done ��Ϣ��ˢ�µ�ǰ��ͼ
			// Npc.refreshNpcStateInMap();
			// }
		}
		
	}
	
	void NDMapMgr::processNpcStatus(NDTransData* data, int len)
	{
		unsigned char count = 0; (*data) >> count;
		
		for (int i = 0; i < count; i++)
		{
			int npcId = 0; npcId = data->ReadInt();
			unsigned char state = 0; (*data) >> state;
			NDNpc *npc = NULL;
			for_vec(m_vNpc, vec_npc_it)
			{
				if ((*it) && (*it)->m_id == npcId)
				{
					npc = *it;
				}
			}
			if (npc)
			{
				npc->SetNpcState(NPC_STATE(state));
			}
		}
		
	}
	
	void NDMapMgr::processMonsterInfo(NDTransData* data, int len)
	{
		unsigned char btAction = 0; (*data) >> btAction;
		unsigned char count = 0;  (*data) >> count;
		switch(btAction){
			case MONSTER_INFO_ACT_INFO:
				for (int i = 0; i < count; i++)
				{
					
					int idType = 0; (*data) >> idType;
					int lookFace = 0; (*data) >> lookFace;
					unsigned char lev = 0; (*data) >> lev;
					unsigned char  btAiType = 0; (*data) >> btAiType;
					unsigned char  btCamp = 0; (*data) >> btCamp;
					unsigned char  btAtkArea = 0; (*data) >> btAtkArea;
					std::string name = data->ReadUnicodeString(); //����ַ���������Ҫ���޸�
					//NDLog("%@", [NSString stringWithUTF8String:name.c_str()]);
					
					NDLog("�յ���������[%d]��Ϣ", idType);
					
					monster_type_info info;
					info.idType = idType;
					info.lookFace = lookFace;
					info.lev = lev;
					info.btAiType = btAiType;
					info.btCamp = btCamp;
					info.btAtkArea = btAtkArea;
					info.name = name;
					//if (lookFace == 19070 || lookFace == 206) continue;
					
					NDMapMgrObj.m_mapMonsterInfo.insert(monster_info_pair(idType, info));
				}
				break;
			case MONSTER_INFO_ACT_POS:
				for (int i = 0; i < count; i++)
				{
					int idMonster  = 0; (*data) >> idMonster;
					int idType = 0; (*data) >> idType;
					unsigned short col = 0; (*data) >> col;
					unsigned short row = 0; (*data) >> row;
					unsigned char  btState = 0; (*data) >> btState;
					
					NDLayer *layer = NDMapMgrObj.getMapLayerOfScene( NDDirector::DefaultDirector()->GetRunningScene());
					if (!layer)
					{
						return;
					}
					//NDMapData *mapdata = ((NDMapLayer*)layer)->GetMapData();
					//NDTile* tile = [mapdata getTileAtRow:row column:col];
					//				CustomCCTexture2D* tile = [mapdata getTileAtRow:row column:col];
					//				if (!tile) 
					//				{
					//					NDLog("���������������");
					//					continue;
					//				}
					
					//if (idType / 1000000 == 6)
					//{ // �ɼ�
					//	GatherPoint *gp = NULL;
					//	if (idMonster > 0)
					//	{
					//		map_gather_point_it it = m_mapGP.find(idMonster);
					//		if (it != m_mapGP.end())
					//		{
					//			gp = it->second;
					//		}
					//	}
					//	
					//	if (gp == NULL) 
					//	{
					//		gp = new GatherPoint(idMonster, idType, col*MAP_UNITSIZE,
					//							 row *MAP_UNITSIZE, idMonster > 0, "");
					//		m_mapGP.insert(map_gather_point_pair(idMonster, gp));
					//	}
					//	
					//	gp->setState(btState);
					//	
					//	continue;
					//}
					
					monster_type_info info;
					if ( !NDMapMgrObj.GetMonsterInfo(info, idType) ) continue;
					//NDLog("���������(%d,%d)",row, col );
					NDMonster *monster = new NDMonster;
					monster->m_id = idMonster;
					monster->SetPosition(ccp(col*MAP_UNITSIZE, row*MAP_UNITSIZE));
					monster->Initialization(idType);
					monster->SetType(MONSTER_ELITE);
					if (idMonster > 0)
					{
						monster->setState(btState);
					}
					
					
					NDMapMgrObj.m_vMonster.push_back(monster);
					
				}
				NDMapMgrObj.AddAllMonsterToMap();
				break;
			case MONSTER_INFO_ACT_STATE:
				for (int i = 0; i < count; i++)
				{
					int idMonster = 0; (*data) >> idMonster;
					unsigned char btState = 0; (*data) >> btState;
					if (idMonster > 0)
					{
						/*
						 SceneElement se = GameScreen.getInstance().getScene()
						 .getGatherPoint(idMonster);
						 if (se == null) {
						 se = GameScreen.getInstance().getScene()
						 .getSceneMonster(idMonster);
						 }
						 if (se != null) {
						 se.setState(btState);
						 }
						 */
						//GatherPoint *gp = NULL;
						//if (idMonster > 0)
						//{
						//	map_gather_point_it it = m_mapGP.find(idMonster);
						//	if (it != m_mapGP.end())
						//	{
						//		gp = it->second;
						//	}
						//}
						//
						//if (gp == NULL) 
						{
							for_vec(m_vMonster, vec_monster_it)
							{
								if ((*it)->m_id == idMonster)
								{
									(*it)->setState(btState);
								}
							}
						}
						//else
						//{
						//	gp->setState(btState);
						//}
					}
				}
				break;
			case MONSTER_INFO_ACT_BOSS_POS:
				for (int i = 0; i < count; i++)
				{
					int idMonster  = 0; (*data) >> idMonster;
					int idType = 0; (*data) >> idType;
					unsigned short col = 0; (*data) >> col;
					unsigned short row = 0; (*data) >> row;
					unsigned char  btState = 0; (*data) >> btState;
					
					NDLayer *layer = NDMapMgrObj.getMapLayerOfScene( NDDirector::DefaultDirector()->GetRunningScene());
					if (!layer)
					{
						return;
					}
					//NDMapData *mapdata = ((NDMapLayer*)layer)->GetMapData();
					//NDTile* tile = [mapdata getTileAtRow:row column:col];
					//				CustomCCTexture2D* tile = [mapdata getTileAtRow:row column:col];
					//				if (!tile) 
					//				{
					//					NDLog("���������������");
					//					continue;
					//				}
					
					//if (idType / 1000000 == 6)
					//{ // �ɼ�
					//	GatherPoint *gp = NULL;
					//	if (idMonster > 0)
					//	{
					//		map_gather_point_it it = m_mapGP.find(idMonster);
					//		if (it != m_mapGP.end())
					//		{
					//			gp = it->second;
					//		}
					//	}
					//	
					//	if (gp == NULL) 
					//	{
					//		gp = new GatherPoint(idMonster, idType, col*MAP_UNITSIZE,
					//							 row *MAP_UNITSIZE, idMonster > 0, "");
					//		m_mapGP.insert(map_gather_point_pair(idMonster, gp));
					//	}
					//	
					//	gp->setState(btState);
					//	
					//	continue;
					//}
					
					monster_type_info info;
					//if ( !NDMapMgrObj.GetMonsterInfo(info, idType) ) continue;
					//NDLog("���������(%d,%d)",row, col );
					NDMonster *monster = new NDMonster;
					monster->m_id = idMonster;
					monster->SetPosition(ccp(col*MAP_UNITSIZE, row*MAP_UNITSIZE));
					monster->Initialization(idType);
					monster->SetType(MONSTER_BOSS);
					if (idMonster > 0)
					{
						monster->setState(btState);
					}
					
					
					NDMapMgrObj.m_vMonster.push_back(monster);
					
				}
				NDMapMgrObj.AddAllMonsterToMap();
				break;
			case MONSTER_INFO_ACT_BOSS_STATE:
				for (int i = 0; i < count; i++)
				{
					int idMonster = 0; (*data) >> idMonster;
					unsigned char btState = 0; (*data) >> btState;
					if (idMonster > 0)
					{
						/*
						 SceneElement se = GameScreen.getInstance().getScene()
						 .getGatherPoint(idMonster);
						 if (se == null) {
						 se = GameScreen.getInstance().getScene()
						 .getSceneMonster(idMonster);
						 }
						 if (se != null) {
						 se.setState(btState);
						 }
						 */
//						GatherPoint *gp = NULL;
//						if (idMonster > 0)
//						{
//							map_gather_point_it it = m_mapGP.find(idMonster);
//							if (it != m_mapGP.end())
//							{
//								gp = it->second;
//							}
//						}
//						
//						if (gp == NULL) 
//						{
							for_vec(m_vMonster, vec_monster_it)
							{
								if ((*it)->m_id == idMonster&&(*it)->GetType() == MONSTER_BOSS)
								{
									(*it)->setState(btState);
								}
							}
//						}
//						else
//						{
//							gp->setState(btState);
//						}
					}
				}
				break;
		}
	}
	
	void NDMapMgr::processChgPoint(NDTransData* data, int len)
	{
		unsigned char ucAnswer = 0; (*data) >> ucAnswer;
		
		if (ucAnswer == 1) 
		{
			// ������,��ԭ���޸��û�����
			NDPlayer::defaultHero().phyPoint -= NDPlayer::defaultHero().iTmpPhyPoint;
			NDPlayer::defaultHero().dexPoint -= NDPlayer::defaultHero().iTmpDexPoint;
			NDPlayer::defaultHero().magPoint -= NDPlayer::defaultHero().iTmpMagPoint;
			NDPlayer::defaultHero().defPoint -= NDPlayer::defaultHero().iTmpDefPoint;
			NDPlayer::defaultHero().restPoint = NDPlayer::defaultHero().iTmpRestPoint;
		}
		
		// ����ӵ����Ի���
		NDPlayer::defaultHero().iTmpPhyPoint = 0;
		NDPlayer::defaultHero().iTmpDexPoint = 0;
		NDPlayer::defaultHero().iTmpMagPoint = 0;
		NDPlayer::defaultHero().iTmpDefPoint = 0;
		NDPlayer::defaultHero().iTmpRestPoint = 0;
		
		// ����������Խ��洦�ڴ�״̬,����¸ý���
		GameUIAttrib *attr = GameUIAttrib::GetInstance();
		if (attr) 
		{
			attr->UpdateGameUIAttrib();
		}
	}
	
	void NDMapMgr::processPetInfo(NDTransData* data, int len)
	{
		NDPlayer& player = NDPlayer::defaultHero();
		
		NDManualRole *role = NULL;
		
		bool bUseNewScene = false;
		
		int action = 0; (*data) >> action;
		
		int idPet = 0; (*data) >> idPet;
		
		if (action == 2)
		{
			NDPlayer& player = NDPlayer::defaultHero();
			ShowPetInfo showPetInfo;
			player.GetShowPetInfo(showPetInfo);
			if (OBJID(idPet) == showPetInfo.idPet)
			{
				player.ResetShowPet();
			}
			
			PetMgrObj.DelPet(idPet);
			CUIPet* pUIPet = PlayerInfoScene::QueryPetScene();
			if (pUIPet) {
				pUIPet->UpdateUI(idPet);
			}
			return;
		}
		
		int ownerid = 0; (*data) >> ownerid;
		
		if (ownerid == player.m_id)
		{
			role = (NDManualRole*)&player;
		}
		else
		{
			role = GetManualRole(ownerid);
		}
		
		if (action != 1) 
		{
			return;
		}
		
		
		PetInfo* petInfo = PetMgrObj.GetPetWithCreate(ownerid, idPet);
		
		NDAsssert(petInfo != NULL);
		
		PetInfo::PetData *pet = &petInfo->data;
		
		pet->int_PET_ID = idPet;
		
		bool bSwithPlayerInfoScene = false;
		
		int attrNum = 0; (*data) >> attrNum;
		for (int i = 0; i < attrNum; i++) 
		{
			int type = 0; (*data) >> type;
			int value = 0;
			
			if (type != 100) {
				value = data->ReadInt();
			}
			
			switch (type) 
			{
				case 1:// PET_ATTR_LEVEL
					pet->int_PET_ATTR_LEVEL = value;
					break;
				case 2:// PET_ATTR_EXP
					pet->int_PET_ATTR_EXP = value;
					break;
				case 3:// PET_ATTR_LIFE
					pet->int_PET_ATTR_LIFE = value;
					break;
				case 4:// PET_ATTR_MAX_LIFE
					pet->int_PET_ATTR_MAX_LIFE = value;
					break;
				case 5:// PET_ATTR_MANA
					pet->int_PET_ATTR_MANA = value;
					break;
				case 6:// PET_ATTR_MAX_MANA6
					pet->int_PET_ATTR_MAX_MANA = value;
					break;
				case 7:// PET_ATTR_STR7
					pet->int_PET_ATTR_STR = value;
					break;
				case 8:// PET_ATTR_STA8
					pet->int_PET_ATTR_STA = value;
					break;
				case 9:// PET_ATTR_AGI9
					pet->int_PET_ATTR_AGI = value;
					break;
				case 10:// PET_ATTR_INI10
					pet->int_PET_ATTR_INI = value;
					break;
				case 11:// PET_ATTR_LEVEL_INIT11
					pet->int_PET_ATTR_LEVEL_INIT = value;
					break;
				case 12:// PET_ATTR_STR_INIT12
					pet->int_PET_ATTR_STR_INIT = value;
					break;
				case 13:// PET_ATTR_STA_INIT13
					pet->int_PET_ATTR_STA_INIT = value;
					break;
				case 14:// PET_ATTR_AGI_INIT14
					pet->int_PET_ATTR_AGI_INIT = value;
					break;
				case 15:// PET_ATTR_INI_INIT15
					pet->int_PET_ATTR_INI_INIT = value;
					break;
				case 16:// PET_ATTR_LOYAL16
					pet->int_PET_ATTR_LOYAL = value;
					break;
				case 17:// PET_ATTR_AGE17
					pet->int_PET_ATTR_AGE = value;
					break;
				case 18:// PET_ATTR_FREE_SP18
					pet->int_PET_ATTR_FREE_SP = value;
					break;
				case 19:// PET_ATTR_STR_RATE19
					pet->int_PET_PHY_ATK_RATE = value;
					break;
				case 20:// PET_ATTR_STA_RATE20
					pet->int_PET_PHY_DEF_RATE = value;
					break;
				case 21:// PET_ATTR_AGI_RATE21
					pet->int_PET_MAG_ATK_RATE = value;
					break;
				case 22:// PET_ATTR_INI_RATE22
					pet->int_PET_MAG_DEF_RATE = value;
					break;
				case 23:// PET_ATTR_HP_RATE23
					pet->int_PET_ATTR_HP_RATE = value;
					break;
				case 24:// PET_ATTR_MP_RATE24
					pet->int_PET_ATTR_MP_RATE = value;
					break;
				case 25:// PET_ATTR_LEVEUP_EXP25
					pet->int_PET_ATTR_LEVEUP_EXP = value;
					break;
				case 26:// PET_ATTR_PHY_ATK26
					pet->int_PET_ATTR_PHY_ATK = value;
					break;
				case 27:// PET_ATTR_PHY_DEF27
					pet->int_PET_ATTR_PHY_DEF = value;
					break;
				case 28:// PET_ATTR_MAG_ATK28
					pet->int_PET_ATTR_MAG_ATK = value;
					break;
				case 29:// PET_ATTR_MAG_DEF29
					pet->int_PET_ATTR_MAG_DEF = value;
					break;
				case 30:// PET_ATTR_HARD_HIT30
					pet->int_PET_ATTR_HARD_HIT = value;
					break;
				case 31:// PET_ATTR_DODGE31
					pet->int_PET_ATTR_DODGE = value;
					break;
				case 32:// PET_ATTR_ATK_SPEED32
					pet->int_PET_ATTR_ATK_SPEED = value;
					break;
				case 33:// PET_ATTR_TYPE33 ;����
					pet->int_PET_ATTR_TYPE = value;
					break;
				case 34:// ���
					pet->int_PET_ATTR_LOOKFACE = value;
					break;
				case 35:// PET_ATTR_SKILL_1 32
					pet->int_PET_MAX_SKILL_NUM = value;
					//PetSkillSceneUpdate();
					PetSkillIconLayer::OnUnLockSkill();
					bUseNewScene = true;
					break;
				case 36:// PET_ATTR_SKILL_2 33
					pet->int_ATTR_FREE_POINT = value;
					break;
				case 37:// �ٶ�����
					pet->int_PET_SPEED_RATE = value;
					break;
				case 38:// �﹥��������
					pet->int_PET_PHY_ATK_RATE_MAX = value;
					break;
				case 39:// �����������
					pet->int_PET_PHY_DEF_RATE_MAX = value;
					break;
				case 40:// ������������
					pet->int_PET_MAG_ATK_RATE_MAX = value;
					break;
				case 41:// ������������
					pet->int_PET_MAG_DEF_RATE_MAX = value;
					break;
				case 42:// �����ӳ���������
					pet->int_PET_ATTR_HP_RATE_MAX = value;
					break;
				case 43:// ħ���ӳ���������
					pet->int_PET_ATTR_MP_RATE_MAX = value;
					break;
				case 44:// �ٶ���������
					pet->int_PET_SPEED_RATE_MAX = value;
					break;
				case 45:// �ɳ���
					pet->int_PET_GROW_RATE = value;
					break;
				case 46:// �ɳ�������
					pet->int_PET_GROW_RATE_MAX = value;
					break;
				case 47:// ����
					pet->int_PET_HIT = value;					
					break;
				case 48://��״̬
					pet->bindStatus = value;
					break;
				case 49: //����λ��
				{
					if (pet->int_PET_ATTR_POSITION != value)
					{
						bSwithPlayerInfoScene = true;
					}
					
					pet->int_PET_ATTR_POSITION = value;
				}	
					break;
				case 100:// ����
				{
					std::string petName = data->ReadUnicodeString();
					petInfo->str_PET_ATTR_NAME = petName;
					break;
				}
				default:
					break;
			}
		}
		
		if (pet->int_PET_ATTR_POSITION & PET_POSITION_SHOW)
		{
			if (role)
			{
				ShowPetInfo showPetInfo(
										pet->int_PET_ID,
										pet->int_PET_ATTR_LOOKFACE,
										petInfo->GetQuality());
				role->SetShowPet(showPetInfo);
			}
		}
		else if (role)
		{
			ShowPetInfo showPetInfo;
			role->GetShowPetInfo(showPetInfo);
			if (OBJID(pet->int_PET_ID) == showPetInfo.idPet)
			{
				role->ResetShowPet();
			}
		}
		
		if (bSwithPlayerInfoScene && role->m_id == ownerid)
		{
			NDScene* scene = NDDirector::DefaultDirector()->GetRunningScene();
			
			if (scene && scene->IsKindOfClass(RUNTIME_CLASS(PlayerInfoScene)))
			{
				PlayerInfoScene *playerInfoScene = (PlayerInfoScene*)scene;
				
				playerInfoScene->SetTabFocusOnIndex(3, true);
			}
		}
		
		CUIPet* pUIPet = PlayerInfoScene::QueryPetScene();
		if (pUIPet) {
			pUIPet->UpdateUI(pet->int_PET_ID);
		}
	}
	
	//void NDMapMgr::processLifeSkillSynthesize(NDTransData& data)
	//{
	//	int btAction = data.ReadByte();//2��ͨ�����ظ���3�䷽�����ظ�
	//	int idSkill = data.ReadInt();
	//	int btLev = data.ReadByte();
	//	int uExp = data.ReadInt(); // �ü��ܵ���������
	//	int nAddExp = data.ReadByte();	// �ü���������������
	//	int idProduct_1 = data.ReadInt();
	//	int num_product_1 = 1;
	//	int idProduct_2 = 0,num_product_2  = 1;
	//	if(btAction == 3){//3�䷽�����ظ�
	//		num_product_1 = data.ReadByte();
	//		idProduct_2 = data.ReadInt();
	//		num_product_2 = data.ReadByte();
	//	}
	//	LifeSkill *lifeSkill = getLifeSkill(idSkill);
	//	if(lifeSkill != NULL){
	//		lifeSkill->updateLearnLifeSkill(uExp,btLev,-1);
	//		NDScene *scene = NDDirector::DefaultDirector()->GetRunningScene();
	//		if (scene && scene->IsKindOfClass(RUNTIME_CLASS(LifeSkillScene))) 
	//		{
	//			((LifeSkillScene*)scene)->UpdateSkillData();
	//		}
	//	}
	//	std::stringstream strBuf; strBuf << (NDCommonCString("MakedTip"));
	//	if(idProduct_1 != 0){
	//		strBuf << (Item(idProduct_1).getItemName());
	//		strBuf << (" ");
	//		strBuf << (num_product_1);
	//		strBuf << NDCommonCString("ge") << "  ";
	//	}
	//	if(idProduct_2 != 0){
	//		strBuf << (Item(idProduct_2).getItemName());
	//		strBuf << (" ");
	//		strBuf << (num_product_2);
	//		strBuf << NDCommonCString("ge") << "  ";
	//	}
	//	strBuf << "," << NDCommonCString("SkillValueUpTip");
	//	strBuf << (nAddExp);
	//	
	//	CloseProgressBar;
	//	
	//	NDScene *scene = NDDirector::DefaultDirector()->GetRunningScene();
	//	if (scene && scene->IsKindOfClass(RUNTIME_CLASS(LifeSkillRandomScene))) 
	//	{
	//		((LifeSkillRandomScene*)scene)->reset();
	//	}
	//	
	//	Chat::DefaultChat()->AddMessage(ChatTypeSystem, strBuf.str().c_str());
	//	//if (GameScreen.getInstance() != null) {
	//	//			ChatRecord cr = new ChatRecord(5, "ϵͳ", strBuf.toString());
	//	//			cr.focus();
	//	//			GameScreen.getInstance().initNewChat(cr,true);
	//	//		}
	//	//if(SkillDialog.instance != null){
	//	//			SkillDialog.instance.initScrollContent();
	//	//		}
	//}
	//
	//void NDMapMgr::processLifeSkill(NDTransData& data)
	//{
	//	Byte btAction = data.ReadByte();
	//	Byte btSkillCount = data.ReadByte();
	//	for (Byte i = 0; i < btSkillCount; i++) {
	//		int idSkill = data.ReadInt();
	//		int uSkillExp = data.ReadInt();
	//		Byte uSkillGrade = data.ReadByte();
	//		int uExp_max = data.ReadInt();
	//		string sSkillName = data.ReadUnicodeString();
	//		LifeSkill* t_skill = this->getLifeSkill(idSkill);
	//		if(t_skill){
	//			t_skill->updateLearnLifeSkill(uSkillExp, uSkillGrade, uExp_max);
	//		}else{
	//			this->m_mapLearnedLifeSkill[idSkill] = LifeSkill(idSkill, uSkillExp,uExp_max, uSkillGrade,sSkillName);
	//		}
	//		
	//		if(btAction == 1){ // 0���·���1��ѧϰ
	//			//NDUIDialog* dialog = new NDUIDialog;
	//			//				dialog->Initialization();
	//			stringstream ss;
	//			ss << NDCommonCString("Common_LearnedTip") << uSkillGrade << NDCommonCString("Ji") << sSkillName;
	//			//dialog->Show(NDCommonCString("OperateSucess"), ss.str().c_str(), NULL, NULL);
	//			GlobalShowDlg(NDCommonCString("OperateSucess"), ss.str());
	//		}
	//	}
	//}
	//
	void NDMapMgr::processCollection(NDTransData& data)
	{
		CloseProgressBar;
		int itemtype = 0; data >> itemtype;
		Item *item = new Item(itemtype);
		stringstream ss; ss << NDCommonCString("GatheredTip") << " " << item->getItemName();
		showDialog(NDCommonCString("system"), ss.str().c_str());
		delete item;
	}
	
	void NDMapMgr::processGameQuit(NDTransData* data, int len)
	{
		BeatHeartMgrObj.Stop();
		CloseProgressBar;
		/*
		 GameQuitDialog *dlg = new GameQuitDialog;
		 dlg->Initialization();
		 dlg->Show("ͨѶ����", "�ǳ���Ǹ����������Ͽ����ӣ���ص��������µ�¼��", "�ص�����", NULL);	
		 */
		 
		//GameQuitDialog::DefaultShow(NDCommonCString("NetErr"), NDCommonCString("NetExceptErr"));
		
//		if (NDDataTransThread::DefaultThread()->GetThreadStatus() == ThreadStatusRunning) 
//		{
//			NDDataTransThread::DefaultThread()->Stop();
//		}
		::quitGame();
	}
	
	void NDMapMgr::processNPCInfo(NDTransData& data)
	{
		int iid = 0; data >> iid;
		unsigned char _unuse = 0, uctype = 0; data >> _unuse >> uctype;
		
		int usLook = 0; data >> usLook;
		data >> _unuse;
		unsigned short col = 0, row = 0;
		data >> col >> row;
		
		unsigned char btState = 0, btCamp = 0;
		data >> btState >> btCamp;
		
		std::string name = data.ReadUnicodeString();
		std::string dataStr = data.ReadUnicodeString();
		std::string talkStr = data.ReadUnicodeString(); 
		
		DelNpc(iid);

		
		NDNpc *npc = new NDNpc;
		npc->m_id = iid;
		npc->col	= col;
		npc->row	= row;
		npc->look	= usLook;
		npc->m_name = name;
		npc->SetPosition(ccp(col*MAP_UNITSIZE+DISPLAY_POS_X_OFFSET, row*MAP_UNITSIZE+DISPLAY_POS_Y_OFFSET));
		npc->SetCamp(CAMP_TYPE(btCamp));
		npc->SetNpcState(NPC_STATE(btState));
		npc->dataStr = dataStr;
		npc->talkStr = talkStr;
		npc->SetType(uctype);
		npc->Initialization(usLook);
		npc->initUnpassPoint();
		AddOneNPC(npc);
	}
	
	void NDMapMgr::processRehearse(NDTransData& data)
	{
		unsigned char btAction = 0; data >> btAction;
		int idTarget = 0; data >> idTarget;
		
		//NDManualRole* role = GetManualRole(idTarget);
		//RequsetInfo info;
		//
		//switch (btAction) {
		//	case REHEARSE_APPLY: 
		//	{
		//		if (role != NULL) 
		//		{
		//			info.set(idTarget, role->m_name, RequsetInfo::ACTION_BIWU);
		//			addRequst(info);
		//		}
		//		break;
		//	}
		//	case REHEARSE_REFUSE: 
		//	{
		//		if (role != NULL) 
		//		{
		//			std::stringstream ss; ss << role->m_name << NDCommonCString("RejectRefraseTip");
		//			Chat::DefaultChat()->AddMessage(ChatTypeSystem, ss.str().c_str());
		//			//if (GameScreen.getInstance() != null) {
		//			//						GameScreen.getInstance().initNewChat(new ChatRecord(5, NDCommonCString("system"), role.getName() + "�ܾ�����ı�������"));
		//			//					}
		//		}
		//		break;
		//	}
		//	case REHEARSE_LOGOUT:
		//	{
		//		Battle* battle = BattleMgrObj.GetBattle();
		//		if (battle) {
		//			battle->SetFighterOnline(idTarget, false);
		//		}
		//	}
		//		break;
		//	case REHEARSE_LOGIN:
		//	{
		//		Battle* battle = BattleMgrObj.GetBattle();
		//		if (battle) {
		//			battle->SetFighterOnline(idTarget, true);
		//		}
		//	}
		//		break;
		//}
		//
		
	}
	
	void NDMapMgr::processTeam(NDTransData& data)
	{
		unsigned short action = 0; data >> action;
		int senderID = 0; data >> senderID;
		int targetID = 0; data >> targetID;
		
		NDPlayer& player = NDPlayer::defaultHero();
		
		bool newJoin = false;
		
		switch (action) {
			case MSG_TEAM_CREATE:
			{
				// TODO: ����ΪʲôҪresetFocus���ᵼ�»�����ˢ��bug
				//player.ResetFocusRole();
				s_team_info rolesTeam;
				if (senderID == player.m_id) {// ���ѽ���
					player.teamId = player.m_id;
					bolEnableAccept = true;
					GlobalShowDlg(NDCommonCString("tip"), NDCommonCString("CreateTeamSucess"));
					
					newJoin = true;
					
				} else {// ������������
					map_manualrole_it it = m_mapManualrole.begin();
					for (; it != m_mapManualrole.end(); it++)
					{
						if (it->second->m_id == senderID)
						{
							it->second->teamId = senderID;
						}
					}
				}
				rolesTeam.team[0] = senderID;
				m_vecTeamList.push_back(rolesTeam);
				break;
			}
			case MSG_TEAM_KICK:
			{
				for_vec(m_vecTeamList, vec_team_it)
				{
					s_team_info& rolesTeam = *it;
					bool bol = false;
					for (int j = 1; j < eTeamLen; j++) {
						if (rolesTeam.team[j] == targetID) {
							resetTeamPosition(rolesTeam.team[0], targetID);
							rolesTeam.team[j] = 0;
							bol = true;
							break;
						}
					}
					if (bol) {
						break;
					}
				}
				
				// �Լ�����
				NDPlayer& player = NDPlayer::defaultHero();
				if (player.m_id == targetID) {
					player.teamId = 0;
				} else {
					NDManualRole *role = GetManualRole(targetID);
					if (role)
					{
						role->teamId = 0;
					}
				}
				break;
			}
			case MSG_TEAM_LEAVE:
			{
				bool bolLeaderLeft = false;
				for_vec(m_vecTeamList, vec_team_it)
				{
					s_team_info& rolesTeam = (*it);
					if (rolesTeam.team[0] == senderID) 
					{
						bolLeaderLeft = true;
						break;
					}				
				}
				
				if (bolLeaderLeft) {// �ӳ����
					// �ȸ���itemList�ڴ�
					for_vec(m_vecTeamList, vec_team_it)
					{
						s_team_info& rolesTeam = (*it);
						if (rolesTeam.team[0] == senderID) 
						{
							for (int j = 1; j < eTeamLen; j++) {
								if (rolesTeam.team[j] == targetID) {
									resetTeamPosition(senderID);
									rolesTeam.team[0] = rolesTeam.team[j];
									rolesTeam.team[j] = 0;
									break;
								}
							}
							// �����û�����
							for (int j = 1; j < eTeamLen; j++) {
								setManualroleTeamID(rolesTeam.team[j], targetID);
							}
							setManualroleTeamID(senderID, 0);
							break;
						}
					}
				} else {// �Ƕӳ����
					// �ȸ���itemList�ڴ�
					bool bFind = false;
					for_vec(m_vecTeamList, vec_team_it)
					{
						if (bFind) break;
						
						s_team_info& rolesTeam = (*it);
						
						for (int j = 1; j < eTeamLen; j++) {
							if (rolesTeam.team[j] == senderID) {
								resetTeamPosition(rolesTeam.team[0], senderID);
								rolesTeam.team[j] = 0;
								setManualroleTeamID(senderID, 0);
								bFind = true;
								break;
							}
						}
					}
				}
				
				break;
			}
			case MSG_TEAM_ENABLEACCEPT:
			{
				bolEnableAccept = true;
				GlobalShowDlg(NDCommonCString("tip"), NDCommonCString("PositiveJoinTeam"));
				break;
			}
			case MSG_TEAM_DISABLEACCEPT:
			{
				bolEnableAccept = false;
				GlobalShowDlg(NDCommonCString("tip"), NDCommonCString("CantPositiveJoinTeam"));
				break;
			}
			case MSG_TEAM_DISMISS:
			{
				// �ȸ���itemList
				for_vec(m_vecTeamList, vec_team_it)
				{
					s_team_info& rolesTeam = (*it);
					
					if (rolesTeam.team[0] == senderID) 
					{
						resetTeamPosition(senderID);
						
						for (int j = 0; j < eTeamLen; j++) {
							setManualroleTeamID(rolesTeam.team[j], 0);
						}
						m_vecTeamList.erase(it);
						break;
					}
				}
				break;
			}
			case MSG_TEAM_JOIN:
			{
				for_vec(m_vecTeamList, vec_team_it)
				{
					s_team_info& rolesTeam = (*it);
					
					if (rolesTeam.team[0] == targetID)// �ҵ�Ŀ�����
					{
						for (int j = 1; j < eTeamLen; j++) {
							if (rolesTeam.team[j] == 0) {
								rolesTeam.team[j] = senderID;
								setManualroleTeamID(senderID, targetID);
								resetTeamPosition(rolesTeam.team[0], senderID);
								
								if (player.m_id == senderID)
									newJoin = true;
								break;
							}
						}
						break;
					}
				}
				
				break;
			}
			case MSG_TEAM_INVITE:
			{
				if ((player.m_id == targetID)) 
				{
					map_manualrole_it it = m_mapManualrole.begin();
					for (; it != m_mapManualrole.end(); it++)
					{
						if (it->second->m_id == senderID)
						{
							//RequsetInfo info;
							//info.set(senderID, it->second->m_name, RequsetInfo::ACTION_TEAM);
							//addRequst(info);
							break;
						}
					}
				}
				break;
			}
			case MSG_TEAM_CHGLEADER:
			{
				// ����itemList
				for_vec(m_vecTeamList, vec_team_it)
				{
					s_team_info& rolesTeam = (*it);
					
					if (rolesTeam.team[0] == senderID)
					{
						for (int j = 1; j < eTeamLen; j++) {
							if (rolesTeam.team[j] == targetID) {
								resetTeamPosition(senderID);
								rolesTeam.team[j] = senderID;
								rolesTeam.team[0] = targetID;
								break;
							}
						}
						for (int j = 0; j < eTeamLen; j++) {
							setManualroleTeamID(rolesTeam.team[j], targetID);
						}
						break;
						
					}
				}
				break;
			}
			case MSG_TEAM_TEAMID:
			{
				NDPlayer& player = NDPlayer::defaultHero();
				player.teamId = senderID;
				// ����Ƕӳ��Լ���������
				if (player.m_id == senderID) {
					break;
				}
				// ��ӽ�team�б�
				for_vec(m_vecTeamList, vec_team_it)
				{
					s_team_info& rolesTeam = (*it);
					
					if (rolesTeam.team[0] == senderID)
					{
						for (int j = 1; j < eTeamLen; j++) {
							if (rolesTeam.team[j] == 0) {
								rolesTeam.team[j] = player.m_id;
								break;
							}
						}
						break;
					}
				}
				
				break;
			}
		}
		
		QuickInteraction::RefreshOptions();
		
		GameScene* gamescene = GameScene::GetCurGameScene();
		if (gamescene)
			gamescene->TeamRefreh(newJoin);
		
		updateTeamCamp();
	}
	
	void NDMapMgr::processGoodFriend(NDTransData& data)
	{
		unsigned char action = 0; data >> action;
		unsigned char friendCount = 0; data >> friendCount;
		
		//RequsetInfo info;
		//switch (action) {
		//	case _FRIEND_APPLY: {
		//		int iID = 0; data >> iID;
		//		// TAMBGString
		//		std::string name = data.ReadUnicodeString();
		//		info.set(iID, name, RequsetInfo::ACTION_FRIEND);
		//		addRequst(info);
		//		break;
		//	}
		//	case _FRIEND_ACCEPT: {
		//		{
		//			int idFriend = data.ReadInt();
		//			string name = data.ReadUnicodeString();
		//			
		//			FriendElement& fe = this->m_mapFriend[idFriend];
		//			fe.m_id = idFriend;
		//			fe.m_text1 = name;
		//			fe.SetState(ES_ONLINE);
		//			
		//			this->onlineNum++;
		//			
		//			GoodFriendUILayer::refreshScroll();
		//			NewGoodFriendUILayer::refreshScroll();
		//			
		//			name += " "; name += NDCommonCString("BeFriendTip"); name += "��";
		//			Chat::DefaultChat()->AddMessage(ChatTypeSystem, name.c_str());
		//		}
		//		break;
		//	}
		//	case _FRIEND_ONLINE: {
		//		{
		//			int idFriend = data.ReadInt();
		//			
		//			FriendElement& fe = this->m_mapFriend[idFriend];
		//			fe.SetState(ES_ONLINE);
		//			this->onlineNum++;
		//			
		//			string content = std::string("") + NDCommonCString("YourFriend") + " " + fe.m_text1 + " " + NDCommonCString("logined");
		//			Chat::DefaultChat()->AddMessage(ChatTypeSystem, content.c_str());
		//			
		//			GoodFriendUILayer::refreshScroll();
		//			NewGoodFriendUILayer::refreshScroll();
		//		}
		//		break;
		//	}
		//	case _FRIEND_OFFLINE: {
		//		{
		//			int idFriend = data.ReadInt();
		//			
		//			FriendElement& fe = this->m_mapFriend[idFriend];
		//			fe.SetState(ES_OFFLINE);
		//			this->onlineNum--;
		//			
		//			string content = std::string("") + NDCommonCString("YourFriend") + " " + fe.m_text1 + " " + NDCommonCString("OfflineTip");
		//			Chat::DefaultChat()->AddMessage(ChatTypeSystem, content.c_str());
		//			GoodFriendUILayer::refreshScroll();
		//			NewGoodFriendUILayer::refreshScroll();
		//		}
		//		break;
		//	}
		//	case _FRIEND_BREAK: {
		//		{
		//			int idFriend = data.ReadInt();
		//			FriendElement& fe = this->m_mapFriend[idFriend];
		//			
		//			string content = fe.m_text1 + " " + NDCommonCString("DeFriendTip");
		//			Chat::DefaultChat()->AddMessage(ChatTypeSystem, content.c_str());
		//			
		//			this->onlineNum--;
		//			
		//			this->m_mapFriend.erase(idFriend);
		//			GoodFriendUILayer::refreshScroll();
		//			NewGoodFriendUILayer::refreshScroll();
		//			NDUISynLayer::Close();
		//		}
		//		break;
		//	}
		//	case _FRIEND_GETINFO: {
		//		{
		//			this->m_mapFriend.clear();
		//			this->onlineNum = 0;
		//			
		//			for (int i = 0; i < friendCount; i++) {
		//				int idFriend = data.ReadInt();
		//				
		//				Byte state = data.ReadByte();
		//				
		//				string name = data.ReadUnicodeString();
		//				
		//				FriendElement& fe = this->m_mapFriend[idFriend];
		//				
		//				fe.m_id = idFriend;
		//				fe.m_text1 = name;
		//				fe.SetState((ELEMENT_STATE(state)));
		//				
		//				if (state == ES_ONLINE) { // 0������״̬
		//					this->onlineNum++;
		//				}
		//				
		//			}
		//			GoodFriendUILayer::refreshScroll();
		//			NewGoodFriendUILayer::refreshScroll();
		//		}
		//		break;
		//	}
		//	case _FRIEND_REFUSE: {
		//		{
		//			data.ReadInt();
		//			string name = data.ReadUnicodeString();
		//			
		//			name += " "; name += NDCommonCString("RejectFriendTip");
		//			Chat::DefaultChat()->AddMessage(ChatTypeSystem, name.c_str());
		//		}
		//		break;
		//	}
		//	case _FRIEND_DELROLE: {
		//		{
		//			int idFriend = data.ReadInt();
		//			
		//			FriendElement& fe = this->m_mapFriend[idFriend];
		//			
		//			if (fe.m_state == ES_ONLINE) {
		//				this->onlineNum--;
		//			}
		//			
		//			string content = fe.m_text1 + " " + NDCommonCString("DeFriendTip");
		//			Chat::DefaultChat()->AddMessage(ChatTypeSystem, content.c_str());
		//			
		//			this->m_mapFriend.erase(idFriend);
		//			
		//			GoodFriendUILayer::refreshScroll();
		//			NewGoodFriendUILayer::refreshScroll();
		//		}
		//		break;
		//	}
		//}
	}
	
	void NDMapMgr::processTalk(NDTransData& data)
	{
		unsigned char _ucUnuse = 0; data >> _ucUnuse;
		unsigned char pindao = 0; data >> pindao;
		int _iUnuse = 0; data >> _iUnuse;
		data >> _ucUnuse;
		unsigned char amount = 0; data >> amount;
		// msg.append("�ֶ���" + amount);
		std::string speaker;
		std::string text;
		for (int i = 0; i < amount; i++) 
		{
			std::string c = data.ReadUnicodeString(); //data.ReadUnicodeString2(false);			
			if (i == 0) 
			{
				text = c;
			} else if (i == 1) 
			{
				speaker = c;
			}
			// msg.append("����" + c);
			
		}
		// showDialog("����ظ�", msg.toString());
		
		CloseProgressBar;
		
		if (speaker.empty()) {// �ֶ���Ϊ0������û��Speaker��������
			return;
		}
		
		if (NewChatScene::DefaultManager()->IsFilterBySpeaker(speaker.c_str())) {
			return;
		}
		
		//std::stringstream ss;
		if (speaker == "SYSTEM" ) {
			speaker = NDCommonCString("system");
		}
		//std::string msg = ss.str();
		
		ChatType chatType = GetChatTypeFromChannel(pindao);
		if (chatType == ChatTypeWorld && !NDDataPersist::IsGameSettingOn(GS_SHOW_WORLD_CHAT)) 
		{
			return;
		}
		else if (chatType == ChatTypeArmy && !NDDataPersist::IsGameSettingOn(GS_SHOW_SYN_CHAT)) 
		{
			return;
		}
		else if (chatType == ChatTypeQueue && !NDDataPersist::IsGameSettingOn(GS_SHOW_TEAM_CHAT)) 
		{
			return;
		}
		else if (chatType == ChatTypeSection && !NDDataPersist::IsGameSettingOn(GS_SHOW_AREA_CHAT)) 
		{
			return;
		}	
		
		if (chatType == ChatTypeSecret) 
		{
			//speaker.insert(0, "��From��");
			std::string text(speaker);
			if (text != (NDPlayer::defaultHero().m_name)) 
			{
				//RequsetInfo info;
				//info.set(0, NDCommonCString("YouHaveNewChat"), RequsetInfo::ACTION_NEWCHAT);
				//NDMapMgrObj.addRequst(info);
			}
			
		}
		Chat::DefaultChat()->AddMessage(chatType, text.c_str(), speaker.c_str());
		
		
		
		//		if (pindao == 14 && byte_WORLDCHAT == 1) {
		//			return;
		//		} else if (pindao == 19 && byte_MAPCHAT == 1) {
		//			return;
		//		} else if (pindao == 3 && byte_TEAMCHAT == 1) {
		//			return;
		//		} else if (pindao == 4 && byte_BANGCHAT == 1) {
		//			return;
		//		}
		
		//ChatRecord chat = new ChatRecord(pindao, speaker, text);
		//		if (pindao == 1) {
		//			chat.miIndication = ChatRecord.MI_FROM;
		//		}
		//		if (ChatUI.instance != null) {
		//			ChatUI.instance.addChatRecode(chat);
		//		} else {
		//			ChatUI.addChatRecodeChatList(chat);
		//		}
		// ��������Ϣ��,��Ļ�·���ʾ
		//if (GameScreen.getInstance() != null) {
		//			ChatRecord c = new ChatRecord(pindao, speaker, text);
		//			if (pindao == 1) {
		//				c.miIndication = ChatRecord.MI_FROM;
		//			}
		//			GameScreen.getInstance().initNewChat(c);
		//		}
		
		
	}
	//
	//LifeSkill* NDMapMgr::getLifeSkill(OBJID idSkill)
	//{
	//	MAP_LEARNED_LIFE_SKILL_IT it = this->m_mapLearnedLifeSkill.find(idSkill);
	//	if (it != this->m_mapLearnedLifeSkill.end()) {
	//		return &(it->second);
	//	}
	//	return NULL;
	//}
	//
	//FormulaMaterialData*  NDMapMgr::getFormulaData(int idFoumula)
	//{
	//	map_fomula_it it = m_mapFomula.find(idFoumula);
	//	return it == m_mapFomula.end() ? NULL : it->second;
	//}
	//
	//void NDMapMgr::getFormulaBySkill(int idSkill, std::vector<int>& vec_id)
	//{
	//	map_fomula_it it = m_mapFomula.begin();
	//	for(; it != m_mapFomula.end(); it++)
	//	{
	//		if (it->second && it->second->getSkillID() == idSkill) 
	//		{
	//			vec_id.push_back(it->first);
	//		}
	//	}
	//}
	//
	NDBaseRole* NDMapMgr::GetRoleNearstPlayer(int iDistance)
	{
		int minDist = iDistance;
		
		NDPlayer *player = &NDPlayer::defaultHero();
		
		NDBaseRole * resrole = NULL;
		
		if (!player)
		{
			return resrole;
		}
		
		do 
		{
			vec_npc_it it = m_vNpc.begin();
			for (; it != m_vNpc.end(); it++)
			{
				NDNpc *npc = *it;
				int dis = getDistBetweenRole(player, npc);
				if ( dis < minDist )
				{
					resrole = npc;
					minDist = dis;
					player->targetIndex = 0;
				}
			}
		} while (0);
		
		do 
		{
			map_manualrole_it it = m_mapManualrole.begin();
			for (; it != m_mapManualrole.end(); it++)
			{
				NDManualRole *otherplayer = it->second;
				
				if (player->teamId !=0 && player->teamId == otherplayer->teamId)
				{
					continue;
				}
				
				int dis = getDistBetweenRole(player, otherplayer);
				if ( dis < minDist )
				{
					resrole = otherplayer;
					minDist = dis;
				}
			}
		} while (0);
		
		return resrole;
	}
	
	NDBaseRole* NDMapMgr::GetNextTarget(int iDistance)
	{
		NDPlayer *player = &NDPlayer::defaultHero();
		
		NDBaseRole * resrole = NULL;
		
		if (!player)
		{
			return resrole;
		}
		
		if (player->targetIndex >= int(m_vNpc.size()+m_mapManualrole.size()))
		{
			player->targetIndex = 0;
		}
		
		if (player->targetIndex < int(m_vNpc.size()) )
		{
			vec_npc_it it = m_vNpc.begin()+player->targetIndex;
			for (; it != m_vNpc.end(); it++)
			{
				player->targetIndex++;
				NDNpc *npc = *it;
				
				if (npc->m_id == player->GetFocusNpcID() )
				{
					continue;
				}
				
				int dis = getDistBetweenRole(player, npc);
				if ( dis < iDistance )
				{
					resrole = npc;
					return resrole;
				}
			}
		}
		
		int iIndexManuRole = player->targetIndex - m_vNpc.size();
		
		if (iIndexManuRole < 0)
		{
			iIndexManuRole = 0;
		}
		
		if (iIndexManuRole < (int)m_mapManualrole.size())
		{
			map_manualrole_it it = m_mapManualrole.begin();
			for (int i = 0; i < iIndexManuRole; i++)
			{
				it++;
			}
			
			for (; it != m_mapManualrole.end(); it++)
			{
				player->targetIndex++;
				
				NDManualRole *otherplayer = it->second;
				
				if (otherplayer->m_id == player->m_iFocusManuRoleID )
				{
					continue;
				}
				
				int dis = getDistBetweenRole(player, otherplayer);
				if ( dis < iDistance )
				{
					resrole = otherplayer;
					return resrole;
				}
			}
		}
		
		return resrole;
	}
	
	int NDMapMgr::getDistBetweenRole(NDBaseRole *firstrole, NDBaseRole *secondrole)
	{
		if (!firstrole || !secondrole)
		{
			return FOCUS_JUDGE_DISTANCE;
		}
		
		
		int w = (firstrole->GetPosition().x-DISPLAY_POS_X_OFFSET)/MAP_UNITSIZE - (secondrole->GetPosition().x-DISPLAY_POS_X_OFFSET)/MAP_UNITSIZE;
		int h = (firstrole->GetPosition().y-DISPLAY_POS_Y_OFFSET)/MAP_UNITSIZE - (secondrole->GetPosition().y-DISPLAY_POS_Y_OFFSET)/MAP_UNITSIZE;
		
		return w * w + h * h;
	}
	
	void NDMapMgr::ClearNPCChat()
	{
		usData = -1;
		strLeaveMsg.clear();
		strTitle.clear();
		strNPCText.clear();
		vecNPCOPText.clear();
		m_iCurDlgNpcID = 0;
	}
	
	void NDMapMgr::quitGame()
	{
		m_vNpc.clear();
		m_vMonster.clear();
		m_mapMonsterInfo.clear();
		m_mapManualrole.clear();
		usData = 0;
		strLeaveMsg.clear();
		strTitle.clear();
		strNPCText.clear();
		vecNPCOPText.clear();
		//m_mapLearnedLifeSkill.clear();
		//m_mapGP.clear();
		m_mapNpcStore.clear();
		
		for_vec(m_vEmail, vec_email_it)
		{
			EmailData	*data = *it;
			delete data;
		}
		m_vEmail.clear();
		NDEmailDataPersist::Destroy();
		
		bFirstCreate = true;
		
		bVerifyVersion = true;
		
		NDPlayer::defaultHero().InvalidNPC();
		
		GameSceneReleaseHelper::End();
		
		bRootItemZhangKai = false;
		bRootMenuZhangKai = false;
		
#ifdef VIEW_PERFORMACE
		PerformanceSave;
		PerformanceDisable;
#endif
	}
	
	void NDMapMgr::WorldMapSwitch(int mapId)
	{
		NDScene* scene = NDDirector::DefaultDirector()->GetRunningScene();
		if (!scene) 
		{
			return;
		}
		
		NDDirector::DefaultDirector()->PushScene(GameSceneLoading::Scene());
		
		NDPlayer& player = NDPlayer::defaultHero();
		NDTransData bao(_MSG_POSITION);
		bao << player.m_id << (unsigned short)0 << (unsigned short)0 
		<< mapId << (unsigned short)_WORD_MAPCHANGE << int(0);
		// SEND_DATA(bao);
	}
	
	void NDMapMgr::throughMap(int mapX, int mapY, int mapId)
	{
		NDScene* scene = NDDirector::DefaultDirector()->GetRunningScene();
		if (!scene) 
		{
			return;
		}
		
		NDDirector::DefaultDirector()->PushScene(GameSceneLoading::Scene());
		
		NDPlayer& player = NDPlayer::defaultHero();
		NDTransData bao(_MSG_POSITION);
		bao << player.m_id << (unsigned short)mapX << (unsigned short)mapY
		<< mapId << (unsigned short)_POSITION_TRANS_FLY << int(0);
		// SEND_DATA(bao);
	}
	
	void NDMapMgr::NavigateTo(int mapX, int mapY, int mapId)
	{
		if (GetMapID() == mapId)
		{
			NDPlayer::defaultHero().Walk(CGPointMake(mapX * MAP_UNITSIZE, mapY * MAP_UNITSIZE), SpriteSpeedStep4, true);
			AutoPathTipObj.work("");
			return;
		}
		
		throughMap(mapX, mapY, mapId);
	}
	
	void NDMapMgr::NavigateToNpc(int nNpcId)
	{
		NDNpc* npc = GetNpcByID(nNpcId);
		if (!npc) return;
		
		NDPlayer& player = NDPlayer::defaultHero();
		
		CGPoint dstPoint = ccp(npc->col * MAP_UNITSIZE + DISPLAY_POS_X_OFFSET, npc->row * MAP_UNITSIZE + DISPLAY_POS_Y_OFFSET);
		
		//if (npc->getNearestPoint(player.GetPosition(), dstPoint))
		{
			NDPlayer& player = NDPlayer::defaultHero();
			
			CGPoint disPos = ccpSub(dstPoint, player.GetPosition());
			
			if (abs(int(disPos.x)) <= 32 && abs(int(disPos.y)) <= 32)
			{
				if (npc && npc->GetType() != 6) 
				{
					player.SendNpcInteractionMessage(npc->m_id);
//					if (npc->IsDirectOnTalk()) 
//					{
//						//npc�����޸�	
//						if (player.GetPosition().x > npc->GetPosition().x) 
//							npc->DirectRight(true);
//						else 
//							npc->DirectRight(false);
//					}
				}
			}
			else
			{
				AutoPathTipObj.work(npc->m_name);
				player.Walk(dstPoint, SpriteSpeedStep4, true);
			}
			
			return;
		}
		
		return;
	}
	
	//bool NDMapMgr::GetRequest(int iID, RequsetInfo& info)
	//{ 
	//	std::vector<RequsetInfo>::iterator it = m_vecRequest.begin();
	//	for(; it != m_vecRequest.end(); it++)
	//	{
	//		if (iID == (*it).iID)
	//		{
	//			info = (*it);
	//			return true;
	//		}
	//		
	//	}
	//	
	//	return false;
	//}
	//
	//bool NDMapMgr::DelRequest(int iID)
	//{
	//	std::vector<RequsetInfo>::iterator it = m_vecRequest.begin();
	//	for(; it != m_vecRequest.end(); it++)
	//	{
	//		if (iID == (*it).iID)
	//		{
	//			m_idAlloc.ReturnID(iID);
	//			m_vecRequest.erase(it);
	//			return true;
	//		}
	//	}
	//	
	//	return false;
	//}
	//
	//void NDMapMgr::addRequst(RequsetInfo& request)
	//{
	//	//if (GameScreen.getInstance() != null) {
	//	//			GameScreen.getInstance().initNewChat(
	//	//												 new ChatRecord(5, NDCommonCString("system"), "����һ���µ�" + request.getMText2()
	//	//																+ ",��������б�鿴"));
	//	//		}
	//	
	//	std::stringstream strBuf; strBuf << NDCommonCString("YouHaveNew") << request.info << "," << NDCommonCString("OpenRequestList");
	//	Chat::DefaultChat()->AddMessage(ChatTypeSystem, strBuf.str().c_str());
	//	
	//	std::vector<RequsetInfo>::iterator it = m_vecRequest.begin();
	//	for (; it != m_vecRequest.end(); it++)
	//	{
	//		RequsetInfo& info = *it;
	//		if (info.iRoleID == request.iRoleID && info.iAction == request.iAction && (info.iAction != RequsetInfo::ACTION_NEWMAIL || info.iAction != RequsetInfo::ACTION_NEWCHAT))
	//		{
	//			DelRequest(info.iID);
	//			break;
	//		}
	//	}
	//	request.iID = m_idAlloc.GetID();
	//	m_vecRequest.push_back(request);
	//	
	//	//NewGameUIRequest::refreshQuestList();
	//	
	//	NDScene *scene = NDDirector::DefaultDirector()->GetScene(RUNTIME_CLASS(GameScene));
	//	if (scene) 
	//	{
	//		GameScene* gamescene = (GameScene*)scene;
	//		gamescene->flashAniLayer(0, true);
	//		
	//		//NDNode *node = gamescene->GetChild(UILAYER_REQUEST_LIST_TAG);
	//		//if (node && node->IsKindOfClass(RUNTIME_CLASS(GameUIRequest)))
	//		//{
	//		//	GameUIRequest *request = (GameUIRequest*)node;
	//		//	if (request->IsVisibled())
	//		//	{
	//		//		request->UpdateMainUI();
	//		//	}
	//		//}
	//	}
	//}
	//
	void NDMapMgr::resetTeamPosition(int leadId, int roleId)
	{
		NDManualRole *lead = GetTeamRole(leadId);
		
		NDManualRole *role = GetTeamRole(roleId);
		
		if (!role || !lead)
		{
			return;
		}
		int iServerCol = lead->GetServerX();
		int iServerRow = lead->GetServerY();
		
		CGPoint pos = ccp(iServerCol*MAP_UNITSIZE+DISPLAY_POS_X_OFFSET, iServerRow*MAP_UNITSIZE+DISPLAY_POS_Y_OFFSET);
		role->SetPosition(pos);
		role->SetServerPositon(iServerCol, iServerRow);
		role->SetAction(false);
		
		if (role->IsKindOfClass(RUNTIME_CLASS(NDPlayer))) 
		{
			NDMapLayer *layer = NDMapMgrObj.getMapLayerOfScene( NDDirector::DefaultDirector()->GetRunningScene());
			if (!layer) 
			{
				return;
			}
			
			layer->SetScreenCenter(pos);
		}
		//ManualRole lead = Scene.getRole(leadId);
		//		if (lead != null) {
		//			int col = lead.getServerCol();
		//			int row = lead.getServerRow();
		//			ManualRole rr = Scene.getRole(roleId);
		//			if (rr != null) {
		//				rr.resetPosition(col, row);
		//				// �����ӳ����ʱ�ػ�һ�εر�
		//				GameScreen.getInstance().setRepaintAllOnce(false);
		//			}
		//		}
	}
	
	void NDMapMgr::resetTeamPosition(int teamID)
	{
		for_vec(m_vecTeamList, vec_team_it)
		{
			s_team_info& info = *it;
			if (info.team[0] == teamID)
			{
				NDManualRole *lead = GetTeamRole(teamID);
				if (!lead)
				{
					return;
				}
				int iServerCol = lead->GetServerX();
				int iServerRow = lead->GetServerY();
				
				CGPoint pos = ccp(iServerCol*MAP_UNITSIZE+DISPLAY_POS_X_OFFSET,
								  iServerRow*MAP_UNITSIZE+DISPLAY_POS_Y_OFFSET);
				
				//if (lead != NULL) {
				//int col = lead.getServerCol();
				//					int row = lead.getServerRow();
				//					for (int j2 = 0; j2 < eTeamLen; j2++) {
				//						ManualRole rr = Scene.getRole(rolesTeam[j2]);
				//						if (rr != null) {
				//							rr.resetPosition(col, row);
				//						}
				//					}
				for(int i = 1; i < eTeamLen; i++ )
				{	
					if (info.team[i] == 0) 
					{
						continue;
					}
					NDManualRole *role = GetTeamRole(info.team[i]);
					if (role) 
					{
						role->SetPosition(pos);
						
						role->SetServerPositon(iServerCol, iServerRow);
						
						role->SetAction(false);
						
						if (role->IsKindOfClass(RUNTIME_CLASS(NDPlayer))) 
						{
							NDMapLayer *layer = NDMapMgrObj.getMapLayerOfScene( NDDirector::DefaultDirector()->GetRunningScene());
							if (!layer) 
							{
								return;
							}
							
							layer->SetScreenCenter(pos);
						}
					}
				}
				//}
			}
		}
	}
	
	NDManualRole* NDMapMgr::GetTeamRole(int iID)
	{
		NDPlayer& player = NDPlayer::defaultHero();
		NDManualRole *role = NULL;
		if (player.m_id == iID)
		{
			role = &(NDPlayer::defaultHero());
		}
		
		if (role == NULL)
		{
			role = GetManualRole(iID);
		}
		
		return role;
	}
	
	void NDMapMgr::setManualroleTeamID(int iRoleID, int iTeamID)
	{
		NDManualRole* role = GetTeamRole(iRoleID);
		if (role) 
		{
			role->teamId = iTeamID;
		}
	}
	
	bool NDMapMgr::GetTeamInfo(int iTeamID, s_team_info& info)
	{
		for_vec(m_vecTeamList, vec_team_it)
		{
			if ((*it).team[0] == iTeamID) 
			{
				info = (*it);
				return true;
			}
		}
		
		return false;
	}
	
	void NDMapMgr::updateTeamCamp()
	{
		if	(m_vecTeamList.empty())
		{
			return;
		}
		
		for_vec(m_vecTeamList, vec_team_it)
		{
			s_team_info& rolesTeam = (*it);
			int campTmp = 0;
			
			for (int j = 0; j < eTeamLen; j++) {
				NDManualRole *role = GetTeamRole(rolesTeam.team[j]);
				if (role != NULL && role->IsInState(USERSTATE_CAMP)) {
					campTmp = role->campOutOfTeam;
					if (campTmp == -1) {
						campTmp = role->GetCamp();
					}
				}
				if (campTmp > 0) {
					break;
				}
			}
			
			for (int j = 0; j < eTeamLen; j++) {
				NDManualRole *role = GetTeamRole(rolesTeam.team[j]);
				if (role != NULL) {
					if (role->campOutOfTeam == -1) { // �״�
						role->campOutOfTeam = role->GetCamp();
					}
					role->SetCamp(CAMP_TYPE(campTmp));
				}
			}
		}
	}
	
	void NDMapMgr::updateTeamListAddPlayer(NDManualRole& role)
	{
		bool bolHas = false;
		if (role.isTeamMember()) {
			// �����£��Ƿ��Ѿ��д��������������ڵĶ���
			for_vec(m_vecTeamList, vec_team_it)
			{
				s_team_info& info = (*it);
				if (info.team[0] == role.teamId) {
					bolHas = true;
					if (!role.isTeamLeader()) 
					{
						for(int i = 1; i < eTeamLen; i++)
						{
							if (info.team[i] == role.m_id) 
							{
								break;
							}
							if (info.team[i] == 0) 
							{
								info.team[i] = role.m_id;
								break;
							}
						}
					}
					break;
				}
			}
			
			if (!bolHas) {
				s_team_info info;
				info.team[0] = role.teamId;
				if (role.teamId != role.m_id) 
				{
					info.team[1] = role.m_id;
				}
				m_vecTeamList.push_back(info);
			}
		}
		
		GameScene* gamescene = GameScene::GetCurGameScene();
		if (gamescene)
			gamescene->TeamRefreh(false);
	}
	
	void NDMapMgr::updateTeamListDelPlayer(NDManualRole& role)
	{
		if (!role.isTeamMember()) {
			return;
		}
		
		for_vec(m_vecTeamList, vec_team_it)
		{
			s_team_info& rolesTeam = *it;
			
			if (rolesTeam.team[0] == role.teamId) {
				if (role.isTeamLeader()) {
					m_vecTeamList.erase(it);
					break;
				} else {
					bool bolHandled = false;
					if (role.teamId == rolesTeam.team[0]) {
						for (int j = 1; j < eTeamLen; j++) {
							if (rolesTeam.team[j] == role.m_id) {
								rolesTeam.team[j] = 0;
								bolHandled = true;
								break;
							}
						}
						if (bolHandled) {
							break;
						}
					}
				}
				
			}
		}
	}
	
	std::vector<NDManualRole*> NDMapMgr::GetPlayerTeamList()
	{
		NDPlayer& player = NDPlayer::defaultHero();
		std::vector<NDManualRole*> res;
		map_manualrole_it it = m_mapManualrole.begin();
		for (; it != m_mapManualrole.end(); it++)
		{
			NDManualRole *otherplayer = it->second;
			if (otherplayer && !otherplayer->bClear && otherplayer->teamId == player.teamId) 
			{
				res.push_back(otherplayer);
			}
		}
		return res;
	}
	
	NDManualRole* NDMapMgr::GetTeamLeader(int teamID)
	{
		for_vec(m_vecTeamList, vec_team_it)
		{
			if ((*it).team[0] == teamID) 
			{
				return GetTeamRole(teamID);
			}
		}
		
		return NULL;
	}
	
	NDManualRole* NDMapMgr::GetTeamRole(int teamID, int index)
	{
		if (index >= eTeamLen) return NULL;
		
		for_vec(m_vecTeamList, vec_team_it)
		{
			if ((*it).team[0] == teamID) 
			{
				if ( (*it).team[index] == 0 ) return NULL;
				
				return GetManualRole((*it).team[index]);
			}
		}
		
		return NULL;
	}
	
	// ����Ի�����Ϣ
	void NDMapMgr::processMsgDlg(NDTransData& data)
	{
		int npcID = data.ReadInt();
		
		this->usData = data.ReadShort();
		
		Byte iDx = data.ReadByte();
		
		Byte ucAction = data.ReadByte();
		string str;
		
		// ��ֹ��Ϣ��ʱ���ã���������
		if (BattleMgrObj.GetBattle() != NULL) {
			return;
		}
		
		NDPlayer& player = NDPlayer::defaultHero();
		switch (ucAction) {
			case MSGDIALOG_LEAVE: {
				this->strLeaveMsg = data.ReadUnicodeString();
				break;
			}
			case MSGDIALOG_TITLE: {
				this->strTitle = data.ReadUnicodeString();
				break;
			}
			case MSGDIALOG_TEXT: { // TEXT �ı���Ϣ����
				if (this->usData == TEXT_TEXT) {
					str = data.ReadUnicodeString();
					strNPCText += str;
				}
				break;
			}
			case MSGDIALOG_LINK: { // ѡ�����ʽ //��
				if (usData == TEXT_TEXT) {
					str = data.ReadUnicodeString();
					str = changeToChineseSign(str);
					st_npc_op op;
					op.idx = iDx;
					op.str = str;
					vecNPCOPText.push_back(op);
				}
				
				//npc �Ի�
				/*TextView temp = new TextView(str, 4);
				 temp.parama = new Byte(iDx);
				 npc_options.addElement(temp);*/
				break;
			}
			case MSGDIALOG_NO_TALK: {
				NDUISynLayer::Close(CLOSE);
				break;
			}
			case MSGDIALOG_DLG: { // �Ի�����ʽ
				if (usData == TEXT_TEXT) {
					str = data.ReadUnicodeString();
					str = changeNpcString(str);
					
					//NDUIDialog *dlg = new NDUIDialog;
					//					dlg->Initialization();
					std::string title="";
					if ( strTitle.empty())
					{
						NDNpc *focusNpc = player.GetFocusNpc();
						if (focusNpc)
						{
							title = focusNpc->m_name;
						}
					}
					
					//dlg->Show(title.c_str(), str.c_str(), "", NULL);
					GlobalDialogObj.Show(NULL, title.c_str(), str.c_str(), 0, NULL);
					CloseProgressBar;
				}
				
				// todo ncp�Ի�
				/*Dialog dialog = new Dialog();
				 str = T.changeNpcString(str);
				 if (T.curNpc != null) {
				 dialog.setContent(T.curNpc.getName(), ChatRecord.parserChat(str, -1));
				 } else {
				 dialog.setContent("Npc", ChatRecord.parserChat(str, -1));
				 }
				 dialog.height = 130;
				 T.addDialog(dialog);*/
				break;
			}
			case MSGDIALOG_USER_OPEN_DLG: {
				switch (this->usData) {
					case 1: { // ���̵����
						NDNpc *npc = player.GetFocusNpc();
						if (!npc) 
						{
							CloseProgressBar;
							return;
						}
						
						CloseProgressBar;
						
						int npcCamp = npc->GetCamp();
						if (npcCamp - 1 >= 0 && npcCamp - 1 < 7) {
							int discount = getDiscount(zhengYing[npcCamp - 1]);
							if (discount != 100) {
								std::stringstream ss; ss << NDCommonCString("ShopEnjoy") << discount << "%" << NDCommonCString("discount");
								showDialog(NDCommonCString("tip"), ss.str().c_str());
							}
						}
						
						map_npc_store_it it = m_mapNpcStore.find(npc->m_id);
						if (it == m_mapNpcStore.end()) 
						{
							NDTransData bao(_MSG_SHOPINFO);
							bao << int(npc->m_id) << (unsigned char)0;
							// SEND_DATA(bao); 
							ShowProgressBar;
						}
						else
						{
							//std::vector<ShopItemInfo>& idList = it->second;
							GameUINpcStore::GenerateNpcItems(npc->m_id);
							GameScene::ShowShop();
						}
						
						break;
					}
					case 2:
					case 3: { // ��װ������
						//T.addDialog(new EquipUIScreen(EquipUIScreen.SHOW_EQUIP_NORMAL));
						break;
					}
					case 4: { // �򿪲ֿ�
						if (!player.GetFocusNpc()) 
						{
							break;
						}
						CloseProgressBar;
						int iNPCID = player.GetFocusNpcID();
						if (ItemMgrObj.GetStorage().empty()) 
						{
							NDTransData bao(_MSG_ITEMKEEPER);
							bao << int(0) << (unsigned char)MSG_STORAGE_ITEM_QUERY << iNPCID;
							// SEND_DATA(bao);
							ShowProgressBar;
						} else {
							NDDirector::DefaultDirector()->PushScene(GameStorageScene::Scene());
						}
						break;
					}
					case 5: { // todo ��ʱû�� �������
						NDPlayer& player = NDPlayer::defaultHero();
						if (player.marriageID != 0) {
							this->m_idDeMarry = GlobalDialogObj.Show(this, NDCommonCString("tip"),
																	 NDCommonCString("ApplyDivorce"), NULL, NDCommonCString("divorce"), NULL);
							//							Dialog dialog = new Dialog();
							//							dialog.setContent("���", " �����ȷ��Ҫ�������?");
							//							TextView ops[] = new TextView[2];
							//							ops[0] = new TextView("���", 4);
							//							ops[0].id = (1);
							//							ops[0].parama = dialog;
							//							ops[0].setOnClickListener(this);
							//							
							//							ops[1] = new TextView("ȡ��", 4);
							//							ops[1].id = (2);
							//							ops[1].parama = dialog;
							//							ops[1].setOnClickListener(this);
							//							dialog.setOperator(ops);
							//							T.addDialog(dialog);
						} else { // �򿪽�����ѡ�����
							if (player.sex == SpriteSexMale) {
								s_team_info info;
								if (!GetTeamInfo(player.m_id, info))
								{
									GlobalShowDlg(NDCommonCString("tip"), NDCommonCString("NotFindObject"));
									return;
								}
								
								vec_marriage vMarriage;
								for (int i = 1; i < eTeamLen; i++) 
								{
									
									int iRoleID = info.team[i];
									if (iRoleID == player.m_id)
										continue;
									NDManualRole* role = GetManualRole(iRoleID);
									if (!role || role->sex != SpriteSexFemale) 
										continue;
									vMarriage.push_back(MarriageInfo(role->m_name, iRoleID));
								}
								
								if (vMarriage.empty())
									if (!GetTeamInfo(player.m_id, info))
									{
										GlobalShowDlg(NDCommonCString("tip"), NDCommonCString("NotFindObject"));
										return;
									}
								
								GameScene* scene = GameScene::GetCurGameScene();
								if (!scene) return;
								
								scene->ShowMarriageList(vMarriage);
								//
								//								Array roles = GameScreen.getInstance().getScene().getRoles();
								//								if (roles == null || roles.size() == 0) {
								//									T.showErrorDialog("û���ҵ�����");
								//								} else {
								//									T.addDialog(new MarryListMenu(5, 64, 120, 60, roles));
								//								}
							} else {
								GlobalShowDlg(NDCommonCString("tip"), NDCommonCString("FindHandsome"));
							}
						}
						
						CloseProgressBar;
						
						break;
					}
					case 6: {// ����
						this->m_idAuctionDlg = GlobalDialogObj.Show(this, NDCommonCString("PaiMaiHang"),
																	NDCommonCString("PaiMaiHangWelcome"), 0, NDCommonCString("MyPaiMai"), NDCommonCString("SearchItem"), NULL);
						break;
					}
					case 7: {// ����
						CreateSynDialog::Show();
						break;
					}
					case 8: {
						int idCurNpc = 0;
						if (player.IsFocusNpcValid()) {
							idCurNpc = player.GetFocusNpcID();
						}
						
						if (idCurNpc == 0) {
							return;
						}
						
						MAP_NPC_SKILL_STORE_IT it = this->m_mapNpcSkillStore.find(idCurNpc);
						
						if (it == m_mapNpcSkillStore.end()) { // û�д棬������
							NDTransData bao(_MSG_MAGIC_GOODS);
							bao << idCurNpc;
							// SEND_DATA(bao);
							ShowProgressBar;
						} else {
							LearnSkillUILayer::Show(it->second);
						}
						break;
					}
					case 9: {// װ������
						CloseProgressBar;
						NDDirector::DefaultDirector()->PushScene(NewEquipRepairScene::Scene());
						//GamePlayerBagScene *scene = new GamePlayerBagScene;
						//scene->Initialization(SHOW_EQUIP_REPAIR);
						//NDDirector::DefaultDirector()->PushScene(scene);
						//T.addDialog(new EquipUIScreen(EquipUIScreen.SHOW_EQUIP_REPAIR));
						break;
					}
					case 11: { // װ��Ʒ������
						
						/*T.closeTopDialog();
						 EquipUpgrade dlg = new EquipUpgrade(EquipUpgrade.EQUIP_UPGRADE);
						 T.addDialog(dlg);*/
						
						CloseProgressBar;
						EquipUpgradeScene *scene = new EquipUpgradeScene;
						scene->Initialization(EQUIP_UPGRADE);
						NDDirector::DefaultDirector()->PushScene(scene);
						break;
					}
					case 12: { // װ������
						CloseProgressBar;
						/*EquipUpgradeScene *scene = new EquipUpgradeScene;
						 scene->Initialization(EQUIP_ENHANCE);
						 NDDirector::DefaultDirector()->PushScene(scene);*/
						NDScene* runningScene = NDDirector::DefaultDirector()->GetRunningScene();
						if (runningScene && runningScene->IsKindOfClass(RUNTIME_CLASS(EquipForgeScene))) {
							return;
						} else {
							EquipForgeScene *scene = new EquipForgeScene;
							scene->Initialization();
							NDDirector::DefaultDirector()->PushScene(scene);
						}
						break;
					}
					case 13: { // ��ʯժ��
						//T.addDialog(new RemoveStone());
						CloseProgressBar;
						RemoveStoneScene *scene = new RemoveStoneScene;
						scene->Initialization();
						NDDirector::DefaultDirector()->PushScene(scene);
						break;
					}
					case 14: { // װ������
						//T.addDialog(new OpenHole());
						CloseProgressBar;
						OpenHoleScene *scene = new OpenHoleScene;
						scene->Initialization();
						NDDirector::DefaultDirector()->PushScene(scene);
						break;
					}
					//case 15: { // ��ȡ���
					//	//T.closeSynDialog();
					//	//T.getDisplay().setCurrent(new InputForm("�������к�",0,InputForm.GIFT_NPC));
					//	NDUICustomView *view = new NDUICustomView;
					//	view->Initialization();
					//	view->SetDelegate(this);
					//	std::vector<int> vec_id; vec_id.push_back(1);
					//	std::vector<std::string> vec_str; vec_str.push_back(NDCommonCString("InputSeq"));
					//	view->SetEdit(1, vec_id, vec_str);
					//	view->SetTag(eCVOP_GiftNPC);
					//	view->Show();
					//	NDScene* scene = NDDirector::DefaultDirector()->GetRunningScene();
					//	if (scene)
					//	{
					//		scene->AddChild(view);
					//	}
					//	CloseProgressBar;					
					//	break;	
					//}
					//case 16:{ // 16�����ҩ��17���䷽��ҩ��18����ϳɣ�19���䷽�ϳ�
					//	CloseProgressBar;
					//	LifeSkillRandomScene *scene = new LifeSkillRandomScene;
					//	scene->Initialization(eChaoYao);
					//	NDDirector::DefaultDirector()->PushScene(scene);
					//	//T.closeTopDialog();
					//	//T.addDialog(new com.nd.kgame.system.skill.SkillDialog(7));
					//	break;
					//}
					//case 17:{ //17���䷽��ҩ
					//	/*if(LifeSkillListener.getLifeSkill(LifeSkill.ALCHEMY_IDSKILL)!=null){
					//	 T.closeTopDialog();
					//	 T.addDialog(new com.nd.kgame.system.skill.SkillDialog(5));
					//	 }else{
					//	 T.addDialog(new Dialog("����ʧ��", "�����㻹ľ��ѧϰ��������!�Ͻ�ȥ����������npc����ѧϰ��.",Dialog.PRIV_HIGH));
					//	 }*/
					//	CloseProgressBar;
					//	if ( getLifeSkill(ALCHEMY_IDSKILL) != NULL )
					//	{
					//		LifeSkillScene *scene = new LifeSkillScene;
					//		scene->Initialization(ALCHEMY_IDSKILL, LifeSkillScene_Product);
					//		NDDirector::DefaultDirector()->PushScene(scene);
					//	}
					//	else 
					//	{
					//		GlobalShowDlg(NDCommonCString("OperateFail"), NDCommonCString("NoLianJingSkillTip"));
					//	}
					//	
					//	break;
					//}
					//case 18:{ //18����ϳ�
					//	CloseProgressBar;
					//	LifeSkillRandomScene *scene = new LifeSkillRandomScene;
					//	scene->Initialization(eBaoShiYuanShi);
					//	NDDirector::DefaultDirector()->PushScene(scene);
					//	//T.closeTopDialog();
					//	//T.addDialog(new com.nd.kgame.system.skill.SkillDialog(8));
					//	break;
					//}
					//case 19:{ //19���䷽�ϳ�
					//	/*if(LifeSkillListener.getLifeSkill(LifeSkill.GEM_IDSKILL)!=null){
					//	 T.closeTopDialog();
					//	 T.addDialog(new com.nd.kgame.system.skill.SkillDialog(6));
					//	 }else{
					//	 T.addDialog(new Dialog(NDCommonCString("OperateFail"),"�����㻹ľ��ѧϰ��ʯ�ϳɼ�����!�Ͻ�ȥ������ʯ�ϳ�npc����ѧϰ��.", Dialog.PRIV_HIGH));
					//	 }*/
					//	CloseProgressBar;
					//	if ( getLifeSkill(GEM_IDSKILL) != NULL )
					//	{
					//		LifeSkillScene *scene = new LifeSkillScene;
					//		scene->Initialization(GEM_IDSKILL, LifeSkillScene_Product);
					//		NDDirector::DefaultDirector()->PushScene(scene);
					//	}
					//	else 
					//	{
					//		GlobalShowDlg(NDCommonCString("OperateFail"), NDCommonCString("NoBaoShiSkillTip"));
					//	}
					//	break;
					//}
					//case 20:
					//	ForgetSkillUILayer::Show();
					//	break;
					//case 21: // ��Ը��
					//	/*T.closeTopDialog();
					//	 Form f = new Form("��Ը");
					//	 f.addCommand(sendCmd);
					//	 f.addCommand(T.backCmd);
					//	 tf = T.getTextField("���������ݣ��������¼�����༭״̬������������ͣ�", null, 40, 3);
					//	 f.append(tf);
					//	 f.append("�������ֲ��ó���40������");
					//	 f.setCommandListener(this);
					//	 T.getDisplay().setCurrent(f);*/
					//{
					//	NDUICustomView *view = new NDUICustomView;
					//	view->Initialization();
					//	view->SetDelegate(this);
					//	std::vector<int> vec_id; vec_id.push_back(1);
					//	std::vector<std::string> vec_str; vec_str.push_back(NDCommonCString("InputWishTip"));
					//	view->SetEdit(1, vec_id, vec_str);
					//	view->SetTag(eCVOP_Wish);
					//	view->Show();
					//	NDScene* scene = NDDirector::DefaultDirector()->GetRunningScene();
					//	if (scene)
					//	{
					//		scene->AddChild(view);
					//	}
					//	CloseProgressBar;
					//}
					//	
					//	break;
					//case 22: // ��Ը��
					//	/*T.closeTopDialog();
					//	 Form f1= new Form("�޸�Ը��");
					//	 f1.addCommand(sendCmd);
					//	 f1.addCommand(T.backCmd);
					//	 tf = T.getTextField("���������ݣ��������¼�����༭״̬������������ͣ�", null, 40, 3);
					//	 f1.append(tf);
					//	 f1.append("�������ֲ��ó���40������");
					//	 f1.append("�޸�Ը����֮ǰ�����µ�Ը�����ᱻ�µ�Ը��������");
					//	 f1.setCommandListener(this);
					//	 T.getDisplay().setCurrent(f1);*/
					//{
					//	NDUICustomView *view = new NDUICustomView;
					//	view->Initialization();
					//	view->SetDelegate(this);
					//	std::vector<int> vec_id; vec_id.push_back(1);
					//	std::vector<std::string> vec_str; vec_str.push_back(std::string("") + NDCommonCString("InputWishTip") + "\n" + NDCommonCString("ModifyWishTip"));
					//	view->SetEdit(1, vec_id, vec_str);
					//	view->SetEditMaxLength(40, 0);
					//	view->SetTag(eCVOP_Wish);
					//	view->Show();
					//	NDScene* scene = NDDirector::DefaultDirector()->GetRunningScene();
					//	if (scene)
					//	{
					//		scene->AddChild(view);
					//	}
					//	CloseProgressBar;
					//}
					//	break;
					case 23://�򿪼��ܺϳɽ���
					{
						CloseProgressBar;
						
						//Item* petItem = ItemMgrObj.GetEquipItemByPos(Item::eEP_Pet);
						//						if (petItem) 
						//						{
						//							NDDirector::DefaultDirector()->PushScene(PetSkillCompose::Scene());
						//						}
						//						else 
						//						{
						//							GlobalShowDlg(NDCommonCString("WenXinTip"),NDCommonCString("EquipPet"));
						//						}
					}
						break;
					case 24:// ��ֲ
					{
						CloseProgressBar;
						showUseItemUI(npcID, 0);
						//int iNPCID = player.m_npcFocus->m_id;
						//SelectBagScene *scene = new SelectBagScene;
						//						scene->Initialization(0, npcID);
						//						NDDirector::DefaultDirector()->PushScene(scene);
						//SelectBag bag = new SelectBag((byte) 0);
						//						bag.npcId = npcId;
						//						T.addToTop(bag);
					}
						break;
					case 25:// ����
					{
						CloseProgressBar;
						showUseItemUI(npcID, 1);
						//int iNPCID = player.m_npcFocus->m_id;
						//SelectBagScene *scene = new SelectBagScene;
						//						scene->Initialization(1, npcID);
						//						NDDirector::DefaultDirector()->PushScene(scene);
						//bag = new SelectBag((byte) 1);
						//						bag.npcId = npcId;
						//						T.addToTop(bag);
					}
						break;
					case 26:// ʩ��
					{
						CloseProgressBar;
						showUseItemUI(npcID, 2);
						//int iNPCID = player.m_npcFocus->m_id;
						//SelectBagScene *scene = new SelectBagScene;
						//						scene->Initialization(2, npcID);
						//						NDDirector::DefaultDirector()->PushScene(scene);
						//bag = new SelectBag((byte) 2);
						//						bag.npcId = npcId;
						//						T.addToTop(bag);
					}
						break;
					case 27:// ι����
					{
						CloseProgressBar;
						showUseItemUI(npcID, 3);
						//int iNPCID = player.m_npcFocus->m_id;
						//SelectBagScene *scene = new SelectBagScene;
						//						scene->Initialization(3, npcID);
						//						NDDirector::DefaultDirector()->PushScene(scene);
						//bag = new SelectBag((byte) 3);
						//						bag.npcId = npcId;
						//						T.addToTop(bag);
					}
						break;
					case 28://ׯ԰����
					{
						CloseProgressBar;
						ShowView(this, NDCommonCString("FarmRename"), eCVOP_FarmName, 15);
						//T.getDisplay().setCurrent(
						//												  new InputForm("ׯ԰����", 0, InputForm.INPUT_NAME_OF_FARM));
					}
						break;
					case 29://ׯ԰��ӭ��
					{
						CloseProgressBar;
						ShowView(this, NDCommonCString("ModifyWelcome"), eCVOP_FarmWelcomeName, 64);
						//T.getDisplay().setCurrent(
						//												  new InputForm("�޸Ļ�ӭ��", 0, InputForm.INPUT_FARM_WELCOME));
					}
						break;
					case 30://��������
					{
						CloseProgressBar;
						showUseItemUI(npcID, 4);
					}
						break;
					case 31://��������
					{
						CloseProgressBar;
						ShowView(this, NDCommonCString("ModifyBuildingName"), eCVOP_FarmBuildingName, 15);
						//InputForm inf=new InputForm("�޸Ľ�������", 0, InputForm.INPUT_NAME_OF_BUILD);
						//						inf.setParam(npcId, 0);
						//						T.getDisplay().setCurrent(inf);
						
						m_iCurDlgNpcID = npcID;
					}
						break;
					case 32://�������
					{
						CloseProgressBar;
						ShowView(this, NDCommonCString("ModifyCunluoName"), eCVOP_FarmHarmletName, 15);
						//inf=new InputForm("�޸Ĵ�������", 0, InputForm.INPUT_NAME_OF_HAMLET);
						//						inf.setParam(npcId, 0);
						//						T.getDisplay().setCurrent(inf);
						
						m_iCurDlgNpcID = npcID;
					}
						break;
					case 33://װ������
					{
						CloseProgressBar;
						EquipUpgradeScene *scene = new EquipUpgradeScene;
						scene->Initialization(EQUIP_UPLEVEL);
						NDDirector::DefaultDirector()->PushScene(scene);
					}
						break;
						
						
				}
				break;
			}
			case MSGDIALOG_CREATE: { // npc�Ի� ���β
				m_iCurDlgNpcID = npcID;
				NDScene *scene = NDDirector::DefaultDirector()->GetRunningScene();
				if (scene->IsKindOfClass(RUNTIME_CLASS(GameScene)))
				{
					GameScene *gamescene = (GameScene*)scene;
					gamescene->ShowNPCDialog();
				}
				CloseProgressBar;
				/*Dialog dialog = new Dialog();
				 if (usData > 0) { // ����ʱ�ر�
				 dialog.setAutoCloseSecond(usData);
				 }
				 if (strTitle != null) {
				 dialog.setContent(strTitle, ChatRecord.parserChat(T.changeNpcString(strBuffer.toString()), -1));
				 strTitle = null;
				 } else {
				 if (T.curNpc != null) {
				 dialog.setContent(T.curNpc.getName(), ChatRecord.parserChat(T.changeNpcString(strBuffer.toString()), -1));
				 } else {
				 dialog.setContent(null, ChatRecord.parserChat(T.changeNpcString(strBuffer.toString()), -1));
				 }
				 }
				 strBuffer = null;
				 
				 if (!npc_options.isEmpty()) {
				 
				 TextView leave = new TextView((strLeaveMsg == null ? strLeave : strLeaveMsg), 4);
				 leave.id = (99);
				 //leave.parama = dialog;
				 npc_options.addElement(leave);
				 
				 TextView[] arr = new TextView[npc_options.size()];
				 for (int i = 0; i < arr.length; i++) {
				 arr[i] = (TextView) npc_options.elementAt(i);
				 String textString = arr[i].getText();
				 if (textString.startsWith("!") || textString.startsWith("��")) {
				 StringBuffer sb = new StringBuffer("������");
				 sb.append(textString.substring(1));
				 textString = sb.toString();
				 arr[i].setText(textString);
				 }
				 if (arr[i].id != 99) {
				 arr[i].id = (100);
				 }
				 arr[i].setOnClickListener(this);
				 }
				 npc_options.removeAllElements();
				 dialog.setOperator(arr);
				 }
				 
				 strLeaveMsg = null;
				 if(dialog.operator!=null&&!T.isDialogListEmpty()){
				 Array dialogList=T.getDialogList();
				 int size=dialogList.size();
				 for(int i=0;i<size;i++){
				 Dialog d=(Dialog)dialogList.elementAt(i);
				 if(d instanceof Dialog){
				 d.close();
				 }
				 }
				 }
				 T.addDialog(dialog);*/
				break;
			}
			case MSGDIALOG_MAGIC_EFFECT: { // todo ��ʾħ����Ч
				/*if (!str.startsWith("effect")) {
				 str = "effect_" + str;
				 }
				 AniGroup effect = AnimationShop.getInstance().getAniGroup(str + ".spr");
				 SubAniGroup sag = new SubAniGroup();
				 sag.aniGroup = effect;
				 ManualRole role = GameScreen.role;
				 sag.x = (short) role.x;
				 sag.y = (short) role.y;
				 sag.coordW = 0;
				 sag.coordH = (short) -role.getAniGroup().getHeight();
				 sag.sceneElement = role;
				 GameScreen.getInstance().getScene().addSubAniGroup(sag);*/
				break;
			}
			case MSGDIALOG_LINK_EX: {// ѡ�����ʾ��ͷ��
				if (usData == TEXT_TEXT) {
					str = data.ReadUnicodeString();
					str = changeToChineseSign(str);
					st_npc_op op;
					op.idx = iDx;
					op.str = str;
					op.bArrow = true;
					vecNPCOPText.push_back(op);
				}
				break;
			}
			case MSGDIALOG_CREATE_EX: { // ���������뿪��ť��
				{
					m_iCurDlgNpcID = npcID;
					NDScene *scene = NDDirector::DefaultDirector()->GetRunningScene();
					if (scene->IsKindOfClass(RUNTIME_CLASS(GameScene)))
					{
						GameScene *gamescene = (GameScene*)scene;
						gamescene->ShowNPCDialog(false);
					}
					CloseProgressBar;
				}
				break;
			}
		}
	}
	
	std::string NDMapMgr::changeNpcString(std::string str)
	{
		if (str.empty())
		{
			return "";
		}
		
		NDString ndstrtmp(str);
		
		ndstrtmp.replace(NDString("&n"), NDString(NDPlayer::defaultHero().m_name));
		switch (NDPlayer::defaultHero().sex)
		{
			case SpriteSexMale:
			{
				ndstrtmp.replace(NDString("&1"), NDString(NDCommonCString("XiaoMei")));
				ndstrtmp.replace(NDString("&2"), NDString(NDCommonCString("DaJie")));
				ndstrtmp.replace(NDString("&3"), NDString(NDCommonCString("NvXia")));
				break;
			}
			case SpriteSexFemale:
			default: {
				ndstrtmp.replace(NDString("&1"), NDString(NDCommonCString("XiaoXiongDi")));
				ndstrtmp.replace(NDString("&2"), NDString(NDCommonCString("DaGG")));
				ndstrtmp.replace(NDString("&3"), NDString(NDCommonCString("ShaoXia")));
				break;
			}
		}
		return changeToChineseSign(std::string(ndstrtmp.getData()));
	}
	
	EmailData* NDMapMgr::GetMail(int iMailID)
	{
		EmailData* res = NULL;
		for_vec(m_vEmail, vec_email_it)
		{
			if ((*it)->getId() == iMailID) {
				res = *it;
				break;
			}
		}
		return res;
	}
	
	NDNpc* NDMapMgr::GetNpcByID(int idNpc)
	{
		NDNpc* npc = NULL;
		for (vec_npc_it it = m_vNpc.begin(); it != m_vNpc.end(); it++) {
			npc = *it;
			if (npc->m_id == idNpc) {
				return npc;
			}
		}
		return NULL;
	}
	
	int NDMapMgr::GetDlgNpcID()
	{
		return m_iCurDlgNpcID;
	}
	
	void sendPKAction(NDManualRole& role, int pkType)
	{
		NDPlayer& player = NDPlayer::defaultHero();
		if (role.IsInState(USERSTATE_DEAD))
		{
			std::stringstream ss; ss << role.m_name << NDCommonCString("DieCantPk");
			Chat::DefaultChat()->AddMessage(ChatTypeSystem, ss.str().c_str());
			//GameScreen.getInstance().initNewChat(role.getName() + "��������δ������ܽ��б������PK", 0x0000ff, 3000);
			return;
		}
		if (role.level < 10) {
			std::stringstream ss; ss << role.m_name << NDCommonCString("LvlWei") << role.level << "��" << (pkType == BATTLE_ACT_PK ? NDCommonCString("LowTenCantPk") : NDCommonCString("LowTenCantRefrase"));
			Chat::DefaultChat()->AddMessage(ChatTypeSystem, ss.str().c_str());
			//GameScreen.getInstance().initNewChat(role.getName() + "�ĵȼ�Ϊ" + role.getLevel() + "��" + (pkType == Battle.BATTLE_ACT_PK ? "�ȼ�����10���ܽ���PK" : "�ȼ�����10���ܽ��б���"), 0x0000ff, 3000);
			return;
		}
		if (player.level < 10) {
			std::stringstream ss; ss << player.m_name << NDCommonCString("LvlWei") << player.level << "��" << (pkType == BATTLE_ACT_PK ? NDCommonCString("LowTenCantPk") : NDCommonCString("LowTenCantRefrase"));
			Chat::DefaultChat()->AddMessage(ChatTypeSystem, ss.str().c_str());
			//GameScreen.getInstance().initNewChat(GameScreen.role.getName() + "�ĵȼ�Ϊ" + GameScreen.role.getLevel() + "��" + (pkType == Battle.BATTLE_ACT_PK ? "�ȼ�����10���ܽ���PK" : "�ȼ�����10���ܽ��б���"), 0x0000ff,
			//												 3000);
			return;
		}
		if (role.IsSafeProtected()) {
			std::stringstream ss; ss << role.m_name << NDCommonCString("ProtecedCantPkOrRefrase");
			Chat::DefaultChat()->AddMessage(ChatTypeSystem, ss.str().c_str());
			//GameScreen.getInstance().initNewChat(role.getName() + "���ڱ���״̬�����ܽ��б������PK", 0x0000ff, 3000);
			return;
		}
		if (role.IsInState(USERSTATE_BOOTH)) {
			std::stringstream ss; ss << role.m_name << NDCommonCString("BoothCantPkOrRefrase");
			Chat::DefaultChat()->AddMessage(ChatTypeSystem, ss.str().c_str());
			//GameScreen.getInstance().initNewChat(role.getName() + "���ڰ�̯״̬�����ܽ��б������PK", 0x0000ff, 3000);
			return;
		}
		if (role.IsInState(USERSTATE_PVE)) {
			std::stringstream ss; ss << role.m_name << NDCommonCString("DePkCantPkOrRefrase");
			Chat::DefaultChat()->AddMessage(ChatTypeSystem, ss.str().c_str());
			//GameScreen.getInstance().initNewChat(role.getName() + "������PK״̬�����ܽ��б������PK", 0x0000ff, 3000);
			return;
		}
		
		if (player.isTeamMember() && role.isTeamMember() && player.teamId == role.teamId)
		{
			Chat::DefaultChat()->AddMessage(ChatTypeSystem, NDCommonCString("TeamCantPkOrRefrase"));
			//GameScreen.getInstance().initNewChat("�����Ƕ��ѣ����ܽ��б������PK", 0x0000ff, 3000);
			return;
		}
		
		NDTransData bao(_MSG_BATTLEACT);
		bao << (unsigned char)pkType << (unsigned char)0 << (unsigned char)1 << int(role.m_id);
		// SEND_DATA(bao);
	}
	
	void trade(int n, int action)
	{
		NDTransData bao(_MSG_TRADE);
		bao << n;// ���׶���ID ��ƷID �������� 4���ֽ�
		bao << (unsigned char)action;
		// SEND_DATA(bao);
	}
	
	bool NDMapMgr::isFriendAdded(string& name)
	{
		//for (MAP_FRIEND_ELEMENT_IT it = this->m_mapFriend.begin(); it != m_mapFriend.end(); it++) {
		//	FriendElement& fe = it->second;
		//	if (fe.m_text1 == name) {
		//		return true;
		//	}
		//}
		return false;
	}
	
	void sendAddFriend(std::string& name)
	{
		NDMapMgr& mgr = NDMapMgrObj;
		//if (mgr.isFriendMax()) {
		//			GlobalShowDlg("������������", "���ĺ��������Ѵﵽ���ޣ��޷�����Ӻ��ѡ�");
		//		} else 
		{
			if (mgr.isFriendAdded(name)) {
				GlobalShowDlg(NDCommonCString("AddFail"), NDCommonCString("NoToFriendTip"));
			} else if (name == NDPlayer::defaultHero().m_name) {
				GlobalShowDlg(NDCommonCString("AddFail"), NDCommonCString("SelfToFriendTip"));
			} else {
				NDTransData bao(_MSG_GOODFRIEND);
				bao << (Byte)_FRIEND_APPLY
				<< (Byte)1;
				bao.WriteUnicodeString(name);
				// SEND_DATA(bao);
			}
		}
	}
	
	void sendRehearseAction(int idTarget, int btAction)
	{
		NDManualRole *role = NDMapMgrObj.GetManualRole(idTarget);
		if (!role)
		{
			return;
		}
		
		NDPlayer& player = NDPlayer::defaultHero();
		if (player.isTeamMember() && role->isTeamMember() && player.teamId == role->teamId)
		{
			//GameScreen.getInstance().initNewChat("�����Ƕ��ѣ����ܽ��б������PK", 0x0000ff, 3000);
			return;
		}
		
		NDTransData bao(_MSG_REHEARSE);
		bao << (unsigned char)btAction << int(idTarget);
		// SEND_DATA(bao);
	}
	
	void sendQueryPlayer(int roleId, int btAction) 
	{
		ShowProgressBar;
		NDTransData bao(_MSG_SEE);
		bao << (unsigned char)btAction << int(roleId);
		// SEND_DATA(bao);
	}
	
	void sendTeamAction(int senderID, int action)
	{
		NDTransData bao(_MSG_TEAM);
		bao << (unsigned short)action << int(NDPlayer::defaultHero().m_id) << int(senderID);
		// SEND_DATA(bao);
	}
	
	void sendChargeInfo(int iID) 
	{
		NDTransData bao(MB_MSG_RECHARGE);
		bao << int(iID);
		bao.WriteUnicodeString(loadPackInfo(STRPARAM));
		// SEND_DATA(bao);
		
		ShowProgressBar;
	}
	
	void sendMarry(int roleId, int playerId, int usType, int usData)
	{
		NDTransData bao(_MSG_MARRIAGE);
		bao << roleId 
		<< playerId 
		<< (unsigned short)usType 
		<< (unsigned short)usData;
		// SEND_DATA(bao);
		
		//ShowProgressBar;
	}
	
	void NDMapMgr::processMarriage(NDTransData& data) 
	{
		int tempId1 = data.ReadInt();
		int tempId2 = data.ReadInt();
		int usType = data.ReadShort();
		int marriage = 0;
		NDManualRole *tar = NULL;
		NDPlayer& role = NDPlayer::defaultHero();
		if (tempId1 == role.m_id) {
			tar = GetManualRole(tempId2);
			marriage = tempId2;
		} else if (tempId2 == role.m_id) {
			tar = GetManualRole(tempId1);
			marriage = tempId1;
		}
		
		if (NULL != tar) {// ���ǽ��յ����ɹ��Ĺ㲥��Ϣ��ô�˴���tar���󽫻�Ϊnull.�������ַ���,����ID�Ͳ��ᱻ�㲥����
			id1 = tempId1;
			id2 = tempId2;
		}
		
		if (usType == _MARRIAGE_QUARY) {
			role.marriageID = marriage;
			role.loverName = data.ReadUnicodeString();
		} else if (usType == _DIVORCE_APPLY) {
			if (tar != NULL) {
				m_idDialogDemarry = GlobalDialogObj.Show(this, NDCommonCString("divorce"),
														 (tar->m_name += NDCommonCString("DivorceReq")).c_str(), NULL,
														 NDCommonCString("divorce"), NDCommonCString("disagree"), NULL);
				
				
				//dialog = new Dialog();
				//				dialog.setContent("���", tar.getName() + "�����������,���Ƿ�ͬ��?");
				//				
				//				TextView ops[] = new TextView[2];
				//				ops[0] = new TextView("���", 2);
				//				ops[0].id = (120);
				//				ops[0].setOnClickListener(this);// ͬ����
				//				ops[1] = new TextView("��ͬ��", 2);
				//				ops[1].id = (121);
				//				ops[1].setOnClickListener(this); // �ܾ����
				//				
				//				dialog.setOperator(ops);
				//				showDialog(dialog);
			}
		} else if (usType == _MARRIAGE_APPLY) {
			if (tar != NULL) {
				m_idDialogMarry = GlobalDialogObj.Show(this, NDCommonCString("MarryReq"),
													   (tar->m_name + NDCommonCString("MarryReqTip")).c_str(), NULL,
													   NDCommonCString("agree"), NDCommonCString("reject"), NULL);
				/*
				 dialog = new Dialog();
				 dialog.setContent(
				 "���",
				 tar.getName()
				 + "��ϥ����������ס����֣��ó���ʶ�Ŀ�Ļ�䣬������װ��Ĵ�����ö�����䣬���ҵİ�һֱΧ�����㣬�ǻ��ţ���Զ��Զ��");
				 
				 TextView ops[] = new TextView[2];
				 ops[0] = new TextView("ͬ��", 2);
				 ops[0].id = (125);
				 ops[0].setOnClickListener(this);
				 ops[1] = new TextView("�ܾ�", 2);
				 ops[1].id = (126);
				 ops[1].setOnClickListener(this);
				 
				 dialog.setOperator(ops);
				 showDialog(dialog);
				 */
			}
		} else if (usType == _MARRIAGE_AGREE) {
			if (tar != NULL) {
				role.marriageID = tar->m_id;
				role.loverName = tar->m_name;
				role.marriageState = _MARRIAGE_MATE_ONLINE;
			}
			role.playerMarry();
			//role.setMarrySuccess(true);// �㲥�����Ч�ÿ��ӷ�Χ�ڵ���Ҷ�������Ч
		} else if (usType == _DIVORCE_AGREE) {
			if (tempId1 == role.m_id || tempId2 == role.m_id) {
				role.marriageID = 0;
				role.loverName = "";
				role.marriageState = _MARRIAGE_MATE_LOGOUT;
			}
		} else if (usType == _MARRIAGE_MATE_LOGIN) { // ��ż����
			role.marriageState = 1;
		} else if (usType == _MARRIAGE_MATE_ONLINE) { // ��ż����
			role.marriageState = 2;
		} else if (usType == _MARRIAGE_MATE_LOGOUT) { // ��ż����
			role.marriageState = 0;
		}
	}
	
	void NDMapMgr::processRespondTreasureHuntProb(NDTransData& data)
	{
		CloseProgressBar;
		
		int huntLost = 0, equipAdd = 0, druation = 0;
		
		huntLost = data.ReadByte();
		equipAdd = data.ReadByte();
		druation = data.ReadInt();
		
		TreasureHuntScene *scene = TreasureHuntScene::Scene();
		scene->SetRateInfo(huntLost, equipAdd, druation);
		NDDirector::DefaultDirector()->PushScene(scene);
	}
	
	void NDMapMgr::processShowTreasureHuntAward(NDTransData& data)
	{
		std::string strText = data.ReadUnicodeString();
		GlobalShowDlg(NDCommonCString("XunBao"), strText.c_str());
	}
	
	void NDMapMgr::processRespondTreasureHuntInfo(NDTransData& data)
	{
		CloseProgressBar;
		TreasureHuntScene::processHuntDesc(data);
	}
	
	void NDMapMgr::processKickOutTip(NDTransData& data)
	{
		CloseProgressBar;
		std::string tip = data.ReadUnicodeString();
		/*
		 GameQuitDialog *dlg = new GameQuitDialog;
		 dlg->Initialization(5.0f);
		 dlg->Show(NDCommonCString("tip"), tip.c_str(), NULL, NULL, NULL);
		 */
		GameQuitDialog::DefaultShow(NDCommonCString("tip"), tip.c_str(), 5.0f, true);
	}
	
	void NDMapMgr::processQueryPetSkill(NDTransData& data)
	{
		CUIPet* pUIPet	= PlayerInfoScene::QueryPetScene();
		if (pUIPet) {
			OBJID idItem	= data.ReadInt();
			std::string	str	= data.ReadUnicodeString();
			pUIPet->UpdateSkillItemDesc(idItem, str);
		}
	}
	
	bool NDMapMgr::DealBugReport()
	{
		if (NDDataTransThread::DefaultThread()->GetThreadStatus() != ThreadStatusRunning)
			return false;
		
		if ([ [NDCrashUpload Shared] HadDataTransBefore])
		{
			std::stringstream ss;
			ss << "Iphone" << ";";
			ss << "accout:" << NDBeforeGameMgrObj.GetUserName() << ";";
			ss << "platform:" << platformString() << ";";
			ss << "software version:" << GetSoftVersion() << ";";
			ss << "ios version" << GetIosVersion() << ";";
			
			NDLog("bug report\"%@\"",[NSString stringWithUTF8String:ss.str().c_str()]);
			
			NDTransData bao(_MSG_CLIENT_BUG_REPORT);
			bao << (unsigned char)3; //1����ʼ,2������,3����,0�м�İ�
			bao << int(0); // uid ��0����
			bao << ::time(NULL);
			bao.WriteUnicodeString(ss.str());
			// SEND_DATA(bao);
			
			[[NDCrashUpload Shared] ResetDataTransBefore];
			
			Chat::DefaultChat()->AddMessage(ChatTypeTip, NDCommonCString("SucessBugReportTip"));
			
			return true;
		}
		
		return false;
	}
	
	bool NDMapMgr::DealCrashRepotUploadFail()
	{
		Chat::DefaultChat()->AddMessage(ChatTypeTip, NDCommonCString("FailBugReportTip"));
		
		return true;
	}
	
	void NDMapMgr::SetBattleMonster(NDMonster* monster)
	{
		if (!monster)
		{
			waitbatteleMonster.Clear();
		}
		else
		{
			waitbatteleMonster = monster->QueryLink();
		}
	}
	
	NDMonster* NDMapMgr::GetBattleMonster()
	{
		return waitbatteleMonster;
	}
}
