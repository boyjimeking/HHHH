#pragma once

/*
��װ�򵥵������Ӧ��
Copyright(c) 1999-2012, TQ Digital Entertainment, All Rights Reserved
*/

#ifndef __NETFACTORY__HEAD__FILE__
#define __NETFACTORY__HEAD__FILE__

#include <vector>
#ifdef ANDROID
#include <sys/select.h>
#endif
#include "Socket.h"
#include "TcpSocket.h"
#include "HTTPSocket.h"
#include "HttpPostSocket.h"
#include "HttpPutSocket.h"
#include "HttpGetSocket.h"
#include "MySemaphore.h"
#include "SocketHandler.h"
#include "BaseType.h"

//���ӿ�Ϊ����ͻ��˻ص��¼���
//pSocket����Ϊ������ǰ���ݵ�SOCKET����ָ��
class INetEvnet
{
public:
	//�ͻ������ӳɹ�ʱ���ص�������
	virtual void OnConnect(Socket* pSocket){};

	//�ͻ�������ʧ��ʱ���ص�������
	virtual void OnConnectFailed(Socket* pSocket){};

	//�ͻ�������ʧ��ʱ����������ʱ���ص����������������ture���������������false��������
	virtual bool OnConnectRetry(Socket* pSocket){return true;};

	virtual void OnReconnect(Socket* pSocket){};

	//�ͻ������ӳ�ʱ���ص�������
	virtual void OnConnectTimeout(Socket* pSocket){};

	//�ͻ������ӶϿ�ʱ���ص�������
	virtual void OnDisconnect(Socket* pSocket){};

	//�ͻ��˽��յ�����ʱ���ص������������ӿڽ��յ���TCP��ε����ݡ�
	//ע�⣺���ΪHTTP���ӣ����Ȼ�ص����������ٻص�OnData�ຯ�����������յ������ݰ���HTTPͷ
	virtual void OnRawData(Socket* pSocket, const char *buf, size_t n){};

	//�ͻ��˽��յ�����ʱ���ص������������ӿڽ��յ���HTTP��ε����ݡ�
	//ע�⣺���ӿڽӵ�������ΪHTTP�����ݣ�������HTTPͷ���ݡ�
	virtual void OnData(Socket* pSocket, const char *buf, size_t n){};

	//�ͻ��˽����������ʱ���ص������������ӿڽ��յ���HTTP��ε����ݡ�
	virtual void OnDataComplete(Socket* pSocket){};

	//�ͻ����������е����ݻ��巢����ɺ󣬻ص�������
	virtual void OnWriteComplete(Socket* pSocket){};

	~INetEvnet(){};
};

//����ѹ������ѹ��
//������Ҫ�����в����������ݿ��ܻ���gZip��ѹ���㷨���д��䡣
class CZipData
{
public:
	
	//ѹ�����ݣ��ɹ�����0����0��ʧ��
	static int ZipCompress(BYTE* pData/*��ѹ����Դ����*/, ULONG lDataLen/*���ݳ���*/,	BYTE* pZipData/*ѹ���������*/, ULONG* pZipDataLen/*����ΪpZipData���ȣ����Ϊѹ��������ݳ���*/);

	//ѹ�����ݣ��ɹ�����0����0��ʧ�ܣ�һ������HTTPЭ�飬���HTTP��ͷΪ��Content-Encoding: gzip
	static int gZipcompress(BYTE* pData/*��ѹ����Դ����*/, ULONG lDataLen/*���ݳ���*/, BYTE* pZipData/*ѹ���������*/, ULONG* pZipDataLen/*����ΪpZipData���ȣ����Ϊѹ��������ݳ���*/);
	
	//��ѹ���ݣ��ɹ�����0����0��ʧ��
	static int ZipUnCompress(BYTE* pZipData/*����ѹ������*/, ULONG nZipDataLen/*���ݳ���*/, BYTE *pData/*��ѹ�������*/, ULONG *pDataLen/*����ΪpData���ȣ����Ϊѹ��������ݳ���*/);

	//������ݣ��ɹ�����0����0��ʧ�ܣ�һ������HTTPЭ�飬���HTTP��ͷΪ��Content-Encoding: gzip
	static int gZipUnCompress(BYTE* pZipData/*����ѹ������*/, ULONG nZipDataLen/*���ݳ���*/, BYTE *pData/*��ѹ�������*/, ULONG *pDataLen/*����ΪpData���ȣ����Ϊѹ��������ݳ���*/);
};

class CNetFactory : Thread
{
public:

	//��������൥ʵ������
	static CNetFactory& sharedInstance();

	//����һ��TCP���ӵ�SOCKET
	TcpSocket* CreateTcpSocket(INetEvnet* pEvent);

	//����һ��ԭʼ��HTTP���ӵ�SOCKET
	//HTTPSocket*	CreateHttpSocket(INetEvnet* pEvent);

	//����һ��Post��ʽ��HTTP���ӵ�SOCKET
	HttpPostSocket* CreateHttpPostSocket(INetEvnet* pEvent);

	//����һ��Put��ʽ��HTTP���ӵ�SOCKET
	HttpPutSocket* CreateHttpPutSocket(INetEvnet* pEvent);

	//����һ��Get��ʽ��HTTP���ӵ�SOCKET
	HttpGetSocket* CreateHttpGetSocket(INetEvnet* pEvent);

	//�ͷ�ǰ��ӿڴ����Ķ���ʵ��
	bool ReleaseSocket(Socket* pSocket);

	//�������紦���̣߳�һ���ڳ�ʼ������һ�μ��ɡ�
	//���û�е��ñ��ӿڣ���ӵ�Pool���SOCKET�������ᱻ����������������ݽ��ա�
	bool Start();

	//��������SOCKET�󣬿��Զ�SOCKET�������ã��������ͨ�����ӿ��й�SOCKET�������ݽ��մ���
	bool AddSocketToPool(Socket* pSocket);

	//ȡ�����ݽ����йܡ�
	bool RemoveSocketFromPool(Socket* pSocket);

	//ȡ�����ݽ����й�,���ͷŶ���
	bool ReleaseSocketFromPool(Socket* pSocket);

	//�ر����紦���̣߳�һ���ڳ����˳�ʱ����һ�μ��ɡ�
	//���ñ��ӿں���Pool���SOCKET���������ٱ�����������������ݽ��ա�
	bool Stop();
	
	//�ڲ�ʵ�֣��û���Ҫ���á�
	SocketHandler& GetSocketHandler();
private:
	CNetFactory(void);
	~CNetFactory(void);
	
protected:
	virtual void Run();

protected:
	
	typedef struct 
	{
		Socket* pSocket;
		int nFlag;
	}SOCKOPINFO;
	typedef std::vector<SOCKOPINFO> SOCKETOPINFOARR;

	SOCKETOPINFOARR	m_arrSockOpInfo;
	Mutex		m_objMutex;

	Semaphore		m_objSemaphore;
	SocketHandler	m_objSockethandler;
};

#endif