#pragma once
#include "LightHelper.h"

class cLightManager : public cSingleton<cLightManager>
{
public:
	// ����Ʈ
	DirectionalLight mSunDirLight;		// �¾�

	PointLight       mPointLight;       // ��� �뵵 ������
	SpotLight        mPlayerSpotLight;  // �÷��̾� ������

public:
	void Init()
	{
		// ���� ����Ʈ�� 1������ ������ @@@@@@@@@@
		// ���� �ִ� ����Ʈ�� �״�� ����Ǵ� �� -_-;;;
		
		//// ����Ʈ �ʱ�ȭ
		//// Directional light. (���� ����)
		//mSunDirLight.Ambient      = XMFLOAT4(0.30f, 0.25f, 0.20f, 1.0f);
		//mSunDirLight.Diffuse      = XMFLOAT4(0.40f, 0.35f, 0.30f, 1.0f);
		//mSunDirLight.Specular     = XMFLOAT4(0.15f, 0.10f, 0.05f, 1.0f);
		//mSunDirLight.Direction    = XMFLOAT3(1.0f ,-1.0f ,-1.0f);

		// ����Ʈ �ʱ�ȭ
		// Directional light.
		mSunDirLight.Ambient   = XMFLOAT4(0.30f, 0.30f, 0.30f, 1.0f);
		mSunDirLight.Diffuse   = XMFLOAT4(0.40f, 0.40f, 0.40f, 1.0f);
		mSunDirLight.Specular  = XMFLOAT4(0.10f, 0.10f, 0.10f, 1.0f);
		mSunDirLight.Direction = XMFLOAT3(1.0f, -1.0f, -1.0f);
		
		// Point light--position is changed every frame to animate in UpdateScene function.
		// ���� �׽�Ʈ��
		//mPointLight.Ambient       = XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
		//mPointLight.Diffuse       = XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
		//mPointLight.Specular      = XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);

		mPointLight.Ambient       = XMFLOAT4(0.50f, 0.50f, 0.50f, 1.0f);
		mPointLight.Diffuse       = XMFLOAT4(0.50f, 0.50f, 0.50f, 1.0f);
		mPointLight.Specular      = XMFLOAT4(0.70f, 0.70f, 0.70f, 1.0f);

		mPointLight.Position      = XMFLOAT3(0.0f, 0.0f, 0.0f);
		mPointLight.Att           = XMFLOAT3(0.7f, 0.7f, 0.7f);
		mPointLight.Range         = 100.0f;

		// Spot light--position and direction changed every frame to animate in UpdateScene function.
		mPlayerSpotLight.Ambient  = XMFLOAT4(0.50f, 0.55f, 0.50f, 1.0f);
		mPlayerSpotLight.Diffuse  = XMFLOAT4(0.80f, 0.75f, 0.70f, 1.0f);
		mPlayerSpotLight.Specular = XMFLOAT4(0.40f, 0.45f, 0.40f, 1.0f);
		mPlayerSpotLight.Att      = XMFLOAT3(1.0f, 0.0f, 0.0f);
		mPlayerSpotLight.Spot     = 125.0f;
		mPlayerSpotLight.Range    = 700.0f;
	}

	void ClearClass()
	{


	}

};

