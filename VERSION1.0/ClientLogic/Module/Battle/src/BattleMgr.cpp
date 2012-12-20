/*
 *  BattleMgr.mm
 *  DragonDrive
 *
 *  Created by wq on 11-1-17.
 *  Copyright 2011 (����)DeNA. All rights reserved.
 *
 */

#include "BattleMgr.h"
#include "NDPlayer.h"
#include "NDTransData.h"
#include "Battle.h"
#include "NDUISynLayer.h"

#include "NDMapMgr.h"
#include "NDDirector.h"
#include "BattleUtil.h"
#include <sstream>
#include "ItemMgr.h"
//#include "LearnSkillUILayer.h"
//#include "DianhuaUILayer.h"
#include "GameScene.h"
//#include "PetSkillCompose.h"
#include "CPet.h"
//#include "PlayerInfoScene.h"
#include "ScriptDataBase.h"
#include "ScriptGlobalEvent.h"
#include "DramaScene.h"
#include "NDConstant.h"
#include "ScriptMgr.h"
#include "ObjectTracker.h"

#define QUIT_BATTLE_TIMER_TAG (13621)
#define QUIT_DRAMA_TIMER_TAG (13622)

using std::stringstream;

using namespace NDEngine;

BattleMgr::BattleMgr() :
m_pkBattle(NULL)
{
	INC_NDOBJ("BattleMgr");

	NDNetMsgPool* kPool = NDNetMsgPoolObj;
	kPool->RegMsg(_MSG_BATTLE, this);
	kPool->RegMsg(_MSG_CONTROLPOINT, this);
	kPool->RegMsg(_MSG_EFFECT, this);
	kPool->RegMsg(_MSG_BATTLEEND, this);
	kPool->RegMsg(_MSG_SKILLINFO, this);
	kPool->RegMsg(_MSG_BATTLE_SKILL_LIST, this);
	kPool->RegMsg(_MSG_PLAYER_RECON, this);
	kPool->RegMsg(_MSG_PLAYER_EXT_RECON, this);
	//	m_Db=new ScriptDB();
	m_pkQuitTimer = NULL;
	m_pkBattleReward = NULL;
	m_pkPrebattleReward = NULL;
	m_nBattleMapId = 0;
	m_nBattleX = 0;
	m_nBattleY = 0;
	m_nLastBattleTeamCount = 0;
	m_nLastSceneMapDocID	= 0;
	m_nLastSceneScreenX		= 0;
	m_nLastSceneScreenY		= 0;
	//
	m_pkStartDramaTimer = NULL;
}

BattleMgr::~BattleMgr()
{
	DEC_NDOBJ("BattleMgr");

	ReleaseAllBattleSkill();
	if (m_pkBattleReward)
	{
		CC_SAFE_DELETE (m_pkBattleReward);
	}
	if(m_pkPrebattleReward)
	{
		CC_SAFE_DELETE (m_pkPrebattleReward);
	}

	ReleaseActionList();
}

void BattleMgr::RestoreActionList()
{
	VEC_FIGHTACTION_IT it = m_vActionList1.begin();

	for (; it != m_vActionList1.end(); it++)
	{
		//CC_SAFE_DELETE((*it)->skill);
		FightAction* action = (*it);
		if(action)
		{
			action->m_eActionStatus = ACTION_STATUS_WAIT;
		}
	}

	VEC_FIGHTACTION_IT it2 = m_vActionList2.begin();

	for (; it2 != m_vActionList2.end(); it2++)
	{
		//CC_SAFE_DELETE((*it)->skill);
		FightAction* action = (*it);
		if(action)
		{
			action->m_eActionStatus = ACTION_STATUS_WAIT;
		}

	}

	VEC_FIGHTACTION_IT it3 = m_vActionList3.begin();

	for (; it3 != m_vActionList3.end(); it3++)
	{
		//CC_SAFE_DELETE((*it)->skill);
		FightAction* action = (*it);
		if(action)
		{
			action->m_eActionStatus = ACTION_STATUS_WAIT;
		}

	}

}

void BattleMgr::ReleaseActionList()
{
	VEC_FIGHTACTION_IT it = m_vActionList1.begin();

	for (; it != m_vActionList1.end(); it++)
	{
		//CC_SAFE_DELETE((*it)->skill);
		VEC_FIGHTERCOMMAND_IT fm_it = (*it)->m_vCmdList.begin();
		CC_SAFE_DELETE((*it)->m_pkSkill);
		for (; fm_it != (*it)->m_vCmdList.end(); fm_it++)
		{
			CC_SAFE_DELETE(*fm_it);
		}
		(*it)->m_vCmdList.clear();
		CC_SAFE_DELETE(*it);

	}

	m_vActionList1.clear();

	VEC_FIGHTACTION_IT it2 = m_vActionList2.begin();

	for (; it2 != m_vActionList2.end(); it2++)
	{
		//CC_SAFE_DELETE((*it)->skill);
		VEC_FIGHTERCOMMAND_IT fm_it = (*it2)->m_vCmdList.begin();
		for (; fm_it != (*it2)->m_vCmdList.end(); fm_it++)
		{
			CC_SAFE_DELETE(*fm_it);
		}
		(*it2)->m_vCmdList.clear();
		CC_SAFE_DELETE(*it2);

	}

	m_vActionList2.clear();

	VEC_FIGHTACTION_IT it3 = m_vActionList3.begin();

	for (; it3 != m_vActionList3.end(); it3++)
	{
		//CC_SAFE_DELETE((*it)->skill);
		VEC_FIGHTERCOMMAND_IT fm_it = (*it)->m_vCmdList.begin();
		for (; fm_it != (*it3)->m_vCmdList.end(); fm_it++)
		{
			CC_SAFE_DELETE(*fm_it);
		}
		(*it3)->m_vCmdList.clear();
		CC_SAFE_DELETE(*it3);

	}

	m_vActionList3.clear();
}

BattleSkill* BattleMgr::GetBattleSkill(OBJID idSkill)
{
	MAP_BATTLE_SKILL_IT it = m_mapBattleSkill.find(idSkill);
	return it == m_mapBattleSkill.end() ? NULL : it->second;
}

void BattleMgr::ReleaseAllBattleSkill()
{
	for (MAP_BATTLE_SKILL_IT it = m_mapBattleSkill.begin();
			it != m_mapBattleSkill.end(); it++)
	{
		CC_SAFE_DELETE(it->second);
	}
	m_mapBattleSkill.clear();
}

void BattleMgr::OnTimer(OBJID tag)
{
	if (tag == QUIT_BATTLE_TIMER_TAG)
	{
		if (m_pkBattle && m_pkBattle->IsEraseInOutEffectComplete())
		{
			quitBattle(false);
		}
	}
	else if(tag == QUIT_DRAMA_TIMER_TAG)
	{
		//������ʱս����������ؽ���
		if ( NDDirector::DefaultDirector()->GetRunningScene()->IsKindOfClass(RUNTIME_CLASS(DramaScene)))
		{
			return;
		}
		if ( NDDirector::DefaultDirector()->GetRunningScene()->IsKindOfClass(RUNTIME_CLASS(DramaTransitionScene)))
		{
			return;
		}

		m_pkStartDramaTimer->KillTimer(this, QUIT_DRAMA_TIMER_TAG);
		loadRewardUI();

	}
}

void BattleMgr::loadRewardUI()
{
	ScriptMgrObj.excuteLuaFunc("LoadUI", "MonsterRewardUI",0);
	//ScriptMgrObj.excuteLuaFunc("SetRewardExp", "MonsterRewardUI",battleReward->exp);
	ScriptMgrObj.excuteLuaFunc("addSophMoney", "MonsterRewardUI", m_pkBattleReward->m_nSoph, m_pkBattleReward->m_nMoney);	

	for(int i = 0; i < 5; i++)
	{
		if(m_pkBattleReward->m_nItemTypes[i] != 0)
		{
			NDLog(@"addRewardItem:%d",m_pkBattleReward->m_nItemTypes[i]);
			ScriptMgrObj.excuteLuaFunc("addRewardItem", "MonsterRewardUI",i+1,m_pkBattleReward->m_nItemTypes[i],m_pkBattleReward->m_nItemAmount[i]);

		}
	}

	for(int i = 0; i < 5; i++)
	{
		if(m_pkBattleReward->m_nPetId[i] !=0)
		{
			NDLog(@"iPetId :%d;  iPetGainExp : %d", m_pkBattleReward->m_nItemTypes[i], m_pkBattleReward->m_nPetGainExp[i]);
			ScriptMgrObj.excuteLuaFunc("SetRewardExp", "MonsterRewardUI", m_pkBattleReward->m_nPetId[i], m_pkBattleReward->m_nPetGainExp[i]);
		}
	}

	ScriptMgrObj.excuteLuaFunc("Refresh", "MonsterRewardUI");	

	//���ҳ����ʾ֮���ͷŽ�����Ʒ
	if (m_pkBattleReward != NULL)
	{
		SAFE_DELETE (m_pkBattleReward);
	}      
}

bool BattleMgr::process(MSGID msgID, NDEngine::NDTransData* bao, int len)
{
	switch (msgID)
	{
		case _MSG_BATTLE:
			processBattleStart(*bao);
			break;
		case _MSG_CONTROLPOINT:
			processControlPoint(*bao);
			break;
		case _MSG_EFFECT:
			processBattleEffect(*bao);
			break;
		case _MSG_BATTLEEND:
			processBattleEnd(*bao);
			break;
		case _MSG_SKILLINFO:
			processSkillInfo(*bao, len);
			break;
		case _MSG_BATTLE_SKILL_LIST:
			processBattleSkillList(*bao, len);
			break;
		case _MSG_PLAYER_RECON:
		{
			NDMapMgrObj.processPlayer(bao, len);
		}
			break;
		case _MSG_PLAYER_EXT_RECON:
		{
			NDMapMgrObj.processPlayerExt(bao, len);
		}
			break;
		default:
			break;
	}
	return true;
}

void BattleMgr::processBattleSkillList(NDTransData& data, int len)
{
	if (m_pkBattle)
	{
		m_pkBattle->processBattleSkillList(data, len);
	}
}

void BattleMgr::processSkillInfo(NDTransData& data, int len)
{
	int btAction = data.ReadByte();
	int btCnt = data.ReadByte();
	int idPet = data.ReadInt();
	for (int i = 0; i < btCnt; i++)
	{
		if (btAction <= 2)
		{ // ��һ���＼��
			int idSkill = data.ReadInt();
			int statusLast = data.ReadByte();
			int atkType = data.ReadInt();
			int area = data.ReadShort();
			int injury = data.ReadInt();
			int speed = data.ReadByte();
			int mpRequire = data.ReadInt();
			int atk_point = data.ReadByte();
			int def_point = data.ReadByte();
			int dis_point = data.ReadByte();
			int lvRequire = data.ReadInt();
			int money = data.ReadInt();
			int spRequire = data.ReadShort();
			int curSp = data.ReadShort();
			int iconIndex = data.ReadInt();
			Byte depFlag = data.ReadByte();
			Byte btCD = data.ReadByte();
			Byte btSlot = data.ReadByte();
			string name = data.ReadUnicodeString();
			string des = data.ReadUnicodeString();
			string update_required;
			if (depFlag == 1)
			{
				update_required = data.ReadUnicodeString();
			}
			BattleSkill* pkSkill = GetBattleSkill(idSkill);
			NDPlayer& kPlayer = NDPlayer::defaultHero();
			// ������Ϣ�Ѵ���
			if (pkSkill)
			{
				if (btAction == 0)
				{ // ��Ҽ���
					kPlayer.AddSkill(idSkill);
					//GameScreen.role.addSkill(skill);
				}
				else if (btAction == 1)
				{
					//NDBattlePet* pet = NDPlayer::defaultHero().battlepet;
					//					if (pet) {
					//						pet->AddSkill(idSkill);
					//					}
					PetMgrObj.AddSkill(kPlayer.m_nID, idPet, idSkill);

					//CUIPet *uiPet = PlayerInfoScene::QueryPetScene();
					//
					//if (uiPet)
					//{
					//	uiPet->UpdateUI(idPet);
					//}
				}
				continue;
			}

			pkSkill = new BattleSkill;
			pkSkill->setId(idSkill);
			pkSkill->setStatusLast(statusLast);
			pkSkill->setAtkType(SKILL_ATK_TYPE(atkType));
			pkSkill->setArea(area);
			pkSkill->setInjury(injury);
			pkSkill->setSpeed(speed);
			pkSkill->setMpRequire(mpRequire);
			pkSkill->setAtk_point(atk_point);
			pkSkill->setDef_point(def_point);
			pkSkill->setDis_point(dis_point);
			pkSkill->setLvRequire(lvRequire);
			pkSkill->setMoney(money);
			pkSkill->setSpRequire(spRequire);
			pkSkill->setName(name);
			pkSkill->setDes(des);
			pkSkill->setCurSp(curSp);
			pkSkill->setUpdate_required(update_required);
			pkSkill->setIconIndex(iconIndex);
			pkSkill->setCd(btCD);
			pkSkill->SetSlot(btSlot);

			m_mapBattleSkill[idSkill] = pkSkill;

			pkSkill->SetSkillOwn(btAction == 0);

			if (btAction == 0)
			{ // ��Ҽ���
				kPlayer.AddSkill(idSkill);
				//GameScreen.role.addSkill(skill);
			}
			else if (btAction == 1)
			{
				//NDBattlePet* pet = NDPlayer::defaultHero().battlepet;
				//				if (pet) {
				//					pet->AddSkill(idSkill);
				//				}
				PetMgrObj.AddSkill(kPlayer.m_nID, idPet, idSkill);
			}
		}
		else if (btAction == 3 || btAction == 4)
		{
			int idNextLevelSkill = data.ReadInt();
			int lqz = data.ReadShort();
			//DianhuaUILayer::RefreshList(btAction, idNextLevelSkill, lqz);
		}
		else if (btAction == 5)
		{ // ���¼���������
			int idSk = data.ReadInt();
			int curSp = data.ReadShort();
			BattleSkill* sk = GetBattleSkill(idSk);
			if (sk)
			{
				sk->setCurSp(curSp);
			}
		}
		else if (btAction == 6)
		{ // ���¼��ܲ�
			int idSk = data.ReadInt();
			int btSlot = data.ReadShort();
			BattleSkill* sk = GetBattleSkill(idSk);
			if (sk)
			{
				sk->SetSlot(btSlot);
			}
		}
	}

	//LearnSkillUILayer::RefreshSkillList();
	//PetSkillCompose::refresh();
}

void BattleMgr::closeUI()
{
	NDDirector* director = NDDirector::DefaultDirector();
	NDScene* scene = director->GetRunningScene();
	while (scene)
	{
		if (scene->IsKindOfClass(RUNTIME_CLASS(GameScene)))
		{
			((GameScene*) scene)->OnBattleBegin();
			break;
		}
		else
		{
			director->PopScene();
			scene = director->GetRunningScene();
		}
	}
}

void BattleMgr::showBattleScene()
{
	if(m_pkBattle)
	{
		m_pkBattle->setBattleMap(m_nBattleMapId, m_nBattleX * MAP_UNITSIZE_X, m_nBattleY * MAP_UNITSIZE_Y);
		NDMapLayer* mapLayer = NDMapMgrObj.getMapLayerOfScene(NDDirector::DefaultDirector()->GetRunningScene());
		mapLayer->GetParent()->AddChild(m_pkBattle);
		// ����ս��,��ͼ�߼�����
		NDMapMgrObj.BattleStart();
		ScriptGlobalEvent::OnEvent(GE_BATTLE_BEGIN);
		//++Guosen 2012.7.6
		NDScene*  pGameScene = NDDirector::DefaultDirector()->GetSceneByTag(SMGAMESCENE_TAG);
		if ( pGameScene )
		{//��ʱ������������UI
			NDUILayer * pLayer	= (NDUILayer *)pGameScene->GetChild(2010);//AffixNormalBoss//��������
			if ( pLayer )
				pLayer->SetVisible( false );
			pLayer	= (NDUILayer *)pGameScene->GetChild(2015);//Arena//����������
			if ( pLayer )
				pLayer->SetVisible( false );
			pLayer	= (NDUILayer *)pGameScene->GetChild(2035);//DynMapGuide//�鿴���Խ���
			if ( pLayer )
				pLayer->SetVisible( false );
		}
	}
}

void BattleMgr::SetBattleOver(void)
{
	if(m_pkBattle != NULL)
	{
		m_pkBattle->SetBattleOver();
	}
}

void BattleMgr::restartLastBattle()
{
	if (m_pkBattle && m_pkBattle->GetParent() != NULL)
	{
		m_pkBattle->RemoveFromParent(true);
		m_pkBattle = NULL;
	}
	if (m_nBattleMapId == 0)
	{
		return;
	}
	if (!m_pkBattle)
	{
		int battleType = this->m_nLastBattleType;
		if(BATTLE_TYPE_MONSTER == battleType)
		{
			m_pkBattle = new Battle(BATTLE_TYPE_MONSTER_PLAYBACK);
		}
		else if(BATTLE_TYPE_SPORTS == battleType || BATTLE_TYPE_BOSS == battleType)
		{
			m_pkBattle = new Battle(BATTLE_TYPE_SPORTS_PLAYBACK);
		}

		m_pkBattle->Initialization(BATTLE_STAGE_START);
		m_pkBattle->StartEraseOutEffect();
		m_pkBattle->setTeamAmout(m_nLastBattleTeamCount);
	}

	for (VEC_FIGHTER_IT it = m_vFighter.begin(); it != m_vFighter.end(); it++)
	{
		NDLog("add_fighter");
		Fighter* fighter = *it;

		fighter->reStoreAttr();
		fighter->setBattle(m_pkBattle);
//		fighter->LoadMonster(idlookface, level, strName);
		m_pkBattle->AddFighter(fighter);
	}

	ScriptMgrObj.excuteLuaFunc("SetUIVisible", "",0);
	NDDirector* director = NDDirector::DefaultDirector();
	NDMapLayer* mapLayer = NDMapMgrObj.getMapLayerOfScene(NDDirector::DefaultDirector()->GetRunningScene());
	mapLayer->showSwitchSprite(SWITCH_TO_BATTLE);
	//mapLayer->SetBattleBackground(true);
	RestoreActionList();
	CCSize winSize = CCDirector::sharedDirector()->getWinSizeInPixels();
	m_pkBattle->SetFrameRect(CCRectMake(0, 0, winSize.width, winSize.height));

	m_pkBattle->InitEudemonOpt();
	m_pkBattle->sortFighterList();
	m_pkBattle->InitSpeedBar();
	//m_battle->SetVisible(false);
	director->EnableDispatchEvent(false);
	m_pkBattle->RestartFight();
}

void BattleMgr::processBattleStart(NDEngine::NDTransData& bao)
{

	CloseProgressBar;
	ScriptMgrObj.excuteLuaFunc("PlayBattleMusic", "Music");

	//���ݽ���ս��ǰһ�����ķǸ�����ͼ����ID������λ��//++Guosen 2012.7.5
	NDMapLayer * pMapLayer = NDMapMgrObj.getMapLayerOfScene(NDDirector::DefaultDirector()->GetRunningScene());
	if( pMapLayer && ( pMapLayer->GetMapIndex() < 3 ) )//3��������ս��ʱ�ı���
	{
		m_nLastSceneMapDocID = pMapLayer->GetMapIndex();
		m_nLastSceneScreenX	= pMapLayer->GetScreenCenter().x;
		m_nLastSceneScreenY	= pMapLayer->GetScreenCenter().y;
	}

	Byte btPackageType = 0;
	Byte btBattleType = 0;
	Byte btTeamAmout = 0;
	int map_id = 0;
	unsigned short posX = 0;
	unsigned short posY = 0;

	bao >> btPackageType >> btBattleType >> btTeamAmout >> map_id >> posX >> posY;	//>> btAction;

	bool bBattleStart = (btPackageType & 2 > 0);
	m_nLastBattleType = btBattleType;

	//	if (btAction == BATTLE_STAGE_LEAVE_WATCH) {
	//		// �˳���ս
	//		if (m_battle && m_battle->IsWatch()) {
	//			m_battle->setServerBattleResult(BATTLE_COMPLETE_END);
	//			if (m_battle->IsPracticeBattle()) {
	//				m_battle->setBattleStatus(Battle::BS_FIGHTER_SHOW_PAS);
	//			} else {
	//				quitBattle();
	//			}
	//			
	//			return;
	//		}
	//	}

	if (bBattleStart && m_pkBattle && m_pkBattle->GetParent() != NULL)
	{
		quitBattle(false);
	}

	// add by jhzheng
	if (bBattleStart)
	{
		// �ȹر����ڲ�����ui����
		CloseProgressBar;
		//		closeUI();
	}

	NDMapMgr& mapMgr = NDMapMgrObj; ///< ��ʱ��ע�� ����

	// ��ʼ��ս������
	if (!m_pkBattle)
	{
		m_pkBattle = new Battle(btBattleType);
		m_pkBattle->Initialization(BATTLE_STAGE_START);
		m_pkBattle->StartEraseOutEffect();

	}

	if (btPackageType & 1 > 0)
	{
		m_pkBattle->setTeamAmout(btTeamAmout);
		m_nLastBattleTeamCount = btTeamAmout;
		for (VEC_FIGHTER_IT it = m_vFighter.begin(); it != m_vFighter.end(); it++)
		{
			CC_SAFE_DELETE(*it);
		}
		m_vFighter.clear();
		ReleaseActionList();
	}

	Byte btAmout = 0;

	bao >> btAmout;

	m_pkBattle->SetTurn(0);

	for (Byte i = 0; i < btAmout; i++)
	{
		int idObj = 0;
		int idType = 0; // ����
		int idlookface = 0;
		USHORT level = 0;
		Byte nFighterType = 0; // ��ң�����,����
		int nLife = 0;
		int nLifeMax = 0;
		int nMana = 0;
		int nManaMax = 0;
		Byte btGroup = 0;
		Byte btBattleTeam = 0;
		Byte btStations = 0;
		Byte btEquipAmount = 0;
		int weapon_id=0;
		int model_id=0;
		int skillID=0;
		int atk_type=0;

		Byte nQuality=0;

		//int idPet = 0;
		idObj = bao.ReadInt();

		bao >> nFighterType;
		idType = bao.ReadInt();
		idlookface = bao.ReadInt();
		skillID=bao.ReadInt();
		level = bao.ReadShort();
		nLife = bao.ReadInt();
		nLifeMax = bao.ReadInt();
		nMana = bao.ReadInt();
		nManaMax = bao.ReadInt();
		bao >> btGroup >> btBattleTeam >> btStations >> btEquipAmount >> nQuality;
		std::string strName;
		strName = bao.ReadUnicodeString();
		for (Byte i = 0; i < btEquipAmount; i++)
		{
			// ����װ��
		}

		if (nFighterType ==	FIGHTER_TYPE_PET)
		{
			atk_type = ScriptDBObj.GetN( "pet_config", idType, DB_PET_CONFIG_ATK_TYPE );
		}
		else if (nFighterType == FIGHTER_TYPE_MONSTER)
		{
			atk_type = ScriptDBObj.GetN( "monstertype", idType, DB_MONSTERTYPE_ATK_TYPE );
		}

		FIGHTER_INFO info;
		info.level = level;
		info.idObj = idObj;
		info.idType = idType;
		info.idlookface = idlookface;
		info.skillId = skillID;
		info.fighterType = FIGHTER_TYPE(nFighterType);
		info.group = BATTLE_GROUP(btGroup);
		info.original_life = nLife;
		info.nLife = nLife;
		info.nLifeMax = nLifeMax;
		info.original_mana = nMana;
		info.nMana = nMana;
		info.nManaMax = nManaMax;
		info.btBattleTeam = btBattleTeam;
		info.btStations = btStations;
		info.atk_type		= atk_type;
		info.nQuality       = nQuality;
		NDLog(@"id:%d,type:%d,fighterType,%d,group:%d,team:%d,pos:%d,life:%d", idObj,idType,nFighterType,btGroup,btBattleTeam,btStations,nLife);
		Fighter* fighter = new Fighter();
		fighter->setFighterInfo(info);
		fighter->setBattle(m_pkBattle);

		if (model_id != 0)
		{
			idlookface = idlookface/100000*100000+model_id*100+idlookface%100;
		}

//		if (nFighterType == FIGHTER_TYPE_PET) {
		//				if (idObj == role.m_id) { // ���ؽ�ɫ
		// ֹͣ��·
		fighter->LoadMonster(idlookface, level, strName);
		//				} else { // ��Χ���
		//					NDManualRole* player = mapMgr.GetManualRole(idObj);
		//					if (player) {
		//						CCPoint pos = CCPointZeroON;
		//						if (player->GetLastPointOfPath(pos))
		//						{
		//							player->SetPositionEx(pos);
		//							player->stopMoving(false, false);
		//						}
		//						else
		//						{
		//							player->stopMoving(true, false);
		//						}
		//						
		//						fighter->SetRole(player);
		//					}
		//				}
//		}else if (nFighterType == FIGHTER_TYPE_MONSTER) { // �ұߵĹ���
//			//				int lookface = 0;
//			//				bao >> lookface;
//			//				string name = bao.ReadUnicodeString();
//			fighter->LoadMonster(idlookface, level, strName);
//		}
		fighter->GetRole()->m_nLife = nLife;
		fighter->GetRole()->m_nMaxLife = nLifeMax;
		fighter->GetRole()->m_nMana = nMana;
		fighter->GetRole()->m_nMaxMana = nManaMax;
		if(weapon_id != 0)
		{
			fighter->GetRole()->SetWeaponImage(weapon_id);
		}
		// ������·���״̬
		//			for (int j = 0; j < statusNum; j++) {
		//				int idStatus = bao.ReadInt();
		//				int idEffect = bao.ReadInt();
		//				Byte num = bao.ReadByte();
		//				string name = bao.ReadUnicodeString();
		//				string des = bao.ReadUnicodeString();
		//				fighter->addAStatus(new FighterStatus(idStatus, idEffect, num, name, des));
		//			}
		m_vFighter.push_back(fighter);
		m_pkBattle->AddFighter(fighter);

		//			// ��������
		//			if (info.nLife == 0)
		//			{
		//				fighter->setDieOK(true);
		//				dieAction(*fighter);
		//			}
	}

	if (bBattleStart)
	{
		/* comment by jhzheng
		 // �ر����ڲ�����ui����
		 CloseProgressBar;
		 closeUI();
		 */

		m_nBattleMapId = map_id;
		m_nBattleX = posX;
		m_nBattleY = posY;
		ScriptMgrObj.excuteLuaFunc("SetUIVisible", "",0);//����UI�������٣�,//����������򸱱����漰���Խ���Ҫ��װ�غõ�ͼ��������
		//++Guosen 2012.7.6
		NDScene*  pGameScene = NDDirector::DefaultDirector()->GetSceneByTag(SMGAMESCENE_TAG);
		if ( pGameScene )
		{//��ʱ�����ص���������UI
			NDUILayer * pLayer	= (NDUILayer *)pGameScene->GetChild(2010);//AffixNormalBoss//��������
			if ( pLayer )
				pLayer->SetVisible( true );
			pLayer	= (NDUILayer *)pGameScene->GetChild(2015);//Arena//����������
			if ( pLayer )
				pLayer->SetVisible( true );
			pLayer	= (NDUILayer *)pGameScene->GetChild(2035);//DynMapGuide//�鿴���Խ���
			if ( pLayer )
				pLayer->SetVisible( true );
		}

		if (btBattleType != BATTLE_TYPE_SPORTS)
		{
			//�Ǿ�����ս��������������
			ScriptMgrObj.excuteLuaFunc("CloseMainUI", "",0);

		}
		NDDirector* director = NDDirector::DefaultDirector();
		NDMapLayer* mapLayer = mapMgr.getMapLayerOfScene(NDDirector::DefaultDirector()->GetRunningScene());

		//mapLayer->SetBattleBackground(true);

		CCSize winSize = CCDirector::sharedDirector()->getWinSizeInPixels();
		m_pkBattle->SetFrameRect(CCRectMake(0, 0, winSize.width, winSize.height));

		mapLayer->showSwitchSprite(SWITCH_TO_BATTLE);
		mapLayer->setBattleType(btBattleType);

		m_pkBattle->InitEudemonOpt();
		m_pkBattle->sortFighterList();
		m_pkBattle->InitSpeedBar();
		//m_battle->SetVisible(false);
		director->EnableDispatchEvent(false);

	}
	//}
}

void BattleMgr::processControlPoint(NDEngine::NDTransData& bao)
{
	Fighter* user = m_pkBattle->GetMainUser();
	if (user)
	{
		bao >> user->m_nAttackPoint >> user->m_nDefencePoint >> user->m_nDistancePoint;
	}
}

void BattleMgr::processBattleEffect(NDEngine::NDTransData& bao)
{
	Byte btPackFlag = 0;
	bao >> btPackFlag;

	Byte btEffectNum = 0;
	bao >> btEffectNum;
	if ((btPackFlag & 1) > 0)
	{
		m_pkBeforeCommand = NULL;
	}

	int idSkillActor = 0;
	int idSkillTarget = 0;
	for (int i = 0; i < btEffectNum; i++)
	{
		BATTLE_EFFECT_TYPE btEffectType = BATTLE_EFFECT_TYPE(bao.ReadShort());
		int idActor = bao.ReadInt();
		long data = bao.ReadLong();

		int lookface;

		Command* cmd = NULL;
		if(btEffectType != BATTLE_EFFECT_TYPE_SKILL_EFFECT)
		{
			cmd = new Command();
			cmd->btEffectType = btEffectType;
			cmd->idActor = idActor;
		}

		switch(btEffectType){
			case BATTLE_EFFECT_TYPE_ATK:
				//cmd->idTarget=(int) data;
				//cmdBefore=cmd;
				//NDLog(@"%d ATK %d", idActor,cmd->idTarget);
				{
					Command* pCmd = new Command();
					pCmd->btEffectType	= BATTLE_EFFECT_TYPE_PLAY_ANIMATION;
					pCmd->idActor		= idActor;
					pCmd->idTarget		= 48;	//���š����������ֶ���
					if ( m_pkBeforeCommand )
					{
						m_pkBattle->AddCommand(pCmd);
					}
					else
					{
						m_pkBeforeCommand->cmdNext = pCmd;
						m_pkBeforeCommand = pCmd;
					}
					cmd->idTarget		= (int)data;
					m_pkBeforeCommand			= cmd;
				}
				break;
			case BATTLE_EFFECT_TYPE_SKILL:
				NDLog(@"%d SKILL ", idActor);
				cmd->skill=new BattleSkill();
				cmd->skill->setName(ScriptDBObj.GetS("skill_config",(unsigned int) data, DB_SKILL_CONFIG_NAME));
				if(data == 999999)//��ָ��ͨ����
				{
					//
					Fighter* pFighter = GetBattle()->GetFighter(idActor);
					int nAtkType = 0;
					if ( pFighter->m_kInfo.fighterType == FIGHTER_TYPE_PET )
					{
						nAtkType = ScriptDBObj.GetN( "pet_config", pFighter->m_kInfo.idType, DB_PET_CONFIG_ATK_TYPE );
					}
					else if( pFighter->m_kInfo.fighterType == FIGHTER_TYPE_MONSTER )
					{
						nAtkType = ScriptDBObj.GetN( "monstertype", pFighter->m_kInfo.idType, DB_MONSTERTYPE_ATK_TYPE );
					}

					if ( nAtkType == FIGHTER_ATK_TYPE_PHY )
					{//�������
						cmd->skill->setAtkType(SKILL_ATK_TYPE_NEAR);
					}
					else
					{//����Զ��������Զ��
						cmd->skill->setAtkType(SKILL_ATK_TYPE_REMOTE);
					}
				}
				else
				{
					cmd->skill->setAtkType(SKILL_ATK_TYPE_REMOTE);//����ȫ��ΪԶս
					cmd->skill->setSelfEffect(ScriptDBObj.GetN("skill_config",(unsigned int) data, DB_SKILL_CONFIG_LOOKFACE_ID));
					cmd->skill->setTargetEffect(ScriptDBObj.GetN("skill_config",(unsigned int) data, DB_SKILL_CONFIG_LOOKFACE_TARGET_ID));
				}
				cmd->skill->SetActId(MANUELROLE_ATTACK);

				cmd->skill->setId((unsigned int) data);
				m_pkBeforeCommand=cmd;
				break;
			case BATTLE_EFFECT_TYPE_SKILL_TARGET:
				cmd->idTarget = (int) data;
				NDLog(@"SKILL atk:%d, target:%d", cmd->idActor, cmd->idTarget);
				break;

			case BATTLE_EFFECT_TYPE_SKILL_EFFECT_TARGET:
				cmd->idTarget = (int) data;
				NDLog(@"SKILL atk:%d, target:%d", cmd->idActor, cmd->idTarget);
				idSkillActor = idActor;//��ס���ܹ���Ŀ��
				idSkillTarget = (int) data;
				break;
			case BATTLE_EFFECT_TYPE_SKILL_EFFECT:
				if(idSkillActor && idSkillTarget)
				{
					//idAction,dataֵΪskill_result_cfg.id����ϣ���ʽΪAABBCC������λ�������������
					//6������ǰ�����ǹ��������������Ƿ��ط�
					int idEffect[6];
					idEffect[0] = idActor%100;
					idEffect[1] = idActor/100%100;
					idEffect[2] = idActor/10000;
					idEffect[3] = data%100;
					idEffect[4] = data/100%100;
					idEffect[5] = data/10000;
					for(int i = 0; i < 6; ++i)
					{
						if(idEffect[i] != 0)
						{
							cmd = new Command();
							cmd->btEffectType = BATTLE_EFFECT_TYPE_SKILL_EFFECT;
							cmd->idActor = (i<3) ? idSkillActor : idSkillTarget;
							int lookface = ScriptDBObj.GetN("skill_result_cfg",(unsigned int) idEffect[i], DB_SKILL_RESULT_CFG_LOOKFACE);
							cmd->idTarget = lookface;
							NDLog(@"SKILL effect id:%d effect:%d", cmd->idActor, cmd->idTarget);
							if(m_pkBeforeCommand!=NULL && cmd){
								m_pkBeforeCommand->cmdNext = cmd;
								m_pkBeforeCommand = cmd;
							}
							cmd = NULL;//���գ��Է������ټӽ�ȥ
						}
					}
				}
				else
				{
					idSkillActor = 0;
					idSkillTarget = 0;
				}
				break;
			case BATTLE_EFFECT_TYPE_LIFE:
				NDLog(@"%d HURT", idActor);
				cmd->nHpLost=(int)data;
				break;
			case BATTLE_EFFECT_TYPE_MANA: // ����
				NDLog(@"%d MANA", idActor);
				cmd->nMpLost=(int)data;
				break;
			case BATTLE_EFFECT_TYPE_DODGE:  // ����
				NDLog(@"%d DODGE", idActor);
				break;
			case BATTLE_EFFECT_TYPE_DRITICAL: // ����
				NDLog(@"%d DRITICAL", idActor);
				cmd->nHpLost=(int)data;
				break;
			case BATTLE_EFFECT_TYPE_BLOCK:  // ��
				NDLog(@"%d BLOCK", idActor);
				break;
			case BATTLE_EFFECT_TYPE_COMBO:  // ����
				NDLog(@"%d COMBO", idActor);
				cmd->nHpLost=(int)data;
				break;
			case BATTLE_EFFECT_TYPE_STATUS_ADD: // ��״̬��
				NDLog(@"%d STATUS_ADD", idActor);
				cmd->idTarget	= (int)data;// skill_result_cfg �� ID
				break;
			case BATTLE_EFFECT_TYPE_STATUS_LOST: // ȡ��״̬
				NDLog(@"%d STATUS_CANCEL", idActor);
				cmd->idTarget	= (int)data;// skill_result_cfg �� ID
				break;
			case BATTLE_EFFECT_TYPE_CTRL:
				break;
			case BATTLE_EFFECT_TYPE_ESCORTING://����/Ԯ��
				NDLog(@"%d ESCORTING", idActor);
				break;
			case BATTLE_EFFECT_TYPE_COOPRATION_HIT://�ϻ�
				NDLog(@"%d COOPRATION_HIT", idActor);
				break;
			case BATTLE_EFFECT_TYPE_CHANGE_POSTION://��λ
				NDLog(@"%d CHANGE_POSTION", idActor);
				cmd->idTarget	= (int)data;//pos
				//cmdBefore = NULL;
				break;
			case BATTLE_EFFECT_TYPE_STATUS_LIFE: // ״̬ȥѪ
				NDLog(@"%d STATUS_LIFE", idActor);
				cmd->nHpLost=(int)data;
				break;
			case BATTLE_EFFECT_TYPE_STATUS_MANA: // ״̬ȥ��
				NDLog(@"%d STATUS_MANA", idActor);
				cmd->nMpLost=(int)data;
				break;
			case BATTLE_EFFECT_TYPE_RESIST: // ����
				NDLog(@"%d STATUS_RESIST", idActor);
				break;
			case EFFECT_TYPE_TURN_END:   // �غϽ���
				NDLog(@"%d TURN_END", idActor);
				m_pkBeforeCommand=NULL;
				break;
			case EFFECT_TYPE_BATTLE_BEGIN:  // ս����ʼ
				NDLog(@"%d BEGIN BATTLE", idActor);
				m_nCurrentTeamId=idActor;
				cmd->idTarget	= (int)data;
				break;
			case EFFECT_TYPE_BATTLE_END:   // ս������
				NDLog(@"%d END BATTLE", idActor);
				cmd->idActor	= m_nCurrentTeamId;
				m_pkBeforeCommand = NULL;
				break;
			case BATTLE_EFFECT_TYPE_DEAD:
				NDLog(@"%d DIE", idActor);
				break;
			case BATTLE_EFFECT_TYPE_PLAY_ANIMATION: // �������ϲ���ָ������
				NDLog(@"%d PLAY_ANIMATION", idActor);
				cmd->idTarget = (int) data;
				//cmdBefore = cmd;
				break;
			default:
				NDLog(@"%d NONE", idActor);
				break;
		}

		if (btEffectType == BATTLE_EFFECT_TYPE_ATK
				|| btEffectType == BATTLE_EFFECT_TYPE_SKILL
				|| btEffectType == EFFECT_TYPE_BATTLE_BEGIN
				|| btEffectType == EFFECT_TYPE_BATTLE_END
				|| btEffectType == BATTLE_EFFECT_TYPE_STATUS_LIFE
				|| btEffectType == BATTLE_EFFECT_TYPE_STATUS_MANA
				|| m_pkBeforeCommand == NULL
				&& cmd)
		{
			m_pkBattle->AddCommand(cmd);
		}
		else if (m_pkBeforeCommand != NULL && cmd)
		{
			m_pkBeforeCommand->cmdNext = cmd;
			m_pkBeforeCommand = cmd;
		}
	}

	if ((btPackFlag & 2) > 0)
	{
		BOOL bIsDramaPlaying = ScriptMgrObj.excuteLuaFunc("IfDramaPlaying","Drama",0);
		if (bIsDramaPlaying == true)
		{
			//m_pkBattle->StartFight();
			return;
		}
		else
		{
			m_pkBattle->StartFight();
		}
	}
}

void BattleMgr::BattleContinue()
{
	m_pkBattle->StartFight();
}

string getStrItemsFromIntArray(vector<OBJID>& items)
{
	stringstream ss;
	ItemMgr& itemMgr = ItemMgrObj;
	while (items.size() > 0) {
		OBJID idItemType = items.at(0);
		int count = 0;
		NDItemType* pItem = itemMgr.QueryItemType(idItemType);
		if (NULL == pItem) {
			NDLog("δ�ҵ���Ʒ���ͣ�[%d]", idItemType);
			items.erase(items.begin());
			continue;
		}

		ss << pItem->m_name;

		for(vector<OBJID>::iterator it = items.begin(); it != items.end(); )
		{
			if(*it == idItemType) {
				count++;
				it = items.erase(it);
			} else {
				it++;
			}
		}

		ss << " * " << count;

		if (items.size() > 0) {
			ss << "\n";
		}
	}

	return ss.str();
}

void BattleMgr::processBattleEnd(NDEngine::NDTransData& bao)
{
	//NDMapMgr& mapMgr = NDMapMgrObj;
	Byte btBattleType = 0;
	int dwMoney = 0;
	int dwEMoney = 0;
	int dwExp = 0;
	int dwRepute = 0;
	int dwSoph = 0;   //����
	Byte dwSoldierNum = 0;     //��ս������
	Byte btResult = 0;
	int dwHurt = 0;
	Byte btItemAmount = 0;    //������Ʒ������
	pair<int, int> pSoldExp;

	bao >> btBattleType >> dwMoney >> dwEMoney >> dwExp
		>> dwRepute>>dwSoph >> btResult >>dwHurt
		>> btItemAmount >> dwSoldierNum;

	NDLog("receive_battle_end:%d", btResult);

	m_pkBattleReward = new BattleReward();
	m_pkBattleReward->m_nMoney = dwMoney;
	m_pkBattleReward->m_nEMoney = dwEMoney;
	m_pkBattleReward->m_nEXP = dwExp;
	m_pkBattleReward->m_nRepute = dwRepute;
	m_pkBattleReward->m_nSoph = dwSoph;
	m_pkBattleReward->m_nBattleResult = btResult;
//	int nPetLoyal = btPetLoyal;
//	if (nPetLoyal > 127) {
//		nPetLoyal -= 256;
//	}

	vector < OBJID > vItems;
	if (btItemAmount > 0)
	{
		for (int i = 0; i < btItemAmount; i++)
		{
			OBJID idItemType = bao.ReadInt();
			OBJID idItemCount = bao.ReadInt();
			NDLog("receive_battle_rewardItem:%d", idItemType);
			m_pkBattleReward->addItem(idItemType, idItemCount);
		}
	}

	//this->battleReward->mSoldExp.clear();
	for(int i = 0; i < dwSoldierNum; i++)
	{
		bao >> m_pkBattleReward->m_nPetId[i] >> m_pkBattleReward->m_nPetGainExp[i];
		//this->battleReward->mSoldExp[pSoldExp.first] = pSoldExp.second;
	}

	if (m_pkBattle)
	{
		m_pkBattle->setServerBattleResult(btResult);
	}
}
/************************************************************************************************************
 Function:         ShowBattleWinResult
 Description:    ������ʾս��ʤ���Ľ�� ����������ս���Լ�������ս����
 Input:               nBattleType ս������
 Output:     
 other:
 ���������������������������������޸�˵��������������������������������������������������
 version:  1. add by tangziqin  2012.8.17   ���Ӻ���ͷע��      
 2.
 ************************************************************************************************************/
void BattleMgr::ShowBattleWinResult(int nBattleType)
{
    //����ս��ʤ�� ��ʾ DynMapSuccess.ini ҳ��
    if (BATTLE_TYPE_MONSTER == nBattleType)
    {
        //�ж��Ƿ񴥷�����
        if( true == ScriptMgrObj.excuteLuaFunc("GlobalEventBattleEnd", "Drama", BATTLE_COMPLETE_WIN)) 
        {
            if (m_pkStartDramaTimer== NULL)
            {
                m_pkStartDramaTimer = new NDTimer;
            }
            m_pkStartDramaTimer->SetTimer(this, QUIT_DRAMA_TIMER_TAG, 1.0f);
        }
        else 
        {
            //��ʾ DynMapSuccess.ini ҳ��
            loadRewardUI();
        }
    }
    
    //������ս��ʤ��  ��ʾ SM_FIGHT_RESULT.ini ҳ��
    else if (BATTLE_TYPE_SPORTS == nBattleType ||  BATTLE_TYPE_BOSS == nBattleType)
    {
        NDLog(@"result,sports");
        ScriptMgrObj.excuteLuaFunc("LoadUI", "ArenaRewardUI",0);
        ScriptMgrObj.excuteLuaFunc("SetResult","ArenaRewardUI", BATTLE_COMPLETE_WIN, m_pkBattleReward->m_nMoney, m_pkBattleReward->m_nRepute);
        //���ҳ����ʾ֮���ͷŽ�����Ʒ
        if (m_pkBattleReward != NULL)
        {
            SAFE_DELETE (m_pkBattleReward);
        }      
    }
}

/************************************************************************************************************
 Function:         ShowBattleLoseResult
 Description:    ������ʾս��ʧ�ܵĽ�� ����������ս���Լ�������ս����
 Input:               nBattleType ս������
 Output:     
 other:
 ���������������������������������޸�˵��������������������������������������������������
 version:  1. add by tangziqin  2012.8.17   ���Ӻ���ͷע��      
 2.
 ************************************************************************************************************/
void BattleMgr::ShowBattleLoseResult(int nBattleType)
{
    //����ս��ʧ�� ��ʾ  BattleFailUI.ini ҳ��
    if (BATTLE_TYPE_MONSTER == nBattleType)
    {
        ScriptMgrObj.excuteLuaFunc("LoadUI", "BattleFailUI",0);
    }
    //������ս��ʧ�� ��ʾ    SM_FIGHT_RESULT.ini ҳ��
    else if (BATTLE_TYPE_SPORTS == nBattleType ||  BATTLE_TYPE_BOSS == nBattleType)
    {
        ScriptMgrObj.excuteLuaFunc("LoadUI", "ArenaRewardUI",0);
        ScriptMgrObj.excuteLuaFunc("SetResult","ArenaRewardUI", BATTLE_COMPLETE_LOSE, m_pkBattleReward->m_nMoney, m_pkBattleReward->m_nRepute);
    }
    
    //���ҳ����ʾ֮���ͷŽ�����Ʒ
    if (m_pkBattleReward != NULL)
    {
        SAFE_DELETE (m_pkBattleReward);
    }
}
/************************************************************************************************************
 Function:         ShowReplayWinResult
 Description:    ������ʾս��ʤ���طŵĽ������������ս���Լ�������ս����
 Input:               nBattleType ս������
 Output:     
 other:
 ���������������������������������޸�˵��������������������������������������������������
 version:  1. add by tangziqin  2012.8.17   ���Ӻ���ͷע��      
 2.
 ************************************************************************************************************/
void BattleMgr::ShowReplayWinResult(int nBattleType)
{
    //�������ɹ��ط� ��ʾ  SM_FIGHT_RESULT.ini ҳ��
    if (BATTLE_TYPE_SPORTS_PLAYBACK == nBattleType)
    {
        ScriptMgrObj.excuteLuaFunc("LoadUI", "ArenaRewardUI",0);
        NDLog(@"money:%d--repute:%d",prebattleReward->money,prebattleReward->repute);
        ScriptMgrObj.excuteLuaFunc("SetResult","ArenaRewardUI", BATTLE_COMPLETE_WIN, m_pkPrebattleReward->m_nMoney, m_pkPrebattleReward->m_nRepute);
    }
    
    //����ս���ɹ��ط� ��ʾ  DynMapSuccess.ini
    else if (BATTLE_TYPE_MONSTER_PLAYBACK == nBattleType)
    {  
        NDLog(@"type:%d",m_pkBattle->GetBattleType());
        ScriptMgrObj.excuteLuaFunc("LoadUI", "MonsterRewardUI",0);
       // ScriptMgrObj.excuteLuaFunc("SetRewardExp", "MonsterRewardUI",prebattleReward->exp);
        ScriptMgrObj.excuteLuaFunc("addSophMoney", "MonsterRewardUI", m_pkPrebattleReward->m_nSoph, m_pkPrebattleReward->m_nMoney);	
        
        for(int i=0;i<5;i++)
        {
            if(m_pkPrebattleReward->m_nItemTypes[i]!=0)
            {
                NDLog("addRewardItem:%d",m_pkPrebattleReward->m_nItemTypes[i]);
                ScriptMgrObj.excuteLuaFunc("addRewardItem", "MonsterRewardUI", i+1, m_pkPrebattleReward->m_nItemTypes[i], m_pkPrebattleReward->m_nItemAmount[i]);
                
            }
        }	
        
        for(int i=0;i<5;i++)
        {
            if(m_pkPrebattleReward->m_nPetId[i] !=0)
            {
                NDLog(@"iPetId :%d;  iPetGainExp : %d", m_pkPrebattleReward->m_ntemtype[i], m_pkPrebattleReward->m_nPetGainExp[i]);
                ScriptMgrObj.excuteLuaFunc("SetRewardExp", "MonsterRewardUI", m_pkPrebattleReward->m_nPetId[i], m_pkPrebattleReward->m_nPetGainExp[i]);
            }
        }
        
        ScriptMgrObj.excuteLuaFunc("Refresh", "MonsterRewardUI");	
    }
}

/************************************************************************************************************
 Function:         ShowReplayLoseResult
 Description:    ������ʾս��ʧ�ܻطŵĽ�� ����������ս���Լ�������ս����
 Input:               nBattleType ս������
 Output:     
 other:
 ���������������������������������޸�˵��������������������������������������������������
 version:  1. add by tangziqin  2012.8.17   ���Ӻ���ͷע��      
 2.
 ************************************************************************************************************/
void BattleMgr::ShowReplayLoseResult(int nBattleType)
{
    //��������սʧ�ܻط� ��ʾ  SM_FIGHT_RESULT.ini ҳ��
    if (BATTLE_TYPE_SPORTS_PLAYBACK == nBattleType)
    {
        ScriptMgrObj.excuteLuaFunc("LoadUI", "ArenaRewardUI",0);
        ScriptMgrObj.excuteLuaFunc("SetResult","ArenaRewardUI", BATTLE_COMPLETE_LOSE, m_pkPrebattleReward->m_nMoney, m_pkPrebattleReward->m_nRepute);
    }
    //����ս��ʧ�ܻط� ��ʾ  BattleFailUI.ini
    else if (BATTLE_TYPE_MONSTER_PLAYBACK == nBattleType)
    {  
        ScriptMgrObj.excuteLuaFunc("LoadUI", "BattleFailUI",0);
    }
}

/************************************************************************************************************
 Function:         showBattleResult
 Description:    ��ʾս�����
 Input:               
 Output:     
 other:
 ���������������������������������޸�˵��������������������������������������������������
 version:  1. add by tangziqin  2012.8.17   ���Ӻ���ͷע��      
 2.
 ************************************************************************************************************/
void BattleMgr::showBattleResult()
{
	int battleType= m_pkBattle->GetBattleType();
	BATTLE_COMPLETE battle_result = BATTLE_COMPLETE_LOSE;

	//�ر���ս�پ���ť
	ScriptMgrObj.excuteLuaFunc("CloseUI", "BattleMapCtrl");

	//Ϊ�˻طŽ�����ʾ��������Ʒ���ؼ�¼�����Ķ���
	if (m_pkPrebattleReward != NULL)
	{
		if(m_pkPrebattleReward == NULL)
		{
			m_pkPrebattleReward = new BattleReward();
			if(m_pkPrebattleReward == NULL)
			{
				SAFE_DELETE (m_pkPrebattleReward);
				return;
			}     
		}
		*m_pkPrebattleReward = *m_pkBattleReward;
	}


	//�Ƿ��н������ط�û�н����������н���
	if (m_pkBattleReward != NULL)
	{
		battle_result = BATTLE_COMPLETE(m_pkBattleReward->m_nBattleResult);

		if (battle_result == BATTLE_COMPLETE_WIN)
		{
			//������ʾս��ʤ���Ľ��
			ShowBattleWinResult(battleType);
		}
		else if(battle_result == BATTLE_COMPLETE_LOSE)
		{
			//������ʾս��ʧ�ܵĽ��
			ShowBattleLoseResult(battleType);
		}
	}
	else 
	{
		//����Ǿ������е� �鿴���� ���߸����еĹ��Թ�����û�н�����                                                
		if ((BATTLE_TYPE_SPORTS == battleType) || (BATTLE_TYPE_MONSTER == battleType) || (m_pkPrebattleReward == NULL))
		{
			//��ʾ SM_FIGHT_REPLAY.ini ҳ��
			SAFE_DELETE (m_pkPrebattleReward);
			ScriptMgrObj.excuteLuaFunc("LoadUI", "ArenaFightReplayUI",0);
		}
		else  //����ս���Լ���������ս�طŹ���
		{   
			if (m_pkPrebattleReward == NULL)
			{
				return;
			}

			battle_result = BATTLE_COMPLETE(m_pkPrebattleReward->m_nBattleResult);

			if(battle_result == BATTLE_COMPLETE_WIN)
			{
				//������ʾս��ʤ���طŵĽ��
				ShowReplayWinResult(battleType);
			}
			else if(battle_result == BATTLE_COMPLETE_LOSE)
			{
				//������ʾս��ʧ�ܻطŵĽ�� 
				ShowReplayLoseResult(battleType);
			}
		}
	}
}

void BattleMgr::quitBattle(bool bEraseOut/*=true*/)
{
	if (m_pkBattle)
	{
		if (bEraseOut)
		{
			if (!m_pkQuitTimer)
			{
				m_pkQuitTimer = new NDTimer;
				m_pkQuitTimer->SetTimer(this, QUIT_BATTLE_TIMER_TAG, 0.1f);
				m_pkBattle->StartEraseInEffect();
			}
			return;
		}

		if (m_pkQuitTimer)
		{
			m_pkQuitTimer->KillTimer(this, QUIT_BATTLE_TIMER_TAG);
			delete m_pkQuitTimer;
			m_pkQuitTimer = NULL;
		}

		BATTLE_COMPLETE battle_result = BATTLE_COMPLETE(
				m_pkBattle->getServerBattleResult());
		int battleType = m_pkBattle->GetBattleType();
		m_pkBattle->RemoveFromParent(true);
		//++Guosen 2012.7.2
		NDScene*  pGameScene = NDDirector::DefaultDirector()->GetSceneByTag(SMGAMESCENE_TAG);
		if ( pGameScene )
		{
			NDUILayer * pLayerAffixNormalBoss = (NDUILayer *)pGameScene->GetChild(2010);//AffixNormalBoss//��������
			NDUILayer * pLayerArena = (NDUILayer *)pGameScene->GetChild(2015);//Arena//����������
			NDUILayer * pLayerDynMapGuide = (NDUILayer *)pGameScene->GetChild(2035);//DynMapGuide//�鿴���Խ���
			if ( pLayerAffixNormalBoss || pLayerArena || pLayerDynMapGuide )
			{
				ScriptMgrObj.excuteLuaFunc("SetUIVisible", "",1);
				//��ԭ��ͼ--Guosen 2012.7.5
				NDMapLayer * pMapLayer = NDMapMgrObj.getMapLayerOfScene(pGameScene);//(NDDirector::DefaultDirector()->GetRunningScene());
				if ( pMapLayer && m_nLastSceneMapDocID )
				{//
					pMapLayer->SetBattleBackground(false);
					pMapLayer->replaceMapData( m_nLastSceneMapDocID, m_nLastSceneScreenX, m_nLastSceneScreenY );
					NDMapMgrObj.AddSwitch();
					m_nLastSceneMapDocID	= 0;
					m_nLastSceneScreenX		= 0;
					m_nLastSceneScreenY		= 0;
				} 
			}
		}

//		NDLog("result:%d",battle_result);
//		if(battleReward&&battle_result == BATTLE_COMPLETE_WIN)
//		{
//			if(battleType==BATTLE_TYPE_MONSTER)
//			{
//				NDLog("type:%d",m_battle->GetBattleType());
//				ScriptMgrObj.excuteLuaFunc("LoadUI", "MonsterRewardUI",0);
//				ScriptMgrObj.excuteLuaFunc("SetRewardExp", "MonsterRewardUI",battleReward->exp);
//
//				for(int i=0;i<5;i++)
//				{
//					if(battleReward->itemtype[i]!=0)
//					{
//						NDLog("addRewardItem:%d",battleReward->itemtype[i]);
//						ScriptMgrObj.excuteLuaFunc("addRewardItem", "MonsterRewardUI",i+1,battleReward->itemtype[i],battleReward->item_amount[i]);
//				
//					}
//				}
//			}else if(battleType==BATTLE_TYPE_SPORTS||battleType==BATTLE_TYPE_BOSS){
//				NDLog("result,sports");
//				ScriptMgrObj.excuteLuaFunc("LoadUI", "ArenaRewardUI",0);
//				ScriptMgrObj.excuteLuaFunc("SetResult","ArenaRewardUI",battle_result,battleReward->money,battleReward->repute);
//			}
//		}else if(battle_result == BATTLE_COMPLETE_LOSE)
//		{
//			if(battleType==BATTLE_TYPE_MONSTER)
//			{
//				ScriptMgrObj.excuteLuaFunc("LoadUI", "ArenaRewardUI",0);
//				ScriptMgrObj.excuteLuaFunc("SetResult","ArenaRewardUI",battle_result,battleReward->money,battleReward->repute);
//			}else if(battleType==BATTLE_TYPE_SPORTS||battleType==BATTLE_TYPE_BOSS){
//				ScriptMgrObj.excuteLuaFunc("LoadUI", "ArenaRewardUI",0);
//				ScriptMgrObj.excuteLuaFunc("SetResult","ArenaRewardUI",battle_result,battleReward->money,battleReward->repute);
//			}
//		}
		m_pkBattle = NULL;

		if (NDMapMgrObj.isMonsterClear()&&battleType==BATTLE_TYPE_MONSTER){
			NDLog("dynmap cleared");
			//ScriptMgrObj.excuteLuaFunc("OnBattleFinish","AffixBossFunc",NDMapMgrObj.GetMotherMapID(), 1);
		}

		NDPlayer::defaultHero().SetLocked(false);
//		GameScene* gs = (GameScene*)NDDirector::DefaultDirector()->GetScene(RUNTIME_CLASS(GameScene));
//		if (gs) {
//			gs->SetUIShow(false);
//		}
	}
}

void BattleMgr::OnDramaFinish()
{
	NDLog("drama call back dynmap cleared");
	//ScriptMgrObj.excuteLuaFunc("OnBattleFinish","AffixBossFunc",NDMapMgrObj.GetMotherMapID(), 1);//--Guosen 2012.7.4 ������ʾ��������
}

BattleMgr& BattleMgr::GetBattleMgr()
{
	if (0 == ms_pkSingleton)
	{
		ms_pkSingleton = new BattleMgr;
	}

	return *((BattleMgr*)ms_pkSingleton);
}