//
//  NDSprite.h
//  DragonDrive
//
//  Created by xiezhenghai on 10-12-11.
//  Copyright 2010 (����)DeNA. All rights reserved.
//

#ifndef __NDSprite_H
#define __NDSprite_H

#include "NDNode.h"
#include <string>
#include <vector>
#include "CCTexture2D.h"
#include "NDAnimation.h"

#include "NDPicture.h"
// #include "basedefine.h"
// #include "NDConstant.h"

//class Fighter;

#include "NDFrame.h"

class NDAnimationGroup;

namespace NDEngine 
{	

#define		WEAPON_NONE				0
#define		ONE_HAND_WEAPON			6
#define		TWO_HAND_WEAPON			5
#define		DUAL_SWORD				1
#define		TWO_HAND_KNIFE			4
#define		DUAL_KNIFE				8
#define		TWO_HAND_WAND			3
#define		TWO_HAND_BOW			2
#define		TWO_HAND_SPEAR			7
#define		SEC_SHIELD				9
#define		SEC_FAQI				10
	
	
	typedef enum 
	{
		SpriteSexNone,
		SpriteSexMale,
		SpriteSexFemale,
		SpriteSexDynamic
	}SpriteSex;	
	
	typedef enum
	{
		SpriteSpeedStep4 = 8,
		SpriteSpeedStep8 = 16
	}SpriteSpeed;
	
	
	class NDEngine::NDPicture;

	class NDSprite : public NDNode 
	{
		DECLARE_CLASS(NDSprite)
	public:

		NDSprite();			
		~NDSprite();

	public:
//		
//		������Initialization
//		���ã���ʼ�����飬���뱻��ʾ����ʽ����
//		������sprFile�����ļ���ÿһ��������Ҫ��һ�������ļ���
//		����ֵ����
		void Initialization(const char* sprFile); hide
//		
//		������OnDrawBegin
//		���ã��÷����ھ������֮ǰ����ܵ���
//		������bDraw�����Ƿ���ʾ����Ļ��
//		����ֵ����
		virtual bool OnDrawBegin(bool bDraw){ return true; };
//		
//		������OnDrawEnd
//		���ã��÷����ھ��������ɺ󱻿�ܵ���
//		������bDraw�����Ƿ���ʾ����Ļ��
//		����ֵ����
		virtual void OnDrawEnd(bool bDraw){};
//		
//		������SetPosition
//		���ã����þ����ڵ�ͼ�ϵ�����
//		������newPosition��ͼ����
//		����ֵ����
		virtual void SetPosition(CGPoint newPosition);
//		
//		������GetPosition
//		���ã���ȡ�����ڵ�ͼ�ϵ�����
//		��������
//		����ֵ����ͼ����
		CGPoint GetPosition();
		
// 		int GetCol() {
// 			return (this->m_position.x - DISPLAY_POS_X_OFFSET) / 32.0f;
// 		}
// 		
// 		int GetRow() {
// 			return (this->m_position.y - DISPLAY_POS_Y_OFFSET) / 32.0f;
// 		}
		
		int GetCurFrameIndex();
		
//		
//		������OnMoveTurning
//		���ã�
//		������
//		����ֵ��		
//		virtual void OnMoveTurning(bool bXTurnigToY, bool bInc){};
//		
//		������stopMoving
//		���ã�ֹͣ�����ڵ�ͼ�ϵ��ƶ����������ʽֹͣ��û���κ�Ӱ��
//		��������
//		����ֵ����		
		virtual void stopMoving();//{ m_moving = false; }
//		
//		������GetOrder
//		���ã���ȡ�������������ֵ
//		��������
//		����ֵ����������ֵ
		virtual int GetOrder();
//		
//		������IsAnimationComplete
//		���ã��жϾ������󶨵Ķ����Ƿ񲥷���һ��
//		��������
//		����ֵ��true�ǣ�false��
		bool IsAnimationComplete();
//		
//		������SetHairImage
//		���ã�����ͷ��ͼƬ
//		������imageFileͼƬ�ļ�
//		����ֵ����	
		virtual void SetHairImage(const char* imageFile, int colorIndex);
//		
//		������SetFaceImage
//		���ã������沿ͼƬ
//		������imageFileͼƬ�ļ�
//		����ֵ����
		virtual void SetFaceImage(const char* imageFile);
//		
//		������SetExpressionImage
//		���ã����ñ���ͼƬ
//		������imageFileͼƬ�ļ�
//		����ֵ����
		virtual void SetExpressionImage(const char* imageFile);
//		
//		������SetCapImage
//		���ã�����ͷ��ͼƬ
//		������imageFileͼƬ�ļ�
//		����ֵ��
		virtual void SetCapImage(const char* imageFile);
//		
//		������SetArmorImage
//		���ã����ÿ���ͼƬ
//		������imageFileͼƬ�ļ�
//		����ֵ����
		virtual void SetArmorImage(const char* imageFile);
//		
//		������SetRightHandWeaponImage
//		���ã�������������ͼƬ
//		������imageFileͼƬ�ļ�
//		����ֵ����
		virtual void SetRightHandWeaponImage(const char* imageFile);
//		
//		������SetLeftHandWeaponImage
//		���ã�������������ͼƬ
//		������imageFileͼƬ�ļ�
//		����ֵ����
		virtual void SetLeftHandWeaponImage(const char* imageFile);
//		
//		������SetDoubleHandWeaponImage
//		���ã�����˫������ͼƬ
//		������imageFileͼƬ�ļ�
//		����ֵ����
		virtual void SetDoubleHandWeaponImage(const char* imageFile);
//		
//		������SetDualSwordImage
//		���ã�
//		������imageFileͼƬ�ļ�
//		����ֵ����
		virtual void SetDualSwordImage(const char* imageFile);
//		
//		������
//		���ã�
//		������imageFileͼƬ�ļ�
//		����ֵ����
		virtual void SetDualKnifeImage(const char* imageFile);
//		
//		������
//		���ã�
//		������imageFileͼƬ�ļ�
//		����ֵ����
		virtual void SetDoubleHandWandImage(const char* imageFile);
//		
//		������
//		���ã�
//		������imageFileͼƬ�ļ�
//		����ֵ����
		virtual void SetDoubleHandBowImage(const char* imageFile);
//		
//		������
//		���ã�
//		������imageFileͼƬ�ļ�
//		����ֵ����
		virtual void SetShieldImage(const char* imageFile);
//		
//		������
//		���ã�
//		������imageFileͼƬ�ļ�
//		����ֵ����
		virtual void SetFaqiImage(const char* imageFile);
//		
//		������
//		���ã�
//		������imageFileͼƬ�ļ�
//		����ֵ����
		virtual void SetCloakImage(const char* imageFile);
//		
//		������
//		���ã�
//		������imageFileͼƬ�ļ�
//		����ֵ����
		virtual void SetDoubleHandSpearImage(const char* imageFile);
//		
//		������
//		���ã�
//		������imageFileͼƬ�ļ�
//		����ֵ����
		virtual void SetLeftShoulderImage(const char* imageFile);
//		
//		������
//		���ã�
//		������imageFileͼƬ�ļ�
//		����ֵ����
		virtual void SetRightShoulderImage(const char* imageFile);
//		
//		������
//		���ã�
//		������imageFileͼƬ�ļ�
//		����ֵ����
		virtual void SetSkirtStandImage(const char* imageFile);
//		
//		������
//		���ã�
//		������imageFileͼƬ�ļ�
//		����ֵ����
		virtual void SetSkirtWalkImage(const char* imageFile);
//		
//		������
//		���ã�
//		������imageFileͼƬ�ļ�
//		����ֵ����
		virtual void SetSkirtSitImage(const char* imageFile);
//		
//		������
//		���ã�
//		������imageFileͼƬ�ļ�
//		����ֵ����
		virtual void SetSkirtLiftLegImage(const char* imageFile);
		
		void SetMoveMap(bool bSet) { m_moveMap = bSet; }
		
		void SetPointList(std::vector<CGPoint>& vec_point){ m_pointList = vec_point; StartMoving(); }
		
		void StartMoving(){ m_moving = true; m_movePathIndex = 0; }
		
		void SetNonRole(bool bNonRole) { this->m_bNonRole = bNonRole; }
		
		bool IsNonRole() { return this->m_bNonRole; }
		
		bool isMoving(){ return m_moving; }
		
		void SetPlayFrameRange(int nStartFrame, int nEndFrame);
		
		void SetHightLight(bool bSet);

		NDFrame* GetCurrentFrame();
	public:
		
		//////////////////////////////////////////////////////////////////////
		
		virtual cocos2d::CCTexture2D	*GetHairImage();
		virtual cocos2d::CCTexture2D	*GetFaceImage();
		virtual cocos2d::CCTexture2D	*GetExpressionImage();
		virtual cocos2d::CCTexture2D	*GetCapImage();
		virtual cocos2d::CCTexture2D	*GetArmorImage();
		virtual cocos2d::CCTexture2D	*GetRightHandWeaponImage();
		virtual cocos2d::CCTexture2D	*GetLeftHandWeaponImage();
		virtual cocos2d::CCTexture2D	*GetDoubleHandWeaponImage();
		virtual cocos2d::CCTexture2D	*GetDualSwordImage();
		virtual cocos2d::CCTexture2D	*GetDualKnifeImage();
		virtual cocos2d::CCTexture2D	*GetDoubleHandWandImage();
		virtual cocos2d::CCTexture2D	*GetDoubleHandBowImage();
		virtual cocos2d::CCTexture2D	*GetShieldImage();
		virtual cocos2d::CCTexture2D	*GetFaqiImage();
		virtual cocos2d::CCTexture2D *GetCloakImage();
		virtual cocos2d::CCTexture2D	*GetDoubleHandSpearImage();
		
		virtual cocos2d::CCTexture2D	*GetLeftShoulderImage();
		virtual cocos2d::CCTexture2D	*GetRightShoulderImage();
		virtual cocos2d::CCTexture2D	*GetSkirtStandImage();
		virtual cocos2d::CCTexture2D	*GetSkirtWalkImage();
		virtual cocos2d::CCTexture2D	*GetSkirtSitImage();
		virtual cocos2d::CCTexture2D	*GetSkirtLiftLegImage();
		
		int GetWidth();
		int GetHeight();
		
		cocos2d::CCTexture2D* getColorTexture(int imageIndex, NDAnimationGroup* animationGroup);
		cocos2d::CCTexture2D* getNpcLookfaceTexture(int imageIndex, NDAnimationGroup* animationGroup);
		cocos2d::CCTexture2D* getArmorImageByCloak(){ return cloak == -1 ? GetArmorImage() : NULL; }
		
		bool IsCloakEmpty(){ return cloak == -1 ? true : false; }
		
		virtual void BeforeRunAnimation(bool bDraw) {}
		void RunAnimation(bool bDraw);
		CGRect GetSpriteRect();		
		void SetCurrentAnimation(int animationIndex, bool reverse);
		
		void SetSpriteDir(int dir){ dir == 2 ? (m_faceRight = m_reverse = false) : (m_faceRight = m_reverse = true);}
		int getGravityY();
		int getGravityX();
		void SetScale(float s){scale = s;}
		float GetScale(){return scale;}
		bool GetLastPointOfPath(CGPoint& pos);
		bool IsReverse() { return m_reverse; }
	protected:
		void MoveToPosition(std::vector<CGPoint> toPos, SpriteSpeed speed, bool moveMap, bool ignoreMask=false, bool mustArrive=false);
		virtual void OnMoveBegin();
		virtual void OnMoveEnd();
		virtual void OnMoving(bool bLastPos);
		void SetSprite(NDPicture* pic);
		
	protected:

		std::string m_hairImage, m_faceImage, m_expressionImage, m_capImage, m_armorImage, m_rightHandWeaponImage, m_leftHandWeaponImage,
		m_doubleHandWeaponImage, m_dualSwordImage, m_dualKnifeImage, m_doubleHandWandImage, m_doubleHandBowImage, m_shieldImage, 
		m_faqiImage, m_cloakImage, m_doubleHandSpearImage;		
		std::string m_leftShoulderImage, m_rightShoulderImage, m_skirtStandImage, m_skirtWalkImage,
		m_skirtSitImage, m_skirtLiftLegImage;
		std::string colorInfoImage;
		
		int m_weaponType, m_secWeaponType, m_weaponQuality, m_secWeaponQuality, m_capQuality, m_armorQuality, m_cloakQuality;
		CGPoint m_position;		
		NDAnimation *m_currentAnimation;
		NDFrameRunRecord *m_frameRunRecord;
		NDAnimationGroup *m_aniGroup;
		bool m_reverse;
		bool m_moveMap;
		bool m_moving;
		int m_movePathIndex;
		
		int npcLookface;
		int colorInfo;
		int cloak;
		
		std::vector<CGPoint> m_pointList;
		int m_iSpeed;
		CGPoint m_targetPos;
		
		bool m_bNonRole;
		float scale;
	public:
		bool m_faceRight;		// �����沿����
	private:
		NDPicture *m_picSprite;
		CGRect m_rectSprite;
		bool m_bHightLight;
	};
}

#endif
