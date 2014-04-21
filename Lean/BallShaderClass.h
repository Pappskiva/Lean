////////////////////////////////////////////////////////////////////////////////
// Filename: ballshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef BALLSHADERCLASS_H
#define BALLSHADERCLASS_H

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include "..\\Include\\d3dx10math.h"
#include "..\\Include\\d3dx11async.h"
#include <fstream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Class name: BallShaderClass
////////////////////////////////////////////////////////////////////////////////
class BallShaderClass
{
private:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	struct PixelBufferType //Relik kod, men l�mnad f�r senare behov
	{
		float textureOffsetX;
		float textureOffsetY;
		float padding1;
		float padding2;
	};

public:
	BallShaderClass();
	BallShaderClass(const BallShaderClass&);
	~BallShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;

	ID3D11Buffer* m_pixelBuffer;
};

#endif
