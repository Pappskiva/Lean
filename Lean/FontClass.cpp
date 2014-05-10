#include "FontClass.h"

FontClass::FontClass()
{
	mFont = 0;
	mTexture = 0;
}

FontClass::~FontClass()
{

}

bool FontClass::Initialize(D3D* d3d, char* fontFileName, WCHAR* textureFileName)
{
	bool result;

	// Ladda in font datan
	result = LoadFontData(fontFileName);
	if (!result)
	{
		return false;
	}

	// Skapa textur objekt
	mTexture = d3d->LoadTextureFromFile(textureFileName);
	if (!mTexture)
	{
		return false;
	}

	return true;
}

void FontClass::Shutdown()
{
	if (mTexture)
	{
		mTexture->Shutdown();
		mTexture = 0;
	}

	if (mFont)
	{
		delete[] mFont;
		mFont = 0;
	}
}

Texture* FontClass::GetTexture()
{
	return this->mTexture;
}

bool FontClass::LoadFontData(char* fileName)
{
	// Skapa font buffer med 95 platser (har bara 95 ASCII tecken i fonten)
	mFont = new Font[95];
	if (!mFont)
	{
		return false;
	}

	// Läs in datan
	ifstream f;
	f.open(fileName);
	if (f.fail())
	{
		return false;
	}

	char temp;
	for (int i = 0; i < 95; i++)
	{
		f.get(temp);
		while (temp != ' ')
		{
			f.get(temp);
		}
		f.get(temp);
		while (temp != ' ')
		{
			f.get(temp);
		}

		f >> mFont[i].left;
		f >> mFont[i].right;
		f >> mFont[i].top;
		f >> mFont[i].size;
	}

	f.close();

	return true;
}

void FontClass::BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY)
{
	// Tvinga void pekaren till en Vertex struct pekare
	Vertex* vertexPtr = (Vertex*)vertices;

	int numLetters = (int)strlen(sentence);
	int index = 0;
	int letter;

	// Rita varje tecken till en quad
	for (int i = 0; i < numLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;

		// Om tecknet är ett mellanrum, flytta ett tecken fram
		if (letter == 0)
		{
			drawX = drawX + mFont[letter].size;
		}
		else
		{
			// Triangel 1
			vertexPtr[index].pos = v3(drawX, drawY, 0.0f);  //top left
			vertexPtr[index].tex = v2(mFont[letter].left, mFont[letter].top);
			index++;

			vertexPtr[index].pos = v3((drawX + mFont[letter].size), (drawY - mFont->size), 0.0f);  //bottom right
			vertexPtr[index].tex = v2(mFont[letter].right, mFont[letter].top + 0.1f);
			index++;

			vertexPtr[index].pos = v3(drawX, (drawY - mFont->size), 0.0f);  //bottom left
			vertexPtr[index].tex = v2(mFont[letter].left, mFont[letter].top + 0.1f);
			index++;

			// Triangel 2
			vertexPtr[index].pos = v3(drawX, drawY, 0.0f);  //top left
			vertexPtr[index].tex = v2(mFont[letter].left, mFont[letter].top);
			index++;

			vertexPtr[index].pos = v3(drawX + mFont[letter].size, drawY, 0.0f);  //top right
			vertexPtr[index].tex = v2(mFont[letter].right, mFont[letter].top);
			index++;

			vertexPtr[index].pos = v3((drawX + mFont[letter].size), (drawY - mFont->size), 0.0f);  //bottom right
			vertexPtr[index].tex = v2(mFont[letter].right, mFont[letter].top + 0.1f);
			index++;

			// Flytta x-positionen ett tecken år höger
			drawX = drawX + mFont[letter].size;
		}
	}
}