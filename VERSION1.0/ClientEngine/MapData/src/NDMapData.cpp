//
//  NDMapData.m
//  MapData
//
//  Created by jhzheng on 10-12-10.
//  Copyright 2010 (����)DeNA. All rights reserved.
//

#include "NDMapData.h"
#include "NDTile.h"
#include "JavaMethod.h"
#include "NDPath.h"
#include "CCTextureCache.h"
#include "CCTexture2D.h"
#include "NDAnimationGroup.h"
#include "NDAnimationGroupPool.h"
#include "CCConfiguration.h"
#include "CCTextureCacheExt.h"
#include "NDDirector.h"
#include "CcPointExtension.h"
#include "NDWorldMapData.h"
#include "CCImage.h"
#include "NDConstant.h"

MapTexturePool* g_pkMapTexturePoolDefaultPool = NULL;

using namespace cocos2d;
using namespace NDEngine;

MapTexturePool::MapTexturePool() :
		m_pkDict(NULL)
{
	NDAsssert(g_pkMapTexturePoolDefaultPool == NULL);
	m_pkDict = new CCMutableDictionary<std::string, CCTexture2D*>();
}

MapTexturePool::~MapTexturePool()
{
	NDAsssert(g_pkMapTexturePoolDefaultPool != NULL);
	g_pkMapTexturePoolDefaultPool = NULL;
	CC_SAFE_RELEASE (m_pkDict);
}

MapTexturePool* MapTexturePool::defaultPool()
{
	if (!g_pkMapTexturePoolDefaultPool)
		g_pkMapTexturePoolDefaultPool = new MapTexturePool;

	return g_pkMapTexturePoolDefaultPool;
}

void MapTexturePool::purgeDefaultPool()
{
	CC_SAFE_RELEASE_NULL (g_pkMapTexturePoolDefaultPool);
}

CCTexture2D* MapTexturePool::addImage(const char* path, bool keep)
{
	NDAsssert(path != NULL);

	CCTexture2D * tex = NULL;

	// MUTEX:
	// Needed since addImageAsync calls this method from a different thread

	tex = m_pkDict->objectForKey(path);

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
			m_pkDict->setObject(tex, path);
			tex->release();
		}
	}

	return tex;
}

NDMapSwitch::NDMapSwitch() :
m_nX(0),
m_nY(0),
m_nMapIndex(0),
m_nPassIndex(0)
{
	memset(m_pkNameLabels, 0, sizeof(m_pkNameLabels));
	memset(m_pkDesLabels, 0, sizeof(m_pkDesLabels));
}

NDMapSwitch::~NDMapSwitch()
{
	if (m_pkNameLabels[0])
	{
		delete m_pkNameLabels[0];
	}

	if (m_pkNameLabels[1])
	{
		delete m_pkNameLabels[1];
	}

	if (m_pkDesLabels[0])
	{
		delete m_pkDesLabels[0];
	}

	if (m_pkDesLabels[1])
	{
		delete m_pkDesLabels[1];
	}
}

// void NDMapSwitch::SetLabel(NDMapData* mapdata)
// {
// 	if (mapdata == NULL) 
// 	{
// 		return;
// 	}
// 	//std::string name = NDCString("notopen"), des = "";
// 	std::string name = "";
// 	int destMapId = NDWorldMapData::SharedData()->getDestMapIdWithSourceMapId(NDMapMgrObj.m_iMapDocID, m_nPassIndex);
// 	if (destMapId > 0) 
// 	{
// 		PlaceNode *placeNode = NDWorldMapData::SharedData()->getPlaceNodeWithMapId(destMapId);
// 		if (placeNode) 
// 		{
// 			name = placeNode->getName();
// 			des  = placeNode->getDescription();
// 			
// 			int idx = des.find("��", 0);
// 			if (idx == -1) 
// 			{
// 				idx = des.find(NDCString("lianjiquyu"));
// 				if (idx != -1) 
// 				{
// 					des.erase(idx, des.size() - idx);
// 				}
// 			}
// 			else 
// 			{
// 				des.erase(idx, des.size() - idx);
// 			}
// 		}
// 		else 
// 		{
// 			name = NDCString("minyuecun");
// 			des  = NDCString("xinshou");
// 		}
// 		
// 		int tw = getStringSize(name.c_str(), 15).width;
// 		int tx = _x*mapdata.unitSize + 10 - tw / 2;			
// 		int ty = _y*mapdata.unitSize- 52;
// 		
// 		if (!des.empty() && des != "") {				
// 			int tx2 = _x*mapdata.unitSize  + 10 - (getStringSize(des.c_str(), 15).width / 2);
// 			int ty2 = _y*mapdata.unitSize - 52;//ty;
// 			//T.drawString2(g, introduce, tx2, ty2, 0xFFF5B4,0xC75900, 0);//������ 0xFFF5B4, 0xC75900
// 			this->SetLableByType(1,
// 							   tx2,
// 							   ty2,
// 								des.c_str(),
// 						 INTCOLORTOCCC4(0xFFF5B4),
// 						 INTCOLORTOCCC4(0xC75900),
// 					 CGSizeMake(mapdata->getColumns()*mapdata.unitSize, mapdata->getRows()*mapdata.unitSize)
// 					 );
// 			ty -= 20;
// 		}
// 		//T.drawString2(g, name, tx, ty, 0xFFFF00,0x2F4F4F,0);//0x2F4F4F
// 		this->SetLableByType(0
// 						   tx,
// 						   ty,
// 						name.c_str(),
// 					  INTCOLORTOCCC4(0xFFFF00),
// 					  INTCOLORTOCCC4(0x2F4F4F),
// 				 CGSizeMake(mapdata->getColumns()*mapdata.unitSize, mapdata->getRows()*mapdata.unitSize)
// 		 );
// 	}
// }

void NDMapSwitch::SetLabelNew(NDMapData* pkMapdata)
{
	float fScaleFactor = NDDirector::DefaultDirector()->GetScaleFactor();

	if (pkMapdata == NULL)
	{
		return;
	}

	//std::string name = NDCString("notopen"), des = "";
	std::string strName = "";
	std::string strDes = m_strDescDesMap;
	strName = m_strNameDesMap;

	int tw = getStringSize(strName.c_str(), 15).width;
	int tx = m_nX * pkMapdata->getUnitSize() + DISPLAY_POS_X_OFFSET - tw / 2;
	int ty = m_nY * pkMapdata->getUnitSize() + DISPLAY_POS_Y_OFFSET
			- 62 * fScaleFactor;

	if (!strDes.empty() && strDes != "")
	{
		int tx2 = m_nX * pkMapdata->getUnitSize() + 10 * fScaleFactor
				- (getStringSize(strDes.c_str(), 15).width / 2);
		int ty2 = m_nY * pkMapdata->getUnitSize() - 52 * fScaleFactor;	//ty;
		//T.drawString2(g, introduce, tx2, ty2, 0xFFF5B4,0xC75900, 0);//������ 0xFFF5B4, 0xC75900
		this->SetLableByType(1, tx2, ty2, strDes.c_str(), INTCOLORTOCCC4(0xFFF5B4),
				INTCOLORTOCCC4(0xC75900),
				CGSizeMake(pkMapdata->getColumns() * pkMapdata->getUnitSize(),
						pkMapdata->getRows() * pkMapdata->getUnitSize()));
		ty -= 20 * fScaleFactor;
	}
	//T.drawString2(g, name, tx, ty, 0xFFFF00,0x2F4F4F,0);//0x2F4F4F
	this->SetLableByType(0, tx, ty, strName.c_str(), INTCOLORTOCCC4(0xFFFF00),
			INTCOLORTOCCC4(0x2F4F4F),
			CGSizeMake(pkMapdata->getColumns() * pkMapdata->getUnitSize(),
					pkMapdata->getRows() * pkMapdata->getUnitSize()));

}

void NDMapSwitch::SetLableByType(int eLableType, int x, int y, const char* text,
		ccColor4B color1, ccColor4B color2, CGSize sizeParent)
{
	if (!text)
	{
		return;
	}

	float fScaleFactor = NDDirector::DefaultDirector()->GetScaleFactor();

	NDUILabel* pkLabels[2];
	memset(pkLabels, 0, sizeof(pkLabels));
	if (eLableType == 0)
	{
		if (!m_pkNameLabels[0])
		{
			m_pkNameLabels[0] = new NDUILabel;
			m_pkNameLabels[0]->Initialization();
		}

		if (!m_pkNameLabels[1])
		{
			m_pkNameLabels[1] = new NDUILabel;
			m_pkNameLabels[1]->Initialization();
		}

		pkLabels[0] = m_pkNameLabels[0];
		pkLabels[1] = m_pkNameLabels[1];
	}
	else if (eLableType == 1)
	{
		if (!m_pkDesLabels[0])
		{
			m_pkDesLabels[0] = new NDUILabel;
			m_pkDesLabels[0]->Initialization();
		}

		if (!m_pkDesLabels[1])
		{
			m_pkDesLabels[1] = new NDUILabel;
			m_pkDesLabels[1]->Initialization();
		}

		pkLabels[0] = m_pkDesLabels[0];
		pkLabels[1] = m_pkDesLabels[1];
	}

	if (!pkLabels[0] || !pkLabels[1])
	{
		return;
	}

	pkLabels[0]->SetText(text);
	pkLabels[1]->SetText(text);

	pkLabels[0]->SetFontColor(color1);
	pkLabels[1]->SetFontColor(color2);

	pkLabels[0]->SetFontSize(15);
	pkLabels[1]->SetFontSize(15);

	CGSize sizewin = NDDirector::DefaultDirector()->GetWinSize();

	pkLabels[1]->SetFrameRect(
			CGRectMake(x + 1, y + sizewin.height + 1 - sizeParent.height,
					sizewin.width, 20 * fScaleFactor));
	pkLabels[0]->SetFrameRect(
			CGRectMake(x, y + sizewin.height - sizeParent.height, sizewin.width,
					20 * fScaleFactor));
}

void NDMapSwitch::draw()
{
	if (m_pkNameLabels[1])
	{
		m_pkNameLabels[1]->draw();
	}

	if (m_pkNameLabels[0])
	{
		m_pkNameLabels[0]->draw();
	}

	if (m_pkDesLabels[1])
	{
		m_pkDesLabels[1]->draw();
	}

	if (m_pkDesLabels[0])
	{
		m_pkDesLabels[0]->draw();
	}
}

NDSceneTile::NDSceneTile() :
m_nOrderID(0)
{
}

NDMapMonsterRange::NDMapMonsterRange() :
		m_nTypeId(0), m_nColumn(0), m_nRow(0)
//, m_bBoss(false) ///< ��ʱ��ע�� ����
{
}

NDMapData::NDMapData() :
m_nLayerCount(0),
m_nColumns(0),
m_nRows(0),
m_nUnitSize(0),
m_nRoadBlockX(-1),
m_nRoadBlockY(-1),
//, m_MapTiles(NULL)
m_pkObstacles(NULL),
m_pkSceneTiles(NULL),
m_pkBackgroundTiles(NULL),
m_pkSwitchs(NULL),
m_pkAnimationGroups(NULL),
m_pkAniGroupParams(NULL)
{
}

NDMapData::~NDMapData()
{
	//CC_SAFE_RELEASE(m_MapTiles);
	CC_SAFE_DELETE (m_pkObstacles);
	CC_SAFE_RELEASE (m_pkSceneTiles);
	CC_SAFE_RELEASE (m_pkBackgroundTiles);
	CC_SAFE_RELEASE (m_pkSwitchs);
	CC_SAFE_RELEASE (m_pkAnimationGroups);
	CC_SAFE_RELEASE (m_pkAniGroupParams);
	MapTexturePool::defaultPool()->release();
}

/*ͨ����ͼ�ļ�(������·��)���ص�ͼ����
 ����:mapFile-��ͼ�ļ���
 */
void NDMapData::initWithFile(const char* mapFile)
{
	if (mapFile)
	{
		FILE* stream = fopen(mapFile, "rb");
		if (!stream)
		{
			return;
		}

		this->decode(stream);
		fclose(stream);
	}
}

/*  ��ͼ�ļ�����
 ����:��ͼ�ļ���
 */

void NDMapData::decode(FILE* pkStream)
{
	FileOp kFileOp;
	//<-------------------��ͼ��
	m_strName = kFileOp.readUTF8String(pkStream);	//[self readUTF8String:stream];
	CCLog(m_strName.c_str());
	//<-------------------��Ԫ��ߴ�
	m_nUnitSize = kFileOp.readByte(pkStream);
	int nTileWidth = m_nUnitSize;
	int nTileHeight = m_nUnitSize;
	//------------------->����
	m_nLayerCount = kFileOp.readByte(pkStream);
	//<-------------------����
	m_nColumns = kFileOp.readByte(pkStream);
	//------------------->����
	m_nRows = kFileOp.readByte(pkStream);
	//<-------------------ʹ�õ���ͼ����Դ
	std::vector < std::string > kTileImages;
	int nTileImageCount = kFileOp.readShort(pkStream);

	for (int i = 0; i < nTileImageCount; i++)
	{
		int nIDx = kFileOp.readShort(pkStream);
		char pszImageName[256] =
		{ 0 };
		sprintf(pszImageName, "%st%d.png",
				NDEngine::NDPath::GetImagePath().c_str(), nIDx);
		FILE* pkFile = fopen(pszImageName, "rt");

		if (pkFile)
		{
			kTileImages.push_back(pszImageName);
		}
		else
		{
			return;
		}
	}

	//------------------->��Ƭ	
	//m_MapTiles = new CCArray<CustomCCTexture2D*>();
	for (int lay = 0; lay < m_nLayerCount; lay++)
	{
		for (uint r = 0; r < m_nRows; r++)
		{
			for (uint c = 0; c < m_nColumns; c++)
			{
				int nImageIndex = kFileOp.readByte(pkStream) - 1;	//��Դ�±�

				if (nImageIndex == -1)
				{
					nImageIndex = 0;
				}

				int nTileIndex = kFileOp.readByte(pkStream);		//ͼ���±�
				bool reverse = kFileOp.readByte(pkStream) == 1;	//��ת

				if (nImageIndex == -1)
				{
					nImageIndex = 0;
					//continue;
				}

				if (kTileImages.size() > nImageIndex)
				{
					std::string imageName = kTileImages[nImageIndex];

					// 					CustomCCTexture2D *tile = new CustomCCTexture2D();
					// 					tile->setTexture(MapTexturePool::defaultPool()->addImage(imageName.c_str(), true));	//[[CCTextureCache sharedTextureCache] addImage:imageName keepData:true]; 
					// 					int PicParts	= tile->getTexture()->getPixelsWide() * tile->getTexture()->getMaxS() / TileWidth;
					// 					tile->setCutRect(CGRectMake(TileWidth*(tileIndex%PicParts), TileHeight*(tileIndex/PicParts), TileWidth, TileHeight));
					// 					tile->setDrawRect(CGRectMake(TileWidth*c, TileHeight*r, TileWidth, TileHeight));
					// 					tile->setHorizontalReverse(reverse);				
					// 					m_MapTiles->addObject(tile);
					// 					tile->release();			
				}
			}
		}
	}
	//<-------------------ͨ��
	m_pkObstacles = new std::vector<bool>();
	for (unsigned int i = 0; i < m_nColumns * m_nRows; i++)
	{
		m_pkObstacles->push_back(true);
	}

	int nNotPassCount = kFileOp.readShort(pkStream);

	for (int i = 0; i < nNotPassCount; i++)
	{
		int nRowIndex = kFileOp.readByte(pkStream);
		int nColumnIndex = kFileOp.readByte(pkStream);

		int nIndex = nRowIndex * m_nColumns + nColumnIndex;

 		if (m_pkObstacles->size() > nIndex)
 		{
 			(*m_pkObstacles)[nIndex] = true;//false ��ʱ�ԵĶ���Ϊtrue ����;
 		}
	}
	//------------------->����
	m_pkSwitchs = CCArray::array();
	m_pkSwitchs->retain();
	int nSwitchsCount = kFileOp.readByte(pkStream);

	for (int i = 0; i < nSwitchsCount; i++)
	{
		//NDMapSwitch *mapSwitch = [[NDMapSwitch alloc] init];

		/*mapSwitch.x = */kFileOp.readByte(pkStream); //������x
		/*mapSwitch.y = */kFileOp.readByte(pkStream); //������y
		/*mapSwitch.mapIndex = */kFileOp.readByte(pkStream); //Ŀ���ͼ
		/*mapSwitch.passIndex = */kFileOp.readByte(pkStream); //Ŀ���
		/*[mapSwitch SetLabel:self];*/

		/*[_switchs addObject:mapSwitch];*/
		/*[mapSwitch release];*/
	}
	//<---------------------ʹ�õ��ı�����Դ
	std::vector < std::string > kBackGroundImages;
	std::vector<int> _bgOrders;
	int bgImageCount = (kFileOp.readByte(pkStream) << 8)
			+ kFileOp.readByte(pkStream);

	for (int i = 0; i < bgImageCount; i++)
	{
		int idx = kFileOp.readShort(pkStream);
		char imageName[256] =
		{ 0 };
		sprintf(imageName, "%sb%d.png",
				NDEngine::NDPath::GetImagePath().c_str(), idx);
		FILE* pkFile = fopen(imageName, "rb");
		if (pkFile)
		{
			kBackGroundImages.push_back(imageName);
		}
		else
		{
			//NDLog("������Դ%@û���ҵ�!!!", imageName);
			kBackGroundImages.push_back(imageName);
		}

		int v = kFileOp.readShort(pkStream);
		_bgOrders.push_back(v);
	}
	//---------------------->����
	m_pkBackgroundTiles = CCArray::array();
	m_pkBackgroundTiles->retain();
	int bgCount = (kFileOp.readByte(pkStream) << 8)
			+ kFileOp.readByte(pkStream);

	for (int i = 0; i < bgCount; i++)
	{
		int nResourceIndex = kFileOp.readByte(pkStream);				//��Դ�±�
		int nX = kFileOp.readShort(pkStream);	//x����
		int nY = kFileOp.readShort(pkStream);	//y����
		bool nReverse = kFileOp.readByte(pkStream) > 0;						//��ת

		if (kBackGroundImages.size() <= nResourceIndex
				|| _bgOrders.size() <= nResourceIndex)
		{
			continue;
		}

		std::string imageName = kBackGroundImages[nResourceIndex];
		NDSceneTile* pkTile = new NDSceneTile;
		pkTile->setOrderID(_bgOrders[nResourceIndex] + nY);
		pkTile->setTexture(
				CCTextureCache::sharedTextureCache()->addImage(
						imageName.c_str()));
		int picWidth = pkTile->getTexture()->getPixelsWide()
				* pkTile->getTexture()->getMaxS();
		int picHeight = pkTile->getTexture()->getPixelsHigh()
				* pkTile->getTexture()->getMaxT();

		pkTile->setMapSize(
				CGSizeMake(m_nColumns * nTileWidth, m_nRows * nTileHeight));
		pkTile->setCutRect(CGRectMake(0, 0, picWidth, picHeight));
		pkTile->setDrawRect(CGRectMake(nX, nY, picWidth, picHeight));
		pkTile->setReverse(nReverse);

		pkTile->make();

		m_pkBackgroundTiles->addObject(pkTile);
		pkTile->release();
	}
	//<-------------------ʹ�õ��Ĳ�����Դ
	std::vector < std::string > kSceneImages;
	std::vector<int> kSceneOrders;
	int nSceneImageCount = (kFileOp.readByte(pkStream) << 8)
			+ kFileOp.readByte(pkStream);

	for (int i = 0; i < nSceneImageCount; i++)
	{
		int nIDx = kFileOp.readShort(pkStream);
		char szImageName[256] =
		{ 0 };
		sprintf(szImageName, "%ss%d.png",
				NDEngine::NDPath::GetImagePath().c_str(), nIDx);
		FILE* pkFile = fopen(szImageName, "rt");

		if (pkFile)
		{
			kSceneImages.push_back(szImageName);
		}
		else
		{
			//NDLog("������Դ%@û���ҵ�!!!", imageName);
			kSceneImages.push_back(szImageName);
		}

		int v = static_cast<short>(kFileOp.readShort(pkStream));
		kSceneOrders.push_back(v);
	}
	//------------------->����
	m_pkSceneTiles = CCArray::array();
	m_pkSceneTiles->retain();
	int nSceneCount = kFileOp.readShort(pkStream);

	for (int i = 0; i < nSceneCount; i++)
	{
		int nResourceIndex = kFileOp.readByte(pkStream);				//��Դ�±�
		int x = (short) ((kFileOp.readByte(pkStream) << 8)
				+ kFileOp.readByte(pkStream));	//x����
		int y = (short) ((kFileOp.readByte(pkStream) << 8)
				+ kFileOp.readByte(pkStream));	//y����
		BOOL bReverse = kFileOp.readByte(pkStream) > 0;						//��ת

		if (kSceneImages.size() <= nResourceIndex
				|| kSceneOrders.size() <= nResourceIndex)
		{
			continue;
		}

		NDSceneTile* pkTile = new NDSceneTile;
		pkTile->setOrderID(kSceneOrders[nResourceIndex] + y);

		std::string strImagePath = kSceneImages[nResourceIndex];

		pkTile->setTexture(
				CCTextureCache::sharedTextureCache()->addImage(
						strImagePath.c_str()));

		int nPicWidth = pkTile->getTexture()->getPixelsWide()
				* pkTile->getTexture()->getMaxS();
		int nPicHeight = pkTile->getTexture()->getPixelsHigh()
				* pkTile->getTexture()->getMaxT();

		pkTile->setMapSize(
				CGSizeMake(m_nColumns * nTileWidth, m_nRows * nTileHeight));
		pkTile->setCutRect(CGRectMake(0, 0, nPicWidth, nPicHeight));
		pkTile->setDrawRect(CGRectMake(x, y, nPicWidth, nPicHeight));
		pkTile->setReverse(bReverse);

		pkTile->make();

		m_pkSceneTiles->addObject(pkTile);
		pkTile->release();
	}
	//<-------------------����
	m_pkAnimationGroups = CCArray::array();
	m_pkAnimationGroups->retain();
	m_pkAniGroupParams = CCArray::array();
	m_pkAniGroupParams->retain();
	int m_AniGroupCount = kFileOp.readShort(pkStream);

	for (int i = 0; i < m_AniGroupCount; i++)
	{
		int nIdentifer = kFileOp.readShort(pkStream);			//����id
		NDAnimationGroup *pkAniGroup =
				NDAnimationGroupPool::defaultPool()->addObjectWithSceneAnimationId(
						nIdentifer);
		if (!pkAniGroup)
		{
			continue;
		}

		m_pkAnimationGroups->addObject(pkAniGroup);
		pkAniGroup->release();

		int x = kFileOp.readShort(pkStream);		//x����
		int y = kFileOp.readShort(pkStream);		//y����
		int nAniOrder = y + kFileOp.readShort(pkStream);	//��������

		anigroup_param* pkDict = new anigroup_param;

		pkDict->insert(std::make_pair("reverse", 0));
		pkDict->insert(std::make_pair("positionX", x));
		pkDict->insert(std::make_pair("positionY", y));
		pkDict->insert(std::make_pair("mapSizeW", m_nColumns * m_nUnitSize));
		pkDict->insert(std::make_pair("mapSizeH", m_nRows * m_nUnitSize));
		pkDict->insert(std::make_pair("orderId", nAniOrder));
		pkDict->insert(std::make_pair("reverse", 0));
		pkDict->insert(std::make_pair("reverse", 0));

		m_pkAniGroupParams->addObject(pkDict);
		pkDict->release();
	}
	//------------------->ˢ����
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

/*�ж�ĳ��λ���Ƿ����
 ����:row-ĳ��,column-ĳ��
 ����ֵ:YES/NO
 */
bool NDMapData::canPassByRow(unsigned int row, unsigned int column)
{
	if (column >= m_nColumns || row >= m_nRows)
	{
		return false;
	}

	if (row == m_nRoadBlockY)
	{
		return false;
	}

	if (column == m_nRoadBlockX)
	{
		return false;
	}

	int nIndex = row * m_nColumns + column;

	if (m_pkObstacles->size() <= nIndex)
	{
		return false;
	}

	return m_pkObstacles->at(nIndex);
}

void NDMapData::setRoadBlock(int x, int y)
{
	m_nRoadBlockX = x;
	m_nRoadBlockY = y;
}

NDSceneTile * NDMapData::getBackGroundTile(unsigned int index)
{
	if (m_pkBackgroundTiles->count() <= index)
	{
		return NULL;
	}
	else
	{
		return (NDSceneTile*) m_pkBackgroundTiles->objectAtIndex(index);
	}

	return NULL;
}

void NDMapData::moveBackGround(int x, int y)
{
	if (0 == x)
	{
		return;
	}

	for (int i = 0; i < 2; i++)
	{
		NDSceneTile* pkTile = this->getBackGroundTile(i);

		if (pkTile)
		{
			CGRect rect = pkTile->getDrawRect();
			rect.origin.x -= -x / 3;
			pkTile->setDrawRect(rect);
			pkTile->make();
		}
	}

}

//- (NDTile *)getTileAtRow:(uint)row column:(uint)column
// CustomCCTexture2D * NDMapData::getTileAtRow(unsigned int row, unsigned int column)
// {
// 	if (row >= _rows) 
// 	{
// 		return NULL;
// 	}
// 	if (column >= _columns) 
// 	{
// 		return NULL;
// 	}
// 	
// 	int index =	row * m_nColumns + column;
// 	if (index>=m_MapTiles->count()) 
// 	{
// 		return NULL;
// 	}
// 	else
// 	{
// 		return m_MapTiles->objectAtIndex(index);
// 	}
// }

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

	int nIndex = row * m_nColumns + column;
	if (m_pkObstacles->size() > nIndex)
	{
		(*m_pkObstacles)[nIndex] = false;
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

	int nIndex = row * m_nColumns + column;
	if (m_pkObstacles->size() > nIndex)
	{
		(*m_pkObstacles)[nIndex] = true;
	}
}

void NDMapData::addMapSwitch(unsigned int x,			// ������ x
		unsigned int y,			// ������ y
		unsigned int index,		// ����������
		unsigned int mapid,		// Ŀ���ͼid
		const char* name,	// Ŀ���ͼ����
		const char* desc)	// Ŀ���ͼ����
{
	NDMapSwitch* pkMapSwitch = new NDMapSwitch;

	pkMapSwitch->setX(x); //������x
	pkMapSwitch->setY(y); //������y
	pkMapSwitch->setMapIndex(mapid); //Ŀ���ͼ
	pkMapSwitch->setPassIndex(index); //Ŀ���
	pkMapSwitch->setNameDesMap(name);
	pkMapSwitch->setDescDesMap(desc);
	pkMapSwitch->SetLabelNew(this);

	m_pkSwitchs->addObject(pkMapSwitch);
	pkMapSwitch->release();
}
