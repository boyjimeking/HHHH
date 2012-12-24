#include <fstream>
#include <stdio.h>
#include <cassert>
#include <stdlib.h>
#include "IniFile.h"
#include "ObjectTracker.h"
#include "define.h"

// #ifdef _DEBUG
// #undef THIS_FILE
// static char THIS_FILE[]=__FILE__;
// #define new DEBUG_NEW
// #endif


using namespace std;
/////////////////////////////////////////////////////////////////////
// Construction/Destruction
/////////////////////////////////////////////////////////////////////
/***
* 临时性注释 郭浩
* all
*/
 CIniFile::CIniFile()
 {
	INC_NDOBJ("CIniFile"); 
 }
 
 CIniFile::CIniFile(const char* inipath)
 {
	 INC_NDOBJ("CIniFile"); 
 	m_strPath = inipath;
 }
 
 
 CIniFile::~CIniFile()
 {
	DEC_NDOBJ("CIniFile"); 
 }
 
 
 void CIniFile::SetPath(const char* newpath)
 {
 	m_strPath = newpath;
 }
 
 static void TrimLeftRight(std::string& s)
 {
 	const char* whitespace = "\r\n\t ";
 	int begin = s.find_first_not_of(whitespace);
 	if ( std::string::npos == begin )
 	{
 		s = "";
 		return;
 	}
 	std::string::size_type end = s.find_last_not_of(whitespace);
 	if (end != string::npos)
 		s = s.substr( begin, end-begin+1 );
 }

/************************************************************************************************************
 Function:         DecryptIniFile
 Description:      先判断ini文件是否加密,如果加密的话解密该文件
 Input:            iBufSize保存未加密的数据buf的大小
 Output:           FileBuf获取未加密前的数据buf
 return:           bool返回是否加密
 other:
 version:  1. add by tangziqin  2012.8.27   新建函数    
 2.
 ************************************************************************************************************/
#include "LuaStateMgr.h"
using namespace NDEngine;
bool CIniFile::DecryptIniFile(char *strBuf, int iBufSize)
{
    size_t size = 0;
    FILE *FileHandle = NULL;
    FileHandle = fopen(m_strPath.c_str(), "rb");
    if ( FileHandle == NULL) 
    { 
        return false;
    } 
    fseek (FileHandle, 0, SEEK_END);
    size = ftell(FileHandle);
    fseek(FileHandle, 0, SEEK_SET);
   
    memset(strBuf, 0, iBufSize);
    size_t nReadNum = fread(strBuf, 1, iBufSize, FileHandle);
    if( nReadNum != size ) 
    { 
        return false;
    }
    
    size_t nResultNum = LuaStateMgrObj.GetState()->DecryptString((unsigned char* )strBuf, nReadNum);
    fclose(FileHandle);   
    
    if (nResultNum != nReadNum)
    {
        return true;
    }
    else 
    {
        return false;
    }
}

#define USE_INI_ENCRYPT        //定义这个宏代表使用ini加密功能
//#undef USE_INI_ENCRYPT

bool CIniFile::ReadFile()
{
	//先判断文件是否加密，加密的话先解密，存储在strbuffer中
#ifdef USE_INI_ENCRYPT
	int iBufSize = 512*1024;
	char *strBuffer = new char[iBufSize];
	DecryptIniFile(strBuffer, iBufSize);
#else
	FILE* inifile = fopen(m_strPath.c_str(), "r");
 	int curkey = -1, curval = -1;
 	if (inifile == NULL)
 	{
 		error = "Unable to open ini file.";
 		return 0;
 	}
#endif

 	std::string keyname, valuename, value, valueline;
 	std::string temp;
 	bool bNote = false;

#ifdef USE_INI_ENCRYPT
	char *p = strtok(strBuffer, "\r\n");
	while(p)
#else
	char buf[2048] = {0};
 	while (fgets(buf, 2048, inifile))
#endif
 	{
		#ifdef USE_INI_ENCRYPT
			std::string readinfo = p;
			p = strtok(NULL,  "\r\n");
		#else
			std::string readinfo = buf;
		#endif

 		if (readinfo != "")
 		{
			#ifndef USE_INI_ENCRYPT
 				TrimLeftRight( readinfo );
			#endif
 			if( readinfo.empty() )
 				continue;
 			
 			if (std::string::npos != readinfo.find("//"))
 			{
 				continue;
 			}
 
 			if(bNote)
 			{
 				if( std::string::npos != readinfo.find("*/") )
 				{
 					bNote = false;
 				}
 				continue;
 			}
 
 			if(!bNote)
 			{
 				if( std::string::npos != readinfo.find("/*") )
 				{
 					bNote = true;
 					continue;
 				}
 			}
 
 			
 
 			if (readinfo[0] == '[')
 			{
 				int pos = readinfo.find_first_of("]");
 				if ( std::string::npos == pos )
 				{
 					continue;
 				}
 
 				readinfo = readinfo.substr( 1, pos-1 );
 				keyname = readinfo;
				#ifndef USE_INI_ENCRYPT
					TrimLeftRight( keyname );
				#endif
 			}
 			else if( !readinfo.empty() )
 			{
 				int pos = readinfo.find_first_of("=");
 				if( std::string::npos == pos )
 				{
 					pos = readinfo.find_first_of("/");
 					if( std::string::npos != pos )
 					{
 						valueline = readinfo.substr( 0, pos );
 					}
 					else
					{
 						valueline = readinfo;
					}

					#ifndef USE_INI_ENCRYPT
						TrimLeftRight( valueline );
					#endif

 					if(!valueline.empty())
 						SetValueLine(keyname.c_str(), valueline.c_str());
 				}
 				else
 				{
 					valuename = readinfo.substr( 0, pos );
 
 					std::string::size_type pos2 = readinfo.find("//", pos);
 					if(pos2 != string::npos)
 					{
 						value = readinfo.substr( pos+1, pos2-pos-1 );
 						if(value=="http:"||value=="ftp:")
 							value = readinfo.substr( pos+1 );
 					}
 					else
 					{
 						value = readinfo.substr( pos+1 );
 					}

					#ifndef USE_INI_ENCRYPT
						TrimLeftRight(valuename);
						TrimLeftRight(value);
					#endif
 				
 					SetValue(keyname.c_str(),valuename.c_str(),value.c_str());
 				}
 			}
 		}
 	}

	#ifdef USE_INI_ENCRYPT
		//未解密释放内存,已解密需重写文件
		SAFE_DELETE(strBuffer);
	#else
		fclose(inifile);
	#endif

 	return 1;
}

#if 0
 bool CIniFile::ReadFile()
 {

 	FILE* inifile = fopen(m_strPath.c_str(), "r");
 	int curkey = -1, curval = -1;
 	if (inifile == NULL)
 	{
 		error = "Unable to open ini file.";
 		return 0;
 	}
 	std::string keyname, valuename, value, valueline;
 	std::string temp;
 	bool bNote = false;
	char buf[2048] = {0};
 	while (fgets(buf, 2048, inifile))
 	{
 		std::string readinfo = buf;
 		if (readinfo != "")
 		{
 			TrimLeftRight( readinfo );
 			if( readinfo.empty() )
 				continue;
 			
 			if (std::string::npos != readinfo.find("//"))
 			{
 				continue;
 			}
 
 			if(bNote)
 			{
 				if( std::string::npos != readinfo.find("*/") )
 				{
 					bNote = false;
 				}
 				continue;
 			}
 
 			if(!bNote)
 			{
 				if( std::string::npos != readinfo.find("/*") )
 				{
 					bNote = true;
 					continue;
 				}
 			}
 
 			
 
 			if (readinfo[0] == '[')// && readinfo[readinfo.GetLength()-1] == ']') //if a section heading
 			{
 				int pos = readinfo.find_first_of("]");
 				if ( std::string::npos == pos )
 				{
 					continue;
 				}
 
 				readinfo = readinfo.substr( 1, pos-1 );
 				keyname = readinfo;
 				TrimLeftRight(keyname);
 			}
 			else if( !readinfo.empty() )
 			{
 				int pos = readinfo.find_first_of("=");
 				if( std::string::npos == pos )
 				{
 					pos = readinfo.find_first_of("/");
 					if( std::string::npos != pos )
 					{
 						valueline = readinfo.substr( 0, pos );
 					}
 					else
 						valueline = readinfo;
 					TrimLeftRight(valueline);
 					if(!valueline.empty())
 						SetValueLine(keyname.c_str(), valueline.c_str());
 				}
 				else
 				{
 					valuename = readinfo.substr( 0, pos );
 
 					std::string::size_type pos2 = readinfo.find("//", pos);
 					if(pos2 != string::npos)
 					{
 						value = readinfo.substr( pos+1, pos2-pos-1 );
 						if(value=="http:"||value=="ftp:")
 							value = readinfo.substr( pos+1 );
 					}
 					else
 					{
 						value = readinfo.substr( pos+1 );
 					}
 					TrimLeftRight(valuename);
 				
 					TrimLeftRight(value);
 				
 					SetValue(keyname.c_str(),valuename.c_str(),value.c_str());
 				}
 			}
 		}
 	}
 	fclose(inifile);
 	return 1;
 }
#endif
 bool CIniFile::ReadFile(const char* newpath)
 {
 	SetPath(newpath);
 	return ReadFile();
 }
 void CIniFile::WriteFile()
 {
 	ofstream inifile(m_strPath.c_str());
 	for (int keynum = 0; keynum < m_sections.size(); keynum++)
 	{
 		if (!m_sections[keynum].vv.empty())
 		{
 			inifile << '[' << m_sections[keynum].name.c_str() << ']' << endl;
 			for (int valuenum = 0; valuenum < m_sections[keynum].vv.size(); valuenum++)
 			{
 				inifile << m_sections[keynum].vv[valuenum].name.c_str()
 					<< "=" << m_sections[keynum].vv[valuenum].value.c_str()
 					<< endl;
 			}
 				inifile << endl;
 		}
 	}
 	inifile.close();
 
 }
 
 void CIniFile::Reset()
 {
 	m_section2Index.clear();
 	m_sections.clear();
 }
 
 int CIniFile::GetKeyAmount()
 {
 	return m_sections.size();
 }
 
 int CIniFile::GetLineAmount(const char* keyname)
 {
 	Section2Index::iterator iter = m_section2Index.find( keyname );
 	if ( iter != m_section2Index.end()
 		&& iter->second < m_sections.size() )
 	{
 		Section& s = m_sections[iter->second];
 		return s.vv.size();
 	}
 	else
 	{
 		return -1;
 	}
 }
 
 int CIniFile::GetValueAmount(const char* keyname)
 {
 	Section2Index::iterator iter = m_section2Index.find( keyname );
 	if ( iter != m_section2Index.end()
 		&& iter->second < m_sections.size() )
 	{
 		Section& s = m_sections[iter->second];
 		return s.vv.size();
 	}
 	else
 	{
 		return -1;
 	}
 }
 
 
 const char* CIniFile::GetLine(const char* keyname, int idx)
 {
 	Section2Index::iterator iter = m_section2Index.find( keyname );
 	if ( iter != m_section2Index.end()
 		&& iter->second < m_sections.size() )
 	{
 		Section& s = m_sections[iter->second];
 		if ( s.vv.size() > idx )
 		{
 			return s.vv[idx].line.c_str();
 		}
 		else
 		{
 			error = "Unable to locate specified idx.";
 			return "";
 		}
 	}
 	else
 	{
 		error = "Unable to locate specified key.";
 		return "";
 	}
 }
 
 
 const char* CIniFile::GetValue(const char* keyname, const char* valuename)
 {
 	Section2Index::iterator iter = m_section2Index.find( keyname );
 	if ( iter != m_section2Index.end()
 		&& iter->second < m_sections.size() )
 	{
 		Section& s = m_sections[iter->second];
 		ValueVector::iterator iter_vv = FindValueVector( s.vv, valuename );
 		if ( iter_vv != s.vv.end() )
 		{
 			return (*iter_vv).value.c_str();
 		}
 		else
 	{
 			error = "Unable to locate specified value.";
 		return "";
 	}
 	}
 	else
 	{
 		error = "Unable to locate specified key.";
 		return "";
 	}
 }
 
 
 int CIniFile::GetValueI(const char* keyname, const char* valuename)
 {
 	return atoi(GetValue(keyname,valuename));
 }
 int CIniFile::GetValueI(int keyname, const char* valuename)
 {
 	return atoi(GetValue(keyname,valuename));
 }
 
 double CIniFile::GetValueF(const char* keyname, const char* valuename)
 {
 	return atof(GetValue(keyname, valuename));
 }
 
 
 bool CIniFile::SetValueLine(const char* keyname, const char* valueline, bool create)
 {
 	Section2Index::iterator iter = m_section2Index.find( keyname );
 	if ( iter != m_section2Index.end()
 		&& iter->second < m_sections.size() )
 	{
 		if ( !create )
 		{
 			return false;
 		}
 		Section& s = m_sections[iter->second];
 		Value v;
 		v.line	= valueline;
 		s.vv.push_back( v );
 	}
 	else
 	{
 		if ( !create )
 	{
 			return false;
 		}
 		Section s;
 		Value v;
 		v.line	= valueline;
 		s.vv.push_back( v );
 		s.name = keyname;
 		m_sections.push_back( s );
 		m_section2Index.insert( Section2Index::value_type(keyname, m_sections.size()-1) );
 
 	}
 	return  true;
 	}
 
 CIniFile::ValueVector::iterator
 CIniFile::FindValueVector(CIniFile::ValueVector& vv, const char* valuename)
 {
 	for ( ValueVector::iterator iter=vv.begin();
 	iter!=vv.end(); ++iter )
 	{
 		if ( 0 == strcmp( (*iter).name.c_str(), valuename ) )
 		{
 			return iter;
 		}
 	}
 	return vv.end();
 }
 
 bool CIniFile::SetValue(const char* keyname, const char* valuename, const char* value, bool create)
 {
 	Section2Index::iterator iter = m_section2Index.find( keyname );
 	if ( iter != m_section2Index.end()
 		&& iter->second < m_sections.size() )
 	{
 		Section& s = m_sections[iter->second];
 		ValueVector::iterator iter_vv = FindValueVector( s.vv, valuename );
 		if ( iter_vv != s.vv.end() )
 		{
 			(*iter_vv).value	= value;
 			(*iter_vv).line		= (*iter_vv).name + "=" + (*iter_vv).value;
 		}
 		else
 	{
 			if ( !create )
 			{
 				return false;
 			}
 			Value v;
 			v.name = valuename;
 			v.value = value;
 			v.line		= v.name + "=" + v.value;
 			s.vv.push_back( v );
 		}
 	}
 	else
 	{
 		if (!create)
 		{
 			return false;
 		}
 		Section s;
 		Value v;
 		v.name = valuename;
 		v.value = value;
 		v.line		= v.name + "=" + v.value;
 		s.vv.push_back( v );
 		s.name = keyname;
 		m_sections.push_back( s );
 		m_section2Index.insert( Section2Index::value_type(keyname, m_sections.size()-1) );
 
 	}
 	return  true;
 }
 
 
 bool CIniFile::SetValueI(const char* keyname, const char* valuename, int value, bool create)
 {
 	//CString temp;
 	//temp.Format("%d",value);
 	char temp[256] = "";
 	sprintf( temp, "%d",value );
 	return SetValue(keyname, valuename, temp, create);
 }
 
 bool CIniFile::SetValueF(const char* keyname, const char* valuename, double value, bool create)
 {
 	//CString temp;
 	//temp.Format("%e",value);
 	char temp[256] = "";
 	sprintf( temp, "%e",value );
 	return SetValue(keyname, valuename, temp, create);
 }
 /*
 
 bool CIniFile::DeleteValue(const char* keyname, const char* valuename)
 {	
 	Section2V::iterator iter = m_section2V.find( keyname );
 	if ( iter != m_section2V.end() )
 	{
 		Key2Value& k2v = iter->second.kv;
 		Key2Value::iterator iter_k2v = k2v.find( valuename );
 		if ( iter_k2v != k2v.end() )
 		{
 			k2v.erase( iter_k2v );
 			return true;
 		}
 	}
 	return false;
 }
 
 bool CIniFile::DeleteKey(const char* keyname)
 {
 	Section2V::iterator iter = m_section2V.find( keyname );
 	if ( iter != m_section2V.end() )
 	{
 		m_section2V.erase( iter );
 		return true;
 	}
 	return false;
 }
 */
 
 int CIniFile::FindKey(const char* keyname)
 {
 	assert(false);
 	return -1;
 	
 }
 int CIniFile::FindValue(int keynum, const char* valuename)
 {
 	assert(false);
 	return -1;	
 }
 
 const char* CIniFile::GetValue(int nKeyIndex, const char* sValueName)
 {
 	if ( nKeyIndex < m_sections.size() )
 	{
 		Section& s = m_sections[nKeyIndex];
 		ValueVector::iterator iter_vv = FindValueVector( s.vv, sValueName );
 		if ( iter_vv != s.vv.end() )
 		{
 			return (*iter_vv).value.c_str();
 		}
 	}
 
 	return "";
 }
 
 const char* CIniFile::GetKeyName(int nKeyIndex)
 {
 	if ( nKeyIndex < m_sections.size() )
 	{
 		return m_sections[nKeyIndex].name.c_str();
 	}
 	return "";
 	
 }
 
 void CIniFile::GetPos(const char* keyname, const char* valuename, int* x, int* y)
 {
 	const char* pPos = this->GetValue( keyname, valuename );
 	*x = 0;
 	*y = 0;
         if(!pPos)return ;
 	sscanf( pPos, "%d,%d", x, y );
 }