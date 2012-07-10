#pragma once
#include "IActionDelegate.h"
#include "IAniObj.h"
#include "CGeometry.h"
#include "cTypes.h"

class CDialog;

//��������(ʹ�Ի������ʹ�ö���)
class CActionDelegate:public IActionDelegate
{
public:
    CActionDelegate(CDialog *pObject);
    ~CActionDelegate(void);
    enum ACION_ACTOR_TYPE
    {
        // AAT_SPRITE=0,
		AAT_WNDOBJECT
		//AAT_WNDOBJECT
    };
public:

    //��ȡ�Ƿ�ɼ�
    virtual bool GetIsVisible();

	//���ÿɼ�
    virtual void SetIsVisible(bool var);

    //��ȡλ��
    virtual const CPoint& GetPosition();

    //����λ��
    virtual void SetPosition(const CPoint& newPosition);

	//�Ƿ��Զ��ͷ�
    virtual bool IsAutoRelease();

	//CCActionManager�ڶ�����������Ƿ��Զ��ͷŵ�IActionDelegate����
    virtual void SetAutoRelease(bool bAutoRelease);

	//��ʾ
    bool Show(bool bShowRelative);

	//����X��Y��������ֵ
    void SetScale(float scale);

	//����X��������ֵ
    void SetScaleX(float newScaleX);

	//��ȡY��������ֵ
    float GetScaleY();

	//����Y��������ֵ
    void SetScaleY(float newScaleY);

	//��ȡ����ֵ
    float GetScaleX();

	//��ȡ͸����
    int  GetOpacity(void);

	//����͸����
    void SetOpacity(int iOpacity);

	//��ȡ��ɫֵ
    const Color3B& GetColor(void);

	//������ɫֵ
    void SetColor(const Color3B& color3);

	//��ȡ��תֵ
    float GetRotation();

	//������תֵ
    void SetRotation(float newRotation);

    //Y���귭ת
    void SetFlipY(bool bFlipY);

    //X���귭ת
    void SetFlipX(bool bFlipX);

	//�������ŵ���֡��
    int GetPlayFrameSize();

	//���ö�����֡��
    void SetImageNum(int i);

private:
    void *m_pObject;
    ACION_ACTOR_TYPE m_aaType;
    bool m_bAutoRelease;//�Ƿ��Զ��ͷ�

};
