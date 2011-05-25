#ifndef ENEMYSHIPBASE__H__
#define ENEMYSHIPBASE__H__

#include <dsound.h>
#include <vector>
#include "MySound.h"
#include "BaseObject.h"
#include "Laser.h"

class EnemyShipBase:public BaseObject
{
public:
	EnemyShipBase();
	virtual bool Init(IDirect3DDevice9* d, IDirectSound8 *s) = 0;
	void Update(D3DXVECTOR3 enemyPosition, float timeDelta);
	enum ShipState { HOLD, ATTACK, MOVETOPOSITION };
	enum ShipType { TYPEA, TYPEB, TYPEC, TYPEBIG, STATION, NONE };
	void setState(ShipState t) { state = t; }
	ShipState getState() { return state; }
	ShipType getType() { return type; }
	void SetSpeed(float s);
	bool isDead() { return (health <= 0); }
	std::vector<Laser> lasers;
	void manToPosition(D3DXVECTOR3 epos);
	void setMoveVector(float x, float y, float z);
	void Hit(int damage);
	virtual void FireLaser() {}
	MySound Engine;
	
protected:
	ShipState state;
	ShipType type;
	IDirectSound8* Sound;
	MySound Fire;
	MySound HitSound;
	unsigned int lastShot;
	FLOAT distance, angle;
	float maxSpeed;
	float minSpeed;
	float casSpeed;
	float rotAngle;
	unsigned int laserInterval;
	int range;
	int health;
	D3DXVECTOR3 moveVector;
};



#endif
