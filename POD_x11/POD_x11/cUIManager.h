#pragma once
class cUIManager : public cSingleton<cUIManager>
{
	// 1. UI전용 쉐이더가 필요할듯 (.fx 파일을 말하는것.. 쉐이더 파일로 렌더링도 다르게??)
	// 2. 드로우 매니저에서 setLens 바꿔줘야함
	// 3. 쉐이더 변수로 XMFLOAT4X4 mUiView; 넣어줘야 함.
	// 4. UI 위치시킬 초기위치 지정 (-y 좌표) <-- Cam에 지정되어있음. <-- stdafx 에서 define으로 지정할까..
	// 5. 이곳에서 UI 충돌체크 관장??
	// 6. 스테이트 별로 UI 변경시켜야할 듯? <-- 아니면 하나하나 출력?

public:
	void Init()
	{
		//XMMatrixOrthographicLH // http://blog.naver.com/masca140/220710652000
	}

	// 업데이트 및 충돌체크
	void Update()
	{

	}

	void ClearClass()
	{

	}

};

