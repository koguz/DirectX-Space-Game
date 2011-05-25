#ifndef ENEMYSHIPBIG__H__
#define ENEMYSHIPBIG__H__

#include <dsound.h>
#include <vector>
#include "MySound.h"
#include "EnemyShipBase.h"

class EnemyShipBig:public EnemyShipBase
{
public:
	EnemyShipBig();
	bool Init(IDirect3DDevice9* d, IDirectSound8 *s);
};



#endif
