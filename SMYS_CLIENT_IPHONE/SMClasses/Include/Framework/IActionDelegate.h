#ifndef _IACTIONDELEGATE_HH__
#define _IACTIONDELEGATE_HH__
#include "CGeometry.h"
#include "cTypes.h"
#include "Object.h"


//������ĳ������(���о���Ļ��඼����ʵ�ִ�������нӿ�,����Ӧ���ڶ�������)
class IActionDelegate:public CObject
{
public:

	 //��ʾ
	 virtual bool Show(bool bShowRelative )=0;

     //Y���귭ת
	 virtual void SetFlipY(bool bFlipY)=0;
    
	 //X���귭ת
	 virtual void SetFlipX(bool bFlipX)=0;

	 //�������ŵ���֡��
     virtual int GetPlayFrameSize()=0;

	//���ö�����֡��
	 virtual void SetImageNum(int i)=0;
	
	//����X��Y��������ֵ
	 virtual void SetScale(float scale)=0;

	//����Y��������ֵ
	 virtual void SetScaleX(float newScaleX)=0;

	 //��ȡY��������ֵ
	 virtual float GetScaleY()=0;

	 //����Y��������ֵ
	 virtual void SetScaleY(float newScaleY)=0;
	
	 //��ȡX��������ֵ
	 virtual float GetScaleX()=0;

	//�����Ƿ�ɼ�
	 virtual bool GetIsVisible()=0;

	 //�����Ƿ�ɼ�
	 virtual void SetIsVisible(bool var)=0;

	//��ȡ͸����
	 virtual int GetOpacity(void)=0;

	//����͸����
	 virtual void SetOpacity(int opacity)=0;

	 //��ȡ��ɫֵ
	 virtual const Color3B& GetColor(void)=0;

	 //������ɫֵ
	 virtual void SetColor(const Color3B& color3)=0;

	//��ȡ��תֵ
	 virtual float GetRotation()=0;

	 //������תֵ
	 virtual void SetRotation(float newRotation)=0;

	//��ȡλ��
	 virtual const CPoint& GetPosition()=0;

	//����λ��
	 virtual void SetPosition(const CPoint& newPosition)=0;

	 //CCActionManager�ڶ���������������AutoRelease,���Զ��ͷŵ�IActionDelegate����
	 virtual bool IsAutoRelease() = 0;

	 //CCActionManager�ڶ�����������Ƿ��Զ��ͷŵ�IActionDelegate����
	 virtual void SetAutoRelease(bool bAutoRelease) = 0;

};
#endif //_IACTIONDELEGATE_HH__
