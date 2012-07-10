//
//  NDMapLayer.h
//  DragonDrive
//
//  Created by xiezhenghai on 10-12-10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//
//	－－介绍－－
//	地图层是游戏中主要的层，游戏中的人物活动都在该层上


#ifndef	__NDMapLayer_H
#define __NDMapLayer_H

#include "NDLayer.h"
#include <string>
#include "NDPicture.h"

#include "NDMapData.h"
#include "NDAnimationGroup.h"
#include "CCLayer.h"
#include "NDUILayer.h"
#include "NDLightEffect.h"
#include "NDUIImage.h"
#include "UISpriteNode.h"

#define blockTimerTag	(5)
#define titleTimerTag	(6)

enum MAP_SWITCH_TYPE {
	SWITCH_NONE = 0,
	SWITCH_TO_BATTLE,
	SWITCH_BACK_FROM_BATTLE,
};

enum BOX_STATUS{
	BOX_NONE=0,
	BOX_SHOWING,
	BOX_CLOSE,
	BOX_OPENING,
	BOX_OPENED,
};
namespace NDEngine 
{
	class NDMapLayer : public NDLayer,public ITimerCallback
	{
		DECLARE_CLASS(NDMapLayer)
	public:
		class MAP_ORDER : public CCObject, public std::map<std::string, int>{};
	public:
		NDMapLayer();
		~NDMapLayer();
	public:
//		
//		函数：Initialization(const char* mapFile)
//		作用：初始化地图
//		参数：mapFile地图文件
//		返回值：无
		void Initialization(const char* mapFile); hide overload
//		
//		函数：Initialization(int mapIndex)
//		作用：初始化地图
//		参数：mapIndex地图索引号
//		返回值：无
		void Initialization(int mapIndex); hide overload
		
		int GetMapIndex();
//		
//		函数：DidFinishLaunching
//		作用：地图初始化完成后由框架回调该方法
//		参数：无
//		返回值：无
		virtual void DidFinishLaunching();
//		
//		同基类
		bool TouchBegin(NDTouch* touch); override
//		
//		同基类
		void TouchEnd(NDTouch* touch); override
//		
//		同基类
		void TouchCancelled(NDTouch* touch); override
//		
//		同基类
		void TouchMoved(NDTouch* touch); override		
//		
//		函数：ConvertToMapPoint
//		作用：将屏幕坐标转化成地图坐标
//		参数：screenPoint屏幕坐标
//		返回值：地图坐标
		CGPoint ConvertToMapPoint(CGPoint screenPoint);
//		
//		函数：isMapPointInScreen
//		作用：判断地图坐标点是否在屏幕范围内
//		参数：mapPoint地图坐标
//		返回值：true是，false否
		bool isMapPointInScreen(CGPoint mapPoint);
//		
//		函数：isMapRectIntersectScreen
//		作用：判断地图上的矩形区域是否在屏幕范围内
//		参数：mapRect地图上的矩形区域
//		返回值：true是，false否
		bool isMapRectIntersectScreen(CGRect mapRect);
//		
//		函数：SetScreenCenter
//		作用：设置地图坐标点对应屏幕中心；如果超出地图的表现范围，则该点并不对应于屏幕的中心
//		参数：p地图坐标点
//		返回值：true 到边界了, false 还没到边界
		bool SetScreenCenter(CGPoint p);
//		
//		函数：GetScreenCenter
//		作用：获取屏幕中心点的地图坐标
//		参数：无
//		返回值：地图坐标
		CGPoint GetScreenCenter();
//		
//		函数：SetBattleBackground
//		作用：设置进入战斗
//		参数：bBattleBackground如果为true就进入战斗，否则退出战斗
//		返回值：无
		void SetBattleBackground(bool bBattleBackground);
		bool IsBattleBackground(){ return m_bBattleBackground;}
		
		void SetNeedShowBackground(bool bNeedShow);
		
		void MapSwitchRefresh();
	
		void ShowRoadSign(bool bShow, int nX = 0, int nY = 0);
	public:
		virtual void draw();
		NDMapData *GetMapData();
		void setStartRoadBlockTimer(int time,int x,int y);
		void setAutoBossFight(bool isAuto);
		bool isAutoFight() {return isAutoBossFight;}
		void walkToBoss();
		void OnTimer(OBJID tag);
		void replaceMapData(int mapId,int center_x,int center_y);
		void ShowTitle(int name_col,int name_row);
		void refreshTitle();
		void showSwitchSprite(MAP_SWITCH_TYPE type);
		void ShowTreasureBox();
		void OpenTreasureBox();
		bool isTouchTreasureBox(CGPoint touchPoint);
	//		void setRoadBlock(int x,int y){roadBlockX=x;roadBlockY=y;}
	private:
		void MakeOrders();
		void MakeOrdersOfMapscenesAndMapanimations();
		int InsertIndex(int order, NSArray *inArray);
		void QuickSort(NSMutableArray *array, int low, int high);
		int Partition(NSMutableArray *array, int low, int high);
		
		void SetPosition(CGPoint p);
		
		void DrawMapTiles();
		void DrawScenesAndAnimations();
		void DrawBgs();

		void MakeFrameRunRecords();
		
		void ReflashMapTexture(CGPoint oldScreenCenter, CGPoint newScreenCenter);
		void ReplaceMapTexture(CCTexture2D* tex, CGRect replaceRect, CGRect tilesRect);	
		void ScrollSplit(int x, int y);
		void ScrollVertical(int y, CGRect newRect);
		void ScrollHorizontal(int x, CGRect newRect);
		void RefreshBoxAnimation();
	private:
		CGPoint m_screenCenter;
		CCMutableArray<MAP_ORDER*> *m_orders;
		CCMutableArray<MAP_ORDER*> *m_ordersOfMapscenesAndMapanimations;
		NDMapData *m_mapData;
		CCMutableArray< CCMutableArray<NDFrameRunRecord*>* > *m_frameRunRecordsOfMapAniGroups;
		CCMutableArray< NDFrameRunRecord* > *m_frameRunRecordsOfMapSwitch;
		NDAnimationGroup *m_switchAniGroup;
		NDSprite *m_TreasureBox;
		NDUILabel *m_lbTime;
		NDUIImage *m_lbTitle;
		NDUIImage *m_lbTitleBg;
		CUISpriteNode* switchSpriteNode;
		bool m_bBattleBackground;
		bool m_bNeedShow;
		NDTimer* m_ndBlockTimer;
		NDTimer* m_ndTitleTimer;
		int roadBlockTimeCount;
		bool isAutoBossFight;
		//CCColorLayer* m_battleBgColor;
		int titleAlpha;
		CCTexture2D *m_texMap;
		NDPicture *m_picMap;
		CGPoint m_ptCamarkSplit;
		NDNode		*subnode; 
		BOX_STATUS box_status;
		typedef enum
		{
			IntersectionAreaLT,
			IntersectionAreaLB,
			IntersectionAreaRT,
			IntersectionAreaRB,
			IntersectionAreaNone
		}IntersectionArea;
		void RectIntersectionRect(CGRect rect1, CGRect rect2, CGRect& intersectionRect, IntersectionArea& intersectionArea);
		IntersectionArea m_areaCamarkSplit;
		
		int m_mapIndex;
		
		bool showTitle;
		MAP_SWITCH_TYPE switch_type;
		
		NDLightEffect* m_leRoadSign;
	};
}

#endif
