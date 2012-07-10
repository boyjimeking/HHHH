#ifndef __UI_CTRL_IMAGE_EX__
#define __UI_CTRL_IMAGE_EX__

#include "CtrlImage.h"
#include "tools.h"

//ͼƬ�ؼ�,�����������ı��Ĺ���
class CCtrlImageEx : public CCtrlImage
{
protected:
	int m_verAlign;//�ؼ���ֱ���뷽ʽ
	int m_herAlign;//�ؼ�ˮƽ���뷽ʽ
	CRect m_ShowRect;//��ԭͼƬ�Ͻ�ȡ��Ҫ��ʾ�������С
	CRect m_CntRect;//ͼƬ����Ļ�ϵ�����
	bool m_bShowRect;//�Ƿ���������ͼƬ
	DWORD m_dwFontColor;//������ɫ
	
public:
	CCtrlImageEx();
	virtual ~CCtrlImageEx();

	//����������ɫ
	void SetFontColor(DWORD dwFontColor);
	
	//���������С
	void SetFontSize(int iFontSize);
	
	//�����ı�
	void SetWindowText(const char* lpText, int verAlign=VERALIGN_CENTER, int herAlign=HERALIGN_CENTER);
	
	//��UTF8���������ı�
	void SetWindowTextWithUTF8(const char* str, int verAlign=VERALIGN_CENTER, int herAlign=HERALIGN_CENTER);
	
	//����ͼƬ��ʾ����
	void SetShowRect(CRect& showRect/*��ԭͼƬ�Ͻ�ȡ��Ҫ��ʾ�������С*/, CRect& cntRect/*ͼƬ����Ļ�ϵ�����*/);
	
	//���ƿؼ��ı���
	virtual void DoPaintBackground(CPoint& cntScreenOrg,CRect& showRect,CRect& cntRect,CPoint& viewOrg);
	
	//���ƿؼ���ǰ��
	virtual void DoPaintForeground(CPoint& cntScreenOrg,CRect& showRect,CRect& cntRect,CPoint& viewOrg);
	
protected:
	char* m_lpText;//�ı�
	int m_iWidth;//�ı��Ŀ��
	
};

#endif
