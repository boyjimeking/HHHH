//
//  NDWorldMapData.mm
//  DragonDrive
//
//  Created by xiezhenghai on 11-5-24.
//  Copyright 2011 (����)DeNA. All rights reserved.
//

#include "NDWorldMapData.h"
#include "JavaMethod.h"
#include "NDPath.h"
#include "CCTextureCache.h"
#include "NDAnimationGroupPool.h"
#include "NDPicture.h"
#include "BaseType.h"
#include "NDDirector.h"
#include "ObjectTracker.h"

using namespace cocos2d;
using namespace NDEngine;

PlaceNode::PlaceNode() :
	m_Texture(NULL),
	m_nPlaceId(0),
	m_nX(0),
	m_nY(0),
	m_nLDir(0),
	m_nRDir(0),
	m_nTDir(0),
	m_nBDir(0)
{
	INC_NDOBJ("PlaceNode");
}

PlaceNode::~PlaceNode()
{
	DEC_NDOBJ("PlaceNode");

	CC_SAFE_RELEASE (m_Texture);
}

#define IMAGE_PATH	[NSString stringWithUTF8String:NDEngine::NDPath::GetImagePath().c_str()]

const int TileWidth = 16;
const int TileHeight = 16;

std::vector<PassWay> m_passWayInfos;



///////////////////////////////////////////////////////////////////
NDWorldMapData::NDWorldMapData() :
	m_nLayerCount(0),
	m_nColumns(0),
	m_nRows(0),
	//m_nUnitSize(0),
	m_MapTiles(NULL),
	m_SceneTiles(NULL),
	m_BgTiles(NULL),
	m_AnimationGroups(NULL),
	m_AniGroupParams(NULL),
	m_PlaceNodes(NULL),
	m_init(false)
{
	INC_NDOBJ("NDWorldMapData");

	m_MapSize = CCSizeZero;
}

NDWorldMapData& NDWorldMapData::instance()
{
	static NDWorldMapData* s_obj = NULL;
	
	if (s_obj == NULL)
	{
		s_obj = new NDWorldMapData;
	}

	s_obj->init();
	return *s_obj;
}

void NDWorldMapData::init()
{
	if (!getInit())
	{
		std::string mapFile = NDPath::GetMapPath() + "map_99999.map";	
		this->initWithFile(mapFile.c_str());
		this->setInit( true );
	}
}

NDWorldMapData::~NDWorldMapData()
{
	DEC_NDOBJ("NDWorldMapData");
}

void NDWorldMapData::destroy()
{
	CC_SAFE_RELEASE (m_MapTiles);
	CC_SAFE_RELEASE (m_SceneTiles);
	CC_SAFE_RELEASE (m_BgTiles);
	CC_SAFE_RELEASE (m_AnimationGroups);
	CC_SAFE_RELEASE (m_AniGroupParams);
	CC_SAFE_RELEASE (m_PlaceNodes);

	this->setInit( false );
}


/*ͨ����ͼ�ļ�(������·��)���ص�ͼ����
 ����:mapFile-��ͼ�ļ���
 */
void NDWorldMapData::initWithFile(const char* mapFile)
{
	FILE* stream = fopen(mapFile, "rb");
	if (stream)
	{
		this->decode(stream);
		fclose(stream);
	}
}

/*  ��ͼ�ļ�����
 ����:��ͼ�ļ���
 */
void NDWorldMapData::decode(FILE* stream)
{
	FileOp kFileOp;
	
	//<-------------------��ͼ��
	m_Name = kFileOp.readUTF8String(stream); //[self readUTF8String:stream];
	
	//<-------------------��Ԫ��ߴ�
	int nUnitSize_DontUseIt = kFileOp.readByte(stream); //��ʹ�ã�
// 	int TileWidth = m_nUnitSize;
// 	int TileHeight = m_nUnitSize; //@del
	int TileWidth = MAP_UNITSIZE_X;
	int TileHeight = MAP_UNITSIZE_Y;

	//------------------->����
	m_nLayerCount = kFileOp.readByte(stream);

	//<-------------------����
	m_nColumns = kFileOp.readByte(stream);
	
	//------------------->����
	m_nRows = kFileOp.readByte(stream);
	
	//m_MapSize = CCSizeMake(m_nColumns << 5, m_nRows << 5);
	m_MapSize = CCSizeMake( m_nColumns * MAP_UNITSIZE_X, m_nRows * MAP_UNITSIZE_Y ); //@android
	
	//<-------------------ʹ�õ���ͼ����Դ
	std::vector < std::string > _tileImages;
	int tileImageCount = kFileOp.readShort(stream);
	for (int i = 0; i < tileImageCount; i++)
	{
		int idx = kFileOp.readShort(stream);
		char imageName[256] =
		{ 0 };
		sprintf(imageName, "%st%d.png",
				NDEngine::NDPath::GetImagePath().c_str(), idx);
		FILE* f = fopen(imageName, "rb");
		if (f)
		{
			fclose(f);
			_tileImages.push_back(imageName);
		}
		else
		{
			return;
		}
	}

	//------------------->��Ƭ	
	m_MapTiles = CCArray::array();
	m_MapTiles->retain();
	for (int lay = 0; lay < m_nLayerCount; lay++)
	{
		for (unsigned int r = 0; r < m_nRows; r++)
		{
			for (unsigned int c = 0; c < m_nColumns; c++)
			{
				int imageIndex = kFileOp.readByte(stream) - 1;	//��Դ�±�
				if (imageIndex == -1)
					imageIndex = 0;
				int tileIndex = kFileOp.readByte(stream);		//ͼ���±�
				bool reverse = kFileOp.readByte(stream) == 1;	//��ת

				if (imageIndex == -1)
				{
					imageIndex = 0;
					//continue;
				}

				if (_tileImages.size() > imageIndex)
				{
					std::string imageName = _tileImages[imageIndex];

					NDTile *pkTile = new NDTile();
					pkTile->setMapSize(m_MapSize);

					pkTile->setTexture(
							NDMapTexturePool::defaultPool()->addImage(
									imageName.c_str(), true));

					int PicParts = pkTile->getTexture()->getPixelsWide()
							* pkTile->getTexture()->getMaxS() / TileWidth;

					pkTile->setCutRect(
						CCRectMake(TileWidth * (tileIndex % PicParts),
						TileHeight * (tileIndex / PicParts),
						TileWidth, TileHeight));

					pkTile->setDrawRect(
						CCRectMake(TileWidth * c, TileHeight * r, TileWidth,
						TileHeight));

					//tile->setHorizontalReverse(reverse);				
					m_MapTiles->addObject(pkTile);
					pkTile->release();
				}
			}
		}
	}
	//<---------------------ʹ�õ��ı�����Դ
	std::vector < std::string > kImages;
	std::vector<int> kOrders;
	int bgImageCount = kFileOp.readShort(stream);
	for (int i = 0; i < bgImageCount; i++)
	{
		int idx = kFileOp.readShort(stream);
		char imageName[256] =
		{ 0 };
		sprintf(imageName, "%sb%d.png",
				NDEngine::NDPath::GetImagePath().c_str(), idx);
		FILE* f = fopen(imageName, "rb");
		if (f)
		{
			fclose(f);
			kImages.push_back(imageName);
		}
		else
		{
			//NDLog("������Դ%@û���ҵ�!!!", imageName);
			kImages.push_back(imageName);
		}

		int v = kFileOp.readShort(stream);
		kOrders.push_back(v);
	}
	//---------------------->����
	m_BgTiles = CCArray::array();
	m_BgTiles->retain();
	int bgCount = kFileOp.readShort(stream);
	for (int i = 0; i < bgCount; i++)
	{
		int resourceIndex = kFileOp.readByte(stream);					//��Դ�±�
		int x = kFileOp.readShort(stream);	//x����
		int y = kFileOp.readShort(stream);	//y����
		bool reverse = kFileOp.readByte(stream) > 0;						//��ת

		if (kImages.size() <= resourceIndex || kOrders.size() <= resourceIndex)
		{
			continue;
		}

		std::string imageName = kImages[resourceIndex];
		NDSceneTile *pkTile = new NDSceneTile;
		pkTile->setOrderID(kOrders[resourceIndex] + y);
		pkTile->setTexture(
				CCTextureCache::sharedTextureCache()->addImage(
						imageName.c_str()));

		int picWidth = pkTile->getTexture()->getPixelsWide()
				* pkTile->getTexture()->getMaxS();

		int picHeight = pkTile->getTexture()->getPixelsHigh()
				* pkTile->getTexture()->getMaxT();

		pkTile->setMapSize( CCSizeMake(m_nColumns * TileWidth, m_nRows * TileHeight));
		pkTile->setCutRect(CCRectMake(0, 0, picWidth, picHeight)); 
		pkTile->SetDrawRect(CCRectMake(x, y, picWidth, picHeight));//@android
		pkTile->make();
		m_BgTiles->addObject(pkTile);
		pkTile->release();
	}
	//<-------------------ʹ�õ��Ĳ�����Դ
	std::vector < std::string > _sceneImages;
	std::vector<int> _sceneOrders;
	int sceneImageCount = kFileOp.readShort(stream);
	for (int i = 0; i < sceneImageCount; i++)
	{
		int idx = kFileOp.readShort(stream);
		char imageName[256] =
		{ 0 };
		sprintf(imageName, "%ss%d.png", NDEngine::NDPath::GetImagePath().c_str(),
				idx);
		FILE* f = fopen(imageName, "rb");
		if (f)
		{
			fclose(f);
			_sceneImages.push_back(imageName);
		}
		else
		{
			//NDLog("������Դ%@û���ҵ�!!!", imageName);
			_sceneImages.push_back(imageName);
		}

		int v = kFileOp.readShort(stream);
		_sceneOrders.push_back(v);
	}
	//------------------->����
	m_SceneTiles = CCArray::array();
	m_SceneTiles->retain();
	int sceneCount = kFileOp.readShort(stream);
	for (int i = 0; i < sceneCount; i++)
	{
		int resourceIndex = kFileOp.readByte(stream);					//��Դ�±�
		int x = kFileOp.readShort(stream);	//x����
		int y = kFileOp.readShort(stream);	//y����
		BOOL reverse = kFileOp.readByte(stream) > 0;						//��ת

		if (_sceneImages.size() <= resourceIndex
				|| _sceneOrders.size() <= resourceIndex)
		{
			continue;
		}

		NDSceneTile *pkTile = new NDSceneTile;
		pkTile->setOrderID(_sceneOrders[resourceIndex] + y);

		//ע�����ü���
		NDPicture *pTile_pic = NDPicturePool::DefaultPool()->AddPicture(_sceneImages[resourceIndex].c_str());
		pkTile->setTexture(pTile_pic->GetTexture());
		delete pTile_pic;
				
		int picWidth = pkTile->getTexture()->getPixelsWide()
				* pkTile->getTexture()->getMaxS();

		int picHeight = pkTile->getTexture()->getPixelsHigh()
				* pkTile->getTexture()->getMaxT();

		pkTile->setMapSize( CCSizeMake(m_nColumns * TileWidth, m_nRows * TileHeight));
		pkTile->setCutRect( CCRectMake(0, 0, picWidth, picHeight)); 
		pkTile->SetDrawRect( CCRectMake(x, y, picWidth, picHeight)); //@android
		pkTile->setReverse(reverse);
		pkTile->make();

		m_SceneTiles->addObject(pkTile);
		pkTile->release();
	}
	//<-------------------����
	m_AnimationGroups = CCArray::array();
	m_AniGroupParams = CCArray::array();
	m_AnimationGroups->retain();
	m_AniGroupParams->retain();

	int aniGroupCount = kFileOp.readShort(stream);
	for (int i = 0; i < aniGroupCount; i++)
	{
		int nIdentifer = kFileOp.readShort(stream);			//����id
		NDAnimationGroup *aniGroup =
				NDAnimationGroupPool::defaultPool()->addObjectWithSceneAnimationId(
						nIdentifer);

		if (!aniGroup)
		{
			continue;
		}

		m_AnimationGroups->addObject(aniGroup);
		aniGroup->release();

		int x = kFileOp.readShort(stream);		//x����
		int y = kFileOp.readShort(stream);		//y����
		int aniOrder = y + kFileOp.readShort(stream);	//��������

		anigroup_param* dict = new anigroup_param;
		dict->insert(std::make_pair("reverse", 0));
		dict->insert(std::make_pair("positionX", x));
		dict->insert(std::make_pair("positionY", y));
		dict->insert(std::make_pair("mapSizeW", int(m_nColumns * MAP_UNITSIZE_X)));
		dict->insert(std::make_pair("mapSizeH", int(m_nRows * MAP_UNITSIZE_Y)));
		dict->insert(std::make_pair("orderId", aniOrder));
		dict->insert(std::make_pair("reverse", 0));
		dict->insert(std::make_pair("reverse", 0));

		m_AniGroupParams->addObject(dict);
		dict->release();
	}
	//------------------->�ڵ�
	m_PlaceNodes = CCArray::array();
	m_PlaceNodes->retain();

	int nodeCount = kFileOp.readByte(stream);
	for (int i = 0; i < nodeCount; i++)
	{
		PlaceNode *pkPlaceNode = new PlaceNode;

		pkPlaceNode->setPlaceId(kFileOp.readShort(stream));
		int imageIndex = kFileOp.readShort(stream);

		char imageName[256] =
		{ 0 };

		sprintf(imageName, "%so%d.png",
				NDEngine::NDPath::GetImage00Path().c_str(), imageIndex);

		FILE* pkFile = fopen(imageName, "rb");
		if (pkFile)
		{
			fclose(pkFile);

			//ע�����ü���
			NDPicture *pTile_pic = NDPicturePool::DefaultPool()->AddPicture(imageName);
			pkPlaceNode->setTexture(pTile_pic->GetTexture());
			delete pTile_pic;

			//pkPlaceNode->setTexture(CCTextureCache::sharedTextureCache()->addImage(imageName));
		}

		pkPlaceNode->setX(kFileOp.readShort(stream));
		pkPlaceNode->setY(kFileOp.readShort(stream));
		pkPlaceNode->setLDir(kFileOp.readShort(stream));
		pkPlaceNode->setRDir(kFileOp.readShort(stream));
		pkPlaceNode->setTDir(kFileOp.readShort(stream));
		pkPlaceNode->setBDir(kFileOp.readShort(stream));
		pkPlaceNode->setName(kFileOp.readUTF8String(stream));
		pkPlaceNode->setDescription(kFileOp.readUTF8String(stream));

		m_PlaceNodes->addObject(pkPlaceNode);
		pkPlaceNode->release();
	}
}

NDTile * NDWorldMapData::getTileAtRow(unsigned int row, unsigned int column)
{
	if (row >= m_nRows)
	{
		return NULL;
	}
	if (column >= m_nColumns)
	{
		return NULL;
	}

	int index = row * m_nColumns + column;

	return (NDTile *) m_MapTiles->objectAtIndex(index);
}

int NDWorldMapData::getDestMapIdWithSourceMapId(int mapId, int passwayIndex)
{
	for (std::vector<PassWay>::iterator it = m_passWayInfos.begin();
			it != m_passWayInfos.end(); it++)
	{
		PassWay& pw = *it;
		if (pw.fromMapID == mapId && pw.fromPassIndex == passwayIndex)
		{
			return pw.desMapID;
		}
	}

	return 0;
}

PlaceNode * NDWorldMapData::getPlaceNodeWithMapId(int mapId)
{
	PlaceNode* result = NULL;
	for (unsigned int i = 0; i < m_PlaceNodes->count(); i++)
	{
		PlaceNode *node = (PlaceNode *) m_PlaceNodes->objectAtIndex(i);
		if (node->getPlaceId() == mapId)
		{
			result = node;
			break;
		}
	}
	return result;
}
