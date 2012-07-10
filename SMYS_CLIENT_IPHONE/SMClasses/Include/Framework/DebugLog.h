/**
��Ļ���������
Copyright(c) 1999-2012, TQ Digital Entertainment, All Rights Reserved
**/
#ifndef __DEBUGLOG__ 
#define  __DEBUGLOG__ 
#include <vector>
#include <string>
using namespace std;

struct DebugLog{
	string strLog;
	long lColor;
};

class CDebugLog
{
public:

	//��ȡ����
	static CDebugLog& sharedInstance();

	//��ʾ������Ϣ
	void ShowLog();

	//void SetLogArea(const int iWidth,const int iHeight);

	//����Log������ʾ�ĸ߶�
	void SetHeight( const int iHeight );

	//���
	void LogOut(const char* strLog/*�ı�*/,const long lColor=0/*��ɫ*/);

	//����ɫ�ĸ�ʽ���ı����
	void LogPrintOut (const long lColor/*=0/*��ɫ*/,const char *format/*��ʽ���ı�*/, ...);

	//����ɫ�ĸ�ʽ���ı����
	void LogPrintOut (const char *format/*��ʽ���ı�*/, ...);

	//�����ƶ�
	void OnMove(int iMoveX,int iMoveY/*Y����ƫ��*/);

	//������е�����Ϣ
	void ClearLogs();

	//������е�����Ϣ,���˵�ǰ��ʾ�ļ�����Ϣ
	void ClearLogsButShow();

	//��ȡ����Ļlog�����Ͻ�����Ļ�ϵĸ߶�
	int GetLogTop();

	//������ౣ��LOG����
	void SetMaxLogs(const int iMax);

	//���������ƶ�LOG�鿴LOG
	void SetIsMoving(bool bMoving=true);

	//�����ƶ�LOG�鿴LOG
	bool GetIsMoving();

	//���������ʾLOG��Ϣ������
	void SetShowLine(const int iShowLine);

	//��ȡ�����ʾLOG��Ϣ������
	int GetShowLine() const;

	//��ȡ��ʾ�Ŀ��
	int GetShowWidth() const;

	//������ʾ�Ŀ��
	void SetShowWidth(const int iWidth);

protected:
	CDebugLog(void);
	~CDebugLog(void);

	int m_iShowLine;//�����ʾLOG��Ϣ������(Ĭ��10)
	int m_iCurLine;//��ǰ�ӵڼ��п�ʼ��ʾ
	std::vector<DebugLog> m_vDebugLog;
	//int m_iTopShow;//��ʼ��ʾ�ĸ߶�
	int m_iShowHeight;//log��ʾ�ĸ߶�(Ĭ��150,Ҫ������������ʾ��FPS��)
	int m_iShowWidth;//��ʾ�Ŀ��(Ĭ��Ϊ��Ļ�Ŀ��)
	bool m_bIsMoving;//�����ƶ�LOG�鿴LOG(�������Զ�����)
	int m_iMaxLogs;//��ౣ��LOG����(Ĭ��500)
private:
	int m_iScreenHeight;//��Ļ�ĸ߶�

};

#endif
