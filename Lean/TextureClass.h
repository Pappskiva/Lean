////////////////////////////////////////////////////////////////////////////////
// Filename: TextureClass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef TEXTURECLASS_H
#define TEXTURECLASS_H

#include <D3D11.h>
#include "..\\Include\\D3DX11tex.h"

class TextureClass
{
public:
	TextureClass();
	TextureClass(const TextureClass&);
	~TextureClass();

	bool Initialize(ID3D11Device*, WCHAR*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

private:

	ID3D11ShaderResourceView* m_texture;

};
#endif