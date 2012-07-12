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

enum HURT_TYPE {
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
	
	Hurt(Fighter* actor, int btType, int hurtHP, int hurtMP, int dwData, HURT_TYPE ht);
	
	Fighter* theActor;// ��˭��ģ������״̬��Ѫ����Ҫ��ָ����˭��ġ�
	int btType;// �Ժ��ַ�ʽ�˺�
	int hurtHP;// ���˶���Ѫ
	int hurtMP;// ���˶�����
	int dwData;// ��ӦdwData��0-��ͨ�˺���������ֵ-�ض�����Ч���˺���
	HURT_TYPE type;// �����˺����Ǳ���Ч���˺�
};

enum {
	EXIST_TIME_MAX = 10,
};

class HurtNumber {
public:
	HurtNumber(int num) {
		hurtNum = num;
		existTime = EXIST_TIME_MAX;
		hurtNumberY = 0;
		disappear = false;
	}
	
	void timeLost() {
		if (existTime < EXIST_TIME_MAX) {
			existTime--;
			if (existTime > EXIST_TIME_MAX - 3) {
				hurtNumberY += 2;
			}
			if (existTime == 0) {
				disappear = true;
				existTime = EXIST_TIME_MAX;
			}
		}
	}
	
	bool isNew() {
		return existTime == EXIST_TIME_MAX ? true : false;
	}
	
	void beginAppear() {
		existTime = EXIST_TIME_MAX - 1;
		hurtNumberY = 1;
	}
	
	bool isDisappear() {
		return disappear;
	}
	
	void setDisappear(bool bDis) {
		disappear = bDis;
	}
	
	int getHurtNum() {
		return hurtNum;
	}
	
	void setHurtNum(int hn) {
		hurtNum = hn;
	}
	
	int getHurtNumberY() {
		return hurtNumberY;
	}
	
	void setHurtNumberY(int hnY) {
		hurtNumberY = hnY;
	}
	
private:
	int hurtNum;
	int existTime;
	
	bool disappear;
	int hurtNumberY;
};

#endif