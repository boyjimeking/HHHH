#ifndef __HTMLHELPER_HH__
#define __HTMLHELPER_HH__
/*֧��HTML��������ʾ
*/

#include "ParserDom.h"
#include "WndObject.h"
#include "DataConvert.h"

using namespace std;
#define _DEFAULT_FONT_SIZE 12

#ifndef _WIN32
/*
 * Edit Control Styles
 */
// ˮƽ����/����/����
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

//��ֱ����/����/����(ͨ��SetStyle(ES_RIGHT|ES_VTOP)����
#define ES_VTOP				 0x4000L
#define ES_VCENTER           0x8000L
#define ES_VBOTTOM           0x10000L

//HTML����
typedef struct sHtmlObj
{
	unsigned int m_color;//��ɫ
	unsigned int m_size;//�����С
	bool m_isLink;//�Ƿ�������
	//CWndObject* m_lpObj;
	char* m_lpHref;//�����ı�
	char* m_lpText;//��ʾ�ı�
	sHtmlObj()
	{
		m_size=_DEFAULT_FONT_SIZE;
		m_color = _DEFAULT_FONT_COLOR;
		m_isLink = false;
		m_lpText = NULL;
		//m_lpObj = NULL;
		m_lpHref = NULL;
	};
}HtmlObj,*LPHtmlObj;

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
	int iLine;//�����к�
	tagVisibleItem();
}VisibleItem,*LPVisibleItem;//���ӵ��ı�����


typedef vector<LPTextItem> VECTEXT;//�ַ���
typedef vector<LPTextAtt> VECTEXTATT;//��������
typedef vector<LPLineAtt> VECLINE;//һ���ַ���
typedef vector<LPLinkAtt> VECLINK;
typedef vector<LPVisibleItem> VECVISI;

typedef vector<LPHtmlObj> VEC_HTML;//html��������

class CTextRender
{
public:
	CTextRender(void);
	~CTextRender(void);
	
	//��ȡ�����ı���html����(m_objs)
	VEC_HTML& GetObjs();

	//����������ı�
	void ClearText();

	//��ȡ�ı�
	const char* GetText(string& strGet/*��ȡ���ı�*/);

	//�����ı�
	bool AddText(const char* lpText, int nFontColor = _DEFAULT_FONT_COLOR/*��ɫ*/, int nFontSize = _DEFAULT_FONT_SIZE/*����*/);//,bool bIsLink=false,const char* lpHref=NULL);

	//����HTML�ı�
	bool AddHtmlText(const char* lpTextHtml,int defFontColor=_DEFAULT_FONT_COLOR/*Ĭ����ɫ*/,int defFontSize=_DEFAULT_FONT_SIZE/*Ĭ������*/);

	void Paint(CRect* rcShow=NULL);//CPoint& cntScreenOrg,CRect& showRect,CRect& cntRect,CPoint& viewOrg);


	void SetPaintWnd(CWndObject* pWnd);


	void SetRect(const CRect& rtPaint, bool bRelative = false);


//protected:
	//html����(�ڲ�����)
	void HtmlParse(const char* str,int defaultFontColor=_DEFAULT_FONT_COLOR,int defaultFontSize=_DEFAULT_FONT_SIZE);


	//��UTF8����html����(�ڲ�����)
	void HtmlParseWithUTF8(const char* str,int defaultfontColor=_DEFAULT_FONT_COLOR,int defaultFontSize=_DEFAULT_FONT_SIZE);

	//����HTML����
	void HtmlCreateObjs(tree<htmlcxx::HTML::Node>& dom,int defaultFontColor=_DEFAULT_FONT_COLOR/*Ĭ����ɫ*/,int defaultFontSize=_DEFAULT_FONT_SIZE/*Ĭ������*/);

	//html����(�ڲ�����)
	void HtmlPreProcess(string& str/*�滻ǰ*/,string& retStr/*�滻��*/,const char* reg/*Ҫ�滻���ı�*/,const char* lpReplace/*�滻�ɵ��ı�*/);
	
	CDataConvert* GetDataConvert();

	void SetShowRect(CRect& val);

protected:
	VEC_HTML m_objs;//�����ı���html����
	CDataConvert m_dataConvert;

	CRect m_rcShow;
	//CRect GetShowRect() const { return m_rcShow; }

	bool m_bRelative;

	CWndObject* m_pWnd;
	//VECVISI m_vecVisi;//���ӵ��ı��б�

	DWORD m_dStyle;//��ʽ(Ĭ��ES_CENTER|ES_VCENTER)

};

#include "CtrlStatic.h"

//֧��HTML��CCtrlStatic
class CCtrlStaticEx :public CCtrlStatic
{
public:
	//�����ı�
	void SetHtmlWindowText(const char* lpText/*�ı�*/,int fontColor=_DEFAULT_FONT_COLOR/*Ĭ����ɫ*/,int defFontSize=_DEFAULT_FONT_SIZE/*Ĭ������*/);

	//�����ı�
	void SetHtmlWindowTextWithUTF8(const char* lpText/*�ı�*/,int fontColor=_DEFAULT_FONT_COLOR/*Ĭ����ɫ*/,int defFontSize=_DEFAULT_FONT_SIZE/*Ĭ������*/);

	virtual const char* GetWindowText();

	CTextRender* GetTextRender();
protected:
	virtual  void DoPaintForeground( CPoint& cntScreenOrg,CRect& showRect,CRect& cntRect,CPoint& viewOrg );
protected:
	CTextRender m_textRender;

};

#include "CtrlButton.h"

//֧��HTML��CCtrlStatic
class CCtrlButtonEx :public CCtrlButton
{
public:
	//�����ı�
	void SetHtmlWindowText(const char* lpText/*�ı�*/,int fontColor=_DEFAULT_FONT_COLOR/*Ĭ����ɫ*/,int defFontSize=_DEFAULT_FONT_SIZE/*Ĭ������*/);

	//�����ı�
	void SetHtmlWindowTextWithUTF8(const char* lpText/*�ı�*/,int fontColor=_DEFAULT_FONT_COLOR/*Ĭ����ɫ*/,int defFontSize=_DEFAULT_FONT_SIZE/*Ĭ������*/);

	virtual const char* GetWindowText();

	CTextRender* GetTextRender();
protected:
	virtual  void DoPaintForeground( CPoint& cntScreenOrg,CRect& showRect,CRect& cntRect,CPoint& viewOrg );
protected:
	CTextRender m_textRender;

};






#endif //__HTMLHELPER_HH__
