//
//  NDLayer.h
//  DragonDrive
//
//  Created by xiezhenghai on 10-12-8.
//  Copyright 2010 (����)DeNA. All rights reserved.
//
//	�������ܣ���
//	������Ϸ���ص㣬��Ϸ�д���ʱ�仨�ڲ��ϡ�
//	�����ʾ��
//		����ӵĲ㽫�Ḳ����ǰ��ӵĲ㣬
//		�����ʱ͸�������߰�͸������ײ��Ĳ㽫����ʾ���߲�����ʾ��
//	����¼���Ӧ��
//		������Ĳ����Ƚ��յ�ϵͳ�¼�(�磺��ָ�����Ļ�¼�)��
//		����¼����������������¼��ķַ�������������Ĳ���зַ��¼���ֱ���¼�������Ϊֹ��

#ifndef __NDLayer_H
#define __NDLayer_H

#include "NDNode.h"
#include "NDTouch.h"
#include <string>

namespace NDEngine
{
class NDLayer: public NDNode
{
DECLARE_CLASS(NDLayer)
public:
	NDLayer();
	~NDLayer();

public:

	static NDLayer* Layer();

	void Initialization();
	void draw();

	void SetTouchEnabled(bool bEnabled);

	virtual bool TouchBegin(NDTouch* touch);
	virtual void TouchEnd(NDTouch* touch);
	virtual void TouchCancelled(NDTouch* touch);
	virtual void TouchMoved(NDTouch* touch);
	virtual bool TouchDoubleClick(NDTouch* touch);

DECLARE_AUTOLINK(NDLayer)
INTERFACE_AUTOLINK(NDLayer)
};
}
#endif
