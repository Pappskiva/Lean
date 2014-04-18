#pragma once
#ifndef BALL_H
#define BALL_H

#include <d3d11.h>
#include "..\\Include\\d3dx10math.h"

#include "TextureClass.h"

class Ball
{
protected:

	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

public:
	Ball();
	Ball(const Ball&);
	~Ball();

	bool Initialize(ID3D11Device*, WCHAR*);
	void Shutdown();
	void Update(float);
	void	Render(ID3D11DeviceContext*);
	int		GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();

	float GetSizeX();
	float GetSizeY();
	void SetSizeX(float);
	void SetSizeY(float);

	void SetPosition(D3DXVECTOR3 position);
	D3DXVECTOR3 GetPosition();

	void UpdateWorldMatrix();
	void GetWorldMatrix(D3DXMATRIX& worldMatrix);

protected:
	bool InitializeBuffers(ID3D11Device*);
	void	ShutdownBuffers();
	void	RenderBuffers(ID3D11DeviceContext*);

	bool	LoadTexture(ID3D11Device*, WCHAR*);
	void	ReleaseTexture();

protected:
	ID3D11Buffer	*m_vertexBuffer, *m_indexBuffer;
	int				m_vertexCount, m_indexCount;

	TextureClass*	m_Texture;

	float sizeX, sizeY;
	float rotationDegrees;

	D3DXVECTOR3 position;
	D3DXMATRIX worldMatrix;

};
#endif