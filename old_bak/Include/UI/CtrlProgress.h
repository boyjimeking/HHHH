/*
 *  CtrlProgress.h
 *  �������ؼ���
 *  Created by ndtq on 11-2-15.
 *  Copyright 2011 TQ Digital Entertainment. All rights reserved.
 */

#ifndef __CTRL_PROGRESS_H__
#define __CTRL_PROGRESS_H__

#include "wndobject.h"
/**
  ������(CCtrlProgress�ؼ����ṩ��ʾ�ٷ�֮��ֵ,Ҫ��CCtrlStatic�ٻ�����)
**/
class CCtrlProgress:public CWndObject
{
public:
	CCtrlProgress();
	virtual ~CCtrlProgress();
	
	//���÷�Χ
	void SetRange(int iMin,int iMax);

	//��ȡ��Сֵ
	int GetMin()const;

	//��ȡ���ֵ
	int GetMax()const;

	//���ý���ֵ
	void SetPos(int pos);

	//��ȡ����ֵ
	int GetPos()const;

	//��ȡ�߿���
	int GetBorderSize()const;

	//���ñ߿���
	void SetBorderSize(int borderSize/*���ֵ(����С��1)*/);

	//����ǰ��ͼƬ
	void SetProgressAni(const char* lpAni);

	//��ȡǰ��ͼƬ
	const char* GetProgressAni();

	//��ȡǰ��ͼƬ֡��
	int GetAniFrame()const;

	//����ǰ��ͼƬ֡��
	void SetAniFrame(int index);

	//��ȡ�ؼ�����CTRL_PROGRESS
	virtual int GetType()const;

	//��ȡǰ��ͼƬ�Ƿ��Զ�����
	bool IsForeStrenth() const;

	//����ǰ��ͼƬ�Ƿ��Զ�����
	void SetForeStrenth(bool bForeStrenth=true);

protected:
	//��ǰ��(����ǰ��ͼƬ������m_foreRect,������ͼƬ��ʾ��ǰ����)
	virtual void DoPaintForeground(CPoint& cntScreenOrg,CRect& showRect,CRect& cntRect,CPoint& viewOrg);

	//�ؼ������󴥷�
	virtual void DoCreated();

	//�ı䴰�ڴ�С��(SetClientRect�󴥷�)
	virtual void DoSized(CRect& rect);

	//���ݽ���ֵ�������ǰǰ������
	void CaluForeRect();

	bool m_bForeStrenth;//ǰ��ͼƬ�Ƿ��Զ�����(Ĭ��true)

private:
	char* m_lpProgreeAni;//ǰ��ͼƬ
	int m_iForeFrame;//ǰ��ͼƬ֡��
	int m_iPos;//����ֵ
	int m_iMin;//��Сֵ(Ĭ��0)
	int m_iMax;//���ֵ(Ĭ��100)
	int m_iBorderSize;//�߿���(Ĭ��1)
	bool m_bVercitial;//�Ƿ��Ǵ�ֱ(Ĭ��false)
	CRect m_foreRect;//ǰ��ͼƬ������
};
#endif
