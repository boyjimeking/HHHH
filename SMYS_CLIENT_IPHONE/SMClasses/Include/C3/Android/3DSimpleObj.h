#ifndef _3DSIMPLEOBJ_H
#define _3DSIMPLEOBJ_H

#if defined(_WIN32)
#ifndef ROLE3D_API
#ifdef ROLE3D_EXPORTS
#define ROLE3D_API __declspec(dllexport)
#else
#define ROLE3D_API __declspec(dllimport)
#endif
#endif
#else
#define ROLE3D_API
#endif

#include "C3Primitive.h"

class C3DMotion;
class C3DTexture;
class C3DSimpleObj 
{
public:
	virtual unsigned long Release() = 0;
	
	virtual bool Create(OBJID idType) = 0;
	
	// Note: �˺������nMapX,nMapY��������,������ʷ�ӿڼ���,һ�ɴ�0.
	// ����false������Ҫ����Դ��δ�������.
	virtual bool Draw2BG(int nMapX = 0, int nMapY = 0, int nAsb = D3DBLEND_SRCALPHA, int nAdb = D3DBLEND_INVSRCALPHA) = 0;
	
	// Note: �˺������nMapX,nMapY��������,������ʷ�ӿڼ���,һ�ɴ�0.
	virtual bool HitTest(int nHitScreenX, int nHitScreenY, int nMapX, int nMapY) = 0;
	
	// ����λ�ú�Yaw�Ƕ�
	// Note: nYʵ�������������������zֵ.
	// Note: nHeight�������ǽ����������Zֵ����nHeight * 2. (I dont know why? It must be a legacy param.)
	virtual void SetPos(int nX, int nY, int nHeight, int nRotateDegree, float fScale) = 0;
	
	// SetDeep���õ���3d��yֵ,���д˷��������˴�yֵ,Ĭ��yֵΪ0.0f.
	virtual void SetDeep(float fDeep) = 0;
	
	// ������X�����б�Ƕ�
	virtual void SetRotateX(int nRotateXDegree) = 0;
	
	// ���������ŷ����ת�Ƕ�,�����ΰ�x,y,z����ת.
	// �����x,y,z��3D����ϵ,ע�⵽��2.5D��ͼ��3D��Z������,Y������.
	virtual void SetRotate(float xRadian, float yRadian, float zRadian) = 0;
	
	// ������Ӱ���ղ���
	// �����x,y,z��3D����ϵ,ע�⵽��2.5D��ͼ��3D��Z������,Y������.
	virtual void SetLightOffset(int type, float x, float y, float z, float a, float r = 0.0f, float g = 0.0f, float b = 0.0f) = 0;
	
	// �Ƿ�Ҫ��X����б,Ϊtrue.��б�Ƕ���SetRotateXָ��,Ĭ��Ϊ-30��.
	virtual void SetOblique(bool bOblique) = 0;
	
	// �����Ƿ���Ⱦ��Ӱ,nShadowType=2ʱ��Ⱦ��Ӱ.
	virtual void SetShadowType(int nShadowType) = 0;

	// ���ö���
	virtual void SetObjMotion(int64_t idMotion) = 0;

	// ������ɫ
	virtual void SetARGB(float a, float r, float g, float b) = 0;
	
	// ������ͼ,Note:
	// 1.SetTexture,SetARGB�޸ĵ��������ͻ�Ϸ�ʽ,����Set֮���1��C3DSimpleObj::Draw2BG��Ч,�ٺ��Draw2BG��Ȼʹ�õ�ini���õ�����ͻ�Ϸ�ʽ.
	// 2.��ָ��Ϊ��,��ʹ��ini���õ�����.
	virtual void SetTexture(C3DTexture* pTex, C3DTexture* pMixTex, int nMixOpt = TEXMIX_MODULATE) = 0;
	
	// ���ö���֡
	virtual void SetFrame(DWORD dwFrame) = 0;
	
	// ���ö���
	virtual void SetMotion(C3DMotion* pMotion) = 0;
	
	// ΪC3DSimpleObj����Ч,��Ч��ʼ����ģ�͵�ԭ��.
	virtual void AddEffect(const char* pszEffect) = 0;
	
	// ����������ű���,�����x,y,zָ3D�����x,y,z.
	virtual void SetScale(float x, float y, float z) = 0;

	// ������Դ
	virtual bool Reload() = 0;

	// ȡ��ǰ�󶨵Ķ��������֡��
	virtual DWORD GetMaxFrameCount() = 0;
	
	// ���ø�3DSimpleObjΪ��ֹ����,�Խ�ʡ��������.
	virtual void SetStatic(bool bStatic) = 0;
	
	// ��Ļ���ڵ������,��ȷ���ռ�������,��Ļ���ؼ�.
	// ���ص�tΪ���߹�ʽ: R = v0 + t * vDir, ����v0Ϊ���еĽ�ƽ���Ͷ���,vDirΪ��ͷ���߷���.
	// Ϊʹ�����˶��µĵ�������ȷ,��ȷ�ϵ��ø÷���ʱ����������ȷ��Motion��Frame.
	virtual bool ProbeIntersection(int nHitScreenX, int nHitScreenY, float& t) = 0;

	// ���ҵߵ�����ʾģ��,��������.
	virtual void SetMirror(bool bMirror) = 0;
	
	// Ϊ����ʽC3DSimpleObj��Ӷ�������
	// 1.����ǰ������ʹ��Simple3DProcedureObjCreate������,����false.
	// 2.���ڲ�ʵ���������ֻ�����16��������,�����򷵻�false.
	// 3.dwTriangleCountΪ�����θ���, pTriVertices��pTexCoords�ֱ�ָ�򶥵��������������,ÿ3�����㹹��1��������.
	virtual bool AddProcPhy(DWORD dwTriangleCount, D3DXVECTOR3* pTriVertices, D3DXVECTOR2* pTexCoords, D3DXVECTOR3* pTriNormals = NULL) = 0;
	
	virtual bool AddProcPhyEx(DWORD dwVertexCount, DWORD dwTriangleCount, int nStride, D3DXVECTOR3* pTriVertices, D3DXVECTOR2* pTexCoords, D3DXVECTOR3* pTriNormals, DWORD* pColors, unsigned short* pIndices) = 0;

	virtual void ClearProcPhy() = 0;
	
	virtual void ChangeTextureMap(OBJID idTex, OBJID idMixTex, int nMixOpt) = 0;
	
	// ��Ļ���ڵ������,��ȷ���ռ�������,��Ļ���ؼ�.
	// ���ص�tΪ���߹�ʽ: R = v0 + t * vDir, ����v0Ϊ���еĽ�ƽ���Ͷ���,vDirΪ��ͷ���߷���.
	// Ϊʹ�����˶��µĵ�������ȷ,��ȷ�ϵ��ø÷���ʱ����������ȷ��Motion��Frame.
	// ���ص�ucPropertyΪ���е��������Ӧ�ò�����ֵ. ������ֵ������ģ���ļ���Ӧ�������ļ���.
	// bNeedMotion: ��ȷ����Ƿ���Ҫ���Ƕ�����Ӱ��.
	virtual bool ProbeObjInfo(int nHitScreenX, int nHitScreenY, bool bNeedMotion, float& t, unsigned char& ucProperty) = 0;
	
	// ����˫������Ļ�Ϸ�ʽ
	virtual void SetMixOperation(int nMixOpt) = 0;
	
	// �󶨹�Ч��ĳ����������
	virtual void BindEffect(const char* pszEffect, const char* pszBindPhyName) = 0;
		
	// ����������ȫ3D�µ�λ��
	virtual void SetPosition(float x, float y, float z) = 0;
	
	// ����˫����ͼ��Ϸ�ʽ������
	virtual void SetTextureShader(int nOperation, DWORD dwOperationData) = 0;

	virtual void SetTexture2(C3DTexture* pTex, C3DTexture* pMixTex, int nOperation, DWORD dwOperationData) = 0;
	
	// ������Ӱ����
	virtual void SetShadowInfo(int type, float x, float y, float z, float fShadowAlpha) = 0;
	
	// �����ݴ�����������Ⱦ,��������SetXXX��λ������.
	virtual bool Draw3D(const D3DXMATRIX& matInplace, const D3DXMATRIX& matTranslate, int nAsb = D3DBLEND_SRCALPHA, int nAdb = D3DBLEND_INVSRCALPHA, D3DMATERIAL* pMaterial = NULL) = 0;
	
	virtual C3_PUBLIC_MODEL_VERTEX* GetModelVertexData(int& nStride, int& nVertexCount, int nPartIndex = 0) = 0;

	virtual WORD* GetTriangleData(int& nTriangleCount, int nPartIndex) = 0;
	
	// ���ǹ����˶���Ϣ��HitTest,��������ֻ��һ�������ļ�����.
	virtual bool HitTest3D(int nHitScreenViewX, int nHitScreenViewY, const D3DXMATRIX& matInplace, const D3DXMATRIX& matTranslate) = 0;
};

// ����3DSimpleObj.ini/3DSimpleObjEx.ini�����õ����IDֱ�Ӵ���һ��3DSimpleObj
extern "C" ROLE3D_API C3DSimpleObj* Simple3DObjCreate(OBJID idType = ID_NONE);

// ������ʽָ���������ԴID����һ��3DSimpleObj
extern "C" ROLE3D_API C3DSimpleObj* Simple3DObjCreate2(OBJID idMesh, OBJID idTex, OBJID idMixTex, int nMixOpt, int nAsb, int nAdb);

// ����һ������ʽ�����3DSimpleObj,����Ķ�������ͨ��C3DSimpleObj::AddProcPhy����.
// ������������C3DSimpleObj�ѱ�ǿ��SetStatic(true).
extern "C" ROLE3D_API C3DSimpleObj* Simple3DProcedureObjCreate(OBJID idTex, OBJID idMixTex = 0, int nMixOpt = 1, int nAsb = D3DBLEND_SRCALPHA, int nAdb = D3DBLEND_INVSRCALPHA);

#endif//_3DSIMPLEOBJ_H