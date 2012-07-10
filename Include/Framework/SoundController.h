#if !defined SOUNDENGINE_KERNEL_H__
#define SOUNDENGINE_KERNEL_H__

//��������
class ISoundController
{
public:
	static ISoundController& sharedInstance();
	virtual bool SetupSound() = 0; 

	virtual void ReleaseSounds() = 0;
	
	//ֻ����һ����Ч
	virtual void  PlaySound(const char* filename) = 0; 

	//�ظ�������Ч
	virtual void  PlayMusic(const char* filename="") = 0; 

	virtual void  StopMusic() = 0; 

	virtual void  StopSound() = 0;
	
	//�����Ƿ�����������
	virtual void SetMusicEnabled(bool bEnabled=true) = 0;

	virtual void SetSoundEnabled(bool bEnabled=true) = 0;
	
	virtual void SetMusicVolume(int Vol) = 0;

	virtual void SetSoundVolume(int Vol) = 0;
};
#endif

   
