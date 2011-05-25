#include "SpaceFightGame.h"

bool SpaceFightGame::Setup()
{
	cam = MyCamera(Device);
	Listener.Init(Sound);
	Listener.setPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	fpscount = GetTickCount();
	fps = 0;

	timeStart = 0;
	timeEnd = 0;
	timerFreq = 0;
	SetFrameLimit(true); 
	msPerFrame = (float)1000 / FRAMELIMITVAL;
	sky = new CubeMap(Device);
	SetupCamera();
	SetupPerspective();

	light.direction = D3DXVECTOR3(0.0f, -1.0f, 1.0f);
	D3DXVec3Normalize(&light.direction, &light.direction);
	light.ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.spec = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	ID3DXBuffer* err = 0;
	D3DXCreateEffectFromFile(Device, "mymesh.fx", 0, 0, D3DXSHADER_DEBUG, 0, &MeshEffect, &err);
	
	if (err)
		MessageBox(0, (char*)err->GetBufferPointer(), 0, 0);

	err = 0;
	D3DXCreateEffectFromFile(Device, "fonteffect.fx", 0, 0, D3DXSHADER_DEBUG, 0, &FontEffect, &err);
	if (err)
		MessageBox(0, (char*)err->GetBufferPointer(), 0, 0);

	D3DXHANDLE mhLight;
	mhLight = MeshEffect->GetParameterByName(0, "gLight");
	MeshEffect->SetValue(mhLight, &light, sizeof(DirLight));

	D3DXHANDLE mhFLight;
	D3DXVECTOR3 a(0.0f, -1.5f, -1.0f);
	D3DXVec3Normalize(&a, &a);
	mhFLight = FontEffect->GetParameterByName(0, "gLightVecW");
	FontEffect->SetValue(mhFLight, &a, sizeof(D3DXVECTOR3)); 

	Ship = 0;
	Portal = 0;

	Button.Init(Sound, "button.wav");
	Button.SetLoop(false);
	Button.setPosition(D3DXVECTOR3(0,0,0));

	continueGame = pexitGame = parrow = pauseTitle = earrow = equitgame = ewin = elose = enewgame = 0;

	state = MAINSCREEN;
	LoadMainScreen();

	return true;
}

void SpaceFightGame::LoadNewGame()
{
	level = 1;
	points = 0;
	typeACount = typeBCount = typeCCount = 0;
	portalPosition = D3DXVECTOR3(0, 0, 100);

	if (Ship != 0)
	{
		delete Ship;
		Ship = 0;
	}
	Ship = new PlayerShip();
	Ship->Init(Device, Sound);

	info = new MyFont(Device, "Level: ");
	info->SetPosition(10, 10, 800, 400);
	info->AddText((float)level);
	info->AddText("\n");

	if (enemyShips.size() != 0)
	{
		std::vector<EnemyShipBase*>::iterator it;
		for(it = enemyShips.begin(); it != enemyShips.end(); ++it)
		{
			delete (*it);
		}
		enemyShips.clear();
	}
	
	EnemyShipBig *bigShip = new EnemyShipBig();
	bigShip->Init(Device, Sound);
	bigShip->TranslateV(portalPosition - D3DXVECTOR3(0, 6, 0));
	enemyShips.push_back(bigShip);

	EnemyShipStation *station = new EnemyShipStation();
	station->Init(Device, Sound);
	station->TranslateV(D3DXVECTOR3(40, 0, 100));
	enemyShips.push_back(station);
	
	GameMusic.Init(Sound, "gameplay.wav");
	GameMusic.Play();

	Mission1.Init(Sound, "mission01.wav");
	Mission1.SetLoop(false);

	Mission2.Init(Sound, "mission02.wav");
	Mission2.SetLoop(false);

	End.Init(Sound, "end.wav");
	End.SetLoop(false);

	endWaitTime = 0;

	Damage.Init(Sound, "damage.wav");
	Damage.SetLoop(false);

	Win.Init(Sound, "win.wav");
	Lose.Init(Sound, "lose.wav");
	EvilLaugh.Init(Sound, "laugh.wav");
	EvilLaugh.SetLoop(false);


	bm1 = be = bm2 = bd = bigship = tower = finalcon = false;
		

	if (Portal != 0)
	{
		delete Portal;
		Portal = 0;
	}
	Portal = new MyMesh();
	Portal->Init(Device, "portal.x");
	Portal->Translate(portalPosition.x, portalPosition.y, portalPosition.z);

	cam.AttachObject(Ship);
}

void SpaceFightGame::LoadMainScreen()
{
	//
	title = new MyFontMesh(Device, "DEEPSPACE", "CloneWars");
	title->Translate(-4.5f, 3.0f, 5.0f);

	newGame = new MyFontMesh(Device, "start", "Zrnic");
	newGame->Scale(0.3f);
	newGame->Translate(0.3f, 0.0f, 2.0f);

	exitGame = new MyFontMesh(Device, "quit", "Zrnic");
	exitGame->Scale(0.3f);
	exitGame->Translate(0.3f, -0.3f, 2.0f);

	arrow = new MyFontMesh(Device, "*", "Zrnic");
	arrow->Scale(0.3f);
	arrow->Translate(0.12f, -0.03f, 2.0f);
	
	theShip = new MyMesh();
	theShip->Init(Device, "ship01.x");
	theShip->Translate(-2.0f, -0.2f, 4.0f);
	theShip->RotateX(3*D3DX_PI/2);

	MenuMusic.Init(Sound, "menu.wav");
	MenuMusic.setPosition(cam.position);
	MenuMusic.Play();

	selectedMenu = 0;
	lastChanged = 0;
}

void SpaceFightGame::LoadEndGameScreen()
{
	cam.position = D3DXVECTOR3(0, 0, 0);
	cam.look = D3DXVECTOR3(0, 0, 1);
	cam.up = D3DXVECTOR3(0, 1, 0);
	cam.Display();

	if (ewin == 0)
	{
		ewin = new MyFontMesh(Device, "C0NGRATS!", "CloneWars");
		ewin->Translate(-3.9f, 3.0f, 5.0f);
	}

	if (elose == 0)
	{
		elose = new MyFontMesh(Device, "Y0U L0SE", "CloneWars");
		elose->Translate(-3.5f, 3.0f, 5.0f);
	}

	if (enewgame == 0)
	{
		enewgame = new MyFontMesh(Device, "new game", "Zrnic");
		enewgame->Scale(0.3f);
		enewgame->Translate(0.3f, 0.0f, 2.0f);
	}

	if (equitgame == 0)
	{
		equitgame = new MyFontMesh(Device, "exit", "Zrnic");
		equitgame->Scale(0.3f);
		equitgame->Translate(0.3f, -0.3f, 2.0f);
	}

	if (earrow == 0)
	{
		earrow = new MyFontMesh(Device, "*", "Zrnic");
		earrow->Scale(0.3f);
		earrow->Translate(0.12f, -0.03f, 2.0f);
	}

	selectedEnd = 0;
}

void SpaceFightGame::PauseScreen()
{
	cam.position = D3DXVECTOR3(0, 0, 0);
	cam.look = D3DXVECTOR3(0, 0, 1);
	cam.up = D3DXVECTOR3(0, 1, 0);
	cam.Display();

	if (pauseTitle == 0)
	{
		pauseTitle = new MyFontMesh(Device, "DEEPSPACE", "CloneWars");
		pauseTitle->Translate(-4.5f, 3.0f, 5.0f);
	}

	if (continueGame == 0)
	{
		continueGame = new MyFontMesh(Device, "continue", "Zrnic");
		continueGame->Scale(0.3f);
		continueGame->Translate(0.3f, 0.0f, 2.0f);
	}
	

	if (pexitGame == 0)
	{
		pexitGame = new MyFontMesh(Device, "exit", "Zrnic");
		pexitGame->Scale(0.3f);
		pexitGame->Translate(0.3f, -0.3f, 2.0f);
	}
	
	
	if (parrow == 0)
	{
		parrow = new MyFontMesh(Device, "*", "Zrnic");
		parrow->Scale(0.3f);
		parrow->Translate(0.12f, -0.03f, 2.0f);
	}

	selectedPause = 0;
}

bool SpaceFightGame::Display(float timeDelta)
{
	if (Device)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&timeStart);
		ClearBuffer(D3DCOLOR_XRGB(0x00,0x00,0x00));
		Device->BeginScene();
		ProcessInput();
		
		sky->Draw(cam.position); // sky (space) is always drawn

		/* display according to the state */

		switch(state)
		{
		case MAINSCREEN:
			title->Draw(FontEffect);
			newGame->Draw(FontEffect);
			exitGame->Draw(FontEffect);
			arrow->Draw(FontEffect);
			theShip->Draw(MeshEffect);
			theShip->RotateX(0.01f);
			theShip->RotateY(0.01f);
			theShip->RotateZ(-0.01f);
			break;
		case PAUSE:
			pauseTitle->Draw(FontEffect);
			continueGame->Draw(FontEffect);
			exitGame->Draw(FontEffect);
			parrow->Draw(FontEffect);
			break;
		case LOADING:
			Listener.setPosition(D3DXVECTOR3(1000, 1000, 1000));
			break;
		case END:
			if (Ship->getHealth() < 0)
				elose->Draw(FontEffect);
			else ewin->Draw(FontEffect);
			enewgame->Draw(FontEffect);
			equitgame->Draw(FontEffect);
			earrow->Draw(FontEffect);
			break;
		case GAMEPLAY:
			info->SetText("Health: ");
			info->AddText((float)Ship->getHealth());
			info->AddText("\nPoints: ");
			info->AddText((float)points);
			info->Draw();
			// players
			Portal->RotateZ(0.01f);
			Portal->Draw(MeshEffect);
			if (Ship->getHealth() > 0)
			{
				Ship->Update(timeDelta);
				Ship->Draw(MeshEffect);
			}

			for (std::vector<EnemyShipBase*>::iterator it = enemyShips.begin(); it != enemyShips.end(); ++it)
			{
				(*it)->Update(Ship->getPosition(), timeDelta);
				(*it)->Draw(MeshEffect);
			}

			bool ed = false;
			std::vector<Explosion*>::iterator temp;
			for(std::vector<Explosion*>::iterator it = explosions.begin(); it != explosions.end(); ++it)
			{
				(*it)->Update(timeDelta);
				if (!(*it)->isAlive())
				{
					ed = true;
					temp = it;
				}
			}
			if (ed) { delete (*temp); explosions.erase(temp);  }

			cam.Update();
			GameMusic.setPosition(Ship->getPosition());
			Mission1.setPosition(Ship->getPosition());
			Mission2.setPosition(Ship->getPosition());
			End.setPosition(Ship->getPosition());
			Damage.setPosition(Ship->getPosition());
			Win.setPosition(Ship->getPosition());
			Lose.setPosition(Ship->getPosition());
			EvilLaugh.setPosition(Ship->getPosition());
			Listener.setPosition(Ship->getPosition());
			CheckGameLogic();
			break;
		}
		
		Device->EndScene();
		Device->Present(0, 0, 0, 0);
		if ( GetTickCount() - fpscount >= 1000)
		{
			fps = 0;
			fpscount = GetTickCount();
		}
		else fps++;
		QueryPerformanceCounter((LARGE_INTEGER*)&timeEnd);
		QueryPerformanceFrequency((LARGE_INTEGER*)&timerFreq);
		double d = (timeEnd - timeStart) * ((float)1/(float)timerFreq);
		if (d < msPerFrame && frameLimit)
		{
			Sleep((DWORD)msPerFrame - (DWORD)d);
		}
	}
	return true;
}



void SpaceFightGame::SetFrameLimit(bool f)
{
	frameLimit = f;
}
bool SpaceFightGame::ColDetect(MyMesh* one, MyMesh* two)
{
	D3DXVECTOR3 min1, max1, min2, max2;
	one->CalcAABBFromOBB(&min1, &max1);
	two->CalcAABBFromOBB(&min2, &max2);

	bool col = false;

	if (
		(max1.x < min2.x) || 
		(min1.x > max2.x) ||
		(max1.y < min2.y) || 
		(min1.y > max2.y) ||
		(max1.z < min2.z) || 
		(min1.z > max2.z)
		)
	{
		col = true;
	}
	return col;
}


void SpaceFightGame::CreateShip(EnemyShipBase::ShipType type)
{
	std::vector<EnemyShipBase*>::iterator it;
	for(it = enemyShips.begin(); it != enemyShips.end(); ++it)
	{
		if((*it)->getState() == EnemyShipBase::MOVETOPOSITION)
		return; // portal is busy
	}

	EnemyShipBase *temp;

	switch(type)
	{
	case EnemyShipBase::TYPEA:
		temp = new EnemyShipA();
		typeACount++;
		break;
	case EnemyShipBase::TYPEB:
		temp = new EnemyShipB();
		typeBCount++;
		break;
	case EnemyShipBase::TYPEC:
		temp = new EnemyShipC();
		typeCCount++;
		break;
	default:
		temp = 0;
		break;
	}
	if (temp == 0) return; // failed!
	temp->Init(Device, Sound);
	temp->TranslateV(portalPosition);
	temp->setMoveVector(portalPosition.x, portalPosition.y, portalPosition.z + 5);
	enemyShips.push_back(temp);
}

bool SpaceFightGame::CheckGameLogic()
{
	bool goOn = true;
	if (Ship->getHealth() < 500 && !bd )
	{
		Damage.Play();
		bd = true;
	}
	if (Ship->getHealth() < 0 && !finalcon)
	{
		finalcon = true;
		GameMusic.Stop();
		Lose.Play();
		EvilLaugh.Play();
		for(std::vector<EnemyShipBase*>::iterator i=enemyShips.begin(); i != enemyShips.end(); ++i)
			(*i)->setState(EnemyShipBase::HOLD);
		// LoadEndGameScreen();
		Ship->Engine.Stop();
		endWaitTime = GetTickCount();
		Explosion* e = new Explosion(Device, Sound, Ship->getPosition());
		explosions.push_back(e);
		return true;
	}
	if (endWaitTime != 0 && GetTickCount() - endWaitTime > 5000 && Ship->getHealth() < 0)
	{
		LoadEndGameScreen();
		state = END;
	}
	switch(level)
	{
	case 1:
		if (!bm1)
		{
			Mission1.Play();
			bm1 = true;
		}
		if (points < 500)
		{
			if (typeACount == 0)
				CreateShip(EnemyShipBase::TYPEA);
		}
		else level = 2;
		break;
	case 2:
		if (points < 1500)
		{
			if (typeBCount == 0)
				CreateShip(EnemyShipBase::TYPEB);
		}
		else level = 3;
		break;
	case 3:
		if (points < 3000)
		{
			if (typeCCount == 0)
				CreateShip(EnemyShipBase::TYPEC);
		}
		else level = 4;
		break;
	case 4:
		if (points < 4000)
		{
			if (typeACount == 0)
				CreateShip(EnemyShipBase::TYPEA);
			if (typeBCount == 0)
				CreateShip(EnemyShipBase::TYPEB);
		}
		if (points > 4800)
			level = 5;
		break;
	case 5:
		if (points < 6000)
		{
			if (typeACount == 0)
				CreateShip(EnemyShipBase::TYPEA);
			if (typeBCount == 0)
				CreateShip(EnemyShipBase::TYPEB);
			if (typeCCount == 0)
				CreateShip(EnemyShipBase::TYPEC);
		}
		if (points > 7500)
			level = 6;
		break;
	case 6:
		if (typeACount == 0 && typeBCount == 0 && typeCCount == 0 && !bm2)
		{
			Mission2.Play();
			bm2 = true;
		}
		if (bigship && tower && !be)
		{
			GameMusic.Stop();
			Win.Play();
			End.Play();
			endWaitTime = GetTickCount();
			be = true;
		}
		if (endWaitTime != 0)
		{
			if (GetTickCount() - endWaitTime > 15000 && Ship->getHealth() > 0)
			{
				Ship->Engine.Stop();
				LoadEndGameScreen();
				state = END;
			}
		}
		break;

	}

	if (Ship->getHealth() < 0) return goOn; // no need to do these if the ship is dead

	if (!ColDetect(Ship, Portal))
	{
		Ship->TranslateV(0.06f*(Ship->getPosition()-portalPosition));
	}
	
	std::vector<EnemyShipBase*>::iterator it;
	for(it = enemyShips.begin();it != enemyShips.end(); it++)
	{
		if (!ColDetect(Ship, (*it)))
		{
			if ((*it)->getType() == EnemyShipBase::TYPEBIG ||(*it)->getType() == EnemyShipBase::STATION)
			{
				Ship->Scratched();
				Ship->TranslateV(0.01f*(Ship->getPosition()-(*it)->getPosition()));
			}
			else (*it)->TranslateV(0.05f*((*it)->getPosition() - Ship->getPosition()));
		}
		// check its lasers
		std::vector<Laser>::iterator lit;
		for(lit = (*it)->lasers.begin(); lit != (*it)->lasers.end(); lit++)
		{
			(*lit).Draw(MeshEffect);
			if (!ColDetect((MyMesh*)&(*lit), Ship))
			{
				Ship->Hit((*lit).getDamage());
				(*lit).TargetHit();
			}
		}

		// check with other ships
		// the trick is that we should check with the rest of the remaining, not all
		std::vector<EnemyShipBase*>::iterator rem;
		for(rem = it; rem != enemyShips.end(); rem++)
		{
			if (rem == it) continue;
			if (!ColDetect((*it), (*rem)))
			{
				if( (*it)->getType() == EnemyShipBase::TYPEBIG)
					(*rem)->TranslateV(0.01f*((*rem)->getPosition() - (*it)->getPosition()));
				else
					(*it)->TranslateV(0.05f*((*it)->getPosition() - (*rem)->getPosition()));
			}
		}
	}

	std::vector<EnemyShipBase*>::iterator temp;
	bool dead = false;
	for(std::vector<Laser>::iterator it = Ship->lasers.begin(); it != Ship->lasers.end(); ++it)
	{
		(*it).Draw(MeshEffect);
		for(std::vector<EnemyShipBase*>::iterator enit = enemyShips.begin(); enit != enemyShips.end(); ++enit)
		{
			if (!ColDetect((MyMesh*)&(*it), (*enit)))
			{
				(*enit)->Hit((*it).getDamage());
				(*it).TargetHit();

				if ((*enit)->isDead())
				{
					// get an explosion at this location:
					// (*enit)->getPosition();
					Explosion* tmp = new Explosion(Device, Sound, (*enit)->getPosition());
					explosions.push_back(tmp);
					dead = true;
					temp = enit;
				}
				switch((*enit)->getType())
				{
				case EnemyShipBase::TYPEA:
					points += 20;
					if (dead) points += 250;
					break;
				case EnemyShipBase::TYPEB:
					points += 30;
					if (dead) points += 350;
					break;
				case EnemyShipBase::TYPEC:
					points += 40;
					if (dead) points += 500;
					break;
				case EnemyShipBase::TYPEBIG:
					points += 5;
					if (dead) { points += 10000; bigship = true; }
					break;
				case EnemyShipBase::STATION:
					points += 5;
					if (dead) { points += 10000; tower = true; }
					break;
				}
			}
		}

	}
	if (dead)
	{
		switch ((*temp)->getType())
		{
		case EnemyShipBase::TYPEA:
			typeACount--;
			break;
		case EnemyShipBase::TYPEB:
			typeBCount--;
			break;
		case EnemyShipBase::TYPEC:
			typeCCount--;
			break;
		default:
			break;
		}

		delete (*temp);
		enemyShips.erase(temp);
	}
	

	return goOn;
}


void SpaceFightGame::ProcessKeyboard()
{
#define KEYDOWN(name, key) (name[key] & 0x80)
	char	buffer[256];
	HRESULT hr;
	hr = Keyboard->GetDeviceState(sizeof(buffer), (LPVOID)&buffer);
	if (FAILED(hr))
	{
		hr = Keyboard->Acquire();
		while(hr == DIERR_INPUTLOST)
		{
			hr = Keyboard->Acquire();
		}
		return;
	}
	// dont forget this
	if (KEYDOWN(buffer, DIK_ESCAPE))
	{
		quit = true;
	}
	switch(state)
	{
	case MAINSCREEN:
		if (KEYDOWN(buffer, DIK_DOWN))
		{
			if (GetTickCount() - lastChanged < 200)
				break;
			if (selectedMenu == 0)
			{
				selectedMenu = 1;
				arrow->Translate(0.0f, -0.3f, 0.0f);
				Button.Play();
			}
			lastChanged = GetTickCount();
		}
		if (KEYDOWN(buffer, DIK_UP))
		{
			if (GetTickCount() - lastChanged < 200)
				break;
			if (selectedMenu == 1)
			{
				selectedMenu = 0;
				arrow->Translate(0.0f, 0.3f, 0.0f);
				Button.Play();
			}
			lastChanged = GetTickCount();
		}
		if (KEYDOWN(buffer, DIK_RETURN))
		{
			if (selectedMenu == 1)
				quit = true;
			else if (selectedMenu == 0)
			{
				// start a new game
				MenuMusic.Stop();
				state = LOADING;
				LoadNewGame();
				state = GAMEPLAY;
			}
			lastChanged = GetTickCount();
		}
		break;
	case PAUSE:
		if (KEYDOWN(buffer, DIK_DOWN))
		{
			if (GetTickCount() - lastChanged < 200)
				break;
			if (selectedPause == 0)
			{
				selectedPause = 1;
				parrow->Translate(0.0f, -0.3f, 0.0f);
				Button.Play();
			}
			lastChanged = GetTickCount();
		}
		if (KEYDOWN(buffer, DIK_UP))
		{
			if (GetTickCount() - lastChanged < 200)
				break;
			if (selectedPause == 1)
			{
				selectedPause = 0;
				parrow->Translate(0.0f, 0.3f, 0.0f);
				Button.Play();
			}
			lastChanged = GetTickCount();
		}
		if (KEYDOWN(buffer, DIK_RETURN))
		{
			if (selectedPause == 1)
				quit = true;
			else if (selectedPause == 0)
			{
				// continue game
				cam.position = Ship->getPosition();
				cam.up = Ship->getUp();
				cam.look = Ship->getLook();
				Ship->Engine.Play();
				for(unsigned int i=0;i<enemyShips.size();i++)
				enemyShips[i]->Engine.Play();
				state = GAMEPLAY;
			}
			lastChanged = GetTickCount();
		}
		break;
	case END:
		if (KEYDOWN(buffer, DIK_DOWN))
		{
			if (GetTickCount() - lastChanged < 200)
				break;
			if (selectedEnd == 0)
			{
				selectedEnd = 1;
				earrow->Translate(0.0f, -0.3f, 0.0f);
				Button.Play();
			}
			lastChanged = GetTickCount();
		}
		if (KEYDOWN(buffer, DIK_UP))
		{
			if (GetTickCount() - lastChanged < 200)
				break;
			if (selectedEnd == 1)
			{
				selectedEnd = 0;
				earrow->Translate(0.0f, 0.3f, 0.0f);
				Button.Play();
			}
			lastChanged = GetTickCount();
		}
		if (KEYDOWN(buffer, DIK_RETURN))
		{
			if (selectedEnd == 1)
				quit = true;
			else if (selectedEnd == 0)
			{
				End.Stop();
				Win.Stop();
				state = LOADING;
				LoadNewGame();
				state = GAMEPLAY;
			}
			lastChanged = GetTickCount();
		}
		break;
		break;
	case GAMEPLAY:
		if (KEYDOWN(buffer, DIK_P))
		{
			Ship->Engine.Stop();
			for(unsigned int i=0;i<enemyShips.size();i++)
				enemyShips[i]->Engine.Stop();
			state = PAUSE;
			PauseScreen();
		}
		if (KEYDOWN(buffer, DIK_UP))
		{
			Ship->RotateX(2*ANGLE);
		}
		if (KEYDOWN(buffer, DIK_DOWN))
		{
			Ship->RotateX(-2*ANGLE);
		}
		if (KEYDOWN(buffer, DIK_LEFT))
		{
			Ship->RotateY(-2*ANGLE);
		}
		if (KEYDOWN(buffer, DIK_RIGHT))
		{
			Ship->RotateY(2*ANGLE);
		}

		if (KEYDOWN(buffer, DIK_K))
		{
			Explosion *t = new Explosion(Device, Sound, Ship->getPosition() + D3DXVECTOR3(0, 0, 20));
			explosions.push_back(t);
		}

		if (KEYDOWN(buffer, DIK_J))
		{
			Explosion *t = new Explosion(Device, Sound, Ship->getPosition() + D3DXVECTOR3(0, 0, 100));
			explosions.push_back(t);
		}

		if (KEYDOWN(buffer, DIK_L))
		{
			Explosion *t = new Explosion(Device, Sound, Ship->getPosition() + D3DXVECTOR3(0, 0, 1));
			explosions.push_back(t);
		}

		if (KEYDOWN(buffer, DIK_A))
		{
			Ship->RotateZ(2*ANGLE);
		}
		if (KEYDOWN(buffer, DIK_D))
		{
			Ship->RotateZ(-2*ANGLE);
		}
		if (KEYDOWN(buffer, DIK_W))
		{
			Ship->SpeedUp();
		}
		else if (KEYDOWN(buffer, DIK_S))
		{
			Ship->SpeedDown();
		}
		else  { Ship->SpeedNormal(); }

		if (KEYDOWN(buffer, DIK_LCONTROL))
		{
			Ship->FireLaser();
		}
		break;
	}
	/*
	if (KEYDOWN(buffer, DIK_ESCAPE))
	{
		quit = true;
	}
	
	*/
}

void SpaceFightGame::ProcessMouse()
{
	DIMOUSESTATE mouseState;
	HRESULT hr;
	hr = Mouse->GetDeviceState(sizeof(mouseState), (LPVOID)&mouseState);
	if (FAILED(hr))
	{
		hr = Mouse->Acquire();
		while(hr == DIERR_INPUTLOST)
		{
			hr = Mouse->Acquire();
		}
		return;
	}
	switch(state)
	{
	case GAMEPLAY:
		if (mouseState.lX != 0)
		{
			Ship->RotateY((float)mouseState.lX/20.5f); // maybe Z?
		}
		if (mouseState.lY != 0)
		{
			Ship->RotateX((float)mouseState.lY/20.5f);
		}
		if (mouseState.rgbButtons[0] & 0x80)
		{
			Ship->FireLaser();
		}
		break;
	}
}

void SpaceFightGame::ProcessJoystick()
{
	HRESULT hr;
	Joystick->Poll();
	DIJOYSTATE2 js;
	hr = Joystick->GetDeviceState(sizeof(DIJOYSTATE2), &js);
	if (FAILED(hr))
	{
		hr = Joystick->Acquire();
		while(hr == DIERR_INPUTLOST)
		{
			hr = Joystick->Acquire();
		}
		return;
	}
	switch(state)
	{
	case GAMEPLAY:
		if (js.lX != 32767)
		{
			float val = ((float)js.lX/65535)*4;
			if (val < 2.0f)
				val = - (2.0f - val);
			else
				val = val - 2.0f;
			Ship->RotateY(val);
		}
		if (js.lY != 32767)
		{
			float val = ((float)js.lY/65535)*4;
			if (val < 2.0f)
				val = val - 2.0f;
			else
				val = - (2.0f - val);
			Ship->RotateX(-val);
		}
		
		if (js.lZ != 32767)
		{
			float val = ((float)js.lZ/65535)*4;
			if (val < 2.0f)
				val = val - 2.0f;
			else
				val = - (2.0f - val);
			Ship->RotateZ(-val);
		}
		if (js.rgbButtons[4] & 0x80)
		{
			Ship->SpeedUp();
		}
		else if (js.rgbButtons[5] & 0x80)
		{
			Ship->SpeedDown();
		}
		if (js.rgbButtons[0] & 0x80)
		{
			Ship->FireLaser();
		}

		break;
	}
	/*
	*/
	/* // check this later
	if (js.rgdwPOV[0] >= 0)
	{
		if (js.rgdwPOV[0] >= 0 && js.rgdwPOV[0] < 9000)
			Ship->RotateX(ANGLE);
		if (js.rgdwPOV[0] <= 18000 && js.rgdwPOV[0] > 9000)
			Ship->RotateX(-ANGLE);
		if (js.rgdwPOV[0] == 27000)
			Ship->RotateY(-ANGLE);
		if (js.rgdwPOV[0] == 9000)
			Ship->RotateY(ANGLE);
	}
	*/
}

void SpaceFightGame::SetupCamera()
{
	cam.Display();
}

void SpaceFightGame::SetupPerspective()
{
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI*0.5, getFWidth()/getFHeight(), 0.1f, 10000.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);
}


void SpaceFightGame::ClearBuffer(DWORD dwColor)
{
	Device->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, dwColor, 1.0f, 0 );
}
