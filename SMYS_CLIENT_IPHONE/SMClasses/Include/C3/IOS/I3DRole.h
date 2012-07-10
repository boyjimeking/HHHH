#ifndef I3DROLE_H
#define I3DROLE_H

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
#include "D3DXMathe.h"

class C3DMotion;
class C3DTexture;
class CGame3DEffectEx;
class I3DRole
{
public:
	virtual unsigned long Release() = 0;
 
	// ���ѡ��̽��
	// Note: �˺������nOffsetX,nOffsetY,��������,������ʷ�ӿڼ���,һ�ɴ�0.
	virtual bool HitTest(int nHitScreenX, int nHitScreenY, int nOffsetX, int nOffsetY, const char* pszPartName = NULL) = 0;

	// �ж���Դ�Ƿ����
	virtual bool IsPartResourceReady(const char* pszPartName) = 0;

	// ��ʾָ������,����ģ�͵����������λ�����ڴ˽ӿ��д����.
	// Note:������λ�û�����ֲ������Ӱ��,�ֲ�������AddPart��SetPartSelfMatrix�ӿ��д���.
	virtual void Draw(const char* pszPartName, 
				  const D3DXMATRIX& matrixCal, 
				  const D3DXMATRIX& matrixTrans, 
				  bool bClearMatrix) = 0;

	// ���Ӳ���, ϵͳ����part_name.ini����ѰidType����Ϣ������none����null����ʾ�ޡ�
	// ���idTypeΪ0��������һ���յ�part������֧��ֱ�����������ϰ���Ч��
	virtual bool AddPart(const char* pszPartName, 
					  const char* pszFollowDumy, 
					  const char* pszDumyPartName,
					  OBJID idType,
					  const D3DXMATRIX* matPartSelf = NULL) = 0;

	// �ϲ���������source_part.ini��idSrcPartType�Ĳ�����Ϣ�ϲ���Ŀ�겿��target_part��ȥ��
	virtual bool MergePart(const char* pszTargetPart, 
					  const char* pszSourcePart, 
					  OBJID idSrcPartType) = 0;

	// �����������Ŀ�겿���а�����ָ�����������ȥ��
	virtual bool DepartPart(const char* pszTargetPart, 
					  const char* pszSourcePart, 
					  OBJID idSrcPartType) = 0;

	// ����Part�������
	virtual bool SetPartSelfMatrix(const char* pszPartName, D3DXMATRIX* matPartSelf) = 0;

	// ɾ������
	virtual void DelPart(const char* pszPartName) = 0;

	// ���Բ�������
	virtual bool TestPart(const char* pszPartName) = 0;
	
	// ����,�޸Ĳ�����Ϣ
	virtual void SetPartInfo(const char* pszPartName, const char* pszFollowDumy, const char* pszDumyPartName) = 0;

	// ���õ�ǰ����
	virtual void SetPartMotion(const char* pszPartName, C3DMotion* pMotion) = 0;
	virtual void SetPartDefaultMotion(const char* pszPartName) = 0;
	virtual void SetPartBlendMotion(const char* pszPartName, C3DMotion* pMotion, float fWeight) = 0;

	// ���ָ�������Ķ���
	virtual void ClrPartMotion(const char* pszPartName) = 0;

	// ������Ӱ����
	// Note: sr,sg,sbû������,����(��ʷ����).
	virtual void SetShadowInfo(int nShadowType, float fLightX, float fLightY, float fLightZ, 
								  float sa = 0.35f, float sr = 0.0f, float sg = 0.0f, float sb = 0.0f) = 0;
	
	// ���û������,������Ե�ͼ��ѧ������ο�DirectX�鼮��MSDN,��ֱ��ʹ�ù�������.
	virtual void SetBlendMode(int nAsb, int nAdb) = 0;
	
	// ������ͼ�������������ΪNULL���������в���
	// 1.SetTexture,SetTexMixOpt�޸ĵ��������ͻ�Ϸ�ʽ,����Set֮���1��IRole3D::Draw��Ч,�ٺ��Draw��Ȼʹ�õ�ini���õ�����ͻ�Ϸ�ʽ.
	// 2.��ָ��Ϊ��,��ʹ��ini���õ�����.
	virtual void SetTexture(C3DTexture* pTex, C3DTexture* pMixTex, const char* pszPartName = NULL) = 0;
	
	// ������ͼ��Ϸ�ʽ,�����������ΪNULL���������в���
	// ��Ϸ�ʽ��ͼ��ѧ����,��ο�������3D�༭��.
	virtual void SetTexMixOpt(int nOpt, const char* pszPartName = NULL) = 0;
	
	// ������ɫ
	// Note: ClrPartMotion�Ὣ���õ���ɫ���.
	virtual void SetColor(float a, float r, float g, float b) = 0;

	// ���õ�ǰ֡
	virtual void SetFrame(DWORD dwFrame) = 0;
	virtual void SetBlendFrame(DWORD dwFrame) = 0;

	// �����������Ч, ����Ϊnullʱ���в�����������
	// bAllowRepeat: �Ƿ������ظ������ͬ��Ч.
	virtual void AddEffect(const char* pszEffect, const char* pszPartName = NULL, bool bAllowRepeat = false) = 0;

	// ������ɾ����Ч, ����Ϊnullʱ���в�����������
	virtual void ClrEffect(const char* pszEffect, const char* pszPartName = NULL) = 0;

	// ���Բ�������Ч
	virtual bool TestEffect(const char* pszEffect, const char* pszPartName = NULL) = 0;

	// ���¼��ز�������
	virtual bool Reload() = 0;
	
	// ��Ӳ�����ЧpszEffect, ������pszPartName��pszBindPhyName��������,��Ч�������������˶�.
	// Note: AddEffect��ӵ���Чֻ�Ǹ��沿���˶�.
	virtual void AddPartEffect(const char* pszEffect, const char* pszPartName = NULL, const char* pszBindPhyName = NULL, bool bAllowRepeat = false) = 0;
	
	// ��Ļ���ڵ������,��ȷ���ռ�������,��Ļ���ؼ�.
	// ���ص�tΪ���߹�ʽ: R = v0 + t * vDir, ����v0Ϊ���еĽ�ƽ���Ͷ���,vDirΪ��ͷ���߷���.
	// Ϊʹ�����˶��µĵ�������ȷ,��ȷ�ϵ��ø÷���ʱ����������ȷ��Motion��Frame.
	virtual bool ProbeIntersection(int nHitScreenX, int nHitScreenY, float& t, const char* pszPartName = NULL) = 0;
	
	// ���ҵߵ�����ʾģ��,��������.
	virtual void SetMirror(bool bMirror) = 0;

	// ȡ�������󶨵���Ч�弯�Ͻӿ�
	virtual CGame3DEffectEx* GetGame3DEffect(const char* pszPartName) = 0; 
	
	// ���㲿��pszPartName�ϵ�������pszDummyName�ĵ�ǰ֡(SetFrame)�ı任����.
	// Note: �˱任��������������԰󶨵����ϵĲ���/��Ч�Ŀռ�����.
	// ����Ҳ���������������,����false.
	virtual bool CalculateVirtualMotion(const char* pszPartName, const char* pszDummyName, D3DXMATRIX& matResult) = 0;
	
	// ���õ���Part����ɫ,��idMergedPartType��Ϊ0,������Part�ж�Ӧ��merge��Obj
	// Note: ClrPartMotion�Ὣ���õ���ɫ���.
	virtual bool SetColorEx(float alpha, float red, float green, float blue, const char* pszPartName, const char* pszMergedPartName, OBJID idMergedPartType = 0) = 0;
	
	// �޸�ĳ��Part����������
	virtual void ChangeTextureMap(OBJID idTex, OBJID idMixTex, int nMixOpt, const char* pszPartName = NULL) = 0;
	
	// ��Ļ���ڵ������,��ȷ���ռ�������,��Ļ���ؼ�.
	// ���ص�tΪ���߹�ʽ: R = v0 + t * vDir, ����v0Ϊ���еĽ�ƽ���Ͷ���,vDirΪ��ͷ���߷���.
	// Ϊʹ�����˶��µĵ�������ȷ,��ȷ�ϵ��ø÷���ʱ����������ȷ��Motion��Frame.
	// ���ص�ucPropertyΪ���е��������Ӧ�ò�����ֵ. ������ֵ������ģ���ļ���Ӧ�������ļ���.
	// bNeedMotion: ��ȷ����Ƿ���Ҫ���Ƕ�����Ӱ��.
	virtual bool ProbeObjInfo(int nHitScreenX, int nHitScreenY, bool bNeedMotion, float& t, unsigned char& ucProperty, const char* pszPartName = NULL) = 0;
	
	// ��ĳ�����󵽹Ǽܵ�����һ��������
	virtual void BindPartToBodyBone(const char* pszPartName, const char* pszFollowPartName, const char* pszFollowBoneName) = 0;
	
	// ֱ�����ÿռ����λ��
	virtual void SetTransform(const D3DXMATRIX& matInplace, const D3DXMATRIX& matTrans) = 0;
	
	// ��Ӳ��󶨲�����ĳ��������
	virtual void AddEffectBindToBone(const char* pszEffect, const char* pszPartName = NULL, const char* pszBoneName = NULL, bool bAllowRepeat = false) = 0;
	
	virtual bool ProbeRoleInfo(int nHitScreenX, int nHitScreenY, bool bNeedMotion, float& t, unsigned char& ucProperty) = 0;
	
	// ����˫����ͼ��Ϸ�ʽ������
	// 1.SetTexture,SetTexMixOpt�޸ĵ��������ͻ�Ϸ�ʽ,����Set֮���1��IRole3D::Draw��Ч,�ٺ��Draw��Ȼʹ�õ�ini���õ�����ͻ�Ϸ�ʽ.
	virtual void SetTextureShader(int nOperation, DWORD dwOperationData, const char* pszPartName = NULL) = 0;

	// �ֲ������õ�ǰ֡
	virtual bool SetFrameEx(DWORD dwFrame, const char* pszPartName) = 0;
	virtual bool SetBlendFrameEx(DWORD dwFrame, const char* pszPartName) = 0;

	virtual bool SetMaterial(const D3DMATERIAL& material) = 0;
};

extern "C" ROLE3D_API I3DRole* I3DRoleCreate();

// һ������������. Note:ħ����Ŀʹ����LoadAdjustConfig,���������ͼ�й�Чλ�õĴ���,���ͼ���ݲ�ͬ��������Ŀ!!!
extern "C" ROLE3D_API void LoadAdjustConfig();

extern "C" ROLE3D_API void SyncObliqueAngle();

#endif//I3DROLE_H