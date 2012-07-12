/*
 *  UIItemButton.h
 *  SMYS
 *
 *  Created by jhzheng on 12-3-5.
 *  Copyright 2012 (����)DeNA. All rights reserved.
 *
 */

#ifndef _UI_ITEM_BUTTON_H_ZJH_
#define _UI_ITEM_BUTTON_H_ZJH_

#include "NDUIButton.h"

using namespace NDEngine;

class CUIItemButton :
public NDUIButton
{
	DECLARE_CLASS(CUIItemButton)
	CUIItemButton();
	~CUIItemButton();
	
public:
	void SetLock(bool bSet);
	bool IsLock();

	void ChangeItem(unsigned int unItemId);
	unsigned int GetItemId();
	
	void ChangeItemType(unsigned int unItemType);
	unsigned int GetItemType();
	
	void RefreshItemCount();
	
	void SetShowAdapt(bool bShowAdapt);
	bool IsShowAdapt();
	
private:
	unsigned int			m_unItemId;
	unsigned int			m_unItemType;
	bool					m_bLock;
	bool					m_bShowAdapt;
	
private:
	void ChangeItemCount(unsigned int unItemCount);
	
protected:
	void draw(); override
};


#endif // _UI_ITEM_BUTTON_H_ZJH_