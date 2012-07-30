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
//#include <iconv.h>
#include "NDConstant.h"
#include "GameApp.h"

IMPLEMENT_CLASS(CSMLoginScene, NDScene)
////////////////////////////////////////////////////////////
CSMLoginScene* CSMLoginScene::Scene()
{
	CSMLoginScene *scene = new CSMLoginScene;
    scene->Initialization();
    scene->SetTag(SMLOGINSCENE_TAG);
	return scene;
}

////////////////////////////////////////////////////////////
CSMLoginScene::CSMLoginScene()
:m_bUpdOk(false)
{
	//m_miniMap	= NULL;	
	//m_mapLayer	= NULL;
    m_rTimer.SetTimer(this,ONTIMER_TAG_LOGIN,1);
}

////////////////////////////////////////////////////////////
CSMLoginScene::~CSMLoginScene()
{
    m_rTimer.KillTimer(this,ONTIMER_TAG_LOGIN);
}

////////////////////////////////////////////////////////////
void 
CSMLoginScene::Initialization()
{
	NDScene::Initialization();
	
	CGSize winSize = NDDirector::DefaultDirector()->GetWinSize();
	
    m_layer = new NDUILayer();
	m_layer->Initialization();
	m_layer->SetFrameRect(CGRectMake(0, 0, winSize.width, winSize.height));
	this->AddChild(m_layer);

    NDUILoad ui;

	ui.Load("Loading.ini", m_layer, this, CGSizeMake(0, 37));
    CUIExp* pProcess = (CUIExp*)m_layer->GetChild(ID_LOADING_PROCESS);
    if (!pProcess) 
	{
        return;
    }
    
    pProcess->SetProcess(0);
    pProcess->SetTotal(100);
    pProcess->SetStyle(1);
   // while (!m_bUpdOk) {
        //������Դ���Ŀ¼

/***
* ��ʱ��ע�� ����
* begin
*/
	//     std::string strPath = CGameApp::sharedInstance().GetAppPath();
//     strPath.append("/RES/");
//     
//     bool bInit = CUpdateFactory::sharedInstance().
// 		Init("http://192.168.64.140:5898","smys",strPath.c_str(),this);
//     if(!bInit)
// 	{
//         return ;
//     }
//     
//     if(!CUpdateFactory::sharedInstance().CheckVersion())
// 	{
//         CUpdateFactory::sharedInstance().Update();
//     }
  //  }
/***
* ��ʱ��ע�� ����
* end
*/
    /*
    if(m_pClientUpd){
        int nVersion = m_pClientUpd->CheckClientVersion();
        m_pClientUpd->StartUpdate(nVersion);  
    }*/
   //ui.free();
}

////////////////////////////////////////////////////////////
 void 
 CSMLoginScene::OnTimer(OBJID idTag)
 {
     //if (idTag != ONTIMER_TAG_LOGIN) {
     //    return;
     //}
     //
     //if(m_bUpdOk){
     //    ScriptGlobalEvent::OnEvent(GE_LOGIN_GAME);
     //    m_bUpdOk = false;
     //}
 }
// 
// //interface of IUpdateEvent
// ////////////////////////////////////////////////////////////
// void 
// CSMLoginScene::OnDownloadEvent(DWORD dwSizeFile,DWORD dwSideDownLoaded)
// {
//     printf("OnDownloadEvent");
// }
// ////////////////////////////////////////////////////////////
// void 
// CSMLoginScene::OnUnCompress(int nFileNum,int nFileIndex,const char* pszFileName)
// {
//     printf("OnUnCompress");
// }
// ////////////////////////////////////////////////////////////
// void 
// CSMLoginScene::CompleteUpdate(ERROR_CODE emErrCode)
// {
//     printf("CompleteUpdate");
// }
// ////////////////////////////////////////////////////////////
// void 
// CSMLoginScene::OnError(ERROR_CODE emErrCode,const char* pszErrMsg)
// {
//     printf("OnError");
// }


////////////////////////////////////////////////////////////
std::string
CSMLoginScene::GetRandomWords(int nNum)
{
    //std::string strBuf;
    srand((unsigned)time(NULL));
    unsigned char Name[64]={0};
    for(int n=0; n<nNum; n++){
        Name[n]=0xA0+16+rand()%(0xF7-0xB0);//���ֽ�.���һ��������0xA1-0xA9 �������
        int iRan = 0xFD-0xA1;
        if(Name[n]==0xD7)
        {
            iRan=0xFA-0xA1;
        }
        Name[n+1]=0xA1+rand()%iRan;//���ֽ�
    }
    
    //char tmp[128]={0};
    std::string curLocale = setlocale(LC_ALL, NULL);
    
    setlocale(LC_CTYPE, "UTF-8");
    
    wchar_t c;
    std::wstring wStr;
    for (int i = 0; i < 64; i += 2) {
        c = ((Name[i] & 0xff) | ((Name[i + 1] & 0xff) << 8));
        wStr += c;
    }
    
    char szStr[129] = { 0 };
    wcstombs(szStr, wStr.c_str(), 129);
    
    setlocale(LC_ALL, curLocale.c_str());
    return std::string(szStr);
}
////////////////////////////////////////////////////////////
int     
CSMLoginScene::OnProcess(int nPercent)
{
    CUIExp* pProcess = (CUIExp*)m_layer->GetChild(ID_LOADING_PROCESS);
    if (!pProcess) {
        return 0;
    }
    
    pProcess->SetProcess(nPercent);
    return 0;
}

////////////////////////////////////////////////////////////
int     
CSMLoginScene::OnFail(const char* pszStrErr)
{
    return 0;
}
////////////////////////////////////////////////////////////
int		
CSMLoginScene::OnComplete(void)
{
    m_bUpdOk = true;
    return 0; 
}

////////////////////////////////////////////////////////////
int InitGameInstance()
{
    // memory check
#ifdef VALGRIND
	if (argc < 2 || (argc >=2 && strcmp(argv[1], "-valgrind") != 0)) {
		execl(VALGRIND, VALGRIND, 
			  "--leak-check=summary",
			  "--show-reachable=yes",
			  "--undef-value-errors=no",
			  argv[0], "-valgrind", NULL);
	}
#endif
	
//	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init]; ///< ��ʱ��ע�� ����
	
#ifndef DEBUG
	/***
	* ��ʱ��ע�� ����
	* begin
	*/
	
	// 	if (![[NDCrashUpload Shared] EnableCrashCatch])
// 		NSLog(@"EnableCrashCatch failed!");
	/***
	* ��ʱ��ע�� ����
	* end
	*/
#endif
	
	// ��ȡ���ػ�����
	GetLocalLanguage();
	
	NDLocalXmlString::GetSingleton();
	
	//NDEngine::NDMapMgr dataObj;
	BattleMgr battleMgr;
	//ItemMgr itemMgr; ///< ��ʱ��ע�� ����
	NDColorPool colorPool;
	NDDataPersist::LoadGameSetting(); ///< ��ʱ��ע�� ����
	//NDFarmMgrObj;
	//BattleFieldMgrObj;
	
    ////////////////////////////////////////////
    //
    //	������־��¼����
    //	���ã��������й����в���ʱdebug�汾����release�汾������ͨ��cpLog��������¼��־
    //	��¼��־�ķ�ʽ��
    //		��xcode�Ĺ��̺궨�����ú�CPLOG
    //		���CPLOG��0������¼�κ���־
    //		���CPLOG��1����־��¼������cplogX����������������ַ���޸Ĵ���
    //		���CPLOG��2����־��¼���豸��"/tmp/DragonDrive.log"�У�ֻ��¼�������һ�����е���־
    //		���CPLOG��3���豸�ͷ�����ͬʱ��¼��־
    //	ע�⣺�����汾һ��Ҫ�ر�������־����CPLOG��0
    //	...
#if CPLOG & 0x01 
	// cplogĬ�Ϸ�����
	NDDataPersist data;
	if (strlen(data.GetData(kCpLogData, kCpLogServerIP)) == 0 || strlen(data.GetData(kCpLogData, kCpLogServerPort)) == 0) {
		data.SetData(kCpLogData, kCpLogServerIP, "192.168.55.176");
		data.SetData(kCpLogData, kCpLogServerPort, "2046");
		data.SaveData();
	}
	
	cpLogEnable(true);
	cpLogSetPriority(LOG_DEBUG);
	KNetworkAddress addr("192.168.55.176", 2046);
	//KNetworkAddress addr(data.GetData(kCpLogData, kCpLogServerIP), atoi(data.GetData(kCpLogData, kCpLogServerPort)));
	cpLogConnServer(addr);
#endif
	
#if CPLOG & 0x02 
	cpLogEnable(true);
	cpLogSetPriority(LOG_DEBUG);
	cpLogOpen("/tmp/DragonDrive.log");
#endif
    //	����������������
    ////////////////////////////////////////////
    return 0;
}