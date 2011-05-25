#include "EnemyShipStation.h"

EnemyShipStation::EnemyShipStation():EnemyShipBase()
{
	speed = 0.0f;
	lastShot = 0;
}

bool EnemyShipStation::Init(IDirect3DDevice9* d, IDirectSound8 *s)
{
	((MyMesh*)(this))->Init(d, "station.x");
	Sound = s;
	Engine.Init(Sound, "station.wav");
	Fire.Init(Sound, "EnemyAFire.wav");
	Fire.SetLoop(false);
	HitSound.Init(Sound, "hitship.wav");
	HitSound.SetLoop(false);
	Engine.Play();

	state = HOLD;
	type = STATION;
	health = 5000;
	rotAngle = 0.1f;
	return true;
}

