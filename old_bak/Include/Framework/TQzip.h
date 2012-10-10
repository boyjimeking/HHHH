#pragma once

/*
��װ�Ŀ���Զ������Դ�����ʽ
Copyright(c) 1999-2012, TQ Digital Entertainment, All Rights Reserved
*/

#ifndef __TQZip__HEAD__FILE__
#define __TQZip__HEAD__FILE__

//���ӿ�Ϊ��Դ����ص�
class ITQZipEvent
{
public:
	enum ERROR_CODE{
		ERRCODE_NONE=0,//����

		ERRCODE_FILE_NOTEXIST,//�ļ�������
		ERRCODE_FILE_ERRCREAT,//�ļ�����ʧ��
		ERRCODE_FILE_ERRSAVE,//�ļ��������
		ERRCODE_FILE_ZEROSIZE,//0��С���ļ�
		ERRCODE_FUN_WRONGPARAM,//�������
		ERRCODE_MEMORY_INSUMEM,//Insufficient memory �ڴ���䲻��
		ERRCODE_FORMAT_ERRFORMAT,//�����ʽ
		ERRCODE_OPERATION_STOPOP,//������ֹ����

		ERRCODE_UNKNOW,//δ֪����

	};
public:
	//������������̵Ļص�
	//������bOpContinue �Ƿ���ֹ�������
	//������nFileNum Ҫ��������ļ���
	//������nFileIndex Ҫ����ĵ�ǰ�ļ�����
	//������pszFileName �ļ���������·��
	virtual void OnCompressEvent(bool &bOpContinue,int nFileNum,int nFileIndex,const char* pszFileName){};

	//������������̵Ļص�
	//������bOpContinue �Ƿ���ֹ�������
	//������nFileNum Ҫ��������ļ���
	//������nFileIndex Ҫ����ĵ�ǰ�ļ�����
	//������pszFileName �ļ���������·��
	virtual void OnUnCompressEvent(bool &bOpContinue,int nFileNum,int nFileIndex,const char* pszFileName){};

	//��������������Ļص�
	//������emErrCode �������
	//������pszErrMsg ������ʾ
	virtual void OnTQZipError(ERROR_CODE emErrCode,const char* pszErrMsg){};
	~ITQZipEvent(){};
};

class CTQZip
{
public:
	//��������Դ������� ע���������Դ��Ϊ�Զ����.TQZip��ʽ
	//������pszSrcDir Ҫ�������ԴĿ¼
	//������pszDestDir �����
	//������pszFileName ��������Դ��������׺
	//������pszNCFilter �˲���������ļ���׺���ˣ���ʽ .jpg|.exe|���������˵��ļ���������ѹ��ֱ�Ӵ��
	//������ITQZipEvent ѹ�����̻ص�
	//����ֵ��true ѹ���ɹ� false ѹ��ʧ��
	static bool Compress(const char*pszSrcDir,const char*pszDestDir,const char*pszFileName,const char* pszNCFilter,ITQZipEvent *pTQZipEvent,bool bInUIThread = true);

	//��������Դ����ѹ���� ע����Դ��Ϊ�Զ����.TQZip��ʽ
	//������pszSrcFile ��Դ���ļ�
	//������pszDestDir ��ѹĿ¼
	//������ITQZipEvent ѹ�����̻ص�
	//����ֵ��true ��ѹ�ɹ� false ��ѹʧ��
	static bool UnCompress(const char*pszSrcFile,const char*pszDestDir,ITQZipEvent *pTQZipEvent,bool bInUIThread = true);

};

#endif