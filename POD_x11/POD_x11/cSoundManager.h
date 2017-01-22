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

	// 브금시작
	void PlayBGM(GAME_STATE_TYPE _state)
	{
		switch (_state)
		{
		case e_MainState:
			PlaySound(mMainStateBGM.c_str(), NULL, SND_ASYNC || SND_LOOP); // 재생
			break;

		case e_PlayState:
			PlaySound(mPlayStateBGM.c_str(), NULL, SND_ASYNC || SND_LOOP); // 재생
			break;

		case e_MapTool:
		default:
			break;
		}
	}

	// 브금 정지
	void StopBGM()
	{
		PlaySound(NULL, 0, 0);
	}

	void ClearClass()
	{
		StopBGM();
	}

	// 사운드 재생
	void PlayESound(string _soundName)
	{
		mSDSystem->playSound(FMOD_CHANNEL_REUSE
			, mESDStorage[_soundName]->mSound
			, false
			, &mESDStorage[_soundName]->mChannel);
	}

	// 이펙트 사운드 정지
	void StopESound(string _soundName)
	{
		mESDStorage[_soundName]->StopSD();
	}

private:
	// BGM 초기화
	void initBGM()
	{
		mMainStateBGM = L"Sound/.wav";
		mPlayStateBGM = L"Sound/.wav";
	}

	// 효과음 초기화
	void initESD()
	{
		// 사운드 시스템 생성
		System_Create(&mSDSystem);
		mSDSystem->init(2, FMOD_INIT_NORMAL, NULL);

		// 효과음 로딩 // string _soundName, string _fileLocation, float _VolumeSize
		//addEffectSound();
	}

	// 효과음 클리어
	void ClearESD()
	{
		// 사운드 해제
		for (auto itor = mESDStorage.begin(); itor != mESDStorage.end(); ++itor)
			SafeDelete(itor->second);

		// 사운드 시스템 닫기 & 해제
		mSDSystem->release();
		mSDSystem->close();
	}

	// 이펙트 파일 생성 (이름, 경로, 볼륨크기)
	void addEffectSound(string _soundName, string _fileLocation, float _VolumeSize)
	{
		mESDStorage[_soundName] = new soundFile();
		mSDSystem->createSound(_fileLocation.c_str()
			, FMOD_LOOP_OFF | FMOD_HARDWARE
			, NULL
			, &mESDStorage[_soundName]->mSound);

		// 플레이?? <-- 필요한지 확인 필요
		//mESDStorage[_EffectSoundName]->mChannel->isPlaying(&IsBGPlaying);

		// 볼륨 설정
		mESDStorage[_soundName]->SetVol(_VolumeSize);
	}
};

