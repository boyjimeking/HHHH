/*
 *  Hurt.h
 *  DragonDrive
 *
 *  Created by wq on 11-1-25.
 *  Copyright 2011 (����)DeNA. All rights reserved.
 *
 */

#ifndef __BATTLE_HURT_H__
#define __BATTLE_HURT_H__

#include "string.h"

enum HURT_TYPE
{
	HURT_TYPE_ACTIVE = 0,
	HURT_TYPE_PASSIVE = 1,
};

class Fighter;
struct Hurt
{
	Hurt()
	{
		memset(this, 0L, sizeof(Hurt));
	}

	Hurt(Fighter* actor, int btType, int hurtHP, int hurtMP, int dwData,
			HURT_TYPE ht);

	Fighter* theActor; // ��˭��ģ������״̬��Ѫ����Ҫ��ָ����˭��ġ�
	int btType; // �Ժ��ַ�ʽ�˺�
	int hurtHP; // ���˶���Ѫ
	int hurtMP; // ���˶�����
	int dwData; // ��ӦdwData��0-��ͨ�˺���������ֵ-�ض�����Ч���˺���
	HURT_TYPE type; // �����˺����Ǳ���Ч���˺�
};

enum
{
	EXIST_TIME_MAX = 10,
};

class HurtNumber
{
public:
	HurtNumber(int num)
	{
		m_nHurtNum = num;
		m_nExistTime = EXIST_TIME_MAX;
		m_nHurtNumberY = 0;
		m_bIsDisappear = false;
	}

	void timeLost()
	{
		if (m_nExistTime < EXIST_TIME_MAX)
		{
			m_nExistTime--;
			if (m_nExistTime > EXIST_TIME_MAX - 2)
			{
				m_nHurtNumberY += 1;
			}
			if (m_nExistTime == 0)
			{
				m_bIsDisappear = true;
				m_nExistTime = EXIST_TIME_MAX;
			}
		}
	}

	bool isNew()
	{
		return m_nExistTime == EXIST_TIME_MAX ? true : false;
	}

	void beginAppear()
	{
		m_nExistTime = EXIST_TIME_MAX - 1;
		m_nHurtNumberY = 1;
	}

	bool isDisappear()
	{
		return m_bIsDisappear;
	}

	void setDisappear(bool bDis)
	{
		m_bIsDisappear = bDis;
	}

	int getHurtNum()
	{
		return m_nHurtNum;
	}

	void setHurtNum(int hn)
	{
		m_nHurtNum = hn;
	}

	int getHurtNumberY()
	{
		return m_nHurtNumberY;
	}

	void setHurtNumberY(int hnY)
	{
		m_nHurtNumberY = hnY;
	}

private:
	int m_nHurtNum;
	int m_nExistTime;

	bool m_bIsDisappear;
	int m_nHurtNumberY;
};

#endif
