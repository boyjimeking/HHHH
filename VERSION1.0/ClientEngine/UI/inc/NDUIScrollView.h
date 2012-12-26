/*
 *  UIScrollView.h
 *  SMYS
 *
 *  Created by jhzheng on 12-2-13.
 *  Copyright 2012 (����)DeNA. All rights reserved.
 *
 */

#ifndef _UI_SCROLL_VIEW_ZJH_
#define _UI_SCROLL_VIEW_ZJH_

#include "NDUIScroll.h"
#include "NDUIScrollContainer.h"

using namespace NDEngine;

class CUIScrollView : public CUIScroll //don't scroll, only listen
{
	DECLARE_CLASS(CUIScrollView)
	
	CUIScrollView();
	~CUIScrollView();
	
public:
	void Initialization(bool bAccerate=false); override
	void SetScrollViewer(NDCommonProtocol* viewer);
	void SetViewId(unsigned int uiId);
	unsigned int GetViewId();
	
private:
	unsigned int				m_uiViewId;
	
protected:
	virtual bool OnHorizontalMove(float fDistance);
	virtual bool OnVerticalMove(float fDistance);
	virtual void OnMoveStop();
	DECLARE_AUTOLINK(CUIScrollView)
	INTERFACE_AUTOLINK(CUIScrollView)
};

/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
class ContainerClientLayer;
class NDUIScrollViewContainer : public NDUIScrollContainer
{
	DECLARE_CLASS(NDUIScrollViewContainer)
	
	NDUIScrollViewContainer();
	~NDUIScrollViewContainer();
	
public:
	void Initialization(); override

    //����list�ؼ�����ʽ ˮƽ���ߴ�ֱ
	void SetStyle(int style);

    //��ȡlist�ؼ�����ʽ ˮƽ���ߴ�ֱ 
	UIScrollStyle GetScrollStyle();
	
	void SetCenterAdjust(bool bSet);
	bool IsCenterAdjust();
	
    //��ȡlist�ؼ� view����
	int	GetViewCount();
    
    //����ÿ��view�Ĵ�С
	void SetViewSize(CCSize size);

    //��ȡÿ��view�Ĵ�С 
	CCSize GetViewSize();
    
    //list�ؼ������view
	void AddView(CUIScrollView* view);
    
    //ɾ������ΪuiIndex��view
    void RemoveView(unsigned int uiIndex);
    
    //ɾ��idΪuiViewId��view
    void RemoveViewById(unsigned int uiViewId);
    
	//void ReplaceView(unsigned int uiIndex, CUIScrollView* view);
	//void ReplaceViewById(unsigned int uiViewId, CUIScrollView* view);

    //ɾ�����е�view
	void RemoveAllView();
	//void InsertView(unsigned int uiIndex, CUIScrollView* view);
	void ShowViewByIndex(unsigned int uiIndex);
	void ShowViewById(unsigned int uiViewId);
	void ScrollViewByIndex(unsigned int uiIndex);
	void ScrollViewById(unsigned int uiViewId);
	
	CUIScrollView* GetView(unsigned int uiIndex);
	CUIScrollView* GetViewById(unsigned int uiViewId);
	CUIScrollView* GetBeginView();
	unsigned int GetBeginIndex();

    void EnableScrollBar(bool bEnable)
    {
        NDUIScrollContainer::EnableScrollBar(bEnable);
    }
	
private:
	float					m_fScrollDistance;
	float					m_fScrollToCenterSpeed;
	bool					m_bIsViewScrolling;
	UIScrollStyle			m_style;
	ContainerClientLayer*	m_pClientUINode; // all view's parent
	CCSize					m_sizeView;
	unsigned int			m_unBeginIndex;
	bool					m_bCenterAdjust;
	bool					m_bRecaclClientEventRect;
	CAutoLink<CUIScrollView> m_linkCurView;
	
private:
	bool CheckView(CUIScrollView* view);
	unsigned int ViewIndex(unsigned int uiViewId);
	
	void AdjustView();
	unsigned int WhichViewToScroll();
	void ScrollView(unsigned int uiIndex, bool bImmediatelySet=false);
	bool CaclViewCenter(CUIScrollView* view, float& fCenter, bool bJudeOver=false);
	CCRect GetClientRect(bool judgeOver);
	float GetContainerCenter();
	float GetViewLen();
	void StopAdjust();
	void MoveClient(float fMove);
	void refrehClientSize();
	bool IsViewScrolling();
	void EnableViewToScroll(bool bEnable);
	void SetBeginViewIndex(unsigned int nIndex);
	unsigned int GetPerPageViews();
	//bool IsViewCanCenter();
	void SetBeginIndex(unsigned int nIndex);
	
	float GetAdjustCenter();
	float GetOverDistance();
	
	void DrawScrollBar();
	
public:
	void draw(); override
	void SetFrameRect(CCRect rect); override
	// CommonProtol
	void OnScrollViewMove(NDObject* object, float fVertical, float fHorizontal); override
	void OnScrollViewScrollMoveStop(NDObject* object); override
	bool CanHorizontalMove(NDObject* object, float& hDistance); override
	bool CanVerticalMove(NDObject* object, float& vDistance); override
	
protected:
	bool CanDestroyOnRemoveAllChildren(NDNode* pNode);override
};

#endif // _UI_SCROLL_VIEW_ZJH_