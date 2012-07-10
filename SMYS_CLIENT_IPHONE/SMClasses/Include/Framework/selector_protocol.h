/*
�����ص��ӿ���
Copyright(c) 1999-2012, TQ Digital Entertainment, All Rights Reserved
*/
#ifndef __COCOA_SELECTOR_PROTOCOL_H__
#define __COCOA_SELECTOR_PROTOCOL_H__

#include "IActionDelegate.h"
#include "cTypes.h"

//�����ص��ӿ���(ע��:��̳��Ը���ʱ,���뽫����д��ǰ��,��class CTestSel :public SelectorProtocol,public CTest����ʹ�û᲻����!)
class SelectorProtocol
{
public:

    virtual void Callfunc() {};
    virtual void Callfunc(IActionDelegate* pSender)    {}
    virtual void Callfunc(IActionDelegate* pSender, void* pData){}
};


typedef void (SelectorProtocol::*SEL_CallFunc)();//Ҫ���õ���ִ�к����ĵ�ַ
typedef void (SelectorProtocol::*SEL_CallFuncN)(IActionDelegate*);//IActionDelegate* senderΪִ�иö����ľ���
typedef void (SelectorProtocol::*SEL_CallFuncND)(IActionDelegate*, void*);//IActionDelegate* senderΪִ�иö����ľ���

//��ӦCCallFunc
#define callfunc_selector(sel) (SEL_CallFunc)(&sel)

//��ӦCCallFuncN
#define callfuncN_selector(sel) (SEL_CallFuncN)(&sel)

//��ӦCCallFuncND
#define callfuncND_selector(sel) (SEL_CallFuncND)(&sel)

/*�ڶ����е�ʹ�÷���:
CSequence::Actions(
					CCallFunc::ActionWithTarget(&m_testSelPro, callfunc_selector(CTestSelPro::testCCCallFunc)),
                        CCallFuncN::ActionWithTarget(this, callfuncN_selector(CTestScene::callback2)),
                        CCallFuncND::ActionWithTarget(this, callfuncND_selector(CTestScene::callback3), (void*)xx),
                        CCallFuncN::ActionWithTarget(this, callfuncN_selector(CTestScene::initBomb)),
                        NULL)
                    );
					
	void CTestScene::testCCCallFunc();
	void CTestScene::callback2(IActionDelegate* sender);//IActionDelegate* senderΪִ�иö����ľ���
	void CTestScene::callback3(IActionDelegate* sender, void* data)//IActionDelegate* senderΪִ�иö����ľ���
	{
		printf("callback3 ok,pos=(%d,%d) data=%d!\n",sender->GetPosition().x,sender->GetPosition().y,data);
	}

�ο�CCCalFunc��ʹ�÷���:
	static CCallFunc * ActionWithTarget(SelectorProtocol* pSelectorTarget, SEL_CallFunc selector);
	����һ��ִ�к����Ķ���
	pSelectorTarget:Ҫ���ӵ�������Ŀ��(�������̳���SelectorProtocol)
	selector:Ҫ���õĺ���(���������ָ�ʽ:
							1.�޲���,
							2.��һ������:���øö����ľ���,
							3.����������:���øö����ľ���,�����κ�����ֵ):
			typedef void (SelectorProtocol::*SEL_CallFunc)();
			typedef void (SelectorProtocol::*SEL_CallFuncN)(IActionDelegate*);
			typedef void (SelectorProtocol::*SEL_CallFuncND)(IActionDelegate*, void*);
	��:class CTestSelPro :	public SelectorProtocol
		void CTestSelPro::testCCCallFunc()
		{
			printf("CTestSelPro::testCCCallFunc ok!\n");
		}
		��CTestScene.cpp��:
		CCallFunc::ActionWithTarget(&m_testSelPro, callfunc_selector(CTestSelPro::testCCCallFunc))
*/
#endif // __COCOA_SELECTOR_PROTOCOL_H__
