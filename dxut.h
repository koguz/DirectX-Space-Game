/* Kaya Oguz
 * An attempt to have basic utility stuff for DirectX 
 * 8/2/08
 */ 
#ifndef DXUT__H__
#define DXUT__H__

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dsound.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib,"winmm.lib")

#define WIN32_MEAN_AND_LEAN

#include <d3dx9.h> // d3d
#include <dsound.h>
#include <dinput.h>
#include "MySound.h"

namespace dxut
{
	const D3DXCOLOR WHITE(1.0f, 1.0f, 1.0f, 1.0f);

	struct Mtrl
	{
		Mtrl()
			:ambient(WHITE), diffuse(WHITE), spec(WHITE), specPower(8.0f){}
		Mtrl(const D3DXCOLOR& a, const D3DXCOLOR& d, 
			 const D3DXCOLOR& s, float power)
			:ambient(a), diffuse(d), spec(s), specPower(power){}

		D3DXCOLOR ambient;
		D3DXCOLOR diffuse;
		D3DXCOLOR spec;
		float specPower;
	};

	struct DirLight
	{
		D3DXCOLOR ambient;
		D3DXCOLOR diffuse;
		D3DXCOLOR spec;
		D3DXVECTOR3 direction;
	};

	class DirectXGame
	{
		/* Typically, in a game, we want to use 
		 *
		 * - Direct3D graphics
		 * - DirectSound
		 * - DirectInput
		 * 
		 * so we init them all...
		 */
	protected:
		HWND hwnd; // window handle
		HINSTANCE hinstance;  // instance
		IDirect3DDevice9* Device; // D3D Device
		LPDIRECTINPUT8 Dinput;
		LPDIRECTINPUTDEVICE8 Keyboard;
		LPDIRECTINPUTDEVICE8 Mouse;
		LPDIRECTINPUTDEVICE8 Joystick;
		LPDIRECTSOUND8 Sound;
		MyListener Listener;

		int width, height;

		bool windowed, k, m, j;
		bool quit;
		
	public:
		DirectXGame();
		~DirectXGame();
		bool Init(
			HINSTANCE hinstance, 
			int width, 
			int height, 
			bool windowed,
			bool initKeyboard,
			bool initMouse,
			bool initJoystick
			);

		bool InitD3D();
		bool InitDinput(bool keyboard, bool mouse, bool joystick);
		bool InitSound();

		void Run();
		void WINAPI ProcessInput();

		virtual bool Display(float timeDelta);
		virtual void ProcessKeyboard();
		virtual void ProcessMouse();
		virtual void ProcessJoystick();


		// 
		WPARAM EnterMsgLoop();
		void Error(LPCSTR title, LPCSTR msg);
		void ReleaseInputDevice(LPDIRECTINPUTDEVICE8 d);
		HWND getHwnd() { return hwnd; }
		LPDIRECTINPUT8 getDinput() { return Dinput; }
		LPDIRECTINPUTDEVICE8* getPJoystick() { return &Joystick; }
		IDirect3DDevice9* getDevice() { return Device; }
		int getWidth() { return width; }
		int getHeight() { return height; }
		float getFWidth() { return (float)width; }
		float getFHeight() { return (float)height; }
	};

BOOL CALLBACK JoystickCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);

LRESULT CALLBACK WndProc(
	HWND hwnd,
	UINT msg, 
	WPARAM wParam,
	LPARAM lParam);


}


#endif
