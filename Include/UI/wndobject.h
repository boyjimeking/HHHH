/*
 *  wndobject.h
 *	���пؼ��Ļ���
 *  Created by ndtq on 11-1-25.
 *  Copyright 2011 TQ Digital Entertainment. All rights reserved.
 */

#ifndef __WND_OBJECT_H__
#define __WND_OBJECT_H__

#include <list>
#include "C3BaseFunc.h"
#include "graphic.h"
#include "uitypes.h"
#include "GameDataSet.h"
#include "DelegateEvent.h"
#include "IAniObj.h"
#include "FrameworkTypes.h"

using namespace std;

//#define _DEBUG_DRAW
#define _DEFAULT_FONT_COLOR 0xd2c2b2

class CWndObject;
typedef list<CWndObject*> LIST_CTRL;

#define CFM_BOLD 0x00000001
#define CFM_UNDERLINE 0x00000004

 /*����ؼ�����,�ɿؼ���GetType()�̶�����(ini���"type"�Ǵ����������ͽ���༭��Լ����,��ͬ,ini���type����:1:CDialog,2:CtrlStatic,3:CtrlButton,4:CtrlImageEx,5:CtrlList,6:CtrlComboBox,7:CtrlProgress,8:CtrlSlider,9:CCtrlCheckBox,10:CtrlEdit,
)*/
enum 
{
	CTRL_WNDOBJ,//0		  CWndObject	
	CTRL_STATIC,		//CCtrlStatic
	CTRL_IMG,    //2	  CCtrlImage
	CTRL_BUTTON,		//CCtrlButton
	CTRL_SLIDER,//4		  CCtrlSlider
	CTRL_CHECKBOX,		//CCtrlComboBox	
	CTRL_PROGRESS,//6     CCtrlProgress
	CTRL_LIST,			//CCtrlList
	CTRL_DLG,//8		//CCtrlDlg	
	CTRL_EDIT,			//CCtrlEdit
	CTRL_COMBOBOX,//10    CCtrlComboBox
	CTRL_LINK,			//CCtrlLink
	CTRL_DLG_INNER,//12    CCtrlComboBoxList(comboBox������ѡ��Ի���)
	CTRL_DIALOG			 //CDialog
};

/** 
�ؼ����ϵͼ
CWndObject
	CCtrlView(������ͼ)
		CCtrlEdit(�༭��)
		CListView(�б�ؼ�)
			CListDataView(�б�������ͼ)
			CListTitleView(�б������ͼ)
		CCtrlDlg(�Ի���)
			CCtrlList(�б�ؼ��ĶԻ���)
			CDialogImpl(�Զ����öԻ���ʵ����)
				CDialog(�Ի���)
			CCtrlComboBox(��Ͽ�)
			CCtrlComboBoxList(comboBox������ѡ��Ի���)
	CCtrlCheckBox(��ѡ��)
	CCtrlSlider(����)
	CCtrlButton(��ť)
	CCtrlImage(ͼƬ�ؼ�)
	CCtrlStatic(��̬�ı�)
	CCtrlProgress(������) 
	CCtrlLink(������)

CTabCtrl(ѡ�)   
INumPad
	CNumPad(�������ֵ���Ͽؼ�,��CCtrlEdit,CCtrlSlider)
*/

enum //�¼�����
{													//ִ�к���
	MSG_MOUSEENTER=1,	  //����ƽ�		MouseEnterHandler
	MSG_MOUSELEAVE,		  //����Ƴ�		MouseLeaveHandler
	MSG_MOUSEMOVE,//3     //����ƶ�		MouseMoveHandler
	MSG_MOUSEDOWN,		  //��갴��		MouseDownHandler
	MSG_MOUSEUP,		  //��굯��		MouseUpHandler
	MSG_MOUSEACTIVE,	  //��꼤��		MouseActive
	MSG_MOUSEINACTIVE,	  //��겻����		MouseActive
	MSG_KEYBOARDSHOW,	  //������ʾ		KeyboardShowed
	MSG_KEYBOARDHIDE,	  //��������		KeyboardHidden
	MSG_KEYBOARDORGCHANGED,//�ı���̷���	KeyBoardChanged	
	MSG_KEYDOWN,		   //���¼���		EnterKey
	MSG_BINDTOFRONT,	   //ǰ�ô���		BindToFront
	MSG_BINDTOBACK,		   //���ô���		BindToBack
	MSG_EDITLINKCLICK,//����˱༭������(��������ʱ����)	WndProc(MSG_EDITLINKCLICK,�༭��,����,����)
	MSG_BINDTOTOP,		  //�����ö��¼�    BindToTop
	MSG_MOUSECLICKED,//16  //������ɵ��    ��WndProc�ﴦ��
	MSG_EDITBEYONDMAX,	  //�༭�������������
	MSG_DRAGTOUCH,//18	  //�����϶�		��굯�����жϲ����͸���Ϣ(WPARAM)�϶����ô���֮��,(LPARAM)��Ҫ�϶��Ĵ���
	MSG_LISTVIEWMOVE,	  //ListView�϶��¼�	//WndProc(message/*MSG_LISTVIEWMOVE*/, obj/*��ǰ�϶���LIST�ؼ�*/, wParam/*����϶���λ��*/, lpParam/*LPARAM����,��λ�ؼ��߶�,��λpos.y */)
	MSG_LONGPRESS,		//WndProc�����¼� wPrarm :��λX���� ��λY���꣬lParam:NULL

	MSG_USEREVENT=1000,//�û��Զ��¼�
};

class CWndObject
{
public:
	enum  //������뷽ʽ 
	{
		LAYER_UNKNOWN,
		LAYER_LEFT_TOP,
		LAYER_RIGHT_TOP,
		LAYER_RIGHT_BOTTOM,
		LAYER_LEFT_BOTTOM,
		LAYER_TOP,
		LAYER_RIGHT,
		LAYER_BOTTOM,
		LAYER_LEFT,
		LAYER_CENTER //9 ����
	};

	enum //�������췽ʽ
	{
		BGTYPE_FIX,
		BGTYPE_NINESQUARE,		//1
		BGTYPE_NINESQUAREEX,	//2�Ź���
		BGTYPE_STRETCH,			//3����
		BGTYPE_PLAT,			
	};

	enum //�ؼ���ֱ���뷽ʽ
	{
		VERALIGN_TOP,//0����
		VERALIGN_CENTER,//1����
		VERALIGN_BOTTOM//2����
	};

	enum  //�ؼ�ˮƽ���뷽ʽ
	{
		HERALIGN_LEFT,//0����
		HERALIGN_CENTER,//1����
		HERALIGN_RIGHT//2����
	};
	
	CWndObject();
	virtual ~CWndObject();

	//�Ƿ������Ĵ���CWndApp::m_rootWnd
	static bool IsRoot(CWndObject* lpWnd);

	//��Ļ���
	static int GetScreenWidth();
	
	//��Ļ�߶�
	static int GetScreenHeight();

	//���ƾ��ο�
	static void ShowRect(int x1/*���Ͻ�X����*/,int y1/*���Ͻ�Y����*/,int x2/*���½�X����*/,int y2/*���½�Y����*/,unsigned int color/*��ɫ*/);

	//��������С
	int GetFontSize();

	//���������С
	void SetFontSize(int iSize);

	//����ı���ʾʱ�ĳ���
	static void GetStringWidth(const char* lpStr/*�ı�*/,int iFontSize/*�����С*/,int& outWidth/*��õĿ��*/,int& outHeight/*��õĸ߶�*/,const char* fntName=NULL/*������*/);
	
	//��������ʾͼƬ
	static void DrawImgFix(char* lpAni/*ani�ļ���[]������*/,int frame/*֡��*/,CPoint& cntScreenOrg,CRect& showRect,CRect& cntRect,CPoint& viewOrg);

	//��������ʾͼƬ,�ɿ�����ʾ����
	static void DrawImgFixEx(char* lpAni,int frame,CPoint& cntScreenOrg,CRect& showRect,CRect& cntRect,CPoint& viewOrg,CRect& drawRect/*Ҫ��ʾ�Ĵ�С*/);	

	//��������ʾͼƬ
	static void DrawImgStretch(char* lpAni,int frame,CPoint& cntScreenOrg,CRect& showRect,CRect& cntRect,CPoint& viewOrg);

	//��������ʾͼƬ,�ɿ�����ʾ����
	static void DrawImgStretchEx(char* lpAni,int frame/*֡��*/,CPoint& cntScreenOrg,CRect& showRect,CRect& cntRect,CPoint& viewOrg,CRect& drawRect/*Ҫ��ʾ�Ĵ�С*/);	
	
	//
	static void DrawImgPlat(char* lpAni,int frame,CPoint& cntScreenOrg,CRect& showRect,CRect& cntRect,CPoint& viewOrg);	

	//
	static void DrawImgPlatEx(char* lpAni,int frame,CPoint& cntScreenOrg,CRect& showRect,CRect& cntRect,CPoint& viewOrg,CRect& drawRect);		

	//
	static void DrawImgPlatEx2(CAni *pAni,int frame,CRect& imgSrc,CPoint& cntScreenOrg,CPoint& viewOrg,CRect& drawRect,CRect& showRect,CRect& cntRect);

	//
	static void DrawImgNineSquareEx(char* lpAni,CPoint& cntScreenOrg,CRect& showRect,CRect& cntRect,CPoint& viewOrg,CRect& drawRect);

	//
	static void DrawImgNineSplitSquareEx(char* lpAni,int frame,CPoint& cntScreenOrg,CRect& showRect,CRect& cntRect,CPoint& viewOrg,CRect& drawRect,int borderSize);	
	
	//����ͼƬ
	static void DrawImgWithImageInterfaceEx(IImageObj* lpImg,CRect& frameRect,CPoint& cntScreenOrg,CRect& showRect,CRect& cntRect,CPoint& viewOrg,CRect& drawRect,bool isStretch);
	
	static void CumDrawImgNineSplitSquareEx(char* lpAni,int frame,CPoint& cntScreenOrg,CRect& showRect,CRect& cntRect,CPoint& viewOrg,C3_NINESQUARE_RECT &nineSqrRect);
	
	//
	static void DrawImgNineSquare(char* lpAni,CPoint& cntScreenOrg,CRect& showRect,CRect& cntRect,CPoint& viewOrg);	
	
	//
	static void DrawImgNineSplitSquare(char* lpAni,int frame,CPoint& cntScreenOrg,CRect& showRect,CRect& cntRect,CPoint& viewOrg,int borderSize);	
	
	//
	void DrawImgWithImageInterface(IImageObj* lpImg,CRect& frameRect,CPoint& cntScreenOrg,CRect& showRect,CRect& cntRect/*ͼƬ����Ļ�ϵ�����*/,CPoint& viewOrg,bool isStretch);
	
	//��ȡͼƬ��С
	static void GetImageSize(const char* lpAni/*ani�ļ���[]������*/,int frame/*֡*/,int& iWidth,int& iHeight);
	
	//���Ƶ����ı�
	static void DrawText(const char* lpText/*�ı�*/,CPoint& cntScreenOrg/*��ʼ���Ƶ�ԭ��*/,CRect& showRect,CRect& cntRect/*ini�����õĴ�С*/,CPoint& viewOrg,
				  int verAlign,int herAlign,int offSetX/*��ʾ������X����ƫ��*/,int offSetY/*��ʾ������Y����ƫ��*/,bool isTextRender/*��ǿ������Ч��*/,unsigned int iColor,int iFontSize/*�����С*/,int iWidth, bool isUnderLine=false,bool isBold=false);

	//���ƶ����ı�
	static void DrawMuliText(const char* lpText/*�ı�*/,CPoint& cntScreenOrg,CRect& showRect/*�ڱ��ؼ���ʾ��λ�ô�С*/,CRect& cntRect/*�ڸ����ڵ�λ��*/,CPoint& viewOrg,
					  int verAlign,int herAlign,int offSetX,int offSetY,bool isTextRender,unsigned int iColor,int iFontSize, bool widthSubOffSet=false);
	
	//���ʱ��������
	static void PlayClickSound();
	
	//��ȡ�Ź����С
	static void GetNineSquareRect();

	//����ʱ���ô���λ��
	void Create(CRect& rect/*������λ�ô�С*/,int posLayerType=LAYER_UNKNOWN/*���񲼾ַ�ʽ*/,CWndObject* lpParent=NULL/*������*/);
	
	//�ͷŴ���
	void DestroyWindow();	

	//����(parentScreenPos is the left-top conter of  parent. It is absolute value to screen.	parentDrawRect is the diplay rect for parent. that is relation value.)
	virtual void Paint(CPoint& parentScreenPos,CRect& parentDrawRect);

	//������Ϣ
	bool SendMessage(UINT message,WPARAM wParam, LPARAM lParam);

	//ǰ�ô���(������ǰ����ʾ����(�����Ӵ��ڲ����ڵ�),������MSG_BINDTOFRONT�¼�֪ͨ)
	void BindToFront();

	//������ǰ���ú�
	void BindToBack();
	
	//�ö���ʾ
	void BindToTop(bool bTop/*�Ƿ��ö�*/);
	
	//������ͼƬ���Ƶ�����ͼƬ1
	void PushAni();
	
	//������ͼƬ1���Ƶ�����ͼƬ,�����ޱ���ͼƬ2
	void PopAni();
	
	//��������ͼƬ(���б���ͼƬ2,�򽫱���ͼƬ1���Ƶ�����ͼƬ(PopAni);�������ñ���ͼƬΪ����ͼƬ2)
	void ExchangeAni();
	
	//�¼� 
	MouseEvent EventMouseMove;
	ClickEvent EventClicked;//��갴�µ��¼�
	MouseEvent EventMouseDown;
	MouseEvent EventMouseUp;
	ClickEvent EventMouseEnter;
	ClickEvent EventMouseLeave;
	ClickEvent EventDrag;//�϶�����
	ClickEvent EventQuickMove;

	//���ô���λ��
	void SetClientRect(CRect& rect);

	//��ÿؼ���λ�ô�С
	CRect& GetClientRect();
	
	//����ӽǵ�����ԭ��
	CPoint& GetViewPos();

	//�����ӽǵ�����ԭ��
	void SetViewPos(CPoint& pos);
	
	//���ô��ڵı���ͼƬ
	void SetBgAni(const char* lpAni/*aniͼƬ,��ΪNULL��ɾ��ԭ����ͼƬ*/,int bgFillType=BGTYPE_FIX,int borderSize=0/*���*/);

	//���ô��ڵı���ͼƬ����ָ��ani�ļ�
	void SetBgAniEx(const char* lpAni/*aniͼƬ,��ΪNULL��ɾ��ԭ����ͼƬ*/,const char* lpAniFile, int bgFillType=BGTYPE_FIX,int borderSize=0/*���*/);


	//��ô��ڵı���ͼƬ
	const char* GetBgAni();

	//���ô��ڵı���ͼƬ2(��Ӧini�����ļ����"Ani1")
	void SetBgAni2(const char* lpAni);

	//��ô��ڵı���ͼƬ2
	const char* GetBgAni2();

	//ֱ�����ô��ڵı���ͼƬ(ֱ�Ӱ�Ĭ�����õ���SetBgAni)
	void SetBgAniDirect(const char* lpAni);
	
	//��ñ���ͼƬ�����췽ʽ
	int GetBgFillType() const;
	
	//���ñ���ͼƬ�����췽ʽ
	void SetBgFillType(int bgFillType/*���췽ʽ*/,int borderSize=0);
	
	//����ani����ͼƬ֡��������
	void SetBgFrame(int index);

	//��ñ���ͼƬ֡��������
	int GetBgFrame() const;
	
	//��ȡ�߿���
	int GetBgBorderSize()const;

	//���ñ߿���
	void SetBgBorderSize(int borderSize);
	
	//��ȡ������
	CWndObject* GetParent();
	
	//���ø�����
	void SetParent(CWndObject* lpParent);

	//��ȡִ����갴���¼��Ŀؼ�
	CWndObject* GetNotifyCtrl();

	//����ִ����갴���¼��Ŀؼ�
	void SetNotifyCtrl(CWndObject* lpCtrl);
	
	//���ÿɼ�
	void SetVisible(bool bIsVisible);

	//�Ƿ�ɼ�
	bool IsVisible() const;
	
	//���ô��ڼ���
	void SetActive(bool active);

	//�����Ƿ񼤻�
	bool IsActive() const;
	
	//���ù�����뷽ʽ
	void SetPosLayerType(int posLayType);
	
	//��ù�����뷽ʽ
	int GetPosLayerType() const;

	//���ػ�ȡ�ؼ��ľ����
	int GetHandle() const;

	//��ȡ��Դ��
	virtual int GetTemplateID();

	//������Դ��
	void SetTemplateID(int templateID);

	//������Դ���ҵ��Ӵ����µĿؼ�
	CWndObject* GetDlgItem(int iTemplateID);

	//���Ӵ����б���ͨ������ҵ���Ӧ�Ŀؼ���Ի���
	CWndObject* GetDlgWithHandle(int iHandle/*���*/);

	//�Ƿ���Ϣ͸��
	bool IsMsgTransparent()const;

	//������Ϣ͸��
	void SetMsgTransparent(bool isTransparent=true);	
	
	//�Ƿ���Ϣ��͸
	bool IsMsgThrough() const;
	
	//������Ϣ��͸
	void SetMsgThrough(bool isMsgThrough);
	
	//�Ƿ��ö�
	bool IsTop()const;
	
	//�����ͼ���ܴ�С(�����������е��Ӵ���)
	virtual void GetViewSize(int& iWidth/*���*/,int& iHeight/*�߶�*/);


	//�϶�����
	virtual bool MouseDragHandler(CWndObject* obj/*����*/,int relateX/*X����ƫ��*/,int relateY/*Y����ƫ��*/,const void* lpParam=NULL/*��������*/);

	//�������
	virtual void MouseEnterHandler(CWndObject* obj/*����*/,CPoint& pos/*���λ��*/,const void* lpParam=NULL);
	
	//����뿪
	virtual void MouseLeaveHandler(CWndObject* obj/*����*/,CPoint& pos/*���λ��*/,const void* lpParam=NULL);

	//����ƶ�
	virtual void MouseMoveHandler(CWndObject* obj/*����*/,CPoint& pos/*���λ��*/,const void* lpParam=NULL);

	//��������϶�
	virtual bool MouseDragOverHandler(CWndObject* obj/*����*/,int relateX/*X����ƫ��*/,int relateY/*Y����ƫ��*/,const void* lpParam=NULL);

	//���������
	virtual bool MouseDownHandler(CWndObject* obj/*����*/,CPoint& pos/*���λ��*/,const void* lpParam=NULL);

	//���������
	virtual void MouseUpHandler(CWndObject* obj/*����*/, CPoint& pos/*���λ��*/,const void* lpParam=NULL);

	//��������
	virtual void MouseClickedHandler(CWndObject* obj/*����*/, CPoint& pos/*���λ��*/,const void* lpParam=NULL);
	
	//���������
	virtual void MouseActive(CWndObject* lpUnActive,CWndObject* lpActive,CPoint& pos/*���λ��*/,const void* lpParam=NULL);
	
	//��ָ���������в���ĳһ�����ϵĴ��ڶ���(����Ϣ͸��,����NULL)
	CWndObject* PointInCtrl(CPoint& pos/*����*/,CRect& refRect/*�����С*/);

	//��ȡ�ؼ�������CTRL_WNDOBJ
	virtual int GetType()const;	

	//�������пؼ��Ĳ���(�������µ������Ӵ�������ˢ��һ�²���)
	void SetClientRectWithLayerType(bool isDoSize=true/*�Ƿ��������ô���λ��*/);
 

	//��������ת��Ļ����(����������:�����Ըô���Ϊ����ԭ��)
	static void PointToScreen(CWndObject* lpSrc/*����*/,CPoint& point/*ԭ��������,�õ����ص���Ļ����*/);

	//��Ļ����ת��������
	static void PointToCtrl(CWndObject* lpSrc/*����*/,CPoint& point/*ԭ��Ļ����,�õ����صĴ�������*/);

	//�ؼ�����ת��Ļ����(�ؼ�������:�����Ըÿؼ�Ϊ����ԭ��;����rcΪCRect(0,0,�ؼ����,�ؼ��߶�),�򷵻ص��Ǹÿؼ�����Ļ������)
	static void RectToScreen(CWndObject* lpSrc/*�ؼ�*/,CRect& rc/*����*/);

	//�����Ϸ���ƫ�Ƹÿؼ�����Ļ�����С
	static void RectToCtrl(CWndObject* lpSrc,CRect& rc);
	
	//��ȡ���ڵ�������(�������ڵĸ���������Ϸ����)
	static CWndObject* GetOwnerDlg(CWndObject* lpCtrl);

	//��ȡ��������ཻ����( ���ཻ����Ϊ���򷵻�false;����GetCutRect����)
	static bool RectIntersect(CRect& src1 /*����1*/,CRect& src2/*����2*/,CRect& resultRec/*�ཻ����*/);

	//���¼���ؼ��Ĳ��ִ�С
	static void ReCalauSizeWithLayer(int layerType,CRect& rect/*Ҫ���õĿؼ���С*/,int iWidth/*�ؼ����*/,int iHeght/*�ؼ��߶�*/,int iPWidth/*�����ڿ��*/,int iPHeight/*�����ڸ߶�*/);	
	
	//��ȡ������ü���С,���ཻ(��ͬӵ��)�������С(���ཻ����ƫ��һ��������СС��0,�򷵻�false;����RectIntersect����)	
	static bool GetCutRect(CRect& imgRect/*����1*/,CRect& showRect/*����2*/,CPoint& viewOrg/*ƫ������*/,CRect& resultRect/*�ཻ����*/); 


	//�����¼�����(Ĭ�����ϲ�ݹ��ɷ�,see:MSG_XXX)
	void DispatchRef(int iType/*����*/,CWndObject* obj/*����Ŀؼ�*/,CPoint& pos/*λ��*/,const void* lpParam=NULL);

	//�������ں��Ӵ����¼�(SendMessage������¼�;�Զ����¼���Ϣ����ֵ����ȡ1-18��ϵͳ�¼�,ȡMSG_USEREVENT���ϵ��¼�)
	virtual bool WndProc(CWndObject* pObj/*������Ϣ�Ĵ���*/, UINT nMessage/*�¼���Ϣ��*/,WPARAM wParam/*����ϵͳ�¼���Ϊ����*/, LPARAM lParam);

	//����Ӵ�������
	LIST_CTRL& GetChild();

	//�����ͼ���ܴ�С(�����������е��Ӵ���)
	virtual void GetViewRect(CRect& size); 


	//����ǰ����ʾ
	void BindToFrontEx();

	//
	void BindToBackEx();

	//
	void BindToFront(CWndObject* lpCld);

	//�����ú���ʾ
	void BindToBack(CWndObject* lpCld);

	//ɾ��һ���Ӵ���,lpChildΪҪ���ҵ��Ӵ���
	void RemoveCld(CWndObject* lpChild);

	//ɾ��ȫ���Ӵ���
	void RemoveAllCld();

	//����һ���Ӵ���
	void AddCld(CWndObject* lpChild);

	//
	void BindToTopEx(bool bTop);

	//
	void BindToTop(CWndObject* lpCld,bool bTop);

#if _DEBUG
	static bool IsDrawDebugRect();
#endif

	//���������϶��Ĵ���
	virtual void PaintDragTouchImage(CPoint& posDragTouch/*����*/,CRect& rcScreen);

	//�����Ƿ������϶�(if the obj EnableDragTouch flag is true,the obj's parent can reciev)
	virtual void EnableDragTouch(bool bDragTouchEnable);

	//�Ƿ������϶�
	virtual bool IsDragTouchEnable();

protected:

	//�ؼ������󴥷�
	virtual void DoCreated();

	//��Ҫ�ı䴰�ڴ�С(��SetClientRect֮ǰ����,����ʧ����ִ��SetClientRect)
	virtual bool DoSizing(CRect& rect/*�ı��Ĵ��ڴ�С*/);

	//�ı䴰�ڴ�С��(SetClientRect�󴥷�)
	virtual void DoSized(CRect& rect/*�ı��Ĵ��ڴ�С*/);

	//only used CTRL_DLG 
	virtual void DoActive(CWndObject* lpPreActive);

	virtual void DoInActive(CWndObject* lpActive);

	//�ı䴰���Ƿ���ʾ�󴥷�
	virtual void DoShow(bool isShow/*�ı���Ƿ���ʾ*/);

	//�ı��ӽǵ�����ԭ���¼�(SetViewPos�󴥷�)
	virtual void DoViewPos(CPoint& pos);

	//�ͷ���Դ
	virtual void DoDestroy();
	
	//���ƴ��ڱ���
	virtual void DoPaintBackground(CPoint& cntScreenOrg/*��ʼ���Ƶ�ԭ��*/,CRect& showRect/*��ԭͼƬ����ʾ�Ĵ�С*/,CRect& cntRect/*ͼƬ����Ļ�ϵ�����*/,CPoint& viewOrg/*��ͼԭ��*/);

	//���ƴ��ڵ�ǰ��(����ʾ�ı�,��ʾ�Ի����ϵĶ�������)
	virtual void DoPaintForeground(CPoint& cntScreenOrg,CRect& showRect,CRect& cntRect,CPoint& viewOrg/*��ͼԭ��*/);

	//���ƻ���
	virtual void DoPaintSlider(CPoint& cntScreenOrg,CRect& showRect,CRect& cntRect,CPoint& viewOrg);	
protected:
	int m_iFontSize;//�����С
	bool m_bDragTouchEnable;//�����϶�(�Ȱ������϶�)
	CPoint m_posDragOffset;//�϶�ʱ��λ�������ԭ�ؼ�λ�õ�ƫ�ƾ���
	bool m_isVisible;//�Ƿ���ʾ
	//wstring m_strCaption;

private:
	static C3_NINESQUARE_RECT m_NineSquareRect;//�Ź���
	CRect m_rect; //�ؼ���λ�ô�С
	int m_posLayerType;//������뷽ʽ(ini�ļ����"layertype")
	CPoint m_viewPos;//�ӽǵ�����ԭ��
	
	CWndObject* m_lpParent;//������
	CWndObject* m_lpNotifyCtrl;//��͸����:�󶨵�ִ����갴���¼��Ŀؼ�(�����ؼ���ͬʱ������Ϣ)
	
	bool m_isActive;//�����Ƿ񼤻�
	LIST_CTRL m_lstChld;//�Ӵ����б�(˫������)
	int m_handle;//���(ÿ���ؼ���ӵ��Ψһ��һ�������)
	
	char* m_lpBgAni;//����ͼƬ
	int m_bgFillType;//�������췽ʽ
	int m_bgFrame;//����ani�ļ���ͼƬ֡id��(enNormal,enDown,enDisable,enActive)
	int m_templateID;//ÿ���Ի����Ի������ԴID
	int m_iNineBorderSize;//�߿���
	bool m_bMsgTransparent;//��Ϣ͸��(��:��ָ���������в���ĳһ�����ϵĴ��ڶ���,����Ϣ͸��,��ȡ������;Ĭ��false)

	bool m_bMsgThrough;//��Ϣ��͸,Ĭ��false;��ָ���������в���ĳһ�����ϵĴ��ڶ���,����Ϣ��͸,���ٽ���ô��ڵ��Ӵ��ڲ���
	
	char* m_lpBKAni;//����ͼƬ1
	char* m_lpAni2;//����ͼƬ2(����ͼƬ,Ϊ���л�ͼƬ)
	bool m_bAni2;//�б���ͼƬ2
	bool m_isTop;//�Ƿ���������(�ؼ��򴰿ڲ����ڵ�)
};

#endif 