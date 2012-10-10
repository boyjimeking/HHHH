/*
 *  CtrlLink.h
 *
 *  Created by ndtq on 11-3-18.
 *  Copyright 2011 TQ Digital Entertainment. All rights reserved.
 *
 */
#ifndef __CTRL_LINK_H__
#define __CTRL_LINK_H__

#include "wndobject.h"

//�����ӿؼ�(����CCtrlEdit��)
class CCtrlLink:public CWndObject
{
public:
	CCtrlLink();
	virtual ~CCtrlLink();

	//���ó������ı�
	void SetHref(const char* lpAtt);

	//��ȡ�������ı� 
	const char* GetHref();

	//��ȡ�ؼ����� CTRL_LINK
	virtual int GetType()const;

	//��ȡ������ɫ
	unsigned int GetFontColor() const;

	//����������ɫ
	void SetFontColor(unsigned int iColor);
	
	//��ȡ�����ı�
	const char* GetWindowText();

	//���ô����ı�
	void SetWindowText(const char* lpText);

	//��UTF8�������ô����ı�
	void SetWindowTextWithUTF8(const char* str);	

protected:

	//�ؽ���С
	void ResetSize();

	virtual void DoPaintForeground(CPoint& cntScreenOrg,CRect& showRect,CRect& cntRect,CPoint& viewOrg);	

	virtual bool MouseDownHandler(CWndObject* obj,CPoint& pos,const void* lpParam=NULL);

	virtual void MouseUpHandler(CWndObject* obj, CPoint& pos,const void* lpParam=NULL);

private:
	char* m_lpAtt;//�������ı�
	char* m_lpText;	//�����ı�
	unsigned int m_fontColor;	//������ɫ
	bool m_isPress;//�Ƿ��¹�
	int m_iWidth;//�����ı��Ŀ��
};


#endif
