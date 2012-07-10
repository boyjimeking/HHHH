/******************************************************************************
 *  File Name:		NDDataTransThread.h
 *  Author:			Guosen
 *  Create Date:	2012-4-9
 *  
 *****************************************************************************/

#ifndef _NDDATATRANSTHREAD_H_2012_04_09_10_32_57_
#define _NDDATATRANSTHREAD_H_2012_04_09_10_32_57_
/////////////////////////////////////////////////////////////////////////////

#include "NDObject.h"
#include "NDSocket.h"
#include <vector>
#include <string>
#include <pthread.h>

/////////////////////////////////////////////////////////////////////////////
//
namespace NDEngine
{
	typedef enum
	{
		ThreadStatusStoped,
		ThreadStatusPaused,
		ThreadStatusRunning
	}ThreadStatus;
	
/*
 ���շ�������ݵ��߳�
 ������
 */
	class NDDataTransThread : public NDObject
	{
		DECLARE_CLASS(NDDataTransThread)
		NDDataTransThread();
		~NDDataTransThread();

	public:
		//		������DefaultThread
		//		���ã��������������������ĵ�������ڴ�
		//		��������
		//		����ֵ��		
		static NDDataTransThread* DefaultThread();
			
		//		������Start
		//		���ã������߳�
		//		������ipAddr��������ַ port�˿ں�
		//		����ֵ����
		void Start(const char* ipAddr, int port);
		
		//		������Stop
		//		���ã�ֹͣ�߳�
		//		��������
		//		����ֵ����
		void Stop();
		
		//		������GetThreadStatus
		//		���ã���ȡ�߳�״̬
		//		��������
		//		����ֵ����
		ThreadStatus GetThreadStatus();		

		//		������GetSocket
		//		���ã���ȡ�׽��֣�һ��������������
		//		��������
		//		����ֵ����Ļ��С
		NDSocket* GetSocket();	
		
	public:
		//		������Execute
		//		���ã��߳��ڲ�ִ���壬 �ⲿ�������
		//		��������
		//		����ֵ����Ļ��С
		void Execute();

	private:
		NDSocket* m_socket;
		ThreadStatus m_status, m_operate;	
		void WaitStatus(ThreadStatus status);
		void BlockDeal();
		void NotBlockDeal();
	};
}

/////////////////////////////////////////////////////////////////////////////
#endif //_NDDATATRANSTHREAD_H_2012_04_09_10_32_57_
