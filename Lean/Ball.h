#pragma once
#ifndef BALL_H
#define BALL_H

#include <d3d11.h>
#include "..\\Include\\d3dx10math.h"
#include <fstream>

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
	void Render(ID3D11DeviceContext*);
	int	 GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();

	void SetPosition(float, float, float);
	void GetPosition(D3DXVECTOR3&);

	void SetRotation(float, float, float);
	void GetRotation(D3DXVECTOR3&);

	void SetScale(float);
	float GetScale();

	void SetRadius(float);
	float GetRadius();

	void UpdateWorldMatrix();
	void GetWorldMatrix(D3DXMATRIX&);

private:
	bool InitializeBuffers(ID3D11Device*);
	void	LoadSphere(VertexType*, unsigned long*);
	void	ShutdownBuffers();
	void	RenderBuffers(ID3D11DeviceContext*);

	bool	LoadTexture(ID3D11Device*, WCHAR*);
	void	ReleaseTexture();

private:
	ID3D11Buffer	*m_vertexBuffer, *m_indexBuffer;
	int				m_vertexCount, m_indexCount;

	TextureClass*	m_Texture;

	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	float m_scale;
	float m_radius;

	D3DXMATRIX worldMatrix;

};
#endif