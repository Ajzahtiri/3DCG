#include "Camera.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}

HRESULT Camera::Initialise()
{
	//set up position
	eye.x = 0.0f;
	eye.y = 0.0f;
	eye.z = -5.0f;

	//set up target
	at.x = 0.0f;
	at.y = 0.0f;
	at.z = 0.0f;

	//set up up
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;
	
	return true;
}

void Camera::moveLeft()
{
	eye.x -= movement;
	at.x -= movement;
}

void Camera::moveRight()
{
	eye.x += movement;
	at.x += movement;
}

void Camera::moveUp()
{
	eye.y += movement;
	at.y += movement;
}

void Camera::moveDown()
{
	eye.y -= movement;
	at.y -= movement;
}

void Camera::moveForward()
{
	eye.z += movement;
	at.z += movement;
}

void Camera::moveBack()
{
	eye.z -= movement;
	at.z -= movement;
}

XMVECTOR Camera::getEyeVector()
{
	XMVECTOR e = XMVectorSet(eye.x, eye.y, eye.z, 0.0f);
	return e;
}

XMVECTOR Camera::getAtVector()
{
	XMVECTOR a = XMVectorSet(at.x, at.y, at.z, 0.0f);
	return a;
}

XMVECTOR Camera::getUpVector()
{
	XMVECTOR u = XMVectorSet(up.x, up.y, up.z, 0.0f);
	return u;
}