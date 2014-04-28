////////////////////////////////////////////////////////////////////////////////
// Filename: d3d.cpp
////////////////////////////////////////////////////////////////////////////////
#include "d3d.h"
#include <assert.h>

#define			WBOX(x) MessageBox(NULL, x, L"D3D Error!!", MB_OK | MB_ICONASTERISK);
#define			SAFE_RELEASE(x)			{ if(x){x->Release(); x = nullptr;}}

D3D::D3D()
{
	m_swapChain = 0;
	m_device = 0;
	m_deviceContext = 0;
	m_renderTargetView = 0;
	m_depthStencilBuffer = 0;
	m_depthStencilState = 0;
	m_depthStencilView = 0;
	m_rasterState = 0;
	m_depthDisabledStencilState = 0;
	m_alphaEnableBlendingState = 0;
	m_alphaDisableBlendingState = 0;
}


D3D::D3D(const D3D& other)
{
}


D3D::~D3D()
{
}


bool D3D::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen,
	float screenDepth, float screenNear)
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator, denominator;
	DXGI_MODE_DESC* displayModeList;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspect;
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	D3D11_BLEND_DESC blendStateDescription;


	// Store the vsync setting.
	m_vsync_enabled = vsync;

	// Create a DirectX graphics interface factory.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		return false;
	}

	// Use the factory to create an adapter for the primary graphics interface (video card).
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return false;
	}

	// Enumerate the primary adapter output (monitor).
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		return false;
	}

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		return false;
	}
	
	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	// Now fill the display mode list structures.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for (i = 0; i<numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)screenWidth)
		{
			if (displayModeList[i].Height == (unsigned int)screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// Release the display mode list.
	delete[] displayModeList;
	displayModeList = 0;

	// Release the adapter output.
	adapterOutput->Release();
	adapterOutput = 0;

	// Release the adapter.
	adapter->Release();
	adapter = 0;

	// Release the factory.
	factory->Release();
	factory = 0;

	// Initialize the swap chain description.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set to a single back buffer.
	swapChainDesc.BufferCount = 1;

	// Set the width and height of the back buffer.
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	// Set regular 32-bit surface for the back buffer.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Set the refresh rate of the back buffer.
	if (m_vsync_enabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
	swapChainDesc.OutputWindow = hwnd;

	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	if (fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;

	// Set the feature level to DirectX 11.
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};

	UINT m_deviceFlags = 0;
#ifdef _DEBUG
	m_deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	// Create the swap chain, Direct3D device, and Direct3D device context.
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, m_deviceFlags, featureLevels,
		ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);
	if (FAILED(result))
	{
		return false;
	}

	// Get the pointer to the back buffer.
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
	{
		return false;
	}

	// Create the render target view with the back buffer pointer.
	result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	// Release pointer to the back buffer as we no longer need it.
	backBufferPtr->Release();
	backBufferPtr = 0;

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(result))
	{
		return false;
	}

	// Set the depth stencil state.
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	// Create the depth stencil resource view.
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	shaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;

	result = m_device->CreateShaderResourceView(m_depthStencilBuffer, &shaderResourceViewDesc, &m_depthStencilShaderResourceView);


	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if (FAILED(result))
	{
		return false;
	}

	// Now set the rasterizer state.
	m_deviceContext->RSSetState(m_rasterState);

	// Setup the viewport for rendering.
	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.
	m_deviceContext->RSSetViewports(1, &viewport);

	// Setup the projection matrix.
	fieldOfView = (float)PI * 0.25f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	// Create the projection matrix for 3D rendering.
	m_projectionMatrix.Projection(fieldOfView, screenNear, screenDepth, screenAspect);

	// Initialize the world matrix to the identity matrix.
	m_worldMatrix = m4::IDENTITY;

	// Create an orthographic projection matrix for 2D rendering.
	m_orthoMatrix = m4::CreateOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	// Clear the second depth stencil state before setting the parameters.
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
	// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the state using the device.
	result = m_device->CreateDepthStencilState(&depthDisabledStencilDesc, &m_depthDisabledStencilState);
	if (FAILED(result))
	{
		return false;
	}

	// Clear the blend state description.
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create the blend state using the description.
	result = m_device->CreateBlendState(&blendStateDescription, &m_alphaEnableBlendingState);
	if (FAILED(result))
	{
		return false;
	}

	// Modify the description to create an alpha disabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;

	// Create the blend state using the description.
	result = m_device->CreateBlendState(&blendStateDescription, &m_alphaDisableBlendingState);
	if (FAILED(result))
	{
		return false;
	}

	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	/////////////////////////////////////////Deferred Renderering

	ID3D11Texture2D *rtDiffuse, *rtNormal, *rtLight;
	D3D11_TEXTURE2D_DESC texture2DDesc;
	ZeroMemory(&texture2DDesc, sizeof(D3D11_TEXTURE2D_DESC));
	texture2DDesc.Width = screenWidth;
	texture2DDesc.Height = screenHeight;
	texture2DDesc.MipLevels = 1;
	texture2DDesc.ArraySize = 1;
	texture2DDesc.SampleDesc.Count = 1;
	texture2DDesc.SampleDesc.Quality = 0;
	texture2DDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texture2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texture2DDesc.Usage = D3D11_USAGE_DEFAULT;
	texture2DDesc.CPUAccessFlags = 0;

	result = m_device->CreateTexture2D(&texture2DDesc, nullptr, &rtDiffuse);

	if (result != S_OK)
		return false;

	result = m_device->CreateTexture2D(&texture2DDesc, nullptr, &rtNormal);

	if (result != S_OK)
		return false;

	result = m_device->CreateTexture2D(&texture2DDesc, nullptr, &rtLight);

	if (result != S_OK)
		return false;

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetDesc;
	ZeroMemory(&renderTargetDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	renderTargetDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	renderTargetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	result = m_device->CreateRenderTargetView(rtDiffuse, &renderTargetDesc, &gBufferRenderTargetView[0]);

	if (result != S_OK)
		return false;

	result = m_device->CreateRenderTargetView(rtNormal, &renderTargetDesc, &gBufferRenderTargetView[1]);

	if (result != S_OK)
		return false;

	result = m_device->CreateRenderTargetView(rtLight, &renderTargetDesc, &lightRenderTargetView);

	if (result != S_OK)
		return false;

	ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	shaderResourceViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//DXGI_FORMAT_R8G8B8A8_UNORM  DXGI_FORMAT_R32_FLOAT;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;

	result = m_device->CreateShaderResourceView(rtDiffuse, &shaderResourceViewDesc, &gBufferShaderResourceView[0]);
	rtDiffuse->Release();

	if (result != S_OK)
		return false;

	result = m_device->CreateShaderResourceView(rtNormal, &shaderResourceViewDesc, &gBufferShaderResourceView[1]);
	rtNormal->Release();

	if (result != S_OK)
		return false;

	result = m_device->CreateShaderResourceView(rtLight, &shaderResourceViewDesc, &lightShaderResourceView);
	rtLight->Release();

	struct QuadVertex
	{
		float x, y, z;
		float u, v;
	}tempVB[] =
	{
		{ -1.0f, -1.0f, 1.0f, 0.0f, 1.0f },
		{ -1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
		{ 1.0f, -1.0f, 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
	};

	uint indices[6];
	indices[0] = 1;
	indices[1] = 3;
	indices[2] = 2;
	indices[3] = 1;
	indices[4] = 2;
	indices[5] = 0;

	fullscreenQuadMesh = CreateMeshFromRam(tempVB, sizeof(QuadVertex), 4, indices, 6);


	ShaderInfo vsInfo, psInfo;
	vsInfo.entryPoint = "VS";
	vsInfo.path = L"fullScreenQuad.vs";
	vsInfo.version = "vs_4_0";

	psInfo.entryPoint = "PS";
	psInfo.path = L"finalComposition.ps";
	psInfo.version = "ps_4_0";

	D3D11_INPUT_ELEMENT_DESC elemDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	finalCompositionShader = (Shader *)LoadVertexShader(vsInfo, elemDesc, 2);

	if (!finalCompositionShader)
	{
		WBOX(L"Couldn't Load Shader!");
		return false;
	}

	if (!LoadShaderStageIntoShader(psInfo, finalCompositionShader, SVF_PIXELSHADER))
		return false;


	D3D11_BLEND_DESC blendDesc;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.AlphaToCoverageEnable = FALSE;

	m_device->CreateBlendState(&blendDesc, &lightBlending);

	return true;
}


void D3D::Shutdown()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if (m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, NULL);
	}

	for (uint i = 0; i < meshes.Length(); ++i)
		meshes[i].Flush();

	for (uint i = 0; i < shaders.Length(); ++i)
		shaders[i].Flush();

	for (uint i = 0; i < textures.Length(); ++i)
		textures[i].Shutdown();

	SAFE_RELEASE(m_alphaEnableBlendingState);
	SAFE_RELEASE(m_alphaDisableBlendingState);
	SAFE_RELEASE(m_rasterState);
	SAFE_RELEASE(m_depthStencilView);
	SAFE_RELEASE(m_depthDisabledStencilState);
	SAFE_RELEASE(m_depthStencilState);
	SAFE_RELEASE(m_depthStencilBuffer);
	SAFE_RELEASE(m_renderTargetView);
	SAFE_RELEASE(m_swapChain);
	SAFE_RELEASE(m_deviceContext);
	SAFE_RELEASE(m_device);	

	return;
}


void D3D::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4];


	// Setup the color to clear the buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the back buffer.
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);

	// Clear the depth buffer.
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}


void D3D::EndScene()
{
	// Present the back buffer to the screen since rendering is complete.
	if (m_vsync_enabled)
	{
		// Lock to screen refresh rate.
		m_swapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		m_swapChain->Present(0, 0);
	}

	return;
}


ID3D11Device* D3D::GetDevice()
{
	return m_device;
}


ID3D11DeviceContext* D3D::GetDeviceContext()
{
	return m_deviceContext;
}


void D3D::GetProjectionMatrix(m4& projectionMatrix)
{
	projectionMatrix = m_projectionMatrix;
	return;
}


void D3D::GetWorldMatrix(m4& worldMatrix)
{
	worldMatrix = m_worldMatrix;
	return;
}


void D3D::GetOrthoMatrix(m4& orthoMatrix)
{
	orthoMatrix = m_orthoMatrix;
	return;
}


void D3D::TurnZBufferOn()
{
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
	return;
}


void D3D::TurnZBufferOff()
{
	m_deviceContext->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
	return;
}


void D3D::TurnOnAlphaBlending()
{
	float blendFactor[4];


	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn on the alpha blending.
	m_deviceContext->OMSetBlendState(m_alphaEnableBlendingState, blendFactor, 0xffffffff);

	return;
}


void D3D::TurnOffAlphaBlending()
{
	float blendFactor[4];


	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn off the alpha blending.
	m_deviceContext->OMSetBlendState(m_alphaDisableBlendingState, blendFactor, 0xffffffff);

	return;
}

void			D3D::BeginDeferredRenderingScene(float clearColor[])
{
	m_deviceContext->ClearRenderTargetView(gBufferRenderTargetView[0], clearColor);
	m_deviceContext->ClearRenderTargetView(gBufferRenderTargetView[1], clearColor);

	static float lightClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_deviceContext->ClearRenderTargetView(lightRenderTargetView, lightClearColor);

	m_deviceContext->OMSetRenderTargets(2, gBufferRenderTargetView, m_depthStencilView);
}

void			D3D::EndDeferredRenderingScene()
{
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
	SetShader(finalCompositionShader);

	static ID3D11ShaderResourceView *tempPointers[2] {gBufferShaderResourceView[0], lightShaderResourceView};
	m_deviceContext->PSSetShaderResources(0, 2, tempPointers);

	RenderMesh(fullscreenQuadMesh);

	m_deviceContext->OMSetDepthStencilState(nullptr, 0);

	static ID3D11ShaderResourceView *const pSRV[8] = { nullptr, nullptr, nullptr, nullptr
		, nullptr, nullptr, nullptr, nullptr };

	m_deviceContext->PSSetShaderResources(0, 8, pSRV);
}

void			D3D::BeginLightStage()
{
	m_deviceContext->OMSetRenderTargets(1, &lightRenderTargetView, nullptr);
	m_deviceContext->OMSetBlendState(lightBlending, nullptr, 0xffffffff);
	m_deviceContext->PSSetShaderResources(0, 1, &gBufferShaderResourceView[1]);
	m_deviceContext->PSSetShaderResources(1, 1, &m_depthStencilShaderResourceView);

	TurnZBufferOff();
}

void			D3D::EndLightStage()
{
	TurnOffAlphaBlending();

	static ID3D11ShaderResourceView *const pSRV[8] = { nullptr, nullptr, nullptr, nullptr
		, nullptr, nullptr, nullptr, nullptr };

	m_deviceContext->PSSetShaderResources(0, 8, pSRV);
}

Texture*		D3D::LoadTextureFromFile(const HString &filePath)
{
	Texture &texture = textures.Append();

	HRESULT result;

	if (filePath.Find(".dds", filePath.Length() - 4) != -1)
		result = DirectX::CreateDDSTextureFromFile(m_device,
		filePath.AsWChar(), 
		nullptr,
		&texture.m_texture);
	else
		result = DirectX::CreateWICTextureFromFile(m_device,
		m_deviceContext,
		filePath.AsWChar(), 
		nullptr, 
		&texture.m_texture);

	if (FAILED(result))
	{
		WBOX(L"Invalid Texture Path!");
		textures.RemoveLast();

		return nullptr;
	}


	return &texture;
}

Mesh*			D3D::CreateMeshFromRam(void *vertexData, const uint vertexSize, const uint nrVertices, uint *indices, const uint nrIndices)
{
	Mesh &mesh = meshes.Append();
	mesh.vertices = new uchar[vertexSize * nrVertices];
	memcpy(mesh.vertices, vertexData, vertexSize * nrVertices);
	mesh.nrVertices = nrVertices;
	mesh.vertexSize = vertexSize;

	if (indices)
	{
		mesh.indices = new uint[sizeof(uint)* nrIndices];
		memcpy(mesh.indices, indices, sizeof(uint)* nrIndices);
		mesh.nrIndices = nrIndices;
	}

	mesh.nrSubsets = 1;
	mesh.subsets = new MeshSubset[mesh.nrSubsets];
	mesh.subsets[0].startVertex = 0;
	mesh.subsets[0].length = nrVertices;

	if (!LoadMeshIntoDevice(&mesh))
	{
		meshes.RemoveLast();
		return nullptr;
	}

	return &mesh;
}

Mesh*			D3D::LoadMeshFromOBJ(const std::string filePath)
{
	Mesh &mesh = meshes.Append();

	if (!mesh.LoadMeshFromObjToRam(filePath))
	{
		meshes.RemoveLast();
		return nullptr;
	}

	if (!LoadMeshIntoDevice(&mesh))
		return nullptr;

	return &mesh;
}

bool			D3D::LoadMeshIntoDevice(Mesh *mesh)
{
	if (mesh->gpuHasCopy)
		return true;

	HRESULT result;

	D3D11_SUBRESOURCE_DATA initData;
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = mesh->vertexSize * mesh->nrVertices;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	initData.pSysMem = mesh->vertices;

	result = m_device->CreateBuffer(&bufferDesc, &initData, &mesh->vertexBuffer);
	if (result != S_OK)
		return false;

	if (mesh->indices)
	{
		bufferDesc.ByteWidth = sizeof(uint)* mesh->nrIndices;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		initData.pSysMem = mesh->indices;

		result = m_device->CreateBuffer(&bufferDesc, &initData, &mesh->indexBuffer);

		if (result != S_OK)
		{
			mesh->vertexBuffer->Release();
			return false;
		}
	}

	mesh->gpuHasCopy = true;

	return true;
}

Shader*			D3D::LoadVertexShader(const ShaderInfo &shaderInfo, const D3D11_INPUT_ELEMENT_DESC elemDesc[], const uint nrElem)
{
	Shader *shader = &shaders.Append();
	HRESULT result;
	ID3D10Blob *shaderBlob = nullptr, *errorBlob = nullptr;

	result = D3DCompileFromFile(shaderInfo.path.AsWChar(),
		nullptr,
		nullptr,
		shaderInfo.entryPoint.AsChar(),
		shaderInfo.version.AsChar(),
		D3D10_SHADER_PACK_MATRIX_ROW_MAJOR,
		0,
		&shaderBlob,
		&errorBlob);

	if (result != S_OK)
	{
		if (!errorBlob)
			return nullptr;

		char msg[20000];
		strcpy_s(msg, sizeof(msg), (char*)errorBlob->GetBufferPointer());
		OutputDebugStringA(msg);
		MessageBoxA(GetDesktopWindow(), msg, "VertexShader Compilation Failed!", MB_OK | MB_ICONERROR);
		errorBlob->Release();

		return nullptr;
	}

	result = m_device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &shader->vertexShader);

	if (result != S_OK)
	{
		SAFE_RELEASE(shaderBlob);

		return nullptr;
	}

	if (elemDesc)
	{
		result = m_device->CreateInputLayout(
			elemDesc,
			nrElem,
			shaderBlob->GetBufferPointer(),
			shaderBlob->GetBufferSize(),
			&shader->inputLayout);

		if (result != S_OK)
		{
			WBOX(L"Cannot create input layout.");

			SAFE_RELEASE(shaderBlob);
			SAFE_RELEASE(shader->vertexShader);

			return nullptr;
		}
	}

	if (shaderBlob)
		_ShaderReflection(shader, shaderBlob, SVF_VERTEXSHADER);

	SAFE_RELEASE(shaderBlob);

	return shader;
}

bool			D3D::LoadShaderStageIntoShader(const ShaderInfo &shaderInfo, Shader *shader, const uint shaderType)
{
	HRESULT result;
	ID3D10Blob *shaderBlob = nullptr, *errorBlob = nullptr;

	result = D3DCompileFromFile(shaderInfo.path.AsWChar(),
		nullptr,
		nullptr,
		shaderInfo.entryPoint.AsChar(),
		shaderInfo.version.AsChar(),
		D3D10_SHADER_PACK_MATRIX_ROW_MAJOR,
		0,
		&shaderBlob,
		&errorBlob);

	if (result != S_OK)
	{
		if (!errorBlob)
			return false;

		char msg[20000];
		strcpy_s(msg, sizeof(msg), (char*)errorBlob->GetBufferPointer());
		OutputDebugStringA(msg);
		MessageBoxA(GetDesktopWindow(), msg, "Shader Stage Compilation Failed!", MB_OK | MB_ICONERROR);
		errorBlob->Release();

		return false;
	}

	switch (shaderType)
	{
	case SVF_VERTEXSHADER:
		result = m_device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &shader->vertexShader);
		break;
	case SVF_PIXELSHADER:
		result = m_device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &shader->pixelShader);
	}

	if (result != S_OK)
	{
		SAFE_RELEASE(shaderBlob);

		return false;
	}


	if (shaderBlob)
		_ShaderReflection(shader, shaderBlob, shaderType);


	SAFE_RELEASE(shaderBlob);


	return true;
}

void			D3D::_ShaderReflection(Shader *shader, ID3D10Blob *shaderBlob, const uint shaderType)
{
	ID3D11ShaderReflection* reflection = nullptr;
	HRESULT result = D3DReflect(shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		IID_ID3D11ShaderReflection,
		(void**)&reflection);

	D3D11_SHADER_DESC shaderDesc;
	reflection->GetDesc(&shaderDesc);

	uint oldConstantLength = shader->constantBuffers.Length();
	uint oldSamplerLength = shader->samplerStates.Length();
	uint oldTextureLength = shader->textureSlots.Length();
	for (uint resourceIndex = 0; resourceIndex < shaderDesc.BoundResources; ++resourceIndex)
	{
		D3D11_SHADER_INPUT_BIND_DESC inputBindDesc;
		reflection->GetResourceBindingDesc(resourceIndex, &inputBindDesc);

		switch (inputBindDesc.Type)
		{
#pragma region ConstantBuffers
		case D3D_SIT_CBUFFER:
		{
			bool alreadyCreated = false;
			for (uint c = 0; c < oldConstantLength; ++c)
			if (shader->constantBuffers[c].slotNumber == inputBindDesc.BindPoint)
			{
				shader->constantBuffers[c].flag |= shaderType;
				alreadyCreated = true;
				break;
			}

			if (alreadyCreated)
				break;

			ID3D11ShaderReflectionConstantBuffer *refCBuffer = reflection->GetConstantBufferByName(inputBindDesc.Name);
			D3D11_SHADER_BUFFER_DESC cBufferDesc;
			refCBuffer->GetDesc(&cBufferDesc);

			ConstantBuffer &tempCbuffer = shader->constantBuffers.Append();
			tempCbuffer.flag = 0;
			Array<ShaderVariable> tempMembers(8, false, false);

			for (uint vIndex = 0; vIndex < cBufferDesc.Variables; ++vIndex)
			{
				ShaderVariable &tempMember = tempMembers.Append();
				ID3D11ShaderReflectionVariable *variable = refCBuffer->GetVariableByIndex(vIndex);
				D3D11_SHADER_VARIABLE_DESC variableDesc;
				ID3D11ShaderReflectionType *variableType;
				D3D11_SHADER_TYPE_DESC typeDesc;
				variable->GetDesc(&variableDesc);
				variableType = variable->GetType();
				variableType->GetDesc(&typeDesc);

				tempMember.bufferPlace = variableDesc.StartOffset;
				tempMember.name = variableDesc.Name;
				tempMember.size = variableDesc.Size;
				tempMember.flag = 0;

				switch (typeDesc.Type)
				{
				case D3D_SVT_FLOAT:
				{
										tempMember.flag |= SVF_FLOAT;
				}break;
				case D3D_SVT_INT:
				{
									tempMember.flag |= SVF_INT;
				}break;
				case D3D_SVT_UINT:
				{
										tempMember.flag |= SVF_UINT;
				}break;
				};									
			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));

			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.ByteWidth = cBufferDesc.Size;
			bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			HRESULT result = m_device->CreateBuffer(&bd, nullptr, &tempCbuffer.gpuBuffer);

			if (result != S_OK)
			{
				WBOX(L"Couldn't Create Constant Buffer");
				return;
			}

			tempCbuffer.slotNumber = inputBindDesc.BindPoint;
			tempCbuffer.flag = shaderType;
			tempCbuffer.bufferSize = cBufferDesc.Size;
			tempCbuffer.nrMembers = tempMembers.Length();
			tempCbuffer.data = new byte[cBufferDesc.Size];
			tempCbuffer.members = new ShaderVariable[tempCbuffer.nrMembers];

			for (uint j = 0; j < tempCbuffer.nrMembers; ++j)
				tempCbuffer.members[j] = tempMembers[j];
		}break;
#pragma endregion ConstantBuffers
		case D3D_SIT_SAMPLER:
		{
			bool alreadyCreated = false;
			for (uint i = 0; i < oldSamplerLength; ++i)
			if (shader->samplerStates[i].slot == inputBindDesc.BindPoint)
			{
				shader->samplerStates[i].flag |= shaderType;
				alreadyCreated = true;
				break;
			}

			if (alreadyCreated)
				break;

			SamplerState &sampler = shader->samplerStates.Append();
			sampler.slot = inputBindDesc.BindPoint;
			sampler.flag = shaderType;
		}break;
		case D3D_SIT_TEXTURE:
		{
			bool alreadyCreated = false;
			for (uint i = 0; i < oldTextureLength; ++i)
			if (shader->textureSlots[i].bufferPlace == inputBindDesc.BindPoint)
			{
				shader->textureSlots[i].flag |= shaderType;
				alreadyCreated = true;
				break;
			}

			if (alreadyCreated)
				break;

			for (uint i = 0; i < inputBindDesc.BindCount; ++i)
			{
				ShaderVariable &textureSlot = shader->textureSlots.Append();
				textureSlot.bufferPlace = inputBindDesc.BindPoint + i;
				textureSlot.flag = shaderType;
				textureSlot.name = inputBindDesc.Name;
				textureSlot.name.Append(i);
			}

		}break;
		}

	}

	reflection->Release();
}

void			D3D::SetShader(const Shader *shader)
{
	assert(shader);

	if (currentShader == shader)
		return;

	currentShader = (Shader *)shader;

	m_deviceContext->VSSetShader(shader->vertexShader, nullptr, 0);
	m_deviceContext->PSSetShader(shader->pixelShader, nullptr, 0);

	if (shader->inputLayout)
		m_deviceContext->IASetInputLayout(shader->inputLayout);

	if (shader->rasterizer)
		m_deviceContext->RSSetState(shader->rasterizer);

	if (shader->depthStencilState)
		m_deviceContext->OMSetDepthStencilState(shader->depthStencilState, 0);

	for (uint i = 0; i < shader->samplerStates.Length(); ++i)
	{
		SamplerState &ss = shader->samplerStates[i];
		if ((shader->samplerStates[i].flag & SVF_VERTEXSHADER) != 0)
			m_deviceContext->VSSetSamplers(shader->samplerStates[i].slot, 1, &shader->samplerStates[i].samplerState);
		if ((shader->samplerStates[i].flag & SVF_PIXELSHADER) != 0)
			m_deviceContext->PSSetSamplers(shader->samplerStates[i].slot, 1, &shader->samplerStates[i].samplerState);
	}

	ApplyConstantBuffers();
}

void			D3D::RenderMesh(const Mesh *mesh, const int subset) const
{
	UINT32 offset = 0;
	m_deviceContext->IASetVertexBuffers(0,
		1,
		&mesh->vertexBuffer,
		&mesh->vertexSize,
		&offset);	

	if (mesh->indexBuffer)
	{
		m_deviceContext->IASetIndexBuffer(mesh->indexBuffer,
			DXGI_FORMAT_R32_UINT,
			0);
		
		m_deviceContext->DrawIndexed(mesh->nrIndices, 0, 0);
	}
	else
	{
		if (subset == -1)
			m_deviceContext->Draw(mesh->nrVertices, 0);
		else
		{
			assert((uint) subset < mesh->nrSubsets);
			m_deviceContext->Draw(mesh->subsets[subset].length, mesh->subsets[subset].startVertex);
		}
	}	
}

void			D3D::ApplyTexture(const Texture *texture, const uint slot)
{
	for (uint i = 0; currentShader->textureSlots.Length(); ++i)
	{
		ShaderVariable &texVar = currentShader->textureSlots[i];
		
		if (texVar.bufferPlace == slot)
		{
			if ((texVar.flag & SVF_VERTEXSHADER) != 0)
				m_deviceContext->VSSetShaderResources(texVar.bufferPlace, 1, (ID3D11ShaderResourceView **)&texture->m_texture);
			else if ( (texVar.flag & SVF_PIXELSHADER) != 0)
				m_deviceContext->PSSetShaderResources(texVar.bufferPlace, 1, (ID3D11ShaderResourceView **)&texture->m_texture);
			break;
		}
	}
}

void			D3D::ApplyConstantBuffers()
{
	ID3D11Buffer *vsBuffers[8] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr },
		*psBuffers[8] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

	for (uint i = 0; i < currentShader->constantBuffers.Length(); ++i)
	{
		ConstantBuffer &cBuffer = currentShader->constantBuffers[i];


		D3D11_MAPPED_SUBRESOURCE subRes;
		if (m_deviceContext->Map(cBuffer.gpuBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subRes) == S_OK)
		{
			memcpy(subRes.pData, cBuffer.data, cBuffer.bufferSize);
			m_deviceContext->Unmap(cBuffer.gpuBuffer, 0);
		}
		else
			WBOX(L"Couldn't Map ConstantBuffer!");

		if ((cBuffer.flag & SVF_VERTEXSHADER) != 0)
			vsBuffers[cBuffer.slotNumber] = cBuffer.gpuBuffer;
		if ((cBuffer.flag & SVF_PIXELSHADER) != 0)
			psBuffers[cBuffer.slotNumber] = cBuffer.gpuBuffer;
	}

	m_deviceContext->VSSetConstantBuffers(0, 8, vsBuffers);
	m_deviceContext->PSSetConstantBuffers(0, 8, psBuffers);
}