//
//  NDUILabel.h
//  DragonDrive
//
//  Created by xiezhenghai on 10-12-29.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//
#ifndef __NDUILabel_H
#define __NDUILabel_H

#include "NDUINode.h"
#include <string>

#include "ccTypes.h"
#include "CCTexture2D.h"

namespace NDEngine
{	
	//��ǩ����ʾ��ʽ
	typedef enum
	{
		LabelTextAlignmentLeft = 0, //�����
		LabelTextAlignmentCenter,	//����
		LabelTextAlignmentRight		//�Ҷ���
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
		std::string GetText(){ return m_text; }
//		
//		������SetFontColor
//		���ã����ñ�ǩ�������ɫ
//		������fontColor��ɫֵrgba
//		����ֵ��
		void SetFontColor(cocos2d::ccColor4B fontColor);
//		
//		������GetFontColor
//		���ã���ȡ��ǩ��������ɫ
//		��������
//		����ֵ����ɫֵrgba
		cocos2d::ccColor4B GetFontColor(){ return m_color; }
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
		unsigned int GetFontSize(){ return m_fontSize; }
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
		int GetTextAlignment(){ return m_textAlignment; }
//		
//		������SetRenderTimes
//		���ã����ñ�ǩ���ݵ���Ⱦ������ע�⣺��Ⱦ����Խ��������Խ����������Ч��Ҳ��Խ����Ĭ����Ⱦ2��
//		������times��Ⱦ����
//		����ֵ����
		void SetRenderTimes(unsigned int times){ m_renderTimes = times; }
//		
//		������GetRenderTimes
//		���ã���ȡ��ǩ����Ⱦ����
//		��������
//		����ֵ������
		unsigned int GetRenderTimes(){ return m_renderTimes; }
		
		void SetFontBoderColer(cocos2d::ccColor4B fontColor);
		
		CGSize GetTextureSize() { if (m_texture) return m_texture->getContentSizeInPixels(); return CGSizeZero; }
	public:
		void draw(); override		
		void OnFrameRectChange(CGRect srcRect, CGRect dstRect); override
		
	private:
		void MakeTexture();
		void MakeCoordinates();
		void MakeVertices();
		
	private:
		std::string m_text;
		cocos2d::ccColor4B m_color;
		unsigned int m_fontSize;
		unsigned int m_renderTimes;
		LabelTextAlignment m_textAlignment;
		CGRect m_cutRect;
		
		bool m_needMakeTex, m_needMakeCoo, m_needMakeVer;
		
		bool m_hasFontBoderColor;
		cocos2d::ccColor4B m_colorFontBoder;
		
		cocos2d::CCTexture2D *m_texture;
		GLfloat m_vertices[12], m_verticesBoder[12];
		GLfloat m_coordinates[8];
		GLbyte m_colors[16], m_colorsBorder[16];
	};
}

#endif


