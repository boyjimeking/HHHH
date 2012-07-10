//
//  NDAnimation.mm
//  DragonDrive
//
//  Created by xiezhenghai on 10-12-9.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#include "NDAnimation.h"
#include "NDTile.h"
#include "NDAnimationGroup.h"
#include "NDFrame.h"

using namespace cocos2d;


NDAnimation::NDAnimation()
: m_Frames(NULL)
, m_nX(0)
, m_nY(0)
, m_nW(0)
, m_nH(0)
, m_nMidX(0)
, m_nBottomY(0)
, m_nType(0)
, m_bReverse(false)
, m_BelongAnimationGroup(NULL)
, m_nCurIndexInAniGroup(-1)
{
	m_Frames = new cocos2d::CCMutableArray<NDFrame*>();
}

NDAnimation::~NDAnimation()
{
	CC_SAFE_RELEASE(m_Frames);
}

CGRect NDAnimation::getRect()
{
	if (m_BelongAnimationGroup) 
	{
		int px = m_BelongAnimationGroup->getPosition().x, py = m_BelongAnimationGroup->getPosition().y;
		if (m_nMidX != 0) 
		{
			px -= m_nMidX - m_nX; 
		}
		if (m_nBottomY != 0) 
		{
			py -= m_nBottomY - m_nY;
		}
		return CGRectMake(px, py, m_nW, m_nH);
	}
	return CGRectZero;
}

void NDAnimation::runWithRunFrameRecord(NDFrameRunRecord* runFrameRecord, bool needDraw, float drawScale)
{
	if (m_Frames->count()) 
	{		
		if (runFrameRecord->getCurrentFrameIndex() >= (int)m_Frames->count()) 
		{
			return;
		}
		
		if (runFrameRecord->getNextFrameIndex() != 0 && runFrameRecord->getCurrentFrameIndex() == 0) 
		{
			if (m_nType == ANIMATION_TYPE_ONCE_END) 
			{
				NDFrame* frame = m_Frames->getLastObject();
				if (needDraw) 
				{
					frame->run(drawScale);
				}
				return;
			}
			else if (m_nType == ANIMATION_TYPE_ONCE_START)
			{
				NDFrame* frame = m_Frames->getObjectAtIndex(0);
				if (needDraw) 
				{
					frame->run(drawScale);
				}
				return;
			}
		}
		
		//��ȡ�����ĵ�ǰ֡
		NDFrame *frame = m_Frames->getObjectAtIndex(runFrameRecord->getCurrentFrameIndex());
		
		//�ж��Ƿ���������һ֡���������������һ֡���������ܵ�ǰ֡
		if (frame->enableRunNextFrame(runFrameRecord)) 
		{
			//runFrameRecord.isCompleted = NO;	
			//ȡ��һ֡
			frame =  m_Frames->getObjectAtIndex(runFrameRecord->getNextFrameIndex());
			
			runFrameRecord->NextFrame((int)m_Frames->count());
			/*
			//��ǰ֡������ֵ�ı�
			if (++runFrameRecord.currentFrameIndex == (int)[_frames count]) 
			{
				runFrameRecord.currentFrameIndex = 0;
				
				if (runFrameRecord.repeatTimes > 0) 
				{
					runFrameRecord.repeatTimes--;
				}
			}
			
			//��һ֡������ֵ�ı�
			runFrameRecord.nextFrameIndex = runFrameRecord.currentFrameIndex + 1;
			if (runFrameRecord.nextFrameIndex == (int)[_frames count]) 
			{
				runFrameRecord.nextFrameIndex = 0;
				if (runFrameRecord.repeatTimes == 0) 
				{
					runFrameRecord.isCompleted = YES;
				}

			}
			*/
		}
		
		if (needDraw) 
		{
			//��һ֡
			frame->run(drawScale);
		}
	}
}

void NDAnimation::runWithRunFrameRecord(NDFrameRunRecord* runFrameRecord, bool needDraw)
{	
	this->runWithRunFrameRecord(runFrameRecord, needDraw, 1.0f);
}

void NDAnimation::SlowDown(unsigned int multi)
{
	if (m_Frames->count()) 
	{
		for (unsigned int i=0; i < m_Frames->count(); i++) 
		{
			NDFrame *frame = m_Frames->getObjectAtIndex(i);
			frame->setEnduration(frame->getEnduration()*multi);
		}
	}
}
