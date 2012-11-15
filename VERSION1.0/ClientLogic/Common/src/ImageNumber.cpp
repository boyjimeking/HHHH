//
//  ImageNumber.mm
//  DragonDrive
//
//  Created by xiezhenghai on 11-1-19.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "ImageNumber.h"
#import "NDUtility.h"

#define title_image ([[NSString stringWithFormat:@"%s", GetImgPath("titles.png")] UTF8String])

static PictureNumber* PictureNumber_SharedInstance = NULL;

PictureNumber::PictureNumber()
{
	NDAsssert(PictureNumber_SharedInstance == NULL);
	
	for (unsigned int i = 0; i < sizeof(m_picTitleGlod) / sizeof(m_picTitleGlod[0]); i++) 
	{
		m_picTitleGlod[i] = NULL;
	}
	for (unsigned int i = 0; i < sizeof(m_picTitleRed) / sizeof(m_picTitleRed[0]); i++) 
	{
		m_picTitleRed[i] = NULL;
	}
	for (unsigned int i = 0; i < sizeof(m_picSmallGold) / sizeof(m_picSmallGold[0]); i++) 
	{
		m_picSmallGold[i] = NULL;
	}
	for (unsigned int i = 0; i < sizeof(m_picBigRed) / sizeof(m_picBigRed[0]); i++) 
	{
		m_picBigRed[i] = NULL;
	}
	for (unsigned int i = 0; i < sizeof(m_picBigGreen) / sizeof(m_picBigGreen[0]); i++) 
	{
		m_picBigGreen[i] = NULL;
	}
	for (unsigned int i = 0; i < sizeof(m_picSmallWhite) / sizeof(m_picSmallWhite[0]); i++) 
	{
		m_picSmallWhite[i] = NULL;
	}
	for (unsigned int i = 0; i < sizeof(m_picSmallRed) / sizeof(m_picSmallRed[0]); i++) 
	{
		m_picSmallRed[i] = NULL;
	}
	m_picTitleSplit = NULL;
}

PictureNumber::~PictureNumber()
{
	PictureNumber_SharedInstance = NULL;
	
	for (unsigned int i = 0; i < sizeof(m_picTitleGlod) / sizeof(m_picTitleGlod[0]); i++) 
	{
		delete m_picTitleGlod[i];
	}
	for (unsigned int i = 0; i < sizeof(m_picTitleRed) / sizeof(m_picTitleRed[0]); i++) 
	{
		delete m_picTitleRed[i];
	}
	for (unsigned int i = 0; i < sizeof(m_picSmallGold) / sizeof(m_picSmallGold[0]); i++) 
	{
		delete m_picSmallGold[i];
	}
	for (unsigned int i = 0; i < sizeof(m_picBigRed) / sizeof(m_picBigRed[0]); i++) 
	{
		delete m_picBigRed[i];
	}
	for (unsigned int i = 0; i < sizeof(m_picBigGreen) / sizeof(m_picBigGreen[0]); i++) 
	{
		delete m_picBigGreen[i];
	}
	for (unsigned int i = 0; i < sizeof(m_picSmallWhite) / sizeof(m_picSmallWhite[0]); i++) 
	{
		delete m_picSmallWhite[i];
	}
	for (unsigned int i = 0; i < sizeof(m_picSmallRed) / sizeof(m_picSmallRed[0]); i++) 
	{
		delete m_picSmallRed[i];
	}
	delete m_picTitleSplit;
}

PictureNumber* PictureNumber::SharedInstance()
{
	if (PictureNumber_SharedInstance == NULL) 
	{
		PictureNumber_SharedInstance = new PictureNumber();
	}
	return PictureNumber_SharedInstance;
}

NDPicture* PictureNumber::TitleGoldNumber(unsigned int number)
{
	if (number >= sizeof(m_picTitleGlod) / sizeof(m_picTitleGlod[0])) 
	{
		return NULL;
	}
	
	if (!m_picTitleGlod[number]) 
	{
		m_picTitleGlod[number] = NDPicturePool::DefaultPool()->AddPicture(title_image);
		int idx = number - 1;
		if (idx == -1) 
			idx = 9;
		m_picTitleGlod[number]->Cut(CCRectMake(120 + idx * 20, 24, GetTitleGoldNumberSize().width, GetTitleGoldNumberSize().height));
	}
	return m_picTitleGlod[number];
}

NDPicture* PictureNumber::TitleRedNumber(unsigned int number)
{
	if (number >= sizeof(m_picTitleRed) / sizeof(m_picTitleRed[0])) 
	{
		return NULL;
	}
	
	if (!m_picTitleRed[number]) 
	{
		m_picTitleRed[number] = NDPicturePool::DefaultPool()->AddPicture(title_image);
		m_picTitleRed[number]->Cut(CCRectMake(80 + number * 20, 48, GetTitleRedNumberSize().width, GetTitleRedNumberSize().height));
	}
	return m_picTitleRed[number];
}

NDPicture* PictureNumber::TitleSplit()
{
	if (!m_picTitleSplit) 
	{
		m_picTitleSplit = NDPicturePool::DefaultPool()->AddPicture(title_image);
		m_picTitleSplit->Cut(CCRectMake(60, 48, GetTitleSplitSize().width, GetTitleSplitSize().height));
	}
	return m_picTitleSplit;
}

CCSize PictureNumber::GetTitleGoldNumberSize()
{
	return CCSizeMake(16, 16);
}

CCSize PictureNumber::GetTitleRedNumberSize()
{
	return CCSizeMake(9, 12);
}

CCSize PictureNumber::GetTitleSplitSize()
{
	return CCSizeMake(11, 11);
}

NDPicture* PictureNumber::SmallRed(unsigned int index)
{
	if (index > 12)
	{
		return NULL;
	}
	
	if (!m_picSmallRed[index])
	{
		m_picSmallRed[index] = NDPicturePool::DefaultPool()->AddPicture(GetImgPath("numberSmall.png"));
		m_picSmallRed[index]->Cut(CCRectMake(GetSmallRedSize().width * index, 0, GetSmallRedSize().width, GetSmallRedSize().height));
	}
	
	return m_picSmallRed[index];
}

NDPicture* PictureNumber::SmallWhite(unsigned int index)
{
	if (index > 12)
	{
		return NULL;
	}
	
	if (!m_picSmallWhite[index])
	{
		m_picSmallWhite[index] = NDPicturePool::DefaultPool()->AddPicture(GetImgPath("number1.png"));
		m_picSmallWhite[index]->Cut(CCRectMake(GetSmallWhiteSize().width * index, 0, GetSmallWhiteSize().width, GetSmallWhiteSize().height));
	}
	
	return m_picSmallWhite[index];
}

NDPicture* PictureNumber::BigGreen(unsigned int index)
{
	if (index > 12)
	{
		return NULL;
	}
	
	if (!m_picBigGreen[index])
	{
		m_picBigGreen[index] = NDPicturePool::DefaultPool()->AddPicture(GetImgPath("numberBigG.png"));
		m_picBigGreen[index]->Cut(CCRectMake(GetBigRedSize().width * index, 0, GetBigRedSize().width, GetBigRedSize().height));
	}
	
	return m_picBigGreen[index];
}

NDPicture* PictureNumber::SmallGold(unsigned int index)
{
	if (index > 9)
	{
		return NULL;
	}
	
	if (!m_picSmallGold[index])
	{
		m_picSmallGold[index] = NDPicturePool::DefaultPool()->AddPicture(GetImgPathBattleUI("numberyellow.png"));
		m_picSmallGold[index]->Cut(CCRectMake(GetSmallGoldSize().width * index, 0, GetSmallGoldSize().width, GetSmallGoldSize().height));
	}
	
	return m_picSmallGold[index];
}

NDPicture* PictureNumber::BigRed(unsigned int index)
{
	if (index > 12)
	{
		return NULL;
	}
	
	if (!m_picBigRed[index])
	{
		m_picBigRed[index] = NDPicturePool::DefaultPool()->AddPicture(GetImgPath("numberBig.png"));
		m_picBigRed[index]->Cut(CCRectMake(GetBigRedSize().width * index, 0, GetBigRedSize().width, GetBigRedSize().height));
	}
	
	return m_picBigRed[index];
}

CCSize PictureNumber::GetSmallWhiteSize()
{
	return CCSizeMake(6.0f, 9.0f);
}

CCSize PictureNumber::GetSmallRedSize()
{
	return CCSizeMake(6.0f, 9.0f);
}

CCSize PictureNumber::GetBigRedSize()
{
	return CCSizeMake(11.0f, 14.0f);
}

CCSize PictureNumber::GetSmallGoldSize()
{
	return CCSizeMake(6.0f, 5.0f);
}

//////////////////////
IMPLEMENT_CLASS(ImageNumber, NDUILayer)

ImageNumber::ImageNumber()
{
}

ImageNumber::~ImageNumber()
{
}

void ImageNumber::Initialization()
{
	NDUILayer::Initialization();
	this->SetFrameRect(CCRectZero);
	this->SetTouchEnabled(false);
}

CCSize ImageNumber::GetNumberSize()
{
	return m_size;
}

unsigned int ImageNumber::SetTitleRedNumber(bool cleanUp, unsigned int number, unsigned int interval, unsigned int startPos)
{
	if (cleanUp) 
		this->RemoveAllChildren(true);
	
	std::vector<unsigned int> bits;
	this->NumberBits(number, bits);
	
	for (unsigned int i = 0; i < bits.size(); i++) 
	{
		unsigned int bit = bits.at(i);
		NDPicture* pic = PictureNumber::SharedInstance()->TitleRedNumber(bit);
		if (pic) 
		{
			NDUIImage* image = new NDUIImage();
			image->Initialization();
			image->SetPicture(pic);
			image->SetFrameRect(CCRectMake(startPos + i * PictureNumber::SharedInstance()->GetTitleRedNumberSize().width + interval, 0, 
										   pic->GetSize().width, pic->GetSize().height));
			this->AddChild(image);
		}
	}
	
	return (bits.size() * (PictureNumber::SharedInstance()->GetTitleRedNumberSize().width + interval));
}

void ImageNumber::SetTitleRedNumber(unsigned int number, unsigned int interval)
{
	m_size.width = this->SetTitleRedNumber(true, number, interval, 0);
	m_size.height = PictureNumber::SharedInstance()->GetTitleRedNumberSize().height;
}

void ImageNumber::SetTitleRedTwoNumber(unsigned int number1, unsigned int number2, unsigned int interval)
{
	unsigned int len = this->SetTitleRedNumber(true, number1, interval, 0);
	
	NDPicture* pic = PictureNumber::SharedInstance()->TitleSplit();
	if (pic) 
	{
		NDUIImage* image = new NDUIImage();
		image->Initialization();
		image->SetPicture(pic);
		image->SetFrameRect(CCRectMake(len, 0, 
									   pic->GetSize().width, pic->GetSize().height));
		this->AddChild(image);
		
		len += interval+pic->GetSize().width;
	}
	
	m_size.width = len + this->SetTitleRedNumber(false, number2, interval, len);
	m_size.height = PictureNumber::SharedInstance()->GetTitleRedNumberSize().height;
}

unsigned int ImageNumber::exp(unsigned int value, unsigned int n)
{
	unsigned int result = 1;
	for (unsigned int i = 0; i < n; i++) 
	{
		result *= value;
	}
	return result;
}

void ImageNumber::NumberBits(unsigned int number, std::vector<unsigned int>& bits)
{
	bits.clear();
	NSString *strNum = [NSString stringWithFormat:@"%u", number];
	unsigned int len = strNum.length;
	
	if (number == 0) 
		bits.push_back(0);
	else 
		for (int i = len; i > 0; i--) 
		{
			unsigned int v = (number / exp(10, i-1)) % 10;
			bits.push_back(v);
		}
}

void ImageNumber::SetSmallRedNumber(int number, bool bWithSign)
{
	this->RemoveAllChildren(true);
	
	int startPos = 0;
	
	if (bWithSign) // 显示符号
	{
		int pciIndex = 11;
		if (number > 0)
		{
			pciIndex = 11;
		}
		else
		{
			pciIndex = 12;
		}
		
		NDPicture* picSign = PictureNumber::SharedInstance()->SmallRed(pciIndex);
		if (picSign)
		{
			NDUIImage* image = new NDUIImage();
			image->Initialization();
			image->SetPicture(picSign);
			image->SetFrameRect(CCRectMake(startPos, 0, picSign->GetSize().width, picSign->GetSize().height));
			this->AddChild(image);
			startPos += picSign->GetSize().width;
		}
	}
	
	number = abs(number);
	
	std::vector<unsigned int> bits;
	this->NumberBits(number, bits);
	
	for (unsigned int i = 0; i < bits.size(); i++) 
	{
		unsigned int bit = bits.at(i);
		NDPicture* pic = PictureNumber::SharedInstance()->SmallRed(bit);
		if (pic) 
		{
			NDUIImage* image = new NDUIImage();
			image->Initialization();
			image->SetPicture(pic);
			image->SetFrameRect(CCRectMake(startPos + i * PictureNumber::SharedInstance()->GetSmallRedSize().width, 0, 
						       pic->GetSize().width, pic->GetSize().height));
			this->AddChild(image);
		}
	}
	
	m_size.width = bits.size() * PictureNumber::SharedInstance()->GetSmallRedSize().width;
	m_size.height = PictureNumber::SharedInstance()->GetSmallRedSize().height;
}

void ImageNumber::SetSmallRedTwoNumber(uint num1, uint num2)
{
	bool bVisible = this->IsVisibled();
	
	this->RemoveAllChildren(true);
	
	int startPos = 0;
	
	std::vector<unsigned int> bits;
	// 数字1
	this->NumberBits(num1, bits);
	for (unsigned int i = 0; i < bits.size(); i++) 
	{
		unsigned int bit = bits.at(i);
		NDPicture* pic = PictureNumber::SharedInstance()->SmallRed(bit);
		if (pic) 
		{
			NDUIImage* image = new NDUIImage();
			image->Initialization();
			image->SetPicture(pic);
			image->SetFrameRect(CCRectMake(startPos, 0, 
										   pic->GetSize().width, pic->GetSize().height));
			this->AddChild(image);
			image->SetVisible(bVisible);
			startPos += pic->GetSize().width;
		}
	}
	
	// 斜线
	NDPicture* picSlash = PictureNumber::SharedInstance()->SmallRed(10);
	if (picSlash) 
	{
		NDUIImage* image = new NDUIImage();
		image->Initialization();
		image->SetPicture(picSlash);
		image->SetFrameRect(CCRectMake(startPos, 0, 
									   picSlash->GetSize().width, picSlash->GetSize().height));
		this->AddChild(image);
		image->SetVisible(bVisible);
		startPos += picSlash->GetSize().width;
	}
	
	this->NumberBits(num2, bits);
	for (unsigned int i = 0; i < bits.size(); i++) 
	{
		unsigned int bit = bits.at(i);
		NDPicture* pic = PictureNumber::SharedInstance()->SmallRed(bit);
		if (pic) 
		{
			NDUIImage* image = new NDUIImage();
			image->Initialization();
			image->SetPicture(pic);
			image->SetFrameRect(CCRectMake(startPos, 0, 
										   pic->GetSize().width, pic->GetSize().height));
			this->AddChild(image);
			image->SetVisible(bVisible);
			startPos += pic->GetSize().width;
		}
	}
	
	m_size.width = startPos;
	m_size.height = PictureNumber::SharedInstance()->GetSmallRedSize().height;
}

void ImageNumber::SetSmallWhiteNumber(int number, bool bWithSign)
{
	this->RemoveAllChildren(true);
	
	int startPos = 0;
	
	if (bWithSign) // 显示符号
	{
		int pciIndex = 11;
		if (number > 0)
		{
			pciIndex = 11;
		}
		else
		{
			pciIndex = 12;
		}
		
		NDPicture* picSign = PictureNumber::SharedInstance()->SmallWhite(pciIndex);
		if (picSign)
		{
			NDUIImage* image = new NDUIImage();
			image->Initialization();
			image->SetPicture(picSign);
			image->SetFrameRect(CCRectMake(startPos, 0, picSign->GetSize().width, picSign->GetSize().height));
			this->AddChild(image);
			startPos += picSign->GetSize().width;
		}
	}
	
	number = abs(number);
	
	std::vector<unsigned int> bits;
	this->NumberBits(number, bits);
	
	for (unsigned int i = 0; i < bits.size(); i++) 
	{
		unsigned int bit = bits.at(i);
		NDPicture* pic = PictureNumber::SharedInstance()->SmallWhite(bit);
		if (pic) 
		{
			NDUIImage* image = new NDUIImage();
			image->Initialization();
			image->SetPicture(pic);
			image->SetFrameRect(CCRectMake(startPos + i * PictureNumber::SharedInstance()->GetSmallWhiteSize().width, 0, pic->GetSize().width, pic->GetSize().height));
			this->AddChild(image);
		}
	}
	
	m_size.width = bits.size() * PictureNumber::SharedInstance()->GetSmallWhiteSize().width;
	m_size.height = PictureNumber::SharedInstance()->GetSmallWhiteSize().height;
}

void ImageNumber::SetBigGreenNumber(int number, bool bWithSign)
{
	this->RemoveAllChildren(true);
	
	int startPos = 0;
	
	if (bWithSign) // 显示符号
	{
		int pciIndex = 11;
		if (number > 0)
		{
			pciIndex = 11;
		}
		else
		{
			pciIndex = 12;
		}
		
		NDPicture* picSign = PictureNumber::SharedInstance()->BigGreen(pciIndex);
		if (picSign)
		{
			NDUIImage* image = new NDUIImage();
			image->Initialization();
			image->SetPicture(picSign);
			image->SetFrameRect(CCRectMake(startPos, 0, picSign->GetSize().width, picSign->GetSize().height));
			this->AddChild(image);
			startPos += picSign->GetSize().width;
		}
	}
	
	number = abs(number);
	
	std::vector<unsigned int> bits;
	this->NumberBits(number, bits);
	
	for (unsigned int i = 0; i < bits.size(); i++) 
	{
		unsigned int bit = bits.at(i);
		NDPicture* pic = PictureNumber::SharedInstance()->BigGreen(bit);
		if (pic) 
		{
			NDUIImage* image = new NDUIImage();
			image->Initialization();
			image->SetPicture(pic);
			image->SetFrameRect(CCRectMake(startPos + i * PictureNumber::SharedInstance()->GetBigRedSize().width, 0, 
						       pic->GetSize().width, pic->GetSize().height));
			this->AddChild(image);
		}
	}
	
	m_size.width = startPos + PictureNumber::SharedInstance()->GetBigRedSize().width;
	m_size.height = PictureNumber::SharedInstance()->GetBigRedSize().height;
}

void ImageNumber::SetSmallGoldNumber(int num)
{
	this->RemoveAllChildren(true);
	
	int startPos = 0;
	
	num = abs(num);
	
	std::vector<unsigned int> bits;
	this->NumberBits(num, bits);
	
	for (unsigned int i = 0; i < bits.size(); i++) 
	{
		unsigned int bit = bits.at(i);
		NDPicture* pic = PictureNumber::SharedInstance()->SmallGold(bit);
		if (pic) 
		{
			NDUIImage* image = new NDUIImage();
			image->Initialization();
			image->SetPicture(pic);
			image->SetFrameRect(CCRectMake(startPos + i * PictureNumber::SharedInstance()->GetSmallGoldSize().width, 0, 
										   pic->GetSize().width, pic->GetSize().height));
			this->AddChild(image);
		}
	}
	
	m_size.width = startPos + PictureNumber::SharedInstance()->GetSmallGoldSize().width;
	m_size.height = PictureNumber::SharedInstance()->GetSmallGoldSize().height;
}

void ImageNumber::SetBigRedNumber(int number, bool bWithSign)
{
	this->RemoveAllChildren(true);
	
	int startPos = 0;
	
	if (bWithSign) // 显示符号
	{
		int pciIndex = 11;
		if (number > 0)
		{
			pciIndex = 11;
		}
		else
		{
			pciIndex = 12;
		}

		NDPicture* picSign = PictureNumber::SharedInstance()->BigRed(pciIndex);
		if (picSign)
		{
			NDUIImage* image = new NDUIImage();
			image->Initialization();
			image->SetPicture(picSign);
			image->SetFrameRect(CCRectMake(startPos, 0, picSign->GetSize().width, picSign->GetSize().height));
			this->AddChild(image);
			startPos += picSign->GetSize().width;
		}
	}
	
	number = abs(number);
	
	std::vector<unsigned int> bits;
	this->NumberBits(number, bits);
	
	for (unsigned int i = 0; i < bits.size(); i++) 
	{
		unsigned int bit = bits.at(i);
		NDPicture* pic = PictureNumber::SharedInstance()->BigRed(bit);
		if (pic) 
		{
			NDUIImage* image = new NDUIImage();
			image->Initialization();
			image->SetPicture(pic);
			image->SetFrameRect(CCRectMake(startPos + i * PictureNumber::SharedInstance()->GetBigRedSize().width, 0, 
						       pic->GetSize().width, pic->GetSize().height));
			this->AddChild(image);
		}
	}
	
	m_size.width = startPos + PictureNumber::SharedInstance()->GetBigRedSize().width;
	m_size.height = PictureNumber::SharedInstance()->GetBigRedSize().height;
}

void ImageNumber::SetBigRedTwoNumber(int number1, int number2)
{
	this->RemoveAllChildren(true);
	
	int startPos = 0;
	
	number1 = abs(number1);
	number2 = abs(number2);
	
	std::vector<unsigned int> bits;
	// 数字1
	this->NumberBits(number1, bits);
	for (unsigned int i = 0; i < bits.size(); i++) 
	{
		unsigned int bit = bits.at(i);
		NDPicture* pic = PictureNumber::SharedInstance()->BigRed(bit);
		if (pic) 
		{
			NDUIImage* image = new NDUIImage();
			image->Initialization();
			image->SetPicture(pic);
			image->SetFrameRect(CCRectMake(startPos, 0, 
						       pic->GetSize().width, pic->GetSize().height));
			this->AddChild(image);
			startPos += pic->GetSize().width;
		}
	}
	
	// 斜线
	NDPicture* picSlash = PictureNumber::SharedInstance()->BigRed(10);
	if (picSlash) 
	{
		NDUIImage* image = new NDUIImage();
		image->Initialization();
		image->SetPicture(picSlash);
		image->SetFrameRect(CCRectMake(startPos, 0, 
					       picSlash->GetSize().width, picSlash->GetSize().height));
		this->AddChild(image);
		startPos += picSlash->GetSize().width;
	}
		
	this->NumberBits(number2, bits);
	for (unsigned int i = 0; i < bits.size(); i++) 
	{
		unsigned int bit = bits.at(i);
		NDPicture* pic = PictureNumber::SharedInstance()->BigRed(bit);
		if (pic) 
		{
			NDUIImage* image = new NDUIImage();
			image->Initialization();
			image->SetPicture(pic);
			image->SetFrameRect(CCRectMake(startPos, 0, 
						       pic->GetSize().width, pic->GetSize().height));
			this->AddChild(image);
			startPos += pic->GetSize().width;
		}
	}
	
	m_size.width = startPos;
	m_size.height = PictureNumber::SharedInstance()->GetBigRedSize().height;
}



