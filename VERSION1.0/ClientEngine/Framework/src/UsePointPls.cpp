//
//  UsePointPls.cpp
//  use point coordinate please.
//
//  Created by zhangwq on 2012-11-8.
//  Copyright 2010 (����)DeNA. All rights reserved.
//
//	˵����
//	���ò��漰����ʱ���������ꡢ�������꣩��Ҫʹ�����ظ��
//	ͳһʹ��Point��������ΪGL���굥λ���������굥λ����
//	ʹ��Point��������Զ���Ӧ��ֱ��ʣ�
//

#include "UsePointPls.h"
#include "CCDirector.h"
#include "NDDirector.h"
#include "CCPointExtension.h"

USING_NS_CC;

NS_NDENGINE_BGN

//Pixel -> Point
void ConvertUtil::convertToPointCoord( CCPoint& pt )
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
	float fScale = CCDirector::sharedDirector()->getContentScaleFactor();
	if (fScale != 0) {
		pt.x /= fScale;
		pt.y /= fScale;
	}
#endif
}

//Pixel -> Point
void ConvertUtil::convertToPointCoord( cocos2d::CCSize& sz )
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
	float fScale = CCDirector::sharedDirector()->getContentScaleFactor();
	if (fScale != 0) {
		sz.width /= fScale;
		sz.height /= fScale;
	}
#endif
}

//Pixel -> Point
void ConvertUtil::convertToPointCoord( cocos2d::CCRect& rc )
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
	float fScale = CCDirector::sharedDirector()->getContentScaleFactor();
	if (fScale != 0) {
		rc.origin.x /= fScale;
		rc.origin.y /= fScale;
		rc.size.width /= fScale;
		rc.size.height /= fScale;
	}
#endif
}

//Point -> Pixel
void ConvertUtil::convertToPixelCoord( CCPoint& pt )
{
	float fScale = CCDirector::sharedDirector()->getContentScaleFactor();
	if (fScale != 0) {
		pt.x *= fScale;
		pt.y *= fScale;
	}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //@android //@todo
#endif
}

//Point -> Pixel
void ConvertUtil::convertToPixelCoord( CCSize& sz )
{
	float fScale = CCDirector::sharedDirector()->getContentScaleFactor();
	if (fScale != 0) {
		sz.width *= fScale;
		sz.height *= fScale;
	}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //@android //@todo
#endif
}

//Point -> Pixel
void ConvertUtil::convertToPixelCoord( CCRect& rc )
{
	float fScale = CCDirector::sharedDirector()->getContentScaleFactor();
	if (fScale != 0) {
		rc.origin.x *= fScale;
		rc.origin.y *= fScale;
		rc.size.width *= fScale;
		rc.size.height *= fScale;
	}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //@android //@todo
#endif
}

//������ͼ�߼���ߴ�
CCSize ConvertUtil::getTextureSizeInPoints( /*const*/ cocos2d::CCTexture2D& tex )
{
	float fScale = CCDirector::sharedDirector()->getContentScaleFactor();
	return CCSizeMake( 
		tex.getPixelsWide() / fScale,
		tex.getPixelsHigh() / fScale );
}



//Pixel -> Point
void ConvertUtil::convertToPointCoord_Android( CCPoint& pt )
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //@android
	pt.x *= NDDirector::DefaultDirector()->getAndroidScale().x;
	pt.y *= NDDirector::DefaultDirector()->getAndroidScale().y;
#endif
}

//Pixel -> Point
void ConvertUtil::convertToPointCoord_Android( cocos2d::CCSize& sz )
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //@android
	sz.width	*= NDDirector::DefaultDirector()->getAndroidScale().x;
	sz.height	*= NDDirector::DefaultDirector()->getAndroidScale().y;
#endif
}

//@android
//Pixel -> Point
void ConvertUtil::convertToPointCoord_Android( cocos2d::CCRect& rc )
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //@android
	rc.origin.x		*= NDDirector::DefaultDirector()->getAndroidScale().x;
	rc.origin.y		*= NDDirector::DefaultDirector()->getAndroidScale().y;
	rc.size.width	*= NDDirector::DefaultDirector()->getAndroidScale().x;
	rc.size.height	*= NDDirector::DefaultDirector()->getAndroidScale().y;
#endif
}

//�������� -> ��ʾ����
CCPoint ConvertUtil::convertCellToDisplay( const int cellX, const int cellY )
{
	return ccp( cellX * MAP_UNITSIZE + DISPLAY_POS_X_OFFSET,
				cellY * MAP_UNITSIZE + DISPLAY_POS_Y_OFFSET);
}

//��ʾ���� -> ��������
CCPoint ConvertUtil::convertDisplayToCell( const CCPoint& display )
{
	return ccp( (display.x - DISPLAY_POS_X_OFFSET) / MAP_UNITSIZE,
				(display.y - DISPLAY_POS_Y_OFFSET) / MAP_UNITSIZE);
}

//�������� -> ��Ļ����
CCPoint ConvertUtil::convertCellToScreen( const int cellX, const int cellY )
{
	return ccp( cellX * MAP_UNITSIZE, cellY * MAP_UNITSIZE );
}

//��Ļ���� -> ��������
CCPoint ConvertUtil::convertScreenToCell( const CCPoint& screen )
{
	return ccp( screen.x / MAP_UNITSIZE, screen.y / MAP_UNITSIZE );
}

//�������� -> ��ʾ���� ��X��
float ConvertUtil::convertCellToDisplayX( const int cellX )
{
	return (float) cellX * MAP_UNITSIZE + DISPLAY_POS_X_OFFSET;
}

//�������� -> ��ʾ���� ��Y��
float ConvertUtil::convertCellToDisplayY( const int cellY )
{
	return (float) cellY * MAP_UNITSIZE + DISPLAY_POS_Y_OFFSET;
}

NS_NDENGINE_END
