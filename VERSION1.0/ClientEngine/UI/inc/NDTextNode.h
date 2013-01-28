//
//  NDStyledNode.h
//  DragonDrive
//
//  Created by xiezhenghai on 11-3-22.
//  Copyright 2011 (����)DeNA. All rights reserved.
//

#ifndef __NDTextNode_H
#define __NDTextNode_H

#include "NDUINode.h"
#include "NDUIImage.h"
#include "NDUILabel.h"
#include "NDUIOptionButton.h"
#include <vector>
#include <map>
#include "HyperLinkLabel.h"


namespace NDEngine
{
//�ڵ�
typedef struct TEXT_NODE
{
	bool hasBreak;
	bool bItem;
	NDUINode* uiNode;
	TEXT_NODE(bool brk, NDUINode* node, bool bItm = false)
	{
		bItem = bItm;
		hasBreak = brk;
		uiNode = node;
	}
} TextNode;

typedef enum
{
	BuildRuleNone,			//�޹���Ĭ��
	BuildRuleExpression,	//�������
	BuildRuleItem,			//��Ʒ����
	BuildRuleColor,			//��ɫ����
	BuildRuleLine			//���»��߹���
} BuildRule;

class NDUIText: public NDUINode
{
	DECLARE_CLASS (NDUIText)
	NDUIText();
	~NDUIText();
public:
	void Initialization(bool usePageArrowControl = true);
	unsigned int GetPageCount()
	{
		return m_uiPageCount;
	}
	unsigned int GetCurrentPageIndex();
	void SetBackgroundColor(cocos2d::ccColor4B color);
	bool OnTextClick(CCPoint touchPos);
	NDUINode* AddNewPage();
	void SetFontSize(unsigned int uiFontSize);
	void SetFontColor(cocos2d::ccColor4B color);
	unsigned int GetFontSize();
	cocos2d::ccColor4B GetFontColor();
public:
	void draw();
	void OnOptionChange(NDUIOptionButton* option);
	void SetVisible(bool visible);
	void SetFrameRect(CCRect rect);
	void ActivePage(unsigned int pageIndex);
private:
	unsigned int m_uiPageCount;
	unsigned int m_uiCurrentPageIndex;
	cocos2d::ccColor4B m_kBackgroundColor;
	NDUIOptionButton* m_pkPageArrow;
	std::vector<NDUINode*> m_pkPages;
	bool m_bUsePageArrowControl;
	unsigned int m_uiFontSize;
	cocos2d::ccColor4B m_kColorFont;
};

//�ڵ�������
class NDUITextBuilder: public NDObject
{
	DECLARE_CLASS (NDUITextBuilder)
	NDUITextBuilder();
	~NDUITextBuilder();
public:
	//����
	static NDUITextBuilder* DefaultBuilder();
	//�����ַ���
	NDUIText* Build(const char* pszText, unsigned int uiFontSize,
			CCSize kContainerSize,
			cocos2d::ccColor4B kDefaultColor = ccc4(0, 0, 0, 255),
			bool bWithPageArrow = false, bool bHpyerLink = false);
	unsigned int StringHeightAfterFilter(const char* text,
			unsigned int textWidth, unsigned int fontSize);
	unsigned int StringWidthAfterFilter(const char* text,
			unsigned int textWidth, unsigned int fontSize);

private:
	NDUIText* Build_WithNDBitmap(const char* pszText, unsigned int uiFontSize,
		CCSize kContainerSize,
		cocos2d::ccColor4B kDefaultColor = ccc4(0, 0, 0, 255),
		bool bWithPageArrow = false, bool bHpyerLink = false);

private:
	//��λ16�����ַ���ת16����ֵ
	unsigned char unsignedCharToHex(const char* usChar);
	//��������β��
	bool AnalysisRuleEnd(const char*& text, BuildRule rule);
	//��������ͷ��
	bool AnalysisRuleHead(const char*& pszText, BuildRule &eRole,
			cocos2d::ccColor4B &kTextColor);
	//��Ͻڵ�
	NDUIText* Combiner(std::vector<TextNode>& textNodeList,
			CCSize containerSize, bool withPageArrow);
	//��ȡ��������
	NDPicture* CreateFacePicture(unsigned int index);
	//��ȡ����ͼƬ ��ʽ��f00
	NDUIImage* CreateFaceImage(const char* strIndex);
	//��ȡ��ǩ
	NDUILabel* CreateLabel(const char* pszText, unsigned int fontSize,
			cocos2d::ccColor4B color, int idItem = 0);
	HyperLinkLabel* CreateLinkLabel(const char* pszText,
			unsigned int uiFontSize, ccColor4B kColor, int nItemID = 0);
private:
	int m_nItemID;
};
}

#endif
