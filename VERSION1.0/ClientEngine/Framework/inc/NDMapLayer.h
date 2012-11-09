//
//  NDMapLayer.h
//  DragonDrive
//
//  Created by xiezhenghai on 10-12-10.
//  Copyright 2010 (����)DeNA. All rights reserved.
//
//	�������ܣ���
//	��ͼ������Ϸ����Ҫ�Ĳ㣬��Ϸ�е��������ڸò���

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
#include "NDTimer.h"
#include "UISpriteNode.h"

#define blockTimerTag	(5)
#define titleTimerTag	(6)
#define switch_ani_modelId		(106)	//���͵㶯����model ID
#define ZORDER_MASK_ANI			(999)	//���ֶ�����Z������game scene��

enum MAP_SWITCH_TYPE
{
	SWITCH_NONE = 0,
	SWITCH_TO_BATTLE,
	SWITCH_BACK_FROM_BATTLE,
	SWITCH_START_BATTLE
};

enum BOX_STATUS
{
	BOX_NONE = 0,
	BOX_SHOWING,
	BOX_CLOSE,
	BOX_OPENING,
	BOX_OPENED,
};

namespace NDEngine
{
class NDMapLayer: public NDLayer,public ITimerCallback
{
DECLARE_CLASS(NDMapLayer)
public:
	class MAP_ORDER:
		public cocos2d::CCObject,
		public std::map<std::string, int>
	{
	};
public:
	NDMapLayer();
	~NDMapLayer();
public:

	virtual void AddChild(NDNode* node, int z, int tag);
//		
//		������Initialization(const char* mapFile)
//		���ã���ʼ����ͼ
//		������mapFile��ͼ�ļ�
//		����ֵ����
	void Initialization(const char* mapFile);
//		
//		������Initialization(int mapIndex)
//		���ã���ʼ����ͼ
//		������mapIndex��ͼ������
//		����ֵ����
	void Initialization(int mapIndex);

	int GetMapIndex();
//		
//		������DidFinishLaunching
//		���ã���ͼ��ʼ����ɺ��ɿ�ܻص��÷���
//		��������
//		����ֵ����
	virtual void DidFinishLaunching();
//		
//		ͬ����
	//bool TouchBegin(NDTouch* touch); override
//		
//		ͬ����
	//void TouchEnd(NDTouch* touch); override
//		
//		ͬ����
	//void TouchCancelled(NDTouch* touch); override
//		
//		ͬ����
	//void TouchMoved(NDTouch* touch); override
//		
//		������ConvertToMapPoint
//		���ã�����Ļ����ת���ɵ�ͼ����
//		������screenPoint��Ļ����
//		����ֵ����ͼ����
	CGPoint ConvertToMapPoint(CGPoint kScreenPoint);
//		
//		������isMapPointInScreen
//		���ã��жϵ�ͼ������Ƿ�����Ļ��Χ��
//		������mapPoint��ͼ����
//		����ֵ��true�ǣ�false��
	bool isMapPointInScreen(CGPoint mapPoint);
//		
//		������isMapRectIntersectScreen
//		���ã��жϵ�ͼ�ϵľ��������Ƿ�����Ļ��Χ��
//		������mapRect��ͼ�ϵľ�������
//		����ֵ��true�ǣ�false��
	bool isMapRectIntersectScreen(CGRect mapRect);
//		
//		������SetScreenCenter
//		���ã����õ�ͼ������Ӧ��Ļ���ģ����������ͼ�ı��ַ�Χ����õ㲢����Ӧ����Ļ������
//		������p��ͼ�����
//		����ֵ��true ���߽���, false ��û���߽�
	bool SetScreenCenter(CGPoint kPoint);
//		
//		������GetScreenCenter
//		���ã���ȡ��Ļ���ĵ�ĵ�ͼ����
//		��������
//		����ֵ����ͼ����
	CGPoint GetScreenCenter();
//		
//		������SetBattleBackground
//		���ã����ý���ս��
//		������bBattleBackground���Ϊtrue�ͽ���ս���������˳�ս��
//		����ֵ����
	//void SetBattleBackground(bool bBattleBackground);
	//bool IsBattleBackground(){ return m_bBattleBackground;}

	void SetNeedShowBackground(bool bNeedShow);

	void MapSwitchRefresh();

	void ShowRoadSign(bool bShow, int nX = 0, int nY = 0);
public:
	virtual void draw();
	virtual void debugDraw();
	NDMapData *GetMapData();
	void setStartRoadBlockTimer(int time, int x, int y);
	
	void setAutoBossFight(bool isAuto);
	bool isAutoFight() {return isAutoBossFight;}
	void walkToBoss();
	 
	void OnTimer(OBJID tag);
	void replaceMapData(int mapId, int center_x, int center_y);
	void ShowTitle(int name_col, int name_row);
	void refreshTitle();
	void showSwitchSprite(MAP_SWITCH_TYPE type);
	void ShowTreasureBox();
	void OpenTreasureBox();
	bool isTouchTreasureBox(CGPoint touchPoint);
	//		void setRoadBlock(int x,int y){roadBlockX=x;roadBlockY=y;}
public:

	virtual bool TouchBegin( NDTouch* touch );
	virtual void TouchEnd( NDTouch* touch );
	virtual void TouchCancelled( NDTouch* touch );
	virtual void TouchMoved( NDTouch* touch );
	virtual bool TouchDoubleClick( NDTouch* touch );

	CC_SYNTHESIZE(int,m_nBattleType,BattleType);

	void PlayNDSprite(const char* pszSpriteFile, int nPosx, int nPosy,
			int nAnimationNo, int nPlayTimes);
private:
	void MakeOrders();
	void MakeOrdersOfMapscenesAndMapanimations();
	int InsertIndex(int order, cocos2d::CCArray* inArray);
	void QuickSort(cocos2d::CCArray*/*<MAP_ORDER*>*/array, int low, int high);
	int Partition(cocos2d::CCArray*/*<MAP_ORDER*>*/array, int low, int high);

	void SetPosition(CGPoint kPosition);

	void DrawMapTiles();
	void DrawScenesAndAnimations();
	void DrawBgs();

	void MakeFrameRunRecords();

	void ReflashMapTexture(CGPoint oldScreenCenter, CGPoint newScreenCenter);
	void ReplaceMapTexture(cocos2d::CCTexture2D* tex, CGRect replaceRect,
			CGRect tilesRect);
	void ScrollSplit(int x, int y);
	void ScrollVertical(int y, CGRect newRect);
	void ScrollHorizontal(int x, CGRect newRect);
	void RefreshBoxAnimation();

private:

	CGPoint m_kScreenCenter;
	cocos2d::CCArray* m_pkOrders;
	cocos2d::CCArray* m_pkOrdersOfMapscenesAndMapanimations;
	NDMapData *m_pkMapData;

	//cocos2d::CCMutableArray<cocos2d::CCMutableArray<NDFrameRunRecord*>*>* m_pkFrameRunRecordsOfMapAniGroups;
	//cocos2d::CCMutableArray<NDFrameRunRecord*>* m_pkFrameRunRecordsOfMapSwitch;
	cocos2d::CCArray* m_pkFrameRunRecordsOfMapAniGroups;
	cocos2d::CCArray* m_pkFrameRunRecordsOfMapSwitch;

	NDAnimationGroup* m_pkSwitchAniGroup;
	NDSprite* m_pkTreasureBox;
	NDUILabel* m_lbTime;
	NDUIImage* m_lbTitle;
	NDUIImage* m_lbTitleBg;
	CUISpriteNode* m_pkSwitchSpriteNode;
	bool m_bBattleBackground;
	bool m_bNeedShow;
	NDTimer* m_ndBlockTimer;
	NDTimer* m_ndTitleTimer;
	int m_nRoadBlockTimeCount;
	bool isAutoBossFight;
	//CCColorLayer* m_battleBgColor;
	int m_nTitleAlpha;
	//cocos2d::CCTexture2D *m_texMap;
	NDPicture* m_pkPicMap;
	CGPoint m_kCamarkSplit;
	NDNode* m_pkSubNode;
	BOX_STATUS m_eBoxStatus;

	typedef enum
	{
		IntersectionAreaLT,
		IntersectionAreaLB,
		IntersectionAreaRT,
		IntersectionAreaRB,
		IntersectionAreaNone
	} IntersectionArea;

	void RectIntersectionRect(CGRect rect1, CGRect rect2,
			CGRect& intersectionRect, IntersectionArea& intersectionArea);
	IntersectionArea m_areaCamarkSplit;

	int m_nMapIndex;

	bool m_bShowTitle;
	MAP_SWITCH_TYPE m_eSwitchType;

	NDLightEffect* m_pkRoadSignLightEffect;

private:

	bool GetMapDataAniParamReverse(int nIndex);
	CGPoint GetMapDataAniParamPos(int nIndex);
	CGSize GetMapDataAniParamMapSize(int nIndex);
	int GetMapDataAniParamOrderId(int nIndex);
	int GetMapOrderId(MAP_ORDER *dict);
};

}

#endif
