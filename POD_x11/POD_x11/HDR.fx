//=============================================================================
// Basic.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Basic effect that currently supports transformations, lighting, and texturing.
//=============================================================================

#include "LightHelper.fx"


// ���ؽ�
GVertexOut VS(GVertexIn vin)
{
	GVertexOut vout;

	// Transform to world space space.
	vout.PosL = float4(vin.PosL, 1.0f);

	// Output vertex attributes for interpolation across triangle.
	// ������ ��ȯ����� ����.
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTFMtx).xy;

	return vout;
}

// �ι�° �Ű������� ����, �ؽ�ó ��� ������ ������.
float4 PS(GVertexOut pin) : SV_Target
{
	//---------------------------------------------------------//
	// HDR ������ 
	//---------------------------------------------------------//
	// ���� ���� ���� ����
	float3 color = gHDRTex.Sample(samPoint, pin.Tex).xyz;

	// �� ����
	color = ToneMapping(color);

	// LDR ���� ����մϴ�.
	return float4(color, 1.0);
}


// ����Ʈ + �ؽ�ó ����
technique11 FinHDR
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));

		SetRasterizerState(0);
		SetDepthStencilState(LessDSS, 0);
	}
}

