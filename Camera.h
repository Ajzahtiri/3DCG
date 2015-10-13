#pragma once

#include <windows.h>
#include "Camera.h"
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
using namespace DirectX;

struct Eye
{
	float x;
	float y;
	float z;
};

struct At
{
	float x;
	float y;
	float z;
};

struct Up
{
	float x;
	float y;
	float z;
};

class Camera
{
private:
	Eye eye;
	At at;
	Up up;

	

public:
	Camera();
	~Camera();

	HRESULT InitCamera();
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void moveForward();
	void moveBack();

	XMVECTOR getEyeVector();
	XMVECTOR getAtVector();
	XMVECTOR getUpVector();

};