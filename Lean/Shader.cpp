#include "Shader.h"
#include <assert.h>

#define			WBOX(x) MessageBox(NULL, x, L"Shader Error!!", MB_OK | MB_ICONASTERISK);
#define			SAFE_DELETE_ARRAY(x)	{ if(x){delete[] x; x = nullptr;}}
#define			SAFE_RELEASE(x)			{ if(x){x->Release(); x = nullptr;}}

ShaderVariable& ShaderVariable::operator = (const ShaderVariable &in)
{
	this->flag =		in.flag;
	this->bufferPlace = in.bufferPlace;
	this->size =		in.size;
	this->name =		in.name;

	return *this;
}

Shader::Shader() : constantBuffers(8, false)
{
	vertexShader		= nullptr;
	pixelShader			= nullptr;
	rasterizer			= nullptr;
	depthStencilState	= nullptr;
	inputLayout			= nullptr;
	nextPass			= nullptr;
}

void Shader::AddPass(Shader const* newPass)
{
	if (!nextPass)
	{
		nextPass = (Shader *) newPass;
		return;
	}

	Shader *walker = nextPass;
	do
	{
		if (walker->nextPass)
			walker = nextPass;
	}while(walker->nextPass);

	walker->nextPass = (Shader *) newPass;
}

void Shader::SetRasterizer(const Shader &shader)
{
	SAFE_RELEASE(this->rasterizer);
	this->rasterizer = shader.rasterizer;
	this->rasterizer->AddRef();
}

void Shader::SetDepthStencilState(const Shader &shader)
{
	SAFE_RELEASE(this->depthStencilState);
	this->depthStencilState = shader.depthStencilState;
	this->depthStencilState->AddRef();
}

void Shader::SetDepthStencilState(const ID3D11DepthStencilState *depthSS)
{
	SAFE_RELEASE(this->depthStencilState);
	this->depthStencilState = (ID3D11DepthStencilState *) depthSS;
	this->depthStencilState->AddRef();
}

void Shader::SetInputLayout(const Shader &shader)
{
	SAFE_RELEASE(this->inputLayout);
	this->inputLayout = shader.inputLayout;
	this->inputLayout->AddRef();
}

void Shader::SetPixelShader(const Shader &shader)
{
	SAFE_RELEASE(this->pixelShader);
	this->pixelShader = shader.pixelShader;
	this->pixelShader->AddRef();
}

void Shader::AddTextureSampler(const Shader &shader, int inSlot, const uint resultSlot)
{
	ID3D11SamplerState *samplerState = nullptr;
	for (uint i = 0; i < shader.samplerStates.Length(); ++i)
	{
		if (shader.samplerStates[i].slot == inSlot)
		{
			samplerState = shader.samplerStates[i].samplerState;
			break;
		}
	}

	if (!samplerState)
		return;

	for (uint i = 0; i < samplerStates.Length(); ++i)
	{
		if (samplerStates[i].slot == resultSlot)
		{
			SAFE_RELEASE(samplerStates[i].samplerState);
			samplerStates[i].samplerState = samplerState;
			samplerStates[i].samplerState->AddRef();
			break;
		}
	}
}

void Shader::AddTextureSampler(const ID3D11SamplerState *sampler, const uint slot)
{
	for (uint i = 0; i < samplerStates.Length(); ++i)
	{
		if (samplerStates[i].slot == slot)
		{
			SAFE_RELEASE(samplerStates[i].samplerState);
			samplerStates[i].samplerState = (ID3D11SamplerState *) sampler;
			samplerStates[i].samplerState->AddRef();
			break;
		}
	}
}

void Shader::UpdateConstantBuffer(const uint index, void *data, const uint size)
{
	assert(index < constantBuffers.Length());

	if(constantBuffers[index].bufferSize != size)
	{
		uint lol = constantBuffers[index].bufferSize;
		WBOX(L"Invalid Size Parameter!\nMust Be The Same Size As ConstantBuffer!");
		return;
	}

	memcpy(constantBuffers[index].data, data, size);
}

bool Shader::SetVariable(const HString &name, void *data, const uint dataSize)
{
	for (uint c = 0; c < constantBuffers.Length(); ++c)
	for (uint i = 0; i < constantBuffers[c].nrMembers; ++i)
	{
		ShaderVariable &cVar = constantBuffers[c].members[i];
		
		if (dataSize > cVar.size)
			continue;
		
		if (name == cVar.name)
		{
			memcpy(constantBuffers[c].data + cVar.bufferPlace, data, dataSize);
			return true;
		}
	}

	return false;
}

void Shader::Flush()
{
	SAFE_RELEASE(vertexShader);
	SAFE_RELEASE(pixelShader);
	SAFE_RELEASE(rasterizer);
	SAFE_RELEASE(depthStencilState);
	SAFE_RELEASE(inputLayout);

	for (uint i = 0; i < samplerStates.Length(); ++i)
		SAFE_RELEASE(samplerStates[i].samplerState);

	for (uint i = 0; i < constantBuffers.Length(); ++i)
	{
		SAFE_DELETE_ARRAY(constantBuffers[i].data);
		SAFE_DELETE_ARRAY(constantBuffers[i].members);
		SAFE_RELEASE(constantBuffers[i].gpuBuffer);
	}
}