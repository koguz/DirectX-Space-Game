#include "SpaceFightGame.h"

int WINAPI WinMain(HINSTANCE hinstance,
				   HINSTANCE prevInstance, 
				   PSTR cmdLine,
				   int showCmd)
{
	SpaceFightGame Game;
	if (!Game.Init(hinstance, 800, 600, true, true, true, true))
	{
		MessageBox(0, "Application Init Failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	if (!Game.Setup())
	{
		MessageBox(0, "Game Setup Failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	
	Game.Run();
	return 0;
}


