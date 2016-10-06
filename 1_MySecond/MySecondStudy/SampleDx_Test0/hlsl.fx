//--------------------------------------------------------------------------------------
// File: hstl.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register(b0) { //b0 �������ͷ� cbuffer�� �޴´�.
	matrix   World;      //���� ���
	matrix   View;      //�� ���
	matrix   Projection;   //���� ���
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
V_OUT VShader(V_IN vIn)// : SV_POSITION //x,y,z,w. w�� 1�� ����
{
	V_OUT vOut;
	//������ ��ġ * ���
	vOut.p = mul(vIn.p, World);
	vOut.p = mul(vOut.p, View);
	vOut.p = mul(vOut.p, Projection);

	vOut.c0 = vIn.c0;
	return vOut;
}
//���������� ȣ��

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(V_OUT vOut) : SV_Target
{
	return vOut.c0;
//return float4(1.0f, 1.0f, 0.5f, 1.0f);    // Yellow, with Alpha = 1
}
//������� ���� ���� ���� ������ ä��, �ȼ�����