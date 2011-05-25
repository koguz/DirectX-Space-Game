#ifndef ENEMYSHIPB__H__
#define ENEMYSHIPB__H__

#include <dsound.h>
#include <vector>
#include <list>
#include "MySound.h"
#include "EnemyShipBase.h"

class EnemyShipB:public EnemyShipBase
{
public:
	EnemyShipB();
	bool Init(IDirect3DDevice9* d, IDirectSound8 *s);
private:
	void FireLaser();
};


#endif
