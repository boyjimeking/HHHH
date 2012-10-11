/*
 *  RequestListScene.h
 *  DragonDrive
 *
 *  Created by jhzheng on 11-8-28.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _REQUEST_LIST_SCENE_H_
#define _REQUEST_LIST_SCENE_H_

#include "NDCommonScene.h"
#include "GameUIRequest.h"

class RequestListScene :
public NDCommonScene
{
	DECLARE_CLASS(RequestListScene)
	
public:	
	RequestListScene();
	
	~RequestListScene();
	
	static RequestListScene* Scene();
	
	void Initialization(); override
	
	void OnButtonClick(NDUIButton* button); override
	
private:
	void OnTabLayerSelect(TabLayer* tab, unsigned int lastIndex, unsigned int curIndex); override
	
private:
	NDFuncTab* m_tabFunc;
	
	NewGameUIRequest *m_request;
	
	NDUIButton *m_btnClear;
	
private:
	void InitRequestList(NDUIClientLayer* client);
	
	void InitHelp(NDUIClientLayer* client);
};

#endif // _REQUEST_LIST_SCENE_H_