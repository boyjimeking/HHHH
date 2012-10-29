#ifndef __GAMEUPDATE_H__
#define __GAMEUPDATE_H__

#include "../NETWORK/myHttpFile.h"

class CGameUpdate
{
public:
	CGameUpdate(void);
	~CGameUpdate(void);
public:
	//��ʼ������ģ�飬��ȡ���ذ汾��
	bool init();

	//���Ӱ汾���·�����
	bool startUpdate();

	//�ڻص��н������ݼ�֪ͨ������½���
	virtual void receiveVerPackCallBack(const char* szUri, int nPercent);

	//��ɰ汾���£�֪ͨ��Ϸ������Ϸ������
	virtual bool  finishUpdate();

protected:
	bool _DownloadFile(const char* szUri, const char* szFileName);
	const char* _GetAppDocumentPath();
private:
	CMyHttpFile m_myHttpFile;


};


#endif //__GAMEUPDATE_H__