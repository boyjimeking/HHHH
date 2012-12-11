/*
 *  ScriptDrama.mm
 *  DragonDrive
 *
 *  Created by jhzheng on 12-1-10.
 *  Copyright 2012 (����)DeNA. All rights reserved.
 *
 */
#include "NDDirector.h"
#include "ScriptDrama.h"
#include "ScriptInc.h"
#include "DramaCommand.h"
#include "Drama.h"

namespace NDEngine {

// �ű�api

////////////////////////////////////////////////////////////////////////////////////
// �Ի�����

// ����: DramaOpenLChatDlg();
// ����: ����߶Ի���

// ����: DramaOpenRChatDlg();
// ����: ���ұ߶Ի���

// ����: DramaCloseLChatDlg();
// ����: �رնԻ���

// ����: DramaCloseRChatDlg();
// ����: �ر��ұ߶Ի���

// ����: DramaSetLChatFigure(string strFileName);
// ����: ���þ����������ͷ��
// ����: strFileName: ͷ���ļ���

// ����: DramaSetLChatName(string strName, int nFontsize, int nFontcolor);
// ����: ���þ��������������
// ����: strName: ����
//		nFontsize: ����(Ĭ��nil)
//		nFontcolor: ��ɫ(Ĭ��nil)

// ����: DramaSetLChatNameBySpriteKey(int nKey, int nFontsize, int nFontcolor);
// ����: ���þ��������������
// ����: nKey: ���Ӿ���ʱ���ص�key
//		nFontsize: ����(Ĭ��nil)
//		nFontcolor: ��ɫ(Ĭ��nil)

// ����: DramaSetLChatContent(string strContent, int nFontsize, int nFontcolor);
// ����: ���þ��������������
// ����: strContent: ��������
//		nFontsize: ����(Ĭ��nil)
//		nFontcolor: ��ɫ(Ĭ��nil)

// ����: DramaSetRChatFigure(string strFileName);
// ����: ���þ����ұ�����ͷ��

// ����: DramaSetRChatName(string strName, int nFontsize, int nFontcolor);
// ����: ���þ����ұ���������

// ����: DramaSetRChatNameBySpriteKey(int nKey, int nFontsize, int nFontcolor);
// ����: ���þ����ұ���������
// ����: nKey: ���Ӿ���ʱ���ص�key
//		nFontsize: ����(Ĭ��nil)
//		nFontcolor: ��ɫ(Ĭ��nil)

// ����: DramaSetRChatContent(string strContent, int nFontsize, int nFontcolor);
// ����: ���þ����ұ���������

////////////////////////////////////////////////////////////////////////////////////
// �������

// ����: DramaAddSprite(int nLookface, int nType, bool faceRight, string name);
// ����: ���������Ӿ���
// ����: nLookface: ��������
//		nType: ���������, 1 ���, 2 npc, 3 ���� 4 ����
//      faceRight:����ĳ���, true ����, false ����
//      name:��ʾ��ͷ�ϵ�����
// ����ֵ: int nKey : ���ں����Ըþ������������

// ����: DramaRemoveSprite(int nKey);
// ����: �Ƴ������еľ���
// ����: nKey: AddDramaSprite�������ص�nKey

// ����: DramaSetSpriteAni(int nKey, int nAniIndex);
// ����: ���ó����еľ���Ķ���
// ����: nKey: AddDramaSprite�������ص�key
//		nAniIndex: ��������

// ����: DramaSetSpritePosition(int nKey, int nPosX, int nPosY);
// ����: ���ó����еľ����λ��
// ����: nKey: AddDramaSprite�������ص�key
//		nPosX: ��ͼ����X(��CellΪ��λ)
//		nPosY: ��ͼ����Y(��CellΪ��λ)

// ����: DramaMoveSprite(int nKey, int nToPosX, int nToPosY, int nStep);
// ����: �ƶ������еľ��鵽ָ��λ��
// ����: nKey: AddDramaSprite�������ص�key
//		nToPosX: ָ��λ�õĵ�ͼ����X(��CellΪ��λ)
//		nToPosY: ��ͼ����Y(��CellΪ��λ)
//		nStep: �������ֵ

////////////////////////////////////////////////////////////////////////////////////
// ��������

// ����: DramaLoadScene(int nMapID);
// ����: ���س���
// ����: nMapID: ��ͼID

// ����: DramaLoadEraseInOutScene(string centerText, float showTime);
// ����: ���ص��뵭������
// ����: centerText: ��Ļ�м���ʾ���ı�
// ����:	nFontsize: ����(Ĭ��nil)
// ����:	nFontcolor: ��ɫ(Ĭ��nil)
// ����: showTime: ������ʾ��ʱ��

// ����: DramaRemoveEraseInOutScene(int nKey)
// ����: �Ƴ����뵭������
// ����: nKey: DramaLoadEraseInOutScene���ص�key

////////////////////////////////////////////////////////////////////////////////////
// �������

// ����: DramaSetCameraPos(int nPosX, int nPosY)
// ����: ���ó����е����λ��
// ����: nPosX: ��ͼ����X(��CellΪ��λ)
//		nPosY: ��ͼ����Y(��CellΪ��λ)

// ����: DramaMoveCamera(int nToPosX, int nToPosY, int nStep)
// ����: �ƶ�����������е�ָ��λ��
// ����: nToPosX: ��ͼ����X(��CellΪ��λ)
//		nToPosY: ��ͼ����Y(��CellΪ��λ)
//		nStep:�����������

////////////////////////////////////////////////////////////////////////////////////
// ��������

// ����: DramaSetWaitTime(float fTime)
// ����: ���������κδ���fTime ms;
// ����: fTime: �Ժ���Ϊ��λ

// ����: DramaWaitPrevActionFinish()
// ����: �ȴ�֮ǰ�Ĳ�����ɺ��ټ�������Ĳ���

// ����: DramaWaitPrevActionFinishAndClick()
// ����: �ȴ�֮ǰ�Ĳ�����ɺ���ͨ�������������Ĳ���

// ����: DramaStart();
// ����: ��ʼ����

// ����: DramaFinish();
// ����: ��������

// ����: DramaShowTipDlg(std::string content)
// ����: �ڳ������м���ʾ��ʾ�Ի���
// ����: content: ����

////////////////////////////////////////////////////////////////////////////////////
// �Ի�����

void DramaCloseRChatDlg();
void DramaOpenLChatDlgNotCloseRight();
void DramaCloseLChatDlg();
void DramaOpenRChatDlgNotCloseLeft();
// ����: DramaOpenLChatDlg();
// ����: ����߶Ի���,ͬʱ�ر��ұ߶Ի���
void DramaOpenLChatDlg()
{
	DramaCloseRChatDlg();
	DramaOpenLChatDlgNotCloseRight();
}
// ����: DramaOpenLChatDlg();
// ����: ����߶Ի���,���ر��ұ߶Ի���
void DramaOpenLChatDlgNotCloseRight()
{
	DramaCommandDlg* command = new DramaCommandDlg;
	command->InitWithOpen(true);
	DramaObj.AddCommond(command);
}

// ����: DramaOpenRChatDlg();
// ����: ���ұ߶Ի���,ͬʱ�ر���߶Ի���
void DramaOpenRChatDlg()
{
	DramaCloseLChatDlg();
	DramaOpenRChatDlgNotCloseLeft();
}
// ����: DramaOpenRChatDlgNotCloseLeft();
// ����: ���ұ߶Ի��򣬲��ر���߶Ի���
void DramaOpenRChatDlgNotCloseLeft()
{
	DramaCommandDlg* command = new DramaCommandDlg;
	command->InitWithOpen(false);
	DramaObj.AddCommond(command);
}


// ����: DramaCloseLChatDlg();
// ����: �رնԻ���
void DramaCloseLChatDlg()
{
	DramaCommandDlg* command = new DramaCommandDlg;
	command->InitWithClose(true);
	DramaObj.AddCommond(command);
}

// ����: DramaCloseRChatDlg();
// ����: �ر��ұ߶Ի���
void DramaCloseRChatDlg()
{
	DramaCommandDlg* command = new DramaCommandDlg;
	command->InitWithClose(false);
	DramaObj.AddCommond(command);
}

// ����: DramaSetLChatFigure(string strFileName);
// ����: ���þ����������ͷ��
// ����: strFileName: ͷ���ļ���
void DramaSetLChatFigure(std::string strFileName, bool bReverse, int nCol, int nRow)
{
	DramaCommandDlg* command = new DramaCommandDlg;
	command->InitWithSetFigure(true, strFileName, bReverse, nCol, nRow);
	DramaObj.AddCommond(command);
}

// ����: DramaSetLChatName(string strName, int nFontsize, int nFontcolor);
// ����: ���þ��������������
// ����: strName: ����
//		nFontsize: ����(Ĭ��nil)
//		nFontcolor: ��ɫ(Ĭ��nil)

void DramaSetLChatName(std::string strName, int nFontSize, int nFontColor)
{
    nFontSize*=FONT_SCALE;
	DramaCommandDlg* command = new DramaCommandDlg;
	command->InitWithSetTitle(true, strName, nFontSize, nFontColor);
	DramaObj.AddCommond(command);
}

// ����: DramaSetLChatNameBySpriteKey(int nKey, int nFontsize, int nFontcolor);
// ����: ���þ��������������
// ����: nKey: ���Ӿ���ʱ���ص�key
//		nFontsize: ����(Ĭ��nil)
//		nFontcolor: ��ɫ(Ĭ��nil)
void DramaSetLChatNameBySpriteKey(int nKey, int nFontSize, int nFontColor)
{
	nFontSize *= FONT_SCALE;
	DramaCommandDlg* command = new DramaCommandDlg;
	command->InitWithSetTitleBySpriteKey(true, nKey, nFontSize, nFontColor);
	DramaObj.AddCommond(command);
}

// ����: DramaSetLChatContent(string strContent, int nFontsize, int nFontcolor);
// ����: ���þ��������������
// ����: strContent: ��������
//		nFontsize: ����(Ĭ��nil)
//		nFontcolor: ��ɫ(Ĭ��nil)
void DramaSetLChatContent(std::string strContent, int nFontSize, int nFontColor)
{
	nFontSize *= FONT_SCALE;
	DramaCommandDlg* command = new DramaCommandDlg;
	command->InitWithSetContent(true, strContent, nFontSize, nFontColor);
	DramaObj.AddCommond(command);
}

// ����: DramaSetRChatFigure(string strFileName);
// ����: ���þ����ұ�����ͷ��
void DramaSetRChatFigure(std::string strFileName, bool bReverse, int nCol, int nRow)
{
	DramaCommandDlg* command = new DramaCommandDlg;
	command->InitWithSetFigure(false, strFileName, bReverse,  nCol ,  nRow);
	DramaObj.AddCommond(command);
}

// ����: DramaSetRChatName(string strName, int nFontsize, int nFontcolor);
// ����: ���þ����ұ���������
void DramaSetRChatName(std::string strName, int nFontSize, int nFontColor)
{
	nFontSize *= FONT_SCALE;
	DramaCommandDlg* command = new DramaCommandDlg;
	command->InitWithSetTitle(false, strName, nFontSize, nFontColor);
	DramaObj.AddCommond(command);
}

// ����: DramaSetRChatNameBySpriteKey(int nKey, int nFontsize, int nFontcolor);
// ����: ���þ����ұ���������
// ����: nKey: ���Ӿ���ʱ���ص�key
//		nFontsize: ����(Ĭ��nil)
//		nFontcolor: ��ɫ(Ĭ��nil)
void DramaSetRChatNameBySpriteKey(int nKey, int nFontSize, int nFontColor)
{
	nFontSize *= FONT_SCALE;
	DramaCommandDlg* command = new DramaCommandDlg;
	command->InitWithSetTitleBySpriteKey(false, nKey, nFontSize, nFontColor);
	DramaObj.AddCommond(command);
}

// ����: DramaSetRChatContent(string strContent, int nFontsize, int nFontcolor);
// ����: ���þ����ұ���������
void DramaSetRChatContent(std::string strContent, int nFontSize, int nFontColor)
{
	nFontSize *= FONT_SCALE;
	DramaCommandDlg* command = new DramaCommandDlg;
	command->InitWithSetContent(false, strContent, nFontSize, nFontColor);
	DramaObj.AddCommond(command);
}

////////////////////////////////////////////////////////////////////////////////////
// �������

// ����: DramaAddSprite(int nLookface, int nType, bool faceRight, string name);
// ����: ���������Ӿ���
// ����: nLookface: ��������
//		nType: ���������, 1 ���, 2 npc, 3 ����
//      faceRight:����ĳ���, true ����, false ����
//      name:��ʾ��ͷ�ϵ�����
// ����ֵ: int nKey : ���ں����Ըþ������������
double DramaAddSprite(int nLookface, int nType, bool faceRight, std::string name)
{
	DramaCommandSprite* command = new DramaCommandSprite;
	command->InitWithAdd(nLookface, nType, faceRight, name);
	DramaObj.AddCommond(command);
	return command->GetKey();
}

// ����: DramaAddSpriteWithFile(std::string filename)
// ����: ���������Ӿ���
// ����: std::string name: ������ļ���
// ����ֵ: int nKey : ���ں����Ըþ������������
double DramaAddSpriteWithFile(std::string filename)
{
	DramaCommandSprite* command = new DramaCommandSprite;
	command->InitWithAddByFile(filename);
	DramaObj.AddCommond(command);
	return command->GetKey();
}

// ����: DramaRemoveSprite(int nKey);
// ����: �Ƴ������еľ���
// ����: nKey: AddDramaSprite�������ص�nKey
void DramaRemoveSprite(int nKey)
{
	DramaCommandSprite* command = new DramaCommandSprite;
	command->InitWithRemove(nKey);
	DramaObj.AddCommond(command);
}

// ����: DramaSetSpriteAni(int nKey, int nAniIndex);
// ����: ���ó����еľ���Ķ���
// ����: nKey: AddDramaSprite�������ص�key
//		nAniIndex: ��������
void DramaSetSpriteAni(int nKey, int nAniIndex)
{
	DramaCommandSprite* command = new DramaCommandSprite;
	command->InitWithSetAnimation(nKey, nAniIndex);
	DramaObj.AddCommond(command);
}

// ����: DramaSetSpriteReverse(int nKey, bool bReverse)
// ����: ���ó����еľ���ķ�ת
// ����: nKey: AddDramaSprite�������ص�key
//		 bReverse: �Ƿ�ת
void DramaSetSpriteReverse(int nKey, bool bReverse)    
{ 
	DramaCommandSprite* command = new DramaCommandSprite;
	command->InitWithSetReverse(nKey, bReverse);
	DramaObj.AddCommond(command);
}

// ����: DramaSetSpritePosition(int nKey, int nPosX, int nPosY);
// ����: ���ó����еľ����λ��
// ����: nKey: AddDramaSprite�������ص�key
//		nPosX: ��ͼ����X(��CellΪ��λ)
//		nPosY: ��ͼ����Y(��CellΪ��λ)
void DramaSetSpritePosition(int nKey, int nPosX, int nPosY)
{
	DramaCommandSprite* command = new DramaCommandSprite;
	command->InitWithSetPos(nKey, nPosX, nPosY);
	DramaObj.AddCommond(command);
}

// ����: DramaMoveSprite(int nKey, int nToPosX, int nToPosY, int nStep);
// ����: �ƶ������еľ��鵽ָ��λ��
// ����: nKey: AddDramaSprite�������ص�key
//		nToPosX: ָ��λ�õĵ�ͼ����X(��CellΪ��λ)
//		nToPosY: ��ͼ����Y(��CellΪ��λ)
//		nStep: �������ֵ
void DramaMoveSprite(int nKey, int nPosX, int nPosY, int nStep)
{
	DramaCommandSprite* command = new DramaCommandSprite;
	command->InitWithMove(nKey, nPosX, nPosY, nStep);
	DramaObj.AddCommond(command);
}

////////////////////////////////////////////////////////////////////////////////////
// ��������

// ����: DramaLoadScene(int nMapID);
// ����: ���س���
// ����: nMapID: ��ͼID
void DramaLoadScene(int nMapID)
{
	DramaCommandScene* command = new DramaCommandScene;
	command->InitWithLoadDrama(nMapID);
	DramaObj.AddCommond(command);
}

// ����: DramaLoadEraseInOutScene(string centerText, int nFontsize, int nFontColor);
// ����: ���ص��뵭������
// ����: centerText: ��Ļ�м���ʾ���ı�
// ����:	nFontsize: ����(Ĭ��nil)
// ����:	nFontcolor: ��ɫ(Ĭ��nil)
// ����ֵ: key(���ں�����������ɾ������)
double DramaLoadEraseInOutScene(string centerText, int nFontsize, int nFontColor)
{
	nFontsize *= FONT_SCALE;
	DramaCommandScene* command = new DramaCommandScene;
	command->InitWithLoad(centerText, nFontsize, nFontColor);
	DramaObj.AddCommond(command);
	return command->GetKey();
}

// ����: DramaRemoveEraseInOutScene(int nKey)
// ����: �Ƴ����뵭������
// ����: nKey: DramaLoadEraseInOutScene���ص�key
void DramaRemoveEraseInOutScene(int nKey)
{
	DramaCommandScene* command = new DramaCommandScene;
	command->InitWithRemove(nKey);
	DramaObj.AddCommond(command);
}

////////////////////////////////////////////////////////////////////////////////////
// �������

// ����: DramaSetCameraPos(int nPosX, int nPosY)
// ����: ���ó����е����λ��
// ����: nPosX: ��ͼ����X(��CellΪ��λ)
//		nPosY: ��ͼ����Y(��CellΪ��λ)
void DramaSetCameraPos(int nPosX, int nPosY)
{
	DramaCommandCamera* command = new DramaCommandCamera;
	command->InitWithSetPos(nPosX, nPosY);
	DramaObj.AddCommond(command);
}

// ����: DramaMoveCamera(int nToPosX, int nToPosY, int nStep)
// ����: �ƶ�����������е�ָ��λ��
// ����: nToPosX: ��ͼ����X(��CellΪ��λ)
//		nToPosY: ��ͼ����Y(��CellΪ��λ)
//		nStep:�����������
void DramaMoveCamera(int nToPosX, int nToPosY, int nStep)
{
	DramaCommandCamera* command = new DramaCommandCamera;
	command->InitWithMove(nToPosX, nToPosY, nStep);
	DramaObj.AddCommond(command);
}

////////////////////////////////////////////////////////////////////////////////////
// ��������

// ����: DramaSetWaitTime(float fTime)
// ����: ���������κδ���fTime ms;
// ����: fTime: �Ժ���Ϊ��λ
void DramaSetWaitTime(float fTime)
{
	DramaCommandWait* command = new DramaCommandWait;
	command->InitWithWait(fTime);
	DramaObj.AddCommond(command);
}

// ����: DramaWaitPrevActionFinish()
// ����: �ȴ�֮ǰ�Ĳ�����ɺ��ټ�������Ĳ���
void DramaWaitPrevActionFinish()
{
	DramaCommandWait* command = new DramaCommandWait;
	command->InitWithWaitPreActionFinish();
	DramaObj.AddCommond(command);
}

// ����: DramaWaitPrevActionFinishAndClick()
// ����: �ȴ�֮ǰ�Ĳ�����ɺ���ͨ�������������Ĳ���
void DramaWaitPrevActionFinishAndClick()
{
	DramaCommandWait* command = new DramaCommandWait;
	command->InitWithWaitPreActFinishAndClick();
	DramaObj.AddCommond(command);
}

// ����: DramaStart();
// ����: ��ʼ����
void DramaStart()
{
	DramaObj.Start();
}

// ����: DramaFinish();
// ����: ��������
void DramaFinish()
{
	DramaCommandScene* command = new DramaCommandScene;
	command->InitWithFinishDrama();
	DramaObj.AddCommond(command);
}

// ����: DramaShowTipDlg(std::string content)
// ����: �ڳ������м���ʾ��ʾ�Ի���
// ����: content: ����
void DramaShowTipDlg(std::string content)
{
	DramaCommandDlg* command = new DramaCommandDlg;
	command->InitWithTip(content);
	DramaObj.AddCommond(command);
}

// ����: DramaPlaySoundEffect
// ����: ������Ч
// ����: nSoundEffectId: ��Ч��id
void DramaPlaySoundEffect(int nSoundEffectId)
{
	DramaCommandSoundEffect* pkCommand = new DramaCommandSoundEffect;
	pkCommand->InitWithSoundEffectId(nSoundEffectId);
	DramaObj.AddCommond(pkCommand);
}
    
void ScriptDramaLoad()
{
	ETCFUNC("DramaPlaySoundEffect", DramaPlaySoundEffect);
	ETCFUNC("DramaOpenLChatDlg", DramaOpenLChatDlg);
	ETCFUNC("DramaOpenLChatDlgNotCloseRight", DramaOpenLChatDlgNotCloseRight);
	ETCFUNC("DramaOpenRChatDlg", DramaOpenRChatDlg);
	ETCFUNC("DramaOpenRChatDlgNotCloseLeft", DramaOpenRChatDlgNotCloseLeft);
	ETCFUNC("DramaCloseLChatDlg", DramaCloseLChatDlg);
	ETCFUNC("DramaCloseRChatDlg", DramaCloseRChatDlg);
	ETCFUNC("DramaSetLChatFigure", DramaSetLChatFigure);
	ETCFUNC("DramaSetLChatName", DramaSetLChatName);
	ETCFUNC("DramaSetLChatNameBySpriteKey", DramaSetLChatNameBySpriteKey);
	ETCFUNC("DramaSetLChatContent", DramaSetLChatContent);
	ETCFUNC("DramaSetRChatFigure", DramaSetRChatFigure);
	ETCFUNC("DramaSetRChatName", DramaSetRChatName);
	ETCFUNC("DramaSetRChatNameBySpriteKey", DramaSetRChatNameBySpriteKey);
	ETCFUNC("DramaSetRChatContent", DramaSetRChatContent);
	ETCFUNC("DramaAddSprite", DramaAddSprite);
	ETCFUNC("DramaAddSpriteWithFile", DramaAddSpriteWithFile);
	ETCFUNC("DramaRemoveSprite", DramaRemoveSprite);
	ETCFUNC("DramaSetSpriteAni", DramaSetSpriteAni);
	ETCFUNC("DramaSetSpritePosition", DramaSetSpritePosition);
	ETCFUNC("DramaMoveSprite", DramaMoveSprite);
	ETCFUNC("DramaLoadScene", DramaLoadScene);
	ETCFUNC("DramaLoadEraseInOutScene", DramaLoadEraseInOutScene);
	ETCFUNC("DramaRemoveEraseInOutScene", DramaRemoveEraseInOutScene);
	ETCFUNC("DramaSetCameraPos", DramaSetCameraPos);
	ETCFUNC("DramaMoveCamera", DramaMoveCamera);
	ETCFUNC("DramaSetWaitTime", DramaSetWaitTime);
	ETCFUNC("DramaWaitPrevActionFinish", DramaWaitPrevActionFinish);
	ETCFUNC("DramaWaitPrevActionFinishAndClick", DramaWaitPrevActionFinishAndClick);
	ETCFUNC("DramaStart", DramaStart);
	ETCFUNC("DramaFinish", DramaFinish);
	ETCFUNC("DramaShowTipDlg", DramaShowTipDlg);
	ETCFUNC("DramaSetSpriteReverse", DramaSetSpriteReverse);
}

}
