#ifndef EXPLOSION__H__
#define EXPLOSION__H__

#include <d3dx9.h>
#include <vector>
#include "MySound.h"

/* with help from Luna's particle system */

struct Particle
{
	D3DXVECTOR3 initialPos;
	D3DCOLOR color;
	float initTime;
	float mass;
	float size;
	static IDirect3DVertexDeclaration9* Decl;
};


class Explosion
{
public:
	Explosion(IDirect3DDevice9* d, IDirectSound8* s, D3DXVECTOR3 position);
	~Explosion();
	void Update(float timeDelta);
	bool isAlive() { return alive; }

private:
	IDirect3DDevice9* Device;
	ID3DXEffect *effect;
	IDirect3DTexture9* mTex;
	IDirect3DVertexBuffer9* mVB;
	int numParticles;
	D3DXVECTOR3 position;
	float runningTime;
	bool alive;

	D3DXMATRIX world;

	D3DXVECTOR3 getRandomVector(); // Luna's random vector generators...
	float getRandomFloat(float a, float b); 
	MySound exsound;
	D3DVIEWPORT9 viewport;

	D3DXHANDLE mhWVP;
	D3DXHANDLE mhTech;
	D3DXHANDLE mhTime;
	D3DXHANDLE mhTexture;
	D3DXHANDLE mhEyePos;
	D3DXHANDLE mhVHeight;

	std::vector<Particle> particles;
};


#endif
