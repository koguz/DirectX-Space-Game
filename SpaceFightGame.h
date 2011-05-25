#ifndef SPACEFIGHTGAME__H__
#define SPACEFIGHTGAME__H__

#define FRAMELIMITVAL 60.0f

#include "dxut.h"
#include "MyCamera.h"
#include "MySound.h"
#include "BaseObject.h"
#include "PlayerShip.h"
#include "EnemyShipA.h"
#include "EnemyShipB.h"
#include "EnemyShipC.h"
#include "EnemyShipBig.h"
#include "EnemyShipStation.h"
#include "MyFont.h"
#include "CubeMap.h"
#include "Explosion.h"
#include "MyFontMesh.h"

using namespace dxut;

class SpaceFightGame:public DirectXGame
{
public:
//	SpaceFightGame(); //{}
	bool Setup();
	void SetupCamera();
	void SetupPerspective();
	void ClearBuffer(DWORD dwColor);
	bool Display(float timeDelta);
	double fpscount;
	int fps;

	void ProcessKeyboard();
	void ProcessMouse();
	void ProcessJoystick();
	void SetFrameLimit(bool f);
	bool CheckGameLogic();
	bool ColDetect(MyMesh* one, MyMesh* two);
	void CreateShip(EnemyShipBase::ShipType type);

private:
	unsigned __int64 timeStart;
	unsigned __int64 timeEnd;
	unsigned __int64 timerFreq;
	bool frameLimit;
	float msPerFrame;
	int level;
	int points;

	void LoadMainScreen();
	void LoadNewGame();
	void LoadEndGameScreen();
	void PauseScreen();

	enum GameState { LOADING, MAINSCREEN, GAMEPLAY, PAUSE, END };
	GameState state;

	MyCamera cam;

	// main screen objects
	MyFontMesh *title, *newGame, *exitGame, *arrow;
	MyMesh *theShip;
	int selectedMenu; // 0 for new game, 1 for exit
	int lastChanged;

	// pause screen
	MyFontMesh *continueGame, *pauseTitle, *pexitGame, *parrow;
	int selectedPause;

	// game end screen;
	MyFontMesh *enewgame, *equitgame, *earrow, *ewin, *elose;
	int selectedEnd;
	int endWaitTime;

	// 
	PlayerShip *Ship;
	std::vector<EnemyShipBase*> enemyShips;
	std::vector<Explosion*> explosions;
	int typeACount, typeBCount, typeCCount;
	D3DXVECTOR3 portalPosition;
	
	MyMesh *Portal; 
	MySound GameMusic, MenuMusic, Button;
	MySound Mission1, End, Mission2, Damage, Win, Lose, EvilLaugh;
	bool bm1, be, bm2, bd;
	bool bigship, tower;
	bool finalcon;
	CubeMap *sky;
	MyFont *info;

	ID3DXEffect *MeshEffect, *FontEffect;
	DirLight light;
};


#endif