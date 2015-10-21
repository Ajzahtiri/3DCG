#pragma once
#include <DirectXMath.h>

struct Light
{
	Light()
	{
		ZeroMemory(this, sizeof(Light));
	}

	XMFLOAT3 mLightVecW;
	XMFLOAT4 mDiffuseMtrl;
	XMFLOAT4 mDiffuseLight;
	XMFLOAT3 mAmbientMtrl;
	XMFLOAT3 mAmbientLight;
};

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
	XMFLOAT3 Normal;
};

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
	Light	mLight;
};