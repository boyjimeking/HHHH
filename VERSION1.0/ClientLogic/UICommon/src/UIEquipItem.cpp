/*
 *  UIEquipItem.cpp
 *  SMYS
 *
 *  Created by jhzheng on 12-3-7.
 *  Copyright 2012 (����)DeNA. All rights reserved.
 *
 */

#include "UIEquipItem.h"
#include "NDPath.h"
#include "NDDirector.h"


IMPLEMENT_CLASS(CUIEquipItem, CUIItemButton)
#define G_UPGRADE_SPRITE "shengjiejt01.spr"
#define R_UPGRADE_SPRITE "shengjiejt02.spr"

#define UPGRADE_ICON_W      (NDDirector::DefaultDirector()->GetScaleFactor() * 15)
#define UPGRADE_ICON_H      (NDDirector::DefaultDirector()->GetScaleFactor() * 30)

CUIEquipItem::CUIEquipItem()
{
    m_nUpgradeIconPos   = 0;
    m_GUpgradeSprite    = NULL;
    m_RUpgradeSprite    = NULL;
    m_nIsUpgrade        = 0;
}

CUIEquipItem::~CUIEquipItem()
{
    m_GUpgradeSprite = NULL;
    m_RUpgradeSprite = NULL;
}
void CUIEquipItem::Initialization(){
    CUIItemButton::Initialization();
    
    
}

void CUIEquipItem::AdjustPos()
{
	if (m_nIsUpgrade != 0)
	{

		if (m_GUpgradeSprite == NULL)
		{
			//** ��ʼ������ͼƬ **//

			m_GUpgradeSprite = new CUISpriteNode;
			m_GUpgradeSprite->Initialization();
			m_GUpgradeSprite->ChangeSprite(
				NDPath::GetAniPath(G_UPGRADE_SPRITE).c_str());
			this->AddChild(m_GUpgradeSprite);
		}

		if (m_RUpgradeSprite == NULL)
		{
			m_RUpgradeSprite = new CUISpriteNode;
			m_RUpgradeSprite->Initialization();
			m_RUpgradeSprite->ChangeSprite(
				NDPath::GetAniPath(R_UPGRADE_SPRITE).c_str());
			this->AddChild(m_RUpgradeSprite);
		}

		if (m_nIsUpgrade == 1)
		{
			m_GUpgradeSprite->SetVisible(true);
			m_RUpgradeSprite->SetVisible(false);
		}
		else
		{
			m_GUpgradeSprite->SetVisible(false);
			m_RUpgradeSprite->SetVisible(true);
		}

		CCRect rect = this->GetFrameRect();
		CCRect rectT;
		if (m_nUpgradeIconPos == 0)
		{
			rectT = CCRectMake(-UPGRADE_ICON_W,
				rect.size.height - UPGRADE_ICON_H, UPGRADE_ICON_W,
				UPGRADE_ICON_H);
		}
		else
		{
			rectT = CCRectMake(rect.size.width,
				rect.size.height - UPGRADE_ICON_H, UPGRADE_ICON_W,
				UPGRADE_ICON_H);
		}
		m_GUpgradeSprite->SetFrameRect(rectT);
		m_RUpgradeSprite->SetFrameRect(rectT);
	}
	else
	{
		if (m_GUpgradeSprite)
		{
			m_GUpgradeSprite->SetVisible(false);
		}
		if (m_GUpgradeSprite)
		{
			m_RUpgradeSprite->SetVisible(false);
		}
	}
}

void CUIEquipItem::SetItemFrameRect(CGRect rect)
{
	SetFrameRect(rect);
}

void CUIEquipItem::CloseItemFrame()
{
	CloseFrame();
}

void CUIEquipItem::SetItemBackgroundPicture(NDPicture *pic,
											 NDPicture *touchPic /*= NULL*/, bool useCustomRect /*= false*/,
											 CGRect customRect /*= CGRectZero*/, bool clearPicOnFree /*= false*/)
{
	SetBackgroundPicture(pic, touchPic, useCustomRect, customRect,
		clearPicOnFree);
}

void CUIEquipItem::SetItemBackgroundPictureCustom(NDPicture *pic,
												   NDPicture *touchPic /*= NULL*/, bool useCustomRect /*= false*/,
												   CGRect customRect /*= CGRectZero*/)
{
	SetBackgroundPictureCustom(pic, touchPic, useCustomRect, customRect);
}

void CUIEquipItem::SetItemFocusImage(NDPicture *pic,
									  bool useCustomRect /*= false*/, CGRect customRect /*= CGRectZero*/,
									  bool clearPicOnFree /*= false*/)
{
	SetFocusImage(pic, useCustomRect, customRect, clearPicOnFree);
}

void CUIEquipItem::SetItemFocusImageCustom(NDPicture *pic,
											bool useCustomRect /*= false*/, CGRect customRect /*= CGRectZero*/)
{
	SetFocusImageCustom(pic, useCustomRect, customRect);
}

void CUIEquipItem::InitializationItem()
{
	Initialization();
}