
#ifndef __NDSubAniGroup_H
#define __NDSubAniGroup_H

#include "basedefine.h"

class NDFrameRunRecord;
class NDAnimationGroup;
class Fighter;
namespace NDEngine 
{	
	enum {
		SUB_ANI_TYPE_SELF = 0,
		SUB_ANI_TYPE_TARGET = 1,
		SUB_ANI_TYPE_NONE = 2,
		SUB_ANI_TYPE_ROLE_FEEDPET = 3,
		SUB_ANI_TYPE_ROLE_WEAPON = 4,
		SUB_ANI_TYPE_SCREEN_CENTER = 5,
	};

	class NDSprite;
	class NDNode;

	struct NDSubAniGroup {

		NDSubAniGroup() {
			memset(this, 0L, sizeof(NDSubAniGroup));
		}

		NDSprite* role;
		NDAnimationGroup* aniGroup;
		Fighter* fighter;
		NDFrameRunRecord* frameRec;

		OBJID idAni;
		short x;
		short y;
		bool reverse;
		short coordW;
		short coordH;
		Byte type;
		int time;	//���Ŵ���
		int antId;	//����ID
		bool bComplete; // �������
		bool isFromOut;
		int startFrame;
		bool isCanStart;//��������ս���е��ӳ�����
	};

	struct NDSubAniGroupEx 
	{
		short x;
		short y;

		short coordW;
		short coordH;

		Byte type;

		std::string anifile;
	};


	//
	// ���ã������Ӷ���
	// ������key: ���ڻ�ȡ�Ӷ����� frameRecord; sag:�Ӷ�����Ϣ
	// ����ֵ���Ӷ����Ƿ񲥷����
	//bool DrawSubAnimation(NDNode* layer, NDSubAniGroup& sag);

	void AddSubAniGroup(NDSubAniGroupEx& group);

	//		
	//		������RunBattleSubAnimation
	//		���ã�����ս���Ӷ���
	//		������fս������
	//		����ֵ����
	void RunBattleSubAnimation(NDSprite* role, Fighter* f);
}

#endif