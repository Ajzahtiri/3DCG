#pragma once

#include "Cube.h"

Cube::Cube(void)
{

}

void Cube::Initialise(ID3D11Device* d)
{
	XMStoreFloat4x4(&_world, XMMatrixIdentity());
	XMStoreFloat4x4(&_scale, XMMatrixIdentity());
	XMStoreFloat4x4(&_rotation, XMMatrixIdentity());

	XMStoreFloat4x4(&_world, XMMatrixIdentity());
	InitVertexBuffer(d);
	InitIndexBuffer(d);
}

Cube::~Cube(void)
{
}


void Cube::SetScale(float x, float y, float z)
{
	XMStoreFloat4x4(&_scale, XMMatrixScaling(x, y, z));
}

void Cube::SetRotation(float x, float y, float z)
{
	XMStoreFloat4x4(&_rotation, XMMatrixRotationX(x) * XMMatrixRotationY(y) * XMMatrixRotationZ(z));
}

void Cube::SetTranslation(float x, float y, float z)
{
	XMStoreFloat4x4(&_translation, XMMatrixTranslation(x, y, z));
}


HRESULT Cube::InitVertexBuffer(ID3D11Device* d)
{
	HRESULT hr;
	// front face
	XMFLOAT3 v0(-1.0f, -1.0f, -1.0f);
	XMFLOAT3 v1(-1.0f, 1.0f, -1.0f);
	XMFLOAT3 v2(1.0f, 1.0f, -1.0f);
	XMFLOAT3 v3(1.0f, -1.0f, -1.0f);
	XMFLOAT3 n0(0.0f, 0.0f, -1.0f);
	XMFLOAT3 n1(0.0f, 0.0f, -1.0f);
	XMFLOAT3 n2(0.0f, 0.0f, -1.0f);
	XMFLOAT3 n3(0.0f, 0.0f, -1.0f);/*
								   XMFLOAT2 tex0(0.0f, 1.0f);
								   XMFLOAT2 tex1(0.0f, 0.0f);
								   XMFLOAT2 tex2(1.0f, 0.0f);
								   XMFLOAT2 tex3(1.0f, 1.0f);*/

								   // back face
	XMFLOAT3 v4(-1.0f, -1.0f, 1.0f);
	XMFLOAT3 v5(1.0f, -1.0f, 1.0f);
	XMFLOAT3 v6(1.0f, 1.0f, 1.0f);
	XMFLOAT3 v7(-1.0f, 1.0f, 1.0f);
	XMFLOAT3 n4(0.0f, 0.0f, 1.0f);
	XMFLOAT3 n5(0.0f, 0.0f, 1.0f);
	XMFLOAT3 n6(0.0f, 0.0f, 1.0f);
	XMFLOAT3 n7(0.0f, 0.0f, 1.0f);/*
								  XMFLOAT2 tex4(1.0f, 1.0f);
								  XMFLOAT2 tex5(0.0f, 1.0f);
								  XMFLOAT2 tex6(0.0f, 0.0f);
								  XMFLOAT2 tex7(1.0f, 0.0f);
								  */
								  // top face
	XMFLOAT3 v8(-1.0f, 1.0f, -1.0f);
	XMFLOAT3 v9(-1.0f, 1.0f, 1.0f);
	XMFLOAT3 v10(1.0f, 1.0f, 1.0f);
	XMFLOAT3 v11(1.0f, 1.0f, -1.0f);
	XMFLOAT3 n8(0.0f, 1.0f, 0.0f);
	XMFLOAT3 n9(0.0f, 1.0f, 0.0f);
	XMFLOAT3 n10(0.0f, 1.0f, 0.0f);
	XMFLOAT3 n11(0.0f, 1.0f, 0.0f);/*
								   XMFLOAT2 tex8(0.0f, 1.0f);
								   XMFLOAT2 tex9(0.0f, 0.0f);
								   XMFLOAT2 tex10(1.0f, 0.0f);
								   XMFLOAT2 tex11(1.0f, 1.0f);*/

								   // bottom face
	XMFLOAT3 v12(-1.0f, -1.0f, -1.0f);
	XMFLOAT3 v13(1.0f, -1.0f, -1.0f);
	XMFLOAT3 v14(1.0f, -1.0f, 1.0f);
	XMFLOAT3 v15(-1.0f, -1.0f, 1.0f);
	XMFLOAT3 n12(0.0f, -1.0f, 0.0f);
	XMFLOAT3 n13(0.0f, -1.0f, 0.0f);
	XMFLOAT3 n14(0.0f, -1.0f, 0.0f);
	XMFLOAT3 n15(0.0f, -1.0f, 0.0f);/*
									XMFLOAT2 tex12(1.0f, 1.0f);
									XMFLOAT2 tex13(0.0f, 1.0f);
									XMFLOAT2 tex14(0.0f, 0.0f);
									XMFLOAT2 tex15(1.0f, 0.0f);
									*/
									// left face
	XMFLOAT3 v16(-1.0f, -1.0f, 1.0f);
	XMFLOAT3 v17(-1.0f, 1.0f, 1.0f);
	XMFLOAT3 v18(-1.0f, 1.0f, -1.0f);
	XMFLOAT3 v19(-1.0f, -1.0f, -1.0f);
	XMFLOAT3 n16(-1.0f, 0.0f, 0.0f);
	XMFLOAT3 n17(-1.0f, 0.0f, 0.0f);
	XMFLOAT3 n18(-1.0f, 0.0f, 0.0f);
	XMFLOAT3 n19(-1.0f, 0.0f, 0.0f);/*
									XMFLOAT2 tex16(0.0f, 1.0f);
									XMFLOAT2 tex17(0.0f, 0.0f);
									XMFLOAT2 tex18(1.0f, 0.0f);
									XMFLOAT2 tex19(1.0f, 1.0f);*/

									// right face
	XMFLOAT3 v20(1.0f, -1.0f, -1.0f);
	XMFLOAT3 v21(1.0f, 1.0f, -1.0f);
	XMFLOAT3 v22(1.0f, 1.0f, 1.0f);
	XMFLOAT3 v23(1.0f, -1.0f, 1.0f);
	XMFLOAT3 n20(1.0f, 0.0f, 0.0f);
	XMFLOAT3 n21(1.0f, 0.0f, 0.0f);
	XMFLOAT3 n22(1.0f, 0.0f, 0.0f);
	XMFLOAT3 n23(1.0f, 0.0f, 0.0f);/*
								   XMFLOAT2 tex20(0.0f, 1.0f);
								   XMFLOAT2 tex21(0.0f, 0.0f);
								   XMFLOAT2 tex22(1.0f, 0.0f);
								   XMFLOAT2 tex23(1.0f, 1.0f);	*/

	CalculateAveragedNormals(n1, v1, v2, v0, v9);
	n8 = n1;
	n18 = n1;

	CalculateAveragedNormals(n21, v21, v22, v20, v8);
	n11 = n21;
	n2 = n21;

	CalculateAveragedNormals(n6, v6, v7, v5, v11);
	n10 = n6;
	n22 = n6;

	CalculateAveragedNormals(n17, v17, v18, v16, v10);
	n9 = n17;
	n7 = n17;

	CalculateAveragedNormals(n12, v12, v13, v15, v1);
	n0 = n12;
	n19 = n12;

	CalculateAveragedNormals(n13, v13, v14, v12, v21);
	n3 = n13;
	n20 = n13;

	CalculateAveragedNormals(n14, v14, v15, v13, v6);
	n5 = n14;
	n23 = n14;

	CalculateAveragedNormals(n15, v15, v12, v14, v17);
	n16 = n15;
	n4 = n15;

	SimpleVertex v[] =
	{
		{ v0, n0	},
		{ v1, n1	},
		{ v2, n2	},
		{ v3, n3	},
		{ v4, n4	},
		{ v5, n5	},
		{ v6, n6	},
		{ v7, n7	},
		{ v8, n8	},
		{ v9, n9	},
		{ v10, n10	},
		{ v11, n11	},
		{ v12, n12	},
		{ v13, n13	},
		{ v14, n14	},
		{ v15, n15	},
		{ v16, n16	},
		{ v17, n17	},
		{ v18, n18	},
		{ v19, n19	},
		{ v20, n20	},
		{ v21, n21	},
		{ v22, n22	},
		{ v23, n23	},
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * (sizeof(v) / sizeof(SimpleVertex));
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = v;

	hr = d->CreateBuffer(&bd, &InitData, &_meshData.VertexBuffer);
	_meshData.VBOffset = 0;
	_meshData.VBStride = sizeof(SimpleVertex);

	return S_OK;
}

HRESULT Cube::InitIndexBuffer(ID3D11Device* d)
{
	HRESULT hr;

	// Create index buffer
	WORD indices[] =
	{
		0, 1, 2,
		0, 2, 3,
		4, 5, 6,
		4, 6, 7,
		8, 9, 10,
		8, 10, 11,
		12, 13, 14,
		12, 14, 15,
		16, 17, 18,
		16, 18, 19,
		20, 21, 22,
		20, 22, 23,
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * 36;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = indices;
	hr = d->CreateBuffer(&bd, &InitData, &_meshData.IndexBuffer);
	_meshData.IndexCount = sizeof(indices) / sizeof(WORD);
	return S_OK;
}

void Cube::CalculateAveragedNormals(XMFLOAT3& normalOut, XMFLOAT3& startPos, XMFLOAT3& v1, XMFLOAT3& v2, XMFLOAT3& v3)
{
	XMVECTOR intersect1, intersect2, intersect3;
	XMVECTOR u1, u2;

	u1 = XMLoadFloat3(&v1) - XMLoadFloat3(&startPos);
	u2 = XMLoadFloat3(&v2) - XMLoadFloat3(&startPos);
	intersect1 = XMVector3Cross(u1, u2);

	u1 = XMLoadFloat3(&v3) - XMLoadFloat3(&startPos);
	u2 = XMLoadFloat3(&v1) - XMLoadFloat3(&startPos);
	intersect2 = XMVector3Cross(u1, u2);

	u1 = XMLoadFloat3(&v2) - XMLoadFloat3(&startPos);
	u2 = XMLoadFloat3(&v3) - XMLoadFloat3(&startPos);
	intersect3 = XMVector3Cross(u1, u2);

	XMVECTOR normal;
	normal = (intersect1 + intersect2 + intersect3) / 3.0f;
	normal = XMVector3Normalize(normal);
	XMStoreFloat3(&normalOut, normal);
}

void Cube::Update(float t)
{	
	XMMATRIX scale = XMLoadFloat4x4(&_scale);
	XMMATRIX rotate = XMLoadFloat4x4(&_rotation);
	XMMATRIX translate = XMLoadFloat4x4(&_translation);
	rotate = XMMatrixRotationY(t);
	XMStoreFloat4x4(&_world, scale * rotate * translate);
}


void Cube::Draw(ID3D11Device * device, ID3D11DeviceContext * context, ID3D11Buffer * buffer, ConstantBuffer * cb, ID3D11VertexShader * vs, ID3D11PixelShader * ps)
{
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->PSSetConstantBuffers(0, 1, &buffer);
	context->VSSetConstantBuffers(0, 1, &buffer);
	context->VSSetShader(vs, NULL, 0);
	context->PSSetShader(ps, NULL, 0);
	context->IASetVertexBuffers(0, 1, &_meshData.VertexBuffer, &_meshData.VBStride, &_meshData.VBOffset);
	context->IASetIndexBuffer(_meshData.IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	context->DrawIndexed(36, 0, 0);
}