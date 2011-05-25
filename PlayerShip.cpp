#include "PlayerShip.h"

PlayerShip::PlayerShip():BaseObject()
{
	speed = 0.0f;
	lastShot = 0;
}

bool PlayerShip::Init(IDirect3DDevice9 *d, IDirectSound8 *s)
{
	((MyMesh*)(this))->Init(d, "ship01.x");
	Sound = s;

	Engine.Init(Sound, "PSEngine.wav");
	Fire.Init(Sound, "PSFire.wav");
	Fire.SetLoop(false);
	Scratch.Init(Sound, "Scratch.wav");
	Scratch.SetLoop(false);
	HitSound.Init(Sound, "hitship.wav");
	HitSound.SetLoop(false);

	/*
	Mission1.Init(Sound, "mission01.wav");
	Mission1.SetLoop(false);
	Mission1.Play();
	*/

	Engine.SetVolume(-800);
	Engine.Play();

	maxSpeed = 6.0f;
	minSpeed = 0.6f;
	casSpeed = 2.0f;
	laserInterval = 500;
	health = 2000;

	return true;
}

void PlayerShip::Scratched()
{
	Scratch.setPosition(position);
	Scratch.Play();
	health -= 5;
}

void PlayerShip::Hit(int d)
{
	health -= d;
	HitSound.Play();
}

void PlayerShip::Update(float deltaTime)
{
	if (health < 2000 && GetTickCount() - lastGen > 100 )
	{
		health++; 
		lastGen = GetTickCount();
	}
	time = deltaTime;

	TranslateV(speed * look * deltaTime);
	Engine.setPosition(position);
	Fire.setPosition(position);
	HitSound.setPosition(position);
	// Mission1.setPosition(position);
	

	std::vector<Laser>::iterator temp;
	std::vector<Laser>::iterator toDel;
	bool d = false;
	
	for(temp = lasers.begin(); temp != lasers.end(); temp++)
	{
		(*temp).Update(time);
		// (*temp).Draw();
		if (abs((*temp).getPosition().x) > 200 || abs((*temp).getPosition().y) > 200 || abs((*temp).getPosition().z) > 200 )
		{
			toDel = temp;
			d = true;
		}
		if ((*temp).getHit())
		{
			toDel = temp;
			d = true;
		}
	}
	if (d) 
		lasers.erase(toDel);
}

void PlayerShip::SpeedUp()
{
	SetSpeed(maxSpeed);
}

void PlayerShip::SpeedDown()
{
	SetSpeed(minSpeed);
}

void PlayerShip::SpeedNormal()
{
	SetSpeed(casSpeed);
}

void PlayerShip::SetSpeed(float s)
{
	if (s == speed) return;
	if (s >= casSpeed && s < maxSpeed && speed != s)
		Engine.setFrequency(0);
	else if (s >= maxSpeed && speed != s)
		Engine.setFrequency(24000);
	else if (s >= minSpeed && s < casSpeed && speed != s)
		Engine.setFrequency(20000);
	speed = s;
}

void PlayerShip::FireLaser()
{
	if (health < 0) return;
	if (GetTickCount() - lastShot > laserInterval)
	{
		Laser laser1(Device, Laser::RED);
		Laser laser2(Device, Laser::RED);

		laser1.SetRotation(rotationMatrix);
		laser1.TranslateV(position + (right * 0.6f));
		laser1.SetOrientation(up, right, look);

		laser2.SetRotation(rotationMatrix);
		laser2.TranslateV(position - (right * 0.6f));
		laser2.SetOrientation(up, right, look);

		lasers.push_back(laser1);
		lasers.push_back(laser2);

		Fire.Play();
		lastShot = GetTickCount();
	}
	else return;
}
