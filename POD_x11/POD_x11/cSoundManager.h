#pragma once
class cSoundManager : public cSingleton<cSoundManager>
{
	wstring mMainStateBGM;
	wstring mPlayStateBGM;

	System* mSDSystem;
	map<string, soundFile*> mESDStorage;
public:
	void Init()
	{
		initBGM();
		initESD();
	}

	// ��ݽ���
	void PlayBGM(GAME_STATE_TYPE _state)
	{
		switch (_state)
		{
		case e_MainState:
			PlaySound(mMainStateBGM.c_str(), NULL, SND_ASYNC || SND_LOOP); // ���
			break;

		case e_PlayState:
			PlaySound(mPlayStateBGM.c_str(), NULL, SND_ASYNC || SND_LOOP); // ���
			break;

		case e_MapTool:
		default:
			break;
		}
	}

	// ��� ����
	void StopBGM()
	{
		PlaySound(NULL, 0, 0);
	}

	void ClearClass()
	{
		StopBGM();
	}

	// ���� ���
	void PlayESound(string _soundName)
	{
		mSDSystem->playSound(FMOD_CHANNEL_REUSE
			, mESDStorage[_soundName]->mSound
			, false
			, &mESDStorage[_soundName]->mChannel);
	}

	// ����Ʈ ���� ����
	void StopESound(string _soundName)
	{
		mESDStorage[_soundName]->StopSD();
	}

private:
	// BGM �ʱ�ȭ
	void initBGM()
	{
		mMainStateBGM = L"Sound/.wav";
		mPlayStateBGM = L"Sound/.wav";
	}

	// ȿ���� �ʱ�ȭ
	void initESD()
	{
		// ���� �ý��� ����
		System_Create(&mSDSystem);
		mSDSystem->init(2, FMOD_INIT_NORMAL, NULL);

		// ȿ���� �ε� // string _soundName, string _fileLocation, float _VolumeSize
		//addEffectSound();
	}

	// ȿ���� Ŭ����
	void ClearESD()
	{
		// ���� ����
		for (auto itor = mESDStorage.begin(); itor != mESDStorage.end(); ++itor)
			SafeDelete(itor->second);

		// ���� �ý��� �ݱ� & ����
		mSDSystem->release();
		mSDSystem->close();
	}

	// ����Ʈ ���� ���� (�̸�, ���, ����ũ��)
	void addEffectSound(string _soundName, string _fileLocation, float _VolumeSize)
	{
		mESDStorage[_soundName] = new soundFile();
		mSDSystem->createSound(_fileLocation.c_str()
			, FMOD_LOOP_OFF | FMOD_HARDWARE
			, NULL
			, &mESDStorage[_soundName]->mSound);

		// �÷���?? <-- �ʿ����� Ȯ�� �ʿ�
		//mESDStorage[_EffectSoundName]->mChannel->isPlaying(&IsBGPlaying);

		// ���� ����
		mESDStorage[_soundName]->SetVol(_VolumeSize);
	}
};

