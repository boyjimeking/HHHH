/*
 *  NDUISpriteNode.h
 *  SMYS
 *
 *  Created by jhzheng on 12-2-22.
 *  Copyright 2012 (����)DeNA. All rights reserved.
 *
 */

#ifndef _UI_SPRITE_NODE_H_ZJH_
#define _UI_SPRITE_NODE_H_ZJH_

#include "NDUINode.h"
#include "NDSprite.h"

using namespace NDEngine;

class CUISpriteNode : public NDUINode/*, public ISpriteEvent*/
{
	DECLARE_CLASS(CUISpriteNode)
	
	CUISpriteNode();
	~CUISpriteNode();
	
public:
	void Initialization();
	void ChangeSprite(const char* sprfile);
	bool IsAnimationComplete();
	void SetAnimation(int nIndex, bool bFaceRight);
	void SetPlayFrameRange(int nStartFrame, int nEndFrame);
	public:
    //interface of ISpriteEvent
    /*���鲥������֡�ص�*/
    //virtual void DisplayFrameEvent(int nCurrentAnimation, int nCurrentFrame);
    /*һ������������ɺ�Ļص�*/
    //virtual void DisplayCompleteEvent(int nCurrentAnimation, int nDispCount);
    
	//+2012.6.3++ Guosen ++ 
	//�������ű���
	void SetScale( float fScale );
	//��ȡ���ű���
	float GetScale();
	
	//��øþ���������������
	unsigned int GetAnimationAmount();
	
	//����ָ����������
	void PlayAnimation( unsigned int nIndex, bool bReverse );
	//+2012.6.3
	
	void setExtra( const int extra );

private:

	NDUINode*				m_pSpriteParentNode;
	NDSprite*				m_pSprite;
	
protected:
	void draw(); override
	void debugDraw();
};

#endif // _UI_SPRITE_NODE_H_ZJH_