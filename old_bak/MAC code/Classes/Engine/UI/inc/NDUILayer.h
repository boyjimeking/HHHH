//
//  NDUILayer.h
//  DragonDrive
//
//  Created by jhzheng on 10-12-21.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//
//	介绍
//	控件容器

// 容器规则简单描述
/*
	原先规则
	1.  touch begin 分发 touch down(true) 事件 产生 touchNode
	2.  touch moved 分发 touch down(false) 事件 且不分发touchEndEvent事件 
	3.  touch end（在产生touch move的前提下）分发  touch down(false) 事件  遍历子控件的各种响应事件

	增加规则
	1. 长按事件
		touch begin 开始时设置定时器 
		touch end 定时已结束 (先分发长按事件,若未处理则分发响应事件)
		具体实现:
			a.定时器规则 出现touch move取消定时器规则
	2. 拖进与拖出事件(限于层内部子结点)
		拖进 touch end 响应
		拖出  touch begin 响应 touch move 在自己区域以外 
		具体实现:
			a.层内部必需出现 touch move(不在自己区域内时)同时分发拖出回调 置存在拖出标志
			b.拖进时必需要有拖出标志
			c.信息传递
	3. 层滚动事件(左或右,上或下)
		触摸过程,所有的控件都没有被处理, 且产生了touch move事件,左右上下判断(touch begin 与 touch end x与y变化大小)
		具体实现:
 */

#ifndef _ND_UI_LAYER_H
#define _ND_UI_LAYER_H

//#include "NDLayer.h"
#include "NDTouch.h"
#include "NDUINode.h"
#include "NDUIEdit.h"
#include "NDPicture.h"
#include "NDTimer.h"

#import "CCTexture2D.h"

namespace NDEngine 
{
	typedef enum
	{
		UILayerMoveBegin = 0,
		UILayerMoveRight = UILayerMoveBegin,		// 往右滑动
		UILayerMoveLeft,							// 往左滑动
		UILayerMoveUp,								// 往上滑动
		UILayerMoveDown,							// 往下滑动
		UILayerMoveEnd,
	}UILayerMove;
	
	class NDUILayer;
	
	class NDUILayerDelegate
	{
		public:
			virtual bool OnLayerMove(NDUILayer* uiLayer, UILayerMove move, float distance) { return false; }
			virtual bool OnLayerMoveOfDistance(NDUILayer* uiLayer, float hDistance, float vDistance) { return false; }
	};
	
	class NDUILayer : public NDUINode , public ITimerCallback
	{
		DECLARE_CLASS(NDUILayer)
	public:
		NDUILayer();
		~NDUILayer();		
	public:
//		
//		函数：Initialization
//		作用：初始化容器，必须被显示或者隐式调用
//		参数：无
//		返回值：无
		void Initialization(); override
//		
//		函数：SetBackgroundImage
//		作用：设置容器的背景图片，调用该方法将会覆盖SetBackgroundColor所设置的背景色
//		参数：imageFile图片资源文件名
//		返回值：无
		void SetBackgroundImage(const char* imageFile);	
		
		void SetBackgroundImage(NDPicture *pic, bool bClearOnFree = false);
		
		void SetBackgroundImageLua(NDPicture *pic);
		
		void SetBackgroundFocusImage(NDPicture *pic, bool bClearOnFree = false);
		
		void SetBackgroundFocusImageLua(NDPicture *pic);
//		
//		函数：SetBackgroundColor
//		作用：设置容器的背景色
//		参数：color颜色值rgba
//		返回值：无
		void SetBackgroundColor(ccColor4B color);
//		
//		函数：SetFocus
//		作用：设置焦点控件
//		参数：node控件
//		返回值：无
		void SetFocus(NDUINode* node);
//		
//		函数：GetFocus
//		作用：获取焦点控件
//		参数：无
//		返回值：控件指针
		NDUINode* GetFocus();
	
//		
//		函数：UITouchBegin
//		作用：如果层响应事件，该方法响应触摸开始事件
//		参数：touch触摸点
//		返回值：true表示该层拦截了点击事件不往底下的层分发触摸事件了，否则往底下的层分发触摸事件，直至被某一个层拦截；默认返回true
		bool UITouchBegin(NDTouch* touch); 
//		
//		函数：UITouchEnd
//		作用：响应触摸结束事件；只有在TouchBegin返回true时该事件才会被触发
//		参数：touch触摸点
//		返回值：无
		virtual void UITouchEnd(NDTouch* touch); 
//		
//		函数：UITouchCancelled
//		作用：响应触摸取消事件；只有在TouchBegin返回true时该事件才会被触发
//		参数：touch触摸点
//		返回值：无
		void UITouchCancelled(NDTouch* touch); 
//		
//		函数：TouchMoved
//		作用：响应触摸移动事件；只有在TouchBegin返回true时该事件才会被触发
//		参数：touch触摸点
//		返回值：无：
		virtual void UITouchMoved(NDTouch* touch); 
		
		bool UITouchDoubleClick(NDTouch* touch); 
		
//		
//		函数：TouchBegin
//		作用：如果层响应事件，该方法响应触摸开始事件
//		参数：touch触摸点
//		返回值：true表示该层拦截了点击事件不往底下的层分发触摸事件了，否则往底下的层分发触摸事件，直至被某一个层拦截；默认返回true
		virtual bool TouchBegin(NDTouch* touch); 
//		
//		函数：TouchEnd
//		作用：响应触摸结束事件；只有在TouchBegin返回true时该事件才会被触发
//		参数：touch触摸点
//		返回值：true表示事件处理了
		virtual bool TouchEnd(NDTouch* touch); 
//		
//		函数：TouchCancelled
//		作用：响应触摸取消事件；只有在TouchBegin返回true时该事件才会被触发
//		参数：touch触摸点
//		返回值：无
		virtual void TouchCancelled(NDTouch* touch); 
//		
//		函数：TouchMoved
//		作用：响应触摸移动事件；只有在TouchBegin返回true时该事件才会被触发
//		参数：touch触摸点
//		返回值：true表示事件处理了
		virtual bool TouchMoved(NDTouch* touch); 
		
		virtual bool TouchDoubleClick(NDTouch* touch); 
//		
//		函数：SetTouchEnabled
//		作用：设置层是否可响应触摸事件
//		参数：bEnabled如果true则响应触摸事件，否则不响应，默认不响应
//		返回值：无
		void SetTouchEnabled(bool bEnabled);
		
		void SwallowDragInEvent(bool swallow);
		
		void SwallowDragOverEvent(bool swallow);
		
		void SetDragOverEnabled(bool bEnabled);
//		
//		函数：OnCancelTouch
//		作用：当层失去触摸响应能力时被调用(由底层框调用)
//		参数：无
//		返回值：无		
		virtual void OnCanceledTouch();
		
		void SetScrollHorizontal(bool bSet);
		bool IsScrollHorizontal();
		// 移动事件时超出范围仍然处理,默认不处理
		void SetMoveOutListener(bool bSet);
		bool IsTouchDown();
	public:			
		void draw(); override
		bool IsVisibled(); override
		
	public:
		CGPoint m_beginTouch, m_endTouch;
		//touch event center
		virtual bool DispatchTouchBeginEvent(CGPoint beginTouch);
		virtual bool DispatchTouchEndEvent(CGPoint beginTouch, CGPoint endTouch);
		virtual bool DispatchTouchDoubleClickEvent(CGPoint beginTouch);
		// touch node down or up
		void DealTouchNodeState(bool down);
		void DealLongTouchNodeState(bool down);
	private:
		NDUINode* m_focusNode; 
		NDUINode* m_touchedNode;
		NDUINode* m_dragOverNode;
		
		NDPicture *m_pic, *m_picFocus;
		bool	  m_bClearOnFree, m_bFocusClearOnFree;
		CCTexture2D *m_backgroudTexture;
		ccColor4B m_backgroudColor;
		
		bool m_dispatchTouchEndEvent;
		
		CGRect RectAdd(CGRect rect, int value);
		//NDUIEdit deals
		void AfterEditClickEvent(NDUIEdit* edit);		
	public:
		void IphoneEditInputFinish(NDUIEdit* edit);
		void IphoneEditInputCancle(NDUIEdit* edit);
		
	// add new rule by jhzheng 2011.7.30
	public:
		
		//		
		//		函数：SetScrollEnabled
		//		作用：设置层是否滚动
		//		返回值：无
		void SetScrollEnabled(bool bEnabled);
		
		void OnTimer(OBJID tag); override
	
		virtual bool DispatchLongTouchClickEvent(CGPoint beginTouch, CGPoint endTouch);
		
		virtual bool DispatchLongTouchEvent(CGPoint beginTouch, bool touch);
		
		virtual bool DispatchDragOutEvent(CGPoint beginTouch, CGPoint moveTouch, bool longTouch=false);
		
		virtual bool DispatchDragOutCompleteEvent(CGPoint beginTouch, CGPoint endTouch, bool longTouch=false);
		
		virtual bool DispatchDragInEvent(NDUINode* dragOutNode, CGPoint beginTouch, CGPoint endTouch, bool longTouch, bool dealByDefault=false);
		
		virtual bool DispatchLayerMoveEvent(CGPoint beginPoint, NDTouch *moveTouch);
		
		virtual bool DispatchDragOverEvent(CGPoint beginTouch, CGPoint moveTouch, bool longTouch=false);
		
	private:
		
		void ResetEventParam();
		
	protected:
	
		NDTimer *m_longTouchTimer;
		
		bool m_longTouch;
		
		bool m_touchMoved;
		
		bool m_dragOutFlag;
		
		bool m_layerMoved;
		
		bool m_enableMove;
		
		bool m_swallowDragIn, m_swallowDragOver;
		
		bool m_enableDragOver;
		
		CGPoint m_moveTouch;
		
		int m_bHorizontal;
		
		bool m_bMoveOutListener;
		
		bool m_bDispatchLongTouchEvent;
		bool m_bTouchDown;
        
        int m_bIsHVFirstTemp;       //0.还未确定滑动的方向 1.水平 2.垂直
        bool m_bIsHVContainer;
	private:
		static bool m_pressing;
		static NDUILayer *m_layerPress;
		bool canDispatchEvent();
		void StartDispatchEvent();
		void EndDispatchEvent();
		
		DECLARE_AUTOLINK(NDUILayer)
		INTERFACE_AUTOLINK(NDUILayer)
	};
	
	
}
#endif // _ND_UI_LAYER_H
