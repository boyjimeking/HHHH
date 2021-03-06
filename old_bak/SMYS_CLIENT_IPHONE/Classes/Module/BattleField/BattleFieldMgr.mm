/*
 *  BattleFieldMgr.mm
 *  DragonDrive
 *
 *  Created by jhzheng on 11-11-4.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "BattleFieldMgr.h"
#include "BattleFieldData.h"
#include "NDDirector.h"
#include "BattleFieldScene.h"
#include "NDUISynLayer.h"
#include "NDUtility.h"
#include "GlobalDialog.h"
#include "BattleMgr.h"
#include "ItemMgr.h"

#include <sstream>

IMPLEMENT_CLASS(BattleFieldMgr, NDObject)

BattleFieldMgr::BattleFieldMgr()
{
	NDNetMsgPool& pool = NDNetMsgPoolObj;
	
	pool.RegMsg(_MSG_SHOP_BATTLE, this);
	
	pool.RegMsg(_MSG_SHOP_BATTLE_GOODS_TYPE, this);
	
	/**可用战场的列表*/
	pool.RegMsg(_MSG_BATTLEFIELD_LIST, this);
	
	/**战场状态报名信息*/
	pool.RegMsg(_MSG_BATTLEFIELD_SIGNUP, this);
	
	/**战场状态报名更新信息*/
	pool.RegMsg(_MSG_BATTLEFIELD_UPDATE_SIGNIN, this);
	
	/**战场背景介绍*/
	pool.RegMsg(_MSG_BATTLEFIELD_INTRO, this);
	
	/**战场报名*/
	pool.RegMsg(_MSG_BATTLEFIELD_SIGNIN, this);
	
	pool.RegMsg(_MSG_BATTLEFIELD_RELIVE, this);
	
	pool.RegMsg(_MSG_BATTLEFIELD_CONFIRM, this);
	
	m_dlgEnterBFConfirm = NULL;
}

BattleFieldMgr::~BattleFieldMgr()
{
}

void BattleFieldMgr::OnDialogClose(NDUIDialog* dialog)
{
	m_dlgEnterBFConfirm = NULL;
}

void BattleFieldMgr::OnDialogButtonClick(NDUIDialog* dialog, unsigned int buttonIndex)
{
	if (dialog == m_dlgEnterBFConfirm)
	{
		NDTransData bao(_MSG_BATTLEFIELD_CONFIRM);
		
		int bfType = dialog->GetTag();
		
		bao << bfType;
		
		if (buttonIndex == 0)
		{ // 立刻进入战场
			bao << (unsigned char)1;
		}
		else if (buttonIndex == 1)
		{ // 稍后进入战场
			bao << (unsigned char)2;
		}
		SEND_DATA(bao);
		
		dialog->Close();
	}
}

bool BattleFieldMgr::process(MSGID msgID, NDTransData* data, int len)
{
	switch (msgID) 
	{
		case _MSG_SHOP_BATTLE:
			processShopBattle(*data);
			break;
		case _MSG_SHOP_BATTLE_GOODS_TYPE:
			processShopBattleGoodsType(*data);
			break;
		case _MSG_BATTLEFIELD_LIST:
			processBattleFieldList(*data);
			break;
		case _MSG_BATTLEFIELD_SIGNUP:
			processBattleFieldSignUp(*data);
			break;
		case _MSG_BATTLEFIELD_UPDATE_SIGNIN:
			processBattleFieldUpdateSignIn(*data);
			break;
		case _MSG_BATTLEFIELD_INTRO:
			processBattleFieldIntro(*data);
			break;
		case _MSG_BATTLEFIELD_SIGNIN:
			processBattleFieldSignIn(*data);
			break;
		case _MSG_BATTLEFIELD_RELIVE:
			processBattleFieldRelive(*data);
			break;
		case _MSG_BATTLEFIELD_CONFIRM:
			processBattleFieldConfirm(*data);
		default:
			break;
	}
	
	return true;
}

void BattleFieldMgr::processShopBattle(NDTransData& data)
{
	int btPackage = data.ReadByte();
	
	int btAmount = data.ReadByte();
	
	map_bf_iteminfo& mapBfItemInfo = BattleField::mapItemInfo;
	
	if (btPackage == 0)
	{ // begin
		mapBfItemInfo.clear();
	}
	
	for (int i = 0; i < btAmount; i++) 
	{
		int btGoodsType = data.ReadByte();
		int iItemType = data.ReadInt();
		int iMedalItemType = data.ReadInt();
		int iMedalReq = data.ReadInt();
		int iHonourReq = data.ReadInt();
		
		ItemMgrObj.QueryItemType(iItemType);
		
		/*
		map_bf_desc& mapBfDesc = BattleField::mapDesc;
		
		if (mapBfDesc.find(btGoodsType) == mapBfDesc.end())
			continue;
		*/
		
		if (mapBfItemInfo.find(btGoodsType) ==  mapBfItemInfo.end())
		{
			vec_bf_item vBfItem;
			mapBfItemInfo.insert(pair_bf_iteminfo(btGoodsType, vBfItem));
		}
		
		vec_bf_item& vBfItem = mapBfItemInfo[btGoodsType];
		
		vBfItem.push_back(BFItemInfo(btGoodsType, iItemType, iMedalItemType, iMedalReq, iHonourReq));
	}
	
	/*
	if (btPackage == 1 && !BattleField::mapDesc.empty())
	{ // end
		BattleFieldScene *scene = BattleFieldScene::Scene();
		scene->Initialization();
		scene->SetTabFocusOnIndex(eBattleFieldShop);
		NDDirector::DefaultDirector()->PushScene(scene);
	}
	*/
}

void BattleFieldMgr::processShopBattleGoodsType(NDTransData& data)
{
	map_bf_desc& mapBfDesc = BattleField::mapDesc;
	
	int btAmount = data.ReadByte();
	
	for (int i = 0; i < btAmount; i++) 
	{
		int btGoodsType = data.ReadByte();
		
		std::string strGoodsName = data.ReadUnicodeString();
		
		mapBfDesc[btGoodsType] = strGoodsName;
	}
	
	NDScene *scene = NDDirector::DefaultDirector()->GetRunningScene();
	if (scene->IsKindOfClass(RUNTIME_CLASS(BattleFieldScene)))
	{
		((BattleFieldScene*)scene)->ShowShop();
	}
	
	CloseProgressBar;
}

void BattleFieldMgr::processBattleFieldList(NDTransData& data)
{
	map_bf_desc& desc = BattleField::mapApplyDesc;
	
	desc.clear();
	
	int count = data.ReadShort();
	
	for (int i = 0; i < count; i++) 
	{
		int bfType = data.ReadInt();
		desc[bfType] = data.ReadUnicodeString();
		
		NDLog(@"\n战场类型[%d],战场名字[%@]", bfType, [NSString stringWithUTF8String:desc[bfType].c_str()]);
	}
	
	NDScene *scene = NDDirector::DefaultDirector()->GetRunningScene();
	if (scene->IsKindOfClass(RUNTIME_CLASS(BattleFieldScene)))
	{
		((BattleFieldScene*)scene)->DealRecvBFDesc();
	}
	
	CloseProgressBar;
}

void BattleFieldMgr::processBattleFieldSignUp(NDTransData& data)
{
	// 报名信息
	int seq = 0, bfTypeId = 0, timeLeft = 0, playerLimit = 0, playerCount = 0;
	
	data >> seq >> bfTypeId >> timeLeft;
	
	playerLimit = data.ReadByte();
	playerCount = data.ReadByte();
	
	map_bf_apply_info& bfApplyInfo = BattleField::mapApplyInfo;
	
	BFApplyInfo& bfApply = bfApplyInfo[bfTypeId];
	
	bfApply.reset();
	bfApply.typeId = bfTypeId;
	bfApply.seqId = seq;
	bfApply.timeLeft = timeLeft;
	bfApply.playerLimit = playerLimit;
	bfApply.applyCount = playerCount;
	
	map_bf_apply& apply = bfApply.applyInfo;
	
	for (int i = 0; i < playerCount; i++) 
	{
		std::string playerName = data.ReadUnicodeString();
		int lev = data.ReadShort();
		std::string rank = data.ReadUnicodeString();
		apply.insert(pair_bf_apply(playerName, 
			BFPlayerInfo(playerName, lev, rank)));
	}
	
	int helpinfoFlag = 0;
	helpinfoFlag = data.ReadByte();
	
	if (helpinfoFlag == 1)
	{
		bfApply.rule = data.ReadUnicodeString();
	}
	
	NDScene *scene = NDDirector::DefaultDirector()->GetRunningScene();
	if (scene->IsKindOfClass(RUNTIME_CLASS(BattleFieldScene)))
	{
		BattleFieldScene* bfScene = (BattleFieldScene*)scene;
		if (bfScene && bfScene->GetApply())
			bfScene->GetApply()->ChangeBfApply(bfApply);
	}
	
	CloseProgressBar;
}

void BattleFieldMgr::processBattleFieldUpdateSignIn(NDTransData& data)
{
	// 报名信息更新
	int seq = 0, bfTypeId = 0, updateCount = 0;
	
	data >> seq >> bfTypeId;
	
	updateCount = data.ReadByte();
	
	map_bf_apply_info& bfApplyInfo = BattleField::mapApplyInfo;
	
	BFApplyInfo& bfApply = bfApplyInfo[bfTypeId];
	
	if (bfApply.seqId != uint(seq))
	{
		return;
	}
	
	map_bf_apply& applyInfo = bfApply.applyInfo;
	
	for (int i = 0; i < updateCount; i++) 
	{
		int action = 0;
		
		action = data.ReadByte();
		
		std::string playerName = data.ReadUnicodeString();
		
		if (action == 1)
		{ // 增加报名
			BFPlayerInfo& info = applyInfo[playerName];
			info.name = playerName;
			info.lvl = data.ReadShort();
			info.rank = data.ReadUnicodeString();
			bfApply.applyCount += 1;
		}
		else if (action == 2)
		{ // 移除报名
			applyInfo.erase(playerName);
			if (bfApply.applyCount > 0)
				bfApply.applyCount -= 0;
		}
	}
	
	int flag = 0;
	flag = data.ReadByte();
	if (flag == 1)
	{ // rule
		bfApply.rule = data.ReadUnicodeString();
	}
	
	NDScene *scene = NDDirector::DefaultDirector()->GetRunningScene();
	if (scene->IsKindOfClass(RUNTIME_CLASS(BattleFieldScene)))
	{
		BattleFieldScene* bfScene = (BattleFieldScene*)scene;
		if (bfScene && bfScene->GetApply())
			bfScene->GetApply()->UpdateBfApply(bfApply);
	}
}

void BattleFieldMgr::processBattleFieldIntro(NDTransData& data)
{
	int  bfTypeId = 0;
	
	data >> bfTypeId;
	
	std::string str = data.ReadUnicodeString();
	
	BattleField::mapApplyBackStory[bfTypeId] = str;
	
	NDScene *scene = NDDirector::DefaultDirector()->GetRunningScene();
	if (scene->IsKindOfClass(RUNTIME_CLASS(BattleFieldScene)))
	{
		BattleFieldScene* bfScene = (BattleFieldScene*)scene;
		if (bfScene->GetBackStory())
			bfScene->GetBackStory()->UpdateBfBackStory(bfTypeId, str);
	}
	
	CloseProgressBar;
}

void BattleFieldMgr::processBattleFieldSignIn(NDTransData& data)
{
	int  bfTypeId = 0, option = 0, res = 0;
	
	data >> bfTypeId;
	
	option = data.ReadByte();
	
	res = data.ReadByte();
	
	bool bSignIn = option == 1;
	
	
	
	map_bf_desc_it it = BattleField::mapApplyDesc.find(bfTypeId);
	
	std::string desc;
	
	if (it != BattleField::mapApplyDesc.end())
	{
		desc = it->second;
	}
	
	std::stringstream content;
	
	if (bSignIn)
	{
		if (res == 1)
		{
			content << NDCString("NinSucessBaoMing");
			content << desc;
		}
		else
		{
			content << NDCString("NinBaoMing");
			content << desc;
			content << NDCString("fail");
		}
	}
	else
	{
		if (res == 1)
		{
			content << NDCString("NinSucessCancelBaoMing");
			content << desc;
		}
		else
		{
			content << NDCString("NinCancelBaoMing");
			content << desc;
			content <<  NDCString("fail");
		}
	}
	
	GlobalShowDlg(NDCommonCString("tip"), content.str().c_str());
	
	CloseProgressBar;
}

void BattleFieldMgr::processBattleFieldRelive(NDTransData& data)
{
	int time = data.ReadShort();
	
	if (BattleMgrObj.GetBattle()) 
	{
		BattleFieldRelive::SetTimeCount(time);
	}
	else
	{
		BattleFieldRelive::Show(time);
	}
}

void BattleFieldMgr::processBattleFieldConfirm(NDTransData& data)
{
	int bfType = 0;
	bfType = data.ReadInt(); // 战场id 暂时不用
	std::string str = data.ReadUnicodeString();
	
	m_dlgEnterBFConfirm = new NDUIDialog;
	m_dlgEnterBFConfirm->Initialization();
	m_dlgEnterBFConfirm->SetTag(bfType);
	m_dlgEnterBFConfirm->SetTime(30);
	m_dlgEnterBFConfirm->SetDelegate(this);
	m_dlgEnterBFConfirm->Show(NDCString("BFConfirm"), str.c_str(), NULL, NDCString("OnceEnterBF"), NDCString("LaterEnterBF"), NULL);
}

void BattleFieldMgr::SendRequestApplyInfo(int bfType, bool bAttachRule)
{
	NDTransData bao(_MSG_BATTLEFIELD_SIGNUP);
	bao << bfType << (unsigned char)(bAttachRule ? 1 : 0);
	SEND_DATA(bao);
}

void BattleFieldMgr::SendSign(int bfType, int seq, bool apply)
{
	NDTransData bao(_MSG_BATTLEFIELD_SIGNIN);
	bao << seq << bfType << (unsigned char)(apply ? 1 : 2);
	SEND_DATA(bao);
	ShowProgressBar;
}

void BattleFieldMgr::SendRequestBfDesc()
{
	NDTransData bao(_MSG_BATTLEFIELD_LIST);
	SEND_DATA(bao);
	ShowProgressBar;
}

void BattleFieldMgr::SendRequestBfBackStory(int bfType)
{
	NDTransData bao(_MSG_BATTLEFIELD_INTRO);
	bao << bfType;
	SEND_DATA(bao);
	ShowProgressBar;
}

void BattleFieldMgr::SendRequestBfRelive(bool reliveInCurPlace)
{
	NDTransData bao(_MSG_BATTLEFIELD_RELIVE);
	bao << (unsigned char)(reliveInCurPlace ? 2 : 1);
	SEND_DATA(bao);
	ShowProgressBar;
}