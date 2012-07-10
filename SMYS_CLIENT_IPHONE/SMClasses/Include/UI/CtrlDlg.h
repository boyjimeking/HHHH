/*
 *  CtrlDlg.h
 *	
 *  Created by ndtq on 11-2-16.
 *  Copyright 2011 TQ Digital Entertainment. All rights reserved.
 *
 */

#ifndef __CTRL_DLG_H__
#define __CTRL_DLG_H__

#include "CtrlView.h"
/**
  �Ի����ڲ�����
**/
class CCtrlDlg:public CCtrlView
{
public:
	CCtrlDlg();
	virtual ~CCtrlDlg();

	//��ÿؼ����� CTRL_DLG
	virtual int GetType()const;
	
	//�϶�����
	virtual bool MouseDragHandler(CWndObject* obj,int relateX,int relateY,const void* lpParam=NULL);
	
	//��갴���¼�
	virtual bool MouseDownHandler(CWndObject* obj,CPoint& pos,const void* lpParam=NULL);	
	
	//���������϶�
	void SetAllowDrag(bool allowDrag=true);
	
	//����Ƿ������϶�
	bool IsAllowDrag() const;
	
	//�����Ƿ�͸��
	void SetTransparent(bool isTransparent=true);

	//����Ƿ�͸��
	bool IsTransparent() const;
	
protected:	

	//�Զ��󶨿ؼ����¼�
	virtual void BindEvent();

	//�Զ��󶨿ؼ�ID�Ϳؼ�����
	virtual void DoDataExchange(void* pDX);    // DDX/DDV support	

	//��������(�ڲ�ִ�пؼ�ID�Ϳؼ�����,�Զ��󶨿ؼ����¼�)
	virtual void DoCreated();

	//�ı䴰���Ƿ���ʾ�������¼�
	virtual void DoShow(bool isShow);

	//�������ں�ִ�е��¼�
	virtual void OnCreated();
	
private:
	bool m_isTransparent;//�Ƿ�͸��,Ĭ��false
	bool m_isAllowDrag;//�Ƿ������϶�����,��������ڸô����ڶ����϶�,Ĭ��false
};

#endif