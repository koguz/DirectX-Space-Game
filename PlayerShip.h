#ifndef PLAYERSHIP__H__
#define PLAYERSHIP__H__

#include <dsound.h>
#include <vector>
#include "MySound.h"
#include "BaseObject.h"
#include "Laser.h"

class PlayerShip:public BaseObject
{
public:
	PlayerShip();
	bool Init(IDirect3DDevice9* d, IDirectSound8 *s);
	void Update(float dtime);
	void SetSpeed(float p);
	void FireLaser();
	void Scratched();
	void Hit(int d);
	int getHealth() { return health; }
	void SpeedUp();
	void SpeedDown();
	void SpeedNormal();
	std::vector<Laser> lasers;
	MySound Engine; //, Mission1;
	
private:
	IDirectSound8* Sound;
	MySound Fire, Scratch, HitSound;// , Mission1;
	unsigned int lastShot;
	float maxSpeed;
	float minSpeed;
	float casSpeed;
	unsigned int laserInterval;
	int health;
	int lastGen;
};


#endif
