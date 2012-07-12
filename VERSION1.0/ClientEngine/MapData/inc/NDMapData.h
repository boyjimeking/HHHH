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
#include "CCMutableDictionary.h"

class MapTexturePool : public cocos2d::CCObject
{
public:
	static MapTexturePool* defaultPool();
	void purgeDefaultPool();

	cocos2d::CCTexture2D* addImage(const char* path, bool keep);

private:
	cocos2d::CCMutableDictionary<std::string, cocos2d::CCTexture2D*> *m_dict;

private:
	MapTexturePool();
public:
	~MapTexturePool();
};



class NDMapData;
//������
class NDMapSwitch : public cocos2d::CCObject
{
	CC_PROPERTY(int, m_nX, X)
	CC_PROPERTY(int, m_nY, Y)
	CC_PROPERTY(int, m_nMapIndex, MapIndex)
	CC_PROPERTY(int, m_nPassIndex, PassIndex)
	CC_PROPERTY(std::string, m_NameDesMap, NameDesMap)
	CC_PROPERTY(std::string, m_DescDesMap, DescDesMap)

public:
	NDMapSwitch();
	~NDMapSwitch();

	void SetLabel(NDMapData* mapdata); 
	void SetLabelNew(NDMapData* mapdata);
	void SetLableByType(int eLableType, int x, int y, const char* text, cocos2d::ccColor4B color1, cocos2d::ccColor4B color2, CGSize sizeParent);
	void draw();

private:
	NDEngine::NDUILabel *_lbName[2], *_lbDes[2];
};

//����
class NDSceneTile : public NDTile
{
	CC_PROPERTY(int, m_nOrderID, OrderID)

public:
	NDSceneTile();
	~NDSceneTile();
};

//ˢ����
class NDMapMonsterRange : public cocos2d::CCObject
{
	CC_PROPERTY(int, m_nTypeId, TypeId)
	CC_PROPERTY(int, m_nColumn, Column)
	CC_PROPERTY(int, m_nRow, Row)
	CC_PROPERTY(bool, m_bBoss, Boss)

public:
	NDMapMonsterRange();
};

class anigroup_param : public cocos2d::CCObject, public std::map<std::string, int>{};

class NDMapData : public cocos2d::CCObject 
{
	
public:
	CC_PROPERTY(std::string, m_Name, Name)
	CC_PROPERTY_READONLY(int, m_nLayerCount, LayerCount)
	CC_PROPERTY_READONLY(unsigned int, m_nColumns, Columns)
	CC_PROPERTY_READONLY(unsigned int, m_nRows, Rows)
	CC_PROPERTY_READONLY(int, m_nUnitSize, UnitSize)
	CC_PROPERTY(unsigned int, m_nRoadBlockX, RoadBlockX)
	CC_PROPERTY(unsigned int, m_nRoadBlockY, RoadBlockY)
	
	//CC_PROPERTY_READONLY(CCArray<CustomCCTexture2D*>*, m_MapTiles, MapTiles)
	CC_PROPERTY_READONLY(std::vector<bool>*, m_Obstacles, Obstacles)
	CC_PROPERTY_READONLY(cocos2d::CCArray*/*<NDSceneTile*>**/, m_SceneTiles, SceneTiles)
	CC_PROPERTY_READONLY(cocos2d::CCArray*/*<NDSceneTile*>**/, m_BgTiles, BgTiles)
	CC_PROPERTY_READONLY(cocos2d::CCArray*/*<NDMapSwitch*>**/, m_Switchs, Switchs)
	CC_PROPERTY_READONLY(cocos2d::CCArray*/*<NDAnimationGroup*>**/, m_AnimationGroups, AnimationGroups)
	CC_PROPERTY_READONLY(cocos2d::CCArray*/*<anigroup_param*>**/, m_AniGroupParams, AniGroupParams)

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
	void decode(FILE* stream);
};
#endif