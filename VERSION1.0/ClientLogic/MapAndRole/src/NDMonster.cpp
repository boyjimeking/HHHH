//
//  NDMonster.mm
//  DragonDrive
//
//  Created by jhzheng on 10-12-31.
//  Copyright 2010 (����)DeNA. All rights reserved.
//

#include "NDMonster.h"
#include "NDPath.h"
#include "NDMapData.h"
#include "NDMapLayer.h"
#include "NDUILayer.h"
#include "NDDirector.h"
///< #include "NDMapMgr.h" ��ʱ��ע�� ����
#include "NDPlayer.h"
#include "NDDirector.h"
#include "NDConstant.h"
#include "NDTransData.h"
#include "NDDataTransThread.h"
#include "Battle.h"
#include "NDUISynLayer.h"
#include "CCPointExtension.h"
#include "NDAnimationGroupPool.h"
#include "SMGameScene.h"
#include "ScriptDataBase.h"
#include "globaldef.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

using namespace NDEngine;

IMPLEMENT_CLASS(NDMonster, NDBaseRole)

bool NDMonster::isRoleMonster(int lookface)
{
	return (lookface / 100000000 % 10) >= 3;
}

NDMonster::NDMonster() :
		state(0)
{
	INIT_AUTOLINK (NDMonster);
	self_move_rectw = NORMAL_MOVE_RECTW;
	self_move_rectH = NORMAL_MOVE_RECTH;
	bossRing = NULL;
	m_bIsAutoAttack = true;

	stop_time_count = 0;

	curStopCount = 0;

	curMoveCount = 0;

	curCount = 0;

	isFrozen = false;

	deadTime = time(NULL);

	frozenTime = time(NULL);

	m_nAttackArea = 0;

	monsterInit();

	moveRectW = 0;
	moveRectH = 0;

	bBattleMap = false;
	m_bRoleMonster = false;

	bClear = false;

	m_nFigure = 0;

	m_lbName = NULL;

	turnFace = false;

	m_bCanBattle = true;

	m_nOriginal_x = 0;
	m_nOriginal_y = 0;

	m_timeBossProtect = 0.0f;
	m_bIsHunt = false;
}

NDMonster::~NDMonster()
{
	SafeClearEffect (bossRing);
	m_lbName = NULL;
}

void NDMonster::Initialization(int idType)
{
//		ScriptDB* m_Db=new ScriptDB();
	m_strName = ScriptDBObj.GetS("monstertype", idType, DB_MONSTERTYPE_NAME);

	m_nType = idType;

	m_eCamp = CAMP_TYPE_NONE;
	m_nLevel = ScriptDBObj.GetN("monstertype", idType, DB_MONSTERTYPE_LEVEL);
	m_nAttackArea = ScriptDBObj.GetN("monstertype", idType,
			DB_MONSTERTYPE_ATK_AREA);
	int lookface = ScriptDBObj.GetN("monstertype", idType,
			DB_MONSTERTYPE_LOOKFACE);
	//lookface=100000000;
	int aiType = ScriptDBObj.GetN("monstertype", idType,
			DB_MONSTERTYPE_AI_TYPE);
//		sex = lookface / 100000000 % 10;
//		
//		if (lookface >= 20000 && lookface <= 20070 && lookface != 20030) {
//			turnFace = true;
//		}
	m_bRoleMonster = true;
	InitNonRoleData(m_strName, lookface, m_nLevel);
//		if ( sex >= 3 ) // isRoleMonster
//		{
//			m_bRoleMonster = true;
//			InitNonRoleData(m_name, lookface, level);
//		}else
//		{
//			
//			direct = lookface % 10;
//			
//			SetNormalAniGroup(lookface);
//		}

//		if (m_id > 0) 
//		{ //boss
//			m_nMonsterCatogary = MONSTER_BOSS;
//			self_move_rectw = BOSS_MOVE_RECTW;
//			self_move_rectH = BOSS_MOVE_RECTH;
//			
//			ElementMonsterDeal();
//		}
//		else 
//		{
	m_nMonsterCatogary = MONSTER_NORMAL;
	self_move_rectw = NORMAL_MOVE_RECTW;
	self_move_rectH = NORMAL_MOVE_RECTH;
//		}

	selfMoveRectX = GetPosition().x - (self_move_rectw - BASE_BOTTOM_WH) / 2;
	selfMoveRectY = GetPosition().y - (self_move_rectH - BASE_BOTTOM_WH) / 2;

	srandom (time(NULL));

m_bFaceRight	= random() % 2;

	if (aiType == 1)
	{
		m_bIsAutoAttack = true;
	}

	if (isUseAI())
	{
		m_bIsHunt = true;
		randomMonsterUseAI();
	}
	else
	{
		m_bIsHunt = false;
		randomMonsterNotAI();
	}
}

void NDMonster::setOriginalPosition(int o_x, int o_y)
{
	m_nOriginal_x = o_x;
	m_nOriginal_y = o_y;
}

void NDMonster::restorePosition()
{
	this->SetPosition(ccp(m_nOriginal_x, m_nOriginal_y));
}

void NDMonster::SetMoveRect(CGPoint point, int size)
{

	self_move_rectw = size * 2 * MAP_UNITSIZE;
	self_move_rectH = size * 2 * MAP_UNITSIZE;

	/***
	 * ��ʱ��ע�� ����
	 * begin
	 */
	// 		NDMapLayer *layer = NDMapMgrObj.getMapLayerOfScene(NDDirector::DefaultDirector()->GetRunningScene());
// 		NDMapData  *mapdata = NULL;
// 		if (layer) {
// 			mapdata=layer->GetMapData();
// 			if (mapdata){
// 				self_move_rectH = mapdata->getRows()*MAP_UNITSIZE;
// 			}
//		}
	/***
	 * ��ʱ��ע�� ����
	 * end
	 */

	selfMoveRectX = point.x - self_move_rectw / 2;
	selfMoveRectY = 0;
}

void NDMonster::Initialization(int lookface, int idNpc, int lvl)
{
//		if (lookface >= 20000 && lookface <= 20070 && lookface != 20030) {
//			turnFace = true;
//		} else {
//			turnFace = false;
//		}

	if (isRoleMonster(lookface))
	{
		m_bRoleMonster = true;
		InitNonRoleData("", lookface, lvl);
		SetNonRole(false);
	}
	else
	{
		SetNormalAniGroup(lookface);
		SetNonRole(true);
	}

	m_nID = idNpc;

	m_nMonsterCatogary = MONSTER_Farm;
	self_move_rectw = BOSS_MOVE_RECTW;
	self_move_rectH = BOSS_MOVE_RECTH;

	selfMoveRectX = GetPosition().x - (self_move_rectw - BASE_BOTTOM_WH) / 2;
	selfMoveRectY = GetPosition().y - (self_move_rectH - BASE_BOTTOM_WH) / 2;

	if (isUseAI())
	{
		randomMonsterUseAI();
	}
	else
	{
		randomMonsterNotAI();
	}
}

void NDMonster::SetType(int type)
{
	m_nMonsterCatogary = type;
	if (type == MONSTER_NORMAL)
	{ //boss

		ElementMonsterDeal();
	}
}

bool NDMonster::OnDrawBegin(bool bDraw)
{
	if (m_nFigure == 0)
	{
		SetShadowOffset(0, 10);
		ShowShadow(true);
	}
	else
	{
		SetShadowOffset(-8, 10);
		ShowShadow(true, true);
	}

	NDBaseRole::OnDrawBegin(bDraw);

	if (this->GetParent()
			&& this->GetParent()->IsKindOfClass(RUNTIME_CLASS(NDMapLayer)))
	{
		drawName(bDraw);
	}

	if (bossRing && m_pkSubNode)
	{
		if (!bossRing->GetParent())
		{
			m_pkSubNode->AddChild(bossRing);
		}

		if (m_nMonsterCatogary == MONSTER_BOSS)
		{
			int iX = m_kPosition.x;
			int iY = m_kPosition.y-DISPLAY_POS_Y_OFFSET;
			iX += getGravityX()-GetWidth()/2;
			iY += BASE_BOTTOM_WH + 8;
			bossRing->SetPosition(ccp(iX, iY));
			bossRing->RunAnimation(bDraw);
		}
	}

	return true;
}

void NDMonster::MoveToPosition(CGPoint toPos,
		SpriteSpeed speed/* = SpriteSpeedStep4*/, bool moveMap/* = false*/)
{
	if (this->GetParent())
	{
		NDLayer* layer = (NDLayer *) this->GetParent();
		if (layer->IsKindOfClass(RUNTIME_CLASS(NDMapLayer)))
		{
			m_kPointList.clear();
			m_bMoveMap = moveMap;
			m_bIsMoving = true;
			m_nMovePathIndex = 0;
			m_kPointList.push_back(toPos);
		}
	}
}

void NDMonster::WalkToPosition(CGPoint toPos)
{
	if (this->GetPosition().x > toPos.x)
	{
		if (!turnFace)
			m_bReverse = m_bFaceRight = true;
		else
			m_bReverse = m_bFaceRight = false;
	}
	else
	{
		if (!turnFace)
			m_bReverse = m_bFaceRight = false;
		else
			m_bReverse = m_bFaceRight = true;
	}

//		if (this->m_bRoleMonster) {
	//NDLog("faceRight:%d",m_faceRight);
	m_bFaceRight = !m_bFaceRight;
	m_bReverse = !m_bReverse;
//		}
	this->MoveToPosition(toPos, SpriteSpeedStep4, false);
}

void NDMonster::Update(unsigned long ulDiff)
{
	if ((!this->GetParent() && state == MONSTER_STATE_NORMAL) || bBattleMap)
	{
		return;
	}

	/***
	 * ��ʱ��ע�� ����
	 * begin
	 */

// 		if (NDMapMgrObj.GetBattleMonster() == this)
// 		{
// 			return;
// 		}
	/***
	 * ��ʱ��ע�� ����
	 * end
	 */

	if (isFrozen)
	{
		if (time(NULL) - frozenTime >= FROZEN_TIME)
		{
			//setMonsterFrozen(false);
		}
	}

	switch (state)
	{
	case MONSTER_STATE_DEAD:
	{
//				if (!(m_nMonsterCatogary == MONSTER_BOSS))
//				{ // ����Ǿ�Ӣ��ֱ�Ӳ����ͺ�
//					long intervalTime = [NSDate timeIntervalSinceReferenceDate] - deadTime;
//					if ( intervalTime > REFRESH_TIME ) 
//					{
//						monsterInit();
//						LogicDraw();
//					}
//					else 
//					{
//						LogicNotDraw();
//					}
//
//				}
//				else 
//				{
		//��Ӣ��ֱ�Ӵ��ڴ��ͷ�
		LogicNotDraw();
//				}

		break;
	}
	case MONSTER_STATE_NORMAL:
	{
		// ��ҵ���ײ�������
		doMonsterCollides();
		if (!isMonsterCollide) //&& T.getDialogList().size() == 0)
		{ // ����ײ״̬
			runLogic();
		}
		//drawMonster(g, offset_x, offset_y);
		break;
	}
	case MONSTER_STATE_BATTLE:
	{
		//drawMonster(g, offset_x, offset_y);
//				if (T.battleImage == null) {
//					T.battleImage = T.loadImage("battle.png",false);
//				}
//				T.drawImage(g, T.battleImage, x - aniGroup.getWidth() / 2 + (aniGroup.getGravityX()) - offset_x, y 
//							+ BASE_BOTTOM_WH - aniGroup.getHeight() - offset_y);
		break;
	}
	}
}

bool NDMonster::isUseAI()
{
	//if (GameScreen.role.isTeamLeader() || GameScreen.role.teamId == 0) {
	// ��ҷǱ���״̬,��ͨ������,����ֻ�е���ҵȼ����ڹ���ȼ�7��,����Ų�����AI��Ұ
	//if (isAutoAttack && !GameScreen.role.isSafeProtected() && m_nMonsterCatogary == MONSTER_NORMAL && !(level - GameScreen.role.getLevel() <= LEVEL_GRAY)) {
	if (m_nMonsterCatogary == MONSTER_Farm)
	{
		return false;
	}

	if (m_bIsAutoAttack && m_nMonsterCatogary == MONSTER_NORMAL)
	{
		int monsterRow = GetPosition().y / MAP_UNITSIZE;
		int monsterCol = GetPosition().x / MAP_UNITSIZE;
		int roleRow = NDPlayer::defaultHero().GetPosition().y / MAP_UNITSIZE;
		int roleCol = NDPlayer::defaultHero().GetPosition().x / MAP_UNITSIZE;
		int roleX = NDPlayer::defaultHero().GetPosition().x;
		int roleY = NDPlayer::defaultHero().GetPosition().y;

		if (roleX >= selfMoveRectX && roleX <= selfMoveRectX + self_move_rectw)
		//&& roleY >= selfMoveRectY
		//&& roleY <= selfMoveRectY + self_move_rectH)
		{ // ��������ڹ���Ļ��Χ��
//					if (abs(monsterRow - roleRow) > MONSTER_VIEW_AI || abs(monsterCol - roleCol) > MONSTER_VIEW_AI)
//					{ // ����̫Զ�Ͳ���ai
//						return false;
//					} else 
//					{
			if (roleX > (selfMoveRectX + self_move_rectw / 2 - 16)
					&& roleX < (selfMoveRectX + self_move_rectw / 2 + 16))
			{
				NDPlayer::defaultHero().SetLocked(true);
				NDPlayer::defaultHero().stopMoving();
			}

			return true;
//					}
		}
	}
	//}
	return false;
}

int NDMonster::getRandomAIDirect()
{
	// 0����,1����,2��, 3�� ,-1���޷���
	int monsterRow = GetPosition().y / MAP_UNITSIZE;
	int monsterCol = GetPosition().x / MAP_UNITSIZE;
	int roleRow = NDPlayer::defaultHero().GetPosition().y / MAP_UNITSIZE;
	int roleCol = NDPlayer::defaultHero().GetPosition().x / MAP_UNITSIZE;

	int xDirect = dir_invalid; // -1���޷���
	int yDirect = dir_invalid; // -1���޷���

//		NDMapLayer *layer = NDMapMgrObj.getMapLayerOfScene(NDDirector::DefaultDirector()->GetRunningScene()); ///< ��ʱ��ע�� ����

	/***
	 * ��ʱ��ע�� ����
	 * begin
	 */
// 		NDMapData  *mapdata = NULL;
// 		if (!layer || !( mapdata = layer->GetMapData() )) 
// 		{
// 			//NDLog("getRandomAIDirect ��ͼ����ͼ����Ϊ��");
// 			return dir_invalid;
// 		}
// 		
// 		if (monsterCol < roleCol) 
// 		{
// 			if ( !mapdata->canPassByRow(monsterRow, monsterCol + 1) ) 
// 			{ // �÷�������
// 				xDirect = dir_invalid;
// 			} else {
// 				xDirect = dir_right;
// 			}
// 			//m_faceRight=false;
// 		} 
// 		else if (monsterCol > roleCol) 
// 		{
// 			if ( !mapdata->canPassByRow(monsterRow, monsterCol - 1))
// 			{// �÷�������
// 				xDirect = dir_invalid;
// 			} else 
// 			{
// 				xDirect = dir_left;
// 			}
// 			//m_faceRight=false;
// 		}
// 		
// 		if (monsterRow < roleRow) 
// 		{
// 			if ( !mapdata->canPassByRow(monsterRow + 1, monsterCol) )
// 			{// �÷�������
// 				yDirect = dir_invalid;
// 			} else 
// 			{
// 				yDirect = dir_down;
// 			}
// 		} 
// 		else if (monsterRow > roleRow) 
// 		{
// 			if ( !mapdata->canPassByRow(monsterRow - 1 ,monsterCol) )
// 			{// �÷�������
// 				yDirect = dir_invalid;
// 			} else 
// 			{
// 				yDirect = dir_up;
// 			}
// 		}
// 		
// 		if (xDirect == dir_invalid) 
// 		{
// 			return yDirect;
// 		} 
// 		else if (yDirect == dir_invalid) 
// 		{
// 			return xDirect;
// 		} 
// 		else 
// 		{
// 			srandom(time(NULL));
// 			int n = abs(random() % 2);
// 			if (n == 0) 
// 			{
// 				return xDirect;
// 			} else 
// 			{
// 				return yDirect;
// 			}
// 		}
	return 0; ///< add by ����
}

void NDMonster::randomMonsterUseAI()
{
	curStopCount = 0;
	srandom (time(NULL));int
	m = getRandomAIDirect();
	//NDLog("monster ai direct:%d",m);
	if (m == dir_invalid)
	{ // -1��ʾ��·������,��ֹ����,��ô����ͨ�������·
		m_bIsAIUseful = false;
		m_nMoveDirect = (random() % 4 + random() % 4) % 4;
		m_nMoveCount = random() % MOVE_MIN + random() % MOVE_MIN;
		stop_time_count = random() % ((m_nMoveCount + 1) * 10);
	}
	else
	{
		m_bIsAIUseful = true;
		m_nMoveDirect = m;
		m_nMoveCount = 1;
		stop_time_count = 0;
	}
	curMoveCount = 0;
	//setAnigroupFace();
	SetCurrentAnimation(MONSTER_MAP_MOVE, m_bFaceRight);
}

void NDMonster::randomMonsterNotAI()
{
	curStopCount = 0;
	m_nMoveDirect = (random() % 4 + random() % 4) % 4;
	m_nMoveCount = random() % MOVE_MIN + random() % MOVE_MIN;
	stop_time_count = random() % ((m_nMoveCount + 1) * 10);
	curMoveCount = 0;
	//setAnigroupFace();
	SetCurrentAnimation(MONSTER_MAP_MOVE, m_bFaceRight);
}

void NDMonster::setAnigroupFace()
{
	switch (m_nMoveDirect)
	{
	case dir_left:
	{
		m_bFaceRight = false;
		break;
	}
	case dir_right:
	{
		m_bFaceRight = true;
		break;
	}
	}

//		if (this->m_bRoleMonster) {
	m_bFaceRight = !m_bFaceRight;
	m_bReverse = !m_bReverse;
//		}
}

void NDMonster::setMonsterFrozen(bool f) // ��Ӣ�ֲ��ж���״̬,��ֹս�ܺ�������
{
	isFrozen = f;
	if (f)
	{
		frozenTime = time(0);
	}
}

void NDMonster::monsterInit()
{
	state = MONSTER_STATE_NORMAL;
	isMonsterCollide = false;
}

void NDMonster::runLogic()
{
	if (m_nMoveDirect == dir_invalid)
	{
		if (curStopCount >= stop_time_count)
		{
			if (isUseAI())
			{
				randomMonsterUseAI();
			}
			else
			{
				randomMonsterNotAI();
			}
		}
		else
		{
			curStopCount++;
			SetCurrentAnimation(MONSTER_MAP_STAND, m_bFaceRight);
		}

	}
	else
	{
		directLogic();
	}
}

void NDMonster::directLogic()
{
	int x = GetPosition().x;
	int y = GetPosition().y;
	int row = y / MAP_UNITSIZE;
	int col = x / MAP_UNITSIZE;

	//NSString *s1 = [NSString stringWithUTF8String:this->m_name.c_str()];
	//NDLog("%@",s1);

	/***
	 * ��ʱ��ע�� ����
	 * begin
	 */

// 		NDMapLayer *layer = NDMapMgrObj.getMapLayerOfScene(NDDirector::DefaultDirector()->GetRunningScene());
// 		NDMapData  *mapdata = NULL;
// 		if (!layer || !( mapdata = layer->GetMapData() )) 
// 		{
// 			//NDLog("directLogic ��ͼ����ͼ����Ϊ��");
// 			return;
// 		}
// 		
// 		CGPoint pos; pos.x = x; pos.y = y;
// 		
// 		switch (moveDirect) 
// 		{
// 			case dir_up: 
// 			{ // ��
// 				if ( (y - EVERY_MOVE_DISTANCE) >= selfMoveRectY && mapdata->canPassByRow(row - 1, col)
// 				{
// 					pos.y -= EVERY_MOVE_DISTANCE;
// 				} 
// 				else 
// 				{
// 					SetCurrentAnimation(MONSTER_MAP_STAND, m_faceRight);
// 				}
// 				break;
// 			}
// 			case dir_down: 
// 			{ // ��
// 				if ((y + BASE_BOTTOM_WH + EVERY_MOVE_DISTANCE) <= selfMoveRectY + self_move_rectH && mapdata->canPassByRow(row + 1 ,col) ) 
// 				{
// 					pos.y += EVERY_MOVE_DISTANCE;
// 					
// 				} 
// 				else 
// 				{
// 					SetCurrentAnimation(MONSTER_MAP_STAND, m_faceRight);
// 				}
// 				break;
// 			}
// 			case dir_left: 
// 			{ // ��
// 				if ( (x - EVERY_MOVE_DISTANCE) >= selfMoveRectX && mapdata->canPassByRow(row ,col - 1) ) 
// 				{
// 					pos.x -= EVERY_MOVE_DISTANCE;
// 					
// 				} 
// 				else 
// 				{
// 					SetCurrentAnimation(MONSTER_MAP_STAND, m_faceRight);
// 				}
// 				break;
// 			}
// 			case dir_right:
// 			{ // ��
// 				if ( (x + BASE_BOTTOM_WH + EVERY_MOVE_DISTANCE) <= selfMoveRectX + self_move_rectw && mapdata->canPassByRow(row ,col + 1) )
// 				{
// 					pos.x += EVERY_MOVE_DISTANCE;
// 					
// 				} 
// 				else 
// 				{
// 					SetCurrentAnimation(MONSTER_MAP_STAND, m_faceRight);
// 				}
// 				break;
// 			}
// 		}
// 		
// 		if (pos.x != x || pos.y != y) 
// 		{
// 			WalkToPosition(pos);
// 		}
// 		
// 		curMoveCount++;
// 		if (curMoveCount >= EVERY_MOVE_COUNT) 
// 		{
// 			curMoveCount = 0;
// 			curCount++;
// 			
// 			if (isAiUseful && isUseAI()) 
// 			{
// 				randomMonsterUseAI();
// 			}
// 		}
// 		if (curCount >= moveCount) 
// 		{
// 			curCount = 0;
// 			moveDirect = -1;
// 			isAiUseful = true;
// 		}
	/***
	 * ��ʱ��ע�� ����
	 * end
	 */

}

void NDMonster::LogicDraw()
{
	if (!GetParent())
	{
		/***
		 * ��ʱ��ע�� ����
		 * begin
		 */

		// 			NDMapLayer *layer = NDMapMgrObj.getMapLayerOfScene(NDDirector::DefaultDirector()->GetRunningScene());
// 			if (layer) 
// 			{
// 				layer->AddChild(this);
// 				// ��������
// 			}
		/***
		 * ��ʱ��ע�� ����
		 * end
		 */
	}
}

void NDMonster::LogicNotDraw(bool bClear /*= false*/)
{
	if (bClear)
	{
		// ��������
		//֪ͨmapmgr���Լ�ɾ��
		//NDMapMgrObj.ClearOneMonster(this);
		this->bClear = bClear;
		return;
	}

	if (GetParent())
	{
		// ��������
		GetParent()->RemoveChild(this, bClear);
	}
}

void NDMonster::doMonsterCollides()
{
	NDPlayer& player = NDPlayer::defaultHero();
	if (player.IsInState(USERSTATE_FIGHTING) || player.IsSafeProtected()
			|| player.IsInState(USERSTATE_BF_WAIT_RELIVE)
			|| player.IsInState(USERSTATE_DEAD) || player.IsGathering()
			|| (player.isTeamMember() && !player.isTeamLeader())
			|| !m_bCanBattle)
	{
		isMonsterCollide = false;
		return;
	}

	if (m_nMonsterCatogary == MONSTER_BOSS
			&& time(NULL) - m_timeBossProtect < BOSS_PROTECT_TIME)
	{
		isMonsterCollide = false;
		return;
	}

	if (player.IsInState(USERSTATE_BATTLEFIELD)
			&& this->m_eCamp == player.m_eCamp)
	{
		isMonsterCollide = false;
		return;
	}

	NDScene *scene = NDDirector::DefaultDirector()->GetRunningScene();
	if (!scene || !scene->IsKindOfClass(RUNTIME_CLASS(CSMGameScene)))
	{
		isMonsterCollide = false;
		return;
	}

	int m_nScope = m_nAttackArea * 32;

	CGRect rectRole = CGRectMake(NDPlayer::defaultHero().GetPosition().x - 32,
			NDPlayer::defaultHero().GetPosition().y - 32, 64, 64);
	CGRect rectMonster = CGRectMake(GetPosition().x - m_nScope,
			GetPosition().y - m_nScope, getCollideW() + m_nScope * 2,
			getCollideH() + m_nScope * 2);

	bool collides = CGRectIntersectsRect(rectRole, rectMonster);

	if (collides)
	{ // ����
		isMonsterCollide = true;

		//setWaitingBattle(true);

//			if ( m_nMonsterCatogary != MONSTER_GUARD )
//			{
		sendBattleAction();
//			} else 
//			{ 
		//final Dialog dialog = new Dialog(monster.monsterName,
//												 GameScreen.role.getName()
//												 + monster.getRandomDialogContent(),
//												 Dialog.PRIV_HIGH);
//				dialog.height = 208;
//				TextView operators[] = new TextView[1];
//				operators[0] = new TextView("ȷ��");
//				operators[0].id = (301);
//				operators[0].parama = monster;
//				operators[0].setOnClickListener(this);
//				dialog.setOperator(operators);
//				T.addDialog(dialog);
//			}
	}
}

int NDMonster::getCollideW()
{
	switch (m_nMonsterCatogary)
	{
	case MONSTER_NORMAL:
	{				// ��ͨ��
		return BASE_BOTTOM_WH;
	}
	case MONSTER_BOSS:
	{				// ����
		return BASE_BOTTOM_WH * 2;
	}
	}
	return BASE_BOTTOM_WH;
}

int NDMonster::getCollideH()
{
	switch (m_nMonsterCatogary)
	{
	case MONSTER_NORMAL:
	{				// ��ͨ��
		return BASE_BOTTOM_WH;
	}
	case MONSTER_BOSS:
	{				// ����
		return BASE_BOTTOM_WH * 2;
	}
	}
	return BASE_BOTTOM_WH;
}

void NDMonster::sendBattleAction()
{
	NDUISynLayer::Show();

	NDTransData bao(_MSG_BATTLEACT);

	switch (m_nMonsterCatogary)
	{
	case MONSTER_NORMAL:
	{
		bao << (unsigned char) BATTLE_ACT_CREATE; // Actionֵ
		bao << (unsigned char) 0; // btturn
		bao << (unsigned char) 1; // datacount
		bao << m_nID; // ��������Id
		break;
	}
	case MONSTER_ELITE:
	{
		bao << (unsigned char) BATTLE_ACT_CREATE_ELITE; // Actionֵ
		bao << (unsigned char) 0; // btturn
		bao << (unsigned char) 1; // datacount
		bao << m_nID; // ��������Id
		break;
	}
	case MONSTER_BOSS:
	{
		bao << (unsigned char) BATTLE_ACT_CREATE_BOSS; // Actionֵ
		bao << (unsigned char) 0; // btturn
		bao << (unsigned char) 1; // datacount
		bao << m_nID; // ����Id
		break;
	}
	default:
		return;
		break;
	}

	NDDataTransThread::DefaultThread()->GetSocket()->Send(&bao);
//		NDMapMgrObj.SetBattleMonster(this); ///< ��ʱ��ע�� ����
	NDPlayer::defaultHero().stopMoving(true);
}

void NDMonster::ElementMonsterDeal()
{
	if (!bossRing)
	{
//			bossRing = new NDSprite;
//			bossRing->Initialization([[NSString stringWithFormat:@"%@effect_101.spr", [NSString stringWithUTF8String:NDPath::GetAnimationPath().c_str()] ] UTF8String]);
//			bossRing->SetCurrentAnimation(0, false);
	}

	m_nMonsterCatogary = MONSTER_BOSS;
}

void NDMonster::setMonsterStateFromBattle(int result)
{
	switch (result)
	{
	case BATTLE_COMPLETE_WIN:
	{ // ��Ҵ�Ӯ
		setState (MONSTER_STATE_DEAD);
		break;
	}
	case BATTLE_COMPLETE_LOSE:
	case BATTLE_COMPLETE_NO:
	{ // ��Ҵ�ƽ��ս��ʧ��
		if (m_nMonsterCatogary == MONSTER_BOSS)
		{
			//setMonsterFrozen(true);
			LogicDraw();
			NDPlayer::defaultHero().setSafeProtected(true);
		}
		else
		{
			setState (MONSTER_STATE_NORMAL);
		}
		break;
	}
	}
}

void NDMonster::startBattle()
{
	LogicNotDraw();
}

void NDMonster::endBattle()
{
	if (m_nMonsterCatogary == MONSTER_BOSS)
	{
		m_timeBossProtect = time(NULL);
	}
}

void NDMonster::setState(int monsterState)
{
	state = monsterState;
	switch (state)
	{
	case MONSTER_STATE_DEAD:
	{
		deadTime = time(NULL);
		break;
	}
	case MONSTER_STATE_NORMAL:
	{
		isMonsterCollide = false;
		if (m_nMonsterCatogary == MONSTER_BOSS && this->GetParent() == NULL)
		{
			LogicDraw();
		}
		break;
	}
	case MONSTER_STATE_BATTLE:
	{
		break;
	}
	}
}

void NDMonster::OnMoving(bool bLastPos)
{

}

void NDMonster::drawName(bool bDraw)
{
	if (!m_pkSubNode)
	{
		return;
	}

	NDPlayer& player = NDPlayer::defaultHero();
	CGPoint posPlayer = player.GetPosition();
	CGRect rect1 = CGRectMake(posPlayer.x - SHOW_NAME_ROLE_W,
			posPlayer.y - SHOW_NAME_ROLE_H, SHOW_NAME_ROLE_W << 1,
			SHOW_NAME_ROLE_H << 1);
	CGRect rect2 = CGRectMake(m_kPosition.x - DISPLAY_POS_X_OFFSET,
			m_kPosition.y - DISPLAY_POS_Y_OFFSET, 16, 16);
	//if (CGRectIntersectsRect(rect1, rect2)) { // ��ʾ�����ڵĹ�������
	int iColor = 0;
	if (m_nLevel - player.m_nLevel <= LEVEL_GRAY)
	{
		iColor = 0x555555;
	}
	else if (m_nLevel - player.m_nLevel <= LEVEL_GREEN)
	{
		iColor = 0x2cf611;
	}
	else if (m_nLevel - player.m_nLevel <= LEVEL_YELLOW)
	{
		iColor = 0xffff00;
	}
	else if (m_nLevel - player.m_nLevel <= LEVEL_ORANGE)
	{
		iColor = 0xffff00;
	}
	else
	{ // ̫�߼��ú�ɫ��ʾ
		iColor = 0xf4031f;
	}
	if (m_eCamp != CAMP_NEUTRAL && m_eCamp == player.GetCamp())
	{
		iColor = 0xffffff;
	}
	else if (m_eCamp != CAMP_NEUTRAL && m_eCamp != player.GetCamp())
	{
		iColor = 0xff0000;
	}

	iColor = 0xff0000;

	int iX = m_kPosition.x - DISPLAY_POS_X_OFFSET;
	int iY = m_kPosition.y - DISPLAY_POS_Y_OFFSET;
	iX += BASE_BOTTOM_WH / 2;
	//iY += BASE_BOTTOM_WH-getGravityY();
	iY -= getGravityY();

	CGSize size = getStringSize(m_strName.c_str(), 12);
	CGSize sizemap;
	sizemap = m_pkSubNode->GetContentSize();

	if (!m_lbName)
	{
		m_lbName = new NDUILabel;
		m_lbName->Initialization();
		m_lbName->SetFontSize(12);
		m_lbName->SetText(m_strName.c_str());
	}

	if (!m_lbName->GetParent() && m_pkSubNode)
	{
		m_pkSubNode->AddChild(m_lbName);
	}

	m_lbName->SetFontColor(INTCOLORTOCCC4(iColor));

	m_lbName->SetFrameRect(
			CGRectMake(iX - size.width / 2,
					iY + NDDirector::DefaultDirector()->GetWinSize().height
							- sizemap.height, size.width, size.height + 5));

	if (bDraw)
	{
		m_lbName->draw();
	}
	//}
}

void NDMonster::SetCanBattle(bool bCanBattle)
{
	m_bCanBattle = bCanBattle;
}

bool NDMonster::CanBattele()
{
	return m_bCanBattle;
}

void NDMonster::changeLookface(int lookface)
{
//		if (lookface >= 20000 && lookface <= 20070 && lookface != 20030) {
//			turnFace = true;
//		} else {
//			turnFace = false;
//		}

	if (isRoleMonster(lookface))
	{
		m_bRoleMonster = true;
		{	//InitNonRoleData
			//m_id = 0; // �û�id
			m_nSex = lookface / 100000000 % 10; // �����Ա�1-���ԣ�2-Ů�ԣ�
			m_nDirect = lookface % 10;
			m_nHairColor = lookface / 1000000 % 10;
			int tmpsex = (m_nSex - 1) / 2 - 1;
			if (tmpsex < 0 || tmpsex > 2)
			{
				tmpsex = 0;
			}
			this->SetHair(tmpsex + 1); // ����

			SetHairImageWithEquipmentId (m_nHair);

			int flagOrRidePet = lookface / 10000000 % 10;
			if (flagOrRidePet > 0)
			{
				if (flagOrRidePet < 5)
				{
					m_eCamp = CAMP_TYPE(flagOrRidePet);
				}
				else
				{
					int id = (flagOrRidePet + 1995) * 10;
					SetEquipment(id, 0);
				}
			}

			m_nWeapon = getEquipmentLookFace(lookface, 0);
			m_nCap = getEquipmentLookFace(lookface, 1);
			m_nArmor = getEquipmentLookFace(lookface, 2);
			SetEquipment(m_nWeapon, 0); //����
			SetEquipment(m_nCap, 0); //ͷ��
			SetEquipment(m_nArmor, 0); //�ؼ�

			//Load Animation Group
			CC_SAFE_DELETE (m_pkAniGroup);

			/***
			 * ��ʱ��ע�� ����
			 * begin
			 */
// 				if (sex % 2 == SpriteSexMale)
// 					m_aniGroup = [[NDAnimationGroupPool defaultPool] addObjectWithSpr:[NSString stringWithUTF8String:MANUELROLE_HUMAN_MALE]];
// 				else
// 					m_aniGroup = [[NDAnimationGroupPool defaultPool] addObjectWithSpr:[NSString stringWithUTF8String:MANUELROLE_HUMAN_FEMALE]];
			/***
			 * ��ʱ��ע�� ����
			 * end
			 */

			m_bFaceRight = m_nDirect == 2;
			SetFaceImageWithEquipmentId (m_bFaceRight);
			SetCurrentAnimation(MANUELROLE_STAND, m_bFaceRight);

			defaultDeal();
		}
	}
	else
	{
		{ // SetNormalAniGroup
			if (lookface <= 0)
			{
				return;
			}

			m_nColorInfo = lookface / 100000 % 10;

			if (m_nColorInfo == 0)
				m_nColorInfo = -1;

//				if (m_aniGroup) 
//				{
//					[m_aniGroup release];
//				}
//				
//				m_aniGroup = [[NDAnimationGroupPool defaultPool] addObjectWithSpr:
//							  [NSString stringWithFormat:@"%@%d%s", [NSString stringWithUTF8String:NDPath::GetAnimationPath().c_str()], (lookface % 100000) / 10, ".spr"]
//							  ];

			m_bFaceRight = lookface % 10 == 2;
			SetCurrentAnimation(MANUELROLE_STAND, m_bFaceRight);
		}
	}

	SetNonRole(!isRoleMonster(lookface));
}
