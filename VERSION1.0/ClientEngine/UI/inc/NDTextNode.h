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
	typedef struct TEXT_NODE{
		bool hasBreak;
		bool bItem;
		NDUINode* uiNode;
		TEXT_NODE(bool brk, NDUINode* node, bool bItm = false)
		{
			bItem = bItm;
			hasBreak = brk;
			uiNode = node;
		}
	}TextNode;
	
	typedef enum
	{
		BuildRuleNone,			//�޹���Ĭ��
		BuildRuleExpression,	//�������
		BuildRuleItem,			//��Ʒ����
		BuildRuleColor			//��ɫ����
	}BuildRule;
	
	class NDUIText : public NDUINode
	{
		DECLARE_CLASS(NDUIText)
		NDUIText();
		~NDUIText();
	public:
		void Initialization(bool usePageArrowControl = true); override
		unsigned int GetPageCount(){ return m_pageCount; }
		unsigned int GetCurrentPageIndex();
		void SetBackgroundColor(cocos2d::ccColor4B color);
		bool OnTextClick(CGPoint touchPos);
		NDUINode* AddNewPage();	
		void SetFontSize(unsigned int uiFontSize);
		void SetFontColor(cocos2d::ccColor4B color);
		unsigned int GetFontSize();
		cocos2d::ccColor4B GetFontColor();
	public:
		void draw(); override
		void OnOptionChange(NDUIOptionButton* option); override
		void SetVisible(bool visible); override	
		void SetFrameRect(CGRect rect); override
		void ActivePage(unsigned int pageIndex);
	private:
		unsigned int m_pageCount, m_currentPageIndex;
		cocos2d::ccColor4B m_backgroundColor;
		NDUIOptionButton* m_pageArrow;
		std::vector<NDUINode*> m_pages;
		bool m_usePageArrowControl;
		unsigned int m_uiFontSize;
		cocos2d::ccColor4B	 m_colorFont;
	};
	
	//�ڵ�������
	class NDUITextBuilder : public NDObject
	{
		DECLARE_CLASS(NDUITextBuilder)
		NDUITextBuilder();
		~NDUITextBuilder();
	public:
		//����
		static NDUITextBuilder* DefaultBuilder();
		//�����ַ���
		NDUIText* Build(const char* text, unsigned int fontSize, CGSize containerSize, cocos2d::ccColor4B defaultColor = ccc4(0, 0, 0, 255), bool withPageArrow = false, bool bHpyerLink = false);
		unsigned int StringHeightAfterFilter(const char* text, unsigned int textWidth, unsigned int fontSize);
		unsigned int StringWidthAfterFilter(const char* text, unsigned int textWidth, unsigned int fontSize);
	private:	
		//��λ16�����ַ���ת16����ֵ
		unsigned char unsignedCharToHex(const char* usChar);
		//��������β��
		bool AnalysisRuleEnd(const char*& text, BuildRule rule);
		//��������ͷ��
		bool AnalysisRuleHead(const char*& text, BuildRule &rule, cocos2d::ccColor4B &textColor);
		//��Ͻڵ�
		NDUIText* Combiner(std::vector<TextNode>& textNodeList, CGSize containerSize, bool withPageArrow);
		//��ȡ��������
		NDPicture* CreateFacePicture(unsigned int index);
		//��ȡ����ͼƬ ��ʽ��f00
		NDUIImage* CreateFaceImage(const char* strIndex);		
		//��ȡ��ǩ
		NDUILabel* CreateLabel(const char* text, unsigned int fontSize, cocos2d::ccColor4B color, int idItem = 0);
		//HyperLinkLabel* CreateLinkLabel(const char* text, unsigned int fontSize, cocos2d::ccColor4B color, int idItem = 0); ///< ��ʱ��ע�� ����
	private:
		int m_idItem;
	};	
}


#endif
