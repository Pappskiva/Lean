#include "Menu.h"

Menu::Menu()
{
	mLogo = 0;
	mOption1 = 0;
	mOption2 = 0;
	mOption3 = 0;
	mInputPtr = 0;
	mCameraPtr = 0;
	colors = 0;
}

Menu::~Menu()
{

}

bool Menu::Initialize(D3D* d3d, Input* inputPtr, Camera* cameraPtr, int screenWidth, int screenHeight)
{
	bool result;

	mInputPtr = inputPtr;
	mCameraPtr = cameraPtr;
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;
	decision = 1;
	nrOfOptions = 3;
	inputCooldown = 0.0f;

	colorIndex = 0;

	// Skapa text klass 1
	mOption1 = new SentenceClass;
	if (!mOption1)
	{
		return false;
	}

	// Initialisera text klass 1
	result = mOption1->Initialize(d3d, "center", 1.5f, 16, screenWidth, screenHeight);
	if (!result)
	{
		return false;
	}

	// Sätt värden
	mOption1->SetText("Play", d3d);
	mOption1->SetColor(1, 1, 1);
	mOption1->SetPosition(screenWidth / 2, (screenHeight / 2));

	// Skapa text klass 2
	mOption2 = new SentenceClass;
	if (!mOption2)
	{
		return false;
	}

	// Initialisera text klass 2
	result = mOption2->Initialize(d3d, "center", 1.5f, 16, screenWidth, screenHeight);
	if (!result)
	{
		return false;
	}

	// Sätt värden
	mOption2->SetText("Highscore", d3d);
	mOption2->SetColor(1, 1, 1);
	mOption2->SetPosition(screenWidth / 2, (screenHeight / 2) + 35);

	// Skapa text klass 3
	mOption3 = new SentenceClass;
	if (!mOption3)
	{
		return false;
	}

	// Initialisera text klass 3
	result = mOption3->Initialize(d3d, "center", 1.5f, 16, screenWidth, screenHeight);
	if (!result)
	{
		return false;
	}

	// Sätt värden
	mOption3->SetText("Quit", d3d);
	mOption3->SetColor(1, 1, 1);
	mOption3->SetPosition(screenWidth / 2, (screenHeight / 2) + 70);

	// Sätt färgvärden
	colors = new Color[7];
	if (!colors)
	{
		return false;
	}

	// Red
	colors[0].r = 255;
	colors[0].g = 0;
	colors[0].b = 0;

	// Orange
	colors[1].r = 255;
	colors[1].g = 127;
	colors[1].b = 0;

	// Yellow
	colors[2].r = 255;
	colors[2].g = 255;
	colors[2].b = 0;

	// Green
	colors[3].r = 0;
	colors[3].g = 255;
	colors[3].b = 0;

	// Blue
	colors[4].r = 0;
	colors[4].g = 0;
	colors[4].b = 255;

	// Indigo
	colors[5].r = 75;
	colors[5].g = 0;
	colors[5].b = 130;

	// Violet
	colors[6].r = 143;
	colors[6].g = 0;
	colors[6].b = 255;

	// Current color (red default)
	currentColor.r = 255;
	currentColor.g = 0;
	currentColor.b = 0;

	return true;
}

int Menu::Update(float deltaTime)
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
	mOption3->SetColor(1, 1, 1);

	// Check inputs
	if (inputCooldown == 0.0f)
	{
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
			// Returnera valet till Application så får den avgöra vad som ska göras
			if (mSubMenu == SUB_PLAY)
			{
				return decision + 3;	// ugly, I know
			}
			else if (mSubMenu == SUB_MAIN)
			{
				if (decision == 1)
				{
					mSubMenu = SUB_PLAY;
				}
				else
				{
					return decision;
				}
			}
		}
		if (mInputPtr->IsEscapePressed())
		{
			if (mSubMenu != SUB_MAIN)
			{
				mSubMenu = SUB_MAIN;
			}
		}
	}

	// Change current color
	bool r1, r2, r3;
	float nr;

	r1 = ChangeNumber(currentColor.r, colors[colorIndex].r, deltaTime);
	r2 = ChangeNumber(currentColor.g, colors[colorIndex].g, deltaTime);
	r3 = ChangeNumber(currentColor.b, colors[colorIndex].b, deltaTime);
	
	if (r1 == true && r2 == true && r3 == true)
	{
		colorIndex++;
		if (colorIndex >= 6)
		{
			colorIndex = 0;
		}
	}

	// Change selected options' color
	switch (decision)
	{
	case 1:
		//mOption1->SetColor(1, 1, 0);
		mOption1->SetColor(currentColor.r / 255.0f, currentColor.g / 255.0f, currentColor.b / 255.0f);
		break;

	case 2:
		//mOption2->SetColor(1, 1, 0);
		mOption2->SetColor(currentColor.r / 255.0f, currentColor.g / 255.0f, currentColor.b / 255.0f);
		break;

	case 3:
		//mOption2->SetColor(1, 1, 0);
		mOption3->SetColor(currentColor.r / 255.0f, currentColor.g / 255.0f, currentColor.b / 255.0f);
		break;

	default:
		mOption1->SetColor(currentColor.r / 255.0f, currentColor.g / 255.0f, currentColor.b / 255.0f);
	}


	// Måste returnera en int så -1 duger :)
	return -1;
}

void Menu::Render(D3D* d3d)
{
	// Set Text depending on which submenu you're currently on
	if (mSubMenu == SUB_MAIN)
	{
		mOption1->SetText("Play", d3d);
		mOption2->SetText("Highscore", d3d);
		mOption3->SetText("Quit", d3d);
	}
	else if (mSubMenu == SUB_PLAY)
	{
		mOption1->SetText("Endless", d3d);
		mOption2->SetText("Classic", d3d);
		mOption3->SetText("Easy", d3d);
	}

	mOption1->Render(d3d);
	mOption2->Render(d3d);
	mOption3->Render(d3d);
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
	if (mOption3)
	{
		mOption3->Shutdown();
		mOption3 = 0;
	}
}

bool Menu::ChangeNumber(float &from, float to, float deltaTime)
{
	bool result = false;

	// Increase number
	if (from == to)
	{
		result = true;
	}
	else if (from < to)
	{
		//float diff = to - from;
		//from += diff * deltaTime;
		from++;
		if (from > to)
		{
			from = to;
			result = true;
		}
	}
	// Decrease number
	else
	{
		//float diff = from - to;
		//from -= diff * deltaTime;
		from--;
		if (from < to)
		{
			from = to;
			result = true;
		}
	}

	return result;
}