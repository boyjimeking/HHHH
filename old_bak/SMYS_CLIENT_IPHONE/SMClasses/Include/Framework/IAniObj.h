/*
 *  IAni.h
 *  Copyright 2010 TQ Digital Entertainment. All rights reserved.
 */

#ifndef _I_ANI_H
#define _I_ANI_H

#include "BaseType.h"
#include "uitypes.h"


enum
{
	IMAGE_SHOWWAY_NORMAL  = 0,
	IMAGE_SHOWWAY_ADDITIVE = 1,
};

//ͼƬ�ӿ�
class IImageObj
{
public:
	virtual ~IImageObj(){};
	
	virtual void Show(int x, int y, int nAlpha = 0, DWORD dwShowWay = IMAGE_SHOWWAY_NORMAL) = 0;
	
	//���ô˽ӿ���ʾ���꽫���浱ǰ����λ����ʾ�����������꣬����������Ϊ��ʾ���꣬����ͼ���ꡣ
	virtual void ShowRelative(int x, int y, int nAlpha = 0, DWORD dwShowWay = IMAGE_SHOWWAY_NORMAL) = 0;

	virtual void ShowEx(int x, int y, CRect *lpSrc, DWORD dwWidth, DWORD dwHeight, int nAlpha = 0, DWORD dwShowWay = IMAGE_SHOWWAY_NORMAL, int nRotateAngle = 0) = 0;

	//���ô˽ӿ���ʾ���꽫���浱ǰ����λ����ʾ�����������꣬����������Ϊ��ʾ���꣬����ͼ���ꡣ
	virtual void ShowExRelative(int x, int y, CRect *lpSrc, DWORD dwWidth, DWORD dwHeight, int nAlpha = 0, DWORD dwShowWay = IMAGE_SHOWWAY_NORMAL, int nRotateAngle = 0) = 0;

	virtual void ShowBlendEx(int x, int y, CRect* lpSrc, DWORD dwWidth, DWORD dwHeight, 
							 int nAlpha = 0, int nRotateAngle = 0) = 0;
	virtual void ShowToWorld(int x, int y) = 0;
	virtual void GetSize(int& nWidth, int& nHeight) = 0;
	virtual DWORD GetSize() = 0;
	
	virtual int	GetWidth() = 0;
	virtual int	GetHeight() = 0;
	
	virtual void SetColor(BYTE a, BYTE r, BYTE g, BYTE b) = 0;
//	virtual void SetSkew(float sx, float sy) = 0;
};

//aniͼƬ�ӿ�
class IAniObj 
{
public:
	virtual ~IAniObj(){};

	virtual bool Show(DWORD uFrame, int x, int y, int alpha = 0, DWORD dwShowWay = IMAGE_SHOWWAY_NORMAL) = 0;

	//���ô˽ӿ���ʾ���꽫���浱ǰ����λ����ʾ�����������꣬����������Ϊ��ʾ���꣬����ͼ����
	virtual bool ShowRelative(DWORD uFrame, int x, int y, int alpha = 0, DWORD dwShowWay = IMAGE_SHOWWAY_NORMAL) = 0;

	virtual bool ShowEx(DWORD uFrame, int x, int y, CRect* lpSrc, DWORD dwWidth, DWORD dwHeight, int alpha = 0, DWORD dwShowWay = IMAGE_SHOWWAY_NORMAL) = 0;

	//���ô˽ӿ���ʾ���꽫���浱ǰ����λ����ʾ�����������꣬����������Ϊ��ʾ���꣬����ͼ����
	virtual bool ShowExRelative(DWORD uFrame, int x, int y, CRect* lpSrc, DWORD dwWidth, DWORD dwHeight, int alpha = 0, DWORD dwShowWay = IMAGE_SHOWWAY_NORMAL) = 0;

	//��֡��
	virtual int	GetFrameAmount() = 0;

	virtual DWORD GetSize() = 0;

	virtual IImageObj* GetImageObj(DWORD uFrameIndex) = 0;

	virtual void Release() = 0;
};


IAniObj* CreateAniObject(const char* pszFile, const char* pszIndex);

//���Ƽ�ʹ�ô˽ӿ�,�����ڴ�й©
IImageObj* CreateImgObject(const char* pszImgFile);

//���ͼƬ(���ص�IImageObj�����Լ��ͷ�)
IImageObj* CreateImgObjectByAni(const char* pszIndex, const char* pszFile="./ani/common.ani");

//�����֡�Ķ���
BOOL CreateImgObjectByAniEx(const char* pszIndex,vector<IImageObj*>& vImages, const char* pszFile="./ani/common.ani");


#endif
