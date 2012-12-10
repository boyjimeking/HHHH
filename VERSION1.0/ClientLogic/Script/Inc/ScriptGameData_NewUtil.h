/*
 *  ScriptGameData_NewUtil.h
 *  DragonDrive
 *
 *  Created by zhangwq on 12-12-7.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *	
 *	˵����	�µ����ݿ����
 *
 *			��Щ�಻�����������ҹ����࣬����������������ݿ�
 *			����Ҫ���ǣ�ȷ���ӿڼ��ݣ�����LUA�ű���Ҫ��Χ��д~
 */
 
#pragma once

#include "ScriptGameData_New.h"


/////////////////////////////////////////////////////////////////////////////////////////

//����
struct NDGameDataUtil
{
#if 1 //get table,record,field...
	//ȡ����
	static NDTableSetGroup* getTableSetGroup( const DATATYPE_MAJOR majorType )
	{
		// fast
		return NDGameData::Instance().getAt( majorType );
	}

	//ȡ����id can be player id, npc id...��
	static NDTableSet* getTableSet( const DATATYPE_MAJOR majorType, const ID id )
	{
		// fast
		NDTableSet* pTableSet = NULL;
		NDTableSetGroup* pTableSetGroup = getTableSetGroup( majorType );
		if (pTableSetGroup)
		{
			if (majorType != eMJR_DataBase) //INI
			{
				pTableSet = pTableSetGroup->getById( id, true ); //autoAlloc
			}
		}
		assert(pTableSet);
		return pTableSet;
	}

	//ȡ����INIר�ã�
	static NDTableSetIni* getTableSet_ini()
	{
		NDTableSetGroup* pTableSetGroup = getTableSetGroup( eMJR_DataBase );
		if (pTableSetGroup)
		{
			return pTableSetGroup->getIni();
		}
		assert(0);
		return NULL;
	}

	//ȡ��
	static NDTable* getTable( const NDTablePtr& tablePtr, bool autoAlloc = true )
	{
		// normal
		if (tablePtr.majorType == eMJR_DataBase)
		{
			NDTableSetIni* pTableSetIni = getTableSet_ini();
			if (pTableSetIni)
			{
				return pTableSetIni->getAt( tablePtr.id );
			}
		}
		else
		{
			NDTableSet* pTableSet = getTableSet( tablePtr.majorType, tablePtr.id );
			if (pTableSet)
			{
				return pTableSet->getAt( tablePtr.minorType, autoAlloc );
			}
		}
		return NULL;
	}

	//ȡ��¼
	static NDRecord* getRecord( const NDTablePtr& tablePtr, const ID idRecord, bool autoAlloc = true )
	{
		// slow
		NDTable* pTable = getTable( tablePtr, autoAlloc );
		if (pTable)
		{
			return pTable->getById( idRecord, autoAlloc );
		}
		return NULL;
	}

	//ȡ�ֶ�
	static NDField* getField( const NDTablePtr& tablePtr, const NDCellPtr& cellPtr, bool autoAlloc = true )
	{
		//very slow !
		NDRecord* pRecord = getRecord( tablePtr, cellPtr.idRecord, autoAlloc );
		if (pRecord)
		{
			return pRecord->getAt( cellPtr.fieldIndex, autoAlloc );
		}
		return NULL;
	}
#endif 

#if 2 //getter
	//get number
	static double getDataN( const NDTablePtr& tablePtr, const NDCellPtr& cellPtr )
	{
		//very slow !
		NDField* pField = NDGameDataUtil::getField( tablePtr, cellPtr );
		if (pField)
		{
			return pField->getDouble();
		}
		return 0.0;
	}

	//get float
	static float getDataF( const NDTablePtr& tablePtr, const NDCellPtr& cellPtr )
	{
		//very slow !
		NDField* pField = NDGameDataUtil::getField( tablePtr, cellPtr );
		if (pField)
		{
			return pField->getFloat();
		}
		return 0.0f;
	}

	//get string
	static string getDataS( const NDTablePtr& tablePtr, const NDCellPtr& cellPtr )
	{
		//very slow !
		NDField* pField = NDGameDataUtil::getField( tablePtr, cellPtr );
		if (pField)
		{
			return pField->getStr();
		}
		return "";
	}

	//get long long
	static unsigned long long getDataULL( const NDTablePtr& tablePtr, const NDCellPtr& cellPtr )
	{
		//very slow
		NDField* pField = getField( tablePtr, cellPtr );
		if (pField)
		{
			return pField->getBigInt();
		}
		return 0L;
	}

	//get unsigned int
	static unsigned int getDataUInt( const NDTablePtr& tablePtr, const NDCellPtr& cellPtr )
	{
		//very slow !
		NDField* pField = NDGameDataUtil::getField( tablePtr, cellPtr );
		if (pField)
		{
			return pField->getUInt();
		}
		return 0;
	}

	//get unsigned long
	static unsigned long getDataULong( const NDTablePtr& tablePtr, const NDCellPtr& cellPtr )
	{
		return getDataUInt( tablePtr, cellPtr );
	}
#endif 

#if 3 //setter
	//set number
	static bool setDataN( const NDTablePtr& tablePtr, const NDCellPtr& cellPtr,
							const double number )
	{
		//very slow !
		NDField* pField = NDGameDataUtil::getField( tablePtr, cellPtr, true );
		if (pField)
		{
			pField->setDouble( number );
			return true;
		}
		return false;
	}

	//set float
	static bool setDataF( const NDTablePtr& tablePtr, const NDCellPtr& cellPtr,
							const float fValue )
	{
		//very slow !
		NDField* pField = NDGameDataUtil::getField( tablePtr, cellPtr, true );
		if (pField)
		{
			pField->setFloat( fValue );
			return true;
		}
		return false;
	}

	//set string
	static bool setDataS( const NDTablePtr& tablePtr, const NDCellPtr& cellPtr,
							const char* str )
	{
		//very slow !
		NDField* pField = NDGameDataUtil::getField( tablePtr, cellPtr, true );
		if (pField)
		{
			pField->setString( str );
			return true;
		}
		return false;
	}
#endif

#if 4 //del
	//del record
	static bool delRecordById( const NDTablePtr& tablePtr, const ID idRecord )
	{
		NDTable* pTable = getTable( tablePtr );
		if (pTable)
		{
			pTable->delById( idRecord );
		}
		return true;
	}

	//del table
	static bool delTable( const NDTablePtr& tablePtr )
	{
		NDTableSet* pTableSet = getTableSet( tablePtr.majorType, tablePtr.id );
		if (pTableSet)
		{
			pTableSet->delAt( tablePtr.minorType );
			return true;
		}
		return false;
	}

	//del tableset
	static bool delTableSet( const DATATYPE_MAJOR majorType, const ID id )
	{
		NDTableSetGroup* pTableSetGroup = getTableSetGroup( majorType );
		if (pTableSetGroup)
		{
			return pTableSetGroup->delById( id );
		}
		return false;
	}

	//del tablesetGroup
	static bool delTableSetGroup( const DATATYPE_MAJOR majorType )
	{
		return NDGameData::Instance().delAt( majorType );
	}

	//del all game data.
	static void destroyAll()
	{
		NDGameData::Instance().destroy();
	}
#endif

#if 5 //for role id
	//��ȡid�б�
	static bool getDataIdList( const NDTablePtr& tablePtr, vector<ID>& out_idList )
	{
		out_idList.clear();

		NDTable* pTable = getTable( tablePtr );
		if (pTable)
		{
			return pTable->getIdList( out_idList );
		}
		return false;
	}
#endif 

#if 6
	//��ȡ��ɫid�б�
	static bool getRoleDataIdList( const NDTablePtr& tablePtr, 
		const ID idRole, const int idListIndex,
		vector<ID>& out_idList )
	{
		out_idList.clear();

		NDTable* pTable = getTable( tablePtr );
		if (pTable)
		{
			if (tablePtr.majorType == eMJR_DataBase) //ini
			{
				//not used: idRole & idListIndex
				return pTable->getIdList( out_idList );
			}
			else
			{
				return pTable->getExtra().getIdList( idRole, idListIndex, out_idList );
			}
		}
		return false;
	}

	//��ӽ�ɫid���б���
	static bool addRoleDataId( const NDTablePtr& tablePtr, 
		const ID idRole, const int idListIndex,
		const ID addId )
	{
		NDTable* pTable = getTable( tablePtr );
		if (pTable)
		{
			return pTable->getExtra().addId( idRole, idListIndex, addId );
		}
		return false;
	}

	//ɾ����ɫid�����б��У�
	static bool delRoleDataId( const NDTablePtr& tablePtr, 
		const ID idRole, const int idListIndex,
		const ID delId )
	{
		NDTable* pTable = getTable( tablePtr );
		if (pTable)
		{
			return pTable->getExtra().delId( idRole, idListIndex, delId );
		}
		return false;
	}

	//ɾ����ɫid�б�
	static bool delRoleDataIdList( const NDTablePtr& tablePtr, 
		const ID idRole, const int idListIndex )
	{
		NDTable* pTable = getTable( tablePtr );
		if (pTable)
		{
			return pTable->getExtra().delIdList( idRole, idListIndex );
		}
		return false;
	}
#endif
};

