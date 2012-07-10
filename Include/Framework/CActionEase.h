/**
���߶�����
Copyright(c) 1999-2012, TQ Digital Entertainment, All Rights Reserved
**/
#ifndef __ACTION_CCEASE_ACTION_H__
#define __ACTION_CCEASE_ACTION_H__

#include "CActionInterval.h"

/** 
 @brief Base class for Easing actions
 ���ٶ����߶���

 ���߶�������һ��ʱ��ı�����⸴�϶������ڶ������������Ǿ�������ΪTweening����Easing action��

 ��Щ�������ڲ��޸Ķ������ٶȡ��������ǲ����޸�����ʱ�䡣���һ��������5�룬��ô�����ĳ���ʱ�����5�롣

 ���߶�����ʱ��Ƭ�ڸı�ֱ�ߡ�

 �������ǿ��Լ��ٻ��߼����ڲ�������

 ��Щ����������Ϊ3�����ͣ�

 In actions:�������ڶ����Ŀ�ʼ
 Out actions:�������ڶ����Ľ�β
 InOut actions:�������ڶ����Ŀ�ʼ�ͽ�β
(
	In    ��������
	Out   �ɿ�����
	InOut �����������ɿ�����
)
Speed �˹��趨�ٶ�,����ͨ�� SetSpeed ���ϵ�����

CActionEase:���ٶȱ仯�Ļ���
 */
class CActionEase : public CActionInterval
{
public:
	virtual ~CActionEase(void);

	/** initializes the action */
    bool InitWithAction(CActionInterval *pAction);

	virtual void StartWithTarget(IActionDelegate *pTarget);
	virtual void Stop(void);
	virtual void Update(fTime time);
	virtual CActionInterval* Reverse(void);

public:
	/** creates the action */
	static CActionEase* ActionWithAction(CActionInterval *pAction);

protected:
	CActionInterval *m_pOther;
};

/** 
 @brief Base class for Easing actions with rate parameters
 failed test
 ���ٶȱ仯����
 */
class CEaseRateAction : public CActionEase
{
public:
	virtual ~CEaseRateAction(void);

	/** set rate value for the actions */
	//���õ�ǰ���ٶ�
	void SetRate(float rate);

	/** get rate value for the actions */
	//��ȡ��ǰ���ٶ�
	float GetRate(void);

	/** Initializes the action with the inner action and the rate parameter */
	bool InitWithAction(CActionInterval *pAction, float fRate);

	virtual CActionInterval* Reverse(void);

public:
	/** Creates the action with the inner action and the rate parameter */
	static CEaseRateAction* ActionWithAction(CActionInterval* pAction, float fRate);

protected:
	float m_fRate;//��ǰ�ļ��ٶ�
};

/** 
��������
 @brief CEaseIn action with a rate
 */
class CEaseIn : public CEaseRateAction
{
public:
	virtual void Update(fTime time);

public:
	/** Creates the action with the inner action and the rate parameter */
	static CEaseIn* ActionWithAction(CActionInterval* pAction, float fRate);
};

/** 
 @brief CEaseOut action with a rate
�ɿ�����
 */
class CEaseOut : public CEaseRateAction
{
public:
	virtual void Update(fTime time);


public:
	/** Creates the action with the inner action and the rate parameter */
	//pAction����,fRate�仯�ı���
    static CEaseOut* ActionWithAction(CActionInterval* pAction, float fRate);
};

/** 
�����������ɿ�����
 @brief CEaseInOut action with a rate
 */
class CEaseInOut : public CEaseRateAction
{
public:
	virtual void Update(fTime time);

	virtual CActionInterval* Reverse(void);

public:
	/** Creates the action with the inner action and the rate parameter */
	static CEaseInOut* ActionWithAction(CActionInterval* pAction, float fRate);
};

/** 
 @brief CCEase Exponential In
 ָ�������߶���,��������
 */
class CEaseExponentialIn : public CActionEase
{
public:
	virtual void Update(fTime time);
	virtual CActionInterval* Reverse(void);


public:
	/** creates the action */
	static CEaseExponentialIn* ActionWithAction(CActionInterval* pAction);
};

/** 
 @brief Ease Exponential Out
 ָ�������߶���,�ɿ�����
 */
class CEaseExponentialOut : public CActionEase
{
public:
    virtual void Update(fTime time);
	virtual CActionInterval* Reverse(void);


public:
	/** creates the action */
	static CEaseExponentialOut* ActionWithAction(CActionInterval* pAction);

};

/** 
 @brief Ease Exponential InOut
 ָ�������߶���,�����������ɿ�����
 */
class CEaseExponentialInOut : public CActionEase
{
public:
	virtual void Update(fTime time);


public:
	/** creates the action */
	static CEaseExponentialInOut* ActionWithAction(CActionInterval* pAction);

};

/** 
 @brief Ease Sine In
 ��������,���������߶����ļ��ٶ�
 */
class CEaseSineIn : public CActionEase
{
public:
	virtual void Update(fTime time);
	virtual CActionInterval* Reverse(void);


public:
	/** creates the action */
	static CEaseSineIn* ActionWithAction(CActionInterval* pAction);
};

/** 
 @brief Ease Sine Out
 �ɿ�����,���������߶����ļ��ٶ�
 */
class CEaseSineOut : public CActionEase
{
public:
	virtual void Update(fTime time);
	virtual CActionInterval* Reverse(void);


public:
	/** creates the action */
	static CEaseSineOut* ActionWithAction(CActionInterval* pAction);
};

/** 
 @brief Ease Sine InOut
 �����������ɿ�����,���������߶����ļ��ٶ�
 */
class CEaseSineInOut : public CActionEase
{
public:
	virtual void Update(fTime time);


public:
	/** creates the action */
	static CEaseSineInOut* ActionWithAction(CActionInterval* pAction);
};

/** 
 @brief Ease Elastic abstract class
 
 �ĵ�ͼ�Ͷ�����

 ��Щ������ʱ��Ƭ��ģ���ĵ�ͼͼ�Σ��������ʹ��ʱ����0��1s�ڣ���������ڲ�����������������ֵ

 ��ЩֵҪ��ֹһ��ʹ�ã�����������Ƕ��󣩡������ڲ�����׼���޸����ֵ������CMoveBy,CScaleBy,CRotateBy�ȵȣ�����CCSquence��CSpawnҲ���ǲ������Ľ����
 */
class CEaseElastic : public CActionEase
{
public:
	/** get period of the wave in radians. default is 0.3 */
	inline float GetPeriod(void) { return m_fPeriod; }
	/** set period of the wave in radians. */
	inline void SetPeriod(float fPeriod) { m_fPeriod = fPeriod; }

	/** Initializes the action with the inner action and the period in radians (default is 0.3) */
	bool InitWithAction(CActionInterval *pAction, float fPeriod);
	/** initializes the action */
	bool InitWithAction(CActionInterval *pAction);

	virtual CActionInterval* Reverse(void);


public:
	/** creates the action */
	static CEaseElastic* ActionWithAction(CActionInterval *pAction);
	/** Creates the action with the inner action and the period in radians (default is 0.3) */
	static CEaseElastic* ActionWithAction(CActionInterval *pAction, float fPeriod);

protected:
	float m_fPeriod;//����
};

/** 
 @brief Ease Elastic In action.
 @warning This action doesn't use a bijective fucntion. Actions like Sequence might have an unexpected result when used with this action.
 
��������,�ĵ�ͼ�Ͷ���
 */
class CEaseElasticIn : public CEaseElastic
{
public:
	virtual void Update(fTime time);
	virtual CActionInterval* Reverse(void);


public:
	/** creates the action */
	static CEaseElasticIn* ActionWithAction(CActionInterval *pAction);
	/** Creates the action with the inner action and the period in radians (default is 0.3) */
	static CEaseElasticIn* ActionWithAction(CActionInterval *pAction, float fPeriod);
};

/** 
 @brief Ease Elastic Out action.
 @warning This action doesn't use a bijective fucntion. Actions like Sequence might have an unexpected result when used with this action.
 
 �ɿ�����,�ĵ�ͼ�Ͷ���
 */
class CEaseElasticOut : public CEaseElastic
{
public:
	virtual void Update(fTime time);
	virtual CActionInterval* Reverse(void);


public:
	/** creates the action */
	static CEaseElasticOut* ActionWithAction(CActionInterval *pAction);
	/** Creates the action with the inner action and the period in radians (default is 0.3) */
	static CEaseElasticOut* ActionWithAction(CActionInterval *pAction, float fPeriod);
};

/** 
 @brief Ease Elastic InOut action.
 @warning This action doesn't use a bijective fucntion. Actions like Sequence might have an unexpected result when used with this action.
 
 �ĵ�ͼ�Ͷ���
 */
class CEaseElasticInOut : public CEaseElastic
{
public:
	virtual void Update(fTime time);
	virtual CActionInterval* Reverse(void);


public:
	/** creates the action */
	static CEaseElasticInOut* ActionWithAction(CActionInterval *pAction);
	/** Creates the action with the inner action and the period in radians (default is 0.3) */
	static CEaseElasticInOut* ActionWithAction(CActionInterval *pAction, float fPeriod);
};

/** 
 @brief CEaseBounce abstract class.
 
 �����ͣ�
 ������ģ����һ������Ч����
 ��ЩҪ��ֹһ��ʹ�ã�����������Ƕ��󣩡������ڲ�����Ӧ��׼���޸����ֵ������CMoveBy,CScaleBy,CRotateBy�ȵȣ�����CCSquence��CSpawnҲ���ǲ������Ľ����
*/
class CEaseBounce : public CActionEase
{
public:
	fTime BounceTime(fTime time);


public:
	/** creates the action */
	static CEaseBounce* ActionWithAction(CActionInterval* pAction);
};

/** 
 @brief CEaseBounceIn action.
 @warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
 ��������Ĺ����Ͷ���
*/
class CEaseBounceIn : public CEaseBounce
{
public:
    virtual void Update(fTime time);
	virtual CActionInterval* Reverse(void);


public:
	/** creates the action */
	static CEaseBounceIn* ActionWithAction(CActionInterval* pAction);
};

/** 
 @brief EaseBounceOut action.
 @warning This action doesn't use a bijective fucntion. Actions like Sequence might have an unexpected result when used with this action.
 �ɿ������Ĺ����Ͷ���
 */
class CEaseBounceOut : public CEaseBounce
{
public:
	virtual void Update(fTime time);
	virtual CActionInterval* Reverse(void);


public:
	/** creates the action */
	static CEaseBounceOut* ActionWithAction(CActionInterval* pAction);
};

/** 
 @brief CEaseBounceInOut action.
 @warning This action doesn't use a bijective fucntion. Actions like Sequence might have an unexpected result when used with this action.
 �����������ɿ������Ĺ����Ͷ���
 */
class CEaseBounceInOut : public CEaseBounce
{
public:
	virtual void Update(fTime time);


public:
	/** creates the action */
	static CEaseBounceInOut* ActionWithAction(CActionInterval* pAction);
};

/** 
 @brief CEaseBackIn action.
 @warning This action doesn't use a bijective fucntion. Actions like Sequence might have an unexpected result when used with this action.
 ����������ٶȽ��䶯��
 */
class CEaseBackIn : public CActionEase
{
public:
	virtual void Update(fTime time);
	virtual CActionInterval* Reverse(void);


public:
	/** creates the action */
	static CEaseBackIn* ActionWithAction(CActionInterval* pAction);
};

/** 
 @brief CEaseBackOut action.
 @warning This action doesn't use a bijective fucntion. Actions like Sequence might have an unexpected result when used with this action.
 �ɿ��������ٶȽ��䶯��
 */
class CEaseBackOut : public CActionEase
{
public:
	virtual void Update(fTime time);
	virtual CActionInterval* Reverse(void);


public:
	/** creates the action */
	static CEaseBackOut* ActionWithAction(CActionInterval* pAction);
};

/** 
 @brief CEaseBackInOut action.
 @warning This action doesn't use a bijective fucntion. Actions like Sequence might have an unexpected result when used with this action.
 �����������ɿ��������ٶȽ��䶯��
*/
class CEaseBackInOut : public CActionEase
{
public:
	virtual void Update(fTime time);


public:
	/** creates the action */
	static CEaseBackInOut* ActionWithAction(CActionInterval* pAction);
};

#endif // __ACTION_CCEASE_ACTION_H__

