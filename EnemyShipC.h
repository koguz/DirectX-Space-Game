#ifndef ENEMYSHIPC__H__
#define ENEMYSHIPC__H__

#include <dsound.h>
#include <vector>
#include <list>
#include "MySound.h"
#include "EnemyShipBase.h"

class EnemyShipC:public EnemyShipBase
{
public:
	EnemyShipC();
	bool Init(IDirect3DDevice9* d, IDirectSound8 *s);
private:
	void FireLaser();
};


#endif
