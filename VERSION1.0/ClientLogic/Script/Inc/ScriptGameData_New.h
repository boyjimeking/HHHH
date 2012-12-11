/*
 *  ScriptGameData_New.h
 *  DragonDrive
 *
 *  Created by zhangwq on 12-12-6.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *	
 *	˵�����µ����ݿ����
 */
 
#pragma once

#include <assert.h>
#include <string>
#include <vector>
#include <map>
using namespace std;

#define WITH_NEW_DB	1 //���أ��Ƿ������µ����ݿ����

typedef unsigned int ID;
#define SAFE_DELETE(p) { if (p) delete p; p = NULL; }

void WriteCon(const char * pszFormat, ...);
#define LOGERR	WriteCon
#define LOGINFO	WriteCon

///////////////////////////////////////////////////////////////////////////////////////

//ǰ����������ظ������
class NDVariant;			//ֵ
class NDField;				//�ֶ�

class NDRecordInterface;	//��¼�ӿ�
class NDRecordFast;			//��¼Fast
class NDRecordSlow;			//��¼Slow
class NDRecord;				//��¼

class NDTableBase;			//�����
class NDTableExtra;			//�����Я������
class NDTable;				//��

class NDTableSet;			//��
class NDTableSetIni;		//����INIר�ã�
class NDTableSetGroup;		//��Ⱥ��
class NDGameDB;				//������Ϸ����

struct NDTablePtr;			//�����ࣺ��ָ��
struct NDCellPtr;			//�����ࣺ��Ԫ��ָ��
///////////////////////////////////////////////////////////////////////////////////////

// �����together use with lua
enum DATATYPE_MAJOR
{
	eMJR_DataBase		= 0, //��̬INI����

	eMJR_Role			= 1, //��ɫ����	
	eMJR_Npc			= 2, //NPC����
	eMJR_Monster		= 3, //�������ݣ�������û�õ���
	eMJR_TaskConfig		= 4, //��������

	eMJR_ItemInfo		= 5, //hero��Ʒ����
	eMJR_SysItem		= 6, //ϵͳ��Ʒ
	eMJR_PetInfo		= 7, //��������
	eMJR_Magic			= 8, //ħ�����ݣ�
	eMJR_AffixBoss		= 9, //Boss����
	// other type of script data ...

	eMJR_COUNT			= 10,
};

///////////////////////////////////////////////////////////////////////////////////////

// �����together use with lua
//		 ��������������ʹ��.
enum DATATYPE_MINOR
{
	eMIN_Basic				= 0, //����
	eMIN_Pet				= 1, //����
	eMIN_Skill				= 2, //����
	eMIN_State				= 3, //״̬��������ûʹ�õ���
	eMIN_Item				= 4, //��Ʒ����
	eMIN_Task				= 5, //����
	eMIN_TaskCanAccept		= 6, //�ɽ�����
	eMIN_UserState       	= 7,
	eMIN_UserEmail       	= 8,
	eMIN_Friend             = 9,
	eMIN_HeroStar			= 10,
	// other Role data ...
	
	eMIN_COUNT				= 11,
};

///////////////////////////////////////////////////////////////////////////////////////

//ֵ������
// together use with lua
enum VAR_TYPE
{
	V_None,

	V_Char,
	V_UChar,

	V_Short,
	V_UShort,

	V_Int,
	V_UInt,

	V_Float,
	V_Double,

	V_Int64,
	V_UInt64,

	V_String,
	V_End,
};

///////////////////////////////////////////////////////////////////////////////////////

//@@ �����ࣺ��ָ��
//	 ������ãã�ı���Ψһȷ��һ�ű�
struct NDTablePtr
{
	NDTablePtr( DATATYPE_MAJOR in_majorType, ID in_id, DATATYPE_MINOR in_minorType)
		: majorType(in_majorType)
		, id(in_id)
		, minorType(in_minorType) {}

	DATATYPE_MAJOR majorType;
	ID id;
	DATATYPE_MINOR minorType;
};

//@@ �����ࣺ��Ԫ��ָ��
//	 ������һ�ű���Ψһȷ��һ������
struct NDCellPtr
{
	NDCellPtr( ID in_idRecord, int in_fieldIndex ) 
		: idRecord(in_idRecord)
		, fieldIndex(in_fieldIndex) {}
	
	ID idRecord;
	int fieldIndex;
};

#define MAKE_NDTABLEPTR(majorType,id,minorType)	\
	NDTablePtr( DATATYPE_MAJOR(majorType), (id), DATATYPE_MINOR(minorType))

#define MAKE_NDTABLEPTR_INI(id)		\
	MAKE_NDTABLEPTR(eMJR_DataBase,id,eMIN_Basic)
	
#define MAKE_CELLPTR(idRecord,fieldIndex)	\
	NDCellPtr( idRecord, fieldIndex )
///////////////////////////////////////////////////////////////////////////////////////

//@@ ��Ӧһ��ֵ
struct NDVariant
{
	NDVariant()
		: type(V_None)
		{ val.ubigVal = 0; }

	VAR_TYPE type;

	union  
	{
		//char
		char				cVal;
		unsigned char		ucVal;

		//short
		short				sVal;
		unsigned short		usVal;

		//int
		int					iVal;
		unsigned int		uiVal;

		//float
		float				fVal;

		//double
		double				dVal;

		//long long
		long long			bigVal;
		unsigned long long	ubigVal;

		//string
		string*				str;
	} val;

	void setStr( const char* in_str )
	{
		if (in_str) {
			if (!val.str) val.str = new string();
			*val.str = in_str;
			type = V_String;
		}
	}
};

///////////////////////////////////////////////////////////////////////////////////////

//@@ ��Ӧһ���ֶ�ֵ
class NDField : NDVariant
{
public: //getter
	char getChar()
	{
		if (type == V_Char)
			return val.cVal;

		if (type == V_UChar)
			return val.ucVal;

		return 0;
	}

	short getShort()
	{
		switch (type)
		{
		case V_Short:	return (short) val.sVal; break;
		case V_UShort:	return (short) val.usVal; break;
		}
		return 0;
	}

	unsigned long getULong()
	{
		return (unsigned long) getUInt();
	}

	//���ݸ��ָ�ʽ
	unsigned int getUInt()
	{
		switch (type)
		{
		case V_Char:	return (unsigned int) val.cVal; break;
		case V_UChar:	return (unsigned int) val.ucVal; break;

		case V_Short:	return (unsigned int) val.sVal; break;
		case V_UShort:	return (unsigned int) val.usVal; break;

		case V_Int:		return (unsigned int) val.iVal; break;
		case V_UInt:	return (unsigned int) val.uiVal; break;

		case V_Float:	return (unsigned int) val.fVal; break;
		case V_Double:	return (unsigned int) val.dVal; break;

		case V_Int64:	return (unsigned int) val.bigVal; break;
		case V_UInt64:	return (unsigned int) val.ubigVal; break;
		}
		return 0;
	}

	//���ݸ��ָ�ʽ
	int	getInt()
	{
		switch (type)
		{
		case V_Char:	return (int) val.cVal; break;
		case V_UChar:	return (int) val.ucVal; break;

		case V_Short:	return (int) val.sVal; break;
		case V_UShort:	return (int) val.usVal; break;

		case V_Int:		return val.iVal; break;
		case V_UInt:	return (int) val.uiVal; break;
		
		case V_Float:	return (int) val.fVal; break;
		case V_Double:	return (int) val.dVal; break;

		case V_Int64:	return (int) val.bigVal; break;
		case V_UInt64:	return (int) val.ubigVal; break;
		}
		return 0;
	}

	//���ݸ��ָ�ʽ
	long long getBigInt()
	{
		switch (type)
		{
		case V_Char:	return (long long) val.cVal; break;
		case V_UChar:	return (long long) val.ucVal; break;

		case V_Short:	return (long long) val.sVal; break;
		case V_UShort:	return (long long) val.usVal; break;

		case V_Int:		return (long long) val.iVal; break;
		case V_UInt:	return (long long) val.uiVal; break;

		case V_Float:	return (long long) val.fVal; break;
		case V_Double:	return (long long) val.dVal; break;

		case V_Int64:	return (long long) val.bigVal; break;
		case V_UInt64:	return (long long) val.ubigVal; break;
		}
		return 0L;
	}

	//���ݸ��ָ�ʽ
	float getFloat()
	{
		switch (type)
		{
		case V_Char:	return (float) val.cVal; break;
		case V_UChar:	return (float) val.ucVal; break;

		case V_Short:	return (float) val.sVal; break;
		case V_UShort:	return (float) val.usVal; break;

		case V_Int:		return (float) val.iVal; break;
		case V_UInt:	return (float) val.uiVal; break;

		case V_Float:	return (float) val.fVal; break;
		case V_Double:	return (float) val.dVal; break;

		case V_Int64:	return (float) val.bigVal; break;
		case V_UInt64:	return (float) val.ubigVal; break;
		}
		return 0.0f;
	}

	//LUA��number��Ӧ���
	//���ݸ��ָ�ʽ
	double getDouble()
	{
		switch (type)
		{
		case V_Char:	return (double) val.cVal; break;
		case V_UChar:	return (double) val.ucVal; break;

		case V_Short:	return (double) val.sVal; break;
		case V_UShort:	return (double) val.usVal; break;

		case V_Int:		return (double) val.iVal; break;
		case V_UInt:	return (double) val.uiVal; break;

		case V_Float:	return (double) val.fVal; break;
		case V_Double:	return (double) val.dVal; break;

		case V_Int64:	return (double) val.bigVal; break;
		case V_UInt64:	return (double) val.ubigVal; break;
		}

		return double(0.0f);
	}

	// by val
	const string getStr()
	{
		if (type == V_String)
			return val.str ? *val.str : "";
		return "";
	}

public: //setter
	void setChar( const char c )					{ val.cVal = c;		type = V_Char; }
	void setUChar( const unsigned char c )			{ val.ucVal = c;	type = V_UChar; }
	
	void setShort( const short s )					{ val.sVal = s;		type = V_Short; }
	void setUShort( const unsigned short s )		{ val.usVal = s;	type = V_UShort; }

	void setInt( const int n )						{ val.iVal = n;		type = V_Int; }
	void setUInt( const unsigned int n )			{ val.uiVal = n;	type = V_UInt; }

	void setBigInt( const long long l )				{ val.bigVal = l;	type = V_Int64; }
	void setUBigInt( const unsigned long long l )	{ val.ubigVal = l;	type = V_UInt64; }

	void setFloat( const float f )					{ val.fVal = f;		type = V_Float; }
	void setDouble( const double d )				{ val.dVal = d;		type = V_Double; }

	void setString( const string& s )				{ setString(s.c_str()); type = V_String; }
	void setString( const char* s )					{ setStr(s);			type = V_String; }

public:
	int getBytesLen() const
	{
		int n = sizeof(val); 
		if (type == V_String && val.str)
			n += val.str->capacity(); //length()
		return n;
	}
}; //NDField

///////////////////////////////////////////////////////////////////////////////////////

//@@ ��¼�ӿ�
class NDRecordInterface
{
public:
	virtual ID getId() = 0;
	virtual int getFieldCount() = 0;
	virtual NDField* getAt( const int index, bool autoAlloc = true ) = 0;
	virtual int getBytesLen() const = 0;
};

//@@ ��Ӧһ����¼
//		slow: use map
class NDRecordSlow : public NDRecordInterface
{
private:
	NDRecordSlow();

public:
	// field count is knowned!
	NDRecordSlow( ID in_idRecord )
		: idRecord( in_idRecord ) {}

	~NDRecordSlow() { this->destroy(); }

private:
	ID					idRecord;	//��¼id
	map<int,NDField*>	mapData;	//��¼�����ɸ��ֶ�ֵ����index������index������!��
	typedef map<int,NDField*>::iterator ITER_FIELD;

public:
	ID getId() { return idRecord; }
	int getFieldCount() { return mapData.size(); }

	NDField* getAt( const int index, bool autoAlloc = true ) 
	{
		ITER_FIELD iter = mapData.find( index );
		if (iter != mapData.end())
			return iter->second;

		if (autoAlloc)
		{
			NDField* pField = new NDField; 
			mapData[index] = pField;
			return pField;
		}
		LOGERR( "@@ NDRecord::getAt(%d), bad index: %d\r\n", index, index );
		return NULL;
	}

private:
	void destroy()
	{
		for (ITER_FIELD iter = mapData.begin(); iter != mapData.end(); ++iter)
		{
			SAFE_DELETE( iter->second );
		}
		mapData.clear();
	}

public:
	int getBytesLen() const
	{
		int n = mapData.size() * 8 + 4;
		for (map<int,NDField*>::const_iterator iter = mapData.begin();
				iter != mapData.end(); ++iter)
		{
			NDField* pField = iter->second;
			if (pField)
				n += pField->getBytesLen();
		}
		return n;
	}
};

//@@ ��Ӧһ����¼
//		fast: use vector
class NDRecordFast : public NDRecordInterface
{
private:
	NDRecordFast();

public:
	// field count should be specified, and can't be 0.
	NDRecordFast( ID in_idRecord, int in_fieldCount )
		: idRecord( in_idRecord ) 
		, fieldCount( in_fieldCount )
	{
		assert (in_fieldCount > 0);
		vecData.resize( in_fieldCount );
	}

	~NDRecordFast() { this->destroy(); }

private:
	int					fieldCount;	//�ֶθ���
	ID					idRecord;	//��¼id
	vector<NDField*>	vecData;	//��¼�����ɸ��ֶ�ֵ

public:
	ID getId() { return idRecord; }
	int getFieldCount() { return vecData.size(); }

	NDField* getAt( const int index, bool autoAlloc = true ) 
	{
		//fast
		if (index >= 0 && index < vecData.size())
		{
			// auto alloc field obj
			NDField*& ptr = vecData[index];
			if (!ptr && autoAlloc)
			{
				ptr = new NDField;
			}
			return ptr;
		}
		LOGERR( "@@ NDRecord::getAt(%d), bad index: %d\r\n", index, index );
		return NULL;
	}

private:
	void destroy()
	{
		for (int i = 0; i < vecData.size(); i++)
		{
			SAFE_DELETE( vecData[i] );
		}
		vecData.clear();
	}

public:
	int getBytesLen() const
	{
		int n = vecData.size() * 4 + 8;
		for (int i = 0; i < vecData.size(); i++)
		{
			NDField* pField = vecData[i];
			if (pField)
				n += pField->getBytesLen();
		}
		return n;
	}
};

//@@ ��Ӧһ����¼
class NDRecord
{
private:
	NDRecord();

	enum {
		E_RECORD_FAST,
		E_RECORD_SLOW,
	} eRecordType;

public:
	// fast record!
	NDRecord( ID in_idRecord, int in_fieldCount )
	{
		assert(in_fieldCount > 0);
		slowRecord = NULL;
		fastRecord = new NDRecordFast( in_idRecord, in_fieldCount ); //fast record!
		eRecordType = E_RECORD_FAST;
	}

	// slow record!
	NDRecord( ID in_idRecord )
	{
		slowRecord = new NDRecordSlow( in_idRecord );
		fastRecord = NULL;
		eRecordType = E_RECORD_SLOW;
	}

	~NDRecord() 
	{ 
		SAFE_DELETE( slowRecord );
		SAFE_DELETE( fastRecord );
	}

public: //ctor helper
	static NDRecord* createFast( ID in_idRecord, int in_fieldCount )
	{
		return new NDRecord( in_idRecord, in_fieldCount );
	}

	static NDRecord* createSlow( ID in_idRecord )
	{
		return new NDRecord( in_idRecord );
	}

public:
	ID getId() 
	{ 
		return getRecordPtr()->getId();
	}
	
	int getFieldCount() 
	{
		return getRecordPtr()->getFieldCount();
	}

	NDField* getAt( const int index, bool autoAlloc = true ) 
	{
		return getRecordPtr()->getAt( index, autoAlloc );
	}

public:
	int getBytesLen() const
	{
		return getRecordPtr()->getBytesLen();
	}

private:
	NDRecordInterface* getRecordPtr() const
	{
		if (eRecordType == E_RECORD_FAST)	return fastRecord;
		if (eRecordType == E_RECORD_SLOW)	return slowRecord;
		return NULL;
	}
private:
	NDRecordFast*	fastRecord;
	NDRecordSlow*	slowRecord;
};

///////////////////////////////////////////////////////////////////////////////////////

//@@ ��Ӧһ�ű����ࣩ
class NDTableBase
{
private:
	ID		idTable;
	int		fieldCount; //�ֶθ���
	map<ID,NDRecord*>	mapRecord; //��¼��
	typedef map<ID,NDRecord*>::iterator ITER_RECORD;

private:
	NDTableBase();
	void init() {}

public:
	NDTableBase( const ID in_idTable, int in_fieldCount = 0)
		: idTable(in_idTable)
		, fieldCount( in_fieldCount ) 
	{ 
		this->init(); 
	}

	~NDTableBase() { this->destroy(); }

public:
	int getCount() { return mapRecord.size(); }

	int getFieldCount() { return fieldCount; }

	ID  getTableId() { return idTable; }

	NDRecord* getById( const ID idRecord, bool autoAlloc = true )
	{
		ITER_RECORD iter = mapRecord.find( idRecord );
		if (iter != mapRecord.end())
			return iter->second;

		if (autoAlloc)
		{
			NDRecord* pRecord = addNew( idRecord );
			return pRecord;
		}
		return NULL;
	}

public:
	// find it, add new when not exist.
	NDRecord* addNew( const ID idRecord )
	{
		NDRecord* pRecord = NULL;

		if (fieldCount > 0)
		{
			// field count is known, we can use fast record.
			pRecord = NDRecord::createFast( idRecord, fieldCount );
		}
		else
		{
			// field count is unknown, we can only use slow record.
			pRecord = NDRecord::createSlow( idRecord );
		}
		
		assert(pRecord);
		mapRecord[ idRecord ] = pRecord;
		return pRecord;
	}

	bool delById( const ID idRecord )
	{
		ITER_RECORD iter = mapRecord.find( idRecord );
		if (iter != mapRecord.end())
		{
			SAFE_DELETE( iter->second );
			mapRecord.erase( iter );
			return true;
		}
		return false;		
	}

public:
	bool getIdList( vector<ID>& vecId )
	{
		vecId.clear();
		vecId.resize( mapRecord.size() );

		int idx = 0;
		for (ITER_RECORD iter = mapRecord.begin();
				iter != mapRecord.end(); ++iter)
		{
			vecId[idx++] = iter->second->getId();
		}
		return true;
	}

private:
	void destroy()
	{
		for (ITER_RECORD iter = mapRecord.begin(); iter != mapRecord.end(); ++iter)
		{
			SAFE_DELETE( iter->second );
		}
		mapRecord.clear();
	}

public:
	virtual int getBytesLen() const
	{
		int n = mapRecord.size() * 8 + 8;
		for (map<ID,NDRecord*>::const_iterator iter = mapRecord.begin(); 
				iter != mapRecord.end(); ++iter)
		{
			NDRecord* pRecord = iter->second;
			if (pRecord)
				n += pRecord->getBytesLen();
		}
		return n;
	}
};

///////////////////////////////////////////////////////////////////////////////////////

//@@ �����Я������
class NDTableExtra			
{
private:
	struct FOUR_IDLIST
	{
		static const int count = 4;
		vector<ID>	vecId[4]; //4��ID�б�

		bool clone( const int index, vector<ID>& out_idList )
		{
			if (index >= 0 && index < count)
			{
				out_idList.clear();
				out_idList = vecId[index]; //copy.
				return true;
			}
			return false;
		}

		bool addId( const int index, const ID id )
		{
			if (index >= 0 && index < count)
			{
				vecId[index].push_back( id );
				return true;
			}
			return false;
		}

		bool delId( const int index, const ID id )
		{
			//slow:
			if (index >= 0 && index < count)
			{
				vector<ID>& vec = vecId[index];
				for (int i = 0; i < vec.size(); i++)
				{
					if (vec[i] == id)
					{
						vec.erase( vec.begin() + i );
						return true;
					}
				}
			}
			return false;
		}

		bool delIdList( const int index )
		{
			if (index >= 0 && index < count)
			{
				vecId[index].clear();
				return true;
			}
			return false;
		}

		int getBytesLen() const
		{
			int n = 0;
			for (int i = 0; i < 4; i++)
				n += vecId[i].size() * 4;
			return n;
		}
	};

protected:
	map<ID,FOUR_IDLIST*> mapFourIdList;
	typedef map<ID,FOUR_IDLIST*>::iterator ITER_FOUR_IDLIST;

public:
	~NDTableExtra() { this->destroy(); }

public:
	//��ȡid�б�
	bool getIdList( const ID idOwner, const int index, vector<ID>& out_IdList )
	{
		FOUR_IDLIST* fourIdList = this->findbyOwner( idOwner, false );
		if (fourIdList)
		{
			fourIdList->clone( index, out_IdList );
			return true;
		}
		return false;
	}

	//���id���б�
	bool addId( const ID idOwner, const int index, const ID id )
	{
		FOUR_IDLIST* fourIdList = this->findbyOwner( idOwner, true );
		if (!fourIdList)
		{
			fourIdList = new FOUR_IDLIST();
			mapFourIdList[ idOwner ] = fourIdList;
		}

		if (fourIdList)
		{
			fourIdList->addId( index, id );
			return true;
		}
		return false;
	}

	//ɾ��һ��id
	bool delId( const ID idOwner, const int index, const ID id )
	{
		FOUR_IDLIST* fourIdList = this->findbyOwner( idOwner, false );
		if (fourIdList)
		{
			return fourIdList->delId( index, id );
		}
		return false;
	}

	//ɾ��һ��id�б�
	bool delIdList( const ID idOwner, const int index )
	{
		FOUR_IDLIST* fourIdList = this->findbyOwner( idOwner, false );
		if (fourIdList)
		{
			return fourIdList->delIdList( index );
		}
		return false;
	}

private:
	FOUR_IDLIST* findbyOwner( const ID idOwner, bool autoAlloc = true )
	{
		ITER_FOUR_IDLIST iter = mapFourIdList.find( idOwner );
		if (iter != mapFourIdList.end())
		{
			return iter->second;
		}

		if (autoAlloc)
		{
			FOUR_IDLIST* p = new FOUR_IDLIST;
			mapFourIdList[ idOwner ] = p;
			return p;
		}
		return NULL;
	}

	void destroy()
	{
		for (ITER_FOUR_IDLIST iter = mapFourIdList.begin(); 
				iter != mapFourIdList.end(); ++iter)
		{
			SAFE_DELETE( iter->second );
		}
		mapFourIdList.clear();
	}

public:
	int getBytesLen() const
	{
		int n = 0;
		for (map<ID,FOUR_IDLIST*>::const_iterator iter = mapFourIdList.begin(); 
				iter != mapFourIdList.end(); ++iter)
		{
			n += 4;
			n += iter->second->getBytesLen();
		}
		return n;
	}
};

///////////////////////////////////////////////////////////////////////////////////////

//@@ ��Ӧһ�ű�=�������� + �������ݣ�
class NDTable : public NDTableBase
{
public:
	NDTable( const ID in_idTable, int in_fieldCount = 0 )
		: NDTableBase( in_idTable, in_fieldCount )
		, extra(NULL)
		, dbgName(NULL) {}
public:
	NDTableExtra& getExtra() 
	{
		if (!extra)
		{
			extra = new NDTableExtra;
		}
		return *extra;
	}
private:
	NDTableExtra* extra;

public:
	void setDbgName( const char* str )
	{
#if _WIN32 && _DEBUG
		if (str && str[0])
		{
			if (!dbgName)
			{
				dbgName = new string();
			}
			*dbgName = str;
		}
#endif
	}
private:
	string* dbgName;

public:
	//override
	int getBytesLen() const
	{
		return NDTableBase::getBytesLen() + 
			(extra ? extra->getBytesLen() : 0);
	}
};


///////////////////////////////////////////////////////////////////////////////////////

//@@ ��Ӧһ�ѱ��ϣ��������ǹ̶��ģ�
//		���ɫ�����У����ܱ��������Ʒ��...
//		������ı������ǹ̶���(����eMIN_COUNT)
class NDTableSet
{
public:
	NDTableSet()
	{
		memset( arrTable, 0, sizeof(arrTable));
	}

	~NDTableSet() { this->destroy(); }

public:
	NDTable* addNew( DATATYPE_MINOR minorType, const ID id, int fieldCount = 0 )
	{
		if (minorType >= 0 && minorType < tableCount)
		{
			NDTable*& table = arrTable[minorType];
			if (!table)
			{
				table = new NDTable( id, fieldCount );
				return table;
			}
		}
		assert(0);
		return NULL;
	}

	NDTable* getAt( const DATATYPE_MINOR minorType, bool autoAlloc = true )
	{
		if (minorType >= 0 && minorType < tableCount)
		{
			if (!arrTable[minorType])
			{
				arrTable[minorType] = addNew( minorType, ID(minorType));
			}
			return arrTable[minorType];
		}

		LOGERR( "@@ NDTableSet::getAt(%d), bad index: %d\r\n", (int)minorType, (int)minorType );
		return NULL;
	}

	bool delAt( const DATATYPE_MINOR minorType )
	{
		if (minorType >= 0 && minorType < tableCount)
		{
			SAFE_DELETE( arrTable[minorType] );
			return true;
		}

		LOGERR( "@@ NDTableSet::delAt(%d), bad index: %d\r\n", (int)minorType, (int)minorType );
		return false;
	}

private:
	void destroy()
	{
		for (int i = 0; i < tableCount; i++)
		{
			SAFE_DELETE( arrTable[i] );
		}
	}

public:
	int getBytesLen() const
	{
		int n = 4 * tableCount;
		for (int i = 0; i < tableCount; i++)
		{
			NDTable* pTable = arrTable[i];
			if (pTable)
			{
				n += pTable->getBytesLen();
			}
		}
		return n;
	}

private:
	NDTable* arrTable[eMIN_COUNT];	//�̶�������
	static const int tableCount = eMIN_COUNT;	
};

///////////////////////////////////////////////////////////////////////////////////////

//@@ ��Ӧһ�ѱ��ϣ�INIר�ã�
class NDTableSetIni
{
public:
	NDTableSetIni() {}
	~NDTableSetIni() { this->destroy(); }

public:
	int getCount() { return mapTable.size(); }

	//implement
	NDTable* addNew( const ID id, const int fieldCount )
	{
		NDTable* newTable = new NDTable( id, fieldCount );
		mapTable.insert( map<ID,NDTable*>::value_type( id, newTable ));
		
		assert(newTable);
		return newTable;
	}

	NDTable* getAt( const ID id )
	{
		ITER_TABLE iter = mapTable.find( id );
		if (iter != mapTable.end())
			return iter->second;
		return NULL;
	}

private:
	void destroy()
	{
		for (ITER_TABLE iter = mapTable.begin(); iter != mapTable.end(); ++iter)
		{
			SAFE_DELETE( iter->second );
		}
		mapTable.clear();
	}

public:
	int getBytesLen() const
	{
		int n = mapTable.size() * 8;
		for (map<ID,NDTable*>::const_iterator iter = mapTable.begin();
				iter != mapTable.end(); ++iter)
		{
			NDTable* pTable = iter->second;
			if (pTable)
			{
				n += pTable->getBytesLen();
			}
		}
		return n;
	}

private:
	map<ID,NDTable*>	mapTable;
	typedef map<ID,NDTable*>::iterator ITER_TABLE;
};

///////////////////////////////////////////////////////////////////////////////////////

//@@ ��Ӧһ������Ⱥ��
//		�磺һ����ɫ���������ݶ�Ӧһ�����ϣ�
//			�����ɫ���������ݶ�Ӧһ������Ⱥ��.
class NDTableSetGroup
{
public:
	NDTableSetGroup() : tableSetIni(NULL) {}
	~NDTableSetGroup() { this->destroy(); }

	//get the table set for ini
	NDTableSetIni* getIni()
	{
		if (!tableSetIni)
		{
			tableSetIni = new NDTableSetIni;
		}
		assert(tableSetIni);
		return tableSetIni;
	}

	//id: can be a role id, or npc id, or...
	NDTableSet* getById( const ID id, bool autoAlloc = true )
	{
		ITER_TABLESET iter = mapTableSet.find( id );
		if (iter != mapTableSet.end())
			return iter->second;

		if (autoAlloc)
		{
			return addNew( id );
		}
		return NULL;
	}

	bool delById( const ID id )
	{
		ITER_TABLESET iter = mapTableSet.find( id );
		if (iter != mapTableSet.end())
		{
			SAFE_DELETE( iter->second );
			mapTableSet.erase( iter );
			return true;
		}
		return false;
	}

private:
	NDTableSet* addNew( ID id, bool iniFlag = false )
	{
		NDTableSet* tableSet = new NDTableSet();
		assert(tableSet);
		mapTableSet[id] = tableSet;
		return tableSet;
	}

private:
	void destroy()
	{
		for (ITER_TABLESET iter = mapTableSet.begin(); 
				iter != mapTableSet.end(); ++iter)
		{
			SAFE_DELETE( iter->second );
		}
		mapTableSet.clear();

		SAFE_DELETE(tableSetIni);
	}

public:
	int getBytesLen() const
	{
		int n = 0;
		if (tableSetIni)
		{
			n += tableSetIni->getBytesLen();
		}
		
		n += mapTableSet.size() * 8;
		for (map<ID,NDTableSet*>::const_iterator iter = mapTableSet.begin(); 
				iter != mapTableSet.end(); ++iter)
		{
			NDTableSet* pTableSet = iter->second;
			if (pTableSet)
				n += pTableSet->getBytesLen();
		}
		return n;
	}

private:
	NDTableSetIni* tableSetIni; //INIר��
	map<ID,NDTableSet*> mapTableSet;
	typedef map<ID,NDTableSet*>::iterator ITER_TABLESET;
};

///////////////////////////////////////////////////////////////////////////////////////

//@@ ��Ӧ������Ϸ����
class NDGameData
{
public:
	static NDGameData& Instance()
	{
		static NDGameData s_obj;
		return s_obj;
	}

	//~NDGameData() { this->destroy(); }

private:
	NDGameData() { this->init(); }

public: //getter
	NDTableSetGroup* getINI()		{ return this->getAt( eMJR_DataBase ); }
	NDTableSetGroup* getRole()		{ return this->getAt( eMJR_Role ); }
	NDTableSetGroup* getNpc()		{ return this->getAt( eMJR_Npc ); }
	NDTableSetGroup* getMonster()	{ return this->getAt( eMJR_Monster ); }
	NDTableSetGroup* getTaskConfig(){ return this->getAt( eMJR_TaskConfig ); }
	NDTableSetGroup* getItemInfo()	{ return this->getAt( eMJR_ItemInfo ); }
	NDTableSetGroup* getSysItem()	{ return this->getAt( eMJR_SysItem ); }
	NDTableSetGroup* getPetInfo()	{ return this->getAt( eMJR_PetInfo ); }
	NDTableSetGroup* getMagic()		{ return this->getAt( eMJR_Magic ); }
	NDTableSetGroup* getAffixBoss()	{ return this->getAt( eMJR_AffixBoss ); }

public:
	NDTableSetGroup* getAt( const DATATYPE_MAJOR majorType )
	{
		if (majorType >= 0 && majorType < tableSetGroupCount)
		{
			NDTableSetGroup*& pTableSetGroup = tableSetGroup[majorType];
			if (!pTableSetGroup)
			{
				pTableSetGroup = new NDTableSetGroup();
			}
			return pTableSetGroup;
		}
		return NULL;
	}

	bool delAt( const DATATYPE_MAJOR majorType )
	{
		if (majorType >= 0 && majorType < tableSetGroupCount)
		{
			SAFE_DELETE( tableSetGroup[majorType] );
			return true;
		}
		return false;
	}

private:
	void init()
	{
		for (int i = 0; i < tableSetGroupCount; i++)
		{
			tableSetGroup[i] = new NDTableSetGroup();
		}
	}

public:
	void destroy()
	{
		for (int i = 0; i < tableSetGroupCount; i++)
		{
			SAFE_DELETE( tableSetGroup[i] );
		}

		//the array is left with empty slots.
	}

public:
	int getBytesLen() const
	{
		int n = 4 * tableSetGroupCount;
		for (int i = 0; i < tableSetGroupCount; i++)
		{
			if (tableSetGroup[i])
				n += tableSetGroup[i]->getBytesLen();
		}
		return n;
	}

private:
	NDTableSetGroup* tableSetGroup[eMJR_COUNT]; //�������
	static const int tableSetGroupCount = eMJR_COUNT;
};