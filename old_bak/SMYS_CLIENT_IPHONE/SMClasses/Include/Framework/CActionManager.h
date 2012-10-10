/**
����������
Copyright(c) 1999-2012, TQ Digital Entertainment, All Rights Reserved
**/
#ifndef __ACTION_CCACTION_MANAGER_H__
#define __ACTION_CCACTION_MANAGER_H__

#include "CAction.h"
#include <vector>
#include "IActionDelegate.h"

using namespace std;
struct _nodeElement;
class  CActionManager 
{
public:
	static CActionManager& sharedInstance();
	/** Adds an action with a target. 
	 If the target is already present, then the action will be added to the existing target.
	 If the target is not present, a new instance of this target will be created either paused or not, and the action will be added to the newly created target.
	 When the target is paused, the queued actions won't be 'ticked'.
	 ����һ������
	 ע��:һ������ͬʱAddAction����������ͬʱִ������������
		  һ�����鲻ҪAddActionͬһ����������,�����ִ��ʱ����ܻ��С
	 */
	bool AddAction(CAction *pAction/*���ӵĶ���*/, IActionDelegate *pTarget/*����*/,bool bPause=false/*��ʼ�Ƿ�����ͣ*/);

	/** 
	 ����һ������
	 ע��:����ֻ֧���ƶ���صĶ�����
	 */
	bool AddAction(CAction *pAction/*���ӵĶ���*/, DLG_HANDLE hDlg, bool bPause= false/*��ʼ�Ƿ�����ͣ*/);

    /** Removes all actions from all the targets.
	ɾ�����еĶ���
    */
	void RemoveAllActions(void);

    /** Removes all actions from a certain target.
	 All the actions that belongs to the target will be removed.
	 ���ݾ����Ի��������ɾ���������ж���,��pTargetΪm_mySprite.GetActionDelegate()
	 */
	void RemoveAllActionsFromTarget(IActionDelegate *pTarget);

    /** Removes an action given an action reference.
	���ݶ���������ɾ��һ������
    */
	void RemoveAction(CAction *pAction);

    /** Removes an action given its tag and the target 
	 ���ݱ�ǩɾ��һ������*/
	void RemoveActionByTag(unsigned int tag, IActionDelegate *pTarget);

	/** Gets an action given its tag an a target
	 @return the Action the with the given tag
	 ���ݱ�ǩ��ö�Ӧ�Ķ���
	 */
	CAction* GetActionByTag(unsigned int tag, IActionDelegate *pTarget);

    /** Returns the numbers of actions that are running in a certain target. 
	 * Composable actions are counted as 1 action. Example:
	 * - If you are running 1 Sequence of 7 actions, it will return 1.
	 * - If you are running 7 Sequences of 2 actions, it will return 7.
	 ��ȡ���鵱ǰִ�еĶ����ĸ���
	 */
	unsigned int NumberOfRunningActionsInTarget(IActionDelegate *pTarget);

    /** Pauses the target: all running actions and newly added actions will be paused
	��ͣһ������
	*/
	void PauseTarget(IActionDelegate *pTarget/*�����Ի������*/);

    /** Resumes the target. All queued actions will be resumed.
	�ָ�ִ��һ������
	*/
	void ResumeTarget(IActionDelegate *pTarget/*�����Ի������*/);

	//�����Ƿ���ڶ�����������
	bool IsActionExist(CAction *pAction/*����*/);

	//bool addAnimation(CAction *pAction,bool bPause=false);
	//	void RemoveActionByTag(unsigned int tag);

	//���Spriteɾ���Ƿ�ȫ
	bool IsSpriteDelSafe(IActionDelegate* pSprite);

	//����(����ڲ�����,�ⲿ��Ҫ����)
	void Update(fTime dt);

	 //����ȡ����ͳһ��sharedInstance�ӿڡ�
	friend	CActionManager* GetActionManager(void);
protected:
	CActionManager(void);
	~CActionManager(void);

protected:
	typedef struct CNodeElement
	{
		std::vector<CAction*> arrAction;//ĳһ��������ж���
		IActionDelegate		  *target;//ĳһ����
		bool  bPause;
		bool  bDeleted; //default value is false;
	}_nodeElement;

	typedef std::vector<_nodeElement> CNodeElementList;
	CNodeElementList m_listNodeElemnt;//���������еĶ����Ͷ���������(����)

	bool				m_bInUpdate;
	CNodeElementList	m_listAdd;
	std::vector<CAction *> m_arrDeleteAction;//��������ʱ����Ĵ�ɾ���������б�����
};

//����ȡ����ͳһ��sharedInstance�ӿڡ�
CActionManager* GetActionManager(void);


#endif // __ACTION_CCACTION_MANAGER_H__
