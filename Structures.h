#pragma once
#include <DirectXMath.h>

struct Light
{
	XMFLOAT3	_lightDirection;
	XMFLOAT4	_diffuseMaterial;
	XMFLOAT4	_diffuseLight;
};

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
	XMFLOAT3 Normal;
};

struct ConstantBuffer
{
	XMMATRIX	mWorld;
	XMMATRIX	mView;
	XMMATRIX	mProjection;
	Light		mLight;
};
