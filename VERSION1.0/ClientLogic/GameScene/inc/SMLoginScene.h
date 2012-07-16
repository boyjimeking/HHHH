//
//  SMLoginScene.h
//  SMYS
//
//  Created by user on 12-3-21.
//  Copyright 2012�� (����)DeNA. All rights reserved.
//

#ifndef SMYS_SMLoginScene_h
#define SMYS_SMLoginScene_h

#include "NDScene.h"
#include "NDUIButton.h"
#include "NDUIlayer.h"
//#include <WndManager.h>
//#include <GameApp.h>
//#include <UpdateFactory.h>
#include "NDTimer.h"
#include "NDUILoad.h"
#include "UIExp.h"
#include <Define.h>
/////< #include "NDMapMgr.h" ��ʱ��ע�� ����
#include "BattleMgr.h"
#include "NDColorPool.h"
//#include "NDDataPersist.h"
#include "NDLocalXmlString.h"
//#include "ItemMgr.h"
//#include "cpLog.h"
#include "NDUtility.h"
//#include "FarmMgr.h"
//#include "BattleFieldMgr.h"
//#include "NDCrashUpload.h"
//#include "UpdateFactory.h"

#define ID_LOADING_PROCESS (90)

class CSMLoginScene :
public NDScene,
public NDUIButtonDelegate,
public NDUITargetDelegate,
public ITimerCallback
{
	DECLARE_CLASS(CSMLoginScene)
	
	CSMLoginScene();
	~CSMLoginScene();
	
	static CSMLoginScene* Scene();
	
public:
	void Initialization(); override
    static std::string GetRandomWords(int nNum);
    
public:
    //interface of IUpdateEvent
// 	virtual void OnDownloadEvent(DWORD dwSizeFile,DWORD dwSideDownLoaded);
// 	virtual void OnUnCompress(int nFileNum,int nFileIndex,const char* pszFileName);
// 	virtual void CompleteUpdate(ERROR_CODE emErrCode);
// 	virtual void OnError(ERROR_CODE emErrCode,const char* pszErrMsg);
    
private:
    void OnTimer(OBJID idTag);
	
public:
    //interface of download
    virtual int     OnProcess(int nPercent); //���½��ȷ���
    virtual int     OnFail(const char* pszStrErr);//����ʧ��
    virtual int		OnComplete(void); //�������
    
private:
    NDUILayer* m_layer;
    NDTimer     m_rTimer;
    bool       m_bUpdOk;
};
#endif
