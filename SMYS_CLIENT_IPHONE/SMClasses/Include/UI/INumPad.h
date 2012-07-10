/*
 *  INumPad.h
 *  Copyright 2011 TQ Digital Entertainment. All rights reserved.
 */
#ifndef __CTRL_NUMPAD_H__
#define __CTRL_NUMPAD_H__

class CCtrlEdit;
class CCtrlSlider;
/**
  �������ֵ���Ͽؼ��ӿ�(��CCtrlEdit,CCtrlSlider)
**/
class INumPad
{
public:	

	virtual void Hide() = 0;

	virtual void Show() = 0;

	//���ù����ı༭��
	virtual void SetBindEdit(CCtrlEdit* pEdit) = 0;

#ifdef WIN32
	//���ֵ�Ƿ�Խ��,�����ù����Ļ���ؼ���ֵ
	virtual int CheckValue(int nCurVal) = 0;
#endif
};

#define NumPadType 1004

extern INumPad* GetNumPad();

#endif