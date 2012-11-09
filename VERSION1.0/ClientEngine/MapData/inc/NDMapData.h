//
//  NDMapData.h
//  MapData
//
//  Created by jhzheng on 10-12-10.
//  Copyright 2010 (����)DeNA. All rights reserved.
//

#ifndef _ND_MAP_DATA_
#define _ND_MAP_DATA_

#include "NDTile.h"
#include "NDUILabel.h"
#include "cocoa/CCDictionary.h"

class MapTexturePool: public cocos2d::CCObject
{
public:

	static MapTexturePool* defaultPool();
	void purgeDefaultPool();

	cocos2d::CCTexture2D* addImage(const char* path, bool keep);

private:
	cocos2d::CCDictionary*	m_pkDict;

private:
	MapTexturePool();
public:
	~MapTexturePool();
};

class NDMapData;
//������
class NDMapSwitch: public cocos2d::CCObject
{
// 	CC_SYNTHESIZE(int, m_nX, X)
// 	CC_SYNTHESIZE(int, m_nY, Y)
	CC_SYNTHESIZE(int, m_nMapIndex, MapIndex)
	CC_SYNTHESIZE(int, m_nPassIndex, PassIndex)
	CC_SYNTHESIZE(std::string, m_strNameDesMap, NameDesMap)
	CC_SYNTHESIZE(std::string, m_strDescDesMap, DescDesMap)

public:
	const CGPoint& getWorldPos() { return m_worldPos; }
	CGPoint getCellPos();
	void setWorldPos( const CGPoint& worldPos ) { m_worldPos = worldPos; }
	void setCellPos( const CGPoint& cellPos );
	
public:
	NDMapSwitch();
	~NDMapSwitch();

	void SetLabel(NDMapData* mapdata);
	void SetLabelNew(NDMapData* pkMapdata);
	void SetLableByType(int eLableType, int x, int y, const char* pszText,
			cocos2d::ccColor4B color1, cocos2d::ccColor4B color2,
			CGSize kParentSize);
	void draw();

private:

	NDEngine::NDUILabel* m_pkNameLabels[2];
	NDEngine::NDUILabel* m_pkDesLabels[2];
	CGPoint m_worldPos;
};

//����
class NDSceneTile: public NDTile
{
	CC_SYNTHESIZE(int, m_nOrderID, OrderID)

public:
	NDSceneTile();
	~NDSceneTile()
	{
	}
};

//ˢ����
class NDMapMonsterRange: public cocos2d::CCObject
{
	CC_SYNTHESIZE(int, m_nTypeId, TypeId)
	CC_SYNTHESIZE(int, m_nColumn, Column)
	CC_SYNTHESIZE(int, m_nRow, Row)
	CC_SYNTHESIZE(bool, m_bBoss, Boss)

public:
	NDMapMonsterRange();
};

class anigroup_param: public cocos2d::CCObject,
		public std::map<std::string, int>
{
};

class NDMapData: public cocos2d::CCObject
{

public:
	CC_SYNTHESIZE(std::string, m_strName, Name)
	CC_SYNTHESIZE(int, m_nLayerCount, LayerCount)
	CC_SYNTHESIZE(unsigned int, m_nColumns, Columns)
	CC_SYNTHESIZE(unsigned int, m_nRows, Rows)
	CC_SYNTHESIZE(int, m_nUnitSize, UnitSize)
	CC_SYNTHESIZE(unsigned int, m_nRoadBlockX, RoadBlockX)
	CC_SYNTHESIZE(unsigned int, m_nRoadBlockY, RoadBlockY)

	CC_SYNTHESIZE_READONLY(CCArray*, m_kMapTiles, MapTiles)
	CC_SYNTHESIZE(std::vector<bool>*, m_pkObstacles, Obstacles)
	CC_SYNTHESIZE(cocos2d::CCArray*, m_pkSceneTiles, SceneTiles)
	CC_SYNTHESIZE(cocos2d::CCArray*, m_pkBackgroundTiles, BgTiles)
	CC_SYNTHESIZE(cocos2d::CCArray*, m_pkSwitchs, Switchs)
	CC_SYNTHESIZE_READONLY(cocos2d::CCArray*, m_pkAnimationGroups, AnimationGroups)
	CC_SYNTHESIZE(cocos2d::CCArray*, m_pkAniGroupParams, AniGroupParams)

public:
	NDMapData();
	~NDMapData();

	/*ͨ����ͼ�ļ�(������·��)���ص�ͼ����
	 ����:mapFile-��ͼ�ļ���
	 */
	void initWithFile(const char* mapFile);

	/*�ж�ĳ��λ���Ƿ����
	 ����:row-ĳ��,column-ĳ��
	 ����ֵ:YES/NO
	 */
	bool canPassByRow(unsigned int row, unsigned int column);

	//CustomCCTexture2D * getTileAtRow(unsigned int row, unsigned int column);

	NDSceneTile * getBackGroundTile(unsigned int index);

	void moveBackGround(int x, int y);

	void addObstacleCell(unsigned int row, unsigned int column);
	void removeObstacleCell(unsigned int row, unsigned int column);
	void addMapSwitch(unsigned int x,			// ������ x
			unsigned int y,			// ������ y
			unsigned int index,		// ����������
			unsigned int mapid,		// Ŀ���ͼid
			const char* name,	// Ŀ���ͼ����
			const char* desc);	// Ŀ���ͼ����
	void setRoadBlock(int x, int y);

private:

	void decode(FILE* pkStream);
};
#endif
