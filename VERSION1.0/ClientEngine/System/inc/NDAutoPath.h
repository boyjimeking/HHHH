//
//  NDAutoPath.h
//  MapDataPool
//
//  Created by jhzheng on 10-12-13.
//  Copyright 2010 (����)DeNA. All rights reserved.
//

#ifndef __NDAUTOPATH_H__
#define __NDAUTOPATH_H__

#include "NDMapLayer.h"
#include "NDObject.h"
#include "AStar.h"

namespace NDEngine
{
bool checkCanPass(NDMapLayer* maplayer, CMyPos& from, CMyPos& to);

class NDAutoPath: NDObject
{
	DECLARE_CLASS (NDAutoPath)
public:
	explicit NDAutoPath();
	~NDAutoPath();

	/* ��ȡ������Զ�Ѱ·ʵ��,���δ�����򴴽�
	 ����ֵ: ��ͼ���ݳ�
	 */
	static NDAutoPath* sharedAutoPath();

	/* �ͷŹ�����Զ�Ѱ·ʵ��
	 */
	void purgeSharedAutoPath();

	/* ��ĳһ�����ص��Զ�Ѱ·������һ�����ص�(���ͨ��getPathPointArray������ȡ)
	 ����:fromPosition-��ʼ���ص�,toPoition-�������ص�,mapLayer-��ͼ��ָ��,step-Ѱ·�������
	 */
	bool autoFindPath(CCPoint kFromPosition, CCPoint kToPosition,
			NDMapLayer* pkMapLayer, int nStep, bool bMustarrive = false,
			bool bIgnoreMask = false);

	/* ��ȡѰ·��ĵõ�����������
	 ����ֵ:��������(Ԫ��ΪCCPoint)
	 */
	const std::vector<CCPoint>& getPathPointVetor();

	bool isIgnoreMask();

private:
	void init();

	//unsigned int GetHValue(auto_path_pos pos);

	void GetPath();

private:
	std::vector<CCPoint> m_kPointVector;					// Ѱ·�����õ�������Vector
	bool m_bIgnoreMask, m_bMustArrive;
	CAStar* m_pkAStar;
	int m_nStep;
};

}
#endif //__NDAUTOPATH_H__
