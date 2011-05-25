/*
	MyCamera.h
	Kaya Oguz <0705907>
*/
#include "BaseObject.h"
#ifndef MYCAMERA__H__
#define MYCAMERA__H__

#include <d3dx9.h>

class MyCamera
{
public:
	MyCamera();
	MyCamera(IDirect3DDevice9* d);
	void Display();
	void AttachObject(BaseObject* o) { attachedObject = o; }

	D3DXVECTOR3 position, look, up;
	void Update(); // gets the position, etc. from the attachedObject
private:
	IDirect3DDevice9* Device;
	BaseObject* attachedObject;
};


#endif