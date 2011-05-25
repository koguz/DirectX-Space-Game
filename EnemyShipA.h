#ifndef ENEMYSHIPA__H__
#define ENEMYSHIPA__H__

#include <dsound.h>
#include <vector>
#include <list>
#include "MySound.h"
#include "EnemyShipBase.h"

class EnemyShipA:public EnemyShipBase
{
public:
	EnemyShipA();
	bool Init(IDirect3DDevice9* d, IDirectSound8 *s);
private:
	void FireLaser();
};


#endif
