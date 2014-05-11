////////////////////////////////////////////////////////////////////////////////
// Filename: d3d.h
////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef D3D_H
#define D3D_H

#include "D3D Includes.h"
#include "DuckRenderer\DMath.h"
#include "DuckRenderer\Array.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "InstanceRenderer.h"

class D3D
{
public:
	D3D();
	D3D(const D3D&);
	~D3D();

	bool Initialize(int, int, bool, HWND, bool, float, float);
	void Shutdown();

	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(m4&);
	void GetWorldMatrix(m4&);
	void GetOrthoMatrix(m4&);

	void TurnZBufferOn();
	void TurnZBufferOff();

	void TurnZBufferReadOnWriteOff();

	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();

	void				BeginDeferredRenderingScene(float clearColor[]);
	void				EndDeferredRenderingScene();

	void				BeginLightStage();
	void				EndLightStage();

	void				BeginShadowPass();
	void				EndShadowPass();

	bool				CreateShadowmap(const uint width, const uint height);

	Texture*			LoadTextureFromFile(const HString &filePath);
	Mesh*				CreateMeshFromRam(void *vertexData, const uint vertexSize, const uint nrVertices, uint *indices = nullptr, const uint nrIndices = 0);
	Mesh*				LoadMeshFromOBJ(const std::string filePath);
	bool				LoadMeshIntoDevice(Mesh *mesh);
	Shader*				LoadVertexShader(const ShaderInfo &shaderInfo, const D3D11_INPUT_ELEMENT_DESC elemDesc[] = nullptr, const uint nrElem = 0);
	bool				LoadShaderStageIntoShader(const ShaderInfo &shaderInfo, Shader *shader, const uint shaderType);

	void				SetShader(const Shader *shader);
	void				RenderMesh(const Mesh *mesh, const int subset = -1) const;
	void				RenderFullScreenQuad() const;

	//it sets the shadowmapSRView to the current shader
	void				ApplyShadowmapResource(const uint slot);
	//it sets the m_depthStencilShaderResourceView to the current shader
	void				ApplyDepthResource(const uint slot);
	//it sets any of the gBufferShaderResourceView[ index ] to the current shader
	void				ApplyGBufferResource(const uint index, const uint slot);
	void				ApplyTexture(const Texture *texture, const uint slot);
	void				ApplyConstantBuffers();

	Shader*				GetCurrentShader();
	Shader*				GetShadowFill();
	
	bool				PrepareInstanceRenderer(InstanceRenderer &instanceRenderer);
	bool				UpdateInstanceRenderer(InstanceRenderer &instanceRenderer);
	bool				RenderInstancerenderer(InstanceRenderer &instanceRenderer, const uint instanceOffset, const uint nrInstances);
	//void				ReloadMesh(const Mesh *mesh);


private:
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11ShaderResourceView* m_depthStencilShaderResourceView;
	ID3D11RasterizerState* m_rasterState;
	
	Array<Mesh> meshes;
	Array<Shader> shaders;
	Array<Texture> textures;
	
	Mesh *fullscreenQuadMesh;
	Shader *currentShader;

	bool m_vsync_enabled;

	m4 m_projectionMatrix;
	m4 m_worldMatrix;
	m4 m_orthoMatrix;

	ID3D11DepthStencilState* m_depthDisabledStencilState;
	ID3D11DepthStencilState* m_depthReadOnWriteOff;

	ID3D11BlendState* m_alphaEnableBlendingState;
	ID3D11BlendState* m_alphaDisableBlendingState;

	//shadow stuff
	bool						shadowPass;
	Shader						*shadowFillShader;
	ID3D11DepthStencilView		*shadowmapDSView;
	ID3D11ShaderResourceView	*shadowmapSRView;

	//deferred
	ID3D11RenderTargetView		*gBufferRenderTargetView[2], *lightRenderTargetView;
	ID3D11ShaderResourceView	*gBufferShaderResourceView[2], *lightShaderResourceView;
	ID3D11BlendState			*lightBlending;

	
	Shader *finalCompositionShader;

	void _ShaderReflection(Shader *shader, ID3D10Blob* shaderBlob, const uint shaderType);
};

inline Shader* D3D::GetCurrentShader()
{
	return currentShader;
}

inline Shader* D3D::GetShadowFill()
{
	return shadowFillShader;
}

#endif

