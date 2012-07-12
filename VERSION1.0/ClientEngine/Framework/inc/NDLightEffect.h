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
		void Initialization(const char* sprFile); hide		
		void SetPosition(CGPoint newPosition);
		void SetRepeatTimes(unsigned int times);
		void SetLightId(unsigned int lightId, bool reverse=true);
		void SlowDown(unsigned int mutli);
		
	public: //��״Ԫ��Чʹ��
		void Run(CGSize mapSize, bool draw=true);
		
	public:
		void draw(); override
		
	private:
		NDFrameRunRecord *m_frameRunRecord;
		NDAnimationGroup *m_aniGroup;
		CGPoint m_position;
		unsigned int m_lightId;
		bool m_stop;
		bool m_reverse;
		
	};
}
#endif
