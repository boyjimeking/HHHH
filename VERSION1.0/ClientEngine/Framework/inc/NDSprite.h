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

#include "NDFrame.h"
#include "define.h"
#include "NDConstant.h"
#include "NDBaseFighter.h"

#define FIGHTER_HEIGHT	70 * (NDDirector::DefaultDirector()->GetScaleFactor())
#define FIGHTER_WIDTH	45 * (NDDirector::DefaultDirector()->GetScaleFactor())

class NDAnimationGroup;
class NDSPrite;
class Fighter;

NS_NDENGINE_BGN

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
	} SpriteSex;

	typedef enum
	{
		SpriteSpeedStep4 = 8,
		SpriteSpeedStep6 = 12,
		SpriteSpeedStep8 = 16,
		SpriteSpeedStep10 = 20,
		SpriteSpeedStep12 = 24
	} SpriteSpeed;

	class NDSprite;

	enum
	{
		SUB_ANI_TYPE_SELF = 0,
		SUB_ANI_TYPE_TARGET = 1,
		SUB_ANI_TYPE_NONE = 2,
		SUB_ANI_TYPE_ROLE_FEEDPET = 3,
		SUB_ANI_TYPE_ROLE_WEAPON = 4,
		SUB_ANI_TYPE_SCREEN_CENTER = 5,
	};


class NDSubAniGroup
{
public:
	NDSubAniGroup()
	{
		memset(this, 0L, sizeof(NDSubAniGroup));
	}

	NDSprite* role;
	NDAnimationGroup* aniGroup;
	NDBaseFighter* fighter;
	NDFrameRunRecord* frameRec;

	OBJID idAni;
	short x;
	short y;
	bool reverse;
	short coordW;
	short coordH;
	Byte type;
	int time;	//���Ŵ���
	int antId;	//����ID
	bool bComplete; // �������
	bool isFromOut;
	int startFrame;
	bool isCanStart;//��������ս���е��ӳ�����
	int pos;//����λ��
};

struct NDSubAniGroupEx 
{
	short x;
	short y;

	short coordW;
	short coordH;

	Byte type;

	std::string anifile;
};

enum FACE_DIRECT
{
	FACE_DIRECT_RIGHT = 0,
	FACE_DIRECT_LEFT,
	FACE_DIRECT_UP,
	FACE_DIRECT_DOWN,
};

class ISpriteEvent
{
public:
	virtual void DisplayFrameEvent(int nCurrentAnimation, int nCurrentFrame) =0;
	virtual void DisplayCompleteEvent(int nCurrentAnimation, int nDispCount)=0;
};

//class NDEngine::NDPicture;

class NDSprite: public NDNode
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
	void Initialization(const char* pszSprFile,bool bFaceRight = true);
	void Initlalization(const char* pszSprFile,ISpriteEvent* pkEvent,bool bFaceRight);
//		
//		������OnDrawBegin
//		���ã��÷����ھ������֮ǰ����ܵ���
//		������bDraw�����Ƿ���ʾ����Ļ��
//		����ֵ����
	virtual bool OnDrawBegin(bool bDraw)
	{
		return true;
	}
//		
//		������OnDrawEnd
//		���ã��÷����ھ��������ɺ󱻿�ܵ���
//		������bDraw�����Ƿ���ʾ����Ļ��
//		����ֵ����
	virtual void OnDrawEnd(bool bDraw)
	{
	}
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

	int GetCol()
	{
		return 0;//(m_kPosition.x - DISPLAY_POS_X_OFFSET) / 32.0f;	������� ����
	}
	
	int GetRow()
	{
		return 0;//(m_kPosition.y - DISPLAY_POS_Y_OFFSET) / 32.0f; ������� ����
	}

	int GetCurFrameIndex();

	virtual void OnMoveTurning(bool bXTurningToY,bool bInc){}

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
	virtual void stopMoving(); //{ m_moving = false; }

	virtual void standAction(bool bStand);
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

	virtual void RunBattleSubAnimation(NDBaseFighter* pkFighter);
	virtual bool DrawSubAnimation(NDSubAniGroup& kSag);
	virtual void SetNormalAniGroup(int nLookface);

	virtual void SetWeaponImage(int weapon_lookface);
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

	void SetMoveMap(bool bSet)
	{
		m_bMoveMap = bSet;
	}

	void SetPointList(std::vector<CGPoint>& vec_point)
	{
		m_kPointList = vec_point;
		StartMoving();
	}

	void StartMoving()
	{
		m_bIsMoving = true;
		m_nMovePathIndex = 0;
	}

	void SetNonRole(bool bNonRole)
	{
		this->m_bNonRole = bNonRole;
	}

	bool IsNonRole()
	{
		return this->m_bNonRole;
	}
	bool isMoving()
	{
		return m_bIsMoving;
	}

	void SetPlayFrameRange(int nStartFrame, int nEndFrame);
	void SetHightLight(bool bSet);

	NDFrame* GetCurrentFrame();
public:

	//////////////////////////////////////////////////////////////////////

	virtual cocos2d::CCTexture2D *GetHairImage();
	virtual cocos2d::CCTexture2D *GetFaceImage();
	virtual cocos2d::CCTexture2D *GetExpressionImage();
	virtual cocos2d::CCTexture2D *GetCapImage();
	virtual cocos2d::CCTexture2D *GetArmorImage();
	virtual cocos2d::CCTexture2D *GetRightHandWeaponImage();
	virtual cocos2d::CCTexture2D *GetLeftHandWeaponImage();
	virtual cocos2d::CCTexture2D *GetDoubleHandWeaponImage();
	virtual cocos2d::CCTexture2D *GetDualSwordImage();
	virtual cocos2d::CCTexture2D *GetDualKnifeImage();
	virtual cocos2d::CCTexture2D *GetDoubleHandWandImage();
	virtual cocos2d::CCTexture2D *GetDoubleHandBowImage();
	virtual cocos2d::CCTexture2D *GetShieldImage();
	virtual cocos2d::CCTexture2D *GetFaqiImage();
	virtual cocos2d::CCTexture2D *GetCloakImage();
	virtual cocos2d::CCTexture2D *GetDoubleHandSpearImage();

	virtual cocos2d::CCTexture2D *GetLeftShoulderImage();
	virtual cocos2d::CCTexture2D *GetRightShoulderImage();
	virtual cocos2d::CCTexture2D *GetSkirtStandImage();
	virtual cocos2d::CCTexture2D *GetSkirtWalkImage();
	virtual cocos2d::CCTexture2D *GetSkirtSitImage();
	virtual cocos2d::CCTexture2D *GetSkirtLiftLegImage();

	void AddSubAniGroup(NDSubAniGroupEx& kGroup);

	int GetWidth();
	int GetHeight();

	cocos2d::CCTexture2D* getColorTexture(int imageIndex,
			NDAnimationGroup* animationGroup);
	cocos2d::CCTexture2D* getNpcLookfaceTexture(int imageIndex,
			NDAnimationGroup* animationGroup);
	cocos2d::CCTexture2D* getArmorImageByCloak()
	{
		return m_nCloak == -1 ? GetArmorImage() : NULL;
	}

	bool IsCloakEmpty()
	{
		return m_nCloak == -1 ? true : false;
	}

	virtual void BeforeRunAnimation(bool bDraw)
	{
	}

#if 1
	virtual void RunAnimation(bool bDraw);
private:
	void RunAnimation_WithFrames(bool bDraw);
	void RunAnimation_WithOnePic(bool bDraw);
	bool TickAnim();
#endif

public:
	CGRect GetSpriteRect();
	void SetCurrentAnimation(int nAnimationIndex, bool bReverse);

	void SetSpriteDir(int dir)
	{
		dir == 2 ? (m_bFaceRight = m_bReverse = false) : (m_bFaceRight =
							m_bReverse = true);
	}
	int getGravityY();
	int getGravityX();
	void SetScale(float s)
	{
		m_fScale = s;
	}
	float GetScale()
	{
		return m_fScale;
	}
	bool GetLastPointOfPath(CGPoint& pos);
	bool IsReverse()
	{
		return m_bReverse;
	}

protected:

	bool MoveByPath( const bool bFirstPath = false );
	void MoveToPosition(std::vector<CGPoint> toPos, SpriteSpeed speed,
			bool moveMap, bool ignoreMask = false, bool mustArrive = false);
	virtual void OnMoveBegin();
	virtual void OnMoveEnd();
	virtual void OnMoving(bool bLastPos);
	void SetSprite(NDPicture* pic);
	void reloadAni(const char* pszSprFile);

protected:

	std::string m_strHairImage;
	std::string m_strFaceImage;
	std::string m_strExpressionImage;
	std::string m_strCapImage;
	std::string m_strArmorImage;
	std::string m_strRightHandWeaponImage;
	std::string m_strLeftHandWeaponImage;
	std::string m_strDoubleHandWeaponImage;
	std::string m_strDualSwordImage;
	std::string m_strDualKnifeImage;
	std::string m_strDoubleHandWandImage;
	std::string m_strDoubleHandBowImage;
	std::string m_strShieldImage;
	std::string m_strFaqiImage;
	std::string m_strCloakImage;
	std::string m_strDoubleHandSpearImage;
	std::string m_strLeftShoulderImage;
	std::string m_strRightShoulderImage;
	std::string m_strSkirtStandImage;
	std::string m_strSkirtWalkImage;
	std::string m_strSkirtSitImage;
	std::string m_strSkirtLiftLegImage;
	std::string m_strColorInfoImage;

	int m_nMasterWeaponType;
	int m_nSecondWeaponType;
	int m_nMasterWeaponQuality;
	int m_nSecondWeaponQuality;
	int m_nCapQuality;
	int m_nArmorQuality;
	int m_nCloakQuality;

	CGPoint m_kPosition;
	NDAnimation* m_pkCurrentAnimation;
	NDFrameRunRecord* m_pkFrameRunRecord;
	NDAnimationGroup* m_pkAniGroup;

	bool m_bReverse;
	bool m_bMoveMap;
	bool m_bIsMoving;

	int m_nMovePathIndex;
	int m_nNPCLookface;
	int m_nColorInfo;
	int m_nCloak;
	int m_nSpeed;
	DWORD m_dwLastMoveTickTime;

	std::vector<CGPoint> m_kPointList;
	CGPoint m_kTargetPos;

	bool m_bNonRole;
	float m_fScale;
public:
	bool m_bFaceRight;		// �����沿����
private:
	NDPicture* m_pkPicSprite;
	CGRect m_kRectSprite;
	bool m_bHightLight;
	ISpriteEvent* m_pkSpriteEvent;
	NSTimeInterval m_dBeginTime;
};

NS_NDENGINE_END

#endif
