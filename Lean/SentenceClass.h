#ifndef SENTENCECLASS_H
#define SENTENCECLASS_H

#include "D3D.h"
#include "Shader.h"
#include "FontClass.h"

// Hanterar en rad text som kan renderas på skärmen
class SentenceClass
{
public:
	SentenceClass();
	~SentenceClass();

	bool Initialize(char* fontInfoFileName, WCHAR* fontTextureFileName, int sentenceMaxLength,
		D3D* d3d, int screenW, int screenH, m4 baseViewMatrix);
	void Shutdown();
	void Render(D3D* d3d);

	bool SetText(char* text, D3D* d3d);
	void SetPosition(int posX, int posY);
	void SetColor(float r, float g, float b);

private:

	// Varje mening har en vertexbuffer, indexbuffer och RGB färg
	struct Sentence
	{
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		int vertexCount, indexCount, maxLength;
		float red, green, blue;
	};

	struct Vertex
	{
		v3 pos;
		v2 tex;
	};

	Sentence*	mSentence;
	FontClass*	mFont;
	Shader*		mFontShader;
	int			mScreenWidth, mScreenHeight, mPosX, mPosY;
	m4			mBaseViewMatrix;

	bool InitializeSentence(int maxLength, D3D* d3d);

};

#endif