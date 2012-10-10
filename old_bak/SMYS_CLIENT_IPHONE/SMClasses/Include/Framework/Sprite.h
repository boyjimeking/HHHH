/**
������
Copyright(c) 1999-2012, TQ Digital Entertainment, All Rights Reserved
**/
#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "IAniObj.h"
#include "CGeometry.h"
#include "cTypes.h"
#include "IActionDelegate.h"
#include "FrameworkTypes.h"

typedef enum
{
    ProgressLR,//������
    ProgressRL,//���ҵ���
    ProgressTB,//���ϵ���
    ProgressBT,//���µ���
} ProgressType;//���ȶ�����ʾ���������ķ���

struct sImageList
{
    CRect rect;//ÿ֡ͼƬ��������ͼƬ��λ�ô�С
    IImageObj* m_Obj;//ͼƬ����Դ
};

//��ʾ��ʾ��Ϣ
void showTip(const char* lpszTip,unsigned long nColor=0x00ff00/*��ɫ*/,BOOL bMuliLine=FALSE/*���Ըò���*/,int iFontSize=18/*�����С*/, const char* lpSpriteName="com_txttipsbg"/*����ͼƬ*/, const char* pszFile="./ani/ui.ani"/*����ani�ļ�*/,float fSacleX=2/*ͼƬX��������*/,float fSacleY=1.f/*ͼƬY��������*/,float fWaitTime=3.0f/*�ȴ�ʱ��*/,int iUpSize=-50/*�����ľ���*/,float fUpTime=.6f/*������Ҫʱ��*/,int xPos=-1/*x����,Ĭ��-1�����*/,int yPos=100/*y����*/,int iRemoveOhters=1/*�Ƿ����������ʾ��Ϣ*/,RENDER_TEXT_STYLE iRendeTextStyle=RENDER_TEXT_SILHOUETTE/*���ַ��*/);

//ɾ��������ʾ��Ϣ
void removeAllTips();

class CAction;
class CSprite :public IActionDelegate
{
public:
    CSprite(void);
    virtual ~CSprite(void);
public:
    //��֡��������
    void InitSprite(const char* lpSpriteName/*ani��[KEY]*/, const char* pszFile="./ani/common.ani");
   
	//��ʼ������,ֻ��һ֡
	void InitSprite( const char* lpSpriteName/*ani��[KEY]*/,CRect rect/*��һ֡�Ĵ�С,�ڶ�֡Ҫ�ٵ���AddFrameWithRect���Ӻ�����֡����*/,const char* pszFile="./ani/common.ani");
  
	void InitSprite( const char* lpSpriteName/*ani��[KEY]*/,CRect rectAll/*��ͼƬ��С*/,int iRow/*��*/,int iCol/*��*/,int iCount/*��ͼƬ��*/,const char* pszFile="./ani/common.ani");

    //��ʾ����
    virtual bool Show(bool bShowRelative = false);

    //��ʱû��(���ö�����ÿ֡�������ʱ����ٴ��ӳ�ʱ��,Ĭ��Ϊ0)
    void SetDelay(float fDelay);

    // ����һ֡���֡��ani [KEY](aniֻ֧��һ��������64֡��ͼƬ,�ô˺������ض�ε�������)
    void AddFrameWithFileName(const char *pszFileName/*ani��[KEY]*/,char* pszFile=NULL);

	//��ԭ��ͼƬ��,����ĳ�����һ֡����;��ԭ�����кü���ͼƬ�ļ���֡����,���ӵİ���һ��ͼƬ��С
    void AddFrameWithRect(CRect rect/*����*/);

	//����֡��Χ��С(��֡�����������е�ͼƬ֡��,���Զ�����������ͼƬ֡��)
	void SetFrameRangeSize(int iStartFrameIndex/*��ʼ֡��*/,int iFrameSize/*��֡��,��Сһ֡*/);
	
	//����֡��Χ(��֡�����������е�ͼƬ֡��,���Զ�����������ͼƬ֡��)
	void SetFrameRange(int iStartFrameIndex/*��ʼ֡��*/,int iEndFrameIndex/*����֡��*/);

	//��ö�����ͼƬ����
    int GetImagesSize();

	//��ȡ������ʼ�����ͼƬ����֡��
	int GetFrameSize();

	//��ȡ�����Ĳ��ŵ���֡��
	int GetPlayFrameSize();

	//��õ�һ֡��λ��
	int GetFrameMinIndex();
	
	//���ö�����֡��
	void SetImageNum(int iFrameIndex);

	//��ȡĳһ֡����
    IImageObj* GetFrameImage(int iFrameIndex);

	//��ȡ���ű���(��Χ:0.f-���޴�)
    float GetScale();

    //���÷Ŵ���С����(������ʼ�Ĵ�С)
    void SetScale(float scale);

	//���÷Ŵ���С����(������ʼ�Ĵ�С)
	void SetScale(float scaleX/*X������ű���*/,float scaleY/*Y������ű���*/);

    //���þ������ĵ������,��setCenterPosition(CPoint(CMyBitmap::GetScreenWidth()/2,100));
    void SetCenterPosition(CPoint& pos);

    //X���귭ת(ͼƬ���ҷ�ת)
	void SetFlipX(bool bFlipX=true/*�Ƿ�ͼƬ���ҷ�ת*/);
	
    //Y���귭ת(ͼƬ���·�ת)
	void SetFlipY(bool bFlipY=true/*�Ƿ�ͼƬ���·�ת*/);

	//X�����Ƿ�ת
    bool IsFlipX(void);

	//Y�����Ƿ�ת
    bool IsFlipY(void);

    //��þ���ʵ����ʾ�Ŀ��
    int GetShowWidth();

    //��þ���ʵ����ʾ�ĸ߶�
    int GetShowHeight();

    //��þ�������ͼƬ�Ŀ��
    int GetWidth();

    //��þ�������ͼƬ�ĸ߶�
    int GetHeight();

    //����Ҫ������,���Ǵ��ϵ�����ʾ����
    void SetProgressType(ProgressType type);

	//���ý���ֵ
    void SetPercentage(float fPercentage);

    //��ȡ����ֵ( ��Χ: 0 - 100)
    float GetPercentage(void);

	virtual float GetRotation();
	virtual void SetRotation(float rotation);

	virtual float GetScaleX();
	virtual void SetScaleX(float scaleX);

	virtual const CPoint& GetPosition(void);
	virtual void SetPosition(const CPoint& var);

#if 0
	//�����Ӿ���
	virtual void AddChild(CSprite* pSprChild);

	/** Adds a child to the container with a z-order
	If the child is added to a 'running' node, then 'onEnter' and 'onEnterTransitionDidFinish' will be called immediately.
	*/
	virtual void AddChild(CSprite * pSprChild, int zOrder);

	/** Adds a child to the container with z order and tag
	If the child is added to a 'running' node, then 'onEnter' and 'onEnterTransitionDidFinish' will be called immediately.
	*/
	virtual void AddChild(CSprite * pSprChild, int zOrder, int tag);

	//��ȡ������
	CSprite* GetParent();

	//���ø�����
	void SetParent(CSprite* pSpriteChild);

	//void SetTouchEnable(BOOL bTouch=TRUE);

	//BOOL IsTouchEnable();

	//��ȡ��ǩ
	virtual int GetTag(void);

	//���ñ�ǩ
	virtual void SetTag(int iTag);

	
#endif

	//����һ������(GetActionManager()->AddAction(action, this))
	CAction * RunAction(CAction* action);

	//ֹͣ�þ�������ж���(GetActionManager()->RemoveAllActionsFromTarget(this);)
	void StopAllActions();

	float m_fDelay;

	//�������������
	string m_strSpriteName;

protected:
	virtual bool IsAutoRelease();
	virtual void SetAutoRelease(bool bAutoRelease);

#if 0
	CSprite* m_pParent;
	BOOL m_bTouchEnable;
	typedef vector<CSprite*> VEC_SPRITE;
	VEC_SPRITE m_vecChild;
#endif

	int m_iFrameMin;//��ʼ֡
	int m_iFrameSize;//ͼƬ��֡��
	int m_iPlayFrameSize;//���ŵ���֡��

	
    float m_fRotation; //��ת�Ƕ�
   
	int m_iImageNum;//���鶯���ĵڼ���ͼƬ	
    vector<sImageList> m_vImages;//ͼƬ�б�
    char m_czAniFile[256];//Ĭ�ϵ�ani·��

    
    CPoint m_tPosition; //����λ��(AnchorPointê���ھ������Ͻ�)
    ProgressType m_eProgressType;//�����ʾ������(���ϵ���/������)

	int m_iWidth;//����ͼƬ�Ŀ��
    int m_iHeight;//����ͼƬ�ĸ߶�

    float	m_fPercentage;
    bool	m_bAutoRelease;

    bool m_bFlipX;//X���귭ת
    bool m_bFlipY;//Y���귭ת
   
#if 0
	int m_iTag;//����ı�ǩ(Ĭ��-1)
#endif

    float m_fScaleX; //X��������

	//Y��������
    PROPERTY(float, m_fScaleY, ScaleY)

    /** Whether of not the node is visible. Default is true */
    PROPERTY(bool, m_bIsVisible, IsVisible)

    /** Opacity: conforms to CCRGBAProtocol protocol */
	//͸����
    PROPERTY(int, m_nOpacity, Opacity)

    /** Color: conforms with CCRGBAProtocol protocol */
	//��ɫ
    PROPERTY_PASS_BY_REF(Color3B, m_sColor, Color)

#if 0
	PROPERTY_PASS_BY_REF(CPoint, m_tAnchorPoint, AnchorPoint)

	/** The z order of the node relative to it's "brothers": children of the same parent */
	//Ĭ��0
	PROPERTY_READONLY(int, m_iZOrder, ZOrder)
private:
	void SetZOrder(int z);
#endif

};


/**
  ��ʼ������:
  ����һ:��N��ͼƬ�ļ�,������ani��,��"./ani/ui.ani"����:
	  [MyTestSprite]
	  FrameAmount=1
	  Frame0=data/Test/grossini_dance_11.png

	  [MyTestSprite_s1]
	  FrameAmount=1
	  Frame0=data/Test/grossini_dance_12.png

	  [MyTestSprite_s2]
	  FrameAmount=1
	  Frame0=data/Test/grossini_dance_13.png

	  [MyTestSprite_s3]
	  FrameAmount=1
	  Frame0=data/Test/grossini_dance_14.png

	  ����������ʼ��:
	  m_mySprite.InitSprite("MyTestSprite","./ani/ui.ani");
	  m_mySprite.AddFrameWithFileName("MyTestSprite_s1");
	  m_mySprite.AddFrameWithFileName("MyTestSprite_s2");
	  m_mySprite.AddFrameWithFileName("MyTestSprite_s3" );

  ��ani�ļ���:
	  [Frames]
	  FrameAmount=4
	  Frame0=data/Test/Player.png
	  Frame1=data/Test/grossini_dance_12.png
	  Frame2=data/Test/grossini_dance_13.png
	  Frame3=data/Test/grossini_dance_14.png
	  ��ʼ��:
	  m_mySprite.InitSprite("Frames","./ani/ui.ani");



  ������:
  һ�Ŵ�ͼ,2��,4��,��6��Сͼ,ÿ��Сͼ��С132*132
  CRect rc={0,0,132*4,132*2};
  m_mySprite3.InitSprite("MyTestSpriteDragon",rc,2,4,6);

  ������:
  ��ʼ��InitSpriteʱ�����һ֡��Сͼ��С,��AddFrameWithRectÿһСͼ��λ�ô�С
  #define MakeRect(x,y,w,h) 	memset(&rc,0,sizeof(rc));rc.top=y;rc.left=x;rc.right=x+w;rc.bottom=y+h;
  CRect rc;
  MakeRect(132*0, 132*0, 132, 132);
  m_mySprite3.InitSprite("MyTestSpriteDragon",rc);

  MakeRect(132*1, 132*0, 132, 132);
  m_mySprite3.AddFrameWithRect(rc);

  MakeRect(132*2, 132*0, 132, 132);
  m_mySprite3.AddFrameWithRect(rc);

  MakeRect(132*3, 132*0, 132, 132);
  m_mySprite3.AddFrameWithRect(rc);

  MakeRect(132*0, 132*1, 132, 132);
  m_mySprite3.AddFrameWithRect(rc);

  MakeRect(132*1, 132*1, 132, 132);
  m_mySprite3.AddFrameWithRect(rc);
**/

//class SpriteTipSet
//{
//	SpriteTipSet(unsigned long nColor=0x00ff00/*��ɫ*/,BOOL bMuliLine=FALSE/*�Ƿ���*/,int iFontSize=18/*�����С*/, const char* lpSpriteName="com_tipsbg"/*����ͼƬ*/, const char* pszFile="./ani/ui.ani"/*����ani�ļ�*/,float fSacleX=2/*ͼƬX��������*/,float fSacleY=1.f/*ͼƬY��������*/,float fWaitTime=3.0f/*�ȴ�ʱ��*/,int iUpSize=-50/*�����ľ���*/,float fUpTime=.6f/*������Ҫʱ��*/,int xPos=-1/*x����,Ĭ��-1�����*/,int yPos=100/*y����*/,int iRemoveOhters=1/*�Ƿ����������ʾ��Ϣ*/)
//	{
//		
//	}
//    void SetTip(unsigned long nColor=0x00ff00/*��ɫ*/,BOOL bMuliLine=FALSE/*�Ƿ���*/,int iFontSize=18/*�����С*/, const char* lpSpriteName="com_tipsbg"/*����ͼƬ*/, const char* pszFile="./ani/ui.ani"/*����ani�ļ�*/,float fSacleX=2/*ͼƬX��������*/,float fSacleY=1.f/*ͼƬY��������*/,float fWaitTime=3.0f/*�ȴ�ʱ��*/,int iUpSize=-50/*�����ľ���*/,float fUpTime=.6f/*������Ҫʱ��*/,int xPos=-1/*x����,Ĭ��-1�����*/,int yPos=100/*y����*/,int iRemoveOhters=1/*�Ƿ����������ʾ��Ϣ*/);
//    {
//        m_nColor=						nColor;
//        m_bMuliLine=							 bMuliLine;
//        m_iFontSize=							 iFontSize;
//        m_lpSpriteName=						 lpSpriteName;
//        m_pszFile=							pszFile;
//        m_fSacleX=fSacleX;
//        m_fSacleY=							fSacleY;
//        m_fWaitTime=							fWaitTime;
//        m_iUpSize=							iUpSize;
//        m_fUpTime=							 fUpTime;
//        m_xPos=							xPos;
//        m_yPos=							yPos;
//        m_iRemoveOhters=							iRemoveOhters;
//    }
//
//    void Show(const char *lpText)
//    {
//        showTip(lpText,
//                m_nColor,
//                m_bMuliLine,
//                m_iFontSize,
//                m_lpSpriteName,
//                m_pszFile,
//                m_fSacleX,
//                m_fSacleY,
//                m_fWaitTime,
//                m_iUpSize,
//                m_fUpTime,
//                m_xPos,
//                m_yPos,
//                m_iRemoveOhters);
//    }
//
//protected:
//    unsigned long m_nColor;
//    BOOL m_bMuliLine;
//    int m_iFontSize;
//    const char* m_lpSpriteName;
//    const char* m_pszFile;
//    float m_fSacleX;
//    float m_fSacleY;
//    float m_fWaitTime;
//    int m_iUpSize;
//    float m_fUpTime;
//    int m_xPos;
//    int m_yPos;
//    int m_iRemoveOhters;
//};

#endif //__SPRITE_H__