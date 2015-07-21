//--------------------------------------------------------------------------------------
// File: Tutorial02.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
//float4 VS( float4 Pos : POSITION ) : SV_POSITION
//{
//    return Pos;
//}
void VS( float4 Pos : POSITION,
		float4 Color : COLOR,
		out float4 oPos : SV_POSITION,
		out float4 oColor : COLOR)
{
    oPos = Pos;
	oColor = Color;
}
                                                                                            
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( float4 Pos : SV_POSITION, float4 Color : COLOR ) : SV_Target
{
    return Color;   // Yellow, with Alpha = 1
}


//--------------------------------------------------------------------------------------
technique10 Render
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}
