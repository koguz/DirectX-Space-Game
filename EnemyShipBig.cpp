#include "EnemyShipBig.h"


EnemyShipBig::EnemyShipBig():EnemyShipBase()
{
	speed = 0.0f;
	lastShot = 0;
}

bool EnemyShipBig::Init(IDirect3DDevice9* d, IDirectSound8 *s)
{
	((MyMesh*)(this))->Init(d, "shuttle02.x");
	Sound = s;
	Engine.Init(Sound, "EnemyBigEngine.wav");
	Fire.Init(Sound, "EnemyAFire.wav");
	Fire.SetLoop(false);
	HitSound.Init(Sound, "hitship.wav");
	HitSound.SetLoop(false);
	Engine.Play();

	state = HOLD;
	type = TYPEBIG;
	health = 5000;
	rotAngle = 0.1f;
	return true;
}

