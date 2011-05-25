/* with help from luna */

uniform extern float4x4 gWVP;
uniform extern float gTime;
uniform extern texture gTex;
uniform extern int gViewportHeight;
uniform extern float3 gEyePosL;

sampler TexS = sampler_state
{
	Texture = <gTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = POINT;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

struct OutputVS
{
	float4 posH	: POSITION0;
	float2 tex0 : TEXCOORD0;
	float4 col  : COLOR0;
	float  size : PSIZE0;
};

OutputVS ExplosionVS(
	float3 posL : POSITION0,
	float4 color: COLOR0,
	float inittime: TEXCOORD0,
	float mass: TEXCOORD1,
	float psize: PSIZE0)
{
	OutputVS outVS = (OutputVS)0;
	float t = gTime - inittime;
	float3 cpos = posL;
	float3 accel = posL * 2;
	posL = posL + posL*t + 0.5f*mass*10*posL*t*t;
	
	outVS.posH = mul(float4(posL, 1.0f), gWVP);
	
	float d = distance(cpos, gEyePosL);
	float ksize = gViewportHeight*psize/(1.0f + 300.0f*d); 
	
	float div = 1.0f;
	if (t > div) div = t;
	outVS.size = ksize / div;

	color[0] = 1.0f - (t/3.0f);
	color[1] = 1.0f - t;
	
	outVS.col = color;
	
	return outVS;
}

float4 ExplosionPS(
	float4 c    : COLOR0,
	float2 tex0 : TEXCOORD0
	):COLOR0
{
	//return c + tex2D(TexS, tex0);
	return tex2D(TexS, tex0);
}

technique ExplosionTech
{
	pass P0
	{
		vertexShader = compile vs_2_0 ExplosionVS();
		pixelShader = compile ps_2_0 ExplosionPS();
		
		PointSpriteEnable = true;
		
		AlphaBlendEnable = true;
		SrcBlend = One;
		DestBlend = One;
		
		ZWriteEnable = false;
	}
}
