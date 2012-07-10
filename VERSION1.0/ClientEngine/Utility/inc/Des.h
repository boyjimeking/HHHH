/*
 *  Des.h
 *  DragonDrive
 *
 *  Created by jhzheng on 10-12-27.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _DES_H_
#define _DES_H_

class CDes  
{
public:
	CDes();
	virtual ~CDes();
	
	//���ܽ���
	enum	
	{
		ENCRYPT	=	0,	//����
		DECRYPT			//����
	};
	
	//DES�㷨��ģʽ
	enum
	{
		ECB		=	0,	//ECBģʽ
		CBC				//CBCģʽ
	};
	
	typedef bool    (*PSubKey)[16][48];
	
	//Pad����ģʽ
	enum
	{
		PAD_ISO_1 =	0,	//ISO_1��䣺���ݳ��Ȳ���8���صı�������0x00���㣬���Ϊ8���صı�������8��0x00
		PAD_ISO_2,		//ISO_2��䣺���ݳ��Ȳ���8���صı�������0x80,0x00..���㣬���Ϊ8���صı�������0x80,0x00..0x00
		PAD_PKCS_7		//PKCS7��䣺���ݳ��ȳ�8����Ϊn,��(8-n)����Ϊ8�ı���
	};
	
	/*******************************************************************/
	/*
	 �� �� �� ��:	RunPad
	 �� �� �� ����	����Э��Լ���ǰ�����ݽ������
	 �� �� ˵ ����	bType	:���ͣ�PAD����
	 In		:���ݴ�ָ��
	 Out		:��������ָ��
	 datalen	:���ݵĳ���
	 padlen	:(in,out)���buffer�ĳ��ȣ�����ĳ���
	 
	 ����ֵ ˵����	bool	:�Ƿ����ɹ�
	 *******************************************************************/
	static bool	RunPad(int nType,
					   const char* In,
					   unsigned int uDataLen,
					   char* Out,
					   unsigned int& uPadLen);
	/*******************************************************************/
	/*
	 �� �� �� ��:	PadCount
	 �� �� �� ����	�������ʱ����λ��
	 �� �� ˵ ���� In		:���ݴ�ָ��
	 count   :���ĳ���
	 
	 ����ֵ ˵����	bool	:����λ���Ƿ�Ϸ�
	 *******************************************************************/
	static bool Pad_PKCS_7_Count(char* In, int& count);
	/*******************************************************************/
	/*
	 �� �� �� ��:	RunDes
	 �� �� �� ����	ִ��DES�㷨���ı��ӽ���
	 �� �� ˵ ����	bType	:���ͣ�����ENCRYPT������DECRYPT
	 bMode	:ģʽ��ECB,CBC
	 In		:�����ܴ�ָ��
	 Out		:�������ָ��
	 datalen	:�����ܴ��ĳ��ȣ�ͬʱOut�Ļ�������СӦ���ڻ��ߵ���datalen
	 Key		:��Կ(��Ϊ8λ,16λ,24λ)֧��3��Կ
	 keylen	:��Կ���ȣ����24λ���ֽ����Զ��ü�
	 
	 ����ֵ ˵����	bool	:�Ƿ���ܳɹ�
	 *******************************************************************/
	static bool RunDes (bool bType,
						bool bMode,
						char* In,
						unsigned int uInDataLen,
						char* Out,
						unsigned int uOutDataLen,
						const char *Key,
						const unsigned char ucKeyLen);
	/*******************************************************************/
	static bool Encrypt_Pad_PKCS_7( const char *Key,
								   const unsigned char ucKeyLen,
								   char* In,
								   unsigned int uInDataLen,
								   char* Out,
								   unsigned int& uOutDataLen);
	/*******************************************************************/
	static bool Decrypt_Pad_PKCS_7( const char *Key,
								   const unsigned char ucKeyLen,
								   char* In,
								   unsigned int uInDataLen,
								   char* Out,
								   unsigned int& uOutDataLen);
	/*******************************************************************/
	//���㲢�������Կ��SubKey������
	static void SetSubKey(PSubKey pSubKey,
						  const char Key[8]);	
	/*******************************************************************/
	//DES��Ԫ����
	static void DES(char Out[8],
					char In[8],
					const PSubKey pSubKey, 
					bool Type);
};
/*******************************************************************/

#endif // _DES_H_