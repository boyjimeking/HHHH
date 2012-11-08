/*
 *  define.h
 *  DragonDrive
 *
 *  Created by wq on 11-1-5.
 *  Copyright 2011 (����)DeNA. All rights reserved.
 *
 */

#ifndef __DEFINE_H__
#define __DEFINE_H__

//typedef unsigned int OBJID;

#include "CCObject.h"

#define SEND_DATA(bao) do{\
	NDSocket* skt = NDEngine::NDDataTransThread::DefaultThread()->GetSocket();\
	if (skt)\
		skt->Send(&bao);\
}while(0)

#define ShowProgressBar (NDUISynLayer::Show())
#define CloseProgressBar (NDUISynLayer::Close())
#define srandom(x) rand()
#define random(x) rand()

#define BEGIN_ND_NAMESPACE namespace NDEngine{
#define END_ND_NAMESPACE }

/**
* �������objective-c �µĳ��ñ������ߺ�
* @author ����
*/

#pragma warning(disable:4006)
#pragma warning(disable:4221)

#ifndef YES
#define YES true
#endif

#ifndef NO
#define NO false
#endif

typedef double NSTimeInterval;

#define NSUInteger int

#ifndef nil
	#define nil 0
#endif

typedef cocos2d::CCObject base,*id;
typedef unsigned int UInt32;
typedef unsigned short UInt16;
typedef unsigned char UInt8;

typedef enum
{
	MainSceneBegin = 10010,
	BottomSpeedBar,
	PlayerAttr,
	PlayerBackBag
}NMAINSCENECHILDTAG;

#define NSString cocos2d::CCString

#define SAFE_DELETE(pObject)\
do \
{\
	if (0 != pObject)\
	{\
		delete pObject;\
		pObject = 0;\
	}\
} while (false)

#define SAFE_DELETE_ARRAY(pObject)\
do \
{\
	if (0 != pObject)\
	{\
		delete [] pObject;\
		pObject = 0;\
	}\
} while (false)

#define SAFE_RELEASE(pObject)\
do \
{\
	if (0 != pObject)\
	{\
		pObject->release();\
		pObject = 0;\
	}\
} while (false)

#define NS_NDENGINE_BGN		namespace NDEngine{
#define NS_NDENGINE_END		}
#define USING_NS_ND			using namespace NDEngine;

#endif // __DEFINE_H__