
#include "GameUpdate.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <set>
#include <map>
#include "CCPlatformConfig.h"

#include "../NETWORK/myHttpFile.h"

#include "xGraph.h"
#include "PacksInfo.h"
#include "UpdateUtilitys.h"
#include "IniFile.h"
//#include "UpdateGUI.h"

CGameUpdate::CGameUpdate(void)
{
}

CGameUpdate::~CGameUpdate(void)
{
}

bool CGameUpdate::init()
{
	return true;
}

bool CGameUpdate::startUpdate()
{
	std::string strXmlFilePath;
	strXmlFilePath.append( this->_GetAppDocumentPath() );
	strXmlFilePath.append( "update.xml" );

	std::string strIniFilePath;
	strIniFilePath.append( this->_GetAppDocumentPath() );
	strIniFilePath.append( "version.ini" );


	//��ȡ����
	CIniFile iniFile;
	iniFile.ReadFile(strIniFilePath.c_str());

	//��������Ӧ�汾��xml�ļ�
	{
		std::string strXmlUri;
		strXmlUri.append("http://");
		strXmlUri.append(iniFile.GetValue("system", "xml"));

		bool succ = this->_DownloadFile(strXmlUri.c_str(), strXmlFilePath.c_str());
		if(!succ)
		{
			return false;
		}

	}

	//����xml�ļ�, ���ݵ�ǰ�汾�����°汾, ��������Ÿ����б�
	CPacksInfo packInfo;
	packInfo.LoadFromXmlFile(strXmlFilePath.c_str());


	std::string strCurVer = iniFile.GetValue("system", "ver");
	std::string strLatestVer = packInfo.GetLatestVersion();

	if(strCurVer==strLatestVer)
	{
		//�Ѿ������°汾...
		return true;
	}


	std::vector<t_PackInfo>* pVecPackInfo = packInfo.GetPackInfo();

	//ɸ�����ڵİ汾���б�
	std::set<std::string> setVersion;
	for(int i=0; i<pVecPackInfo->size(); ++i)
	{
		setVersion.insert((*pVecPackInfo)[i].strVersionSrc);
		setVersion.insert((*pVecPackInfo)[i].strVersionDst);
	}

	//���汾�ŷ����ź�����ӳ��
	std::vector<std::string> vecVersion;
	std::map<std::string, int> mapVersion2Index;

	{
		int i=0;
		std::set<std::string>::iterator it = setVersion.begin();
		for(; it!=setVersion.end(); ++it)
		{
			vecVersion.push_back( *it );
			mapVersion2Index[ *it ] = i++;
		}
	}


	graph_t* pgraph = graph_create(pVecPackInfo->size());

	//��ͼ�ṹ��ӱ�, �ߵ�Ȩ��ͳһΪ 1 
	for(int i=0; i<pVecPackInfo->size(); ++i)
	{
		int si = mapVersion2Index[ (*pVecPackInfo)[i].strVersionSrc ];
		int di = mapVersion2Index[ (*pVecPackInfo)[i].strVersionDst ];

		int* pEdge;
		pEdge = graph_get_edge_ptr(pgraph, si, di);
		*pEdge = 1;
		pEdge = graph_get_edge_ptr(pgraph, di, si);
		*pEdge = 1;
	}


	int arWaypoint[MAX_GRAPH_VECTEX_AMT];
	int lenWaypoint = graph_find_best_path(pgraph, mapVersion2Index[strCurVer], mapVersion2Index[strLatestVer], arWaypoint, MAX_GRAPH_VECTEX_AMT);
	graph_release(pgraph);

	if(lenWaypoint==-1)
	{
		return false;
	}

	//�ҵ�·��, ����·�����ɸ��°�Uri�б�
	std::vector<t_PackInfo> vecFinalPackInfo;
	for(int i=0; i<lenWaypoint-1; ++i)
	{
		std::string srcVer = vecVersion[arWaypoint[i]];
		std::string dstVer = vecVersion[arWaypoint[i+1]];

		for(int k=0; k<pVecPackInfo->size(); ++k)
		{
			if( (*pVecPackInfo)[k].strVersionSrc==srcVer && (*pVecPackInfo)[k].strVersionDst==dstVer )
			{
				vecFinalPackInfo.push_back((*pVecPackInfo)[k]);
			}
		}
	}

	//���ݸ����б�, �����ļ�, ��ѹ��Ŀ���ļ���, �����°汾��, ��˷���, ���µ����°汾(������һ�������б�)
	for(int i=0; i<vecFinalPackInfo.size(); ++i)
	{
		//ȷ�ϸ��µİ汾
		std::string strCurVersion = iniFile.GetValue("system", "ver");
		if(strCurVersion != vecFinalPackInfo[i].strVersionSrc)
		{
			NetApi::PrintLog("Update Error: current version error~");
			return false;
		}

		//�����ļ�
		std::string strPackUri = vecFinalPackInfo[i].strUri;
		std::string strPackFileName = vecFinalPackInfo[i].strFileName;
		std::string strPackFilePath;
		strPackFilePath.append( this->_GetAppDocumentPath() );
		strPackFilePath.append(strPackFileName);


		bool succ = this->_DownloadFile(strPackUri.c_str(), strPackFilePath.c_str());
		if(!succ)
		{
			return false;
		}


		//��ѹ�ļ�
		utility::UnpackZipFile(strPackFilePath.c_str(), this->_GetAppDocumentPath());
		::remove(strPackFilePath.c_str());

		//���µ�ǰ�İ汾��
		iniFile.SetValue("system", "ver", vecFinalPackInfo[i].strVersionDst.c_str());
		iniFile.WriteFile();
	}

	return true;

}

void CGameUpdate::receiveVerPackCallBack(const char* szUri, int nPercent)
{
/*	if(CUpdateMenu::layer)
	{
		CUpdateMenu::layer->retain();
		CUpdateMenu::layer->SetUpdateFileInfo(szUri);
		CUpdateMenu::layer->m_updateStatusQueue.push(&nPercent, sizeof(nPercent));

		CUpdateMenu::layer->release();
	}*/
}

bool CGameUpdate::finishUpdate()
{
	//int op = -1;
	//CUpdateMenu::layer->m_updateStatusQueue.push(&op, sizeof(op));

	return true;

}

bool CGameUpdate::_DownloadFile( const char* szUri, const char* szFileName )
{
	m_myHttpFile.PostAsyncGet(1, szUri, szFileName);
	bool bDownloadDone = false;
	while(!bDownloadDone)
	{
		CMyHttpFile::t_AsyncResponse tmp;
		if(m_myHttpFile.AsyncGetResponse(&tmp))
		{
			switch(tmp.nActionCode)
			{
			case CMyHttpFile::eAYNCRESPONSE_DOWNLOAD_BEGIN:
				this->receiveVerPackCallBack(szUri, 0);
				break;
			case CMyHttpFile::eAYNCRESPONSE_DOWNLOAD_PERCENT:
				this->receiveVerPackCallBack(szUri, tmp.nActionParam);
				break;
			case CMyHttpFile::eAYNCRESPONSE_DOWNLOAD_OK:
				this->receiveVerPackCallBack(szUri, 100);
				bDownloadDone = true;
				break;
			case CMyHttpFile::eAYNCRESPONSE_DOWNLOAD_FAILED:
				NetApi::PrintLog("Error: download %s failed", szUri);
				return false;
				break;
			default:
				break;
			}
		}

		NetApi::Sleep(0);
	}

	return true;
}

const char* CGameUpdate::_GetAppDocumentPath()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	return "./";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	return "./";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	return "/sdcard/ly2_test/";
#endif
}



