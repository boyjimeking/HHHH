/*
 *  DialogImpl.h
 *  ʵ�ִ�����,��ȡini�����Զ���ʼ������
 *  Created by ndtq on 11-2-16.
 *  Copyright 2011 TQ Digital Entertainment. All rights reserved.
 */
#ifndef __DIALOG_IMPL_H__
#define __DIALOG_IMPL_H__

#include "CtrlDlg.h"

//�����ť(fun())
#define ON_BN_CLICKED(id,fun) GetDlgItem(id)->EventClicked.bind(this,&MY_GUI_CLASS_NAME::fun);

//����ƶ�(fun(0,����))
#define ON_MOUSE_MOVE(id,fun) GetDlgItem(id)->EventMouseMove.bind(this,&MY_GUI_CLASS_NAME::fun);

//��갴��(fun((����1,����))
#define ON_MOUSE_DOWN(id,fun) GetDlgItem(id)->EventMouseDown.bind(this,&MY_GUI_CLASS_NAME::fun);

//��굯��(fun(0,����))
#define ON_MOUSE_UP(id,fun) GetDlgItem(id)->EventMouseUp.bind(this,&MY_GUI_CLASS_NAME::fun);

//�����봰��(fun(0,����))
#define ON_MOUSE_ENTER(id,fun) GetDlgItem(id)->EventMouseEnter.bind(this,&MY_GUI_CLASS_NAME::fun);

//����뿪����(fun(0,����))
#define ON_MOUSE_LEAVE(id,fun) GetDlgItem(id)->EventMouseLeave.bind(this,&MY_GUI_CLASS_NAME::fun);

//������ֵ�ı�(fun(��ԴID��,��ǰ����ֵ))
#define ON_SLIDER_CHANGED(id,fun) ((CCtrlSlider*)GetDlgItem(id))->EventSlider.bind(this,&MY_GUI_CLASS_NAME::fun);

//�༭��ı��ı�(fun())
#define ON_EN_CHANGE(id,fun) ((CCtrlEdit*) GetDlgItem(id))->EventChanged.bind(this,&MY_GUI_CLASS_NAME::fun);

//�б�����ѡ����(fun())
#define ON_LIST_SEL_CHANGE(id,fun) ((CCtrlList*)GetDlgItem(id))->SelChangeEvent.bind(this,&MY_GUI_CLASS_NAME::fun);

//�б��������ͷ(fun(����1,����2))
#define ON_LIST_TITLE_CLICKED(id,fun) ((CCtrlList*)GetDlgItem(id))->EventTitleClick.bind(this,&MY_GUI_CLASS_NAME::fun);

//��Ͽ����ѡ����(fun())
#define ON_COMBOBOX_SEL_CHANGE(id,fun) ((CCtrlComboBox*)GetDlgItem(id))->SelChangeEvent.bind(this,&MY_GUI_CLASS_NAME::fun);

//��Ͽ�ɾ��ѡ��(fun(����1,����2))
#define ON_COMBOBOX_DEL_ITEM(id,fun) ((CCtrlComboBox*)GetDlgItem(id))->ItemDelEvent.bind(this,&MY_GUI_CLASS_NAME::fun);

//��Ͽ���ȷ����ť(fun())
#define ON_COMBOBOX_CONFIRM_CLICK(id,fun) ((CCtrlComboBox*)GetDlgItem(id))->EventOK.bind(this,&MY_GUI_CLASS_NAME::fun);

//��Ͽ���ȡ����ť(fun())
#define ON_COMBOBOX_CANCEL_CLICK(id, fun) ((CCtrlComboBox*)GetDlgItem(id))->EventCancel.bind(this,&MY_GUI_CLASS_NAME::fun);

//�϶��ؼ�(fun())
#define ON_DRAG(id,fun) GetDlgItem(id)->EventDrag.bind(this,&MY_GUI_CLASS_NAME::fun);

//�༭���������(fun(UITypes::WPARAM NULL,UITypes::WPARAM NULL))
#define ON_EN_DONE(id,fun) ((CCtrlEdit*) GetDlgItem(id))->EventDone.bind(this,&MY_GUI_CLASS_NAME::fun);

//�Զ��󶨿ؼ����¼�
#define BEGIN_MESSAGE_MAP(theClass,baseClass) void theClass::BindEvent() {
#define END_MESSAGE_MAP() }
#define DECLARE_MESSAGE_MAP() virtual void BindEvent();

//�Զ����öԻ���ʵ����
class CDialogImpl : public CCtrlDlg
{
public:
	CDialogImpl();
	virtual ~CDialogImpl();
	
	
protected:
	//����ini�ļ��Զ����ÿؼ�������
	void DDX_Control(void *pDX, int nIDC, CWndObject &rControl);
};


#endif


