#include "Texture.h"

Texture::Texture()
{
	m_texture = 0;
}

Texture::Texture(const Texture& other)
{

}

Texture::~Texture()
{

}

void Texture::Shutdown()
{
	if (m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}
}

ID3D11ShaderResourceView* Texture::GetTexture()
{
	return m_texture;
}