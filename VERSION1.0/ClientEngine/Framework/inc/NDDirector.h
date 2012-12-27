//
//  NDDirector.h
//  DragonDrive
//
//  Created by xiezhenghai on 10-12-10.
//  Copyright 2010 (����)DeNA. All rights reserved.
//
//	�������ܣ���
//	���ݶ������������̵Ĵ�����������Ϸȫ���̵ĳ����л���
//	���ݶ�����ܳ�����Ҫ�󣬰���Ԥ����ƺõ���������ֹ��ѹջ�����ǰ������������һ��������
//	���ݶ����ڳ�ʼ����ʱ��Ĭ��Ϊ��Ϸ������һ�ַ�����
//	��Ϸ���������п�ͨ���������ṩ�Ľӿڸ������á�
//	����ͨ��ֻ��һ����
#ifndef __NDDIRECTOR_H
#define __NDDIRECTOR_H

#include "NDScene.h"
#include "NDObject.h"
#include <vector>
#include "CCDirector.h"
#include "CCEGLView.h"
#include "NDNode.h"
#include "CCTouchDispatcher.h"

//--------------------------------------------------------------------------------
//	***	��Դ���ű��� & �������ű���	***
//
//	��ע����Щ�����Ѿ����ǵ���ios & android�Ĳ���.
//	������
//		�ֱ���480*320		scale=1
//		�ֱ���960*640		scale=2
//		�ֱ���800*480		scale=1.5����YΪ����

//��Դ���ű���&�������ű�����һ����ͬ��
#define RESOURCE_SCALE		(NDDirector::DefaultDirector()->getResourceScale())
#define RESOURCE_SCALE_INT	int(RESOURCE_SCALE)
#define FONT_SCALE			RESOURCE_SCALE
#define FONT_SCALE_INT		int(FONT_SCALE)	
//--------------------------------------------------------------------------------

NS_NDENGINE_BGN

class NDDirector;
class NDDirectorDelegate
{
public:

//
//		ע�⣺ʹ������ί�з���ʱ����AddDelegate���ί�ж����RemoveDelegateɾ��ί�ж���
//
//		������BeforeDirectorPopScene
//		���ã�������������ջ֮ǰ������ڲ�����
//		������director���ݶ���ָ�룬scene�����������ĳ�����cleanScene�ó����Ƿ��ͷ�
//		����ֵ����
	virtual void BeforeDirectorPopScene(NDDirector* director, NDScene* scene,
			bool cleanScene)
	{
	}
//		
//		������AfterDirectorPopScene
//		���ã�������������ջ֮�󱻿���ڲ�����
//		������director���ݶ���ָ�룬cleanScene���ͷ���
//		����ֵ����
	virtual void AfterDirectorPopScene(NDDirector* director, bool cleanScene)
	{
	}
//		
//		������BeforeDirectorPushScene
//		���ã���������ѹ��ջ֮ǰ������ڲ�����
//		������director���ݶ���ָ�룬scene����ѹ��ĳ���
//		����ֵ����
	virtual void BeforeDirectorPushScene(NDDirector* director, NDScene* scene)
	{
	}
//		
//		������AfterDirectorPushScene
//		���ã���������ѹ��ջ֮�󱻿���ڲ�����
//		������director���ݶ���ָ�룬scene��ѹ��ĳ���
//		����ֵ����
	virtual void AfterDirectorPushScene(NDDirector* director, NDScene* scene)
	{
	}
};


///////////////////////////////////////////////////////////////////////////////////////////////////
class NDDirector: public NDObject
{
DECLARE_CLASS(NDDirector)
public:
	NDDirector();
	~NDDirector();

public:
//		
//		������DefaultDirector
//		���ã�������̬��������Ա�����ķ����붼ͨ���ýӿ��ȡ�
//		��������
//		����ֵ��������ָ��
	static NDDirector* DefaultDirector();
//		
//		������Initialization
//		���ã����ݶ�����ʹ��֮����Ҫ��ʼ��һ�����������Ϸ���ã��÷���������������ֻ������һ��
//		��������
//		����ֵ����
	void Initialization() {}
//		
//		������RunScene
//		���ã����г������ó�������Ϸ�ĵ�һ��������������Ϸ�����л���ʱ��Ҫʹ�ø÷���
//		������scene����
//		����ֵ����
	void RunScene(NDScene* scene);
//		
//		������ReplaceScene
//		���ã��滻���������ڳ��������ʹ�ø÷�������ȷ����Ϸ���г����������У���һ���������ᱻ�滻��ֻ�ᱻѹ��ջ��
//		������scene
//		����ֵ����
	void ReplaceScene(NDScene* pkScene, bool bAnimate = false);
//		
//		������PushScene
//		���ã�ѹ�볡�������ڳ��������
//		������scene����
//		����ֵ����
	void PushScene(NDScene* scene, bool bAnimate = false);
//	
//		������PopScene
//		���ã����������������������ڴ棬���ڳ����������Ϸ�����ٴ�����������������²Ż���Ч
//		��������
//		����ֵ��true�ɹ�, falseʧ��
	bool PopScene(NDScene* scene = NULL, bool bAnimate = false);

//		
//		������PopScene(bool cleanUp)
//		���ã��������������ڳ����������Ϸ�����ٴ�����������������²Ż���Ч
//		������cleanUp���Ϊtrue�������ڴ棬���������ڴ�
//		����ֵ��true�ɹ�, falseʧ��
	bool PopScene(bool cleanUp);
//		
//		������GetRunningScene
//		���ã���ȡ��ǰ�������еĳ���
//		��������
//		����ֵ��ʧ�ܷ���null
	NDScene* GetRunningScene();
//		
//		������GetRunningScene
//		���ã���ȡ�ڴ��е�ָ������
//		������runtimeClass ָ�����͵ĳ���
//		����ֵ��ʧ�ܷ���null		
	NDScene* GetScene(const NDRuntimeClass* runtimeClass);

//		
//		������Stop
//		���ã�ֹͣ��Ϸ
//		��������
//		����ֵ����
	void Stop();
//		
//		������PurgeCachedData
//		���ã�������Ϸ�����еĻ������ݣ�һ������Ϸ����ʱ����
//		��������
//		����ֵ����
	void PurgeCachedData();


//
//		������SetViewRect
//		���ã����ýڵ�Ļ�������һ�������˽ڵ�Ļ����������ӽڵ�Ļ�������Ҳ���ᳬ��������Χ��
//			 ���ʹ�ø÷���ͨ��ʱ��draw��������ã����á�
//		������rect�������Ļ������node�����û�������Ľڵ�
//		����ֵ����
	void SetViewRect(CCRect kRect, NDNode* pkNode);
//		
//		������ResumeViewRect
//		���ã��ָ���ʼ��������
//		������drawingNode��ǰ֡���ڷ��ʵĽڵ㣬���drawingNode�Ǳ����û�������Ľڵ�������ӽڵ㣬��ָ���Ч
//		����ֵ����	
	void ResumeViewRect(NDNode* drawingNode);
//		
//		������AddDelegate
//		���ã���Ӷ���ί�У�ע�⣺����ע����ί�У��ͷ�ʱ��ע��RemoveDelegate()
//		������receiverί���¼�������
//		����ֵ����	
	void AddDelegate(NDObject* receiver) { m_delegates.push_back(receiver); }
//		
//		������RemoveDelegate
//		���ã�ע�������ί��
//		������receiverί���¼�������
//		����ֵ����	
	void RemoveDelegate(NDObject* receiver);

	void TransitionAnimateComplete();

	void EnableDispatchEvent(bool enable) {
		CCDirector::sharedDirector()->getTouchDispatcher()->setDispatchEvents( enable );
	}

	NDScene* GetSceneByTag(int nSceneTag);
	
	bool IsEnableRetinaDisplay() { return CCEGLView::sharedOpenGLView()->isRetinaEnabled(); }

	void DisibleScissor();

	void SetDisplayFPS(bool bDisplayed) { m_pkDirector->setDisplayStats(bDisplayed); }

	//UIWindow * GetWindow() { return window; } ���UIWindow��ô����? ����
public:
	// only for LUA, don't try to call this.
	CCSize getWinSizeInPixels_Lua() { 
		return m_pkDirector->getWinSizeInPixels(); 
	}

#if 1 //������ű���������Ļ������벻Ҫ�޸ģ�
public: //@android
	CCPoint getAndroidScale() const;
	float getResourceScale();

	//ֻ��������������
	float getCoordScaleX();
	float getCoordScaleY();

#endif
	
private:
	typedef enum
	{
		eTransitionSceneNone,
		eTransitionSceneReplace,
		eTransitionScenePush,
		eTransitionScenePop
	} TransitionSceneType;

	void SetTransitionScene(NDScene *scene, TransitionSceneType type);

	void SetDelegate(NDObject* pkReceiver) {}
	NDObject* GetDelegate() { return NULL; }

	void BeforeDirectorPopScene(NDScene* scene, bool cleanScene);
	void AfterDirectorPopScene(bool cleanScene);
	void BeforeDirectorPushScene(NDScene* scene);
	void AfterDirectorPushScene(NDScene* scene);

private:
	cocos2d::CCDirector* m_pkDirector;
	std::vector<NDScene*> m_kScenesStack;
	NDNode* m_pkSetViewRectNode;
	bool m_bResetViewRect;
	std::vector<NDObject*> m_delegates;

	NDScene* m_pkTransitionSceneWait;
	TransitionSceneType m_eTransitionSceneType;
};

NS_NDENGINE_END

#endif //__NDDIRECTOR_H