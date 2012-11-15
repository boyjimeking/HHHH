/******************************************************************************
 *  FileName:		NDTransData.h
 *  Author:			Guosen
 *  Create Date:	2012-3-30
 *  
 *****************************************************************************/

#ifndef _NDTRANSDATA_H_2012_03_30_12_44_11_
#define _NDTRANSDATA_H_2012_03_30_12_44_11_
//////////////////////////////////////////////////////////////////////////

#include "NDObject.h"

#include <string>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#import <Foundation/Foundation.h>
#endif
//////////////////////////////////////////////////////////////////////////

namespace NDEngine
{	
#if (defined(USE_NDSDK) || defined(USE_MGSDK))
	const int ND_C_MSGID_BEGIN  = 2;
	const int ND_C_HEAD_SIZE   = 4;
#else
	const int ND_C_MSGID_BEGIN  = 4;
	const int ND_C_HEAD_SIZE   = 6;
#endif

	class NDTransData : public NDObject 
	{
		DECLARE_CLASS(NDTransData)
	public:
		explicit NDTransData();
		NDTransData(unsigned short code);
		~NDTransData();
		const unsigned char* GetBuffer() { return m_pBuffer; };
		
	public:
		/**�ӽ�������ֻ�е�½��Ϣ��Ҫ,������Ϣ����Ҫ*/
		bool encrypt(const std::string& strKey);
		bool decrypt(const std::string& strKey);

//		������Read
//		���ã������ж�ȡһ������
//		������pBuffer(�������)���ݣ�nLen���ݳ���
//		����ֵ��true����ȷ false������
		bool Read(unsigned char* pBuffer, unsigned short nLen);

//		������Write
//		���ã�������д��һ������
//		������pBuffer���ݣ�nLen���ݳ���
//		����ֵ��trueд��ȷ falseдʧ��
		bool Write(const unsigned char* pBuffer, unsigned short nLen);

//		������ReadByte
//		���ã������ж�ȡһ�ֽ�
//		��������
//		����ֵ��һ�ֽ�
		unsigned char ReadByte();

//		������ReadInt
//		���ã������ж�ȡһ��������4�ֽ�
//		��������
//		����ֵ������
		int ReadInt();

//		������ReadShort
//		���ã������ж�ȡһ����������2�ֽ�
//		��������
//		����ֵ��������
		unsigned short ReadShort();
		
		long long ReadLong();
	
		//std::string ReadString();
		/**���ص��ַ�����utf8����*/
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		NSString* ReadUnicodeNString();
        NSString* ReadUTF8NString();
#endif
		std::string ReadUnicodeString();
		std::string ReadUnicodeString2(bool bCareCode=true);
		bool		ReadUnicodeString(char* out, unsigned int& outLen);
		
		void WriteByte(unsigned char ucValue);

//		������WriteInt
//		���ã�������д��һ������
//		������nValue����
//		����ֵ����
		void WriteInt(const int nValue);

//		������WriteShort
//		���ã�������д��һ��������
//		������nValue������
//		����ֵ����
		void WriteShort(const unsigned short nValue);
		//void WriteString(const std::string& strValue);
		void WriteUnicodeString(const std::string& strValue);
		void WriteUnicodeString(const char* strValue);
		
		//��������������������ṩ����ʹ�ã�������ǰ������������
		//
		NDTransData& operator >>(int& nValue);
		NDTransData& operator >>(unsigned short& nValue);
		//NDTransData& operator >>(std::string& strValue);
		NDTransData& operator >>(unsigned char& cValue);
		
		
		NDTransData& operator <<(const int nValue);
		NDTransData& operator <<(const unsigned short nValue);
		//NDTransData& operator <<(const std::string& strValue);
		NDTransData& operator <<(const unsigned char cValue);
		
		NDTransData& operator <<(const long nValue);
		
		NDTransData& operator <<(const unsigned long nValue);
		
		NDTransData& operator =(const NDTransData& rhs);		

//		������Clear
//		���ã������
//		��������
//		����ֵ����
		void Clear();

//		������GetSize
//		���ã���ȡ����С
//		��������
//		����ֵ������С
		unsigned short GetSize(){ return m_nSize; };
		
		unsigned short GetCode();
		void SetPackageSize();
	private:
        void SetSize(unsigned short newSize){ m_nSize = newSize;}
        bool SetBufSize(unsigned short newSize);
		void SetPackageHead();
		
	private:
		unsigned short m_nSize;
		unsigned short m_nBufSize;
		unsigned short m_nReadPos;
		unsigned short m_nWritePos;
		unsigned char* m_pBuffer;
		
	};	
}

//////////////////////////////////////////////////////////////////////////
#endif //_NDTRANSDATA_H_2012_03_30_12_44_11_
