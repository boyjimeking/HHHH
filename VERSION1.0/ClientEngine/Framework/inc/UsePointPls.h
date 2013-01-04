//
//  UsePointPls.h
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

#pragma once

#include "define.h"
#include "CCGeometry.h"
#include "CCTexture2D.h"
#include "NDUtil.h"

USING_NS_CC;

NS_NDENGINE_BGN

class ConvertUtil
{
public:
	// pixel -> point (������android) ��ע����һ�׳����Ƚϱ���.
	static void convertToPointCoord_Android( CCPoint& pt );
	static void convertToPointCoord_Android( CCSize& sz );
	static void convertToPointCoord_Android( CCRect& rc );

	// pixel -> point
	static void convertToPointCoord( CCPoint& pt );
	static void convertToPointCoord( CCSize& sz );
	static void convertToPointCoord( CCRect& rc );

	// point -> pixel
	static void convertToPixelCoord( CCPoint& pt );
	static void convertToPixelCoord( CCSize& sz );
	static void convertToPixelCoord( CCRect& rc );

	static CCSize getTextureSizeInPoints( /*const*/ CCTexture2D& tex );

public: //�����������ת�� @cell

	//�������� -> ��ʾ����
	static CCPoint convertCellToDisplay( const int cellX, const int cellY );

	//��ʾ���� -> ��������
	static CCPoint convertDisplayToCell( const CCPoint& display );

	//�������� -> ��Ļ����
	static CCPoint convertCellToScreen( const int cellX, const int cellY );

	//��Ļ���� -> ��������
	static CCPoint convertScreenToCell( const CCPoint& screen );

	//�������� -> ��ʾ���� ����ֿ�ת����
	static float convertCellToDisplayX( const int cellX );
	static float convertCellToDisplayY( const int cellY );

public:
	static CCSize getCellSize();

public: //@android
	static CCPoint getAndroidScale();
	static float getIosScale();
};

//֮ǰ�ĸ����Ƿ��񣨼��ȿ�ȸߣ������ǵ�androidƽ̨��ֱ��ʣ�MAP_UNITSIZEӦ���Ϊxy��������.

#define MAP_UNITSIZE_X				(ConvertUtil::getCellSize().width)
#define MAP_UNITSIZE_Y				(ConvertUtil::getCellSize().height)

//��ɫ��ʾʱ�����Cell��ƫ��
#define DISPLAY_POS_X_OFFSET		(MAP_UNITSIZE_X / 2)
#define DISPLAY_POS_Y_OFFSET		(MAP_UNITSIZE_Y)

//android���ű�������YΪ����
#define ANDROID_SCALE				(ConvertUtil::getAndroidScale().y)

#define IOS_SCALE					(ConvertUtil::getIosScale())

NS_NDENGINE_END