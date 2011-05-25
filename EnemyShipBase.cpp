#include "EnemyShipBase.h"

EnemyShipBase::EnemyShipBase():BaseObject()
{
	lastShot = 0;
	state = MOVETOPOSITION;
	type = NONE;
	distance = angle = 0;
	speed = casSpeed = maxSpeed = minSpeed = 0.0f;
	laserInterval = 500; // twice a second
	range = 10;
	health = 1;
	rotAngle = 0.0f;
	moveVector = D3DXVECTOR3(0, 0, 0);
}

void EnemyShipBase::setMoveVector(float x, float y, float z)
{
	moveVector.x = x;
	moveVector.y = y;
	moveVector.z = z;
}

void EnemyShipBase::Hit(int damage)
{
	health -= damage;
	HitSound.Play();
}

void EnemyShipBase::Update(D3DXVECTOR3 enemyPosition, float timeDelta)
{
	time = timeDelta;
	std::vector<Laser>::iterator temp;
	std::vector<Laser>::iterator toDel;
	bool d = false;

	switch(state)
	{
	case MOVETOPOSITION:
		if (D3DXVec3Length(&(moveVector - position)) < 1)
			state = ATTACK;
		else manToPosition(moveVector);

		break;
	case HOLD:
		SetSpeed(minSpeed);
		break;
	case ATTACK:
		manToPosition(enemyPosition);
		
		if (distance < range && distance > 2 && angle < D3DX_PI / 36 && angle > 0)
			FireLaser();
		if (distance < 5)
			SetSpeed(minSpeed);
		else if (distance > range)
			SetSpeed(maxSpeed);
		else SetSpeed(casSpeed);

		break;
	}

	for(temp = lasers.begin(); temp != lasers.end(); temp++)
	{
		(*temp).Update(time);
		// (*temp).Draw();
		if (abs((*temp).getPosition().x) > 200 || abs((*temp).getPosition().y) > 200 || abs((*temp).getPosition().z) > 200 )
		{
			toDel = temp;
			d = true;
		}
		if((*temp).getHit())
		{
			toDel = temp;
			d = true;
		}
	}
	if (d) 
		lasers.erase(toDel);

	TranslateV(speed * look * time);
	Engine.setPosition(position);
	Fire.setPosition(position);
	HitSound.setPosition(position);
}

void EnemyShipBase::manToPosition(D3DXVECTOR3 epos)
{
	D3DXVECTOR3 man = epos - position;
	distance = D3DXVec3Length(&man);
	D3DXVECTOR3 uman;
	D3DXVec3Normalize(&uman, &man);

	angle = acos(D3DXVec3Dot(&uman, &look));

	if (angle >= 0)
	{
		FLOAT lr = D3DXVec3Dot(&uman, &right);
		if (lr > 0)
			RotateY(rotAngle);
		else RotateY(-rotAngle);

		FLOAT ud = D3DXVec3Dot(&uman, &up);
		if (ud > 0)
			RotateX(-rotAngle);
		else RotateX(rotAngle);
	}
}

void EnemyShipBase::SetSpeed(float s)
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

