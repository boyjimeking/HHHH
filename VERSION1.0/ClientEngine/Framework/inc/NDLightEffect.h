//
//  NDLightEffect.h
//  DragonDrive
//
//  Created by xiezhenghai on 11-4-21.
//  Copyright 2011 (����)DeNA. All rights reserved.
//

#ifndef __NDLightEffect_H
#define __NDLightEffect_H

#include "NDNode.h"
#include "NDLayer.h"

#include "NDAnimation.h"

namespace NDEngine
{
	class NDLightEffect : public NDNode
	{
		DECLARE_CLASS(NDLightEffect)
		NDLightEffect();
		~NDLightEffect();

	public:

		void Initialization(const char* sprFile);		
		void SetPosition(CGPoint kNewPosition);
		void SetRepeatTimes(unsigned int times);
		void SetLightId(unsigned int uiLightID, bool bReverse = true);
		void SlowDown(unsigned int mutli);
		
	public: //��״Ԫ��Чʹ��
		void Run(CGSize mapSize, bool draw = true);
		
	public:

		void draw();
		
	private:

		NDFrameRunRecord* m_pkFrameRunRecord;
		NDAnimationGroup* m_pkAniGroup;

		CGPoint m_kPosition;
		unsigned int m_nLightID;

		bool m_bIsStop;
		bool m_bReverse;
	};
}
#endif
