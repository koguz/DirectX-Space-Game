#include "Laser.h"

Laser::Laser(IDirect3DDevice9* d, LaserType type):BaseObject()
{
	hit = false;
	Device = d;
	switch (type)
	{
	case RED:
		Init(Device, "redlaser.x");
		SetSpeed(30.0f);
		damage = 25;
		break;
	case BLUE:
		Init(Device, "bluelaser.x");
		SetSpeed(28.0f);
		damage = 10;
		break;
	case ORANGE:
		Init(Device, "orangelaser.x");
		SetSpeed(29.5f);
		damage = 15;
	case YELLOW:
		Init(Device, "yellowlaser.x");
		SetSpeed(31.0f);
		damage = 20;
		break;
	}
}

void Laser::TargetHit()
{
	hit = true;
}

