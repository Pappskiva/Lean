#ifndef SKYBOX_H
#define SKYBOX_H

#include "D3D.h"
#include "Texture.h"
#include "Shader.h"

class Skybox
{
public:
	Skybox();
	~Skybox();

	bool Initialize(D3D* d3d, const HString &textureFilePath);
	void Shutdown();
	void Render(D3D* d3d);
	void UpdateShaderVariables(v3 cameraPos, m4 viewMatrix, m4 projectionMatrix);
	void SetYawPitchRoll(float yaw, float pitch, float roll);	// Om man vill rotera skyboxen
	bool SwitchSkybox(D3D* d3d, const HString &textureFilePath);

	Texture* GetTextureCube();

private:
	Mesh*		mMesh;
	Shader*		mShader;
	Texture*	mTextureCube;

	float mYaw, mPitch, mRoll;
};

#endif