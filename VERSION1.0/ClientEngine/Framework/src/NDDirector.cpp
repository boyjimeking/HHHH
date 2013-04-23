//
//  NDDirector.mm
//  DragonDrive
//
//  Created by xiezhenghai on 10-12-10.
//  Copyright 2010 (����)DeNA. All rights reserved.
//

#include "NDDirector.h"
#include "ccMacros.h"
#include "CCEGLView.h"
#include "CCTexture2D.h"
#include "CCTextureCache.h"
#include "CCTouchDispatcher.h"
#include "NDPicture.h"
#include "NDAnimationGroupPool.h"
#include "define.h"
#include "CCTransition.h"
#include "CCPointExtension.h"
#include "ObjectTracker.h"
#include "NDPicture.h"
#include "NDMapData.h"
#include "NDAnimationGroupPool.h"

using namespace cocos2d;

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

NS_NDENGINE_BGN

IMPLEMENT_CLASS(NDDirector, NDObject)

static NDDirector* gs_pkNDDirectorDefaultDirector = NULL;

NDDirector::NDDirector()
{
	INC_NDOBJ_RTCLS;

	NDAsssert(gs_pkNDDirectorDefaultDirector == NULL);

	m_pkDirector = CCDirector::sharedDirector();
	m_pkSetViewRectNode = NULL;
	m_bResetViewRect = false;
	m_pkTransitionSceneWait = NULL;
	m_eTransitionSceneType = eTransitionSceneNone;
}

NDDirector::~NDDirector()
{
	DEC_NDOBJ_RTCLS;

	SAFE_DELETE (m_pkTransitionSceneWait);

	SAFE_RELEASE( m_pkDirector );

	gs_pkNDDirectorDefaultDirector = NULL;
}

NDDirector* NDDirector::DefaultDirector()
{
	if (gs_pkNDDirectorDefaultDirector == NULL)
	{
		gs_pkNDDirectorDefaultDirector = new NDDirector();
	}
	return gs_pkNDDirectorDefaultDirector;
}

void NDDirector::RemoveDelegate(NDObject* receiver)
{
	std::vector<NDObject*>::iterator iter;

	for (iter = m_delegates.begin(); iter != m_delegates.end(); iter++)
	{
		NDObject* obj = (NDObject*) *iter;

		if (obj == receiver)
		{
			m_delegates.erase(iter);
			break;
		}
	}
}

void NDDirector::TransitionAnimateComplete()
{
	if (!m_pkTransitionSceneWait)
	{
		return;
	}

	switch (m_eTransitionSceneType)
	{
	case eTransitionSceneReplace:
		ReplaceScene (m_pkTransitionSceneWait);
		break;
	case eTransitionScenePush:
		PushScene(m_pkTransitionSceneWait);
		break;
	case eTransitionScenePop:
		PopScene(NULL, false);
	default:
		break;
	}

	m_pkTransitionSceneWait = NULL;
}

void NDDirector::SetTransitionScene(NDScene *scene, TransitionSceneType type)
{
	m_pkTransitionSceneWait = scene;

	m_eTransitionSceneType = type;

	m_pkDirector->pushScene(CCTransitionFade::transitionWithDuration(1.2f,
		(CCScene *)scene->getCCNode()));
}


void NDDirector::RunScene(NDScene* scene)
{
	LOGD("Entry run scene,scene value is %d",(int)scene);

	m_kScenesStack.push_back(scene);

	LOGD("ready to runwithscene,scene->m_ccNode value is %d",(int)(scene->m_pkCCNode));

	m_pkDirector->runWithScene((CCScene *) scene->getCCNode());
}

void NDDirector::ReplaceScene(NDScene* pkScene, bool bAnimate/*=false*/)
{
// 	if (bAnimate) 
// 	{
// 		SetTransitionScene(pkScene, eTransitionSceneReplace);
// 
// 		return;
// 	}

	if (m_kScenesStack.size() > 0)
	{
		//NDLog("===============================��ǰ����ջ��С[%u]", m_scenesStack.size());
		BeforeDirectorPopScene(m_kScenesStack.back(), true);

		NDScene* pkScene = m_kScenesStack.back();

		if (pkScene)
		{
			delete pkScene;
		}

		m_kScenesStack.pop_back();

		AfterDirectorPopScene(true);
	}
	
	BeforeDirectorPushScene(pkScene);
	m_kScenesStack.push_back(pkScene);
	m_pkDirector->replaceScene((CCScene *) pkScene->getCCNode());
	//RunScene(pkScene);
	AfterDirectorPushScene(pkScene);

	//NDLog("===============================��ǰ����ջ��С[%u]", m_scenesStack.size());
}

void NDDirector::PushScene(NDScene* scene, bool bAnimate/*=false*/)
{
	BeforeDirectorPushScene(scene);

	m_kScenesStack.push_back(scene);
	m_pkDirector->pushScene((CCScene *) scene->getCCNode());

	AfterDirectorPushScene(scene);

	//NDLog("===============================��ǰ����ջ��С[%u]", m_scenesStack.size());
}

bool NDDirector::PopScene(NDScene* scene/*=NULL*/, bool bAnimate/*=false*/)
{
	return PopScene(true);
}

bool NDDirector::PopScene(bool cleanUp)
{
	if (m_kScenesStack.size() < 2)
	{
		return false;
	}

	BeforeDirectorPopScene(GetRunningScene(), cleanUp);

	//NDLog("===============================��ǰ����ջ��С[%u]", m_scenesStack.size());

	if (cleanUp)
	{
		delete m_kScenesStack.back();
	}
	m_kScenesStack.pop_back();
	m_pkDirector->popScene();

	AfterDirectorPopScene(cleanUp);

	//NDLog("===============================��ǰ����ջ��С[%u]", m_scenesStack.size());

	return true;
}

void NDDirector::PurgeCachedData()
{
	CCTextureCache::sharedTextureCache()->removeAllTextures();

	NDPicturePool::DefaultPool()->PurgeDefaultPool();

	NDAnimationGroupPool::purgeDefaultPool();
}

void NDDirector::Stop()
{
	m_pkDirector->end();

	while (m_kScenesStack.begin() != m_kScenesStack.end())
	{
		delete m_kScenesStack.back();
		m_kScenesStack.pop_back();
	}
}

NDScene* NDDirector::GetScene(const NDRuntimeClass* runtimeClass)
{
	for (size_t i = 0; i < m_kScenesStack.size(); i++)
	{
		NDScene* scene = m_kScenesStack.at(i);

		if (scene->IsKindOfClass(runtimeClass))
		{
			return scene;
		}
	}
	return NULL;
}

NDScene* NDDirector::GetRunningScene()
{
	if (m_kScenesStack.size() > 0)
	{
		return m_kScenesStack.back();
	}

	return NULL;
}

void NDDirector::SetViewRect(CCRect kRect, NDNode* pkNode)
{
	if (m_pkTransitionSceneWait)
	{
		return;
	}

	CCSize kWinSize = m_pkDirector->getWinSizeInPixels();

#if 1 //@check
	glEnable (GL_SCISSOR_TEST);	
	glScissor(	kRect.origin.x,
				kWinSize.height - (kRect.origin.y + kRect.size.height),
	 			kRect.size.width, 
				kRect.size.height );

#endif

	/***
	*	�������в�ͬ�����ǿ��Խ��ܣ���ΪMac���ǵ���UIDevice.h�еģ����ﲻ��Ҫ��
	*
	*	@author ����
	*/

	m_pkSetViewRectNode = pkNode;
	m_bResetViewRect = true;
}

void NDDirector::ResumeViewRect(NDNode* drawingNode)
{
	if (!m_bResetViewRect)
	{
		return;
	}

	if (m_pkSetViewRectNode)
	{
		if (drawingNode == m_pkSetViewRectNode)
		{
			return;
		}

		if (drawingNode->IsChildOf(m_pkSetViewRectNode))
		{
			return;
		}
	}

	DisibleScissor();
}

void NDDirector::DisibleScissor()
{
	glDisable (GL_SCISSOR_TEST);
	m_pkSetViewRectNode = NULL;
	m_bResetViewRect = false;
}

void NDDirector::BeforeDirectorPopScene(NDScene* scene, bool cleanScene)
{
	std::vector<NDObject*>::iterator iter;

	for (iter = m_delegates.begin(); iter != m_delegates.end(); iter++)
	{
		NDObject* obj = (NDObject*) *iter;
		NDDirectorDelegate* delegate = dynamic_cast<NDDirectorDelegate*>(obj);

		if (delegate)
		{
			delegate->BeforeDirectorPopScene(this, scene, cleanScene);
		}
	}
}

void NDDirector::AfterDirectorPopScene(bool cleanScene)
{
	std::vector<NDObject*>::iterator iter;
	for (iter = m_delegates.begin(); iter != m_delegates.end(); iter++)
	{
		NDObject* obj = (NDObject*) *iter;
		NDDirectorDelegate* delegate = dynamic_cast<NDDirectorDelegate*>(obj);

		if (delegate)
		{
			delegate->AfterDirectorPopScene(this, cleanScene);
		}
	}
}

void NDDirector::BeforeDirectorPushScene(NDScene* scene)
{
	std::vector<NDObject*>::iterator iter;

	for (iter = m_delegates.begin(); iter != m_delegates.end(); iter++)
	{
		NDObject* obj = (NDObject*) *iter;
		NDDirectorDelegate* delegate = dynamic_cast<NDDirectorDelegate*>(obj);

		if (delegate)
		{
			delegate->BeforeDirectorPushScene(this, scene);
		}
	}
}

void NDDirector::AfterDirectorPushScene(NDScene* scene)
{
	std::vector<NDObject*>::iterator iter;

	for (iter = m_delegates.begin(); iter != m_delegates.end(); iter++)
	{
		NDObject* obj = (NDObject*) *iter;
		NDDirectorDelegate* delegate = dynamic_cast<NDDirectorDelegate*>(obj);

		if (delegate)
		{
			delegate->AfterDirectorPushScene(this, scene);
		}
	}
}

NDScene* NDDirector::GetSceneByTag(int nSceneTag)
{
	for (size_t i = 0; i < m_kScenesStack.size(); i++)
	{
		NDScene* scene = m_kScenesStack.at(i);
		if (scene->GetTag() == nSceneTag)
		{
			return scene;
		}
	}
	return NULL;
}

//@gc
void NDDirector::Recyle()
{
	NDPicturePool::DefaultPool()->Recyle();
	NDMapTexturePool::defaultPool()->Recyle();
	NDAnimationGroupPool::defaultPool()->Recyle();
}

//@ios
//��������ο�960*640
//��ע������������������з�androidƽ̨
float NDDirector::getIosScale() const
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	return 0.5f * CCDirector::sharedDirector()->getContentScaleFactor();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	return 0.5f * CCDirector::sharedDirector()->getContentScaleFactor();
#else
	return 1.0f;
#endif
}

//@android
//����ֵ��	xֵ���x��������ű���
//			yֵ���y�ŵ�����ű���
//�����������android�ֻ���Ч
//�ο�960*640
CCPoint NDDirector::getAndroidScale() const
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	CCSize sz = CCDirector::sharedDirector()->getVisibleSize();
	return ccp( sz.width/960, sz.height/640 );
#else
	return ccp( 1,1 );
#endif
}

//������Դ���ű���
//�ο�480*320
float NDDirector::getResourceScale()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	return 2.0f * getAndroidScale().y; //��2����Ϊ�ǲο�960*640�ģ���Ҫ�ĳ�1
#else
	return m_pkDirector->getContentScaleFactor();
#endif
}

//����x�����������ű���
//�ο�960*640
float NDDirector::getCoordScaleX()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	return getAndroidScale().x;
#else
	return 0.5f * m_pkDirector->getContentScaleFactor();
#endif
}

//����y�����������ű���
//�ο�960*640
float NDDirector::getCoordScaleY()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	return getAndroidScale().y;
#else
	return 0.5f * m_pkDirector->getContentScaleFactor();
#endif
}

NS_NDENGINE_END
