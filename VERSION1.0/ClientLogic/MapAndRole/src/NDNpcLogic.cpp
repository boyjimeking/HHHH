//---------------------------------------------------------------
//	NDNpcLogic.cpp
//	Npc�߼�
//---------------------------------------------------------------

#include "NDNpcLogic.h"
#include "NDNpc.h"
#include "ScriptGameData.h"
#include "ScriptGameData_NewUtil.h"
#include "ScriptDataBase.h"
#include "NDPlayer.h"
#include "ScriptTask.h"

NS_NDENGINE_BGN

using namespace NDEngine;


int NDHeroTaskLogic::getHeroID() 
{
	return NDPlayer::defaultHero().m_nID;
}

// ˢ���ѽ������б�
void NDHeroTaskLogic::refreshListAccepted()
{
	if (idlistAccept)
	{
		idlistAccept->clear();
		ScriptGameDataObj.GetDataIdList( eScriptDataRole, getHeroID(), eRoleDataTask, *idlistAccept );
	}
}

// ˢ�¿ɽ������б�
bool NDHeroTaskLogic::refreshCanAcceptList()
{
	if (idCanAccept)
	{
		idCanAccept->clear();

		if (!ScriptGameDataObj.GetDataIdList( eScriptDataRole, getHeroID(), eRoleDataTaskCanAccept, *idCanAccept))
		{
			return false;
		}
	}
	return true;
}

// ˢ��ĳ��npc״̬
void NDHeroTaskLogic::tickNpc( NDNpcLogic* npcLogic )
{
	if (!npcLogic || !idlistAccept || !idCanAccept) return;

	int idNpc = npcLogic->Owner->m_nID;
	NDNpc* npc = npcLogic->Owner;

	bool bIfSetState = false;

#if 1
	for (ID_VEC::iterator it = idlistAccept->begin();
			it != idlistAccept->end(); it++)
	{
		// �ɽ�
		int nState = ScriptGetTaskState(*it);
		if (TASK_STATE_COMPLETE == nState
			&& (idNpc == ScriptDBObj.GetN( "task_type", *it, DB_TASK_TYPE_FINISH_NPC )))
		{
			//�����򷵻�
			if (*it /10000 == 5)
			{
				npc->SetNpcState((NPC_STATE)QUEST_FINISH);
				return;
			}
			else 
			{
				//֧�����ȼ��������ߣ����������򷵻�
				npc->SetNpcState((NPC_STATE)QUEST_FINISH_SUB);
				bIfSetState = true;
			}
		}
	}

	if(bIfSetState)
		return;
#endif

#if 2
	ID_VEC idVec;
	npcLogic->GetTaskListByNpc( idVec );

	for (ID_VEC::iterator it = idVec.begin(); it != idVec.end(); it++)
	{
		// �ɽ�
		for (ID_VEC::iterator itCanAccept = idCanAccept->begin();
			itCanAccept != idCanAccept->end(); itCanAccept++)
		{
			if (*it != *itCanAccept) continue;

			if (*it /10000 == 5)
			{
				npc->SetNpcState((NPC_STATE)QUEST_CAN_ACCEPT);
				return;
			}
			else 
			{
				npc->SetNpcState((NPC_STATE)QUEST_CAN_ACCEPT_SUB);                
				bIfSetState = true;
			}
		}
	}

	if(bIfSetState)
		return;
#endif

#if 3
	//δ�������
	for (ID_VEC::iterator it = idlistAccept->begin(); 
		it != idlistAccept->end(); 
		it++) 
	{
		//���ɽ�
		int nState = ScriptGetTaskState(*it);
		if (TASK_STATE_UNCOMPLETE == nState)
		{
			if ( idNpc == ScriptDBObj.GetN("task_type", *it, DB_TASK_TYPE_FINISH_NPC))
			{
				npc->SetNpcState((NPC_STATE)QUEST_NOT_FINISH);
				return;
			}
		}
	}
	npc->SetNpcState((NPC_STATE) QUEST_CANNOT_ACCEPT);
#endif
}
//////////////////////////////////////////////////////////////////////////////////////

//wrapper for time control
void NDNpcLogic::RefreshTaskState()
{
    struct cc_timeval currentTime;
    if (CCTime::gettimeofdayCocos2d(&currentTime, NULL) != 0)
    {
        return;
    }
    double fDeltaTime = (currentTime.tv_sec - tickLastRefresh.tv_sec)*1000.0f + (currentTime.tv_usec - tickLastRefresh.tv_usec) / 1000.0f;
    
    if (TAbs(fDeltaTime) > 1000*1) //1 second
    {
		NDHeroTaskLogic::Instance().tickHero();

		NDHeroTaskLogic::Instance().tickNpc( this );

		tickLastRefresh = currentTime;
	}
}

// ȡĳ��npc�������б�
bool NDNpcLogic::GetTaskListByNpc( ID_VEC& idVec )
{
	if (idlist == NULL) return false;
	
	int TASK_ID = 1;
	int  NPC_ID = 2;

	idVec.clear();

	// init npc task idlist
	if(idlist->empty())
	{
		ScriptDBObj.GetIdList("task_npc", *idlist);

		for(ID_VEC::iterator it = idlist->begin(); it!= idlist->end(); it++)
		{
			int nNpcId = ScriptDBObj.GetN("task_npc", *it, NPC_ID); 
			if(nNpcId == Owner->m_nID )
			{
				int nTaskId = ScriptDBObj.GetN("task_npc", *it, TASK_ID); 
				idVec.push_back(nTaskId);
			}        
		}
	}
	
	return !idVec.empty();
}

NS_NDENGINE_END