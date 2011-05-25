#include "EnemyShipB.h"


EnemyShipB::EnemyShipB():EnemyShipBase()
{
	speed = 2.0f;
	lastShot = 0;
}

bool EnemyShipB::Init(IDirect3DDevice9* d, IDirectSound8 *s)
{
	((MyMesh*)(this))->Init(d, "sf2.x");
	Sound = s;
	Engine.Init(Sound, "EnemyBEngine.wav");
	Fire.Init(Sound, "EnemyBFire.wav");
	Fire.SetLoop(false);
	Engine.Play();
	HitSound.Init(Sound, "hitship.wav");
	HitSound.SetLoop(false);
	maxSpeed = 6.0f;
	minSpeed = 0.6f;
	casSpeed = 2.0f;
	laserInterval = 800;
	range = 50;
	health = 600;
	rotAngle = 0.7f;
	type = TYPEB;
	return true;
}

void EnemyShipB::FireLaser()
{
	if (GetTickCount() - lastShot > laserInterval)
	{
		Laser laser1(Device, Laser::ORANGE);
		Laser laser2(Device, Laser::ORANGE);

		laser1.SetRotation(rotationMatrix);
		laser1.TranslateV(position + (right * 1.0f));
		laser1.SetOrientation(up, right, look);

		laser2.SetRotation(rotationMatrix);
		laser2.TranslateV(position - (right * 1.0f));
		laser2.SetOrientation(up, right, look);

		lasers.push_back(laser1);
		lasers.push_back(laser2);

		Fire.Play();
		lastShot = GetTickCount();
	}
	else return;
}
