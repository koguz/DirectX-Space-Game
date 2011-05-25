#include "EnemyShipA.h"

EnemyShipA::EnemyShipA():EnemyShipBase()
{
	speed = 2.0f;
	lastShot = 0;
}

bool EnemyShipA::Init(IDirect3DDevice9* d, IDirectSound8 *s)
{
	((MyMesh*)(this))->Init(d, "sf1.x");
	Sound = s;
	Engine.Init(Sound, "EnemyAEngine.wav");
	Fire.Init(Sound, "EnemyAFire.wav");
	Fire.SetLoop(false);
	Engine.Play();
	HitSound.Init(Sound, "hitship.wav");
	HitSound.SetLoop(false);
	maxSpeed = 5.0f;
	minSpeed = 0.6f;
	casSpeed = 2.0f;
	laserInterval = 1000;
	range = 40;
	health = 500;
	rotAngle = 0.5f;
	type = TYPEA;
	return true;
}

void EnemyShipA::FireLaser()
{
	if (GetTickCount() - lastShot > laserInterval)
	{
		Laser laser1(Device, Laser::BLUE);
		Laser laser2(Device, Laser::BLUE);

		laser1.SetRotation(rotationMatrix);
		laser1.TranslateV(position + (right * 0.75f));
		laser1.SetOrientation(up, right, look);

		laser2.SetRotation(rotationMatrix);
		laser2.TranslateV(position - (right * 0.75f));
		laser2.SetOrientation(up, right, look);

		lasers.push_back(laser1);
		lasers.push_back(laser2);

		Fire.Play();
		lastShot = GetTickCount();
	}
	else return;
}
