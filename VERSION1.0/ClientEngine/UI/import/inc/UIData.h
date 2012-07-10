/////////////////////////////
//�ؼ���Ϣ��ȡ,�ӽ���༭���ĵ��������л�ȡ�ؼ���Ϣ
//write by yay
//2011-11-22
///////////////////////////////


/*
ʹ��

CUIData  uiData;
uiData.opeUiFile("data/ini/1.ini");

int nCtrlAmount = uidata.GetCtrlAmount();
for(int i=0; i<nCtrlAmount; i++)
{
	std:string str = uidata.getCtrlName(0);
	uidata.getCtrlData((char*)str.c_str()); 

	unsigned long nType = uidata.getType();	

	//here creat ctrl
	//ȡ�ؼ�����
	cocos2d::CCPoint ctrlPos = uidata.getPos();	
	//ȡ�ؼ�ê��ֵ
	cocos2d::CCPoint AnchorPos = uidata.getAnchorPoint();	 
	//ȡ�ؼ�ID 
	unsigned long nTag = uidata.getID();	
	//ȡ�ؼ�����

	switch(nType)
	{
	case 1;

	break;

	case 2:
	break;
	}

}


*/

#pragma once

#include <string>
#include "cocos2d.h"
#include "CCPointExtension.h"
#include "IniFile.h"
#include "Utility.h"

struct CTRL_UV
{
	CTRL_UV()
	{
		x = 0;
		y = 0;
		w = 0;
		h = 0;
	}
	int x;
	int y;
	int w;
	int h;
};

struct UIINFO
{
	std::string strNormalFile;
	std::string strSelectedFile;
	std::string strDisableFile;
	std::string strFocusFile;
	std::string strBackFile;

	CTRL_UV rectNormal;
	CTRL_UV rectSelected;
	CTRL_UV rectDisable;
	CTRL_UV rectFocus;
	CTRL_UV rectBack;

	CGPoint CtrlPos;
	CGPoint CtrlAnchorPos;

	unsigned long   nID;
	unsigned long   nType;

	unsigned long   nCtrlWidth;
	unsigned long   nCtrlHeight;

	std::string strText;
	std::string strTextAlign;
	std::string strTextTradition;
	
	unsigned long nTextFontSize;
	unsigned long nTextFontColor;


};

class CUIData
{
public:
	CUIData(void);
	~CUIData(void);
public:
	bool openUiFile(const char* pszIniFile);
	int  GetCtrlAmount();
	std::string getCtrlName(int nIndex);
	bool getCtrlData(char* szCtrlName);


	UIINFO& getCtrlUiInfo()
	{
		return m_info;
	}


	unsigned long getCtrlWidth()
	{
		return m_info.nCtrlWidth;
	}

	unsigned long getCtrlHeight()
	{
		return m_info.nCtrlHeight;
	}

	//�ؼ�ͼƬ·����ע��Ϊ���·����
	std::string getNormalFile();
	std::string getSelectedFile();
	std::string getDisableFile();
	std::string getFocusFile();
	std::string getBackFile();

	//ȡ�ؼ�ͼƬ��UV��Ϣ
	CTRL_UV getNormalFileUV()
	{
		return m_info.rectNormal;
	}
	CTRL_UV getSelectedFileUV()
	{
		return m_info.rectSelected;
	}
	CTRL_UV getDisableFileUV()
	{
		return m_info.rectDisable;
	}
	CTRL_UV ggetFocusFileUV()
	{
		return m_info.rectFocus;
	}
	
	CTRL_UV ggetBackFileUV()
	{
		return m_info.rectBack;
	}


	//ȡ�ؼ����꣬[ע��ê������]
	CGPoint getPos()
	{
		return m_info.CtrlPos;
	}

	//ȡ�ؼ�ê��ֵ
	CGPoint getAnchorPoint()
	{
		return m_info.CtrlAnchorPos;
	}

	//ȡ�ؼ�ID 
	unsigned long getID()
	{
		return m_info.nID;
	}

	//ȡ�ؼ�����
	unsigned long getType()	
	{
		return m_info.nType;
	}

	//ȡ�ؼ��ı�����
	std::string getCtrlText()
	{
		return m_info.strText;
	}

	//ȡ�ı����뷽ʽ
	std::string getCtrlTextAlign()
	{
		return m_info.strTextAlign;
	}


private:
	UIINFO m_info;
	CIniFile ini;
};
