---------------------------------------------------
--����: ������ѡ�����
--ʱ��: 2012.3.25
--����: HJQ
---------------------------------------------------

Login_ServerUI = {}
local p = Login_ServerUI;

p.Account=nil;
p.Pwd=nil;

--�Ƽ�������(15--21)
local ID_SERVER_SELECT_1 = 15;
local ID_SERVER_SELECT_2 = 16;
local ID_SERVER_SELECT_3 = 17;
local ID_SERVER_SELECT_4 = 18;
local ID_SERVER_SELECT_5 = 20;
local ID_SERVER_SELECT_6 = 21;

--��ť���
local ID_SERVER_SELECT_PAGE_UP                      = 22;
local ID_SERVER_SELECT_PAGE_DOWN                    = 23;

local ID_SERVER_BUTTON_START = 7;
local ID_SERVER_BUTTON_END   = 22;

p.ServerListTag={
--������ID,�ؼ�TAG,����������,��������,������IP,�������˿�,��������
{1,13,'1���ڲ�����','server01','192.168.9.104',5877,1},
{2,12,'2���ڲ�����','server02','192.168.9.104',5877,1},
{3,11,'3���ڲ�����','server03','192.168.9.104',5877,1},
{4,10,'4���ڲ�����','server04','192.168.9.104',5877,1},
{5,9,'5���ڲ�����','server05','192.168.9.104',5877,1},
{6,8,'6���ڲ�����','server06','192.168.9.104',5877,1},
{7,7,'7����������','lyol4','121.207.255.120',5877,2},
};

function p.LoadUI(strAccount, strPwd)
    local scene = GetSMLoginScene();
    if scene == nil then
        LogInfo("scene == nil,load Login_ServerUI failed!");
        return false;
    end

    scene:RemoveAllChildren(true);
    local layer = createNDUILayer();
    if layer == nil then
        return false;
    end
	
    p.Account=strAccount;
    p.Pwd=strPwd;
    layer:Init();
    layer:SetBackgroundColor(ccc4(125, 125, 125, 125));
    layer:SetFrameRect(RectFullScreenUILayer);
	layer:SetTag(111);
    scene:AddChild(layer);

    --��ʼ��ui
    local uiLoad = createNDUILoad();
    if nil == uiLoad then
        layer:Free();
        return false;
    end

    uiLoad:Load("login_2.ini", layer, p.OnUIEvent, 0, 0);--ѡ�������
    uiLoad:Free();

    p.Init();
	p.Refresh();
    return true;
end

function p.Init()
    --�Ȳ���ʾ���е�serverbutton�ͷ�ҳ��ť
    local layer = p.getUiLayer();
end


function p.getUiLayer()
    local scene = GetSMLoginScene();

    local layer = GetUiLayer(scene, 111);
    return layer;
end

function p.Refresh()
    --�������б����򣬸���ʵ�����������Ƿ���ʾ��һҳ����һҳ�İ�ť
   local layer = p.getUiLayer();

    local nServerNum = 7;--table.getn(p.ServerListTag);
	for i = 1,nServerNum do
		local ServerBtn=GetButton(layer,p.ServerListTag[i][2]);
		ServerBtn:SetTitle(p.ServerListTag[i][3]);
        ServerBtn:SetVisible(true);
    end
end

function p.LoginGame(strServerName,strServerIp,strServerPort)
    --�����½
    local bSucc=SwichKeyToServer(strServerIp,strServerPort,p.Account,p.Pwd,strServerName);
    if bSucc == false then
        Login_Main.LoadUI();
    else
        --����Loading����
        Login_LoadingUI.LoadUI();
        Login_LoadingUI.SetStyle(1);
        Login_LoadingUI.SetProcess(10);
    end
end

function p.OnUIEvent(uiNode, uiEventType, param)
	PrintString("fefaew");
	return true;
end
RegisterGlobalEventHandler(103,"Login_ServerUI.LoadUI", p.LoadUI);