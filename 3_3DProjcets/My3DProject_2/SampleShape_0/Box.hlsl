cbuffer cb0
{
	matrix g_matWorld : packoffset(c0);
	matrix g_matView: packoffset(c4);
	matrix g_matProj: packoffset(c8);
	float4  g_MeshColor     : packoffset(c12);
};

struct V_in
{
	float4 p : POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXCOORD;
};
struct V_OUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
// float4  ������ ��ȯ�ؾ� �Ѵ�.
V_OUT VS(V_in vIn)
{
	V_OUT vOut;
	vOut.p = mul(vIn.p, g_matWorld);
	vOut.p = mul(vOut.p, g_matView);
	vOut.p = mul(vOut.p, g_matProj);
	vOut.n = vIn.n;
	vOut.c = vIn.c;
	vOut.t = vIn.t;
	return vOut;
}
// �ؽ�ó ���� ��������(0��)
Texture2D g_txDiffuse : register(t0);
// ���÷� ���� ��������(0��)
SamplerState samLinear : register(s0);
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(V_OUT vIn) : SV_Target
{
	float4 texColor = g_txDiffuse.Sample(samLinear, vIn.t);
	float4 FinalColor = texColor *vIn.c;
	return FinalColor;
}

float4 PS_Shadow(V_OUT vIn) : SV_Target
{
	return float4(0,0,0,0.5f);
}