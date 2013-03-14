/*
 *  NDUIScrollViewContainer.h
 *  SMYS
 *
 *  Created by jhzheng on 12-2-13.
 *  Copyright 2012 (����)DeNA. All rights reserved.
 *
 */

#pragma once

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
class ContainerClientLayer;
class NDUIScrollViewContainer : public NDUIScrollContainer
{
	DECLARE_CLASS(NDUIScrollViewContainer)
	
	NDUIScrollViewContainer();
	~NDUIScrollViewContainer();
	
public:
	virtual void Initialization();

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
	void SetBottomSpeedBar(bool bBar);

    //��ȡÿ��view�Ĵ�С 
	CCSize GetViewSize();
    
    //list�ؼ������view
	virtual void AddView(CUIScrollView* pkView);
    
    //ɾ������ΪuiIndex��view
    virtual void RemoveView(unsigned int uiIndex);
    
    //ɾ��idΪuiViewId��view
    virtual void RemoveViewById(unsigned int uiViewId);
    
	//virtual void ReplaceView(unsigned int uiIndex, CUIScrollView* view);
	//virtual void ReplaceViewById(unsigned int uiViewId, CUIScrollView* view);

    //ɾ�����е�view
	virtual void RemoveAllView();
	//virtual void InsertView(unsigned int uiIndex, CUIScrollView* view);
	virtual void ShowViewByIndex(unsigned int uiIndex);
	virtual void ShowViewById(unsigned int uiViewId);
	virtual void ScrollViewByIndex(unsigned int uiIndex);
	virtual void ScrollViewById(unsigned int uiViewId);
	
	virtual CUIScrollView* GetView(unsigned int uiIndex);
	virtual CUIScrollView* GetViewById(unsigned int uiViewId);
	virtual CUIScrollView* GetBeginView();
	virtual unsigned int GetBeginIndex();

    void EnableScrollBar(bool bEnable)
    {
        NDUIScrollContainer::EnableScrollBar(bEnable);
    }
	
private:

	CC_SYNTHESIZE(float,m_fMaxScrollDistance,MaxScrollDistance); ///< ������������ @����
	CC_SYNTHESIZE(float,m_fScrollSpeed,ScrollSpeed);			 ///< �����ٶ� @����
	CC_SYNTHESIZE(float,m_fInitialVelocity,ScrollInitialVelocity); ///< ��_�L�ӗl�r�ĳ��ٶ� @����
	CC_SYNTHESIZE(float,m_fDragCoefficient,DragCoefficient);	 ///< �����S�� @����
	CC_SYNTHESIZE(float,m_fAcceleratedSpeed,AcceleratedSpeed);	 ///< �������ٶȂS�� @����
	CC_SYNTHESIZE(float,m_bOpenBounce,OpenBounce);				 ///< �Ƿ���_�L�ӗl����Ч�� @����

	float					m_fScrollDistance;
	float					m_fScrollToCenterSpeed;
	bool					m_bIsViewScrolling;
	UIScrollStyle			m_eStyle;
	ContainerClientLayer*	m_pClientUINode; // all view's parent
	CCSize					m_sizeView;
	unsigned int			m_unBeginIndex;
	bool					m_bCenterAdjust;
	bool					m_bIsBottomSpeedBar;
	bool					m_bRecaclClientEventRect;
	int						m_nBounceDir;

	CAutoLink<CUIScrollView> m_linkCurView;
	
private:
	bool CheckView(CUIScrollView* view);
	unsigned int ViewIndex(unsigned int uiViewId);
	
	void AdjustView();
	unsigned int WhichViewToScroll();
	void ScrollView(unsigned int uiIndex, bool bImmediatelySet = false);
	bool CaclViewCenter(CUIScrollView* view, float& fCenter, bool bJudeOver = false);
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
	virtual void draw();
	virtual void SetFrameRect(CCRect rect);
	// CommonProtol
	void OnScrollViewMove(NDObject* object, float fVertical, float fHorizontal); override
	void OnScrollViewScrollMoveStop(NDObject* object); override
	bool CanHorizontalMove(NDObject* object, float& hDistance); override
	bool CanVerticalMove(NDObject* object, float& vDistance); override
	
protected:
	bool CanDestroyOnRemoveAllChildren(NDNode* pNode);override
};