/*
 *  CtrlCheckBox.h

 *
 *  Created by ndtq on 11-2-15.
 *  Copyright 2011 TQ Digital Entertainment. All rights reserved.
 *
 */

#ifndef __CTRL_CHECKBOX_H__
#define __CTRL_CHECKBOX_H__

#include "wndobject.h"
/**
  ��ѡ��
**/
class CCtrlCheckBox:public CWndObject
{
public:
	enum 
	{
		enNormal,
		enDown,
		enDisable,
		enActive
	};
	CCtrlCheckBox();
	virtual ~CCtrlCheckBox();

	//�Ƿ����
	bool IsEnabled() const;

	//�����Ƿ����
	void SetEnabled(bool isEnable);

	//�Ƿ�ѡ��
	bool IsChecked() const;
	
	//����ѡ��
	void SetChecked(bool isChecked);
	
	//�����ı�
	void SetWindowText(const char* lpStr);
	
	//��UTF8���������ı�
	void SetWindowTextWithUTF8(const char* str);

	//����ı�
	const char* GetWindowText();

	//�ı��Ƿ�ɻ���
	bool IsMultiLine()const;

	//�����ı��Ƿ�ɻ���
	void SetMultiLine(bool multiLine);
	
	//�����ı���ʾλ�õ�X����ƫ����
	void SetLeftOffSet(int offset);

	//��ȡ�ı���ʾλ�õ�X����ƫ����
	int GetLeftOffSet()const;
	
	//���δѡ��ʱ��������ɫ
	unsigned int GetFontColor()const;

	//����δѡ��ʱ��������ɫ
	void SetFontColor(unsigned int fontColor);

	//���ѡ��ʱ��������ɫ
	unsigned int GetCheckColor()const;

	//����ѡ��ʱ��������ɫ
	void SetCheckColor(unsigned int checkColor);
	
	//�õ��ؼ����� CTRL_CHECKBOX
	virtual int GetType()const;

	//��ȡ״̬
	int GetStatus()const;

	//����״̬
	void SetStatus(int status);	

public:	
	virtual void MouseEnterHandler(CWndObject* obj,CPoint& pos,const void* lpParam=NULL);
	virtual void MouseLeaveHandler(CWndObject* obj,CPoint& pos,const void* lpParam=NULL);
	virtual void MouseMoveHandler(CWndObject* obj,CPoint& pos,const void* lpParam=NULL);
	virtual bool MouseDownHandler(CWndObject* obj,CPoint& pos,const void* lpParam=NULL);
	virtual void MouseUpHandler(CWndObject* obj, CPoint& pos,const void* lpParam=NULL);
	virtual void MouseClickedHandler(CWndObject* obj, CPoint& pos,const void* lpParam=NULL);
protected:

	//���ƿؼ���ǰ��
	virtual void DoPaintForeground(CPoint& cntScreenOrg,CRect& showRect,CRect& cntRect,CPoint& viewOrg);
private:
	int m_status;//״̬
	bool m_isCheck;//ѡ��
	char* m_lpText;//��ʾ���ı�
	bool m_isMultiLine;//�ɻ���
	int m_iLeftOffSet;//�ı���ʾλ�õ�X����ƫ����(Ĭ��ֵ16)
	int m_iWidth;//�ı���ʾ�Ŀ��
	unsigned int m_fontColor;//δѡ�е�������ɫ
	unsigned int m_checkColor;//ѡ�е�������ɫ
};

#endif