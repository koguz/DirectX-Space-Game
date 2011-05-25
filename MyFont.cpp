#include <sstream>
#include "MyFont.h"


MyFont::MyFont()
{
	Device = 0;
	font = 0;
}

MyFont::~MyFont()
{
	font->Release();
}

MyFont::MyFont(IDirect3DDevice9 *d, std::string t)
{
	Device = d;
	text = t;
	AddFontResource("font.ttf");
	
	HRESULT hr = D3DXCreateFont(
		Device, 24, 0, FW_NORMAL, 0, FALSE, 
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, DEFAULT_PITCH, "CloneWars", &font);

	if (FAILED(hr))
	{
		font->Release();
		font = 0;
	}
}

void MyFont::SetPosition(int l, int r, int t, int b)
{
	SetRect(&rect, l, r, t, b);
}

void MyFont::Draw()
{
	if (!font)
		return;
	font->DrawText(NULL, text.c_str(), (int) text.length(), &rect, DT_LEFT,  0xFFFFFFFF);
}

void MyFont::SetText(std::string t)
{
	text = t;
}

void MyFont::AddText(std::string t)
{
	text = text + t;
}

void MyFont::AddText(float f)
{
	std::stringstream temp;
	temp << f;
	text = text + temp.str();
}


