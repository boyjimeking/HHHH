//  NDDataTransThread.h
//  DragonDrive
//
//  Created by xiezhenghai on 10-12-21.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//
/*
 ���շ�������ݵ��߳�
 ������
 */

#ifndef __NDDataTransThread_H
#define __NDDataTransThread_H

#include "NDObject.h"
#include "NDSocket.h"
#include <vector>
#include <string>

namespace NDEngine
{
	typedef enum
	{
		ThreadStatusStoped,
		ThreadStatusPaused,
		ThreadStatusRunning
	}ThreadStatus;
	
	class NDDataTransThread : public NDObject
	{
		DECLARE_CLASS(NDDataTransThread)
		NDDataTransThread();
		~NDDataTransThread();		
	public:
		//		
		//		������DefaultThread
		//		���ã��������������������ĵ�������ڴ�
		//		��������
		//		����ֵ��
		static NDDataTransThread* DefaultThread();
		static void               ResetDefaultThread();
		//		
		//		������Start
		//		���ã������߳�
		//		������ipAddr��������ַ port�˿ں�
		//		����ֵ����
		void Start(const char* ipAddr, int port);
		//		
		//		������Stop
		//		���ã�ֹͣ�߳�
		//		��������
		//		����ֵ����
		void Stop();
#ifndef __USE_CFSOCKET__
		//		
		//		������Pause
		//		���ã���ͣ�߳�
		//		��������
		//		����ֵ����Ļ��С
		void Pause();
		//		
		//		������Resume
		//		���ã������̣߳�ֻ������ͣ����²���Ч
		//		��������
		//		����ֵ����Ļ��С
		void Resume();
#endif
		//		
		//		������GetThreadStatus
		//		���ã���ȡ�߳�״̬
		//		��������
		//		����ֵ����
		ThreadStatus GetThreadStatus();		

		//		
		//		������GetSocket
		//		���ã���ȡ�׽��֣�һ��������������
		//		��������
		//		����ֵ����Ļ��С
		NDSocket* GetSocket();	
        
		//		
		//		������GetQuitGame
		//		���ã���ȡ���߱�־
		//		��������
		//		����ֵ���Ƿ���� 
        bool GetQuitGame();
        
        //      ������SetQuitGame
        //      ���ã����ö��߱�־
        //      ��������־λ
        //      ����ֵ����
        void SetQuitGame(bool bQuitGame);
        
	public:
		//		
		//		������Execute
		//		���ã��߳��ڲ�ִ���壬 �ⲿ�������
		//		��������
		//		����ֵ����Ļ��С
		void Execute();
        void ChangeCode(DWORD dwCode);
	private:
		NDSocket* m_socket;
		ThreadStatus m_status, m_operate;	
        bool m_bQuitGame;
		void WaitStatus(ThreadStatus status);
		void BlockDeal();
		void NotBlockDeal();
	};
}

#endif
