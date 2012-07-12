/*
 *  NDLocalization.h
 *  DragonDrive
 *
 *  Created by jhzheng on 11-12-7.
 *  Copyright 2011 (����)DeNA. All rights reserved.
 *
 */

// ���ػ��ӿ�
// 1.NDNSString, ����NSString�ַ���, ���ʷ�Χ��ǰ�ļ�(key���޵�ǰ�ļ�ʹ��)
// 2.NDCString, ����C�ַ���, ���ʷ�Χ��ǰ�ļ�(key���޵�ǰ�ļ�ʹ��)
// 3.NDCommonNSString, ����NSString�ַ���, ���ʷ�Χ�����ļ�(�������ļ�����key,����������ʷ�ΧΪ��ǰ�ļ��Ĳ�����ͻ)
// 4.NDCommonCString, ����C�ַ���, ���ʷ�Χ�����ļ�(�������ļ�����key,����������ʷ�ΧΪ��ǰ�ļ��Ĳ�����ͻ)
// �÷�:
// 1.NDNSString(@"example");
// 2.NDCString("example");
// 3.NDCommonNSString(@"example");
// 4.NDCommonCString("example");
#ifndef _ND_LOCALIZATION_H_ZJH_
#define _ND_LOCALIZATION_H_ZJH_

#include "NDLocalXmlString.h"

// inline const char* __NDLOCAL_INNER_STRING(NSString* nsKeyName)
// {
// 	//return [NSLocalizedStringWithDefaultValue(nsKeyName, @"lyol", [NSBundle mainBundle], nsKeyName, NULL) UTF8String];
// 	return [NDLocalXmlString::GetSingleton().GetString(nsKeyName) UTF8String];
// }

inline const char* __NDLOCAL_INNER_C_STRING(const char* szModuleName, const char* szKeyName)
{
	//return [NSLocalizedStringWithDefaultValue(nsKeyName, @"lyol", [NSBundle mainBundle], nsKeyName, NULL) UTF8String];
	if (!szModuleName || !szKeyName)
	{
		return "";
	}
	
	std::string str		= szModuleName;
	str					+= szKeyName; 
	return NDLocalXmlString::GetSingleton().GetCString(str.c_str()).c_str();
}

#define _NDLOCAL_INNER_NS_STRING( NSKeyFileName, NSKeyName) \
__NDLOCAL_INNER_STRING("%s_%s", NSKeyFileName, NSKeyName])

#define _NDLOCAL_INNER_CString_STRING( NSKeyFileName, cStringKeyName) \
__NDLOCAL_INNER_STRING("%s_%s", NSKeyFileName, cStringKeyName])

#pragma mark ���ļ���+"_"+KeyName��Ϊ��(ͨ��key���ص��ִ��ڵ�ǰ�ļ���ͬ)

// key_name type NSString
#define NDNSString(NSKeyName) \
_NDLOCAL_INNER_NS_STRING((NDString(__FILE__).getFileName(), NSKeyName)

// key_name type cString(null end)
#define NDCString(cStringKeyName) \
_NDLOCAL_INNER_CString_STRING(NDString(__FILE__).getFileName(), cStringKeyName)

#pragma mark ��"Common"+"_"+KeyName��Ϊ��(ͨ��key���ص��ִ���ͬ)

// key_name type NSString
#define NDCommonNSString(NSKeyName) \
_NDLOCAL_INNER_NS_STRING(@"Common", NSKeyName)

// key_name type cString(null end)
#define NDCommonCString(cStringKeyName) __NDLOCAL_INNER_C_STRING("Common", cStringKeyName)

////////////////////////////////////////////////////////////////////////////////
// ���ط��ӿ�(����NSString),�÷�ͬ��
// 1.NDNSString_RETNS
// 2.NDCString_RETNS
// 3.NDCommonNSString_RETNS
// 4.NDCommonCString_RETNS
// inline NSString* __NDLOCAL_INNER_STRING_RETNS(NSString* nsKeyName)
// {
// 	//return NSLocalizedStringWithDefaultValue(nsKeyName, @"lyol", [NSBundle mainBundle], nsKeyName, NULL);
// 	return NDLocalXmlString::GetSingleton().GetString(nsKeyName);
// }

#define _NDLOCAL_INNER_NS_STRING_RETNS( NSKeyFileName, NSKeyName) \
__NDLOCAL_INNER_STRING_RETNS("%s_%s", NSKeyFileName, NSKeyName)

#define _NDLOCAL_INNER_CString_STRING_RETNS( NSKeyFileName, cStringKeyName) \
__NDLOCAL_INNER_STRING_RETNS("%s_%s", NSKeyFileName, cStringKeyName])

#pragma mark ���ļ���+"_"+KeyName��Ϊ��(ͨ��key���ص��ִ��ڵ�ǰ�ļ���ͬ)-----����NSString

// key_name type NSString
#define NDNSString_RETNS(NSKeyName) \
_NDLOCAL_INNER_NS_STRING_RETNS(NDString(__FILE__).getFileName(), NSKeyName)

// key_name type cString(null end)
#define NDCString_RETNS(cStringKeyName) \
_NDLOCAL_INNER_CString_STRING_RETNS(NSString(__FILE__).getFileName(), cStringKeyName)

#pragma mark ��"Common"+"_"+KeyName��Ϊ��(ͨ��key���ص��ִ���ͬ)-----����NSString

// key_name type NSString
#define NDCommonNSString_RETNS(NSKeyName) \
_NDLOCAL_INNER_NS_STRING_RETNS(@"Common", NSKeyName)

// key_name type cString(null end)
#define NDCommonCString_RETNS(cStringKeyName) \
_NDLOCAL_INNER_CString_STRING_RETNS(@"Common", cStringKeyName)

#endif // _ND_LOCALIZATION_H_ZJH_