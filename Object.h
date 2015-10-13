#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <directxmath.h>

using namespace DirectX;

struct MeshData
{
	ID3D11Buffer * VertexBuffer;
	ID3D11Buffer * IndexBuffer;
	UINT VBStride;
	UINT VBOffset;
	UINT IndexCount;
};

class Object
{
//variables
private:
	//shape
	MeshData _meshData;

	//transformations
	XMMATRIX _rotation, _scale, _translation, _final;
	XMFLOAT4X4 _world;

//functions
private:
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);
	void SetTranslation(float x, float y, float z);

public:
	Object(void);
	~Object(void);

	void Update(float elapsed);
	void Draw(ID3D11Device * device, ID3D11DeviceContext * context);
};