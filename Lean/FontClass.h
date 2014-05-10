#ifndef FONTCLASS_H
#define FONTCLASS_H

#include <fstream>

#include "D3D.h"
#include "Texture.h"

using namespace std;

// Hanterar en font i projektet
class FontClass
{
public:
	FontClass();
	~FontClass();

	bool Initialize(D3D* d3d, char* fontFileName, WCHAR* textureFileName);
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

	Font* mFont;
	Texture* mTexture;

	bool LoadFontData(char* fileName);

};

#endif