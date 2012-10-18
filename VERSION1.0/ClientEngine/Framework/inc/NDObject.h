//
//  NDObject.h
//  DragonDrive
//
//  Created by xiezhenghai on 10-12-7.
//  Copyright 2010 (网龙)DeNA. All rights reserved.
//
//	－－介绍－－
//	游戏框架基础�??
//	程序中大多数的类都继承自该类
//	如果派生类分别在头文件中实现DECLARE_CLASS，在源文件中实现IMPLEMENT_CLASS这两个宏，那么该派生类就具有了动态识别功�??
//	派生类必须拥有默认构造函数，以备动�??�生成使�??

#ifndef __NDObject_H
#define __NDObject_H

#include "Utility.h"

#define override
#define hide
#define overload

namespace NDEngine
{
//－－模仿mfc动�??�识别－�??
//......
class NDObject;

struct NDRuntimeClass
{
	char* className;
	int objectSize;
	NDObject* (*m_pfnCreateObject)();
	NDRuntimeClass* m_pBaseClass;
	NDRuntimeClass* m_pNextClass;
	static NDRuntimeClass* pFirstClass;
	static NDRuntimeClass* RuntimeClassFromString(const char* ndClassName);
	NDObject* CreateObject();
};

struct NDClassInit
{
	NDClassInit(NDRuntimeClass* pNewClass)
	{
		pNewClass->m_pNextClass = NDRuntimeClass::pFirstClass;
		NDRuntimeClass::pFirstClass = pNewClass;
	}
};

#define RUNTIME_CLASS(class_name) (&class_name::class##class_name)

#define DECLARE_CLASS(class_name) \
public:\
	static NDRuntimeClass class##class_name; \
	virtual NDRuntimeClass* GetRuntimeClass() const; \
	static NDObject* CreateObject();

#define IMPLEMENT_CLASS(class_name, base_class_name) \
	NDObject* class_name::CreateObject()\
	{ \
		return new class_name; \
	} \
	static char _lpsz##class_name[] = #class_name; \
	NDRuntimeClass class_name::class##class_name = { \
	_lpsz##class_name, sizeof(class_name), class_name::CreateObject, \
	RUNTIME_CLASS(base_class_name), NULL }; \
	static NDClassInit _init_##class_name(&class_name::class##class_name); \
	NDRuntimeClass* class_name::GetRuntimeClass() const \
	{\
		return &class_name::class##class_name; \
	}	
//－－－－

class NDObject
{
public:
	NDObject();
	virtual ~NDObject();

public:
//
//		������IsKindOfClass
//		���ã����ڶ�̬ʶ�����ͣ�������֤�����Ƿ�ʱĳһ������丸��Ķ���
//		������runtimeClass��Ҫ��ʶ����࣬���磺RUNTIME_CLASS(NDObject)
	bool IsKindOfClass(const NDRuntimeClass* runtimeClass);
//		
//		������SetDelegate
//		���ã�����ί�У�ע�⣺ȫ�ֶ���ע����ί�У��ͷ�ʱ��ע��SetDelegate(NULL)
//		������receiverί���¼�������
//		����ֵ����	
	void SetDelegate(NDObject* receiver);
//		
//		������GetDelegate
//		���ã���ȡί�еĶ���
//		��������
//		����ֵ����	
	NDObject* GetDelegate();
//		
//		������GetRuntimeClass
//		���ã���ȡ��ʶ����Ϣ
//		��������
//		����ֵ����ʶ����Ϣ�ṹ��	
	virtual NDRuntimeClass* GetRuntimeClass() const;

public:
	static NDRuntimeClass classNDObject;
	static NDObject* CreateObject();

private:
	NDObject* m_delegate;
};
}

#endif
