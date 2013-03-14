/*
 *  NDUIScrollViewContainer.cpp
 *  SMYS
 *
 *  Created by jhzheng on 12-2-13.
 *  Copyright 2012 (����)DeNA. All rights reserved.
 *
 */

#include "NDUIScrollViewContainer.h"
#include "ScriptUI.h"
#include "CCPointExtension.h"
#include "NDDirector.h"
#include "NDUtil.h"
#include "ScriptGameLogic.h"
#include "NDBaseScriptMgr.h"
#include "ObjectTracker.h"

IMPLEMENT_CLASS(CUIScrollView, CUIScroll)

CUIScrollView::CUIScrollView()
{
	INC_NDOBJ_RTCLS
	INIT_AUTOLINK(CUIScrollView);
	m_uiViewId = 0;
}

CUIScrollView::~CUIScrollView()
{
	DEC_NDOBJ_RTCLS
}

void CUIScrollView::Initialization(bool bAccerate/*=false*/)
{
	CUIScroll::Initialization(bAccerate);
}

void CUIScrollView::SetScrollViewer(NDCommonProtocol* viewer)
{
	AddViewer(viewer);
}

void CUIScrollView::SetViewId(unsigned int uiId)
{
	m_uiViewId = uiId;
}

unsigned int CUIScrollView::GetViewId()
{
	return m_uiViewId;
}

bool CUIScrollView::OnHorizontalMove(float fDistance)
{
	LIST_COMMON_VIEWER_IT it = m_listCommonViewer.begin();
	for (; it != m_listCommonViewer.end(); it++)
	{
		if (!(*it).IsValid())
		{
			continue;
		}

		(*it)->OnScrollViewMove(this, 0.0f, fDistance);
	}
	return false;
}

bool CUIScrollView::OnVerticalMove(float fDistance)
{
	LIST_COMMON_VIEWER_IT it = m_listCommonViewer.begin();
	for (; it != m_listCommonViewer.end(); it++)
	{
		if (!(*it).IsValid())
		{
			continue;
		}

		(*it)->OnScrollViewMove(this, fDistance, 0.0f);
	}
	return false;
}

void CUIScrollView::OnMoveStop()
{
	LIST_COMMON_VIEWER_IT it = m_listCommonViewer.begin();
	for (; it != m_listCommonViewer.end(); it++)
	{
		LIST_COMMON_VIEWER_IT it1 = m_listCommonViewer.begin();

		if (!(*it).IsValid())
		{
			continue;
		}

		(*it1)->OnScrollViewScrollMoveStop(this);
	}
}

/////////////////////////////////////////////////////////

class ContainerClientLayer: public NDUILayer
{
	DECLARE_CLASS (ContainerClientLayer)

	ContainerClientLayer()
	{
		m_kEventRect = CCRectZero;
	}

public:
	void SetEventRect(CCRect rect)
	{
		m_kEventRect = rect;
	}

private:
	CCRect m_kEventRect;

private:
	bool CanDealEvent(CCPoint pos)
	{
		return cocos2d::CCRect::CCRectContainsPoint(m_kEventRect, pos);
	}

protected:
	virtual bool TouchBegin(NDTouch* touch)
	{
		if (!this->IsVisibled())
		{
			return false;
		}

		if (CanDealEvent(touch->GetLocation()))
		{
			return NDUILayer::TouchBegin(touch);
		}
		return false;
	}
	virtual bool DispatchLongTouchClickEvent(CCPoint beginTouch,
			CCPoint endTouch)
	{
		if (CanDealEvent(endTouch))
		{
			return NDUILayer::DispatchLongTouchClickEvent(beginTouch, endTouch);
		}
		return false;
	}

	virtual bool DispatchLongTouchEvent(CCPoint beginTouch, bool touch)
	{
		if (CanDealEvent(beginTouch))
		{
			return NDUILayer::DispatchLongTouchEvent(beginTouch, touch);
		}
		return false;
	}

	virtual bool DispatchDragOutEvent(CCPoint beginTouch, CCPoint moveTouch,
			bool longTouch = false)
	{
		if (CanDealEvent(moveTouch))
		{
			return NDUILayer::DispatchDragOutEvent(beginTouch, moveTouch,
					longTouch);
		}
		return false;
	}

	virtual bool DispatchDragInEvent(NDUINode* dragOutNode, CCPoint beginTouch,
			CCPoint endTouch, bool longTouch, bool dealByDefault = false)
	{
		if (CanDealEvent(endTouch))
		{
			return NDUILayer::DispatchDragInEvent(dragOutNode, beginTouch,
					endTouch, longTouch, dealByDefault);
		}
		return false;
	}
};

IMPLEMENT_CLASS(ContainerClientLayer, NDUILayer)

/////////////////////////////////////////////////////////

IMPLEMENT_CLASS(NDUIScrollViewContainer, NDUIScrollContainer)

NDUIScrollViewContainer::NDUIScrollViewContainer()
{
	INC_NDOBJ_RTCLS
	m_fScrollDistance = 0.0f;
	m_fScrollToCenterSpeed = 50.0f;
	m_bIsViewScrolling = false;
	m_eStyle = UIScrollStyleHorzontal;
	m_pClientUINode = NULL;
	m_sizeView = CCSizeMake(0, 0);
	m_unBeginIndex = 0;
	m_fMaxScrollDistance = 0.0f;
	m_bCenterAdjust = false;
	m_bRecaclClientEventRect = false;
	m_fScrollSpeed = 0.0f;
	m_bIsBottomSpeedBar = false;
	m_nBounceDir = 0;
	m_fDragCoefficient = 0.3f;
	m_fAcceleratedSpeed = 2.0f;
	m_fInitialVelocity = 0.0f;
	m_bOpenBounce = true;
}

NDUIScrollViewContainer::~NDUIScrollViewContainer()
{
	DEC_NDOBJ_RTCLS SAFE_DELETE_NODE(m_pClientUINode);
}

void NDUIScrollViewContainer::Initialization()
{
	NDUIScrollContainer::Initialization();

	m_pClientUINode = new ContainerClientLayer;
	m_pClientUINode->Initialization();
	m_pClientUINode->setDebugName("ContainerClientLayer");

	AddChild(m_pClientUINode);
}

void NDUIScrollViewContainer::SetStyle(int style)
{
	if (style < UIScrollStyleBegin || style >= UIScrollStyleEnd)
	{
		return;
	}
	m_eStyle = (UIScrollStyle) style;
}

UIScrollStyle NDUIScrollViewContainer::GetScrollStyle()
{
	return m_eStyle;
}

void NDUIScrollViewContainer::SetCenterAdjust(bool bSet)
{
	m_bCenterAdjust = bSet;
}

bool NDUIScrollViewContainer::IsCenterAdjust()
{
	return m_bCenterAdjust;
}

void NDUIScrollViewContainer::SetViewSize(CCSize size)
{
	m_sizeView = size;
}

int NDUIScrollViewContainer::GetViewCount()
{
	if (!m_pClientUINode)
	{
		return 0;
	}
	return m_pClientUINode->GetChildren().size();
}

CCSize NDUIScrollViewContainer::GetViewSize()
{
	return m_sizeView;
}

void NDUIScrollViewContainer::AddView(CUIScrollView* pkView)
{
	if (!CheckView(pkView))
	{
		return;
	}

	if (!m_pClientUINode)
	{
		return;
	}

	size_t kChildSize = m_pClientUINode->GetChildren().size();
	CCRect kRect = pkView->GetFrameRect();
	if (UIScrollStyleHorzontal == GetScrollStyle())
	{
		kRect.origin.x = kChildSize * GetViewLen();
		kRect.origin.y = 0;
	}
	else
	{
		kRect.origin.y = kChildSize * GetViewLen();
		kRect.origin.x = 0;
	}
	kRect.size = m_sizeView;
	pkView->SetFrameRect(kRect);
	pkView->SetScrollStyle(GetScrollStyle());
	pkView->SetMovableViewer(this);
	pkView->SetScrollViewer(this);
	pkView->SetContainer(this);
	m_pClientUINode->AddChild(pkView);

	refrehClientSize();
}

void NDUIScrollViewContainer::RemoveView(unsigned int uiIndex)
{
	if (!m_pClientUINode)
	{
		return;
	}

	size_t uiChildSize = m_pClientUINode->GetChildren().size();
	if (uiIndex >= uiChildSize)
	{
		return;
	}

	unsigned int nBeginIndex = 0;
	if (uiIndex >= uiChildSize - 1)
	{
		//���һ��view
		nBeginIndex = uiChildSize <= 1 ? 0 : uiChildSize - 2;
	}
	else
	{
		nBeginIndex = uiIndex;
	}

	m_pClientUINode->RemoveChild(m_pClientUINode->GetChildren()[uiIndex], true);

	const std::vector<NDNode*>& kChildren = m_pClientUINode->GetChildren();
	uiChildSize = m_pClientUINode->GetChildren().size();

	if (uiIndex < uiChildSize)
	{
		for (size_t i = uiIndex; i < uiChildSize; i++)
		{
			NDNode *child = kChildren[i];
			if (!child || !child->IsKindOfClass(RUNTIME_CLASS(CUIScrollView)))
			{
				continue;
			}
			CUIScrollView* pkView = (CUIScrollView*) child;
			CCRect kRect = pkView->GetFrameRect();
			if (UIScrollStyleHorzontal == GetScrollStyle())
			{
				kRect.origin.x = i * GetViewLen();
			}
			else
			{
				kRect.origin.y = i * GetViewLen();
			}
			pkView->SetFrameRect(kRect);
		}
	}

	refrehClientSize();

	if (0 != uiChildSize)
	{
		ScrollView(nBeginIndex);
	}
}

void NDUIScrollViewContainer::RemoveViewById(unsigned int uiViewId)
{
	size_t stFindIndex = ViewIndex(uiViewId);

	if ((size_t) - 1 == stFindIndex)
	{
		return;
	}

	RemoveView(stFindIndex);
}

void NDUIScrollViewContainer::RemoveAllView()
{
	while (m_pClientUINode && m_pClientUINode->GetChildren().size() > size_t(0))
	{
		RemoveView(m_pClientUINode->GetChildren().size() - 1);
	}

	if (m_pClientUINode)
	{
		m_pClientUINode->SetFrameRect(CCRectZero);
	}
	EnableViewToScroll(false);
	m_fScrollDistance = 0.0f;
}

void NDUIScrollViewContainer::ShowViewByIndex(unsigned int uiIndex)
{
	CUIScrollView* pkView = GetView(uiIndex);
	if (!pkView)
	{
		return;
	}

	float fCenter = 0.0f;
	if (!CaclViewCenter(pkView, fCenter))
	{
		return;
	}

	if (!m_pClientUINode)
	{
		return;
	}

	StopAdjust();

	ScrollView(uiIndex, true);
}

void NDUIScrollViewContainer::ShowViewById(unsigned int uiViewId)
{
	size_t findIndex = ViewIndex(uiViewId);
	if ((size_t) - 1 != findIndex)
	{
		ShowViewByIndex(findIndex);
	}
}

void NDUIScrollViewContainer::ScrollViewByIndex(unsigned int uiIndex)
{
	CUIScrollView* pkView = GetView(uiIndex);
	if (!pkView)
	{
		return;
	}
	if (pkView == m_linkCurView)
	{
		return;
	}

	float fCenter = 0.0f;
	if (!CaclViewCenter(pkView, fCenter))
	{
		return;
	}

	if (!m_pClientUINode)
	{
		return;
	}

	StopAdjust();

	ScrollView(uiIndex);
}

void NDUIScrollViewContainer::ScrollViewById(unsigned int uiViewId)
{
	size_t uiFindIndex = ViewIndex(uiViewId);
	if ((size_t) - 1 != uiFindIndex)
	{
		ScrollViewByIndex(uiFindIndex);
	}
}

CUIScrollView* NDUIScrollViewContainer::GetView(unsigned int uiIndex)
{
	if ((size_t) - 1 == uiIndex)
	{
		return NULL;
	}

	if (!m_pClientUINode)
	{
		return NULL;
	}

	const std::vector<NDNode*>& kChild = m_pClientUINode->GetChildren();
	size_t uiChildSize = kChild.size();

	if (uiIndex >= uiChildSize)
	{
		return NULL;
	}

	NDNode* pkChildNode = kChild[uiIndex];
	if (!pkChildNode || !pkChildNode->IsKindOfClass(RUNTIME_CLASS(CUIScrollView)))
	{
		return NULL;
	}

	return (CUIScrollView*) pkChildNode;
}

CUIScrollView* NDUIScrollViewContainer::GetViewById(unsigned int uiViewId)
{
	size_t findIndex = ViewIndex(uiViewId);
	return GetView(findIndex);
}

bool NDUIScrollViewContainer::CheckView(CUIScrollView* view)
{
	if (!view)
	{
		return false;
	}
	return true;
}

unsigned int NDUIScrollViewContainer::ViewIndex(unsigned int uiViewId)
{
	size_t uiFindIndex = -1;
	if (!m_pClientUINode)
	{
		return uiFindIndex;
	}

	const std::vector<NDNode*>& children = m_pClientUINode->GetChildren();
	size_t childsize = children.size();

	for (size_t i = 0; i < childsize; i++)
	{
		NDNode* pkChild = children[i];
		if (!pkChild || !pkChild->IsKindOfClass(RUNTIME_CLASS(CUIScrollView)))
		{
			continue;
		}
		CUIScrollView* pkView = (CUIScrollView*) pkChild;

		if (pkView->GetViewId() == uiViewId)
		{
			uiFindIndex = i;
			break;
		}
	}

	return uiFindIndex;
}

void NDUIScrollViewContainer::AdjustView()
{
	StopAdjust();

	if (!m_pClientUINode)
	{
		return;
	}

	unsigned int uiFindInex = WhichViewToScroll();
	if ((size_t) - 1 == uiFindInex)
	{
		return;
	}

	ScrollView(uiFindInex);
}

unsigned int NDUIScrollViewContainer::WhichViewToScroll()
{
	unsigned int uiIndexFind = -1;

	if (!m_pClientUINode)
	{
		return uiIndexFind;
	}

	float fCenter = 0.0f;
	fCenter = GetAdjustCenter();

	const std::vector<NDNode*>& kChildren = m_pClientUINode->GetChildren();
	size_t kSize = kChildren.size();
	float fMin = 1000.0f;

	if (UIScrollStyleHorzontal == GetScrollStyle())
	{
		//�@ȡ��ǰ�Ƅӵľ��x
		NDNode* pkChild = kChildren[0];
		if (!pkChild || !pkChild->IsKindOfClass(RUNTIME_CLASS(CUIScrollView)))
		{
			return 0;
		}
		CUIScrollView* pkViewScrollView = (CUIScrollView*) pkChild;
		CCRect kClientRect = GetClientRect(true);
		CCRect kViewRect = pkViewScrollView->GetFrameRect();
		int iCurShowIndex = GetBeginIndex();

		int iCurMoveDis = kClientRect.origin.x
				- (-iCurShowIndex * kViewRect.size.width);

		//�@ȡ���ƄӾ��x���С�0�����N�����Ƅӣ���������
		if (iCurMoveDis < 0)
		{
			if (abs(iCurMoveDis) > kViewRect.size.width / 8)
			{
				/***
				* ���@�e����iCurshowIndex��0Ӌ�㲻���Ć��}
				* ����
				*/
				int iMoveNum = abs(iCurMoveDis) / kViewRect.size.width + 1;

				/************************************************************************/
				/* �@�e���ˆ��}��Ӌ��ĕr����һ�����ԓ�p1							*/
				/* 																		*/
				/*                                       				   ���� ����	*/
				/************************************************************************/

				if (m_bIsBottomSpeedBar)
				{
					iCurShowIndex =
						iCurShowIndex + iMoveNum > kSize - 1 ?
						kSize - 1 : iCurShowIndex + iMoveNum - 1;
				}
				else
				{
					iCurShowIndex =
						iCurShowIndex + iMoveNum > kSize - 1 ?
						kSize - 1 : iCurShowIndex + iMoveNum;
				}
			}
		}
		//�@ȡ���ƄӾ��x������0�����N�����Ƅӣ������p��
		else if (iCurMoveDis > 0)
		{
			if (iCurMoveDis > kViewRect.size.width / 8)
			{
				//iCurShowIndex = iCurShowIndex - 1 < 0 ? iCurShowIndex : iCurShowIndex - 1;
				int iMoveNum = abs(iCurMoveDis) / kViewRect.size.width + 1;
				iCurShowIndex =
						iCurShowIndex - iMoveNum < 0 ?
								0 : iCurShowIndex - iMoveNum;
			}
		}

		return iCurShowIndex;
	}
	else
	{
		for (size_t i = 0; i < kSize; i++)
		{
			NDNode* pkChildNode = kChildren[i];
			if (!pkChildNode
					|| !pkChildNode->IsKindOfClass(
							RUNTIME_CLASS(CUIScrollView)))
			{
				continue;
			}

			CUIScrollView* pkView = (CUIScrollView*) pkChildNode;
			float fViewCenter = 0.0f;
			if (!CaclViewCenter(pkView, fViewCenter, true))
			{
				continue;
			}

			float fTempViewCenter = fViewCenter - fCenter;
			if (fTempViewCenter < 0.0f)
			{
				fTempViewCenter = -fTempViewCenter;
			}

			if (fTempViewCenter < fMin)
			{
				fMin = fTempViewCenter;
				uiIndexFind = i;
			}
		}

		return uiIndexFind;
	}
}

// cacl view center and container center dis; m_fScrollDistance = dis; m_bIsViewScrolling = true;
void NDUIScrollViewContainer::ScrollView(unsigned int uiIndex,
		bool bImmediatelySet/*=false*/)
{
	if ((size_t)-1 == uiIndex || !m_pClientUINode)
	{
		return;
	}

	const std::vector<NDNode*>& kChildren = m_pClientUINode->GetChildren();
	if (uiIndex >= kChildren.size())
	{
		return;
	}

	NDNode* pkChildNode = kChildren[uiIndex];

	if (!pkChildNode
			|| !pkChildNode->IsKindOfClass(RUNTIME_CLASS(CUIScrollView)))
	{
		return;
	}

	CUIScrollView* pkView = (CUIScrollView*)pkChildNode;

	float fViewCenter = 0.0f;
	if (!CaclViewCenter(pkView, fViewCenter))
	{
		return;
	}

	float fCenter = 0.0f;

	fCenter = GetAdjustCenter();

	float fDistance = fCenter - fViewCenter;

	if (UIScrollStyleHorzontal == GetScrollStyle())
	{
		if (!CanHorizontalMove(pkView, fDistance))
		{
			SetBeginIndex(uiIndex);
			SetBeginViewIndex(uiIndex);
			return;
		}
	}
	else
	{
		if (!CanVerticalMove(pkView, fDistance))
		{
			SetBeginIndex(uiIndex);
			SetBeginViewIndex(uiIndex);
			return;
		}
	}

	SetBeginIndex(uiIndex);

	if (bImmediatelySet)
	{
		MoveClient(fDistance);

		SetBeginViewIndex(uiIndex);
	}
	else
	{
		m_fScrollDistance = fDistance;
		m_fMaxScrollDistance = abs(fDistance);
		m_fScrollSpeed = m_fInitialVelocity;

		EnableViewToScroll(true);
	}

	return;
}

bool NDUIScrollViewContainer::CaclViewCenter(CUIScrollView* view,
		float& fCenter, bool bJudeOver/*=false*/)
{
	if (!view || !m_pClientUINode)
	{
		return false;
	}

	CCRect kClientRect = GetClientRect(bJudeOver);
	CCRect kViewRect = view->GetFrameRect();
	fCenter = 0.0f;

	if (UIScrollStyleHorzontal == GetScrollStyle())
	{
		fCenter = kClientRect.origin.x + kViewRect.origin.x
				+ kViewRect.size.width / 2;
	}
	else
	{
		fCenter = kClientRect.origin.y + kViewRect.origin.y
				+ kViewRect.size.height / 2;
	}

	return true;
}

CCRect NDUIScrollViewContainer::GetClientRect(bool judgeOver)
{
	if (!m_pClientUINode)
	{
		return CCRectZero;
	}

	CCRect kClientRect = m_pClientUINode->GetFrameRect();

	if (!judgeOver)
	{
		return kClientRect;
	}

	if (IsCenterAdjust())
	{
		return kClientRect;
	}

	CCRect kSelfRect = this->GetFrameRect();

	if (UIScrollStyleHorzontal == GetScrollStyle())
	{
		if (kClientRect.origin.x > 0.0f)
		{
			kClientRect.origin.x = 0.0f;
		}
		else if (kClientRect.origin.x + kClientRect.size.width
				< kSelfRect.size.width)
		{
			kClientRect.origin.x = kSelfRect.size.width - kClientRect.size.width;
		}
	}
	else
	{
		if (kClientRect.origin.y > 0.0f)
		{
			kClientRect.origin.y = 0.0f;
		}
		else if (kClientRect.origin.y + kClientRect.size.height
				< kSelfRect.size.height)
		{
			kClientRect.origin.y = kSelfRect.size.height - kClientRect.size.height;
		}
	}

	return kClientRect;
}

float NDUIScrollViewContainer::GetViewLen()
{
	float fres = 0.0f;

	if (UIScrollStyleHorzontal == GetScrollStyle())
	{
		//fres		= this->GetFrameRect().size.width;
		fres = m_sizeView.width;
	}
	else
	{
		//fres		= this->GetFrameRect().size.height;
		fres = m_sizeView.height;
	}

	return fres;
}

float NDUIScrollViewContainer::GetContainerCenter()
{
	CCRect rect = this->GetFrameRect();
	float fCenter = 0.0f;

	if (UIScrollStyleHorzontal == GetScrollStyle())
	{
		fCenter = rect.size.width / 2;
	}
	else
	{
		fCenter = rect.size.height / 2;
	}

	return fCenter;
}

void NDUIScrollViewContainer::StopAdjust()
{
	EnableViewToScroll(false);
	//m_bIsViewScrolling		= false;
	m_fScrollDistance = 0.0f;
}

void NDUIScrollViewContainer::MoveClient(float fMove)
{
	if (!m_pClientUINode)
	{
		return;
	}

	CCRect kRect = m_pClientUINode->GetFrameRect();

	if (m_eStyle == UIScrollStyleHorzontal)
	{
		kRect.origin.x += fMove;
	}
	else
	{
		kRect.origin.y += fMove;
	}

	m_pClientUINode->SetFrameRect(kRect);
}

void NDUIScrollViewContainer::refrehClientSize()
{
	if (!m_pClientUINode)
	{
		return;
	}

	CCRect kRect = m_pClientUINode->GetFrameRect();

	if (m_eStyle == UIScrollStyleHorzontal)
	{
		kRect.size.width = m_pClientUINode->GetChildren().size() * GetViewLen();
		kRect.size.height = GetFrameRect().size.height;
	}
	else
	{
		kRect.size.height = m_pClientUINode->GetChildren().size() * GetViewLen();
		kRect.size.width = GetFrameRect().size.width;
	}

	if (0 == m_pClientUINode->GetChildren().size())
	{
		kRect.origin = ccp(0, 0);
	}

	m_pClientUINode->SetFrameRect(kRect);
}

void NDUIScrollViewContainer::draw() //  m_fScrollDistance += speed; only to zero;  clientnode speed;
{
	if (m_bRecaclClientEventRect)
	{
		if (m_pClientUINode)
		{
			m_pClientUINode->SetEventRect(GetScreenRect());
		}
		m_bRecaclClientEventRect = false;
	}

	if (!IsVisibled())
	{
		return;
	}

	if (!IsViewScrolling() || !m_pClientUINode)
	{
		NDUIScrollContainer::draw();
		DrawScrollBar();
		return;
	}

	float fMove = 0.0f;

	if (0 != m_nBounceDir)
	{
		m_fScrollSpeed *= m_fDragCoefficient;
		
		if (1 == m_nBounceDir)
		{
			fMove = m_fScrollSpeed;
		}
		else
		{
			fMove = -m_fScrollSpeed;
		}

		m_fScrollDistance -= fMove;

		if (m_fScrollSpeed <= 1.0f)
		{
			m_fScrollSpeed = 0.0f;
			m_nBounceDir = 0;
		}
	}
	else
	{
		do 
		{
			if (m_fScrollDistance > 0.0f)
			{
				if (m_fScrollDistance < m_fScrollSpeed)
				{
					fMove = m_fScrollDistance;

					if (m_fMaxScrollDistance * m_fDragCoefficient > 10.0f && m_bOpenBounce)
					{
						m_fMaxScrollDistance *= m_fDragCoefficient;
						m_nBounceDir = 1;
						m_fScrollDistance = 0.0f;
						break;
					}

					m_fScrollDistance = 0.0f;
					EnableViewToScroll(false);
				}
				else
				{
					fMove = m_fScrollSpeed;

					if (fMove >= m_fScrollToCenterSpeed)
					{
						fMove = m_fScrollToCenterSpeed;
					}

					m_fScrollDistance = m_fScrollDistance - fMove;
				}

				m_fScrollSpeed += m_fAcceleratedSpeed;
			}
			else if (m_fScrollDistance < 0.0f)
			{
				if (m_fScrollDistance > -m_fScrollSpeed)
				{
					fMove = m_fScrollDistance;

					if (m_fMaxScrollDistance * m_fDragCoefficient > 10.0f && m_bOpenBounce)
					{
						m_fMaxScrollDistance *= m_fDragCoefficient;
						m_nBounceDir = 2;
						m_fScrollDistance = 0.0f;
						break;
					}

					m_fScrollDistance = 0.0f;
					EnableViewToScroll(false);
				}
				else
				{
					fMove = -m_fScrollSpeed;

					m_fScrollDistance = m_fScrollDistance - fMove;
				}

				m_fScrollSpeed += m_fAcceleratedSpeed;
			}
			else
			{
				EnableViewToScroll(false);
			}
		} while (false);
	}

	MoveClient(fMove);

	NDUIScrollContainer::draw();

	DrawScrollBar();

	if (!IsViewScrolling())
	{
		SetBeginViewIndex (GetBeginIndex());}
	}

void NDUIScrollViewContainer::SetFrameRect(CCRect rect)
{
	NDUIScrollContainer::SetFrameRect(rect);

	m_bRecaclClientEventRect = true;
}

// clientnode  can move.if can then, move clientnode. m_bIsViewScrolling = false;
void NDUIScrollViewContainer::OnScrollViewMove(NDObject* object,
		float fVertical, float fHorizontal)
{
	if (!object || !object->IsKindOfClass(RUNTIME_CLASS(CUIScrollView)))
	{
		return;
	}

	if (!m_pClientUINode)
	{
		return;
	}

	if (UIScrollStyleHorzontal == GetScrollStyle())
	{
		MoveClient(fHorizontal);
	}
	else
	{
		MoveClient(fVertical);
	}

	EnableViewToScroll(false);
	//m_bIsViewScrolling	= false;
	m_fScrollDistance = 0.0f;

	return;
}

// when call back, just call AdjustView
void NDUIScrollViewContainer::OnScrollViewScrollMoveStop(NDObject* object)
{
	if (!object || !object->IsKindOfClass(RUNTIME_CLASS(CUIScrollView)))
	{
		return;
	}

	if (m_pClientUINode)
	{
		const std::vector<NDNode*>& children = m_pClientUINode->GetChildren();
		size_t size = children.size();
		for (size_t i = 0; i < size; i++)
		{
			NDNode *child = children[i];
			if (!child || !child->IsKindOfClass(RUNTIME_CLASS(CUIScrollView)))
			{
				continue;
			}

			CUIScrollView* pkView = (CUIScrollView*) child;
			pkView->StopAccerate();
		}
	}

	BaseScriptMgrObj.excuteLuaFunc("PlayEffectSound", "Music", 15);

	AdjustView();
}

bool NDUIScrollViewContainer::CanHorizontalMove(NDObject* object,
		float& hDistance)
{
	if (!object || !object->IsKindOfClass(RUNTIME_CLASS(CUIMovableLayer)))
	{
		return false;
	}

	CUIMovableLayer *layer = (CUIMovableLayer*) object;

	if (!layer || !m_pClientUINode)
	{
		return false;
	}

	CCRect rectself = this->GetFrameRect();
	CCRect rectmove = m_pClientUINode->GetFrameRect();

	CGFloat fOver = GetOverDistance();

	if (hDistance > 0.0f)
	{ // ����
		if ((rectself.size.width - m_uiRightDistance + fOver)
				< (rectmove.origin.x + hDistance))
		{
			return false;
		}
	}
	else if (hDistance < 0.0f)
	{ // ����
		if ((m_uiLeftDistance - fOver)
				> (rectmove.origin.x + rectmove.size.width + hDistance))
		{
			if (rectmove.origin.x + hDistance > -0.01f)
			{
				return true;
			}

			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}

bool NDUIScrollViewContainer::CanVerticalMove(NDObject* object,
		float& vDistance)
{
	if (!object || !object->IsKindOfClass(RUNTIME_CLASS(CUIMovableLayer)))
	{
		return false;
	}

	CUIMovableLayer *layer = (CUIMovableLayer*) object;

	if (!layer || !m_pClientUINode)
	{
		return false;
	}

	CCRect rectself = this->GetFrameRect();
	CCRect rectmove = m_pClientUINode->GetFrameRect();

	CGFloat fOver = GetOverDistance();

	if (vDistance > 0.0f)
	{ // ����
		if ((rectself.size.height - m_uiBottomDistance + fOver)
				< (rectmove.origin.y + vDistance))
		{
			return false;
		}
	}
	else if (vDistance < 0.0f)
	{ // ����
		if ((m_uiTopDistance - fOver)
				> (rectmove.origin.y + rectmove.size.height + vDistance))
		{
			if ((rectmove.origin.y + vDistance > -0.01f))
			{
				return true;
			}

			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}

bool NDUIScrollViewContainer::IsViewScrolling()
{
	return m_bIsViewScrolling;
}

void NDUIScrollViewContainer::EnableViewToScroll(bool bEnable)
{
	m_bIsViewScrolling = bEnable;
}

void NDUIScrollViewContainer::SetBeginViewIndex(unsigned int nIndex)
{
	CUIScrollView* pkView = GetView(nIndex);
	if (pkView && pkView != m_linkCurView)
	{
		m_linkCurView = pkView->QueryLink();
		OnScriptUiEvent(this, TE_TOUCH_SC_VIEW_IN_BEGIN, nIndex);
	}

	SetBeginIndex(nIndex);
}

void NDUIScrollViewContainer::SetBeginIndex(unsigned int nIndex)
{
	m_unBeginIndex = nIndex;
}

CUIScrollView* NDUIScrollViewContainer::GetBeginView()
{
	return GetView(GetBeginIndex());
}

unsigned int NDUIScrollViewContainer::GetBeginIndex()
{
	return m_unBeginIndex;
}

float NDUIScrollViewContainer::GetAdjustCenter()
{
	if (!IsCenterAdjust())
	{
		return GetViewLen() / 2;
	}

	return this->GetContainerCenter();
}

float NDUIScrollViewContainer::GetOverDistance()
{
	if (!m_pClientUINode)
	{
		return 0.0f;
	}

	CCRect kSelfRect = this->GetFrameRect();
	CCRect kMoveRect = m_pClientUINode->GetFrameRect();

	CGFloat fOver = 0.0f;
	if (IsCenterAdjust())
	{
		if (kSelfRect.size.height / 2 < kMoveRect.size.height)
		{
			fOver = kSelfRect.size.height * 0.333;
		}
	}
	else
	{
		fOver = GetViewLen() * 0.333;
	}

	return fOver;
}

void NDUIScrollViewContainer::DrawScrollBar()
{
	if (m_bOpenScrollBar && m_picScroll && m_pClientUINode
			&& GetScrollStyle()
					== UIScrollStyleVerical /*&& 
					 m_pClientUINode->GetFrameRect().size.height > this->GetFrameRect().size.height*/)
	{
		bool bTouchDown = false;
		if (m_pClientUINode)
		{
			const std::vector<NDNode*>& kChildren =
					m_pClientUINode->GetChildren();
			size_t size = kChildren.size();
			for (size_t i = 0; i < size; i++)
			{
				NDNode* pkChild = kChildren[i];
				if (!pkChild
						|| !pkChild->IsKindOfClass(RUNTIME_CLASS(CUIScrollView)))
				{
					continue;
				}

				CUIScrollView* pkView = (CUIScrollView*) pkChild;
				if (pkView->IsTouchDown())
				{
					bTouchDown = true;
					break;
				}
			}
		}
		if (!bTouchDown)
		{
			//return;
		}
		CCRect kSelfRect = GetSrcRectCache();
		//rectself.origin = m_kScrRectCache.origin;
		//rectself.size = m_kScrRectCache.size;

		CCRect kClientRect = m_pClientUINode->GetFrameRect();
		CCRect kRect = CCRectZero;
		CCSize kPicRect = m_picScroll->GetSize();

		//** chh 2012-07-24 **//
		//rect.size.width		= sizePic.width * fScale;
		//rect.size.height	= rectself.size.height / rectClient.size.height * rectself.size.height;//sizePic.height * fScale;
		kRect.size.width = kPicRect.width;
		kRect.size.height = kPicRect.height;

		//rect.origin			= ccp(rectself.size.width - rect.size.width, -rectClient.origin.y / rectClient.size.height * rectself.size.height);

		/*
		 rect.origin			= ccp(rectself.size.width - rect.size.width,
		 -rectClient.origin.y / (rectClient.size.height-rectself.size.height+rect.size.height) * rectself.size.height);
		 */

		/*rect.origin			= ccp(rectself.size.width - rect.size.width,   (-rectClient.origin.y/(rectClient.size.height -rectself.size.height))*(rectself.size.height - rectself.origin.y -rect.size.height));*/

		if (GetViewCount() * GetViewSize().height <= kSelfRect.size.height)
		{
			return;
		}

		kRect.origin = ccp(kSelfRect.size.width - kRect.size.width,
				(-kClientRect.origin.y
						/ (kClientRect.size.height - kSelfRect.size.height))
						* (kSelfRect.size.height - kRect.size.height));

		if (kRect.origin.y < 0)
		{
			kRect.origin.y = 0;
		}

		kRect.origin = ccpAdd(kRect.origin, this->GetScreenRect().origin);

		if (m_picScroll->GetSize().height != kRect.size.height)
		{
			//** chh 2012-07-24 **//
			//delete	m_picScroll;
			//m_picScroll = NDPicturePool::DefaultPool()->AddPicture(GetSMImgPath("General/texture/texture5.png"),rect.size.width, rect.size.height);
		}

		if (m_picScrollBg)
		{
			CCSize kPictureSize = m_picScrollBg->GetSize();
			CCRect kTempRect;
			kTempRect.origin.x = kRect.origin.x;
			kTempRect.origin.y = kSelfRect.origin.y;
			kTempRect.size.width = kPictureSize.width;
			kTempRect.size.height = kSelfRect.size.height;
			m_picScrollBg->DrawInRect(kTempRect);
		}

		m_picScroll->DrawInRect(kRect);
	}
}
bool NDUIScrollViewContainer::CanDestroyOnRemoveAllChildren(NDNode* pNode)
{
	if (pNode == m_pClientUINode)
	{
		return false;
	}
	return true;
}

void NDUIScrollViewContainer::SetBottomSpeedBar( bool bBar )
{
	m_bIsBottomSpeedBar = bBar;
}

/*
 unsigned int NDUIScrollViewContainer::GetPerPageViews()
 {
 unsigned int nViews		= 0;
 
 CCSize sizeContainer	= this->GetFrameRect().size;
 
 if (UIScrollStyleHorzontal == GetScrollStyle())
 {
 
 nViews	= (sizeContainer.width / m_sizeView.width + 0.5);
 }
 else
 {
 nViews	= (sizeContainer.height / m_sizeView.height + 0.5);
 }
 
 return nViews;
 }

 bool NDUIScrollViewContainer::IsViewCanCenter()
 {
 CCSize sizeContainer	= this->GetFrameRect().size;

 if (UIScrollStyleHorzontal == GetScrollStyle())
 {
 int nContainterW	= int(sizeContainer.width);
 int nViewW			= int(m_sizeView.width);
 if (0 == nViewW)
 {
 return false;
 }
 
 if (nViewW == nContainterW || (GetPerPageViews() % 2) != 0)
 {
 return true;
 }
 }
 else
 {
 int nContainterH	= int(sizeContainer.height);
 int nViewH			= int(m_sizeView.height);
 if (0 == nViewH)
 {
 return false;
 }
 
 if (nViewH == nContainterH || (GetPerPageViews() % 2) != 0)
 {
 return true;
 }
 }
 
 return false;
 }
 */