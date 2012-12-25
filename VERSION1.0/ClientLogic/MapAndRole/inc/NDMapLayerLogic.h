/*
 *  NDMapLayerLogic.h
 *  DragonDrive
 *
 *  Created by jhzheng on 11-1-7.
 *  Copyright 2011 (����)DeNA. All rights reserved.
 *
 */

#ifndef _ND_MAP_LAYER_LOGIC_H_
#define _ND_MAP_LAYER_LOGIC_H_

#include "NDMapLayer.h"
#include "NDUIDialog.h"
#include "NDTimer.h"

NS_NDENGINE_BGN

class NDMapLayerLogic:
	public NDMapLayer,
	public NDUIDialogDelegate
{
	DECLARE_CLASS (NDMapLayerLogic)
public:
	NDMapLayerLogic();
	~NDMapLayerLogic();

public:
	void DidFinishLaunching();

	bool TouchBegin(NDTouch* touch);
	void TouchEnd(NDTouch* touch);
	void TouchCancelled(NDTouch* touch);
	void TouchMoved(NDTouch* touch);
	void Update(unsigned long ulDiff);
	void OnTimer(OBJID uiTag);

	virtual void draw();

	//@opt: �����Ż���أ���ʱ�����.
public:
	static bool IsWorldMapVisible();
	static bool hasFullScreenOpaqueUI();
	static bool isFullScreenOpaqueUI( const char* layerName );
	static bool canDrawMapLayer();
	static bool canDrawWorldMapLayer();

private:

	NDTimer m_kTimer;
	double m_dTimeStamp;
	bool m_bLongTouch;
	CCPoint m_kPosTouch;
	bool m_bPathing;

private:
	void SetLongTouch(bool bSet);
	bool IsLongTouch();
	void SetPathing(bool bPathing);
	bool IsPathing();
};

NS_NDENGINE_END

#endif // _ND_MAP_LAYER_LOGIC_H_
