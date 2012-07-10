/**
  ʱ����������
Copyright(c) 1999-2012, TQ Digital Entertainment, All Rights Reserved
**/
#ifndef __ACTION_CCINTERVAL_ACTION_H__
#define __ACTION_CCINTERVAL_ACTION_H__

#include "CAction.h"
#include "cTypes.h"
#include "CPointExtension.h"
#include "Sprite.h"

/** 
@brief An interval action is an action that takes place within a certain period of time.
It has an start time, and a finish time. The finish time is the parameter
duration plus the start time.

These CActionInterval actions have some interesting properties, like:
- They can run normally (default)
- They can run reversed with the reverse method
- They can run with the time altered with the Accelerate, AccelDeccel and Speed actions.

For example, you can simulate a Ping Pong effect running the action normally and
then running it again in Reverse mode.

Example:

CAction *pingPongAction = CSequence::Actions(action, action->Reverse(), NULL);
��ʱ����:�����������Ҫһ��ʱ��
***To: ��ζ���˶���ָ����λ�á�

***By:��ζ���˶�������ָ���m x��y ������λ�á�(x��y �����Ǹ�ֵ)

�ƶ��� �C CMoveTo

�ƶ��C CMoveBy

��Ծ�� �C CJumpTo

�����յ�λ�ú���Ծ�m�߶Ⱥʹ�����

��Ծ �C CJumpBy

�����յ�λ�ú���Ծ�m�߶Ⱥʹ�����

������ �C CBezierBy

֧�� 3 �α���������:P0-���,P1-������߷���,P2-�յ����߷���,P3-�յ㡣

�������ö� Bezier ����,Ȼ��ִ�С�

�Ŵ� �C CScaleTo

���÷Ŵ���,�Ǹ����͡�

�Ŵ� �C CScaleBy

��ת�� �C CRotateTo

��ת �C CRotateBy

��˸ �C CBlink
�趨��˸����

ɫ���仯�� �C CTintTo

ɫ���任 �C CTintBy

�䰵�� �C CFadeTo

���ޱ��� �C CFadeIn

�������� �C CFadeOut

*/
//��ʱ����
class  CActionInterval : public CFiniteTimeAction
{
public:
//	CActionInterval():m_bIsEnd(false){}
	/** how many seconds had elapsed since the actions started to run. */
	inline fTime GetElapsed(void) { return m_elapsed; }

	/** initializes the action */
	bool InitWithDuration(fTime d);

	/** returns true if the action has finished */
	//�����Ƿ����
	virtual bool IsDone(void);

// 	virtual CCObject* copyWithZone(CCZone* pZone);

	virtual void Step(fTime dt);
	virtual void StartWithTarget(IActionDelegate *pTarget);

	//������
	virtual CActionInterval* Reverse(void);

public:
	/** creates the action */
	static CActionInterval* ActionWithDuration(fTime d);

public:
    //extension in CCGridAction 
	void SetAmplitudeRate(float amp);
	float GetAmplitudeRate(void);

protected:
	fTime m_elapsed;
	bool   m_bFirstTick;
// private:
	//bool m_bIsEnd;
};

/** @brief Runs actions sequentially, one after another
���ж���
��Ҫ���þ��������������ɸ�����,Ȼ���Ⱥ�������ִ�С�
 */
class  CSequence : public CActionInterval
{
public:
	~CSequence(void);

	/** initializes the action */
    bool InitOneTwo(CFiniteTimeAction *pActionOne, CFiniteTimeAction *pActionTwo);


	virtual void StartWithTarget(IActionDelegate *pTarget);
	virtual void Stop(void);
	virtual void Update(fTime time);
	virtual CActionInterval* Reverse(void);//������

public:
	/** helper constructor to create an array of sequenceable actions */
	//������N������,�м�:���һ������������ΪNULL
	static CFiniteTimeAction* Actions(CFiniteTimeAction *pAction1, ...);
	/** helper contructor to create an array of sequenceable actions given an array */
//	static CFiniteTimeAction* actionsWithArray(CCArray *actions);

	/** creates the action */
	static CSequence* ActionOneTwo(CFiniteTimeAction *pActionOne, CFiniteTimeAction *pActionTwo);
protected:
	CFiniteTimeAction *m_pActions[2];
	fTime m_split;
	int m_last;
};

/** @brief Repeats an action a number of times.
 * To repeat an action forever use the CRepeatForever action.
 �ظ����޴��� �C Repeate
 */
class  CRepeat : public CActionInterval
{
public:
	~CRepeat(void);

	/** initializes a CRepeat action. Times is an unsigned integer between 1 and pow(2,30) 
	pAction:����ʱ��
	times:�ظ��Ĵ���
	*/
	bool InitWithAction(CFiniteTimeAction *pAction, unsigned int times);


	virtual void StartWithTarget(IActionDelegate *pTarget);
	virtual void Stop(void);
	virtual void Update(fTime time);
	virtual bool IsDone(void);

	//������
	virtual CActionInterval* Reverse(void);

	inline void SetInnerAction(CFiniteTimeAction *pAction)
	{
		if (m_pInnerAction != pAction)
		{
			CC_SAFE_RELEASE(m_pInnerAction);
			m_pInnerAction = pAction;
//			CC_SAFE_RETAIN(m_pInnerAction);
		}
	}

	inline CFiniteTimeAction* GetInnerAction()
	{
		return m_pInnerAction;
	}

public:
	/** creates a CRepeat action. Times is an unsigned integer between 1 and pow(2,30) */
	//pAction�ظ�ִ�еĶ���,timesִ���ظ�����
	static CRepeat* ActionWithAction(CFiniteTimeAction *pAction, unsigned int times);

protected:
	unsigned int m_uTimes;
	unsigned int m_uTotal;
	/** Inner action */
	CFiniteTimeAction *m_pInnerAction;
};

/** @brief Repeats an action for ever.
To repeat the an action for a limited number of times use the Repeat action.
@warning This action can't be Sequenceable because it is not an IntervalAction
�ظ�һֱѭ��
*/
class  CRepeatForever : public CActionInterval
{
public:
	CRepeatForever()
		: m_pInnerAction(NULL)
	{}
	virtual ~CRepeatForever();

	/** initializes the action */
	bool InitWithAction(CActionInterval *pAction);

	virtual void StartWithTarget(IActionDelegate* pTarget);
	virtual void Step(fTime dt);
	virtual bool IsDone(void);

	//������
	virtual CActionInterval* Reverse(void);

	inline void SetInnerAction(CActionInterval *pAction)
	{
		if (m_pInnerAction != pAction)
		{
			CC_SAFE_RELEASE(m_pInnerAction);
			m_pInnerAction = pAction;
		//	CC_SAFE_RETAIN(m_pInnerAction);
		}
	}

	inline CActionInterval* GetInnerAction()
	{
		return m_pInnerAction;
	}

public:
	/** creates the action */
	static CRepeatForever* ActionWithAction(CActionInterval *pAction);

protected:
	/** Inner action */
	CActionInterval *m_pInnerAction;
};

/** @brief Spawn a new action immediately
ͬ�� �C Spawn  
ͬʱ����ִ�����ɸ�����,��Ҫ�����������ǿ���ͬʱִ�еġ�����:�ƶ�ʽ��ת����ɫ�����С�ȡ�
ע��:ͬ��ִ���������ʱ���ɻ�����������ʱ����߾�����
 */
class  CSpawn : public CActionInterval
{
public:
	~CSpawn(void);

	/** initializes the Spawn action with the 2 actions to spawn */
	bool InitOneTwo(CFiniteTimeAction *pAction1, CFiniteTimeAction *pAction2);


	virtual void StartWithTarget(IActionDelegate *pTarget);
	virtual void Stop(void);
	virtual void Update(fTime time);
	virtual CActionInterval* Reverse(void);//������

public:
	/** helper constructor to create an array of spawned actions */
	//pAction1,...ͬ��ִ�е�N������,���һ����������ΪNULL
	static CFiniteTimeAction* Actions(CFiniteTimeAction *pAction1, ...);

	/** helper contructor to create an array of spawned actions given an array */
//	static CFiniteTimeAction* actionsWithArray(CCArray *actions);

	/** creates the Spawn action */
	static CSpawn* ActionOneTwo(CFiniteTimeAction *pAction1, CFiniteTimeAction *pAction2);

protected:
	CFiniteTimeAction *m_pOne;
	CFiniteTimeAction *m_pTwo;
};

/** @brief Rotates a IActionDelegate object to a certain angle by modifying it's
 rotation attribute.
 The direction will be decided by the shortest angle.
 ����ת�����ٶ�,��˳ʱ������ٶ�
*/ 
class  CRotateTo : public CActionInterval
{
public:
	/** initializes the action */
	bool InitWithDuration(fTime duration, float fDeltaAngle);


	virtual void StartWithTarget(IActionDelegate *pTarget);
	virtual void Update(fTime time);

public:
	/** creates the action */
	static CRotateTo* ActionWithDuration(fTime duration, float fDeltaAngle);

protected:
	float m_fDstAngle;
	float m_fStartAngle;
	float m_fDiffAngle;
};

/** @brief Rotates a IActionDelegate object clockwise a number of degrees by modifying it's rotation attribute.
����ת���ٶ�
*/
class  CRotateBy : public CActionInterval
{
public:
	/** initializes the action */
	//duration:����ʱ��,fDeltaAngle:ת�ĽǶ�,��90�Ⱦ���90
    bool InitWithDuration(fTime duration, float fDeltaAngle);


	virtual void StartWithTarget(IActionDelegate *pTarget);
	virtual void Update(fTime time);
	virtual CActionInterval* Reverse(void);//������

public:
	/** creates the action */
	static CRotateBy* ActionWithDuration(fTime duration, float fDeltaAngle);

protected:
	float m_fAngle;
	float m_fStartAngle;
};

/** @brief Moves a IActionDelegate object to the position x,y. x and y are absolute coordinates by modifying it's position attribute.
�ƶ���һ��λ��
*/
class  CMoveTo : public CActionInterval
{
public:
	/** initializes the action */
	bool InitWithDuration(fTime duration, const CPoint& position);


// 	virtual CAction* copyWithZone(IActionDelegate* pZone);
	virtual void StartWithTarget(IActionDelegate *pTarget);
	virtual void Update(fTime time);

public:
	/** creates the action */
	static CMoveTo* ActionWithDuration(fTime duration, const CPoint& position);
protected:
	CPoint m_endPosition;
	CPoint m_startPosition;
	CPoint m_delta;
};

/** @brief Moves a IActionDelegate object x,y pixels by modifying it's position attribute.
 x and y are relative to the position of the object.
 Duration is is seconds.
 �ƶ�����Ծ���
*/ 
class  CMoveBy : public CMoveTo
{
public:
	/** initializes the action */
	bool InitWithDuration(fTime duration, const CPoint& position);


	virtual void StartWithTarget(IActionDelegate *pTarget);
	virtual CActionInterval* Reverse(void);//������

public:
	/** creates the action */
	static CMoveBy* ActionWithDuration(fTime duration/*ִ��ʱ��(��)*/, const CPoint& position);
};

/** Skews a IActionDelegate object to given angles by modifying it's skewX and skewY attributes
Ť��:(δʵ��)
*/
#if 0
class  CSkewTo : public CActionInterval
{
public:
	CSkewTo();
	virtual bool InitWithDuration(fTime t, float sx, float sy);
	virtual void StartWithTarget(IActionDelegate *pTarget);
	virtual void Update(fTime time);

public:
	static CSkewTo* ActionWithDuration(fTime t, float sx, float sy);

protected:
	float m_fSkewX;
	float m_fSkewY;
	float m_fStartSkewX;
	float m_fStartSkewY;
	float m_fEndSkewX;
	float m_fEndSkewY;
	float m_fDeltaX;
	float m_fDeltaY;
};

/** Skews a IActionDelegate object by skewX and skewY degrees
Ť��:(δʵ��)
*/
class  CSkewBy : public CSkewTo
{
public:
	virtual bool InitWithDuration(fTime t, float sx, float sy);
	virtual void StartWithTarget(IActionDelegate *pTarget);
	virtual CActionInterval* Reverse(void);//������

public:
	static CSkewBy* ActionWithDuration(fTime t, float deltaSkewX, float deltaSkewY);
};
#endif

/** @brief Moves a IActionDelegate object simulating a parabolic jump movement by modifying it's position attribute.
��Ծ��Ծ���
*/
class  CJumpBy : public CActionInterval
{
public:
	/** initializes the action */
	bool InitWithDuration(fTime duration, const CPoint& position, fTime height, unsigned int jumps);

	virtual void StartWithTarget(IActionDelegate *pTarget);
	virtual void Update(fTime time);
	virtual CActionInterval* Reverse(void);//������

public:
	/** creates the action */
	//
	static CJumpBy* ActionWithDuration(fTime duration/*����ʱ��*/, const CPoint& position/*�յ�����*/, fTime height/*��Ծ�߶�*/, unsigned int jumps/*��Ծ����*/);

protected:
	CPoint			m_startPosition;
	CPoint			m_delta;
	fTime			m_height;
	unsigned int    m_nJumps;
};

/** @brief Moves a IActionDelegate object to a parabolic position simulating a jump movement by modifying it's position attribute.
��Ծ��
*/ 
class  CJumpTo : public CJumpBy
{
public:
    virtual void StartWithTarget(IActionDelegate *pTarget);

public:
	/** creates the action */
	static CJumpTo* ActionWithDuration(fTime duration/*����ʱ��*/, const CPoint& position/*�յ�����*/, fTime height/*��Ծ�߶�*/, int jumps/*��Ծ����*/);
};

/** @typedef bezier configuration structure
 */
typedef struct _ccBezierConfig {
	//! end position of the bezier
	CPoint endPosition;
	//! Bezier control point 1
	CPoint controlPoint_1;
	//! Bezier control point 2
	CPoint controlPoint_2;
} ccBezierConfig;

/** @brief An action that moves the target with a cubic Bezier curve by a certain distance.
�����������˶�
 */
class  CBezierBy : public CActionInterval
{
public:
	/** initializes the action with a duration and a bezier configuration */
	bool InitWithDuration(fTime t, const ccBezierConfig& c);

	virtual void StartWithTarget(IActionDelegate *pTarget);
	virtual void Update(fTime time);
	virtual CActionInterval* Reverse(void);//������

public:
	/** creates the action with a duration and a bezier configuration */
	static CBezierBy* ActionWithDuration(fTime t, const ccBezierConfig& c);

protected:
	ccBezierConfig m_sConfig;
	CPoint m_startPosition;
};

/** @brief An action that moves the target with a cubic Bezier curve to a destination point.
�����������˶���,Ҫ��������������,��
		ccBezierConfig bezier;
		bezier.controlPoint_1 = CPointMake(0, 200);
		bezier.controlPoint_2 = CPointMake(240, 500);
		bezier.endPosition = pos;
		actionTo = CBezierTo::ActionWithDuration(2.0f,bezier);
 */
class  CBezierTo : public CBezierBy
{
public:
    virtual void StartWithTarget(IActionDelegate *pTarget);

public:
	/** creates the action with a duration and a bezier configuration */
    static CBezierTo* ActionWithDuration(fTime t, const ccBezierConfig& c);
};

/** @brief Scales a IActionDelegate object to a zoom factor by modifying it's scale attribute.
 @warning This action doesn't support "reverse"
 ����Ŵ󵽵ı���(��Գ�ʼ�Ĵ�С:Ч���Ǵӵ�ǰ��С���Ҫ�Ŵ󵽵ı���(��Գ�ʼ�ı���))
 */
class  CScaleTo : public CActionInterval
{
public:
	/** initializes the action with the same scale factor for X and Y */
	bool InitWithDuration(fTime duration, float s);

	/** initializes the action with and X factor and a Y factor */
	bool InitWithDuration(fTime duration, float sx, float sy);

	virtual void StartWithTarget(IActionDelegate *pTarget);
	virtual void Update(fTime time);

public:
	/** creates the action with the same scale factor for X and Y 
		�Ŵ󵽵ı���
		duration����ʱ��
		fScaleFactor,�Ŵ󵽵ı���
	*/
	static CScaleTo* ActionWithDuration(fTime duration, float fScaleFactor);

	/** creates the action with and X factor and a Y factor
		�Ŵ󵽵ı���
		duration:����ʱ��
		fScaleFactorX:X����Ŵ󵽵ı���
		fScaleFactorY:Y����Ŵ󵽵ı���
	*/
	static CScaleTo* ActionWithDuration(fTime duration, float fScaleFactorX, float fScaleFactorY);
protected:
	float m_fScaleX;
	float m_fScaleY;
	float m_fStartScaleX;
  	float m_fStartScaleY;
    float m_fEndScaleX;
	float m_fEndScaleY;
	float m_fDeltaX;
	float m_fDeltaY;
};

/** @brief Scales a IActionDelegate object a zoom factor by modifying it's scale attribute.
����Ŵ�ı���(������ڵĴ�С,��2.0f,���ǷŴ󵽵�ǰ��С������)
*/
class  CScaleBy : public CScaleTo
{
public:
    virtual void StartWithTarget(IActionDelegate *pTarget);
	virtual CActionInterval* Reverse(void);//������

public:
	/*
	    creates the action with the same scale factor for X and Y 
		duration:����ʱ��,
		fScaleFactor:�Ŵ�ı���(��������)(��0.5������Сһ��)
	*/
	static CScaleBy* ActionWithDuration(fTime duration, float fScaleFactor);

	/*
		creates the action with and X factor and a Y factor 
		duration:����ʱ��,
		fScaleFactor:�Ŵ�ı���(��������)(fXScale:X����,fYScale:Y����)
	*/
	static CScaleBy* ActionWithDuration(fTime duration, float fXScale, float fYScale);
};

/** @brief Blinks a IActionDelegate object by modifying it's visible attribute
��˸
*/
class  CBlink : public CActionInterval
{
public:
	/**
	initializes the action
	*/
	//duration:��˸������ʱ��,uBlinks:��˸����
	bool InitWithDuration(fTime duration, unsigned int uBlinks);
	virtual void Update(fTime time);
	virtual CActionInterval* Reverse(void);//������

public:
	/** creates the action */
	static CBlink* ActionWithDuration(fTime duration, unsigned int uBlinks);
protected:
	unsigned int m_nTimes;
};


/** @brief Fades In an object that implements the CCRGBAProtocol protocol. It modifies the opacity from 0 to 255.
 The "reverse" of this action is FadeOut
 ͸���仯:���޵�����ʾ����
 */
class  CFadeIn : public CActionInterval
{
public:
    virtual void Update(fTime time);
	virtual CActionInterval* Reverse(void);//������

public:
	/** creates the action */
	static CFadeIn* ActionWithDuration(fTime d);
};

/** @brief Fades Out an object that implements the CCRGBAProtocol protocol. It modifies the opacity from 255 to 0.
 The "reverse" of this action is FadeIn
 ͸���仯:���е���,������û
*/
class  CFadeOut : public CActionInterval
{
public:
	virtual void Update(fTime time);
	virtual CActionInterval* Reverse(void);//������


public:
	/** creates the action */
	//dTime:ִ�ж���������ʱ��
	static CFadeOut* ActionWithDuration(fTime dTime);
};

/** @brief Fades an object that implements the CCRGBAProtocol protocol. It modifies the opacity from the current value to a custom one.
 @warning This action doesn't support "reverse"
 ͸���仯:��͸����,��ʾ��͸����ΪX(0-255)
 */
class  CFadeTo : public CActionInterval
{
public:
	/** initializes the action with duration and opacity */
	bool InitWithDuration(fTime duration, GLubyte opacity/*�仯����͸����ֵ,0-255,��90����RGB��ÿ����ɫ��Ϊԭֵ��90/255��*/);


	virtual void StartWithTarget(IActionDelegate *pTarget);
	virtual void Update(fTime time);

public:
	/** creates an action with duration and opacity */
	static CFadeTo* ActionWithDuration(fTime duration, GLubyte opacity);

protected:
	GLubyte m_toOpacity;
	GLubyte m_fromOpacity;
};

/** @brief Tints a IActionDelegate that implements the CCNodeRGB protocol from current tint to a custom one.
 @warning This action doesn't support "reverse"
 ��ɫ�仯:��ɫ�仯��X(RGB��ֵ)
*/
class  CTintTo : public CActionInterval
{
public:
	/** initializes the action with duration and color */
	bool InitWithDuration(fTime duration, GLubyte red, GLubyte green, GLubyte blue);


	virtual void StartWithTarget(IActionDelegate *pTarget);
	virtual void Update(fTime time);

public:
	/** creates an action with duration and color
	duration:��ʱ
	//Ҫ�仯��RGB��ɫֵ��С,��27, 55, -27
	*/
	static CTintTo* ActionWithDuration(fTime duration, GLubyte red, GLubyte green, GLubyte blue);

protected:
	Color3B m_to;
	Color3B m_from;
};

/** @brief Tints a IActionDelegate that implements the CCNodeRGB protocol from current tint to a custom one 
��ɫ�仯:��ɫ�仯�˶���(RGB��ֵ,����-�����ȥ����ֵ)
 */
class  CTintBy : public CActionInterval
{
public:
	/** initializes the action with duration and color */
	bool InitWithDuration(fTime duration, GLshort deltaRed, GLshort deltaGreen, GLshort deltaBlue);


	virtual void StartWithTarget(IActionDelegate *pTarget);
	virtual void Update(fTime time);

	//������
	virtual CActionInterval* Reverse(void);

public:
	/** creates an action with duration and color */
	static CTintBy* ActionWithDuration(fTime duration, GLshort deltaRed, GLshort deltaGreen, GLshort deltaBlue);//�仯����RGB��ɫֵ,��-127, -255, -127

protected:
	GLshort m_deltaR;
	GLshort m_deltaG;
	GLshort m_deltaB;

	GLshort m_fromR;
	GLshort m_fromG;
	GLshort m_fromB;
};

/** @brief Delays the action a certain amount of seconds
�ӳ�ʱ��Ķ���(ʲô����������,ͣסһ����ʱ��)
*/
class  CDelayTime : public CActionInterval
{
public:
//	CDelayTime(){m_bIsEnd=false;}
	virtual void Update(fTime time);
	
	//void end();
	//������
	virtual CActionInterval* Reverse(void);


public:
	/** creates the action 
	*/
	static CDelayTime* ActionWithDuration(fTime d/*Ҫ��ʱ��ʱ��*/);

};

/** @brief Executes an action in reverse order, from time=duration to time=0
 
 @warning Use this action carefully. This action is not
 sequenceable. Use it as the default "reversed" method
 of your own actions, but using it outside the "reversed"
 scope is not recommended.
 ʱ������(��ʱû���õ�)
*/
class  CReverseTime : public CActionInterval
{
public:
	~CReverseTime(void);
	CReverseTime();

	/** initializes the action */
    bool InitWithAction(CFiniteTimeAction *pAction);


	virtual void StartWithTarget(IActionDelegate *pTarget);
	virtual void Stop(void);
	virtual void Update(fTime time);
	virtual CActionInterval* Reverse(void);//������

public:
	/** creates the action */
	static CReverseTime* ActionWithAction(CFiniteTimeAction *pAction);

protected:
	CFiniteTimeAction *m_pOther;
};

#define CAnimation CSprite
//����
/** @brief Animates a sprite given the name of an Animation */
class  CAnimate : public CActionInterval
{
public:
	~CAnimate(void);
	/** Get animation used for the animate */
	inline CAnimation* GetAnimation(void) { return m_pAnimation; }
	/** Set animation used for the animate, the object is retained */
	inline void SetAnimation(CAnimation *pAnimation) 
	{
//		CC_SAFE_RETAIN(pAnimation);
		CC_SAFE_RELEASE(m_pAnimation);
		m_pAnimation = pAnimation;
	}

	/** initializes the action with an Animation and will restore the original frame when the animation is over */
    bool InitWithAnimation(CAnimation *pAnimation);

	/** initializes the action with an Animation */
	bool InitWithAnimation(CAnimation *pAnimation, bool bRestoreOriginalFrame);

	/** initializes an action with a duration, animation and depending of the restoreOriginalFrame, it will restore the original frame or not.
	 The 'delay' parameter of the animation will be overridden by the duration parameter.

	 */
	bool InitWithDuration(fTime duration, CAnimation *pAnimation, bool bRestoreOriginalFrame);


	virtual void StartWithTarget(IActionDelegate *pTarget);
	virtual void Stop(void);
	virtual void Update(fTime time);

	//������
	virtual CActionInterval* Reverse(void);

public:
	/** creates the action with an Animation and will restore the original frame when the animation is over */
	static CAnimate* ActionWithAnimation(CAnimation *pAnimation);

	/** creates the action with an Animation */
	static CAnimate* ActionWithAnimation(CAnimation *pAnimation, bool bRestoreOriginalFrame);

	/** creates an action with a duration, animation and depending of the restoreOriginalFrame, it will restore the original frame or not.
	 The 'delay' parameter of the animation will be overridden by the duration parameter.
	duration:ÿһ֡��������ʱ��,
	pAnimation:����CMySprite
	bRestoreOriginalFrame:����������,�Ƿ�ָ���Ĭ�ϵĵ�һ֡����
	 */	
     static CAnimate* ActionWithDuration(fTime duration, CAnimation *pAnimation, bool bRestoreOriginalFrame=false/*����������,�Ƿ�ָ���Ĭ�ϵĵ�һ֡����*/);
protected:
	CAnimation *m_pAnimation;
    bool m_bRestoreOriginalFrame;
};


#endif //__ACTION_CCINTERVAL_ACTION_H__
