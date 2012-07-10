/*
 *  CtrlList.h
 *  Created by ndtq on 11-2-18.
 *  Copyright 2011 TQ Digital Entertainment. All rights reserved.
 */

#ifndef __CTRL_LIST_H__
#define __CTRL_LIST_H__

#include <vector>
#include "CtrlDlg.h"
#include "CtrlView.h"
#include "CtrlStatic.h"
#include "CtrlImage.h"
//#include "Event.h"

using namespace std;

//�б���ͼ�ؼ�������
typedef struct tagListCommAtt
{
	//for title
	int m_iColCount;//����(��Ӧini�ļ����"ColAmount")
	int* m_iColWidth;//һ�еĿ�(��Ӧini�ļ����"ColWidth")
	
	int* m_iTitleVerAlign;//����ÿһ�еı�����ˮƽ���뷽ʽ
	int* m_iTitleHerAlign;//����ÿһ�еı�������ֱ���뷽ʽ
	
	int* m_iValueVerAlign;//����ÿһ�еĿؼ���ֱ���뷽ʽ
	int* m_iValueHerAlign;//����ÿһ�еĿؼ�ˮƽ���뷽ʽ
	
	unsigned int* m_clrSel;//���������е�ѡ��ʱ����ɫ
	unsigned int* m_clrComm;//���������е���ɫ
	char* m_lpSelBg;
	int m_iRowHeight;	//�и߶�(Ĭ��16)
	//line space
	int m_iRowSpace;//ÿһ��֮��ļ��(Ĭ��0)
	int m_iColSpace;//ÿһ��֮��ļ��(Ĭ��0)
	//for value
	char* m_lpValueBg;//����ͼƬ
	char* m_lpSelectBg;//ѡ��ʱ�ı���ͼƬ
	DWORD* m_dwFontStyle;//���������е�������ʽ
	
	tagListCommAtt();
}ListCommAtt,*LPListCommAtt;

enum //һ����Ŀؼ�����
{
	LISTITEM_TYPE_TEXT,//����
	LISTITEM_TYPE_IMAGE,//ͼƬ
	LISTITEM_TYPE_OBJ//�����ؼ�
};
typedef struct tagListTitleAtt
{
	CCtrlImage** m_lpBg;//����ͼƬ
	CWndObject** m_lpObject;//�ؼ�
	bool m_isTitle;//�Ƿ���������ͷ�ı�
	tagListTitleAtt();
}ListTitleAtt,*LPListTitleAtt;//�����б���


typedef struct tagListValueAtt
{
	int m_type;
	bool m_autoDestory;//�Զ�����(Ĭ��false)
	CCtrlImage* m_lpBg;//����ͼƬ
	CWndObject* m_lpObject;//�ؼ�
	DWORD	m_dwData;
	tagListValueAtt();
}ListValueAtt,*LPListValueAtt;//һ��


typedef struct tagRowAtt
{
	LPListValueAtt m_lpItems;//ָ��һ���ϵ������е�ָ��
	CCtrlImage* m_lpBg;
	int m_rowHeight;
	tagRowAtt();
}RowAtt,*LPRowAtt;//һ�е�����


typedef vector<LPRowAtt> VECROWS; //���е���
/*		CListView
			CListDataView
			CListTitleView
*/
//�б���ͼ
class CListView:public CCtrlView
{
public:
	CListView();
	virtual ~CListView();
	virtual bool MouseDragHandler(CWndObject* obj,int relateX,int relateY,const void* lpParam=NULL);
	CommonEvent m_dragEvent;//(��CCtrlList::DoViewViewMove,CCtrlList::DoTitleViewMove)

	//�Ƿ����
	bool IsEnabled() const;

	//�����Ƿ����
	void SetEnabled(bool isEnable);
private:
	bool m_isEnable;//�Ƿ����
};		

//�б�������ͼ
class CListDataView:public CListView
{
public:
	CListDataView();
	virtual ~CListDataView();
	
	void SetRows(VECROWS* lpRows);

	void SetCommAtt(LPListCommAtt lpAtt);

	//��ͼ������ѡ����,ʹ֮�ɼ�
	void ScrollToSel();

	//��ͼ������ĳһ��,ʹ֮�ɼ�
	void ScroolToRow(int iRow/*�к�*/);
	
	//����ѡ����
	void SetSelRow(int iRow);

	//����ѡ����
	void SetSelCol(int iCol);
	
	//��ȡ��ǰѡ�е���
	int GetSelRow() const;

	//��ȡ��ǰѡ�е���
	int GetSelCol() const;

	//���ݴ���������������(���Ҳ���,�����кŶ�Ϊ-1)
	void GetRowAndCol(CPoint& pos/*�����ϵ�����*/,int& iRow/*��*/,int& iCol/*��*/);	

	//������¼�(CCtrlList::DoSelChange)
	ClickEvent m_selEvent;

protected:

	//�˻ص��ӿ�ͳһ��Ϊ:WndProc�ص��ӿ� 
	//virtual void RefWndPro(int iType,CWndObject* obj,CPoint& pos,const void* lpParam=NULL);

	//�������ں��Ӵ����¼�(SendMessage������¼�)
	virtual bool WndProc(CWndObject* pObj/*������Ϣ�Ĵ���*/,UINT message,WPARAM wParam, LPARAM lParam);

	//�����ͼ���ܴ�С(�����������е��Ӵ���)
	virtual void GetViewRect(CRect& size); 


	//
	LPListCommAtt GetCommAtt();	

	//��ȡ���е���
	VECROWS* GetRows();	

private:
	VECROWS* m_lpRows;//���е���
	LPListCommAtt m_lpCommAtt;//�б���ͼ�ؼ�������
	int m_sel;//��ǰѡ����
	int m_selCol;
};

//�б�ͷ��ͼ
class CListTitleView:public CListView
{
public:
	CListTitleView();
	virtual ~CListTitleView();

	//���������б���
	void SetTitleAtt(LPListTitleAtt lpAtt);

	//��ȡÿһ�е���ͷ
	LPListTitleAtt GetTitleAtt();

	//�����б���ͼ�ؼ�������
	void SetCommAtt(LPListCommAtt lpAtt);

	//��ȡ�б���ͼ�ؼ�������
	LPListCommAtt GetCommAtt();	

	//������������¼�
	CommonEvent EventTitleClick;
protected:

	//�˻ص��ӿ�ͳһ��Ϊ:WndProc�ص��ӿ�
	//virtual void RefWndPro(int iType,CWndObject* obj,CPoint& pos,const void* lpParam=NULL);
	
	//�������ں��Ӵ����¼�(SendMessage������¼�)
	virtual bool WndProc(CWndObject* pObj/*������Ϣ�Ĵ���*/,UINT message,WPARAM wParam, LPARAM lParam);

	//�����ͼ���ܴ�С(�����������е��Ӵ���)
	virtual void GetViewRect(CRect& size); 

	//��ȡѡ����
	void GetSelCol(CPoint& pos,int& iCol);	
private:
	LPListTitleAtt m_lpTitleAtt;//�����б���
	LPListCommAtt m_lpCommAtt;//�б���ͼ�ؼ�������
	int m_selCol;
};

//�б�ؼ�
class CCtrlList:public CCtrlDlg
{
public:
	CCtrlList();
	virtual ~CCtrlList();

	//�����������п��(����Ҫ������)
	void SetColCount(int iCountCols/*����*/,int* iWidth/*ÿ�п�ȵ�����*/,int rowHeight=16/*�и߶�*/);

	//��ȡ����
	int	 GetColCount();
	//titleText array must be match with col count

	//�����б���ͷ
	void CreateTitle(char** titleText/*�����е�����*/,const char* titleBg=NULL, unsigned int titleColor=_DEFAULT_FONT_COLOR);
	
	//Item function
	//����һ��
	int  InsertRow();

	int  InsertRow(int iRow/*���������*/);

	int  InsertRow(int iRow, LPRowAtt lpItem);

	//��������
	CWndObject* SetColValue(int row,int col,const char* lpText,bool isImage=false);

	//��UTF8������������
	CWndObject* SetColValueWithUTF8(int row,int col,const char* lpText,bool isImage=false);

	//��ȡһ���ϵ��ı�
	int GetItemText(int row/*��*/,int col/*��*/, char *pszBuffer/*�õ����ı�*/, int nMaxLen/*����õĳ���*/) const;

	//��ȡһ���ϵ��ı�
	const char* GetItemText(int row/*��*/,int col/*��*/);

	//����һ��Ķ���
	CWndObject* SetColObj(int row/*��*/,int col/*��*/,CWndObject* lpObj/*�ؼ�*/,CRect& size/*��С*/,bool autoDestory=true/*�Զ��ͷ��ڴ�*/);

	//ɾ��һ��
	void DeleteRow(int row/*�к�*/);

	//ɾ��һ��
	void DeleteRowAtt(LPRowAtt lpItem/*ָ��Ҫɾ�����е�ָ��*/);


	LPRowAtt RemoveRow(int row/*�к�*/);

	//���һ���ϵĿؼ�
	CWndObject* GetColObj(int row/*��*/,int col/*��*/);

	DWORD SetData(int row,int col,DWORD dwData);

	DWORD GetData(int row,int col);
	
	//��ͼ������ѡ����,ʹ֮�ɼ�
	void ScrollToSel();
	
	//��ȡѡ�е��к�
	int GetSelRow() const;
	
	//��ȡѡ�е��к�
	int GetSelCol() const;
	
	//����ѡ�е�iRow��
	void SetSelRow(int iRow);

	//����ѡ�е�iCol��
	void SetSelCol(int iCol);
	
	//����ѡ��ʱ�ı���ͼƬ
	void SetSelBgAni(const char* lpAni);

	//���ñ���ͼƬ
	void SetContentAni(const char* lpAni);
	
	//����һ�еĿؼ���ֱ���뷽ʽ
	void SetValueVerAlign(int col/*��*/,int align/*���뷽ʽ*/);

	//����һ�еĿؼ�ˮƽ���뷽ʽ
	void SetValueHerAlign(int col,int align);

	//����һ�еı�������ֱ���뷽ʽ
	void SetTitleHerAlign(int col,int align);

	//����һ�еı�����ˮƽ���뷽ʽ
	void SetTitleVerAlign(int col,int align);

	//����һ�е���ɫ
	void SetValueCommClr(int col,unsigned int clr);

	//����һ�е�ѡ��ʱ����ɫ
	void SetValueSelClr(int col,unsigned int clr);

	//����һ�е�������ʽ
	void SetValFontStyle(int col, DWORD dwFontStyle);
	
	//ȡ���б���
	void CancelTitle();

	//����б�(��������е���ͼ����)
	void ClearAllItems();

	//��ÿؼ����� CTRL_LIST
	virtual int GetType()const;

	//�Ƿ���ѡ����
	bool IsRowSel()const;

	//�����Ƿ���ѡ����
	void SetRowSel(bool rowSel=true/*�Ƿ���ѡ����*/);

	//���ñ�����䷽ʽ
	void SetSelBgFillType(int fillType,int borderSize=0/*�߿���*/);

	//����ѡ��ʱ�ı�����䷽ʽ
	void SetValBgFillType(int fillType,int borderSize=0);	
	
	//����һ�еĿ��
	void SetColWidth(int index/*�к�*/,int iWidth/*���*/);

	//��ȡһ�еĿ��
	int GetColWidth(int index)/*�к�*/;
	
	//����һ�еĸ߶�
	void SetRowHeight(int iHeight);

	//��ȡһ�еĸ߶�
	int GetRowHeight()const;

	//����һ��ѡ��ʱ����ɫ
	void SetSelColor(int iCol/*��*/,unsigned int clrSel/*��ɫ*/);
	
	//����һ�е���ɫ
	void SetValColor(int iCol/*��*/,unsigned int clrComm/*��ɫ*/);
	
	//�Ƿ��й�����
	bool IsSlider()const;

	//�����Ƿ��й�����
	void SetSlider(bool slider);

	//��ù������ı���ͼ
	const char* GetSliderAni();

	//���ù������ı���ͼ
	void SetSliderAni(const char* lpAni);

	//�Ƿ���ˮƽ������
	bool IsHerSlider()const;

	//������ˮƽ������
	void SetHerSlider(bool slider);

	//���ˮƽ�������ı���ͼƬ
	const char* GetHerSliderAni();

	//����ˮƽ�������ı���ͼƬ
	void SetHerSliderAni(const char* lpAni);

	//�������Ŀ��
	int GetSliderSize()const;

	//���ù������Ŀ��
	void SetSliderSize(int size);

	//�б�������ͼm_view���ܴ�С(�����������е��Ӵ���)
	void GetDataViewSize(int& iWidth,int& iHeight);

	//���������
	int GetRowCount()const;

	//����ѡ�и���¼�(��ӦON_LIST_SEL_CHANGE(id,fun))
	ClickEvent SelChangeEvent;

	//������������¼�(��ӦON_LIST_TITLE_CLICKED(id,fun))
	CommonEvent EventTitleClick;	

	//��ȡ�б���ͼ��λ��(���������ڵ�λ��)
	CPoint GetListViewPos();

	//�����б���ͼ��λ��(���������ڵ�λ��)
	void SetListViewPos(CPoint& ptRefPoint);

	bool IsEnabled() const;

	void SetEnabled(bool isEnable);

	//��ȡ�б��������
	bool GetHeaderTitle(int nIndex/*�к�*/, string& strTitle/*����*/);

	//��UTF8���������б�������
	void SetHeaderTitleWithUTF8(int nIndex, const char* pszText);

	//����һ���������ɫ(���óɹ�����true)
	bool SetCellFontColor(int iRow/*��*/, int iCol/*��*/, DWORD dwColor/*��ɫ*/);

	//������Ļ�����ȡ�������ڵĵ�Ԫ��(���Ҳ���,�����кŶ�Ϊ-1)
	void GetCell( CPoint& posScreen/*��Ļ����*/,int& iRow/*��*/,int& iCol/*��*/);

protected:

	//���ٿؼ�
	void Destory();

	//���һ��Ŀؼ�
	void ClearValueItem(LPListValueAtt item/*ָ��ø��ָ��*/);

	//����������б���
	void ClearTitleAtt();

	//��������ٸ��б���ͼ�ؼ�������
	void ClearCommAtt();

	//�к��Ƿ���Ч(������Χ��С��0)
	bool OutOfRange(int index/*�к�*/);

	//�ؼ������󴥷�
	virtual void DoCreated();

	//�ı䴰�ڴ�С��(SetClientRect�󴥷�)
	virtual void DoSized(CRect& rect);

	//�ƶ��б���ͼ
	void DoViewViewMove(WPARAM viewX, LPARAM viewY);

	//�ƶ�������
	void DoTitleViewMove(WPARAM viewX, LPARAM viewY);

	//ѡ�в�ͬ���к�,���»�ͼ(�ı��ı���ɫ,�ı䱳����ɫ)
	void DoSelChange();

	//����������ͼλ��
	void ResetSize();

	//�ؽ��б��С
	void RebuildItemSize(int iStartRow/*��ʼ����*/);

	//������������¼�
	void DoTitleClicked(WPARAM wParam/*��*/,LPARAM lParam/*��*/);

	//���������������б��С
	void CaluRectWidthRowAndCol(CRect& rect/*�б��С*/,int row/*��*/,int col/*��*/);

	//������뷽ʽ������
	bool CaluAlignRect(CRect& rect,int iWidth,int iHeight,int iVerAlign/*��ֱ���뷽ʽ*/,int iHerAlign/*ˮƽ���뷽ʽ*/,CRect& retPos);

private:
	int m_iSelBgFillType;//ѡ��ʱ�ı�����䷽ʽ
	int m_iValBgFillType;//������䷽ʽ
	int m_iSelBgBorderSize;//ѡ��ʱ�ı����߿���
	int m_iValBgBorderSize;//�����߿���
	bool m_isEnable;
	bool m_isDragStatus;	//�Ƿ���ק״̬
	CListDataView m_view;//�б�������ͼ
	CListTitleView m_title;//�б�ͷ��ͼ
	ListCommAtt m_commAtt;//���б���ͼ�ؼ�������
	ListTitleAtt m_titleAtt;//�����б���
	VECROWS m_rows;//���е���
	int m_selRow;//��ǰѡ����
	int m_selCol;//��ǰѡ����
	bool m_isRowSel;//�Ƿ���ѡ����
	
	typedef map<CCtrlStatic*, DWORD> CELL_COLOR;
	CELL_COLOR m_cellcolor;//��ɫ
};

//int CCtrlList::AppendItem( const char* lpText,int iValue,bool image )
//{
//	return InsertRow();
//}
//
//void CCtrlList::SetItemText( int row,int col,const char* lpText,bool image )
//{
//	SetColValue(row, col, lpText, image);
//}
//
//int CCtrlList::GetCurSel() const
//{
//	return GetSelRow();
//}
//
//int CCtrlList::AddString( const char* lpText )
//{
//	return InsertRow();
//}
#endif


