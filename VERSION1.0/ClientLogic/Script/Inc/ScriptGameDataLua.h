/*
 *  ScriptGameDataLua.h
 *  DragonDrive
 *
 *  Created by jhzheng on 12-1-6.
 *  Copyright 2012 (����)DeNA. All rights reserved.
 *
 *	˵����ע�����ݿ�LUA�ӿ�
 */
#pragma once
#include "ScriptInc.h"
#include <string>
using namespace std;


//lyol.strings����utf8��ʽ
std::string GetTxtPub_Common(const char* pszTableName);	//"Common_"ǰ׺
std::string GetTxtPri(const char* pszTableName);
