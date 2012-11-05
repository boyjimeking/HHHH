//
//  NDClassFactory.h
//  NDClassFactory
//
//  Copyright 2010 (����)DeNA. All rights reserved.
//	
//	@author ����
//
//	�������ܣ���
//  ������ǿ��Ա��̳еġ�

#ifndef NDENABLESHAREDFROMTHIS_H
#define NDENABLESHAREDFROMTHIS_H

#include "NDSharedPtr.h"
#include "NDWeakPtr.h"

template<typename T>
class NDEnableSharedFromThis
{
public:
	NDSharedPtr<T> SharedFromThis()
	{
		return NDSharedPtr<T>( m_kWeakThis );
	}
public:
	void InternalAcceptOwner(const NDSharedPtr<T>& p)
	{
		m_kWeakThis = p;
	}
private:
	NDWeakPtr<T> m_kWeakThis;
};

#endif