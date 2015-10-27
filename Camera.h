#pragma once

#include <windows.h>
#include "Camera.h"
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
using namespace DirectX;
//
//struct Eye
//{
//	float x;
//	float y;
//	float z;
//};
//
//struct At
//{
//	float x;
//	float y;
//	float z;
//};
//
//struct Up
//{
//	float x;
//	float y;
//	float z;
//};

class Camera
{
private:
	/*Eye eye;
	At at;
	Up up;*/
	
	float movement = 0.001;	

public:
	XMFLOAT3 eye;
	   XMFLOAT3 at;
	   XMFLOAT3 up;
	   Camera();
	~Camera();
	
	HRESULT Initialise();
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void moveForward();
	void moveBack();
	XMFLOAT3 GetPosition() const { return eye; }

	XMVECTOR getEyeVector();
	XMVECTOR getAtVector();
	XMVECTOR getUpVector();
};