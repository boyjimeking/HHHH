/*
 *  NewVipStore.h
 *  DragonDrive
 *
 *  Created by jhzheng on 11-11-4.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _NEW_VIP_STROE_H_
#define _NEW_VIP_STROE_H_

#include "NDCommonScene.h"
#include "NDUIItemButton.h"
#include "NDCommonControl.h"
#include "BattleFieldShop.h"

using namespace NDEngine;

class VipItem;

#pragma mark 系统商店信息
class VipShopInfo :
public NDUILayer,
public NDUIButtonDelegate
{
	DECLARE_CLASS(VipShopInfo)
	
	enum { max_btns = 20, col = 5, };
	
public:
	VipShopInfo();
	
	~VipShopInfo();
	
	void Initialization(int shopType); override
	
	void OnButtonClick(NDUIButton* button); override
	
	void Update(); // 包括AttrPanel, 令牌, 荣誉值
	
private:
	NDUIItemButton				*m_btnCurItemButton;
	NDUILabel					*m_lbItemName, *m_lbItemLvl;
	NDUIImage					*m_imgMedal, *m_imgRepute;
	NDUILabel					*m_lbItemPrice, *m_lbVipPrice;
	NDUILabel					*m_lbReqMedal, *m_lbReqRepute;
	
	NDUIContainerScrollLayer	*m_descScroll;
	
	NDSlideBar					*m_slide;
	
	NDUIButton					*m_btnBuy;
	
	NDUIItemButton				*m_btnGood[max_btns];
	
	NDUILabel					*m_lbEMoney, *m_lbMoney;
	
	NDUIButton					*m_btnPrev, *m_btnNext;
	
	NDUILabel					*m_lbPage;
	
	int							m_iCurPage;
	
	int							m_iShopType;
	
	int							m_iCurItemType;
	
	NDUILayer					*m_layerPrice;
	
private:
	void InitAttrPanel();
	void InitItemPanel();
	
	void ChangeAttrPanel(int bfItemType);
	int  GetAttrPanelBFItemType();
	
	void DealBuy();
	int GetCurBuyCount();
	int GetCanBuyMaxCount(int bfItemType);
	void SetBuyCount(int minCount, int maxCount);
	bool CheckBuyCount(int buyCount);
	
	void ShowNext();
	void ShowPrev();
	void OnClickGoodItem(NDUIButton *btn);
	
	void UpdateCurpageGoods(); // 包括商品,页标签
	void UpdateAttrPanel();
	void UpdateMoney();
	
	bool SetDescContent(const char *text, ccColor4B color=ccc4(0, 0, 0, 255), unsigned int fontsize=12);
	int GetGoodPageCount();
	
	bool GetVipItemInfo(int bfItemType, VipItem*& pVipItem);
};


#pragma mark 系统商店

class SystemShopInfoNew;

class VipShop :
public NDCommonLayer
{
	DECLARE_CLASS(VipShop)
public:
	VipShop();
	~VipShop();
	
	void Initialization(){}
	
	void UpdateShopInfo(){}
};

class SystemShopUIItem : public ShopUIItem
{
	DECLARE_CLASS(SystemShopUIItem)
	
public:
	SystemShopUIItem();
	
	~SystemShopUIItem();
	
	void Initialization(int shopType); override
	
	void ChangeSystemShopItem(int sysItemType);
	
private:
	NDUILabel					*m_lbItemName;
	NDUIImage					*m_imgEmoney;
	int							m_iShopType;
	NDUILayer					*m_layerPrice;
private:
	bool GetVipItemInfo(int sysItemType, VipItem*& pVipItem);
};

#pragma mark 系统商店信息-新
class SystemShopInfoNew :
public NDUILayer,
public NDUIButtonDelegate
{
	DECLARE_CLASS(SystemShopInfoNew)
	
	enum { max_btns = 12, col = 3, };
	
public:
	SystemShopInfoNew();
	
	~SystemShopInfoNew();
	
	void Initialization(int shopType); override
	
	void OnButtonClick(NDUIButton* button); override
	
	void OnFocusShopUIItem(ShopUIItem* shopUiItem);
	
	void Update(); // 包括AttrPanel, 令牌, 荣誉值
	
private:
	NDSlideBar					*m_slide;
	
	NDUIButton					*m_btnBuy;
	
	BFShopUIItem				*m_btnGood[max_btns];
	
	NDUILabel					*m_lbEmoney, *m_lbMoney;
	
	NDUIButton					*m_btnPrev, *m_btnNext;
	
	NDUILabel					*m_lbPage;
	
	int							m_iCurPage;
	
	int							m_iShopType;
	
	int							m_iCurItemType;
	
	
	NDUIContainerScrollLayer	*m_scrollItem;
	
	std::vector<SystemShopUIItem*>  m_vGoods;
	
private:
	void InitAttrPanel();
	void InitItemPanel();
	
	void ChangeAttrPanel(int bfItemType);
	int  GetAttrPanelBFItemType();
	
	void DealBuy();
	int GetCurBuyCount();
	int GetCanBuyMaxCount(int bfItemType);
	void SetBuyCount(int minCount, int maxCount);
	bool CheckBuyCount(int buyCount);
	
	void ShowNext();
	void ShowPrev();
	void OnClickGoodItem(ShopUIItem* shopUiItem);
	
	void UpdateCurpageGoods(); // 包括商品,页标签
	void UpdateAttrPanel();
	void UpdateMoney();
	
	int GetGoodPageCount();
	
	bool GetVipItemInfo(int sysItemType, VipItem*& pVipItem);
};

#endif // _NEW_VIP_STROE_H_