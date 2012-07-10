/**
��־��
Copyright(c) 1999-2012, TQ Digital Entertainment, All Rights Reserved
**/
#pragma once
#ifdef WIN32
#include <direct.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
using namespace std;

/**
��DEBUG��,ʹ�÷���:
Ĭ����DebugĿ¼�±�����"TQFramework+��������.log",��־����Ϊ0
ʹ�÷���:
TQLOG(1, "δ���ִ���[%d]������Ϣ", GetTemplateID());
**/
namespace TQFramework
{
	class CLog
	{
	public:
		CLog(const char* lpszDir/*�����Ŀ¼*/, const char* lpszPrefix/*�ļ���(����������)*/, unsigned int nLogLevel/*��־����*/);
		~CLog(void);

		//�����־
		void Log(unsigned int nLogLevel/*��־����*/, const char* fmt/*��ʽ���ı�*/, ...);

		void LogNoTimeTag(unsigned int nLogLevel, const char* fmt, ...);

		//������־����
		void SetLogLevel(unsigned int nLevel);

		//��ȡ��־����
		unsigned int GetLogLevel();
	private:
		bool OpenLog(const char* lpszDir, const char* lpszPrefix);

		bool CloseLog();

		bool MakePath(const char* lpszDir, char* pFullPath, unsigned int nFullPathLen);

	private:
		FILE*	m_fpFile;
		string	m_sLogDir;
		string	m_sPrefix;

		time_t	m_tTimeLogBegin;
		time_t	m_tTimeLogEnd;
		unsigned int	m_nLogLevel;//��־����(��TQLOG�ļ���С���趨�ļ���,�򲻱���)
	};
}
#endif

#ifdef _DEBUG
extern TQFramework::CLog g_logTq;
#define TQLOG	g_logTq.Log
#else
#define TQLOG	//
#endif