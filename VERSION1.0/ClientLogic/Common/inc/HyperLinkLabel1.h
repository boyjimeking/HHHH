/*
 *  HyperLinkLabel.h
 *  SMYS
 *
 *  Created by jhzheng on 12-2-23.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "NDUILabel.h"

using namespace NDEngine;

class HyperLinkLabel : public NDUILabel
{
	//DECLARE_CLASS(HyperLinkLabel)
public:
	HyperLinkLabel() 
	{
		m_isLink = false;
	}
	~HyperLinkLabel() 
	{
	}
	
	void draw();
	
	void SetIsLink(bool isLink) 
	{
		m_isLink = isLink;
	}
	
private:
	bool m_isLink;
};