//-----------------------------------------------------------------
//  StringConvert.h
//
//  Created by zhangwq on 2012-12-21.
//  Copyright 2010 (����)DeNA. All rights reserved.
//
//	�ַ�����ͬ����ת��
//	��֧��GB2312��utf8���룬����֧�����п��ַ������ʽ
//
//	֧��win32,android,ios
//
//	��������������cocos2dx����Ա�ΪCCImage���ṩ����ת������.
//
//	CCImage����ڱ��������⣺
//	CCImageʵ�ֺ����˲�ͬ���루android&ios��utf8, win32��ansi��
//	��ͬ���뵼���˺���ԭ�͵�ģ������һ���������ϲ㴫�����������.
//-----------------------------------------------------------------

#pragma once

#include "platform/CCPlatformMacros.h"

class CC_DLL StringConvert
{
public:
	static const char* convert( const char* fromcode, const char* tocode, const char* str );
	static bool isUTF8ChineseCharacter( const char* str );
};

#define CONVERT_GBK_TO_UTF8(gbk)		StringConvert::convert("gb2312", "utf-8", gbk)
#define CONVERT_UTF8_TO_GBK(utf8)		StringConvert::convert("utf-8", "gb2312", utf8)

#define GBKToUTF8						CONVERT_GBK_TO_UTF8 
