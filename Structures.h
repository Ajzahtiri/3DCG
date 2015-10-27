#pragma once

#include <DirectXMath.h>
using namespace DirectX;
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

	//XMFLOAT4 mSpecularMtrl;
	//XMFLOAT4 mSpecularLight;
	//FLOAT	 mSpecularPower;
};

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
};

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
	
	//XMFLOAT3 mEyePosW;

	Light	mLight;
};

struct MeshData
{
	ID3D11Buffer * VertexBuffer;
	ID3D11Buffer * IndexBuffer;
	UINT VBStride;
	UINT VBOffset;
	UINT IndexCount;
};
