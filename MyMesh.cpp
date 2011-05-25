/*
	MyMesh.cpp
	Kaya Oguz <0705907>
*/

#include "MyMesh.h"

MyMesh::MyMesh()
{
	numMtrls = 0;
	D3DXMatrixIdentity(&worldTransform);
	D3DXMatrixIdentity(&translationMatrix);
	D3DXMatrixIdentity(&rotationMatrix);
	D3DXMatrixIdentity(&scalingMatrix);
	minBounds = maxBounds = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	transparent = false;
}

bool MyMesh::Init(IDirect3DDevice9 *d, char *fn)
{
	Device = d;
	HRESULT hr;
	ID3DXBuffer* adjBuffer = 0;
	ID3DXBuffer* mtrlBuffer = 0;

	D3DXCreateTextureFromFile(Device, "whitetex.dds", &whiteTex);

	D3DXMATERIAL* mtrls = 0;
	D3DXLoadMeshFromX(fn, D3DXMESH_MANAGED, Device, &adjBuffer, &mtrlBuffer, 0, &numMtrls, &mesh);

	// Louis Natanson Lab. Code
	if (!mesh)
	{
		MessageBox(0, "Mesh Load Failed", fn, MB_OK);
		return false;
	}

	if ( (mtrlBuffer != 0) && (numMtrls != 0 ) )
	{
		mtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();
		for (DWORD i = 0; i < numMtrls; i++)
		{
			Mtrl m;
			m.ambient = mtrls[i].MatD3D.Diffuse;
			m.diffuse = mtrls[i].MatD3D.Diffuse;
			m.spec = mtrls[i].MatD3D.Specular;
			m.specPower = mtrls[i].MatD3D.Power;

			// mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;
			// Mtrls.push_back( mtrls[i].MatD3D );
			Mtrls.push_back(m);

			if(mtrls[i].pTextureFilename != 0)
			{
				IDirect3DTexture9* tex = 0;
				hr = D3DXCreateTextureFromFile(Device, mtrls[i].pTextureFilename, &tex);
				if (hr == S_OK) Textures.push_back(tex);
			}
			else Textures.push_back(0);
		}
	}
	mtrlBuffer->Release();
	
	D3DXMatrixIdentity(&worldTransform);
	D3DXMatrixIdentity(&translationMatrix);
	D3DXMatrixIdentity(&rotationMatrix);
	D3DXMatrixIdentity(&scalingMatrix);

	// calculate the bounding box
	BYTE* pVertices = NULL;
	hr = mesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&pVertices);
	if (!FAILED(hr))
	{
		D3DXComputeBoundingBox(
			(D3DXVECTOR3*)pVertices, 
			mesh->GetNumVertices(), 
			D3DXGetFVFVertexSize(mesh->GetFVF()), 
			&minBounds, &maxBounds);

		// calculate object bounds for Oriented Bounding Box
		objectBounds[0] = D3DXVECTOR3( minBounds.x, minBounds.y, minBounds.z );
		objectBounds[1] = D3DXVECTOR3( maxBounds.x, minBounds.y, minBounds.z );
		objectBounds[2] = D3DXVECTOR3( minBounds.x, maxBounds.y, minBounds.z );
		objectBounds[3] = D3DXVECTOR3( maxBounds.x, maxBounds.y, minBounds.z );
		objectBounds[4] = D3DXVECTOR3( minBounds.x, minBounds.y, maxBounds.z );
		objectBounds[5] = D3DXVECTOR3( maxBounds.x, minBounds.y, maxBounds.z );
		objectBounds[6] = D3DXVECTOR3( minBounds.x, maxBounds.y, maxBounds.z );
		objectBounds[7] = D3DXVECTOR3( maxBounds.x, maxBounds.y, maxBounds.z );

	}
	mesh->UnlockVertexBuffer();

	return true;
}

void MyMesh::Draw(ID3DXEffect* effect)
{
	mhTech				= effect->GetTechniqueByName("MyMeshLighting");
	mhWVP				= effect->GetParameterByName(0, "gWVP");
	mhWorldInvTrans		= effect->GetParameterByName(0, "gWorldInvTrans");
	mhMtrl				= effect->GetParameterByName(0, "gMtrl");
	mhEyePos			= effect->GetParameterByName(0, "gEyePosition");
	mhWorld				= effect->GetParameterByName(0, "gWorld");
	mhTex				= effect->GetParameterByName(0, "gTex");

	worldTransform = scalingMatrix * rotationMatrix * translationMatrix;
	D3DXMATRIX proj;
	D3DXMATRIX view;
	Device->GetTransform(D3DTS_PROJECTION, &proj);
	Device->GetTransform(D3DTS_VIEW, &view);
	D3DXVECTOR3 eyePos = D3DXVECTOR3(view._41, view._42, view._43);
	effect->SetValue(mhEyePos, &eyePos, sizeof(D3DXVECTOR3));
	effect->SetMatrix(mhWVP, &(worldTransform * view * proj) );
	D3DXMATRIX worldInvTrans;
	D3DXMatrixInverse(&worldInvTrans, 0, &worldTransform);
	D3DXMatrixTranspose(&worldInvTrans, &worldInvTrans);
	effect->SetMatrix(mhWorldInvTrans, &worldInvTrans);
	effect->SetMatrix(mhWorld, &worldTransform);
	effect->SetTechnique(mhTech);

	UINT numPasses = 0;
	effect->Begin(&numPasses, 0);
	effect->BeginPass(0);

	if (transparent)
			Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	for (DWORD i=0; i<numMtrls; i++)
	{
		effect->SetValue(mhMtrl, &Mtrls[i], sizeof(Mtrl));
		if (Textures[i] != 0)
			effect->SetTexture(mhTex, Textures[i]);
		else effect->SetTexture(mhTex, whiteTex);
		effect->CommitChanges();
		mesh->DrawSubset(i);
	}
	if (transparent)
			Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

	effect->EndPass();
	effect->End();
}


void MyMesh::CalcAABBFromOBB(D3DXVECTOR3 *minB, D3DXVECTOR3 *maxB)
{
	D3DXVECTOR3 obb[8];
	for (int i=0;i<8;i++)
	{
		D3DXVec3TransformCoord(&obb[i], &objectBounds[i], &worldTransform);
	}
	/*
	assert(minB);
	assert(maxB);
	assert(obb);
	*/

	minB->x=maxB->x=obb[0].x;
	minB->y=maxB->y=obb[0].y;
	minB->z=maxB->z=obb[0].z;

	for (int i=1;i<8;i++)
	{
		if (obb[i].x < minB->x) minB->x=obb[i].x;
		if (obb[i].x > maxB->x) maxB->x=obb[i].x;
		if (obb[i].y < minB->y) minB->y=obb[i].y;
		if (obb[i].y > maxB->y) maxB->y=obb[i].y;
		if (obb[i].z < minB->z) minB->z=obb[i].z;
		if (obb[i].z > maxB->z) maxB->z=obb[i].z;
	}
}

/*
	Translating, Rotating and Scaling are done with
	the worldTransform...
*/
void MyMesh::Translate(FLOAT x, FLOAT y, FLOAT z)
{
	D3DXMATRIX trans;
	D3DXMatrixIdentity(&trans);
	D3DXMatrixTranslation(&trans, x, y, z);
	translationMatrix = translationMatrix * trans;
}

void MyMesh::RotateX(FLOAT q)
{
	D3DXMATRIX rot;
	D3DXMatrixIdentity(&rot);
	D3DXMatrixRotationX(&rot, q);
	rotationMatrix = rotationMatrix * rot;
}

void MyMesh::RotateY(FLOAT q)
{
	D3DXMATRIX rot;
	D3DXMatrixIdentity(&rot);
	D3DXMatrixRotationY(&rot, q);
	rotationMatrix = rotationMatrix * rot;
}

void MyMesh::RotateZ(FLOAT q)
{
	D3DXMATRIX rot;
	D3DXMatrixIdentity(&rot);
	D3DXMatrixRotationZ(&rot, q);
	rotationMatrix = rotationMatrix * rot;
}

void MyMesh::Scale(FLOAT sx, FLOAT sy, FLOAT sz)
{
	D3DXMATRIX scale;
	D3DXMatrixIdentity(&scale);
	D3DXMatrixScaling(&scale, sx, sy, sz);
	scalingMatrix = scalingMatrix * scale;
}

void MyMesh::Scale(FLOAT s)
{
	Scale(s, s, s);
}
