#ifndef ENEMYSHIPSTATION__H__
#define ENEMYSHIPSTATION__H__

#include <dsound.h>
#include <vector>
#include "MySound.h"
#include "EnemyShipBase.h"

class EnemyShipStation:public EnemyShipBase
{
public:
	EnemyShipStation();
	bool Init(IDirect3DDevice9* d, IDirectSound8 *s);
};



#endif
