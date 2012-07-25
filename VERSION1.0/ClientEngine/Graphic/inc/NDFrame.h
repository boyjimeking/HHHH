//
//  NDFrame.h
//  DragonDrive
//
//  Created by xiezhenghai on 10-12-9.
//  Copyright 2010 (����)DeNA. All rights reserved.
//

#ifndef _ND_FRAME_H_
#define _ND_FRAME_H_

#include "platform/CCPlatformMacros.h"
#include "CCMutableArray.h"
#include "NDTile.h"
#include "NDAnimationGroup.h"

class NDFrameRunRecord : public cocos2d::CCObject
{
	CC_PROPERTY(int, m_nNextFrameIndex, NextFrameIndex)
	CC_PROPERTY(int, m_nCurrentFrameIndex, CurrentFrameIndex)
	CC_PROPERTY(int, m_nRunCount, RunCount)
//	CC_PROPERTY(bool, m_bIsCompleted, IsCompleted)
	CC_PROPERTY(int, m_nRepeatTimes, RepeatTimes)

public:
	NDFrameRunRecord();

	void SetPlayRange(int nStartFrame, int nEndFrame);
	void NextFrame(int nTotalFrames);

private:
	int m_nStartFrame, m_nEndFrame;
	BOOL m_bSetPlayRange;
};

//////////////////////////////////////////////////////////////////////////
//��������ʹ�õ�����Ƭ
class NDFrameTile : public cocos2d::CCObject
{
	CC_PROPERTY(int, m_nX, X)
	CC_PROPERTY(int, m_nY, Y)
	CC_PROPERTY(int, m_nRotation, Rotation)
	CC_PROPERTY(int, m_nTableIndex, TableIndex)

public:
	NDFrameTile();
};

//////////////////////////////////////////////////////////////////////////
typedef struct _TILE_REVERSE_ROTATION
{
	BOOL reverse;					//�Ƿ�ת
	NDRotationEnum rotation;		//��ת�Ƕ�
	float tileW;
}TILE_REVERSE_ROTATION;

class NDAnimation;
class NDAnimationGroup;
class NDFrame : public cocos2d::CCObject
{
	CC_PROPERTY(int, m_nEnduration, Enduration)
	CC_PROPERTY(NDAnimation*, m_BelongAnimation, BelongAnimation)
	CC_SYNTHESIZE_RETAIN(cocos2d::CCMutableArray<NDAnimationGroup*>*, m_SubAnimationGroups, SubAnimationGroups)
	CC_SYNTHESIZE_RETAIN(cocos2d::CCMutableArray<NDFrameTile*>*, m_FrameTiles, FrameTiles)

public:
	NDFrame();
	~NDFrame();

	void initTiles();
	//�Ƿ���������һ֡
	bool enableRunNextFrame(NDFrameRunRecord* frameRunRecord);
	//��һ֡
	void run();
	void run(float scale);

	// ��������ͷ��
	void drawHeadAt(CGPoint pos);

private:
	int						m_count;
	bool					m_needInitTitles;
	cocos2d::CCMutableArray<NDTile*>* m_tiles;

private:
	TILE_REVERSE_ROTATION tileReverseRotationWithReverse(bool reverse, int rota);
	cocos2d::CCTexture2D* getTileTextureWithImageIndex(int imageIndex, int replace);
	float getTileW(int w, int h, NDRotationEnum rotation);
};

#endif