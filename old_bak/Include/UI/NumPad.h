#ifndef _NUMPAD_H
#define _NUMPAD_H
#include "INumPad.h"
class CCtrlEdit;
class CCtrlSlider;
/**
  �������ֵ���Ͽؼ�(��CCtrlEdit,CCtrlSlider)
**/
class CNumPad : public INumPad
{
public:
	CNumPad();
    virtual ~CNumPad();

	virtual void Hide();
	virtual void Show();

	//���ù����ı༭��
	virtual void SetBindEdit(CCtrlEdit* pEdit);

	//���ֵ�Ƿ�Խ��,�����ù����Ļ���ؼ���ֵ
	virtual int CheckValue(int nCurVal);

protected:
	CCtrlEdit* m_pBindEdit;//�����ı༭��ؼ�
	CCtrlSlider* m_pBindSlider;//�����Ļ���ؼ�

	int m_nMinValue;//��Сֵ
	int m_nMaxValue;//���ֵ

protected:

	//���ù����ؼ���Ĭ������
	void Reset();	
};

#endif