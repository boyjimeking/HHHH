/*
 *  Dialog.h
 *  ������
 *  Copyright 2011 TQ Digital Entertainment. All rights reserved.
 *
 */

#ifndef __BUSS_BASE_DIALOG_H__
#define __BUSS_BASE_DIALOG_H__

#include <map>
#include "DialogImpl.h"
#include "Log.h"
#include "Timer.h"

using namespace std;
//�Ի���
class CDialog:public CDialogImpl, public ISysTimerDelegate
{
public:
	//���ڷɵĶ�������(��һ�������ƶ�)
	enum DLG_FLY_ACTION {
		FROMTOP=0,//���ϵ���
		FROMBOTTOM,
		FROMLEFT,
		FROMRIGHT
	};
public: 
	CDialog();
	virtual ~CDialog();

	//�ú����滻��WndProc
	//���ڴ����Զ������������
	//virtual void SetParam(int paramType, void* pParam);
	
	//��ÿؼ����� CTRL_DIALOG
 	virtual int GetType()const;

	//��ʱ��
	virtual int OnTimer(int TimerId, void* pParam);

	//����((����ģ̬�Ի�����))
	virtual bool Process(int nType, int nX, int nY);

	//�����ں����и����ڶ��ɼ�ʱ������(���и����ڶ���m_isVisibleΪ��)
	virtual bool IsVisible();

	//�Ƿ�ɼ�(����m_isVisible)
	bool IsShow();

	//ִ����ͼ����ʱ�Ļص�����(��CCtrlViewҲ��)
	virtual void ActionCallBack(IActionCtrl* Sender, OBJ_ACTIONPOS_INFO& opInfo, bool bFinished);

	//�����Ƿ���ʾ����
	virtual void ShowDialog(bool bShow=true);

	//�رնԻ���(Ŀǰ�Ի���û��ʵ������,ֻ�����ض���)
	virtual void EndDialog();

	//���հ�,�����Ӵ��ڶԻ��򶼿ɽ��յ�
	virtual void OnNetPackage(void* pDataPkg);
	
	//���ٴ���
	virtual void Destroy();//TODO:

	//����λ���ƶ���
	virtual void MoveTo(int nX, int nY);

	//���ƴ��ں��Ӵ��ڻ���(���Ĭ���޵���)
	virtual void Render();

	//����((�Ի��򴴽��ĺ���ִ��˳��:
	/*CUI::CreateDialog,
			CRegistDlgClass_CDlgXXX::CreateDialog,
				CDialog::Create,
					CConstructorCtrl::ConfigDlg,
						CWndObject::Create,
							CDialog::DoCreated,
								CDialogImpl::DoCreated,
					CDialog::OnInitDialog)*/
	virtual void Create(int nIDTemplate/*��ԴID��*/, CWndObject *pParent = NULL/*������*/);	

	//��ʹ�ö���,��m_bFlyEffectΪfalse
	void DisableShowAction();

	//ʹ�ö���,��m_bFlyEffectΪtrue
	void EnableShowAction();

	//�Ƿ��ж���
	bool HasShowAction();

	//���ô��ڵĶ���
	void SetShowAction(DLG_FLY_ACTION FlyFrom=FROMTOP/*����*/, int FlyTime=1000/*��ʱ(ms)*/, bool isVisible=false);
	
	//�˻ص��ӿ�ͳһ��Ϊ:WndProc�ص��ӿ�
	//virtual void RefWndPro(int iType/*�¼�����*/,CWndObject* obj/*�Ӵ���*/,CPoint& pos/*λ��*/,const void* lpParam=NULL);

	//��ʼ���Ի���
	virtual bool OnInitDialog();	

	//��ȡ����ʵ��������
// #if _DEBUG
	virtual	const char* GetDialogClassName();
// #endif
protected:

	//��������
	virtual void DoCreated();

	bool WndProc(CWndObject* pObj/*������Ϣ�Ĵ���*/,UINT message,WPARAM wParam, LPARAM lParam);
private:
	
	bool m_isInit;//�Ի����Ƿ��Ѵ���(Create()��Ϊtrue;��DoCreatedʱm_isInitΪfalse,��ִ�г�ʼ���Ի���OnInitDialog)
	CRect m_rcClient;//����λ�úʹ�С,��CWndObject::m_rect
	
	bool m_bFlyEffect;//����
	DLG_FLY_ACTION m_FlyFrom;//����
	int m_FlyTime;//����ִ��ʱ��
	IActionCtrl* m_pActionIn;//��ʾ�Ի���Ķ���
	IActionCtrl* m_pActionOut;//���ضԻ���Ķ���
	int m_iDis;//�Ի����ƶ���ƫ����
	
	//bool m_bPressed;
//	int m_count;
//	int m_timerID;
};






class CMyDialogBuilder;
typedef int DIALOG_IDD;
typedef  map<DIALOG_IDD, CMyDialogBuilder*> DLGBUILDER_MAP;//ͨ��ID���ҵ��Ի��������
inline DLGBUILDER_MAP& get_DlgBuilderMap()
{
	static DLGBUILDER_MAP s_mapDlgBuilderMap;
	return s_mapDlgBuilderMap;
}
#define g_DialogBuilderMap get_DlgBuilderMap()


#define SET_DIALOGID(Dialog_ID) static int getDialogIDD(){return Dialog_ID;}

#define DIALOG_BUILDER_MUTI(Dialog_ClassName) \
class CRegistDlgClass_##Dialog_ClassName: public CMyDialogBuilder \
{\
public:\
	CRegistDlgClass_##Dialog_ClassName()\
	{\
		get_DlgBuilderMap()[Dialog_ClassName::getDialogIDD()] = this;\
	}\
	virtual CDialog* CreateDialog(){ return new Dialog_ClassName();}\
};\
CRegistDlgClass_##Dialog_ClassName regMe_##Dialog_ClassName;

//�������ĶԻ���,����һ����,s_mapDlgBuilderMap��ͱ��������л�û�����ĶԻ����ID
#define DIALOG_BUILDER(Dialog_ClassName) \
	class CRegistDlgClass_##Dialog_ClassName: public CMyDialogBuilder \
	{\
	public:\
		CRegistDlgClass_##Dialog_ClassName()\
		{\
			get_DlgBuilderMap()[Dialog_ClassName::getDialogIDD()] = this;\
		}\
		~CRegistDlgClass_##Dialog_ClassName()\
		{\
			s_SingleDlg = NULL;\
		}\
		virtual CDialog* CreateDialog(CDialog* lpParent){ if (!s_SingleDlg) { s_SingleDlg = new Dialog_ClassName(); \
			if(s_SingleDlg!=NULL) { s_SingleDlg->Create(Dialog_ClassName::getDialogIDD(),lpParent); } } return s_SingleDlg;}\
		virtual const char* GetDialogClassName(){return #Dialog_ClassName;}\
	private:\
		static CDialog* s_SingleDlg;\
};\
CDialog* CRegistDlgClass_##Dialog_ClassName::s_SingleDlg=NULL;\
CRegistDlgClass_##Dialog_ClassName regMe_##Dialog_ClassName;

//�Ի������Ļ���
class CMyDialogBuilder {
public:

#ifdef WIN32
	#ifdef CreateDialog
		#undef CreateDialog
	#endif
#endif
	//�����Ի���,�����ظ�����;���Ѵ���,��ֱ�ӷ��ض�Ӧ�ĶԻ���
	virtual CDialog* CreateDialog(CDialog* lpParent) = 0;
// #if _DEBUG
	virtual const char* GetDialogClassName()=0;
// #endif
};

// #if _DEBUG
// #define GetDialogClassName GetDialogClassNameA
// #else
// #define GetDialogClassName 
// #endif

#endif
