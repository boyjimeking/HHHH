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
#include "ccTypes.h"

namespace NDEngine
{
typedef enum
{
	PictureRotation0,
	PictureRotation90,
	PictureRotation180,
	PictureRotation270
} PictureRotation;

class NDPicture: public NDObject
{
	DECLARE_CLASS (NDPicture)
	NDPicture(bool canGray = false);
	~NDPicture();
public:

	void Initialization(const char* imageFile);

	//void Initialization(vector<const char*>& vImgFiles); hide

	//void Initialization(vector<const char*>& vImgFiles, vector<CGRect>& vImgCustomRect, vector<CGPoint>&vOffsetPoint); hide

	void Initialization(const char* imageFile, int hrizontalPixel,
			int verticalPixel = 0);

	void Cut(CGRect rect);

	void SetReverse(bool reverse);

	void Rotation(PictureRotation rotation);

	void SetColor(cocos2d::ccColor4B color);

	void DrawInRect(CGRect rect);

	CGSize GetSize();

	NDPicture* Copy();

	bool SetGrayState(bool gray);

	bool IsGrayState();

public:
	cocos2d::CCTexture2D *GetTexture();

	void SetTexture(cocos2d::CCTexture2D* tex);
private:
	cocos2d::CCTexture2D *m_texture;
	CGRect m_cutRect;
	bool m_reverse, m_bAdvance;
	PictureRotation m_rotation;

	// ���
	bool m_canGray;
	bool m_stateGray;
	cocos2d::CCTexture2D *m_textureGray;

	GLfloat m_coordinates[8];
	GLubyte m_colors[16];
	GLfloat m_vertices[8];

	std::string m_strfile;
	int m_hrizontalPixel;
	int m_verticalPixel;

	void SetCoorinates();
	void SetVertices(CGRect drawRect);
};

class NDPictureDictionary: public NDDictionary
{
	DECLARE_CLASS (NDPictureDictionary)
public:
	void Recyle();
};

class NDPicturePool: public NDObject
{
	DECLARE_CLASS (NDPicturePool)
	NDPicturePool();
	~NDPicturePool();
public:
	static NDPicturePool* DefaultPool();

	static void PurgeDefaultPool();

	NDPicture* AddPicture(const char* imageFile, bool gray = false);
	NDPicture* AddPicture(const char* imageFile, int hrizontalPixel,
			int verticalPixel = 0, bool gray = false);

	void RemovePicture(const char* imageFile);

	void Recyle();

private:
	NDPictureDictionary *m_textures;

	std::map<std::string, CGSize> m_mapStr2Size;

private:
	CGSize GetImageSize(std::string filename);
};

}

#endif
