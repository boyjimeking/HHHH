/*
 *  SyndicateList.mm
 *  DragonDrive
 *
 *  Created by wq on 11-4-6.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "SyndicateList.h"
#include "NDUISynLayer.h"
#include "GameScene.h"
#include "NDConstant.h"
#include "NDDirector.h"
#include "SocialTextLayer.h"
#include "SyndicateCommon.h"
#include <sstream>

IMPLEMENT_CLASS(SyndicateListUILayer, NDUIMenuLayer)

SyndicateListUILayer* SyndicateListUILayer::s_instance = NULL;

void SyndicateListUILayer::refreshScroll(NDTransData& data)
{
	CloseProgressBar;
	if (!s_instance) {
		NDScene* scene = NDDirector::DefaultDirector()->GetRunningScene();
		if (scene && scene->IsKindOfClass(RUNTIME_CLASS(GameScene))) {
			GameScene* gameScene = (GameScene*)scene;
			SyndicateListUILayer *list = new SyndicateListUILayer;
			list->Initialization();
			gameScene->AddChild(list, UILAYER_Z);
			gameScene->SetUIShow(true);
		} else {
			return;
		}
	}
	
	s_instance->refreshMainList(data);
}

void SyndicateListUILayer::releaseElement()
{
	for (VEC_SOCIAL_ELEMENT_IT it = this->m_vSyn.begin(); it != this->m_vSyn.end(); it++) {
		SAFE_DELETE(*it);
	}
	m_vSyn.clear();
}

void SyndicateListUILayer::refreshMainList(NDTransData& data)
{
	// 清除相关数据
	this->m_curSelEle = NULL;
	
	if (this->m_optLayer) {
		this->m_optLayer->RemoveFromParent(true);
		this->m_optLayer = NULL;
	}
	
	NDDataSource *ds = m_tlMain->GetDataSource();
	ds->Clear();
	NDSection* sec = new NDSection;
	ds->AddSection(sec);
	
	this->releaseElement();
	
	int usSynCount = data.ReadShort(); // 总帮派数
	m_nCurPage = data.ReadByte(); // 当前页数
	
	int btCurPageMbrCount = data.ReadByte(); // 当前页帮派数
	
	int t_totalPage = 1;
	if (usSynCount % ONE_PAGE_COUNT == 0) {
		t_totalPage = usSynCount / ONE_PAGE_COUNT;
	} else {
		t_totalPage = usSynCount / ONE_PAGE_COUNT + 1;
	}
	t_totalPage = max(1, t_totalPage);
	
	this->m_btnPage->SetPages(m_nCurPage + 1, t_totalPage);
	
	bool bChangeClr = false;
	for (int i = 0; i < btCurPageMbrCount; i++) {
		long long synContribute = data.ReadLong();
		int synId = data.ReadInt();
		string strMbrName = data.ReadUnicodeString();
		
		SocialElement* se = new SocialElement;
		this->m_vSyn.push_back(se);
		se->m_id = synId;
		se->m_text1 = strMbrName;
		
		stringstream ss;
		ss << synContribute;
		
		se->m_text2 = ss.str();
		
		SocialTextLayer* st = new SocialTextLayer;
		st->Initialization(CGRectMake(5.0f, 0.0f, 460.0f, 27.0f),
				   CGRectMake(10.0f, 0.0f, 450.0f, 27.0f), se);
		
		if (bChangeClr) {
			st->SetBackgroundColor(INTCOLORTOCCC4(0xc3d2d5));
		} else {
			st->SetBackgroundColor(INTCOLORTOCCC4(0xe3e5da));
		}
		
		bChangeClr = !bChangeClr;
		sec->AddCell(st);
	}
	
	this->m_tlMain->ReflashData();
}

SyndicateListUILayer::SyndicateListUILayer()
{
	NDAsssert(s_instance == NULL);
	s_instance = this;
	this->m_tlMain = NULL;
	this->m_curSelEle = NULL;
	this->m_optLayer = NULL;
	this->m_btnPage = NULL;
	m_nCurPage = 1;
}

SyndicateListUILayer::~SyndicateListUILayer()
{
	s_instance = NULL;
	this->releaseElement();
}

void SyndicateListUILayer::OnButtonClick(NDUIButton* button)
{
	if (button == this->GetCancelBtn())
	{
		if (this->m_optLayer) {
			this->m_optLayer->RemoveFromParent(true);
			this->m_optLayer = NULL;
		} else {
			if (this->GetParent() && this->GetParent()->IsKindOfClass(RUNTIME_CLASS(GameScene))) 
			{
				((GameScene*)(this->GetParent()))->SetUIShow(false);
				this->RemoveFromParent(true);
			}
		}
	}
}

void SyndicateListUILayer::OnTableLayerCellSelected(NDUITableLayer* table, NDUINode* cell, unsigned int cellIndex, NDSection* section)
{
	if (this->m_tlMain == table) {
		this->m_curSelEle = ((SocialTextLayer*)cell)->GetSocialElement();
		// 显示操作选项
		NDUITableLayer* opt = new NDUITableLayer;
		opt->Initialization();
		opt->VisibleSectionTitles(false);
		opt->SetDelegate(this);
		
		CGSize winSize = NDDirector::DefaultDirector()->GetWinSize();
		int nHeight = 0;
		
		NDDataSource* ds = new NDDataSource;
		NDSection* sec = new NDSection;
		ds->AddSection(sec);
		opt->SetDataSource(ds);
		
		NDUIButton* btn = new NDUIButton;
		btn->Initialization();
		btn->SetTitle(NDCommonCString("ApplyJoin"));
		btn->SetFocusColor(ccc4(253, 253, 253, 255));
		sec->AddCell(btn);
		nHeight += 30;
		
		btn = new NDUIButton;
		btn->Initialization();
		btn->SetTitle(NDCommonCString("InfoChangKang"));
		btn->SetFocusColor(ccc4(253, 253, 253, 255));
		sec->AddCell(btn);
		nHeight += 30;
		
		opt->SetFrameRect(CGRectMake((winSize.width - 94) / 2, (winSize.height - nHeight) / 2, 94, nHeight));
		sec->SetFocusOnCell(0);
		
		this->m_optLayer = new NDOptLayer;
		this->m_optLayer->Initialization(opt);
		this->AddChild(m_optLayer);
	} else if (this->m_optLayer && this->m_optLayer->GetOpt() == table) {
		switch (cellIndex) {
			case 0: // 申请加入
			{
				sendApply(this->m_curSelEle->m_id);
			}
				break;
			case 1: // 查看
			{
				sendQueryTaxisDetail(this->m_curSelEle->m_id);
			}
				break;
			default:
				break;
		}
		this->m_optLayer->RemoveFromParent(true);
		this->m_optLayer = NULL;
	}
}

void SyndicateListUILayer::Initialization()
{
	NDUIMenuLayer::Initialization();
	
	CGSize winsize = NDDirector::DefaultDirector()->GetWinSize();
	
	if ( this->GetCancelBtn() ) 
	{
		this->GetCancelBtn()->SetDelegate(this);
	}
	
	NDUILabel* title = new NDUILabel;
	title->Initialization();
	title->SetText(NDCommonCString("JunTuanPaiHanBang"));
	title->SetFontSize(15);
	title->SetTextAlignment(LabelTextAlignmentCenter);
	title->SetFrameRect(CGRectMake(0, 5, winsize.width, 15));
	title->SetFontColor(ccc4(255, 240, 0,255));
	this->AddChild(title);
	
	NDUILayer* columnTitle = new NDUILayer;
	columnTitle->Initialization();
	columnTitle->SetBackgroundColor(ccc4(115, 117, 115, 255));
	columnTitle->SetFrameRect(CGRectMake(7, 32, 466, 17));
	this->AddChild(columnTitle);
	
	title = new NDUILabel;
	title->Initialization();
	title->SetText(NDCommonCString("JunTuanName"));
	title->SetFontSize(15);
	title->SetTextAlignment(LabelTextAlignmentLeft);
	title->SetFrameRect(CGRectMake(12, 32, 456, 17));
	title->SetFontColor(ccc4(0, 0, 0,255));
	this->AddChild(title);
	
	title = new NDUILabel;
	title->Initialization();
	title->SetText(NDCommonCString("ContributeDu"));
	title->SetFontSize(15);
	title->SetTextAlignment(LabelTextAlignmentRight);
	title->SetFrameRect(CGRectMake(12, 32, 456, 17));
	title->SetFontColor(ccc4(0, 0, 0,255));
	this->AddChild(title);
	
	this->m_tlMain = new NDUITableLayer;
	m_tlMain->Initialization();
	m_tlMain->SetDelegate(this);
	m_tlMain->SetFrameRect(CGRectMake(2, 50, 476, 200));
	m_tlMain->VisibleSectionTitles(false);
	m_tlMain->SetBackgroundColor(ccc4(255, 255, 255, 0));
	this->AddChild(m_tlMain);
	m_tlMain->SetDataSource(new NDDataSource);
	
	m_btnPage = new NDPageButton;
	m_btnPage->Initialization(CGRectMake(160.0f, 250.0f, 160.0f, 24.0f));
	m_btnPage->SetDelegate(this);
	this->AddChild(m_btnPage);
}

void SyndicateListUILayer::OnPageChange(int nCurPage, int nTotalPage)
{
	this->m_nCurPage = nCurPage - 1;
	sendQueryTaxis(m_nCurPage);
}