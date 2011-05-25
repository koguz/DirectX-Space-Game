/* with help from luna */

uniform extern float4x4 gWorldInverseTranspose;
uniform extern float4x4 gWVP;
uniform extern float4 gDiffuseMtrl;
uniform extern float4 gDiffuseLight;
uniform extern float3 gLightVecW;


struct OutputVS
{
	float4 posH : POSITION0;
	float4 color: COLOR0;
};

OutputVS FontEffectVS(
	float3 posL:POSITION0,
	float3 normalL: NORMAL0
	)
{
	OutputVS outVS = (OutputVS)0;
	float3 normalW = mul(float4(normalL, 0.0f), gWorldInverseTranspose).xyz;
	normalW = normalize(normalW);
	
	float s = max(dot(gLightVecW, normalW), 0.0f);
	outVS.color.rgb = s*(gDiffuseMtrl*gDiffuseLight).rgb;
	outVS.color.a = gDiffuseMtrl.a;
	outVS.posH = mul(float4(posL, 1.0f), gWVP);
	return outVS;
}


float4 FontEffectPS ( float4 c : COLOR0 ) : COLOR
{
	return c;
}


technique FontEffect
{
	pass P0
	{
		vertexShader = compile vs_2_0 FontEffectVS();
		pixelShader  = compile ps_2_0 FontEffectPS();
	}
}