//
//  NDMapData.m
//  MapData
//
//  Created by jhzheng on 10-12-10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#include "NDMapData.h"
#include "NDTile.h"
#include "JavaMethod.h"
#include "NDPath.h"
#include "CCTextureCache.h"
#include "CCTexture2D.h"
#include "NDAnimationGroup.h"
#include "NDAnimationGroupPool.h"
#include "cpLog.h"
#include "CCConfiguration.h"
#include "CCTextureCacheExt.h"
#include "cpLog.h"
#include "WorldMapScene.h"
#include "NDDirector.h"
#include "NDUtility.h"
#include "define.h"
#include "CGPointExtension.h"
#include "NDMapMgr.h"
#include "NDWorldMapData.h"

MapTexturePool *MapTexturePool_defaultPool = NULL;

using namespace cocos2d;

MapTexturePool::MapTexturePool()
: m_dict(NULL)
{
	NDAsssert(MapTexturePool_defaultPool == NULL);
	m_dict = new CCMutableDictionary<std::string, CCTexture2D*>();
}

MapTexturePool::~MapTexturePool()
{
	NDAsssert(MapTexturePool_defaultPool != NULL);
	MapTexturePool_defaultPool = NULL;
	CC_SAFE_RELEASE(m_dict);
}

MapTexturePool* MapTexturePool::defaultPool()
{
	if (!MapTexturePool_defaultPool)
		MapTexturePool_defaultPool = new MapTexturePool;
	
	return MapTexturePool_defaultPool;
}

void MapTexturePool::purgeDefaultPool()
{
	CC_SAFE_RELEASE_NULL(MapTexturePool_defaultPool);
}

CCTexture2D* MapTexturePool::addImage(const char* path, bool keep)
{
	NDAsssert(path != NULL);
	
	CCTexture2D * tex = NULL;
	
	// MUTEX:
	// Needed since addImageAsync calls this method from a different thread
	
	tex = m_dict->objectForKey(path);	
	
	if (!tex) 
	{
		CCImage image;
		if (image.initWithImageFile(path))
		{
				//todo
			//tex = [ [CCTexture2D alloc] initWithImage:image keepData:keep];
		}
		
		if (tex) 
		{
			m_dict->setObject(tex, path);
			tex->release();
		}
	}

	return tex;
}

NDMapSwitch::NDMapSwitch()
: m_nX(0)
, m_nY(0)
, m_nMapIndex(0)
, m_nPassIndex(0)
{
	memset(_lbName, 0, sizeof(_lbName));
	memset(_lbDes, 0, sizeof(_lbDes));
}

NDMapSwitch::~NDMapSwitch()
{
	if (_lbName[0]) 
	{
		delete _lbName[0];
	}

	if (_lbName[1]) 
	{
		delete _lbName[1];
	}

	if (_lbDes[0]) 
	{
		delete _lbDes[0];
	}

	if (_lbDes[1]) 
	{
		delete _lbDes[1];
	}
}

void NDMapSwitch::SetLabel(NDMapData* mapdata)
{
	if (mapdata == NULL) 
	{
		return;
	}
	std::string name = NDCString("notopen"), des = "";
	int destMapId = NDWorldMapData::SharedData()->getDestMapIdWithSourceMapId(NDMapMgrObj.m_iMapDocID, m_nPassIndex);
	if (destMapId > 0) 
	{
		PlaceNode *placeNode = NDWorldMapData::SharedData()->getPlaceNodeWithMapId(destMapId);
		if (placeNode) 
		{
			name = placeNode->getName();
			des  = placeNode->getDescription();
			
			int idx = des.find("（", 0);
			if (idx == -1) 
			{
				idx = des.find(NDCString("lianjiquyu"));
				if (idx != -1) 
				{
					des.erase(idx, des.size() - idx);
				}
			}
			else 
			{
				des.erase(idx, des.size() - idx);
			}
		}
		else 
		{
			name = NDCString("minyuecun");
			des  = NDCString("xinshou");
		}
		
		int tw = getStringSize(name.c_str(), 15).width;
		int tx = _x*mapdata.unitSize + 10 - tw / 2;			
		int ty = _y*mapdata.unitSize- 52;
		
		if (!des.empty() && des != "") {				
			int tx2 = _x*mapdata.unitSize  + 10 - (getStringSize(des.c_str(), 15).width / 2);
			int ty2 = _y*mapdata.unitSize - 52;//ty;
			//T.drawString2(g, introduce, tx2, ty2, 0xFFF5B4,0xC75900, 0);//后文字 0xFFF5B4, 0xC75900
			this->SetLableByType(1,
							   tx2,
							   ty2,
								des.c_str(),
						 INTCOLORTOCCC4(0xFFF5B4),
						 INTCOLORTOCCC4(0xC75900),
					 CGSizeMake(mapdata->getColumns()*mapdata.unitSize, mapdata->getRows()*mapdata.unitSize)
					 );
			ty -= 20;
		}
		//T.drawString2(g, name, tx, ty, 0xFFFF00,0x2F4F4F,0);//0x2F4F4F
		this->SetLableByType(0
						   tx,
						   ty,
						name.c_str(),
					  INTCOLORTOCCC4(0xFFFF00),
					  INTCOLORTOCCC4(0x2F4F4F),
				 CGSizeMake(mapdata->getColumns()*mapdata.unitSize, mapdata->getRows()*mapdata.unitSize)
		 );
	}
}

void NDMapSwitch::SetLabelNew(NDMapData* mapdata)
{
	float fScaleFactor	= NDDirector::DefaultDirector()->GetScaleFactor();
	if (mapdata == NULL) 
	{
		return;
	}
	
	std::string name = NDCString("notopen"), des = "";
	des = m_DescDesMap;
	name = m_NameDesMap;

	int tw = getStringSize(name.c_str(), 15).width;
	int tx = _x*mapdata.unitSize + DISPLAY_POS_X_OFFSET - tw / 2;			
	int ty = _y*mapdata.unitSize + DISPLAY_POS_Y_OFFSET - 62 * fScaleFactor;
	
	if (!des.empty() && des != "") {				
		int tx2 = _x*mapdata.unitSize  + 10 * fScaleFactor - (getStringSize(des.c_str(), 15).width / 2);
		int ty2 = _y*mapdata.unitSize - 52 * fScaleFactor;//ty;
		//T.drawString2(g, introduce, tx2, ty2, 0xFFF5B4,0xC75900, 0);//后文字 0xFFF5B4, 0xC75900
		this->SetLableByType(1,
						   tx2,
						   ty2,
						des.c_str(),
					 INTCOLORTOCCC4(0xFFF5B4),
					  INTCOLORTOCCC4(0xC75900),
				  CGSizeMake(mapdata->getColumns()*mapdata.unitSize, mapdata->getRows()*mapdata.unitSize)
		 );
		ty -= 20 * fScaleFactor;
	}
	//T.drawString2(g, name, tx, ty, 0xFFFF00,0x2F4F4F,0);//0x2F4F4F
	this->SetLableByType(0,
					   tx,
					   ty,
					name.c_str(),
				  INTCOLORTOCCC4(0xFFFF00),
				  INTCOLORTOCCC4(0x2F4F4F),
			  CGSizeMake(mapdata->getColumns()*mapdata.unitSize, mapdata->getRows()*mapdata.unitSize)
	 );
	
}

void NDMapSwitch::SetLableByType(int eLableType, int x, int y, char* text, ccColor4B color1, ccColor4B color2, CGSize sizeParent)
{
	if (!text) 
	{
		return;
	}
	
	float fScaleFactor	= NDDirector::DefaultDirector()->GetScaleFactor();
	
	NDUILabel *lable[2]; memset(lable, 0, sizeof(lable));
	if (eLableType == 0) 
	{
		if (!_lbName[0]) 
		{
			_lbName[0] = new NDUILabel;
			_lbName[0]->Initialization();
		}
		
		if (!_lbName[1]) 
		{
			_lbName[1] = new NDUILabel;
			_lbName[1]->Initialization();
		}
		
		lable[0] = _lbName[0];
		lable[1] = _lbName[1];
	}
	else if (eLableType == 1) 
	{
		if (!_lbDes[0]) 
		{
			_lbDes[0] = new NDUILabel;
			_lbDes[0]->Initialization();
		}
		
		if (!_lbDes[1]) 
		{
			_lbDes[1] = new NDUILabel;
			_lbDes[1]->Initialization();
		}
		
		lable[0] = _lbDes[0];
		lable[1] = _lbDes[1];
	}
	
	if (!lable[0] || !lable[1]) 
	{
		return;
	}
	
	lable[0]->SetText(text);
	lable[1]->SetText(text);
	
	lable[0]->SetFontColor(color1);
	lable[1]->SetFontColor(color2);
	
	lable[0]->SetFontSize(15);
	lable[1]->SetFontSize(15);
	
	CGSize sizewin = NDDirector::DefaultDirector()->GetWinSize();
	lable[1]->SetFrameRect(CGRectMake(x+1, y+sizewin.height+1-sizeParent.height, sizewin.width, 20 * fScaleFactor));
	lable[0]->SetFrameRect(CGRectMake(x, y+sizewin.height-sizeParent.height, sizewin.width, 20 * fScaleFactor));
}

void NDMapSwitch::draw()
{
	if (_lbName[1]) {
		_lbName[1]->draw();
	}
	if (_lbName[0]) {
		_lbName[0]->draw();
	}
	
	if (_lbDes[1]) {
		_lbDes[1]->draw();
	}
	if (_lbDes[0]) {
		_lbDes[0]->draw();
	}
}

NDSceneTile::NDSceneTile()
: m_nOrderID(0)
{
}

NDMapMonsterRange::NDMapMonsterRange()
: m_nTypeId(0)
, m_nColumn(0)
, m_nRow(0)
, m_bBoss(false)
{
}

NDMapData::NDMapData()
: m_nLayerCount(0)
, m_nColumns(0)
, m_nRows(0)
, m_nUnitSize(0)
, m_nRoadBlockX(-1)
, m_nRoadBlockY(-1)
, m_MapTiles(NULL)
, m_Obstacles(NULL)
, m_SceneTiles(NULL)
, m_BgTiles(NULL)
, m_Switchs(NULL)
, m_AnimationGroups(NULL)
, m_AniGroupParams(NULL)
{
}

NDMapData::~NDMapData()
{
	CC_SAFE_RELEASE(m_MapTiles);
	CC_SAFE_RELEASE(m_Obstacles);
	CC_SAFE_RELEASE(m_SceneTiles);
	CC_SAFE_RELEASE(m_BgTiles);
	CC_SAFE_RELEASE(m_Switchs);
	CC_SAFE_RELEASE(m_AnimationGroups);
	CC_SAFE_RELEASE(m_AniGroupParams);
	MapTexturePool::defaultPool()->release();
}

/*通过地图文件(不包含路径)加载地图数据
 参数:mapFile-地图文件名
 */
void NDMapData::initWithFile(const char* mapFile)
{
	if (mapFile) 
	{		
		FILE*  stream = fopen(mapFile, "rt");
		if (!stream) 
		{
			return;
		}
	
		this->decode(stream);
		fclose(stream);
	}
}

/*  地图文件解析
    参数:地图文件流
*/

void NDMapData::decode(FILE* stream)
{	
	FileOp op;
	//<-------------------地图名
	m_Name = op.readUTF8String(stream);//[self readUTF8String:stream];
	//<-------------------单元格尺寸
	m_nUnitSize=op.readByte(stream);
	int TileWidth		= m_nUnitSize;
	int TileHeight		= m_nUnitSize;
	//------------------->层数
	m_nLayerCount = op.readByte(stream);
	//<-------------------列数
	m_nColumns = op.readByte(stream);
	//------------------->行数
	m_nRows = op.readByte(stream);
	//<-------------------使用到的图块资源
	std::vector<std::string> _tileImages;
	int tileImageCount = op.readShort(stream);	
	for (int i = 0; i < tileImageCount; i++) 
	{
		int idx = op.readShort(stream);
		char* imageName[256] = {0};
		sprintf(imageName, "%st%d.png", NDEngine::NDPath::GetImagePath().c_str(), idx);
		FILE* f = fopen(imageName, "rt");
		if (f ) 
		{
			_tileImages.push_back(imageName);
		}
		else 
		{
			return;
		}
	}
	
	//------------------->瓦片	
	m_MapTiles = new CCArray<CustomCCTexture2D*>();
	for ( int lay = 0; lay < m_nLayerCount; lay++) 
		for (uint r = 0; r < m_nRows; r++) 
			for (uint c = 0; c < m_nColumns; c++)
			{					
				int imageIndex		= op.readByte(stream) - 1;	//资源下标
				if (imageIndex == -1) 
					imageIndex = 0;
				int	tileIndex		= op.readByte(stream);		//图块下标
				bool reverse		= op.readByte(stream) == 1;	//翻转
				
				if (imageIndex == -1)
				{
					imageIndex = 0;
					//continue;
				}
				
				if(_tileImages.size() > imageIndex)
				{
					std::string imageName	=_tileImages[imageIndex];
					
					CustomCCTexture2D *tile = new CustomCCTexture2D();
					tile->setTexture(MapTexturePool::defaultPool()->addImage(imageName.c_str(), true));	//[[CCTextureCache sharedTextureCache] addImage:imageName keepData:true]; 
					int PicParts	= tile->getTexture()->getPixelsWide() * tile->getTexture()->getMaxS() / TileWidth;
					tile->setCutRect(CGRectMake(TileWidth*(tileIndex%PicParts), TileHeight*(tileIndex/PicParts), TileWidth, TileHeight));
					tile->setDrawRect(CGRectMake(TileWidth*c, TileHeight*r, TileWidth, TileHeight));
					tile->setHorizontalReverse(reverse);				
					m_MapTiles->addObject(tile);
					tile->release();			
				}
			}
	//<-------------------通行
	m_Obstacles = new std::vector<bool>();
	for (unsigned int i = 0; i < m_nColumns*m_nRows; i++)
	{
		m_Obstacles->push_back(true);
	}
	
	int notPassCount = op.readShort(stream);	
	for (int i = 0; i < notPassCount; i++) 
	{		
		int	rowIndex	= op.readByte(stream);
		int columnIndex	= op.readByte(stream);
		int nIndex		= rowIndex*_columns+columnIndex;
		if (m_Obstacles->size() > nIndex)
		{
			(*m_Obstacles)[nIndex]	= false;
		}
	}
	//------------------->切屏
	m_Switchs = new CCArray<NDMapSwitch*>();
	int switchsCount = op.readByte(stream);
	for (int i = 0; i < switchsCount; i++) 
	{
		//NDMapSwitch *mapSwitch = [[NDMapSwitch alloc] init];
		
		/*mapSwitch.x = */op.readByte(stream); //切屏点x
		/*mapSwitch.y = */op.readByte(stream);//切屏点y
		/*mapSwitch.mapIndex = */op.readByte(stream); //目标地图
		/*mapSwitch.passIndex = */op.readByte(stream); //目标点
		/*[mapSwitch SetLabel:self];*/
		
		/*[_switchs addObject:mapSwitch];*/
		/*[mapSwitch release];*/
	}
	//<---------------------使用到的背景资源
	std::vector<std::string> _bgImages;
	std::vector<int>_bgOrders;
	int bgImageCount = op.readShort(stream);
	for (int i = 0; i < bgImageCount; i++) 
	{
		int idx = op.readShort(stream);
		char* imageName[256] = {0};
		sprintf(imageName, "%sb%d.png", NDEngine::NDPath::GetImagePath().c_str(), idx);
		FILE* f = fopen(imageName, "rt");
		if (f )  
		{
			_bgImages.push_back(imageName);
		}
		else 
		{
			NDLog(@"背景资源%@没有找到!!!", imageName);
			_bgImages.push_back(imageName);
		}
		
		int v = op.readShort(stream);
		_bgOrders.push_back(v);
	}
	//---------------------->背景
	m_BgTiles = new CCArray<NDSceneTile*>;
	int bgCount = op.readShort(stream);
	for (int i = 0; i < bgCount; i++) 
	{		
		int resourceIndex	= op.readByte(stream);										//资源下标
		int	x				= op.readShort(stream);	//x坐标
		int y				= op.readShort(stream);	//y坐标
		bool reverse		= op.readByte(stream) > 0;									//翻转
		
		if (_bgImages.size() <= resourceIndex || _bgOrders.size() <= resourceIndex)
		{
			continue;
		}
		
		std::string imageName		= _bgImages [resourceIndex];
		NDSceneTile	*tile = new NDSceneTile;
		tile->setOrderID(_bgOrders[resourceIndex] + y);
		tile->setTexture(CCTextureCache::sharedTextureCache->addImage(imageName.c_str()));
		int picWidth	= tile->getTexture()->getPixelsWide() * tile->getTexture()->getMaxS(); 
		int picHeight	= tile->getTexture()->getPixelsHigh() * tile->getTexture()->getMaxT();
		
		tile->setMapSize(CGSizeMake(m_nColumns*TileWidth, m_nRows*TileHeight));
		tile->setCutRect(CGRectMake(0, 0, picWidth, picHeight));
		tile->setDrawRect(CGRectMake(x, y, picWidth, picHeight));
		tile->setReverse(reverse);	
		
		tile->make();
		
		m_BgTiles->addObject(tile);
		tile->release();
	}
	//<-------------------使用到的布景资源
	std::vector<std::string> _sceneImages;
	std::vector<int> _sceneOrders;
	int sceneImageCount = op.readShort(stream);
	for (int i = 0; i < sceneImageCount; i++) 
	{
		int idx = op.readShort(stream);
		char* imageName[256] = {0};
		sprintf(imageName, "%s%d.png", NDEngine::NDPath::GetImagePath().c_str(), idx);
		FILE* f = fopen(imageName, "rt");
		if (f )  
		{
			_sceneImages.push_back(imageName);
		}
		else 
		{
			NDLog(@"布景资源%@没有找到!!!", imageName);
			_sceneImages.push_back(imageName);
		}
		
		int v = op.readShort(stream);
		_sceneOrders.push_back(v);
	}
	//------------------->布景
	m_SceneTiles = new CCArray<NDSceneTile*>();
	int sceneCount = op.readShort(stream);
	for (int i = 0; i < sceneCount; i++) 
	{		
		int resourceIndex	= op.readByte(stream);										//资源下标
		int	x				= op.readShort(stream);	//x坐标
		int y				= op.readShort(stream);	//y坐标
		BOOL reverse		= op.readByte(stream); > 0;									//翻转
		
		if (_sceneImages.size() <= resourceIndex || _sceneOrders.size() <= resourceIndex)
		{
			continue;
		}
		
		NDSceneTile	*tile = new NDSceneTile;
		tile->setOrderID(_sceneOrders[resourceIndex] + y);
		tile->setTexture(CCTextureCache::sharedTextureCache()->addImage(imageName);
		int picWidth	= tile->getTexture()->getPixelsWide() * tile->getTexture()->getMaxS(); 
		int picHeight	= tile->getTexture()->getPixelsHigh() * tile->getTexture()->getMaxT();
		
		tile->setMapSize(CGSizeMake(m_nColumns*TileWidth, m_nRows*TileHeight));
		tile->setCutRect(CGRectMake(0, 0, picWidth, picHeight));
		tile->setDrawRect(CGRectMake(x, y, picWidth, picHeight));
		tile->setReverse(reverse);
		
		tile->make();
		
		m_SceneTiles->addObject(tile);
		tile->release();
	}
	//<-------------------动画
	m_AnimationGroups = new CCArray<NDAnimationGroup*>();
	m_AniGroupParams  = new CCArray<anigroup_param*>();
	int aniGroupCount = op.readShort(stream);
	for (int i = 0; i < aniGroupCount; i++) 
	{			
		int identifer = op.readShort(stream);			//动画id
		NDAnimationGroup *aniGroup = NDAnimationGroupPool::defaultPool()->addObjectWithSceneAnimationId(identifer);
		if (!aniGroup) continue;
		m_AnimationGroups->addObject(aniGroup);
		aniGroup->release();

		int x = op.readShort(stream);		//x坐标
		int y = op.readShort(stream);		//y坐标
		int aniOrder = y + op.readShort(stream);	//排序重心
		
		anigroup_param* dict = new anigroup_param;
		dict->insert(std::make_pair("reverse", 0));
		dict->insert(std::make_pair("positionX", x));
		dict->insert(std::make_pair("positionY", y));
		dict->insert(std::make_pair("mapSizeW", m_nColumns * m_nUnitSize));
		dict->insert(std::make_pair("mapSizeH", m_nRows * m_nUnitSize));
		dict->insert(std::make_pair("orderId", aniOrder));
		dict->insert(std::make_pair("reverse", 0));
		dict->insert(std::make_pair("reverse", 0));
		
		m_AniGroupParams->addObject(dict);
		dict->release();	
	}
	//------------------->刷怪区
// 	NSMutableArray	*_monsterRanges = [[NSMutableArray alloc] init];
// 	int monsterRangeCount = [stream readByte];
// 	for (int i = 0; i < monsterRangeCount; i++) 
// 	{
// 		NDMapMonsterRange *monsterRange = [[NDMapMonsterRange alloc] init];
// 		
// 		monsterRange.typeId = [stream readInt];
// 		monsterRange.column = [stream readByte];
// 		monsterRange.row = [stream readByte];
// 		monsterRange.boss = [stream readByte] != 0;
// 		
// 		[_monsterRanges addObject:monsterRange];
// 		[monsterRange release];
// 	}
// 	
// 	[_tileImages release];
// 	[_sceneImages release];
// 	[_sceneOrders release];
// 	[_monsterRanges release];
// 	[_bgImages release];
// 	[_bgOrders release];
}

/*判断某个位置是否可走
 参数:row-某行,column-某列
 返回值:YES/NO
 */
bool NDMapData::canPassByRow(unsigned int row, unsigned int column)
{
	if ( column >= m_nColumns || row >= m_nRows ) {
		return false;
	}
	if(row == m_nRoadBlockY)
	{
		return false;
	}
	if(column == m_nRoadBlockX)
	{
		return false;
	}
	
	int nIndex = row*m_nColumns+column;

	if (m_Obstacles->size() <= nIndex)
	{
		return false;
	}

	return m_Obstacles->at(nIndex);
}

void NDMapData::setRoadBlock(int x, int y)
{
	m_nRoadBlockX=x;
	m_nRoadBlockY=y;
}

NDSceneTile * NDMapData::getBackGroundTile(unsigned int index)
{
	if (index>=m_BgTiles->count()) 
	{
		return NULL;
	}else
	{	
		return m_BgTiles->objectAtIndex(index);
	}
}

void NDMapData::moveBackGround(int x, int y)
{
	if(x ==0)
		return;
	for(int i= 0; i< 2; i++)
	{
		NDSceneTile* pTile = this->getBackGroundTile(i);
		if(pTile)
		{
			CGRect rect = pTile->getDrawRect();
			rect.origin.x -= -x/3;
			pTile->setDrawRect(rect);
			pTile->make();
		}
	}	
	
}


//- (NDTile *)getTileAtRow:(uint)row column:(uint)column
CustomCCTexture2D * NDMapData::getTileAtRow(unsigned int row, unsigned int column)
{
	if (row >= _rows) 
	{
		return NULL;
	}
	if (column >= _columns) 
	{
		return NULL;
	}
	
	int index =	row * m_nColumns + column;
	if (index>=m_MapTiles->count()) 
	{
		return NULL;
	}
	else
	{
		return m_MapTiles->objectAtIndex(index);
	}
}

void NDMapData::addObstacleCell(unsigned int row, unsigned int column)
{
	if (row >= m_nRows || row < 0) 
	{
		return;
	}
	if (column >= m_nColumns || column < 0) 
	{
		return;
	}
	
	int nIndex = row*m_nColumns+column;
	if (m_Obstacles->size() > nIndex)
	{
		(*m_Obstacles)[nIndex] = false;
	}
}

void NDMapData::removeObstacleCell(unsigned int row, unsigned int column)
{
	if (row >= m_nRows || row < 0) 
	{
		return;
	}
	if (column >= m_nColumns || column < 0) 
	{
		return;
	}
	
	int nIndex = row*m_nColumns+column;
	if (m_Obstacles->size() > nIndex)
	{
		(*m_Obstacles)[nIndex] = true;
	}
}

void NDMapData::addMapSwitch(unsigned int x,			// 切屏点 x
					  unsigned int y,			// 切屏点 y
					  unsigned int index,		// 切屏点索引
					  unsigned int mapid,		// 目标地图id
					  const char* name,	// 目标地图名称
					  const char* desc);	// 目标地图描述
{
	NDMapSwitch *mapSwitch = new NDMapSwitch;
	
	mapSwitch->setX(x); //切屏点x
	mapSwitch->setY(y); //切屏点y
	mapSwitch->setMapIndex(mapid); //目标地图
	mapSwitch->setPassIndex(index); //目标点
	mapSwitch->setNameDesMap(name);
	mapSwitch->setDescDesMap(desc);
	mapSwitch->SetLabelNew(this);
	
	m_Switchs->addObject(mapSwitch);
	mapSwitch->release();
}
