/*
 *  ScriptTimer.h
 *  SMYS
 *
 *  Created by jhzheng on 12-3-9.
 *  Copyright 2012 (����)DeNA. All rights reserved.
 *
 */

#ifndef _SCRIPT_TIMER_H_
#define _SCRIPT_TIMER_H_

#include "Singleton.h"
#include "NDTimer.h"
#include "ScriptInc.h"
#include "GlobalDialog.h"

#define ScriptTimerMgrObj	ScriptTimerMgr::GetSingleton()

class ScriptTimerMgr:
	public TSingleton<ScriptTimerMgr>,
	public ITimerCallback
{
public:
	void OnLoad();
	void OnTimer(OBJID tag);
	unsigned int AddTimer(LuaObject func, float fInterval);
	bool RemoveTimer(OBJID tag);
	bool RemoveAllTimer();
private:
	CIDFactory m_idAlloc;
	std::map<OBJID, LuaObject> m_kMapFunc;
	NDTimer m_kTimer;
};

#endif // _SCRIPT_TIMER_H_
