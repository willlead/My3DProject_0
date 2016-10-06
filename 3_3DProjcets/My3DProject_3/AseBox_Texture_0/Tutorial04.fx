//--------------------------------------------------------------------------------------
// File: Tutorial04.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D txDiffuse: register (t0);
SamplerState samLinear: register (s0);

cbuffer ConstantBuffer : register( b0 )
{
	matrix World;
	matrix View;
	matrix Projection;
	float4 vMeshColor;
}

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 p : POSITION;
	//float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXCOORD;
};


struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
//	float4 Norm : NORMAL0;
    float4 Color : COLOR;
	float2 Tex : TEXCOORD;
};



//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS( VS_INPUT input )
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Pos = mul(input.p, World );
    output.Pos = mul( output.Pos, View );
    output.Pos = mul( output.Pos, Projection );
//	output.Norm = input.n    
	output.Tex = input.t;
	output.Color = input.c;// *g_MeshColor;
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( VS_OUTPUT input ) : SV_Target
{
  // return input.Color;
	return txDiffuse.Sample(samLinear, input.Tex) * vMeshColor;
}
