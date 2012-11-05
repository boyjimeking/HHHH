//
//  NDClassFactory.h
//  NDClassFactory
//
//  Copyright 2010 (����)DeNA. All rights reserved.
//	
//	@author ����
//
//	�������ܣ���
//  ����ָ�������

#ifndef NDSPTRCOUNTED_H
#define NDSPTRCOUNTED_H

#include "NDRefLong.h"

struct NDSPtrCounted
{
	NDRefLong m_kUsed;
	NDRefLong m_kWeak;

	NDSPtrCounted(NDRefLong::CountType _use, NDRefLong::CountType _weak):
	m_kUsed(_use),
	m_kWeak(_weak)
	{}
};

#endif