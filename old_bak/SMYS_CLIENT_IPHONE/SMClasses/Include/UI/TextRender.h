#ifndef __HTMLHELPER_HH__
#define __HTMLHELPER_HH__
/*֧��HTML��������ʾ
*/

#include "ParserDom.h"
#include "WndObject.h"
#include "DataConvert.h"

using namespace std;
#define _DEFAULT_FONT_SIZE 12

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

typedef vector<LPHtmlObj> VEC_HTML;//html��������

// typedef map<int,string> ColorMap;

class CTextRender
{
public:
	CTextRender(void);
	~CTextRender(void);
	
	//��ȡ��ɫ��
	//void GetColorMap(int index, string& colorStr);

	//������ɫ��(��ID2����ɫ��0xff0000,�Ϳ���[2#��ʾ����ɫ,������д<font color=\"0xff0000\"/>;�˷�����Ӧ�Ľ�������#],������</font>;��ʹ�ô˷�������ͬʱ����size)
//	void SetColorMap( ColorMap& ClrMap );

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

	//html�����滻����ɫ(�ڲ�����)
	//void HtmlReplaceColor(string& str,string& colorStr);

	//html������ɫ(�ڲ�����)
	//void HtmlPreProcessColor(string& str,string& retStr);

	//html����(�ڲ�����)
	void HtmlPreProcess(string& str/*�滻ǰ*/,string& retStr/*�滻��*/,const char* reg/*Ҫ�滻���ı�*/,const char* lpReplace/*�滻�ɵ��ı�*/);
	
	CDataConvert* GetDataConvert();

	void SetShowRect(CRect& val);

protected:
	VEC_HTML m_objs;//�����ı���html����
//	ColorMap g_ColorIndex;//��ɫ��,��ID12����ɫ��0xff0000,�Ϳ���[12#��ʾ����ɫ,������д<font color=\"0xff0000\"/>
	CDataConvert m_dataConvert;

	CRect m_rcShow;
	//CRect GetShowRect() const { return m_rcShow; }

	bool m_bRelative;

	CWndObject* m_pWnd;
	//VECVISI m_vecVisi;//���ӵ��ı��б�


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





#endif //__HTMLHELPER_HH__
