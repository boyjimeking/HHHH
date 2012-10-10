/*
 *  CtrlImage.h
 *	ͼƬ�ؼ�
 *  Created by ndtq on 11-2-14.
 *  Copyright 2011 TQ Digital Entertainment. All rights reserved.
 *
 */

#ifndef __CTRL_IMAGE_H__
#define __CTRL_IMAGE_H__

#include "wndobject.h"
/**
  ͼƬ�ؼ�
**/
class CCtrlImage:public CWndObject
{
	enum //״̬
	{
		enNormal,
		enDown,
		enDisable,
		enActive
	};
public:
	CCtrlImage();
	virtual ~CCtrlImage();
	
	//�Ƿ����
	bool IsEnabled() const;

	//�����Ƿ����
	void SetEnabled(bool isEnable);

	//��������ͼƬ(Ҫ�Ŵ󵽵Ĵ�С��ini������)
	void SetImage(IImageObj* lpImg/*ͼƬ*/,CRect& frameRect/*ͼƬ��ԭ��С �� ��ԭͼƬ��Ҫ��ʾ������*/,bool isStretch=false/*�Ƿ�����*/);	

	//��ȡ�������õı���ͼƬ
	IImageObj* GetImage();

	//��ÿؼ����� CTRL_IMG
	virtual int GetType()const;

public:	
	virtual void MouseEnterHandler(CWndObject* obj,CPoint& pos,const void* lpParam=NULL);
	virtual void MouseLeaveHandler(CWndObject* obj,CPoint& pos,const void* lpParam=NULL);
	virtual void MouseMoveHandler(CWndObject* obj,CPoint& pos,const void* lpParam=NULL);
	virtual bool MouseDownHandler(CWndObject* obj,CPoint& pos,const void* lpParam=NULL);
	virtual void MouseUpHandler(CWndObject* obj, CPoint& pos,const void* lpParam=NULL);
	virtual void MouseClickedHandler(CWndObject* obj, CPoint& pos,const void* lpParam=NULL);
	
protected:
	//��ȡ״̬
	int GetStatus()const;

	//����״̬
	void SetStatus(int status);

	//���ƿؼ��ı���
	virtual void DoPaintBackground(CPoint& cntScreenOrg,CRect& showRect,CRect& cntRect,CPoint& viewOrg);
protected:
	CRect m_recOuterImg;//�������õı���ͼƬ��
	int m_status;//״̬
	bool m_isStretchOut;//�Ƿ�����
	IImageObj* m_lpOuterImg;//�������õı���ͼƬ
};

#endif