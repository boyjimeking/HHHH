#ifndef _GRAPHIC_H
#define _GRAPHIC_H
 
#include "C3Primitive.h"
#include "DXPrimitive.h"

#ifndef GRAPHIC_API
#if defined(_WIN32)
#ifdef GRAPHIC_EXPORTS
#define GRAPHIC_API __declspec(dllexport)
#else
#define GRAPHIC_API __declspec(dllimport)
#endif
#else// ����ƽ̨���Ǿ�̬��
#define GRAPHIC_API
#endif
#endif

/*********************************************************************
**  Must Read
**  C3��׼: �漰����ָ��Ľӿڷ���,���ǽӿڷ�������Create��,����ָ����
**  �ͻ��˸�������������ά��,��Release. ���ǽӿڷ�������Get��,����ָ��
**  �ɵײ�ά������������,�ϲ㲻��Release,���ñ���ָ��������ʹ��.
*********************************************************************/
//////////////////////////////////////////////////////////////////////
struct	C3Motion;
struct	C3Texture;
class	C3DTexture;

typedef void* HC3MOTIONTRACKER;

enum 
{ 
	CAMERA_EYE	= 0x1, 
	CAMERA_TO	= 0x2, 
	CAMERA_BOTH = CAMERA_EYE | CAMERA_TO,
};

struct IDataIcon
{
	virtual class CAni* GetIconAni(int idxIcon) = 0;
};

/***********************************************************************
** Note: �����ڵ�����CMyBitmap::Init3DEx����ܵ�������CMyBitmap::����.
***********************************************************************/
class GRAPHIC_API CMyBitmap
{	
public:
	virtual unsigned long Release() = 0;
	
	// Note: ʹ���˷�0��nAlpha��,�´���ָ��0,��ʾʹ��֮ǰָ���ķ�0��ֵ.
	// �ָ�ԭ��Alpha��ʹ��SetColor(255, 255, 255, 255)
	virtual void Show(int x, int y, int nAlpha = 0, DWORD dwShowWay = _SHOWWAY_NORMAL) = 0;
	virtual void ShowEx(int x, int y, C3_RECT *lpSrc, DWORD dwWidth, DWORD dwHeight, int nAlpha = 0, DWORD dwShowWay = _SHOWWAY_NORMAL, int nRotateAngle = 0) = 0;
	
	virtual void ShowBlendEx(int x, int y, C3_RECT* lpSrc, DWORD dwWidth, DWORD dwHeight, 
			int nAlpha = 0, int nRotateAngle = 0, DWORD dwSrcBlend = D3DBLEND_SRCALPHA, DWORD dwDestBlend = D3DBLEND_INVSRCALPHA) = 0;

	virtual void GetSize(int& nWidth, int& nHeight) const = 0;
	virtual DWORD GetSize() const = 0;

	virtual int	GetWidth() const = 0;
	virtual int	GetHeight() const = 0;

	virtual void SetColor(BYTE a, BYTE r, BYTE g, BYTE b) = 0;

	// ͸����ʽ�ж�
	virtual int	IsTransparentFormat() const = 0;

	// ͸�����ж�
	virtual bool IsTransparent(C3_POS posPoint) const = 0;
	
	virtual C3Texture* GetC3Texture() = 0;
	
	virtual void ShowLinear(int x, int y, DWORD dwWidth, DWORD dwHeight, C3_RECT* pTexSrc = NULL, 
		int nAlpha = 0, int nRotateAngle = 0, DWORD dwSrcBlend = D3DBLEND_SRCALPHA, DWORD dwDestBlend = D3DBLEND_INVSRCALPHA) = 0;
	
public:	// font
	// �����ײ��Ĭ������(Note: ��������ini/font.ini)
	// ��pszDefaultFontFileName==NULL,ȡini/font.ini���õ�����,��֮��ȡ�����ֵ.
	// ��dwFontSize==0,ȡini/font.ini���õ������С,��֮��ȡ�����ֵ.
	static bool	GameFontCreate(const char* pszDefaultFontFileName = NULL, DWORD dwFontSize = 0);
	static void GameFontDestroy();

	static DWORD GetFontSize(void);
	static void  GetFontSize(C3_SIZE& infoSize);

	// ̽���ַ�����ĳ���ַ��Ƿ�DBCS�ĵ�һ���ַ�
	static bool IsDBCSLeadByte(const char* buf, int nOffset);

	// �����ַ�������ʾ����(���ַ�������Ϊ��λ)
	static int	GetStrShowLen(const char* pszStr, int nBegin, int nEnd);

	// ���Ƿ�(��ͼ��)�ַ�
	static bool	CheckStringPixels(const char* pszString, DWORD uMinPixels, const char* pszFont, int nFontSize);
	
	// ��ʾ�ַ���(֧�ֻس�)
	// pszFont:     ��������TTF/TTC�����ļ���
	// bAntialias:	ʹ�ÿ��������
	// style:		������
	// secondColor: ��Ӱ/���/�����ɫ
	// ptOffset:	��Ӱ/���ƫ����
	static void	ShowString(int iPosX, int iPosY, DWORD color, const char *pszString, const char* pszFont = NULL, int nFontSize = 0, bool bAntialias = false, RENDER_TEXT_STYLE style = RENDER_TEXT_SIMPLE, DWORD secondColor = 0xFF000000, C3_POS ptOffset = C3_POS());
	
	// ��ʾ�����ַ���(֧�ֱ���ͼ��)
	static void ShowEmotionString(int iPosX, int iPosY, DWORD color, const char* pszString, IDataIcon* pDataIcon,
									 const char* pszFont = NULL, int nFontSize = 14,
									 int nImgSize = 0, int nImgOffsetX = 0, int nImgOffsetY = 0, 
									 RENDER_TEXT_STYLE style = RENDER_TEXT_SIMPLE, DWORD secondColor = 0xFF000000, C3_POS ptOffset = C3_POS());
		
public: // camera
	static bool GameCameraCreate();
	
	// �м�,��Ҫ����GameCameraDestroy!!!
	static void GameCameraDestroy();
	
	// ��ͷ�ߴ�Ӧ�÷��ϵ�ǰ��Orientation.
	static void GameCameraBuild(int nWidth, int nHeight);
	static void GameCameraBuild();
	static void GameCameraSet(float fX, float fZ, float fY = -1000.0f, DWORD dwOpt = CAMERA_BOTH);
	static void GameCameraGetPos(float& x, float& y, float& z, DWORD dwOpt = CAMERA_EYE);
	static void GameCameraMove(bool bForward = true);
	static void GameCameraRotate(int nAngle);
	static void GameCameraZoom(bool bZoomIn = true);
	static void GameCameraSetFov(float fFovDegree);
	static void GameCameraSetType(bool bProject);
	static bool GameCameraIsProject();
	static void GameCameraCircum(D3DXVECTOR3 vCenter, float fAngleDegree);
	static void GameCameraPitch(D3DXVECTOR3 vCenter, float fAngleDegree);
	
	// �Զ����ݵ�ǰOOrientation�޸ľ�ͷ������
	static bool GameCameraSetOrientation(int nOrientationMode);
	static void GameCameraLook(const D3DXVECTOR3& vPos, const D3DXVECTOR3& vViewAt, const D3DXVECTOR3& vUp);
	
	// Note: fAspectΪ0,��ʹ�û����豸�ĳ������,����Ӧ�÷��ϵ�ǰPortraitOrientation
	static void GameCameraPerspectiveLook(float fFovDegree, float fAspect, float zNear, float zFar);
	static void GameCameraOrthoLook(float fCameraWidth, float fCameraHeight, float zNear, float zFar);
	
	// ��������3D��ͷ�Ľ�ƽ���Զƽ��,�����������ĳ�����ֵ����,������ͷ��Զƽ�淶Χ.
	static void GameCameraSetNearFarPlane(float fNear, float fFar);
	
public: // init and destory
	// ��ʼ��,����EAGLDrawableָ��
	static bool Init3DEx(int nBackBufferWidth, int nBackBufferHeight, int nPixelFormat, bool bHasStencilBuffer, const char* pszResourcePath);

	static bool Init3DEx(HNATIVEWINDOW hNativeWindow, int nDepthSize, bool bNeedStencilBuffer, const char* pszResourcePath);

	// ��ֹ3d
	static void Quit3D();
	
	// ̽��3d�豸
	static int	IfDeviceLost();
	
	// ����3d�豸
	static bool ResetDevice();
	
	// ���û���ϸ������
	// 1.������ֵΪtrue,��ʾ��Ҫ����������ܵõ�����Ч��,�ϲ�Ӧ�����´�����,����3D�豸ǰ,���ø÷���.
	// 2.��Ҫ��Begin3D��End3D֮����ø÷���
	enum { GRAPHIC_DETAIL_FAST = 0, GRAPHIC_DETAIL_NORMAL = 1, GRAPHIC_DETAIL_GOOD = 2 }; 
	static bool HintGraphicDetail(DWORD nGraphicDetailLevel);
	
public:	// 3d relative
	// ��ʼ3d����
	static bool Begin3D();
	
	// ����3d����
	static bool End3D();
	
	// ���zbuffer����Ļ
	static bool ClearBuffer(bool bZBuffer, bool bTarget, DWORD color);
	
	// ��Begin3D/End3D֮���Draw����̨�����Ļ���,��ʽˢ��������Ļ��.
	// hDestWindowOverride: ˢ���ĸ�������
	// pDestRect: ˢ�����ڵ�ָ�����η�Χ,NULL��ʾ�����������.
	// pSourceRect: ʹ��Դ������ָ�����η�Χ,NULL��ʾ��������.
	static bool Flip();
	
	// ׼�����ƾ���
	static void PrepareSprite();
	
	// ׼�����ƻ�Ͼ���
	static void PrepareMixSprite();
	
	// ׼��������
	static void	PrepareLine();
	
	// ׼������3D��
	static void	Prepare3DLine();
	
	// ׼��������
	static void	PrepareModel();
	
	static void PrepareLinearSprite();
	
	// ���õ�1յ�ƹ�,x,y,zΪ��������Է���
	static void	SetLight(bool bEnable,  float x, float y, float z, DWORD dwColorLight, DWORD dwColorAmbient, float fSpecularPower);
	
	// ȡ��Ļ�ӿڴ�С
	static void	GetScreenSize(int& nWidth, int& nHeight);
	static int	GetScreenWidth();
	static int	GetScreenHeight();
	
	// ���ߺ���
	struct LineVertex
	{
		float		x, y;		// ����
		DWORD	color;
	};
	
	static void ShowLine(LineVertex* line, DWORD num);
	static void ShowLine(int nPosX1, int nPosY1, int nPosX2, int nPosY2, DWORD dwColor);
	
	// �����ο�
	static void	ShowRect(int x1, int y1, int x2, int y2, DWORD color);
	
	// ��ʵ�ľ���
	static void ShowBlock(int x1, int y1, int x2, int y2, DWORD color);
	static void ShowBlock(int x1, int y1, int x2, int y2, DWORD color1, DWORD color2, DWORD color3, DWORD color4);	
	static void ShowBlock(C3_POS posA, C3_POS posB, C3_POS posC, C3_POS posD, DWORD color);	

	// ��3ά��
	static void Show3DBox(const D3DXVECTOR3& vMin, const D3DXVECTOR3& vMax, DWORD color, bool bWireLine);

	// ����ĻΪJPEG��ScreenShotĿ¼
	static void	ScreenShot(const char* pszFileName);

	// ȡ��ǰ֡���Ƶ�����������
	static DWORD GetShowTriTotal();

	// ȡ��ɫ��б�Ƕ�(���x����ת)
	static int GetObliqueAngle();

	// ����ļ�
	static void	PackFileOpen(const char* pszPackFile);
	static void	PackFileClose();

	// ͨ�ú���
	static void Common_Translate(D3DXMATRIX* pMatrix, float x, float y, float z);	
	static void Common_Rotate(D3DXMATRIX* pMatrix, float xRadian, float yRadian, float zRadian);
	static void Common_Scale(D3DXMATRIX* pMatrix, float x, float y, float z);
	
	static void Motion_GetMatrix(C3Motion* pMotion, DWORD dwBone, D3DXMATRIX* pMatrix);
	static void Motion_GetMatrix_Blend(HC3MOTIONTRACKER hMotionTracker, DWORD dwBone, D3DXMATRIX* pMatrix);
	
	static void ScreenToWorld(int nScrX, int nScrY, float& x, float& y, float& z);
	static void WorldToScreen(float x, float y, float z, int& nScrX, int& nScrY);
	static void	ScrRayBuild(int nScrX, int nScrY, D3DXVECTOR3& vFrom, D3DXVECTOR3& vDir);

	static void SetReflectFluidSpeed(float fUChangePerSecond, float fVChangePerSecond);
	
	// ��ȡָ����С����ĵȿ������Ű�,����ֵΪ���ֽ��ַ���,�������2.
	static DWORD GetFontWidth(const char* pszFont = NULL, int nFontSize = 12);

	static bool ChangeDisplayResulotion(HEAGLDRAWABLE hEaglDrawable, DWORD dwWidth, DWORD dwHeight);
	
	// �����ӿڴ�С,������ڴ����еĳ����С.
	// bDeviceOrLogic: 
	// Ϊtrue,�����ӿڵ������С
	// Ϊfalse,�����ӿڵ��߼���С,���ӿ������С����������,�ı����߼���С(���е�C3��Ⱦ���������߼���λ).
	// Note: ������CTargetBmp��Ⱦ������ʹ��
	static bool ChangeViewportResolution(DWORD dwWidth, DWORD dwHeight, bool bDeviceOrLogic = true);

	// ���ȿ�����֧��
	// ����ֵΪ��Ⱦ�����ַ�Χ�ľ��δ�С
	static C3_SIZE ShowStringEx(int iPosX, int iPosY, DWORD color, const char *pszString, const char* pszFont = NULL, int nFontSize = 0, bool bAntialias = false, 
								RENDER_TEXT_STYLE style = RENDER_TEXT_SIMPLE, DWORD secondColor = 0xFF000000, C3_POS ptOffset = C3_POS());
	
	static C3_SIZE ShowEmotionStringEx(int iPosX, int iPosY, DWORD color, const char* pszString, IDataIcon* pDataIcon,
									 const char* pszFont = NULL, int nFontSize = 14, bool bAntialias = false, 
									 int nImgSize = 0, int nImgOffsetX = 0, int nImgOffsetY = 0, 
									 RENDER_TEXT_STYLE style = RENDER_TEXT_SIMPLE, DWORD secondColor = 0xFF000000, C3_POS ptOffset = C3_POS());
	
	// ����������������Ļ����ʾ�ķ�Χ�ľ��δ�С
	// Note: pDataIcon == NULLʱ,#64���������Ž����ַ�����ʾ���Ƚ���.
	static C3_SIZE CalcuTextExtent(const char* pszString, const char* pszFont = NULL, int nFontSize = 0, IDataIcon* pDataIcon = NULL, int nImageSize = 0);
	
	static void ProbeRayBuild(int nScrX, int nScrY, D3DXVECTOR3& vFrom, D3DXVECTOR3& vDir);
	
	#define LINE_3DVERTEX ( D3DFVF_XYZ | D3DFVF_DIFFUSE )
	struct Line3DVertex
	{
		float	x, y, z;		// ����
		DWORD	color;			// ABGR Format
	};
	static void Show3DLine(Line3DVertex* pLineVertex, DWORD nLineCount);
	
	// FontDrawBegin/FontDrawEnd���ʹ��.
	// ��FontDrawBegin��FontDrawEnd֮���ShowString,ShowTString,ShowEmotionString
	// ���������ύ������ʾ,���ǽ������ۻ�����,ֱ��FontDrawEndʱ�������ύ��Ⱦ.
	// Note: ����������ʾʱ��ʹ�øýӿ�!
	static void FontDrawBegin();
	static void FontDrawEnd();

	// ������б�Ƕȣ����x����ת)
	static void SetObliqueAngle(int nAngle);
	
	static const char* GetDefaultFontName();
	
	// Unicode������ʾ֧��
	static C3_SIZE ShowStringW(int iPosX, int iPosY, DWORD color, const wchar_t* pszString, const char* pszFont = NULL, int nFontSize = 0, bool bAntialias = false, 
								RENDER_TEXT_STYLE style = RENDER_TEXT_SIMPLE, DWORD secondColor = 0xFF000000, C3_POS ptOffset = C3_POS());
	
	static C3_SIZE ShowEmotionStringW(int iPosX, int iPosY, DWORD color, const wchar_t* pszString, IDataIcon* pDataIcon,
		const char* pszFont = NULL, int nFontSize = 14, bool bAntialias = false, 
		int nImgSize = 0, int nImgOffsetX = 0, int nImgOffsetY = 0, 
		RENDER_TEXT_STYLE style = RENDER_TEXT_SIMPLE, DWORD secondColor = 0xFF000000, C3_POS ptOffset = C3_POS());
	
	static C3_SIZE CalcuTextExtentW(const wchar_t* pszString, const char* pszFont = NULL, int nFontSize = 0, IDataIcon* pDataIcon = NULL, int nImageSize = 0);
	
	// ������nMaxPixelWidth��Ļ�������������Ⱦ�����ֽ��ַ� 
	static int CalcuShowStringRangeEx(const char* pszString, int nBytesLen, int nMaxPixelWidth, const char* pszFont, int nFontSize, bool bAntialias, IDataIcon* pDataIcon = NULL, int nImageSize = 0);

	// �򿪹ر�/����
	// Note: ���֧��3յ�ƹ�
	static void EnableLight(int nLightIndex, bool bEnable);
	
	// ��ԭ��graphic.ini���õĹ��ղ���
	// Note: ��ʷ����ԭ��,GameCameraSetTypeҲ���Զ�����RestoreConfigLight��ԭ���ղ���.
	static void RestoreConfigLight(bool bProject);
	
	// �ֶ����ù��շ���(��������)
	static void SetLightDirection(int nLightIndex, float x, float y, float z, bool bProject = true);
	
	// ���ù����ɫ
	static void SetLightDiffuseColor(int nLightIndex, DWORD dwColorLight);
	
	// ���ù�Ļ�������ɫ
	static void SetLightAmbientColor(int nLightIndex, DWORD dwColorAmbiemt);
	
	// ��Ŀ���
	// �򿪹ر���,Ĭ����ر�
	// Note: �����Ӱ��������е���Ⱦ����.
	static void EnableFog(bool bEnable);
	
	// ������������
	// fFogStart/fFogEnd����Ծ�ͷ�ľ���.
	static void SetFogEnv(DWORD dwFogColor, float fFogStart, float fFogEnd);
	
	// Point & Spot Light
	// Note: ���֧��3յ�ƹ�,�Ƿ����Թ�ԴӦ���ڷ����Թ�Դ֮ǰ.
	// Note: ֻ�ܰ�˳��ʹ�������ƹ�,������ɢʹ��!!!
	static void SetLightType(int nLightIndex, C3_RENDER_LIGHT_TYPE type);

	static void SetLightColor(int nLightIndex, DWORD dwColorDiffuse, DWORD dwColorAmbient);
	
	static void SetLightPosition(int nLightIndex, float x, float y, float z);
	
	static void SetPointLightAttenuation(int nLightIndex, float a, float b, float c);
	
	static void SetSpotLightFalloff(int nLightIndex, float fInnerAngleRadian, float fOuterAngleRadian, float fFalloff = 1.0f);

	static D3DXMATRIX GetCurrentViewMatrix();

	static D3DXMATRIX GetCurrentViewMatrixInverse();

	static D3DXMATRIX GetCurrentProjectMatrix();

	static D3DXVECTOR2 GetCameraSize(bool bProject);

	static void ImageToWorld(int nImageX, int nImageY, bool bNearPlane, DWORD nImageWidth, DWORD nImageHeight, const D3DXMATRIX& matView, const D3DXMATRIX& matProject, float& x, float& y, float& z);
	static void WorldToImage(float x, float y, float z, DWORD nImageWidth, DWORD nImageHeight, const D3DXMATRIX& matView, const D3DXMATRIX& matProject, int& nImageX, int& nImageY);

	// ����ShowStringϵ�к�������ʾ���,����ShowStringϵ�к����ܴ�Ӱ��.
	static void SetShowStringScale(float fScale);
	
	// ��Ļ�ü�
	static bool BeginClip(C3_RECT& rect);
	static void EndClip();
	
	// ��ʾC3��ǰ��Դ����
	static void DrawC3ResourceInfo(int x, int y);
	
	// ��Ⱦһ���Զ��������ģ��.
	// pVertices,pTexCoords,pVertexColors,pVertNormals: ��������,��pVertices���������ݿ���ΪNULL.
	// dwVertCount: �������
	// nStride: �������ݽṹ��С
	// pIndices: ��������,ΪNULL,��ÿ3���������1����������Ⱦ.
	// dwTriCount: �����θ���
	// pTex: C3����,����ΪNULL,��ʹ��ʹ������ҲӦ�ô���һ��4X4�İ�ɫ����.
	// dwColor: ������ɫ,����pTex != NULLʱ������.
	// dwAsb,dwAdb: D3DBLEND_XXX,DX���ģʽ
	// pMatWorld: �������,ΪNULL,����ԭ��.
	static void DrawTrianglesUp(D3DXVECTOR3* pVertices, D3DXVECTOR2* pTexCoords, DWORD* pVertexColors, D3DXVECTOR3* pVertNormals, 
								DWORD dwVertCount, int nStride, WORD* pIndices, DWORD dwTriCount, 
								C3Texture* pTex, DWORD dwColor, DWORD dwAsb, DWORD dwAdb, D3DXMATRIX* pMatWorld, bool bTwoSided = true);
	
	// ʹ��CPuzzleBlock����Ļ�ϻ�һ������,��͸���ĵط�Ϊ����.
	static void BeginDrawClipMask();
	static void EndDrawClipMask();

	// ʹ������,��BeginUseClipMask/EndUseClipMask֮���������Ⱦ��������������.
	static void BeginUseClipMask();
	static void EndUseClipMask();
};

// Note: ע��iPad/iPhone����Ļ������ת,����Ļ��Width/Height����Portraitʱ�ĸ���.
#define _SCR_WIDTH	CMyBitmap::GetScreenWidth()
#define _SCR_HEIGHT CMyBitmap::GetScreenHeight()

class CAni
{	
public:
	virtual unsigned long Release() = 0;
	
	virtual bool Create(const char* pszDataFile, const char* pszIndex) = 0;
	virtual bool Show(DWORD uFrame, int x, int y, int alpha = 0, DWORD dwShowWay = _SHOWWAY_NORMAL) const = 0;
	virtual bool ShowEx(DWORD uFrame, int x, int y, C3_RECT* lpSrc, DWORD dwWidth, DWORD dwHeight, int alpha = 0, DWORD dwShowWay = _SHOWWAY_NORMAL) = 0;
	
	virtual CMyBitmap* GetFrame(DWORD uFrameIndex) const = 0;
	virtual int	GetFrameAmount() const = 0;
	
	virtual DWORD GetSize() const = 0;
	virtual C3_SIZE GetSize(DWORD uFrame) const = 0;
	
	virtual bool ShowLinear(DWORD uFrame, int x, int y, DWORD dwWidth = 0, DWORD dwHeight = 0, C3_RECT* pSrcTex = NULL, int alpha = 0, DWORD dwShowWay = _SHOWWAY_NORMAL) = 0;

	virtual void LockLife(bool bLock) = 0;

	virtual bool IsLifeLocked() = 0;
};

class CPuzzleCell
{
public:
	virtual unsigned long Release() = 0;
	virtual void SetVertexColor(int x, int y, DWORD dwARGB, int nLayer = 1) = 0;
	virtual void Show(int x, int y, int nWidth, int nHeight, CMyBitmap* pBmp1, CMyBitmap* pBmp2 = NULL, CMyBitmap* pBmp3 = NULL, CMyBitmap* pBmp4 = NULL) = 0;
};

class CPuzzleTriangle
{
public:
	virtual unsigned long Release() = 0;

	virtual void SetBlend(DWORD dwAsb, DWORD dwAdb, DWORD dwColorOp) = 0;
	virtual void SetVertexData(int idxVertex, int x, int y, DWORD color, float u, float v) = 0;
	virtual void Show(CMyBitmap* pBmp) = 0;
};

typedef enum 
{
	C3_MIX_MODULATE	  = 4,
	C3_MIX_MODULATE2X = 5,
	C3_MIX_MODULATE4X = 6,
	C3_MIX_ADD        = 7
}	TEXTURE_SHADER_OPERATION;

typedef enum 
{
	C3_BLEND_ZERO               = 1,
    C3_BLEND_ONE                = 2,
    C3_BLEND_SRCCOLOR           = 3,
    C3_BLEND_INVSRCCOLOR        = 4,
    C3_BLEND_SRCALPHA           = 5,
    C3_BLEND_INVSRCALPHA        = 6,
	C3_BLEND_DESTALPHA          = 7,
    C3_BLEND_INVDESTALPHA       = 8,
    C3_BLEND_DESTCOLOR          = 9,
    C3_BLEND_INVDESTCOLOR       = 10,
}	FRAME_BLEND_OPERATION;

/*********************************************************************
**  �����ύ��ʾһ�ű��ָ�ɾ��󷽸��ͼ��.
**  ����ָ������ÿ���ĵ����ɫ,����������ս��ʵ��.
*********************************************************************/
class CPuzzleBlock
{
public:
	virtual unsigned long Release() = 0;
	
	// ����Ŀ����εĴ�С
	// Note: ���ӵĺ��ݸ�����PuzzelBlockCreate�Ĳ���ָ��.
	virtual void SetSize(DWORD dwWidth, DWORD dwHeight) = 0;
	
	// ���û�Ϸ�ʽ
	virtual void SetBlend(DWORD dwAsb, DWORD dwAdb, DWORD dwColorOp) = 0;
	
	// ���ö�����ɫ x <= w, y <= h
	// Note: Ĭ����ɫ��0xFFFFFFFF(��ɫ)
	virtual void SetVertexColor(int x, int y, DWORD color) = 0;
	
	// ���ö������������ x <= w, y <= h
	// Note: Ĭ���������갴����ƽ���ָ�.
	virtual void SetVertexUv(int x, int y, float u, float v) = 0;
	
	// ��ʾ
	virtual void Show(int nPosX, int nPosY, CMyBitmap* pBmp) = 0;
	
	// ����Alpha��Ϸ�ʽ
	virtual void SetAlphaOperation(DWORD dwAlphaOpt) = 0;
};

/*********************************************************************
**  ����һ���ύ��ʾͬһ��ͼ���ж����ͬ������,���߽�ͬһ��ͼ����ʾ��
**  ��Ļ�Ķ����ͬ����.
**  ��������ʾ�ߵ�,��ת,�������ͼ������.
*********************************************************************/
class CMultiBmp
{
public:
	virtual	unsigned long Release() = 0;

	// Ԥ֪�����ڴ�,�����Ż��ڴ�.
	virtual void PresetPartCount(DWORD dwPartCount) = 0;

	virtual void AddPartEx(int nShowX, int nShowY, int nWidth, int nHeight, DWORD dwColor = 0xFFFFFFFF, 
						   float fU0 = 0.0f, float fV0 = 0.0f, float fU1 = 1.0f, float fV1 = 1.0f) = 0;
	
	// nShowX, nShowY, nWidth, nHeight: ������ʾ����Ļ���ڵľ��η�Χ(��֧��nWidth/nHeightĬ��ȡ0�ļ���).
	// [fU0,fV0] - [fU3,fV3] ����Դ��ͼ�е�һ���ı��η�Χ,��Z����˳����. UV����ȡֵ��ΧΪ[0 - 1].
	virtual	void AddPart(int nShowX, int nShowY, int nWidth, int nHeight,
						 float fU0 = 0.0f, float fV0 = 0.0f, float fU1 = 1.0f, float fV1 = 0.0f,
						 float fU2 = 0.0f, float fV2 = 1.0f, float fU3 = 1.0f, float fV3 = 1.0f) =	0;
	
	// ���AddPart��������о���
	virtual	void ClearPart() = 0;
	
	// ʹ��pBmp��ʾAddPart�����������ͼ����.
	virtual	void Show(CMyBitmap* pBmp, D3DBLEND srcBlend = D3DBLEND_SRCALPHA, D3DBLEND dstBlend = D3DBLEND_INVSRCALPHA) = 0;
};

/*********************************************************************
**  BeginScene/EndScene�����������Ϸ��ͼ��ѭ������ 
**  ShowӦ������Ϸ��ͼ��ѭ��������
**  �豸��ʧʱ����ɾ������CTargetBmp���� �����豸�޷��ָ�
**  �ڽ���һ��5X5�Ķ��㼯�� ������aoxpuzzle��Alpha�ع�
*********************************************************************/
class CTargetBmp
{
public:
	virtual	DWORD Release()	= 0;

	// ��ʼ��Ⱦ����ͼ
	virtual	bool BeginScene() =	0;

	// ������Ⱦ����ͼ
	virtual	void EndScene()	= 0;
		
	// ��ʾTargetBmp
	virtual	void Show(int x, int y, int nWidth, int nHeight) = 0;
	virtual	void SetAlpha(BYTE uIndex, BYTE uAlpha) = 0;
	virtual	void ShowBlend(int x, int y, int nWidth, int nHeight, DWORD dwColor) = 0;

	// Note: ������C3Textureָ��Ϊ�ڲ�����,�ⲿ����ɾ��.
	virtual	C3Texture* QueryC3Texture()	= 0;

	virtual int	QueryTextureID() = 0;
	 
	// ʹ����Ȼ���,Ĭ��ʱû����Ȼ���Ͳ���.
	virtual bool UseDepthSurface() = 0;

	// ���ݵ�ǰRenderTarget����һ��Managed����.
	virtual C3Texture* MakeManagedTexture() = 0;
	
	virtual C3Texture* QueryDepthTexture() = 0;
};

/*********************************************************************
**  3D ���μ�, ������, һ���ύ, �Ż�Ч����.
**  ������������3D�е���Ƭ/���ǵȼ�����.
*********************************************************************/
class CMultiRect
{
public:
	virtual	unsigned long Release()	= 0;
	
	// �����Ҫ��Ⱦ�Ŀռ�ƽ�����(Note: ��Z����������������4������)
	virtual	void AddRect(const D3DXVECTOR3 verts[4], const D3DXVECTOR2 texCoords[4]) = 0;
	
	// ���AddRect�ľ���
	virtual	void ClearRect() = 0;

	virtual	void Show(CMyBitmap* pBmp, const D3DMATERIAL* pMaterial, bool bTwoSide = false, DWORD dwAsb = D3DBLEND_SRCALPHA, DWORD dwAdb = D3DBLEND_INVSRCALPHA) = 0;

	virtual	void Show(CTargetBmp* pBmp, DWORD dwAsb = D3DBLEND_SRCALPHA, DWORD dwAdb = D3DBLEND_INVSRCALPHA) = 0;
	
	// �����Ҫ��Ⱦ�Ŀռ�ƽ�����(Note: ����ʱ��˳��������������4������)
	virtual bool AddRenderRect(const D3DXVECTOR3 verts[4], const D3DXVECTOR2 texCoords[4], DWORD dwColor = 0xFFFFFFFF, const D3DXVECTOR3& normal = D3DXVECTOR3(0.0f, -1.0f, 0.0f)) = 0;
	
	// ��Ⱦ
	virtual	void Draw(C3Texture* pC3Tex, const D3DMATERIAL* pMaterial, DWORD dwAsb = D3DBLEND_SRCALPHA, DWORD dwAdb = D3DBLEND_INVSRCALPHA, bool bTwoSide = true, bool bWriteZBuffer = true) = 0;

	// ȡ��ǰ�Ѿ���ӵľ��θ���
	virtual DWORD GetRectCount() = 0;
};

class C3DGridPlane
{
public:
	virtual	unsigned long Release()	= 0;
	
	// ȡ�ú��ݸ�����(Note: ���ݶ�����Ϊ��������1)
	virtual int GetRowCount() = 0;
	
	virtual int GetColCount() = 0;
	
	// ������Ҫ��Ⱦ�Ŀռ�ƽ�����(Note: ����ʱ��˳��������������4������)
	virtual void SetRenderRect(const D3DXVECTOR3 verts[4], const D3DXVECTOR2 texCoords[4], const D3DXVECTOR3& normal = D3DXVECTOR3(0.0f, -1.0f, 0.0f)) = 0;
		
	virtual void SetTransform(const D3DXMATRIX& matWorld) = 0;
	
	virtual void SetColor(DWORD dwColor) = 0;
	
	// ���ù���ͼӳ��ƽ�����(Note: ����ʱ��˳��������������4������)
	virtual	void SetLightMapRect(const D3DXVECTOR3 verts[4], D3DXVECTOR3 normal) = 0;
	
	// �Թ���ͼ��ʽ��ʾ
	virtual	void ShowLightMap() = 0;
	
	virtual	void Draw(C3Texture* pC3Tex, const D3DMATERIAL* pMaterial, DWORD dwAsb = D3DBLEND_SRCALPHA, DWORD dwAdb = D3DBLEND_INVSRCALPHA, bool bTwoSide = true, bool bWriteZBuffer = true) = 0;
	
	virtual bool Probe(const D3DXVECTOR3& vOrigin, const D3DXVECTOR3& vDir, float& t) = 0;
	
	virtual void* GetStridePositionArray(int& nStride, int& nCount) = 0;
	
	virtual void* GetStrideTexCoordArray(int& nStride, int& nCount) = 0;
};

//////////////////////////////////////////////////////////////////////
struct C3Texture;
class C3DTexture
{
public:
	virtual unsigned long Release() = 0;
	
	virtual bool Create(const char* pszFileName) = 0;
	virtual void Destroy() = 0;
	
	virtual int	GetTexID() = 0;

	// Note: ������C3Textureָ��Ϊ�ڲ�����,�ⲿ����ɾ��.
	virtual C3Texture* GetTexture() = 0;
	
	virtual bool Reload() = 0;
	
	virtual int GetWidth() = 0;
	
	virtual int GetHeght() = 0;
	
	virtual void Show(int x, int y, int nDestWidth = 0, int nDestHeight = 0, C3_RECT* rcTex = NULL, DWORD dwColor = 0xFFFFFFFF, DWORD dwShowWay = _SHOWWAY_NORMAL, int nRotateAngle = 0) = 0;
	
	virtual void ShowBlend(int x, int y, DWORD dwWidth = 0, DWORD dwHeight = 0, C3_RECT* pSrcTex = NULL, DWORD dwColor = 0xFFFFFFFF, DWORD dwSrcBlend = D3DBLEND_SRCALPHA, DWORD dwDestBlend = D3DBLEND_INVSRCALPHA) = 0;
};

/**************************************************************
**  C3�Ǽ���Դ�ӿ�
**  Note: ע��C3DSkeleton����Դ�ӿ�,��������ʵ���ӿ�.
**  ��Դ�ӿڱ�ʾ�ö��󽫱����ʹ����������,���ı仯����������
**  ʹ���ߵı仯.һ����Դ�ӿڿ��Դ�����ʵ���ӿ�.
**  C3DSkeleton��C3DMotion�����������Ĺ̶��Ķ�������.
**  C3DDynamicSkeletonInstance���ɳ�����ƵĶ�������,��
**  C3DDynamicSkeletonInstance����C3DMotion�������յ�����+����
**  ���Ͽ��ƵĶ���.
**************************************************************/
class C3DDynamicSkeletonInstance;
class C3DSkeleton
{
public:
	virtual void Release() = 0;
	
	// �������Ʋ���ĳ������
	virtual int FindBoneByName(const char* pszBoneName) = 0;
	
	// ȡ�Ӷ�������,�����Ǽܶ������������㶯��
	virtual int GetSubMotionCount() = 0;

	// ȡ�Ǽܶ���������λ��
	virtual int GetBodyMotionIndex() = 0;

	// ȡ�����󶨵�
	virtual D3DXVECTOR3 GetPivotPoint(DWORD dwBoneId) = 0;

	virtual int GetParentBoneID(DWORD dwBoneId) = 0;
	
	virtual int GetVirtualPhyBoneId(DWORD dwVirtualPhyIndex) = 0;
	
	// ������̬�����ĹǼ�ʵ��
	virtual C3DDynamicSkeletonInstance* CreateDynamicSkeletonInstance() = 0;

	virtual const char* GetBoneName(int nBoneID) = 0;
	
	virtual void GetBoneReferencePose(int nBoneID, D3DXVECTOR3& vPivot, D3DXQUATERNION& qRotation) = 0;

	virtual void GetBoneReferencePose(int nBoneID, D3DXMATRIX& matBone) = 0;
	
	virtual int GetBoneCount() = 0;

	virtual int GetVirtualPhyCount() = 0;
};

/**************************************************************
**  C3��̬�Ǽ�ʵ���ӿ�
**************************************************************/
class C3DDynamicSkeletonInstance
{	
public:
	virtual void Release() = 0;
	
	// ȡ�����õ�ʵ�ʹǼ�
	virtual C3DSkeleton* GetReferenceSkeleton() = 0;
	
	// �������Ʋ���ĳ������
	virtual int FindBoneByName(const char* pszBoneName) = 0;
	
	// ����ĳ�������ı任����
	virtual bool TransformBoneDynamically(int nBoneID, const D3DXMATRIX& matBoneTransform) = 0;
	
	// ����ĳ�������ı任�˶�,���ܹ�����.
	virtual bool TransformBoneDynamically(const char* pszBoneName, const D3DXMATRIX& matBoneTransform) = 0;
	
	// ���û�ԭĳ�������ı任�˶�,���ܹ�����.
	virtual bool ResetBoneTransform(const char* pszBoneName) = 0;

	// ����ID���ҹ�������
	virtual const char* GetBoneName(int nBoneID) = 0;
	
	// �������֮ǰ��TransformBoneDynamically������
	virtual void ResetAllBoneState() = 0;
};

//////////////////////////////////////////////////////////////////////
struct C3Motion;
class C3DMotion
{
public:
	virtual unsigned long Release() = 0;

	virtual bool Create(const char *filename, bool bBufferMotion) = 0;
	virtual void Destroy() = 0;

	virtual DWORD GetFrameAmount() = 0;
	virtual	DWORD GetMotionAmount() = 0;

	virtual C3Motion* GetMotion(int idx) = 0;
	
	virtual DWORD GetMotionKeyFrames(int idxMotion) = 0;
	virtual const D3DXMATRIX& GetKeyFrameMatrix(int idxMotion, int idxKeyFrame) = 0;
	
	virtual void Reload(const char* pszFileName) = 0;
	
	virtual const char* GetDataBaseName() = 0;		// ��������Դ��ذ汾
	
	virtual void LockLife(bool bLock) = 0;
	virtual bool IsLifeLocked() = 0;
	
	// ���øö��������ĹǼ�
	virtual bool SetSkeleton(C3DSkeleton* pSkeleton) = 0;
	
	// ��øö��������õĹǼ�
	virtual C3DSkeleton* GetSkeleton() = 0;
	
	// ���㵱ǰ��������ڻ�׼����p3DBasedMotion��ƫ�ƾ���.
	virtual bool CalcuSkeletonOffsetMatrix(C3DMotion* p3DBasedMotion, D3DXMATRIX& matOffset) = 0;
	
	// ���趯���Զ�̬�����ĹǼ�,ʹ�������������Եı任.
	virtual bool ApplyDynamicSkeleton(C3DDynamicSkeletonInstance* pDynamicSkeletonInst) = 0;
	
	virtual void ResetTrackerState() = 0;
	
	// ȡ��ǰ֡(��SetFrame����)�¹��������ģ��ԭ��ľ���.
	virtual bool GetCurrentSkeletonBoneMatrix(int nBoneID, D3DXMATRIX& matBone) = 0;
	
	// ���õ�ǰ֡(��SetFrame����)�¹��������ģ��ԭ��ľ���.
	virtual bool SetCurrentSkeletonBoneMatrix(int nBoneID, const D3DXMATRIX& matBone) = 0;
};

//////////////////////////////////////////////////////////////////////
struct C3Phy;
struct IPhyLoadConfig;
class C3DObj
{
public:
	virtual unsigned long Release() = 0;

	// ʵ���Ƿ���Ч
	virtual bool IsValid() = 0;

	// �Ƿ�������
	virtual bool IsDummy(int idx) const = 0;

	// ����һ��ģ�� *.c3
	virtual bool Create(const char* filename, IPhyLoadConfig& phyLoadConfig) = 0;

	// �ͷ�
	virtual void Destroy() = 0;

	// ����� x ��Ӧ��Ļ����� x, z ��Ӧ��Ļ����� y, 1 ����λ���� 1 ������
	// �ƶ�
	virtual void Move(float x, float y, float z) = 0;
	
	// ��ת, ���������ǻ���
	virtual void Rotate(float x, float y, float z) = 0;

	// ���ţ� 1.0 ����ԭ��С
	virtual void Scale(float x, float y, float z) = 0;

	// �ӳ��˶�����
	virtual void Multiply(D3DXMATRIX& matrix) = 0;

	// ��ɫ����Χ�� 0.0f - 1.0f
	virtual void SetARGB(float alpha, float red, float green, float blue) = 0;

	// ͨ���������ֵõ���������
	virtual int GetIndexByName(const char* pszName) = 0;

	// ͨ������ȡ��������
	virtual const char* GetNameByIndex(int idx) = 0;

	// ȡphy����
	virtual DWORD GetPhyAmount() const = 0;

	// ͨ���������bounding box
	virtual void GetBoundingBox(int idx, D3DXVECTOR3& vctMin, D3DXVECTOR3& vctMax) = 0;

	// ���ö���
	virtual void SetMotion(C3DMotion* pMotion) = 0;
	virtual void SetBlendMotion(C3DMotion* pMotion, float fWeight) = 0;

	// ͨ������ȡ�ö���
	virtual C3Motion* GetMotion(int idx) = 0;
	virtual C3Motion* GetBlendMotion(int idx) = 0;

	// ͨ������ȡ�����綯������
	virtual D3DXMATRIX&	GetWorldMatrix(int idxPhy) = 0;

	// ��һ֡ nStep ���� ��ֱΪ����
	virtual void NextFrame(int nStep) = 0;

	// ���õ�ǰ֡��
	virtual void SetFrame(DWORD dwFrame) = 0;
	virtual void SetBlendFrame(DWORD dwFrame) = 0;

	// ����
	virtual void Draw(int typeShadow, const D3DXVECTOR3& vLight, float fShadowAlpha, 
					  DWORD asb = D3DBLEND_SRCALPHA, DWORD adb = D3DBLEND_INVSRCALPHA, 
					  bool bIgnoreStaticMotion = false, bool bMirror = false) = 0;
	
	// ����ͼ
	virtual void ChangeTexture(C3DTexture* pTexture, const char* pszPhyName = NULL) = 0;
	virtual void ChangeMixTex(C3DTexture* pTexture, const char* pszPhyName = NULL) = 0;

	// ������ͼ��Ϸ�ʽ�ͻ�ϲ���
	virtual void SetMixOpt(int nOpt, DWORD dwData) = 0;

	// ���ò���
	virtual void SetMaterial(const D3DMATERIAL& mat) = 0;

	// ����ϴ���Ⱦ��ģ��ʹ�õ��˶��������ɫ
	virtual void ClearMatrix() = 0;

	// ����ת���˶�����
	// matInPlace: ԭ�ؾ���,������ȷ����ԭ����Ӱ
	// matTrans: ƫ�ƾ���
	virtual void SetTransform(const D3DXMATRIX& matInPlace, const D3DXMATRIX& matTrans, const D3DXMATRIX& matWorld) = 0;

	// ȡ���֡��
	virtual DWORD GetMaxFrameCount() = 0;

	virtual bool ProbeIntersection(const D3DXVECTOR3& vFrom, const D3DXVECTOR3& vDir, float& t, bool bNeedMotion, int* pPhyIndex = NULL, int* pTriFaceIndex = NULL) = 0;

	virtual C3Phy* GetPhy(int idx) = 0;

	virtual bool AddProcPhy(DWORD dwTriangleCount, D3DXVECTOR3* pVertices, D3DXVECTOR2* pTexCoords, D3DXVECTOR3* pNormals = NULL) = 0;

	virtual bool AddProcPhyEx(DWORD dwVertexCount, DWORD dwTriangleCount, int nStride, D3DXVECTOR3* pTriVertices, D3DXVECTOR2* pTexCoords, D3DXVECTOR3* pTriNormals, DWORD* pVertColors, unsigned short* pIndices) = 0;
	
	virtual void Reload(const char* pszFileName, IPhyLoadConfig& phyLoadConfig) = 0;
	
	virtual bool LoadPhyFaceProperty(const char* pszModelFileName, const char* pszPropertyFileName) = 0;
	
	virtual unsigned char GetFaceProperty(int nPhyIndex, int nFaceIndex) = 0;

	virtual C3Motion* GetBodyMotion() = 0;

	virtual C3Motion* GetBodyBlendMotion() = 0;

	virtual C3DMotion* GetCurrent3DMotion() = 0;

	virtual C3_PUBLIC_MODEL_VERTEX* GetModelVertexData(int& nStride, int& nCount) = 0;

	virtual WORD* GetTriangleFaceData(int& nTriangleCount) = 0;
};

//////////////////////////////////////////////////////////////////////
typedef void* HSHAPEANIMTRACKER;
typedef void* HPTCLMOTIONTRACKER;
typedef void* HPTCL3MOTIONTRACKER;

class C3DEffect  
{
public:
	virtual unsigned long Release() = 0;
	
	virtual bool Load(const char* pszFilename) = 0;
	virtual void Unload() = 0;

	virtual void Bind(C3DObj* lpObj, const char* pszPhyName = NULL, bool bIsBindToBone = false, D3DXMATRIX* pMatSelfTransform = NULL) = 0;
	virtual void Draw(bool bNextFrame = true, DWORD dwSrcBlend = D3DBLEND_SRCALPHA, DWORD dwDestBlend = D3DBLEND_INVSRCALPHA, bool bZuffer = false, DWORD dwStepCount = 0, int billboardType = 0, D3DXVECTOR3* pCenter = NULL) = 0;
	virtual void SetTexture(C3DTexture* pTexture) = 0;
	virtual void ResetShape() = 0;
	
	virtual void Move(float x, float y, float z) = 0;
	virtual void Rotate(float xRadian, float yRadian, float zRadian) = 0;
	virtual void Scale(float x, float y, float z) = 0;
	virtual void SetMatrix(const D3DXMATRIX& matrix) = 0;
	
	virtual void SetColor(DWORD dwColor) = 0;
	
	virtual DWORD GetPtclAmount() const = 0;
	virtual void SetPtclCurFrame(DWORD dwIndex, DWORD dwFrame) = 0;
	virtual int	GetPtclFrames(DWORD dwIndex) const = 0;
	
	virtual DWORD GetPhyAmount() const = 0;
	virtual void SetPhyCurFrame(DWORD dwIndex, DWORD dwFrame) = 0;
	virtual int	GetPhyFrames(DWORD dwIndex) const = 0; 
	
	virtual DWORD GetMaxFrames() const = 0;
	virtual void SetFrame(DWORD dwFrame) = 0;
	
	virtual DWORD GetShapeAmount() const = 0;
	
	virtual HSHAPEANIMTRACKER CreateShapeInfo() = 0;
	virtual void BackupShapeInfo(HSHAPEANIMTRACKER hShapeAnimTracker) = 0;
	virtual void RestoreShapeInfo(HSHAPEANIMTRACKER hShapeAnimTracker) = 0;
	
	virtual HPTCLMOTIONTRACKER CreatePtclInfo() = 0;
	virtual void SetPtclInfo(HPTCLMOTIONTRACKER info) = 0;

	virtual HPTCL3MOTIONTRACKER CreatePtcl3Info() = 0;
	virtual void SetPtcl3Info(HPTCL3MOTIONTRACKER info) = 0;
	virtual void SetGravity(D3DXVECTOR3 vGravity) = 0;
	virtual	void SetExternalSpeed(D3DXVECTOR3 vSpeed) = 0;
	virtual void SetTexture(int nTextureID) = 0;
	virtual bool GetAnimationCenter(D3DXVECTOR3& vCenter) = 0;
	virtual void Reload(const char* pszFileName) = 0;
	virtual const D3DXMATRIX& GetWorldMatrix() = 0;

	virtual bool Set3DMotion(C3DMotion* p3DMotion) = 0;

	virtual bool HitTest(const D3DXVECTOR3& vFrom, const D3DXVECTOR3& vDir) = 0;
};

//////////////////////////////////////////////////////////////////////
class C3DScene
{
public:
	virtual unsigned long Release() = 0;

	// ����һ��c3
	virtual bool Create(const char* filename) = 0;

	// ���õ�ǰ֡��
	virtual void SetFrame(DWORD dwFrame) = 0;

	// ����
	virtual void Draw(bool bStaticScene) = 0;

	// ����ͼ
	virtual void ChangeTexture(C3DTexture* pTexture, const char* pszObjName = NULL) = 0;
	virtual void ChangeMixTex(C3DTexture* pTexture, const char* pszObjName = NULL) = 0;

	// ������ͼ��Ϸ�ʽ
	virtual void SetMixOpt(int nOpt) = 0;

	// ���û��ģʽ
	virtual void SetBlendMode(int asb, int adb) = 0;

	// ���ò�����ɫ
	virtual void SetColor(DWORD dwARGB) = 0;

	// ���ò���
	virtual void SetMaterial(const D3DMATERIAL& mat) = 0;
	
	// �ཻ���㣬true��ʾ���ཻ, ����ж���ཻ�㣬����zֵ��С�ĵ㡣
	virtual bool HitTest(int nScrX, int nScrY, float& x, float& y, float& z) = 0;
};

// Graphic������ ///////////////////////////////////////////////////////////////
extern "C" GRAPHIC_API CMyBitmap* MyBitmapCreate(const char* pszFile);
extern "C" GRAPHIC_API CMyBitmap* BmpCreate(void);
extern "C" GRAPHIC_API CPuzzleCell* PuzzelCellCreate();
extern "C" GRAPHIC_API CPuzzleTriangle* PuzzelTriangleCreate();
extern "C" GRAPHIC_API CPuzzleBlock* PuzzelBlockCreate(DWORD dwWidth, DWORD dwHeight, DWORD dwGridCountX, DWORD dwGridCountY);
extern "C" GRAPHIC_API CMultiBmp* MultiBmpCreate(void);
extern "C" GRAPHIC_API CMultiRect* MultiRectCreate(void);
extern "C" GRAPHIC_API CTargetBmp* TargetBmpCreate(DWORD uWidth, DWORD uHight);

// ��C3Texture����C3DTexture��CMyBitmap����.
// bAutoDeleteC3Texture:�Ƿ���C3DTexture/CMyBitmap��ɾ��ʱ,�Զ�ɾ������ʱ�����C3Texture.
extern "C" GRAPHIC_API C3DTexture* TextureCreateEx(C3Texture* pC3Tex, bool bAutoDeleteC3Texture);
extern "C" GRAPHIC_API CMyBitmap* MyBitmapCreateEx(C3Texture* pC3Tex, bool bAutoDeleteC3Texture);
extern "C" GRAPHIC_API CTargetBmp* TargetBmpCreateEx(DWORD uWidth, DWORD uHight);
extern "C" GRAPHIC_API CMyBitmap* MyBitmapCreateFrom3DTexture(C3DTexture* p3DTexture);
extern "C" GRAPHIC_API C3DTexture* TextureCreateFromTargetBmp(CTargetBmp* pTargetBmp);
extern "C" GRAPHIC_API CMyBitmap* MyBitmapCreateFromTargetBmp(CTargetBmp* pTargetBmp);
extern "C" GRAPHIC_API C3DGridPlane* GridPlaneCreate(DWORD dwGridRow, DWORD dwGridCol);

extern "C" GRAPHIC_API void SetCartoonSilhouetteWidth(float fWidth);
extern "C" GRAPHIC_API void SetCartoonSilhouetteColor(DWORD dwColor);

#endif//_GRAPHIC_H