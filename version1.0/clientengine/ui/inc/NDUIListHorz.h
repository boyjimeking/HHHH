/*--------------------------------------------------------------------------
*  NDUIListHorz.h
*
*	ˮƽ�б��
*
*  Created by zhangwq on 2013.01.29
*  Copyright 2012 (����)DeNA. All rights reserved.
*--------------------------------------------------------------------------
*/

#pragma once

#include "NDUIScrollViewContainer.h"

using namespace NDEngine;


//ˮƽ�б��
class NDUIListHorz : public NDUIScrollViewContainer
{
	DECLARE_CLASS(NDUIListHorz)

public:
	NDUIListHorz();
	~NDUIListHorz();

public:
	void Initialization()
	{
		NDUIScrollViewContainer::Initialization();
	}

	void SetFrameRect(CCRect rect)
	{
		NDUIScrollViewContainer::SetFrameRect(rect);
	}

	void draw();

private:
	void debugDraw();

public:
	//list�ؼ������view
	void AddView(CUIScrollView* view)
	{
		NDUIScrollViewContainer::AddView(view);
	}

	//ɾ������ΪuiIndex��view
	void RemoveView(unsigned int uiIndex)
	{
		NDUIScrollViewContainer::RemoveView(uiIndex);
	}

	//ɾ��idΪuiViewId��view
	void RemoveViewById(unsigned int uiViewId)
	{
		NDUIScrollViewContainer::RemoveViewById(uiViewId);
	}

	//ɾ�����е�view
	void RemoveAllView()
	{
		NDUIScrollViewContainer::RemoveAllView();
	}

	CUIScrollView* GetView(unsigned int uiIndex)
	{
		return NDUIScrollViewContainer::GetView(uiIndex);
	}

	CUIScrollView* GetViewById(unsigned int uiViewId)
	{
		return NDUIScrollViewContainer::GetViewById(uiViewId);
	}
};