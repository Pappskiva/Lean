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

	bool Initialize(D3D* d3d, const char* alignment, float letterScale, int sentenceMaxLength, int screenWidth, int screenHeight);
	void Shutdown();
	void Render(D3D* d3d);

	bool SetText(char* text, D3D* d3d);
	void SetPosition(int posX, int posY);
	void SetColor(float r, float g, float b);

	float GetLetterScale() const;

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
	Alignment	mAlignment;
	int			mScreenWidth, mScreenHeight;
	int			mPosX, mPosY;
	int			mSentenceLength;
	float		mLetterScale;
	m4			mBaseViewMatrix;

	bool InitializeSentence(int maxLength, D3D* d3d);

};

#endif