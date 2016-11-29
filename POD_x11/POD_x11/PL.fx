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

	// Ãâ·Â		
	Out.Depth        = float4(0, 0, 0, 0);
	Out.Color        = gMaterial.Diffuse;
	Out.Normal       = float4(pin.NormalW, 0.0f);
	Out.Position     = float4(pin.PosW, 1.0);
	Out.Specular     = gMaterial.Specular;

	return Out;

}

technique11 LightTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}



