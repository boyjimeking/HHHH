//--------------------------------------------------------------------
//  TextureList.h
//  ��������
//
//  Created by zhangwq on 2012-12-27.
//  Copyright 2010 (����)DeNA. All rights reserved.
//
//	�����������ͼ��Դ�������л�ʱ�ͷ�������ͼ��Դ.
//	��ע��������CCTexture2D�����������������ϲ�������������.
//		  ��ͼ�Ĵ�����ɾ������map������ܻ��е�����
//		  ���ǿ��ǵ���ͼ����һ��Ҳ��100����������Ӧ�ÿ�������.
//--------------------------------------------------------------------

#pragma once

#include <map>
#include <string>
#include <vector>
using namespace std;
#include "platform/CCPlatformMacros.h"

class CC_DLL TextureList
{
private:
	TextureList() {};

public:
	static TextureList& instance()
	{
		static TextureList s_obj;
		return s_obj;
	}

public:
	void add_tex( void* tex );
	void del_tex( void* tex );

	void add_tex_pvr( void* tex );
	void del_tex_pvr( void* tex );

	void purge();

	string dump( const string& out_file );

private:
	void sortTex( vector<void*>& vecTex );

private:
	map<void*,int> mapTex;
	map<void*,int> mapTexPVR;

	typedef map<void*,int>::iterator		ITER_MAPTEX;
	typedef map<void*,int>::value_type		VAL_MAPTEX;
};