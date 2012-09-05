/*
 *  SMString.cpp
 *  SMYS
 *
 *  Created by jhzheng on 12-2-10.
 *  Copyright 2012 (����)DeNA. All rights reserved.
 *
 */

#include "SMString.h"
#include <stdio.h>

using namespace std;
using namespace tq;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CString::CString() :
string()
{

}

//////////////////////////////////////////////////////////////////////
CString::CString(const char* fmt, ...) :
		string()
{
	if (fmt)
	{
		try
		{
			char buffer[MAX_STRING] = "";
#if _MSC_VER < 1400     //below VS.net 2003
			::vsprintf(buffer, fmt, (char*) ((&fmt) + 1));
#else
			buffer[sizeof(buffer)-1]='\0';
			::vsprintf_s(buffer, sizeof(buffer)-1, fmt, (char *)((&fmt)+1) );
#endif
			this->assign(buffer);
		} catch (...)
		{
			//LogSave("Error: too big size of string in format.");
		}
	}
}

//////////////////////////////////////////////////////////////////////
CString::~CString()
{

}

//////////////////////////////////////////////////////////////////////
void CString::Format(const char* fmt, ...)
{
	if (!fmt)
		return;

	try
	{
		char buffer[MAX_STRING] = "";
#if _MSC_VER >= 1400      //below VS.net 2003
		buffer[sizeof(buffer)-1]='\0';
		::vsprintf_s(buffer, sizeof(buffer)-1, fmt, (char *)((&fmt)+1) );
#else
		va_list arglist;
		va_start(arglist, fmt);
		::vsprintf(buffer, fmt, arglist);
		va_end(arglist);
#endif
		this->assign(buffer);
	} catch (...)
	{
		//LogSave("Error: too big size of string in format.");
	}
}
