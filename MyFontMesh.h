#ifndef MYFONTMESH__H__
#define MYFONTMESH__H__

#include <d3dx9.h>
#include <string>
#include "MyMesh.h"

struct VertexMesh
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	static IDirect3DVertexDeclaration9* Decl;
};

class MyFontMesh:public MyMesh
{
public:
	MyFontMesh();
	MyFontMesh(IDirect3DDevice9* d, std::string t, std::string fontface);
	~MyFontMesh();
	void Draw(ID3DXEffect* effect);
	
private:
	HDC hdc;
	HFONT font;
	std::string text;
	dxut::Mtrl fontMat;
	D3DXHANDLE mhTech;
	D3DXHANDLE mhWVP;
	D3DXHANDLE mhWorldInverseTranspose;
	D3DXHANDLE mhDiffuseMtrl;
	D3DXHANDLE mhDiffuseLight;

	D3DXCOLOR mDiffuseMtrl;
	D3DXCOLOR mDiffuseLight;

};


#endif
