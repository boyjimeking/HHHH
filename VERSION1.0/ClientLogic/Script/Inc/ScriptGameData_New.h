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

#define WITH_NEW_DB	0 //���أ��Ƿ������µ����ݿ����

typedef unsigned int ID;
#define SAFE_DELETE(p) { if (p) delete p; p = NULL; }

///////////////////////////////////////////////////////////////////////////////////////

//ǰ����������ظ������
class NDVariant;			//ֵ
class NDField;				//�ֶ�
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
	string  str;

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
	} val;
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
		if (type == V_Short)
			return val.sVal;

		if (type == V_UShort)
			return val.usVal;

		return 0;
	}

	int	getInt()
	{
		switch (type)
		{
		case V_Int:		return val.iVal; break;
		case V_UInt:	return val.uiVal; break;
		
		case V_Short:	return val.sVal; break;
		case V_UShort:	return val.usVal; break;

		case V_Char:	return val.cVal; break;
		case V_UChar:	return val.ucVal; break;
		}
		return 0;
	}

	long long getBigInt()
	{
		if (type == V_Int64	||
			type == V_UInt64 )
			return val.bigVal;	
		return 0;
	}

	float getFloat()
	{
		if (type == V_Float)
			return val.fVal;

		if (type == V_Double)
			return val.dVal;

		return 0.0f;
	}

	double getDouble()
	{
		if (type == V_Double)
			return val.dVal;

		return double(0.0f);
	}

	// by ref
	const string& getStr()
	{
		if (type != V_String)
			return "";

		return str;
	}

	// by val
	const string getStrCopy()
	{
		if (type != V_String)
			return "";

		return str;
	}

public: //setter
	void setChar( const char c )					{ val.cVal = c; }
	void setUChar( const unsigned char c )			{ val.ucVal = c; }
	
	void setShort( const short s )					{ val.sVal = s; }
	void setUShort( const unsigned short s )		{ val.usVal = s; }

	void setInt( const int n )						{ val.iVal = n; }
	void setUInt( const unsigned int n )			{ val.uiVal = n; }

	void setBigInt( const long long l )				{ val.bigVal = l; }
	void setUBigInt( const unsigned long long l )	{ val.ubigVal = l; }

	void setFloat( const float f )					{ val.fVal = f; }
	void setDouble( const double d )				{ val.dVal = d; }
	void setString( const string& s )				{ str = s; }
	void setString( const char* s )					{ str = (s ? s : ""); }

}; //NDField

///////////////////////////////////////////////////////////////////////////////////////

//@@ ��Ӧһ����¼
class NDRecord
{
private:
	NDRecord();

public:
	NDRecord( ID in_idRecord, int in_fieldCount )
		: idRecord( in_idRecord ) 
	{
		vecData.resize( in_fieldCount );
	}

	~NDRecord() { this->destroy(); }

private:
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
			NDField*& ptr = vecData[index];
			if (!ptr && autoAlloc)
			{
				ptr = new NDField;
			}
			return ptr;
		}
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
	NDTableBase( const ID in_idTable, int in_fieldCount )
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

	NDRecord* getById( const ID idRecord )
	{
		return this->find( idRecord );
	}

	NDRecord* find( const ID idRecord )
	{
		ITER_RECORD iter = mapRecord.find( idRecord );
		if (iter != mapRecord.end())
			return iter->second;
		return NULL;
	}

public:
	// find it, add new when not exist.
	NDRecord* addNew( const ID idRecord )
	{
		NDRecord* pRecord = new NDRecord( idRecord, fieldCount );
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
		FOUR_IDLIST* fourIdList = this->findbyOwner( idOwner );
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
		FOUR_IDLIST* fourIdList = this->findbyOwner( idOwner );
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

	//���б���ɾ��id
	bool delId( const ID idOwner, const int index, const ID id )
	{
		FOUR_IDLIST* fourIdList = this->findbyOwner( idOwner );
		if (fourIdList)
		{
			mapFourIdList[ idOwner ] = fourIdList;
		}

		if (fourIdList)
		{
			return fourIdList->delId( index, id );
		}
		return false;
	}

	//ɾ����ɫ�б�
	bool delIdList( const ID idOwner, const int index )
	{
		FOUR_IDLIST* fourIdList = this->findbyOwner( idOwner );
		if (fourIdList)
		{
			return fourIdList->delIdList( index );
		}
		return false;
	}

private:
	FOUR_IDLIST* findbyOwner( const ID idOwner )
	{
		ITER_FOUR_IDLIST iter = mapFourIdList.find( idOwner );
		if (iter != mapFourIdList.end())
		{
			return iter->second;
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
};

///////////////////////////////////////////////////////////////////////////////////////

//@@ ��Ӧһ�ű�=�������� + �������ݣ�
class NDTable : public NDTableBase
{
public:
	NDTable( const ID in_idTable, int in_fieldCount )
		: NDTableBase( in_idTable, in_fieldCount ) {}
public:
	NDTableExtra extra;
};


///////////////////////////////////////////////////////////////////////////////////////

//@@ ��Ӧһ�ѱ���
//		���ɫ�����У����ܱ��������Ʒ��...
//		������ı������ǹ̶���(����eMIN_COUNT)
class NDTableSet
{
public:
	NDTableSet()
	{
		bIniOnly = false;
		memset( arrTable, 0, sizeof(arrTable));
	}

	~NDTableSet() { this->destroy(); }

public:
	NDTableSetIni* castIni()
	{
		return bIniOnly ? (NDTableSetIni*) this : NULL;
	}

public:
	NDTable* addNew( DATATYPE_MINOR minorType, const ID id, const int fieldCount )
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

	NDTable* getAt( const DATATYPE_MINOR minorType )
	{
		if (minorType >= 0 && minorType < tableCount)
		{
			return arrTable[minorType];
		}
		return NULL;
	}

	bool delAt( const DATATYPE_MINOR minorType )
	{
		if (minorType >= 0 && minorType < tableCount)
		{
			SAFE_DELETE( arrTable[minorType] );
			return true;
		}
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

private:
	NDTable* arrTable[eMIN_COUNT];	//�̶�������
	static const int tableCount = eMIN_COUNT;	

protected:
	bool bIniOnly; //�Ƿ�INIר�ñ�
};

///////////////////////////////////////////////////////////////////////////////////////

//@@ ��Ӧһ�ѱ��ϣ�INIר�ã�
//		��NDTableSet��ͬ���ǣ����������ǹ̶��ģ���INI�кܶ�ı�����eMIN_COUNT.
class NDTableSetIni : public NDTableSet
{
public:
	NDTableSetIni() { bIniOnly = true; }
	~NDTableSetIni() { this->destroy_Ini(); }

public:
	int getCount() { return mapTable.size(); }

	NDTable* addNew_INI( const ID id, const int fieldCount )
	{
		NDTable* newTable = new NDTable( id, fieldCount );
		mapTable[id] = newTable;
		
		assert(newTable);
		return newTable;
	}

	NDTable* getAt_INI( const ID id )
	{
		ITER_TABLE iter = mapTable.find( id );
		if (iter != mapTable.end())
			return iter->second;
		return NULL;
	}

private:
	void destroy_Ini()
	{
		for (ITER_TABLE iter = mapTable.begin(); iter != mapTable.end(); ++iter)
		{
			SAFE_DELETE( iter->second );
		}
		mapTable.clear();
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
	~NDTableSetGroup() { this->destroy(); }

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

	//ֻ��һ�������������INI��ÿ��INI��һ�ű�����INI��һ������
	NDTableSet* getSingle( bool iniFlag )
	{
		if (mapTableSet.size() == 0)
		{
			return addNew(0, iniFlag);
		}
		return mapTableSet[0];
	}

private:
	NDTableSet* addNew( ID id, bool iniFlag = false )
	{
		NDTableSet* tableSet = NULL;
		if (iniFlag)
		{
			tableSet = new NDTableSetIni();
		}
		else
		{
			tableSet = new NDTableSet();
		}

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
	}

private:
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
			return tableSetGroup[majorType];
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

private:
	NDTableSetGroup* tableSetGroup[eMJR_COUNT]; //�������
	static const int tableSetGroupCount = eMJR_COUNT;
};