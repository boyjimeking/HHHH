#ifndef __NDLOGIN__HEADER__FILE
#define __NDLOGIN__HEADER__FILE

#ifdef IOS

#include "BaseType.h"
enum eLOGIN_EVENT {
	LE_LOGIN,
    LE_LOGINCANCEL,
	LE_UPDATE,
    LE_BYRESULT,
};

class INDLoginEvent
{
public:
	virtual int NDLoginEvent(eLOGIN_EVENT eEvntID, WPARAM wParam, LPARAM lParam) = 0;
};

class CNDLogin
{
public:
	static CNDLogin& sharedInstance();
	//����Ӧ�ó����91SDK��¼����Ϣ��������ֵ�Ǵ�dev.91.com�ϵ�ע�Ὺ�����ʺ������õõ��ġ�
	bool SetAppInfo(int nAppID/*Ӧ�ó����ID*/, const char* pAppKey/*Ӧ�ó����Key*/);
	
	//����91ƽ̨�ĵ�¼���档
	bool Login();
	
	//�˳�91ƽ̨
	bool Logout(bool bClearAutoLogin/*�Ƿ�ͬʱȡ���Զ���¼*/);
	
	//�Ƿ��Զ���¼91ƽ̨
	bool IsAutoLogin();
	
	//�Ƿ��Ѿ���¼91ƽ̨
	bool IsLogin();
	
	//���õ���ģʽ����Ҫ����֧�����������ܲ��ԡ�
	void SetDebugMode();
	
	//���������,��ص�UpdateResult�ӿ�
	int UpdateApp();
	
	//�����Զ���ת
	void SetAutoRotation(bool bAutoRotation);
	
	//���ý���ƽ̨ʱ�ĺ�����״̬��
	void SetScreenOrientation(int nOrientation);
	
	//�����¼��Ļص��ӿڡ����ؾɵ��¼��ӿ�
	INDLoginEvent* SetEvent(INDLoginEvent* pEvent);
	
	//��ȡ�¼��Ľӿ�
	INDLoginEvent* GetEvent();
    
    bool GetUserName(string& strUin);
    
    bool Get91SdkNickName(string& strName);
    
    void Pay(unsigned int nCount,const char *pszGoodID,const char*pszServerID,const char* pszGoodName);
    
private:
	CNDLogin(void);
	~CNDLogin(void);
	
	INDLoginEvent*	m_pEvent;
};
#endif

#endif