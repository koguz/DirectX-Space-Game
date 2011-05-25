/*
	MyCamera.cpp
	Kaya Oguz <0705907>
*/

#include "MyCamera.h"

MyCamera::MyCamera()
{
	Device = 0;
	position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	look = D3DXVECTOR3(0.0f, 0.0f, 1000.0f);
	up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
}

MyCamera::MyCamera(IDirect3DDevice9 *d)
{
	Device = d;
	position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	look = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
}

void MyCamera::Display()
{
	D3DXMATRIX View;
	D3DXMatrixLookAtLH(&View, &position, &look, &up);
	Device->SetTransform(D3DTS_VIEW, &View);
}


void MyCamera::Update()
{	
	D3DXVECTOR3 desiredPosition = attachedObject->getPosition() - (2.4f*attachedObject->getLook()) + (1.2f*attachedObject->getUp());
	D3DXVECTOR3 moveDirection = desiredPosition - position;
	FLOAT length = D3DXVec3Length(&moveDirection);
	FLOAT distanceToMove = length / 4;
	position += moveDirection * distanceToMove;

	up = attachedObject->getUp();
	look = attachedObject->getPosition() + (attachedObject->getLook()*100);
	
	Display();
}
