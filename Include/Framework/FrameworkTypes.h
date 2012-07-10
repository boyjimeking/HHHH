/*
��ܳ��ö���
Copyright(c) 1999-2012, TQ Digital Entertainment, All Rights Reserved
*/
#ifndef __FRAMEWORKTYPES__H
#define __FRAMEWORKTYPES__H
#include "BaseType.h"
#include "C3Primitive.h"
#include "uitypes.h"

#define UNUSED_PARAM(unsedparam)


//��������
typedef struct
{
	CPoint OldPos;
	CPoint CurPos;
} TOUCHPOS_INFO;

//����״̬	
typedef enum TOUCHSTATE_TAG
{
	TOUCH_DOWN,//����
	TOUCH_MOVE,//�ƶ�
	TOUCH_UP,//����
}TOUCHSTATE;

//����
typedef struct
{
	int tapCount;					
	TOUCHSTATE state;//����״̬						
	vector<TOUCHPOS_INFO> vecPos;	
} TOUCH_EVENT_INFO;


typedef struct TQTimeVal_TAG
{
	long tv_Sec; //Second
	long tv_uSec; //MicroSecond
}TQTimeVal;

//ʱ��
class TQTime
{
public:
	static void GetTimeofDay(TQTimeVal& refTime, void* timeZone);

	static TQTimeVal GetTimeInterval(const TQTimeVal& refTvBegin, const TQTimeVal& refTvEnd);

	static float GetTimeIntervalEx(const TQTimeVal& refTvBegin, const TQTimeVal& refTvEnd);
};


typedef enum {
	/// Device oriented vertically, home button on the bottom
	TQDeviceOrientationPortrait = 0, // UIDeviceOrientationPortrait,	
	/// Device oriented vertically, home button on the top
	TQDeviceOrientationPortraitUpsideDown = 1, // UIDeviceOrientationPortraitUpsideDown,
	/// Device oriented horizontally, home button on the right
	TQDeviceOrientationLandscapeLeft = 2, // UIDeviceOrientationLandscapeLeft,
	/// Device oriented horizontally, home button on the left
	TQDeviceOrientationLandscapeRight = 3, // UIDeviceOrientationLandscapeRight,
} TQDeviceOrientation;


void ShowLineRelative(int nPosX1, int nPosY1, int nPosX2, int nPosY2, DWORD dwColor);
#ifndef WIN32
//MACϵͳ�£���֧��itoa,ʵ��һ����
char * __itoa(int _Val, char * _DstBuf, int _Radix);
#endif


#endif