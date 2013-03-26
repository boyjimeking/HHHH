//
//  NDClassFactory.h
//  NDClassFactory
//
//  Copyright 2010 (����)DeNA. All rights reserved.
//	
//	@author ����
//
//	�������ܣ���
//	������Ч������Ч��

#ifndef CCSHAKE_H
#define CCSHAKE_H

#include "CCActionInterval.h"

NS_CC_BEGIN

class CC_DLL CCShake : public CCActionInterval
{
public:
	CCShake();
	virtual ~CCShake();
	static CCShake* create(float d, float strength );
	static CCShake* createWithStrength(float d, float strength_x, float strength_y );
	bool initWithDuration(float d, float strength_x, float strength_y );

protected:

	void startWithTarget(CCNode *pTarget);
	void update(float time);
	void stop(void);

	float m_fInitialX;
	float m_fInitialY;
	float m_fStrengthX;
	float m_StrengthY;
};
NS_CC_END

#endif