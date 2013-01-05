//
//  SMLoginScene.cpp
//  SMYS
//
//  Created by user on 12-3-21.
//  Copyright 2012�� (����)DeNA. All rights reserved.
//

#include "SMLoginScene.h"
#include "NDDirector.h"
#include "ScriptGlobalEvent.h"
#include "ScriptInc.h"
//#include "SimpleAudioEngine_objc.h"
#include "NDPath.h"
#include "SMUpdate.h"
#include "GameApp.h"
//#include "NDUtility.h"
#include "NDUtil.h"
#include "sys/stat.h"
#include "SystemSetMgr.h"
#include "InstallSelf.h"
#include "NDBeforeGameMgr.h"
#include "NDTargetEvent.h"
#include "NDLocalXmlString.h"
#include "NDProfile.h"
#include "ScriptMgr.h"
#include <iostream>
#include <sstream>
#include "ScriptRegLua.h"
#include "ObjectTracker.h"
#include "CCPlatformConfig.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include <android/log.h>

#define  LOG_TAG    "DaHuaLongJiang"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGERROR(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#else
#define  LOG_TAG    "DaHuaLongJiang"
#define  LOGD(...)
#define  LOGERROR(...)
#endif
////////////////////////////////////////////////////////////

//--------------------//

#define UPDATE_ON		0	//0�ر����أ�1��������
#define CACHE_MODE 		0   //����ģʽ//0�رտ�����1��������Դ������cacheĿ¼������

//--------------------//

#define TAG_INSTALL_SUCCESS			1
#define TAG_INSTALL_FAILED			2
#define TAG_TIMER_INIT				3
#define TAG_TIMER_GET_STATUS		4
#define	TAG_REQUEST_URL_ERROR		5
#define TAG_TIMER_DOWNLOAD_SUCCESS	6
#define TAG_TIMER_UPDATE			7	//
#define TAG_TIMER_CHECK_WIFI		8	// ���WIFI
#define TAG_TIMER_UNZIP_SUCCESS     9
#define TAG_TIMER_CHECK_UPDATE      10  // ���UPDATE
#define TAG_TIMER_CHECK_COPY        11  //
#define TAG_TIMER_FIRST_RUN         12  // 
#define TAG_TIMER_LOAD_RES_OK       13  // װ�����ֺ�Lua���
#define TAG_TIMER_CHECK_LOGIN_COPY  14

//----------------------------------------------------------
//Update Layer ��
#define TAG_CTRL_PIC_BG					1	//����ͼƬ�ؼ�tag
#define TAG_LABEL_PROMPT				4	//���ֱ�ǩ�ؼ�tag
#define TAG_CTRL_PROGRESS				2	//�������ؼ�tag
#define TAG_UPDATE_LAYER				(2000+167)	//��Tag

#define TAG_DLG_CONFIRM					100	//ȷ�϶Ի���tag

#define TAG_BTN_OK						101	//ȷ����ť
#define TAG_BTN_CANCEL					102	//ȡ����ť
#define TAG_LABEL_TIP					103	//���ֱ�ǩ
#define TAG_PIC_DLG_BG					12	//�Ի��򱳾�

#define TAG_SPRITE_NODE					200	//

//----------------------------------------------------------
#define SZ_ERROR_01						"LOGIN_SZ_ERROR_01"			//"��汾����,����������������Ϸ�汾"
#define SZ_ERROR_02						"LOGIN_SZ_ERROR_02"			//"��ǰ�汾��������,���������ػ�����ϵGM"
#define SZ_ERROR_03						"LOGIN_SZ_ERROR_03"			//"�汾��Ϣ�𻵣����������ػ�����ϵGM"
#define SZ_ERROR_04						"LOGIN_SZ_ERROR_04"			//"��Ǹ,������Դδ�ҵ�,����ϵGM"
#define SZ_ERROR_05						"LOGIN_SZ_ERROR_05"			//"����ʧ��,�����������ӻ��������豸����"
#define SZ_DOWNLOADING					"LOGIN_SZ_DOWNLOADING"		//"�汾�����С���"
#define SZ_INSTALLING					"LOGIN_SZ_INSTALLING"		//"���ڰ�װ���¡���"
#define SZ_WIFI_OFF						"LOGIN_SZ_WIFI_OFF"			//"�������ظ��°�,����δ����WIFI,�Ƿ������"
#define SZ_UPDATE_OFF					"LOGIN_SZ_UPDATE_OFF"		//"�޷����ӷ�����,��������"
#define SZ_FIRST_INSTALL                "LOGIN_SZ_FIRST_INSTALL"    //"�״�����,��ʼ�������С���"
#define SZ_CONNECT_SERVER               "LOGIN_SZ_CONNECT_SERVER"   //"���ӷ���������"
#define SZ_SETUP						"LOGIN_SZ_SETUP"			//"�����С���"


#define SZ_UPDATE_URL					"192.168.19.169"//���·������ĵ�ַ
#define SZ_DEL_FILE						"del.txt"//������ɾ���ļ�·���������ļ�/CACHESĿ¼��

#define SZ_MOBAGE_BG_PNG_PATH			"/res/image00/Res00/Load/mobage_bg.png"
#define SZ_UPDATE_BG_PNG_PATH			"/res/image00/Res00/Load/entry_bg.png"

////////////////////////////////////////////////////////////
//NSAutoreleasePool * globalPool = [[NSAutoreleasePool alloc] init];
IMPLEMENT_CLASS(CSMLoginScene, NDScene)

//===========================================================================
CSMLoginScene* CSMLoginScene::Scene( bool bShowEntry /*= false*/  )
{
	CSMLoginScene* pkScene = new CSMLoginScene;
    pkScene->Initialization();
    pkScene->SetTag(SMLOGINSCENE_TAG);
    
	if ( bShowEntry )
	{
		NDLocalXmlString::GetSingleton().LoadLoginString();

		CCSize kWinSize = CCDirector::sharedDirector()->getWinSizeInPixels();

		NDUILayer* pkLayer = new NDUILayer();

		pkLayer->Initialization();
		pkLayer->SetFrameRect(CCRectMake(0, 0, kWinSize.width, kWinSize.height));
		pkScene->AddChild(pkLayer);
		pkScene->m_pLayerOld = pkLayer;

		NDPicturePool& kPool = *(NDPicturePool::DefaultPool());
		NDUIImage* pkBackgroundImage = new NDUIImage;

		pkBackgroundImage->Initialization();
		pkBackgroundImage->SetFrameRect(CCRectMake(0, 0, kWinSize.width, kWinSize.height));

#ifdef USE_MGSDK
		NDPicture* pic = kPool.AddPicture( NDPath::GetImgPath("Res00/Load/mobage_bg.png") );
#else
#if CACHE_MODE
		NDPicture* pic = kPool.AddPicture("bg_load.png");
#else
		NDPicture* pic = kPool.AddPicture( NDPath::GetImg00Path("Res00/Load/bg_load.png") );
#endif
#endif
		if (pic) 
		{
			pkBackgroundImage->SetPicture(pic, true);
		}

		pkLayer->AddChild(pkBackgroundImage);

		//layer->SetFrameRect( CCRectMake(winSize.width*0.0, winSize.height*0.0, winSize.width*0.7, winSize.height*0.225f));
		//layer->SetBackgroundColor( ccc4( 20,30,0,50) );
		CCLog( "@@login01: open CSMLoginScene\r\n" );
		LOGD("TAG_TIMER_FIRST_RUN is register");
		pkScene->m_pTimer->SetTimer( pkScene, TAG_TIMER_FIRST_RUN,0.5f );
    }
	return pkScene;
}

//===========================================================================
CSMLoginScene::CSMLoginScene()
: m_bUpdOk(false)
, m_pLayerOld(NULL)
, m_pLayerUpdate(NULL)
, m_pTimer(NULL)
, m_pCtrlProgress(NULL)
, m_pLabelPromtp(NULL)
, m_iAccountID(0)
, m_iState(0)
, m_pLayerCheckWIFI(NULL)
{
	INC_NDOBJ_RTCLS

	WriteCon( "%08X: CSMLoginScene::CSMLoginScene()\r\n", this);
}

//===========================================================================
CSMLoginScene::~CSMLoginScene()
{
	DEC_NDOBJ_RTCLS
   
	WriteCon( "%08X: CSMLoginScene::~CSMLoginScene()\r\n", this);

   	SAFE_DELETE( m_pTimer );

	NDDirector::DefaultDirector()->Recyle();
}

//===========================================================================
void CSMLoginScene::Initialization(void)
{
	NDScene::Initialization();
	//m_doucumentPath = NDPath::GetDocumentPath();
	m_strCachePath = NDPath::GetCashesPath();
	m_strSavePath = m_strCachePath + "supdate.zip";
	//m_resPath = NDPath::GetResPath();
	PackageCount = 0;
	m_pTimer = new NDTimer();
}

//===========================================================================
void CSMLoginScene::OnTimer( OBJID idTag )
{
	static bool bFirst = true;

	if (bFirst)
	{
		LOGD("Entry First OnTimer");
		//idTag = TAG_TIMER_UPDATE;
		bFirst = false;
	}

	if ( idTag == TAG_TIMER_UPDATE ) 
	{
		if ( !rename( m_strSavePath.c_str(), m_strSavePath.c_str() ) )
		{
			if ( remove( m_strSavePath.c_str() ) )
			{ 
				m_pTimer->KillTimer(this, TAG_TIMER_UPDATE);
				return;
			}
		}

		FromUrl(m_strUpdateURL.c_str());
		ToPath(m_strSavePath.c_str()); 
		Download();
		m_pTimer->KillTimer(this, TAG_TIMER_UPDATE);
	}
	else if ( idTag == TAG_TIMER_DOWNLOAD_SUCCESS )
	{
		m_pTimer->KillTimer(this, TAG_TIMER_DOWNLOAD_SUCCESS);
        
		LOGD("Entry TAG_TIMER_DOWNLOAD_SUCCESS process.");
		UnZipFile( m_strSavePath.c_str(), m_strCachePath.c_str());
	}
    else if ( idTag == TAG_TIMER_UNZIP_SUCCESS )
	{
		m_pTimer->KillTimer(this, TAG_TIMER_UNZIP_SUCCESS);
		if ( remove(m_strSavePath.c_str()) )
		{
		    NDLog("delete:%s failed",m_strSavePath.c_str());
		    //return;
		}
        std::string szListFile = NDPath::GetCashesPath() + SZ_DEL_FILE;
		DeleteFileFromFile( szListFile );
    
		if(kDeqUpdateUrl.size() > 0)
		{
		    kDeqUpdateUrl.pop_front();
		}

		PackageCount++;
		//�������ض���
		if (kDeqUpdateUrl.size() > 0)
		{
		    //���屣��·��
		    m_strUpdateURL = *kDeqUpdateUrl.begin();
		    //m_savePath = [[NSString stringWithFormat:@"%s/update%d.zip", m_cachPath.c_str(), PackageCount] UTF8String];
		    m_pTimer->SetTimer( this, TAG_TIMER_UPDATE, 0.5f );
		    StartDownload();
		}
		else
		{
		    //��ת����������
		    StartEntry();
		}
	}
	else if ( TAG_TIMER_CHECK_WIFI == idTag )
	{
		//������û����WIFI�򲻶ϼ��//
    	if ( NDBeforeGameMgrObj.isWifiNetWork() )
    	{
			m_pTimer->KillTimer( this, TAG_TIMER_CHECK_WIFI );
			CloseConfirmDlg();
			StartUpdate();
    	}
	}
	else if ( TAG_TIMER_CHECK_UPDATE == idTag )
	{
        m_pTimer->KillTimer(this, TAG_TIMER_CHECK_UPDATE);
        ShowUpdateOff();
	}
	else if ( TAG_TIMER_CHECK_COPY == idTag )
	{
        int nCopyStatus = NDBeforeGameMgr::GetCopyStatus();
        switch (nCopyStatus) 
        {
            case -1:
                m_pTimer->KillTimer( this, TAG_TIMER_CHECK_COPY );
                LOGERROR("Copy files error!");
                exit(0);
                break;
            case 0:
                break;
            case 1:
				LOGD("Copy files succeeded!");
                m_pTimer->KillTimer( this, TAG_TIMER_CHECK_COPY );
                NDBeforeGameMgrObj.doNDSdkLogin();
                ShowWaitingAni();
				StartEntry();
                break;
            default:
                break;
        }
	}
	else if (TAG_TIMER_CHECK_LOGIN_COPY == idTag)
	{
		int nCopyStatus = NDBeforeGameMgr::GetLoginCopyStatus();

		switch(nCopyStatus)
		{
		case 1:
			{
				LOGD("Copy login res succeeded!");
				CreateUpdateUILayer();
			}
			break;
		default:
			break;
		}
	}
    else if ( TAG_TIMER_FIRST_RUN == idTag )
	{
		LOGD("Entry TAG_TIMER_FIRST_RUN == idTag");
		m_pTimer->KillTimer( this, TAG_TIMER_FIRST_RUN );
		
		CreateUpdateUILayer();

// #if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
 		CCLog( "@@login02: to call OnEvent_LoginOKNormal()\r\n" );
		m_iAccountID = NDBeforeGameMgrObj.GetCurrentUser();
		OnEvent_LoginOKNormal(m_iAccountID);
// #else
#ifdef USE_MGSDK
// 		NDUIImage * pImage = (NDUIImage *)m_pLayerUpdate->GetChild( TAG_CTRL_PIC_BG);
// 		if ( pImage )
// 		{
// 			NDPicture * pPicture = new NDPicture;
// 			pPicture->Initialization( NDPath::GetUIImgPath( SZ_MOBAGE_BG_PNG_PATH ).c_str() );
// 			pImage->SetPicture( pPicture, true );
// 		}
#endif
#if CACHE_MODE == 1
    	if ( NDBeforeGameMgrObj.CheckFirstTimeRuning() )
        {
        	if ( m_pLabelPromtp )
            {
        		m_pLabelPromtp->SetText( NDCommonCString2(SZ_FIRST_INSTALL).c_str() );
        		m_pLabelPromtp->SetVisible( true );
                ShowWaitingAni();
#ifdef USE_MGSDK
        		m_pLabelPromtp->SetVisible( false );//Mobage�İ汾�ݽ����ֻ��ڱ���ͼ��
#endif
            }
			
			m_pTimer->SetTimer( this, TAG_TIMER_CHECK_COPY, 0.5f );
        }
        else
        {
            NDBeforeGameMgrObj.doNDSdkLogin();
			CloseWaitingAni();
			StartEntry();
           // ShowWaitingAni();
		}
#else
		NDBeforeGameMgrObj.doNDSdkLogin();
		CloseWaitingAni();
#endif
    	//CreateUpdateUILayer();
		//NDBeforeGameMgrObj.CheckClientVersion(SZ_UPDATE_URL);
	}
	else if ( TAG_TIMER_LOAD_RES_OK == idTag )
	{
		m_pTimer->KillTimer( this, TAG_TIMER_LOAD_RES_OK );
		CloseWaitingAni();
		CloseUpdateUILayer();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		//if ( m_iAccountID == 0 )
		m_iAccountID = ScriptMgrObj.excuteLuaFuncRetN( "GetAccountID", "Login_ServerUI" );
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		m_iAccountID = NDBeforeGameMgrObj.GetCurrentUser();
#endif
		ScriptMgrObj.excuteLuaFunc( "ShowUI", "Entry", m_iAccountID );
		//    ScriptMgrObj.excuteLuaFunc("ProecssLocalNotification", "MsgLoginSuc");
	}
}

//==========================================================
//--Guosen 2012.8.7
//int     
//CSMLoginScene::OnProcess(int nPercent)
//{
//    CUIExp* pProcess = (CUIExp*)m_layer->GetChild(ID_LOADING_PROCESS);
//	if (!pProcess) {
//        return 0;
//    }
//    pProcess->SetProcess(nPercent);
//    return 0;
//}
//==========================================================
//--Guosen 2012.8.7
//int     
//CSMLoginScene::OnFail(const char* pszStrErr)
//{
//    return 0;
//}
//==========================================================
//--Guosen 2012.8.7
//int		
//CSMLoginScene::OnComplete(void)
//{
//    m_bUpdOk = true;
//    return 0; 
//}
//===========================================================================
// ��������
bool CSMLoginScene::StartUpdate()
{
	if ( kDeqUpdateUrl.empty() )
	{
		return false;
	}
	//�����һ����
	std::string url = *kDeqUpdateUrl.begin();
	m_strUpdateURL	= url;
	m_pTimer->SetTimer( this, TAG_TIMER_UPDATE, 0.5f );	
	StartDownload();
	return true;
}


//===========================================================================
//������������̵Ļص�
//������bOpContinue �Ƿ���ֹ�������
//������nFileNum Ҫ��������ļ���
//������nFileIndex Ҫ����ĵ�ǰ�ļ�����
//������pszFileName �ļ���������·��
void CSMLoginScene::OnUnCompressEvent( bool &bOpContinue,int nFileNum,int nFileIndex,const char* pszFileName )
{
	int percent = (nFileIndex + 1) *100/ nFileNum;
	SetProgress( percent );
}

//===========================================================================
//��������������Ļص�
//������emErrCode �������
//������pszErrMsg ������ʾ
void CSMLoginScene::OnTQZipError(ISMUpdateEvent::ERROR_CODE emErrCode,const char* pszErrMsg)
{
	NDLog("OnTQZipError %s", pszErrMsg);
}

///////////////////////////////////////////////
//bool 
//CSMLoginScene::CheckClientVersion()
//{
//    int nCurVer = 0;
//	NSString *strIniPath = [NSString stringWithFormat:@"%s", NDPath::GetResPath("version.ini")];
//	NSInputStream *stream  = [NSInputStream inputStreamWithFileAtPath:strIniPath];
//	if (!stream)
//	{
//		return false;
//	}
//	//nCurVer = [stream readInt];
//    NDDataTransThread::DefaultThread()->Stop();
//    NDDataTransThread::ResetDefaultThread();
//    NDDataTransThread::DefaultThread()->Start("192.168.64.30", 9500);
//	if (NDDataTransThread::DefaultThread()->GetThreadStatus() != ThreadStatusRunning)	
//	{
//		return false;
//	}
//	NDTransData data(_MSG_CLIENT_VERSION);
//	
//	int version = 1;
//	data << version;
//	NDDataTransThread::DefaultThread()->GetSocket()->Send(&data);
//
//}
//===========================================================================
/*void 
CSMLoginScene::OnDrawUI()
{
    (NDDirector::DefaultDirector())->ForceDraw();
}
*/
//===========================================================================
void 
CSMLoginScene::OnDownloadEvent(DWORD dwSizeFile,DWORD dwSideDownLoaded)
{
    float nProcess = (float)dwSideDownLoaded/(float)dwSizeFile;
    //ScriptMgrObj.excuteLuaFunc("OnDownloadEvent", "Login_Upd",int(nProcess*60));
}
//===========================================================================
void 
CSMLoginScene::OnUnCompress(int nFileNum,int nFileIndex,const char* pszFileName)
{
    //ScriptMgrObj.excuteLuaFunc("OnUnCompress", "Login_Upd");
}
//===========================================================================
void 
CSMLoginScene::CompleteUpdate(ISMUpdateEvent::ERROR_CODE emErrCode)
{
    //ScriptMgrObj.excuteLuaFunc("CompleteUpdate", "Login_Upd");
}
//===========================================================================
void 
CSMLoginScene::OnError(ISMUpdateEvent::ERROR_CODE emErrCode,const char* pszErrMsg)
{
	if(emErrCode == ISMUpdateEvent::ERRCODE_VERSION_LATEST){
        return;
    }
    //ScriptMgrObj.excuteLuaFunc("OnUpdError", "Login_Upd",pszErrMsg);
}

//===========================================================================
void CSMLoginScene::ReflashPercent(int percent, int pos, int filelen )
{
    /*
	if (m_label) 
	{
		//NSString *str = [NSString stringWithFormat:@"�����أ�%d\%", percent];
		//m_label->SetText([str UTF8String]);
		char buff[100] = {0x00};
		sprintf(buff, "�����أ�%d\%%", percent);
		m_label->SetText(buff);
		
		m_progressBar->SetCurrentStep(percent);
	}
    */ 
	SetProgress( percent );
}

//===========================================================================
void CSMLoginScene::DidDownloadStatus( DownloadStatus status )
{
	if (status == DownloadStatusResNotFound) 
	{
		//m_label->SetText( "��Ǹ��������Դδ�ҵ�������ϵGM" );
		if (m_pLabelPromtp)
		{
			m_pLabelPromtp->SetText( NDCommonCString2(SZ_ERROR_04).c_str() );
			m_pLabelPromtp->SetFontColor( ccc4(0xFF,0x0,0x0,255) );
			//m_pLabelPromtp->SetFontSize( 20 );
			//CCRect tRect = m_pLabelPromtp->GetFrameRect();
			//m_pLabelPromtp->SetFrameRect( CCRectMake( tRect.origin.x, tRect.origin.y, tRect.size.width*3, tRect.size.height*2));
			//m_pLabelPromtp->SetVisible( true );
		}
	}
	else if (status == DownloadStatusFailed)
	{
		if (m_pLabelPromtp)
		{
			//m_label->SetText( "����ʧ�ܣ������������ӻ��������豸����" );
			m_pLabelPromtp->SetText( NDCommonCString2(SZ_ERROR_05).c_str() );
			m_pLabelPromtp->SetFontColor( ccc4(0xFF,0x0,0x0,255) );
			//m_pLabelPromtp->SetFontSize( 20 );
			//CCRect tRect = m_pLabelPromtp->GetFrameRect();
			//m_pLabelPromtp->SetFrameRect( CCRectMake( tRect.origin.x/2, tRect.origin.y, tRect.size.width*3, tRect.size.height*2));
			//m_pLabelPromtp->SetVisible( true );
		}
	}
	else 
	{
		//m_label->SetText("������ɣ����ڽ��а�װ���������Ժ�......");		
		m_pTimer->SetTimer( this, TAG_TIMER_DOWNLOAD_SUCCESS, 0.5f );
		StartInstall();
	}
}

//===========================================================================
//wt
// ��ʼ�����¶���
void CSMLoginScene::InitDownload( std::string & szUpdatePath )
{
	kDeqUpdateUrl.push_back(szUpdatePath);
}

//++Guosen2012.8.7
void CSMLoginScene::ShowRequestError()
{
	NDUIDialog* dlg = new NDUIDialog();
	dlg->Initialization();
	dlg->SetTag(TAG_REQUEST_URL_ERROR);
	dlg->SetDelegate(this);
	dlg->Show(NDCommonCString2("Common_error").c_str(), NDCommonCString2("LOGIN_SZ_REQUEST_DOWNLOAD_FAIL").c_str(), NULL, NDCommonCString2("Common_Ok").c_str(), NULL);
}

//===========================================================================
//ͨ�����ݽ����ļ�·������ô�ɾ�����ļ�·����ɾ����ɾ�����ļ���ɾ�����ݽ����ļ�
bool CSMLoginScene::DeleteFileFromFile( std::string & szDelListFile )
{
	std::ifstream	tmpFile;
	tmpFile.open( szDelListFile.c_str(), ios_base::in );
	if ( !tmpFile )
	{
		if ( tmpFile.is_open() )
			tmpFile.close();
		return false;
	}
	std::string  lineStr;
	while ( getline( tmpFile, lineStr ) )
	{
		std::string DelFile = m_strCachePath + lineStr;
 		if ( remove( DelFile.c_str() ) )
		{
			NDLog( "ɾ���ļ�ʧ�ܣ�%s",DelFile.c_str() );
		}
	}
	tmpFile.close();
	remove( szDelListFile.c_str() );
	return true;
}

//===========================================================================
bool CSMLoginScene::CreateUpdateUILayer()
{
	LOGD("Entry CreateUpdateUILayer");

	if ( m_pLayerUpdate )
	{
		LOGERROR("m_pLayerUpdate is null");
		return false;
	}
	
	CCSize kWinSize = CCDirector::sharedDirector()->getWinSizeInPixels();
	
	NDUILayer *	pLayer = new NDUILayer();

	if ( !pLayer )
	{
		LOGERROR( "CSMLoginScene::CreateUpdateUILayer() pLayer is null" );
		return false;
	}

	pLayer->Initialization();
	pLayer->SetFrameRect( CCRectMake(0, 0, kWinSize.width, kWinSize.height) );
	pLayer->SetTag( TAG_UPDATE_LAYER );
	AddChild(pLayer);
	m_pLayerUpdate = pLayer;
	
	NDUILoad tmpUILoad;

	LOGD("Ready to load update.ini file");
	tmpUILoad.Load( "UpdateUI.ini", pLayer, this, CCSizeMake(0, 0));
	
	m_pCtrlProgress	= (CUIExp*)pLayer->GetChild( TAG_CTRL_PROGRESS );
	if ( !m_pCtrlProgress )
	{
		NDLog( "CSMLoginScene::CreateUpdateUILayer() m_pCtrlProgress is null" );
		return false;
	}

	m_pCtrlProgress->SetProcess(0);
	m_pCtrlProgress->SetTotal(100);
	m_pCtrlProgress->SetStyle(2);
	m_pCtrlProgress->SetVisible(false);
	
	m_pLabelPromtp	= (NDUILabel*)pLayer->GetChild( TAG_LABEL_PROMPT );
	if ( !m_pLabelPromtp )
	{
		NDLog( "CSMLoginScene::CreateUpdateUILayer() m_pLabelPromtp is null" );
		return false;
	}
	if (m_pLabelPromtp) m_pLabelPromtp->SetVisible(false);
	if (m_pLayerOld) m_pLayerOld->SetVisible(false);
	return true;
}

//===========================================================================
void CSMLoginScene::CloseUpdateUILayer()
{
	if ( m_pLayerUpdate )
	{
		m_pLayerUpdate->RemoveFromParent(true);
		m_pLayerUpdate	= NULL;
		m_pCtrlProgress	= NULL;
		m_pLabelPromtp	= NULL;
	}
}

//===========================================================================
void CSMLoginScene::OnMsg_ClientVersion(NDTransData& kData)
{
	bool bUpdate = false;
	
	int bLatest				= kData.ReadByte();
	int bForceUpdate		= kData.ReadByte();
	int FromVersion			= kData.ReadInt();
	int ToVersion			= kData.ReadInt();
	std::string UpdatePath	= kData.ReadUnicodeString();
	
	if ( bForceUpdate )
	{
        CloseWaitingAni();
		//printf("���û���������������Ϸ�汾");
		if ( m_pLabelPromtp )
		{
			m_pLabelPromtp->SetText( NDCommonCString2(SZ_ERROR_01).c_str() );
			m_pLabelPromtp->SetFontColor( ccc4(0xFF,0x0,0x0,255) );
    		m_pLabelPromtp->SetVisible( true );
    		//m_pLabelPromtp->SetFontSize( 20 );
		}
		return ;
	}
	else if ( ( FromVersion ==  ToVersion ) &&  ( !bLatest ) )
	{
        CloseWaitingAni();
		//printf("��ǰ�汾��������,���������ػ�����ϵGM");
		if ( m_pLabelPromtp )
		{
			m_pLabelPromtp->SetText( NDCommonCString2(SZ_ERROR_02).c_str() );
			m_pLabelPromtp->SetFontColor( ccc4(0xFF,0x0,0x0,255) );
    		m_pLabelPromtp->SetVisible( true );
    		//m_pLabelPromtp->SetFontSize( 20 );
		}
		return ;
	}
	else if ( ( FromVersion == 0 ) && ( ToVersion == 0 ) )
	{
        CloseWaitingAni();
		//printf("�汾��Ϣ�𻵣����������ػ�����ϵGM");
		if ( m_pLabelPromtp )
		{
			m_pLabelPromtp->SetText( NDCommonCString2(SZ_ERROR_03).c_str() );
			m_pLabelPromtp->SetFontColor( ccc4(0xFF,0x0,0x0,255) );
    		m_pLabelPromtp->SetVisible( true );
    		//m_pLabelPromtp->SetFontSize( 20 );
		}

		return ;
	}
	else if ( ( FromVersion == ToVersion ) && (bLatest) )
	{
		//printf("��ǰ�汾��������Ϸ�汾");
		StartEntry();
		return;
	}
	else
	{
		bUpdate = true;
	}
	    
	NDLog("URL:%s",UpdatePath.c_str());
	kDeqUpdateUrl.push_back(UpdatePath);
	if (bUpdate)
	{
		if (!bLatest) 
		{
			CloseWaitingAni();
			//if ( !NDBeforeGameMgrObj.isWifiNetWork() )//�رյ��ӵ���WIFI���
			//{
			//	ShowCheckWIFIOff();
			//	m_pTimer->SetTimer( this, TAG_TIMER_CHECK_WIFI, 1.0f );
			//}
			//else
			{
				StartUpdate();
			}
		}
	}
}

//===========================================================================

void CSMLoginScene::OnEvent_LoginOKNormal( int iAccountID )
{
	CCLog( "@@login03: OnEvent_LoginOKNormal()\r\n" );

	m_iAccountID = iAccountID;
#ifdef USE_MGSDK
    if(m_pLayerUpdate)
    {
        NDUIImage * pImage = (NDUIImage *)m_pLayerUpdate->GetChild( TAG_CTRL_PIC_BG);
        if ( pImage )
        {
            NDPicture * pPicture = new NDPicture;
            std::string str = SZ_UPDATE_BG_PNG_PATH;
            pPicture->Initialization( NDPath::GetUIImgPath( str.c_str() ).c_str() );
            pImage->SetPicture( pPicture, true );
        }
    }
#endif
	
#if (UPDATE_ON == 0 && CACHE_MODE == 0)
		CloseWaitingAni();
		StartEntry();
#endif
#if UPDATE_ON == 1
	const char*	pszUpdateURL	= SZ_UPDATE_URL;//ScriptMgrObj.excuteLuaFuncRetS( "GetUpdateURL", "Update" );//��ʱLua�ű�δ���ء���
	if ( !pszUpdateURL )
	{
		CloseWaitingAni();
		StartEntry();
		return;
	}
		
	if (m_pLabelPromtp)
	{
		m_pLabelPromtp->SetText( NDCommonCString2(SZ_CONNECT_SERVER).c_str() );
		m_pLabelPromtp->SetVisible( true );
	}
	if ( !NDBeforeGameMgrObj.CheckClientVersion( pszUpdateURL ) )
	{
		CloseWaitingAni();
		StartEntry();
	}
#endif
}

//---------------------------------------------------------------------------
void CSMLoginScene::OnEvent_LoginOKGuest( int iAccountID )
{
	OnEvent_LoginOKNormal( iAccountID );
}

//---------------------------------------------------------------------------
void CSMLoginScene::OnEvent_LoginOKGuest2Normal( int iAccountID )
{
	OnEvent_LoginOKNormal( iAccountID );
}

//---------------------------------------------------------------------------
void CSMLoginScene::OnEvent_LoginError( int iError )
{
    std::stringstream  tmpSS;
    tmpSS << "Error:" << iError;
	if ( m_pLabelPromtp )
    {
		m_pLabelPromtp->SetVisible( true );
		m_pLabelPromtp->SetText( tmpSS.str().c_str() );
		m_pLabelPromtp->SetVisible( true );
    }
}

//===========================================================================
void CSMLoginScene::StartDownload()
{
	if ( m_pLabelPromtp )
	{
		m_pLabelPromtp->SetText( NDCommonCString2(SZ_DOWNLOADING).c_str() );
		m_pLabelPromtp->SetVisible( true );
	}
	if ( m_pCtrlProgress )
	{
		m_pCtrlProgress->SetVisible( true );
	}
}

//---------------------------------------------------------------------------
void CSMLoginScene::StartInstall()
{
	if ( m_pLabelPromtp )
	{
		m_pLabelPromtp->SetText( NDCommonCString2(SZ_INSTALLING).c_str() );
		m_pLabelPromtp->SetVisible( true );
	}
	if ( m_pCtrlProgress )
	{
		m_pCtrlProgress->SetVisible( true );
	}
}
//---------------------------------------------------------------------------
void CSMLoginScene::SetProgress( int nPercent )
{
	NDLog("CSMLoginScene::SetProgress() nPercent:%d",nPercent);
	if ( m_pCtrlProgress )
	{
		m_pCtrlProgress->SetProcess( nPercent );
	}
}

//===========================================================================
void CSMLoginScene::StartEntry()
{
	WriteCon( "@@ CSMLoginScene::StartEntry()\r\n" );
	CCLog( "@@login04: StartEntry()\r\n" );

#if 1
	if (m_pLabelPromtp)
	{
		m_pLabelPromtp->SetText( NDCommonCString2(SZ_SETUP).c_str() );
		m_pLabelPromtp->SetVisible( true );
	}

	ShowWaitingAni();

	{
		WriteCon( "@@ NDLocalXmlString::LoadData()...\r\n" );
		TIME_SLICE("NDLocalXmlString::LoadData()");
		NDLocalXmlString::GetSingleton().LoadData();
	}

	{
		WriteCon( "@@ ScriptMgrObj.Load()...\r\n" );
		TIME_SLICE("ScriptMgrObj.Load()");
		ScriptMgrObj.Load(); //����LUA�ű�
	}

	ScriptMgrPtr->excuteLuaFunc( "LoadData", "GameSetting" ); 
	CloseUpdateUILayer();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	//if ( m_iAccountID == 0 )
	m_iAccountID = ScriptMgrPtr->excuteLuaFuncRetN( "GetAccountID", "Login_ServerUI" );
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	m_iAccountID = NDBeforeGameMgrObj.GetCurrentUser();
#endif

	ScriptMgrPtr->excuteLuaFunc( "ShowUI", "Entry", m_iAccountID );
	//    ScriptMgrObj.excuteLuaFunc("ProecssLocalNotification", "MsgLoginSuc");

#else //���̲߳�����ʲô�ô��������Ǳ����Ͳ��ȶ���
	  //ʵ���������̺߳Ϳ���̨�̶߳��Ƕ���ģ����߳��㹻�ˣ�
	if (m_pLabelPromtp)
	{
		m_pLabelPromtp->SetText( NDCommonCString2(SZ_SETUP).c_str() );
		m_pLabelPromtp->SetVisible( true );
	}
	ShowWaitingAni();
	NDLocalXmlString::GetSingleton();
	ScriptMgrObj;
	pthread_t pid = {0};
	pthread_create(&pid, NULL, CSMLoginScene::LoadTextAndLua, (void*)this);	
#endif

	CCLOG( "@@ CSMLoginScene::StartEntry() -- done.\r\n" );
}

//===========================================================================
void CSMLoginScene::OnDialogButtonClick(NDUIDialog* dialog, unsigned int buttonIndex)
{
	if (dialog->GetTag() == TAG_DLG_CONFIRM)
	{
    	NDBeforeGameMgrObj.doNDSdkLogin();
	}
}

//===========================================================================
bool CSMLoginScene::OnTargetBtnEvent( NDUINode * uiNode, int targetEvent )
{
	int	iTag = uiNode->GetTag();
	if ( TE_TOUCH_BTN_CLICK == targetEvent )
	{
		if ( TAG_BTN_OK == iTag ) 
		{
			CloseConfirmDlg();
			StartUpdate();
		}
		else if ( TAG_BTN_CANCEL == iTag ) 
		{
			exit(0);
		}
	}
	return true;
}



//===========================================================================
bool CSMLoginScene::CreatConfirmDlg( const char * szTip )
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSizeInPixels();
	
	NDUILayer *	pLayer	= new NDUILayer();

	if (!pLayer)
	{
		return false;
	}

	pLayer->Initialization();
	pLayer->SetFrameRect( CCRectMake(0, 0, winSize.width, winSize.height) );
	pLayer->SetTag( TAG_DLG_CONFIRM );
	AddChild(pLayer);	
	
	NDUILoad tmpUILoad2;
	tmpUILoad2.Load( "ShowYesOrNoDlg.ini", pLayer, this, CCSizeMake(0, 0) );
	
	NDUILabel * pLabelTip	= (NDUILabel*)pLayer->GetChild( TAG_LABEL_TIP );
	if ( pLabelTip && szTip )
	{
		pLabelTip->SetText( szTip );
	}
	return true;
}
void CSMLoginScene::CloseConfirmDlg()
{
	RemoveChild( TAG_DLG_CONFIRM, true );
}

//===========================================================================
void CSMLoginScene::UnzipPercent(int nFileNum,int nFileIndex)
{    
    int nPercent = 100*(nFileIndex+1)/nFileNum;
    SetProgress( nPercent );
}

void CSMLoginScene::UnzipStatus(bool bResult)
{
	if (!bResult) 
    {
        NDLog("UnZipFile:%s failed",m_strSavePath.c_str());
        //return;
    }
	m_pTimer->SetTimer( this, TAG_TIMER_UNZIP_SUCCESS, 0.5f );	
}

//===========================================================================
//��ʾ�ȴ���תȦȦ����
void CSMLoginScene::ShowWaitingAni()
{
	CUISpriteNode * pNode = (CUISpriteNode *)GetChild(TAG_SPRITE_NODE);
	if ( pNode )
	{
		return;
	}
	CCSize winSize = CCDirector::sharedDirector()->getWinSizeInPixels();	
	CUISpriteNode *node = new CUISpriteNode;
	node->Initialization();
	node->ChangeSprite(NDPath::GetAniPath("busy.spr").c_str());
	node->SetTag( TAG_SPRITE_NODE );
	node->SetFrameRect(CCRectMake(0, 0, winSize.width, winSize.height));
	AddChild(node);
}
void CSMLoginScene::CloseWaitingAni()
{
	RemoveChild( TAG_SPRITE_NODE, true );
}

//===========================================================================
//��ʾ���WIFIʧ�ܶԻ���
void CSMLoginScene::ShowCheckWIFIOff()
{
	//CCSize winSize = CCDirector::sharedDirector()->getWinSizeInPixels();
	//
	//NDUILayer *	pLayer	= new NDUILayer();
	//if ( !pLayer )
	//	return;
	//pLayer->Initialization();
	//pLayer->SetFrameRect( CCRectMake(0, 0, winSize.width, winSize.height) );
	//AddChild(pLayer);
	//m_pLayerCheckWIFI = pLayer;
	//NDUILoad tmpUILoad;
	//tmpUILoad.Load( "CheckWIFIDlg.ini", pLayer, this, CCSizeMake(0, 0) );
	CreatConfirmDlg( NDCommonCString2(SZ_WIFI_OFF).c_str() );
	m_iState = 1;
}

//��ʾ��������ʧ�ܶԻ���
void CSMLoginScene::ShowUpdateOff()
{
	CreatConfirmDlg( NDCommonCString2(SZ_UPDATE_OFF).c_str() );
	m_iState = 2;
}

//װ���ı���Lua//���߳�
void * CSMLoginScene::LoadTextAndLua( void * pPointer )
{
	if ( pPointer )
	{
		CSMLoginScene * pScene = (CSMLoginScene*)pPointer;
		NDLocalXmlString::GetSingleton().LoadData();
		//ScriptMgrObj.Load();//
		ScriptMgrObj.excuteLuaFunc( "LoadData", "GameSetting" ); 
		pScene->m_pTimer->SetTimer( pScene, TAG_TIMER_LOAD_RES_OK,0.05f );
	}
	return pPointer;
}