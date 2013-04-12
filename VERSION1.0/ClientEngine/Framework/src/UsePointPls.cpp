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
#include "CCApplication.h"

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
}

//Point -> Pixel
void ConvertUtil::convertToPixelCoord( CCSize& sz )
{
	float fScale = CCDirector::sharedDirector()->getContentScaleFactor();
	if (fScale != 0) {
		sz.width *= fScale;
		sz.height *= fScale;
	}
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
}

//������ͼ�߼���ߴ�
CCSize ConvertUtil::getTextureSizeInPoints( /*const*/ cocos2d::CCTexture2D& tex )
{
	float fScale = CCDirector::sharedDirector()->getContentScaleFactor();
	return CCSizeMake( 
		tex.getPixelsWide() / fScale,
		tex.getPixelsHigh() / fScale );
}

//�������� -> ��ʾ����
CCPoint ConvertUtil::convertCellToDisplay( const int cellX, const int cellY )
{
	return ccp( cellX * MAP_UNITSIZE_X + DISPLAY_POS_X_OFFSET,
				cellY * MAP_UNITSIZE_Y + DISPLAY_POS_Y_OFFSET);
}

//��ʾ���� -> ��������
CCPoint ConvertUtil::convertDisplayToCell( const CCPoint& display, bool bAligned /*= false*/ )
{
	CCPoint cellPos = ccp( 
		(display.x - DISPLAY_POS_X_OFFSET) / MAP_UNITSIZE_X,
		(display.y - DISPLAY_POS_Y_OFFSET) / MAP_UNITSIZE_Y);

// #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
// 	if (bAligned)
// 	{
// 		cellPos.x = int(cellPos.x + 0.5f);
// 		cellPos.y = int(cellPos.y + 0.5f);
// 	}
// #endif
	return cellPos;
}

//�������� -> ��Ļ����
CCPoint ConvertUtil::convertCellToScreen( const int cellX, const int cellY )
{
	return ccp( cellX * MAP_UNITSIZE_X, cellY * MAP_UNITSIZE_Y );
}

//��Ļ���� -> ��������
CCPoint ConvertUtil::convertScreenToCell( const CCPoint& screen )
{
	return ccp( screen.x / MAP_UNITSIZE_X, screen.y / MAP_UNITSIZE_Y );
}

//�������� -> ��ʾ���� ��X��
float ConvertUtil::convertCellToDisplayX( const int cellX )
{
	return (float) cellX * MAP_UNITSIZE_X + DISPLAY_POS_X_OFFSET;
}

//�������� -> ��ʾ���� ��Y��
float ConvertUtil::convertCellToDisplayY( const int cellY )
{
	return (float) cellY * MAP_UNITSIZE_Y + DISPLAY_POS_Y_OFFSET;
}

//ȡ���ӳߴ�
CCSize ConvertUtil::getCellSize()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //@android
	float fScale = getAndroidScale().y; //��Y�������ȣ�ά�ָ߿�ȣ��ȱ�����
	return CCSizeMake( 32 * fScale, 32 * fScale );
#else
    float fSize = 16 * CCDirector::sharedDirector()->getContentScaleFactor();
    if (IS_IPAD)
    {
        fSize *= (768.0 / 320.0);
    }
    return CCSizeMake( fSize, fSize );
#endif
}

//@android
CCPoint ConvertUtil::getAndroidScale()
{
	return NDDirector::DefaultDirector()->getAndroidScale();
}

//@ios
float ConvertUtil::getIosScale()
{
	return NDDirector::DefaultDirector()->getIosScale();
}

bool ConvertUtil::is_iphone5()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	CCSize sizePoints = CCDirector::sharedDirector()->getWinSize();
	if (int(sizePoints.width) == IPHONE5_POINT_SIZE_WIDTH)
	{
		return true;
	}
#endif
	return false;
}

float ConvertUtil::get_iphone5_width_scale_for_ui_480_based()
{
	//UI�����ǻ���480*320
	return float(IPHONE5_POINT_SIZE_WIDTH) / 480.0f;
}

bool ConvertUtil::is_ipad()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (CCApplication::sharedApplication()->getTargetPlatform() == kTargetIpad)
        return true;
#endif
	return false;
}

float ConvertUtil::get_ipad_width_scale_for_ui_480_based()
{
    return CCDirector::sharedDirector()->getContentScaleFactor() *
                float(IPAD_POINT_SIZE_WIDTH) / 480.0f;
}

float ConvertUtil::get_ipad_height_scale_for_ui_480_based()
{
    return CCDirector::sharedDirector()->getContentScaleFactor() *
                float(IPAD_POINT_SIZE_HEIGHT) / 320.0f;
}

NS_NDENGINE_END
