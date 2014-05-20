#ifndef FONTCLASS_H
#define FONTCLASS_H

#include <fstream>

#include "D3D.h"
#include "Texture.h"

using namespace std;

enum Alignment
{
	ALIGNMENT_LEFT,
	ALIGNMENT_CENTER,
	ALIGNMENT_RIGHT,
	ALIGNMENT_VERTICAL
};

// Hanterar en font i projektet
class FontClass
{
public:
	FontClass();
	~FontClass();

	bool Initialize(D3D* d3d, char* fontFileName, WCHAR* textureFileName, float letterScale, Alignment alignment);
	void Shutdown();

	// Skapar en quad för varje tecken
	void BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY);

	Texture* GetTexture();

private:

	// left: 	U koordinaten för vänster om tecknet
	// right:	U koordinaten för höger om tecknet
	// top:		V koordinaten för toppen av tecknet
	// size:	storleken på tecknet i pixlar på skärmen
	struct Font
	{
		float left, right, top;
		int size;
	};

	struct Vertex
	{
		v3 pos;
		v2 tex;
	};

	Font*		mFont;
	Texture*	mTexture;
	Alignment	mAlignment;

	bool LoadFontData(char* fileName, float letterScale);

};

#endif