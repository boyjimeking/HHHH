//
//  NDStyledNode.h
//  DragonDrive
//
//  Created by cl on 12-4-20.
//  Copyright 2011 (����)DeNA. All rights reserved.
//


#include "NDUILayer.h"
#include "NDUIImage.h"
#include "NDUILabel.h"
#include "AutoLink.h"
#include <list>

#define CHAT_FONTSIZE	(13)

enum CHAT_CHANNEL_TYPE {
	CHAT_CHANNEL_ALL=0,
	CHAT_CHANNEL_SYS,
	CHAT_CHANNEL_WORLD,
	CHAT_CHANNEL_FACTION,
	CHAT_CHANNEL_PRIVATE,
};
namespace NDEngine
{
	//�ڵ�
	typedef struct CHAT_NODE{
		bool hasBreak;
		bool bItem;
		NDUINode* uiNode;
		CHAT_NODE(bool brk, NDUINode* node, bool bItm = false)
		{
			bItem = bItm;
			hasBreak = brk;
			uiNode = node;
		}
	}ChatNode;
	
	typedef enum
	{
		ChatNone,			//�޹���Ĭ��
		ChatRole,			//���
		ChatFace,			//�������
		ChatItem,			//��Ʒ����
		ChatColor			//��ɫ����
	}ChatTextType;
	
	class CUIChatText :
	public NDUINode
	{
		DECLARE_CLASS(CUIChatText)
		
		CUIChatText();
		~CUIChatText();
	private:
		int contentWidth;
		int m_idItem;
		int contentHeight;
	public:
		void Initialization(); override
		void SetContentWidth(int width){this->contentWidth=width;}
		void SetContent(int speakerID,int channel,const char* speaker,const char* text);
		int GetContentHeight(){return contentHeight;}
	protected:
//		bool TouchEnd(NDTouch* touch); override

	private:
		NDUILabel* CreateLabel(const char* text, unsigned int fontSize, cocos2d::ccColor4B color, int idItem/* = 0*/);
		//��������β��
		bool AnalysisRuleEnd(const char*& text, ChatTextType type);
		//��������ͷ��
		bool AnalysisRuleHead(const char*& text, ChatTextType &type, cocos2d::ccColor4B &textColor);
		
		//��ȡ��������
		NDPicture* CreateFacePicture(unsigned int index);
		//��ȡ����ͼƬ ��ʽ��f00
		NDUIImage* CreateFaceImage(const char* strIndex);	
		void Combiner(std::vector<ChatNode>& textNodeList);
		unsigned char unsignedCharToHex(const char* usChar);
		std::string GetChannelStr(CHAT_CHANNEL_TYPE channel);
	};
}