#ifndef __PHYX_CRAFT_H__
#define __PHYX_CRAFT_H__

typedef void* HPHYXMESH;
typedef void* HPHYXACTOR;
typedef void* HPHYXCONTROLLER;
typedef void* HPHYTERRAIN;
typedef void* HSOFTBODYACTOR;
typedef void* HPHYXREGION;
typedef void* HPHYXCONSTRAINT;

/**********************************************************************************
** ����������������˶�״̬
**********************************************************************************/
enum PHYX_ROLE_MOVE_STATE
{	
	DM_ROLE_IDLE		=	0,  // ��ֹ
	DM_ROLE_HORMOVE		=	1,  // ˮƽ�ƶ�
	DM_ROLE_JUMPUP		=	2,  // �������ϵ��ٶ�
	DM_ROLE_SLIP_DOWN	=	3,  // ��������»�
	DM_ROLE_REBOUND		=	4,  // ��������
};

/**********************************************************************************
** ��Ϸ���������������,�����Ǳ������ڲ�ʹ�õĳ������.
** �����Զ�����չ,��չ��6��ʼ,���31��!Ĭ����:
** 1.ROLE_CONTROLER��DYNAMIC_OBJECTS��STATIC_TERRAIN&STATIC_OBJECT����֮�䷢����ײ,
** 2.STATIC_TERRAIN/STATIC_OBJECT֮�䲻������ײ.
** 3.����PHYX_GROUP_CUSTOM_BEGIN������Ĭ�϶���������ײ,���ڵĶ����Ǿ�̬��.
** 4.�رմ򿪷�������ײʹ��EnableGroupCollision.
** 5.�������ڶ����Ƿ�̬ʹ��EnableGroupDynamic
**********************************************************************************/
enum 
{ 
	PHYX_GROUP_NONE					= 0,	
	PHYX_GROUP_STATIC_TERRAIN		= 1,
	PHYX_GROUP_STATIC_OBJECT		= 2, 
	PHYX_GROUP_ROLE_CONTROLER		= 3,
	PHYX_GROUP_DYNAMIC_OBJECTS		= 4,
	PHYX_GROUP_CAMERA				= 5,
	PHYX_GROUP_CUSTOM_BEGIN			= 6,
	PHYX_GROUP_MAX					= 32
};

/***********************************************************************
**  ����ӿ���������Ϸ���������������������.�κ�����Ӧ��ʵ���������ӿ�,
**  ����������Ϸȥ��Ӧ�ⲿ������������Ĺ�������.
***********************************************************************/
struct PHYX_DEBUG_DATA
{
	bool bCollide;
	D3DXVECTOR3 vNormal;
	void* pCollideObjectUserData;
	D3DXVECTOR3 ptCollide1;
	D3DXVECTOR3 ptCollide2;
};

struct RAY_PICKUP_INFO
{	
	D3DXVECTOR3 vHitPos;
	D3DXVECTOR3 vHitNormal;
	float fRayHit;
	void* pClientObject;
	HPHYXACTOR  hHitActor;
};

struct PHYX_COLLIDE_INFO
{	
	HPHYXACTOR  hHitActor;
	void* pClientObject;
	D3DXVECTOR3 vHitPos;
	D3DXVECTOR3 vHitNormal;
};

struct PHYX_ROLE_INFO
{	
	PHYX_ROLE_MOVE_STATE state;
	D3DXVECTOR3 vVertSpeed;
};

struct PHYX_ENGINE_PARAM_DESC
{
	enum
	{
		SET_GRAVITY					= 0x1,
		SET_CONVEX_SHAPE_MARGIN		= 0x2,
		SET_CONCAVE_SHAPE_MARGIN	= 0x4,
	};
	int nFlag;
	float fConvexShapeMargin;		// ͹��ı�Ե��С
	float fConcaveShapeMargin;		// ����ı�Ե��С
	D3DXVECTOR3 vWorldGravity;		// ��������ֵ
};

struct HAVOK_PRECOMPILE_DATA
{
	D3DXVECTOR4 info;
	DWORD size;
	BYTE* pData;
};

typedef enum
{
	PHYX_INTERNAL_ACOTR			=	0,
	PHYX_STATIC_OBJECT_ACTOR	=	1,
	PHYX_DYNAMIC_OBJECT_ACTOR	=	1,
	PHYX_KINEMATIC_OBJECT_ACTOR	=	3,
	PHYX_TERRAIN_ACTOR			=	4,
	PHYX_ROLE_ACTOR				=	5,
	PHYX_REGION_ACOTR			=	6,
}	PHYX_ACTOR_TYPE;

struct IDrawForDebug
{
	virtual void DrawLine(const D3DXVECTOR3& vFrom, const D3DXVECTOR3& vTo, const D3DXVECTOR3& color) = 0;
};

struct IPhyxWorld
{	
	/************************************************************
	**  ����������������,��ǰ���д�������Դ����������.
	************************************************************/
	virtual void Release() = 0;

	/************************************************************
	**  ��������������ڲ�����
	************************************************************/
	virtual void AdjustPhysicsEngineParamters(const PHYX_ENGINE_PARAM_DESC& paramDesc) = 0;
	
	/*****************************************************************
	**  ����Mesh���������ײģ������,ע��HPHYXMESH������,��������ʵ��.
	**  ���������ڴ�ָ���Ϊ��ʱ�ڴ�,����ֱ�ӱ���ʹ��.
	*****************************************************************/
	virtual HPHYXMESH BuildPhyxMesh(DWORD nPolygonSoupVertices, const D3DXVECTOR3* pVertices, DWORD nTrianglePrims, const WORD* pTrianglePrims, void* pPreCompileData = NULL) = 0;

	/*****************************************************************
	**  ���ݴ���Ķ��㼯�ϴ���͹����ײģ.
	**  ���������ڴ�ָ���Ϊ��ʱ�ڴ�,����ֱ�ӱ���ʹ��.
	*****************************************************************/
	virtual HPHYXMESH BuildConvexHullMesh(DWORD nPolygonSoupVertices, const D3DXVECTOR3* pVertices, void* pPreCompileData = NULL) = 0;

	/*****************************************************************
	**  ����Box Mesh���������ײģ������.
	**  vExtentΪBox�ĳ����,Box����Ĭ�Ͻ�����ԭ����
	*****************************************************************/
	virtual HPHYXMESH BuildBoxMesh(D3DXVECTOR3 vExtent) = 0;
	
	/*****************************************************************
	**  ����Sphere Mesh���������ײģ������.
	**  fRadiusΪ����İ뾶,��������Ĭ�Ͻ�����ԭ����
	*****************************************************************/
	virtual HPHYXMESH BuildSphereMesh(float fRadius) = 0;
	
	/*****************************************************************
	**  ����Plane Mesh���������ײģ������.
	**  vNormal��ƽ�淨��,fDist��ƽ�浽ԭ��ľ���.
	*****************************************************************/
	virtual HPHYXMESH BuildPlaneMesh(D3DXVECTOR3 vNormal, float fDist) = 0;
	
	/*****************************************************************
	**  ����Capsule Mesh���������ײģ������.
	**  fHeight���������ҵĸ�,fRadius�ǽ���Բ���岿�ֵĺ�ذ뾶.
	**  nUpAxis: ���ҵĸ߶�����0=X,1=Y,2=Z
	*****************************************************************/
	virtual HPHYXMESH BuildCapsuleMesh(float fHeight, float fRadius, int nUpAxis) = 0;
	
	/*******************************************************************************
	**  ����������ײģ������
	**  Note: ֻ��������ʹ�����HPHYXMESH��Actor�����ٺ���ܶ������DestroyPhyxMesh.
	*******************************************************************************/
	virtual void DestroyPhyxMesh(HPHYXMESH hPhyxMesh) = 0;
	
	/*****************************************************************
	**  ��HPHYXMESHΪ����ģ������,����һ������ʵ��,����ӵ�����������ȥ.
	**  vPos/qRot/fScale: ����ʵ�������λ��
	**  nGameGroup: ��ӵ��ĸ�����
	**  userData: �ͻ��˰���ֵ.
	*****************************************************************/
	virtual HPHYXACTOR AddNewPhyxActor(HPHYXMESH hPhyxMesh, D3DXVECTOR3 vPos, D3DXQUATERNION qRot, float fScale, DWORD nGameGroup, void* userData) = 0;

	/************************************************************
	**  ȡ��hPhyxActor�ĵ�ǰ����λ��
	************************************************************/
	virtual bool GetPhyxActorPose(HPHYXACTOR hPhyxActor, D3DXMATRIX& matResult) = 0;
	
	/**********************************
	**  ��̬�޸�Actor�����ű���
	**  Note: fScale������ڵ�ǰ�ı���.
	**********************************/
	virtual bool ChangeActorScale(HPHYXACTOR hPhyxActor, float fScale) = 0;
	
	/**********************************
	**  ǿ���ƶ�Actor��ĳ��λ��
	**********************************/
	virtual bool MoveActorTo(HPHYXACTOR hPhyxActor, const D3DXVECTOR3& vPos, const D3DXQUATERNION& qRot) = 0;

	/**********************************
	**  ��ȡActor��ǰ�ٶ�
	**********************************/
	virtual D3DXVECTOR3 GetActorCurrentSpeed(HPHYXACTOR hPhyxActor) = 0;

	/********************************************
	**  ��ȡActor������(Refer to PHYX_ACTOR_TYPE)
	********************************************/
	virtual PHYX_ACTOR_TYPE GetActorType(HPHYXACTOR hPhyxActor) = 0;
	
	/*****************************************************************************
	**  ��ȡActor���ϰ󶨵Ŀͻ�������(����AddNewPhyxActorʱͨ��userData�������ֵ.
	*****************************************************************************/
	virtual void* GetActorUserData(HPHYXACTOR hPhyxActor) = 0;
	
	/*****************************************************************
	**  ɾ����������ʵ��
	*****************************************************************/
	virtual void RemovePhyxActor(HPHYXACTOR hPhyxActor) = 0;
	
	/*******************************************************************
	**  ������������,��ӵ�ROLE_CONTROLLERN��.
	**  vAABBMin/vAABBMax: ����ģ�͵İ�Χ��
	**  vInitPos/vOrientation: ��ʼλ��
	**  userData: �ͻ��˰���ֵ.
	*******************************************************************/
	virtual HPHYXCONTROLLER AddRoleController(const D3DXVECTOR3& vAABBMin, const D3DXVECTOR3& vAABBMax, const D3DXVECTOR3& vInitPos, const D3DXVECTOR3& vOrientation, void* userData) = 0;

	/*******************************************************************
	**  ɾ�����������
	*******************************************************************/
	virtual void RemoveRoleController(HPHYXCONTROLLER hPhyxController) = 0;
	
	/*********************************************************************************
	** ���õ��α�׼,֧�ֶ����������,ÿ������ɷֳɶ��Block,ÿ��Block�ɷֳɶ��Grid.
	** nAreaSideBlockCount: ÿ��������nAreaSideBlockCount * nAreaSideBlockCount��Block���.
	** nBlockSideGridCount: ÿ��Block��nBlockSideGridCount * nBlockSideGridCount��Grid���
	** fGridSize: ��С��λ��ı߳�(ֻ֧��������Ϊ��С��λ��)
	** Note: ����ͻ���ֻʹ�õ�һ�����,������nAreaSideBlockCount=1.
	**********************************************************************************/
	virtual void SetHeighFieldTerrainSpec(int nAreaSideBlockCount, int nBlockSideGridCount, float fGridSize) = 0;
	
	/*********************************************************************************
	**  ���һ������Block,��ӵ�STATIC_TERRAIN��.
	**  Note:һ��Block����ΪnBlockSideGridCount * nBlockSideGridCount��Grid.
	**  vBase: �õ��ο����ʼ�������.
	**  fHeights: �õ��ο�ĸ���߶�ͼ����,������(nBlockSideGridCount + 1)*((nBlockSideGridCount + 1)��������.
	**  Note: ���յĵ�����vBaseΪԭ��,��XY�����ϰ�fGridSizeÿ�����,��Z�߶��ϰ�: ����߶�ͼ��ֵ + vBase.z
	*********************************************************************************/
	virtual HPHYTERRAIN AddHeighFieldTerrainArea(D3DXVECTOR3 vBase, float* fHeights, void* pClientPointer) = 0;
	
	/************************************************************
	**  ɾ����ײ������
	************************************************************/
	virtual void RemoveHeighFieldTerrainArea(HPHYTERRAIN hPhyxTerrain) = 0;

	/***********************************************************************************************
	**  ��������(vBegin -> vEnd)���еĵ�һ����������ʵ��,
	**  ����ֵΪ�����е�HPHYXACTOR��AddNewPhyxActor����Ŀͻ���ָ�����.
	**  nGroupFlagΪ��Ҫ�������λ(����1 << nGroupID),Ϊ0��ʾ�������!
	************************************************************************************************/
	virtual bool RayPickup(D3DXVECTOR3& vBegin, D3DXVECTOR3& vEnd, DWORD nGroupFlags, RAY_PICKUP_INFO* pResultInfo) = 0;
	virtual bool SegmentPickup(D3DXVECTOR3& vBegin, D3DXVECTOR3& vEnd, DWORD nGroupFlags, RAY_PICKUP_INFO* pResultInfo) = 0;
	
	/***********************************************************************************************
	**  ���Խ�HPHYXACTOR���õ�vDestPos��,����������ײ�µ�ʵ�ʿɷ��õ�λ��.
	**  vOrientation: ��ɫ��ǰ����,�Ǳ�׼����.
	************************************************************************************************/
	virtual D3DXVECTOR3 MoveRoleController(HPHYXCONTROLLER hPhyxController, const D3DXVECTOR3& vUserSpeed, float fMoveTime, PHYX_ROLE_INFO& resultInfo) = 0;

	/************************************************************
	**  ��̬�ı���������ģ�͵Ĵ�С.
	************************************************************/
	virtual bool SetRoleControllerSize(HPHYXCONTROLLER hPhyxController, const D3DXVECTOR3& vAABBMin, const D3DXVECTOR3& vAABBMax) = 0;
	
	// �������������Ӱ��
	virtual bool EnableRoleControllerGravity(HPHYXCONTROLLER hPhyxController, bool bEnable) = 0;
	
	// �������ﵥ����߿����ϵ�̨�׵ĸ߶�.
	virtual bool SetRoleControllerMaxStepHeight(HPHYXCONTROLLER hPhyxController, float fMaxStepHeight) = 0;
	
	// �����������վ�������б��
	virtual bool SetRoleControllerMaxSlopeAngle(HPHYXCONTROLLER hPhyxController, float fSlopeDegree) = 0;
	
	/************************************************************
	**  ģ��������������fTimeElippse��.
	**  Note: �ͻ��˱���ÿ֡���������÷���
	************************************************************/
	virtual void Simulate(float fTimeEllipse) = 0;

	/************************************************************
	**  ���ø÷����ڵ��������Ϊ����
	************************************************************/
	struct PHYX_GROUP_BEHAVIOR_DESC
	{
		bool bDynamic;			// ��̬����
		bool bKinematic;		// ��̬���߼��ƶ�
	};
	virtual void SetGroupBehavior(DWORD nFromCollisionGroupID, const PHYX_GROUP_BEHAVIOR_DESC& desc) = 0;

	/************************************************************
	**  ���������֮���Ƿ������ײ
	**  Note: ��ײ��˫���,A����B������ײ,������B����A������ײ.
	************************************************************/
	virtual void EnableGroupCollision(DWORD nFromCollisionGroupID, DWORD nToCollisionGroupID, bool bEnableCollision) = 0;
	
	/************************************************************
	**  ȡ�õ�����Ϣ,��������м��.
	************************************************************/
	virtual PHYX_DEBUG_DATA& GetDebugData() = 0;

	/************************************************************
	**  ������Ϸ�ľ�ͷ����.
	************************************************************/
	virtual float SetupCollisionCamera(float fNearPlane, float fFovDegree, float fAspect) = 0;
	
	/************************************************************
	**  ��ͷ������ײ,֮ǰ�����ȵ���CastCamera
	************************************************************/
	virtual bool CastCamera(const D3DXVECTOR3& vEye, const D3DXVECTOR3& vFocus, D3DXVECTOR3& vCollidePosition) = 0;

	/********************************************************************
	**  ����̬���hPhyxActor������������
	********************************************************************/
	struct DYNAMIC_ACTOR_PHYSICS_DESC
	{
		enum
		{
			SET_MASS	= 0x1,
			SET_GRAVITY = 0x2,
			SET_MASS_CENTER = 0x4,
		};
		int nFlag;
		float fMass;
		float fGravityFactor;
		float vMassCenter[3];
	};
	virtual bool SetDynamicActorPhysics(HPHYXACTOR hPhyxActor, const DYNAMIC_ACTOR_PHYSICS_DESC& desc) = 0;

	/********************************************************************
	**  ����̬���hPhyxActorʩ�����緽���ϵ�˲������vImpulse������.
	********************************************************************/
	virtual bool ApplyImpulseToActor(HPHYXACTOR hPhyxActor, const D3DXVECTOR3& vImpulse) = 0;

	/*****************************************************************************
	**  ����̬���hPhyxActorʩ�����緽���ϵ���vForce������,����ʩ��fContinueTime.
	*****************************************************************************/
	virtual bool ApplyForceToActor(HPHYXACTOR hPhyxActor, const D3DXVECTOR3& vForce, float fContinueTime) = 0;
	
	/********************************************************************
	**  ��ײ�¼��ص�
	********************************************************************/
	struct IActorCollisionEvent
	{
		// ��⵽��������ײ��hRegisteredActor
		// Note: ʹ��GetActorType���infoCollide.hHitActor�ľ�������
		virtual void ActorCollided(HPHYXACTOR hRegisteredActor, PHYX_COLLIDE_INFO infoCollide) = 0;
	};

	struct IRegionEvent
	{	
		// ��⵽�������������hRegion
		// Note: ʹ��GetActorType���Actor�ľ�������
		virtual void ActorEnterRegion(HPHYXREGION hRegion, HPHYXACTOR hPhyxActor) = 0;

		// ��⵽�������뿪����hRegion
		virtual void ActorLeaveRegion(HPHYXREGION hRegion, HPHYXACTOR hPhyxActor) = 0;
	};

	/********************************************************************
	**  ע��ĳ���������ײ�¼��ص�
	********************************************************************/
	virtual bool RegisterActorCollisionEvent(HPHYXACTOR hPhyxActor, IActorCollisionEvent& event) = 0;
	virtual bool UnregisterActorCollisionEvent(HPHYXACTOR hPhyxActor) = 0;

	/********************************************************************
	**  ע��ĳ���������ײ�ص�
	**  nGameGroup: ������ײ����
	********************************************************************/
	virtual HPHYXREGION AddAABBGhostRegion(const D3DXVECTOR3& vMin, const D3DXVECTOR3& vMax, DWORD nGameGroup, IRegionEvent& callback) = 0;
	virtual HPHYXREGION AddShapeGhostRegion(HPHYXMESH hPhyxMesh, const D3DXVECTOR3& vPos, const D3DXQUATERNION& qRot, DWORD nGameGroup, IRegionEvent& callback) = 0;
	virtual bool MoveGhostRegionTo(HPHYXREGION hGhostRegion, const D3DXVECTOR3& vPos, const D3DXQUATERNION& qRot) = 0;
	virtual bool RemoveGhostRegion(HPHYXREGION hRegion) = 0;

	/************************************************************
	**  �ڲ�����ʹ��
	************************************************************/
	virtual void DrawDebugPhysicsWorld(IDrawForDebug* pDrawForDebug) = 0; 

	/********************************************************************
	**  ������Լ��
	********************************************************************
	**  ������ת�İ󶨵�Լ��
	********************************************************************/
	struct PHYX_SOCKETBALL_CONSTRAINT_DESC
	{	
		PHYX_SOCKETBALL_CONSTRAINT_DESC() : bDisableCollisionBetweenLinkedBodys(false), fSpringDistance(0.0f), bSetInWorldSpace(false) {}
		bool bDisableCollisionBetweenLinkedBodys;
		bool bSetInWorldSpace;
		float fSpringDistance;	// !=0��ʾ����������
	};
	virtual HPHYXCONSTRAINT AddSocketBallConstraint(HPHYXACTOR hActorA, HPHYXACTOR hActorB, const D3DXVECTOR3& vLocalPivotInA, const D3DXVECTOR3& vLocalPivotInB, const PHYX_SOCKETBALL_CONSTRAINT_DESC& desc) = 0;

	/********************************************************************
	**  ����ת��Լ��
	********************************************************************/
	struct PHYX_HINGE_CONSTRAINT_DESC
	{	
		PHYX_HINGE_CONSTRAINT_DESC() : fLowerAngleDegree(180.0f), fUpperAngleDegree(-180.0f), bDisableCollisionBetweenLinkedBodys(false),
			bMotorEnable(false), bSetInWorldSpace(false), fAutoRotateSpeedDegree(0.0f), fAutoRotateTargetDegree(0.0f) {}
		float fLowerAngleDegree;	// Note: ȡֵ[-180,180],�������ʼλ��Ϊ��׼,˳ʱ�뷽��Ϊ��,��ʱ�뷽��Ϊ��.
		float fUpperAngleDegree;	// [-180,180]
		float fAutoRotateTargetDegree;
		float fAutoRotateSpeedDegree;
		bool  bMotorEnable;
		bool  bSetInWorldSpace;		// Ϊtrue��ʾ���������������������ϵ��.
		bool  bDisableCollisionBetweenLinkedBodys;
	};
	virtual HPHYXCONSTRAINT AddHingeConstraint(HPHYXACTOR hActorA, HPHYXACTOR hActorB, const D3DXVECTOR3& vLocalPivotInA, const D3DXVECTOR3& vLocalPivotInB, const D3DXVECTOR3& vLocalAxisInA, const D3DXVECTOR3& vLocalAxisInB, const PHYX_HINGE_CONSTRAINT_DESC& desc) = 0;

	/********************************************************************************
	**  �������ı�������ϵ��X��Ϊ������
	********************************************************************************/
	struct PHYX_BONE_CONSTRAINT_DESC
	{	
		PHYX_BONE_CONSTRAINT_DESC() : fMaxConeDegree(0.0f), fMinTwistDegree(-90.0f), fMaxTwistDegree(90.0f), bSetInWorldSpace(false), bDisableCollisionBetweenLinkedBodys(false){}
		float fMinConeDegree;
		float fMaxConeDegree;		// [ 0,180]
		float fMinTwistDegree;		// [-90, 0]
		float fMaxTwistDegree;		// [ 90, 0]
		bool bSetInWorldSpace;
		bool bDisableCollisionBetweenLinkedBodys;
	};
	virtual HPHYXCONSTRAINT AddBoneConstraint(HPHYXACTOR hActorA, HPHYXACTOR hActorB, const D3DXVECTOR3& vLocalPivotInA, const D3DXVECTOR3& vLocalPivotInB, const D3DXQUATERNION& qLocalRotateInA, const D3DXQUATERNION& qLocalRotateInB, const PHYX_BONE_CONSTRAINT_DESC& desc) = 0;

	/********************************************************************
	**  ƽ��Լ��
	********************************************************************/
	struct PHYX_TRANSLATE_CONSTRAINT_DESC
	{	
		PHYX_TRANSLATE_CONSTRAINT_DESC() : fMinTranslate(-FLT_MAX), fMaxTranslate(FLT_MAX), bDisableCollisionBetweenLinkedBodys(false), bSetInWorldSpace(false) {}
		float fMinTranslate;
		float fMaxTranslate;
		bool bDisableCollisionBetweenLinkedBodys;
		bool bSetInWorldSpace;
	};
	virtual HPHYXCONSTRAINT AddPrismaticConstraint(HPHYXACTOR hPhyxActorA, HPHYXACTOR hPhyxActorB, const D3DXVECTOR3& vLocalPivotInA, const D3DXVECTOR3& vLocalPivotInB, const D3DXVECTOR3& vLocalAxisInA, const D3DXVECTOR3& vLocalAxisInB, const PHYX_TRANSLATE_CONSTRAINT_DESC& desc) = 0;
	
	/********************************************************************
	**  Լ����
	********************************************************************/
	struct PHYX_CHAIN_CONSTRAINT_DESC
	{	
		PHYX_CHAIN_CONSTRAINT_DESC() : nType(1), fSpringLength(0.0f) {}
		int nType;
		float fSpringLength;
	};
	virtual HPHYXCONSTRAINT AddConstraintChain(int nActorCount, HPHYXACTOR* arrActors, const D3DXVECTOR3& vFirstLocalPivotInA, const D3DXVECTOR3& vLocalPivotInA, const D3DXVECTOR3& vLocalPivotInB, const PHYX_CHAIN_CONSTRAINT_DESC& desc) = 0;

	virtual void RemoveConstraint(HPHYXCONSTRAINT hConstraint) = 0;
	
	// ����֧��
	enum 
	{
		CLOTH_CORNER_LEFTTOP		= 0x1,
		CLOTH_CORNER_RIGHTTOP		= 0x2,
		CLOTH_CORNER_LEFTBOTTOM		= 0x4,
		CLOTH_CORNER_RIGHTBOTTOM	= 0x8,
	};

	enum 
	{
		SOFTBODY_COLLIDE_QUALITY_NONE	= 0,
		SOFTBODY_COLLIDE_QUALITY_NORMAL	= 1,
		SOFTBODY_COLLIDE_QUALITY_GOOD	= -1,
	};

	struct CLOTH_PARAM_DESC
	{
		CLOTH_PARAM_DESC() : nGridCX(8), nGridCY(8), nCollisionQuality(0),
			fClothGridLength(0.25f), nInitFixCornerFlag(CLOTH_CORNER_LEFTTOP | CLOTH_CORNER_RIGHTTOP),
			fTotalMass(1.0f), fLinearStiffness(0.5f), fAngularStiffness(0.5f), fMargin(0.25f),
			nBlendingConstraints(2)
		{
		}
			
		int nGridCX;				// ���������
		int nGridCY;				// ���������
		int nCollisionQuality;		// 0:no collision	1: simple collision 2: accurate collision
		float fClothGridLength;		// ÿ�񳤶�
		int nInitFixCornerFlag;		// ��ʼ
		float fTotalMass;			// ����
		float fLinearStiffness;		// ���������ٶȽ�ֱ��
		float fAngularStiffness;	// ���Ͻ��ٶȽ�ֱ��
		float fMargin;				// ���Ϻ������������ײҪ���ֵ���С����.
		int   nBlendingConstraints;	// �������ɳ̶�[0-10]
	};

	struct SOFTBODY_PARAM_DESC
	{	
		SOFTBODY_PARAM_DESC() : nRes(32), nCollisionQuality(0), fTotalMass(1.0f), fLinearStiffness(0.5f), fAngularStiffness(0.5f), fMargin(0.2f)
		{
		}
		
		int nRes;
		int nCollisionQuality;// 0:no collision	1: simple collision 2: accurate collision
		float fTotalMass;
		float fLinearStiffness;
		float fAngularStiffness;
		float fMargin;
	};
	
	enum 
	{
		ROPE_CORNER_TOP		= 0x1,
		ROPE_CORNER_BOTTOM	= 0x2,
	};

	struct ROPE_PARAM_DESC
	{	
		ROPE_PARAM_DESC() : nRes(32), nCollisionQuality(1), fTotalMass(1.0f), fLinearStiffness(0.5f), nInitFixCornerFlag(1)
		{
		}
		
		int nRes;
		float fTotalMass;
		int nInitFixCornerFlag;
		int nCollisionQuality;// 0:no collision	1: simple collision 2: accurate collision
		float fLinearStiffness;
	};
	
	// ����֧��
	// Note: Havok��֧������,��Щ������Havokʵ���·���NULL��false.
	virtual HSOFTBODYACTOR AddCloth(const CLOTH_PARAM_DESC& clothDesc, const D3DXVECTOR3& vPos, const D3DXQUATERNION& qRot, DWORD nGameGroup, void* userdata) = 0;
	virtual HSOFTBODYACTOR AddEllipsoidSoftBody(const SOFTBODY_PARAM_DESC& sbDesc, const D3DXVECTOR3& vCenter, const D3DXVECTOR3& vRadius, DWORD nGameGroup, void* userdata) = 0;
	virtual HSOFTBODYACTOR AddRope(const ROPE_PARAM_DESC& rpDesc, const D3DXVECTOR3& vFrom, const D3DXVECTOR3& vTo, DWORD nGameGroup, void* userdata) = 0;
	virtual HSOFTBODYACTOR AddMeshSoftBody(HPHYXMESH hPhyxMesh, const D3DXVECTOR3& vPos, const D3DXQUATERNION& qRot, DWORD nGameGroup, void* userdata) = 0;
	virtual bool RemoveSoftBody(HSOFTBODYACTOR hSoftBodyActor) = 0;
	
	virtual int GetSoftBodyPointCount(HSOFTBODYACTOR hSoftBodyActor) = 0;
	virtual bool GetSoftBodyPoints(HSOFTBODYACTOR hSoftBodyActor, D3DXVECTOR3* pVertices, D3DXVECTOR3* pNormals, int nVertexCount, int nStride) = 0;
	virtual int GetSoftBodyTriangleCount(HSOFTBODYACTOR hSoftBodyActor) = 0;
	virtual bool GetSoftBodyTriangleIndices(HSOFTBODYACTOR hSoftBodyActor, WORD* pIndices, int nTriangleCount) = 0;
	virtual bool BindSoftBodyToActor(HSOFTBODYACTOR hSoftBodyActor, int nVertexIndex, HPHYXACTOR hPhyxActor, bool bDisableCollisionBetweenLinkedBodies) = 0;
	virtual bool AddForceToSoftBody(HSOFTBODYACTOR hSoftBodyActor, int nVertexIndex, const D3DXVECTOR3& vForce) = 0; 
	virtual bool SetFixedSoftBodyPoint(HSOFTBODYACTOR hSoftBodyActor, int nPointIndex, bool bFixed) = 0;
};

extern "C" IPhyxWorld* CreatePhyxWorld();

#endif//__PHYX_CRAFT_H__