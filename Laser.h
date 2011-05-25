#ifndef LASER__H__
#define LASER__H__

#include "BaseObject.h"

class Laser:public BaseObject
{
public: 
	enum LaserType { RED, BLUE, YELLOW, ORANGE };
	Laser( IDirect3DDevice9* d, LaserType type );
	void TargetHit();
	bool getHit() { return hit; }
	int getDamage() { return damage; }
private:
	int damage;
	IDirect3DDevice9* Device;
	bool hit;
};

#endif
