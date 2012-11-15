#include "Singleton.h"
#include "define.h"
#include <vector>
#include <string>
#include <map>
#include "NDMapLayer.h"
#include "NDNetMsg.h"
#include "GlobalDialog.h"
#include "NDTimer.h"
#include "SMBattleScene.h"
#include "EnumDef.h"

using namespace std;

#define NDMapMgrObj NDEngine::NDMapMgr::GetSingleton()
#define NDMapMgrPtr	NDEngine::NDMapMgr::GetSingletonPtr()

class EmailData;
class BattleSkill;
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
		REHEARSE_APPLY = 0, // ����
		REHEARSE_ACCEPT = 1, // ����
		REHEARSE_REFUSE = 2, // �ܾ�
		REHEARSE_LOGOUT = 3, // ս��������
		REHEARSE_LOGIN = 4, // ս���ڶ�������

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
	
	enum  {
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
		_MARRIAGE_APPLY = 1, // �������
		_MARRIAGE_AGREE = 2, // ͬ����
		_MARRIAGE_REFUSE = 3, // �ܾ����
		_DIVORCE_APPLY = 4, // �������
		_DIVORCE_AGREE = 5, // ͬ�����
		_MARRIAGE_QUARY = 6, // �´���żID
		_MARRIAGE_MATE_LOGIN = 7, // ��ż����
		_MARRIAGE_MATE_ONLINE = 8, // ��ż����
		_MARRIAGE_MATE_LOGOUT = 9, // ��ż����
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
	
	//typedef map<int, FriendElement>			MAP_FRIEND_ELEMENT;
	//typedef MAP_FRIEND_ELEMENT::iterator		MAP_FRIEND_ELEMENT_IT;
	
	
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
	typedef vector<EmailData*>	vec_email;
	typedef vec_email::iterator	vec_email_it;
	
	// ȫ�����ݹ����� + ��Ϣ����
	class NDMapMgr :
	public TSingleton<NDMapMgr>,
	public NDMsgObject,
	public NDObject,
	public NDUIDialogDelegate
	//public NDUICustomViewDelegate
	{
		DECLARE_CLASS(NDMapMgr)
	public:
		NDMapMgr();
		~NDMapMgr();
		
		void Update(unsigned long ulDiff);
		
		typedef vector<NDNpc*> VEC_NPC;
		typedef vector<NDMonster*> VEC_MONSTER;
		typedef VEC_NPC::iterator vec_npc_it;
		typedef VEC_MONSTER::iterator vec_monster_it;
		typedef map<int, NDManualRole*> map_manualrole;
		typedef map_manualrole::iterator map_manualrole_it;
		typedef pair<int, NDManualRole*> map_manualrole_pair;
		
		//typedef map<OBJID, LifeSkill> MAP_LEARNED_LIFE_SKILL;
		//typedef MAP_LEARNED_LIFE_SKILL::iterator MAP_LEARNED_LIFE_SKILL_IT;
		
		//typedef map<int,FormulaMaterialData*>		map_fomula;
		//typedef map_fomula::iterator				map_fomula_it;
		//typedef pair<int,FormulaMaterialData*>		map_fomula_pair;
		
		virtual void OnDialogButtonClick(NDUIDialog* dialog, unsigned int buttonIndex); override
		void OnDialogClose(NDUIDialog* dialog); override
		//bool OnCustomViewConfirm(NDUICustomView* customView); override
		/*�������Ȥ��������Ϣ*/
		bool process(MSGID msgID, NDTransData* data, int len); override	
		void processUserInfo(NDTransData* data, int len);
		void processUserAttrib(NDTransData* data, int len);
		void processChangeRoom(NDTransData* data, int len);
		void processPlayer(NDTransData* data, int len);
		void processPlayerExt(NDTransData* data, int len);
		void processWalk(NDTransData* data, int len);
		void processKickBack(NDTransData* data, int len);
		void processDisappear(NDTransData* data, int len);
		void processNpcInfoList(NDTransData* data, int len);
		void processNpcStatus(NDTransData* data, int len);
		void processMonsterInfo(NDTransData* data, int len);
		void processChgPoint(NDTransData* data, int len);
		void processPetInfo(NDTransData* data, int len);
		void processMsgDlg(NDTransData& data);
		//void processLifeSkill(NDTransData& data);
		void processCollection(NDTransData& data);
		//void processLifeSkillSynthesize(NDTransData& data);
		void processNPCInfo(NDTransData& data);
		
		void processRehearse(NDTransData& data);
		void processTeam(NDTransData& data);
		void processGoodFriend(NDTransData& data);
		void processTutor(NDTransData& data);
		
		void processGameQuit(NDTransData* data, int len);
		void processTalk(NDTransData& data);
		
		void processTrade(NDTransData& data);
		
		void processBillBoard(NDTransData& data);
		void processBillBoardField(NDTransData& data);
		void processBillBoardList(NDTransData& data);
		void processBillBoardUser(NDTransData& data);
		
		void processMsgCommonList(NDTransData& data);
		
		void processMsgCommonListRecord(NDTransData& data);
		
		void processShopInfo(NDTransData& data);
		void processShop(NDTransData& data);
		
		void processUserInfoSee(NDTransData& data);
		void processEquipInfo(NDTransData& data);
		
		void processEquipImprove(NDTransData& data);
		void processFormula(NDTransData& data);
		
		void processSkillGoods(NDTransData& data);
		void processMsgSkill(NDTransData& data);
		void processMsgPetSkill(NDTransData& data);
		// �������
		void processSyndicate(NDTransData& data);
		
		void processDigout(NDTransData& data);
		
		void processPlayerLevelUp(NDTransData& data);
		void processNPC(NDTransData& data);
		void processSee(NDTransData& data);
		void processCampStorage(NDTransData& data);
		void processNpcPosition(NDTransData& data);
		void processNpcTalk(NDTransData& data);
		void processItemTypeInfo(NDTransData& data);
		void processWish(NDTransData& data);
		void processCompetition(NDTransData& data);
		void processWalkTo(NDTransData& data);
		
		void processReCharge(NDTransData& data);
		void processRechargeReturn(NDTransData& data);
		
		void processVersionMsg(NDTransData& data);

		void processActivity(NDTransData& data);
		
		void processDeleteRole(NDTransData& data);
		
		void processPortal(NDTransData& data);
		
		void processMarriage(NDTransData& data);
		
		void processRespondTreasureHuntProb(NDTransData& data);
		
		void processShowTreasureHuntAward(NDTransData& data);
		
		void processRespondTreasureHuntInfo(NDTransData& data);
		
		void processKickOutTip(NDTransData& data);
		
		void processQueryPetSkill(NDTransData& data);
		
		void processRoadBlock(NDTransData& data);
		void processBossInfo(NDTransData& data);
		void processBossSelfInfo(NDTransData& data);
		
		void OnMsgNpcList(NDTransData* data);
		void OnMsgMonsterList(NDTransData* data);
		void LoadMapMusic();
		// ��ͼ��ز���
		bool loadSceneByMapDocID(int mapid);
		void LoadSceneMonster();
		CSMBattleScene* loadBattleSceneByMapID(int mapid,int x,int y);
		NDMapLayer* getMapLayerOfScene(NDScene* scene);
		NDMapLayer* getBattleMapLayerOfScene(NDScene* scene);
		NDUILayer* getUILayerOfRunningScene(int iTag);
		int GetMapDocID(){ return m_iMapDocID; }
		int GetMotherMapID();
		int GetMapID(){return m_mapID;}
		// npc��ز���
		void setNpcTaskStateById(int npcId, int state);
		void DelNpc(int iID);
		void ClearNpc();
		void AddAllNpcToMap();
		void AddOneNPC(NDNpc *npc);
		NDNpc* GetNpc(int iID);
		void UpdateNpcLookface(unsigned int idNpc, unsigned int newLookface);
		
		// ������ز���
		void ClearMonster();
		void ClearOneMonster(NDMonster* monster);
		void AddAllMonsterToMap();
		bool GetMonsterInfo(monster_type_info& info, int nType);
		//void ClearOneGP(GatherPoint* gp);
		//void ClearGP();
		NDMonster* GetMonster(int iID);
		void AddOneMonster(NDMonster* monster);
		
		// ս�����
		void BattleStart();
		void BattleEnd(int iResult);
		
		// ���������ز���
		NDManualRole* GetManualRole(int nID);
		NDManualRole* GetManualRole(const char* name);
		void AddManualRole(int nID, NDManualRole* role);
		void DelManualRole(int nID);
		void ClearManualRole();
		map_manualrole& GetManualRoles() { return m_mapManualrole; }
		
		NDManualRole* NearestDacoityManualrole(NDManualRole& role, int iDis);
		
		NDManualRole* NearestBattleFieldManualrole(NDManualRole& role, int iDis);
		
		// ���շ�����·�����Ʒ������Ϣ
		void OnMsgItemType(NDTransData* data);
		
		// ��������
		//LifeSkill* getLifeSkill(OBJID idSkill);
		//FormulaMaterialData*  getFormulaData(int idFoumula);
		//void getFormulaBySkill(int idSkill, std::vector<int>& vec_id);
		NDMonster* GetBoss();
		// �������ĳ�����뷶Χ�����������Ľ�ɫ��NPC
		NDBaseRole* GetRoleNearstPlayer(int iDistance);
		
		// �������ĳ�����뷶Χ����һ��Ŀ��
		NDBaseRole* GetNextTarget(int iDistance);
		
		int getDistBetweenRole(NDBaseRole *firstrole, NDBaseRole *secondrole);
		
		void ClearNPCChat();
		
		void quitGame();
		
		/**
		 * ���ͼ���л���ͼ
		 */
		void WorldMapSwitch(int mapId);
		
		/**
		 * ʹ�þ����л���ͼ
		 */
		void throughMap(int mapX, int mapY, int mapId); 
		
		void NavigateTo(int mapX, int mapY, int mapId); 
		
		void NavigateToNpc(int nNpcId);
		
		//�����б����
		//std::vector<RequsetInfo>& GetRequestList() { return m_vecRequest; }
		//bool GetRequest(int iID, RequsetInfo& info);
		//bool DelRequest(int iID);
		//void addRequst(RequsetInfo& request);
		
		// �������
		void resetTeamPosition(int leadId, int roleId);
		void resetTeamPosition(int teamID);
		NDManualRole* GetTeamRole(int iID);
		void setManualroleTeamID(int iRoleID, int iTeamID);
		bool GetTeamInfo(int iTeamID, s_team_info& info);
		void updateTeamCamp();
		void updateTeamListAddPlayer(NDManualRole& role);
		void updateTeamListDelPlayer(NDManualRole& role);
		std::vector<s_team_info>& GetTeamList(){ return m_vecTeamList; }
		
		std::vector<NDManualRole*> GetPlayerTeamList();		
		
		NDManualRole* GetTeamLeader(int teamID);
		NDManualRole* GetTeamRole(int teamID, int index);
		
		bool DealBugReport();
		bool DealCrashRepotUploadFail();
	public:
		std::string changeNpcString(std::string str);
		int GetCurrentMonsterRound(){return m_nCurrentMonsterRound;}
		// �������
		bool isFriendMax() {
			return true;//this->m_mapFriend.size() >= MAX_FRIEND_NUM;
		}
		
		bool isFriendAdded(string& name);
		
		int GetFriendOnlineNum() const {
			return this->onlineNum;
		}
		
		int GetFriendNum() const {
			return 0;//this->m_mapFriend.size();
		}
		
		bool isMonsterClear();
		
		//MAP_FRIEND_ELEMENT& GetFriendMap() {
		//	return this->m_mapFriend;
		//}
		
		bool canChangeMap() {
			return (this->mapType & MAPTYPE_CHGMAP_DISABLE) == 0;
		}
		
		bool canRecord() {
			return (this->mapType & MAPTYPE_RECORD_DISABLE) == 0;
		}
		
		bool canPk() {
			return (this->mapType & MAPTYPE_PK_DISABLE) == 0;
		}
		
		bool canBooth() {
			return (this->mapType & MAPTYPE_BOOTH_ENABLE) > 0;
		}
		
		bool canBattle() {
			return (this->mapType & MAPTYPE_BATTLE_DISABLE) == 0;
		}
		
		bool canFly() {
			return (this->mapType & MAPTYPE_FLY_DISABLE) == 0 && canDrive();
		}
		
		bool canDrive() {
			return (this->mapType & MAPTYPE_DRIVE_DISABLE) == 0;
		}
		
		bool canTreasureHunt() {
			return (this->mapType & MAPTYPE_NO_TREASURE_HUNT) == 0;
		}
		
		const VEC_NPC& GetNpc() const {
			return this->m_vNpc;
		}
		
		NDNpc* GetNpcByID(int idNpc);
		
		int GetDlgNpcID();
		
		void AddSwitch();
		
	// �ʼ����
	EmailData* GetMail(int iMailID);
	public:
		void SetBattleMonster(NDMonster* monster);
		NDMonster* GetBattleMonster();
	private:
		CAutoLink<NDMonster>	waitbatteleMonster;	
	public:
		VEC_NPC m_vNpc;				// ��ǰ��ͼ��ʾ������npc
		VEC_MONSTER m_vMonster;		// ��ǰ��ͼ��ʾ�����й�
		monster_info m_mapMonsterInfo; // ��ǰ��ͼ���й�����Ϣ
		map_manualrole m_mapManualrole;// �����������
		vec_email m_vEmail;
		//map_gather_point m_mapGP;
		map_npc_store m_mapNpcStore;
		
		/**npc�������*/
		USHORT usData;
		string strLeaveMsg;
		string strTitle;
		string strNPCText;
		string noteTitle; // ����
		string noteContent;
		struct st_npc_op
		{
			int idx; string str; bool bArrow;
			st_npc_op() { bArrow = false; }
		};
		vector<st_npc_op> vecNPCOPText;
		/**npc�������end*/

		//MAP_LEARNED_LIFE_SKILL m_mapLearnedLifeSkill; // ���ѧ��������
		//
		//map_fomula	m_mapFomula; //�䷽
		
		int zhengYing[CAMP_TYPE_END];
		
	public:
		static bool bFirstCreate, bVerifyVersion;
		int m_iMapDocID;//��ͼ��ԴID
		int m_mapID;//��ͼʵ��ID
		CCSize m_sizeMap;
		std::string mapName;
	private:
		int m_nCurrentMonsterRound;
		//std::vector<RequsetInfo> m_vecRequest;
		CIDFactory m_idAlloc;
		
		//���������
		typedef std::vector<s_team_info>::iterator	vec_team_it;
		std::vector<s_team_info> m_vecTeamList;
		
		// �����б�
		//MAP_FRIEND_ELEMENT m_mapFriend;
		int onlineNum;
		
		// �����̵�
		typedef map<int/*idNpc*/, VEC_BATTLE_SKILL> MAP_NPC_SKILL_STORE;
		typedef MAP_NPC_SKILL_STORE::iterator MAP_NPC_SKILL_STORE_IT;
		
		MAP_NPC_SKILL_STORE m_mapNpcSkillStore;
		
		int mapType;
		
		int m_iCurDlgNpcID;
	public:
		bool bolEnableAccept; //�������
		bool bRootItemZhangKai;
		bool bRootMenuZhangKai;
	private:
		int m_idTradeRole;
		OBJID m_idTradeDlg;
		OBJID m_idAuctionDlg;
		OBJID m_idDeMarry;
		OBJID m_idDialogDemarry;
		OBJID m_idDialogMarry;
		
		int id1, id2; // ��������id;
	};
	
	void sendPKAction(NDManualRole& role, int pkType);
	void trade(int n, int action);
	void sendAddFriend(std::string& name);
	void sendRehearseAction(int idTarget, int btAction);
	void sendQueryPlayer(int roleId, int btAction);
	void sendTeamAction(int senderID, int action);
	void sendChargeInfo(int iID);
	void sendMarry(int roleId, int playerId, int usType, int usData);
}
