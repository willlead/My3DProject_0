//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D g_txDiffuse: register (t0);
Texture2D g_txSandDiffuse: register (t1);
Texture2D g_txGrassDiffuse: register (t2);
Texture2D g_txRockDiffuse: register (t3);
Texture2D g_txSnowDiffuse: register (t4);

SamplerState samLinear: register (s0);
cbuffer cb0
{
	float4x4	g_matWorld	: packoffset(c0);
	float4x4	g_matView		: packoffset(c4);
	float4x4	g_matProj		: packoffset(c8);
	float  g_fZonePlains : packoffset(c12.x);
	float  g_fZoneHills : packoffset(c12.y);
	float  g_fZoneAlpine : packoffset(c12.z);
	float  g_fZoneMountain : packoffset(c12.w);
};

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 p : POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXCOORD;
};
struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT vIn)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	float4 vWorld = mul(vIn.p, g_matWorld);
	output.p = mul(vWorld, g_matView);
	output.p = mul(output.p, g_matProj);
	output.n = vIn.n;
	output.t = vIn.t * 100;
	output.c = vIn.c;
	//g_fMaxElev = 250*200
	output.c.w = clamp(vIn.p.y / (255 * 20.0f), 0.0f, 1.0f);
	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 TexturePresenceSand(in float2 texCoord,
	in float lowB, //-0.1
	in float upB, // 0.3
	in float opt, // 0.0f
	in float elev) //0.3
{
	//  e < -0.1f || e > 0.3f
	if ((elev < lowB) || (elev > upB))
	{
		return float4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	float4 k = (elev <= opt) ? smoothstep(lowB, opt, elev) : smoothstep(upB, opt, elev);

	return k * g_txSandDiffuse.Sample(samLinear, texCoord);// tex2D(SandTextureSampler, texCoord);
}

float4 TexturePresenceGrass(in float2 texCoord,
	in float lowB,
	in float upB,
	in float opt,
	in float elev)
{
	if ((elev < lowB) || (elev > upB))
	{
		return float4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	float4 k = (elev <= opt) ? smoothstep(lowB, opt, elev) : smoothstep(upB, opt, elev);

	return k * g_txGrassDiffuse.Sample(samLinear, texCoord); //tex2D(GrassTextureSampler, texCoord);
}

float4 TexturePresenceRock(in float2 texCoord,
	in float lowB,
	in float upB,
	in float opt,
	in float elev)
{
	if ((elev < lowB) || (elev > upB))
	{
		return float4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	float4 k = (elev <= opt) ? smoothstep(lowB, opt, elev) : smoothstep(upB, opt, elev);

	return k * g_txRockDiffuse.Sample(samLinear, texCoord); //tex2D(RockTextureSampler, texCoord);
}

float4 TexturePresenceSnow(in float2 texCoord,
	in float lowB,
	in float upB,
	in float opt,
	in float elev)
{
	if ((elev < lowB) || (elev > upB))
	{
		return float4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	float4 k = (elev <= opt) ? smoothstep(lowB, opt, elev) : smoothstep(upB, opt, elev);

	return k * g_txSnowDiffuse.Sample(samLinear, texCoord); //tex2D(SnowTextureSampler, texCoord);
}
float4 PS(VS_OUTPUT vIn) : SV_Target
{
	float4 vColor = float4(0,0,0,1);

	vColor = TexturePresenceSand(vIn.t,	-0.1f,	g_fZoneHills,	g_fZonePlains,	vIn.c.w);
	vColor += TexturePresenceGrass(vIn.t,g_fZonePlains,	g_fZoneAlpine,	g_fZoneHills,vIn.c.w);
	vColor += TexturePresenceRock(vIn.t,g_fZoneHills,g_fZoneMountain,g_fZoneAlpine,	vIn.c.w);
	vColor += TexturePresenceSnow(vIn.t,g_fZoneAlpine,100.1f,g_fZoneMountain,	vIn.c.w);

	vColor *= vIn.c;
	vColor.a = 1.0f;
	return (g_txDiffuse.Sample(samLinear, vIn.t) + vIn.c.w) * vColor * float4(vIn.c.rgb, 1.0f);
}
