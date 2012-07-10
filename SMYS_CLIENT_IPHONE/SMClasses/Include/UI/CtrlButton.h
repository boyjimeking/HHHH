/*
 *  CtrlButton.h
 *
 *  Created by ndtq on 11-2-12.
 *  Copyright 2011 TQ Digital Entertainment. All rights reserved.
 *
 */

#ifndef __CTRL_BUTTON_H__
#define __CTRL_BUTTON_H__

#include "wndobject.h"
//��ť���
typedef enum BUTTON_STYLE//  
{
	STYLE_NOMAL,//Ĭ�Ϸ��
	STYLE_PUSHLIKE,//��ѹʽ���
	STYLE_CUSTOM  //״̬���û��Լ�ͨ��SetStatus��������status	
}BUTTONSTYLE;
	enum //��ť״̬
	{
		enNormal,
		enDown,
		enDisable,
		enActive
	};
class CCtrlButton : public CWndObject
{
public:

	//�����ã��û������á�
	static void SetClickSound(const char* pSound);

	CCtrlButton();

	virtual ~CCtrlButton();
   
	//���ð�ť���
	void SetStyle(BUTTONSTYLE eButtonStyle);
    
	//��ȡ��ť���
	int GetStyle() const;

	//�����Ƿ�ɼ�
	void ShowWindow(int show);

	//�Ƿ����
	bool IsEnabled() const;

	//�����Ƿ����
	void SetEnabled(bool isEnable=true);
	
	//�����ʾ�ı�
	const char* GetWindowText();

	//������ʾ�ı�
	void SetWindowText(const char* lpText);
	
	//��UTF8����������ʾ�ı�
	void SetWindowTextWithUTF8(const char* str);

	//�õ�������ɫ
	unsigned int GetFontColor() const;

	//����������ɫ
	void SetFontColor(unsigned int iColor);

	//�õ��ؼ�����:CTRL_BUTTON
	virtual int GetType()const;

	//��ȡ��ť״̬
	int GetStatus()const;

	//���ð�ť״̬
	virtual void SetStatus(int status);

public:	
	virtual void MouseEnterHandler(CWndObject* obj,CPoint& pos,const void* lpParam=NULL);
	virtual void MouseLeaveHandler(CWndObject* obj,CPoint& pos,const void* lpParam=NULL);
	virtual void MouseMoveHandler(CWndObject* obj,CPoint& pos,const void* lpParam=NULL);
	virtual bool MouseDownHandler(CWndObject* obj,CPoint& pos,const void* lpParam=NULL);
	virtual void MouseUpHandler(CWndObject* obj, CPoint& pos,const void* lpParam=NULL);
	virtual void MouseClickedHandler(CWndObject* obj, CPoint& pos,const void* lpParam=NULL);
public:

	//���ƿؼ���ǰ��
	virtual void DoPaintForeground(CPoint& cntScreenOrg,CRect& showRect,CRect& cntRect,CPoint& viewOrg);

protected:
	char* m_lpText;//��ʾ���ı�
	int m_status; //��ť״̬
	unsigned int m_fontColor;//�������ɫ
	int m_iWidth;//���ֳ���
	BUTTON_STYLE m_enStyle;//��ť���
};

#endif