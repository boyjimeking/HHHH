/*
����������
Copyright(c) 1999-2012, TQ Digital Entertainment, All Rights Reserved
*/
#ifndef __ACTIONS_CCACTION_H__
#define __ACTIONS_CCACTION_H__
#include <stdio.h> 
#include "IActionDelegate.h"


#ifdef _WIN32
#include <Windows.h>
#include <float.h>
typedef __int64 int64_t;
#else// unix

#include <inttypes.h>
#include <float.h>

typedef long LONG;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef unsigned char BYTE;
#endif

#ifndef NULL
#define NULL 0
#endif//NULL
//
//enum {
//	//! Default tag
//	ActionTagInvalid = -1,
//};

/** 
@brief Base class for CAction objects.
�������ϵͼ:
CAction(��������)
	CSpeed(�����ٶȵĶ���)
	CFiniteTimeAction(����ʱ��)
		CActionInterval(��ʱ����)
			CAnimate(����)
			CPlace(����)
			CSequence(���ж���:˳��ִ�ж������)
			CSpawn(ͬ��:ͬʱִ�ж������)
			CRepeat(�ظ����޴���)
			CRepeatForever(�ظ�һֱѭ��)
			CReverseTime
			CDelayTime(��ʱ)
			CFadeIn(���޵���),CFadeOut(���е���),CFadeTo(͸��ֵ�䵽) [͸���仯]
			CProgressFromTo(���ȱ仯��Χ),CProgressTo(���ȱ仯��) [���ȱ仯����]
			CTintBy(�仯��),CTintTo(�仯��) [��ɫ�仯]
			CRotateBy(����ת��),CRotateTo(��ת��) [����ת�Ƕȶ���<��ʱֻ��Χ�ƾ�������ĵ�����ת>]
			CActionEase(���ٶ����߶���)
				CEaseRateAction(���ٶȱ仯����)
					CEaseIn(��������),CEaseOut(�ɿ�����)
				CEaseExponentialIn(��������),CEaseExponentialOut(�ɿ�����),CEaseExponentialInOut(�����������ɿ�����)[ָ�������߶���]
				CEaseSineIn(��������),CEaseSineOut(�ɿ�����),CEaseSineInOut(�����������ɿ�����) [���������߶����ļ��ٶ�]
				CEaseElastic(�ĵ�ͼ�Ͷ���)
					CCEaseElasticIn(��������),CEaseElasticOut(�ɿ�����) [�ĵ�ͼ�Ͷ���]
				CEaseBounce(�����Ͷ���)
					CEaseBounceIn(��������),CEaseBounceOut(�ɿ�����),CEaseBounceInOut(�����������ɿ�����) [�����Ͷ���]
				CEaseBackIn(��������),CEaseBackOut(�ɿ�����),CEaseBackInOut(�����������ɿ�����) [�ٶȽ��䶯��]
		CActionInstant(˲ʱ����)
			CShow(��ʾ),CHide(����)
			CToggleVisibility(�ɼ��л�)
			CFlipX(ˮƽ��ת),CFlipY(��ֱ��ת)
			CCallFunc(��������)
				CCallFuncN(ִ�д�һ��IActionDelegate*�����ĺ���)
					CCallFuncND(ִ�д���������(IActionDelegate*,void*)�ĺ���)
*/
//���������
class CAction  :public CObject
{
public:
    CAction(void);
	virtual ~CAction(void);

	//�ͷ�(���ٸö��������ü���)
	//virtual void Release(void);

	////����(���Ӹö��������ü���)
	//virtual void Retain(void);
public:

	//! return true if the action has finished
	//�����Ƿ����
	virtual bool IsDone(void);

	//! called before the action start. It will also set the target.
	//��һ������
	virtual void StartWithTarget(IActionDelegate *pTarget);//

	/** 
	called after the action has finished. It will set the 'target' to nil.
    IMPORTANT: You should never call "[action stop]" manually. Instead, use: "target->stopAction(action);"
	*/
	//ֹͣ����
    virtual void Stop(void);

	//! called every frame with it's delta time. DON'T override unless you know what you are doing.
	//����ִ��һ��ʱ��Ƭ
	virtual void Step(fTime dt);

	/** 
	called once per frame. time a value between 0 and 1

	For example: 
	- 0 means that the action just started
	- 0.5 means that the action is in the middle
	- 1 means that the action is over
	*/
	//���¶���
	virtual void Update(fTime time);
	
	//��ȡִ�ж����ľ���
	inline IActionDelegate* GetTarget(void);
	
	/** The action will modify the target properties. */
	//����Ҫ���Ƶľ������
	inline void SetTarget(IActionDelegate *pTarget);
	
	//��ȡ��ʼ�ľ������
	inline IActionDelegate* GetOriginalTarget(void); 
	
	/** Set the original target, since target can be nil.
	Is the target that were used to run the action. Unless you are doing something complex, like CActionManager, you should NOT call this method.
	The target is 'assigned', it is not 'retained'.
	
	*/
	//����Ҫ��ʼ�ľ������
	inline void SetOriginalTarget(IActionDelegate *pOriginalTarget) { m_pOriginalTarget = pOriginalTarget; }

	//��ȡ��ǩ��
	int GetTag(void);

	//���ñ�ǩ��
	void SetTag(int nTag);

public:
	/** Allocates and initializes the action */
	static CAction* Action();

protected:
	IActionDelegate	*m_pOriginalTarget;//��ʼ�ľ���(������ִ�ж����ľ�����ÿ�)
	/** The "target".
	The target will be set with the 'StartWithTarget' method.
	When the 'stop' method is called, target will be set to nil.
	The target is 'assigned', it is not 'retained'.
	*/
	IActionDelegate	*m_pTarget;//ִ�ж����ľ���
	/** The action tag. An identifier of the action */
	int 	m_nTag;//��ǩ��(Ĭ��-1)
	unsigned int m_uReference;//�ö��������ü���(�����Զ�ɾ��)
};

/** 
@brief 
 Base class actions that do have a finite time duration.
 Possible actions:
   - An action with a duration of 0 seconds
   - An action with a duration of 35.5 seconds

 Infinite time actions are valid
 ����ʱ��Ķ���
 */
class  CFiniteTimeAction : public CAction
{
public:
	CFiniteTimeAction()
		: m_fDuration(0)
	{}
	virtual ~CFiniteTimeAction(){}

    //! get duration in seconds of the action
	//��ȡ�ö����ĳ���ʱ��
	fTime GetDuration(void);

	//! set duration in seconds of the action
	//���øö����ĳ���ʱ��
	void SetDuration(fTime duration);

	/** returns a reversed action */
	virtual CFiniteTimeAction* Reverse(void);//�����ķ���
protected:
	//! duration in seconds
	fTime m_fDuration;//�ѻ�ʱ��
};

class CActionInterval;

/** 
 @brief Changes the speed of an action, making it take longer (speed>1)
 or less (speed<1) time.
 Useful to simulate 'slow motion' or 'fast forward' effect.
 @warning This action can't be Sequenceable because it is not an CCIntervalAction
 �����ٶȵĶ���(�޸����ڲ��ĳ���ʱ��)
 */
class  CSpeed : public CAction
{
public:
	CSpeed()
		: m_fSpeed(0.0)
		, m_pInnerAction(NULL)
	{}
	virtual ~CSpeed(void);

	float GetSpeed(void);

	/** alter the speed of the inner function in runtime */
	void SetSpeed(float fSpeed);

	/** initializes the action */
	bool InitWithAction(CActionInterval *pAction, float fRate);

// 	virtual CObject* copyWithZone(CCZone *pZone);
	virtual void StartWithTarget(IActionDelegate* pTarget);
	virtual void Stop();
	virtual void Step(fTime dt);
	virtual bool IsDone(void);
	virtual CActionInterval* Reverse(void);

	void SetInnerAction(CActionInterval *pAction);

	CActionInterval* GetInnerAction();

public:
	/** creates the action */
	static CSpeed* ActionWithAction(CActionInterval *pAction, float fRate);
    
protected:
	float m_fSpeed;
	CActionInterval *m_pInnerAction;
};



#endif // __ACTIONS_CCACTION_H__
