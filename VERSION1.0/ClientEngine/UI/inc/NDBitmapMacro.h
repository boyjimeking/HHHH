//-------------------------------------------------------------
//  NDBitmapMacro.h
//
//  Created by zhangwq on 2013-02-20.
//  Copyright 2010 (����)DeNA. All rights reserved.
//
//	NDBitmap���ƵĿ���
//-------------------------------------------------------------

#pragma once

//@ndbitmap
//�Ƿ��NDBITMAP���׻��Ʊ����ȥ����֧��android��
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	#define WITH_NDBITMAP 1
#else
	#define WITH_NDBITMAP 0
#endif
#define WITH_NDBITMAP 0 //���뿪��

