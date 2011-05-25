#include "Explosion.h"

IDirect3DVertexDeclaration9* Particle::Decl = 0;

Explosion::Explosion(IDirect3DDevice9 *d, IDirectSound8* s, D3DXVECTOR3 pos)
{
	alive = true;
	Device = d;
	runningTime = 0.0f;
	numParticles = 10000;
	particles.resize(numParticles);
	position = pos;

	Device->GetViewport(&viewport);

	exsound.Init(s, "explosion.wav");
	exsound.SetLoop(false);
	exsound.setPosition(pos);
	exsound.Play();

	if (Particle::Decl == 0)
	{
		D3DVERTEXELEMENT9 elems[] = 
		{
			{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 }, 
			{ 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
			{ 0, 16, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
			{ 0, 20, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
			{ 0, 24, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_PSIZE, 0 },
			D3DDECL_END()
		};
		Device->CreateVertexDeclaration(elems, &Particle::Decl);
	}

	Device->CreateVertexBuffer( numParticles * sizeof(Particle), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY | D3DUSAGE_POINTS, 
		0, D3DPOOL_DEFAULT, &mVB, 0);

	Particle *p0 = 0;
	mVB->Lock(0, 0, (void**)&p0, D3DLOCK_DISCARD);

	for(int i=0;i<numParticles;i++)
	{
		particles[i].initialPos = getRandomVector();
		particles[i].color = D3DCOLOR_XRGB(200, 200, 0); 
		particles[i].initTime = 0.0f; 
		particles[i].mass = getRandomFloat(0, 1);
		particles[i].size = getRandomFloat(1, 3);
		p0[i] = particles[i];
	}
	mVB->Unlock();

	ID3DXBuffer* err = 0;
	D3DXCreateEffectFromFile(Device, "explosion.fx", 0, 0, D3DXSHADER_DEBUG, 0, &effect, &err);
	if (err)
		MessageBox(0, (char*)err->GetBufferPointer(), 0, 0);

	mhTech = effect->GetTechniqueByName("ExplosionTech");
	mhWVP  = effect->GetParameterByName(0, "gWVP");
	mhTime = effect->GetParameterByName(0, "gTime");
	mhTexture = effect->GetParameterByName(0, "gTex");
	mhEyePos = effect->GetParameterByName(0, "gEyePosL");
	mhVHeight = effect->GetParameterByName(0, "gViewportHeight");

	D3DXCreateTextureFromFile(Device, "torch.dds", &mTex);

	effect->SetTechnique(mhTech);

	D3DXMatrixIdentity(&world);
	D3DXMatrixTranslation(&world, position.x, position.y, position.z);
}

Explosion::~Explosion()
{
	effect->Release();
	mVB->Release();
	mTex->Release();
}

D3DXVECTOR3 Explosion::getRandomVector()
{
	D3DXVECTOR3 temp;
	temp.x  = getRandomFloat(-1.0f, 1.0f);
	temp.y  = getRandomFloat(-1.0f, 1.0f);
	temp.z  = getRandomFloat(-1.0f, 1.0f);
	D3DXVECTOR3 temp2(getRandomFloat(-0.5f, 0.5f), getRandomFloat(-0.3f, 0.3f), getRandomFloat(-0.7f, 0.7f));
	D3DXVec3Normalize(&temp, &temp);
	temp += temp2;
	return temp;
}

float Explosion::getRandomFloat(float a, float b)
{
	if (a>=b)
		return a;
	float f = (rand()%10001) * 0.0001f;
	return (f*(b-a))+a;
}

void Explosion::Update(float timeDelta)
{
	runningTime += timeDelta;
	if (runningTime > 5)
		alive = false;
	if (!alive)
		return;

	D3DXMATRIX proj;
	D3DXMATRIX view;
	Device->GetTransform(D3DTS_PROJECTION, &proj);
	Device->GetTransform(D3DTS_VIEW, &view);
	effect->SetMatrix(mhWVP, &(world * view * proj) );
	effect->SetInt(mhVHeight, (int)viewport.Height);
	effect->SetFloat(mhTime, runningTime);
	D3DXVECTOR3 campos(view._41, view._42, view._43);
	D3DXVECTOR3 eyepos;
	D3DXMATRIX inv;
	D3DXMatrixInverse(&inv, 0, &view);
	D3DXVec3TransformCoord(&eyepos, &campos, &inv);
	// D3DXVECTOR3 eyepos(view._41, view._42, view._43);

	/*D3DXVECTOR3 eyePosW = gCamera->pos();
	D3DXVECTOR3 eyePosL;
	D3DXVec3TransformCoord(&eyePosL, &eyePosW, &mInvWorld);*/

	effect->SetValue(mhEyePos, &eyepos, sizeof(D3DXVECTOR3));

	UINT numPasses = 0;
	effect->Begin(&numPasses, 0);
	effect->BeginPass(0);
	Device->SetStreamSource(0, mVB, 0, sizeof(Particle));
	Device->SetVertexDeclaration(Particle::Decl);
	effect->SetTexture(mhTexture, mTex);
	Device->DrawPrimitive(D3DPT_POINTLIST, 0, numParticles);
	effect->EndPass();
	effect->End();
}
