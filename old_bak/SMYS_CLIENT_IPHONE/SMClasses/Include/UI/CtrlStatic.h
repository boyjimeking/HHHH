/*
 *  CtrlStatic.h
 *  Created by ndtq on 11-2-14.
 *  Copyright 2011 TQ Digital Entertainment. All rights reserved.
 */

#ifndef __CTRL_STATIC_H__
#define __CTRL_STATIC_H__

#include "WndObject.h"
#include "DataConvert.h"

/**
  ��̬�ı��ؼ�
**/
class CCtrlStatic:public CWndObject
{
public:
	CCtrlStatic();
	virtual ~CCtrlStatic();
	
	//�õ��ı�
	virtual const char* GetWindowText();

	//�����ı�
	void SetWindowText(const char* lpText);
	
	//����UTF�ı�(���Զ���UTF-8ת��GB18030)
	void SetWindowTextWithUTF8(const char* str);
	
	//�õ�������ɫ
	unsigned int GetFontColor() const;

	//����������ɫ
	void SetFontColor(unsigned int iColor);
	
	//��ȡˮƽ���뷽ʽ
	int GetVerAlign() const;
	
	//���ô�ֱ���뷽ʽ
	void SetVerAlign(int align);
	
	//��ȡ��ֱ���뷽ʽ
	int GetHerAlign() const;
	
	//����ˮƽ���뷽ʽ
	void SetHerAlign(int align);
	
	//�����Ƿ��Զ�����
	void SetBreak(bool isBreak=true);	
	
	//�Ƿ��Զ�����
	bool GetBreak()const;

	//�õ��ؼ�����CTRL_STATIC
	virtual int GetType()const;

	//����������ʽ
	void SetFontStyle(DWORD dwFontStyle);

protected:
	//���ƿؼ���ǰ��
	virtual void DoPaintForeground(CPoint& cntScreenOrg,CRect& showRect,CRect& cntRect,CPoint& viewOrg);	

	char* m_lpText;//��ʾ�ı�
private:
	unsigned int m_fontColor;//��ɫ
	int m_verAlign;//��ֱ���뷽ʽ
	int m_herAlign;//ˮƽ���뷽ʽ
	int m_iWidth;//�ı���ʾ�Ŀ��
	bool m_isMuliLine;//�Ƿ��Զ�����
	DWORD m_dwFontStyle;//�ı�������ʽ(CFM_BOLD|CFM_UNDERLINE)
	CDataConvert m_dataConvert;
};

#endif
