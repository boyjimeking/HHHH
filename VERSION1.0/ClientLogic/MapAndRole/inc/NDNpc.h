//
//  NDNpc.h
//  DragonDrive
//
//  Created by xiezhenghai on 10-12-15.
//  Copyright 2010 (����)DeNA. All rights reserved.
//

#ifndef __NDNpc_H
#define __NDNpc_H

#include "NDBaseRole.h"
#include <string>
#include "EnumDef.h"
#include "NDUILabel.h"
#include "NDPicture.h"
#include "globaldef.h"

namespace NDEngine 
{
	/** btStateΪ1ʱ��ɫ̾��,2��ɫ̾��,3��ɫ�ʺ�,4��ɫ�ʺ� */
	enum 
	{
		QUEST_NONE = 0x01,// û������
		QUEST_CANNOT_ACCEPT = 0x02,// �������Ǽ��𲻶�
		QUEST_CAN_ACCEPT = 0x04, // ������ɽ�
		QUEST_NOT_FINISH = 0x08, // ������δ���
		QUEST_FINISH = 0x10, // �����������(���ȼ����)
	};
	
	class NDNpc : public NDBaseRole 
	{
		DECLARE_CLASS(NDNpc)
	public:
		NDNpc();
		~NDNpc();
		void Init(); override				
		bool OnDrawBegin(bool bDraw); override
		void OnDrawEnd(bool bDraw); override
		
	public:
		//���·������߼���ʹ�ã�����begin
		//......	
		void Initialization(int lookface); hide
		
		void WalkToPosition(CGPoint toPos);
		//������end
		//void SetHairImage(int hair, int hairColor);
		void SetExpresstionImage(int nExpresstion);
		
		// ����npc������ʾ
		void SetNpcState(NPC_STATE state);
		
		NPC_STATE GetNpcState() const {
			return this->npcState;
		}
		
		void SetStatus(int status);
		
		// ����,�����ȡ��ֱ�ӷ���ridePet
		//NDRidePet* GetRidePet();
		
		void SetType(int iType);
		int GetType();
		
		enum  LableType{ eLableName, eLabelDataStr, };
		void SetLable(LableType eLableType, int x, int y, std::string text, cocos2d::ccColor4B color1, cocos2d::ccColor4B color2);
		
		bool IsRoleNpc() {
			return this->m_bRoleNpc;
		}
		
		bool IsPointInside(CGPoint point);
		
		bool getNearestPoint(CGPoint srcPoint, CGPoint& dstPoint);
		
		void ShowHightLight(bool bShow);
	public:
		int col;
		int row;
		int look;
		int model;
		
	private:
		 NPC_STATE npcState;
	public:
		// ������
		//NDRidePet	*ridepet;
		std::string dataStr;
		std::string talkStr;

	private:
		NDUILabel *m_lbName[2], *m_lbDataStr[2];
		NDPicture *m_picBattle, *m_picState;
		int m_iStatus;
		// role�� npc
		bool m_bRoleNpc;
		int m_iType;
		CGRect m_rectState;

	private:
		
		void RefreshTaskState();
		bool GetTaskList(ID_VEC& idVec);
		int GetDataBaseData(int nIndex);
		bool GetPlayerCanAcceptList(ID_VEC& idVec);
	};
}

#endif