#ifndef __MY_EVENT_H__
#define __MY_EVENT_H__
/**
�Զ��庯����
Copyright(c) 1999-2012, TQ Digital Entertainment, All Rights Reserved
**/
#include "FastDelegate.h"

//�޺��������ĺ�����(fun())
typedef fastdelegate::FastDelegate0<> ClickEvent;

//���������ĺ�����(fun(����1,����2))
typedef fastdelegate::FastDelegate2<WPARAM, LPARAM> CommonEvent;

//���������ĺ�����(fun(����1,����))
typedef fastdelegate::FastDelegate2<UINT, CPoint/*����*/> MouseEvent;

//���������ĺ�����(fun(����1,����,����3))
typedef fastdelegate::FastDelegate3<UINT, CPoint/*����*/, short> MouseWheelEvent;

#endif
