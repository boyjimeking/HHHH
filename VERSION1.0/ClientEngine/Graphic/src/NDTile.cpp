//
//  NDTile.m
//  DragonDrive
//
//  Created by xiezhenghai on 10-12-8.
//  Copyright 2010 (����)DeNA. All rights reserved.
//

#include "NDTile.h"

using namespace cocos2d;

static bool s_bTileHightLight = false;

void TileSetHightLight(bool bHightLight)
{
	s_bTileHightLight = bHightLight;
}

bool IsTileHightLight()
{
	return s_bTileHightLight;
}

NDTile::NDTile() :
m_pkTexture(NULL),
m_bReverse(false),
m_Rotation(NDRotationEnumRotation0),
m_pfVertices(NULL),
m_pfCoordinates(NULL)
{
	m_kCutRect = CGRectMake(0, 0, 0, 0);
	m_kDrawRect = CGRectMake(0, 0, 0, 0);
	m_MapSize = CGSizeMake(0, 0);

	m_pfCoordinates = (float *) malloc(sizeof(float) * 8);
	m_pfVertices = (float *) malloc(sizeof(float) * 12);
}

NDTile::~NDTile()
{
	free (m_pfCoordinates);
	free (m_pfVertices);
	CC_SAFE_FREE (m_pkTexture);
}

void NDTile::makeTex(float* pData)
{
	//<-------------------��������
	float *pc = pData;
	//BOOL re=NO;
	if (getReverse())
	{
		*pc++ = (m_kCutRect.origin.x + m_kCutRect.size.width)
				/ m_pkTexture->getPixelsWide();
		*pc++ = (m_kCutRect.origin.y + m_kCutRect.size.height)
				/ m_pkTexture->getPixelsHigh();
		*pc++ = m_kCutRect.origin.x / m_pkTexture->getPixelsWide();
		*pc++ = pData[1];
		*pc++ = pData[0];
		*pc++ = m_kCutRect.origin.y / m_pkTexture->getPixelsHigh();
		*pc++ = pData[2];
		*pc++ = pData[5];
	}
	else
	{
		*pc++ = m_kCutRect.origin.x / m_pkTexture->getPixelsWide();
		*pc++ = (m_kCutRect.origin.y + m_kCutRect.size.height)
				/ m_pkTexture->getPixelsHigh();
		*pc++ = (m_kCutRect.origin.x + m_kCutRect.size.width)
				/ m_pkTexture->getPixelsWide();
		*pc++ = pData[1];
		*pc++ = pData[0];
		*pc++ = m_kCutRect.origin.y / m_pkTexture->getPixelsHigh();
		*pc++ = pData[2];
		*pc++ = pData[5];
	}
}

void NDTile::makeVetex(float* pData, CGRect kRect)
{
	//--------------->��Ļ����
	float* pfVector = pData;
	//int r=NDRotationEnumRotation15;
	switch (m_Rotation)
	{
	case NDRotationEnumRotation0:
		*pfVector++ = kRect.origin.x;
		*pfVector++ = m_MapSize.height - kRect.origin.y - kRect.size.height;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x + kRect.size.width;
		*pfVector++ = pData[1];
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x;
		*pfVector++ = m_MapSize.height - kRect.origin.y;
		*pfVector++ = 0;
		*pfVector++ = pData[3];
		*pfVector++ = pData[7];
		*pfVector++ = 0;
		break;
	case NDRotationEnumRotation15:
		//			NDLog("15");
		*pfVector++ = kRect.origin.x;
		*pfVector++ = m_MapSize.height - kRect.origin.y - kRect.size.height * COS15;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x + COS15 * kRect.size.width;
		*pfVector++ = pData[1] - SIN15 * kRect.size.width;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x + SIN15 * kRect.size.height;
		*pfVector++ = m_MapSize.height - kRect.origin.y;
		*pfVector++ = 0;
		*pfVector++ = pData[6] + COS15 * kRect.size.width;
		*pfVector++ = m_MapSize.height - kRect.origin.y - SIN15 * kRect.size.width;
		*pfVector++ = 0;

		//			*pv++		=	rect.origin.x+COS75*rect.size.height;
		//			*pv++		=	m_MapSize.height - rect.origin.y - rect.size.height*SIN75-COS75*rect.size.width;
		//			*pv++		=	0;
		//			*pv++		=	m_vertices[0]+SIN75*rect.size.width;
		//			*pv++		=	m_MapSize.height - rect.origin.y-SIN75*rect.size.height;
		//			*pv++		=	0;
		//			*pv++		=	rect.origin.x;
		//			*pv++		=	m_MapSize.height - rect.origin.y-COS75*rect.size.width;
		//			*pv++		=	0;
		//			*pv++		=	rect.origin.x+SIN75*rect.size.width;
		//			*pv++		=	m_MapSize.height - rect.origin.y;
		//			*pv++		=	0;
		break;
	case NDRotationEnumRotation30:
		*pfVector++ = kRect.origin.x;
		*pfVector++ = m_MapSize.height - kRect.origin.y - kRect.size.height * COS30;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x + COS30 * kRect.size.width;
		*pfVector++ = pData[1] - SIN30 * kRect.size.width;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x + SIN30 * kRect.size.height;
		*pfVector++ = m_MapSize.height - kRect.origin.y;
		*pfVector++ = 0;
		*pfVector++ = pData[6] + COS30 * kRect.size.width;
		*pfVector++ = m_MapSize.height - kRect.origin.y - SIN30 * kRect.size.width;
		*pfVector++ = 0;
		break;
	case NDRotationEnumRotation45:
		*pfVector++ = kRect.origin.x;
		*pfVector++ = m_MapSize.height - kRect.origin.y - kRect.size.height * COS45;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x + COS45 * kRect.size.width;
		*pfVector++ = pData[1] - SIN45 * kRect.size.width;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x + SIN45 * kRect.size.height;
		*pfVector++ = m_MapSize.height - kRect.origin.y;
		*pfVector++ = 0;
		*pfVector++ = pData[6] + COS45 * kRect.size.width;
		*pfVector++ = m_MapSize.height - kRect.origin.y - SIN45 * kRect.size.width;
		*pfVector++ = 0;
		break;
	case NDRotationEnumRotation60:
		*pfVector++ = kRect.origin.x;
		*pfVector++ = m_MapSize.height - kRect.origin.y - kRect.size.height * COS60;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x + COS60 * kRect.size.width;
		*pfVector++ = pData[1] - SIN60 * kRect.size.width;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x + SIN60 * kRect.size.height;
		*pfVector++ = m_MapSize.height - kRect.origin.y;
		*pfVector++ = 0;
		*pfVector++ = pData[6] + COS60 * kRect.size.width;
		*pfVector++ = m_MapSize.height - kRect.origin.y - SIN60 * kRect.size.width;
		*pfVector++ = 0;
		break;
	case NDRotationEnumRotation75:
		*pfVector++ = kRect.origin.x;
		*pfVector++ = m_MapSize.height - kRect.origin.y - kRect.size.height * COS75;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x + COS75 * kRect.size.width;
		*pfVector++ = pData[1] - SIN75 * kRect.size.width;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x + SIN75 * kRect.size.height;
		*pfVector++ = m_MapSize.height - kRect.origin.y;
		*pfVector++ = 0;
		*pfVector++ = pData[6] + COS75 * kRect.size.width;
		*pfVector++ = m_MapSize.height - kRect.origin.y - SIN75 * kRect.size.width;
		*pfVector++ = 0;
		break;
	case NDRotationEnumRotation90:

		*pfVector++ = kRect.origin.x;
		*pfVector++ = m_MapSize.height - kRect.origin.y;
		*pfVector++ = 0;
		*pfVector++ = pData[0];
		*pfVector++ = pData[1] - kRect.size.width;
		*pfVector++ = 0;
		*pfVector++ = pData[0] + kRect.size.height;
		*pfVector++ = pData[1];
		*pfVector++ = 0;
		*pfVector++ = pData[6];
		*pfVector++ = pData[4];
		*pfVector++ = 0;
		break;
	case NDRotationEnumRotation105:
		//			NDLog("105");
		*pfVector++ = kRect.origin.x + SIN15 * kRect.size.width;
		*pfVector++ = m_MapSize.height - kRect.origin.y;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x;
		*pfVector++ = m_MapSize.height - kRect.origin.y - COS15 * kRect.size.width;
		*pfVector++ = 0;
		*pfVector++ = pData[0] + COS15 * kRect.size.height;
		*pfVector++ = pData[1] - SIN15 * kRect.size.height;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x + COS15 * kRect.size.height;
		*pfVector++ = pData[4] - SIN15 * kRect.size.height;
		*pfVector++ = 0;
		break;
	case NDRotationEnumRotation120:

		*pfVector++ = kRect.origin.x + SIN30 * kRect.size.width;
		*pfVector++ = m_MapSize.height - kRect.origin.y;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x;
		*pfVector++ = m_MapSize.height - kRect.origin.y - COS30 * kRect.size.width;
		*pfVector++ = 0;
		*pfVector++ = pData[0] + COS30 * kRect.size.height;
		*pfVector++ = pData[1] - SIN30 * kRect.size.height;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x + COS30 * kRect.size.height;
		*pfVector++ = pData[4] - SIN30 * kRect.size.height;
		*pfVector++ = 0;
		break;
	case NDRotationEnumRotation135:

		*pfVector++ = kRect.origin.x + SIN45 * kRect.size.width;
		*pfVector++ = m_MapSize.height - kRect.origin.y;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x;
		*pfVector++ = m_MapSize.height - kRect.origin.y - COS45 * kRect.size.width;
		*pfVector++ = 0;
		*pfVector++ = pData[0] + COS45 * kRect.size.height;
		*pfVector++ = pData[1] - SIN45 * kRect.size.height;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x + COS45 * kRect.size.height;
		*pfVector++ = pData[4] - SIN45 * kRect.size.height;
		*pfVector++ = 0;
		break;
	case NDRotationEnumRotation150:

		*pfVector++ = kRect.origin.x + SIN60 * kRect.size.width;
		*pfVector++ = m_MapSize.height - kRect.origin.y;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x;
		*pfVector++ = m_MapSize.height - kRect.origin.y - COS60 * kRect.size.width;
		*pfVector++ = 0;
		*pfVector++ = pData[0] + COS60 * kRect.size.height;
		*pfVector++ = pData[1] - SIN60 * kRect.size.height;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x + COS60 * kRect.size.height;
		*pfVector++ = pData[4] - SIN60 * kRect.size.height;
		*pfVector++ = 0;
		break;
	case NDRotationEnumRotation165:

		*pfVector++ = kRect.origin.x + SIN75 * kRect.size.width;
		*pfVector++ = m_MapSize.height - kRect.origin.y;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x;
		*pfVector++ = m_MapSize.height - kRect.origin.y - COS75 * kRect.size.width;
		*pfVector++ = 0;
		*pfVector++ = pData[0] + COS75 * kRect.size.height;
		*pfVector++ = pData[1] - SIN75 * kRect.size.height;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x + COS75 * kRect.size.height;
		*pfVector++ = pData[4] - SIN75 * kRect.size.height;
		*pfVector++ = 0;
		break;
	case NDRotationEnumRotation180:
		*pfVector++ = kRect.origin.x + kRect.size.width;
		*pfVector++ = m_MapSize.height - kRect.origin.y;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x;
		*pfVector++ = pData[1];
		*pfVector++ = 0;
		*pfVector++ = pData[0];
		*pfVector++ = m_MapSize.height - kRect.origin.y - kRect.size.height;
		*pfVector++ = 0;
		*pfVector++ = pData[3];
		*pfVector++ = pData[7];
		*pfVector++ = 0;
		break;
	case NDRotationEnumRotation195:
		*pfVector++ = kRect.origin.x + COS15 * kRect.size.width
				+ SIN15 * kRect.size.height;
		*pfVector++ = m_MapSize.height - kRect.origin.y - SIN15 * kRect.size.width;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x + SIN15 * kRect.size.height;
		*pfVector++ = m_MapSize.height - kRect.origin.y;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x + COS15 * kRect.size.width;
		*pfVector++ = pData[1] - COS15 * kRect.size.height;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x;
		*pfVector++ = m_MapSize.height - kRect.origin.y - COS15 * kRect.size.height;
		*pfVector++ = 0;
		break;
	case NDRotationEnumRotation210:
		*pfVector++ = kRect.origin.x + COS30 * kRect.size.width
				+ SIN30 * kRect.size.height;
		*pfVector++ = m_MapSize.height - kRect.origin.y - SIN30 * kRect.size.width;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x + SIN30 * kRect.size.height;
		*pfVector++ = m_MapSize.height - kRect.origin.y;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x + COS30 * kRect.size.width;
		*pfVector++ = pData[1] - COS30 * kRect.size.height;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x;
		*pfVector++ = m_MapSize.height - kRect.origin.y - COS30 * kRect.size.height;
		*pfVector++ = 0;
		break;
	case NDRotationEnumRotation225:
		*pfVector++ = kRect.origin.x + COS45 * kRect.size.width
				+ SIN45 * kRect.size.height;
		*pfVector++ = m_MapSize.height - kRect.origin.y - SIN45 * kRect.size.width;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x + SIN45 * kRect.size.height;
		*pfVector++ = m_MapSize.height - kRect.origin.y;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x + COS45 * kRect.size.width;
		*pfVector++ = pData[1] - COS45 * kRect.size.height;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x;
		*pfVector++ = m_MapSize.height - kRect.origin.y - COS45 * kRect.size.height;
		*pfVector++ = 0;
		break;
	case NDRotationEnumRotation240:
		*pfVector++ = kRect.origin.x + COS60 * kRect.size.width
				+ SIN60 * kRect.size.height;
		*pfVector++ = m_MapSize.height - kRect.origin.y - SIN60 * kRect.size.width;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x + SIN60 * kRect.size.height;
		*pfVector++ = m_MapSize.height - kRect.origin.y;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x + COS60 * kRect.size.width;
		*pfVector++ = pData[1] - COS60 * kRect.size.height;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x;
		*pfVector++ = m_MapSize.height - kRect.origin.y - COS60 * kRect.size.height;
		*pfVector++ = 0;
		break;
	case NDRotationEnumRotation255:
		*pfVector++ = kRect.origin.x + COS75 * kRect.size.width
				+ SIN75 * kRect.size.height;
		*pfVector++ = m_MapSize.height - kRect.origin.y - SIN75 * kRect.size.width;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x + SIN75 * kRect.size.height;
		*pfVector++ = m_MapSize.height - kRect.origin.y;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x + COS75 * kRect.size.width;
		*pfVector++ = pData[1] - COS75 * kRect.size.height;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x;
		*pfVector++ = m_MapSize.height - kRect.origin.y - COS75 * kRect.size.height;
		*pfVector++ = 0;
		break;
	case NDRotationEnumRotation270:
		*pfVector++ = kRect.origin.x + kRect.size.height;
		*pfVector++ = m_MapSize.height - (kRect.origin.y + kRect.size.width);
		*pfVector++ = 0;
		*pfVector++ = pData[0];
		*pfVector++ = pData[1] + kRect.size.width;
		*pfVector++ = 0;
		*pfVector++ = pData[0] - kRect.size.height;
		*pfVector++ = pData[1];
		*pfVector++ = 0;
		*pfVector++ = pData[6];
		*pfVector++ = pData[4];
		*pfVector++ = 0;
		break;
	case NDRotationEnumRotation285:
		*pfVector++ = kRect.origin.x + COS15 * kRect.size.height;
		*pfVector++ = m_MapSize.height - kRect.origin.y - kRect.size.height * SIN15
				- COS15 * kRect.size.width;
		*pfVector++ = 0;
		*pfVector++ = pData[0] + SIN15 * kRect.size.width;
		*pfVector++ = m_MapSize.height - kRect.origin.y - SIN15 * kRect.size.height;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x;
		*pfVector++ = m_MapSize.height - kRect.origin.y - COS15 * kRect.size.width;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x + SIN15 * kRect.size.width;
		*pfVector++ = m_MapSize.height - kRect.origin.y;
		*pfVector++ = 0;
		break;
	case NDRotationEnumRotation300:
		*pfVector++ = kRect.origin.x + COS30 * kRect.size.height;
		*pfVector++ = m_MapSize.height - kRect.origin.y - kRect.size.height * SIN30
				- COS30 * kRect.size.width;
		*pfVector++ = 0;
		*pfVector++ = pData[0] + SIN30 * kRect.size.width;
		*pfVector++ = m_MapSize.height - kRect.origin.y - SIN30 * kRect.size.height;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x;
		*pfVector++ = m_MapSize.height - kRect.origin.y - COS30 * kRect.size.width;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x + SIN30 * kRect.size.width;
		*pfVector++ = m_MapSize.height - kRect.origin.y;
		*pfVector++ = 0;
		break;
	case NDRotationEnumRotation315:
		*pfVector++ = kRect.origin.x + COS45 * kRect.size.height;
		*pfVector++ = m_MapSize.height - kRect.origin.y - kRect.size.height * SIN45
				- COS45 * kRect.size.width;
		*pfVector++ = 0;
		*pfVector++ = pData[0] + SIN45 * kRect.size.width;
		*pfVector++ = m_MapSize.height - kRect.origin.y - SIN45 * kRect.size.height;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x;
		*pfVector++ = m_MapSize.height - kRect.origin.y - COS45 * kRect.size.width;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x + SIN45 * kRect.size.width;
		*pfVector++ = m_MapSize.height - kRect.origin.y;
		*pfVector++ = 0;
		break;
	case NDRotationEnumRotation330:
		*pfVector++ = kRect.origin.x + COS60 * kRect.size.height;
		*pfVector++ = m_MapSize.height - kRect.origin.y - kRect.size.height * SIN60
				- COS60 * kRect.size.width;
		*pfVector++ = 0;
		*pfVector++ = pData[0] + SIN60 * kRect.size.width;
		*pfVector++ = m_MapSize.height - kRect.origin.y - SIN60 * kRect.size.height;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x;
		*pfVector++ = m_MapSize.height - kRect.origin.y - COS60 * kRect.size.width;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x + SIN60 * kRect.size.width;
		*pfVector++ = m_MapSize.height - kRect.origin.y;
		*pfVector++ = 0;
		break;
	case NDRotationEnumRotation345:
		//			NDLog("345");
		*pfVector++ = kRect.origin.x + COS75 * kRect.size.height;
		*pfVector++ = m_MapSize.height - kRect.origin.y - kRect.size.height * SIN75
				- COS75 * kRect.size.width;
		*pfVector++ = 0;
		*pfVector++ = pData[0] + SIN75 * kRect.size.width;
		*pfVector++ = m_MapSize.height - kRect.origin.y - SIN75 * kRect.size.height;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x;
		*pfVector++ = m_MapSize.height - kRect.origin.y - COS75 * kRect.size.width;
		*pfVector++ = 0;
		*pfVector++ = kRect.origin.x + SIN75 * kRect.size.width;
		*pfVector++ = m_MapSize.height - kRect.origin.y;
		*pfVector++ = 0;
		break;
	default:
		break;
	}
}

void NDTile::make()
{
	this->makeTex(m_pfCoordinates);
	this->makeVetex(m_pfVertices, m_kDrawRect);
}

static GLbyte tileColors[] =
		{ 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
				255, 255 };

static GLbyte tileHightLightColors[] =
		{ 255, 255, 255, 125, 255, 255, 255, 125, 255, 255, 255, 125, 255, 255,
				255, 125 };

void NDTile::draw()
{
	if (m_pkTexture)
	{
		glBindTexture(GL_TEXTURE_2D, m_pkTexture->getName());		//������
		glVertexPointer(3, GL_FLOAT, 0, m_pfVertices);		//��Ŀ��λ������
		glColorPointer(4, GL_UNSIGNED_BYTE, 0,
				s_bTileHightLight ? tileHightLightColors : tileColors);
		glTexCoordPointer(2, GL_FLOAT, 0, m_pfCoordinates);	//����Ƭ����
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);				//��opengl��ϻ�ͼ
	}
}

void NDTile::drawSubRect(CGRect rect)
{
	/**��������*/
	float coordinates[8];
	float *pc = coordinates;

	float xl = m_kCutRect.origin.x + m_kCutRect.size.width * rect.origin.x;
	float xr = m_kCutRect.origin.x
			+ m_kCutRect.size.width * (rect.size.width + rect.origin.x);
	float yt = m_kCutRect.origin.y + m_kCutRect.size.height * rect.origin.y;
	float yb = m_kCutRect.origin.y
			+ m_kCutRect.size.height * (rect.size.height + rect.origin.y);

	//BOOL re=NO;
	if (getReverse())
	{
		*pc++ = xr / m_pkTexture->getPixelsWide();
		*pc++ = yb / m_pkTexture->getPixelsHigh();
		*pc++ = xl / m_pkTexture->getPixelsWide();
		*pc++ = coordinates[1];
		*pc++ = coordinates[0];
		*pc++ = yt / m_pkTexture->getPixelsHigh();
		*pc++ = coordinates[2];
		*pc++ = coordinates[5];
	}
	else
	{
		*pc++ = xl / m_pkTexture->getPixelsWide();
		*pc++ = yb / m_pkTexture->getPixelsHigh();
		*pc++ = xr / m_pkTexture->getPixelsWide();
		*pc++ = coordinates[1];
		*pc++ = coordinates[0];
		*pc++ = yt / m_pkTexture->getPixelsHigh();
		*pc++ = coordinates[2];
		*pc++ = coordinates[5];
	}

	float vertices[12] =
	{ 0.0f };

	CGRect kDrawRect;
	kDrawRect.origin.x = m_kDrawRect.origin.x
			+ rect.origin.x * m_kDrawRect.size.width;
	kDrawRect.origin.y = m_kDrawRect.origin.y
			+ rect.origin.y * m_kDrawRect.size.height;
	kDrawRect.size.width = rect.size.width * m_kDrawRect.size.width;
	kDrawRect.size.height = rect.size.height * m_kDrawRect.size.height;

	this->makeVetex(vertices, kDrawRect);

	if (m_pkTexture)
	{
		glBindTexture(GL_TEXTURE_2D, m_pkTexture->getName());		//������

		glVertexPointer(3, GL_FLOAT, 0, vertices);		//��Ŀ��λ������

		glColorPointer(4, GL_UNSIGNED_BYTE, 0, tileColors);

		glTexCoordPointer(2, GL_FLOAT, 0, coordinates);	//����Ƭ����

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);				//��opengl��ϻ�ͼ
	}
}
