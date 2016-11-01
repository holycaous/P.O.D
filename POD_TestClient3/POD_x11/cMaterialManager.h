#pragma once

class cMaterialManager : public cSingleton<cMaterialManager>
{
	// ���̴� �Ŵ���
	cShaderManager* mShader = cShaderManager::GetInstance();

public:
	// ����
	map<int, Material> mMat;

public:
	// �ʱ�ȭ
	void Init()
	{
		// �⺻ ���� ����
		Material tMat;

		// ���
		tMat.Ambient  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		tMat.Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		tMat.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		mMat[e_MatWhite] = tMat;
 
		// ����
		tMat.Ambient  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		tMat.Diffuse  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		tMat.Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		mMat[e_MatBlack] = tMat;

		// ����
		tMat.Ambient  = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		tMat.Diffuse  = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		tMat.Specular = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		mMat[e_MatRed] = tMat;

		// �ʷ�
		tMat.Ambient  = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
		tMat.Diffuse  = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
		tMat.Specular = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
		mMat[e_MatGreen] = tMat;

		// �Ķ�
		tMat.Ambient  = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
		tMat.Diffuse  = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
		tMat.Specular = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
		mMat[e_MatBlue] = tMat;

	}

	void ClearClass()
	{
		// ���� ����
		mMat.clear();


	}

};

