#ifndef __CMAPVIEW__HH__
#define __CMAPVIEW__HH__
#include <vector>
#include "graphic.h"
#include "uitypes.h"
using namespace std;
/**
  ��ͼ����,�����ڳ�����
**/

class CMapView
{
public:
	CMapView(void);
	~CMapView(void);

	//��ʾ
	bool Show(int iPosViewX/*�Ӿ�ԭ��X*/,int iPosViewY/*�Ӿ�ԭ��Y*/,int nAlpha = 0, DWORD dwShowWay = _SHOWWAY_NORMAL);

	//���������ͼ
	bool LoadMap(const char* pAniFileName/*�ļ�·��*/,  int nColCount/*����������*/,int nRowCount/*����*/, int nImageSize = 256/*������ͼ����*/);
	
	//���õ�ͼ����ʾ����(����Ĭ��0����ʾ����Ϊ��Ļ��С)
	void SetShowSize(int iWidht=0/*Ĭ��0����ʾ����Ϊ��Ļ��С*/,int iHeight=0);
	
	//��ȡ��ͼ�黺���С
	int GetCacheSize() const;

	//���õ�ͼ�黺���С
	void SetCacheSize(int iCacheSize);
	
	//��������ת��Ļ����
	void World2d2Screen( int nWorldX, int nWorldY, int* pScreenX, int* pScreenY);
	
	//��ȡ���Ŵ�С
	float GetScale() const;

	//�������Ŵ�С
	BOOL SetScale(float val);

	//���õ�ͼԭʼ��С
	void SetMapSize(const CSize& mapSize);

	//�������ŷ�Χ
	BOOL SetScaleRange(float fMinScale,float fMaxScale);
	
	//��ȡĳһ��ͼ��Ķ�������(����������--���ͼ������)
	CPoint GetCellPos(int iRow/*��*/,int iCol/*��*/);
	
	//����һ�������ͼ��
	void AddCache(int iPndNum/*��ͼ��*/);
	
	//��ȡ��Ļ��ʾ�ĵ�ͼ��Ĵ�С
	float GetCellWidth();
	
	//��ȡ��ͼ������
	int GetColCount() const;
	
	//��ȡ��ͼ������
	int GetRowCount() const;

	//��ȡ��С���ű���
	float GetScaleMin() const;

	//��ȡ������ű���
	float GetScaleMax() const;

protected:
	bool ReadFile(const char* newpath);
	BOOL OnSetScale(float minScale=0);
	int GetNumOfMap(int iPosViewX,int iPosViewY,int* startRow,int* startCol);

	typedef map<int,CMyBitmap*> MAP_BMP;//������С��ͼ�б�(��ͼ��-��ӦͼƬ)
	MAP_BMP m_mapCache;
	typedef std::vector<CMyBitmap*> VecBmp;
	VecBmp m_vecCache;//������
	vector<string> m_vecMapPng;//ȫ����С��ͼ��ַ�б�
	float m_fScale;//���ű���
	string m_szAniFileName;
	int m_nRowCount;
	int m_nColCount;
	int m_nImageSize;//������ͼ����
	int m_nCacheSize;//�����ͼ��(Ĭ��20)
	CSize m_showSize;//��ʾ����
	CPoint m_posView;//��Ļ�����ԭ�����������
	float m_fScaleMin;//��С���ű���(Ĭ��.5f,��������ͼƬ���ó�����С���Զ�������С���ű���)
	float m_fScaleMax;//������ű���(Ĭ��1.f)
	CSize m_sizeMap;//ԭ��ͼ�Ĵ�С

	int m_iStartYPre;//���һ�εĳ�ʼ��ͼY����
	int m_iStartXPre;//���һ�εĳ�ʼ��ͼX����
	float m_fScalePre;
	//int m_iNeedCachePre;//

};
#endif //__CMAPVIEW__HH__
/*
  ʹ�÷���:
#define X_NUM 13
#define Y_NUM 12 
#define MAP_WIDTH 256
const int iWidth=X_NUM*MAP_WIDTH-93;
const int iHeight=Y_NUM*MAP_WIDTH-(MAP_WIDTH-153);
CLoginScene::CLoginScene(void)
{
	//���������ͼ
	m_mapView.LoadMap("./ani/puzzle.ani",X_NUM,Y_NUM,MAP_WIDTH);
	
	//���õ�ͼ����ʾ����
	m_mapView.SetShowSize();

	m_fSacle=.5f;
	SetScale(m_fSacle);
}

BOOL CLoginScene::SetScale(float fSacle)
{
	if (fSacle<m_mapView.GetScaleMin())
	{
		return FALSE;
	}

	m_mapView.SetMapSize(CSize(iWidth,iHeight));
	SetSceneSize(CSize(iWidth*m_fSacle,iHeight*m_fSacle));
	if(!m_mapView.SetScale(m_fSacle))
	{
		return FALSE;
	}

	OnMove(0,0);
	return TRUE;
}
**/