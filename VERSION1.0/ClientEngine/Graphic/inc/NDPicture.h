//
//  NDPicture.h
//  DragonDrive
//
//  Created by xiezhenghai on 11-1-8.
//  Copyright 2011 (����)DeNA. All rights reserved.
//
//	�������ܣ���
//	NDPicture��ר����Ա���ͼƬ�Ĳ���
//	�����Ҫ����ͼƬ��Դ����ͨ��NDPicturePool��ȡNDPicture����

#ifndef __NDPicture_H
#define __NDPicture_H

#include "NDObject.h"
#include "NDDictionary.h"
#include "CCTexture2D.h"
#include "CCGeometry.h"
#include "ccTypes.h"
#include "shaders/ccGLStateCache.h"
#include "shaders/ccGLProgram.h"
#include <vector>
#include <map>
#include <string>

using namespace cocos2d;
using namespace std;

NS_NDENGINE_BGN

typedef enum
{
	PictureRotation0,
	PictureRotation90,
	PictureRotation180,
	PictureRotation270
} PictureRotation;


//-------------------------------------------------------------------------
//ͼƬ��Դ����Ӧһ����ͼ��������ͼ�еĲü�����.
class NDPicture : public NDObject
{
	DECLARE_CLASS (NDPicture)
	NDPicture(bool canGray = false);
	~NDPicture();

public:
	CC_SYNTHESIZE(float,m_fScale,Scale);
	CC_SYNTHESIZE(bool,m_bIsTran,IsTran);

	//@shader
	CC_SYNTHESIZE_RETAIN(CCGLProgram*, m_pShaderProgram, ShaderProgram);
	CC_SYNTHESIZE(ccGLServerState, m_glServerState, GLServerState);

public:
	void Initialization(const char* imageFile);
	void Initialization(vector<const char*>& vImgFiles);
	void Initialization(vector<const char*>& vImgFiles, vector<CCRect>& vImgCustomRect, vector<CCPoint>&vOffsetPoint);
	void Initialization(const char* imageFile, int hrizontalPixel,int verticalPixel = 0);

	void DrawInRect(CCRect kRect);
	void Cut(CCRect kRect);

	void SetReverse(bool reverse);
	void Rotation(PictureRotation rotation);
	void SetColor(cocos2d::ccColor4B color);
	CCSize GetSize();
	
	bool SetGrayState(bool gray);
	bool IsGrayState();

public:
	cocos2d::CCTexture2D *GetTexture() { return m_pkTexture; }
	void SetTexture(cocos2d::CCTexture2D* tex);

public:
	NDPicture* Copy();

protected:
	void DrawSetup( const char* shaderType = kCCShader_PositionTexture_uColor );
	virtual void debugDraw();

private:
	void SetCoorinates();
	void SetVertices(CCRect drawRect);
	void destroy();

private:
	cocos2d::CCTexture2D* m_pkTexture;
	CCRect m_kCutRect;
	bool m_bReverse;
	bool m_bAdvance;
	PictureRotation m_kRotation;

	// ���
	bool m_bCanGray;
	bool m_bStateGray;
	cocos2d::CCTexture2D *m_pkTextureGray;

	GLfloat m_coordinates[8];
	GLubyte m_colors[16];
	GLfloat m_pfVertices[8];

	string m_strfile;
	int m_hrizontalPixel;
	int m_verticalPixel;
};


//-------------------------------------------------------------------------
class NDPictureDictionary: public NDDictionary
{
public:
	NDPictureDictionary();
	~NDPictureDictionary();
	DECLARE_CLASS (NDPictureDictionary)
public:
	void Recyle();
};


//-------------------------------------------------------------------------
class NDPicturePool : public NDObject
{
	DECLARE_CLASS (NDPicturePool)
	NDPicturePool();
	~NDPicturePool();

public:
	static NDPicturePool* DefaultPool();
	
public:
	NDPicture* AddPicture(const string& imageFile, bool gray = false) {
		return AddPicture(imageFile.c_str(), gray);
	}

	NDPicture* AddPicture(const string& imageFile, int hrizontalPixel, int verticalPixel = 0, bool gray = false) {
		return 	AddPicture(imageFile.c_str(), hrizontalPixel, verticalPixel, gray );
	}

public:
	NDPicture* AddPicture(const char* imageFile, bool gray = false);
	NDPicture* AddPicture(const char* imageFile, int hrizontalPixel,int verticalPixel = 0, bool gray = false);

public:
	void RemovePicture(const char* imageFile);
	void RemovePictureByTex(CCTexture2D* tex);

public:
	void PurgeDefaultPool();
	void Recyle()
	{
		if (m_pkPicturesDict)
			m_pkPicturesDict->Recyle();
	}

public:
	void dump();

private:
	NDPictureDictionary*			m_pkPicturesDict;	//����NDPicture*
	map<CCTexture2D*,string>		m_mapTexture;	//����CCTexture2D*

private: //����ͼƬ�ߴ�
	map<string, CCSize>	m_mapImageSize;
	CCSize GetImageSize( const string& filename );
};

NS_NDENGINE_END

#endif
