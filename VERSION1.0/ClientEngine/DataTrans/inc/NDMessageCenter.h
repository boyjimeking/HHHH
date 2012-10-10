//
//  NDMessageCenter.h
//  DragonDrive
//
//  Created by xiezhenghai on 10-12-8.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//
/*
 ��Ϣ����
*/
	
#ifndef __NDMessageCenter_H
#define __NDMessageCenter_H

#include "NDObject.h"
#include <deque>
#include <string>
#include "NDTransData.h"
#include "CircleBuffer.h"
#include "KMutex.h"

namespace NDEngine
{
	class NDMessageCenter : public NDObject
	{
		DECLARE_CLASS(NDMessageCenter)
	public:
		NDMessageCenter();
		~NDMessageCenter();
		struct _transdata 
		{
			NDTransData* data;
		};
	public:
//		
//		������DefaultMessageCenter
//		���ã�������̬��������Ա�����ķ����붼ͨ���ýӿ��ȡ�
//		��������
//		����ֵ��������ָ��
		static NDMessageCenter* DefaultMessageCenter();		
//		
//		������AddMessage
//		���ã������Ϣ����Ϣ�б�
//		������data�ӷ��������յ���һ����Ϣ
//		����ֵ����
		void AddMessage(NDTransData* data);
//		
//		������CanAddMessage
//		���ã��ж϶����Ƿ�������Ϣ
//		��������
//		����ֵ��true����� false�������
		bool CanAddMessage();
		
//		������GetMessage
//		���ã�����Ϣ���л�ȡһ����Ϣ����ȡ��ɶ��н����ٱ����������ݣ������ⲿ�����������
//		��������
//		����ֵ��ʧ�ܷ���null
		NDTransData* GetMessage();
		
		
		//void DelMessage();
	private:
		//std::deque<NDTransData*> m_messageQueue;
		CRingBuffer<_transdata> m_messageQueue;
	};
	
	//#pragma mark ������Ϣ������(֧�ֶ��߳�)
	
	// ����:������Ϣ������(֧�ֶ��߳�)
	// �ṩ��������ԭʼ���ݹ���
	// �ṩ��ȡ��������Ϣ������
	// �ṩ���ӻ���������Ϣ������
	// �ṩ�����������ԭʼ���ݹ���
	// �ṩ���ٱ���������
	// �ṩ���������������������
	// jhzhen 2011.12.1
	
	class NDNetMsgMgr
	{
		public:
			static NDNetMsgMgr&	GetSingleton();
			
			static void purge();
			
			~NDNetMsgMgr();
			
			bool AddNetRawData(const unsigned char* data, unsigned int uilen, bool net = true);
			
			bool GetServerMsgPacket(NDTransData& data);
			
			bool AddBackToMenuPacket();
			
			bool ClearNetRawData();
			
			void Report();
			
		private:
			CRingBuffer<unsigned char, 1046 * 256, 1046> m_buffer;
			
			unsigned int m_uiTotalValidRecv, m_uiTotalInvalidRecv;
			
			unsigned int m_uiTotalNetData;
			
			KMutex m_lockNetData;
		private:
			NDNetMsgMgr();
			
	};
	
}

#endif
