/*
 *  CtrlView.h
 *  Created by ndtq on 11-2-15.
 *  Copyright 2011 TQ Digital Entertainment. All rights reserved.
 */

#ifndef __CTRL_VIEW_H__
#define __CTRL_VIEW_H__

#include "wndobject.h"

class CtrlPositionAction;
class IActionCtrl; 

/**
  �ؼ�λ�ü�����
**/
class ICtrlPositionListenner
{
public:
	virtual void CtrlActionCallback(CtrlPositionAction* Sender, CPoint& opInfo, bool bFinished) = 0;
	virtual void CtrlMoveCallback(CPoint& opInfo) = 0;
};

//����������
struct OBJ_ACTIONPOS_INFO
{
	C3DPos posCur;//����λ��
	float fScaleCur;//����

	OBJ_ACTIONPOS_INFO()
	{
		memset(&posCur, 0, sizeof(posCur));
		fScaleCur = 1.0;
	};			
};

/**
  ����������
**/
class IActionListenner {
public:
	
	virtual void ActionCallBack(IActionCtrl* Sender, OBJ_ACTIONPOS_INFO& opInfo, bool bFinished) = 0;
};

/**
  ������ͼ
**/
class CCtrlView : public CWndObject, public IActionListenner,public ICtrlPositionListenner
{
public:
	CCtrlView();
	virtual ~CCtrlView();

public:
	//�Ƿ��д�ֱ������
	bool IsSlider()const;

	//�����д�ֱ������
	void SetSlider(bool slider);

	//��ô�ֱ�������ı���ͼƬ
	const char* GetSliderAni();

	//���ô�ֱ�������ı���ͼ
	void SetSliderAni(const char* lpAni);
	
	//�Ƿ���ˮƽ������
	bool IsHerSlider()const;
	
	//������ˮƽ������
	void SetHerSlider(bool slider);

	//���ˮƽ�������ı���ͼƬ
	const char* GetHerSliderAni();

	//����ˮƽ�������ı���ͼƬ
	void SetHerSliderAni(const char* lpAni);
	
	//��ù������Ŀ��
	int GetSliderSize()const;
	
	//���ù������Ŀ��
	void SetSliderSize(int size);
	
	//�Ƿ�����ı���ͼԭ��
//	bool IsChangeViewPos()const;

	//�����Ƿ�����ı���ͼԭ��
	//void SetChangeViewPos(bool isAllow=true);

	//�������ں��Ӵ����¼�(SendMessage������¼�;�Զ����¼���Ϣ����ֵ����ȡϵͳ�¼�,ȡMSG_USEREVENT���ϵ��¼�)
	virtual bool WndProc(CWndObject* pObj/*������Ϣ�Ĵ���*/,UINT message,WPARAM wParam, LPARAM lParam);

public:
	
	virtual bool MouseDragHandler(CWndObject* obj,int relateX,int relateY,const void* lpParam=NULL);
	virtual bool MouseDownHandler(CWndObject* obj,CPoint& pos,const void* lpParam=NULL);
	virtual void MouseUpHandler(CWndObject* obj, CPoint& pos,const void* lpParam=NULL);
	virtual bool MouseDragOverHandler(CWndObject* obj,int relateX,int relateY,const void* lpParam=NULL);

	//ִ����ͼ����ʱ�Ļص�����
	virtual void ActionCallBack(IActionCtrl* Sender, OBJ_ACTIONPOS_INFO& opInfo, bool bFinished);

	//ִ����ͼλ�ö��������Ļص�����(�ı�λ�õĶ�����,�ж��Ƿ�Ҫ�����ö���)
	virtual void CtrlActionCallback(CtrlPositionAction* Sender/*ִ�еĶ���*/, CPoint& opInfo/*����ִ���е�ǰ������*/, bool bFinished/*�����Ƿ�ִ�н���*/) ;

	//����϶�ʱ�ı���ͼ��λ�õ��¼�
	virtual void CtrlMoveCallback(CPoint& opInfo);

	//bool GoBackPosition();

	//�б�����(2:���������(��x���򻬶��Ҵ�����X������ƶ�;
//	1:�����������(��Y���򻬶��Ҵ�����Y������ƶ�,��CCtrlList��Ĭ��1)
	void SetListType(int listType);

	//�����Ƿ��Ӵ��ڹ̶�,�ڸ������в����϶�
	void SetFixed(bool bfixed=true);

	//�����Ӵ����Ƿ�̶�,�ڸ������в����϶�
	bool IsFixed() const;

protected:	
	//���ƹ�����
	virtual void DoPaintSlider(CPoint& cntScreenOrg,CRect& showRect,CRect& cntRect,CPoint& viewOrg);	
	
	virtual bool CaluViewPos(int relateX,int relateY,const void* lpParam=NULL);
	
	//virtual void RefWndPro(int iType,CWndObject* obj,CPoint& pos,const void* lpParam=NULL);

	int CaluViewX(int relateX,CRect& viewRect);
	int CaluViewY(int relateY,CRect& viewRect);	
	bool IsPress()const;
	void SetPress(bool isPress);
	virtual void DrawVerSlider(CRect& viewSize,CPoint& cntScreenOrg,CRect& showRect,CRect& cntRect,CPoint& viewOrg);

	//��ˮƽ������
	virtual void DrawHerSlider(CRect& viewSize,CPoint& cntScreenOrg,CRect& showRect,CRect& cntRect,CPoint& viewOrg);
	
private:
	bool m_bFixed;//�Ƿ��Ӵ��ڹ̶�,�ڸ������в����϶�(Ĭ��false)
	int m_listType;//�б�����(2:���������(��x���򻬶��Ҵ�����X������ƶ�)
					//1:�����������(��Y���򻬶��Ҵ�����Y������ƶ�,��CCtrlList��Ĭ��1)
	bool m_bPress;
	bool m_bSlider;//�Ƿ��д�ֱ������
	bool m_bHerSlider;//�Ƿ���ˮƽ������(�б���ͼƬ�򻭳�)
	char* m_lpSliderAni;//��ֱ�������ı���ͼƬ
	char* m_lpHerSliderAni;//ˮƽ�������ı���ͼƬ
	int m_sliderSize;//�������Ŀ��,��Ӧini���"sliderSize"
	//bool m_isChangeViewPos;//�Ƿ�����ı���ͼԭ��(ɾȥ:��m_bFixed����)
	//CRect m_viewSize;
	//CRect m_cntRect;
	CtrlPositionAction* m_ctrlAction;//����
	//bool m_bMoving;
// 	int m_distance;
// 	int m_speed;
// 	int m_speedType;
// 	int m_dex;
// 	int m_dey;
// 	bool m_OutRangeMoving;
};

#endif
