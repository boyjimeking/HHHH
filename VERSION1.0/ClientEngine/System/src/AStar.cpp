#include "AStar.h"
#include <stack>
#include <queue>
#include <map>
#include <algorithm>

using namespace std;

//////////////////////////////////////////////////////////////////////

CAStar::CAStar() :
m_pCheckFun(NULL),
m_nMapWidth(0),
m_nMapHeight(0),
m_pGamemap(NULL)
{
}

CAStar::~CAStar()
{
	this->ClearNodeSet();
}

void CAStar::SetCheckMethod(CHECK_FUN pFun)
{
	NDAsssert(pFun);
	m_pCheckFun = pFun;
}

void CAStar::SetAStarRange(int nMapWidth, int nMapHeight)
{
	NDAsssert(nMapWidth > 0 && nMapHeight > 0);

	m_nMapWidth = nMapWidth;
	m_nMapHeight = nMapHeight;
}

void CAStar::EndFindPath()
{
	m_bDoFindPath = false;
}
//----------------------------------------------------//
BOOL CAStar::FindPath(NDMapLayer* pGamemap, const CMyPos& posStart,
		const CMyPos& posTarget, unsigned long dwMaxSerachTime,
		bool mustArrive/*=false*/)
{
	NDAsssert(m_nMapWidth > 0 && m_nMapHeight > 0);
	NDAsssert (m_pCheckFun);
	NDAsssert(pGamemap);

	m_bDoFindPath = true;
	m_pGamemap = pGamemap;

	m_kSetPath.clear();

	if ((posStart.x == posTarget.x) && (posStart.y == posTarget.y))
		return TRUE;

	m_kPosRealTarget = posStart;
	m_kPosStart = posStart;
	m_kPosTarget = posTarget;

	m_kSetClose.clear();
	m_kSetOpen.clear();

	m_nNodeIndex = 0;
	m_pkNearestNode = NULL;

	///\�����һ���ڵ�
	NodeInfo* pNode = this->CreateNewNode();
	if (pNode)
	{
		pNode->nX = m_kPosStart.x;
		pNode->nY = m_kPosStart.y;
		pNode->nG = 0;
		pNode->nH = GetHValue(m_kPosStart);
		pNode->nStep = 0;
		pNode->pParent = NULL;
		pNode->nChildNum = 0;
		pNode->nDir = 0;
		pNode->nF = pNode->nG + pNode->nH;
		pNode->nNumber = m_kPosStart.x << 16 | m_kPosStart.y;
		m_kSetOpen[pNode->nNumber] = pNode;
	}

	double dTimeBeg = time(NULL); //[NSDate timeIntervalSinceReferenceDate];

	unsigned int nPathStep = 0;

	while (true)
	{
		double dTimePass = time(NULL) - dTimeBeg;
		unsigned long usec = (unsigned long) (dTimePass * 1000000);

		/***
		* ��ʱ��ע�� ����
		* begin
		*/
		// 		if (usec > dwMaxSerachTime)
		// 		{
		// 			//NDLog("astar time out");
		// 			break;
		// 		}
		/***
		* ��ʱ��ע�� ����
		* end
		*/

		if (!m_bDoFindPath)
		{
			return FALSE;
		}

		NodeInfo* pkCurrentNode = this->GetCurrentNode();


		if (NULL == pkCurrentNode)
		{ ///\open���Ѿ�����
			break;
		}

		if ((pkCurrentNode->nX == m_kPosTarget.x)
				&& (pkCurrentNode->nY == m_kPosTarget.y))
		{
			///\�յ����close ��
			this->GetPath(pkCurrentNode);
			break;
		}

		///\����close�� 
		this->AddToList(m_kSetClose, pkCurrentNode);
		this->SearchChild(pkCurrentNode);

		nPathStep++;
	}

	//NDLog("astar step=%d", pathstep);

	if (m_kSetPath.empty())
	{
		if (m_pkNearestNode && mustArrive)
		{ ///\ѡ��һ�����յ������·��
			this->GetPath(m_pkNearestNode);

			return TRUE;
		}
		///\���ܵ��� 
		return FALSE;
	}

	return TRUE;
}

//----------------------------------------------------//
int CAStar::GetHValue(const CMyPos& kPos)
{
	int nDx = abs(m_kPosTarget.x - kPos.x);
	int nDy = abs(m_kPosTarget.y - kPos.y);

	if (nDx > nDy)
	{
		return 10 * nDx + 10 * nDy;
	}

	return 10 * nDy + 10 * nDx;
}

void CAStar::AddToList(MAP_NODE& _list, NodeInfo* pNodeNew)
{
	_list.insert(MAP_NODE::value_type(pNodeNew->nNumber, pNodeNew));
}

struct COMPARE_NODE: public binary_function<int, NodeInfo*, bool>
{
	bool operator()(const pair<int, NodeInfo*>& _1,
			const pair<int, NodeInfo*>& _2) const
	{
		return _1.second->nF < _2.second->nF;
	}
};

NodeInfo* CAStar::GetCurrentNode()
{
	if (m_kSetOpen.empty())
	{
		return NULL;
	}

	///\��open ����ȡһ����ѵ�(F��С)
	MAP_NODE::iterator iterPos;
	iterPos = min_element(m_kSetOpen.begin(), m_kSetOpen.end(), COMPARE_NODE());
	NodeInfo* pNode = iterPos->second;
	m_kSetOpen.erase(iterPos);

	///\���������ǰ��
	return pNode;
}

NodeInfo* CAStar::CheckList(MAP_NODE& _list, int nNumber)
{
	MAP_NODE::iterator iter = _list.find(nNumber);
	if (iter != _list.end())
	{
		NodeInfo* pNode = iter->second;
		return pNode;
	}

	return NULL;
}

void CAStar::UpdateChildren(NodeInfo* pParent)
{
	int nG = pParent->nG + 1;

	NodeInfo* pKidNode = NULL;
	stack<NodeInfo*> kStackNode;

	for (int i = 0; i < pParent->nChildNum; ++i)
	{
		pKidNode = pParent->pChildNode[i];

		if (nG < pKidNode->nG)
		{
			pKidNode->nG = nG;
			pKidNode->nF = nG + pKidNode->nH;
			pKidNode->pParent = pParent;
			pKidNode->nStep = pParent->nStep + 1;
			///\ʡȥ���¼����ӽڵ�ķ������������
			kStackNode.push(pKidNode);
		}
	}

	NodeInfo* pKidParent = NULL;
	while (!kStackNode.empty())
	{
		pKidParent = kStackNode.top();
		kStackNode.pop();

		for (int j = 0; j < pKidParent->nChildNum; ++j)
		{ ///\���������ӽڵ� �������Ƕ�push��ջ�м������ӽڵ�
			pKidNode = pKidParent->pChildNode[j];

			if (pKidParent->nG + 10 < pKidNode->nG)
			{ ///\���·��������������ǵ�G H
				pKidNode->nG = pKidParent->nG + 10;
				pKidNode->nF = nG + pKidNode->nH;
				pKidNode->pParent = pKidParent;
				pKidNode->nStep = pParent->nStep + 1;
				///\ʡȥ���¼����ӽڵ�ķ������������
				kStackNode.push(pKidNode);
			}
		}
	}
}

void CAStar::SearchChild(NodeInfo* pParentNode)
{
	if (NULL == pParentNode)
	{
		return;
	}

	CMyPos kPosNode;
	CMyPos kPosNewNode;

	kPosNode.x = pParentNode->nX;
	kPosNode.y = pParentNode->nY;
	/*
	 int nArrayX[4] = {0, -1, 0, 1};
	 int nArrayY[4] = {1, 0, -1, 0};
	 */
	int nArrayX[8] =
	{ 0, -1, -1, -1, 0, 1, 1, 1 };
	int nArrayY[8] =
	{ 1, 1, 0, -1, -1, -1, 0, 1 };

	for (int i = 0; i < 8; ++i)
	{
		kPosNewNode.x = kPosNode.x + nArrayX[i];
		kPosNewNode.y = kPosNode.y + nArrayY[i];

	//	CCLog("Add Node:%d,%d",posNewNode.x,posNewNode.y);

		if (kPosNewNode.x < 0)
			continue;
		if (kPosNewNode.y < 0)
			continue;
		if (kPosNewNode.x > m_nMapWidth)
			continue;
		if (kPosNewNode.y > m_nMapHeight)
			continue;

		if (!(*m_pCheckFun)(m_pGamemap, kPosNode, kPosNewNode))
		{ ///\��⺯��
			continue;
		}

		int nNumber = kPosNewNode.x << 16 | kPosNewNode.y;
		int nG = 0;

		i % 2 == 0 ? nG = pParentNode->nG + 10 : nG = pParentNode->nG + 14; 

		NodeInfo* pCheck = NULL;
		if (pCheck = this->CheckList(m_kSetOpen, nNumber)) //ifû����!
		{ ///\�Ѿ�����open����
			pParentNode->pChildNode[pParentNode->nChildNum++] = pCheck;
			if (nG < pCheck->nG)
			{ ///\���� G H
				pCheck->pParent = pParentNode;
				pCheck->nG = nG;
				pCheck->nF = nG + pCheck->nH;
				pCheck->nDir = i;
				pCheck->nStep = pParentNode->nStep + 1;
			}
			//pCell[m_nMapWidth*posNewNode.y + posNewNode.x].nOpen =1;
		}
		else if (pCheck = this->CheckList(m_kSetClose, nNumber)) //ifû����!
		{ ///\�Ѿ�����close����
			pParentNode->pChildNode[pParentNode->nChildNum++] = pCheck;
			if (nG < pCheck->nG)
			{ ///\���� G H
				pCheck->pParent = pParentNode;
				pCheck->nG = nG;
				pCheck->nF = nG + pCheck->nH;
				pCheck->nDir = i;
				pCheck->nStep = pParentNode->nStep + 1;
			}

			//pCell[m_nMapWidth*posNewNode.y + posNewNode.x].nClose =1;
			UpdateChildren(pCheck);

			continue;
		}
		else
		{ ///\����һ���µĵ㲢����open����
			NodeInfo* pNewNode = this->CreateNewNode();

			if (!pNewNode)
			{
				continue;
			}

			pNewNode->nX = kPosNewNode.x;
			pNewNode->nY = kPosNewNode.y;

			pNewNode->nG = nG;
			pNewNode->nH = this->GetHValue(kPosNewNode);
			pNewNode->nF = nG + pNewNode->nH;
			pNewNode->nStep = pParentNode->nStep + 1;
			pNewNode->pParent = pParentNode;
			pNewNode->nDir = i;
			pNewNode->nChildNum = 0;
			pNewNode->nNumber = nNumber;

			pParentNode->pChildNode[pParentNode->nChildNum++] = pNewNode;
			//pCell[m_nMapWidth*posNewNode.y + posNewNode.x].nOpen =1;
			this->AddToList(m_kSetOpen, pNewNode);

			// �������...
			unsigned long dwOldV = (m_kPosRealTarget.x - m_kPosTarget.x)
					* (m_kPosRealTarget.x - m_kPosTarget.x)
					+ (m_kPosRealTarget.y - m_kPosTarget.y)
							* (m_kPosRealTarget.y - m_kPosTarget.y);

			unsigned long dwNewV = (pNewNode->nX - m_kPosTarget.x)
					* (pNewNode->nX - m_kPosTarget.x)
					+ (pNewNode->nY - m_kPosTarget.y)
							* (pNewNode->nY - m_kPosTarget.y);

			if (dwNewV < dwOldV)
			{
				m_kPosRealTarget.x = pNewNode->nX;
				m_kPosRealTarget.y = pNewNode->nY;
				m_pkNearestNode = pNewNode;
			}
		}
	}
}

//----------------------------------------------------//
void CAStar::GetPath(NodeInfo* pNode)
{
	NodeInfo* pMyNode = pNode;
	while (pMyNode->pParent)
	{
		m_kSetPath.push_front(pMyNode);
		pMyNode = pMyNode->pParent;
	}

	///\�ӵ�ǰ����λ�ÿ�ʼ�㷽��
	//DEQUE_NODE setDirPath;	
	NodeInfo* pFirstNode = this->CreateNewNode();
	if (pFirstNode)
	{
		pFirstNode->nX = m_kPosStart.x;
		pFirstNode->nY = m_kPosStart.y;
		//setDirPath.push_front(pFirstNode );
		m_kSetPath.push_front(pFirstNode);
	}

	//setDirPath.insert(setDirPath.end(),m_setPath.begin(),m_setPath.end());
	/*
	 ///\���·���
	 for ( int i = 0 ; i < setDirPath.size()-1 ; ++i )
	 {
	 NodeInfo* pMyNode   = setDirPath[i];
	 NodeInfo* pNextNode = setDirPath[i+1];

	 pNextNode->nDir = m_pGamemap->GetDirection(pMyNode->nX, pMyNode->nY,pNextNode->nX, pNextNode->nY);
	 pNextNode->nDir = (pNextNode->nDir+1)%8;
	 }
	 */
}

//----------------------------------------------------//
unsigned long CAStar::GetPathAmount()
{
	return m_kSetPath.size();
}
//----------------------------------------------------//
NodeInfo* CAStar::GetNodeByIndex(int nIndex)
{
	int nAmount = m_kSetPath.size();
	if (nIndex < 0 || nIndex >= nAmount)
		return NULL;
	return m_kSetPath[nIndex];
}

//----------------------------------------------------//
void CAStar::ClearNodeSet()
{
	int nAmount = m_kSetNode.size();
	for (int i = 0; i < nAmount; ++i)
	{
		NodeInfo* pNod = m_kSetNode[i];
		CC_SAFE_DELETE(pNod);
	}
	m_kSetNode.clear();
	m_nNodeIndex = 0;
}

//----------------------------------------------------//
NodeInfo* CAStar::CreateNewNode()
{
	int nAmount = m_kSetNode.size();
	NodeInfo* pNode = NULL;

	if (m_nNodeIndex >= nAmount)
	{
		int n = 1000;
		while (n--)
		{
			pNode = new (NodeInfo);
			m_kSetNode.push_back(pNode);
		}
	}

	pNode = m_kSetNode[m_nNodeIndex];
	++m_nNodeIndex;
	return pNode;
}

DEQUE_NODE& CAStar::GetAStarPath()
{
	return m_kSetPath;
}