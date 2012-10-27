/*
*
*/

#ifndef NDMAPMGR_H
#define NDMAPMGR_H

#include <vector>
#include <string>
#include <map>
#include "NDObject.h"
//#include "GatherPoint.h"
#include "FriendElement.h"
#include "NDScene.h"
#include "NDManualRole.h"
#include "NDTransData.h"
#include "NDMonster.h"
#include "NDNpc.h"
#include "NDBaseRole.h"
#include "NDUICustomView.h"
#include "NDMapLayer.h"
#include "NDConsole.h"
#include "NDTimer.h"
#include "AutoLink.h"

using namespace std;

#define NDMapMgrObj NDEngine::NDMapMgr::GetSingleton()
#define NDMapMgrPtr NDEngine::NDMapMgr::GetSingletonPtr()

namespace NDEngine
{

class NDBaseRole;
class NDNpc;
class NDMonster;
class NDTransData;
class NDManualRole;
class NDScene;

enum 
{
	// battlelistener
	REHEARSE_APPLY = 0, // ����
	REHEARSE_ACCEPT = 1, // ����
	REHEARSE_REFUSE = 2, // �ܾ�
	REHEARSE_LOGOUT = 3, // ս��������
	REHEARSE_LOGIN = 4, // ս���ڶ�������

	// baselistener
	_SEE_USER_INFO = 0, // �鿴�����Ϣ
	SEE_EQUIP_INFO = 1, // �鿴���װ��
	SEE_PET_INFO = 3, // �鿴��ҳ���
};

enum 
{
	_FRIEND_APPLY = 10,// ������Ӻ���:�ͻ��˷�������ֻ��Name��
	// ����������ȥ����ϢӦ�ø���ID
	_FRIEND_ACCEPT = 11, // ͬ����Ӻ���:�ͻ��˷�������ֻ��ID������������ȥ����ϢӦ�ø���Name
	_FRIEND_ONLINE = 12, // ֪ͨ�����ߺ��ѱ����������
	_FRIEND_OFFLINE = 13,// ֪ͨ�����ߺ��ѱ����������
	_FRIEND_BREAK = 14,// ɾ������
	_FRIEND_GETINFO = 15,// ���ͺ�����Ϣ //OBJID idFriend, bool bOnline, const
	// char* pszName
	_FRIEND_REFUSE = 16,// �ܾ���Ӻ���:�ͻ��˷�������ֻ��ID������������ȥ����ϢӦ�ø���Name
	_FRIEND_DELROLE = 17,// ���ɾ����ɫ
};

enum
{
	MONSTER_INFO_ACT_INFO,				//����������Ϣ
	MONSTER_INFO_ACT_POS,				//����λ��
	MONSTER_INFO_ACT_STATE,				//���¹���״̬
	MONSTER_INFO_ACT_BOSS_POS,			//BOSSλ��
	MONSTER_INFO_ACT_BOSS_STATE,		//����BOSS״̬
};

enum 
{
	MSG_TEAM_CREATE = 7758,

	MSG_TEAM_DISMISS = 7759,

	MSG_TEAM_DISMISSFAILE = 7760,

	MSG_TEAM_JOIN = 7761,

	MSG_TEAM_JOINOK = 7762,

	MSG_TEAM_JOINFAILE = 7763,

	MSG_TEAM_KICK = 7764,

	MSG_TEAM_KICKOK = 7765,

	MSG_TEAM_KICKFAILE = 7766,

	MSG_TEAM_INVITE = 7767,

	MSG_TEAM_INVITEOK = 7768,

	MSG_TEAM_INVITEFAILE = 7769,

	MSG_TEAM_LEAVE = 7770,

	MSG_TEAM_ENABLEACCEPT = 7771,

	MSG_TEAM_DISABLEACCEPT = 7772,

	MSG_TEAM_CHGLEADER = 7773,

	MSG_TEAM_TEAMID = 7774,
};

enum  
{
	eTeamLen = 5,
	MAX_FRIEND_NUM = 20,
};

// �ֿ����
enum  
{
	MSG_STORAGE_MONEY_SAVE = 1 ,

	MSG_STORAGE_MONEY_DRAW = 2 ,

	MSG_STORAGE_MONEY_QUERY = 3 ,

	MSG_STORAGE_ITEM_IN = 4 ,

	MSG_STORAGE_ITEM_OUT = 5 ,

	MSG_STORAGE_ITEM_QUERY = 6 ,

	MSG_STORAGE_EMONEY_SAVE = 7 ,

	MSG_STORAGE_EMONEY_DRAW = 8 ,

	MSG_STORAGE_EMONEY_QUERY = 9 ,
};

// �Ի���customview����
enum  
{
	eCVOP_GiftNPC = 300,
	eCVOP_Wish,
	eCVOP_FarmName,
	eCVOP_FarmWelcomeName,
	eCVOP_FarmBuildingName,
	eCVOP_FarmHarmletName,
};
enum  
{
	KEY_LENGTH = 10, // ������кų���
};

enum
{
	RECHARGE_RETURN_ACT_QUERY			= 0,	// ��ֵ˵��
	RECHARGE_RETURN_ACT_TRUE			= 1,	// ��ֵ�ɹ�	
	RECHARGE_RETURN_ACT_FALSE			= 2,	// ��ֵʧ��
	RECHARGE_RETURN_ACT_QUERY_RECORD	= 3,	// ��ֵ��¼
};

//////////////////////////////////
int getShengWangLevel(int v);
int getDiscount(int v);

struct s_team_info
{
	int team[eTeamLen];
	s_team_info() { memset(this, 0, sizeof(*this)); }
};


typedef struct _STRU_MONSTER_TYPE_INFO
{
	int idType;
	int lookFace;
	unsigned char  lev;
	unsigned char  btAiType;
	unsigned char  btCamp;
	unsigned char  btAtkArea;
	string		   name;

	_STRU_MONSTER_TYPE_INFO()
	{
		idType			= 0;
		lookFace		= 0;
		lev				= 0;
		btAiType		= 0;
		btCamp			= 0;
		btAtkArea		= 0;
		name			= "";
	}

	void operator = (_STRU_MONSTER_TYPE_INFO & other)
	{
		if (this == &other)
		{
			return;
		}
		idType			= other.idType;
		lookFace		= other.lookFace;
		lev				= other.lev;
		btAiType		= other.btAiType;
		btCamp			= other.btCamp;
		btAtkArea		= other.btAtkArea;
		name			= other.name;
	}

}monster_type_info;

typedef map<int,monster_type_info>			monster_info;
typedef monster_info::iterator				monster_info_it;
typedef pair<int, monster_type_info>		monster_info_pair;

//typedef map<int,GatherPoint*>				map_gather_point;
//typedef map_gather_point::iterator			map_gather_point_it;
//typedef pair<int, GatherPoint*>				map_gather_point_pair;

typedef map<int, FriendElement>			MAP_FRIEND_ELEMENT;
typedef MAP_FRIEND_ELEMENT::iterator		MAP_FRIEND_ELEMENT_IT;


typedef struct _tagShopItemInfo 
{
	int itemType, payType;
	_tagShopItemInfo() { memset(this, 0, sizeof(*this)); }
	_tagShopItemInfo(int itemType, int payType)
	{
		this->itemType = itemType;
		this->payType = payType;
	}
}ShopItemInfo;

typedef map<int ,vector<ShopItemInfo> >		map_npc_store;
typedef map_npc_store::iterator				map_npc_store_it;
typedef pair<int, vector<ShopItemInfo> >	map_npc_store_pair;

// npc����ѧϰ
typedef vector<int> VEC_BATTLE_SKILL;
typedef VEC_BATTLE_SKILL::iterator VEC_BATTLE_SKILL_IT;

// �ʼ�
//typedef vector<EmailData*>	vec_email;
//typedef vec_email::iterator	vec_email_it;

class NDMapMgr:
	public NDObject,
	public TSingleton<NDMapMgr>,
	public NDMsgObject,
	public NDConsoleListener,
	public ITimerCallback
{
public:

	typedef map<int,NDManualRole*> map_manualrole;
	typedef vector<NDNpc*> VEC_NPC;
	typedef vector<NDMonster*> VEC_MONSTER;

	DECLARE_CLASS(NDMapMgr);

	NDMapMgr();
	virtual ~NDMapMgr();

	virtual void Update(unsigned long ulDiff);
	NDMonster* GetMonster(int nID);

	virtual bool process( MSGID usMsgID, NDEngine::NDTransData* pkData, int nLength );
	void processPlayer(NDTransData* pkData,int nLength);
	void processPlayerExt(NDTransData* pkData,int nLength);
	void processWalk(NDTransData* pkData,int nLength);
	void processWalkTo(NDTransData& kData);
	void processNpcTalk(NDTransData& kData);
	void ProcessLoginSuc(NDTransData& kData);
	void processChangeRoom(NDTransData* pkData,int nLength);
	void processNPCInfoList(NDTransData* pkData,int nLength);

protected:

	void LoadSceneMonster();
	void AddManualRole(int nID,NDManualRole* pkRole);
	NDManualRole* GetManualRole(int nID);
	NDManualRole* GetManualRole(const char* pszName);
	void ClearManualRole();
	void AddAllNPCToMap();
	void AddAllMonsterToMap();
	void DelManualRole(int nID);
	void ClearNPC();
	void ClearMonster();
	void ClearGP();
	bool loadSceneByMapDocID(int nMapID);
	void AddSwitch();
	int GetMapID();
	int GetMotherMapID();

	NDNpc* GetNPC(int nID);

	NDMapLayer* getMapLayerOfScene(NDScene* pkScene);

	virtual void OnCustomViewRadioButtonSelected( NDUICustomView* customView,
		unsigned int radioButtonIndex, int ortherButtonTag );

	virtual bool processConsole( const char* pszInput );

	virtual void OnTimer( OBJID tag );
	virtual NDMonster* GetBattleMonster();
	void SetBattleMonster(NDMonster* pkMonster);

	static bool m_bFirstCreate;
	static bool m_bVerifyVersion;

	map_manualrole m_mapManualRole;
	VEC_NPC m_vNPC;
	VEC_MONSTER m_vMonster;

	NDTimer m_kTimer;

	int m_nCurrentMonsterBound;
	int m_nRoadBlockX;
	int m_nRoadBlockY;
	int m_nMapType;
	int m_nSaveMapID;
	int m_nMapID;
	int m_nMapDocID;

	CCSize m_kMapSize;

	string m_strMapName;

	CAutoLink<NDMonster> m_apWaitBattleMonster;

private:
};

}

#endif