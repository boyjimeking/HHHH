/*
 *  ScriptGameLogic.h
 *  SMYS
 *
 *  Created by jhzheng on 12-2-13.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once
#include <string>
using namespace std;

typedef struct _STRU_PLAYER_INFO
{
	int m_iId;                    //����ID
	int m_iLookFace;               //��������ID(ָ���Ӧ��ͼƬ��
	int m_iBornX;                  //������X���꣨��λ��
	int m_iBornY;                  //������Y����
	int m_iRideStatus;              //��˵�״̬
	int m_iRideType;               //��˵�����
	std::string m_strName;              //���������
}stuPlayerInfo;

namespace NDEngine {

	void SetPlayerInfo(int iId, int iLookFace, int iBornX, int iBornY, int iRideStatus, int iRideType, std::string strName);
	void GetPlayerInfo(stuPlayerInfo &stuinfo);
	void CreatePlayerWithMount(int lookface, int x, int y, int userid, std::string name, int nRideStatus=0, int nMountType=0 );

	unsigned long GetPlayerId();
	unsigned long GetMapId();
	int GetCurrentMonsterRound();
	int GetPlayerLookface();
	std::string GetSMImgPath(const char* name);
	void ScriptGameLogicLoad();
	
}
