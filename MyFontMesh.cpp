#include "MyFontMesh.h"

IDirect3DVertexDeclaration9* VertexMesh::Decl  = 0;

MyFontMesh::MyFontMesh()
{
	Device = 0;
	mesh = 0;
	font = 0;
}

MyFontMesh::~MyFontMesh()
{
	/*
	if (fontMesh)
	{
		mesh->Release();
		fontMesh = 0;
	}
	*/
}

MyFontMesh::MyFontMesh(IDirect3DDevice9 *d, std::string t, std::string fontface)
{
	Device = d;
	hdc = CreateCompatibleDC(NULL);
	AddFontResource("font.ttf");
	AddFontResource("font2.ttf");

	font = CreateFont(10, 0, 0, 0, FW_NORMAL, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
		DEFAULT_PITCH|FF_DONTCARE, fontface.c_str());

	SelectObject(hdc, font);
	D3DXCreateText(Device, hdc, t.c_str(), 0.001f, 0.4f, &mesh, NULL, NULL);

	// D3DXCreateTextureFromFile(Device, "whitetex.dds", &whiteTex);

	mDiffuseLight = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	mDiffuseMtrl  = D3DXCOLOR(0.7f, 0.7f, 1.0f, 1.0f);
	/*
	numMtrls = 1;
	fontMat = Mtrl(D3DXCOLOR(255,255,255,255), D3DXCOLOR(255,255,255,255), D3DXCOLOR(255,255,255,255), 2.0f);
	Textures.push_back(whiteTex);
	Mtrls.push_back(fontMat);
	*/
}


void MyFontMesh::Draw(ID3DXEffect* effect)
{
	mhTech = effect->GetTechniqueByName("FontEffect");
	mhWVP = effect->GetParameterByName(0, "gWVP");
	mhWorldInverseTranspose = effect->GetParameterByName(0, "gWorldInverseTranspose");
	mhDiffuseMtrl = effect->GetParameterByName(0, "gDiffuseMtrl");
	mhDiffuseLight = effect->GetParameterByName(0, "gDiffuseLight");


	worldTransform = scalingMatrix * rotationMatrix * translationMatrix;


	effect->SetTechnique(mhTech);
	D3DXMATRIX proj;
	D3DXMATRIX view;
	Device->GetTransform(D3DTS_PROJECTION, &proj);
	Device->GetTransform(D3DTS_VIEW, &view);
	effect->SetMatrix(mhWVP, &(worldTransform*view*proj));

	D3DXMATRIX worldInvTrans;
	D3DXMatrixInverse(&worldInvTrans, 0, &worldTransform);
	D3DXMatrixTranspose(&worldInvTrans, &worldInvTrans);
	effect->SetMatrix(mhWorldInverseTranspose, &worldInvTrans);
	effect->SetValue(mhDiffuseMtrl, &mDiffuseMtrl, sizeof(D3DXCOLOR));
	effect->SetValue(mhDiffuseLight, &mDiffuseLight, sizeof(D3DXCOLOR));

	UINT numPasses = 0;
	effect->Begin(&numPasses, 0);
	effect->BeginPass(0);

	effect->CommitChanges();
	mesh->DrawSubset(0);

	effect->EndPass();
	effect->End();

	/*
	Device->SetTransform(D3DTS_WORLD, &worldTransform);
	mesh->DrawSubset(0);*/
}
