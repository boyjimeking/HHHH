/**
������
Copyright(c) 1999-2012, TQ Digital Entertainment, All Rights Reserved
**/
#ifndef __FRAMEWORK__CScene__H__
#define __FRAMEWORK__CScene__H__
#include "FrameworkTypes.h"
#include "selector_protocol.h"
#include "Timer.h"

class CSprite;
class CScene :public SelectorProtocol,public ISysTimerDelegate
{
public:
	CScene(void);
	virtual ~CScene(void);
	
	//��ȡ������ID��
	virtual int GetSceneID() = 0;

	//�û����������¼���
	virtual bool OnTouchEvent(TOUCH_EVENT_INFO* tei=NULL/*������Ϣ*/) ;

	//ÿһ֡׼������֮ǰ�ص�������������ǰ�ڻ��ƻ���Դ׼����
	virtual void PreRender();

	//ÿһ֡����ʱ�ص������������г����Ļ��ơ�
	virtual bool OnRender() = 0;

	//ÿһ֡׼�����ƽ���֮ǰ�ص����������ڴ˹����л��ƵĶ��󣬽���UI(���ڡ��ؼ�)֮�ϡ�
	virtual	void PostRender();

	//�յ��������ݰ������ݰ��ȹ㲥�����ӵĴ��ں󣬲Ź㲥����ǰ�������
	virtual void OnNetPackage(void* pDataPkg/*�����ָ��*/) = 0;

	//�ƶ��������ɿ�ܻص�������û��ڳ����е���ShowRelative�ӿڣ�
	//������������е���CScene::OnMove��������ɶԳ����ϵ�����ƶ������йܡ�
	virtual void OnMove(int nXOffset/*X����ƶ�����,��λ������*/, int nYOffset/*Y����ƶ�����,��λ������*/);
	
	//���ų������ɿ�ܻص�,
	virtual void OnScale(int nScale/*���ŵ�����,��λ������*/) = 0;

	//����������ɿ�ܻص�
	virtual void OnActive()=0;

	//����ʧȥ�״̬
	virtual void OnDeactive()=0;

	//��ʱ���ص�����
	virtual int OnTimer(int TimerId, void* pParam);

	//��ȡ��ǰ��������ͼԭ��(��������������Ͻ�����)
	void GetViewPos(CPoint& refPos);

	//���õ�ǰ��������ͼԭ��
	void SetViewPos(const CPoint& refPos);

	//��ȡ��ǰ��������ͼ��С(�����Ӵ�С)
	void GetViewRect(CRect& refRect);

	//��ȡ�����Ĵ�С��
	void GetSceneSize(CSize& refSceneSize);

	//���ó����Ĵ�С
	void SetSceneSize(const CSize& refSceneSize);

	//����ͼ����ת���ɳ�������(��������)
	void ViewPos2WordPos(CPoint& refViewPos);



	//�����������ľ���
	void AddTouchSprite(CSprite* pSprite);

	//ɾ���������ľ���
	void RemoveTouchSprite(CSprite* pSprite);

	//�������еľ������¼�(������FALSE���ٲ��������ľ���)
	virtual BOOL OnSpriteEvent(CSprite* pObj, int nEventType);

	friend class CDirector;
protected:

	//��Ϣ�������ڴ�����SendMessage���͵���Ϣ��
	virtual bool MsgHandler(UINT nMessage,WPARAM wParam, LPARAM lParam=0);

	CPoint	m_posOrg;//�Ӿ�ԭ��
	CSize m_sizeScene;//�����Ĵ�С

	vector<CSprite*> m_vecSpriteTouch;//�������ľ���
};

#endif

