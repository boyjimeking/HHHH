/*
 *  TabCtrl.h
 *  ѡ��ؼ���ѡ���ť��
 *  Copyright 2011 TQ Digital Entertainment. All rights reserved.
 *
 */
#ifndef __CTRL_TAB_CTRL_H__
#define __CTRL_TAB_CTRL_H__

#include "CtrlDlg.h"
#include "CtrlButton.h"
#include <vector>
#include <string>

using namespace std;

class CTabCtrl;
//ѡ��ϵ�ÿ����ť
class CTabCtrlBtn : public CCtrlButton
{
public:
	enum //״̬
	{
		enNormal,
		enDown,
		enDisable,
		enActive
	};
public:
	CTabCtrlBtn();

	//����Ϊѡ��(�ı䰴ť��ʽ)
	void SetSelected(bool bSelected);

	//���ö�Ӧ��ѡ��ؼ�
	void SetTabCtrl(CTabCtrl* pTabCtrl);

	//����״̬
	virtual void SetStatus(int status);

protected:
	bool m_bSelected;//�Ƿ�ѡ��
	CTabCtrl* m_pTabCtrl;//���ڵ�ѡ��ؼ�
};

//һ��ѡ���ǩ
class CTabCtrlItem 
{
public:
	CTabCtrlItem();
	CWndObject* m_pDlg;//һ��ѡ���ǩ��ĶԻ���
	CTabCtrlBtn* m_pBtn;//һ��ѡ���ǩ����л���ť
	bool m_bShow;//��ʾ�ɼ�,Ĭ��true
};

typedef std::vector<CTabCtrlItem*> CTabItemArray;//���б�ǩ����

//ѡ��¼���,�����Tabѡ��İ�ť(���Լ̳и���,�������ťʱ��ִ��ClickTabBtn)
class CTabCtrlEvent
{
public:
	virtual void ClickTabBtn(bool bSwitch/*�л�ѡ�*/, bool& bContinue/*�����Ƿ�����л�*/) = 0;
};

/**
  ѡ�
**/
class CTabCtrl  
{
public:
	CTabCtrl();
	virtual ~CTabCtrl();
	
public:
	//���ѡ������б�ǩ������
	CTabItemArray& GetTabItemArray();

	//���ݱ�ǩ�Ż�öԻ���
	CWndObject* GetItemDlg(int nIndex) const;
	
	//����һ��ѡ�
	virtual int InsertItem(int nItem/*���*/, CWndObject* pDlg/*�Ի���*/, CTabCtrlBtn* pBtn/*ѡ���ť*/, bool bVisible=true/*�ɼ�*/);

	//ɾ��һ����ǩ(��ɾ�����ǵ�ǰѡ�еı�ǩ,������ѡ��һ����ǩ,��֤��ǰѡ�еı�ǩ�Ų�Խ��)
	virtual bool DeleteItem(int nItem);

	//����һ����ǩ�ɼ�
	virtual void SetItemVisible(int nItem, bool bVisible);

	//ˢ��ѡ�
	virtual void RefreshTabCtrl();
	
	//ɾ�����б�ǩ,���ѵ�ǰѡ�����ÿ�
	virtual bool DeleteAllItems();

	//���ѡ�еı�ǩ��
	virtual int GetCurSel() const;

	//���ѡ��һ��ѡ���ǩ
	virtual int SetCurSel(int nItem);

	//����ѡ��һ��ѡ���ǩ
	int SetCurSel(CCtrlButton* pBtn/*ѡ��ϵ��л���ť*/);

	//��ñ�ǩ������
	virtual int GetItemCount() const;

	//����һ����ǩ��Ӧ�ĶԻ���
	bool	SetItemWnd(int nItem, CWndObject* pDlg);
	
	//����ѡ��¼���
	void SetTabCtrlListener(CTabCtrlEvent* pEvent);
	
protected:

	//ɾ�����б�ǩ
	void ClearAllItems();

	vector<CRect> m_RectList;//�������б�ǩ��ť��λ�ô�С������
	
protected:
	CTabItemArray m_tabItems;//����ѡ������б�ǩ������
	int m_nCurSel;	//��ǰѡ�еı�ǩ��
	CTabCtrlEvent* m_pEvent;//�����ǩ���¼�
protected:

	//�ͷ�һ����ǩ
	virtual bool ReleaseItem(int nItem);

	//��������nItem��ǩ���Ժ�ı�ǩ��ť
	virtual void ResetItemsPos(int nItem);
};

#endif