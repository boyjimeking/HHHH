/*
 *  NDUILoad.h
 *  DragonDrive
 *
 *  Created by jhzheng on 11-12-15.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "NDUINode.h"

#pragma mark 加载ui

using namespace NDEngine;

class NDUILoad : public NDObject
{
	DECLARE_CLASS(NDUILoad)
	
	bool Load(
		  const char* uiname,
		  NDUINode *parent, 
		  NDUITargetDelegate* delegate, 
		  CGSize sizeOffset = CGSizeZero);
		  
	bool LoadLua(
		  const char* uiname,
		  NDUINode *parent, 
		  LuaObject luaDelegate,
		  CGFloat sizeOffsetW = 0.0f,
		  CGFloat sizeOffsetH = 0.0f);
};