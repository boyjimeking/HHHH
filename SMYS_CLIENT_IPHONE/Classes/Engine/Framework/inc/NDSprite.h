//
//  NDSprite.h
//  DragonDrive
//
//  Created by xiezhenghai on 10-12-11.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#ifndef __NDSprite_H
#define __NDSprite_H

#include "NDNode.h"
#include <string>
#include <vector>
#import "CCTexture2D.h"
#import "NDAnimation.h"

#import "NDPicture.h"
#include "define.h"
#include "NDConstant.h"

class Fighter;

@class NDAnimationGroup;
@class NDFrame;
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
	
	class NDSprite;
	
	enum {
		SUB_ANI_TYPE_SELF = 0,
		SUB_ANI_TYPE_TARGET = 1,
		SUB_ANI_TYPE_NONE = 2,
		SUB_ANI_TYPE_ROLE_FEEDPET = 3,
		SUB_ANI_TYPE_ROLE_WEAPON = 4,
		SUB_ANI_TYPE_SCREEN_CENTER = 5,
	};
	
	struct NDSubAniGroup {
		
		NDSubAniGroup() {
			memset(this, 0L, sizeof(NDSubAniGroup));
		}
		
		NDSprite* role;
		NDAnimationGroup* aniGroup;
		Fighter* fighter;
		NDFrameRunRecord* frameRec;
		
		OBJID idAni;
		short x;
		short y;
		bool reverse;
		short coordW;
		short coordH;
		Byte type;
		int time;	//播放次数
		int antId;	//动作ID
		bool bComplete; // 播放完成
		bool isFromOut;
		int startFrame;
		bool isCanStart;//用来控制战斗中的延迟问题
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
	
	class NDPicture;
	
	class NDSprite : public NDNode 
	{
		DECLARE_CLASS(NDSprite)
	public:
		NDSprite();			
		~NDSprite();
		
	public:
//		
//		函数：Initialization
//		作用：初始化精灵，必须被显示或隐式调用
//		参数：sprFile动画文件，每一个精灵需要与一个动画文件绑定
//		返回值：无
		void Initialization(const char* sprFile); hide
//		
//		函数：OnDrawBegin
//		作用：该方法在精灵绘制之前被框架调用
//		参数：bDraw精灵是否显示在屏幕上
//		返回值：无
		virtual bool OnDrawBegin(bool bDraw){ return true; };
//		
//		函数：OnDrawEnd
//		作用：该方法在精灵绘制完成后被框架调用
//		参数：bDraw精灵是否显示在屏幕上
//		返回值：无
		virtual void OnDrawEnd(bool bDraw){};
//		
//		函数：SetPosition
//		作用：设置精灵在地图上的坐标
//		参数：newPosition地图坐标
//		返回值：无
		virtual void SetPosition(CGPoint newPosition);
//		
//		函数：GetPosition
//		作用：获取精灵在地图上的坐标
//		参数：无
//		返回值：地图坐标
		CGPoint GetPosition();
		
		int GetCol() {
			return (this->m_position.x - DISPLAY_POS_X_OFFSET) / 32.0f;
		}
		
		int GetRow() {
			return (this->m_position.y - DISPLAY_POS_Y_OFFSET) / 32.0f;
		}
		
		int GetCurFrameIndex();
		
//		
//		函数：OnMoveTurning
//		作用：
//		参数：
//		返回值：		
		virtual void OnMoveTurning(bool bXTurnigToY, bool bInc){};
//		
//		函数：stopMoving
//		作用：停止精灵在地图上的移动，如果精灵式停止的没有任何影响
//		参数：无
//		返回值：无		
		virtual void stopMoving();//{ m_moving = false; }
//		
//		函数：GetOrder
//		作用：获取精灵的排序重心值
//		参数：无
//		返回值：排序重心值
		virtual int GetOrder();
//		
//		函数：IsAnimationComplete
//		作用：判断精灵所绑定的动画是否播放完一遍
//		参数：无
//		返回值：true是，false否
		bool IsAnimationComplete();
//		
//		函数：RunBattleSubAnimation
//		作用：播放战斗子动画
//		参数：f战斗对象
//		返回值：无
		void RunBattleSubAnimation(Fighter* f);
		
		//
		// 作用：播放子动画
		// 参数：key: 用于获取子动画的 frameRecord; sag:子动画信息
		// 返回值：子动画是否播放完成
		bool DrawSubAnimation(NDSubAniGroup& sag);
//		
//		函数：SetNormalAniGroup
//		作用：设置默认动画组
//		参数：lookface服务器下发的标识，用于精灵的表现
//		返回值：无		
		void SetNormalAniGroup(int lookface);
//		
//		函数：SetHairImage
//		作用：设置头发图片
//		参数：imageFile图片文件
//		返回值：无	
		virtual void SetHairImage(const char* imageFile, int colorIndex);
//		
//		函数：SetFaceImage
//		作用：设置面部图片
//		参数：imageFile图片文件
//		返回值：无
		virtual void SetFaceImage(const char* imageFile);
//		
//		函数：SetExpressionImage
//		作用：设置表情图片
//		参数：imageFile图片文件
//		返回值：无
		virtual void SetExpressionImage(const char* imageFile);
//		
//		函数：SetCapImage
//		作用：设置头盔图片
//		参数：imageFile图片文件
//		返回值：
		virtual void SetCapImage(const char* imageFile);
//		
//		函数：SetArmorImage
//		作用：设置盔甲图片
//		参数：imageFile图片文件
//		返回值：无
		virtual void SetArmorImage(const char* imageFile);
//		
//		函数：SetRightHandWeaponImage
//		作用：设置右手武器图片
//		参数：imageFile图片文件
//		返回值：无
		virtual void SetRightHandWeaponImage(const char* imageFile);
//		
//		函数：SetLeftHandWeaponImage
//		作用：设置左手武器图片
//		参数：imageFile图片文件
//		返回值：无
		virtual void SetLeftHandWeaponImage(const char* imageFile);
//		
//		函数：SetDoubleHandWeaponImage
//		作用：设置双手武器图片
//		参数：imageFile图片文件
//		返回值：无
		virtual void SetDoubleHandWeaponImage(const char* imageFile);
//		
//		函数：SetDualSwordImage
//		作用：
//		参数：imageFile图片文件
//		返回值：无
		virtual void SetDualSwordImage(const char* imageFile);
//		
//		函数：
//		作用：
//		参数：imageFile图片文件
//		返回值：无
		virtual void SetDualKnifeImage(const char* imageFile);
//		
//		函数：
//		作用：
//		参数：imageFile图片文件
//		返回值：无
		virtual void SetDoubleHandWandImage(const char* imageFile);
//		
//		函数：
//		作用：
//		参数：imageFile图片文件
//		返回值：无
		virtual void SetDoubleHandBowImage(const char* imageFile);
//		
//		函数：
//		作用：
//		参数：imageFile图片文件
//		返回值：无
		virtual void SetShieldImage(const char* imageFile);
//		
//		函数：
//		作用：
//		参数：imageFile图片文件
//		返回值：无
		virtual void SetFaqiImage(const char* imageFile);
//		
//		函数：
//		作用：
//		参数：imageFile图片文件
//		返回值：无
		virtual void SetCloakImage(const char* imageFile);
//		
//		函数：
//		作用：
//		参数：imageFile图片文件
//		返回值：无
		virtual void SetDoubleHandSpearImage(const char* imageFile);
//		
//		函数：
//		作用：
//		参数：imageFile图片文件
//		返回值：无
		virtual void SetLeftShoulderImage(const char* imageFile);
//		
//		函数：
//		作用：
//		参数：imageFile图片文件
//		返回值：无
		virtual void SetRightShoulderImage(const char* imageFile);
//		
//		函数：
//		作用：
//		参数：imageFile图片文件
//		返回值：无
		virtual void SetSkirtStandImage(const char* imageFile);
//		
//		函数：
//		作用：
//		参数：imageFile图片文件
//		返回值：无
		virtual void SetSkirtWalkImage(const char* imageFile);
//		
//		函数：
//		作用：
//		参数：imageFile图片文件
//		返回值：无
		virtual void SetSkirtSitImage(const char* imageFile);
//		
//		函数：
//		作用：
//		参数：imageFile图片文件
//		返回值：无
		virtual void SetSkirtLiftLegImage(const char* imageFile);
		
		void SetMoveMap(bool bSet) { m_moveMap = bSet; }
		
		void SetPointList(std::vector<CGPoint>& vec_point){ m_pointList = vec_point; StartMoving(); }
		
		void StartMoving(){ m_moving = true; m_movePathIndex = 0; }
		
		void SetNonRole(bool bNonRole) { this->m_bNonRole = bNonRole; }
		
		bool IsNonRole() { return this->m_bNonRole; }
		
		void AddSubAniGroup(NDSubAniGroupEx& group);
		
		bool isMoving(){ return m_moving; }
		
		void SetPlayFrameRange(int nStartFrame, int nEndFrame);
		
		void SetHightLight(bool bSet);
	public:
		
		//////////////////////////////////////////////////////////////////////
		
		virtual CCTexture2D	*GetHairImage();
		virtual CCTexture2D	*GetFaceImage();
		virtual CCTexture2D	*GetExpressionImage();
		virtual CCTexture2D	*GetCapImage();
		virtual CCTexture2D	*GetArmorImage();
		virtual CCTexture2D	*GetRightHandWeaponImage();
		virtual CCTexture2D	*GetLeftHandWeaponImage();
		virtual CCTexture2D	*GetDoubleHandWeaponImage();
		virtual CCTexture2D	*GetDualSwordImage();
		virtual CCTexture2D	*GetDualKnifeImage();
		virtual CCTexture2D	*GetDoubleHandWandImage();
		virtual CCTexture2D	*GetDoubleHandBowImage();
		virtual CCTexture2D	*GetShieldImage();
		virtual CCTexture2D	*GetFaqiImage();
		virtual CCTexture2D *GetCloakImage();
		virtual CCTexture2D	*GetDoubleHandSpearImage();
		
		virtual CCTexture2D	*GetLeftShoulderImage();
		virtual CCTexture2D	*GetRightShoulderImage();
		virtual CCTexture2D	*GetSkirtStandImage();
		virtual CCTexture2D	*GetSkirtWalkImage();
		virtual CCTexture2D	*GetSkirtSitImage();
		virtual CCTexture2D	*GetSkirtLiftLegImage();
		
		int GetWidth();
		int GetHeight();
		
		CCTexture2D* getColorTexture(int imageIndex, NDAnimationGroup* animationGroup);
		
		CCTexture2D* getNpcLookfaceTexture(int imageIndex, NDAnimationGroup* animationGroup);
		
		CCTexture2D* getArmorImageByCloak(){ return cloak == -1 ? GetArmorImage() : NULL; }
		
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
		NSString *m_hairImage, *m_faceImage, *m_expressionImage, *m_capImage, *m_armorImage, *m_rightHandWeaponImage, *m_leftHandWeaponImage,
		*m_doubleHandWeaponImage, *m_dualSwordImage, *m_dualKnifeImage, *m_doubleHandWandImage, *m_doubleHandBowImage, *m_shieldImage, 
		*m_faqiImage, *m_cloakImage, *m_doubleHandSpearImage;		
		NSString *m_leftShoulderImage, *m_rightShoulderImage, *m_skirtStandImage, *m_skirtWalkImage,
		*m_skirtSitImage, *m_skirtLiftLegImage;
		NSString *colorInfoImage;
		
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
		bool m_faceRight;		// 精灵面部朝向
	private:
		NDPicture *m_picSprite;
		CGRect m_rectSprite;
		bool m_bHightLight;
	};
}

#endif
