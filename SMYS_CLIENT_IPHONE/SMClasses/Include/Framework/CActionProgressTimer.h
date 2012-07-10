/*   ������������
 	Copyright(c) 1999-2012, TQ Digital Entertainment, All Rights Reserved
*/
#ifndef __ACTION_CCPROGRESS_TIMER_H__
#define __ACTION_CCPROGRESS_TIMER_H__

#include "CActionInterval.h"

/**
@brief Progress to percentage
���ȱ仯���Ľ��ȶ���,������/���ϵ�����ʾͼƬ����
@Warning:��ʱ���ܺͶ�������ͬʱ����
ִ�ж���ǰ,���ý��ȶ���������CSprite::SetProgressType(ProgressType type)
*/
class CProgressTo : public CActionInterval
{
public:
	/** Initializes with a duration and a percent */
	bool InitWithDuration(fTime duration, float fPercent);

//	virtual CCObject* copyWithZone(CCZone *pZone);
	virtual void StartWithTarget(IActionDelegate *pTarget);
	virtual void Update(fTime time);

public:
	/** Creates and initializes with a duration and a percent */
	static CProgressTo* ActionWithDuration(fTime duration, float fPercent=100.f/*������0-100*/);

protected:
	float m_fTo;
	float m_fFrom;
};

/**
@brief Progress from a percentage to another percentage
���ȱ仯��Χ�Ķ���
*/
class CProgressFromTo : public CActionInterval
{
public:
	/** Initializes the action with a duration, a "from" percentage and a "to" percentage */
    bool InitWithDuration(fTime duration, float fFromPercentage, float fToPercentage);

//	virtual CCObject* copyWithZone(CCZone *pZone);
	virtual CActionInterval* Reverse(void);
	virtual void StartWithTarget(IActionDelegate *pTarget);
	virtual void Update(fTime time);

public:
	/** Creates and initializes the action with a duration, a "from" percentage and a "to" percentage */
	//���ý��ȶ�����Χ
	static CProgressFromTo* ActionWithDuration(fTime duration/*����ʱ��*/, float fFromPercentage/*��ʼ�Ľ���0-100*/, float fToPercentage/*�����Ľ���0-100*/);

protected:
	float m_fTo;
	float m_fFrom;
};


#endif // __ACTION_CCPROGRESS_TIMER_H__
