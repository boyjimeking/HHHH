//
//  NDUINode.h
//  DragonDrive
//
//  Created by xiezhenghai on 10-12-28.
//  Copyright 2010 (����)DeNA. All rights reserved.
//
//
//	�������ܣ���
//	NDUINode������ui�ؼ��Ļ��࣬�̳��Ը���Ҳ������ʾ������ʽ����Initialization����
//  modify by yay 12-1-12
//  add lua event callback


#ifndef __NDUINode_H
#define __NDUINode_H

#include "NDNode.h"

namespace NDEngine
{	
	class NDUINode;
	
	class NDSection;
	
	class NDUITargetDelegate
	{
		DECLARE_AUTOLINK(NDUITargetDelegate)
		INTERFACE_AUTOLINK(NDUITargetDelegate)
		
	public:	
		
		NDUITargetDelegate();
		virtual ~NDUITargetDelegate();
		
		virtual bool OnTargetBtnEvent(NDUINode* uinode, int targetEvent);
		virtual bool OnTargetTableEvent(NDUINode* uinode, NDUINode* cell, unsigned int cellIndex, NDSection *section);
	};
	
	////////////////////////////////////////////////////////////
	class NDUINode : public NDNode
	{
		DECLARE_CLASS(NDUINode)
	public:
		NDUINode();
		~NDUINode();
	public:
//		
//		������Initialization
//		���ã���ʼ��ui�ڵ㣬���뱻��ʾ����ʽ����
//		��������
//		����ֵ����
		void Initialization(); override
//		
//		������SetFrameRect
//		���ã�����ui����ڸ��׽ڵ�ľ�����ʾ��Χ��Ĭ��ֵΪ��0�� 0�� 0�� 0��
//		������rect������ʾ��Χ
//		����ֵ����
		virtual void SetFrameRect(CGRect rect);
		virtual void FlyToRect(CGRect rect, int nFrameNum, int nDirect);
//		
//		������GetFrameRect
//		���ã���ȡui�ڵ�����ڸ��׽ڵ�ľ��η�Χ��
//		��������
//		����ֵ�����η�Χ
		virtual CGRect GetFrameRect();		
//		
//		������SetVisible
//		���ã����øýڵ��Ƿ���ӣ������ӽڵ�Ҳͬʱ������
//		������visible���true���ӣ����򲻿���
//		����ֵ����
		virtual void SetVisible(bool visible);
//		
//		������IsVisibled
//		���ã��жϸýڵ��Ƿ����
//		��������
//		����ֵ��true���ӣ����򲻿���
		virtual bool IsVisibled();
//		
//		������EnableEvent
//		���ã����ýڵ��¼��Ƿ���Ӧ
//		������enabled���true��Ӧ��������Ӧ
//		����ֵ����
		void EnableEvent(bool enabled);
//		
//		������EventEnabled
//		���ã��жϽڵ��Ƿ���Ӧ�¼�
//		��������
//		����ֵ��true��Ӧ��������Ӧ
		bool EventEnabled();
//		
//		������GetScreenRect
//		���ã���ȡ�ڵ��������Ļ�ľ��η�Χ
//		��������
//		����ֵ�����η�Χ		
		CGRect GetScreenRect();	
//		
//		������OnFrameRectChange
//		���ã����ڵ�ľ��η�Χ�ı�ʱ������ڲ�����
//		������srcRect�ı�ǰ�ľ��η�Χ��dstRect�ı��ľ��η�Χ���������Ļ
//		����ֵ����		
		virtual void OnFrameRectChange(CGRect srcRect, CGRect dstRect);
	public:		
		void draw(); override

	private:		
		bool m_bVisibled;
		CGRect m_kFrameRect;
		bool m_bEventEnabled;
		CGRect m_kScrRectCache; //for cache

		float  m_fStep;  //����
		int  m_nStepNum; //����
		int  m_nDirect;//����
		float m_fBoundScale;
		
	public:
		void SetTargetDelegate(NDUITargetDelegate* targetDelegate);
		NDUITargetDelegate* GetTargetDelegate();
		
		void SetLuaDelegate(LuaObject func);
		bool GetLuaDelegate(LuaObject& func);

		bool OnScriptUiEvent(NDUINode* uinode, int targetEvent);

		template<typename T>
		bool OnScriptUiEvent(NDUINode* uinode, int targetEvent, T param)
		{
			if (!uinode)
			{
				return false;
			}

			LuaObject funcObj;

			if (!uinode->GetLuaDelegate(funcObj)
				|| !funcObj.IsFunction())
			{
				return false;
			}

			LuaFunction<bool> luaUiEventCallBack = funcObj;

			//bool bRet = luaUiEventCallBack(uinode, targetEvent, param); ///< ��ʱ��ע�� ����

			return true;//bRet;///< ��ʱ��ע�� ����
		}
		
		/*
		void registerLuaClickFunction(const char* szFunc);
		void unregisterLuaClickFunction();
		void callLuaFunction();
		bool isHaveLuaFunc()
		{
			return m_strLuaFunc.size();
		}*/

		
	private:
		CAutoLink<NDUITargetDelegate> m_delegateTarget;
		std::string		m_strLuaFunc;
		
		LuaObject		m_delegateLua;
	};
}
#endif
