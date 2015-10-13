#pragma once

#include "Object.h"

Object::Object(void)
{

}

Object::~Object(void)
{

}

void Object::SetRotation(float x, float y, float z)
{
	_rotation = XMMatrixRotationX(x) * XMMatrixRotationY(y) * XMMatrixRotationZ(z);
}

void Object::SetScale(float x, float y, float z)
{
	_scale = XMMatrixScaling(x, y, z);
}

void Object::SetTranslation(float x, float y, float z)
{
	_translation = XMMatrixTranslation(x, y, z);
}

void Object::Update(float elapsed)
{
	XMStoreFloat4x4(&_world, _rotation * _scale * _translation);
}

void Object::Draw(ID3D11Device * device, ID3D11DeviceContext * context)
{
	context->IASetVertexBuffers(0, 1, &_meshData.VertexBuffer, &_meshData.VBStride, &_meshData.VBOffset);
	context->IASetIndexBuffer(_meshData.IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	context->DrawIndexed(_meshData.IndexCount, 0, 0);
}