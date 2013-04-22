//
//  NDNode.h
//  DragonDrive
//
//  Created by xiezhenghai on 10-12-7.
//  Copyright 2010 (����)DeNA. All rights reserved.
//
//	�������ܣ���
//	NDNode�ǿ�ܵĻ����ڵ�
//	�ڵ����ʾ��z���йأ�z��Խ����Խ��ǰ��ʾ
//	�����������ʽ����ʾ��ʵ��Initialization����
//	������Ķ�������֮�������ʽ����ʾ�ĵ���Initialization����

#ifndef	__NDNODE_H
#define __NDNODE_H

#include "NDCommonProtocol.h"
#include "ScriptInc.h"
#include <vector>
#include <list>

#include "ccTypes.h"
#include "CCNode.h"
#include "CCObject.h"

#include "shaders/ccGLStateCache.h"
#include "shaders/CCGLProgram.h"

#define DECLARE_AUTOLINK(class_name) \
private: CAutoLink<class_name> m_autolink##class_name;

#define INTERFACE_AUTOLINK(class_name) \
public: CAutoLink<class_name> QueryLink() { return  m_autolink##class_name; }

#define INIT_AUTOLINK(class_name) \
m_autolink##class_name.Init(this)

using namespace cocos2d;

NS_NDENGINE_BGN

enum NODE_LEVEL
{
	NODE_LEVEL_NONE = 0,
	NODE_LEVEL_TEAM_LEADER,
	NODE_LEVEL_MAIN_ROLE,
};

//����cocos2d�Ĺ���priority��ֵԽС��ʵ�����ȼ�Խ�ߣ�
//���ο�CCTouchDispatcher::setPriority()������ע�ͣ�
//ö�ٵ�˳���Ⱥ����˳��
enum ND_LAYER_PRIORITY
{
	E_LAYER_PRIORITY_POPUPDLG	= 0x11,	//������
	E_LAYER_PRIORITY_WORLDMAP	= 0x12,	//�����ͼ��WorldMapLayer��
	E_LAYER_PRIORITY_UILAYER	= 0x13,	//�ǵ���ʽUI����������ȣ�
	E_LAYER_PRIORITY_MAPLAYER	= 0x14,	//��Ϸ��ͼ
	E_LAYER_PRIORITY_DEFAULT	= 0x20,
};

typedef CAutoLink<NDCommonProtocol> COMMON_VIEWER;
typedef std::list<COMMON_VIEWER> LIST_COMMON_VIEWER;
typedef LIST_COMMON_VIEWER::iterator LIST_COMMON_VIEWER_IT;

class NDAction;
class NDNode;
class NDNodeDelegate
{
public:

	virtual void OnBeforeNodeRemoveFromParent(NDNode* node, bool bCleanUp)
	{
	}
	virtual void OnAfterNodeRemoveFromParent(NDNode* node, bool bCleanUp)
	{
	}
};

class NDNode: public NDCommonProtocol
{
DECLARE_CLASS(NDNode)
public:
	NDNode();
	~NDNode();
public:

	//
	//		������Node
	//		���ã���̬������ͨ���÷�������������ʾ����Initialization����
	//		��������
	//		����ֵ��������ָ��
	static NDNode* Node();
	//		
	//		������Initialization
	//		���ã���ʼ���ڵ㣬���뱸��ʾ����ʽ����
	//		��������
	//		����ֵ����
	virtual void Initialization(); 
	virtual void Destroy();

	//		
	//		������draw
	//		���ã���Ϸ��ѭ���壬��Ϸÿһ֡���ø÷���������ڲ�����
	//		��������
	//		����ֵ����
	virtual void draw();
	virtual void preDraw() {}
	virtual void postDraw() {}

	//		
	//		������GetParent
	//		���ã���ȡ���׽ڵ�
	//		��������
	//		����ֵ�����ظ��׽ڵ㣬û�и��׽ڵ��򷵻�null
	NDNode* GetParent();	
	//		
	//		������GetChildren
	//		���ã���ȡ�ӽڵ��б�
	//		��������
	//		����ֵ���ڵ��б�
	const std::vector<NDNode*>& GetChildren();
	//		
	//		������RemoveAllChildren
	//		���ã�ɾ�������ӽڵ�
	//		������bCleanUp���Ϊtrue��ڵ��ڲ����������ӽڵ���ڴ棬�������ⲿ����
	//		����ֵ����
	void RemoveAllChildren(bool bCleanUp);
	//		
	//		������GetContentSize
	//		���ã���ȡ�ڵ�����ݴ�С
	//		��������
	//		����ֵ�����ݴ�С
	CCSize GetContentSize();
	//		
	//		������SetContentSize
	//		���ã����ýڵ�����ݴ�С�������ݵĴ�С��Ӱ�쵽���ƣ��������ⲿ����Ҫ��ʾ����
	//		������size���ݴ�С
	//		����ֵ����
	void SetContentSize(CCSize size);
	//		
	//		������GetzOrder
	//		���ã���ȡz���С
	//		��������
	//		����ֵ��z���С, Ĭ��0
	int GetzOrder();
	//		
	//		������GetTag
	//		���ã���ȡ�ڵ��ʶ
	//		��������
	//		����ֵ���ڵ��ʶ
	int GetTag();
	//		
	//		������SetTag
	//		���ã����ýڵ��ʶ
	//		������tag�ڵ��ʶ
	//		����ֵ����
	void SetTag(int tag);
	//
	//		ע�⣺����ӵ��ӽڵ㲻���ظ�
	//		
	//		������AddChild(NDNode* node)
	//		���ã�����ӽڵ�
	//		������node�ӽڵ�
	//		����ֵ����
	void AddChild(NDNode* node);
	//		
	//		������AddChild(NDNode* node, int z)
	//		���ã�����ӽڵ�
	//		������node�ӽڵ㣬z�ӽڵ��z���С
	//		����ֵ����
	void AddChild(NDNode* node, int z);
	//		
	//		������AddChild(NDNode* node, int z, int tag)
	//		���ã�����ӽڵ�
	//		������node�ӽڵ㣬z�ӽڵ��z���С��tag�ӽڵ�ı�ʶ
	//		����ֵ����
	virtual void AddChild(NDNode* pkNode, int nZBuffer, int nTag);
	//		
	//		������RemoveChild(NDNode* node, bool bCleanUp)
	//		���ã�ɾ���ӽڵ�
	//		������node�ӽڵ㣬bCleanUp���Ϊtrue���ɽڵ��ڲ����������ӽڵ���ڴ棬������Ҫ�ⲿ��������
	//		����ֵ����
	virtual void RemoveChild(NDNode* pkNode, bool bCleanUp);
	//		
	//		������RemoveChild(int tag, bool bCleanUp)
	//		���ã�ɾ���ӽڵ�
	//		������tag�ӽڵ�ı�ʶ��bCleanUp���Ϊtrue���ɽڵ��ڲ����������ӽڵ���ڴ棬������Ҫ�ⲿ��������
	//		����ֵ����
	virtual void RemoveChild(int tag, bool bCleanUp);
	//		
	//		������ContainChild
	//		���ã��Ƿ�����ӽڵ�
	//		������node�ӽڵ�
	//		����ֵ��true�ǣ�false��
	bool ContainChild(NDNode* node);
	//		
	//		������RemoveFromParent
	//		���ã��Ӹ��׽ڵ���ɾ�����ڵ㣬���뱣֤���׽ڵ����
	//		������bCleanUp���true������ýڵ���ڴ棬�������������ⲿ��������
	//		����ֵ����
	void RemoveFromParent(bool bCleanUp);
	//		
	//		������GetChild
	//		���ã���ȡ�ӽڵ�
	//		������tag�ӽڵ�ı�ʶ
	//		����ֵ���ӽڵ㣬ʧ�ܷ���null
	NDNode* GetChild(int tag);		
	//		
	//		������IsChildOf
	//		���ã��ж��Ƿ���ĳһ�ڵ���ӽڵ�
	//		������node���׻����游�ڵ�
	//		����ֵ��true�ǣ�false��
	bool IsChildOf(NDNode* node);
	//		
	//		�����������������������ڲ����ã����������
	//		
	//		������EnableDraw
	//		���ã����ýڵ��Ƿ���������
	//		������enabled���false��ֹͣ���ʸýڵ㣬Ĭ��Ϊtrue
	//		����ֵ����
	void EnableDraw(bool enabled);
	//		
	//		������DrawEnabled
	//		���ã��жϽڵ��Ƿ��������
	//		��������
	//		����ֵ��true������ʣ�false��������ʣ�Ĭ��Ϊtrue
	bool DrawEnabled();

	void SetParam1(int nParam1);
	void SetParam2(int nParam2);
	int GetParam1();
	int GetParam2();

	NDAction* RunAction(NDAction* pkAction);

public: //@shader

	CC_SYNTHESIZE_RETAIN(CCGLProgram*, m_pShaderProgram, ShaderProgram);
	CC_SYNTHESIZE(ccGLServerState, m_glServerState, GLServerState);
	CC_SYNTHESIZE_READONLY(CCNode*,m_pkCCNode,CCNode);

protected:

	void DrawSetup( const char* shaderType = kCCShader_PositionTexture_uColor );

protected:

	int m_nParam1;
	int m_nParam2;

protected:

	std::vector<NDNode*> m_kChildrenList;
	NDNode* m_pkParent;
	bool m_bDrawEnabled;

	void SetParent(NDNode* node);

public:

	void SetDestroyNotify(LuaObject func);
	bool GetDestroyNotify(LuaObject& func);

private:

	LuaObject m_kDelegateDestroy;

public:

	void SetPosx(int nposx)
	{
		m_nPosx = nposx;
	}
	void SetPosy(int nposy)
	{
		m_nPosy = nposy;
	}
	int GetPosx(void)
	{
		return m_nPosx;
	}
	int GetPosy(void)
	{
		return m_nPosy;
	}
	void SetNodeLevel(int nLevel)
	{
		m_nLevel = nLevel;
	}
	int GetNodeLevel()
	{
		return m_nLevel;
	}

private:
	//��Ļ��������
	int m_nPosx;
	int m_nPosy;
	int m_nLevel; //��ֵԽ�߼���Խ�ߣ���ֵΪ0����������

public:
	void AddViewer(NDCommonProtocol* viewer);
	void RemoveViewer(NDCommonProtocol* viewer);
public:
	bool DispatchClickOfViewr(NDObject* object);
protected:
	LIST_COMMON_VIEWER m_listCommonViewer;
};

NS_NDENGINE_END

#endif //__NDNODE_H