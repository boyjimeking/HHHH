//
//  NDAutoPath.mm
//  MapDataPool
//
//  Created by jhzheng on 10-12-13.
//  Copyright 2010 (����)DeNA. All rights reserved.
//

#include "NDAutoPath.h"
#include "NDConstant.h"
#include <sstream>
#include <stdlib.h>
//#include "Performance.h"

namespace NDEngine
{
IMPLEMENT_CLASS(NDAutoPath, NDObject)

static NDAutoPath *shareAutoPath;

NDAutoPath::NDAutoPath()
{
	init();
}

/* ��ȡ������Զ�Ѱ·ʵ��,���δ�����򴴽�
 ����ֵ: ��ͼ���ݳ�
 */
NDAutoPath* NDAutoPath::sharedAutoPath()
{
	if (!shareAutoPath)
	{
		shareAutoPath = new NDAutoPath;
	}

	return shareAutoPath;
}

/* �ͷŹ�����Զ�Ѱ·ʵ��
 */
void NDAutoPath::purgeSharedAutoPath()
{
	m_pointVector.clear();

	CC_SAFE_DELETE (m_pkAStar);
}

void NDAutoPath::init()
{
	m_pointVector.clear();

	m_nStep = 0;

	m_bIgnoreMask = false;

	m_bMustArrive = true;

	m_pkAStar = new CAStar;

	m_pkAStar->SetCheckMethod(checkCanPass);
}

/* ��ĳһ�����ص��Զ�Ѱ·������һ�����ص�(���ͨ��getPathPointArray������ȡ)
 ����:fromPosition-��ʼ���ص�,toPoition-�������ص�,mapLayer-��ͼ��ָ��,step-Ѱ·�������
 */
bool NDAutoPath::autoFindPath(CGPoint kFromPosition, CGPoint kToPosition,
		NDMapLayer* pkMapLayer, int nStep, bool bMustarrive/*=false*/,
		bool bIgnoreMask/*=false*/)
{
	if (!pkMapLayer || !pkMapLayer->GetMapData())
	{
		return false;
	}

	if (!m_pkAStar)
		return false;

	//PerformanceTestName("�Զ�Ѱ·");

	m_nStep = nStep;

	m_bIgnoreMask = bIgnoreMask;

	m_bMustArrive = bMustarrive;

	kFromPosition.x -= DISPLAY_POS_X_OFFSET;
	kFromPosition.y -= DISPLAY_POS_Y_OFFSET;

	kToPosition.x -= DISPLAY_POS_X_OFFSET;
	kToPosition.y -= DISPLAY_POS_Y_OFFSET;

	m_pointVector.clear();

	CMyPos kStartPos;
	CMyPos kEndPos;

	kStartPos.x = (int) kFromPosition.x / MAP_UNITSIZE;
	kStartPos.y = (int) kFromPosition.y / MAP_UNITSIZE;

	kEndPos.x = (int) kToPosition.x / MAP_UNITSIZE;
	kEndPos.y = (int) kToPosition.y / MAP_UNITSIZE;

	std::stringstream ss;

	ss << "\n��ʼѰ·==================" << "\n" << "Ѱ·��ʼ��[" << kStartPos.x << ","
			<< kStartPos.y << "]\n" << "Ѱ·Ŀ���[" << kEndPos.x << "," << kEndPos.y
			<< "]\n";

	//NDLog("%@", [NSString stringWithUTF8String:ss.str().c_str()]);

	m_pkAStar->SetAStarRange(pkMapLayer->GetMapData()->getColumns(),
			pkMapLayer->GetMapData()->getRows());

	unsigned long maxTime = 1000 * 20;

	if (bMustarrive)
	{
		maxTime = 1000 * 1000;
	}

	if (!m_pkAStar->FindPath(pkMapLayer, kStartPos, kEndPos, maxTime, bMustarrive))
		return false;

	this->GetPath();

	return true;

	//if( !checkFromOnePosToAnother(posCur, posNext) && !ignoreMask )
	//if( ![m_PathSet count] )
	//		{
	//			if ( m_NearestNode && mustarrive)
	//			{//ѡ��һ�����յ������·��
	//				GetPath(m_NearestNode);
	//			}
	//			//�޷�����
	//			return NO;
	//		}
	/*
	 std::stringstream ss;
	 uint nStartCellX = (uint)m_curPixelX / 16;
	 uint nStartCellY = (uint)m_curPixelY / 16;
	 uint nEndCellX = (uint)m_targetPixelX / 16;
	 uint nEndCellY = (uint)m_targetPixelY / 16;

	 ss << "\n����Ѱ··��==================" << "\n";

	 ss << "Ѱ·��ʼ:" << "[" << nStartCellX << "," << nStartCellY << "]"
	 << ", Ѱ·Ŀ��" << "[" << nEndCellX << "," << nEndCellY << "]\n";
	 */

	//ss << "[" << currentNode.x << "," << currentNode.y << "]";
	//ss << "\n·������==================\n";
	//NDLog("%@", [NSString stringWithUTF8String:ss.str().c_str()]);
}

/* ��ȡѰ·��ĵõ�����������
 ����ֵ:��������(Ԫ��ΪCGPoint)
 */
const std::vector<CGPoint>& NDAutoPath::getPathPointVetor()
{
	return m_pointVector;
}

/*
 unsigned int NDAutoPath::GetHValue(auto_path_pos pos)
 {
 int nDx = abs((long)(m_posTarget.x-pos.x));
 int nDy = abs((long)(m_posTarget.y-pos.y));

 if(nDx > nDy)
 return 10 * nDy;
 return 10 * nDx;

 }
 */

void NDAutoPath::GetPath()
{
	if (!m_pkAStar)
		return;

	DEQUE_NODE& path = m_pkAStar->GetAStarPath();

	if (path.empty())
		return;

	int nCount = (int) path.size();

	if (nCount > 1)
	{
		std::stringstream ss;
		ss << "\n����Ѱ··��==================" << "\n";
		ss << "�ܲ���:" << nCount << "\n";
		//<< "Ѱ·��ʼ:" << "[" << nStartCellX << "," << nStartCellY << "]"
		//<< ", Ѱ·Ŀ��" << "[" << nEndCellX << "," << nEndCellY << "]"
		//<< "\n";
		for (int i = 0; i < nCount; i++)
		{
			NodeInfo* pathnode = path[i];
			ss << "[" << pathnode->nX << "," << pathnode->nY << "]";
		}
		ss << "\n·������==================\n";

		//NDLog("%@", [NSString stringWithUTF8String:ss.str().c_str()]);
	}

	do
	{
		int iTimes = MAP_UNITSIZE / m_nStep;

		CGPoint kPos;
		NodeInfo* pkNode = path[0];
		kPos.x = pkNode->nX * MAP_UNITSIZE + DISPLAY_POS_X_OFFSET;
		kPos.y = pkNode->nY * MAP_UNITSIZE + DISPLAY_POS_Y_OFFSET;

		for (int index = 0; index < nCount - 1; index++)
		{
			NodeInfo& first = *(path[index]);
			NodeInfo& second = *(path[index + 1]);
			/*
			 if (first.nX > second.nX)
			 {
			 for (int j=0; j<iTimes; j++)
			 {
			 // �ӵ�
			 pos.x				-=	m_nStep;

			 m_pointVector.push_back(pos);
			 }
			 }

			 if (first.nX < second.nX)
			 {
			 for (int j=0; j<iTimes; j++)
			 {
			 // �ӵ�
			 pos.x				+=	m_nStep;

			 m_pointVector.push_back(pos);
			 }
			 }


			 if (first.nY > second.nY)
			 {
			 for (int j=0; j<iTimes; j++)
			 {
			 // �ӵ�
			 pos.y				-= m_nStep;

			 m_pointVector.push_back(pos);
			 }
			 }

			 if (first.nY < second.nY)
			 {
			 for (int j=0; j<iTimes; j++)
			 {
			 // �ӵ�
			 pos.y				+= m_nStep;

			 m_pointVector.push_back(pos);
			 }
			 }
			 */
			if (first.nX > second.nX)
			{
				if (first.nY > second.nY)
				{
					for (int j = 0; j < iTimes; j++)
					{
						// �ӵ�
						kPos.y -= m_nStep;
						kPos.x -= m_nStep;

						m_pointVector.push_back(kPos);
					}
				}
				else if (first.nY < second.nY)
				{
					for (int j = 0; j < iTimes; j++)
					{
						// �ӵ�
						kPos.y += m_nStep;
						kPos.x -= m_nStep;

						m_pointVector.push_back(kPos);
					}
				}
				else if (first.nY == second.nY)
				{
					for (int j = 0; j < iTimes; j++)
					{
						// �ӵ�
						kPos.x -= m_nStep;

						m_pointVector.push_back(kPos);
					}
				}
			}
			else if (first.nX < second.nX)
			{
				if (first.nY > second.nY)
				{
					for (int j = 0; j < iTimes; j++)
					{
						// �ӵ�
						kPos.y -= m_nStep;
						kPos.x += m_nStep;

						m_pointVector.push_back(kPos);
					}
				}
				else if (first.nY < second.nY)
				{
					for (int j = 0; j < iTimes; j++)
					{
						// �ӵ�
						kPos.y += m_nStep;
						kPos.x += m_nStep;

						m_pointVector.push_back(kPos);
					}
				}
				else if (first.nY == second.nY)
				{
					for (int j = 0; j < iTimes; j++)
					{
						// �ӵ�
						kPos.x += m_nStep;

						m_pointVector.push_back(kPos);
					}
				}
			}
			else if (first.nX == second.nX)
			{
				if (first.nY > second.nY)
				{
					for (int j = 0; j < iTimes; j++)
					{
						// �ӵ�
						kPos.y -= m_nStep;

						m_pointVector.push_back(kPos);
					}
				}
				else if (first.nY < second.nY)
				{
					for (int j = 0; j < iTimes; j++)
					{
						// �ӵ�
						kPos.y += m_nStep;

						m_pointVector.push_back(kPos);
					}
				}
				else if (first.nY == second.nY)
				{
				}
			}
		}
	} while (0);

	return;
}

//bool NDAutoPath::checkFromOnePosToAnother(auto_path_pos pos, auto_path_pos otherPos)
//	{
//		return [m_MapData canPassByRow:otherPos.y andColumn:otherPos.x];
//	}

bool NDAutoPath::isIgnoreMask()
{
	return m_bIgnoreMask;
}

bool checkCanPass(NDMapLayer* maplayer, CMyPos& from, CMyPos& to)
{
	if (!maplayer || !maplayer->GetMapData())
		return false;

	if (NDAutoPath::sharedAutoPath()->isIgnoreMask())
	{
		NDMapData *mapData = maplayer->GetMapData();
		if (!mapData || to.y >= mapData->getRows()
				|| to.x >= mapData->getColumns() || to.x <= 0 || to.y <= 0)
		{
			return false;
		}
		return true;
	}

	return maplayer->GetMapData()->canPassByRow(to.y, to.x);
}

}