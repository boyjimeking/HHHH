/**
  ��ʱ������
	Copyright(c) 1999-2012, TQ Digital Entertainment, All Rights Reserved
**/
#ifndef __CCINSTANT_ACTION_H__
#define __CCINSTANT_ACTION_H__

#include <string>
#include "CAction.h"
#include "selector_protocol.h"

/**
@brief Instant actions are immediate actions. They don't have a duration like
the CCIntervalAction actions.
˲ʱ����:����Ҫʱ��,���Ͼ���ɵĶ���
*/
class CActionInstant : public CFiniteTimeAction 
{
public:
    CActionInstant();
    virtual ~CActionInstant();

    // CAction methods
    virtual bool IsDone(void);
    virtual void Step(fTime dt);
    virtual void Update(fTime time);
    //CFiniteTimeAction method
    virtual CFiniteTimeAction * Reverse(void);
};

/** @brief Show the node
��ʾ �C Show
Ч�������� [node setVisible:YES].
*/
class CShow : public CActionInstant
{
public:
    CShow();
    virtual ~CShow();
    //super methods
    virtual void StartWithTarget(IActionDelegate *pTarget);
    virtual CFiniteTimeAction * Reverse(void);
public:
    //override static method
    /** Allocates and initializes the action */
    static CShow * Action();
};



/**
@brief Hide the node
���� �C Hide
Ч�������� [node setVisible:NO].
*/
class CHide : public CActionInstant
{
public:
    CHide() {}
    virtual ~CHide() {}
    //super methods
    virtual void StartWithTarget(IActionDelegate *pTarget);
    virtual CFiniteTimeAction * Reverse(void);
//    virtual CObject* copyWithZone(CCZone *pZone);
public:
    //override static method
    /** Allocates and initializes the action */
    static CHide * Action();
};

/** @brief Toggles the visibility of a node
�ɼ��л�
*/
class CToggleVisibility : public CActionInstant
{
public:
    CToggleVisibility();
    virtual ~CToggleVisibility();
    //super method
    virtual void StartWithTarget(IActionDelegate *pTarget);
public:
    //override static method
    /** Allocates and initializes the action */
    static CToggleVisibility * Action();
};

/**
@brief Flips the sprite horizontally

����X���귴ת
*/
class CFlipX : public CActionInstant
{
public:
    CFlipX();
    virtual ~CFlipX();

    /** create the action */
	//����X���귭ת�Ķ���
    static CFlipX * ActionWithFlipX(bool x);
    /** init the action */
    bool InitWithFlipX(bool x);
    //super methods
    virtual void StartWithTarget(IActionDelegate *pTarget);
    virtual CFiniteTimeAction * Reverse(void);

protected:
    bool	m_bFlipX;
};

/**
@brief Flips the sprite vertically

����Y���귴ת
*/
class CFlipY : public CActionInstant
{
public:
    CFlipY();
    virtual ~CFlipY();

    /** create the action */
	//����Y���귭ת�Ķ���
	static CFlipY * ActionWithFlipY(bool y);
    /** init the action */
    bool InitWithFlipY(bool y);
    //super methods
    virtual void StartWithTarget(IActionDelegate *pTarget);
    virtual CFiniteTimeAction * Reverse(void);

protected:
    bool	m_bFlipY;
};

/** @brief Places the node in a certain position
����,��������λ�õĶ���
*/
class CPlace : public CActionInstant //<NSCopying>
{
public:
    CPlace() {}
    virtual ~CPlace() {}
    /** creates a Place action with a position */

	//��������λ�õĶ���
    static CPlace * ActionWithPosition(const CPoint& pos/*Ҫ�ƶ���������λ��*/);

    /** Initializes a Place action with a position */
    bool InitWithPosition(const CPoint& pos);
    //super methods
    virtual void StartWithTarget(IActionDelegate *pTarget);
//    virtual CObject* copyWithZone(CCZone *pZone);
protected:
    CPoint m_tPosition;
};

/** @brief Calls a 'callback'
��������,ִ����һ�������Ķ���
*/
class CCallFunc : public CActionInstant //<NSCopying>
{
public:
    CCallFunc();
    virtual ~CCallFunc();
    /** creates the action with the callback

    typedef void (SelectorProtocol::*SEL_CallFunc)();
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
    static CCallFunc * ActionWithTarget(SelectorProtocol* pSelectorTarget, SEL_CallFunc selector);
   // static CCallFunc* actionWithScriptFuncName(const char* pszFuncName);
    virtual bool InitWithTarget(SelectorProtocol* pSelectorTarget);
    /** executes the callback */
    virtual void Execute();
    //super methods
    virtual void StartWithTarget(IActionDelegate *pTarget);

 //   void registerScriptFunction(const char* pszFunctionName);

    SelectorProtocol* GetTargetCallback();

    void SetTargetCallback(SelectorProtocol* pSel);

protected:
    /** Target that will be called */
    SelectorProtocol*   m_pSelectorTarget;
    /** the script function name to call back */
 //   std::string         m_scriptFuncName;

    union
    {
        SEL_CallFunc	m_pCallFunc;
        SEL_CallFuncN	m_pCallFuncN;
        SEL_CallFuncND	m_pCallFuncND;
     //   SEL_CallFuncO   m_pCallFuncO;
    };
};

/**
@brief Calls a 'callback' with the node as the first argument
N means Node
ִ�к���,��������� �����Զ��庯��ʱ,�����õĲ�������
��:void CTestScene::callback2(IActionDelegate* sender)
{
printf("callback2 ok,pos=(%d,%d)!\n",sender->GetPosition().x,sender->GetPosition().y);
//�Ѿ����͸����������
sender->SetOpacity(128);
}
*/
class CCallFuncN : public CCallFunc
{
public:
    CCallFuncN();
    virtual ~CCallFuncN();
    static CCallFuncN * ActionWithTarget(SelectorProtocol* pSelectorTarget, SEL_CallFuncN selector);
   // static CCallFuncN* actionWithScriptFuncName(const char* pszFuncName);
    virtual bool InitWithTarget(SelectorProtocol* pSelectorTarget, SEL_CallFuncN selector);
    virtual void Execute();
};


/**
@brief Calls a 'callback' with the node as the first argument and the 2nd argument is data
* ND means: Node and Data. Data is void *, so it could be anything.
ִ�к�������������:һ������ʹ����κ����͵�ֵ
*/
class CCallFuncND : public CCallFuncN
{
public:

    /** creates the action with the callback and the data to pass as an argument */
    static CCallFuncND * ActionWithTarget(SelectorProtocol* pSelectorTarget, SEL_CallFuncND selector, void* d);
   // static CCallFuncND* actionWithScriptFuncName(const char* pszFuncName, void *d);
    /** initializes the action with the callback and the data to pass as an argument */
    virtual bool InitWithTarget(SelectorProtocol* pSelectorTarget, SEL_CallFuncND selector, void* d);
    virtual void Execute();

protected:
    void			*m_pData;
};



#endif //__CCINSTANT_ACTION_H__
