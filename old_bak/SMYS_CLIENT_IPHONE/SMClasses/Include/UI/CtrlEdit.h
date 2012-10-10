/*
 *  CtrlEdit.h
 *  Created by ndtq on 11-2-22.
 *  Copyright 2011 TQ Digital Entertainment. All rights reserved.
 */

#ifndef __CTRL_EDIT_H__
#define __CTRL_EDIT_H__

#include <vector>
#include <string>
#include "CtrlView.h"
#include "CtrlSlider.h"
#include "DataConvert.h"
#include "TextRender.h"

using namespace std;

//mac os
//keyboard type��������
enum
{
    KT_UIKeyboardTypeDefault,
    KT_UIKeyboardTypeASCIICapable,
    KT_UIKeyboardTypeNumbersAndPunctuation,
    KT_UIKeyboardTypeURL,
    KT_UIKeyboardTypeNumberPad,     //iPad
    KT_UIKeyboardTypePhonePad,		//iPhone
    KT_UIKeyboardTypeNamePhonePad,  //iPhone & iPad
    KT_UIKeyboardTypeEmailAddress
};

//clear btn mode
enum
{
    KT_UITextFieldViewModeNever,
    KT_UITextFieldViewModeWhileEditing,
    KT_UITextFieldViewModeUnlessEditing,
    KT_UITextFieldViewModeAlways
};

//�༭�򷵻�����(return type)
enum
{
    KT_UIReturnKeyDefault,
    KT_UIReturnKeyGo,
    KT_UIReturnKeyGoogle,
    KT_UIReturnKeyJoin,
    KT_UIReturnKeyNext,
    KT_UIReturnKeyRoute,
    KT_UIReturnKeySearch,
    KT_UIReturnKeySend,
    KT_UIReturnKeyYahoo,
    KT_UIReturnKeyDone,
    KT_UIReturnKeyEmergencyCall,
};
//end mac os

//#define _DEFAULT_CURSOR_COLOR 0xffd2c2b2
#define _DEFAULT_CURSOR_COLOR 0xffffffff
#define _DEFAULT_FONT_BG_COLOR 0xff000000
#define _DEFAULT_CURSOR_SIZE 2

#define _MAX_LENGTH_PASSWORD 256
#define _PWD_SHOW_CHAR '*'
extern char g_PwdShowStr[_MAX_LENGTH_PASSWORD];

//һ���ֵ�����
typedef struct tagTextAtt
{
	char* m_fntName;//��������
	int m_fntSize;//���ָ߶�
	int m_fntColor;	//������ɫ
	int m_iRef; //���ּ���
	tagTextAtt();
}TextAtt,*LPTextAtt;//һ���ֵ�����

//һ���ı�
typedef struct tagLineAtt
{
	int m_iStart;//��ʼλ��
	int m_iEnd;//����λ��
	int m_iHeight;//һ�еĸ߶�
	int m_iWidth;
	tagLineAtt();
}LineAtt,*LPLineAtt;//һ���ı�

//������
typedef struct tagLinkAtt
{
	bool m_isClick;//�Ƿ�����
	char* m_lpHref;//�����ӱ�����ı�(��:event:player-1411;����ʾ���ı�)
	int m_iRef;
	unsigned int m_clrNormal;//������ɫ
	unsigned int m_clrClicked;//������������ɫ
	tagLinkAtt();
}LinkAtt,*LPLinkAtt;//������

typedef struct tagTextItem
{
	bool m_isObj;//�Ƿ�Ϊ�ؼ�
	bool m_isAutoDestory;//�Ƿ��Զ�����
	CWndObject* m_lpObj;//�ؼ�
	LPLinkAtt m_lpLinkAtt;//������
	char m_char[4];//һ����
	int m_iHeight;//һ���ֵĸ߶�
	int m_iWidth;//һ���ֵĿ��
	LPTextAtt m_lpAtt;	//һ���ֵ�����
	tagTextItem();
}TextItem,*LPTextItem;//һ����

//���ӵ��ı�����
typedef struct tagVisibleItem
{
	CRect m_Rect;//��ʾ����
	CRect m_selRect;//ѡ�е�����(Ҫ������)
	string m_str;//��ʾ�ı�
	LPTextAtt m_lpAtt;//һ���ֵ�����
	LPLinkAtt m_lpLink;//������
	int m_itemCount;//���ָ���
	tagVisibleItem();
}VisibleItem,*LPVisibleItem;//���ӵ��ı�����


typedef vector<LPTextItem> VECTEXT;//�ַ���
typedef vector<LPTextAtt> VECTEXTATT;//��������
typedef vector<LPLineAtt> VECLINE;//һ���ַ���
typedef vector<LPLinkAtt> VECLINK;
typedef vector<LPVisibleItem> VECVISI;

#if 0
#ifndef _WIN32
/*
 * Edit Control Styles
 */
#define ES_LEFT             0x0000L
#define ES_CENTER           0x0001L
#define ES_RIGHT            0x0002L
#define ES_MULTILINE        0x0004L
// #define ES_UPPERCASE        0x0008L
// #define ES_LOWERCASE        0x0010L
#define ES_PASSWORD         0x0020L
// #define ES_AUTOVSCROLL      0x0040L
// #define ES_AUTOHSCROLL      0x0080L
// #define ES_NOHIDESEL        0x0100L
// #define ES_OEMCONVERT       0x0400L
#define ES_READONLY         0x0800L
// #define ES_WANTRETURN       0x1000L
// #define ES_NUMBER           0x2000L
#endif
#endif
/**
  �༭��
**/
class CCtrlEdit:public CCtrlView
{
public:
    CCtrlEdit();
    virtual ~CCtrlEdit();

    //��ȡĳһ�е��ı�
    bool GetLineString(int iLine/*�к�*/,string& ret/*�ı�*/);

    //��ȡ��ǰ������ڵ���
    int GetCurSelLine();

    //����Ƶ�ĳһ��
    void SetLineSelect(int iLine);

    //
    bool GetLinePos(int iLine, int& nBegin, int& nEnd);


    //����ĳһ����(�Ըñ༭��Ϊ�ӽ�ԭ��)��ȡ����λ�����
    int GetCharIndexWithCntPoint(CPoint& pos);

    //
    void GetCntPointWithCharIndex(int index,CPoint& resPoint);

    //��ȡ���ӵ�����������
    void GetVisibleLineRange(int& first/*��ʼ��*/,int& last/*������*/,CRect& rect/*����*/);

    //��ÿؼ����� CTRL_EDIT
    virtual int GetType()const;

    //��ձ༭���ı�
    void Clear();

    //�Ƿ�ֻ��
    bool  IsReadOnly() const;

    //����ֻ������
    void SetReadOnly(bool readonly);

    //�Ƿ��ý���
    bool IsFocus() const;

    //���û�ȡ����
    void SetFocus(bool bFocus);

    //Get item length,include object. Not equal to strlen(GetWindowText());
    int GetLength()const;

    //��ȡ�ı�
    const char* GetWindowText();

    //�����ı�
    void SetWindowText(const char* lpText);

    //�������������ӵ��ı�
    void SetSpecialWindowText(const char* str);

    //��UTF8�����������������ӵ��ı�
    void SetSpecialWindowTextWithUTF8(const char* lpText);

    //��UTF8���������ı�
    void SetWindowTextWithUTF8(const char* lpText);

    //��ȡ���������ʾ���ı�ֵ�ĳ���
    int GetTextLength() const;

    //�Ƿ����
    bool IsMultiLine()const;

    //�����Ƿ����
    void SetMultiLine(bool isMultiLine);

    //��ȡ������
    int GetLineCount() const;

    //��ȡĳһ�еĿ�ʼλ��
    int GetLineCharIndex(int iLine) const;

    //����������ɫ
    void SetFontColor(unsigned int color);

    //��ȡ������ɫ
    unsigned int GetFontColor() const;

    //��������߶�
    void SetFontSize(int size);

    //��ȡ����߶�
    int GetFontSize() const;

    //������������
    void SetFontName(const char* fntName);

    //��ȡ��������
    const char* GetFontName();

    //����ѡ��ʱ�ı�����ɫ(bgColor must include alpha value begin head)
    void SetSelBgColor(unsigned int bgColor);

    //�Ƿ�����������
    bool IsPassword() const;

    //�����Ƿ���������
    void SetPassword(bool isPassWord);

    //��ȡ��ʼѡ�е�λ��
    int GetSelStart()const;

    //��ȡѡ�е��ı�����
    int GetSelLen()const;

    //��ȡ�ı�(m_vecText)
    VECTEXT& GetTextItems();

    //��ȡĳһ��ŵĳ������ı�,��������޳�����,�򷵻�NULL(�����:��N������)
    const char* GetHref(int index/*���*/);

    //����ĳһ��Ż�ȡ�������ı�
    //����ֵ:�Ƿ��ȡ�ɹ�
    bool GetLinkText(int index/*���*/,char* lpRec/*�������ı�*/,int iSize/*�������ı�����󳤶�*/);

    //��ȡ�ı���ʾ�Ĵ�С
    void GetContentSize(int& iWidth/*��*/,int& iHeight/*��*/);

    //�����Ƿ���������������
    void SetSpecial(bool isSpecial);

    //��ȡ�Ƿ���������������
    bool GetSpecial()const;

    //�����������������
    void SetMaxLen(int iLenMAx);

    //��ȡ�������������
    int GetMaxLen();

    //���������ĳ���
    int GetInputLength();

    //����������ĳ���
    void SetInputLength(int iLen);

    //��һ������(������ֵ��С)
    void BindSlider(CCtrlSlider* pBindSlider);

    //��ȡ�󶨵Ļ���
    CCtrlSlider* GetBindSlider();

    //string operator

    //����ѡ��
    void SetSel(int iStart/*��ʼλ��*/,int iEnd/*����λ��,-1�����ı�����λ��*/);

    //�滻�ı�
    void Replace(const char* str/*�ı�*/,unsigned int fontColor=_DEFAULT_FONT_COLOR/*��ɫ*/,int fontSize=0/*�����С*/,const char* fontName=NULL/*������*/,int iType=0/*ɾ������*/);

    //�滻�ɿؼ�
    void ReplaceWithObj(CWndObject* lpObj/*�ؼ�*/,bool bAutoDestory=true/*�Զ�����*/);

    //�滻
    void ReplaceWithUTF8(const char* str/*�ı�*/,unsigned int fontColor=_DEFAULT_FONT_COLOR,int fontSize=0,const char* fontName=NULL);

    //�滻�ɳ�����
    bool ReplaceLink(const char* str/*��ʾ���ı�*/,const char* lpHref/*�����ӱ�����ı�*/,unsigned int fontColor=_DEFAULT_FONT_COLOR/*������ɫ*/,
                     unsigned int clrClicked=_DEFAULT_FONT_COLOR/*������������ɫ*/, int fontSize=0/*�����С*/,const char* fontName=NULL/*������*/);

    //��UTF8�����滻�ɳ�����
    bool ReplaceLinkWithUTF8(const char* str,const char* lpHref,unsigned int fontColor=_DEFAULT_FONT_COLOR,
                             unsigned int clrClicked=_DEFAULT_FONT_COLOR, int fontSize=0,const char* fontName=NULL);

    //��html�滻�ı�(<font color=\"0xff0000\" size = 22/>��<a href='event:player-11'>����</a>��<br/>)
    void ReplaceWithHtml(const char* str/*�ı�*/,int fontColor=_DEFAULT_FONT_COLOR/*Ĭ����ɫ*/,int defFontSize=0/*Ĭ������*/);

    //��UTF8�����html�滻�ı�
    void ReplaceWithHtmlUTF8(const char* str,int fontColor=_DEFAULT_FONT_COLOR,int defFontSize=0/*Ĭ������*/);

    CTextRender* GetTextRender();

    //special key
    void BackspaceKey();

    void DelKey();

    void EnterKey();

    void OnBeyongMaxLen();

    //for mac  os
    //�ı���̷���
    void KeyBoardChanged(int iOrg);

    //��ȡ��������
    int GetKeyboardType()const;

    //���ü�������
    void SetKeyboardType(int ktype);

    //clear btn mode
    int GetClearBtnMode()const;

    void SetClearBtnMode(int mode);

    //��÷�������
    int GetReturnType()const;

    //���÷�������
    void SetReturnType(int retType);

    //key board event

    //���ɼ�����ʾ�¼�MSG_KEYBOARDSHOW
    virtual void KeyboardShowed(int iLeft,int iTop,int iRight,int iBottom);

    //���ɼ��������¼�MSG_KEYBOARDHIDE
    virtual void KeyboardHidden();

    //�༭��ı��ı�
    ClickEvent EventChanged;

    //��������(�������뷨)
    ClickEvent EventInputEnd;

    //�������
    CommonEvent EventDone;

    //event
    virtual void MouseClickedHandler(CWndObject* obj, CPoint& pos,const void* lpParam=NULL);

    //������Сֵ
    void SetMinValue(int iMin);

    //�������ֵ
    void SetMaxValue(int iMax);

    //��ȡ��Сֵ
    int GetMinValue();

    //��ȡ���ֵ
    int GetMaxValue();

    //�������
    void OnEditCtrlDone();

    //��ȡĳ������ʾ�ĸ߶�
    int GetLineHeight(int startIndex/*��ʼ��*/,int endIndex/*������*/);

	//��ȡ�������
	int GetMaxLine() const;

	//�����������
	void SetMaxLine(int iMaxLine);

protected:

    virtual void DoPaintForeground(CPoint& cntScreenOrg,CRect& showRect,CRect& cntRect,CPoint& viewOrg);

    virtual void DoCreated();

    virtual void DoSized(CRect& rect);

    virtual void DoViewPos(CPoint& pos);

    virtual void GetViewRect(CRect& rc);

    //������������
    bool SetLinkAtt(int iStart/*�ı���ʼλ��*/,int iLen/*�����ӵ��ı�����*/, const char* lpHref,unsigned int fontColor=_DEFAULT_FONT_COLOR,
                    unsigned int clrClicked=_DEFAULT_FONT_COLOR, int fontSize=0,const char* fontName=NULL);

    //ɾ������(adjust is destory link att)
    void AdjustDestoryLink(int iStartSel/*��ʼλ�����*/,int iType=0/*ɾ������*/);

    //help
    VECTEXTATT& GetAttItems();

    //�����ı�������
    int Length() const;

    LPTextAtt GetAttByIndex(int index);

    //����Ĭ������
    void CreateDefaultFontAtt();

    //clear function
    void ClearAtt();

    void ClearItem(VECTEXT* lpVec);

    void ClearLine();

    //��������ı�
    void ClearVisible();

    //������г�����
    void ClearLink();

    //size function
    void ResetViewSize();

    void CaluViewSize();


    //��ȡ��ʾ�Ŀ��
    int GetLineWidth(int iStart/*�ı���ʼλ��*/,int iEnd/*�ı�����λ��*/);

    //��ȡһ�е��ı���ʾ�Ŀ��
    int GetLineWidth(LPLineAtt lpAtt/*һ�е��ı�*/);

    //string cut function

    //�ؽ������ı�
    void RebuildLine(int startLine/*��ʼ������*/);

    //ɾ�������ı�
    void CutLineFromIndex(int startLine/*��ʼ������*/);

	//�ؽ�һ���ı�������ʾ
    int  RebuildSingleLine(LPLineAtt lpLine,int iStartChar,int iEndChar,int cntWidth/*��ʾ���*/,CPoint& lineOrgPoint);

    //point function

    //������Ļ�����ȡ����λ�����
    int GetCharIndexWithPoint(CPoint& pos,CRect& realPos,bool& bInRange);

    //
    void GetPosWithCharIndex(int index,CPoint& pos);

    //����λ����ŵõ��к�
    int GetLineWithCharIndex(int index/*λ�����*/);


    //��������
    int GetLineOrgPosWithCharIndex(int index,CPoint& pos);

    //str function
    bool DeleteChar(int index,int endIndex);

    //�����ı�
    int Append(int index/*λ��*/,const char* str/*�ı�*/,int fontSize=0/*�����С*/,const char* fontName=NULL/*������*/,unsigned int fontColor=
                   _DEFAULT_FONT_COLOR/*��ɫ*/);


    void SplitChinaStr(const char* lpStr,int fontSize,const char* fontName,VECTEXT& vecRet,LPTextAtt lpTextAtt);

    //��ȡ��ʾ�ı��ĳ���
    int GetTextLength();

    void GetWindowText(string& str/*��õ��ı�*/);

    //adjust link
    int LastIndexWithLink();

    //��ȡĳ�γ��ȵ��ı�(range:[start,end])
    void GetString(int iStart/*��ʼλ��*/,int iEnd/*����λ��,����С���ı��ĳ���*/,string& ret/*�ı�*/);

    //cursor function
    void RebuildCursor();

private:
    //for malloc
    void ReleaseTextItem(LPTextItem lpItem);

    void ReleaseTextAtt(LPTextAtt lpAtt);

    //�ͷ�һ�е��ı�
    void ReleaseLineAtt(LPLineAtt lpAtt/*һ���ı�*/);

    //����������ı�
    void ReleaseLinkAtt(LPLinkAtt lpAtt);

    //�ͷſ��ӵ��ı�
    void ReleaseVisibleItem(LPVisibleItem lpVisible);

    //�ı���ų���
    bool OutOfRange(int index);

    //�ı�����ų���
    bool OutOfLineRange(int index/*�к�*/);

    int GetSameEndIndex(LPTextAtt lpSameAtt/*�ı�����*/,LPLinkAtt lpLinkAtt/*����������*/, int startIndex/*��ʼ��*/,int endIndex/*������*/);

    void GetItemVisAtt(int iLineIndex/*��*/,int startIndex,int endIndex,CRect& retRect,CRect& selRect,string& retStr);

    bool CompareToItem(LPTextItem lpItem,const char*& lpStr);

    void OnHtmlParse();

    //for mac os
    int m_keyboardType;//��������(keyboard type,Ĭ��:KT_UIKeyboardTypeDefault)
    bool m_isPassword;//����
    int m_returnType;//��������
    int m_clearType;//�������
    bool m_bSpecialLink;//�Ƿ���������������(Ĭ��false)
    //end

    CRect m_viewSize;
    int m_iMaxWidth;//�ı���ʾ�Ŀ��
    int m_iMaxHeight;//�ı���ʾ�ĸ߶�
    VECTEXT m_vecText;//������ı�
    VECTEXTATT m_vecTxtAtt;//��������
    VECLINE m_lineAtt;//ÿһ�е��ı���ɵ�����
    VECLINK m_linkAtt;
    int m_iSelLen;//ѡ�е��ı�����
    int m_iSelStart;//��ʼѡ�е�λ�����(��ȫ���ı���һ���ַ���ʼ����)
    TextAtt m_defaultAtt;//Ĭ�ϵ�һ���ֵ�����
    int m_iCharSpace; //�ּ��(Ĭ��0)
    int m_iLineSpace;//�м��(Ĭ��4)

    //not handle
    bool m_bFocus;//�Ƿ�õ�����
    bool m_bReadOnly;//�Ƿ�ֻ��
    bool m_isMulLine;//�Ƿ����
    unsigned int m_bgSelColor;//ѡ��ʱ�ı�����ɫ(Ĭ�Ϻ�ɫ)

    //cursor
    unsigned int m_iCurTicked;
    bool m_bCurShow;

    CPoint m_curPos;
    int m_iCurHeight;//���ָ߶�(Ĭ��12)

    int m_iFirstVisLine;//�������е�����
    int m_iLastVisLine;//����ĩ�е�����
    CRect m_iVisibleRect;//��������
    VECVISI m_vecVisi;//���ӵ��ı��б�

    string m_text;	//���������ʾ���ı�ֵ
    int m_iMaxLen;//�������������(Ĭ��0,������)
	int m_iInputLen;//������ĳ���
    int m_iMaxLine;//�����������(Ĭ��300)

    int m_iMinVal;//��Сֵ(�༭�򲻿��ƴ�С,ֻ�������ֵ)
    int m_iMaxVal;//���ֵ(�༭�򲻿��ƴ�С,ֻ�������ֵ)

    CCtrlSlider* m_pBindSlider;//Ҫ�󶨵Ļ���(������ֵ��С)

    int m_CurCharIndex;//������ڵ�λ�����(��ʼ-1)

    CTextRender m_textRender;
    CDataConvert m_dataConvert;
 	//DWORD m_dStyle;
 	//DWORD GetStyle() const { return m_dStyle; }
 	//void SetStyle(DWORD val) { m_dStyle = val; }
};

#endif
