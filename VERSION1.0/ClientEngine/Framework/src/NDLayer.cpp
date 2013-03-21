//
//  NDLayer.mm
//  DragonDrive
//
//  Created by xiezhenghai on 10-12-8.
//  Copyright 2010 (����)DeNA. All rights reserved.
//

#include "NDLayer.h"
#include "NDBaseLayer.h"
#include "ObjectTracker.h"

namespace NDEngine
{
IMPLEMENT_CLASS(NDLayer, NDNode)

NDLayer::NDLayer()
{
	INC_NDOBJ_RTCLS
	INIT_AUTOLINK(NDLayer);
}

NDLayer::~NDLayer()
{
	DEC_NDOBJ_RTCLS
}

NDLayer* NDLayer::Layer()
{
	NDLayer *layer = new NDLayer();
	layer->Initialization();
	return layer;
}

void NDLayer::Initialization()
{
	NDAsssert(m_pkCCNode == NULL);
	m_pkCCNode = new NDBaseLayer();
	NDBaseLayer *layer = (NDBaseLayer *) m_pkCCNode;
	layer->SetLayer(this);
	layer->setDebugName( "NDLayer" );
	this->SetContentSize(CCSizeZero);
}

void NDLayer::draw()
{

}

void NDLayer::SetTouchEnabled(bool bEnabled)
{
	NDAsssert(m_pkCCNode != NULL);

	NDBaseLayer *layer = (NDBaseLayer*) m_pkCCNode;
	layer->setTouchEnabled(bEnabled);
}

bool NDLayer::TouchBegin(NDTouch* touch)
{
	return true;
}

void NDLayer::TouchEnd(NDTouch* touch)
{

}

void NDLayer::TouchCancelled(NDTouch* touch)
{

}

void NDLayer::TouchMoved(NDTouch* touch)
{

}

bool NDLayer::TouchDoubleClick(NDTouch* touch)
{
	return true;
}
}
