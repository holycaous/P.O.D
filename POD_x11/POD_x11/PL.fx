//=============================================================================
// Lighting.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Transforms and lights geometry.
//=============================================================================
#include "LightHelper.fx"

PLVertexOut VS(PLVertexIn vin)
{
	PLVertexOut vout;

	// Transform to world space space.
	vout.PosW    = mul(float4(vin.PosL, 1.0f), vin.World).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)vin.World);

	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vout.PosW, 1.0f), gViewProj);

	return vout;
}

PS_GBUFFER_OUT PS(PLVertexOut pin) /*: SV_Target*/
{
	PS_GBUFFER_OUT Out;

	pin.NormalW = normalize(pin.NormalW);

	// 출력		
	Out.Depth        = float4(0, 0, 0, 0);
	Out.Color = gMaterial.Diffuse;
	Out.Normal       = float4(pin.NormalW, 0.0f);
	Out.Position     = float4(pin.PosW, 1.0);
	Out.Specular      = gMaterial.Specular;

	return Out;

}

//float4 PS(PLVertexOut pin) : SV_Target
//{
//	// Interpolating normal can unnormalize it, so normalize it.
//	pin.NormalW = normalize(pin.NormalW);
//
//	float3 toEyeW = normalize(gEyePosW - pin.PosW);
//	
//	// Start with a sum of zero. 
//	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
//	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
//	float4 spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);
//
//	// Sum the light contribution from each light source.
//	float4 A, D, S;
//
//	// 디렉셔널 라이트
//	ComputeDirectionalLight(gMaterial, gDirLight, pin.NormalW, toEyeW, A, D, S);
//	ambient += A;
//	diffuse += D;
//	spec    += S;
//
//	// 포인트 라이트
//	ComputePointLight(gMaterial, gPointLight, pin.PosW, pin.NormalW, toEyeW, A, D, S);
//	ambient += A;
//	diffuse += D;
//	spec    += S;
//
//	// 스폿 라이트
//	ComputeSpotLight(gMaterial, gSpotLight, pin.PosW, pin.NormalW, toEyeW, A, D, S);
//	ambient += A;
//	diffuse += D;
//	spec    += S;
//
//	float4 litColor = ambient + diffuse + spec;
//
//	// Common to take alpha from diffuse material.
//	litColor.a = gMaterial.Diffuse.a;
//
//	return litColor;
//}


technique11 LightTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}



