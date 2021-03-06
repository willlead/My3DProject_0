//--------------------------------------------------------------------------------------
// File: Tutorial02.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register(b0) // 0번 슬롯으로 받는다.
{
	matrix  World;
	matrix  View;
	matrix  Projection;
}

struct V_in
{
	float4 p : POSITION;
	float4 c0 : COLORa;
	//float4 c1 : COLORb;
};
struct V_OUT
{
	float4 p : SV_POSITION;
	float4 c0 : KGCA;
	//float4 c1 : GAME;
};
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
// float4  정점을 반환해야 한다.
V_OUT VShader(V_in vIn)//: SV_POSITION
{
	V_OUT vOut;
	//  정점의 위치 * 행렬
	vOut.p = mul(vIn.p, World);
	vOut.p = mul(vOut.p, View);
	vOut.p = mul(vOut.p, Projection);

	vOut.c0 = vIn.c0;
	//vOut.c1 = vIn.c1;
	return vOut;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(V_OUT vOut) : SV_Target
{
	return vOut.c0;//float4(1,1,1,1);
}