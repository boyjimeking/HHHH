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
#ifndef __NDDirector_H
#define __NDDirector_H

#include "NDScene.h"
#include "NDObject.h"
#include <vector>
#include "CCDirector.h"
#include "NDNode.h"

namespace NDEngine
{
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
	void Initialization();
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
//		������Pause
//		���ã���ͣ���е�ǰ����
//		��������
//		����ֵ����
	void Pause();
//		
//		������Resume
//		���ã��ָ����е�ǰ����
//		��������
//		����ֵ����
	void Resume();

	void StopAnimation();
	void StartAnimation();
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
//		������SetDisplayFPS
//		���ã���ʾ��Ϸ����ʱ��֡��
//		������bDisplayed���true��ʾ��������ʾ
//		����ֵ����
	void SetDisplayFPS(bool bDisplayed);
//		
//		������isPaused
//		���ã��ж���Ϸ�Ƿ�����ͣ״̬
//		��������
//		����ֵ��true�� false��
	bool isPaused();
//		
//		������GetWinSize
//		���ã���ȡ��Ļ��С
//		��������
//		����ֵ����Ļ��С
	CGSize GetWinSize();
//
//		������SetViewRect
//		���ã����ýڵ�Ļ�������һ�������˽ڵ�Ļ����������ӽڵ�Ļ�������Ҳ���ᳬ��������Χ��
//			 ���ʹ�ø÷���ͨ��ʱ��draw��������ã����á�
//		������rect�������Ļ������node�����û�������Ľڵ�
//		����ֵ����
	void SetViewRect(CGRect rect, NDNode* node);
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
	void AddDelegate(NDObject* receiver);
//		
//		������RemoveDelegate
//		���ã�ע�������ί��
//		������receiverί���¼�������
//		����ֵ����	
	void RemoveDelegate(NDObject* receiver);

	void TransitionAnimateComplete();

	void EnableDispatchEvent(bool enable);

	NDScene* GetSceneByTag(int nSceneTag);

	float GetScaleFactor();

	void DisibleScissor();

private:

	typedef enum
	{
		eTransitionSceneNone,
		eTransitionSceneReplace,
		eTransitionScenePush,
		eTransitionScenePop
	} TransitionSceneType;

	void SetTransitionScene(NDScene *scene, TransitionSceneType type);

private:
	cocos2d::CCDirector* m_pkDirector;
	std::vector<NDScene*> m_kScenesStack;
	NDNode* m_pkSetViewRectNode;
	bool m_bResetViewRect;
	std::vector<NDObject*> m_delegates;

	void SetDelegate(NDObject* receiver)
	{
	}
	hide
	NDObject* GetDelegate()
	{
		return NULL;
	}
	hide

	void BeforeDirectorPopScene(NDScene* scene, bool cleanScene);
	void AfterDirectorPopScene(bool cleanScene);
	void BeforeDirectorPushScene(NDScene* scene);
	void AfterDirectorPushScene(NDScene* scene);

	NDScene* m_pkTransitionSceneWait;
	TransitionSceneType m_eTransitionSceneType;
};
}

#endif
