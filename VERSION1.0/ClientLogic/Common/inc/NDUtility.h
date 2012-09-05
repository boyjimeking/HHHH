/*
 *  Utility.h
 *  DragonDrive
 *
 *  Created by wq on 11-1-11.
 *  Copyright 2011 (����)DeNA. All rights reserved.
 *
 */

#ifndef __ND_UTILITY_H__
#define __ND_UTILITY_H__

#include "globaldef.h"

bool IsPointInside(CGPoint pt, CGRect rect);
int GetNumBits(int num);
bool VerifyUnsignedNum(const std::string strnum);
std::string getStringTime(long nSeconds);

std::string changeToChineseSign(std::string old);

//void showDialog(const char* title,const char* content){}
//void showDialog(const char* content){}

void quitGame(bool bTipNet = false);

// string getStringTime(long nSeconds);
// 
// std::string getNextMonthDay(long nSeconds);

// iType: 1->��Ϸ����, 2->��Ϸ����
/*std::string cutBytesToString(NSInputStream* stream, int iType);*/

const char* GetCopyCacheData();
void CopyDataToCopyCache(const char* data);

enum
{
	STRPARAM = 1, UPDATEURL = 2,
};
std::string loadPackInfo(int param);

//��ȡ����汾��
std::string GetSoftVersion();
//��ȡϵͳ�̼��汾
std::string GetIosVersion();
//��ȡ������Ϣ
std::string GetChannelInfo();
//��ȡ���µ�ַ
std::string GetUpdateUrl();

//bool IsSupportPrecisionPic();

std::string platformString();

void drawRectBar2(int x, int y, int color, int num1, int num2, int width);

CGRect getNewNumCut(unsigned int num, bool hightlight);

void ShowAlert(const char* pszAlert);

// ʱ��ת������
typedef enum
{
	TIME_SECOND,
	TIME_MINUTE,
	TIME_HOUR,
	TIME_DAY,
	TIME_DAYTIME,
	TIME_STAMP,
} TIME_TYPE;

// long_time ����Ϊ��λ
uint TimeConvert(TIME_TYPE type /*=TIME_MILLISECOND*/, time_t long_time);

// long_time ����Ϊ��λ
std::string TimeConvertToStr(TIME_TYPE type, time_t long_time);

enum NDLANGUAGE
{
	NDLANGUAGE_None,
	NDLANGUAGE_SimplifiedChinese,				// ��������
	NDLANGUAGE_TraditionalChinese,			// ��������
};

bool IsInSimplifiedChinese();

bool IsTraditionalChinese();

NDLANGUAGE GetLocalLanguage();

#endif

