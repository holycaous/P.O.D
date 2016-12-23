//=============================================================================
// Basic.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Basic effect that currently supports transformations, lighting, and texturing.
//=============================================================================

#include "LightHelper.fx"


// 버텍스
GVertexOut VS(GVertexIn vin)
{
	GVertexOut vout;

	// Transform to world space space.
	vout.PosL = float4(vin.PosL, 1.0f);

	// Output vertex attributes for interpolation across triangle.
	// 어차피 변환결과는 같음.
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTFMtx).xy;

	return vout;
}

// 두번째 매개변수를 통해, 텍스처 사용 유무를 가른다.
float4 PS(GVertexOut pin) : SV_Target
{
	//---------------------------------------------------------//
	// HDR 입히기 
	//---------------------------------------------------------//
	// 색상 샘플 가져 오기
	float3 color = gHDRTex.Sample(samPoint, pin.Tex).xyz;

	// 톤 매핑
	color = ToneMapping(color);

	// LDR 값을 출력합니다.
	return float4(color, 1.0);
}


// 라이트 + 텍스처 버전
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

