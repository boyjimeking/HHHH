//
//  NDPlayer.h
//  DragonDrive
//
//  Created by xiezhenghai on 10-12-27.
//  Copyright 2010 (����)DeNA. All rights reserved.
//
#ifndef __NDPlayer_H
#define __NDPlayer_H

#include "NDManualRole.h"
#include <string>
#include "EnumDef.h"
#include "NDUIDialog.h"
#include "NDTimer.h"
#include "BattleUtil.h"
//#include "GatherPoint.h"

class Task;

using namespace std;

#define FOCUS_JUDGE_DISTANCE (33*RESOURCE_SCALE_960)

NS_NDENGINE_BGN

class NDNpc;
class NDUIDialog;

typedef vector<Task*> vec_task;
typedef vec_task::iterator vec_task_it;

class NDPlayer: public NDManualRole,
		public NDUIDialogDelegate,
		public ITimerCallback
{
	DECLARE_CLASS (NDPlayer)
public:
	NDPlayer();
	~NDPlayer();

public:
	// �߼��ӿ�
	static NDPlayer& defaultHero(int lookface = 0, bool bSetLookFace = false);
	static void pugeHero();
	void Walk(CCPoint toPos, SpriteSpeed speed, bool mustArrive = false);

	void SetPosition(CCPoint newPosition);

	void Update(unsigned long ulDiff);
	//���ڻ���
	//void SetPositionEx(CCPoint newPosition);
public:
	// true:Ѱ·������ִ��,false:����������ִ��
	bool ClickPoint(CCPoint point, bool bLongTouch, bool bPath = true);
	bool DealClickPointInSideNpc(CCPoint point);
	bool CancelClickPointInSideNpc();

	void stopMoving(bool bResetPos = true, bool bResetTeamPos = true);

	//����
	void OnMoving(bool bLastPos);
	void OnMoveBegin();
	void OnMoveEnd();

	void OnDrawEnd(bool bDraw);
	void DrawNameLabel(bool bDraw);

	void SetLoadMapComplete()
	{
		m_bIsLoadingMap = false;
	}

	void CaclEquipEffect();

	int GetOrder();

	void OnDialogClose(NDUIDialog* dialog);
	void OnDialogButtonClick(NDUIDialog* dialog, unsigned int buttonIndex);
	virtual void OnTimer(OBJID tag);

	void NextFocusTarget();
	void UpdateFocus();

	bool IsGathering()
	{
		return m_bCollide;
	}
	void ResetGather()
	{
		m_bCollide = false;
	//	m_gp = NULL;
	}

	void ResetFocusRole();

	void SetFocusRole(NDBaseRole *baserole);

	void UpdateFocusPlayer();

	// ս������
	void AddSkill(OBJID idSkill);
	void DelSkill(OBJID idSkill);
	SET_BATTLE_SKILL_LIST& GetSkillList(SKILL_TYPE type);
	bool IsBattleSkillLearned(OBJID idSkill);

	Task* GetPlayerTask(int idTask);

	void BattleStart();

	void BattleEnd(int iResult);

	bool canUnpackRidePet();

	int GetCanUseRepute();

	void SendNpcInteractionMessage(unsigned int uiNPCID);
	// NPC������ز���
public:
	NDNpc* GetFocusNpc();
	int GetFocusNpcID();
	bool IsFocusNpcValid();
	void InvalidNPC();

	bool DirectSwitch(int iSwitchCellX, int iSwitchCellY, int iPassIndex);
	bool CanSwitch(int iSwitchCellX, int iSwitchCellY);

private:

//	bool doGatherPointCollides(GatherPoint *se);
	void processSwitch();
	bool isRoleCanMove();

	void HandleDirectKey();

	void HandleStateBattleField();

	void HandleStateDacoity();

public:
	override void RunAnimation(bool bDraw);

protected:

	virtual void InitializationFroLookFace(int lookface, bool bSetLookFace = true);
	virtual void debugDraw();

private:
	// ����
	static bool ms_bFirstUse;
private:
	bool m_bIsLoadingMap;

public:

	int m_nEMoney; // Ԫ��
	int m_nPhyPoint; // ����  --basic
	int m_nDexPoint; // ���� --basic
	int m_nMagPoint; // ħ�� --basic
	int m_nDefPoint; // ���� --basic
	int m_nLevelUpExp; // �´���������EXP
	int m_nSP;
	/** ����: ����, ��Ӫ; ���� */
	int m_nSWCountry;
	int m_nSWCamp;
	int m_nHonour;
	int m_nExpendHonour; // ����������ֵ

	int m_nSynMoney; // ��������
	int m_nSynContribute; // ���ɰﹱ
	int m_nSynSelfContribute; // ���˰ﹱ
	int m_nSynSelfContributeMoney; // ���˾���Ǯ

	int m_nStorgeMoney; // �ֿ�����
	int m_nStorgeEmoney; // �ֿ�Ԫ��
	int m_nSkillPoint; // ����ֵ

	int iRecordMap;

	// ��ҵ�ǰ���ڵ�ͼid
	int m_nCurMapID;

	int m_eAtkSpd;
	int m_eAtk;
	int m_eDef;
	int m_eHardAtk;
	int m_eSkillAtk;
	int m_eSkillDef;
	int m_eSkillHard;
	int m_eDodge;
	int	m_ePhyHit; // װ��ֱ����������ֵ

	// �������,���Խ���������Ի���
	int m_iTmpPhyPoint;
	int m_iTmpDexPoint;
	int m_iTmpMagPoint;
	int m_iTmpDefPoint;
	int m_iTmpRestPoint;

	// ����״̬��������
	/** �������� */
	int phyAdd;

	/** ���ݸ��� */
	int dexAdd;

	/** �������� */
	int defAdd;

	/** �������� */
	int magAdd;

	/** �﹥���� */
	int wuGongAdd;

	/** ������� */
	int wuFangAdd;

	/** �������� */
	int faGongAdd;

	/** �������� */
	int faFangAdd;

	/** ���ܸ��� */
	int sanBiAdd;

	/** �������� */
	int baoJiAdd;

	/** �������и��� */
	int wuLiMingZhongAdd;

	/** ����*/
	int m_nGamePoints;

	/**�ֵ*/
	int m_nActivityValue;
	int m_nActivityValueMax;
	/**���ܲ�*/
	int m_nMaxSlot;
	/**VIP�ȼ�*/
	int m_nVipLev;

public:
	void SetLocked(bool locked)
	{
		m_bLocked = locked;
	}
public:
	int m_iFocusManuRoleID;
	//NDNpc *m_npcFocus;

	int m_nTargetIndex; // ��ҵ�ǰѡ��Ľ�ɫ����,��������magmgr�������NPC,���������������������.

	vec_task m_vPlayerTask; //��������б�
private:
	bool m_bCollide;
//	GatherPoint *m_gp;
	NDTimer* m_pkTimer;
	NDUIDialog* m_kGatherDlg;
	bool m_bRequireDacoity;
	bool m_bRequireBattleField;
	int m_iDacoityStep;
	int m_iBattleFieldStep;
	int m_iFocusNpcID;
	bool m_bLocked;
	SET_BATTLE_SKILL_LIST m_setActSkill;
	SET_BATTLE_SKILL_LIST m_setPasSkill;

	//int m_nLookface;
public:
	// ����˷���������Ҫ���������
	struct
	{
		int extra_phyPoint;
		int extra_dexPoint;
		int extra_magPoint;
		int extra_defPoint;
		int phy_atk;
		int phy_def;
		int mag_atk;
		int mag_def;
		int atk_speed;
		int hitrate;
		int dodge;
		int hardhit;
	} m_caclData;

};

NS_NDENGINE_END

#endif
