/*
 *  NDControlHelp.cpp
 *  DragonDrive
 *
 *  Created by zhangwq 2013.01.29
 *  Copyright 2011 (����)DeNA. All rights reserved.
 *
 */

#include "NDControlHelp.h"



void CtrolBase::Init(UIINFO& info, CCSize& sizeOffset)
{
	m_info = info;

	m_sizeOffset = sizeOffset;
}

CCRect CtrolBase::GetFrameRect()
{
	CCRect rect = CCRectZero;

	rect.origin = ccpAdd(
		m_info.CtrlPos, 
		ccp(m_sizeOffset.width, m_sizeOffset.height));

	if (m_info.nCtrlWidth != 0 && m_info.nCtrlHeight != 0)
	{
		rect.size = CCSizeMake(m_info.nCtrlWidth, m_info.nCtrlHeight);
		return rect;
	}

	if (m_info.strNormalFile.empty()) 
	{
		NDAsssert(0);

		return rect;
	}

	NDPicture* pic = NDPicturePool::DefaultPool()->AddPicture(
		NDPath::GetUIImgPath(m_info.strNormalFile.c_str()) );

	rect.size = pic->GetSize();

	SAFE_DELETE( pic );

	return rect;
}

LabelTextAlignment CtrolBase::GetTextAlign()
{
	/*
	"�����";
	"�Ҷ���";
	"ˮƽ����"
	"��ֱ����"
	"����"
	*/

	LabelTextAlignment align = LabelTextAlignmentLeft;
	CCLog("test %s", m_info.strTextAlign.c_str());

	if (stricmp( m_info.strTextAlign.c_str(), GetTxtPri("LeftAlign").c_str()) == 0) //�����
		align = LabelTextAlignmentLeft;

	else if (stricmp( m_info.strTextAlign.c_str(), GetTxtPri("RightAlign").c_str()) == 0) //�Ҷ���
		align = LabelTextAlignmentRight;

	else if (stricmp( m_info.strTextAlign.c_str(), GetTxtPri("HorzCenterAlign").c_str()) == 0) //ˮƽ���ж���
		align = LabelTextAlignmentHorzCenter;

	else if (stricmp( m_info.strTextAlign.c_str(), GetTxtPri("VertCenterAlign").c_str()) == 0) //��ֱ���ж���
		align = LabelTextAlignmentVertCenter;

	else if (stricmp( m_info.strTextAlign.c_str(), GetTxtPri("CenterAlign").c_str()) == 0) //���ж���
		align = LabelTextAlignmentCenter;

	return align;
}

NDPicture* CtrolBase::GetPicture(std::string& filename, CTRL_UV& uv)
{
	NDPicture* res = NULL;

	if (filename.empty())
	{
		return res;
	}

	if (m_info.nCtrlWidth != 0 && m_info.nCtrlHeight != 0)
	{ 
		// ���� (����󲻽���u,v����)
		// ��ȡͼƬ��С����u,v�Ƚ�,��С��һ��˵���ǿ�ͼ,��������,��һ���Ժ���Էŵ�UI�༭��(ֱ�ӵ�������Ϣ)
		const string strTemp = NDPath::GetUIImgPath(filename.c_str());
		
		NDPicture *pic = NDPicturePool::DefaultPool()->AddPicture(strTemp.c_str());

		if (pic)
		{
			CCSize size = pic->GetSize();
			if (uv.w <= int(size.width) && uv.h <= int(size.height))
			{
				pic->Cut(CCRectMake(uv.x, uv.y, uv.w, uv.h ) );

				return pic;
			}
		}

		res  = NDPicturePool::DefaultPool()->AddPicture(
									NDPath::GetUIImgPath(filename.c_str()),
									m_info.nCtrlWidth, m_info.nCtrlHeight );
								
		SAFE_DELETE( pic );
	}
	else
	{ 
		// ������ (�۳�����ͼ�������촦��)
		res  = NDPicturePool::DefaultPool()->AddPicture( NDPath::GetUIImgPath(filename.c_str()) );

		if (uv.w != 0 && uv.h != 0)
		{
			res->Cut(CCRectMake( uv.x, uv.y, uv.w, uv.h ));
		}
	}

	return res;
}