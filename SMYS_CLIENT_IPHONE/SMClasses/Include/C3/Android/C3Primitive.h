#ifndef __C3_PRIMITIVE_H__
#define __C3_PRIMITIVE_H__

#include "C3BaseType.h"	// System data typedef
#include "DXPrimitive.h"// C3 depend on DX Primitives

typedef void* HEAGLDRAWABLE;
typedef void* HNATIVEWINDOW; 

// ˫�������Ϸ�ʽ
typedef enum 
{ 
	TEXMIX_NONE					= 2,	// D3DTOP_SELECTARG1,		
	TEXMIX_MODULATE				= 4,	// D3DTOP_MODULATE,				
	TEXMIX_DARK_MAP				= 4,	// D3DTOP_MODULATE
	TEXMIX_MODULATE2X			= 5,	// D3DTOP_MODULATE2X,		
	TEXMIX_MODULATE4X			= 6,	// D3DTOP_MODULATE4X,		
	TEXMIX_ADD					= 7,	// D3DTOP_ADD,					
	TEXMIX_ADDSIGNED			= 8,	// D3DTOP_ADDSIGNED,				
	TEXMIX_ADDSIGNED2X			= 9,	// D3DTOP_ADDSIGNED2X,
	TEXMIX_SUBTRACT				= 10,	// D3DTOP_SUBTRACT,				
	TEXMIX_ADDSMOOTH			= 11,	// D3DTOP_ADDSMOOTH,			
	TEXMIX_DECAL_MAP			= 13,	// D3DTOP_BLENDTEXTUREALPHA,	
	TEXMIX_BLEND_FACTOR_ALPHA	= 14,	// D3DTOP_BLENDFACTORALPHA,		
	TEXMIX_BLEND_PRE_ALPHA		= 15,	// D3DTOP_BLENDTEXTUREALPHAPM,	
	TEXMIX_BLEND_CUR_ALPHA		= 16,	// D3DTOP_BLENDCURRENTALPHA,	
	TEXMIX_REFLECT				= 17,	// ������ͼ
	TEXMIX_REF_BASEALPHA		= 18,
	TEXMIX_SILHOUETTE			= 19,	// ��Ⱦ��Ӱ
	TEXMIX_CARTOON_SILHOUETTE	= 20,	// ��ͨ����
	TEXMIX_CARTOON_SHADOW		= 21,	// ��ͨɫ����Ӱ
	TEXMIX_CARTOON_FULLSTYLE	= 22,	// ��ͨ����+��ͨɫ����Ӱ
	TEXMIX_FLUID_REFLECT		= 23,	// ����ʽ������ͼ
	TEXMIX_USE_FIRST_RGB_SECOND_ALPHA = 25,// ʹ�õ�1�������RGB,����ȡ��2�������Alpha.
	TEXMIX_USE_FIRST_RGB_MODULATE_SECOND_ALPHA = 26,// ʹ�õ�1�������RGB,����ȡ��1������͵�2�������Alpha���ֵ.		
	TEXMIX_BLEND_CLIENT_ALPHA	= 27,	// ��������ͼ��ʹ��Alphaֵ�仯���,��������Ч��.
	TEXMIX_DOT3_NORMALMAP		= 28,
	TEXMIX_ENV_SPHEREMAP		= 29,
	TEXMIX_ENV_PROJECTIVESHADOW = 30,
	TEXMIX_ENV_SHADOWMAP		= 31,
}	C3_TEXMIX_TYPE;

// ���ַ��
typedef enum 
{
	RENDER_TEXT_SIMPLE = 0, 
	RENDER_TEXT_SHADOW = 1,			// ��ӰЧ��
	RENDER_TEXT_SILHOUETTE = 2,		// ����Ч��
	RENDER_TEXT_ENGRAVE_BLOCK = 3,	// ���Ч��
	RENDER_TEXT_COLORFUL = 4,		// ������ɫ����
}	RENDER_TEXT_STYLE;

// ��������
typedef enum 
{
	C3_RENDER_LIGHT_POINT          = 1,
	C3_RENDER_LIGHT_SPOT           = 2,
	C3_RENDER_LIGHT_DIRECTIONAL    = 3,
}	C3_RENDER_LIGHT_TYPE;

// ��Ӱģʽ
typedef enum
{
	C3_SHADOW_INVALID				= 0,
	C3_SHADOW_NONE					= 1,
	C3_SHADOW_25D_POINT_PLANAR		= 2,	// 2.5D����ģʽ(����Z������),�������ģ��ԭ��ĵ��Դλ��.
	C3_SHADOW_3D_POINT_PLANAR		= 3,	// ȫ3D,�������ģ��ԭ��(��ģ������ϵ)�ĵ��Դλ��
	C3_SHADOW_3D_DIRECTION_PLANAR	= 4,	// ����ƽ�й�ķ���
	C3_SHADOW_TYPE_MAX				= 5,
}	C3_SHADOW_TYPE;

// ��������
enum 
{
	C3_HINT_GRAPHIC_FLAGS			= 0xFFFFFFF0,
	C3_HINT_USE_STENCILBUFFER		= 0x80000000,
};

// ͼ����ʾ��ʽ
enum
{
	_SHOWWAY_NORMAL	  = 0,
	_SHOWWAY_ADDITIVE = 1,
};

// ��ɫֵ
enum 
{	
	_COLOR_WHITE	=	0xFFFFFFFF,
	_COLOR_GRAY		=	0xFF7F7F7F,
	_COLOR_DARKGRAY	=	0xFF3F3F3F,
	_COLOR_BLACK	=	0xFF000000,
	_COLOR_RED		=	0xFFFF0000,
	_COLOR_GREEN	=	0xFF00FF00,
	_COLOR_BLUE		=	0xFF0000FF,
	_COLOR_YELLOW	=	0xFFFFFF00,
};

typedef unsigned int OBJID;
const OBJID	ID_NONE = 0;

typedef struct 
{
	int	x;
	int   y;
}	C3_POS;

typedef struct
{
	int left;
	int top;
	int right;
	int bottom;
}	C3_RECT;

typedef struct 
{
	float fHorizontal;
	float fVertical;
}	C3_DIR;

typedef struct 
{
	int  iWidth;
	int  iHeight;
}	C3_SIZE;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR2 uv;
}	C3_PUBLIC_MODEL_VERTEX;

#endif//__C3_PRIMITIVE_H__