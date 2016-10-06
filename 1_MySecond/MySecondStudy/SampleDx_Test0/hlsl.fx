//--------------------------------------------------------------------------------------
// File: hstl.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register(b0) { //b0 레지스터로 cbuffer를 받는다.
	matrix   World;      //월드 행렬
	matrix   View;      //뷰 행렬
	matrix   Projection;   //투영 행렬
};

struct V_IN {
	float4 p : POSITION;
	float4 c0 : COLORa;
};

struct V_OUT {
	float4 p : SV_POSITION;
	float4 c0 : COLORa;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
V_OUT VShader(V_IN vIn)// : SV_POSITION //x,y,z,w. w는 1로 들어옴
{
	V_OUT vOut;
	//정점의 위치 * 행렬
	vOut.p = mul(vIn.p, World);
	vOut.p = mul(vOut.p, View);
	vOut.p = mul(vOut.p, Projection);

	vOut.c0 = vIn.c0;
	return vOut;
}
//정점단위로 호출

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(V_OUT vOut) : SV_Target
{
	return vOut.c0;
//return float4(1.0f, 1.0f, 0.5f, 1.0f);    // Yellow, with Alpha = 1
}
//만들어진 정점 안을 위의 색으로 채움, 픽셀단위