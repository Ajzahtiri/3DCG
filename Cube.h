#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <directxmath.h>
#include "Structures.h"


using namespace DirectX;

class Cube
{
private:
	MeshData _meshData;

	//transformations
	/*XMMATRIX _scale;
	XMMATRIX _rotation;
	XMMATRIX _translation;
	XMMATRIX _transformation;*/
	
	XMFLOAT4X4 _scale, _rotation, _translation, _transformation;

	XMFLOAT4X4 _world;
	
	HRESULT InitVertexBuffer(ID3D11Device* d);
	HRESULT InitIndexBuffer(ID3D11Device* d);
	void CalculateAveragedNormals(XMFLOAT3& normalOut, XMFLOAT3& startPos, XMFLOAT3& v1, XMFLOAT3& v2, XMFLOAT3& v3);

public:
	Cube();
	void Initialise(ID3D11Device* d);
	~Cube(void);
	XMFLOAT4X4 GetWorld() const { return _world; };
	

	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);
	void SetTranslation(float x, float y, float z);
	void SetTransformation();

	void Update(float t);
	void Draw(ID3D11Device* device, ID3D11DeviceContext * context, ID3D11Buffer * buffer, ConstantBuffer * cb, ID3D11VertexShader * vs, ID3D11PixelShader * ps);
};