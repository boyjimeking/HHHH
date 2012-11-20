
/*
 *  ScriptUI.mm
 *  DragonDrive
 *
 *  Created by jhzheng on 12-1-6.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
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
#include "NDUIScrollView.h"
#include "UIScrollViewMulHand.h"
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
#include "UIList.h"
#include "UITabLogic.h"
//...
// smys end
#include "NDPath.h"

#include "UIScrollContainerExpand.h"
#include "UIScrollViewExpand.h"
#include "CCGeometry.h"

using namespace cocos2d;
using namespace NDEngine;


int GetChildrenTagList(LuaState* state)
{
	lua_State* L	= state->GetCState();
	if (!L)
	{
		LuaStateMgrObj.GetState()->PushNil();
		return 1;
	}
	lua_newtable(L);
	if (LUA_TUSERDATA != lua_type(L, 1))
	{
		return 1;
	}
	NDNode* pNode	= (NDNode*)lua_unboxpointer(L, 1);
	if (!pNode)
	{
		return 1;
	}
	const std::vector<NDNode*>& children	= pNode->GetChildren();
	if (children.empty())
	{
		return 1;
	}
	for (unsigned int i = 0; i < children.size(); i++) 
	{
		lua_pushnumber(L, i);
		lua_pushnumber(L, children[i]->GetTag());
		lua_settable(L, -3);
	}
	return 1;
}


//#pragma mark ͨ���ڵ��ȡĳ�ڵ���ӽڵ�

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

NDUIScrollViewContainer*			
GetScrollViewContainer(NDNode* pNode, int tag)
{
	if (!pNode)
	{
		return NULL;
	}
	
	NDNode *sc = pNode->GetChild(tag);
	
	if (!sc || !sc->IsKindOfClass(RUNTIME_CLASS(NDUIScrollViewContainer)))
	{
		return NULL;
	}
	
	return (NDUIScrollViewContainer*)sc;
}


CUIScrollViewContainerM*			
GetScrollViewContainerM(NDNode* pNode, int tag)
{
	if (!pNode)
	{
		return NULL;
	}
	
	NDNode *sc = pNode->GetChild(tag);
	
	if (!sc || !sc->IsKindOfClass(RUNTIME_CLASS(CUIScrollViewContainerM)))
	{
		return NULL;
	}
	
	return (CUIScrollViewContainerM*)sc;
}

CUIScrollContainerExpand*			
GetScrollContainerExpand(NDNode* pNode, int tag)
{
	if (!pNode)
	{
		return NULL;
	}
	
	NDNode *sc = pNode->GetChild(tag);
	
	if (!sc || !sc->IsKindOfClass(RUNTIME_CLASS(CUIScrollContainerExpand)))
	{
		return NULL;
	}
	
	return (CUIScrollContainerExpand*)sc;
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

//#pragma mark ͨ��tag�б��ȡ�ӽ��

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
        int test = tag.GetInteger();
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
NDUIScrollContainer*
RecursiveScrollContainer(NDNode* pParentNode, LuaObject tagTable)
{
	NDNode* pResultNode = RecursiveNode(pParentNode, tagTable);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(NDUIScrollContainer)))
	{
		return NULL;
	}
	
	return (NDUIScrollContainer*)pResultNode; 
}

//ͨ��tag�б��ȡ��ͼ�����ڵ�
NDUIScrollViewContainer*							
RecursiveSVC(NDNode* pParentNode, LuaObject tagTable)
{
	NDNode* pResultNode = RecursiveNode(pParentNode, tagTable);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(NDUIScrollViewContainer)))
	{
		return NULL;
	}
	
	return (NDUIScrollViewContainer*)pResultNode; 
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


//ͨ��tag�б��ȡ��ͼ�����ڵ�
CUIScrollViewContainerM*							
RecursiveSVCM(NDNode* pParentNode, LuaObject tagTable)
{
	NDNode* pResultNode = RecursiveNode(pParentNode, tagTable);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(CUIScrollViewContainerM)))
	{
		return NULL;
	}
	
	return (CUIScrollViewContainerM*)pResultNode; 
}

//ͨ��tag�б��ȡ��ͼ�ڵ�
CUIScrollViewM*							
RecursiveSVM(NDNode* pParentNode, LuaObject tagTable)
{
	NDNode* pResultNode = RecursiveNode(pParentNode, tagTable);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(CUIScrollViewM)))
	{
		return NULL;
	}
	
	return (CUIScrollViewM*)pResultNode; 
}

//ͨ��tag�б��ȡ��ͼ�����ڵ�
CUIScrollContainerExpand*							
RecursiveSCE(NDNode* pParentNode, LuaObject tagTable)
{
	NDNode* pResultNode = RecursiveNode(pParentNode, tagTable);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(CUIScrollContainerExpand)))
	{
		return NULL;
	}
	
	return (CUIScrollContainerExpand*)pResultNode; 
}

//ͨ��tag�б��ȡ��ͼ�ڵ�
UIScrollViewExpand*							
RecursiveSCEV(NDNode* pParentNode, LuaObject tagTable)
{
	NDNode* pResultNode = RecursiveNode(pParentNode, tagTable);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(UIScrollViewExpand)))
	{
		return NULL;
	}
	
	return (UIScrollViewExpand*)pResultNode; 
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
//ͨ��tag�б��ȡ��ɫ�ڵ�
CUIRoleNode*							
RecursivUIRoleNode(NDNode* pParentNode, LuaObject tagTable)
{
	NDNode* pResultNode = RecursiveNode(pParentNode, tagTable);
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(CUIRoleNode)))
	{
		return NULL;
	}
	return (CUIRoleNode*)pResultNode; 
}
//ͨ��tag�б��ȡ�б�ڵ�

#if 0
CUIList*							
RecursivUIList(NDNode* pParentNode, LuaObject tagTable)
{
	NDNode* pResultNode = RecursiveNode(pParentNode, tagTable);
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(CUIList)))
	{
		return NULL;
	}
	return (CUIList*)pResultNode; 
}
#endif

//#pragma mark ͨ��tag�б��ȡ�����
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
NDUIScrollViewContainer*							
PRecursiveSVC(NDNode* pChildNode, unsigned int nRecuriveCount)
{
	NDNode* pResultNode = PRecursiveNode(pChildNode, nRecuriveCount);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(NDUIScrollViewContainer)))
	{
		return NULL;
	}
	
	return (NDUIScrollViewContainer*)pResultNode; 
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



//** chh 2012-06-25 **//

//ͨ��tag�б��ȡ��ͼ�����ڵ�
CUIScrollViewContainerM*							
PRecursiveSVCM(NDNode* pChildNode, unsigned int nRecuriveCount)
{
	NDNode* pResultNode = PRecursiveNode(pChildNode, nRecuriveCount);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(CUIScrollViewContainerM)))
	{
		return NULL;
	}
	
	return (CUIScrollViewContainerM*)pResultNode; 
}

//ͨ��tag�б��ȡ��ͼ�ڵ�
CUIScrollViewM*							
PRecursiveSVM(NDNode* pChildNode, unsigned int nRecuriveCount)
{
	NDNode* pResultNode = PRecursiveNode(pChildNode, nRecuriveCount);
	
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(CUIScrollViewM)))
	{
		return NULL;
	}
	
	return (CUIScrollViewM*)pResultNode; 
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
//ͨ��tag�б��ȡ����ڵ�
CUIRoleNode*	
PRecursiveUIRoleNode(NDNode* pParentNode, unsigned int nRecuriveCount)
{
	NDNode* pResultNode = PRecursiveNode(pParentNode, nRecuriveCount);
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(CUIRoleNode)))
	{
		return NULL;
	}
	return (CUIRoleNode*)pResultNode; 
}
//ͨ����˷������ȡ�б�ڵ�
#if 0
CUIList*	
PRecursiveUIList(NDNode* pParentNode, unsigned int nRecuriveCount)
{
	NDNode* pResultNode = PRecursiveNode(pParentNode, nRecuriveCount);
	if (!pResultNode || 
		!pResultNode->IsKindOfClass(RUNTIME_CLASS(CUIList)))
	{
		return NULL;
	}
	return (CUIList*)pResultNode; 
}
#endif
//#pragma mark �ڵ�����ת��
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

NDUIScrollViewContainer*			
ConverToSVC(NDNode* pNode)
{
	if (!pNode || !pNode->IsKindOfClass(RUNTIME_CLASS(NDUIScrollViewContainer)))
	{
		return NULL;
	}
	
	return (NDUIScrollViewContainer*)pNode;
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


CUIScrollViewContainerM*			
ConverToSVCM(NDNode* pNode)
{
	if (!pNode || !pNode->IsKindOfClass(RUNTIME_CLASS(CUIScrollViewContainerM)))
	{
		return NULL;
	}
	
	return (CUIScrollViewContainerM*)pNode;
}

CUIScrollViewM*			
ConverToSVM(NDNode* pNode)
{
	if (!pNode || !pNode->IsKindOfClass(RUNTIME_CLASS(CUIScrollViewM)))
	{
		return NULL;
	}
	
	return (CUIScrollViewM*)pNode;
}



CUIScrollContainerExpand*			
ConverToSCE(NDNode* pNode)
{
	if (!pNode || !pNode->IsKindOfClass(RUNTIME_CLASS(CUIScrollContainerExpand)))
	{
		return NULL;
	}
	
	return (CUIScrollContainerExpand*)pNode;
}

UIScrollViewExpand*			
ConverToSCEV(NDNode* pNode)
{
	if (!pNode || !pNode->IsKindOfClass(RUNTIME_CLASS(UIScrollViewExpand)))
	{
		return NULL;
	}
	
	return (UIScrollViewExpand*)pNode;
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

CUIRoleNode*
ConverToRoleNode(NDNode* pNode)
{
	if (!pNode || !pNode->IsKindOfClass(RUNTIME_CLASS(CUIRoleNode)))
	{
		return NULL;
	}
	return (CUIRoleNode*)pNode;
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
	
	CUIEdit* pEdit = (CUIEdit*)pNode;  
    if(pEdit && pEdit == CUIEdit::sharedCurEdit())
        pEdit->AutoInputReturn();
    return pEdit;
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
#if 0
CUIList*							
ConverToUIList(NDNode* pNode)
{
	if (!pNode || !pNode->IsKindOfClass(RUNTIME_CLASS(CUIList)))
	{
		return NULL;
	}
	return (CUIList*)pNode;  
}
CUIListSection*							
ConverToUIListSection(NDNode* pNode)
{
	if (!pNode || !pNode->IsKindOfClass(RUNTIME_CLASS(CUIListSection)))
	{
		return NULL;
	}
	return (CUIListSection*)pNode;  
}
CUIListCell*							
ConverToUIListCell(NDNode* pNode)
{
	if (!pNode || !pNode->IsKindOfClass(RUNTIME_CLASS(CUIListCell)))
	{
		return NULL;
	}
	return (CUIListCell*)pNode;  
}
#endif
//#pragma mark ����uiͨ�ú���

CCSize GetStringSize(const char* str, unsigned int fontsize)
{
	if (!str || 0 == strlen(str))
	{
		return CCSizeZero;
	}
	return getStringSize(str, fontsize);
}
CCSize GetMutiLineStringSize(const char* str, int fontsize, int nWidth)
{
	if (!str || 0 == strlen(str))
	{
		return CCSizeZero;
	}
	CCSize winsize	= NDDirector::DefaultDirector()->GetWinSize();
	return getStringSizeMutiLine(str, fontsize, CCSizeMake(nWidth, winsize.height * 5));
}

CCSize GetHyperLinkTextSize(const char* str, unsigned int fontsize, int nBoundWidth)
{
	if (!str)
	{
		return CCSizeZero;
	}
	CCSize textSize;
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
	CCSize winsize	= NDDirector::DefaultDirector()->GetWinSize();
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

CCSize GetWinSize()
{
	return NDDirector::DefaultDirector()->GetWinSize();
}

CCRect RectZero()
{
	return CCRectZero;
}

CCSize SizeZero()
{
	return CCSizeZero;
}

CCPoint PointZero()
{
	return CCPointZero;
}
#if 0
LIST_ATTR_SEC MakeListSecAttr(
	int nSectionWidth, int nSectionHeight,
	int nLInner, int nRInner, int nTInner, int nBInner,
	int nCellInner, int nCellWidth, int nCellHeight)
{
	LIST_ATTR_SEC attr;
	attr.unSectionWidth			= nSectionWidth;
	attr.unSectionHeight		= nSectionHeight;
	attr.unContentLInner		= nLInner;
	attr.unContentRInner		= nRInner;
	attr.unContentTInner		= nTInner;
	attr.unContentBInner		= nBInner;
	attr.unCellInner			= nCellInner;
	attr.unCellWidth			= nCellWidth;
	attr.unCellHeight			= nCellHeight;
	return attr;
}
#endif
void ShowLoadBar()
{
	ShowProgressBar;
}

void CloseLoadBar()
{
	CloseProgressBar;
}

//++ Guosen 2012.6.3 //��ø���·��
std::string NDPath_GetResourcePath()	{ return NDPath::GetResPath(); }
std::string NDPath_GetImagePath()		{ return NDPath::GetImagePath(); }
std::string NDPath_GetMapPath()			{ return NDPath::GetMapPath(); }
std::string NDPath_GetAnimationPath()	{ return NDPath::GetAnimationPath(); }
std::string NDPath_GetResPath()			{ return NDPath::GetResPath(); }
std::string NDPath_GetSoundPath()		{ return NDPath::GetSoundPath(); }
//tanwt 20120913 //ע�᱾��֪ͨ
/*//�¹�����ʱ������
int RegisterLocalNotification(string noticeTime,string Content)
{
    //ָ����yyyymmddhh24miss����Ϊtime_t��ʱ��
    struct tm tm1;
    time_t time1;
    char timeFormart[20] = {0};
    //sprintf(notifyTime, "%ld",noticeTime);
    sscanf(noticeTime.c_str(),"%4d%2d%2d%2d%2d%2d",&tm1.tm_year,&tm1.tm_mon,&tm1.tm_mday,&tm1.tm_hour,&tm1.tm_min,&tm1.tm_sec);
    sprintf(timeFormart,"%4d-%02d-%02d %02d:%02d:%02d",tm1.tm_year,tm1.tm_mon,tm1.tm_mday,tm1.tm_hour,tm1.tm_min,tm1.tm_sec);
    
    //ע�����еı���֪ͨ
    [[UIApplication sharedApplication] cancelAllLocalNotifications];
    //���屾��֪ͨ
    UILocalNotification *notification=[[UILocalNotification alloc] init]; 

    //strftime(Noticetime, sizeof(Noticetime), "%F %T",localtime(&notifytime));
    if (notification!=nil) 
    { 
        NSString *noticeDate = [NSString stringWithFormat:@"%s", timeFormart];
        NSString *noticeContent = [NSString stringWithUTF8String:Content.c_str()];
        NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
        //NSString *noticeDate = @"2012-09-14 00:02:30";
        formatter.dateFormat = @"yyyy-MM-dd HH:mm:ss";
        //���ô���ʱ��
        notification.fireDate = [formatter dateFromString:noticeDate];
        [formatter release];
        //NSDate *now=[NSDate new]; 
        //notification.fireDate=[now addTimeInterval:10]; 
        notification.timeZone=[NSTimeZone defaultTimeZone]; 
        notification.applicationIconBadgeNumber = 1;
        //notification.alertBody=@"��������ʽ���������һ������"; 
        notification.alertBody=noticeContent; 
        [[UIApplication sharedApplication]   scheduleLocalNotification:notification];
    }
}
 */
int RegisterLocalNotification(string Date,string Content)
{
#if 0
    //���屾��֪ͨ
    UILocalNotification *notification=[[UILocalNotification alloc] init]; 
    if (notification!=nil) 
    { 
        NSString *noticeDate = [NSString stringWithFormat:@"%s", Date.c_str()];
        NSString *noticeContent = [NSString stringWithUTF8String:Content.c_str()];
        NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
        //NSString *noticeDate = @"2012-09-14 00:02:30";
        formatter.dateFormat = @"yyyy-MM-dd HH:mm:ss";
        //���ô���ʱ��
        notification.fireDate = [formatter dateFromString:noticeDate];
        [formatter release];
        //NSDate *now=[NSDate new]; 
        //notification.fireDate=[now addTimeInterval:10]; 
        notification.timeZone=[NSTimeZone defaultTimeZone]; 
        notification.applicationIconBadgeNumber = 1;
        //notification.alertBody=@"��������ʽ���������һ������"; 
        notification.alertBody=noticeContent; 
        [[UIApplication sharedApplication]   scheduleLocalNotification:notification];
    }
#endif 
	return 0;
}

CCRect RectMake(float x, float y, float width, float height)
{
	return CCRectMake((x), (y), (width), (height));
}

CCPoint PointMake(float x, float y)
{
	return CCPointMake((x), (y));
}

CCSize SizeMake(float width, float height)
{
	return CCSizeMake((width), (height));
}

namespace NDEngine {



//#pragma mark ui�ű���������
	void ScriptUiLoad()
	{
		ETLUAFUNC("GetChildrenTagList", GetChildrenTagList)
		ETCFUNC("GetUiNode",		GetUiNode)
		ETCFUNC("GetLabel",			GetLabel)
		ETCFUNC("GetButton",		GetButton)
		ETCFUNC("GetUiLayer",		GetUiLayer)
		ETCFUNC("GetTableLayer",	GetTableLayer)
		ETCFUNC("GetImage",			GetImage)
		ETCFUNC("GetScrollLayer",	GetScrollLayer)
		ETCFUNC("GetHScrollLayer",	GetHScrollLayer)
		ETCFUNC("GetScrollViewContainer",	GetScrollViewContainer)
		ETCFUNC("GetScrollViewContainerM",	GetScrollViewContainerM)
		ETCFUNC("GetScrollContainerExpand",	GetScrollContainerExpand)

		ETCFUNC("CGSizeMake",		SizeMake)
		ETCFUNC("CGPointMake",		PointMake)
		ETCFUNC("CGRectMake",		RectMake)
		ETCFUNC("ccc4",				ccc4);
		ETCFUNC("ccc3",				ccc3);
		ETCFUNC("ShowLoadBar",		ShowLoadBar);
		ETCFUNC("CloseLoadBar",		CloseLoadBar);
		ETCFUNC("GetSMGameScene",	GetSMGameScene);
		ETCFUNC("GetSMLoginScene",	GetSMLoginScene);
		ETCFUNC("GetWinSize",		GetWinSize);
		ETCFUNC("RectZero",			RectZero);
		ETCFUNC("SizeZero",			SizeZero);
		ETCFUNC("PointZero",		PointZero);
		//ETCFUNC("MakeListSecAttr",	MakeListSecAttr);
		ETCFUNC("GetStringSize",		GetStringSize);
		ETCFUNC("GetMutiLineStringSize", GetMutiLineStringSize);
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

		//** chh 2012-06-25 **//
		ETCFUNC("RecursiveSVCM", RecursiveSVCM);
		ETCFUNC("RecursiveSVM", RecursiveSVM);



		//** chh 2012-07-24 **//
		ETCFUNC("RecursiveSCE", RecursiveSCE);
		ETCFUNC("RecursiveSCEV", RecursiveSCEV);





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
		ETCFUNC("RecursivUIRoleNode", RecursivUIRoleNode);

		ETCFUNC("PRecursiveUINode", PRecursiveUINode);
		ETCFUNC("PRecursiveLabel", PRecursiveLabel);
		ETCFUNC("PRecursiveButton", PRecursiveButton);
		ETCFUNC("PRecursiveUILayer", PRecursiveUILayer);
		ETCFUNC("PRecursiveImage", PRecursiveImage);
		ETCFUNC("PRecursiveSVC", PRecursiveSVC);
		ETCFUNC("PRecursiveSV", PRecursiveSV);

		//** chh 2012-06-25 **//
		ETCFUNC("PRecursiveSVCM", PRecursiveSVCM);
		ETCFUNC("PRecursiveSVM", PRecursiveSVM);

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
		ETCFUNC("PRecursiveUIRoleNode", PRecursiveUIRoleNode);

		ETCFUNC("ConverToUiNode", ConverToUiNode);
		ETCFUNC("ConverToLabel", ConverToLabel);
		ETCFUNC("ConverToButton", ConverToButton);
		ETCFUNC("ConverToUiLayer", ConverToUiLayer);
		ETCFUNC("ConverToTableLayer", ConverToTableLayer);
		ETCFUNC("ConverToImage", ConverToImage);
		ETCFUNC("ConverToSVC", ConverToSVC);
		ETCFUNC("ConverToSV", ConverToSV);

		//** chh 2012-06-25 **//
		ETCFUNC("ConverToSVCM", ConverToSVC);
		ETCFUNC("ConverToSVM", ConverToSV);

		ETCFUNC("ConverToSCE", ConverToSCE);
		ETCFUNC("ConverToSCEV", ConverToSCEV);


		ETCFUNC("ConverToHLT", ConverToHLT);
		ETCFUNC("ConverToHLB", ConverToHLB);
		ETCFUNC("ConverToItemButton", ConverToItemButton);
		ETCFUNC("ConverToEquipBtn", ConverToEquipBtn);
		ETCFUNC("ConverToRoleNode",ConverToRoleNode);

		ETCFUNC("ConverToCheckBox", ConverToCheckBox);
		ETCFUNC("ConverToRadioBtn", ConverToRadioBtn);
		ETCFUNC("ConverToRadioGroup", ConverToRadioGroup);
		ETCFUNC("ConverToEdit", ConverToEdit);
		ETCFUNC("ConverToSprite", ConverToSprite);
        
		//++ Guosen 2012.6.3 //��ø���·��
		ETCFUNC("NDPath_GetResourcePath",						NDPath_GetResourcePath);
		ETCFUNC("NDPath_GetImagePath",							NDPath_GetImagePath);
		ETCFUNC("NDPath_GetMapPath",							NDPath_GetMapPath);
		ETCFUNC("NDPath_GetAnimationPath",						NDPath_GetAnimationPath);
		ETCFUNC("NDPath_GetResPath",							NDPath_GetResPath);
		ETCFUNC("NDPath_GetSoundPath",							NDPath_GetSoundPath);
		//tanwt 20120913 //ע�᱾��֪ͨ
		ETCFUNC("RegisterLocalNotification",				    RegisterLocalNotification);
	}

	bool OnScriptUiEvent(NDUINode* uinode, int targetEvent, int param/*= 0*/)
	{
		if (!uinode)
		{
			return false;
		}
		
		LuaObject funcObj;
		
		if (!uinode->GetLuaDelegate(funcObj)
			|| !funcObj.IsFunction())
		{
			return false;
		}
		
		LuaFunction<bool> luaUiEventCallBack = funcObj;
		
		bool bRet = luaUiEventCallBack(uinode, targetEvent, param);
		
		return bRet;
	}
	
//#pragma mark ͼƬ��������
	// ��ɫ�ṹ����
	ETSTRUCTBEGIN(ccColor4B)
	ETSTRUCTPROP("r",								&ccColor4B::r)
	ETSTRUCTPROP("g",								&ccColor4B::g)
	ETSTRUCTPROP("b",								&ccColor4B::b)
	ETSTRUCTPROP("a",								&ccColor4B::a)
	ETSTRUCTEND(ccColor4B)
	
	ETSTRUCTBEGIN(ccColor3B)
	ETSTRUCTPROP("r",								&ccColor3B::r)
	ETSTRUCTPROP("g",								&ccColor3B::g)
	ETSTRUCTPROP("b",								&ccColor3B::b)
	ETSTRUCTEND(ccColor3B)
	
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
    
    //** CHH 2012-08-01 **//
    ETMEMBERFUNC("SetIsTran",						&NDPicture::setIsTran)
	ETCLASSEND(NDPicture)
	
//#pragma mark UI����
	
//#pragma mark UI�ṹ����
	// ��С�ṹ����
	ETSTRUCTBEGIN(CCSize)
	ETSTRUCTPROP("w",								&CCSize::width)
	ETSTRUCTPROP("h",								&CCSize::height)
	ETSTRUCTEND(CCSize)
	
	// ��ṹ����
	ETSTRUCTBEGIN(CCPoint)
	ETSTRUCTPROP("x",								&CCPoint::x)
	ETSTRUCTPROP("y",								&CCPoint::y)
	ETSTRUCTEND(CCPoint)
	
	// ��Χ�ṹ����
	ETSTRUCTBEGIN(CCRect)
	ETSTRUCTPROP("origin",							&CCRect::origin)
	ETSTRUCTPROP("size",							&CCRect::size)
	ETSTRUCTEND(CCRect)
	
//#pragma mark UI���ص���
	ETCLASSBEGIN(NDUILoad)
	ETCONSTRUCT("createNDUILoad")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("Load",							(bool (NDUILoad::*)(const char*, NDUINode *, LuaObject, CGFloat, CGFloat))&NDUILoad::LoadLua)
	ETCLASSEND(NDUILoad)
	
//#pragma mark �ڵ��ർ��"NDNode.h"
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
	
//#pragma mark �����ർ��"NDDirector.h"
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
	ETMEMBERFUNC("GetScaleFactorY",					&NDDirector::GetScaleFactor)
	ETMEMBERFUNC("IsEnableRetinaDisplay",			&NDDirector::IsEnableRetinaDisplay)
	ETCLASSEND(NDDirector)
	
//#pragma mark �����ർ��"NDScene.h"
	// �����ർ��
	ETSUBCLASSBEGIN(NDScene, NDNode)
	ETCFUNC("Scene",								&NDScene::Scene)
	ETCLASSEND(NDScene)
	

    
//#pragma mark  UI���ർ��"NDUINode.h"
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
    // 1-����߷��� 2-���ұ߷��� 3-���Ϸ��� 4-���·���
    ETMEMBERFUNC("FlyToRect",                       &NDUINode::FlyToRect/*(CCRect rect, int nFrameNum, int nDirect)*/)
    ETMEMBERFUNC("SetBoundScale",					&NDUINode::SetBoundScale)
	ETCLASSEND(NDUINode)
	
//#pragma mark ���ർ��"NDUILayer.h"
	// ���ർ��
	ETSUBCLASSBEGIN(NDUILayer, NDUINode)
	ETCONSTRUCT("createNDUILayer")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("Init",							&NDUILayer::Initialization)
	ETMEMBERFUNC("SetBackgroundImage",				&NDUILayer::SetBackgroundImageLua)
	ETMEMBERFUNC("SetBackgroundFocusImage",			&NDUILayer::SetBackgroundFocusImageLua)
	ETMEMBERFUNC("SetBackgroundColor",				&NDUILayer::SetBackgroundColor)
    ETMEMBERFUNC("SetTouchEnabled",                 &NDUILayer::SetTouchEnabled)                //���
	ETMEMBERFUNC("SetFocus",						&NDUILayer::SetFocus)
	ETMEMBERFUNC("GetFocus",						&NDUILayer::GetFocus)
	ETCLASSEND(NDUILayer)
	
//#pragma mark ��ť�ർ��"NDUIButton.h"
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
	ETMEMBERFUNC("ChangeSprite",					&NDUIButton::ChangeSprite)
    ETMEMBERFUNC("SetFocus",                        &NDUIButton::SetFocus)
    ETMEMBERFUNC("SetSoundEffect",                  &NDUIButton::SetSoundEffect)
    ETMEMBERFUNC("SetChecked",                      &NDUIButton::SetChecked)
    //** chh 2012-07-19 **//
    ETMEMBERFUNC("TabSel",                          &NDUIButton::TabSel)
	ETCLASSEND(NDUIButton)
	
//#pragma mark ͼƬ�ർ��"NDUIImage.h"
	ETSUBCLASSBEGIN(NDUIImage, NDUINode)
	ETCONSTRUCT("createNDUIImage")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("GetPicture",						&NDUIImage::GetPicture)
	ETMEMBERFUNC("SetPicture",						&NDUIImage::SetPictureLua)
	ETCLASSEND(NDUIImage)
	
//#pragma mark ��ǩ�ർ����NDUILabel��
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
    
    //** chh 2012-08-14 **//
    ETMEMBERFUNC("SetHasFontBoderColor",			&NDUILabel::SetHasFontBoderColor)
    
	ETCLASSEND(NDUILabel)
	
//#pragma mark ��ǩ�ർ��"NDUIDialog.h"
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
	
//#pragma mark �����ı�����ർ����UIChatText.h��
	ETSUBCLASSBEGIN(CUIChatText,NDUINode)
	ETCONSTRUCT("createUIChatText")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("Init",							&CUIChatText::Initialization)
	ETMEMBERFUNC("SetContentWidth",					&CUIChatText::SetContentWidth)	
	ETMEMBERFUNC("SetContent",						&CUIChatText::SetContent)
	ETMEMBERFUNC("GetContentHeight",				&CUIChatText::GetContentHeight)	
	ETCLASSEND(CUIChatText)
//#pragma mark ���ƶ��㵼��"UIMovableLayer.h"
	ETSUBCLASSBEGIN(CUIMovableLayer, NDUILayer)
	ETCONSTRUCT("createUIMovableLayer")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("Init",							&CUIMovableLayer::Initialization)
	ETMEMBERFUNC("SetMovableViewer",				&CUIMovableLayer::SetMovableViewer)
	ETCLASSEND(CUIMovableLayer)
	
//#pragma mark �����ർ��"UIScroll.h"
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
	
//#pragma mark ������������"UIScrollContainer.h"
	ETSUBCLASSBEGIN(NDUIScrollContainer, NDUILayer)
	ETCONSTRUCT("createUIScrollContainer")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("Init",							&NDUIScrollContainer::Initialization)
	ETMEMBERFUNC("SetLeftReserveDistance",			&NDUIScrollContainer::SetLeftReserveDistance)
	ETMEMBERFUNC("SetRightReserveDistance",			&NDUIScrollContainer::SetRightReserveDistance)
	ETMEMBERFUNC("SetTopReserveDistance",			&NDUIScrollContainer::SetTopReserveDistance)
	ETMEMBERFUNC("SetBottomReserveDistance",		&NDUIScrollContainer::SetBottomReserveDistance)
	ETMEMBERFUNC("ScrollToTop",						&NDUIScrollContainer::ScrollToTop)
	ETMEMBERFUNC("ScrollToBottom",					&NDUIScrollContainer::ScrollToBottom)
	ETMEMBERFUNC("EnableScrollBar",					&NDUIScrollContainer::EnableScrollBar)
	ETCLASSEND(NDUIScrollContainer)
	
//#pragma mark ������ͼ����"UIScrollView.h"
	ETSUBCLASSBEGIN(CUIScrollView, CUIScroll)
	ETCONSTRUCT("createUIScrollView")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("Init",							&CUIScrollView::Initialization)
	ETMEMBERFUNC("SetScrollViewer",					&CUIScrollView::SetScrollViewer)
	ETMEMBERFUNC("SetViewId",						&CUIScrollView::SetViewId)
	ETMEMBERFUNC("GetViewId",						&CUIScrollView::GetViewId)
	ETCLASSEND(CUIScrollView)
	
	ETSUBCLASSBEGIN(NDUIScrollViewContainer, NDUIScrollContainer)
	ETCONSTRUCT("createUIScrollViewContainer")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("Init",						&NDUIScrollViewContainer::Initialization)
	ETMEMBERFUNC("SetStyle",					&NDUIScrollViewContainer::SetStyle)
	ETMEMBERFUNC("GetScrollStyle",				&NDUIScrollViewContainer::GetScrollStyle)
	ETMEMBERFUNC("SetCenterAdjust",				&NDUIScrollViewContainer::SetCenterAdjust)
	ETMEMBERFUNC("IsCenterAdjust",				&NDUIScrollViewContainer::IsCenterAdjust)
	ETMEMBERFUNC("GetViewCount",				&NDUIScrollViewContainer::GetViewCount)
	ETMEMBERFUNC("SetViewSize",				&NDUIScrollViewContainer::SetViewSize)
	ETMEMBERFUNC("GetViewSize",				&NDUIScrollViewContainer::GetViewSize)
	ETMEMBERFUNC("AddView",						&NDUIScrollViewContainer::AddView)
	ETMEMBERFUNC("RemoveView",					&NDUIScrollViewContainer::RemoveView)
	ETMEMBERFUNC("RemoveViewById",				&NDUIScrollViewContainer::RemoveViewById)
	ETMEMBERFUNC("RemoveAllView",				&NDUIScrollViewContainer::RemoveAllView)
	ETMEMBERFUNC("ShowViewByIndex",				&NDUIScrollViewContainer::ShowViewByIndex)
	ETMEMBERFUNC("ShowViewById",				&NDUIScrollViewContainer::ShowViewById)
	ETMEMBERFUNC("ScrollViewByIndex",			&NDUIScrollViewContainer::ScrollViewByIndex)
	ETMEMBERFUNC("ScrollViewById",				&NDUIScrollViewContainer::ScrollViewById)
	ETMEMBERFUNC("GetView",						&NDUIScrollViewContainer::GetView)
	ETMEMBERFUNC("GetViewById",					&NDUIScrollViewContainer::GetViewById)
	ETMEMBERFUNC("GetBeginView",				&NDUIScrollViewContainer::GetBeginView)
	ETMEMBERFUNC("GetBeginIndex",				&NDUIScrollViewContainer::GetBeginIndex)
	ETMEMBERFUNC("EnableScrollBar",				&NDUIScrollViewContainer::EnableScrollBar)
	ETCLASSEND(NDUIScrollViewContainer)

    
    
    //** chh 2012-06-15 **//
//#pragma mark ������ͼ����"UIScrollViewMulHand.h"
	ETSUBCLASSBEGIN(CUIScrollViewM, CUIScroll)
	ETCONSTRUCT("createUIScrollViewM")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("Init",							&CUIScrollViewM::Initialization)
	ETMEMBERFUNC("SetScrollViewer",					&CUIScrollViewM::SetScrollViewer)
	ETMEMBERFUNC("SetViewId",						&CUIScrollViewM::SetViewId)
	ETMEMBERFUNC("GetViewId",						&CUIScrollViewM::GetViewId)
    ETMEMBERFUNC("SetViewPos",						&CUIScrollViewM::SetViewPos)
    ETMEMBERFUNC("GetViewPos",						&CUIScrollViewM::GetViewPos)
	ETCLASSEND(CUIScrollViewM)
	
    
    ETSUBCLASSBEGIN(ContainerClientLayerM, NDUILayer)
	ETCONSTRUCT("createContainerClientLayerM")
	ETDESTRUCT("Free")
    ETMEMBERFUNC("Init",                            &ContainerClientLayerM::Initialization)
    ETMEMBERFUNC("AddView",                         &ContainerClientLayerM::AddView)
    ETMEMBERFUNC("GetView",                         &ContainerClientLayerM::GetView)
    ETMEMBERFUNC("GetViewCount",                         &ContainerClientLayerM::GetViewCount)
    ETMEMBERFUNC("SetViewSize",                     &ContainerClientLayerM::SetViewSize)
    ETMEMBERFUNC("GetViewSize",                     &ContainerClientLayerM::GetViewSize)
    ETCLASSEND(ContainerClientLayerM)
    
    
	ETSUBCLASSBEGIN(CUIScrollViewContainerM, NDUIScrollContainer)
	ETCONSTRUCT("createUIScrollViewContainerM")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("Init",						&CUIScrollViewContainerM::Initialization)
	ETMEMBERFUNC("SetCenterAdjust",				&CUIScrollViewContainerM::SetCenterAdjust)
	ETMEMBERFUNC("IsCenterAdjust",				&CUIScrollViewContainerM::IsCenterAdjust)
	ETMEMBERFUNC("GetViewCount",				&CUIScrollViewContainerM::GetViewCount)
	ETMEMBERFUNC("SetViewSize",                 &CUIScrollViewContainerM::SetViewSize)
	ETMEMBERFUNC("AddView",						&CUIScrollViewContainerM::AddView)
	ETMEMBERFUNC("RemoveView",					&CUIScrollViewContainerM::RemoveView)
	ETMEMBERFUNC("RemoveViewById",				&CUIScrollViewContainerM::RemoveViewById)
	ETMEMBERFUNC("RemoveAllView",				&CUIScrollViewContainerM::RemoveAllView)
	ETMEMBERFUNC("ShowViewByIndex",				&CUIScrollViewContainerM::ShowViewByIndex)
	ETMEMBERFUNC("ShowViewById",				&CUIScrollViewContainerM::ShowViewById)
	ETMEMBERFUNC("ScrollViewByIndex",			&CUIScrollViewContainerM::ScrollViewByIndex)
	ETMEMBERFUNC("ScrollViewById",				&CUIScrollViewContainerM::ScrollViewById)
	ETMEMBERFUNC("GetView",						&CUIScrollViewContainerM::GetView)
	ETMEMBERFUNC("GetViewById",					&CUIScrollViewContainerM::GetViewById)
	ETMEMBERFUNC("GetBeginView",				&CUIScrollViewContainerM::GetBeginView)
	ETMEMBERFUNC("GetBeginIndex",				&CUIScrollViewContainerM::GetBeginIndex)
	ETMEMBERFUNC("EnableScrollBar",				&CUIScrollViewContainerM::EnableScrollBar)
	ETCLASSEND(CUIScrollViewContainerM)
    
    
    //** chh 2012-07-24 **//
    ETSUBCLASSBEGIN(CUIScrollContainerExpand, NDUIScrollContainer)
	ETCONSTRUCT("createUIScrollContainerExpand")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("Init",						&CUIScrollContainerExpand::Initialization)
	ETMEMBERFUNC("AddView",                     &CUIScrollContainerExpand::AddView)
    ETMEMBERFUNC("SetSizeView",                 &CUIScrollContainerExpand::SetSizeView)
    ETMEMBERFUNC("GetCurrentIndex",             &CUIScrollContainerExpand::GetCurrentIndex)
	ETMEMBERFUNC("GetViewCount",                &CUIScrollContainerExpand::GetViewCount)
    ETMEMBERFUNC("GetViewByIndex",              &CUIScrollContainerExpand::GetViewByIndex)
    ETMEMBERFUNC("GetPreIndex",                 &CUIScrollContainerExpand::GetPreIndex)
    ETCLASSEND(CUIScrollContainerExpand)
    
    ETSUBCLASSBEGIN(UIScrollViewExpand, NDUINode)
	ETCONSTRUCT("createUIScrollViewExpand")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("Init",							&UIScrollViewExpand::Initialization)
	ETMEMBERFUNC("SetViewId",						&UIScrollViewExpand::SetViewId)
	ETMEMBERFUNC("GetViewId",						&UIScrollViewExpand::GetViewId)
	ETCLASSEND(UIScrollViewExpand)
    
    
//#pragma mark ��ɫUI�ڵ㵼��"UIRoleNode.h"
	ETSUBCLASSBEGIN(CUIRoleNode, NDUINode)
	ETCONSTRUCT("createUIRoleNode")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("Init",						&CUIRoleNode::Initialization)
	ETMEMBERFUNC("ChangeLookFace",				&CUIRoleNode::ChangeLookFace)
	ETMEMBERFUNC("SetEquip",					&CUIRoleNode::SetEquip)
	ETMEMBERFUNC("SetMove",						&CUIRoleNode::SetMove)
	ETMEMBERFUNC("SetRoleScale",				&CUIRoleNode::SetRoleScale)
	ETMEMBERFUNC("SetRidePet",					&CUIRoleNode::SetRidePet)
	ETCLASSEND(CUIRoleNode)
	
//#pragma mark �������ı�����"UIHyperLink.h"
	ETSUBCLASSBEGIN(CUIHyperlinkText, NDUINode)
	ETCONSTRUCT("createUIHyperLink")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("Init",							&CUIHyperlinkText::Initialization)
	ETMEMBERFUNC("SetLinkBoundRect",				&CUIHyperlinkText::SetLinkBoundRect)
	ETMEMBERFUNC("SetLinkText",						&CUIHyperlinkText::SetLinkText)
	ETMEMBERFUNC("SetLinkTextFontSize",				&CUIHyperlinkText::SetLinkTextFontSize)
	ETMEMBERFUNC("SetLinkTextColor",				&CUIHyperlinkText::SetLinkTextColor)
	ETMEMBERFUNC("EnableLine",						&CUIHyperlinkText::EnableLine)
	ETMEMBERFUNC("SetLinkTextAlignment",			&CUIHyperlinkText::SetLinkTextAlignment)
	ETCLASSEND(CUIHyperlinkText)
	
//#pragma mark �����Ӱ�ť����"UIHyperLink.h"
	ETSUBCLASSBEGIN(CUIHyperlinkButton, NDUIButton)
	ETCONSTRUCT("createUIHyperlinkButton")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("Init",							&CUIHyperlinkButton::Initialization)
	ETMEMBERFUNC("SetLinkBoundRect",				&CUIHyperlinkButton::SetLinkBoundRect)
	ETMEMBERFUNC("SetLinkText",						&CUIHyperlinkButton::SetLinkText)
	ETMEMBERFUNC("SetLinkTextFontSize",				&CUIHyperlinkButton::SetLinkTextFontSize)
	ETMEMBERFUNC("SetLinkTextColor",				&CUIHyperlinkButton::SetLinkTextColor)
	ETMEMBERFUNC("EnableLine",						&CUIHyperlinkButton::EnableLine)
	ETMEMBERFUNC("SetLinkTextAlignment",			&CUIHyperlinkButton::SetLinkTextAlignment)
	ETCLASSEND(CUIHyperlinkButton)
	
//#pragma mark ��Ʒ��ť "UIItemButton.h"
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
	ETMEMBERFUNC("GetItemCount",					&CUIItemButton::GetItemCount)
	ETCLASSEND(CUIItemButton)
	
	ETSUBCLASSBEGIN(CUIEquipItem, CUIItemButton)
	ETCONSTRUCT("createUIEquipItem")
	ETDESTRUCT("Free")
    
    ETMEMBERFUNC("SetUpgrade",				&CUIEquipItem::SetUpgrade)
    ETMEMBERFUNC("GetUpgrade",				&CUIEquipItem::GetUpgrade)
    
    
	ETMEMBERFUNC("GetUpgradeIconPos",				&CUIEquipItem::GetUpgradeIconPos)
    ETMEMBERFUNC("SetUpgradeIconPos",				&CUIEquipItem::SetUpgradeIconPos)
    
	ETCLASSEND(CUIEquipItem)
	
//#pragma mark Check��ť "UICheckBox.h"
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
	
//#pragma mark ��ѡ��ť����ѡ��(���ʹ��)
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
	
//#pragma mark ������ "UIExp.h"
	ETSUBCLASSBEGIN(CUIExp, NDUINode)
	ETCONSTRUCT("createUIExp")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("Init",							(void (CUIExp::*)(const char*, const char*))&CUIExp::Initialization)
	ETMEMBERFUNC("SetStart",						&CUIExp::SetStart)
	ETMEMBERFUNC("SetProcess",						&CUIExp::SetProcess)
	ETMEMBERFUNC("SetTotal",						&CUIExp::SetTotal)
    ETMEMBERFUNC("SetStyle",                        &CUIExp::SetStyle)
	ETMEMBERFUNC("GetStart",						&CUIExp::GetStart)
	ETMEMBERFUNC("GetProcess",						&CUIExp::GetProcess)
	ETMEMBERFUNC("GetTotal",						&CUIExp::GetTotal)
	ETMEMBERFUNC("SetText",							&CUIExp::SetText)
	ETMEMBERFUNC("SetTextFontColor",				&CUIExp::SetTextFontColor)
	ETMEMBERFUNC("SetTextFontSize",					&CUIExp::SetTextFontSize)
	ETCLASSEND(CUIExp)
	
//#pragma mark �����ؼ� "UISpriteNode.h"
	ETSUBCLASSBEGIN(CUISpriteNode, NDUINode)
	ETCONSTRUCT("createUISpriteNode")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("Init",							&CUISpriteNode::Initialization)
	ETMEMBERFUNC("ChangeSprite",					&CUISpriteNode::ChangeSprite)
	ETMEMBERFUNC("SetAnimation",					&CUISpriteNode::SetAnimation)
	ETMEMBERFUNC("SetPlayFrameRange",				&CUISpriteNode::SetPlayFrameRange)
	ETMEMBERFUNC("SetScale",						&CUISpriteNode::SetScale)
	ETMEMBERFUNC("GetScale",						&CUISpriteNode::GetScale)
	ETMEMBERFUNC("GetAnimationAmount",				&CUISpriteNode::GetAnimationAmount)
	ETMEMBERFUNC("PlayAnimation",					&CUISpriteNode::PlayAnimation)
	ETMEMBERFUNC("IsAnimationComplete",				&CUISpriteNode::IsAnimationComplete)
	ETCLASSEND(CUISpriteNode)
	
//#pragma mark �༭��ؼ� "UIEdit.h"
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
	ETMEMBERFUNC("SetTextColor",					&CUIEdit::SetTextColor)
    ETMEMBERFUNC("SetFlag",                         &CUIEdit::SetFlag)
	ETMEMBERFUNC("GetFlag",                         &CUIEdit::GetFlag)
    
	ETCLASSEND(CUIEdit)
	
//#pragma mark �༭��ؼ� "UIList.h"
#if 0
	ETSTRUCTBEGIN(LIST_ATTR_SEC)
	ETSTRUCTPROP("secw",								&LIST_ATTR_SEC::unSectionWidth)
	ETSTRUCTPROP("sech",								&LIST_ATTR_SEC::unSectionHeight)
	ETSTRUCTPROP("lInner",								&LIST_ATTR_SEC::unContentLInner)
	ETSTRUCTPROP("rInner",								&LIST_ATTR_SEC::unContentRInner)
	ETSTRUCTPROP("tInner",								&LIST_ATTR_SEC::unContentTInner)
	ETSTRUCTPROP("bInner",								&LIST_ATTR_SEC::unContentBInner)
	ETSTRUCTPROP("cellInner",							&LIST_ATTR_SEC::unCellInner)
	ETSTRUCTPROP("cellw",								&LIST_ATTR_SEC::unCellWidth)
	ETSTRUCTPROP("cellh",								&LIST_ATTR_SEC::unCellHeight)
	ETSTRUCTEND(LIST_ATTR_SEC)
	
	ETSUBCLASSBEGIN(CUIListCell, NDUINode)
	ETCONSTRUCT("createListCell")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("Init",							&CUIListCell::Initialization)
	ETMEMBERFUNC("SetText",							&CUIListCell::SetText)
	ETMEMBERFUNC("GetText",							&CUIListCell::GetText)
	ETMEMBERFUNC("SetFontSize",						&CUIListCell::SetFontSize)
	ETMEMBERFUNC("SetFontColor",					&CUIListCell::SetFontColor)
	ETMEMBERFUNC("SetBGPicture",					&CUIListCell::SetBGPicture)
	ETMEMBERFUNC("SetCellId",						&CUIListCell::SetCellId)
	ETMEMBERFUNC("GetCellId",						&CUIListCell::GetCellId)
	ETCLASSEND(CUIListCell)
	ETSUBCLASSBEGIN(CUIListSection, NDUINode)
	ETCONSTRUCT("createListSection")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("Init",							&CUIListSection::Initialization)
	ETMEMBERFUNC("SetAttr",							&CUIListSection::SetAttr)
	ETMEMBERFUNC("SetText",							&CUIListSection::SetText)
	ETMEMBERFUNC("GetText",							&CUIListSection::GetText)
	ETMEMBERFUNC("SetFontSize",						&CUIListSection::SetFontSize)
	ETMEMBERFUNC("SetFontColor",					&CUIListSection::SetFontColor)
	ETMEMBERFUNC("SetBGPicture",					&CUIListSection::SetBGPicture)
	ETMEMBERFUNC("SetStatePicture",					&CUIListSection::SetStatePicture)
	ETMEMBERFUNC("SetInnerPicture",					&CUIListSection::SetInnerPicture)
	ETMEMBERFUNC("SetSectionId",					&CUIListSection::SetSectionId)
	ETMEMBERFUNC("GetSectionId",					&CUIListSection::GetSectionId)
	ETMEMBERFUNC("GetFocusIndex",					&CUIListSection::GetFocusIndex)
	ETMEMBERFUNC("GetFocusId",						&CUIListSection::GetFocusId)
	ETMEMBERFUNC("CellCount",						&CUIListSection::CellCount)
	ETMEMBERFUNC("IsExpand",						&CUIListSection::IsExpand)
	ETCLASSEND(CUIListSection)
#endif
	
   #if 0
 ETSUBCLASSBEGIN(CUIList, NDUILayer)
	ETCONSTRUCT("createUIList")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("Init",							&CUIList::Initialization)
	ETMEMBERFUNC("SetSectionInner",					&CUIList::SetSectionInner)
	ETMEMBERFUNC("SetScrollBarWidth",				&CUIList::SetScrollBarWidth)
	ETMEMBERFUNC("GetScrollBarWidth",				&CUIList::GetScrollBarWidth)
	ETMEMBERFUNC("AddSection",						&CUIList::AddSection)
	ETMEMBERFUNC("RemoveSection",					&CUIList::RemoveSection)
	ETMEMBERFUNC("RemoveSectionByIndex",			&CUIList::RemoveSectionByIndex)
	ETMEMBERFUNC("RemoveAllSection",				&CUIList::RemoveAllSection)
	ETMEMBERFUNC("AddCell",							&CUIList::AddCell);
	ETMEMBERFUNC("RemoveCell",						&CUIList::RemoveCell)
	ETMEMBERFUNC("RemoveCellByIndex",				&CUIList::RemoveCellByIndex)
	ETMEMBERFUNC("RemoveAllCell",					&CUIList::RemoveAllCell)
	ETMEMBERFUNC("Expand",							&CUIList::Expand)
	ETMEMBERFUNC("ExpandWithIndex",					&CUIList::ExpandWithIndex)
	ETCLASSEND(CUIList)
#endif
    
//#pragma mark �༭��ؼ� "UITabLogic.h"
	ETSUBCLASSBEGIN(CUITabLogic, NDUINode)
	ETCONSTRUCT("createUITabLogic")
	ETDESTRUCT("Free")
	ETMEMBERFUNC("AddTab",							&CUITabLogic::AddTab)
	ETMEMBERFUNC("Select",							&CUITabLogic::Select)
	ETMEMBERFUNC("SelectWithIndex",					&CUITabLogic::SelectWithIndex)
	ETCLASSEND(CUITabLogic)
}
