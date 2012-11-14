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
#include "UtilityInc.h"

enum DISPLAY_RESOLUTION
{
	DISPLAY_RESOLUTION_BEGIN,
	DISPLAY_RESOLUTION_480_320				= DISPLAY_RESOLUTION_BEGIN,
	DISPLAY_RESOLUTION_960_640,
	DISPLAY_RESOLUTION_1024_768,
	DISPLAY_RESOLUTION_2048_1536,
	DISPLAY_RESOLUTION_END,
};

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

	void reset() {
		x = y = w = h = 0;
	}
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

    cocos2d::CCPoint CtrlPos;
    cocos2d::CCPoint CtrlAnchorPos;

	unsigned long   nID;
	unsigned long   nType;

	unsigned long   nCtrlWidth;
	unsigned long   nCtrlHeight;

	std::string strText;
	std::string strTextAlign;
	std::string strTextTradition;
	
	unsigned long nTextFontSize;
	unsigned long nTextFontColor;

	void reset()
	{
		strNormalFile = "";
		strSelectedFile = "";
		strDisableFile = "";
		strFocusFile = "";
		strBackFile = "";

		rectNormal.reset();
		rectSelected.reset();
		rectDisable.reset();
		rectFocus.reset();
		rectBack.reset();

		CtrlPos.x = CtrlPos.y = 0.f;
		CtrlAnchorPos.x = CtrlAnchorPos.y = 0.f;

		nID = 0;
		nType = 0;
		nCtrlWidth = 0;
		nCtrlHeight = 0;

		strText = "";
		strTextAlign = "";
		strTextTradition = "";

		nTextFontSize = 0;
		nTextFontColor = 0;
	}
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
		return m_kInfo;
	}

	unsigned long getCtrlWidth()
	{
		return m_kInfo.nCtrlWidth;
	}

	unsigned long getCtrlHeight()
	{
		return m_kInfo.nCtrlHeight;
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
		return m_kInfo.rectNormal;
	}
	CTRL_UV getSelectedFileUV()
	{
		return m_kInfo.rectSelected;
	}
	CTRL_UV getDisableFileUV()
	{
		return m_kInfo.rectDisable;
	}
	CTRL_UV ggetFocusFileUV()
	{
		return m_kInfo.rectFocus;
	}
	
	CTRL_UV ggetBackFileUV()
	{
		return m_kInfo.rectBack;
	}


	//ȡ�ؼ����꣬[ע��ê������]
    cocos2d::CCPoint getPos()
	{
		return m_kInfo.CtrlPos;
	}

	//ȡ�ؼ�ê��ֵ
	cocos2d::CCPoint getAnchorPoint()
	{
		return m_kInfo.CtrlAnchorPos;
	}

	//ȡ�ؼ�ID 
	unsigned long getID()
	{
		return m_kInfo.nID;
	}

	//ȡ�ؼ�����
	unsigned long getType()	
	{
		return m_kInfo.nType;
	}

	//ȡ�ؼ��ı�����
	std::string getCtrlText()
	{
		return m_kInfo.strText;
	}

	//ȡ�ı����뷽ʽ
	std::string getCtrlTextAlign()
	{
		return m_kInfo.strTextAlign;
	}


private:

	string GetValKeyStr(const char* str);

	UIINFO m_kInfo;
	CIniFile m_kINIFile;
	DISPLAY_RESOLUTION m_kDisplayResolution;
};
