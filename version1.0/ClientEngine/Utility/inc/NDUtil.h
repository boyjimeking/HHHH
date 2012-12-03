/*
*
*/

#ifndef NDUTIL_H
#define NDUTIL_H

#include "define.h"
#include "NDPath.h"
#include "CCGeometry.h"

NS_NDENGINE_BGN

//����ϵ��
#define SCREEN_SCALE					(NDDirector::DefaultDirector()->GetScaleFactor())

bool IsPointInside(cocos2d::CCPoint kPoint, cocos2d::CCRect kRect);

//const char* GetSMImgPath(const char* name);


//������صĺ궨��
//---------------------------------------------------------------------------------------------------------<<
//���ӳߴ磨���أ�
#define MAP_UNITSIZE					(16 * ((int)(NDDirector::DefaultDirector()->GetScaleFactor())))

//��ɫ��ʾʱ�����Cell��ƫ��
#define DISPLAY_POS_X_OFFSET			(MAP_UNITSIZE / 2)
#define DISPLAY_POS_Y_OFFSET			(MAP_UNITSIZE)

//--------------------------------------------------------------------------------------------------------->>

NS_NDENGINE_END

#endif