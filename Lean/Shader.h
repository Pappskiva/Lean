#ifndef DSHADER_H
#define DSHADER_H

#include "DuckRenderer\Array.h"
#include "DuckRenderer\HString.h"

#include <string>
#include <D3D11.h>
#include <fstream>
#include <string>
/*
DESCRIPTION
The shader class contains all the necessary stages and states to render an object to the screen
The vertex stage is the only required stage that has to be set!
*/
enum ShaderVariableFlags : uint
{
	SVF_VERTEXSHADER = 0x1,
	SVF_HULLSHADER = 0x2,
	SVF_DOMAINSHADER = 0x4,
	SVF_GEOMETRYSHADER = 0x8,
	SVF_PIXELSHADER = 0x10,
	SVF_COMPUTESHADER = 0x20,

	SVF_FLOAT = 0x100,
	SVF_UINT = 0x200,
	SVF_INT = 0x800,

	SVF_GBUFFER = 0x1000,
	SVF_RENDERTARGET = 0x2000,
	SVF_DEPTHSTENCIL = 0x4000,

};

struct ShaderInfo
{
	HString path;
	HString entryPoint;
	HString version;

	ShaderInfo()
	{
		entryPoint = "";
		version = "";
	}

	ShaderInfo(const HString &path, const HString &entryPoint, const HString &version)
	{
		this->path = path;
		this->entryPoint = entryPoint;
		this->version = version;
	}
};

struct ShaderVariable
{
	int flag;
	uint bufferPlace;
	uint size;
	HString name;

	ShaderVariable& operator = (const ShaderVariable &member);

	ShaderVariable(){ flag = (int)0; }
	ShaderVariable(const int flag, const uint bufferPlace);
};

struct ConstantBuffer
{
	int flag;
	uint slotNumber;
	uint bufferSize;
	uint nrMembers;
	ID3D11Buffer *gpuBuffer;

	byte *data;
	ShaderVariable *members;
};

struct SamplerState
{
	uint slot;
	int64 flag;
	ID3D11SamplerState *samplerState;

	SamplerState()
	{
		flag = 0;
		samplerState = nullptr;
	}
};

class Shader
{
public:
	ID3D11VertexShader			*vertexStage;
	ID3D11GeometryShader		*geometryStage;
	ID3D11HullShader			*hullStage;
	ID3D11DomainShader			*domainStage;
	ID3D11PixelShader			*pixelStage;
	ID3D11ComputeShader			*computeStage;
	ID3D11RasterizerState		*rasterizer;
	ID3D11DepthStencilState		*depthStencilState;
	ID3D11InputLayout			*inputLayout;
	Array<SamplerState>			samplerStates;
	Array<ConstantBuffer>		constantBuffers;
	Array<ShaderVariable>		textureSlots;

	uint						ID;
	Shader						*nextPass;

public:

	Shader();
	void						Flush();

	void						AddPass(Shader const* newPass);

	void						AddTextureSampler(const Shader &shader, int inSlot, const uint resultSlot);
	void						AddTextureSampler(const ID3D11SamplerState *sampler, const uint slot);
	void						AddTextureSlotFlag(const uint index, const ShaderVariableFlags flag);


	void						SetRasterizer(const Shader &shader);
	void						SetDepthStencilState(const Shader &shader);
	void						SetDepthStencilState(const ID3D11DepthStencilState *depthSS);
	void						SetInputLayout(const Shader &shader);

	void						SetVertexShader(const Shader &shader);
	void						SetPixelShader(const Shader &shader);
	//not fast!
	bool						SetVariable(const HString &name, void *data, const uint dataSize);
	//fast :D
	void						UpdateConstantBuffer(const uint index, void *data, const uint size);

	uint						GetID() const;

	friend class Renderer;
};

inline uint Shader::GetID() const
{
	return ID;
}

#endif