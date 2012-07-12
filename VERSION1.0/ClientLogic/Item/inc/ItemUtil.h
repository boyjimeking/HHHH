/*
 *  ItemUtil.h
 *  SMYS
 *
 *  Created by jhzheng on 12-3-5.
 *  Copyright 2012 (����)DeNA. All rights reserved.
 *
 */

#ifndef _ITEM_UTIL_H_ZJH_
#define _ITEM_UTIL_H_ZJH_

enum
{
	ITEM_ID							= 0,               // Id
	ITEM_TYPE							= 1,               // ����
	ITEM_OWNERTYPE					= 2,               // ����������
	ITEM_OWNER_ID						= 3,               // ������id	
	ITEM_USER_ID						= 4,               // ���id
	ITEM_AMOUNT						= 5,               // ����
	ITEM_POSITION						= 6,               // λ��
	ITEM_ADDITION						= 7,               // ǿ���ȼ�
	ITEM_EXP							= 8,               // ��������
	ITEM_ATTR_NUM						= 9,               // ��������
	ITEM_ATTR_BEGIN					= 10,               // ���Զ��忪ʼ()
};

unsigned int GetItemInfoN(unsigned int nItemId, unsigned int  nIndex);
const char* GetItemInfoS(unsigned int nItemId, unsigned int  nIndex);

unsigned int GetItemDBN(unsigned int nItemType, unsigned int  nIndex);
const char* GetItemDBS(unsigned int nItemType, unsigned int  nIndex);

bool IsItemCanChaiFen(unsigned int nItemType);
bool isItemRidePet(unsigned int nItemType);


#endif // _ITEM_UTIL_H_ZJH_