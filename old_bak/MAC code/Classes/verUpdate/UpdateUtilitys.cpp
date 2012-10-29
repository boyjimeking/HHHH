#include "UpdateUtilitys.h"
#include "CCPlatformConfig.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>



#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <io.h>
#include <direct.h>

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#endif


#include "unzip.h"
using namespace cocos2d;


bool utility::UnpackZipFile( const char* szZipFile, const char* szDstPath )
{
	int rv = UNZ_ERRNO;
	unz_global_info global_info;
	unz_file_info finfo;
	char szFileName[256];
	char chExtraField[255];
	char szComment[255];
	char buf[10240];
	int lenBuf = 10240;

	//��ѹ���ļ�

	unzFile uzf = unzOpen(szZipFile);

	if(uzf == NULL)
	{
		return false;
	}

	//��ȡѹ���ļ���Ϣ

	rv = unzGetGlobalInfo(uzf, &global_info);

	if(rv!= UNZ_OK)
	{
		return false;
	}

	//ö��ѹ�����ڵ�һ���ļ�
	rv = unzGoToFirstFile(uzf);

	if(rv!= UNZ_OK)
	{
		return false;
	}

	for(int i=0; i<global_info.number_entry; ++i)
	{
		//��ȡѹ�������ļ�����Ϣ
		rv = unzGetCurrentFileInfo(uzf,
			&finfo,
			szFileName,
			sizeof(szFileName),
			(void*)chExtraField,
			sizeof(chExtraField),
			szComment,
			sizeof(szComment));
		if(rv != UNZ_OK)
		{
			break;
		}

		std::string strFileName;
		strFileName.append(szDstPath);
		int nLenPath = strlen(szDstPath);
		if(szDstPath[nLenPath-1] != '/')
		{
			strFileName.append("/");
		}
		strFileName.append(szFileName);

		int nLenFile = strlen(szFileName);
		if(szFileName[nLenFile-1] == '/')
		{
			//����Ŀ¼��
			if(access(strFileName.c_str(), 0) == -1)
			{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
				mkdir(strFileName.c_str());
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
				mkdir(strFileName.c_str(), S_IROTH|S_IWOTH);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
				mkdir(strFileName.c_str(), S_IROTH|S_IWOTH);
#endif
			}
		}
		else
		{
			//�����ļ���
			//��ѹ�����ڵ��ļ�
			rv = unzOpenCurrentFile(uzf);
			if(rv!= UNZ_OK)
			{
				break;
			}
			
			rv = UNZ_ERRNO;
			FILE* fp = fopen(strFileName.c_str(), "wb");
			if(fp==NULL)
			{
				break;
			}
			//��ѹ�����ڵ��ļ�

			do
			{
				rv = unzReadCurrentFile(uzf, buf, lenBuf);
				if(rv>0)
				{
					fwrite(buf, 1, rv, fp);
				}
			}while(rv>0);

			//�ر�ѹ�������ļ����
			unzCloseCurrentFile(uzf);
			fclose(fp);
		}

		//ö��ѹ��������һ���ļ�
		rv = unzGoToNextFile(uzf);

		if(rv != UNZ_OK)
		{
			break;
		}
	}

	//�ر�ѹ�����ļ�
	rv = unzClose(uzf);
	return (rv == UNZ_OK);
}


unsigned long utility::ConvertVersionStrToValue( const char* pszVersion )
{
	if( !pszVersion )
	{
		return 0;
	}

	unsigned long c[4] = {0,0,0,0};
	sscanf( pszVersion, "%d.%d.%d.%d", &c[3],&c[2],&c[1],&c[0] );

	unsigned long ans = 0;
	ans =  (c[3]<<24) + (c[2]<<16) + (c[1]<<8) + (c[0]);

	return ans;
}











