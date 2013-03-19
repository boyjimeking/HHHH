/*
*
*/

#ifndef NDSCROLLIMAGENUMBER_H
#define NDSCROLLIMAGENUMBER_H

#include "define.h"
#include "NDObject.h"
#include "ImageNumber.h"

NS_NDENGINE_BGN

using namespace std;

class NDScrollImageNumber:
	public NDObject,
	public ImageNumber
{
	DECLARE_CLASS(NDScrollImageNumber)

public:

	typedef enum __tagFontType
	{
		Font_SmallRed,
		Font_MaxCount
	}FontType;

	NDScrollImageNumber();
	virtual ~NDScrollImageNumber();

	virtual void draw();

	bool SetScrollNumber(int uiNumber,float fSecond,FontType eType,bool bWithSign = false);

protected:

	CC_SYNTHESIZE(float,m_fScrollTime,ScrollTime);
	CC_SYNTHESIZE(int,m_nScrollNumber,ScrollNumber);			///< ÿһλ����λ��λǧλ����0��9��ʱ�� @����

	CC_SYNTHESIZE_READONLY(vector<unsigned int>,m_kBits,Bits);

	FontType m_eFontType;

private:
};

NS_NDENGINE_END

#endif