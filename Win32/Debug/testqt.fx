///////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
// 
// FileName : testqt.fx
// Creator  : Miao Kaixiang
// Date     : 2015-7-17 15:00:00
// Comment  : Shader File
//
///////////////////////////////////////////////////////////////

// Vertex Shader
void VS(float4 Pos: POSITION,
	float4 Color: COLOR,
	out float4 oPos: SV_POSITION,
	out float4 oColor: COLOR)
{
    oPos = Pos;
	oColor = Color;
}
                                                                                            
// Pixel Shader
float4 PS(float4 Pos: SV_POSITION, float4 Color: COLOR): SV_Target
{
    return Color;
}

technique10 Render
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_4_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, PS()));
    }
}
