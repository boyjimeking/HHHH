/*
 *  CtrlSlider.h
 *	����ؼ�
 *  Created by ndtq on 11-2-14.
 *  Copyright 2011 TQ Digital Entertainment. All rights reserved.
 *
 */

#ifndef __CTRL_SLIDER_H__
#define __CTRL_SLIDER_H__

#include "wndobject.h"
/*
����ؼ�
*/
class CCtrlSlider:public CWndObject
{
public:
	enum //״̬
	{
		enNormal,
		enDown,
		enDisable,
		enActive
	};
	CCtrlSlider();
	virtual ~CCtrlSlider();

	//���ÿ��
	void SetSliderSize(int sliderSize);

	//��ȡ���
	int GetSliderSize() const;

	//���÷�Χ
	void SetRange(int min,int max);

	//������ֵ
	int GetMax()const;

	//�����Сֵ
	int GetMin()const;

	//��õ�ǰֵ
	int GetValue() const;

	//���û����ֵ
	void SetValue(int value);	
	
	//����Ƿ����
	bool IsEnabled() const;

	//���ÿ���
	bool SetEnabled(bool isEnable);

	//���õ��ڰ�ť����ͼƬ
	void SetSliderAni(const char* lpAni);

	//��õ��ڰ�ť����ͼƬ
	const char* GetSliderAni();

	//��ÿؼ����� CTRL_SLIDER
	virtual int GetType()const;
	
	//���û��鱳��ͼƬ������
	void SetSliderAniStretch(bool Stretch);
	
	//�϶��¼������󻬿�ֵ��С�ı���¼�(�󶨵ĺ�����������ԴID��,��ǰ����ֵ)
	CommonEvent EventSlider;
public:	
	//�϶�����
	virtual bool MouseDragHandler(CWndObject* obj,int relateX,int relateY,const void* lpParam=NULL);	

	//�������
	virtual void MouseEnterHandler(CWndObject* obj,CPoint& pos,const void* lpParam=NULL);

	virtual void MouseLeaveHandler(CWndObject* obj,CPoint& pos,const void* lpParam=NULL);

	virtual void MouseMoveHandler(CWndObject* obj,CPoint& pos,const void* lpParam=NULL);

	virtual bool MouseDownHandler(CWndObject* obj,CPoint& pos,const void* lpParam=NULL);

	virtual void MouseUpHandler(CWndObject* obj, CPoint& pos,const void* lpParam=NULL);

	virtual void MouseClickedHandler(CWndObject* obj, CPoint& pos,const void* lpParam=NULL);
protected:

	//���ƿؼ���ǰ��
	virtual void DoPaintForeground(CPoint& cntScreenOrg,CRect& showRect,CRect& cntRect,CPoint& viewOrg);	

	//�ؼ������󴥷�
	virtual void DoCreated();

	//�ı䴰�ڴ�С(SetClientRect�󴥷�)
	virtual void DoSized(CRect& rect);
	
	//�����ƶ���Сֵ
	void OffSetSlider(int size/*�ı�Ĵ�С(X��Y������)*/);

	//��õ�ǰֵ
	int CaluValue();
	
	//��ȡ״̬
	int GetStatus()const;

	//����״̬
	void SetStatus(int status);

private:
	CRect m_sliderRect;//����ı�������
	bool m_bVertical;//�Ƿ�ֱ
	int m_iSliderSize;//���ڰ�ť�Ŀ��
	int m_status;//״̬
	int m_iMin;//��Сֵ
	int m_iMax;//���ֵ
	int m_iValue;//��ǰ����ֵ
	int m_iFrameIndex;//���ڰ�ť��ͼƬ֡��
	char* m_lpSliderAni;//���ڰ�ť����ͼƬ(������ı���ͼƬ)
	bool m_bSliderAniStretch;//���ڰ�ť����ͼƬ������
};
//int GetSliderValueEx() const
//{
//	return GetValue();
//}

//void SetSliderValue( int value )
//{
//	SetValue(value);
//}

#endif