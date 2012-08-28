/*
 *  NDBaseRole.h
 *  DragonDrive
 *
 *  Created by jhzheng on 11-1-7.
 *  Copyright 2011 (����)DeNA. All rights reserved.
 *
 */

#ifndef _ND_BASE_ROLE_H
#define _ND_BASE_ROLE_H

#include "NDSprite.h"
#include "EnumDef.h"
#include "NDPath.h"
#include "NDRidePet.h"

namespace NDEngine
{
	#define RING_IMAGE		(NDPath::GetFullImagepath("ui_ring.png"))
	#define SHADOW_IMAGE	(NDPath::GetFullImagepath("shadow.png"))
	#define BIG_SHADOW_IMAGE (NDPath::GetFullImagepath("shadowBig.png"))
	
	#define CAMP_NEUTRAL	0
	#define CAMP_TANG		1
	#define CAMP_SUI		2
	#define CAMP_TUJUE		3
	#define CAMP_FOR_ESCORT 4
	#define CAMP_EUDEMON	6 
	#define CAMP_BATTLE		9
	
	class NDNode;
	class NDRidePet;
	class NDBaseRole : 
		public NDSprite,
		public NDNodeDelegate
	{
		DECLARE_CLASS(NDBaseRole)
	public:
		NDBaseRole();
		~NDBaseRole();
	public:
		//���·������߼���ʹ�ã�����begin
		//......
		//������end

 		bool OnDrawBegin(bool bDraw); override
// 		void OnDrawEnd(bool bDraw); override
// 		void OnBeforeNodeRemoveFromParent(NDNode* node, bool bCleanUp); override
		
		CGPoint GetScreenPoint();
		void DirectRight(bool bRight);
		
	public:

		virtual void Update(unsigned long ulDiff){}
		virtual void SetAction(bool bMove);
		virtual bool AssuredRidePet();
		NDRidePet* GetRidePet();
		virtual void setMoveActionWithRidePet();
		virtual void setStandActionWithRidePet();
		virtual void updateRidePetEffect();
		
		virtual void drawEffects(bool bDraw);
		
		// ��ҵĹ����ʼ��lookface
		void InitRoleLookFace(int lookface);
		
		// NPC�����ĳ�ʼ��
		void InitNonRoleData(std::string name, int lookface, int lev);
		
		void SetHair(int style, int color = 1);
		
		void SetEquipment(int equipmentId, int quality);
		
		void defaultDeal();
		
		/** �����ιֵ�lookface�н��������õ��������ؼף�ͷ����lookface*/
		int getEquipmentLookFace(int lookface, int type);
		
		void SetHairImageWithEquipmentId(int equipmentId);
		void SetFaceImageWithEquipmentId(int equipmentId);
		void SetExpressionImageWithEquipmentId(int equipmentId);
		void SetCapImageWithEquipmentId(int equipmentId);
		void SetArmorImageWithEquipmentId(int equipmentId);
		void SetCloakImageWithEquipmentId(int equipmentId);
		
		void SetPositionEx(CGPoint newPosition);
		
		void unpackEquip(int iEquipPos);
		
		//������û�б���װ�����ݵĽ�ɫ
		virtual void unpakcAllEquip();
		
		void addTalkMsg(std::string msg,int timeForTalkMsg);
		//		
		//		������SetNormalAniGroup
		//		���ã�����Ĭ�϶�����
		//		������lookface�������·��ı�ʶ�����ھ���ı���
		//		����ֵ����		
		void SetNormalAniGroup(int lookface);
	public:
		void DrawHead(const CGPoint& pos);
		void SetWeaponType(int weaponType);
		int GetWeaponType();
		
		void SetSecWeaponType(int secWeaponType);
		int GetSecWeaponType();
		
		void SetWeaponQuality(int quality);
		int GetWeaponQuality();
		
		void SetSecWeaponQuality(int quality);
		int GetSecWeaponQuality();
		
		void SetCapQuality(int quality);
		int GetCapQuality();
		
		void SetArmorQuality(int quality);
		int GetArmorQuality();
		
		void SetCloakQuality(int quality);
		int GetCloakQuality();
		
		void SetMaxLife(int nMaxLife);
		int GetMaxLife() const { return this->m_nMaxLife; }
		
		void SetMaxMana(int nMaxMana);
		int GetMaxMana() const { return this->m_nMaxMana; }
		
		void SetCamp(CAMP_TYPE btCamp);
		CAMP_TYPE GetCamp() const { return this->m_eCamp; }
		
		void SetFocus(bool bFocus) { m_bFocus = bFocus; }
		bool IsFocus() { return m_bFocus; }
		virtual CGRect GetFocusRect();
		
		int getFlagId(int index);
		
	protected:	
		void SafeClearEffect(NDSprite*& sprite);
		void SafeAddEffect(NDSprite*& sprite, std::string file);
		
		void ShowShadow(bool bShow, bool bBig = false);
		void SetShadowOffset(int iX, int iY);
		void HandleShadow(CGSize parentsize);
		
		void DrawRingImage(bool bDraw);
	public: 
		int m_nID;
		
	public:
		/**����ĸ�������*/
		// lookface������5������
		int sex;	
		int skinColor;		
		int hairColor;
		int hair;
		
		int direct;
		int expresstion;
		int model;
		int weapon;
		int cap;
		int armor;
	
		/*������ɫ��Ϣ*/
		int m_nLife;				//����ֵ
		int m_nMaxLife;			//�������ֵ
		int m_nMana;				//ħ��
		int m_nMaxMana;			//���ħ��ֵ
		int m_nLevel;				//�ȼ�
		
		CAMP_TYPE m_eCamp;			//��Ӫ
		std::string m_name;
		std::string m_strRank;		// ����
	public:
		// ������
		NDRidePet*	m_pkRidePet;
	private:
		bool		m_bFocus;
	public:
		NDNode*     m_pkSubNode; // ��ɫ��������������ڵ㶼��������ڵ���
		CGPoint		m_posScreen;
	protected:
		NDPicture	*m_picRing;
		NDSprite	*m_pkEffectFlagAniGroup;
		NDSprite	*m_pkEffectRidePetAniGroup;
		NDPicture*	m_pkPicShadow;
		NDPicture*	m_pkPicShadowBig;
		int m_iShadowOffsetX;
		int m_iShadowOffsetY;
		bool m_bShowShadow;
		bool m_bBigShadow;
	public:
		static bool ms_bGameSceneRelease;
	};
	
}


#endif // _ND_BASE_ROLE_H