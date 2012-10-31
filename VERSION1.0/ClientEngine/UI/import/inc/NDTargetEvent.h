/*
 *  NDTargetEvent.h
 *  DragonDrive
 *
 *  Created by jhzheng on 11-12-15.
 *  Copyright 2011 (����)DeNA. All rights reserved.
 *
 */

#pragma once 

enum TARGET_EVENT
{
	TE_NONE = 0,
	
	TE_TOUCH_BTN_CLICK = 1,
	
	TE_TOUCH_BTN_DRAG_OUT = 2,
	
	TE_TOUCH_BTN_DRAG_OUT_COMPLETE = 3,
	
	TE_TOUCH_BTN_DRAG_IN = 4,
	
	//������ĳ����ͼ�ܵ�������ʼ��(����Ϊ��ͼ����)
	TE_TOUCH_SC_VIEW_IN_BEGIN = 5,
	
	//checkbox״̬�����ı�
	TE_TOUCH_CHECK_CLICK = 6,
	
	//RadioGroupѡ����ı�
	TE_TOUCH_RADIO_GROUP = 7,
	
	//edit�¼�
	TE_TOUCH_EDIT_RETURN = 8,
	TE_TOUCH_EDIT_TEXT_CHANGE = 9,
	TE_TOUCH_EDIT_INPUT_FINISH =10,

	//��ť˫��
	TE_TOUCH_BTN_DOUBLE_CLICK = 11,
	
	//�б�ؼ�
	TE_TOUCH_LIST_CLICK = 12,
	//�б�ؼ�����
	TE_TOUCH_LIST_SHINK = 13,
    
    //*chh 2012-08-01 �����¼��Ľ���*//
    TE_TOUCH_SC_VIEW_IN_END = 14,
	// ...
	TE_END,
};