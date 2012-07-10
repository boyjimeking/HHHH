/*
�����Ĺ�����
Copyright(c) 1999-2012, TQ Digital Entertainment, All Rights Reserved
*/
#ifndef __FRAMEWORK__DIRECTOR__H__
#define  __FRAMEWORK__DIRECTOR__H__
#include "FrameworkTypes.h"
#include "Scene.h"
#include <map>
using namespace std;

const unsigned int  INVALIDSCENE = 0xFFFFFFFF;//����ID���Ƿ��ĳ���ID
const unsigned int  ACTIVESCENE = INVALIDSCENE-1;//��ǰ��ĳ���

//�����Ĺ�����
class CDirector
{
public:
	//��ӳ���
	bool AddScene(CScene* sce);

	//�Ƴ�����
	bool RemoveScene(CScene* sce);
	bool RemoveScene(int nSceneID);

	//�Ƴ����г���
	bool RemoveAllScenes();

	//���õ�ǰ�����
	bool SetActiveScene(CScene* sce);
	bool SetActiveScene(int nSceneID);

	//��ȡ��ǰ�����
	CScene* GetActiveScene();

	//���ݳ���ID����ȡ��������ָ��
	CScene* GetSceneByID(int nSceneID);

	//���ͳ�����Ϣ���ɳ�����MsgHandler����
	bool SendSceneMessage(int nSceneID, UINT nMsg, WPARAM wParam, LPARAM lParam);

public:
	static CDirector& sharedInstance(void);
	
private:
	virtual ~CDirector(void);
	CDirector(void);

private:
	typedef map<int, CScene*> SCENEMAP;
	SCENEMAP	m_mapScenes;
	int			m_nAcitveSceneID;
};
#endif


