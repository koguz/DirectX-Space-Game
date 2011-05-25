#include "CubeMap.h"

CubeMap::CubeMap(IDirect3DDevice9* d)
{
	Device = d;
	D3DXCreateSphere(Device, 10000.0f, 30, 30, &mesh, 0);
	D3DXCreateCubeTextureFromFile(Device, "cubemap.dds", &envMap);
	ID3DXBuffer* err = 0;
	D3DXCreateEffectFromFile(Device, "cubemap.fx", 0, 0, 0, 0, &effect, &err);

	if (err)
		MessageBox(0, (char*)err->GetBufferPointer(), 0, 0);

	mhTech	 = effect->GetTechniqueByName("SkyTech");
	mhWVP	 = effect->GetParameterByName(0, "gWVP");
	mhEnvMap = effect->GetParameterByName(0, "gEnvMap");

	effect->SetTechnique(mhTech);
	effect->SetTexture(mhEnvMap, envMap);

}

CubeMap::~CubeMap()
{
	mesh->Release();
	envMap->Release();
	effect->Release();
}

void CubeMap::Draw(D3DXVECTOR3 p)
{
	D3DXMATRIX W;
	D3DXMATRIX proj;
	D3DXMATRIX view;
	D3DXMatrixTranslation(&W, p.x, p.y, p.z);
	Device->GetTransform(D3DTS_PROJECTION, &proj);
	Device->GetTransform(D3DTS_VIEW, &view);

	effect->SetMatrix(mhWVP, &(W*view*proj));

	UINT numPasses = 0;
	effect->Begin(&numPasses, 0);
	effect->BeginPass(0);
	mesh->DrawSubset(0);
	effect->EndPass();
	effect->End();
}

