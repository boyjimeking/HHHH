//
//  NDClassFactory.h
//  NDClassFactory
//
//  Copyright 2010 (����)DeNA. All rights reserved.
//	
//	@author ����
//
//	�������ܣ���
//	���乤���������÷���������ע���봴��
//	REGISTER_CLASS��ע��
//	CREATE_CLASS�Ǵ���

#ifndef NDCLASSFACTORY_H
#define NDCLASSFACTORY_H

#include "define.h"
#include <string>
#include <map>

#define REGISTER_CLASS(BASE_CLASS, DERIVED_CLASS) \
	RegisterInpFactory<BASE_CLASS, DERIVED_CLASS> Register##DERIVED_CLASS(#DERIVED_CLASS)

#define CREATE_CLASS(BASE_CLASS,DERIVED_CLASS) \
	NDClassFactory<BASE_CLASS>::instance().Create(DERIVED_CLASS)

using std::string;
using std::map;

template <class ManufacturedType, typename ClassIDKey = std::string>
class NDClassFactory
{
	typedef ManufacturedType* (*BaseCreateFn)();

	typedef std::map<ClassIDKey, BaseCreateFn> FunctionRegistry;

	FunctionRegistry m_kRegistry;

	NDClassFactory(){}
	NDClassFactory(const NDClassFactory&){}

	NDClassFactory &operator=(const NDClassFactory&){}

public:
	static NDClassFactory& instance()
	{
		static NDClassFactory<ManufacturedType,ClassIDKey> kBF;
		return kBF;
	}

	void RegCreateFn(const ClassIDKey &classname, BaseCreateFn pFunction)
	{
		m_kRegistry[classname] = pFunction;
	}

	ManufacturedType* Create(const ClassIDKey &classname) const
	{
		ManufacturedType* pkTheObject(0);
		typename FunctionRegistry::const_iterator regEntry = m_kRegistry.find(classname);
		if (regEntry != m_kRegistry.end())
		{
			pkTheObject = regEntry->second();
		}
		return pkTheObject;
	}
};

template <class AncestorType, class ManufacturedType, typename ClassIDKey = std::string>
class RegisterInpFactory
{
public:
	static AncestorType* CreateInstance()
	{
		return (new ManufacturedType);
	}
	RegisterInpFactory(const ClassIDKey &id)
	{
		NDClassFactory<AncestorType,ClassIDKey>::instance().RegCreateFn(id, CreateInstance);
	}
};

#endif