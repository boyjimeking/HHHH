/*
 *  ScriptNetMsg.mm
 *  DragonDrive
 *
 *  Created by jhzheng on 12-1-6.
 *  Copyright 2012 (����)DeNA. All rights reserved.
 *
 */
 
#include "ScriptNetMsg.h"
#include "ScriptInc.h"
#include "NDTransData.h"
#include "NDUtility.h"
#include <map>
#include "ScriptMgr.h"

using namespace NDEngine;
using namespace LuaPlus;

std::map<MSGID, LuaObject> mapNetMsgHandler;

// luaע��������Ϣʹ��ע������:
// lua�ص�����ԭ��(LuaCallBack) : bool (NDTransData*);
// luaע�ắ��ԭ��: bool RegisterNetMsgHandler(MSGID, char*, LuaCallBack)

int RegisterNetMsgHandler(LuaState* pkLuaState)
{
	LuaStack args(pkLuaState);
	LuaObject loId = args[1];
	LuaObject loStr = args[2];
	LuaObject loLuaFunc = args[3];
	
	if (!loId.IsInteger()	||
		!loStr.IsString()	||
		!loLuaFunc.IsFunction())
	{
		pkLuaState->PushBoolean(false);
		
		return 1;
	}
	
	ScriptMgrObj.DebugOutPut("RegisterNetMsgHandler : [%d][%s]", 
							 loId.GetInteger() ,loStr.GetString());
	
	std::map<MSGID, LuaObject>::iterator 
	it = mapNetMsgHandler.find(loId.GetNumber());
	
	if (it != mapNetMsgHandler.end())
	{
		NDAsssert(0);
	
		pkLuaState->PushBoolean(false);
		
		return 1;
	}

	if (loId.GetNumber() == 2028)
	{
		int a = 10;
	}
	
	mapNetMsgHandler.insert(
	std::pair<MSGID, LuaObject>(
	loId.GetNumber(), loLuaFunc) );
	
	pkLuaState->PushBoolean(true);
	
	return 1;
}

void SendMsg(NDTransData* pkData)
{
	printf("\nqxx++++++MSG_TYPE=[%d]\n",pkData->GetMsgType());
	printf("\nqxx++++++MSG_SIZE=[%d]\n",pkData->GetSize());
	if (!pkData)
	{
		return;
	}
	SEND_DATA(*pkData);
}

void ScriptNetMsg::Load()
{
	ETLUAFUNC("RegisterNetMsgHandler", RegisterNetMsgHandler)
	ETCFUNC("SendMsg", SendMsg)
}

bool ScriptNetMsg::Process(MSGID msgID, NDTransData* pkData)
{
	std::map<MSGID, LuaObject>::iterator 
	itFun = mapNetMsgHandler.find(msgID);
	
	if (itFun == mapNetMsgHandler.end()) 
		return false;
	
	LuaObject& fun = itFun->second;
	if (!fun.IsFunction()) 
		return true;
	
	LuaFunction<int> luaFunc(fun);
	
	/*int nRet = */luaFunc(pkData);
	
	return true;
}
