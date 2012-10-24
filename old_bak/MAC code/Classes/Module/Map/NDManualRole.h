//
//  NDManualRole.h
//  DragonDrive
//
//  Created by xiezhenghai on 10-12-17.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//
#ifndef __NDManualRole_H
#define __NDManualRole_H

#include "NDBaseRole.h"
#include "NDBattlePet.h"
#include "NDRidePet.h"
#include <string>
#include <deque>
#include "NDMonster.h"
#include "NDUIImage.h"
#include "SyndicateCommon.h"
#include "NDUILabel.h"
#include "NDLightEffect.h"

using namespace std;

namespace NDEngine
{
	#define BEGIN_PROTECTED_TIME 3// 人物一开始的的保护时间,即不会遇怪
	
	
	#define SHOW_NAME_ROLE_W (64) // 在w(-64,64)区域内显示怪物和npc名字

	#define SHOW_NAME_ROLE_H (64)
	
	typedef struct _tagShowPetInfo
	{
		OBJID	idPet;
		int		lookface;
		int		quality;
		
		_tagShowPetInfo() 
		{
			memset(this, 0, sizeof(*this));
		}
		
		_tagShowPetInfo(OBJID idPet, int lookface, int quality)
		{
			this->idPet		= idPet;
			this->lookface	= lookface;
			this->quality	= quality;
		}
		
		bool operator==(const _tagShowPetInfo& r)
		{
			return this->lookface == r.lookface;
		}
	}ShowPetInfo;
	
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
	class NDManualRole : public NDBaseRole 
	{
		DECLARE_CLASS(NDManualRole)
		
	public:
		// 服务端下发的光效显示
		typedef struct _tagServerEffect
		{
			bool bQiZhi;
			int severEffectId;
			NDLightEffect* effect;
			_tagServerEffect()
			{
				this->bQiZhi = false;
				this->effect = NULL;
				this->severEffectId = 0;
			}
			
			_tagServerEffect(bool bQiZhi, NDLightEffect* effect, int severEffectId)
			{
				this->bQiZhi = bQiZhi;
				this->effect = effect;
				this->severEffectId = severEffectId;
			}
			
		}ServerEffect;
		
	public:
		NDManualRole();
		~NDManualRole();
		
	public:
		void Update(unsigned long ulDiff); override
		void ReLoadLookface(int lookface);
		void SetAction(bool bMove, bool ignoreFighting=false); hide
		bool AssuredRidePet(); hide
		
		void Initialization(int lookface, bool bSetLookFace=true); hide 
		
		virtual void Walk(CGPoint toPos, SpriteSpeed speed);
		void OnMoving(bool bLastPos); override
		void OnMoveEnd(); override
		void SetPosition(CGPoint newPosition); override
		void OnMoveTurning(bool bXTurnigToY, bool bInc); override
		
		bool OnDrawBegin(bool bDraw); override
		void OnDrawEnd(bool bDraw); override
		
		virtual void stopMoving(bool bResetPos=true, bool bResetTeamPos=true); hide
		
		void drawEffects(bool bDraw); override
		
		void unpakcAllEquip(); override
		
		void SetServerPositon(int col, int row);
		
		void SetServerDir(int dir);
		
		int GetServerX(){ return serverCol; }
		int GetServerY(){ return serverRow; }
		
		void uppackBattlePet();
		
		// 更新某一状态
		void UpdateState(int nState, bool bSet);
		// 直接设置状态
		void SetState(int nState);
		
		bool IsInState(int iState);
		
		// 正负状态
		bool IsInDacoity();
		
		void HandleEffectDacoity();
		
		// 勿用,如需获取请直接访问battlepet
		//NDBattlePet* GetBattlePet();
		
		void AddWalkDir(int dir);
		
		// 队伍相关接口
		bool isTeamMember(){ return teamId != 0; }
		bool isTeamLeader(){ return teamId == m_id; }
		void teamSetServerDir(int dir);
		void teamSetServerPosition(int iCol, int iRow);
		
		bool IsSafeProtected(){ return isSafeProtected; }
		void setSafeProtected(bool isSafeProtected);
		
		void updateFlagOfQiZhi();
		
		void updateTransform(int idLookface);
		
		bool isTransformed();
		
		// 军团相关
		void setSynRank(int rank) {
			this->synRank = rank;
			this->synRankStr = getRankStr(synRank);
		}
		
		int getSynRank() const {
			return this->synRank;
		}
		
		void TeamSetToLastPos(bool bSet) { bToLastPos = bSet; }
		
		void playerLevelUp();
		
		void playerMarry();
		
		void updateBattlePetEffect();
		void refreshEquipmentEffectData();
		
		void BackupPositon();
		CGPoint GetBackupPosition();
		
		// 勿用,如需获取请直接访问ridePet
		//NDRidePet* GetRidePet();
		// 判断传入的角色绘制的优先级是否比自己高
		bool IsHighPriorDrawLvl(NDManualRole* otherRole);
		
		void SetServerEffect(std::vector<int>& vEffect);
		bool isEffectTurn(int effectTurn);
		void drawServerEffect(std::vector<ServerEffect>& vEffect, bool draw);
		void drawServerBackEffect(bool draw);
		void drawServerFrontEffect(bool draw);
		bool IsServerEffectHasQiZhi();
		
		CGRect GetFocusRect(); override
		
		bool IsInGraveStoneState();
		
		void SetShowPet(ShowPetInfo& info);
		bool GetShowPetInfo(ShowPetInfo& info);
		const NDBattlePet* GetShowPet();
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
		// 爵位
		static std::string	GetPeerageName(int nPeerage);
		static unsigned int	GetPeerageColor(int nPeerage);
		
	private:
		bool CheckToLastPos();
		void ShowNameLabel(bool bDraw);
		enum  LableType{ eLableName, eLabelSynName, eLablePeerage,};
		void SetLableName(std::string text, int x, int y, bool isEnemy);
		void SetLable(LableType eLableType, int x, int y, std::string text, ccColor4B color1, ccColor4B color2);
		SpriteSpeed GetSpeed();
	protected:
		void WalkToPosition(const std::vector<CGPoint>& vec_toPos, SpriteSpeed speed, bool moveMap, bool mustArrive=false);
		void processTeamMemberMove(bool bDraw);
		
		enum { eMoveTypeBegin = 0, eMoveTypeMoving, eMoveTypeEnd,};
		void processTeamMemberOnMove(int itype);
		
		void resetTeamPos();
		void teamMemberWalkToPosition(const std::vector<CGPoint>& toPos);
		void teamMemberWalkResetPosition();
		void teamMemberAction(bool bAction);
		void teamMemberStopMoving(bool bResetPos);
		bool TeamIsToLastPos() { return bToLastPos; }

	public:
		void SetTeamToLastPos();
		//++Guosen 2012.7.13
		int GetLookface(){ return m_nLookface; }
		bool ChangeModelWithMount( int nRideStatus, int nMountType );
	public:
		int state;								// 状态
		int money;								// 银币
		int	dwLookFace;							// 创建人物的时候有6种外观可供选择外观
		int iProfesstion;						//玩家的职业
		int pkPoint;							// pk值
		
		string synName;							// 帮派名字
		string synRankStr;						// 官职名字
		
		string rank;
		int		m_nRank;
		
		bool levelup;
		
		int exp; // 经验值
		int restPoint; // 剩余点数
		
		int teamId; // 队伍id
		
		/* 伴侣id**/
		int marriageID;
		
		/* 伴侣名字**/
		std::string loverName;
		
		int marriageState;
		
		// 骑宠相关
		//NDRidePet	*ridepet;
		
		int campOutOfTeam;
		
		bool bClear;
		
		NDMonster* aniGroupTransformed;
		
		vector<int> m_vEquipsID;
		
		// 爵位
		int m_nPeerage;
		//++Guosen 2012.7.14
		int		m_nRideStatus;	//骑乘状态0=步行，1=骑行
		int		m_nMountType;	//坐骑类型
		int		m_nLookface;	//
        int     m_nQuality;
	private:
		// 战宠相关
		CAutoLink<NDBattlePet> m_pBattlePetShow;
		ShowPetInfo	m_infoShowPet;
		
		deque<int>	m_dequeWalk;
		bool		bUpdateDiff;
		int idTransformTo;
		NDPicture* m_picVendor;
		NDPicture* m_picBattle;
		NDPicture* m_picGraveStone;
		int synRank;							// 帮派级别
		
		NDLightEffect* m_numberOneEffect;
		void RunNumberOneEffect();
		
	protected:
		int beginProtectedTime;
		bool isSafeProtected;
		int serverCol, serverRow;
	public:
		CGPoint m_oldPos;
	protected:
		bool bToLastPos;
		
		NDUILabel *m_lbName[2];
		NDUILabel *m_lbSynName[2];
		NDUILabel *m_lbPeerage[2];
		
		NDSprite *effectFeedPetAniGroup;
		NDSprite *effectArmorAniGroup;
		NDSprite *effectDacoityAniGroup;
		
		CGPoint m_posBackup;
	private:
		std::vector<ServerEffect> m_serverEffectBack, m_serverEffectFront;
	private:
		std::map<std::string, NDLightEffect*>		m_arrMapSMEffect[eSM_EFFECT_DRAW_ORDER_END][eSM_EFFECT_ALIGNMENT_END];
		
	};
}

#endif


