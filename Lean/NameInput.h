#ifndef NAMEINPUT_H
#define NAMEINPUT_H

#include "D3D.h"
#include "SentenceClass.h"

using namespace std;

class NameInput
{
public:
	NameInput();
	~NameInput();

	bool Initialize(D3D* d3d, int screenWidth, int screenHeight);
	void Update(float deltaTime);
	void Render(D3D* d3d);
	void Shutdown();

	string InputName();

private:
	SentenceClass* nameRender;
	SentenceClass* lineRender;

	string mName;
	bool held;
	float counter;
	bool renderingLine;

	char CheckKeyboardState();

};

#endif