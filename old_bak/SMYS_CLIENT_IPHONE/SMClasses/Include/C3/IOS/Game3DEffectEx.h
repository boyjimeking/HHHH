// Game3DEffectEx.h: interface for the CGame3DEffectEx class.
//
//////////////////////////////////////////////////////////////////////
#ifndef _GAME_3DEFFECT_EX_H_
#define _GAME_3DEFFECT_EX_H_

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

// ��������-1��ζ��ʧ��
// Note: ������I3DEffectEvent�ص�������ɾ����Ч!
class I3DEffectEvent
{
public:
	// ���ſ�ʼ/��ʱ���
	virtual void OnPlay(int id) = 0;

	// ѭ����ʼ
	virtual void OnLoop(int id) = 0;

	// ����ѭ�����
	virtual void OnInterval(int id) = 0;

	// ���
	virtual void OnOver(int id) = 0;

	// �����Զ�ɾ��
	virtual void OnDelete(int id) = 0;
};

class CGame3DEffectEx  
{
public:
	virtual unsigned long Release()	= 0;

	virtual	bool Process() = 0;
	virtual void Clear() = 0;
	virtual void Show(C3_POS posWorld, int id = -1) = 0;
	
	// �������Ч�����ɹ�������Чid�����󷵻� -1
	virtual int  Add(const char* pszIndex, bool bAutoDelete = true) = 0;

	// ɾ��ָ������Ч������ж����ͬ��������Ч����ɾ����һ����
	virtual void Delete(const char* pszIndex) = 0;

	// ����ָ��id����Ч��������ɾ����Ч
	virtual void Delete(int id) = 0;

	// ����ָ��id��ͣ/������Ч����
	virtual void Pause(int id = -1) = 0;
	virtual void Continue(int id = -1) = 0;
	virtual void SetFrame(int nFrame, int id = -1) = 0;
	virtual void SetElapseTime(DWORD dwElapsedTime, int id = -1) = 0;

	// �ռ�λ�ñ仯��idxΪ��Ч��������Ż���id����bCtrlID����, -1��������Ч
	// �����x,y,z��3D����ϵ,ע�⵽��2.5D��ͼ��3D��Z������,Y������.
	virtual void Rotate(float xRadian, float yRadian, float zRadian, int idx = -1, bool bCtrlID = false) = 0;
	virtual void Scale(float x, float y, float z, int idx = -1, bool bCtrlID = false) = 0;
	virtual void Move(float x, float y, float z, int idx = -1, bool bCtrlID = false) = 0;
	
	// RotateZ���Լ�����ǰ�Ĵ���,�벻Ҫ��ʹ��.
	// RotateZ�൱�൱�� Rotate(0.0f, 0.0f, D3DXToRadian(nAngleDegree), id, true);
	virtual void RotateZ(int id, int nAngleDegree) = 0;
	
	// ֱ�����þ���
	virtual void SetMatrix(const D3DXMATRIX& matrix, int id = -1) = 0;
	
	// ����������Ч���˶�
	virtual void ResetMovement() = 0;
	
	// ������Z���ϵ�ƫ��(��2.5D�е���������)
	virtual void SetOffset(int nOffset) = 0;
	virtual void SetHeight(float fHeight) = 0;
	
	// �����Ƿ���2.5D��б
	virtual void SetOblique(bool bOblique) = 0;

	// ������ɫARGB, idΪ��Ч��������Ż���id����bCtrlID����, -1��������Ч.
	virtual void SetColor(DWORD dwColor, int idx = -1, bool bCtrlID = false) = 0;

	// ������Ч�Ļ�Ϸ�ʽ, ����idΪ��Чid, numΪ��Ч���Ӽ��±�
	virtual void SetBlend(DWORD dwAsb, DWORD dwAdb, int id = -1, int num = -1) = 0;

	// ȡ��Ч֡��
	virtual int	 GetFrameAmount(int id = -1) = 0;

	// �����ΪpszIndex����Ч�Ƿ����
	virtual bool TestEffect(const char* pszIndex) = 0;

	// ������ΪpszIndex����Ч,����id
	virtual int	 FindEffect(const char* pszIndex) = 0;	

	// ʹ��Ч�������ϲ���
	virtual void EverPlay(int id = -1) = 0;

	// ����ĳ����Ч��ƫ��
	virtual void SetOffset(int nX, int nY, int nZ, int id = -1) = 0;

	// ���ÿ��ƻص��ӿ�
	virtual void SetEventInterface(I3DEffectEvent* pEvent) = 0;
	
	virtual void SetGravity(D3DXVECTOR3 vctGravity, int id = -1) = 0;
	virtual void SetWind(D3DXVECTOR3 vctWind, int id = -1) = 0;
		
	// id��Add���ص���Чid, index��3DEffect.ini����Ķ����Ч������(EffectIdXX).
	// nTextureID���ڲ�����ID,�ⲿ�߼�����ȷ����CGame3DEffectEx��������nTextureID����Ч��.
	// Note: ��nTextureID == -1�����֮ǰSetTexture��Ч��.
	// ��Чֻ֧�ֵ������ͼ
	virtual void SetTexture(int nTextureID, int nId = -1, int nIndex = -1) = 0;
	
	virtual bool Reload() = 0;
		
	// ������Ч֡���
	virtual void SetFrameInterval(DWORD dwFrameInterval, int id = -1) = 0;
	
	// ȡ��Ч�ӳٲ��ŵ�֡��, = Delay / FrameInterval.
	virtual	int GetDelayFrameCount(int id) = 0;	
	
	// ������Ч��ǰ��ʱ֡
	virtual void SetImmediateFrame(int nFrame, int id = -1) = 0;
	
	// ����󶨵�����Ĺ�Ч���Լ��ľֲ��任.
	// Note: ��������ʷ��Ĭ�ϱ��ر�.
	virtual void AllowSelfTransformInRolePart(bool bEnable) = 0;
	
	// ��ӹ�Ч,��ͬ��Add����,AddOnlyShellֻ������Ч����,�����Լ���ʵ����Դ.
	virtual int AddOnlyShell(const char* pszIndex, bool bAutoDelete = true) = 0;
	
	virtual void SetLoopTime(DWORD dwLoopTime, int id) = 0;
	
	// ��Ӳ���ʱ����,��������Ч��DelayΪ0,�������벥��״̬.
	virtual int AddImmediate(const char* pszIndex, const char* szBindPhyName = NULL, bool bAutoDelete = true) = 0;
	
	// ȡTitleΪpszIndex�Ĺ�Ч����(pszIndex��Сд���)
	virtual DWORD GetEffectCountByName(const char* pszTitle) = 0;
	
	// ȡTitleΪpszIndex�ĵ�nIndex����Ч�Ŀ���ID(pszIndex��Сд���,nIndex��0Ϊ��)
	// ��û�е�nIndex��,����-1.
	virtual int FindEffectByName(const char* pszTitle, int nIndex) = 0;
	
	virtual bool GetEffectOffset(int id, int& nX, int& nY, int& nZ) = 0;
	
	// ֱ��ʹ�þ�����ʾ
	// Note: Show3D����������Move/Rotate/Scale/SetMatrix/SetOblique/RotateZ/SetHeight/SetOffset������
	// Show3D��Ȼ����3DEffect.ini�����õ�Scale,Offset.
	virtual void Show3D(const D3DXMATRIX& matWorld, int id = -1) = 0;
	
	virtual void SetResourceFrame(DWORD dwFrame) = 0;
	
	// ��ģ�͹�Ч���ö���,��NULL���ù�Ч�Դ�Ĭ�϶���.
	// Note: ���ܸ�����ϵͳ���ö���!
	// pszTitle: ��Ϲ�Ч��
	// p3DMotion: ������Դ
	// nSubEffectIndex: �ӹ�Ч����
	virtual bool Set3DMotion(const char* pszTitle, C3DMotion* p3DMotion, int nSubEffectIndex = 0) = 0;
	
	// ��ӦShow3D��HitTest.
	virtual bool HitTest3D(int nScreenX, int nScreenY, const D3DXMATRIX& matWorld, int nId) = 0;

	virtual int GetSubEffectCount() = 0;
};

extern "C" ROLE3D_API CGame3DEffectEx* Game3DEffectCreate(I3DEffectEvent* pEvent = NULL);
extern "C" ROLE3D_API void LimitGame3DEffectLevel(int nGameEffectLevelLimit);
#endif//_GAME_3DEFFECT_EX_H_