#pragma once
class cMapManager : public cSingleton<cMapManager>
{
public:
	// ���� ���� �� �� ����
	string mCurMapName;

	// ���� ���� �� ť��� ����
	string mCurCubeMapName;

	// �Ϲ� �� ����
	map<string, MapINFO> mData;

	// ť�� ������
	map<string, ID3D11ShaderResourceView*> mCubemap;
public:
	~cMapManager()
	{
		ClearClass();
	}

	// ���� �� �̸�
	void SetMapStage(string _mapName)
	{
		mCurMapName = _mapName;
	}

	// ���� ť��� �̸�
	void SetCubeMapStage(string _mapName)
	{
		mCurCubeMapName = _mapName;
	}
	
	// ����Ʈ�� ��������
	float GetHeightMap(float& _x, float& _z)
	{
		return mData[mCurMapName].GetHeight(_x, _z);
	}
	
	// ť��� ���ҽ� �� �����ϱ�
	void CreateRSV(string& _name, string& _FilePos)
	{
		wstring _WsTexName;
		StringToWchar_t(_FilePos, _WsTexName);

		HR(D3DX11CreateShaderResourceViewFromFile(cCoreStorage::GetInstance()->md3dDevice, _WsTexName.c_str(), 0, 0, &mCubemap[_name], 0));
	}

	// ť��� ��������
	ID3D11ShaderResourceView* GetCubeMap()
	{
		return mCubemap[mCurCubeMapName];
	}

	void ClearClass()
	{
		// �Ϲ� �𵨵����ʹ� �����͸� ���� ���̱⿡, mAllModelData���� �������ְ� ����.

		// ť�� �� ����
		for (auto itor = mCubemap.begin(); itor != mCubemap.end(); ++itor)
			ReleaseCOM(itor->second);
		mCubemap.clear();
	}

	// ��ȯ �Լ�
	void StringToWchar_t(string& _string, wstring& _wstring)
	{
		// ��ȯ
		for (unsigned int i = 0; i < _string.length(); ++i)
			_wstring += wchar_t(_string[i]);

		// ������
		_wstring += wchar_t('\0');
	}
};

