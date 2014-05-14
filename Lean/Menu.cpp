#include "Menu.h"

Menu::Menu()
{
	mLogo = 0;
	mOption1 = 0;
	mOption2 = 0;
	mInputPtr = 0;
	mCameraPtr = 0;
}

Menu::~Menu()
{

}

bool Menu::Initialize(D3D* d3d, Input* inputPtr, Camera* cameraPtr, int screenWidth, int screenHeight, m4 baseViewMatrix)
{
	bool result;

	mInputPtr = inputPtr;
	mCameraPtr = cameraPtr;
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;
	decision = 1;
	nrOfOptions = 2;
	inputCooldown = 0.0f;

	// Skapa text klass 1
	mOption1 = new SentenceClass;
	if (!mOption1)
	{
		return false;
	}

	// Initialisera text klass 1
	result = mOption1->Initialize("data/fontdata_picross.txt", L"data/font_picross.png", 16, d3d, screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		return false;
	}

	// Sätt värden
	mOption1->SetText("Resume", d3d);
	mOption1->SetColor(1, 1, 1);
	mOption1->SetPosition(50, 300);

	// Skapa text klass 2
	mOption2 = new SentenceClass;
	if (!mOption2)
	{
		return false;
	}

	// Initialisera text klass 2
	result = mOption2->Initialize("data/fontdata_picross.txt", L"data/font_picross.png", 16, d3d, screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		return false;
	}

	// Sätt värden
	mOption2->SetText("Quit", d3d);
	mOption2->SetColor(1, 1, 1);
	mOption2->SetPosition(50, 330);

	return true;
}

bool Menu::Update(float deltaTime)
{
	//// Kamera
	//mCameraPtr->SetPosition(v3(0, 0, 0));
	//mCameraPtr->LookAt(v3(0, 10, 0));

	// Input cooldown
	if (inputCooldown > 0.0f)
	{
		inputCooldown -= deltaTime;
		if (inputCooldown < 0.0f)
		{
			inputCooldown = 0.0f;
		}
	}

	// Reset colors
	mOption1->SetColor(1, 1, 1);
	mOption2->SetColor(1, 1, 1);

	// Check inputs
	if (inputCooldown == 0.0f)
	{
		mInputPtr->Frame();
		if (mInputPtr->IsDownPressed())
		{
			inputCooldown = 0.2f;
			decision++;

			if (decision > nrOfOptions)
			{
				decision = 1;
			}
		}
		if (mInputPtr->IsUpPressed())
		{
			inputCooldown = 0.2f;
			decision--;

			if (decision < 1)
			{
				decision = nrOfOptions;
			}
		}
		if (mInputPtr->IsEnterPressed())
		{
			switch (decision)
			{
			case 1:
				return false;
				break;

			case 2:
				return false;
				break;

			default:
				return false;
			}
		}
	}

	// Change selected options' color
	switch (decision)
	{
	case 1:
		mOption1->SetColor(1, 1, 0);
		break;

	case 2:
		mOption2->SetColor(1, 1, 0);
		break;

	default:
		mOption1->SetColor(1, 1, 0);
	}


	return true;
}

void Menu::Render(D3D* d3d)
{
	mOption1->Render(d3d);
	mOption2->Render(d3d);
}

void Menu::Shutdown()
{
	if (mLogo)
	{
		mLogo->Shutdown();
		mLogo = 0;
	}

	if (mOption1)
	{
		mOption1->Shutdown();
		mOption1 = 0;
	}

	if (mOption2)
	{
		mOption2->Shutdown();
		mOption2 = 0;
	}
}