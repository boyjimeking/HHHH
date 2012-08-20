/*
 *  NDCombinePicture.h
 *  DragonDrive
 *
 *  Created by jhzheng on 11-7-5.
 *  Copyright 2011 ����(DeNA). All rights reserved.
 *
 */

#ifndef _ND_COMBINE_PICTURE_H_
#define _ND_COMBINE_PICTURE_H_

#include "NDPicture.h"
#include <vector>

using namespace NDEngine;

// ��Ҫ����
// 1.ƴ��N��NDPicture����
// 2.��������NDPicture����Ļ������ͷ�
// 3.ƴ�ӹ���:�����ӵĶ���ɷ������һ������ľŸ�����λ��(�����Լ�)

typedef enum
{
	// �Լ�������һ������Ϊ��׼,Ϊ��ʱ����ԭ�㴦
	CombintPictureAligmentBegin			= 0, 
	CombintPictureAligmentSelf			= CombintPictureAligmentBegin,
	CombintPictureAligmentRight,
	CombintPictureAligmentRightDown,
	CombintPictureAligmentDown,
	CombintPictureAligmentLeftDown,
	CombintPictureAligmentLeft,
	CombintPictureAligmentLeftUp,
	CombintPictureAligmentUp,
	CombintPictureAligmentRightUp,
	CombintPictureAligmentEnd,										
}CombintPictureAligment;

class NDCombinePicture : public NDObject
{
	DECLARE_CLASS(NDCombinePicture)

public:
	
	NDCombinePicture();
	
	~NDCombinePicture();
	
	void AddPicture(NDPicture* pic, CombintPictureAligment aligment);
	
	void SetColor(cocos2d::ccColor4B color);
	
	CGSize GetSize();
	
	void DrawInRect(CGRect rect);
	
private:
	
	CGPoint caclNext(CGPoint origin, CombintPictureAligment aligment, CGSize originSize, CGSize selfSize);
	
	void Clear();
	
private:
	
	struct CombinePicture 
	{
		NDPicture* pic;
		
		CombintPictureAligment aligment;
		
		CGRect rectDraw;
		
		CombinePicture(NDPicture* pic, CombintPictureAligment aligment, CGRect rectDraw)
		{
			this->pic = pic;
			
			this->aligment = aligment;
			
			this->rectDraw = rectDraw;
		}
	};
	
	CGRect m_rectLast;
	
	CGSize m_sizeMax;
	
	std::vector<CombinePicture> m_vecCombinePic;
};

#endif // _ND_COMBINE_PICTURE_H_