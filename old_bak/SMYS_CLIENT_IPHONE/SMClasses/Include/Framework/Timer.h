#ifndef TQ_TIMER_HEADER
#define TQ_TIMER_HEADER
//��ʱ��
class ISysTimerDelegate
{
public:
	virtual int OnTimer(int TimerId, void* pParam) = 0;
};

//���ö�ʱ������ʼ(���ض�ʱ����ID)
int SetTimer(unsigned int uElapse/*����*/, void* pParam, ISysTimerDelegate* pCallBack);

//ɾ����ʱ��
bool KillTimer(int nTimerID/*��ʱ��ID*/);

#endif
