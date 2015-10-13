#include "Camera.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}

HRESULT Camera::InitCamera()
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
	eye.x += 0.001;
	at.x += 0.001;
}

void Camera::moveRight()
{
	eye.x -= 0.001;
	at.x -= 0.001;
}

void Camera::moveUp()
{
	eye.y -= 0.001;
	at.y -= 0.001;
}

void Camera::moveDown()
{
	eye.y += 0.001;
	at.y += 0.001;
}

void Camera::moveForward()
{
	eye.z += 0.001;
	at.z += 0.001;
}

void Camera::moveBack()
{
	eye.z -= 0.001;
	at.z -= 0.001;
}

XMVECTOR Camera::getEyeVector()
{
	XMVECTOR e = XMVectorSet(eye.x, eye.y, eye.z, 0.0f);
	return e;
}

XMVECTOR Camera::getAtVector()
{
	XMVECTOR a = XMVectorSet(eye.x, eye.y, eye.z, 0.0f);
	return a;
}

XMVECTOR Camera::getUpVector()
{
	XMVECTOR u = XMVectorSet(up.x, up.y, up.z, 0.0f);
	return u;
}