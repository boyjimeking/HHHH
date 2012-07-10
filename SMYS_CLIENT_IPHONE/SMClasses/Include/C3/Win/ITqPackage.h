/********************************************************************
Created:		2006_4_10 21:18
Filename:		ITqPackage.h
Author:		    Huang Yuhang
Purpose:		
********************************************************************/
#ifndef _INCLUDE_ONCE_ITqPackage_txt_2006_4_10_21_18
#define _INCLUDE_ONCE_ITqPackage_txt_2006_4_10_21_18
/********************************************************************/
#include <stdio.h>
/********************************************************************/
#ifdef TQPACKAGE_EXPORTS
#define TQPACKAGE_API __declspec(dllexport)
#else
#define TQPACKAGE_API __declspec(dllimport)
#endif
/********************************************************************/
#define TQEDITOR
/********************************************************************/
enum TQPRESULT{
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
enum OpenFileMode{
	OpenFileMode_Read		=		0,//ֻ��
	OpenFileMode_ReadWrite	=		1,//��д
};
/********************************************************************/
class ITqPackage
{
public:
	// ���ͷŽӿ�
	virtual		ULONG		Release(void)													=	0;

	// ����ļ��Ƿ����
	virtual		bool		CheckFile( const char* pszFileName, ULONG* uFileSize=NULL )		=	0; 

	// ����package...
	virtual		TQPRESULT	Create(const char* pszPackageFileName)							=	0;

	// ��package...
	virtual		TQPRESULT	Open(const char* pszPackageFileName, enum OpenFileMode eMode )	=	0;
	
	// �����ļ���Buffer��uSize����Buffer��С��������ѹ���ļ���С
	virtual		TQPRESULT	Load(const char* pszFileName, BYTE* pBuffer, ULONG& uSize)		=	0;
	// �ر�package...
	virtual		TQPRESULT	Close(void)														=	0;
		
	// ��ӻ����һ���ļ�
	virtual		TQPRESULT   AddFile(const char* pszFileName)								=	0;
	
	// ���������ļ� 
	virtual		TQPRESULT   UpdateIndex(void)												=	0;

	// ɾ��һ���ļ�
	virtual		TQPRESULT	DelFile(const char* pszFileName)								=	0;

	// ������Ƭ
	virtual		TQPRESULT	Repack( ULONG& uItem, char* pFileName )							=	0;

	// ȡ�ļ�����
	virtual		ULONG		GetFileAmount(void)												=	0;
	
#ifdef TQEDITOR // �����Ʒ����
	virtual		const char*	GetFileName(ULONG uIndex)										=	0;
#endif	//TQEDITOR
};

TQPACKAGE_API ITqPackage* TqPackageCreate();




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
TQPRESULT	TQPACKAGE_API		TqPackagesOpen( const char* pszPackageFile );

// TQPackageClose �ر����ݰ��ļ� ����NULLȫ���ر�
void		TQPACKAGE_API		TqPackagesClose( const char* pszPackageFile );
////////////////////////�ļ����////////////////////////////////////////

// ̽��ָ���ļ��Ƿ���ָ�����ļ��б���
bool		TQPACKAGE_API		TqFCheck( const char* pszFile, FILETYPE eFileType );

// ����ĳ���ļ�, ���ػ������Լ���������С; ������ݰ��ļ���û���ҵ�����ļ�, �򷵻�false
TQPRESULT	TQPACKAGE_API		TqFDump( const char* pszFile, void*& pBuf, ULONG& uSize );

// ���DUMP������
TQPRESULT	TQPACKAGE_API	 	TqFUndump( void* pBuffer );

// ��λ���ݰ��е�ĳ���ļ�	 
TQPRESULT	TQPACKAGE_API		TqFOpen( const char* pszFile, HTQF& hTqf );

//�ر�
TQPRESULT	TQPACKAGE_API		TqFClose( HTQF hTqf );

// ���ļ�����һ�����ݿ�, ���ض�������ݴ�С(buf���ⲿ����)(����fread)
LONG		TQPACKAGE_API		TQFRead( void* pBuf, ULONG uSize, ULONG uCount, HTQF hTqf );

// ��λ�ļ�ƫ��
bool		TQPACKAGE_API		TqFSeek( HTQF hTqf, LONG uOffset, TQSEEKMODE eSeek );

// ȡ�ö�дָ��λ��
ULONG		TQPACKAGE_API		TqFTell( HTQF hTqf );

// ̽���ļ��Ƿ��ļ�β
bool		TQPACKAGE_API		TqFIsEof( HTQF hTqf );

//ȡ�ļ�����
LONG		TQPACKAGE_API		TqFGetLen( HTQF hTqf );

//ȡ�汾��
LONG		TQPACKAGE_API		TqFGetVersion(void);

TQPRESULT	TQPACKAGE_API		TqPackagesInit(const char* pszAppFolderPath);
/********************************************************************/
#endif  //_INCLUDE_ONCE_ITqPackage_txt_2006_4_10_21_18
/********************************************************************/



