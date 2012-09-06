//
//  NDUILayer.h
//  DragonDrive
//
//  Created by jhzheng on 10-12-21.
//  Copyright 2010 (����)DeNA. All rights reserved.
//

//	����
//	�ؼ�����

// �������������
/*
 ԭ�ȹ���
 1.  touch begin �ַ� touch down(true) �¼� ���� touchNode
 2.  touch moved �ַ� touch down(false) �¼� �Ҳ��ַ�touchEndEvent�¼�
 3.  touch end���ڲ���touch move��ǰ���£��ַ�  touch down(false) �¼�  �����ӿؼ��ĸ�����Ӧ�¼�

 ���ӹ���
 1. �����¼�
 touch begin ��ʼʱ���ö�ʱ��
 touch end ��ʱ�ѽ��� (�ȷַ������¼�,��δ������ַ���Ӧ�¼�)
 ����ʵ��:
 a.��ʱ������ ����touch moveȡ����ʱ������
 2. �Ͻ����ϳ��¼�(���ڲ��ڲ��ӽ��)
 �Ͻ� touch end ��Ӧ
 �ϳ�  touch begin ��Ӧ touch move ���Լ���������
 ����ʵ��:
 a.���ڲ�������� touch move(�����Լ�������ʱ)ͬʱ�ַ��ϳ��ص� �ô����ϳ���־
 b.�Ͻ�ʱ����Ҫ���ϳ���־
 c.��Ϣ����
 3. ������¼�(�����,�ϻ���)
 ��������,���еĿؼ���û�б�����, �Ҳ�����touch move�¼�,���������ж�(touch begin �� touch end x��y�仯��С)
 ����ʵ��:
 */

#ifndef _ND_UI_LAYER_H
#define _ND_UI_LAYER_H

//#include "NDLayer.h"
#include "NDTouch.h"
#include "NDUINode.h"
#include "NDPicture.h"
#include "NDTimer.h"
#include "CCTexture2D.h"

namespace NDEngine
{
typedef enum
{
	UILayerMoveBegin = 0,
	UILayerMoveRight = UILayerMoveBegin,
	UILayerMoveLeft,
	UILayerMoveUp,
	UILayerMoveDown,
	UILayerMoveEnd,
} UILayerMove;

class NDUILayer;

class NDUILayerDelegate
{
public:
	virtual bool OnLayerMove(NDUILayer* uiLayer, UILayerMove move,
			float distance)
	{
		return false;
	}
	virtual bool OnLayerMoveOfDistance(NDUILayer* uiLayer, float hDistance,
			float vDistance)
	{
		return false;
	}
};

class NDUILayer: public NDUINode, public ITimerCallback
{
	DECLARE_CLASS (NDUILayer)
public:
	NDUILayer();
	~NDUILayer();
public:
	void Initialization();override

	void SetBackgroundImage(const char* imageFile);

	void SetBackgroundImage(NDPicture *pic, bool bClearOnFree = false);

	void SetBackgroundImageLua(NDPicture *pic);

	void SetBackgroundFocusImage(NDPicture *pic, bool bClearOnFree = false);

	void SetBackgroundFocusImageLua(NDPicture *pic);

	void SetBackgroundColor(cocos2d::ccColor4B color);

	void SetFocus(NDUINode* node);

	NDUINode* GetFocus();

	bool UITouchBegin(NDTouch* touch);

	virtual void UITouchEnd(NDTouch* touch);

	void UITouchCancelled(NDTouch* touch);

	void UITouchMoved(NDTouch* touch);

	bool UITouchDoubleClick(NDTouch* touch);

	virtual bool TouchBegin(NDTouch* touch);

	virtual bool TouchEnd(NDTouch* touch);

	virtual void TouchCancelled(NDTouch* touch);

	virtual bool TouchMoved(NDTouch* touch);

	virtual bool TouchDoubleClick(NDTouch* touch);

	void SetTouchEnabled(bool bEnabled);

	void SwallowDragInEvent(bool swallow);

	void SwallowDragOverEvent(bool swallow);

	void SetDragOverEnabled(bool bEnabled);

	virtual void OnCanceledTouch();

	void SetScrollHorizontal(bool bSet);
	bool IsScrollHorizontal();

	void SetMoveOutListener(bool bSet);
public:
	void draw();override
	bool IsVisibled();override

public:
	CGPoint m_kBeginTouch;
	CGPoint m_kEndTouch;
	//touch event center
	virtual bool DispatchTouchBeginEvent(CGPoint beginTouch);
	virtual bool DispatchTouchEndEvent(CGPoint beginTouch, CGPoint endTouch);
	virtual bool DispatchTouchDoubleClickEvent(CGPoint beginTouch);
	// touch node down or up
	void DealTouchNodeState(bool down);
	void DealLongTouchNodeState(bool down);
private:
	NDUINode* m_pkFocusNode;
	NDUINode* m_pkTouchedNode;
	NDUINode* m_pkDragOverNode;

	NDPicture* m_pkPic;
	NDPicture* m_pkPicFocus;
	bool m_bClearOnFree, m_bFocusClearOnFree;
	cocos2d::CCTexture2D* m_pkBackgroudTexture;
	cocos2d::ccColor4B m_kBackgroudColor;

	bool m_bDispatchTouchEndEvent;

	CGRect RectAdd(CGRect rect, int value);
	//NDUIEdit deals
	//void AfterEditClickEvent(NDUIEdit* edit);
public:
	//void IphoneEditInputFinish(NDUIEdit* edit);
	//void IphoneEditInputCancle(NDUIEdit* edit);

	// add new rule by jhzheng 2011.7.30
public:
	void SetScrollEnabled(bool bEnabled);

	void OnTimer(OBJID tag);

	void StartDispatchEvent();
	void EndDispatchEvent();

	virtual bool DispatchLongTouchClickEvent(CGPoint beginTouch,
			CGPoint endTouch);
	virtual bool DispatchLongTouchEvent(CGPoint beginTouch, bool touch);
	virtual bool DispatchDragOutEvent(CGPoint beginTouch, CGPoint moveTouch,
			bool longTouch = false);
	virtual bool DispatchDragOutCompleteEvent(CGPoint beginTouch,
			CGPoint endTouch, bool longTouch = false);
	virtual bool DispatchDragInEvent(NDUINode* dragOutNode, CGPoint beginTouch,
			CGPoint endTouch, bool longTouch, bool dealByDefault = false);
	virtual bool DispatchLayerMoveEvent(CGPoint beginPoint, NDTouch *moveTouch);
	virtual bool DispatchDragOverEvent(CGPoint beginTouch, CGPoint moveTouch,
			bool longTouch = false);

private:

	void ResetEventParam();

protected:

	NDTimer* m_pkLongTouchTimer;

	bool m_bLongTouch;
	bool m_bTouchMoved;
	bool m_bDragOutFlag;
	bool m_bLayerMoved;
	bool m_bEnableMove;
	bool m_bSwallowDragIn;
	bool m_bSwallowDragOver;
	bool m_bEnableDragOver;
	bool m_bHorizontal;
	bool m_bMoveOutListener;
	bool m_bDispatchLongTouchEvent;

	CGPoint m_kMoveTouch;

private:

	static bool ms_bPressing;
	static NDUILayer* m_pkLayerPress;
// 		bool canDispatchEvent();
// 		void StartDispatchEvent();
// 		void EndDispatchEvent();

	DECLARE_AUTOLINK (NDUILayer)
	INTERFACE_AUTOLINK (NDUILayer)
};

}
#endif // _ND_UI_LAYER_H
