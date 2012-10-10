/*
 *  �ӿ���:ani֡�ӿ�IAniObj,ͼƬ�ӿ�IImageObj
 *  Copyright 2010 TQ Digital Entertainment. All rights reserved.
	MyBitmapCreate������CMyBitmap����Release
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

class IImageObj;

//ani֡�ӿ�(����Ani�ļ�,�����꼴�Ը�Ani�ļ��������֡ͼƬ��CMyBitmap* m_pFrame[64]��������,���֡��Ϊ64)
class IAniObj
{
public:
	virtual ~IAniObj() {};

	//��ʾ(����CMyBitmap��ʾ)
	virtual bool Show(DWORD uFrame/*֡��*/, int x, int y, int alpha = 0, DWORD dwShowWay = IMAGE_SHOWWAY_NORMAL) = 0;

	//���ô˽ӿ���ʾ���꽫���浱ǰ����λ����ʾ�����������꣬����������Ϊ��ʾ���꣬����ͼ����
	virtual bool ShowRelative(DWORD uFrame, int x, int y, int alpha = 0, DWORD dwShowWay = IMAGE_SHOWWAY_NORMAL) = 0;

	virtual bool ShowEx(DWORD uFrame, int x, int y, CRect* lpSrc, DWORD dwWidth, DWORD dwHeight, int alpha = 0, DWORD dwShowWay = IMAGE_SHOWWAY_NORMAL) = 0;

	//���ô˽ӿ���ʾ���꽫���浱ǰ����λ����ʾ�����������꣬����������Ϊ��ʾ���꣬����ͼ����
	virtual bool ShowExRelative(DWORD uFrame, int x, int y, CRect* lpSrc, DWORD dwWidth, DWORD dwHeight, int alpha = 0, DWORD dwShowWay = IMAGE_SHOWWAY_NORMAL) = 0;

	//��ȡ��֡��
	virtual int	GetFrameAmount() = 0;

	//��ȡ����֡��ͼƬ�ĳ�*����ܺ�,û�õĽӿ�,ɾȥ
	//virtual DWORD GetSize() = 0;

	//��ȡĳһ֡��ͼƬ
	virtual IImageObj* GetImageObj(DWORD uFrameIndex/*֡��*/) = 0;

	//�ͷ�(��delete this����)
	virtual void Release() = 0;
};

//ͼƬ�ӿ�(����ͼƬ,���沢����һ��CMyBitmap*,�Ƕ�CMyBitmap�Ķ��η�װ)
class IImageObj
{
public:
    virtual ~IImageObj() {};

	//�ͷ�(��CMyBitmap����Release,��delete this)
	virtual void Release()=0;

	//��ʾ(����CMyBitmap��ʾ)
    virtual void Show(int x, int y, int nAlpha = 0, DWORD dwShowWay = IMAGE_SHOWWAY_NORMAL) = 0;

	virtual void ShowEx(int x, int y, CRect *lpSrc, DWORD dwWidth, DWORD dwHeight, int nAlpha = 0, DWORD dwShowWay = IMAGE_SHOWWAY_NORMAL, int nRotateAngle = 0) = 0;

    //���ô˽ӿ���ʾ���꽫���浱ǰ����λ����ʾ�����������꣬����������Ϊ��ʾ���꣬����ͼ����
    virtual void ShowRelative(int x, int y, int nAlpha = 0, DWORD dwShowWay = IMAGE_SHOWWAY_NORMAL) = 0;

    //���ô˽ӿ���ʾ���꽫���浱ǰ����λ����ʾ�����������꣬����������Ϊ��ʾ���꣬����ͼ����
    virtual void ShowExRelative(int x, int y, CRect *lpSrc, DWORD dwWidth, DWORD dwHeight, int nAlpha = 0, DWORD dwShowWay = IMAGE_SHOWWAY_NORMAL, int nRotateAngle = 0) = 0;


    virtual void ShowBlendEx(int x, int y, CRect* lpSrc, DWORD dwWidth, DWORD dwHeight,
                             int nAlpha = 0, int nRotateAngle = 0) = 0;

    virtual void ShowToWorld(int x, int y) = 0;

	//��ȡͼƬ��С
	virtual void GetSize(int& nWidth/*��*/, int& nHeight/*��*/) = 0;

	//��ȡͼƬ��*���ֵ
	virtual DWORD GetSize() = 0;

	//��ȡͼƬ���
    virtual int	GetWidth() = 0;

	//��ȡͼƬ�߶�
    virtual int	GetHeight() = 0;

	//������ɫ
    virtual void SetColor(BYTE a, BYTE r, BYTE g, BYTE b) = 0;
//	virtual void SetSkew(float sx, float sy) = 0;
};

//��ȡAni(���ص�IAniObj����Release())
IAniObj* CreateAniObject(const char* pszFile/*�ļ���*/, const char* pszIndex/*KEY����*/);

//����ͼƬ·����ȡͼƬ(���ص�IImageObj������Release())
IImageObj* CreateImgObject(const char* pszImgFile/*ͼƬ·��*/);

//����Ani��ȡͼƬ(���ص�IImageObj����Release()ɾ��)
IImageObj* CreateImgObjectByAni(const char* pszIndex/*Key����*/, const char* pszFile="./ani/common.ani");

//�����֡�Ķ���(���ص�IAniObj�ͻ�õ�IImageObj*���������Լ��ͷ�!)
IAniObj* CreateImgObjectByAniEx(const char* pszIndex/*Key����*/,vector<IImageObj*>& vImages/*��ȡ�Ķ�֡ͼƬ*/, const char* pszFile="./ani/common.ani");


#endif
