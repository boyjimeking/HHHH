/******************************************************************************
 *  FileName:		NDSocket.h
 *  Author:			Guosen
 *  Create Date:	2012-3-30
 *  
 *****************************************************************************/

#ifndef _NDSOCKET_H_2012_03_30_10_04_25_
#define _NDSOCKET_H_2012_03_30_10_04_25_
//////////////////////////////////////////////////////////////////////////

#include "NDObject.h"

//#include "NetSocket.h"
#include "NDTransData.h"

//////////////////////////////////////////////////////////////////////////

namespace NDEngine
{
	class NDSocket : public NDObject
	{
		DECLARE_CLASS(NDSocket)
		NDSocket();
		~NDSocket();
	public:
		bool Connect( const char* address, unsigned short port, bool blocking = false );
		bool IsConnected(){ return m_bConnected; }
		bool Receive( unsigned char* data, int& iReadedLen, int iReadLen=512 );
		bool Send( NDTransData* data );		
		//bool DealReceivedData(const unsigned char* data, unsigned int dataLen);
		void Close();

		std::string GetIpAddress(){ return m_szIPAddress; }
		unsigned short GetPort(){ return m_port; }

	protected:
		std::string		m_szIPAddress;			//��������ַ
		unsigned short	m_port;					//�������˿�
		bool			m_bBlocking;			//�Ƿ�����ģʽ
		int				m_iSocket;				//Socket ID
		bool			m_bConnected;			//�Ƿ�����
		std::vector<unsigned char> m_data;

	protected:
		//����socket
		bool StartupSocketLib();
		//�Ƴ�socket
		bool ClearupSocketLib();
		//���÷�����
		bool SetSocketNotBlock(unsigned int sock);
		//�ر�socket
		int CloseSocket(int sock);
		//
		int Recv( char* pBuffer, int nLen );
	};
}

//////////////////////////////////////////////////////////////////////////
#endif //_NDSOCKET_H_2012_03_30_10_04_25_
