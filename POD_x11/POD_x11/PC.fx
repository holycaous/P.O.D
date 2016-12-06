//***************************************************************************************
// color.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************

#include "UtillFX.fx"

PCVertexOut VS(PCVertexIn vin)
{
	PCVertexOut vout;
	
	// Transform to world space space.
	float3 PosW = mul(float4(vin.PosL, 1.0f), vin.World).xyz;

	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(PosW, 1.0f), gViewProj);

	// Just pass vertex color into the pixel shader.
    vout.Color = vin.Color;
    
    return vout;
}

PS_GBUFFER_OUT PS(PCVertexOut pin)/* : SV_Target*/
{
	PS_GBUFFER_OUT Out;

	// Ãâ·Â		
	Out.Depth        = float4(0.0f, 0.0f, 0.0f, 0.0f);
	Out.Color        = pin.Color;
	Out.Normal       = float4(0.0f, 0.0f, 0.0f, 0.0f);
	Out.Position     = pin.PosH;
	Out.Specular     = float4(0.0f, 0.0f, 0.0f, 0.0f);

	return Out;
}

technique11 ColorTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );

		SetRasterizerState(0);
		SetDepthStencilState(LessDSS, 0);
    }
}
