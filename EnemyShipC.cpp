#include "EnemyShipC.h"


EnemyShipC::EnemyShipC():EnemyShipBase()
{
	speed = 3.0f;
	lastShot = 0;
}

bool EnemyShipC::Init(IDirect3DDevice9* d, IDirectSound8 *s)
{
	((MyMesh*)(this))->Init(d, "sf3.x");
	Sound = s;
	Engine.Init(Sound, "EnemyCEngine.wav");
	Fire.Init(Sound, "EnemyCFire.wav");
	Fire.SetLoop(false);
	HitSound.Init(Sound, "hitship.wav");
	HitSound.SetLoop(false);
	Engine.Play();
	maxSpeed = 6.5f;
	minSpeed = 1.0f;
	casSpeed = 3.0f;
	laserInterval = 600;
	range = 60;
	type = TYPEC;
	rotAngle = 1.0f;
	health = 750;
	return true;
}

void EnemyShipC::FireLaser()
{
	if (GetTickCount() - lastShot > laserInterval)
	{
		Laser laser1(Device, Laser::YELLOW);
		Laser laser2(Device, Laser::YELLOW);

		laser1.SetRotation(rotationMatrix);
		laser1.TranslateV(position + (right * 0.9f));
		laser1.SetOrientation(up, right, look);

		laser2.SetRotation(rotationMatrix);
		laser2.TranslateV(position - (right * 0.9f));
		laser2.SetOrientation(up, right, look);

		lasers.push_back(laser1);
		lasers.push_back(laser2);

		Fire.Play();
		lastShot = GetTickCount();
	}
	else return;
}
