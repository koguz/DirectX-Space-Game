#ifndef CUBEMAP__H__
#define CUBEMAP__H__

#include <d3dx9.h>

class CubeMap
{
public:
	CubeMap(IDirect3DDevice9* d);
	~CubeMap();
	void Draw(D3DXVECTOR3 p);
private:
	IDirect3DDevice9* Device;
	ID3DXMesh *mesh;
	IDirect3DCubeTexture9* envMap;
	ID3DXEffect* effect;
	D3DXHANDLE mhTech;
	D3DXHANDLE mhEnvMap;
	D3DXHANDLE mhWVP;
};


#endif