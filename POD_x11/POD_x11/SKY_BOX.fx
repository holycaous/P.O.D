//***************************************************************************************
// color.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************

#include "UtillFX.fx"

SkyBoxOut VS(SkyBoxIn vin)
{
	SkyBoxOut vout;
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorld).xyzz;	// WVP

	// 큐브맵의 lookup vector로 로컬좌표를 사용한다.
	vout.PosL = vin.PosL;

	return vout;
}

PS_GBUFFER_OUT PS(SkyBoxOut pin)/* : SV_Target*/
{
	PS_GBUFFER_OUT Out;

	// 출력		
	Out.Depth    = float4(0.0f, 0.0f, 0.0f, 0.0f);
	Out.Color    = gSkyBox.Sample(samLinear, pin.PosL);
	Out.Normal   = gSkyBox.Sample(samLinear, pin.PosL);
	Out.Position = pin.PosH;
	Out.Specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	return Out;
}

technique11 SkyBoxTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));

		SetRasterizerState(NoCull);
		SetDepthStencilState(LessEqualDSS, 0);
	}
}

technique11 SDSkyBoxTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));

		SetRasterizerState(NoCull);
		SetDepthStencilState(LessEqualDSS, 0);
	}
}

