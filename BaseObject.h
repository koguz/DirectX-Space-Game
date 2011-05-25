#ifndef BASEOBJECT__H__
#define BASEOBJECT__H__

//#define ANGLE 0.0174f
#define ANGLE 1.0f

#include "MyMesh.h"

class BaseObject:public MyMesh
{
public:
	BaseObject();
	BaseObject(IDirect3DDevice9* Device, char* filename, D3DXVECTOR3 position, D3DXVECTOR3 velocity);
	D3DXVECTOR3 getPosition();
	void setPosition (D3DXVECTOR3 p);

	D3DXVECTOR3 getUp();
	D3DXVECTOR3 getRight();
	D3DXVECTOR3 getLook();

	void Update(float deltaTime);
	void TranslateV(D3DXVECTOR3 t);
	void RotateX(FLOAT q, bool limit=true);
	void RotateY(FLOAT q, bool limit=true);
	void RotateZ(FLOAT q);
	void SetSpeed(float s) { speed = s; }
	void SetOrientation(D3DXVECTOR3 up, D3DXVECTOR3 right, D3DXVECTOR3 look);
	float getSpeed() { return speed; }

protected:
	D3DXVECTOR3 position;
	D3DXVECTOR3 up, right, look;
	float speed; 
	float time;
};


#endif
