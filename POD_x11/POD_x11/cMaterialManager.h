#pragma once

class cMaterialManager : public cSingleton<cMaterialManager>
{
	// 쉐이더 매니저
	cShaderManager* mShader = cShaderManager::GetInstance();

public:
	// 재질
	map<int, Material> mMat;

public:
	// 초기화
	void Init()
	{
		// 기본 재질 생성
		Material tMat;

		// 흰색
		tMat.Ambient  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		tMat.Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		tMat.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		mMat[e_MatWhite] = tMat;
 
		// 검정
		tMat.Ambient  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		tMat.Diffuse  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		tMat.Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		mMat[e_MatBlack] = tMat;

		// 빨강
		tMat.Ambient  = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		tMat.Diffuse  = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		tMat.Specular = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		mMat[e_MatRed] = tMat;

		// 초록
		tMat.Ambient  = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
		tMat.Diffuse  = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
		tMat.Specular = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
		mMat[e_MatGreen] = tMat;

		// 파랑
		tMat.Ambient  = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
		tMat.Diffuse  = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
		tMat.Specular = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
		mMat[e_MatBlue] = tMat;

	}

	void ClearClass()
	{
		// 재질 삭제
		mMat.clear();


	}

};

