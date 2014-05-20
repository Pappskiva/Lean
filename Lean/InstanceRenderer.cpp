#include "D3D.h"

InstanceRenderer::InstanceRenderer()
{

}

InstanceRenderer::InstanceRenderer(const uint maxInstances, const uint instanceDataMaxSize, const Mesh *mesh)
{
	this->maxInstances = maxInstances;
	this->instanceMaxSize = instanceDataMaxSize;
	this->mesh = (Mesh *) mesh;

	this->numberOfInstances = 0;
	instanceIdentifications.Reserve(maxInstances);
	instanceIdentifications.Fix(true);


	instanceData = new byte[instanceDataMaxSize * maxInstances];
}

void InstanceRenderer::QuickSort(Array<InstanceID> &arr, uint left, uint right)
{
	if(right < left)		// maybe not required because of the ifs at the bottom?
		return;	

	uint L = left, R = right;
	InstanceID tmp;
	byte *temp = new byte[instanceMaxSize];

	if(right - left < 2)
	{
		if(arr[L] > arr[R])
		{
			tmp = arr[L];
			arr[L] = arr[R];
			arr[R] = tmp;

			memcpy(temp, instanceData + instanceMaxSize * L, instanceMaxSize);
			memcpy(instanceData + instanceMaxSize * L, 
				instanceData + instanceMaxSize * R, instanceMaxSize);
			memcpy(instanceData + instanceMaxSize * R, temp, instanceMaxSize);
		}

		delete[] temp;

		return;
	}

	InstanceID pivot = arr[(left + right) / 2];

    while (L <= R)
	{
		while (arr[L] < pivot)
			L++;
		while (arr[R] > pivot)
			R--;

		if (L <= R)
		{
			tmp = arr[L];
			arr[L] = arr[R];
			arr[R] = tmp;

			memcpy(temp, instanceData + instanceMaxSize * L, instanceMaxSize);
			memcpy(instanceData + instanceMaxSize * L, 
				instanceData + instanceMaxSize * R, instanceMaxSize);
			memcpy(instanceData + instanceMaxSize * R, temp, instanceMaxSize);

			L++;
			R--;
		}
	};

	delete[] temp;

    if (left < R)
        QuickSort(arr, left, R);
    if (L < right)
        QuickSort(arr, L, right);

	/*if(right < left)		// maybe not required because of the ifs at the bottom?
		return;

	ParticleID tmp;
	byte *temp = new byte[instanceMaxSize];

	if(right - left < 2)
	{
		if(arr[left] > arr[right])
		{
			tmp = arr[left];
			arr[left] = arr[right];
			arr[right] = tmp;

			memcpy(temp, instanceData + instanceMaxSize * left, instanceMaxSize);
			memcpy(instanceData + instanceMaxSize * left, 
				instanceData + instanceMaxSize * right, instanceMaxSize);
			memcpy(instanceData + instanceMaxSize * right, temp, instanceMaxSize);
		}

		return;
	}

	int pivotIndex = (left + right) / 2;
	ParticleID pivotElement = arr[pivotIndex];
	byte *pivotData = new byte[instanceMaxSize];
	memcpy(pivotData, instanceData + instanceMaxSize * pivotIndex, instanceMaxSize);
	

	arr[pivotIndex] = arr[right];
	arr[right] = pivotElement;

	memcpy(instanceData + instanceMaxSize * pivotIndex, 
		instanceData + instanceMaxSize * right, instanceMaxSize);
	memcpy(instanceData + instanceMaxSize * right, 
		pivotData, instanceMaxSize);


	int storeIndex = left;

	for(int i = left; i < right; ++i)
		if(arr[i] < pivotElement)
		{
			tmp = arr[i];
			arr[i] = arr[storeIndex];
			arr[storeIndex] = tmp;

			memcpy(temp, instanceData + instanceMaxSize * i, instanceMaxSize);
			memcpy(instanceData + instanceMaxSize * i,
				instanceData + instanceMaxSize * storeIndex, instanceMaxSize);
			memcpy(instanceData + instanceMaxSize * storeIndex, temp, instanceMaxSize);

			++storeIndex;
		}

	arr[right] = arr[storeIndex];
	arr[storeIndex] = pivotElement;

	memcpy(instanceData + instanceMaxSize * right, 
		instanceData + instanceMaxSize * storeIndex, instanceMaxSize);
	memcpy(instanceData + instanceMaxSize * storeIndex, 
		pivotData, instanceMaxSize);

	delete[] temp;
	delete[] pivotData;

	if(storeIndex > left + 1)
		QuickSort(arr, left, storeIndex - 1);
	if(storeIndex < right - 1)
		QuickSort(arr, storeIndex + 1, right);*/
}

void InstanceRenderer::Update()
{
	if(!numberOfInstances)
		return;
	
	QuickSort(instanceIdentifications, 0, instanceIdentifications.Length() - 1);
	direct3D->UpdateInstanceRenderer(*this);
}

void InstanceRenderer::Render()
{
	if(!numberOfInstances)
		return;

	uint offset = 0;
	for(uint i = 0; i < batchInfo.Length(); i++)
	{
		if(batchInfo[i].quantity)
		{			
			direct3D->SetShader(batchInfo[i].instanceType.shader);
			direct3D->ApplyTexture(batchInfo[i].instanceType.texture, 0);
			direct3D->RenderInstancerenderer(*this, offset, batchInfo[i].quantity);

			offset += batchInfo[i].quantity;
		}
	}
}

void InstanceRenderer::ClearInstances()
{
	//memset(instanceData, 0, maxInstances * instanceMaxSize);//test temp
	numberOfInstances = 0;
	instanceIdentifications.Clear();
	for (uint i = 0; i < batchInfo.Length(); ++i)
		batchInfo[i].quantity = 0;
}

void InstanceRenderer::Flush()
{
	instanceBuffer->Release();
	instanceBuffer = nullptr;
	delete[] instanceData;
}

void InstanceRenderer::AddInstance(void* data, const uint dataSize, const InstanceID &instanceiD)
{
	if(dataSize > instanceMaxSize || numberOfInstances >= maxInstances)
		return;

	for(uint i = 0; i < batchInfo.Length(); ++i)
	{
		if (batchInfo[i].instanceType == instanceiD)
		{
			instanceIdentifications.Append(instanceiD);
			++batchInfo[i].quantity;
			memcpy(instanceData + instanceMaxSize * numberOfInstances++, data, dataSize);

			return;
		}
	}
	
	instanceIdentifications.Append(instanceiD);
	batchInfo.Append(BatchInfo(instanceiD));
	memcpy(instanceData + instanceMaxSize * numberOfInstances++, data, dataSize);
	batchInfo.Sort();	
}

void InstanceRenderer::SetMesh(const Mesh* mesh)
{
	this->mesh = (Mesh *) mesh;
}

void InstanceRenderer::Initialize(const uint maxInstances, const uint instanceMaxSize, const Mesh *mesh)
{
	this->maxInstances = maxInstances;
	this->instanceMaxSize = instanceMaxSize;
	this->mesh = (Mesh *)mesh;

	this->numberOfInstances = 0;
	instanceIdentifications.Reserve(maxInstances);
	instanceIdentifications.Fix(true);


	instanceData = new byte[instanceMaxSize * maxInstances];
}

/*void InstanceRenderer::_SaveSortToFile(HString &path)
{
	#include <fstream>

	std::ofstream lol(path.AsChar(), std::ios::out);
	
	for(uint i = 0; i < batchInfo.Length(); ++i)
	{
		lol << "MaterialID: " << batchInfo[i].materialID
			<< "      ShaderID: " << particleInfo[i].shaderID 
			<< "      TextureID: " << particleInfo[i].textureID
			<< "      Quantity: " << particleInfo[i].quantity << std::endl;
	}

	for(uint i = 0; i < materials.Length(); ++i)
	{

		lol << i + 1 << ". "
			<< "ShaderID: " << materials[i].shaderID 
			<< "     TextureID : " << materials[i].textureID 
			<< std::endl;
	}

	lol.close();
}*/
//////////////////////////////////////////

bool InstanceID::operator<(const InstanceID &in)
{
	if(shader < in.shader)
	{
		return true;
	}

	if(shader == in.shader && texture < in.texture)
	{
		return true;
	}

	return false;
}

bool InstanceID::operator>(const InstanceID &in)
{
	if(shader > in.shader)
	{
		return true;
	}

	if(shader == in.shader && texture > in.texture)
	{
		return true;
	}

	return false;
}

bool InstanceID::operator == (const InstanceID &in)
{
	return (this->shader == in.shader && this->texture == in.texture);
}

InstanceID& InstanceID::operator=(const InstanceID &in)
{
	this->texture = in.texture;
	this->shader = in.shader;

	return *this;
}

////////////////////////
/*
bool ParticleEffectInfo::operator<(const ParticleEffectInfo &in)
{
	if(shaderID < in.shaderID)
	{
		return true;
	}

	if(shaderID == in.shaderID && textureID < in.textureID)
	{
		return true;
	}

	return false;
}

bool ParticleEffectInfo::operator>(const ParticleEffectInfo &in)
{
	if(shaderID > in.shaderID)
	{
		return true;
	}

	if(shaderID == in.shaderID && textureID > in.textureID)
	{
		return true;
	}

	return false;
}

ParticleEffectInfo& ParticleEffectInfo::operator=(const ParticleEffectInfo &in)
{
	this->textureID = in.textureID;
	this->shaderID = in.shaderID;
	this->materialID = in.materialID;
	this->quantity = in.quantity;

	return *this;
}
*/