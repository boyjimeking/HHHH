---------------------------------------------------
--描述: 服务器选择界面
--时间: 2012.3.25
--作者: HJQ
---------------------------------------------------

Login_ServerUI = {}
local p = Login_ServerUI;

p.Account=nil;
p.Pwd=nil;

--推荐服务器(15--21)
local ID_SERVER_SELECT_1 = 15;
local ID_SERVER_SELECT_2 = 16;
local ID_SERVER_SELECT_3 = 17;
local ID_SERVER_SELECT_4 = 18;
local ID_SERVER_SELECT_5 = 20;
local ID_SERVER_SELECT_6 = 21;

--按钮编号
local ID_SERVER_SELECT_PAGE_UP                      = 22;
local ID_SERVER_SELECT_PAGE_DOWN                    = 23;

local ID_SERVER_BUTTON_START = 7;
local ID_SERVER_BUTTON_END   = 22;

p.ServerListTag={
--服务器ID,控件TAG,服务器别名,服务器名,服务器IP,服务器端口,内外服标记
{1,13,'1服内部测试','server01','192.168.9.104',5877,1},
{2,12,'2服内部测试','server02','192.168.9.104',5877,1},
{3,11,'3服内部测试','server03','192.168.9.104',5877,1},
{4,10,'4服内部测试','server04','192.168.9.104',5877,1},
{5,9,'5服内部测试','server05','192.168.9.104',5877,1},
{6,8,'6服内部测试','server06','192.168.9.104',5877,1},
{7,7,'7服外网测试','lyol4','121.207.255.120',5877,2},
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
    layer:SetTag(NMAINSCENECHILDTAG.Login_ServerUI);
    layer:SetFrameRect(RectFullScreenUILayer);
    layer:SetBackgroundColor(ccc4(125, 125, 125, 125));
    scene:AddChild(layer);

    --初始化ui
    local uiLoad = createNDUILoad();
    if nil == uiLoad then
        layer:Free();
        return false;
    end

    uiLoad:Load("ServerSelect.ini", layer, p.OnUIEvent, 0, 0);--选择服务器
    uiLoad:Free();

    p.Init();
    p.Refresh();
    return true;
end

function p.Init()
    --先不显示所有的serverbutton和分页按钮
    local layer = p.getUiLayer();
    local PageUp=GetButton(layer,ID_SERVER_SELECT_PAGE_UP);
    if CheckP(PageUp) then
        PageUp:SetVisible(false);
    end

    local PageDown=GetButton(layer,ID_SERVER_SELECT_PAGE_DOWN);
    if CheckP(PageDown) then
        PageDown:SetVisible(false);
    end
    
    for nBtnSerialNo=ID_SERVER_BUTTON_START,ID_SERVER_BUTTON_END do
        local BtnServer=GetButton(layer,nBtnSerialNo);
        if CheckP(BtnServer) then
            BtnServer:SetVisible(false);
        end
    end
end


function p.getUiLayer()
    local scene = GetSMLoginScene();
    if not CheckP(scene) then
        return nil;
    end

    local layer = GetUiLayer(scene, NMAINSCENECHILDTAG.Login_ServerUI);
    if not CheckP(layer) then
        LogInfo("nil == layer")
        return nil;
    end
    return layer;
end

function p.Refresh()
    --服务器列表排序，根据实际数量决定是否显示上一页和下一页的按钮
    local layer = p.getUiLayer();

    local nServerNum = table.getn(p.ServerListTag);
    LogInfo("table.getn(p.ServerListTag)[%d]", nServerNum);
    for i=1,nServerNum do
        local ServerBtn=GetButton(layer,p.ServerListTag[i][2]);
        if CheckP(ServerBtn) then
            ServerBtn:SetTitle(p.ServerListTag[i][3]);
            ServerBtn:SetVisible(true);
        end        
    end
end

function p.LoginGame(strServerName,strServerIp,strServerPort)
    --发起登陆
    local bSucc=SwichKeyToServer(strServerIp,strServerPort,p.Account,p.Pwd,strServerName);
    if bSucc == false then
        Login_Main.LoadUI();
    else
        --进入Loading界面
        Login_LoadingUI.LoadUI();
        Login_LoadingUI.SetStyle(1);
        Login_LoadingUI.SetProcess(10);
    end
end

function p.OnUIEvent(uiNode, uiEventType, param)
	local tag = uiNode:GetTag();
	if uiEventType == NUIEventType.TE_TOUCH_BTN_CLICK then
        local nServerNum = table.getn(p.ServerListTag);
        for n=1, nServerNum do
            if p.ServerListTag[n][2] == tag then
                p.LoginGame(p.ServerListTag[n][4],p.ServerListTag[n][5],p.ServerListTag[n][6]);
            end
        end
	end
	return true;
end
