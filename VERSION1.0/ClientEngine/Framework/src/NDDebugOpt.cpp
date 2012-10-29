//-------------------------------------------------------------------------
//  NDDebugOpt.cpp
//
//  Created by zhangwq on 22012-10-26.
//  Copyright 2010 (����)DeNA. All rights reserved.
//
//	���ܣ����Կ���
//-------------------------------------------------------------------------

#include "NDDebugOpt.h"

NS_NDENGINE_BGN

IMPLEMENT_CLASS(NDDebugOpt, NDObject)

#define IMP_STATIC_PROPERTY(varType,varName,varVal,clsName)	\
	varType clsName::varName = varVal;

IMP_STATIC_PROPERTY(bool,bTick,true,NDDebugOpt)
IMP_STATIC_PROPERTY(bool,bScript,true,NDDebugOpt)
IMP_STATIC_PROPERTY(bool,bNetwork,true,NDDebugOpt)

IMP_STATIC_PROPERTY(bool,bMainLoop,true,NDDebugOpt)
IMP_STATIC_PROPERTY(bool,bDrawHud,true,NDDebugOpt)
IMP_STATIC_PROPERTY(bool,bDrawUI,true,NDDebugOpt)
IMP_STATIC_PROPERTY(bool,bDrawUILabel,true,NDDebugOpt)
IMP_STATIC_PROPERTY(bool,bDrawRole,true,NDDebugOpt)
IMP_STATIC_PROPERTY(bool,bDrawMap,true,NDDebugOpt)

IMP_STATIC_PROPERTY(bool,bLightEffect,true,NDDebugOpt)

NS_NDENGINE_END