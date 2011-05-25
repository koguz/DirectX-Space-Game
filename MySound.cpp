#include "MySound.h"

MySound::MySound()
{
	Device = 0;
	buffer = 0;
	buffer3d = 0;
	loop = true;
}

MySound::~MySound()
{
	Device = 0; // dxut releases this
	if (buffer)
	{
		buffer->Release(); buffer = NULL;
	}
	if (buffer3d)
	{
		buffer3d->Release(); buffer3d = NULL;
	}
}

void MySound::Init(IDirectSound8 *d, char* filename)
{
	Device = d;

	HMMIO wavefile;
	wavefile = mmioOpen(filename, 0, MMIO_READ|MMIO_ALLOCBUF);
	if (wavefile == NULL)
	{
		MessageBox(0, "Could not read WAV file", "Error", 0);
		return;
	}

	MMCKINFO parent;
	memset(&parent, 0, sizeof(MMCKINFO));
	parent.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mmioDescend(wavefile, &parent, 0, MMIO_FINDRIFF);

	MMCKINFO child;
	memset(&child,0,sizeof(MMCKINFO));
	child.fccType = mmioFOURCC('f','m','t',' ');
	mmioDescend(wavefile,&child,&parent,0);

	WAVEFORMATEX wavefmt;
	mmioRead(wavefile,(char*)&wavefmt,sizeof(wavefmt));
	if(wavefmt.wFormatTag != WAVE_FORMAT_PCM)
	{
		MessageBox(0, "LoadDirectSound mmioRead", "Error", 0);
		return;
	}

	//find the wave data chunk
	mmioAscend(wavefile,&child,0);
	child.ckid = mmioFOURCC('d','a','t','a');
	mmioDescend(wavefile,&child,&parent,MMIO_FINDCHUNK);

	//create a directsound buffer to hold wave data
	DSBUFFERDESC bufdesc;
	memset(&bufdesc,0,sizeof(DSBUFFERDESC));
	bufdesc.dwSize = sizeof(DSBUFFERDESC);


	bufdesc.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLVOLUME;

	bufdesc.dwBufferBytes = child.cksize;
	bufdesc.lpwfxFormat = &wavefmt;
	if(DS_OK != ( Device->CreateSoundBuffer(&bufdesc, &buffer, NULL)))
	{
		MessageBox(0,"LoadDirectSound CreateSoundBuffer", "Error", 0);
		return;
	}

	//write wave data to directsound buffer you just created
	void *write1=0,*write2=0;
	unsigned long length1,length2;
	buffer->Lock(0,child.cksize,&write1,&length1,&write2,&length2,0);
	if(write1>0)
		mmioRead(wavefile,(char*)write1,length1);
	if(write2>0)
		mmioRead(wavefile,(char*)write2,length2);
	buffer->Unlock(write1,length1,write2,length2);

	//close the wavefile, don't need it anymore, it's in the directsound buffer now
	mmioClose(wavefile,0);

	buffer->QueryInterface(IID_IDirectSound3DBuffer, (LPVOID*)&buffer3d);
	buffer3d->GetPosition(&position);
}

void MySound::Play()
{
	buffer->SetCurrentPosition(0);
	if (loop)
		buffer->Play(0, 0, DSBPLAY_LOOPING);
	else 
		buffer->Play(0, 0, 0);
}

void MySound::Stop()
{
	buffer->Stop();
}

void MySound::Rewind()
{
	buffer->SetCurrentPosition(0);
}

void MySound::SetVolume(LONG val)
{
	HRESULT hr = buffer->SetVolume(val);
}

void MySound::setFrequency(LONG freq)
{
	HRESULT hr = buffer->SetFrequency(freq);
}

void MySound::setPosition(D3DXVECTOR3 pos)
{
	position.x = pos.x;
	position.y = pos.y;
	position.z = pos.z;
	buffer3d->SetPosition(pos.x, pos.y, pos.z, DS3D_IMMEDIATE);
}

///////////////////////////////////////////////////
///////////////////////////////////////////////////

MyListener::MyListener()
{
	Device = 0;
	Listener = 0;
}

void MyListener::Init(LPDIRECTSOUND8 d)
{
	Device = d;
	HRESULT hr;
	Listener = NULL;
	DSBUFFERDESC dsbd;
	LPDIRECTSOUNDBUFFER primary;
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER;
	if (SUCCEEDED(hr = Device->CreateSoundBuffer(&dsbd, &primary, NULL)))
	{
		hr = primary->QueryInterface(IID_IDirectSound3DListener8, (LPVOID*)&Listener);
		primary->Release();
	}

}

MyListener::~MyListener()
{

}

void MyListener::setPosition(D3DXVECTOR3 pos)
{
	position.x = pos.x;
	position.y = pos.y;
	position.z = pos.z;

	Listener->SetPosition(pos.x, pos.y, pos.z, DS3D_IMMEDIATE);
}
