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
#include "NDDirector.h"

USING_NS_CC;

//-----------------------------------------------------------------------------------------------------------
//	***	��Դ���ű��� & �������ű���	***
//
//	��ע����Щ�����Ѿ����ǵ���ios & android�Ĳ���.
//	������
//		�ֱ���480*320		scale=1
//		�ֱ���960*640		scale=2
//		�ֱ���800*480		scale=1.5����YΪ����

//��Դ���ű���&�������ű�����һ����ͬ��
//��ע����"_960"��β�ĺ꣬��ʾ����960*640���������480*320
//		�ṩ���׺꣬�����ڴ����г��ֳ�2��2��.

/** ����Դ���ű����� */
#define RESOURCE_SCALE		(NDDirector::DefaultDirector()->getResourceScale())			//����480*320
#define RESOURCE_SCALE_INT	int(RESOURCE_SCALE)											//����480*320
#define RESOURCE_SCALE_960	(0.5f * RESOURCE_SCALE)										//����960*640

/** ���������ű����� */
#define COORD_SCALE_X_960	(NDDirector::DefaultDirector()->getCoordScaleX())			//����960*640
#define COORD_SCALE_Y_960	(NDDirector::DefaultDirector()->getCoordScaleY())			//����960*640
#define COORD_SCALE_X		(COORD_SCALE_X_960 * 2.0f)									//����480*320
#define COORD_SCALE_Y		(COORD_SCALE_Y_960 * 2.0f)									//����480*320

/** ���������ű����� */
#define FONT_SCALE			RESOURCE_SCALE												//����480*320
#define FONT_SCALE_INT		int(FONT_SCALE)												//����480*320

#define FONT_SCALE_X		(COORD_SCALE_X)											//����480*320������������X����Ϊ��׼��
#define FONT_SCALE_DRAMA	(FONT_SCALE_X > 2 ? 2 : FONT_SCALE_X)					//����480*320

//-----------------------------------------------------------------------------------------------------------

#define IPHONE5_POINT_SIZE_WIDTH	568



NS_NDENGINE_BGN
class ConvertUtil
{
public:
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
	static CCPoint convertDisplayToCell( const CCPoint& display, bool bAligned = false );

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

public:
	static bool		is_iphone5();
	static float	get_iphone5_width_scale_for_ui_480_based();
};

//-----------------------------------------------------------------------------------------------------------

//֮ǰ�ĸ����Ƿ��񣨼��ȿ�ȸߣ������ǵ�androidƽ̨��ֱ��ʣ�MAP_UNITSIZEӦ���Ϊxy��������.
#define MAP_UNITSIZE_X				int(ConvertUtil::getCellSize().width)
#define MAP_UNITSIZE_Y				int(ConvertUtil::getCellSize().height)

//��ɫ��ʾʱ�����Cell��ƫ��
#define DISPLAY_POS_X_OFFSET		(MAP_UNITSIZE_X / 2)
#define DISPLAY_POS_Y_OFFSET		(MAP_UNITSIZE_Y)

//-----------------------------------------------------------------------------------------------------------

#define IS_IPHONE5					(ConvertUtil::is_iphone5())
#define IPHONE5_WIDTH_SCALE			(ConvertUtil::get_iphone5_width_scale_for_ui_480_based())

NS_NDENGINE_END