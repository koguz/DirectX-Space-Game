#include "dxut.h"
dxut::DirectXGame::DirectXGame()
{
	hwnd = 0;
	quit = false;
	Device = 0;
	Keyboard = 0;
	Mouse = 0;
	Joystick = 0;
	Sound = 0;
}

dxut::DirectXGame::~DirectXGame()
{
	Device->Release();
	ReleaseInputDevice(Keyboard);
	ReleaseInputDevice(Mouse);
	ReleaseInputDevice(Joystick);
	Dinput->Release();
	Sound->Release();
}

void dxut::DirectXGame::ReleaseInputDevice(LPDIRECTINPUTDEVICE8 d)
{
	if (d)
	{
		d->Unacquire();
		d->Release();
	}
}
void dxut::DirectXGame::Error(LPCSTR title, LPCSTR msg)
{
	// a simple wrapper for msgbox
	MessageBox(hwnd, msg, title, MB_OK | MB_ICONERROR);
}


bool dxut::DirectXGame::Init(
						HINSTANCE hInstance, 
						int pwidth, 
						int pheight, 
						bool pwindowed,
						bool kp,
						bool mp,
						bool jp
						)
{
	hinstance = hInstance;
	windowed = pwindowed;
	k = kp; m = mp; j = jp;

	width = pwidth;
	height = pheight;

	// creating a main window
	WNDCLASS wc;

	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = (WNDPROC)dxut::WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = "Direct3D9App";

	if( !RegisterClass(&wc) ) 
	{
		Error("Error!", "RegisterClass FAILED!!");
		return false;
	}
		
	hwnd = CreateWindow("Direct3D9App", "Deepspace - Kaya Oguz - 0705907 - oguz.name.tr", 
		WS_EX_TOPMOST,
		0, 0, width, height,
		0 /*parent hwnd*/, 0 /* menu */, hInstance, 0 /*extra*/); 

	if( !hwnd )
	{
		Error("Error!", "CreateWindow FAILED!!");
		return false;
	}

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	if (!InitD3D())
		return false;
	if (!InitDinput(k, m, j))
		return false;
	InitSound();
	return true;
}

bool dxut::DirectXGame::InitD3D()
{
	HRESULT hr = 0;
	IDirect3D9* d3d9 = 0;
	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (!d3d9)
	{
		Error("Error", "Direct3DCreate9 failed");
		return false;
	}

	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	int vp = 0;
	if(caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else 
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	D3DDISPLAYMODE displayMode;
	d3d9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode);

	D3DPRESENT_PARAMETERS d3dpp;
	memset(&d3dpp, 0, sizeof(D3DPRESENT_PARAMETERS));
	d3dpp.BackBufferWidth            = displayMode.Width;
	d3dpp.BackBufferHeight           = displayMode.Height;
	d3dpp.BackBufferFormat           = displayMode.Format;
	d3dpp.BackBufferCount            = 1;
	d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality         = 0;
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
	d3dpp.hDeviceWindow              = hwnd;
	d3dpp.Windowed                   = windowed;
	d3dpp.EnableAutoDepthStencil     = true; 
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;// D3DFMT_D16
	d3dpp.Flags                      = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

	hr = d3d9->CreateDevice(
		D3DADAPTER_DEFAULT, // primary adapter
		D3DDEVTYPE_HAL,     // device type
		hwnd,               // window associated with device
		vp,                 // vertex processing
	    &d3dpp,             // present parameters
	    &Device);            // return created device

	if( FAILED(hr) )
	{
		// try again using a 16-bit depth buffer
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		
		hr = d3d9->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hwnd,
			vp,
			&d3dpp,
			&Device);

		if( FAILED(hr) )
		{
			d3d9->Release(); // done with d3d9 object
			Error("Error", "Create Device Failed");
			return false;
		}
	}

	d3d9->Release();

	return true;
}

bool dxut::DirectXGame::InitDinput(bool k, bool m, bool j)
{
	HRESULT hr;
	hr = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&Dinput, NULL);
	if (FAILED(hr))
	{
		Error("Error", "Could not init DirectInput");
		return false;
	}

	if (k)
	{
		hr = Dinput->CreateDevice(GUID_SysKeyboard, &Keyboard, NULL);
		if (FAILED(hr))
		{
			Error("InitDinput, CreateDevice", "Could not create keyboard device");
			return false;
		}

		hr = Keyboard->SetDataFormat(&c_dfDIKeyboard);
		if (FAILED(hr))
		{
			Error("InitDinput, SetDataformat", "Could not set keyboard data format");
			return false;
		}

		hr = Keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if (FAILED(hr))
		{
			Error("InitDinput, SetCooperativeLevel", "Could not set keyboard behaviour");
			return false;
		}

		Keyboard->Acquire();
	}

	if (m)
	{
		hr = Dinput->CreateDevice(GUID_SysMouse, &Mouse, NULL);
		if (FAILED(hr))
		{
			Error("InitDinput, CreateDevice", "Could not create mouse device");
			return false;
		}

		hr = Mouse->SetDataFormat(&c_dfDIMouse);
		if (FAILED(hr))
		{
			Error("InitDinput, SetDataformat", "Could not set mouse data format");
			return false;
		}

		hr = Mouse->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
		if (FAILED(hr))
		{
			Error("InitDinput, SetCooperativeLevel", "Could not set mouse behaviour");
			return false;
		}

		Mouse->Acquire();
	}

	if(j)
	{
		hr = Dinput->EnumDevices(DI8DEVCLASS_GAMECTRL, dxut::JoystickCallback, this, DIEDFL_ATTACHEDONLY);
		if (FAILED(hr))
		{
			Error("InitDinput, EnumDevice", "Could not enum for joysticks");
			/* instead of returning false, i think it's better to disable joystick */
			// return false;
			j = false;
			return true;
		}

		if(!Joystick)
		{
			j = false;
			return true;
		}

		hr = Joystick->SetDataFormat(&c_dfDIJoystick2);
		if (FAILED(hr))
		{
			Error("InitDinput, Setdataformat", "Could not set Joystick data format");
			return false;
		}
		hr = Joystick->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
		if (FAILED(hr))
		{
			Error("InitDinput, SetCoopLevel", "Could not set joystick cooperative level");
			return false;
		}

		Joystick->Acquire();
	}

	return true;
}

void WINAPI dxut::DirectXGame::ProcessInput() // this can be redefined in a subclass...
{
	if (Keyboard)
	{
		ProcessKeyboard();
	}

	if (Mouse)
	{
		ProcessMouse();
	}

	if (Joystick)
	{
		ProcessJoystick();
	}
}

WPARAM dxut::DirectXGame::EnterMsgLoop()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	static float lastTime = (float)timeGetTime();

	while(msg.message != WM_QUIT && !quit)
	{
		if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
        {	
			float currTime  = (float)timeGetTime();
			float timeDelta = (currTime - lastTime)*0.001f;
			Display(timeDelta);
			lastTime = currTime;
        }
	}
	return msg.wParam;
}

bool dxut::DirectXGame::Display(float timeDelta)
{
	return true;
}

void dxut::DirectXGame::ProcessKeyboard() {}
void dxut::DirectXGame::ProcessMouse() {}
void dxut::DirectXGame::ProcessJoystick() {}

void dxut::DirectXGame::Run()
{
	EnterMsgLoop();
}

bool dxut::DirectXGame::InitSound()
{
	HRESULT hr;
	hr = DirectSoundCreate8(NULL, &Sound, NULL);
	if (FAILED(hr))
	{
		Error("DirectSound Error", "Sound Device could NOT be created!");
		Sound = 0;
		return true; // no need to stop execution...
	}

	hr = Sound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
	if (FAILED(hr))
	{
		Error("Sound Cooperative Error", "Could not set cooperative level of the sound device!");
		Sound->Release(); // release it
		Sound = 0; // will not be heard anyway
		return true;
	}
	return true;
}


LRESULT CALLBACK dxut::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

BOOL CALLBACK dxut::JoystickCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext)
{
	HRESULT hr;
	DirectXGame* temp = (DirectXGame*) pContext;
	hr = temp->getDinput()->CreateDevice(pdidInstance->guidInstance, temp->getPJoystick(), NULL);
	if (FAILED(hr))
	{
		temp->Error("Callback func. err", "Can not create joystick device!");
		return DIENUM_CONTINUE;
	}
	return DIENUM_STOP;
}

