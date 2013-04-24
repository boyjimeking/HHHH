//
//  NDBaseDirector.mm
//  DragonDrive
//
//  Created by xiezhenghai on 10-12-15.
//  Copyright 2010 (网龙)DeNA. All rights reserved.
//

#include "NDBaseDirector.h"
#include "NDDirector.h"
#include <string>
#include <vector>
#include <NDTransData.h>
#include "NDBaseScriptMgr.h"
#include "NDBaseNetMgr.h"
#include <NDMessageCenter.h>
#include "NDDebugOpt.h"
#include "CCScheduler.h"
#include "NDBaseMsgDefine.h"
#include "StringConvert.h"
#include "NDDataTransThread.h"
#include "CCPlatformConfig.h"
#include "NDConstant.h"
#include "NDUtil.h"

// ��֪������
#define FRAME_LIMIT_SWITCH 1

// �������ƣ������@�N�����x����� @����
#define FRAME_LIMIT (24) ssss

#if FRAME_LIMIT_SWITCH == 1
#define FRAME_CACULATION \
		do{ \
			static NSTimeInterval frameBegin = 0.0f, frameEnd = 0.0f; \
			frameEnd = [NSDate timeIntervalSinceReferenceDate]; \
			if (frameBegin != 0.0f) \
			{ \
				NSTimeInterval limit = 1.0f / FRAME_LIMIT; \
				NSTimeInterval frame = frameEnd - frameBegin; \
				if (frame < limit) \
					usleep((limit-frame)*1000000);\
			} \
			frameBegin = [NSDate timeIntervalSinceReferenceDate]; \
		} while (0)
#else
#define FRAME_CACULATION
#endif

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

#define PERFORMANCE_DEBUG_SWITCH 0

#if PERFORMANCE_DEBUG_SWITCH == 1
#define PERFORMANCE_DEBUG_1												\
		static NDPerformance::key64 key;								\
		PerformanceTest.EndTestModule(key);								\
		PerformanceEnd;													\
		PerformanceStart;												\
		PerformanceTestBeginName("消息处理");							

#define PERFORMANCE_DEBUG_2												\
		PerformanceTestEndName("消息处理");								\
		PerformanceTestBeginName("游戏场景");								

#define PERFORMANCE_DEBUG_3												\
		PerformanceTestEndName("游戏场景");								\
		PerformanceTest.BeginTestModule("游戏帧调用间�??", key);
#else
#define PERFORMANCE_DEBUG_1	

#define PERFORMANCE_DEBUG_2	

#define PERFORMANCE_DEBUG_3
#endif

using namespace NDEngine;

void NDBaseDirector::mainLoop(void)
{
	if (true)
	{
		OnIdle();

		if (NDDebugOpt::getNetworkEnabled())
		{
			DispatchOneMessage();
		}

		if (NDDataTransThread::DefaultThread()->GetQuitGame())
		{
			LOGERROR("%s",CONVERT_GBK_TO_UTF8("�ѽ��c������ʧȥ�B�ӣ�"));
			if (NDDirector::DefaultDirector()->GetSceneByTag(SMGAMESCENE_TAG))
			{
				g_pUtil.QuitGameToServerList();
			}
		}

		//NDDirector::DefaultDirector()->DisibleScissor(); // ��ʱ��ע�͵�

		m_pScheduler->setTickEnabled( NDDebugOpt::getTickEnabled());

		CCDisplayLinkDirector::mainLoop();
	}
}

void NDBaseDirector::OnIdle()
{
	if (NDDebugOpt::getScriptEnabled())
	{
		BaseScriptMgrObj.update();
	}
}

void NDBaseDirector::DispatchOneMessage()
{
    static NDTransData kPackage;
    //NDTransData* data = NDMessageCenter::DefaultMessageCenter()->GetMessage();
    for (int n = 0; n < 10; n++) 
    {
        if (NDBaseNetMsgPoolObj.GetServerMsgPacket(kPackage))
        {
            NDBaseNetMsgPoolObj.Process(&kPackage);

            if ( (kPackage.GetCode() != _MSG_WALK)			&& 
                (kPackage.GetCode() != _MSG_PLAYER_EXT)		&&
                (kPackage.GetCode() != _MSG_TALK)	
                )
			{
				break;
			}
                
        }
        else 
        {
            break;
        }
    }
}
