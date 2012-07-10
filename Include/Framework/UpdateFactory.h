#pragma once

/*
��װ�ĸ���ģ��
Copyright(c) 1999-2012, TQ Digital Entertainment, All Rights Reserved
*/

#ifndef __NUPDATEFACTORY__HEAD__FILE__
#define __NUPDATEFACTORY__HEAD__FILE__
#include "NetFactory.h"
#include "TQzip.h"
#include "BaseType.h"
#include "IAsynTaskManager.h"

//���ӿ�Ϊ���µ����ػص�
class IUpdateEvent
{
public:
	enum ERROR_CODE{
		ERRCODE_NONE=0,//������ɹ�
		ERRCODE_CONECTION_FAILED,//����ʧ��
		ERRCODE_CONECTION_SERVERERR,//�������ڲ�����
		ERRCODE_FILE_SAVEFAIL,//�ļ�����ʧ��
		ERRCODE_VERSION_LATEST,//�Ѿ������µİ汾
		ERRCODE_VERSION_NOPACKAT,//û�пɸ��µİ汾
		ERRCODE_UNCOMPRESS_FAILED//��ѹʧ��
	};
public:
	//���������ع��̵Ļص�
	//������dwSizeFile ���ļ���С
	//������dwSideDownLoaded �Ѿ����ص��ļ���С
	virtual void OnDownloadEvent(DWORD dwSizeFile,DWORD dwSideDownLoaded){};


	//��������Դ��ѹ ע��ֻ�ܽ�ѹ����Զ������Դ������Դ���꿴CTQZip
	//������nFileNum ��Դ���е��ļ���
	//������nFileIndex ��ѹ����nFileIndex���ļ�
	//������pszFileName ���ڽ�ѹ���ļ���
	virtual void OnUnCompress(int nFileNum,int nFileIndex,const char* pszFileName){};

	//���������»����ز�������ʱ�Ļص�
	virtual void CompleteUpdate(ERROR_CODE emErrCode){ };

	//����������ʧ�ܵĻص�
	//������emErrCode �������
	//������pszErrMsg ������ʾ
	virtual void OnError(ERROR_CODE emErrCode,const char* pszErrMsg){};
	~IUpdateEvent(){};
};
class HttpClientSocket;
class CUpdateFactory:public INetEvnet,public ITQZipEvent
{
public:
	enum UPDATE_MODE
	{
		UM_ASP,
		UM_STATIC,
	};
public:
	//���������µ�ʵ������
	static CUpdateFactory& sharedInstance();

	//��������ʼ���������� ���������ֻ��һ��
	//������pszServerUrl ������·�������ַ��HTTP URL��
	//������pszAppName ����APP��Դ����
	//������pszOutSourcePath ���������Դ���µ�·��
	//������pUpdateEvent  ���»����صĻص�
	//����ֵ�� true ��ʾ�ɹ���ʼ����false��ʾ��ʼ��ʧ��
	bool Init(const char*pszServerUrl,const char* pszAppName,const char *pszOutSourcePath,IUpdateEvent *pUpdateEvent);

	//����������ģʽ
	//������emUpdateMode ���µ�ģʽ
	void SetUpdateMode(UPDATE_MODE emUpdateMode);

	//�������ж��Ƿ����°汾
	//����ֵ�� true ��ʾ�а汾Ҫ���£�false��ʾ�Ѿ������°汾
	bool CheckVersion();

	//��������������
	//����ֵ�� true ��ʾ���³ɹ���ɣ�false��ʾ����ʧ��
	bool Update(bool bInUIThread = true);

	//�������������ݰ� ע��bool Update()�ߵ��������������� �����������������ݰ�
	//������pszServerUrl  ���������ص�ַ
	//������pszSourcePath ���ص���ԴĿ¼
	//������bUnCompress �Ƿ�Ҫ��ѹ ֻ֧�ֿ���ṩ��ѹ����ʽ
	//������pDownloadEvent 
	//����ֵ�� true ��ʾ���سɹ���ɣ�false��ʾ����ʧ��
	bool Download(const char*pszServerUrl,const char*pszSourcePath,bool bUnCompress,IUpdateEvent *pDownloadEvent,bool bInUIThread = true);
protected:
	//INetEvent�ӿڷ���
	virtual void OnData(Socket* pSocket, const char *buf, size_t n);
	virtual void OnConnect(Socket* pSocket);
	virtual void OnConnectFailed(Socket* pSocket);
	virtual void OnDisconnect(Socket* pSocket);
	virtual void OnDataComplete(Socket* pSocket);
	virtual const char*GetErrorMsg(IUpdateEvent::ERROR_CODE ecErro);
protected:
	//ITQZipEvent�ӿڷ���
	virtual void OnUnCompressEvent(bool &bOpContinue,int nFileNum,int nFileIndex,const char* pszFileName);
	void OnTQZipError(ERROR_CODE emErrCode,const char* pszErrMsg);
protected:
	void GetNewsversion(bool bInUIThread = true);
	void GetApplicationXml(bool bInUIThread = true);
	void GetPacketXml(bool bInUIThread = true);
protected:
	virtual void CheckDownloadBegine(const char*pszFilePath);
private:
	CUpdateFactory(void);
	~CUpdateFactory(void);
private:
	std::string m_strServerUrl;
	std::string m_strAppName;
	std::string m_strOutSorucePath;

	std::string m_strUpdatePacketUrl;//Ҫ�����ļ���URL
	std::string m_strUpdatePacketName;//Ҫ�����ļ����ļ���
	DWORD m_dwUpdatePackeSize;//Ҫ�����ļ����ܴ�С���ֽڣ�
	std::string m_strPacketxmlUrl;//���°�������XML��ַ

	IUpdateEvent *m_pUpdateEvent;//�����õĻص�
	DWORD m_nCurVersion;//��ǰ���ذ汾
	DWORD m_dwSizeDownloaded;//�Ѿ����ص��ļ���С���ֽڣ�
	DWORD m_dwDownloadBegin;//�ϵ�����ǰ���ļ���С���ֽڣ�
	bool m_bNeedUpdate;//�Ƿ���Ҫ����
	bool m_bUnCompress;//�Ƿ���Ҫ��ѹ

	HttpClientSocket* m_pSocket;
	bool m_bDataComplete;//�ļ��Ƿ����ؽ�������

	DWORD m_nStatus;//��ǰ�Ĳ���״̬

	IUpdateEvent::ERROR_CODE m_ecErro;
	std::ofstream m_file;

	UPDATE_MODE m_emUpdateMode;
	CRITICAL_SECTION_ID	m_idCS;

};

#endif