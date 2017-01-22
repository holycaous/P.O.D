#pragma once
class cMapManager : public cSingleton<cMapManager>
{
public:
	// 현재 선택 된 맵 상테
	string mCurMapName;

	// 현재 선택 된 큐브맵 상테
	string mCurCubeMapName;

	// 일반 맵 정보
	map<string, MapINFO> mData;

	// 큐브 맵정보
	map<string, ID3D11ShaderResourceView*> mCubemap;
public:
	~cMapManager()
	{
		ClearClass();
	}

	// 현재 맵 이름
	void SetMapStage(string _mapName)
	{
		mCurMapName = _mapName;
	}

	// 현재 큐브맵 이름
	void SetCubeMapStage(string _mapName)
	{
		mCurCubeMapName = _mapName;
	}
	
	// 하이트맵 가져오기
	float GetHeightMap(float& _x, float& _z)
	{
		return mData[mCurMapName].GetHeight(_x, _z);
	}
	
	// 큐브맵 리소스 뷰 생성하기
	void CreateRSV(string& _name, string& _FilePos)
	{
		wstring _WsTexName;
		StringToWchar_t(_FilePos, _WsTexName);

		HR(D3DX11CreateShaderResourceViewFromFile(cCoreStorage::GetInstance()->md3dDevice, _WsTexName.c_str(), 0, 0, &mCubemap[_name], 0));
	}

	// 큐브맵 가져오기
	ID3D11ShaderResourceView* GetCubeMap()
	{
		return mCubemap[mCurCubeMapName];
	}

	void ClearClass()
	{
		// 일반 모델데이터는 포인터만 얻어온 것이기에, mAllModelData에서 삭제해주고 있음.

		// 큐브 맵 삭제
		for (auto itor = mCubemap.begin(); itor != mCubemap.end(); ++itor)
			ReleaseCOM(itor->second);
		mCubemap.clear();
	}

	// 변환 함수
	void StringToWchar_t(string& _string, wstring& _wstring)
	{
		// 변환
		for (unsigned int i = 0; i < _string.length(); ++i)
			_wstring += wchar_t(_string[i]);

		// 마무리
		_wstring += wchar_t('\0');
	}
};

