//
//  NDMonster.h
//  DragonDrive
//
//  Created by jhzheng on 10-12-31.
//  Copyright 2010 (����)DeNA. All rights reserved.
//

#ifndef __ND_MONSTER_H
#define __ND_MONSTER_H

#include "NDBaseRole.h"
#include "NDUILabel.h"
#include <string>

namespace NDEngine
{

#define	NORMAL_MOVE_RECTW 90
#define	NORMAL_MOVE_RECTH 90 // ��ͨ��������Ļ��Χwe
#define	BOSS_MOVE_RECTW	60
#define	BOSS_MOVE_RECTH 50 // ��Ӣ��������Ļ��Χwe
#define BASE_BOTTOM_WH 32

#define	REFRESH_TIME 90  // ��ͨ���������ˢ��ʱ��
#define	FROZEN_TIME 10

#define	MONSTER_VIEW_AI 5 //������Ұ������,���1����3*3,2����5*5,��������
#define MOVE_DISTANCE 32 // ��ÿ������
#define EVERY_MOVE_COUNT 4 // ��ÿ���ֳɶ��ٴ�����,��ϳ���ˢ��ʱ��������֡����
#define EVERY_MOVE_DISTANCE ( MOVE_DISTANCE / EVERY_MOVE_COUNT ) // ÿһ֡���߾���
#define MOVE_MIN 2 // ��һ����С����,��������ļ���,
#define BOSS_PROTECT_TIME (10)

enum
{
	MONSTER_STATE_DEAD = 0, // �����Ѿ�������
	MONSTER_STATE_NORMAL = 1, // ��������
	MONSTER_STATE_BATTLE = 2, // ����������ս����
};

enum
{
	LEVEL_GRAY = -7, // �������
	LEVEL_GREEN = -2,
	LEVEL_YELLOW = 2,
	LEVEL_ORANGE = 5,
};

enum
{
	MONSTER_NORMAL = 0, // ��ͨ��
	MONSTER_ELITE,		// ��Ӣ��
	MONSTER_BOSS,		// BOSS��
	MONSTER_Farm,		// ׯ԰
};

// 0����,1����,2��, 3�� ,-1���޷���
enum
{
	dir_up = 0, dir_down = 1, dir_left = 2, dir_right = 3, dir_invalid = -1,
};

class NDMonster: public NDBaseRole
{
	DECLARE_CLASS (NDMonster)
public:
	static bool isRoleMonster(int lookface);
public:
	NDMonster();
	~NDMonster();
	//void Init(); override
	virtual void OnMoving(bool bLastPos);
public:
	//���·������߼���ʹ�ã�����begin
	//......
	void Initialization(int idType);hide

	void Initialization(int lookface, int idNpc, int lvl);

	bool OnDrawBegin(bool bDraw);override

	void MoveToPosition(CGPoint toPos, SpriteSpeed speed = SpriteSpeedStep4,
			bool moveMap = false);hide

	void WalkToPosition(CGPoint toPos);

	void Update(unsigned long ulDiff);override

	void SetType(int type);
	int GetType()
	{
		return m_nMonsterCatogary;
	}

	//����Ӣ�ִ���
	void ElementMonsterDeal();
	//������end
public:
	void setMonsterStateFromBattle(int result);
	void startBattle();
	void endBattle();
	int getState()
	{
		return m_nState;
	}
	void restorePosition();
	void changeLookface(int lookface);
	void SetCanBattle(bool bCanBattle);
	void SetMoveRect(CGPoint point, int size);
	//CGRect GetMoveRect(){return CGRectMake(selfMoveRectX, selfMoveRectY, self_move_rectW, self_move_rectH);}
	bool CanBattele();
private:
	bool isUseAI();
	int getRandomAIDirect();
	void randomMonsterUseAI();
	void randomMonsterNotAI();
	void setAnigroupFace();
	void setMonsterFrozen(bool f);	// ��Ӣ�ֲ��ж���״̬,��ֹս�ܺ�������
	void monsterInit();				// ��������һ��״̬
	void runLogic();
	void directLogic();
	void LogicDraw();
	void LogicNotDraw(bool bClear = false);
	void doMonsterCollides();
	int getCollideW();
	int getCollideH();
	void sendBattleAction();
	void drawName(bool bDraw);
public:
	void setState(int monsterState);
	void setOriginalPosition(int o_x, int o_y);
public:
	int m_nState;						//����״̬
private:
	int m_nAttackArea;					//���﹥����Χ
	int m_nType;						//��������
	CGSize m_kMoveSize;
	int m_nFigure;						// ���ͣ�0-Сֻ��1-��ֻ��
	NDSprite* m_pkBossRing;				//��Ӣ�ֹ⻷
	NDUILabel *m_lbName;
	bool m_bIsHunt;
	int m_nOriginal_x;
	int m_nOriginal_y;
public:
	int m_nMoveDirect;
	bool m_bIsAIUseful; // AI�㷨�Ƿ����������޷��߶�,�ǵĻ�����ͨ�߷�
	bool m_bRoleMonster;

	bool m_bIsAutoAttack; // ������������
	int m_nMonsterCatogary;

	int m_nSelfMoveRectW;
	int m_nSelfMoveRectH; // ������Χwe
	int m_nSelfMoveRectX;
	int m_nSelfMoveRectY; // ����������Χ�����Ͻ�xy

	bool m_bIsMonsterCollide; // ��ʾ������ײ�����˶�
	bool m_bIsFrozen; // ��Ӣ�ֲ��ж���״̬

	long m_nDeadTime;
	long m_nFrozenTime; // ��������ʱ��,��Ӣ�ֶ���

private:

	int m_nMoveCount; // �л�������ó��Ĳ���
	int m_nStopTimeCount; // ÿ�ι�����·���̶�ͣ��һ��ʱ��
	int m_nCurStopCount; // ��ǰͣ��
	int m_nCurMoveCount; // ���EVERY_MOVE_COUNT, ��ÿһ����ֶ��С��

	int m_nCurCount; // ��ǰ����

	/** ĳЩ��Դ�����෴�����¶����ƶ����������� */
	bool m_bTurnFace;

	bool m_bCanBattle;

	//����,û�õ�
	int m_nMoveRectW;
	int m_nMoveRectH;

	double m_dTimeBossProtect;

public:
	//�Ƿ���ս�����
	bool m_bBattleMap;
	bool m_bIsSafeProtected;
	bool m_bClear;

	DECLARE_AUTOLINK (NDMonster)
	INTERFACE_AUTOLINK (NDMonster)
};
}

#endif // __ND_MONSTER_H
