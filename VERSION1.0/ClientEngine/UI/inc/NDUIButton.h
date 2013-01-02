/*
 *  NDUIButton.h
 *  DragonDrive
 *
 *  Created by wq on 10-12-29.
 *  Copyright 2010 (����)DeNA. All rights reserved.
 *
 *   modify by yay on 12-1-12
 *   add lua callback
 */

#ifndef __ND_UI_BUTTON_H__
#define __ND_UI_BUTTON_H__

#include "NDUINode.h"
#include <string>
#include "NDUILabel.h"
#include "NDUIScrollText.h"
#include "NDCombinePicture.h"
//#include "NDLightEffect.h"
#include "NDLightEffect.h"
#include "NDUIBaseItemButton.h"


NS_NDENGINE_BGN


//////////////////////////////////////////////////////////////////////////////////////////////
class NDUIButton;	
//delegates begin
class NDUIButtonDelegate
{
public:
//		
//		������OnButtonClick
//		���ã�����ť�����֮���ܵ��ø÷���
//		������button������İ�ť
//		����ֵ����
	virtual void OnButtonClick(NDUIButton* button);
	virtual void OnButtonDown(NDUIButton* button);
	virtual void OnButtonUp(NDUIButton* button);
	virtual bool OnButtonLongClick(NDUIButton* button);
	virtual bool OnButtonDragOut(NDUIButton* button, CCPoint beginTouch, CCPoint moveTouch, bool longTouch);
	virtual bool OnButtonDragOutComplete(NDUIButton* button, CCPoint endTouch, bool outOfRange);
	virtual bool OnButtonDragIn(NDUIButton* desButton, NDUINode *uiSrcNode, bool longTouch);
	virtual bool OnButtonDragOver(NDUIButton* overButton, bool inRange);
	virtual bool OnButtonLongTouch(NDUIButton* button);
	virtual bool OnButtonLongTouchCancel(NDUIButton* button);
};
//delegates end


//////////////////////////////////////////////////////////////////////////////////////////////
class NDUIButton : public NDUIBaseItemButton
{
	DECLARE_CLASS(NDUIButton)
public:
	NDUIButton();
	~NDUIButton();
	
public:		
//		
//		������Initialization
//		���ã���ʼ����ť�����뱻��ʾ������ʽ����
//		��������
//		����ֵ����
	void Initialization(); override
//		
//		������SetImage
//		���ã����ð�ť��ͼƬ
//		������picͼƬ��useCustomRectͼƬ�Ƿ���ʾ���Զ��巶Χ������ڰ�ť����ʾ��Χ����customRect�Զ��巶Χ
//		����ֵ����
	void SetImage(NDPicture* pic, bool useCustomRect = false, CCRect customRect = CCRectZero, bool clearPicOnFree = false);
	void SetImageCustom(NDPicture* pic, bool useCustomRect = false, CCRect customRect = CCRectZero){
		this->SetImage(pic, useCustomRect, customRect, true);
	}
	
	void SetImageLua(NDPicture* pic);
//		
//		������SetCombineImage
//		���ã����ð�ť��ͼƬ(���)
//		������combinepicͼƬ(���)��useCustomRectͼƬ�Ƿ���ʾ���Զ��巶Χ������ڰ�ť����ʾ��Χ����customRect�Զ��巶Χ
//		����ֵ����
	void SetCombineImage(NDCombinePicture* combinepic, bool useCustomRect = false, CCRect customRect = CCRectZero, bool clearPicOnFree = false);
//		
//		������SetTouchDownImage
//		���ã����ð�ť������ʱ��ͼƬ�����ø÷�����ʹ��SetTouchDownColor����ʧЧ
//		������picͼƬ��useCustomRectͼƬ�Ƿ���ʾ���Զ��巶Χ������ڰ�ť����ʾ��Χ����customRect�Զ��巶Χ
//		����ֵ����
	void SetTouchDownImage(NDPicture* pic, bool useCustomRect = false, CCRect customRect = CCRectZero, bool clearPicOnFree = false);	
	void SetTouchDownImageCustom(NDPicture* pic, bool useCustomRect = false, CCRect customRect = CCRectZero){
		this->SetTouchDownImage(pic, useCustomRect, customRect, true);
	}
	
	void SetTouchDownImageLua(NDPicture* pic);
//		
//		������SetTouchDownCombineImage
//		���ã����ð�ť������ʱ��ͼƬ(���)�����ø÷�����ʹ��SetTouchDownColor����ʧЧ
//		������picͼƬ��useCustomRectͼƬ�Ƿ���ʾ���Զ��巶Χ������ڰ�ť����ʾ��Χ����customRect�Զ��巶Χ
//		����ֵ����
	void SetTouchDownCombineImage(NDCombinePicture* combinepic, bool useCustomRect = false, CCRect customRect = CCRectZero, bool clearPicOnFree = false);
	
//		
//		������SetTouchDownColor
//		���ã����ð�ť������ʱ����ɫ�����ø÷�����ʹ��SetTouchDownImage����ʧЧ
//		������touchDownColor��ɫֵrgba
//		����ֵ����
	void SetTouchDownColor(cocos2d::ccColor4B touchDownColor);
//		
//		������SetFocusColor
//		���ã����ý���ʱ����ɫ�����ø÷�����ʹSetFocusRimImage��SetFocusNormal����ʧЧ
//		������focusColor��ɫֵrgba
//		����ֵ����
	void SetFocusColor(cocos2d::ccColor4B focusColor);		
//		
//		������SetFocusRimImage
//		���ã����ý���ʱʹ�ÿ���ṩ��һ�ֱ߿򣬵��ø÷�����ʹSetFocusColor��SetFocusNormal����ʧЧ
//		��������
//		����ֵ����
	void SetFocusRimImage();
	
//		
//		������SetFocusNormal
//		���ã����ý���ʱ�����κβ��������ø÷�����ʹSetFocusColor��SetFocusRimImage����ʧЧ
//		��������
//		����ֵ����
	void SetFocusNormal();

//		
//		������SetFocusImage
//		���ã����ý���ʱʹ��ͼƬ�����ø÷�����ʹSetFocusColor,SetFocusNormal��SetFocusRimImage����ʧЧ
//		��������
//		����ֵ����
	void SetFocusImage(NDPicture *pic, bool useCustomRect = false, CCRect customRect = CCRectZero, bool clearPicOnFree = false);
	void SetFocusImageCustom(NDPicture *pic, bool useCustomRect = false, CCRect customRect = CCRectZero)
	{
		SetFocusImage(pic, useCustomRect, customRect, true);
	}
	
	void SetFocusImageLua(NDPicture *pic);

	void SetDisImage(NDPicture *pic, bool useCustomRect = false, CCRect customRect = CCRectZero, bool clearPicOnFree = false);
//		
//		������OpenFrame
//		���ã������ְ�ť�ı߿����ʱ���ְ�ť����Ĭ��ʱ�򿪵�
//		��������
//		����ֵ����
	void OpenFrame(){ m_framed = true; }
//		
//		������CloseFrame
//		���ã��ر����ְ�ť�ı߿�
//		��������
//		����ֵ����
	void CloseFrame(){ m_framed = false; }
public:
//		
//		������SetTitle
//		���ã����ð�ť���ı���Ϣ
//		������title, bScroll�Ƿ��Զ�����(������ť�ĳ��Ⱦ͹���,���򲻹���),��bForceǿ�ƹ���
//		����ֵ����		
	void SetTitle(const char* title, bool bAutoScroll=true, bool bForce=false, unsigned int leftWidth=0, unsigned int rightWidth=0);
	
	void SetTitleLua(const char* title);
//	
//		������GetTitle
//		���ã���ȡ��ť���ı���Ϣ
//		��������
//		����ֵ���ı���Ϣ
	std::string GetTitle();
//		
//		������SetFontColor
//		���ã����ð�ť��������ɫ
//		������fontColor��ɫֵrgba
//		����ֵ��
	void SetFontColor(cocos2d::ccColor4B fontColor);
	
	void SetFocusFontColor(cocos2d::ccColor4B focusFontColor);
//		
//		������GetFontColor
//		���ã���ȡ��ť��������ɫ
//		��������
//		����ֵ����ɫֵrgba
	cocos2d::ccColor4B GetFontColor();
//		
//		������SetBackgroundColor
//		���ã����ð�ť�ı���ɫ��Ĭ��ֵΪ��ɫ
//		������color��ɫֵrgba
//		����ֵ����		
	void SetBackgroundColor(cocos2d::ccColor4B color);
	
//		
//		������SetBackgroundPicture
//		���ã����ð�ť�ı���ͼ
//		������picͼƬ, clearPicOnFree�й��ͷ�
//		����ֵ����		
	void SetBackgroundPicture(NDPicture *pic, NDPicture *touchPic = NULL, bool useCustomRect = false, CCRect customRect = CCRectZero, bool clearPicOnFree = false);
	void SetBackgroundPictureCustom(NDPicture *pic, NDPicture *touchPic = NULL, bool useCustomRect = false, CCRect customRect = CCRectZero){
		this->SetBackgroundPicture(pic, touchPic, useCustomRect, customRect, true);
	}
	
	void SetBackgroundPictureLua(NDPicture *pic, NDPicture *touchPic = NULL);
//		
//		������SetBackgroundColor
//		���ã����ð�ť�ı������ͼ
//		������combinepic���ͼƬ, clearPicOnFree�й��ͷ�
//		����ֵ����		
	void SetBackgroundCombinePic(NDCombinePicture *combinepic, NDCombinePicture *touchCombinePic = NULL, bool clearPicOnFree = false);
	
//		
//		������SetFontSize
//		���ã����ð�ť����������С
//		������fontSize�����С��Ĭ��ֵΪ15
//		����ֵ����
	void SetFontSize(unsigned int fontSize);
//		
//		������GetFontSize
//		���ã���ȡ��ť����������С
//		��������
//		����ֵ�������С
	unsigned int GetFontSize();	
	
	//����ʾ������ǩ(������ʾ)�Ľӿڲ�������ʾһ����ǩ��ͬʱ��,����ʱֻ��Ҫ���ýӿ�,����Ҫ��ȡ�ӿ��ټ�
	void SetText(const char* text1, 
				 const char* text2,
				 unsigned int interaval = 0,
				 cocos2d::ccColor4B color1 = ccc4(11, 34, 18, 255),
				 cocos2d::ccColor4B color2 = ccc4(11, 34, 18, 255),
				 unsigned int fontSize1 = 13,
				 unsigned int fontSize2 = 13);
				 
	void SetArrow(bool bSet);
	
	void EnalbeGray(bool gray)
	{ 
		m_bGray = gray; 
		
		if (0 == m_disImage)
		{
			if (m_image) m_image->SetGrayState(gray); 
			if (m_picBG) m_picBG->SetGrayState(gray);
			if (m_picTouchBG) m_picTouchBG->SetGrayState(gray);
			if (m_touchDownImage) m_touchDownImage->SetGrayState(gray);
			if (m_focusImage) m_focusImage->SetGrayState(gray);
		}
	}
	
	bool IsGray() { return m_bGray; }
	
	
	void EnalbelBackgroundGray(bool gray) { if (m_picBG) m_picBG->SetGrayState(gray);
											if (m_picTouchBG) m_picTouchBG->SetGrayState(gray); 	
											}
	
	bool IsBackgroundGray() { if (m_picBG) return m_picBG->IsGrayState(); return false; }
	
	void SetNormalImageColor(cocos2d::ccColor4B color) { m_normalImageColor = color; }
	
	NDPicture* GetImage() { return m_image; }
	NDPicture* GetImageCopy() { if (m_image) return m_image->Copy(); return NULL; }
	void TabSel(bool bSel);
	bool IsTabSel();
	void ChangeSprite(const char* szSprite, CCPoint posOffset);
	void SetFocus(bool bFocus);
	void SetSoundEffect(int nId);
	int GetSoundEffect();

public:
	void draw();
	void drawFocus();
	void drawTouchDown();
	void drawButtonImage();
	void drawButtonFrame();
	void drawLongTouch();
	void SetFrameRect(CCRect rect); 
	void OnTouchDown(bool touched);
	void OnLongTouchDown(bool touched);
	//void SetChecked( bool bChecked ){ m_bChecked = bChecked; m_bGray = bChecked; }
	void SetChecked( bool bChecked ){ m_bTabSel = bChecked; m_bFocusEnable = bChecked; m_bChecked = bChecked;}

	
	bool CanDestroyOnRemoveAllChildren(NDNode* pNode);

public:
	void SetTitle();
	void SetTwoTitle();

protected: //pic
	NDPicture* m_image, *m_touchDownImage;
	NDPicture* m_rimImageLT, *m_rimImageRT, *m_rimImageLB, *m_rimImageRB;
	NDPicture* m_focusImage;
	NDPicture* m_disImage;
	NDUILabel* m_title;	

protected:
	NDCombinePicture *m_combinepicImg, *m_combinepicTouchDownImg;
	NDCombinePicture *m_combinePicBG, *m_combinePicTouchBG;
	NDPicture		 *m_picBG,  *m_picTouchBG;
	bool			 m_bClearBgOnFree;
	bool			 m_bArrow;
	//NDLightEffect	 *m_spriteArrow;

protected:
	//��ʾ������ǩ���
	NDUILabel *m_lbTitle1, *m_lbTitle2;
	bool m_bNeedSetTwoTitle;
	unsigned m_uiTwoTitleInter;

protected:
	cocos2d::ccColor4B m_touchDownColor;
	cocos2d::ccColor4B m_focusColor;
	cocos2d::ccColor4B m_backgroundColor;
	cocos2d::ccColor4B m_normalImageColor;
	bool m_touched;
	bool m_longTouched;
	bool m_framed;
	bool m_useCustomRect, m_touchDownImgUseCustomRect;
	bool m_clearUpPicOnFree, m_clearDownPicOnFree;
	bool m_bCustomFocusImageRect;
	bool m_ClearFocusImageOnFree;
	bool m_useBackgroundCustomRect;
	int m_SoundEffectId;
	CCRect m_backgroundCustomRect;
	CCRect m_customRect;
	CCRect m_touchDownImgCustomRect;
	CCRect m_customFocusImageRect;
	CCRect m_customDisImageRect;

	bool m_bCustomDisImageRect;
	bool m_ClearDisImageOnFree;
	
	typedef enum
	{
		TouchDownNone,
		TouchDownImage,
		TouchDownColor
	}TouchDownStatus;
	TouchDownStatus m_touchDownStatus;
	
	typedef enum
	{
		FocusNone,
		FocusColor,
		FocusRimImage,
		FocusImage,
	}FocusStatus;
	FocusStatus m_focusStatus;
	bool m_bFocusEnable;

protected: //scroll text
	NDUIScrollText* m_scrtTitle;
	bool m_bAutoScroll, m_bForce;
	std::string m_strTitle;
	unsigned int m_uiTitleFontSize;
	cocos2d::ccColor4B m_colorTitle, m_colorFocusTitle;
	bool m_bNeedSetTitle;
	bool m_bScrollTitle;
			
	unsigned int	 m_uiTitleLeftWidth;
	unsigned int m_uiTitleRightWidth;
	bool			 m_bGray;
	bool m_bChecked;
	bool			 m_bTabSel;
	NDLightEffect* m_pSprite;
	CCPoint			m_posSprite;
};

NS_NDENGINE_END 

#endif