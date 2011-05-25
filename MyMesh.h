/*
	MyMesh.h
	Kaya Oguz <0705907>
*/

#ifndef MYMESH__H__
#define MYMESH__H__

#include <d3dx9.h>
#include <vector>
#include "dxut.h"

using namespace dxut;

/* 
	A mesh class to hold unanimated meshes
*/
class MyMesh
{
public:
	MyMesh();
	bool Init(IDirect3DDevice9* d, char* filename);
	void Draw(ID3DXEffect* effect);
	void Translate(FLOAT x, FLOAT y, FLOAT z);
	void RotateX(FLOAT q);
	void RotateY(FLOAT q);
	void RotateZ(FLOAT q);
	void Scale(FLOAT sx, FLOAT sy, FLOAT sz);
	void Scale(FLOAT s);
	void CalcAABBFromOBB(D3DXVECTOR3 *minB, D3DXVECTOR3 *maxB);
	void SetRotation(D3DXMATRIX r) { rotationMatrix = r; }
	void SetTransparent(bool t) { transparent = t; }
	IDirect3DTexture9* whiteTex;

protected:
	D3DXMATRIX worldTransform;
	D3DXMATRIX translationMatrix;
	D3DXMATRIX rotationMatrix;
	D3DXMATRIX scalingMatrix;

	ID3DXMesh* mesh;

	D3DXHANDLE mhTech;
	D3DXHANDLE mhWVP;
	D3DXHANDLE mhWorldInvTrans;
	D3DXHANDLE mhEyePos;
	D3DXHANDLE mhWorld;
	D3DXHANDLE mhTex;
	D3DXHANDLE mhMtrl;
	
	D3DXVECTOR3 minBounds, maxBounds;
	D3DXVECTOR3 objectBounds[8];
	std::vector<Mtrl> Mtrls;
	std::vector<IDirect3DTexture9*> Textures;
	DWORD numMtrls;
	IDirect3DDevice9* Device;
	bool transparent;
};


#endif