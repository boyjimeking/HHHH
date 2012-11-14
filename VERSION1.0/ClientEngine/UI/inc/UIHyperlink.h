/*
 *  UIHyperlink.h
 *  SMYS
 *
 *  Created by jhzheng on 12-2-23.
 *  Copyright 2012 (����)DeNA. All rights reserved.
 *
 */

#ifndef _UI_HYPER_LINK_H_ZJH_
#define _UI_HYPER_LINK_H_ZJH_

#include "NDTextNode.h"
#include "NDUIButton.h"

using namespace NDEngine;

class CUIHyperlinkText : public NDUINode
{
	DECLARE_CLASS(CUIHyperlinkText)
	CUIHyperlinkText();
	~CUIHyperlinkText();
	
public:
	void Initialization(); override
	void SetLinkBoundRect(CCRect rectBound);
	CCRect GetLinkBoundRect();
	void SetLinkText(const char* text);
	void SetLinkTextFontSize(unsigned int uiFontSize);
	void SetLinkTextColor(ccColor4B color);
	void SetLinkTextAlignment(int alignment);
	void EnableLine(bool bEnable);
	const char* GetLinkText();
	bool IsLineEnable();
	int  GetLinkTextAlignment();
private:
	CCRect					m_rectLinkRect;
	unsigned int			m_uiLinkFontSize;
	ccColor4B				m_colorLinkFont;
	NDUIText*				m_uiLinkText;
	bool					m_bLineEnabel;
	std::string				m_strText;
	LabelTextAlignment		m_alignment;
protected:
	bool CanDestroyOnRemoveAllChildren(NDNode* pNode);override
};

////////////////////////////////////////////////////////////////////////////

class CUIHyperlinkButton : public NDUIButton
{
	DECLARE_CLASS(CUIHyperlinkButton)
	CUIHyperlinkButton();
	~CUIHyperlinkButton();
	
public:
	void Initialization(); override
	void SetLinkBoundRect(CCRect rectBound);
	void SetLinkText(const char* text);
	void SetLinkTextFontSize(unsigned int uiFontSize);
	void SetLinkTextColor(ccColor4B color);
	void EnableLine(bool bEnable);
	void SetLinkTextAlignment(int alignment);
	int	 GetLinkTextAlignment();
private:
	CUIHyperlinkText*	m_hyperlinkText;
	CCRect				m_rectLinkRect;
	
public:
	void draw();
protected:	
	bool CanDestroyOnRemoveAllChildren(NDNode* pNode);override
};


#endif // _UI_HYPER_LINK_H_ZJH_