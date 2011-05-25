#ifndef MYSOUND__H__
#define MYSOUND__H__

#include <d3dx9.h>
#include <dsound.h>
#include <vector>

class MySound
{
protected:
	LPDIRECTSOUND8 Device;
	LPDIRECTSOUNDBUFFER buffer;
	LPDIRECTSOUND3DBUFFER buffer3d;
	D3DVECTOR position;
	bool loop;
public:
	MySound();
	~MySound();
	void Init(IDirectSound8 *Device, char* filename);
	void Play();
	void Stop();
	void Rewind();
	void SetLoop(bool l) { loop = l; }
	void SetVolume(LONG val);
	D3DVECTOR getPosition() { return position; }
	void setPosition(D3DXVECTOR3 pos);
	void setFrequency(LONG freq);
};

class MyListener
{
protected:
	IDirectSound8* Device;
	LPDIRECTSOUND3DLISTENER8 Listener;
	D3DVECTOR position;
public:
	MyListener();
	~MyListener();
	void Init(LPDIRECTSOUND8 Device);
	D3DVECTOR getPosition() { return position; }
	void setPosition(D3DXVECTOR3 pos);
};


#endif
