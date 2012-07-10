/*
 *  ComboBox.h
 *
 *  Created by ndtq on 11-3-10.
 *  Copyright 2011 TQ Digital Entertainment. All rights reserved.
 *
 */

#ifndef __COMBO_BOX_H__
#define __COMBO_BOX_H__

#include <map>
#include "CtrlDlg.h"
#include "CtrlEdit.h"
#include "CtrlImage.h"
#include "CtrlList.h"
#include "CtrlButton.h"
#include "CtrlCheckBox.h"

enum  //�Ի���ť��ʾ����
{
    enBTN_OK,//ֻ��ʾȷ��
    enBTN_CANCEL,//ֻ��ȡ��
    enBTN_ALL,//ȫ��
    enBTN_NULL,//ȫ����ʾ
};

typedef CCtrlCheckBox CCtrlComboxCheckBox;
typedef vector<CCtrlButton*> BTNLIST;
typedef pair<int, LPRowAtt> RowIdxPair;
typedef vector<RowIdxPair> RowIdxPairList;

#define _COMBOBOX_DROPBUTTON_SIZE 18
#define _COMBOBOX_VIEW_BORDER_SIZE 5
#define _COMBOBOX_INPUT_BORDER_SIZE 2
#define _COMBOBOX_DEFAULT_LINE_SAPCE 0
#define _COMBOBOX_DROPBUTTON_COL_SPACE 2
#define _COMBOBOX_BUTTON_SPACE 2

class IRemoveItemNotify
{
public:
    virtual void NotifyRemoveItem(vector<int> ItemVec) = 0;
    virtual void Reset() = 0;

};

//��Ͽ�CCtrlComboBox������ѡ���б�Ի���
class CCtrlComboBoxList:public CCtrlDlg
{
public:
    CCtrlComboBoxList();
    virtual ~CCtrlComboBoxList();

    //��ȡ�ؼ����� CCtrlComboBoxList
    virtual int GetType()const;

    //���ñ߿���
    void SetBorderSize(int iSize);

    //��ȡ�߿���
    int GetBorderSize()const;

    //static area  hight
    void SetStaticHeight(int iHeight);
    int GetStaticHeight()const;

    //���ð�ť�߶�
    void SetBtnHeight(int iHeight);

    //��ȡ��ť�߶�
    int GetBtnHeight()const;

    //Bind obj
    void SetBindObj(CWndObject* lpBind);
    CWndObject* GetBindObj();

    //style

    //button type
    int GetBtnType()const;

    //���öԻ���ť��ʾ����
    void SetBtnType(int btnType);

    //item style
    void SetCheckBoxAni(const char* lpAni,int fillType=CWndObject::BGTYPE_FIX);
    void SetDelItemBtnAni(const char* lpAni,int fillType=CWndObject::BGTYPE_FIX);
    const char* GetCheckBoxAni();
    void SetSelAni(const char* lpAni,int fillType,int borderSize=0);
    void SetContentAni(const char* lpAni);
    void SetItemHeight(int iHeight);
    //button style
    void SetConfirmBtnStyle(const char* lpAni,int fillType,int borderSize=0);
    void SetCancelBtnStyle(const char* lpAni,int fillType,int borderSize=0);

    //bg style
    void SetFloodBg(const char* lpAni,int fillType=CWndObject::BGTYPE_STRETCH,int borderSize=0);
    void SetHeadBg(const char* lpAni,int fillType=CWndObject::BGTYPE_STRETCH,int borderSize=0);

    //���ñ������ɫ
    void SetTitleColor(unsigned int color);

    //color
    void SetSelColor(unsigned int clrSel);
    void SetValColor(unsigned int clrComm);

    //���ñ���
    void SetTitle(const char* lpText);


    //item
    int InsertItem();
    int InsertItem(int iRow, LPRowAtt lpItem);
    void DeleteItem(int iRow);
    LPRowAtt RemoveItem(int row);

    //�����б���index�е��ı�ΪlpText
    void SetItemText(int index,const char* lpText);

    const char* GetSelItemText();

    void Clear();

    void SetSelBgFillType(int fillType,int borderSize=0);

    //����ѡ����
    void SetSelItem(int index);

    //��ȡѡ����
    int GetSelItem()const;

    //��ȡѡ�е����к�
    void GetSelPos(int& iSelRow/*��*/, int& iSelCol/*��*/);

    //option
    bool IsClickedAndConfirm() const;
    void SetClickedAnConfirm(bool isClickedAndConfirm);

    //�û������б�ѡ����
    void  OnListChanged();

    //���ȷ��
    void OnBtnClicked();

    //���ȡ��
    void OnCancelClicked();

    //����ĳһ�ı���ѡ��,�����к�
    int SearchSelItem(const char* lpText/*�ı�*/);

    //��ͼ������ѡ����,ʹ֮�ɼ�
    void ScrollToSel();

    ClickEvent EventShow;
    ClickEvent EventOK;
    ClickEvent EventCancel;

protected:
    //�˻ص��ӿ�ͳһ��Ϊ:WndProc�ص��ӿ�
    //virtual void RefWndPro(int iType,CWndObject* obj,CPoint& pos,const void* lpParam/*=NULL*/);
    virtual bool WndProc(CWndObject* pObj,UINT message,WPARAM wParam, LPARAM lParam);

    //��ȡĳһ�е��ı�
    const char* GetItemText(int index/*��*/);

    virtual void DoCreated();
    virtual void DoSized(CRect& rect);
    virtual bool DoSizing(CRect& rect);
    virtual void DoShow(bool isShow);
    //
    void ResetSize();
    void RecaluListSize();
protected:
    void RebuildMiniSize();
private:
    int m_iBorderSize;
    CCtrlStatic m_static;//����(�б���������ʾ�ı�)
    CCtrlList m_list;//�б��
    int m_iBtnType;//�Ի���ť��ʾ����
    CCtrlButton m_button;//ȷ����ť
    CCtrlButton m_cancel;//ȡ����ť
    CCtrlStatic m_buttonBg;
    int m_iStaticHeight;//�߶�
    int m_iButtonHeight;	//��ť�߶�
    CSize m_minSize;
    CWndObject* m_lpBindObj;
    //stype
    char* m_lpCheckBoxAni;
    char* m_lpDelBtnAni;
    int m_checkBoxFillType;
    int m_delBtnFillType;
    CRect m_checkBoxRect;
    int m_iSel;	//��ǰѡ�еڼ���
    bool m_isSelAndConfirm;
    CRect m_delItemRect;
    RowIdxPairList m_RemoveRows;
    IRemoveItemNotify* m_pINotifyCtrl;
    int m_iSide;
public:
    void ClearRemoveRows();
    void UndoRemove();
    void SetNotifyCtrl(IRemoveItemNotify* pNotifyCtrl);
    void SetDockSide(int iSide);
    void SetBtnTextWithUTF8(const char* pszOKText, const char* pszCancelText);
};


/**
��Ͽ�(�̳��ԶԻ���,��Ϊ�ڲ�����CCtrlEdit,CCtrlButton,CCtrlComboBoxList(��CCtrlComboBox������ѡ��Ի���))
**/
class CCtrlComboBox:public CCtrlDlg
{
public:
    CCtrlComboBox();
    virtual ~CCtrlComboBox();

    //��ȡ�ؼ����͡�CTRL_COMBOBOX
    virtual int GetType()const;

    //property
    //drop btn size
    void SetDropBtnWidth(int width);

    int GetDropBtnWidth()const;

    void SetDropDownBtnAni(const char* lpAni,int fillType=CWndObject::BGTYPE_STRETCH,int borderSize=0);

    //text
    void SetWindowText(const char* lpText);


    void SetWindowTextWithUTF8(const char* lpText);

    //���m_edit��������ֵ
    const char* GetWindowText();

    //���ñ���
    void SetTitleCaption(const char* lpText);

    //��UTF8�������ñ���
    void SetTitleCaptionWithUTF8(const char* lpText);

    //����ѡ��
    void SetSel(int sel);

    //��ȡѡ����
    int GetSel()const;

    //����ѡ��(������ѡ��Ի����б�������һ���ı�)
    void Append(const char* lpText);

    //��UTF8��������ѡ��(������ѡ��Ի����б�������һ���ı�)
    void AppendWithUTF8(const char* lpText);

    void Clear();

    void DeleteItem(int index);

    //edit style
    void SetFontColor(unsigned int color);
    void SetFontSize(int size);
    void SetFontName(const char* name);
    void SetEditBgAni(const char* lpAni,int fillType=CWndObject::BGTYPE_STRETCH,int borderSize=0);

    //���ñ༭���Ƿ�ֻ��
    void SetReadOnly(bool ReadOnly);

    //�༭���Ƿ�ֻ��
    bool IsReadOnley()const;

    void SetSelBgFillType(int fillType,int borderSize=0);
    //item style

    //border size
    void SetBorderSize(int iSize);
    int GetBorderSize() const;

    //item dlg border size
    void SetItemDlgBorderSize(int iSize);

    //title area  hight
    void SetTitleAreaHeight(int iHeight);

    // confirm button area height
    void SetOKBtnAreaHeight(int iHeight);

    //check box
    void SetCheckBoxAni(const char* lpAni,int fillType=CWndObject::BGTYPE_FIX);

    //DelBtn
    void SetDelItemBtnAni(const char* lpAni,int fillType=CWndObject::BGTYPE_FIX);

    //sel ani
    void SetSelAni(const char* lpAni,int fillType=CWndObject::BGTYPE_NINESQUAREEX,int borderSize=2);

    //content ani
    void SetContentAni(const char* lpAni);

    //list line height
    void SetItemHeight(int iHeight);

    //����ȷ����ť����ʽ
    void SetOKBtnAni(const char* lpAni,int fillType=CWndObject::BGTYPE_FIX,int borderSize=0);

    //����ȡ����ť����ʽ
    void SetCancelBtnAni(const char* lpAni,int fillType=CWndObject::BGTYPE_FIX,int borderSize=0);

    //���ñ������ɫ
    void SetTitleColor(unsigned int color);

    //
    void SetItemDlgBgAni(const char* lpAni,int fillType=CWndObject::BGTYPE_STRETCH,int borderSize=0);
    void SetItemDlgBgFillType(int fillType,int borderSize);

    //color
    void SetSelColor(unsigned int clrSel);
    void SetValColor(unsigned int clrComm);

    //bg
    void SetFloodBg(const char* lpAni,int fillType=CWndObject::BGTYPE_STRETCH,int borderSize=0);
    void SetHeadBg(const char* lpAni,int fillType=CWndObject::BGTYPE_STRETCH,int borderSize=0);

    //btn type
    void SetBtnType(int btnType);

    //clicked and confirm
    void SetClickedAnConfirm(bool clickedAndConfirm);

    //drop button title
    void SetDropBtnText(const char* lpText);
    void SetDropBtnTextWithUTF8(const char* lpText);
    const char* GetDropBtnText();

    //enable
    bool IsEnabled() const;
    void SetEnabled(bool isEnable);

    void DragUp();
    void DragDown();

    CWndObject* GetDropBtn();

    ClickEvent SelChangeEvent;//�û������б�ѡ�е��¼�
    ClickEvent EventOK;	//����ok��ť���¼�
    ClickEvent EventCancel;//����cancel��ť���¼�
    CommonEvent ItemDelEvent;//ɾ��һ��ѡ����¼�
protected:

    //����ȷ��
    void OnSelOK();

    //����ȡ��
    void OnCancel();

    void OnEditChange();
    void OnDlgShow();
    void OnDropClick();
    void OnEditClick();
    virtual void DoCreated();
    virtual void DoSized(CRect& rect);

    //�˻ص��ӿ�ͳһ��Ϊ:WndProc�ص��ӿ�
    //virtual void RefWndPro(int iType,CWndObject* obj,CPoint& pos,const void* lpParam=NULL);
    virtual bool WndProc(CWndObject* pObj,UINT message,WPARAM wParam, LPARAM lParam);

    void ResetSize();
private:
    CCtrlEdit m_edit;//��Ͽ���ı༭��
    CCtrlButton m_button;
    CCtrlComboBoxList m_dlg;//��CCtrlComboBox������ѡ��Ի���
    int m_iDropBtnSize;
    int m_iBorderSize;
    bool m_bEnable;
    bool m_editReady;//�༭���Ƿ�ֻ��

public:
    void SetNotifyCtrl(IRemoveItemNotify* pNotifyCtrl);
    void GetSelPos(int& iSelRow, int& iSelCol);

    //���öԻ���ť��ʾ����
    void SetStyle(int iStyle=enBTN_ALL);
    void SetDockSide(int iSide);
    void SetListBtnTextWithUTF8(const char* pszOKText, const char* pszCancelText);
};



#endif

