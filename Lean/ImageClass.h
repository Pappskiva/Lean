#ifndef IMAGECLASS_H
#define IMAGECLASS_H

#include "D3D.h"
#include "Texture.h"
#include "Shader.h"

// Hanterar rendering av 2D bilder på skärmen
class ImageClass
{
private:
	struct Vertex
	{
		v3 pos;
		v2 tex;
	};

	Texture*		mTexture;
	Shader*			mShader;
	ID3D11Buffer*	mVertexBuffer;
	ID3D11Buffer*	mIndexBuffer;

	int mVertexCount, mIndexCount;
	int mScreenWidth, mScreenHeight;
	int mImageWidth, mImageHeight;
	int mPosX, mPosY;

	bool InitializeBuffers(D3D* d3d);
	bool UpdateBuffers(D3D* d3d, int posX, int posY);

public:
	ImageClass();
	~ImageClass();

	bool Initialize(D3D* d3d, WCHAR* textureFileName, int screenW, int screenH, int imageW, int imageH);
	void Shutdown();
	bool Render(D3D* d3d);

	Texture* GetTexture();
	void SetPosition(int posX, int posY);

};

#endif