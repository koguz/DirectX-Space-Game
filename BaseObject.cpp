#include "BaseObject.h"

BaseObject::BaseObject()
{
	Device = 0;
	position = D3DXVECTOR3(0, 0, 0);
	look = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	Translate(0, 0, 0);
	speed = 0.0f;
	time = (float)GetTickCount();
}

BaseObject::BaseObject(
					   IDirect3DDevice9 *Device, 
					   char *filename, 
					   D3DXVECTOR3 p, 
					   D3DXVECTOR3 v)
{
	Init(Device, filename);
	position = p;
	Translate(p.x, p.y, p.z);
	time = (float)GetTickCount();
}

D3DXVECTOR3 BaseObject::getPosition()
{
	return position; 
}

D3DXVECTOR3 BaseObject::getUp()
{
	return up; 
}

D3DXVECTOR3 BaseObject::getRight()
{
	return right; 
}

D3DXVECTOR3 BaseObject::getLook()
{
	return look; 
}

void BaseObject::SetOrientation(D3DXVECTOR3 u, D3DXVECTOR3 r, D3DXVECTOR3 l)
{
	up = u;
	right = r;
	look = l;
}

void BaseObject::TranslateV(D3DXVECTOR3 t)
{
	Translate(t.x, t.y, t.z);
	position.x += t.x;
	position.y += t.y;
	position.z += t.z;
}

void BaseObject::Update(float deltaTime)
{
	time = deltaTime;
	TranslateV(speed * look * deltaTime);
}

void BaseObject::setPosition(D3DXVECTOR3 p)
{ position = p; }

void BaseObject::RotateX(FLOAT q, bool limit)
{
	q = q * time;
	if (limit)
	{
		if (q > ANGLE)
			q = ANGLE;
		if (q < -ANGLE)
			q = -ANGLE;
	}
	// rotate around x axis, which is the right vector, q degrees
	D3DXMATRIX temp; // rotation matrix
	D3DXMatrixRotationAxis(&temp, &right, q);

	rotationMatrix = rotationMatrix * temp;

	// rotate up and look
	D3DXVec3TransformCoord(&up, &up, &temp);
	D3DXVec3TransformCoord(&look, &look, &temp);
}

void BaseObject::RotateY(FLOAT q, bool limit)
{
	q = q * time;
	if (limit)
	{
		if (q > ANGLE)
			q = ANGLE;
		if (q < -ANGLE)
			q = -ANGLE;
	}
	// rotate object & other vectors around up 
	D3DXMATRIX temp;
	D3DXMatrixRotationAxis(&temp, &up, q);

	rotationMatrix = rotationMatrix * temp;

	D3DXVec3TransformCoord(&right, &right, &temp);
	D3DXVec3TransformCoord(&look, &look, &temp);
}

void BaseObject::RotateZ(FLOAT q)
{
	q = q * time;
	if (q > ANGLE)
		q = ANGLE;
	if (q < -ANGLE)
		q = -ANGLE;
	// rotate object & other vectors around look
	D3DXMATRIX temp;
	D3DXMatrixRotationAxis(&temp, &look, q);

	rotationMatrix = rotationMatrix * temp;

	D3DXVec3TransformCoord(&up, &up, &temp);
	D3DXVec3TransformCoord(&right, &right, &temp);
}

