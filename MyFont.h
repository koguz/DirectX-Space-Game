#ifndef MYFONT__H__
#define MYFONT__H__

#include <d3dx9.h>
#include <string>

class MyFont
{
public:
	MyFont();
	MyFont(IDirect3DDevice9* d, std::string t);
	~MyFont();
	void Draw();
	void SetPosition(int left, int right, int top, int bottom);
	void SetText(std::string t);
	void AddText(std::string t);
	void AddText(float f);
	
private:
	
	IDirect3DDevice9* Device;
	ID3DXFont* font;
	std::string text;
	RECT rect;
};


#endif
