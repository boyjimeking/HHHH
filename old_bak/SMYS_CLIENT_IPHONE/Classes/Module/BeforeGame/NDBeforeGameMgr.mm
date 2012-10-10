/*
 *  NDBeforeGameMgr.mm
 *  DragonDrive
 *
 *  Created by jhzheng on 11-1-7.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#import "NDBeforeGameMgr.h"
#import "NDDataTransThread.h"
#import "NDMsgDefine.h"
#import "NDMapMgr.h"
#import "CreateRoleScene.h"
#import "MD5checksum.h"
#import "WjcDes.h"
#import "InitMenuScene.h"
#import "NDDirector.h"
#import "GameSceneLoading.h"
#import "RegisterAccountScene.h"
#import "LoginScene.h"
#import "NDSocket.h"
#import "NDTransData.h"
#import "NDDirector.h"
#import "ServerListScene.h"
#import "NDUIDialog.h"
#import "NDUISynLayer.h"
#include "RobotScene.h"
#include "UpdateScene.h"
#include "XMLReader.h"
#include "cpLog.h"
#include "NDUtility.h"
#include <sstream>

#ifdef USE_NDSDK
//// sdk相关
#define ENABLE_NDCP_CUSTOM_LOGIN_EXTENT 1
#import <NdComPlatform/NdComPlatform+CustomLoginExtent.h>
#endif

using namespace NDEngine;

#define DES_KEY "n7=7=7d" //密钥
#define DOMAIN_NAME	"panda.sj.91.com"

// 前置机第一选择
#define AREA_IP_1 "121.207.255.120"
#define AREA_PORT_1 5818

// 前置机第二选择
#define AREA_IP_2 "221.179.216.113"
#define AREA_PORT_2 30003

#define TAG_DLG_UPDATE 12

#define TAG_DLG_UPDATE_ERR (13)

#define TIMER_CHECK_VERSION_TAG (12345678)
#define TIMER_CHECK_VERSION (4*3600)

/////////////////////////////////////////////////////////////////////
// 帐号注册
@interface NDRegisterAccount : NSObject
{
	NSMutableData *recvdata;
	int m_iType;				// 0:注册 1:快速注册 2:快速游戏
}

- (void)connectFast:(int)iType;

- (void)connectUserName:(const char*)username andPassWord:(const char*)password;

- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response;

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error;

- (void)connectionDidFinishLoading:(NSURLConnection *)connection;

- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data;

@end

@implementation NDRegisterAccount

- (void)connectFast:(int)iType
{
	m_iType = iType;
	
	if (recvdata) 
	{
		[recvdata release];
		recvdata = nil;
	}
	
	recvdata = [[NSMutableData alloc] init];
	
	NSString *nsUrl = [NSString stringWithFormat:@"http://%s:80/Service/GetResourceData.aspx?mt=5&qt=6044",DOMAIN_NAME];
	
	NSURL *url = [NSURL URLWithString:nsUrl];
	if (!url) 
	{
		NDLog(@"url不合法");
		return;
	}
	NSURLRequest *request = [[NSURLRequest alloc] initWithURL:url];
	NSURLConnection *connection = [[NSURLConnection alloc] initWithRequest:request delegate:self];
	
	[connection release];
	[request release];	
}

- (void)connectUserName:(const char*)username andPassWord:(const char*)password
{
	m_iType = 0;
	
	if (recvdata) 
	{
		[recvdata release];
	}
	
	recvdata = [[NSMutableData alloc] init];
	
	if ( strlen(username) == 0 || strlen(password) == 0 ) 
	{
		return;
	}
	
	// 帐号名
	NSString *nsUserName = [NSString stringWithCString:username encoding:NSUTF8StringEncoding];
	
	// 密码
	char* tmpBufPassword = EncryptPwd((char*)password,(char*)DES_KEY);	
	NSString *nsPassword = [NSString stringWithFormat:@"%s", tmpBufPassword];
	free(tmpBufPassword);
	
	// 时间戳+随机数
	srandom(time(NULL));
	unsigned int timestamp = (unsigned int)([NSDate timeIntervalSinceReferenceDate]) + (random()%65535 +1);
	NSString *nsTS = [NSString stringWithFormat:@"%u",timestamp];
	
	// 时间戳+随机数+固定字符串"!!)@)^@$"的md5校验码
	unsigned char md5TimeStamp[256] = {0};
	memcpy(md5TimeStamp, [nsTS UTF8String], [nsTS length]);
	memcpy(md5TimeStamp+[nsTS length], "!!)@)^@$", strlen("!!)@)^@$"));
	Encrypt::CMD5ChecksumStandard md5;
	std::string strText = md5.EncryptEx((unsigned char*)md5TimeStamp, [nsTS length]+strlen("!!)@)^@$"));
	NSString *nsTimeStamp = [NSString stringWithFormat:@"%s", strText.c_str()];
	
	// 待注册的URL
	NSString *nsUrl = [NSString stringWithFormat:@"http://%s:80/Service/GetResourceData.aspx?mt=5&qt=604&username=%s&password=%s&ts=%s&sign=%s",
					   DOMAIN_NAME, [nsUserName UTF8String], [nsPassword UTF8String], [nsTS UTF8String], [nsTimeStamp UTF8String]];

	NSURL *url = [NSURL URLWithString:nsUrl];
	if (!url) 
	{
		NDLog(@"url不合法");
		return;
	}
	NSURLRequest *request = [[NSURLRequest alloc] initWithURL:url];
	NSURLConnection *connection = [[NSURLConnection alloc] initWithRequest:request delegate:self];
	
	[connection release];
	[request release];	
}

- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response
{
	NSHTTPURLResponse *HTTPresponse = (NSHTTPURLResponse *)response;  
	NSInteger statusCode = [HTTPresponse statusCode];  
	if ( 404 == statusCode || 500 == statusCode ) // Not Found , Internal Server Error
	{
		if (m_iType == 0) 
		{
			NDBeforeGameMgrObj.RegiserCallBack(3, NDCommonCString("CheckNet"));
		}
		else 
		{
			NDBeforeGameMgrObj.FastCallBack(3,"", m_iType, NDCommonCString("CheckNet"));
		}

	}
}

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
{
	NDLog(@"%@", [error localizedDescription]);
	//　断开的网络
	if (m_iType == 0) 
	{
		NDBeforeGameMgrObj.RegiserCallBack(3, NDCommonCString("CheckNet"));
	}
	else 
	{
		NDBeforeGameMgrObj.FastCallBack(3,"", m_iType, NDCommonCString("CheckNet"));
	}
}


- (void)connectionDidFinishLoading:(NSURLConnection *)connection
{
	if (!recvdata) 
		return;
	
	NSString *recvString = [[NSString alloc] initWithData:recvdata encoding:NSUTF8StringEncoding];
	NSString *findString = [NSString stringWithFormat:@"%s", "<username>"];
	NSString *findEndString = [NSString stringWithFormat:@"%s", "</username>"];
	NSString *errStartString = [NSString stringWithFormat:@"%s", "<error>"];
	NSString *errEndString = [NSString stringWithFormat:@"%s", "</error>"];
	
	if ( [recvString rangeOfString:errStartString].length > 0 )
	{
		//注册失败
		if ( [recvString rangeOfString:errEndString].length > 0 ) 
		{
			NSRange range;
			range.location = [recvString rangeOfString:errStartString].location+7;
			range.length   = [recvString rangeOfString:errEndString].location-range.location;
			NSString *nsErr = [recvString substringWithRange:range];
			NDLog(@"%@", nsErr);
			if (m_iType == 0) 
			{
				NDBeforeGameMgrObj.RegiserCallBack(2, [nsErr UTF8String]);
			}
			else 
			{
				NDBeforeGameMgrObj.FastCallBack(2,"",m_iType, [nsErr UTF8String]);
			}
		}
		else 
		{
			NDLog(@"用户注册失败");
			if (m_iType == 0) 
			{
				NDBeforeGameMgrObj.RegiserCallBack(2, NDCommonCString("RegisterFail"));
			}
			else 
			{
				NDBeforeGameMgrObj.FastCallBack(2,"",m_iType, NDCommonCString("RegisterFail"));
			}
		}

	}
	else if ( [recvString rangeOfString:findString options:NSCaseInsensitiveSearch].length > 0 )
	{
		//注册成功
		NDLog(@"用户注册成功");
		NSRange range;
		range.location = [recvString rangeOfString:findString options:NSCaseInsensitiveSearch].location+10;
		range.length   = [recvString rangeOfString:findEndString options:NSCaseInsensitiveSearch].location-range.location;
		NSString *nsUsname = [recvString substringWithRange:range];
		if (m_iType == 0) 
		{
			NDBeforeGameMgrObj.RegiserCallBack(1, "");
		}
		else 
		{
			NDBeforeGameMgrObj.FastCallBack(1,std::string([nsUsname UTF8String]),m_iType, "");
		}
	}
	else 
	{
		NDLog(@"用户注册失败");
		if (m_iType == 0) 
		{
			NDBeforeGameMgrObj.RegiserCallBack(2, NDCommonCString("RegisterFail"));
		}
		else 
		{
			NDBeforeGameMgrObj.FastCallBack(2,"",m_iType, NDCommonCString("RegisterFail"));
		}
	}


	[recvString release];
	[recvdata release];
	recvdata = NULL;
}

- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
{
	[recvdata appendData:data];
}

@end
// 帐号注册　end
/////////////////////////////////////////////////////////////////////


static NDRegisterAccount	*ndRegisterAccount = NULL;
//static std::string servername = "lyol4";//"server02";//"Server06";//

NDBeforeGameMgr::NDBeforeGameMgr()
{
	NDNetMsgPoolObj.RegMsg(MB_LOGINSYSTEM_EXCHANG_KEY, this);
	//NDNetMsgPoolObj.RegMsg(MB_LOGINSYSTEM_MOBILE_SERVER_NOTIFY, this);
	NDNetMsgPoolObj.RegMsg(MB_SERVER_INFO_REQUEST, this);
	//NDNetMsgPoolObj.RegMsg(_MSG_NOTIFY_CLIENT, this);
	//NDNetMsgPoolObj.RegMsg(_MSG_MPF_VERSION, this);
	
	ndRegisterAccount = [[NDRegisterAccount alloc] init];
#if USE_ROBOT == 0
	NDDataPersist loginData;
	const char* ip =  loginData.GetData(kLoginData, kLastServerIP);
	if (ip) 
	{
		m_serverIP = ip;
	}
	else
	{
		m_serverIP = "";
	}
	const char* diplayName =  loginData.GetData(kLoginData, kLastServerName);
	if (diplayName) 
	{
		m_serverDisplayName = diplayName;
	}
	else
	{
		m_serverDisplayName = "";
	}
	const char* sendName =  loginData.GetData(kLoginData, kLastServerSendName);
	if (sendName) 
	{
		m_serverName = sendName;
	}
	else
	{
		m_serverName = "";
	}
	
	const char* port =  loginData.GetData(kLoginData, kLastServerPort);
	string strPort;
	if (port) 
	{
		strPort = port;
	}
	else
	{
		strPort = "";
	}
	if (!strPort.empty())
	{
		m_serverPort = atoi(strPort.c_str());
	}
	else 
	{
		m_serverPort = -1;
	}
#endif	
	m_LoginState = eLS_Login;
	
	m_dlgWait = NULL;
	
	m_uiDlgCheckFail = -1;
	
	m_httpCheckVersion = nil;
	
	//m_bNeedCheck = true;
	
	//m_timerCheckVersion = new NDTimer;
	m_timerCheckVersion = NULL;
	
	m_bInSwithCMNetState = false;
	
	m_sdkLogin = nil;
}

NDBeforeGameMgr::~NDBeforeGameMgr()
{
	if (ndRegisterAccount) 
	{
		[ndRegisterAccount release];
	}
	
	if (m_httpCheckVersion) 
	{
		[m_httpCheckVersion release];
	}
	
	if (m_timerCheckVersion) 
	{
		m_timerCheckVersion->KillTimer(this, TIMER_CHECK_VERSION_TAG);
		delete m_timerCheckVersion;
	}
	
	if (m_sdkLogin)
	{
		[m_sdkLogin release];
		m_sdkLogin = nil;
	}
}

bool NDBeforeGameMgr::Load()
{
	return DownLoadServerList();
}

void NDBeforeGameMgr::SetServerInfo(const char* serverIP, const char* serverName, const char* serverSendName, int serverPort)
{
	if (serverIP)
	{
		this->m_serverIP = serverIP;
	}
	
	if (serverName)
	{
		this->m_serverName = serverSendName;
	}
	
	if (serverSendName) {
		this->m_serverDisplayName = serverName;
	}
	this->m_serverPort = serverPort;
}

void NDBeforeGameMgr::generateClientKey() 
{
	// ------ 产生随机key ------
	srandom(time(NULL));
	NSString *sb = [NSString stringWithFormat:@"%d",(int)[NSDate timeIntervalSinceReferenceDate]]; 
	NSMutableString *str = [[NSMutableString alloc] init];
	[str appendString:sb];
	for (int i = [str length]; i < 24; i++) { // 产生 a-z的随机串
		NSString *sb = [NSString stringWithFormat:@"%c", (char) ((random()%100 +1) % 26 + 97)];
		[str appendString:sb];
	}
	phoneKey.clear();
	for (int i = 0; i < 24; i++)
	{
		unichar c = [str characterAtIndex:i];
		phoneKey.push_back((char)c);
	}	
	[str release];
}

/** 交换密钥 */
void NDBeforeGameMgr::sendClientKey() 
{
	NDTransData data(MB_LOGINSYSTEM_NORSA_EXCHANGE_KEY);
	
	int version = VERSION_IPHONE;
	data << version;
	data.Write((unsigned char*)(phoneKey.c_str()), phoneKey.size());
	NDDataTransThread::DefaultThread()->GetSocket()->Send(&data);
}

void NDBeforeGameMgr::RegisterAccout( string username, string password )
{
	if ( username.empty() || password.empty() ) 
	{
		return;
	}
	
	SetUserName(username);
	SetPassWord(password);
	
	[ndRegisterAccount connectUserName:username.c_str() andPassWord:password.c_str()];
}

void NDBeforeGameMgr::RegiserCallBack( int errCode, std::string strtip  )/* errCode=(1,注册成功),(2,注册失败),(3,网络问题)*/
{
	CloseProgressBar;
	
	switch (errCode) 
	{
		case 1:
		{ // 注册成功
			
			NDDataPersist loginData;
			
			loginData.SetData(kLoginData, kLastAccountName, username.c_str());			
			loginData.SetData(kLoginData, kLastAccountPwd, password.c_str());			
			loginData.SaveLoginData();
			loginData.AddAcount(username.c_str(), password.c_str());
			
			loginData.SaveAccountList();
			
			NDDirector::DefaultDirector()->ReplaceScene(ServerListScene::Scene(), true);
			m_LoginState = eLS_Register;
			return;
		}
		break;
		case 2:
		{ // 注册失败
			
		}
		break;
		case 3:
		{ // 网络问题
			
		}
			break;	
		default:
			break;
	}
	SetUserName("");
	SetPassWord("");
	
	showDialog(NDCommonCString("tip"), strtip.c_str());
	
	//NDDirector::DefaultDirector()->ReplaceScene(RegisterAccountScene::Scene());
}

void NDBeforeGameMgr::FastGameOrRegister(int iType)
{
	[ndRegisterAccount connectFast:iType];
}
void NDBeforeGameMgr::FastCallBack(int errCode, string username, int iType, std::string strtip)
{
	CloseProgressBar;
	switch (errCode) 
	{
		case 1:
		{ // 注册成功
			if ( iType == 1) 
			{ //快速注册
				// 弹出确定对话框
				//转到注册帐号界面
				//RegisterAccountScene *scene = RegisterAccountScene::Scene();
//				scene->SetAccountText(username.c_str());
//				NDDirector::DefaultDirector()->ReplaceScene(scene);
				
				SetUserName(username);
				NDScene *scene = NDDirector::DefaultDirector()->GetRunningScene();
				if (scene->IsKindOfClass(RUNTIME_CLASS(InitMenuScene))) 
				{
					InitMenuScene *menuscene = (InitMenuScene *)scene;
					menuscene->FastGameOrRegisterTip(1);
				}
			}
			else 
			{ // 快速游戏
				// 弹出确定对话框
				//转到注册帐号界面
				//RegisterAccountScene *scene = RegisterAccountScene::Scene();
//				scene->SetAccountText(username.c_str());
//				scene->SetPasswordText(password.c_str());
//				NDDirector::DefaultDirector()->ReplaceScene(scene);
				
				SetUserName(username);
				SetPassWord(username);
				NDScene *scene = NDDirector::DefaultDirector()->GetRunningScene();
				if (scene->IsKindOfClass(RUNTIME_CLASS(InitMenuScene))) 
				{
					InitMenuScene *menuscene = (InitMenuScene *)scene;
					menuscene->FastGameOrRegisterTip(2);
				}
				
			}

			return;
		}
			break;
		case 2:
		{ // 注册失败
			
		}
			break;
		case 3:
		{ // 网络问题
			
		}
			break;	
		default:
			break;
	}
	
	showDialog(NDCommonCString("tip"), strtip.c_str());
	
}

bool NDBeforeGameMgr::ConnectServer(const char* ip, unsigned int port, bool wapFlag, bool switchNet/*=false*/)
{
	if (!ip) return false;
	
	NDSocket socServerList;
	// 套接字初始化,并连接
	
	if (!socServerList.Connect(ip, port, true) ) 
	//if (!socServerList.Connect("192.168.55.58", 8888, true) ) // test
	{
		//#ifdef DEBUG
		//		NDLog(@"连接服务器列表失败");
		//		return false;
		//#else
	
//		if ( !socServerList.Connect(AREA_IP_2, AREA_PORT_2, true)) 
//		{
//			NDLog(@"AREA_IP_2..连接服务器列表失败");
//			return false;
//		}
		//#endif
		
		NDLog(@"连接服务器列表失败,ip:%s, port:%d", ip, port);
		
		return false;
	}
	
	NDTransData data(MB_SERVER_INFO_REQUEST);
	//data << (MSGID)MB_SERVER_INFO_REQUEST;
	data << (unsigned short)(!wapFlag);
	socServerList.Send(&data);
	
	int iMsgLen = 0;
	
	do 
	{
		// 先读4个字节
		unsigned char buf[4]={0};
		unsigned char* ptr = buf;
		int iLen = 0;
		if ( -1 == socServerList.Receive(ptr, iLen, 4) || iLen != 4 )
		{
			NDLog(@"获取服务器列表读消息头出错");
			socServerList.Close();
			return false;
		}
		
		// 校验
		if ((unsigned char)0xff == buf[0] && (unsigned char)0xfe == buf[1]) 
		{
		}else 
		{
			// 校验不对,扔吧
			NDLog(@"获取服务器列表校验不对");
			socServerList.Close();
			return false;
		}
		
		iMsgLen  = (buf[2] & 0xff) + ((buf[3] & 0xff) << 8);
		
		if (iMsgLen < 0 || iMsgLen >= 1046) 
		{
			NDLog(@"获取服务器列表消息长度不对");
			socServerList.Close();
			return false;
		}
	} while (0);
	
	do 
	{
		unsigned char buf[1046]={0};
		unsigned char* ptr = buf;
		int iLen=0;
		if ( -1 == socServerList.Receive(ptr, iLen, iMsgLen-4) || iLen != iMsgLen-4 )
		{
			NDLog(@"获取服务器列表读消息体出错");
			socServerList.Close();
			return false;
		}
		
		m_bInSwithCMNetState = switchNet;
		
		//处理消息了
		NDTransData recvdata;
		recvdata.Write(buf, iMsgLen-4);
		int nMsgID  = recvdata.ReadShort();
		NDNetMsgPoolObj.Process(nMsgID, &recvdata, iMsgLen-6);
		
		socServerList.Close();
		
	} while (0);
	
	//CloseProgressBar;
	return true;
}

bool NDBeforeGameMgr::DownLoadServerList(bool switchNet/*=false*/)
{
	std::string server;
	int iPort;
#if USE_ROBOT == 0
	NDDataPersist loginData;
	std::string strLinkType(loginData.GetData(kLoginData, kLinkType));
	
	if (strLinkType == "i_dd") {
		server = "121.207.255.120";
		iPort = 5877;
	}else if (strLinkType == "o" || strLinkType == ""){
		server = AREA_IP_1;
		iPort = AREA_PORT_1;
		
		if (strLinkType == "") {
			loginData.SetData(kLoginData, kLinkType, "o");
			loginData.SaveLoginData();
		}
	}else {
		return false;
	}
#else
	NDScene *scene = NDDirector::DefaultDirector()->GetScene(RUNTIME_CLASS(RobotScene));
	if (!scene) return false;
	if (((RobotScene*)scene)->IsOuterNet()) 
	{
		server = AREA_IP_1;
		iPort = AREA_PORT_1;
	}
	else
	{
		server = "121.207.255.120";
		iPort = 5877;
	}
#endif
	
	//const char* server = "117.135.145.134";
//	const int iPort = 5051;

	if (!ConnectServer(server.c_str(), iPort, false, switchNet))
		return ConnectServer(AREA_IP_2, AREA_PORT_2, true, switchNet);

	return true;
}

/*处理感兴趣的网络消息*/
bool NDBeforeGameMgr::process(MSGID msgID, NDTransData* data, int len)
{ 
	switch (msgID) 
	{
		case MB_LOGINSYSTEM_EXCHANG_KEY:
			processLogin(data, len);
			break;
		case MB_LOGINSYSTEM_MOBILE_SERVER_NOTIFY:
			//processNotify(data, len);
			break;
		case MB_SERVER_INFO_REQUEST:
			processAcquireServerInfoRecieve(data, len);
			break;
		case _MSG_NOTIFY_CLIENT:
			//processNotifyClient(data, len);
			break;
		/*
		case _MSG_MPF_VERSION:
			processMPFVersionMsg(*data);
			break;
		*/
		default:
			break;
	}
	
	return true;
}

//////////////////////////////////////////////////////////////
//消息处理

void NDBeforeGameMgr::processLogin(NDTransData* data, int len)
{
	if (!data) return;
	
	if (!data->decrypt(phoneKey))
	{
		NDLog(@"解密失败,NDMsgLogin::Process");
		return;
	}
	
	len = data->GetSize()-6;
	
	srandom(time(NULL));
	int nRandNum = random()%16 +1; //1~16
	
	NSMutableString *str = [[NSMutableString alloc] init];
	for (int i = 1; i <= nRandNum; i++) { // 产生 a-z的随机串
		NSString *sb = [NSString stringWithFormat:@"%c", (char) ((random()%100 +1) % 26 + 97)];
		[str appendString:sb];
	}
	
	
	char buf[1024] = {0x00};
	data->Read((unsigned char*)buf, len);
	
	serverPhoneKey.clear();

	for (int i=0; i < len; i++) {
		serverPhoneKey.push_back(buf[i]);
	} 
	
	
	NDTransData SEND_DATA;
	
	SEND_DATA.WriteShort(MB_LOGINSYSTEM_LOGIN_GAME_SERVER);
	SEND_DATA.WriteUnicodeString(GetUserName());
	SEND_DATA.WriteUnicodeString(GetPassWord());
	SEND_DATA.WriteUnicodeString(GetServerName());
	NDLog(@"------本次的登陆服务器名:[%s]-----", GetServerName().c_str());
	NDLog(@"------本次的登陆用户名:[%s]-------", GetUserName().c_str());
	NDLog(@"------本次的登陆密码:[%s]---------", GetPassWord().c_str());
	//SEND_DATA.WriteUnicodeString(std::string("tcp0811"));
	//SEND_DATA.WriteUnicodeString(std::string("1"));
	//SEND_DATA.WriteUnicodeString(std::string("server06"));
	SEND_DATA.WriteShort([str length]);
	SEND_DATA.Write((unsigned char*)[str UTF8String], [str length]);
	
	[str release];
	
	if ( !SEND_DATA.encrypt(serverPhoneKey) )
	{
		NDLog(@"消息发送加密失败,NDMsgLogin::Process");
		return;
	}
	
	NDDataTransThread::DefaultThread()->GetSocket()->Send(&SEND_DATA);
}

void NDBeforeGameMgr::processNotify(NDTransData* data, int len)
{
	int action = data->ReadShort();
	int code = data->ReadShort();
	int count = data->ReadShort();
	
	std::stringstream ss;
	for (int i = 0; i < count; i++) {
		ss << data->ReadUnicodeString();
	}

	if (action == MB_LOGINSYSTEM_LOGIN_GAME_SERVER) 
	{
		std::string strTip = "", strTitle = "";
		
		bool bSucess = true;
		switch (code) 
		{
			case 0: // <帐号>服务器认证失败
				strTitle = NDCommonCString("RegisterFail");
				strTip += ss.str().empty() ? NDCommonCString("RenZhengFailNoneRet") : ss.str();
				bSucess = false;
				break;
			case 1: // 登录<游戏>服务器成功
				break;
			case 2: // 登录<游戏>服务器失败
			case 3: // 连接<帐号>服务器失败
			case 4: // 连接<游戏>服务器失败
				strTitle = NDCommonCString("ConnectFail");
				strTip += ss.str().empty() ? NDCommonCString("ConnectFailNoneRet") : ss.str();
				bSucess = false;
				break;
			case 5: // 登录<帐号>服务器超时
			case 6: // 登录<游戏>服务器超时
				strTitle = NDCommonCString("ConnectFail");
				strTip += NDCommonCString("ServerTimeOut");
				bSucess = false;
				break;
			case 7: // <代理服务器>繁忙
				strTitle = NDCommonCString("ConnectFail");
				strTip += NDCommonCString("ServerPlayerUpMax");
				bSucess = false;
				break;
			case 8: // <代理服务器>已达到人数上限
				strTitle = NDCommonCString("ConnectFail");
				strTip += NDCommonCString("ServerPlayerUpMax");
				bSucess = false;
				break;
			case 9: // 登录<游戏>服务器成功 无角色
				//NDDirector::DefaultDirector()->ReplaceScene(CreateRoleScene::Scene(), true);
                //this->CreateRole();
				break;
			case 10: // 继续登录需要等待,由玩家选择
			{
				m_dlgWait = new NDUIDialog;
				m_dlgWait->Initialization();
				m_dlgWait->SetDelegate(this);
				m_dlgWait->Show(NDCommonCString("LoginTip"), ss.str().empty() ? NDCommonCString("WaitServerDeal") : ss.str().c_str(), NDCommonCString("quit"), NDCommonCString("ContinueWait"), NULL);
			}
				break;
			default: // 登录<游戏>服务器失败
				NDLog(@"连接失败");//, str);
				strTitle = NDCommonCString("ConnectFail");
				strTip += NDCommonCString("ConnectFail");
				bSucess = false;
				break;
		}
		CloseProgressBar;        
		if (!bSucess) 
		{
			if (NDDataTransThread::DefaultThread()->GetThreadStatus() != ThreadStatusRunning)	
			{	
				NDDataTransThread::DefaultThread()->Start(NDBeforeGameMgrObj.GetServerIP().c_str(), NDBeforeGameMgrObj.GetServerPort());
			}
			
			if (NDDataTransThread::DefaultThread()->GetThreadStatus() != ThreadStatusRunning)	
			{	
				exit(0);
			}
			
			NDBeforeGameMgrObj.Login();
			return;
			
			if (!(IsInRegisterState() || IsInAccountListState()))
			{
				NDDirector::DefaultDirector()->ReplaceScene(LoginScene::Scene());
			}else 
			{
				NDScene *scene = NDDirector::DefaultDirector()->GetRunningScene();
				if (scene->IsKindOfClass(RUNTIME_CLASS(LoginScene)))
				{
					NDDirector::DefaultDirector()->ReplaceScene(LoginScene::Scene());
				}else
				{
					NDDirector::DefaultDirector()->ReplaceScene(InitMenuScene::Scene());
				}
			}

			NDUIDialog *dialog = new NDUIDialog;
			dialog->Initialization();
			dialog->Show(strTitle.c_str(), strTip.c_str(), NULL,NULL);
		}
	}
}

void NDBeforeGameMgr::processAcquireServerInfoRecieve(NDTransData* data, int len)
{
	m_vecBigArea.clear();
	int size = 0; (*data) >> size;
	for (int i = 0; i < size; i++) 
	{	
		big_area area;
		std::string bigArea;
		bigArea = data->ReadUnicodeString(); //大区名(String)
		area.name = bigArea;
		
		std::string ips;
		ips = data->ReadUnicodeString(); //大区的ip地址
		area.ip = ips;
		
		int iPort = 0; (*data) >> iPort; //大区的ip端口
		area.iPort = iPort;
		int available = 0; (*data) >> available; //可用服务器个数(int)
		area.iLines = available;
		for (int j = 0; j < available; j++) 
		{
			big_area::line line;
			std::string serverNameDisplay = data->ReadUnicodeString(); //具体服务器名显示(String)
			line.displayName = serverNameDisplay;
		
		
			std::string serverNameTrans = data->ReadUnicodeString(); //具体服务器名传输(String)
			line.sendName = serverNameTrans;
			
			if (m_bInSwithCMNetState)
			{
				if (serverNameTrans == m_serverName)
				{
					m_serverIP = ips;
					
					m_serverPort = iPort;
					
					m_bInSwithCMNetState = false;
					
					return;
				}
			}
			
			unsigned char cState = 0; (*data) >> cState;
			line.iState = cState;
			area.vec_lines.push_back(line);
		}
		m_vecBigArea.push_back(area);
	}
	
	m_bInSwithCMNetState = false;
}

void NDBeforeGameMgr::processNotifyClient(NDTransData* data, int len)
{
	unsigned char ret = 0; (*data) >> ret;
	std::string strTip = "";
	switch (ret) 
	{ // 0：创建失败 1：创建成功 2：人物名已存在 3：人物名不合法
		case 0:
			strTip += NDCommonCString("RoleCreateFail");
			break;
		case 1:
			NDLog(@"创建成功");
			break;
		case 2:
			strTip += NDCommonCString("PlayerNameExist");
			break;
		case 3:
			strTip += NDCommonCString("PlayerNameInvalid");
			break;
		default:
			strTip += NDCommonCString("RoleCreateFailUnknown");
			break;
	}
	
	NDUIDialog *dialog = new NDUIDialog;
	dialog->Initialization();
	dialog->Show(NDCommonCString("tip"), strTip.c_str(), NULL,NULL);
	NDUISynLayer::Close(SYN_CREATE_ROLE);
}

/*
void NDBeforeGameMgr::processMPFVersionMsg(NDTransData& data)
{
	int verisonID = data.ReadByte();
	std::string strVersion = data.ReadUnicodeString();
	
	if (verisonID != VERSION_IPHONE)
	{
		NDDirector::DefaultDirector()->ReplaceScene(InitMenuScene::Scene(), true);
		return;
	}
	
	NDScene *scene = NDDirector::DefaultDirector()->GetRunningScene();
	
	if (strVersion == GetSoftVersion())
	{
		if (scene && scene->IsKindOfClass(RUNTIME_CLASS(GameSceneLoading)))
			((GameSceneLoading*)scene)->CheckVersionSucess();
		else
			NDDirector::DefaultDirector()->ReplaceScene(InitMenuScene::Scene(), true);
		return;
	}
	
	if (scene && scene->IsKindOfClass(RUNTIME_CLASS(GameSceneLoading)))
	{
		((GameSceneLoading*)scene)->UpdateTitle("检测版本更新...");
		CheckVersion();
	}
	else
		NDDirector::DefaultDirector()->ReplaceScene(InitMenuScene::Scene(), true);
	
}
*/

bool NDBeforeGameMgr::CanFastLogin()
{
	NDDataPersist loginData;
	string lastAccountName = loginData.GetData(kLoginData, kLastAccountName);
	string lastAccountPwd = loginData.GetData(kLoginData, kLastAccountPwd);
	string lastServerIP = loginData.GetData(kLoginData, kLastServerIP);
	string lastServerPort = loginData.GetData(kLoginData, kLastServerPort);
	string lastServerName = loginData.GetData(kLoginData, kLastServerName);
	string lastServerSendName = loginData.GetData(kLoginData, kLastServerSendName);
	
	if (!lastServerIP.empty() &&
	    !lastServerPort.empty() &&
	    !lastAccountPwd.empty() &&
	    !lastAccountName.empty() &&
	    !lastServerName.empty() &&
	    !lastServerSendName.empty()) {
		
		this->SetUserName(lastAccountName);
		this->SetPassWord(lastAccountPwd);
		this->SetServerInfo(lastServerIP.c_str(), lastServerName.c_str(), lastServerSendName.c_str(), atoi(lastServerPort.c_str()));
		
		return true;
	}
	
	return false;
}

void NDBeforeGameMgr::OnDialogClose(NDUIDialog* dialog)
{
	if (dialog == m_dlgWait) 
	{
		if (!(IsInRegisterState() || IsInAccountListState()))
		{
			NDDirector::DefaultDirector()->ReplaceScene(LoginScene::Scene());
		}else 
		{
			NDScene *scene = NDDirector::DefaultDirector()->GetRunningScene();
			if (scene->IsKindOfClass(RUNTIME_CLASS(LoginScene)))
			{
				NDDirector::DefaultDirector()->ReplaceScene(LoginScene::Scene());
			}else
			{
				NDDirector::DefaultDirector()->ReplaceScene(InitMenuScene::Scene());
			}
		}
		m_dlgWait = NULL;
	}
	else if (dialog->GetTag() == int(m_uiDlgCheckFail))
	{
		exit(0);
	}
	else if (dialog->GetTag() == TAG_DLG_UPDATE_ERR)
	{
		quitGame(false);
	}
}

void NDBeforeGameMgr::OnDialogButtonClick(NDUIDialog* dialog, unsigned int buttonIndex)
{
	if (dialog == m_dlgWait) 
	{
		NDScene *scene = NDDirector::DefaultDirector()->GetRunningScene();
		if (scene->IsKindOfClass(RUNTIME_CLASS(GameSceneLoading)))
		{
			((GameSceneLoading*)scene)->UpdateTitle(NDCommonCString("LoginQueueTip"));
		}
		dialog->Close();
	}
	else if (dialog->GetTag() == TAG_DLG_UPDATE_ERR) 
	{
		exit(0);
	}
	else if (dialog->GetTag() == TAG_DLG_UPDATE) 
	{
		if (buttonIndex == 0) 
		{
			/*
			char buff[100] = {0x00};
			sprintf(buff, "最新版本：%s", m_latestVersion.c_str());
			
			UpdateScene* scene = new UpdateScene();
			scene->Initialization(m_fileUrl.c_str(), buff);
			NDDirector::DefaultDirector()->PushScene(scene);
			*/
			
			bool res = [[UIApplication sharedApplication] openURL:
											 [NSURL URLWithString:
								   [NSString stringWithUTF8String:m_fileUrl.c_str()]]];
			
			if (!res) 
			{
				NDUIDialog *dlg = new NDUIDialog;
				dlg->Initialization();
				dlg->SetTag(TAG_DLG_UPDATE_ERR);
				dlg->Show(NDCommonCString("tip"), NDCommonCString("UpdateErr"), NULL, NULL);
			}
		}
		else 
		{
			exit(0);
		}
	}
}

void NDBeforeGameMgr::OnTimer(OBJID tag)
{
	/*
	if (tag == TIMER_CHECK_VERSION_TAG && m_timerCheckVersion) 
	{
		m_timerCheckVersion->KillTimer(this, TIMER_CHECK_VERSION_TAG);
		m_bNeedCheck = true;
	}
	*/
}

void NDBeforeGameMgr::OnRecvData(id http, char* data, unsigned int len)
{
	if (http != m_httpCheckVersion) return;
	
	bool res = true;
	XMLReader *reader = [[XMLReader alloc] initWithData:data length:len];
	int retStatus = [reader getIntWithPath:@"/result/resultState/code" andIndexs:NULL size:0];
	if (retStatus == 0) 
	{
		cpLog(LOG_DEBUG, "xml return success");
		m_fileUrl = [[reader getStringWithPath:[NSString stringWithUTF8String:"/result/data/filelist/file"] andIndexs:NULL size:0] UTF8String];
		
		NSString* latestVer = [reader getStringWithPath:[NSString stringWithUTF8String:"/result/data/version"] andIndexs:NULL size:0];
		NSString* localVersion = [NSString stringWithUTF8String:GetSoftVersion().c_str()];
		if ([latestVer isEqualToString:localVersion]) 
			res = true;
		else 
			res = false;
			
		m_latestVersion = [latestVer UTF8String];
	}
	
	[reader release];
	
	if (res != true) 
	{
		// update version
		/*
		char buff[100] = {0x00};
		sprintf(buff, "最新版本：%s，是否升级?", m_latestVersion.c_str());
		
		NDUIDialog* dlg = new NDUIDialog();
		dlg->Initialization();
		dlg->SetDelegate(this);
		dlg->SetTag(TAG_DLG_UPDATE);
		dlg->Show(NDCommonCString("tip"), buff, NULL, "确定","取消", NULL);
		*/	
		//NSString *softUrl = [NSString stringWithUTF8String:m_fileUrl.c_str()];
		
		/*
		NSString *softUrl = [NSString stringWithUTF8String:m_fileUrl.c_str()];
		static NSString *updatePrefix = @"itms-services://?action=download-manifest&url=";
		NSString *updateUrl = [softUrl hasPrefix:updatePrefix] ? softUrl : [NSString stringWithFormat:@"%@%@", updatePrefix, softUrl];
		NDLog(updateUrl);
		BOOL res = [[UIApplication sharedApplication] openURL:[NSURL URLWithString:updateUrl]];
		*/
		NDLog(@"%@", [NSString stringWithUTF8String:m_fileUrl.c_str()]);
		bool res = [[UIApplication sharedApplication] openURL:
					[NSURL URLWithString:
					 [NSString stringWithUTF8String:m_fileUrl.c_str()]]];
		
		if (!res) 
		{
			
			NDUIDialog *dlg = new NDUIDialog;
			dlg->Initialization();
			dlg->SetTag(TAG_DLG_UPDATE_ERR);
			dlg->Show(NDCommonCString("tip"), NDCommonCString("UpdateErr"), NULL, NULL);
		}
		else 
		{
			exit(0);
		}
	}
	else
	{
		// login
		/*
		NDScene* scene = NDDirector::DefaultDirector()->GetRunningScene();
		
		if (!scene) exit(0);
		
		if (scene->IsKindOfClass(RUNTIME_CLASS(GameSceneLoading)))
		{
			((GameSceneLoading*)scene)->CheckVersionSucess();
		}
		else
		{
			NDDirector::DefaultDirector()->ReplaceScene(InitMenuScene::Scene(), true);
		}
		*/
		
		quitGame(true);
	}

}

void NDBeforeGameMgr::OnRecvError(id http, NDHttpErrCode errCode)
{
	if (http != m_httpCheckVersion) return;
	
	CheckFail(errCode);
}

void NDBeforeGameMgr::CheckVersion()
{
	/*
	if (!m_bNeedCheck) 
	{
		Login();
		return;
	}
	*/
	
	//m_bNeedCheck = false;
	
	//if (m_timerCheckVersion) m_timerCheckVersion->SetTimer(this, TIMER_CHECK_VERSION_TAG, TIMER_CHECK_VERSION);
	
	if (m_httpCheckVersion) [m_httpCheckVersion release];
	
	m_httpCheckVersion = [[NDDefaultHttp alloc] init];
	
	[m_httpCheckVersion AysnSendRequst:[NSString stringWithUTF8String:GetUpdateUrl().c_str()] delegate:this];
}

bool NDBeforeGameMgr::IsCMNet()
{
	return m_serverIP == AREA_IP_1; 
}

bool NDBeforeGameMgr::SwitchToCMNet()
{
	if (IsCMNet()) return true;
		
	return ConnectServer(AREA_IP_1, AREA_PORT_1, false, true);
}

void NDBeforeGameMgr::Login()
{
	generateClientKey();
	sendClientKey();
	
	// 处理nd sdk login
	doNDSdkLogin();
}

void NDBeforeGameMgr::doNDSdkLogin()
{
#ifdef USE_NDSDK
	if (m_sdkLogin)
	{
		[m_sdkLogin release];
		m_sdkLogin = nil;
	}
	m_sdkLogin = [[NDSdkLogin alloc] init];
	[m_sdkLogin LoginWithUser:GetUserName().c_str() AndPassword:GetPassWord().c_str()];
#endif
}

void NDBeforeGameMgr::CheckFail(NDHttpErrCode errCode)
{
	NDDataTransThread::DefaultThread()->Stop();
	
	CloseProgressBar;
	
	/*
	if (NDHttpErrCodeNotFound == errCode) 
	{
		GlobalShowDlg(this, NDCommonCString("tip"), "更新时找不到资源,游戏将退出!", 3.0f);
	}
	else if (NDHttpErrCodeInternalServerError == errCode)
	{
		GlobalShowDlg(this, NDCommonCString("tip"), "更新时网络连接失败,请检查网络!", 3.0f);
	}
	else
	{
		GlobalShowDlg(this, NDCommonCString("tip"), "更新时出现未知错误!", 3.0f);
	}
	*/
	
	//NDDirector::DefaultDirector()->ReplaceScene(InitMenuScene::Scene(true), true);
	
	quitGame(true);
}

bool NDBeforeGameMgr::SynConnect()
{
	if (m_SynSocket.Connected()) 
	{
		return false;
	}
	/*
	//..test
	if (!m_SynSocket.Connect("192.168.55.58", 8888, true))
	{
		return false;
	}
	
	return true;
	//.. test end
	*/
	
	std::string server;
	int iPort;
	
	NDDataPersist loginData;
	std::string strLinkType(loginData.GetData(kLoginData, kLinkType));
	
	if (strLinkType == "i_dd") {
		server = "121.207.255.120";
		iPort = 5877;
	}else 
	{
		server = AREA_IP_1;
		iPort = AREA_PORT_1;
	}
	
	// 套接字初始化,并连接
	
	if (!m_SynSocket.Connect(server.c_str(), iPort, true))
	{
		if (!m_SynSocket.Connect(AREA_IP_2, AREA_PORT_2, true))
		{
			return false;
		}
	}
	
	return true;
}

bool NDBeforeGameMgr::SynSEND_DATA(NDTransData& data)
{
	if (!m_SynSocket.Connected()) 
	{
		return false;
	}
	
	m_SynSocket.Send(&data);
	
	return true;
}

bool NDBeforeGameMgr::SynProcessData()
{
	if (!m_SynSocket.Connected()) 
	{
		return false;
	}
	
	int iMsgLen = 0;
	
	do 
	{
		// 先读4个字节
		unsigned char buf[4]={0};
		unsigned char* ptr = buf;
		int iLen = 0;
		if ( -1 == m_SynSocket.Receive(ptr, iLen, 4) || iLen != 4 )
		{
			NDLog(@"获取服务器列表读消息头出错");
			SynConnectClose();
			return false;
		}
		
		// 校验
		if ((unsigned char)0xff == buf[0] && (unsigned char)0xfe == buf[1]) 
		{
		}else 
		{
			// 校验不对,扔吧
			NDLog(@"获取服务器列表校验不对");
			SynConnectClose();
			return false;
		}
		
		iMsgLen  = (buf[2] & 0xff) + ((buf[3] & 0xff) << 8);
		
		if (iMsgLen < 0 || iMsgLen >= 1046) 
		{
			NDLog(@"获取服务器列表消息长度不对");
			SynConnectClose();
			return false;
		}
	} while (0);
	
	do 
	{
		unsigned char buf[1046]={0};
		unsigned char* ptr = buf;
		int iLen=0;
		if ( -1 == m_SynSocket.Receive(ptr, iLen, iMsgLen-4) || iLen != iMsgLen-4 )
		{
			NDLog(@"获取服务器列表读消息体出错");
			SynConnectClose();
			return false;
		}
		

		
		//处理消息了
		NDTransData recvdata;
		recvdata.Write(buf, iMsgLen-4);
		int nMsgID  = recvdata.ReadShort();
		NDNetMsgPoolObj.Process(nMsgID, &recvdata, iMsgLen-6);
		
	} while (0);
	
	return true;
}

void NDBeforeGameMgr::SynConnectClose()
{
	if (m_SynSocket.Connected()) 
	{
		return;
	}
	
	m_SynSocket.Close();
}

void NDBeforeGameMgr::VerifyVesion()
{
	NDTransData bao(_MSG_MPF_VERSION);
	
	bao.WriteShort(VERSION_IPHONE);
	
	bao.WriteUnicodeString(GetSoftVersion());
	
	SEND_DATA(bao);
}

///////////////////////////////////////////////////////////////
bool 
NDBeforeGameMgr::SwichKeyToServer(const char* pszIp, int nPort,const char* pszAccountName,const char* pszPwd,const char* pszServerName)
{
    if(strlen(pszIp) < 6 || pszAccountName[0]==0x00 || pszServerName[0]==0x00){
        return false;
    }
    
    NDDataTransThread::DefaultThread()->Start(pszIp, nPort);
	if (NDDataTransThread::DefaultThread()->GetThreadStatus() != ThreadStatusRunning)	
	{
		return false;
	}
    
    this->SetUserName(pszAccountName);
    this->SetServerInfo(pszIp,pszServerName,pszServerName,nPort);
    this->SetPassWord(pszPwd);
    this->Login();
    //登陆成功纪录登陆信息
    NDDataPersist loginData;
    loginData.SetData(kLoginData, kLastAccountName, username.c_str());
    loginData.SetData(kLoginData, kLastAccountPwd, password.c_str());
    loginData.AddAcount(username.c_str(), password.c_str());
    loginData.SetData(kLoginData, kLastServerIP, NDBeforeGameMgrObj.GetServerIP().c_str());
    loginData.SetData(kLoginData, kLastServerName, NDBeforeGameMgrObj.GetServerDisplayName().c_str());	
    loginData.SetData(kLoginData, kLastServerSendName, NDBeforeGameMgrObj.GetServerName().c_str());	
    NSString* strPort = [NSString stringWithFormat:@"%d", NDBeforeGameMgrObj.GetServerPort()];
    loginData.SetData(kLoginData, kLastServerPort, [strPort UTF8String]);
    loginData.SaveLoginData();
    loginData.SaveAccountList();
    return true;
}

////////////////////////////////////////////////////////////
void 
NDBeforeGameMgr::SetRole(unsigned long ulLookFace, const char* pszRoleName, int nProfession)
{
    m_ulLookFace = ulLookFace;
    m_strRoleName= pszRoleName;
    m_nProfession= nProfession;
}

////////////////////////////////////////////////////////////
void
NDBeforeGameMgr::CreateRole(const char* pszName, Byte nProfession, int nLookFace, const char* pszAccountName)
{
    // 发送创建角色消息
    Byte camp = 0;
    Byte sex = 1;
    Byte model = 1;
    Byte hairColor = 1;
    Byte career = nProfession;
    int dwLook =nLookFace;
    NDTransData data(MB_LOGINSYSTEM_CREATE_NEWBIE);
    data << dwLook // 人物模型 4个字节
    << career // 职业 1个字节 // 1,2,3
    << camp; // 阵营 1个字节 // 1,2
    
    data.WriteUnicodeString(pszName);
    
    // 增加帐号名、手机机型、渠道号
    data.WriteUnicodeString(pszAccountName);
    data.WriteUnicodeString(platformString());
    data.WriteUnicodeString(loadPackInfo(STRPARAM));
    
    NDDataTransThread::DefaultThread()->GetSocket()->Send(&data);
}

///////////////////////////////////////////////////////////////
bool 
NDBeforeGameMgr::LoginByLastData()
{
    NDDataPersist loginData;
    return  this->SwichKeyToServer(loginData.GetData(kLoginData,kLastServerIP), ::atoi(loginData.GetData(kLoginData,kLastServerPort)),loginData.GetData(kLoginData,kLastAccountName),loginData.GetData(kLoginData,kLastAccountPwd)
        ,loginData.GetData(kLoginData,kLastServerName));
}

/////////////////////////////////////////////////////////////
int  
NDBeforeGameMgr::GetAccountListNum(void)
{
    NDDataPersist loginData;
    VEC_ACCOUNT setAccount;
    loginData.GetAccount(setAccount);
    return setAccount.size();
}

////////////////////////////////////////////////////////////
const char* 
NDBeforeGameMgr::GetRecAccountNameByIdx(int idx)
{
    NDDataPersist loginData;
    VEC_ACCOUNT vAccount;
    loginData.GetAccount(vAccount);
    int nIdx = 0;
    for (VEC_ACCOUNT::reverse_iterator it = vAccount.rbegin(); it != vAccount.rend(); it++,nIdx++) {
        if(nIdx == idx){
            return (*it).first.c_str();
        }
	}
    return NULL;
}

////////////////////////////////////////////////////////////
const char* 
NDBeforeGameMgr::GetRecAccountPwdByIdx(int idx)
{
    VEC_ACCOUNT vAccount;
    NDDataPersist loginData;
    loginData.GetAccount(vAccount);
    int nIdx = 0;
    for (VEC_ACCOUNT::reverse_iterator it = vAccount.rbegin(); it != vAccount.rend(); it++,nIdx++) {
        if(nIdx == idx){
            return (*it).second.c_str();
        }
	}
    return NULL;
}

///////////////////////////////////////////////////////////////
// 内部逻辑
