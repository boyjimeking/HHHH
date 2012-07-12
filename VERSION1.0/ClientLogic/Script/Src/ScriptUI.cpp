/*
 *  ScriptUI.mm
 *  DragonDrive
 *
 *  Created by jhzheng on 12-1-6.
 *  Copyright 2012 (����)DeNA. All rights reserved.
 *
 */

#include "ScriptUI.h"
#include "ScriptInc.h"
#include "NDUILabel.h"
#include "NDUIButton.h"
#include "NDUITableLayer.h"
#include "NDUIImage.h"
#include "NDScrollLayer.h"

#include "define.h"
#include "NDUISynLayer.h"

#include "NDUILoad.h"
#include "CCPointExtension.h"
#include "NDNode.h"
#include "NDDirector.h"
#include "NDScene.h"
#include "NDUINode.h"
#include "NDUILayer.h"
#include "NDUIButton.h"
#include "NDPicture.h"
#include "NDUIImage.h"
#include "NDUILabel.h"
#include "NDUIDialog.h"

// smys begin
#include "UIScroll.h"
#include "UIScrollView.h"
#include "UIRoleNode.h"
#include "UIHyperlink.h"
#include "UIItemButton.h"
#include "UIEquipItem.h"
#include "UICheckBox.h"
#include "UIRadioButton.h"
#include "UIExp.h"
#include "UIEdit.h"
#include "UISpriteNode.h"
#include "UIChatText.h"
//...
// smys end

using namespace NDEngine;


#pragma mark ͨ���ڵ��ȡĳ�ڵ���ӽڵ�

NDUINode*							
GetUiNode(NDNode* pNode, int tag)
{
	if (!pNode)
	{
		return NULL;
	}
	
	NDNode *uinode = pNode->GetChild(tag);
	
	if (!uinode || !uinode->IsKindOfClass(RUNTIME_CLASS(NDUINode)))
	{
		return NULL;
	}
	
	return (NDUINode*)uinode;
}


NDUILabel*							
GetLabel(NDNode* pNode, int tag)
{
	if (!pNode)
	{
		return NULL;
	}
	
	NDNode *lb = pNode->GetChild(tag);
	
	if (!lb || !lb->IsKindOfClass(RUNTIME_CLASS(NDUILabel)))
	{
		return NULL;
	}
	
	return (NDUILabel*)lb;
}

NDUIButton*							
GetButton(NDNode* pNode, int tag)
{
	if (!pNode)
	{
		return NULL;
	}
	
	NDNode *btn = pNode->GetChild(tag);
	
	if (!btn || !btn->IsKindOfClass(RUNTIME_CLASS(NDUIButton)))
	{
		return NULL;
	}
	
	return (NDUIButton*)btn;
}

NDUILayer*							
GetUiLayer(NDNode* pNode, int tag)
{
	if (!pNode)
	{
		return NULL;
	}
	
	NDNode *layer = pNode->GetChild(tag);
	
	if (!layer || !layer->IsKindOfClass(RUNTIME_CLASS(NDUILayer)))
	{
		return NULL;
	}
	
	return (NDUILayer*)layer;
}

NDUITableLayer*						
GetTableLayer(NDNode* pNode, int tag)
{
	if (!pNode)
	{
		return NULL;
	}
	
	NDNode *tl = pNode->GetChild(tag);
	
	if (!tl || !tl->IsKindOfClass(RUNTIME_CLASS(NDUITableLayer)))
	{
		return NULL;
	}
	
	return (NDUITableLayer*)tl;
}

NDUIImage*							
GetImage(NDNode* pNode, int tag)
{	
	if (!pNode)
	{
		return NULL;
	}
	
	NDNode *img = pNode->GetChild(tag);
	
	if (!img || !img->IsKindOfClass(RUNTIME_CLASS(NDUIImage)))
	{
		return NULL;
	}
	
	return (NDUIImage*)img;
}

NDUIContainerScrollLayer*			
GetScrollLayer(NDNode* pNode, int tag)
{
	if (!pNode)
	{
		return NULL;
	}
	
	NDNode *sl = pNode->GetChild(tag);
	
	if (!sl || !sl->IsKindOfClass(RUNTIME_CLASS(NDUIContainerScrollLayer)))
	{
		return NULL;
	}
	
	return (NDUIContainerScrollLayer*)sl;
}

NDUIContainerHScrollLayer*			
GetHScrollLayer(NDNode* pNode, int tag)
{
	if (!pNode)
	{
		return NULL;
	}
	
	NDNode *hsl = pNode->GetChild(tag);
	
	if (!hsl || !hsl->IsKindOfClass(RUNTIME_CLASS(NDUIContainerHScrollLayer)))
	{
		return NULL;
	}
	
	return (NDUIContainerHScrollLayer*)hsl;
}

CUIScrollViewContainer*			
GetScrollViewContainer(NDNode* pNode, int tag)
{
	if (!pNode)
	{
		return NULL;
	}
	
	NDNode *sc = pNode->GetChild(tag);
	
	if (!sc || !sc->IsKindOfClass(RUNTIME_CLASS(CUIScrollViewContainer)))
	{
		return NULL;
	}
	
	return (CUIScrollViewContainer*)sc;
}

CUIHyperlinkText*
GetHyperLinkText(NDNode* pNode, int tag)
{
	if (!pNode)
	{
		return NULL;
	}
	
	NDNode *hlt = pNode->GetChild(tag);
	
	if (!hlt || !hlt->IsKindOfClass(RUNTIME_CLASS(CUIHyperlinkText)))
	{
		return NULL;
	}
	
	return (CUIHyperlinkText*)hlt;
}

CUIHyperlinkButton*
GetHyperLinkButton(NDNode* pNode, int tag)
{
	if (!pNode)
	{
		return NULL;
	}
	
	NDNode* hlb = pNode->GetChild(tag);
	
	if (!hlb || !hlb->IsKindOfClass(RUNTIME_CLASS(CUIHyperlinkButton)))
	{
		return NULL;
	}
	
	return (CUIHyperlinkButton*)hlb;
}

CUIItemButton*
GetItemButton(NDNode* pNode, int tag)
{
	if (!pNode)
	{
		return NULL;
	}
	
	NDNode* itemBtn = pNode->GetChild(tag);
	
	if (!itemBtn || !itemBtn->IsKindOfClass(RUNTIME_CLASS(CUIItemButton)))
	{
		return NULL;
	}
	
	return (CUIItemButton*)itemBtn;
}

CUIEquipItem*							
GetEquipButton(NDNode* pNode, int tag)
{
	if (!pNode)
	{
		return NULL;
	}
	
	NDNode* equipBtn = pNode->GetChild(tag);
	
	if (!equipBtn || !equipBtn->IsKindOfClass(RUNTIME_CLASS(CUIEquipItem)))
	{
		return NULL;
	}
	
	return (CUIEquipItem*)equipBtn; 
}

#pragma mark ͨ��tag�б��ȡ�ӽ��

//ͨ��tag�б��ȡ�ڵ�
NDNode* RecursiveNode(NDNode* pParentNode, LuaObject& tagTable)
{
	if (!pParentNode || !tagTable.IsTable())
	{
		return NULL;
	}
	
	int nTableCount = tagTable.GetTableCount();
	if (nTableCount <= 0)
	{
		return NULL;
	}
	
	NDNode* pResultNode = pParentNode;
	for (int i = 1; i <= nTableCount; i++) 
	{
		LuaObject tag = tagTable[i];
		if (!tag.IsInteger())
		{
			return NULL;
		}
		
		if (!pResultNode)
		{
			return NULL;
		}
		pResultNode = pResultNode->GetChild(tag.GetInteger());
	}
	
	return pResultNode;
}

//ͨ��tag�б��ȡUI�ڵ�(�������ű�)
NDUINode*							
RecursiveUINode(NDNode* pParentNode, LuaObject tagTable)
{
	NDNode* pResultNode = RecursiveNode(pParentNode, tagTable);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(NDUINode)))
	{
		return NULL;
	}
	
	return (NDUINode*)pResultNode; 
}

//ͨ��tag�б��ȡ��ǩ�ڵ�
NDUILabel*							
RecursiveLabel(NDNode* pParentNode, LuaObject tagTable)
{
	NDNode* pResultNode = RecursiveNode(pParentNode, tagTable);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(NDUILabel)))
	{
		return NULL;
	}
	
	return (NDUILabel*)pResultNode; 
}

//ͨ��tag�б��ȡ��ť�ڵ�
NDUIButton*							
RecursiveButton(NDNode* pParentNode, LuaObject tagTable)
{
	NDNode* pResultNode = RecursiveNode(pParentNode, tagTable);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(NDUIButton)))
	{
		return NULL;
	}
	
	return (NDUIButton*)pResultNode; 
}

//ͨ��tag�б��ȡ��ڵ�
NDUILayer*							
RecursiveUILayer(NDNode* pParentNode, LuaObject tagTable)
{
	NDNode* pResultNode = RecursiveNode(pParentNode, tagTable);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(NDUILayer)))
	{
		return NULL;
	}
	
	return (NDUILayer*)pResultNode; 
}

//ͨ��tag�б��ȡ��ڵ�
NDUIImage*							
RecursiveImage(NDNode* pParentNode, LuaObject tagTable)
{
	NDNode* pResultNode = RecursiveNode(pParentNode, tagTable);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(NDUIImage)))
	{
		return NULL;
	}
	
	return (NDUIImage*)pResultNode; 
}

//ͨ��tag�б��ȡ�����ڵ�
CUIScroll*
RecursiveScroll(NDNode* pParentNode, LuaObject tagTable)
{
	NDNode* pResultNode = RecursiveNode(pParentNode, tagTable);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(CUIScroll)))
	{
		return NULL;
	}
	
	return (CUIScroll*)pResultNode; 
}

//ͨ��tag�б��ȡ������ڵ�
CUIScrollContainer*
RecursiveScrollContainer(NDNode* pParentNode, LuaObject tagTable)
{
	NDNode* pResultNode = RecursiveNode(pParentNode, tagTable);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(CUIScrollContainer)))
	{
		return NULL;
	}
	
	return (CUIScrollContainer*)pResultNode; 
}

//ͨ��tag�б��ȡ��ͼ�����ڵ�
CUIScrollViewContainer*							
RecursiveSVC(NDNode* pParentNode, LuaObject tagTable)
{
	NDNode* pResultNode = RecursiveNode(pParentNode, tagTable);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(CUIScrollViewContainer)))
	{
		return NULL;
	}
	
	return (CUIScrollViewContainer*)pResultNode; 
}

//ͨ��tag�б��ȡ��ͼ�ڵ�
CUIScrollView*							
RecursiveSV(NDNode* pParentNode, LuaObject tagTable)
{
	NDNode* pResultNode = RecursiveNode(pParentNode, tagTable);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(CUIScrollView)))
	{
		return NULL;
	}
	
	return (CUIScrollView*)pResultNode; 
}

//ͨ��tag�б��ȡ�������ı��ڵ�
CUIHyperlinkText*							
RecursiveHyperText(NDNode* pParentNode, LuaObject tagTable)
{
	NDNode* pResultNode = RecursiveNode(pParentNode, tagTable);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(CUIHyperlinkText)))
	{
		return NULL;
	}
	
	return (CUIHyperlinkText*)pResultNode; 
}

//ͨ��tag�б��ȡ�����Ӱ�ť�ڵ�
CUIHyperlinkButton*							
RecursiveHyperBtn(NDNode* pParentNode, LuaObject tagTable)
{
	NDNode* pResultNode = RecursiveNode(pParentNode, tagTable);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(CUIHyperlinkButton)))
	{
		return NULL;
	}
	
	return (CUIHyperlinkButton*)pResultNode; 
}

//ͨ��tag�б��ȡ��Ʒ��ť�ڵ�
CUIItemButton*							
RecursiveItemBtn(NDNode* pParentNode, LuaObject tagTable)
{
	NDNode* pResultNode = RecursiveNode(pParentNode, tagTable);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(CUIItemButton)))
	{
		return NULL;
	}
	
	return (CUIItemButton*)pResultNode; 
}

//ͨ��tag�б��ȡװ����ť�ڵ�
CUIEquipItem*							
RecursiveEquipBtn(NDNode* pParentNode, LuaObject tagTable)
{
	NDNode* pResultNode = RecursiveNode(pParentNode, tagTable);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(CUIEquipItem)))
	{
		return NULL;
	}
	
	return (CUIEquipItem*)pResultNode; 
}

//ͨ��tag�б��ȡCheckBox�ڵ�
CUICheckBox*							
RecursiveCheckBox(NDNode* pParentNode, LuaObject tagTable)
{
	NDNode* pResultNode = RecursiveNode(pParentNode, tagTable);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(CUICheckBox)))
	{
		return NULL;
	}
	
	return (CUICheckBox*)pResultNode; 
}

//ͨ��tag�б��ȡRadioButton�ڵ�
CUIRadioButton*							
RecursiveRadioBtn(NDNode* pParentNode, LuaObject tagTable)
{
	NDNode* pResultNode = RecursiveNode(pParentNode, tagTable);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(CUIRadioButton)))
	{
		return NULL;
	}
	
	return (CUIRadioButton*)pResultNode; 
}

//ͨ��tag�б��ȡRadioGroup�ڵ�
CUIRadioGroup*							
RecursiveRadioGroup(NDNode* pParentNode, LuaObject tagTable)
{
	NDNode* pResultNode = RecursiveNode(pParentNode, tagTable);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(CUIRadioGroup)))
	{
		return NULL;
	}
	
	return (CUIRadioGroup*)pResultNode; 
}

//ͨ��tag�б��ȡ�������ڵ�
CUIExp*							
RecursivUIExp(NDNode* pParentNode, LuaObject tagTable)
{
	NDNode* pResultNode = RecursiveNode(pParentNode, tagTable);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(CUIExp)))
	{
		return NULL;
	}
	
	return (CUIExp*)pResultNode; 
}

//ͨ��tag�б��ȡedit�ڵ�
CUIEdit*							
RecursivUIEdit(NDNode* pParentNode, LuaObject tagTable)
{
	NDNode* pResultNode = RecursiveNode(pParentNode, tagTable);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(CUIEdit)))
	{
		return NULL;
	}
	
	return (CUIEdit*)pResultNode; 
}

//ͨ��tag�б��ȡ����ڵ�
CUISpriteNode*							
RecursivUISprite(NDNode* pParentNode, LuaObject tagTable)
{
	NDNode* pResultNode = RecursiveNode(pParentNode, tagTable);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(CUISpriteNode)))
	{
		return NULL;
	}
	
	return (CUISpriteNode*)pResultNode; 
}


#pragma mark ͨ��tag�б��ȡ�����
//ͨ��tag�б��ȡ�ڵ�
NDNode* PRecursiveNode(NDNode* pChildNode, unsigned int nRecuriveCount)
{
	if (!pChildNode || nRecuriveCount == 0)
	{
		return NULL;
	}
	
	NDNode* pResultNode = pChildNode;
	for (unsigned int i = 0; i < nRecuriveCount; i++) 
	{
		if (!pResultNode)
		{
			return NULL;
		}
		pResultNode = pResultNode->GetParent();
	}
	
	return pResultNode;
}

//ͨ��tag�б��ȡUI�ڵ�(�������ű�)
NDUINode*							
PRecursiveUINode(NDNode* pChildNode, unsigned int nRecuriveCount)
{
	NDNode* pResultNode = PRecursiveNode(pChildNode, nRecuriveCount);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(NDUINode)))
	{
		return NULL;
	}
	
	return (NDUINode*)pResultNode; 
}

//ͨ��tag�б��ȡ��ǩ�ڵ�
NDUILabel*							
PRecursiveLabel(NDNode* pChildNode, unsigned int nRecuriveCount)
{
	NDNode* pResultNode = PRecursiveNode(pChildNode, nRecuriveCount);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(NDUILabel)))
	{
		return NULL;
	}
	
	return (NDUILabel*)pResultNode; 
}

//ͨ��tag�б��ȡ��ť�ڵ�
NDUIButton*							
PRecursiveButton(NDNode* pChildNode, unsigned int nRecuriveCount)
{
	NDNode* pResultNode = PRecursiveNode(pChildNode, nRecuriveCount);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(NDUIButton)))
	{
		return NULL;
	}
	
	return (NDUIButton*)pResultNode; 
}

//ͨ��tag�б��ȡ��ڵ�
NDUILayer*							
PRecursiveUILayer(NDNode* pChildNode, unsigned int nRecuriveCount)
{
	NDNode* pResultNode = PRecursiveNode(pChildNode, nRecuriveCount);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(NDUILayer)))
	{
		return NULL;
	}
	
	return (NDUILayer*)pResultNode; 
}

//ͨ��tag�б��ȡ��ڵ�
NDUIImage*							
PRecursiveImage(NDNode* pChildNode, unsigned int nRecuriveCount)
{
	NDNode* pResultNode = PRecursiveNode(pChildNode, nRecuriveCount);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(NDUIImage)))
	{
		return NULL;
	}
	
	return (NDUIImage*)pResultNode; 
}

//ͨ��tag�б��ȡ��ͼ�����ڵ�
CUIScrollViewContainer*							
PRecursiveSVC(NDNode* pChildNode, unsigned int nRecuriveCount)
{
	NDNode* pResultNode = PRecursiveNode(pChildNode, nRecuriveCount);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(CUIScrollViewContainer)))
	{
		return NULL;
	}
	
	return (CUIScrollViewContainer*)pResultNode; 
}

//ͨ��tag�б��ȡ��ͼ�ڵ�
CUIScrollView*							
PRecursiveSV(NDNode* pChildNode, unsigned int nRecuriveCount)
{
	NDNode* pResultNode = PRecursiveNode(pChildNode, nRecuriveCount);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(CUIScrollView)))
	{
		return NULL;
	}
	
	return (CUIScrollView*)pResultNode; 
}

//ͨ��tag�б��ȡ�������ı��ڵ�
CUIHyperlinkText*							
PRecursiveHyperText(NDNode* pChildNode, unsigned int nRecuriveCount)
{
	NDNode* pResultNode = PRecursiveNode(pChildNode, nRecuriveCount);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(CUIHyperlinkText)))
	{
		return NULL;
	}
	
	return (CUIHyperlinkText*)pResultNode; 
}

//ͨ��tag�б��ȡ�����Ӱ�ť�ڵ�
CUIHyperlinkButton*							
PRecursiveHyperBtn(NDNode* pChildNode, unsigned int nRecuriveCount)
{
	NDNode* pResultNode = PRecursiveNode(pChildNode, nRecuriveCount);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(CUIHyperlinkButton)))
	{
		return NULL;
	}
	
	return (CUIHyperlinkButton*)pResultNode; 
}

//ͨ��tag�б��ȡ��Ʒ��ť�ڵ�
CUIItemButton*							
PRecursiveItemBtn(NDNode* pChildNode, unsigned int nRecuriveCount)
{
	NDNode* pResultNode = PRecursiveNode(pChildNode, nRecuriveCount);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(CUIItemButton)))
	{
		return NULL;
	}
	
	return (CUIItemButton*)pResultNode; 
}

//ͨ��tag�б��ȡװ����ť�ڵ�
CUIEquipItem*							
PRecursiveEquipBtn(NDNode* pChildNode, unsigned int nRecuriveCount)
{
	NDNode* pResultNode = PRecursiveNode(pChildNode, nRecuriveCount);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(CUIEquipItem)))
	{
		return NULL;
	}
	
	return (CUIEquipItem*)pResultNode; 
}

//ͨ��tag�б��ȡCheckBox�ڵ�
CUICheckBox*							
PRecursiveCheckBox(NDNode* pParentNode, unsigned int nRecuriveCoun)
{
	NDNode* pResultNode = PRecursiveNode(pParentNode, nRecuriveCoun);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(CUICheckBox)))
	{
		return NULL;
	}
	
	return (CUICheckBox*)pResultNode; 
}

//ͨ��tag�б��ȡRadioButton�ڵ�
CUIRadioButton*							
PRecursiveRadioBtn(NDNode* pParentNode, unsigned int nRecuriveCount)
{
	NDNode* pResultNode = PRecursiveNode(pParentNode, nRecuriveCount);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(CUIRadioButton)))
	{
		return NULL;
	}
	
	return (CUIRadioButton*)pResultNode; 
}

//ͨ��tag�б��ȡRadioGroup�ڵ�
CUIRadioGroup*							
PRecursiveRadioGroup(NDNode* pParentNode, unsigned int nRecuriveCount)
{
	NDNode* pResultNode = PRecursiveNode(pParentNode, nRecuriveCount);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(CUIRadioGroup)))
	{
		return NULL;
	}
	
	return (CUIRadioGroup*)pResultNode; 
}

//ͨ��tag�б��ȡ�������ڵ�
CUIExp*	
PRecursiveUIExp(NDNode* pParentNode, unsigned int nRecuriveCount)
{
	NDNode* pResultNode = PRecursiveNode(pParentNode, nRecuriveCount);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(CUIExp)))
	{
		return NULL;
	}
	
	return (CUIExp*)pResultNode; 
}

//ͨ��tag�б��ȡedit���ڵ�
CUIEdit*	
PRecursiveUIEdit(NDNode* pParentNode, unsigned int nRecuriveCount)
{
	NDNode* pResultNode = PRecursiveNode(pParentNode, nRecuriveCount);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(CUIEdit)))
	{
		return NULL;
	}
	
	return (CUIEdit*)pResultNode; 
}

//ͨ��tag�б��ȡ����ڵ�
CUISpriteNode*	
PRecursiveUISprite(NDNode* pParentNode, unsigned int nRecuriveCount)
{
	NDNode* pResultNode = PRecursiveNode(pParentNode, nRecuriveCount);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(CUISpriteNode)))
	{
		return NULL;
	}
	
	return (CUISpriteNode*)pResultNode; 
}

#pragma mark �ڵ�����ת��
NDUINode*							
ConverToUiNode(NDNode* pNode)
{
	if (!pNode || !pNode->IsKindOfClass(RUNTIME_CLASS(NDUINode)))
	{
		return NULL;
	}
	
	return (NDUINode*)pNode;
}


NDUILabel*							
ConverToLabel(NDNode* pNode)
{
	if (!pNode || !pNode->IsKindOfClass(RUNTIME_CLASS(NDUILabel)))
	{
		return NULL;
	}
	
	return (NDUILabel*)pNode;
}

NDUIButton*							
ConverToButton(NDNode* pNode)
{
	if (!pNode || !pNode->IsKindOfClass(RUNTIME_CLASS(NDUIButton)))
	{
		return NULL;
	}
	
	return (NDUIButton*)pNode;
}

NDUILayer*							
ConverToUiLayer(NDNode* pNode)
{
	if (!pNode || !pNode->IsKindOfClass(RUNTIME_CLASS(NDUILayer)))
	{
		return NULL;
	}
	
	return (NDUILayer*)pNode;
}

NDUITableLayer*						
ConverToTableLayer(NDNode* pNode)
{
	if (!pNode || !pNode->IsKindOfClass(RUNTIME_CLASS(NDUITableLayer)))
	{
		return NULL;
	}
	
	return (NDUITableLayer*)pNode;
}

NDUIImage*							
ConverToImage(NDNode* pNode)
{	
	if (!pNode || !pNode->IsKindOfClass(RUNTIME_CLASS(NDUIImage)))
	{
		return NULL;
	}
	
	return (NDUIImage*)pNode;
}

/*
NDUIContainerScrollLayer*			
GetScrollLayer(NDNode* pNode, int tag)
{
	if (!pNode)
	{
		return NULL;
	}
	
	NDNode *sl = pNode->GetChild(tag);
	
	if (!sl || !sl->IsKindOfClass(RUNTIME_CLASS(NDUIContainerScrollLayer)))
	{
		return NULL;
	}
	
	return (NDUIContainerScrollLayer*)sl;
}

NDUIContainerHScrollLayer*			
GetHScrollLayer(NDNode* pNode, int tag)
{
	if (!pNode)
	{
		return NULL;
	}
	
	NDNode *hsl = pNode->GetChild(tag);
	
	if (!hsl || !hsl->IsKindOfClass(RUNTIME_CLASS(NDUIContainerHScrollLayer)))
	{
		return NULL;
	}
	
	return (NDUIContainerHScrollLayer*)hsl;
}
*/

CUIScrollViewContainer*			
ConverToSVC(NDNode* pNode)
{
	if (!pNode || !pNode->IsKindOfClass(RUNTIME_CLASS(CUIScrollViewContainer)))
	{
		return NULL;
	}
	
	return (CUIScrollViewContainer*)pNode;
}

CUIScrollView*			
ConverToSV(NDNode* pNode)
{
	if (!pNode || !pNode->IsKindOfClass(RUNTIME_CLASS(CUIScrollView)))
	{
		return NULL;
	}
	
	return (CUIScrollView*)pNode;
}

CUIHyperlinkText*
ConverToHLT(NDNode* pNode)
{
	if (!pNode || !pNode->IsKindOfClass(RUNTIME_CLASS(CUIHyperlinkText)))
	{
		return NULL;
	}
	
	return (CUIHyperlinkText*)pNode;
}

CUIHyperlinkButton*
ConverToHLB(NDNode* pNode)
{
	if (!pNode || !pNode->IsKindOfClass(RUNTIME_CLASS(CUIHyperlinkButton)))
	{
		return NULL;
	}
	
	return (CUIHyperlinkButton*)pNode;
}

CUIItemButton*
ConverToItemButton(NDNode* pNode)
{
	if (!pNode || !pNode->IsKindOfClass(RUNTIME_CLASS(CUIItemButton)))
	{
		return NULL;
	}
	
	return (CUIItemButton*)pNode;
}

CUIEquipItem*							
ConverToEquipBtn(NDNode* pNode)
{
	if (!pNode || !pNode->IsKindOfClass(RUNTIME_CLASS(CUIEquipItem)))
	{
		return NULL;
	}
	
	return (CUIEquipItem*)pNode; 
}

CUICheckBox*							
ConverToCheckBox(NDNode* pNode)
{
	if (!pNode || !pNode->IsKindOfClass(RUNTIME_CLASS(CUICheckBox)))
	{
		return NULL;
	}
	
	return (CUICheckBox*)pNode;  
}

CUIRadioButton*							
ConverToRadioBtn(NDNode* pNode)
{
	if (!pNode || !pNode->IsKindOfClass(RUNTIME_CLASS(CUIRadioButton)))
	{
		return NULL;
	}
	
	return (CUIRadioButton*)pNode;
}

CUIRadioGroup*							
ConverToRadioGroup(NDNode* pNode)
{
	if (!pNode || !pNode->IsKindOfClass(RUNTIME_CLASS(CUIRadioGroup)))
	{
		return NULL;
	}
	
	return (CUIRadioGroup*)pNode;  
}

CUIEdit*							
ConverToEdit(NDNode* pNode)
{
	if (!pNode || !pNode->IsKindOfClass(RUNTIME_CLASS(CUIEdit)))
	{
		return NULL;
	}
	
	return (CUIEdit*)pNode;  
}

CUISpriteNode*							
ConverToSprite(NDNode* pNode)
{
	if (!pNode || !pNode->IsKindOfClass(RUNTIME_CLASS(CUISpriteNode)))
	{
		return NULL;
	}
	
	return (CUISpriteNode*)pNode;  
}

#pragma mark ����uiͨ�ú���

CGSize GetStringSize(const char* str, unsigned int fontsize)
{
	if (!str || 0 == strlen(str))
	{
		return CGSizeZero;
	}
	return getStringSize(str, fontsize);
}

CGSize GetHyperLinkTextSize(const char* str, unsigned int fontsize, int nBoundWidth)
{
	if (!str)
	{
		return CGSizeZero;
	}
	CGSize textSize;
	textSize.width	= nBoundWidth;
	textSize.height = NDUITextBuilder::DefaultBuilder()->StringHeightAfterFilter(str, textSize.width, fontsize);
	textSize.width	= NDUITextBuilder::DefaultBuilder()->StringWidthAfterFilter(str, textSize.width, fontsize);
	return textSize;
}

NDUINode* CreateColorLabel(const char* str, unsigned int fontsize, unsigned int nConstraitWidth)
{
	if (!str)
	{
		return NULL;
	}
	CGSize winsize	= NDDirector::DefaultDirector()->GetWinSize();
	winsize.width	= nConstraitWidth;
	return (NDUINode*)NDUITextBuilder::DefaultBuilder()->Build(str, fontsize, winsize, ccc4(255, 255, 255, 255));
}

NDScene* GetSMGameScene()
{
	return NDDirector::DefaultDirector()->GetSceneByTag(SMGAMESCENE_TAG);
}
NDScene* GetSMLoginScene()
{
	return NDDirector::DefaultDirector()->GetSceneByTag(SMLOGINSCENE_TAG);
}

CGSize GetWinSize()
{
	return NDDirector::DefaultDirector()->GetWinSize();
}

CGRect RectZero()
{
	return CGRectZero;
}

CGSize SizeZero()
{
	return CGSizeZero;
}

void ShowLoadBar()
{
	ShowProgressBar;
}

void CloseLoadBar()
{
	CloseProgressBar;
}
	
#pragma mark ui�ű���������
	
	void ScriptObjectUI::OnLoad()
	{
		ETCFUNC("GetUiNode",		GetUiNode)
		ETCFUNC("GetLabel",			GetLabel)
		ETCFUNC("GetButton",		GetButton)
		ETCFUNC("GetUiLayer",		GetUiLayer)
		ETCFUNC("GetTableLayer",	GetTableLayer)
		ETCFUNC("GetImage",			GetImage)
		ETCFUNC("GetScrollLayer",	GetScrollLayer)
		ETCFUNC("GetHScrollLayer",	GetHScrollLayer)
		ETCFUNC("GetScrollViewContainer",	GetScrollViewContainer)
		ETCFUNC("CGSizeMake",		CGSizeMake)
		ETCFUNC("CGPointMake",		CGPointMake)
		ETCFUNC("CGRectMake",		CGRectMake)
		ETCFUNC("ccc4",				ccc4);
		ETCFUNC("ccc3",				ccc3);
		ETCFUNC("ShowLoadBar",		ShowLoadBar);
		ETCFUNC("CloseLoadBar",		CloseLoadBar);
		ETCFUNC("GetSMGameScene",	GetSMGameScene);
        ETCFUNC("GetSMLoginScene",	GetSMLoginScene);
		ETCFUNC("GetWinSize",		GetWinSize);
		ETCFUNC("RectZero",			RectZero);
		ETCFUNC("SizeZero",			SizeZero);
		ETCFUNC("GetStringSize",		GetStringSize);
		ETCFUNC("GetHyperLinkTextSize", GetHyperLinkTextSize);
		ETCFUNC("GetHyperLinkText",	GetHyperLinkText);
		ETCFUNC("GetHyperLinkButton", GetHyperLinkButton);
		ETCFUNC("GetItemButton", GetItemButton);
		ETCFUNC("GetEquipButton", GetEquipButton);
		
		ETCFUNC("CreateColorLabel", CreateColorLabel);
		
		ETCFUNC("RecursiveUINode", RecursiveUINode);
		ETCFUNC("RecursiveLabel", RecursiveLabel);
		ETCFUNC("RecursiveButton", RecursiveButton);
		ETCFUNC("RecursiveUILayer", RecursiveUILayer);
		ETCFUNC("RecursiveImage", RecursiveImage);
		ETCFUNC("RecursiveScroll", RecursiveScroll);
		ETCFUNC("RecursiveScrollContainer", RecursiveScrollContainer);
		ETCFUNC("RecursiveSVC", RecursiveSVC);
		ETCFUNC("RecursiveSV", RecursiveSV);
		ETCFUNC("RecursiveHyperText", RecursiveHyperText);
		ETCFUNC("RecursiveHyperBtn", RecursiveHyperBtn);
		ETCFUNC("RecursiveItemBtn", RecursiveItemBtn);
		ETCFUNC("RecursiveEquipBtn", RecursiveEquipBtn);
		ETCFUNC("RecursiveCheckBox", RecursiveCheckBox);
		ETCFUNC("RecursiveRadioBtn", RecursiveRadioBtn);
		ETCFUNC("RecursiveRadioGroup", RecursiveRadioGroup);
		ETCFUNC("RecursivUIExp", RecursivUIExp);
		ETCFUNC("RecursivUIEdit", RecursivUIEdit);
		ETCFUNC("RecursivUISprite", RecursivUISprite);
		
		ETCFUNC("PRecursiveUINode", PRecursiveUINode);
		ETCFUNC("PRecursiveLabel", PRecursiveLabel);
		ETCFUNC("PRecursiveButton", PRecursiveButton);
		ETCFUNC("PRecursiveUILayer", PRecursiveUILayer);
		ETCFUNC("PRecursiveImage", PRecursiveImage);
		ETCFUNC("PRecursiveSVC", PRecursiveSVC);
		ETCFUNC("PRecursiveSV", PRecursiveSV);
		ETCFUNC("PRecursiveHyperText", PRecursiveHyperText);
		ETCFUNC("PRecursiveHyperBtn", PRecursiveHyperBtn);
		ETCFUNC("PRecursiveItemBtn", PRecursiveItemBtn);
		ETCFUNC("PRecursiveEquipBtn", PRecursiveEquipBtn);
		
		ETCFUNC("PRecursiveCheckBox", PRecursiveCheckBox);
		ETCFUNC("PRecursiveRadioBtn", PRecursiveRadioBtn);
		ETCFUNC("PRecursiveRadioGroup", PRecursiveRadioGroup);
		ETCFUNC("PRecursiveUIExp", PRecursiveUIExp);
		ETCFUNC("PRecursiveUIEdit", PRecursiveUIEdit);
		ETCFUNC("PRecursiveUISprite", PRecursiveUISprite);
		
		ETCFUNC("ConverToUiNode", ConverToUiNode);
		ETCFUNC("ConverToLabel", ConverToLabel);
		ETCFUNC("ConverToButton", ConverToButton);
		ETCFUNC("ConverToUiLayer", ConverToUiLayer);
		ETCFUNC("ConverToTableLayer", ConverToTableLayer);
		ETCFUNC("ConverToImage", ConverToImage);
		ETCFUNC("ConverToSVC", ConverToSVC);
		ETCFUNC("ConverToSV", ConverToSV);
		ETCFUNC("ConverToHLT", ConverToHLT);
		ETCFUNC("ConverToHLB", ConverToHLB);
		ETCFUNC("ConverToItemButton", ConverToItemButton);
		ETCFUNC("ConverToEquipBtn", ConverToEquipBtn);
		
		ETCFUNC("ConverToCheckBox", ConverToCheckBox);
		ETCFUNC("ConverToRadioBtn", ConverToRadioBtn);
		ETCFUNC("ConverToRadioGroup", ConverToRadioGroup);
        ETCFUNC("ConverToEdit", ConverToEdit);
		ETCFUNC("ConverToSprite", ConverToSprite);
	}
	
#pragma mark ͼƬ��������
	// ��ɫ�ṹ����
	ETSTRUCTBEGIN(cocos2d::ccColor4B)
	ETSTRUCTPROP("r",								&cocos2d::ccColor4B::r)
	ETSTRUCTPROP("g",								&cocos2d::ccColor4B::g)
	ETSTRUCTPROP("b",								&cocos2d::ccColor4B::b)
	ETSTRUCTPROP("a",								&cocos2d::ccColor4B::a)
	ETSTRUCTEND(cocos2d::ccColor4B)
	
	ETSTRUCTBEGIN(cocos2d::cocos2d::ccColor3B)
	ETSTRUCTPROP("r",								&cocos2d::cocos2d::ccColor3B::r)
	ETSTRUCTPROP("g",								&cocos2d::cocos2d::ccColor3B::g)
	ETSTRUCTPROP("b",								&cocos2d::cocos2d::ccColor3B::b)
	ETSTRUCTEND(cocos2d::cocos2d::ccColor3B)
	
	// ͼƬ�ص���
	ETCLASSBEGIN(NDPicturePool)
	ETCFUNC("DefaultPicPool",						&NDPicturePool::DefaultPool)
	ETMEMBERFUNC("AddPicture",						(NDPicture* (NDPicturePool::*)(const char*, bool))&NDPicturePool::AddPicture)
	ETMEMBERFUNC("AddPictureEx",					(NDPicture* (NDPicturePool::*)(const char*, int, int, bool))&NDPicturePool::AddPicture)
	ETCLASSEND(NDPicturePool)
	
	// ͼƬ�ർ��
	ETCLASSBEGIN(NDPicture)
	//ETCONSTRUCTARG1("createNDPicture", bool)
	//ETDESTRUCT("Free")
	//ETMEMBERFUNC("Initialization",					(void (NDPicture::*)(const char*))&NDPicture::Initialization)
	//ETMEMBERFUNC("InitializationEx",				(void (NDPicture::*)(const char*, int, int))&NDPicture::Initialization)
	ETMEMBERFUNC("Cut",								&NDPicture::Cut)
	ETMEMBERFUNC("SetReverse",						&NDPicture::SetReverse)
	ETMEMBERFUNC("Rotation",						&NDPicture::Rotation)
	ETMEMBERFUNC("SetColor",						&NDPicture::SetColor)
	ETMEMBERFUNC("GetSize",							&NDPicture::GetSize)
	ETMEMBERFUNC("SetGrayState",					&NDPicture::SetGrayState)
	ETMEMBERFUNC("IsGrayState",						&NDPicture::IsGrayState)
	ETCLASSEND(NDPicture)
	
#pragma mark UI����
	
#pragma mark UI�ṹ����
	// ��С�ṹ����
	ETSTRUCTBEGIN(CGSize)
	ETSTRUCTPROP("w",								&CGSize::width)
	ETSTRUCTPROP("h",								&CGSize::height)
	ETSTRUCTEND(CGSize)
	
	// ��ṹ����
	ETSTRUCTBEGIN(CGPoint)
	ETSTRUCTPROP("x",								&CGPoint::x)
	ETSTRUCTPROP("y",								&CGPoint::y)
	ETSTRUCTEND(CGPoint)
	
	// ��Χ�ṹ����
	ETSTRUCTBEGIN(CGRect)
	ETSTRUCTPROP("origin",							&CGRect::origin)
	ETSTRUCTPROP("size",							&CGRect::size)
	ETSTRUCTEND(CGRect)
	
#pragma mark UI���ص���
	ETCLASSBEGIN(NDUILoad)
	ETCONSTRUCT("createNDUILoad")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("Load",							(bool (NDUILoad::*)(const char*, NDUINode *, LuaObject, CGFloat, CGFloat))&NDUILoad::LoadLua)
	ETCLASSEND(NDUILoad)
	
#pragma mark �ڵ��ർ��"NDNode.h"
	// �ڵ��ർ��
	ETCLASSBEGIN(NDNode)
	ETCONSTRUCT("createNDNode")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("Init",							&NDNode::Initialization)
	ETMEMBERFUNC("GetParent",						&NDNode::GetParent)
	ETMEMBERFUNC("RemoveAllChildren",				&NDNode::RemoveAllChildren)
	ETMEMBERFUNC("GetzOrder",						&NDNode::GetzOrder)
	ETMEMBERFUNC("GetTag",							&NDNode::GetTag)
	ETMEMBERFUNC("SetTag",							&NDNode::SetTag)
	ETMEMBERFUNC("AddChild",						(void (NDNode::*)(NDNode*))&NDNode::AddChild)
	ETMEMBERFUNC("AddChildZ",						(void (NDNode::*)(NDNode*, int))&NDNode::AddChild)
	ETMEMBERFUNC("AddChildZTag",					(void (NDNode::*)(NDNode*, int, int))&NDNode::AddChild)
	ETMEMBERFUNC("RemoveChild",						(void (NDNode::*)(NDNode*, bool))&NDNode::RemoveChild)
	ETMEMBERFUNC("RemoveChildByTag",				(void (NDNode::*)(int, bool))&NDNode::RemoveChild)
	ETMEMBERFUNC("ContainChild",					&NDNode::ContainChild)
	ETMEMBERFUNC("RemoveFromParent",				&NDNode::RemoveFromParent)
	ETMEMBERFUNC("GetChild",						&NDNode::GetChild)
	ETMEMBERFUNC("IsChildOf",						&NDNode::IsChildOf)
	ETMEMBERFUNC("DrawEnabled",						&NDNode::DrawEnabled)
	ETMEMBERFUNC("SetParam1",						&NDNode::SetParam1)
	ETMEMBERFUNC("SetParam2",						&NDNode::SetParam2)
	ETMEMBERFUNC("GetParam1",						&NDNode::GetParam1)
	ETMEMBERFUNC("GetParam2",						&NDNode::GetParam2)
	ETMEMBERFUNC("SetDestroyNotify",				&NDNode::SetDestroyNotify)
	ETCLASSEND(NDNode)
	
#pragma mark �����ർ��"NDDirector.h"
	// �����ർ��
	ETCLASSBEGIN(NDDirector)
	ETCFUNC("DefaultDirector",						&NDDirector::DefaultDirector)
	ETMEMBERFUNC("PushScene",						&NDDirector::PushScene)
	ETMEMBERFUNC("PopScene",						(bool (NDDirector::*)(bool))&NDDirector::PopScene)
	ETMEMBERFUNC("GetRunningScene",					&NDDirector::GetRunningScene)
	ETMEMBERFUNC("GetWinSize",						&NDDirector::GetWinSize)
	ETMEMBERFUNC("SetViewRect",						&NDDirector::SetViewRect)
	ETMEMBERFUNC("ResumeViewRect",					&NDDirector::ResumeViewRect)
	ETMEMBERFUNC("GetSceneByTag",					&NDDirector::GetSceneByTag)
	ETMEMBERFUNC("GetScaleFactor",					&NDDirector::GetScaleFactor)
	ETCLASSEND(NDDirector)
	
#pragma mark �����ർ��"NDScene.h"
	// �����ർ��
	ETSUBCLASSBEGIN(NDScene, NDNode)
	ETCFUNC("Scene",								&NDScene::Scene)
	ETCLASSEND(NDScene)
	
#pragma mark  UI���ർ��"NDUINode.h"
	// UI���ർ��
	ETSUBCLASSBEGIN(NDUINode, NDNode)
	ETCONSTRUCT("createNDUINode")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("Init",							&NDUINode::Initialization)
	ETMEMBERFUNC("SetFrameRect",					&NDUINode::SetFrameRect)
	ETMEMBERFUNC("GetFrameRect",					&NDUINode::GetFrameRect)
	ETMEMBERFUNC("SetVisible",						&NDUINode::SetVisible)
	ETMEMBERFUNC("IsVisibled",						&NDUINode::IsVisibled)
	ETMEMBERFUNC("EnableEvent",						&NDUINode::EnableEvent)
	ETMEMBERFUNC("EventEnabled",					&NDUINode::EventEnabled)
	ETMEMBERFUNC("GetScreenRect",					&NDUINode::GetScreenRect)
	ETMEMBERFUNC("SetLuaDelegate",					&NDUINode::SetLuaDelegate)
	ETCLASSEND(NDUINode)
	
#pragma mark ���ർ��"NDUILayer.h"
	// ���ർ��
	ETSUBCLASSBEGIN(NDUILayer, NDUINode)
	ETCONSTRUCT("createNDUILayer")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("Init",							&NDUILayer::Initialization)
	ETMEMBERFUNC("SetBackgroundImage",				&NDUILayer::SetBackgroundImageLua)
	ETMEMBERFUNC("SetBackgroundFocusImage",			&NDUILayer::SetBackgroundFocusImageLua)
	ETMEMBERFUNC("SetBackgroundColor",				&NDUILayer::SetBackgroundColor)
	ETMEMBERFUNC("SetFocus",						&NDUILayer::SetFocus)
	ETMEMBERFUNC("GetFocus",						&NDUILayer::GetFocus)
	ETCLASSEND(NDUILayer)
	

#pragma mark ��ť�ർ��"NDUIButton.h"
	// ��ť�ർ��
	ETSUBCLASSBEGIN(NDUIButton, NDUINode)
	ETCONSTRUCT("createNDUIButton")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("Init",							&NDUIButton::Initialization)
	ETMEMBERFUNC("SetImage",						&NDUIButton::SetImageLua)
	ETMEMBERFUNC("SetImageEx",						&NDUIButton::SetImageCustom)
	ETMEMBERFUNC("SetTouchDownImage",				&NDUIButton::SetTouchDownImageLua)
	ETMEMBERFUNC("SetTouchDownImageEx",				&NDUIButton::SetTouchDownImageCustom)
	ETMEMBERFUNC("SetTouchDownColor",				&NDUIButton::SetTouchDownColor)
	ETMEMBERFUNC("SetFocusColor",					&NDUIButton::SetFocusColor)
	ETMEMBERFUNC("SetFocusFontColor",				&NDUIButton::SetFocusFontColor)
	ETMEMBERFUNC("SetFocusImage",					&NDUIButton::SetFocusImageLua)
	ETMEMBERFUNC("SetFocusImageEx",					&NDUIButton::SetFocusImageCustom)
	ETMEMBERFUNC("OpenFrame",						&NDUIButton::OpenFrame)
	ETMEMBERFUNC("CloseFrame",						&NDUIButton::CloseFrame)
	ETMEMBERFUNC("SetTitle",						&NDUIButton::SetTitleLua)
	ETMEMBERFUNC("GetTitle",						&NDUIButton::GetTitle)
	ETMEMBERFUNC("SetFontColor",					&NDUIButton::SetFontColor)
	ETMEMBERFUNC("GetFontColor",					&NDUIButton::GetFontColor)
	ETMEMBERFUNC("SetBackgroundPicture",			&NDUIButton::SetBackgroundPictureLua)
	ETMEMBERFUNC("SetBackgroundPictureEx",			&NDUIButton::SetBackgroundPictureCustom)
	ETMEMBERFUNC("SetFontSize",						&NDUIButton::SetFontSize)
	ETMEMBERFUNC("GetFontSize",						&NDUIButton::GetFontSize)
	ETMEMBERFUNC("SetArrow",						&NDUIButton::SetArrow)
	ETMEMBERFUNC("EnalbeGray",						&NDUIButton::EnalbeGray)
	ETMEMBERFUNC("IsGray",							&NDUIButton::IsGray)
	ETMEMBERFUNC("EnalbelBackgroundGray",			&NDUIButton::EnalbelBackgroundGray)
	ETMEMBERFUNC("IsBackgroundGray",				&NDUIButton::IsBackgroundGray)
	ETMEMBERFUNC("SetNormalImageColor",				&NDUIButton::SetNormalImageColor)
	ETMEMBERFUNC("GetImage",						&NDUIButton::GetImage)
	ETMEMBERFUNC("GetImageCopy",					&NDUIButton::GetImageCopy)
	ETCLASSEND(NDUIButton)
	
#pragma mark ͼƬ�ർ��"NDUIImage.h"
	ETSUBCLASSBEGIN(NDUIImage, NDUINode)
	ETCONSTRUCT("createNDUIImage")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("SetPicture",						&NDUIImage::SetPictureLua)
	ETCLASSEND(NDUIImage)
	
#pragma mark ��ǩ�ർ����NDUILabel��
	ETSUBCLASSBEGIN(NDUILabel, NDUINode)
	ETCONSTRUCT("createNDUILabel")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("SetText",							&NDUILabel::SetText)
	ETMEMBERFUNC("GetText",							&NDUILabel::GetText)
	ETMEMBERFUNC("SetFontColor",					&NDUILabel::SetFontColor)
	ETMEMBERFUNC("GetFontColor",					&NDUILabel::GetFontColor)
	ETMEMBERFUNC("SetFontSize",						&NDUILabel::SetFontSize)
	ETMEMBERFUNC("GetFontSize",						&NDUILabel::GetFontSize)
	ETMEMBERFUNC("SetTextAlignment",				&NDUILabel::SetTextAlignment)
	ETMEMBERFUNC("GetTextAlignment",				&NDUILabel::GetTextAlignment)
	ETMEMBERFUNC("SetRenderTimes",					&NDUILabel::SetRenderTimes)
	ETMEMBERFUNC("GetRenderTimes",					&NDUILabel::GetRenderTimes)
	ETMEMBERFUNC("SetFontBoderColer",				&NDUILabel::SetFontBoderColer)
	ETCLASSEND(NDUILabel)
	
#pragma mark ��ǩ�ർ��"NDUIDialog.h"
	ETSUBCLASSBEGIN(NDUIDialog, NDUILayer)
	ETCONSTRUCT("createNDUIDialog")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("Show0",							&NDUIDialog::Show0)
	ETMEMBERFUNC("Show1",							&NDUIDialog::Show1)
	ETMEMBERFUNC("Show2",							&NDUIDialog::Show2)
	ETMEMBERFUNC("Show3",							&NDUIDialog::Show3)
	ETMEMBERFUNC("Show4",							&NDUIDialog::Show4)
	ETMEMBERFUNC("Show5",							&NDUIDialog::Show5)
	ETMEMBERFUNC("Show6",							&NDUIDialog::Show6)
	ETMEMBERFUNC("Show7",							&NDUIDialog::Show7)
	ETMEMBERFUNC("Show8",							&NDUIDialog::Show8)
	ETMEMBERFUNC("Show9",							&NDUIDialog::Show9)
	ETMEMBERFUNC("Show10",							&NDUIDialog::Show10)
	ETCLASSEND(NDUIDialog)
	
#pragma mark �����ı�����ർ����UIChatText.h��
	ETSUBCLASSBEGIN(CUIChatText,NDUINode)
	ETCONSTRUCT("createUIChatText")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("Init",							&CUIChatText::Initialization)
	ETMEMBERFUNC("SetContentWidth",					&CUIChatText::SetContentWidth)	
	ETMEMBERFUNC("SetContent",						&CUIChatText::SetContent)
	ETMEMBERFUNC("GetContentHeight",				&CUIChatText::GetContentHeight)	
	ETCLASSEND(CUIChatText)
	
#pragma mark ���ƶ��㵼��"UIMovableLayer.h"
	ETSUBCLASSBEGIN(CUIMovableLayer, NDUILayer)
	ETCONSTRUCT("createUIMovableLayer")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("Init",							&CUIMovableLayer::Initialization)
	ETMEMBERFUNC("SetMovableViewer",				&CUIMovableLayer::SetMovableViewer)
	ETCLASSEND(CUIMovableLayer)
	
#pragma mark �����ർ��"UIScroll.h"
	ETSUBCLASSBEGIN(CUIScroll, CUIMovableLayer)
	ETCONSTRUCT("createUIScroll")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("Init",							&CUIScroll::Initialization)
	ETMEMBERFUNC("SetScrollStyle",					&CUIScroll::SetScrollStyle)
	ETMEMBERFUNC("IsCanAccerate",					&CUIScroll::IsCanAccerate)
	ETMEMBERFUNC("IsInAccceratState",				&CUIScroll::IsInAccceratState)
	ETMEMBERFUNC("StopAccerate",					&CUIScroll::StopAccerate)
	ETMEMBERFUNC("GetScrollStyle",					&CUIScroll::GetScrollStyle)
	ETMEMBERFUNC("SetContainer",					&CUIScroll::SetContainer)
	ETCLASSEND(CUIScroll)
	
#pragma mark ������������"UIScrollContainer.h"
	ETSUBCLASSBEGIN(CUIScrollContainer, NDUILayer)
	ETCONSTRUCT("createUIScrollContainer")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("Init",							&CUIScrollContainer::Initialization)
	ETMEMBERFUNC("SetLeftReserveDistance",			&CUIScrollContainer::SetLeftReserveDistance)
	ETMEMBERFUNC("SetRightReserveDistance",			&CUIScrollContainer::SetRightReserveDistance)
	ETMEMBERFUNC("SetTopReserveDistance",			&CUIScrollContainer::SetTopReserveDistance)
	ETMEMBERFUNC("SetBottomReserveDistance",		&CUIScrollContainer::SetBottomReserveDistance)
	ETCLASSEND(CUIScrollContainer)
	
#pragma mark ������ͼ����"UIScrollView.h"
	ETSUBCLASSBEGIN(CUIScrollView, CUIScroll)
	ETCONSTRUCT("createUIScrollView")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("Init",							&CUIScrollView::Initialization)
	ETMEMBERFUNC("SetScrollViewer",					&CUIScrollView::SetScrollViewer)
	ETMEMBERFUNC("SetViewId",						&CUIScrollView::SetViewId)
	ETMEMBERFUNC("GetViewId",						&CUIScrollView::GetViewId)
	ETCLASSEND(CUIScrollView)
	
	ETSUBCLASSBEGIN(CUIScrollViewContainer, CUIScrollContainer)
	ETCONSTRUCT("createUIScrollViewContainer")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("Init",						&CUIScrollViewContainer::Initialization)
	ETMEMBERFUNC("SetStyle",					&CUIScrollViewContainer::SetStyle)
	ETMEMBERFUNC("GetScrollStyle",				&CUIScrollViewContainer::GetScrollStyle)
	ETMEMBERFUNC("SetCenterAdjust",				&CUIScrollViewContainer::SetCenterAdjust)
	ETMEMBERFUNC("IsCenterAdjust",				&CUIScrollViewContainer::IsCenterAdjust)
	ETMEMBERFUNC("GetViewCount",				&CUIScrollViewContainer::GetViewCount)
	ETMEMBERFUNC("SetViewSize",				&CUIScrollViewContainer::SetViewSize)
	ETMEMBERFUNC("GetViewSize",				&CUIScrollViewContainer::GetViewSize)
	ETMEMBERFUNC("AddView",						&CUIScrollViewContainer::AddView)
	ETMEMBERFUNC("RemoveView",					&CUIScrollViewContainer::RemoveView)
	ETMEMBERFUNC("RemoveViewById",				&CUIScrollViewContainer::RemoveViewById)
	ETMEMBERFUNC("RemoveAllView",				&CUIScrollViewContainer::RemoveAllView)
	ETMEMBERFUNC("ShowViewByIndex",				&CUIScrollViewContainer::ShowViewByIndex)
	ETMEMBERFUNC("ShowViewById",				&CUIScrollViewContainer::ShowViewById)
	ETMEMBERFUNC("ScrollViewByIndex",			&CUIScrollViewContainer::ScrollViewByIndex)
	ETMEMBERFUNC("ScrollViewById",				&CUIScrollViewContainer::ScrollViewById)
	ETMEMBERFUNC("GetView",						&CUIScrollViewContainer::GetView)
	ETMEMBERFUNC("GetViewById",					&CUIScrollViewContainer::GetViewById)
	ETMEMBERFUNC("GetBeginView",				&CUIScrollViewContainer::GetBeginView)
	ETMEMBERFUNC("GetBeginIndex",				&CUIScrollViewContainer::GetBeginIndex)
	ETMEMBERFUNC("EnableScrollBar",				&CUIScrollViewContainer::EnableScrollBar)
	ETCLASSEND(CUIScrollViewContainer)
	
#pragma mark ��ɫUI�ڵ㵼��"UIRoleNode.h"
	ETSUBCLASSBEGIN(CUIRoleNode, NDUINode)
	ETCONSTRUCT("createUIRoleNode")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("Init",						&CUIRoleNode::Initialization)
	ETMEMBERFUNC("ChangeLookFace",				&CUIRoleNode::ChangeLookFace)
	ETMEMBERFUNC("SetEquip",					&CUIRoleNode::SetEquip)
	ETCLASSEND(CUIRoleNode)
	
#pragma mark �������ı�����"UIHyperLink.h"
	ETSUBCLASSBEGIN(CUIHyperlinkText, NDUINode)
	ETCONSTRUCT("createUIHyperLink")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("Init",							&CUIHyperlinkText::Initialization)
	ETMEMBERFUNC("SetLinkBoundRect",				&CUIHyperlinkText::SetLinkBoundRect)
	ETMEMBERFUNC("SetLinkText",						&CUIHyperlinkText::SetLinkText)
	ETMEMBERFUNC("SetLinkTextFontSize",				&CUIHyperlinkText::SetLinkTextFontSize)
	ETMEMBERFUNC("SetLinkTextColor",				&CUIHyperlinkText::SetLinkTextColor)
	ETMEMBERFUNC("EnableLine",						&CUIHyperlinkText::EnableLine)
	ETCLASSEND(CUIHyperlinkText)
	
#pragma mark �����Ӱ�ť����"UIHyperLink.h"
	ETSUBCLASSBEGIN(CUIHyperlinkButton, NDUIButton)
	ETCONSTRUCT("createUIHyperlinkButton")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("Init",							&CUIHyperlinkButton::Initialization)
	ETMEMBERFUNC("SetLinkBoundRect",				&CUIHyperlinkButton::SetLinkBoundRect)
	ETMEMBERFUNC("SetLinkText",						&CUIHyperlinkButton::SetLinkText)
	ETMEMBERFUNC("SetLinkTextFontSize",				&CUIHyperlinkButton::SetLinkTextFontSize)
	ETMEMBERFUNC("SetLinkTextColor",				&CUIHyperlinkButton::SetLinkTextColor)
	ETMEMBERFUNC("EnableLine",						&CUIHyperlinkButton::EnableLine)
	ETCLASSEND(CUIHyperlinkButton)
	
#pragma mark ��Ʒ��ť "UIItemButton.h"
	ETSUBCLASSBEGIN(CUIItemButton, NDUIButton)
	ETCONSTRUCT("createUIItemButton")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("ChangeItem",						&CUIItemButton::ChangeItem)
	ETMEMBERFUNC("GetItemId",						&CUIItemButton::GetItemId)
	ETMEMBERFUNC("ChangeItemType",					&CUIItemButton::ChangeItemType)
	ETMEMBERFUNC("GetItemType",						&CUIItemButton::GetItemType)
	ETMEMBERFUNC("RefreshItemCount",				&CUIItemButton::RefreshItemCount)
	ETMEMBERFUNC("SetLock",							&CUIItemButton::SetLock)
	ETMEMBERFUNC("IsLock",							&CUIItemButton::IsLock)
	ETMEMBERFUNC("SetShowAdapt",					&CUIItemButton::SetShowAdapt)
	ETCLASSEND(CUIItemButton)
	
	ETSUBCLASSBEGIN(CUIEquipItem, CUIItemButton)
	ETCONSTRUCT("createUIEquipItem")
	ETDESTRUCT("Free")
	ETCLASSEND(CUIEquipItem)
	
#pragma mark Check��ť "UICheckBox.h"
	ETSUBCLASSBEGIN(CUICheckBox, NDUINode)
	ETCONSTRUCT("createUICheckBox")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("Init",							(void (CUICheckBox::*)(const char*, const char*))&CUICheckBox::Initialization)
	ETMEMBERFUNC("SetSelect",						&CUICheckBox::SetSelect)
	ETMEMBERFUNC("IsSelect",						&CUICheckBox::IsSelect)
	ETMEMBERFUNC("SetText",							&CUICheckBox::SetText)
	ETMEMBERFUNC("GetText",							&CUICheckBox::GetText)
	ETMEMBERFUNC("SetTextFontColor",				&CUICheckBox::SetTextFontColor)
	ETMEMBERFUNC("SetTextFontSize",					&CUICheckBox::SetTextFontSize)
	ETCLASSEND(CUICheckBox)
	
#pragma mark ��ѡ��ť����ѡ��(���ʹ��)
	ETSUBCLASSBEGIN(CUIRadioButton, CUICheckBox)
	ETCONSTRUCT("createUIRadioButton")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("Init",							(void (CUIRadioButton::*)(const char*, const char*))&CUIRadioButton::Initialization)
	ETCLASSEND(CUIRadioButton)
	
	ETSUBCLASSBEGIN(CUIRadioGroup, NDUINode)
	ETCONSTRUCT("createUIRadioGroup")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("AddRadio",						&CUIRadioGroup::AddRadio)
	ETMEMBERFUNC("ContainRadio",					&CUIRadioGroup::ContainRadio)
	ETMEMBERFUNC("SetIndexSelected",				&CUIRadioGroup::SetIndexSelected)
	ETMEMBERFUNC("SetRadioSelected",				&CUIRadioGroup::SetRadioSelected)
	ETMEMBERFUNC("GetSelectedRadio",				&CUIRadioGroup::GetSelectedRadio)
	ETMEMBERFUNC("GetSelectedIndex",				&CUIRadioGroup::GetSelectedIndex)
	ETCLASSEND(CUIRadioGroup)
	
#pragma mark ������ "UIExp.h"
	ETSUBCLASSBEGIN(CUIExp, NDUINode)
	ETCONSTRUCT("createUIExp")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("Init",							(void (CUIExp::*)(const char*, const char*))&CUIExp::Initialization)
	ETMEMBERFUNC("SetProcess",						&CUIExp::SetProcess)
	ETMEMBERFUNC("SetTotal",						&CUIExp::SetTotal)
    ETMEMBERFUNC("SetStyle",                        &CUIExp::SetStyle)
	ETMEMBERFUNC("GetProcess",						&CUIExp::GetProcess)
	ETMEMBERFUNC("GetTotal",						&CUIExp::GetTotal)
	ETMEMBERFUNC("SetText",							&CUIExp::SetText)
	ETMEMBERFUNC("SetTextFontColor",				&CUIExp::SetTextFontColor)
	ETMEMBERFUNC("SetTextFontSize",					&CUIExp::SetTextFontSize)
	ETCLASSEND(CUIExp)
	
#pragma mark �����ؼ� "UISpriteNode.h"
	ETSUBCLASSBEGIN(CUISpriteNode, NDUINode)
	ETCONSTRUCT("createUISpriteNode")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("Init",							(void (CUISpriteNode::*)(const char*, const char*))&CUISpriteNode::Initialization)
	ETMEMBERFUNC("ChangeSprite",					&CUISpriteNode::ChangeSprite)
	ETMEMBERFUNC("SetAnimation",					&CUISpriteNode::SetAnimation)
	ETMEMBERFUNC("SetPlayFrameRange",				&CUISpriteNode::SetPlayFrameRange)
	ETCLASSEND(CUISpriteNode)
	
#pragma mark �༭��ؼ� "UIEdit.h"
	ETSUBCLASSBEGIN(CUIEdit, NDUINode)
	ETCONSTRUCT("createUIEdit")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("Init",							&CUIEdit::Initialization)
	ETMEMBERFUNC("SetText",							&CUIEdit::SetText)
	ETMEMBERFUNC("GetText",							&CUIEdit::GetText)
	ETMEMBERFUNC("SetPassword",						&CUIEdit::SetPassword)
	ETMEMBERFUNC("IsPassword",						&CUIEdit::IsPassword)
	ETMEMBERFUNC("SetMaxLength",					&CUIEdit::SetMaxLength)
	ETMEMBERFUNC("GetMaxLength",					&CUIEdit::GetMaxLength)
	ETMEMBERFUNC("SetMinLength",					&CUIEdit::SetMinLength)
	ETMEMBERFUNC("GetMinLength",					&CUIEdit::GetMinLength)
	ETMEMBERFUNC("SetImage",						&CUIEdit::SetImage)
	ETMEMBERFUNC("SetFocusImage",					&CUIEdit::SetFocusImage)
	ETMEMBERFUNC("EnableAdjustView",				&CUIEdit::EnableAdjustView)
	ETMEMBERFUNC("IsTextLessMinLen",				&CUIEdit::IsTextLessMinLen)
	ETMEMBERFUNC("IsTextLessMinLen",				&CUIEdit::IsTextMoreMaxLen)
	ETCLASSEND(CUIEdit)
	
	
}