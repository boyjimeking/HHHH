//
//  NDUILabel.h
//  DragonDrive
//
//  Created by xiezhenghai on 10-12-29.
//  Copyright 2010 (����)DeNA. All rights reserved.
//
#ifndef __NDUILabel_H
#define __NDUILabel_H

#include "NDUINode.h"
#include <string>

#include "ccTypes.h"
#include "CCTexture2D.h"

NS_NDENGINE_BGN

//��ǩ����ʾ��ʽ
typedef enum
{
	LabelTextAlignmentLeft = 0, //�����
	LabelTextAlignmentCenter,	//����
	LabelTextAlignmentRight,	//�Ҷ���
	LabelTextAlignmentHorzCenter,//ˮƽ����
	LabelTextAlignmentVertCenter,//��ֱ����
	LabelTextAlignmentLeftCenter    //ˮƽ����룬��ֱ���ж���
}LabelTextAlignment;
	
class NDUILabel : public NDUINode
{
	DECLARE_CLASS(NDUILabel)
	NDUILabel();
	~NDUILabel();
public:
//		
//		������SetText
//		���ã����ñ�ǩ�ı�����
//		������text�ı�����
//		����ֵ����
		void SetText(const char* text);
//		
//		������GetText
//		���ã���ȡ��ǩ���ı�����
//		��������
//		����ֵ���ı�����
		std::string GetText(){ return m_strText; }
//		
//		������SetFontColor
//		���ã����ñ�ǩ�������ɫ
//		������fontColor��ɫֵrgba
//		����ֵ��
		void SetFontColor(cocos2d::ccColor4B fontColor);

//		������GetFontColor
//		���ã���ȡ��ǩ��������ɫ
//		��������
//		����ֵ����ɫֵrgba
		cocos2d::ccColor4B GetFontColor(){ return m_kColor; }
//		
//		������SetFontSize
//		���ã����ñ�ǩ�������С
//		������fontSize�����С
//		����ֵ����
		void SetFontSize(unsigned int fontSize);
//		
//		������GetFontSize
//		���ã���ȡ��ǩ�����С
//		��������
//		����ֵ�������С
		unsigned int GetFontSize(){ return m_uiFontSize; }
//		
//		������SetTextAlignment
//		���ã����ñ�ǩ�ı��Ķ��뷽ʽ
//		������alignment���뷽ʽ
//		����ֵ����
		void SetTextAlignment(int alignment);
//		
//		������GetTextAlignment
//		���ã���ȡ��ǩ�ı��Ķ��뷽ʽ
//		��������
//		����ֵ�����뷽ʽ
		int GetTextAlignment(){ return m_eTextAlignment; }
//		
//		������SetRenderTimes
//		���ã����ñ�ǩ���ݵ���Ⱦ������ע�⣺��Ⱦ����Խ��������Խ����������Ч��Ҳ��Խ����Ĭ����Ⱦ2��
//		������times��Ⱦ����
//		����ֵ����
		void SetRenderTimes(unsigned int times){ m_uiRenderTimes = times; }

//		������GetRenderTimes
//		���ã���ȡ��ǩ����Ⱦ����
//		��������
//		����ֵ������
		unsigned int GetRenderTimes(){ return m_uiRenderTimes; }
		
		void SetFontBoderColer(cocos2d::ccColor4B fontColor);
		
		CCSize GetTextureSize() { if (m_texture) return m_texture->getContentSizeInPixels(); return CCSizeZero; }
		void SetHasFontBoderColor(bool bIsBorder){m_bHasFontBoderColor = bIsBorder;};
	public:
		virtual void draw(); override
		void OnFrameRectChange(CCRect srcRect, CCRect dstRect); override
		
protected:
	void preDraw();
	void postDraw();
	virtual void debugDraw();
	void MakeTexture();
	void MakeCoordinates();
	void MakeVertices();
	friend class NDUIButton;
		
protected:
	std::string m_strText;
	cocos2d::ccColor4B m_kColor;
	unsigned int m_uiFontSize;
	unsigned int m_uiRenderTimes;
	LabelTextAlignment m_eTextAlignment;
	CCRect m_kCutRect;
	
	bool m_bNeedMakeTex;
	bool m_bNeedMakeCoo;
	bool m_bNeedMakeVer;
	
	bool m_bHasFontBoderColor;
	cocos2d::ccColor4B m_kColorFontBoder;
	
	cocos2d::CCTexture2D* m_texture;
	GLfloat m_pfVertices[12];
	GLfloat m_pfVerticesBoder[12];
	GLfloat m_pfCoordinates[8];
	GLbyte m_pbColors[16];
	CCRect m_cutRect;
	GLbyte m_pbColorsBorder[16];
};

NS_NDENGINE_END

#endif
