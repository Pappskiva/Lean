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

	// Skapar en quad f�r varje tecken
	void BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY);

	Texture* GetTexture();

private:

	// left: 	U koordinaten f�r v�nster om tecknet
	// right:	U koordinaten f�r h�ger om tecknet
	// top:		V koordinaten f�r toppen av tecknet
	// size:	storleken p� tecknet i pixlar p� sk�rmen
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