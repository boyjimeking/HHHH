/*
 *  ParseHtml.h
 *  ����HTML
 */

#ifndef __HTML_IMPL_H__
#define __HTML_IMPL_H__

#include <string>
#include "ParserDom.h"
#include "CtrlEdit.h"



using namespace htmlcxx;
using namespace HTML;
using namespace std;
/*
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
		m_size=0;
		m_color = _DEFAULT_FONT_COLOR;
		m_isLink = false;
		m_lpText = NULL;
		//m_lpObj = NULL;
		m_lpHref = NULL;
	};
}HtmlObj,*LPHtmlObj;

typedef vector<LPHtmlObj> VEC_HTML;

typedef map<int,string> ColorMap;
*/

/**
��Сд������,/�ſ��п��޿ɶ��
	��ʽ��ǰ		��ʽ����
	[����#			<font color=\"��ɫ\"/>	���ó�IDΪ�����ֵ������
	#]��</font>     <fontEnd></fontEnd>
	<br/>			  \n	����
	<b/>			 �յ�(�ݲ�֧�����ô���)
	<u>				 �ո�(�ݲ�֧�������»���,����Ĭ�����»���)
font�ɱ�ʾΪ0xff00ff��#ff00ff
a href��������

����:
	ColorMap clrMap;
	clrMap[4]="0xff00ff";//ע�ⲻ��û��""
	clrMap[104]="0x00ff00";
	clrMap[105]="0x00ffff";
	m_Parse.SetCorlorMap(clrMap);
	string strShow="[4#��<a href='event:player-11'>�h��</a>��#]��[104#��<a href='event:hero-21874'>̫ʷ��</a>��#]�ɹ�������[105# size=33��<a href='event:item-{\"id\":84209,\"itemID\":89}'>������ʵƪ1</a>��#]��ʹ���ؾ�������������<br/><br/>";
	m_Parse.ParseToEdit(strShow,&m_edtword2,m_fontColor);

	//��һ�ַ�����[4#������ɫ����ͬʱ���������С,�Ƽ�ʹ������һ�ַ���,color��size����д��<font>��:
	strShow="<font color=\"0xff0000\" size = 22/>��<a href='event:player-11'>�h��</a>��</font>��<font color=\"0x0000ff\"/>��<a href='event:hero-21874'>̫ʷ��</a>��</font>�ɹ�������<font color=\"0x00ff00\" size = 44/>��<a href='event:item-{\"id\":84209,\"itemID\":89}'>������ʵƪ1</a>��</font>��ʹ���ؾ�������������<br/><br/>";
	m_Parse.ParseToEdit(strShow,&m_edtword2,m_fontColor);

������������bool CDlgX::WndProc(CWndObject* pObj,UINT message,WPARAM wParam, LPARAM lpParam)
��������MSG_EDITLINKCLICKʱ,LPARAM�������char*���͵��ַ���:event:item-{"id":84209,"itemID":89}

��������const char* strHref=m_Parse.GetMouseDownHref(&m_edtword2,pObj, message, wParam,  lpParam);
			printf("GetMouseDownHref=%s\n",strHref);


*/		
class IParseHtml
{
public:

    //parse
    virtual void Parse(string& str,int fontColor=_DEFAULT_FONT_COLOR)=0;
  
	virtual void ParseWithUTF8(string& str,int fontColor=_DEFAULT_FONT_COLOR)=0;
 
	//get push result
    virtual VEC_HTML& GetObjs()=0;

    //parse string to some edit
    virtual void ParseToEdit(string& str,CCtrlEdit* lpEdit,int fontColor=_DEFAULT_FONT_COLOR)=0;
  
	virtual void ParseToEditWithUTF8(string& str,CCtrlEdit* lpEdit,int fontColor=_DEFAULT_FONT_COLOR)=0;

    //add link to some edit
    virtual void AppendLink(CCtrlEdit* lpEdit,const char* lpText,const char* lpHref,unsigned int clr=_DEFAULT_FONT_COLOR)=0;
 
	virtual void AppendLinkWithUTF8(CCtrlEdit* lpEdit,const char* lpText,const char* lpHref,unsigned int clr=_DEFAULT_FONT_COLOR)=0;

    virtual void AppendText(CCtrlEdit* lpEdit,const char* lpText,unsigned int fontColor=_DEFAULT_FONT_COLOR,int fontSize=0,const char* fontName=NULL)=0;
   
	virtual void AppendTextWithUTF8(CCtrlEdit* lpEdit,const char* lpText,unsigned int fontColor=_DEFAULT_FONT_COLOR,int fontSize=0,const char* fontName=NULL)=0;

};

class CParseHtml:public IParseHtml
{
public:
    CParseHtml();   
	virtual ~CParseHtml();
  
	virtual void Parse(string& str,int fontColor=_DEFAULT_FONT_COLOR);
  
	virtual void ParseWithUTF8(string& str,int fontColor=_DEFAULT_FONT_COLOR);
  
	virtual void ParseToEdit(string& str,CCtrlEdit* lpEdit,int fontColor=_DEFAULT_FONT_COLOR);
   
	virtual void ParseToEditWithUTF8(string& str,CCtrlEdit* lpEdit,int fontColor=_DEFAULT_FONT_COLOR);

    //
    virtual void AppendLink(CCtrlEdit* lpEdit,const char* lpText,const char* lpHref,unsigned int clr=_DEFAULT_FONT_COLOR);

    virtual void AppendText(CCtrlEdit* lpEdit,const char* lpText,unsigned int fontColor=_DEFAULT_FONT_COLOR,int fontSize=0,const char* fontName=NULL);
  
	virtual void  AppendLinkWithUTF8(CCtrlEdit* lpEdit,const char* lpText,const char* lpHref,unsigned int clr=_DEFAULT_FONT_COLOR);

    virtual void AppendTextWithUTF8(CCtrlEdit* lpEdit,const char* lpText,unsigned int fontColor=_DEFAULT_FONT_COLOR,int fontSize=0,const char* fontName=NULL);
  
	//��ȡ�����ı���html����(m_objs)
	virtual VEC_HTML& GetObjs();

	//bool WndProc(CWndObject* pObj,UINT message,WPARAM wParam, LPARAM lParam);
	//������ɫ��(��ID12����ɫ��0xff0000,�Ϳ���[12#��ʾ����ɫ,������д<font color=\"0xff0000\"/>)
	void SetCorlorMap(ColorMap& clrMap);

	//��ȡ��ɫ��
	void MapColor(int index, string& colorStr);

	//��ȡ��갴�µ������ı�,��������,�򷵻�NULL(���ú���д��WndProc��)
	const char* GetMouseDownHref(CCtrlEdit* pEdit/*Ҫ��ȡ�Ĵ���ı༭��*/,CWndObject* pObj,UINT message,WPARAM wParam, LPARAM lParam=NULL);

protected:
    //constructor
    void CreateObjs(tree<HTML::Node>& dom,int defaultFontColor=_DEFAULT_FONT_COLOR/*Ĭ����ɫ*/,int defaultFontSize=0);

    void ReplaceColor(string& str,string& colorStr);

    //pre handle
    void PreProcessColor(string& str,string& retStr);

    void PreProcess(string& str/*�滻ǰ*/,string& retStr/*�滻��*/,const char* reg/*Ҫ�滻���ı�*/,const char* lpReplace/*�滻�ɵ��ı�*/);

	//clear
    void ClearObj(LPHtmlObj lpObj);

    void Clear();


private:
    VEC_HTML m_objs;//�����ı���html����
	ColorMap g_ColorIndex;//��ɫ��,��ID12����ɫ��0xff0000,�Ϳ���[12#��ʾ����ɫ,������д<font color=\"0xff0000\"/>
};



#endif