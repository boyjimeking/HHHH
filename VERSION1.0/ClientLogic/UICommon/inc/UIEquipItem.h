/*
 *  UIEquipItem.h
 *  SMYS
 *
 *  Created by jhzheng on 12-3-7.
 *  Copyright 2012 (����)DeNA. All rights reserved.
 *
 */

#ifndef _UI_EQUIP_ITEM_H_ZJH_
#define _UI_EQUIP_ITEM_H_ZJH_

#include "UIItemButton.h"
#include "UISpriteNode.h"

class CUIEquipItem :
public CUIItemButton
{
	DECLARE_CLASS(CUIEquipItem)
	
	CUIEquipItem();
	~CUIEquipItem();
    
    void Initialization(); override
    
    void SetUpgradeIconPos(int nUpgradeIconPos){
        m_nUpgradeIconPos = nUpgradeIconPos;
        AdjustPos();
    };
	int GetUpgradeIconPos(){
        return m_nUpgradeIconPos;
    };
    
    void SetUpgrade(int nSet){
        m_nIsUpgrade = nSet;
        AdjustPos();
    };
	int GetUpgrade(){return m_nIsUpgrade;};

private:
    int m_nIsUpgrade;       //�Ƿ������ 0.����ʾ 1.������ 2.��������
    int m_nUpgradeIconPos;  //0.��� 1.�ұ�
    CUISpriteNode *m_GUpgradeSprite;
    CUISpriteNode *m_RUpgradeSprite;
    
    void AdjustPos();
};

#endif // _UI_EQUIP_ITEM_H_ZJH_