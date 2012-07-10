#ifndef _GAMEDATASET_H
#define _GAMEDATASET_H

#include "C3Primitive.h"
#include "DXPrimitive.h"

#if defined(_WIN32)
#ifdef GRAPHICDATA_EXPORTS
#define GRAPHICDATA_API __declspec(dllexport)
#else
#define GRAPHICDATA_API __declspec(dllimport)
#endif
#else
#define GRAPHICDATA_API
#endif

class CAni;
class C3DObj;
class C3DMotion;
class C3DEffect;
class C3DTexture;
class C3DScene;

enum { _MAX_EFFECT_C3 = 16 };

// Note: ��ֱ��ʹ��CMy3DEffectInfo�ṹ����,��ȷ��C3�汾�Ƿ�֧��
struct CMy3DEffectInfo
{
	char szIndex[32];
	int nAmount;
	OBJID idEffect[_MAX_EFFECT_C3];
	OBJID idTexture[_MAX_EFFECT_C3];
	int	nAsb[_MAX_EFFECT_C3];
	int nAdb[_MAX_EFFECT_C3];
	int nScale[_MAX_EFFECT_C3];
	int nFrameOffset[_MAX_EFFECT_C3];
	int nC3FrameInterval[_MAX_EFFECT_C3];
	unsigned char nLoopOnce[_MAX_EFFECT_C3];
	bool bZBuffer[_MAX_EFFECT_C3];
	D3DXVECTOR3 vSelfOffset[_MAX_EFFECT_C3];
	unsigned char ucBillboard[_MAX_EFFECT_C3];
	int nDelay;
	int nLoopTime;
	int nFrameInterval;
	int nLoopInterval;
	int nOffsetX;
	int nOffsetY;
	int nOffsetZ;
	int nBillboard;
	int nRunLevel;				// ����ʱ��Ҫ�ȼ�,֡�ʵ�ʱ�ײ��(��ֵ��)�Ĺ�Ч������.
	bool bColorEnable;			// �Ƿ�������ɫ,Ĭ���¹�Ч����ɫ������Ӱ��.
};

enum { _MAXPART_SIMPLEOBJ = 4 };
struct C3DSimpleObjInfo
{
	OBJID idType;
	int	nParts;
	OBJID idMesh[_MAXPART_SIMPLEOBJ];
	OBJID idTexture[_MAXPART_SIMPLEOBJ];
};

struct C3DSimpleObjInfoEx : C3DSimpleObjInfo
{
	OBJID idMixTex[_MAXPART_SIMPLEOBJ];
	int	nMixOpt[_MAXPART_SIMPLEOBJ];
	const D3DMATERIAL* material[_MAXPART_SIMPLEOBJ];
	int	nAsb[_MAXPART_SIMPLEOBJ];
	int	nAdb[_MAXPART_SIMPLEOBJ];
};

enum { MAX_PART = 4 };
struct RolePartInfo
{
	int	nPart;
	OBJID idMesh[MAX_PART];
	OBJID idTexture[MAX_PART];
	OBJID idMixTex[MAX_PART];
	int	nMixOpt[MAX_PART];
	const D3DMATERIAL* material[MAX_PART];
	int	nAsb[MAX_PART];
	int	nAdb[MAX_PART];
};

enum 
{ 
	C3_EXIGENCE_NORMAL = 0,					  // �첽����	
	C3_EXIGENCE_IMMEDIATE = 1,					  // ��������	
	C3_EXIGENCE_ABOVENORMAL = 2,				  // ����(��ʷ����)
	C3_EXIGENCE_FORCE_BACKGROUND = 3			  // ��̨����	
};
enum 
{ 
	INTERFACE_ICONANI = 0 
};
enum 
{ 
	DETAIL_HIGHT, 
	DETAIL_NORMAL, 
	DETAIL_LOW
};
enum 
{
	RELOAD_3DTEXTURE		= 0x1,				// ������������Դ
	RELOAD_3DOBJ			= 0x2,				// ������3DObj��Դ
   	RELOAD_3DEFFECT			= 0x4,				// ������3DEffect��Դ
   	RELOAD_3DMOTION			= 0x8,				// ������3DMotion��Դ
  	RELOAD_DATA_ANI			= 0x10,				// ������Data Ani��Դ
   	RELOAD_3DTEXTURE_INI	= 0x20,				// ������3DTexture.ini����
   	RELOAD_3DEFFECT_INI		= 0x40,				// ������3DEffectObj.ini,3DEffect.ini����
   	RELOAD_3DOBJ_INI		= 0x80,				// ������3DObj.ini����
   	RELOAD_ROLEINFO_INI		= 0x100,			// ����RolePart.ini��RolePart.ini�����õ���armor.ini,3dmotion.ini�Ƚ�ɫ����
   	RELOAD_3DSIMPLEOBJ_INI	= 0x200,			// ������3DSimpleObj.ini����
    RELOAD_DATA_ANI_INI		= 0x400,			// ������DataAni������
	RELOAD_3DSCENE			= 0x800,			// ����3DScene
	RELOAD_ALL				= 0xFFF,			// ����ȫ��
};

enum
{
	C3_INIT_ONLY_ANI = 1
};

struct C3DObjInfo;
class CGameDataSet 
{
public:
	virtual bool Init(const char* pszAppFolder) = 0;
	virtual bool InitStrRes() = 0;
	virtual void Destroy() = 0;
	virtual void Process() = 0;
	
	virtual CAni* GetDataAni(const char* pszFileName, const char* pszTitle, DWORD dwExigence = C3_EXIGENCE_NORMAL, DWORD dwLife = 30*1000) = 0;
	virtual C3DObj* Get3DObj(OBJID id, DWORD dwExigence = C3_EXIGENCE_NORMAL, DWORD dwLife = 0) = 0;
	virtual C3DTexture* Get3DTexture(OBJID id, DWORD dwExigence = C3_EXIGENCE_NORMAL, DWORD dwLife = 0) = 0;
	virtual C3DEffect* Get3DEffect(OBJID id, DWORD dwExigence = C3_EXIGENCE_NORMAL, DWORD dwLife = 0) = 0;

	// pszPartName: ȡ�ĸ������Ķ���
	virtual C3DMotion* Get3DMotion(const char* pszPartName, int64_t id, DWORD dwExigence = C3_EXIGENCE_NORMAL, DWORD dwLife = 0) = 0;
	
	virtual const C3DObjInfo* Get3DObjInfo(OBJID id) = 0;
	virtual OBJID Get3DTextureID(C3DTexture* pTexture) = 0;
	
	// ��Ҫʹ�øýӿ�!!
	virtual C3DEffect*  CreateNew3DEffect(OBJID id, bool bForceReload, bool bConfirmReload) = 0;
	
	virtual const char*	GetSound(OBJID idSound) = 0;
	virtual const char*	GetVersionInfo() = 0;
	virtual const char* Get3DMotionRes(const char* pszPartName, int64_t idMotion) const = 0;

	virtual const char* GetStr(OBJID idRes) const = 0;

	virtual const C3DSimpleObjInfo*	Get3DSimpleObjInfo(OBJID idType) const = 0;
	virtual const C3DSimpleObjInfoEx* Get3DSimpleObjInfoEx(OBJID idType) const = 0;

	virtual const CMy3DEffectInfo* GetMy3DEffectInfo(const char* pszIndex) const = 0;
	virtual CMy3DEffectInfo* GetMy3DEffectInfo(const char* pszIndex) = 0;
	virtual void AddMy3DEffectInfo(const CMy3DEffectInfo* info) = 0;

	virtual const RolePartInfo*	GetRoleRartInfo(const char* pszPartName, OBJID id) const = 0;
	
	// ͨ��idȡ�ýӿ�ָ��
	virtual void* QueryInterface(int iid) = 0;
	virtual int	GetEmotionIconInfoAmount() const = 0;
	virtual const char*	GetEmotionIconInfo(int nIndex) const = 0;
	virtual void BuildBin() = 0;
	virtual	DWORD GetActionTransitionTime(DWORD dwActionFrom, DWORD dwActionTo) const = 0;
	virtual	void ImmediateExigence(DWORD dwTime) = 0;

	// �����ǰ���ص�3D��Դ,�����¼��������ļ�.
	virtual bool Reload(bool bBinary, DWORD dwReloadFlag = RELOAD_ALL) = 0;

	virtual C3DScene* Get3DScene(OBJID id, DWORD dwExigence = C3_EXIGENCE_NORMAL, DWORD dwLife = 0) = 0;
	
	// ��̬����StrRes�ַ�����
	virtual void SetStr(OBJID idRes, const char* pszStr) = 0;
	
	// ȡMaterial.ini�����õ�������������.
	virtual const D3DMATERIAL*	GetMaterial(const char* pszMaterial) = 0;
	
	virtual void OnResetDevice() = 0;

	virtual bool InitDBStrRes() = 0;
	
	virtual bool IsInitSuccessfully() = 0;
	
	// ȡ���в�������,��������ָ�뵽arrRolePartNames����
	// nMaxCountΪ�����arrRolePartNames������ɵ�Ԫ�ظ���
	// ����ʵ��ȡ�õĸ���.
	virtual int GetRolePartConfig(const char** ppRolePartNames, int nMaxCount) = 0;
	
	struct C3_ANI_RESOURCE_DESC
	{
		const char* pszAniFileName;
		const char* pszAniIndex;
	};
	
	struct C3_ROLEPART_RESOURCE_DESC
	{
		const char* pszRolePartName;
		OBJID idPart;
	};
	
	struct C3_ROLE_MOTION_RESOURCE_DESC
	{
		const char* pszRolePartName;
		int64_t idMotion;
	};
	
	// �ϲ����Դ������ӿ�
	struct IResourceDemand
	{	
		// �ײ��������û�ȡ������Դ
		virtual int GetEffectTitleCount() = 0;
		virtual const char* GetEffectTitle(int nIndex) = 0;
		
		// Ani��Դ
		virtual int GetAniIndexCount() = 0;
		virtual C3_ANI_RESOURCE_DESC GetAniIndex(int nIndex) = 0;
		
		// 3DSimpleObj��Դ
		virtual int GetSimpleObjCount() = 0;
		virtual OBJID GetSimpleObjID(int nIndex) = 0;
		
		// ��ɫģ����ʾ��Դ
		virtual int GetRolePartCount() = 0;
		virtual C3_ROLEPART_RESOURCE_DESC GetRolePartDesc(int nIndex) = 0;
		
		// ��ɫ������Դ��RolePartNameǿ��ΪRP_BODY
		virtual int GetRoleMotionCount() = 0;
		virtual C3_ROLE_MOTION_RESOURCE_DESC GetRoleMotionDesc(int nIndex) = 0;
		
		// ֱ���ļ���Դ
		virtual int GetFileResourceCount() = 0;
		virtual const char* GetFileResourceName(int nIndex) = 0;
		
		// �ڵײ��������Դ��,�ܽ�㱨��Щ��Դ��Ҫʹ�õ��ļ���Դ.
		// Note: ������ַ���ָ����������ʱ��,���ܱ���ָ��ʹ��!
		virtual void ReportReferenceFileResource(int nFileResouceCount, const char** pArrFileNames, DWORD dwUserData) = 0;
		
		// ֪ͨ��ʼ����ͬ����Դ
		virtual void ResourceSynchronizeBegin(DWORD dwUserData) = 0;
		
		// ֪ͨ����Դ�������Դ�Ѿ�׼����
		// Note: bSuccess����Ϊfalse,����P2Pʧ��.
		virtual void ResourceSynchronizeFinish(bool bSuccess, DWORD dwUserData) = 0;
		
		// ֪ͨ����Դ�������Դ�Ѿ�׼����
		// Note: bSuccess����Ϊfalse,����P2Pʧ��.
		virtual void SingleFileSynchronizeFinish(const char* pszFileName, DWORD dwUserData) = 0;
	};
	
	// Note: �ײ�ͨ��IResourceDemand�ӿڻ����Ҫ����Դ�б�
	typedef enum 
	{	
		RESOURCE_NO_REFERENCE				= 0,
		RESOURCE_FILES_COMPLETE				= 1,
		RESOURCE_MISSING_TO_DOWNLOAD		= 2,
		RESOURCE_NOT_SUPPORT_MULTI_MISSION	= 3,
		RESOURCE_P2P_BROKEN					= 4,
	}	RESOURCE_SYN_RESULT;

	// Note: ��û�лص�ResourceSynchronizeFinishǰ,�����߱��뱣֤resourceDemand����Ч��.
	virtual RESOURCE_SYN_RESULT SynchronizeResource(IResourceDemand& resourceDemand, DWORD dwUserData) = 0;

	virtual	void SetResourceLifeCheckPeriod(DWORD dwTime) = 0;
	
	// ǿ��ж��ĳ����Ч��Դ
	virtual bool ReloadGameEffectResource(const char* pszEffectTitle) = 0;
	
	// ǿ��ж��ĳ��Ani��Դ
	virtual bool ReloadAniResource(C3_ANI_RESOURCE_DESC& desc) = 0;

	// ǿ��ж��ĳ��3DSimpleObj��Դ
	virtual bool Reload3DSimpleObjResource(OBJID nSimpleObjID) = 0;
	
	// ǿ��ж��ĳ��RolePart��Դ
	virtual bool ReloadRolePartResource(C3_ROLEPART_RESOURCE_DESC& desc) = 0;

	// ǿ��ж��ĳ��������Դ
	virtual bool ReloadRoleMotionResource(C3_ROLE_MOTION_RESOURCE_DESC& desc) = 0;

	// ǿ��ж��ĳ��3DScene��Դ
	virtual bool Reload3DSceneResource(OBJID n3DSceneID) = 0;
	
	virtual bool PrepareBeforeInit(const char* pszAppFolder, DWORD dwInitFlags) = 0;

	// ʹCGameDataSet::GetDataAni���Ҳ���Ani��Title����ʱ,���ظ�Ani�ļ�����Ϊ[__Default]��CAni.
	virtual void EnableReturnDefaultAni(bool bEnable) = 0;
};

//////////////////////////////////////////////////////////////////////
extern "C" GRAPHICDATA_API CGameDataSet& GameDataSetQuery();
extern "C" GRAPHICDATA_API void GameDataSetDestroy();
#define g_objGameDataSet GameDataSetQuery()

#endif//_GAMEDATASET_H
