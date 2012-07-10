  /********************************************************************
Created:		2006_4_10 21:18
Filename:		ITqPackage.h
Author:		    Huang Yuhang
Purpose:		
********************************************************************/
#ifndef _INCLUDE_ONCE_ITqPackage_txt_2006_4_10_21_18
#define _INCLUDE_ONCE_ITqPackage_txt_2006_4_10_21_18
/********************************************************************/
#include "C3BaseType.h"
/********************************************************************/

/********************************************************************/
enum TQPRESULT
{
	TQR_OK					=		0, // ����
	TQR_PARAMETER_ERROR		=		1, // ����������
	TQR_BUFFER_OVERFLOW		=		2, // �������
	TQR_FILE_NOTFOUND		=		3, // δ�ҵ��ļ�
	TQR_FILE_OPEN_ERROR		=		4, // ���ļ�ʧ��
	TQR_FILE_READ_ERROR		=		5, // �ļ���ʧ��
	TQR_FILE_WRITE_ERROR	=		6, // �ļ�дʧ��
	TQR_WRITEMUTEX_ERROR	=		7, // ��д��������
	TQR_READMUTEX_ERROR		=		8, // ������������
	TQR_COMPRESS_ERROR		=		9, // ѹ�����ѹ����
	TQR_NOT_MATCHING		=		10, // �����������ļ���ƥ��
	TQR_NOT_OPENFILE		=		11,	// �ļ�δ��
	TQR_REPACK_OK			=		12,	// �������ɹ�
	TQR_REPACK_NOTINIT		=		13, // δ��ʼ��
	TQR_FILENAME_ERROR		=		14,	// �ļ�������
	TQR_NOTINIT				=		15,	// û��ʼ��
	TQR_INVALID_BLOCK		=		16,	// ��Ч�Ŀ�
	TQR_ALLOCMEMORY_ERROR	=		17,	// �����ڴ�ʧ��
};

//�����ļ���дģʽ
enum OpenFileMode
{
	OpenFileMode_Read		=		0,//ֻ��
	OpenFileMode_ReadWrite	=		1,//��д
};

/********************************************************************/
class ITqPackage
{
public:
	// ���ͷŽӿ�
	virtual		DWORD		Release(void)													=	0;

	// ����ļ��Ƿ����
	virtual		bool		CheckFile( const char* pszFileName, DWORD* nFileSize=NULL )		=	0; 

	// ����package...
	virtual		TQPRESULT	Create(const char* pszPackageFileName)							=	0;

	// ��package...
	virtual		TQPRESULT	Open(const char* pszPackageFileName, enum OpenFileMode eMode )	=	0;
	
	// �����ļ���Buffer��uSize����Buffer��С��������ѹ���ļ���С
	virtual		TQPRESULT	Load(const char* pszFileName, BYTE* pBuffer, DWORD& uSize)		=	0;
	// �ر�package...
	virtual		TQPRESULT	Close(void)														=	0;
		
	// ��ӻ����һ���ļ�
	virtual		TQPRESULT   AddFile(const char* pszFileName)								=	0;
	
	// ���������ļ� 
	virtual		TQPRESULT   UpdateIndex(void)												=	0;

	// ɾ��һ���ļ�
	virtual		TQPRESULT	DelFile(const char* pszFileName)								=	0;

	// ������Ƭ
	virtual		TQPRESULT	Repack( DWORD& uItem, char* pFileName )							=	0;

	// ȡ�ļ�����
	virtual		DWORD		GetFileAmount(void)												=	0;
};

ITqPackage* TqPackageCreate();




/********************************************************************/
enum FILETYPE//�ļ�����
{
	FILETYPE_ALL	=	0,	// ����
	FILETYPE_UNPACK,		// ĩ������ļ�
	FILETYPE_PACK,			// ������ļ�
};

enum TQSEEKMODE
{
	TQSEEK_CUR		=	SEEK_CUR,
	TQSEEK_END		=	SEEK_END,
	TQSEEK_BEGIN	=	SEEK_SET,
};
typedef void*	HTQF;		// TqFileHandle
/********************************************************************/

///////////////////////���ݰ����//////////////////////////////////////
// ��һ�����ݰ��ļ�
TQPRESULT			TqPackagesOpen( const char* pszPackageFile );

// TQPackageClose �ر����ݰ��ļ� ����NULLȫ���ر�
void				TqPackagesClose( const char* pszPackageFile );
////////////////////////�ļ����////////////////////////////////////////

// ̽��ָ���ļ��Ƿ���ָ�����ļ��б���
bool				TqFCheck( const char* pszFile, FILETYPE eFileType );

// ����ĳ���ļ�, ���ػ������Լ���������С; ������ݰ��ļ���û���ҵ�����ļ�, �򷵻�false
TQPRESULT			TqFDump( const char* pszFile, void*& pBuf, DWORD& uSize );

// ���DUMP������
TQPRESULT		 	TqFUndump( void* pBuffer );

// ��λ���ݰ��е�ĳ���ļ�	 
TQPRESULT			TqFOpen( const char* pszFile, HTQF& hTqf );

//�ر�
TQPRESULT			TqFClose( HTQF hTqf );

// ���ļ�����һ�����ݿ�, ���ض�������ݴ�С(buf���ⲿ����)(����fread)
LONG				TQFRead( void* pBuf, DWORD uSize, DWORD uCount, HTQF hTqf );

// ��λ�ļ�ƫ��
bool				TqFSeek( HTQF hTqf, LONG uOffset, TQSEEKMODE eSeek );

// ȡ�ö�дָ��λ��
DWORD				TqFTell( HTQF hTqf );

// ̽���ļ��Ƿ��ļ�β
bool				TqFIsEof( HTQF hTqf );

//ȡ�ļ�����
LONG				TqFGetLen( HTQF hTqf );

//ȡ�汾��
LONG				TqFGetVersion(void);

TQPRESULT			TqPackagesInit(const char* pszAppFolderPath);

/********************************************************************/
#endif  //_INCLUDE_ONCE_ITqPackage_txt_2006_4_10_21_18
/********************************************************************/



