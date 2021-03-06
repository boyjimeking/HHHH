/*
 *  CPetNode.h
 *  DragonDrive
 *
 *  Created by jhzheng on 12-1-14.
 *  Copyright 2012 (网龙)DeNA. All rights reserved.
 *
 */

#pragma once

#include "NDUILayer.h"
#include "NDBaseRole.h"

using namespace NDEngine;

class CPetNode : public NDUILayer
{
	DECLARE_CLASS(CPetNode)
	
public:
	
	CPetNode();
	~CPetNode();
	
	void Initialization();
	
	void draw();
	
	void ChangePet(OBJID idPet);
	
	void SetDisplayPos(CCPoint pos); // 相对于整个屏幕
	
private:
	void refeshPosition();
	
private:
	NDBaseRole* m_role;
	CCPoint		m_pos;
	
};
