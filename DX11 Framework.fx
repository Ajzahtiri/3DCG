//--------------------------------------------------------------------------------------
// File: DX11 Framework.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
struct Light
{
	float3 LightVecW;
	float4 DiffuseMtrl;
	float4 DiffuseLight;

	float3 AmbientMtrl;
	float3 AmbientLight;

	//float4 SpecularMtrl;
	//float4 SpecularLight;
	//float  SpecularPower;
};

cbuffer ConstantBuffer : register( b0 )
{
	matrix World;
	matrix View;
	matrix Projection;
	
	//float3 EyePosW;

	Light  light;
}

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR0;
};

//------------------------------------------------------------------------------------
// Vertex Shader - Implements Gouraud Shading using Diffuse lighting only
//------------------------------------------------------------------------------------
VS_OUTPUT VS(float4 Pos : POSITION, float3 Normal : NORMAL)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.Pos = mul(Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);

	// Compute the vector from the vertex to the eye position
	//float3 toEye = normalize(EyePosW - output.Pos.xyz);

	// Convert from local space to world space 
	// W component of vector is 0 as vectors cannot be translated
	float3 normalW = mul(float4(Normal, 0.0f), World).xyz;
	normalW = normalize(normalW);
	float3 lightLecNorm = normalize(light.LightVecW);

	// Compute reflection vector
	float3 r = reflect(-lightLecNorm, normalW);


	// Compute specular lighting
	//float specularAmount = pow(max(dot(r, toEye), 0.0f), light.SpecularPower);
	//float3 specular = specularAmount * (light.SpecularMtrl * light.SpecularLight).rgb;

	// Compute diffuse lighting 
	float diffuseAmount = max(dot(lightLecNorm, normalW), 0.0f);

	float3 diffuse = diffuseAmount * (light.DiffuseMtrl * light.DiffuseLight).rgb;
	output.Color.rgb = diffuse;
	
	// Compute ambient lighting
	float3 ambient = light.AmbientMtrl * light.AmbientLight;
	//specular += specularAmount * (light.SpecularMtrl * light.SpecularLight).rgb;
	//diffuse += diffuseAmount * (light.DiffuseMtrl * light.DiffuseLight).rgb;
	//ambient += (light.AmbientMtrl * light.AmbientLight).rgb;

	// COMBINE
	output.Color.rgb = diffuse + ambient ;
	
	output.Color.a = light.DiffuseMtrl.a; 

	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( VS_OUTPUT input ) : SV_Target
{
    return input.Color;
}
