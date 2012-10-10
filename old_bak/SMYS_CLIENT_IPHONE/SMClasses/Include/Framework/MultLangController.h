/**
������ini��ȡ����
Copyright(c) 1999-2012, TQ Digital Entertainment, All Rights Reserved
**/
#ifndef MULTLANGCONTROLLER__HEAD__
#define MULTLANGCONTROLLER__HEAD__
#include <string>
#include <vector>
#include <map>
using namespace std;

//ÿһ���ַ�����������Ҫ9
#define WORDID_LENGTH 9


struct LANG_SETTING
{
	std::string strShowText; 
	std::string iniFileName; 
};

//������ini��ȡ����
class CMultLangController
{
public:

	static CMultLangController& sharedInstance();

	void SetCurLangID(int idLang);
	
	int GetCurLangID();
		
	//��ȡĳһID���ı�
	const char* GetSysResStringByID(int iWordID);
	
	
	bool GetMultiLangWordByIDWithRepStrs(int iWordID, const std::vector<std::string>& strList, std::string& strOut);
		
	const std::vector<LANG_SETTING>& GetLangSetting();
	
	void TrimColorInfo(std::string& strInfo);
	
protected:

	void LoadResString();
	
	const char* GetSysResStringIniFileName();


private:
	CMultLangController(void);
	~CMultLangController(void);

	

private:
	int m_CurLangID;
	bool m_bLoad;

	std::vector<LANG_SETTING> m_vecLangSetting; 
	typedef std::map<int,std::string> MAP_LANG;

	MAP_LANG m_mapSysResStringIni;
};

#endif