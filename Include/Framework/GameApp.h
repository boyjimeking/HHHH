/*
��Ϸ�����ڹ���
Copyright(c) 1999-2012, TQ Digital Entertainment, All Rights Reserved
*/
#if !defined __GAMEAPP__HEADER__
#define __GAMEAPP__HEADER__
#include "C3Primitive.h"
#include "FrameworkTypes.h"
#include "Log.h"


//��Ϸ��ʼ�����
extern int InitGameInstance();

//��Ϸ�˳��������
extern int ExitGameInstance();

//����Ŀ¼
int CreateDirectory(const char * lpPath);

class INetHelper
{
public:
	//�û��������߳̽��յ������ݣ�ѹ�뵽����ڲ�ʱ���ڲ��ɷ���󣬻���ô˽ӿڽ�����Դ���ͷ�
	virtual bool ReleaseData(void* pNetData) = 0;

	//�û���װ���������ݷ��ͽӿڣ��ڲ���ܻ����ô˽ӿڽ����������ݷ���,
	//pData�������ڴ�飬��lExData��Ϊ����
	//pDataҲ���Ƕ�����lExData�����û��Լ���������ֵ
	virtual bool SendData(const void* pData, long lExData = 0) = 0;
};

//����HOOK�Ļص��ӿڣ��μ�CGameApp.SetTouchHook
class ITouchHook
{
public:
	//����true�����������´���false�򡰳ԡ������롣
	virtual bool TouchEvent(TOUCH_EVENT_INFO& tei) = 0;
};

//�������ݰ���HOOK�ص��ӿڣ��μ�CGameApp.SetTouchHook
class INetPackageHook
{
public:
	//����true�����������´���false�򡰳ԡ������롣
	virtual bool OnPackageFilter(void* pDataPkg) = 0;
};


typedef enum PLATFORM_TAG
{
	PLATFORM_WIN32,
	PLATFORM_IOS,
	PLATFORM_ANDROID,
	PLATFORM_UNKNOW,
} ePLATFORM;

class CGameApp 
{
public:
	//�ַ��û�������Ϣ��һ���ɿ�ܵ��á�
	bool DispatchUserEvent(TOUCH_EVENT_INFO& tei);	

	//�����û�����HOOK�������������е��û����롣���DispatchUserEvent������ʵ��"�����طŵĹ���"
	ITouchHook* SetTouchHook(ITouchHook* pTouchHook);

	//��ȡ�ϴεĴ�������(��������,�򷵻�CPoint(-1,-1))
	CPoint GetLastTouchPos();

	//�ַ��������ݰ������û����������ã��������ݷַ����������������ںͻ������
	//�û���������ַ���һ��Ҫ���ô˽ӿڡ�
	bool DispatchNetPackage(void* pDataPkg);

	//�����������ݰ��ַ�ʱ��HOOK���������طַ��������������ݰ������DispatchNetPackage������ʵ��"�������ݻطŵĹ���"
	INetPackageHook* SetNetPackageHook(INetPackageHook* pHook);

	//�������縨���ӿڣ����û�����DispatchNetPackageʱ������й�pDataPkg�ڴ档
	//���ݷַ���ɺ󣬽����ô˽ӿڵ�ReleaseData���������ͷ�
	INetHelper* SetHelperInterface(INetHelper* pHelperInf);

	//������ǰ�������ݰ������������ݷַ���һ����OnNewNetPackage�е��á�
	void DiscardPackage();

	//����豸��Ļ��С
	CSize GetWindowSize(void);
	
	//����豸��Ļ���
	int GetScreenWidth();

	//����豸��Ļ�߶�
	int GetScreenHeight();

	//����豸��Ļ�ķ���
	TQDeviceOrientation getDeviceOrientation(void);
	
	//������Ϸ�����֡����
	void SetFrameRatio(float fRatio);

	//�����Ƿ���ʾFPP�����λ�ú�Log����(�û�����ͨ��CDebugLog�������־���)
	void SetShowDebugLog( bool bShow=true );
	
	//�Ƿ���ʾFPP�����λ�ú�Log����
	bool IsShowDebugLog();

	//�����������ݰ����ڲ�������INetHelper�ӿڵ�SendData�������ݷ���
	bool SendData(const void* pData, long lExData = 0);
	
	//��ȡ��ǰ����·��
	const char* GetAppPath();

	//��ȡ��ԴĿ¼·����
	const char* GetResPath();

	//���ó����¼���ʱ�䣬Ĭ��Ϊ1��,nElapse��λΪ����
	void	SetLongPressTime(unsigned int nElapse);

	//��ȡ�����¼���ʱ��
	unsigned int GetLongPressTime();

	//��ȡ����ĵ���
	static CGameApp& sharedInstance();
	
	//������ʾ��������
	void SetFocusRect(CRect& rc/*����,Ϊ������ʾ��������*/,DWORD iColor=0xff0000/*��ɫ*/,int iBorder=1/*���*/);


	//����UI�������ļ���(iniĿ¼��)��ֻ����InitGameInstance��������Ч����Ҫ�ڴ��ڴ���֮ǰ��û�����ã���ȡĬ��ֵgui.ini
	bool SetUICfgFile(const char* lpFileName);

	//��ȡUI�������ļ�����
	const char* GetUICfgFile();

	//����UI�ؼ���Ĭ��������
	void SetDefaultUIFontInfo(RENDER_TEXT_STYLE eStyle = RENDER_TEXT_SILHOUETTE, DWORD dwShadowColor = 0xff000000, const CPoint ptShadow = CPoint(1,1));

	//ǿ�����»���֡��������������Ϣ���û�����ֶ�Run()����Ҳ���ػ�,��Run�ᴦ��������Ϣ���ܻᵼ����ѭ����
	void ForceReDraw();

	//�˳���Ϸ.
	void ExitGame();

	//��ȡƽ̨������
	ePLATFORM	GetPlatform();

	//IOSƽ̨����
#ifdef IOS
	//�ж��Ƿ�IPAD
	bool IsIPad();

	//�Ƿ�֧�ָ�����
	bool IsRetina();
#endif


	//�ڲ�ʵ�֡��ⲿ��Ҫ����
	bool InitApp(HEAGLDRAWABLE hEaglDrawable, const char* pszAppPath, int iWidth, int iHeight);
	void Run();

private:
	CGameApp();
	~CGameApp();


	bool MainLoop(TOUCH_EVENT_INFO* pKeyEvent, bool bHandleMsg = true);
	bool PreDraw();
	bool Draw();
	bool PostDraw();

	//��ʾFTP�����λ��
	void ShowDebugInfo();
	
	void CalcFrameRatio();
private:
	HEAGLDRAWABLE	m_hViewHandle;
	
	TOUCH_EVENT_INFO m_infoTouchLast;
	ITouchHook*		 m_pTouchHook;

	INetPackageHook* m_pNetPackageHook;

	TQTimeVal	m_timeLastFrame;//for Actions
	TQTimeVal	m_tmCurFrame;	//for Debug info
	float m_fCurFrameRatio;
	float m_fFrameInterval;
	int m_nFrameCount;
	
	struct FocusRect{
		CRect rc/*����*/;
		DWORD iColor/*��ɫ*/;
		int iBorder/*���*/;
		FocusRect()
		{
			iColor=0xff0000/*��ɫ*/;
			iBorder=1/*���*/;
		}

	};

	int m_iTimerId;
	FocusRect m_FocusRect;
	bool	m_bShowFocusRect;//��ʾ��������
	bool	m_bShowDebugLog;//�Ƿ���ʾFPS�����λ�ú�log
	bool   m_bDiscardPackage;
	unsigned int m_nLongPressElapse;
};

#endif
