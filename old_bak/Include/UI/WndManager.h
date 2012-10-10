#ifndef  __WNDMANAGER_HH__
#define  __WNDMANAGER_HH__
#include "uitypes.h"

class CWndObject;
//�Ի��������
class CWndManager
{
public:

	//���ݶԻ���ID�����Ի���  
	static DLG_HANDLE CreateDialog(DLG_IDD nDialogIDD, DLG_HANDLE hParent=NULL);

	//���ݴ��ھ���õ����ڶ���
	static CWndObject* GetDialog(DLG_HANDLE hDlg);

	//��ʾ�Ի���(�ҵ��Ի���������ʾ;����Ҳ���,�򷵻�false)
	static bool ShowDialog(DLG_HANDLE nDlgHandle/*���*/, bool bShow = true/*�Ƿ���ʾ*/, bool bFront = false/*�ö�*/);	

	//��ʾģ̬�Ի���
	static bool ShowModalDialog(DLG_HANDLE hModalDlg);

	//���ü���Ի���
	static bool SetActiveDialog(CWndObject* lpActiveWnd);

	//����ģ̬�Ի���
	static bool SetModalDialog(CWndObject* lpModalObj);


	//ȡ�öԻ���������С
	static bool GetDialogRect(DLG_HANDLE hDlg, CRect& rcDialog);

	//���öԻ���������С
	static bool SetDialogRect(DLG_HANDLE hDlg, CRect& rcDialog);

	//���ͶԻ�����Ϣ
	static bool SendMessage(DLG_HANDLE hDlg, UINT Msg, WPARAM wParam, LPARAM lParam =NULL);

	//�ж϶Ի����Ƿ����
	static bool IsDialogVisible(DLG_HANDLE hDlg);

	//�ƶ��Ի���
	static bool MoveDialog(DLG_HANDLE hDlg, int nX, int nY);	

	//�ر����жԻ���
	static bool CloseAllDialog();

	//��ȡ�������ڵĿؼ�(����ΪCPoint(-1,-1)�򷵻���Ϸ������)
	static CWndObject* GetPointObject(CPoint& pos);

	//���ݿؼ���ȡ��Ļ����ʾ������
	static void GetWndViewRect(CWndObject* pWnd,CRect& reccScreen);

	//��ȡ�ϴεĴ�������(��������,�򷵻�CPoint(-1,-1))
	//�����:CGameApp::sharedInstance().GetLastTouchPos()
	//static CPoint GetLastTouchPos();

protected:
	CWndManager(void);
	~CWndManager(void);
};

#endif //__WNDMANAGER_HH__
