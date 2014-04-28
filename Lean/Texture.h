////////////////////////////////////////////////////////////////////////////////
// Filename: Texture.h
////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <D3D11.h>

class Texture
{
public:
	Texture();
	Texture(const Texture&);
	~Texture();

	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

private:

	ID3D11ShaderResourceView* m_texture;

	friend class D3D;
};
#endif