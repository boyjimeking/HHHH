//
//  NDManualRole.h
//  DragonDrive
//
//  Created by xiezhenghai on 10-12-17.
//  Copyright 2010 (����)DeNA. All rights reserved.
//
#ifndef __NDManualRole_H
#define __NDManualRole_H

#include "NDBaseRole.h"
//#include "NDBattlePet.h"
//#include "NDRidePet.h"
#include <string>
#include <deque>
#include "NDMonster.h"
#include "NDUIImage.h"

#include "NDUILabel.h"
#include "NDLightEffect.h"
#include "../../Syndicate/inc/SyndicateCommon.h"

using namespace std;

namespace NDEngine
{
#define BEGIN_PROTECTED_TIME 3// ����һ��ʼ�ĵı���ʱ��,����������
#define SHOW_NAME_ROLE_W (64) // ��w(-64,64)��������ʾ�����npc����
#define SHOW_NAME_ROLE_H (64)

typedef struct _tagShowPetInfo
{
	OBJID idPet;
	int lookface;
	int quality;

	_tagShowPetInfo()
	{
		memset(this, 0, sizeof(*this));
	}

	_tagShowPetInfo(OBJID idPet, int lookface, int quality)
	{
		idPet = idPet;
		lookface = lookface;
		quality = quality;
	}

	bool operator==(const _tagShowPetInfo& r)
	{
		return lookface == r.lookface;
	}
} ShowPetInfo;

enum eSM_EFFECT_ALIGNMENT
{
	eSM_EFFECT_ALIGNMENT_BEGIN,
	eSM_EFFECT_ALIGNMENT_BOTTOM	= eSM_EFFECT_ALIGNMENT_BEGIN,
	eSM_EFFECT_ALIGNMENT_CENTER,
	eSM_EFFECT_ALIGNMENT_TOP,
	eSM_EFFECT_ALIGNMENT_END,
};
enum eSM_EFFECT_DRAW_ORDER
{
	eSM_EFFECT_DRAW_ORDER_BEGIN,
	eSM_EFFECT_DRAW_ORDER_FRONT	= eSM_EFFECT_DRAW_ORDER_BEGIN,
	eSM_EFFECT_DRAW_ORDER_BACK,
	eSM_EFFECT_DRAW_ORDER_END,
};
class NDPicture;
class NDManualRole: public NDBaseRole
{
	DECLARE_CLASS (NDManualRole)

public:
	// ������·��Ĺ�Ч��ʾ
	typedef struct _tagServerEffect
	{
		bool bQiZhi;
		int severEffectId;
		NDLightEffect* effect;
		_tagServerEffect()
		{
			bQiZhi = false;
			effect = NULL;
			severEffectId = 0;
		}

		_tagServerEffect(bool bQiZhi, NDLightEffect* effect, int severEffectId)
		{
			bQiZhi = bQiZhi;
			effect = effect;
			severEffectId = severEffectId;
		}

	} ServerEffect;

public:
	NDManualRole();
	~NDManualRole();

public:

	void Update(unsigned long ulDiff); override
	void ReLoadLookface(int lookface);
	void SetAction(bool bMove, bool bIgnoreFighting = false); hide
	bool AssuredRidePet();hide

	void Initialization(int lookface, bool bSetLookFace = true); hide

	virtual void Walk(CCPoint toPos, SpriteSpeed speed);
	void OnMoving(bool bLastPos); override
	void OnMoveEnd(); override 
	void SetPosition(CCPoint newPosition); override
	void OnMoveTurning(bool bXTurnigToY, bool bInc); override

	bool OnDrawBegin(bool bDraw); override
	void OnDrawEnd(bool bDraw); override

	virtual void stopMoving(bool bResetPos = true, bool bResetTeamPos = true);

	void drawEffects(bool bDraw);

	void unpakcAllEquip();

	void SetServerPositon(int col, int row);

	void SetServerDir(int dir);

	int GetServerX()
	{
		return m_nServerCol;
	}
	int GetServerY()
	{
		return m_nServerRow;
	}

	void uppackBattlePet();

	// ����ĳһ״̬
	void UpdateState(int nState, bool bSet);
	// ֱ������״̬
	void SetState(int nState);

	bool IsInState(int iState);

	// ����״̬
	bool IsInDacoity();

	void HandleEffectDacoity();

	void AddWalkDir(int dir);

	// ������ؽӿ�
	bool isTeamMember()
	{
		return m_nTeamID != 0;
	}
	bool isTeamLeader()
	{
		return m_nTeamID == m_nID;
	}
	void teamSetServerDir(int dir);
	void teamSetServerPosition(int iCol, int iRow);

	bool IsSafeProtected()
	{
		return m_bIsSafeProtected;
	}
	void setSafeProtected(bool isSafeProtected);

	void updateFlagOfQiZhi();

	void updateTransform(int idLookface);

	bool isTransformed();

	// �������
	void setSynRank(int rank)
	{
		m_nSynRank = rank;
		m_strSynRank = getRankStr(m_nSynRank);
	}

	int getSynRank() const
	{
		return m_nSynRank;
	}

	void TeamSetToLastPos(bool bSet)
	{
		m_bToLastPos = bSet;
	}

	void playerLevelUp();

	void playerMarry();

	void updateBattlePetEffect();
	void refreshEquipmentEffectData();

	void BackupPositon();
	CCPoint GetBackupPosition();

	// ����,�����ȡ��ֱ�ӷ���ridePet
	//NDRidePet* GetRidePet();
	// �жϴ���Ľ�ɫ���Ƶ����ȼ��Ƿ���Լ���
	bool IsHighPriorDrawLvl(NDManualRole* otherRole);

	void SetServerEffect(std::vector<int>& vEffect);
	bool isEffectTurn(int effectTurn);
	void drawServerEffect(std::vector<ServerEffect>& vEffect, bool draw);
	void drawServerBackEffect(bool draw);
	void drawServerFrontEffect(bool draw);
	bool IsServerEffectHasQiZhi();

	CCRect GetFocusRect();override

	bool IsInGraveStoneState();

	void SetShowPet(ShowPetInfo& kInfo);
	bool GetShowPetInfo(ShowPetInfo& info);
	//const NDBattlePet* GetShowPet();
	void ResetShowPetPosition();
	void ResetShowPet();
	int GetPathDir(int oldX, int oldY, int newX, int newY);
	bool GetXYByDir(int oldX, int oldY, int dir, int& newX, int& newY);
	bool IsDirFaceRight(int nDir);
	bool AddSMEffect(std::string strEffectPath, int nSMEffectAlignment, int nDrawOrder);
	bool RemoveSMEffect(std::string strEffectPath);
private:
	void RemoveAllSMEffect();
	void RunSMEffect(int nDrawOrder);
public:
	// ��λ
	static std::string GetPeerageName(int nPeerage);
	static unsigned int GetPeerageColor(int nPeerage);

private:
	bool CheckToLastPos();
	void ShowNameLabel(bool bDraw);
	enum LableType
	{
		eLableName,
		eLabelSynName,
		eLablePeerage,
	};
	void SetLableName(std::string text, int x, int y, bool isEnemy);
	void SetLable(LableType eLableType, int x, int y, std::string text,
			cocos2d::ccColor4B color1, cocos2d::ccColor4B color2);
	SpriteSpeed GetSpeed();
protected:
	void WalkToPosition(const std::vector<CCPoint>& kToPosVector,
			SpriteSpeed eSpeed, bool bMoveMap, bool bMustArrive = false);
	void processTeamMemberMove(bool bDraw);

	enum
	{
		eMoveTypeBegin = 0,
		eMoveTypeMoving,
		eMoveTypeEnd,
	};
	void processTeamMemberOnMove(int itype);

	void resetTeamPos();
	void teamMemberWalkToPosition(const std::vector<CCPoint>& toPos);
	void teamMemberWalkResetPosition();
	void teamMemberAction(bool bAction);
	void teamMemberStopMoving(bool bResetPos);
	bool TeamIsToLastPos()
	{
		return m_bToLastPos;
	}

public:
	void SetTeamToLastPos();
	//++Guosen 2012.7.13
	int GetLookface(){ return m_nLookface; }
	bool ChangeModelWithMount( int nRideStatus, int nMountType );
public:
	int m_nState;								// ״̬
	int m_nMoney;								// ����
/*	int m_dwLookFace;							// ���������ʱ����6����ۿɹ�ѡ�����*/
	int m_nProfesstion;						//��ҵ�ְҵ
	int m_nPKPoint;							// pkֵ

	string m_strSynName;							// ��������
	string m_strSynRank;						// ��ְ����

	string m_strRank;
	int m_nRank;

	bool m_bLevelUp;

	int m_nExp; // ����ֵ
	int m_nRestPoint; // ʣ�����

	int m_nTeamID; // ����id

	/* ����id**/
	int m_nMarriageID;

	/* ��������**/
	std::string m_strLoverName;

	int m_nMarriageState;

	// ������
	//NDRidePet	*ridepet;

	int m_nCampOutOfTeam;

	bool m_bClear;

	NDMonster* m_pkAniGroupTransformed;

	vector<int> m_vEquipsID;

	// ��λ
	int m_nPeerage;
	//++Guosen 2012.7.14
	int	m_nRideStatus;	//���״̬0=���У�1=����
	int	m_nMountType;	//��������
	int	m_nLookface;	//
	int m_nQuality;
private:
	// ս�����
	//CAutoLink<NDBattlePet> m_pBattlePetShow;
	ShowPetInfo m_kInfoShowPet;

	deque<int> m_kDequeWalk;
	bool m_bUpdateDiff;
	int m_nIDTransformTo;
	NDPicture* m_pkVendorPicture;
	NDPicture* m_pkBattlePicture;
	NDPicture* m_pkGraveStonePicture;
	int m_nSynRank;							// ���ɼ���

	NDLightEffect* m_pkNumberOneEffect;
	void RunNumberOneEffect();

protected:

	int m_nBeginProtectedTime;
	bool m_bIsSafeProtected;
	int m_nServerCol;
	int m_nServerRow;

public:

	CCPoint m_kOldPos;

protected:

	bool m_bToLastPos;

	NDUILabel *m_lbName[2];
	NDUILabel *m_lbSynName[2];
	NDUILabel *m_lbPeerage[2];

	NDSprite* m_pkEffectFeedPetAniGroup;
	NDSprite* m_pkEffectArmorAniGroup;
	NDSprite* m_pkEffectDacoityAniGroup;

	CCPoint m_kPosBackup;

private:

	std::vector<ServerEffect> m_kServerEffectBack;
	std::vector<ServerEffect> m_kServerEffectFront;

private:

	std::map<std::string, NDLightEffect*> m_kArrMapSMEffect[eSM_EFFECT_DRAW_ORDER_END][eSM_EFFECT_ALIGNMENT_END];
};
}

#endif

