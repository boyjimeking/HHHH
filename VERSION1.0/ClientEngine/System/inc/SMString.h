/*
 *  SMString.h
 *  SMYS
 *
 *  Created by jhzheng on 12-2-10.
 *  Copyright 2012 (网龙)DeNA. All rights reserved.
 *
 */

#ifndef _STRING_H_ZJH_
#define _STRING_H_ZJH_

#include <string>
#include <stdlib.h>
#include "ObjectTracker.h"

// 这里要求后续使用CString格式化后的字符串长度不能超过4096-1
namespace tq
{

class CString: public std::string
{
public:
	CString();
	CString(const std::string& str)
	{
		INC_NDOBJ("CString[tq]");
		this->assign(str.c_str());
	}
	CString(const CString& str)
	{
		INC_NDOBJ("CString[tq]");
		this->assign(str.c_str());
	}
	CString(int i)
	{
		INC_NDOBJ("CString[tq]");
		this->Format("%d", i);
	}
	CString(unsigned int u)
	{
		INC_NDOBJ("CString[tq]");
		this->Format("%u", u);
	}
	CString(float f)
	{
		INC_NDOBJ("CString[tq]");
		this->Format("%f", f);
	}
	CString(double d)
	{
		INC_NDOBJ("CString[tq]");
		this->Format("%f", d);
	}

	//CString(const char* pstr)		{ if (pstr) this->assign(pstr); }
	explicit CString(const char* fmt, ...);
	virtual ~CString();

	CString& operator =(const std::string& str)
	{
		this->assign(str.c_str());
		return *this;
	}
	CString& operator =(const CString& str)
	{
		this->assign(str.c_str());
		return *this;
	}
	CString& operator =(const char* pstr)
	{
		if (pstr)
			this->assign(pstr);
		return *this;
	}
	CString& operator =(int i)
	{
		this->Format("%d", i);
		return *this;
	}
	CString& operator =(unsigned int u)
	{
		this->Format("%u", u);
		return *this;
	}
	CString& operator =(float f)
	{
		this->Format("%f", f);
		return *this;
	}
	CString& operator =(double d)
	{
		this->Format("%f", d);
		return *this;
	}

	const char operator [](int nPos)
	{
		return this->at(nPos);
	}

	bool operator ==(const std::string& str)
	{
		return (0 == this->compare(str));
	}
	bool operator !=(const std::string& str)
	{
		return (0 != this->compare(str));
	}

	bool operator ==(const char* pstr)
	{
		if (pstr)
			return (0 == this->compare(pstr));
		else
			return false;
	}
	bool operator !=(const char* pstr)
	{
		return !(operator ==(pstr));
	}

	operator const char*() const
	{
		return this->c_str();
	}

	operator char() const
	{
		return (char) atoi(this->c_str());
	}
	operator unsigned char() const
	{
		return (unsigned char) atoi(this->c_str());
	}

	operator int() const
	{
		return atoi(this->c_str());
	}
	operator unsigned int() const
	{
		return atoi(this->c_str());
	}

	operator long() const
	{
		return atol(this->c_str());
	}
	operator unsigned long() const
	{
		return atol(this->c_str());
	}

	operator double() const
	{
		return atof(this->c_str());
	}
	operator float() const
	{
		return (float) atof(this->c_str());
	}

	enum
	{
		MAX_STRING = 4096
	};
	void Format(const char* fmt, ...);
};

}

#endif // _STRING_H_ZJH_
