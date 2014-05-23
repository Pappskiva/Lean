#include "NameInput.h"

NameInput::NameInput()
{
	nameRender = nullptr;
	lineRender = nullptr;
	mName = "";
	held = false;
	counter = 0.0f;
	renderingLine = true;
}

NameInput::~NameInput()
{

}

bool NameInput::Initialize(D3D* d3d, int screenWidth, int screenHeight)
{
	bool result;

	// Renderingen av namnet
	nameRender = new SentenceClass;
	if (!nameRender)
	{
		return false;
	}

	result = nameRender->Initialize(d3d, "center", 2.0f, 4, screenWidth, screenHeight);
	if (!result)
	{
		return false;
	}

	nameRender->SetText(" ", d3d);
	nameRender->SetColor(1, 1, 0);
	nameRender->SetPosition(screenWidth * 0.5, screenHeight * 0.5);

	// Renderingen av understrecket som visar att man kan skriva
	lineRender = new SentenceClass;
	if (!lineRender)
	{
		return false;
	}

	result = lineRender->Initialize(d3d, "left", 2.0f, 1, screenWidth, screenHeight);
	if (!result)
	{
		return false;
	}
	
	lineRender->SetText("_", d3d);
	lineRender->SetColor(1, 1, 0);
	lineRender->SetPosition(screenWidth * 0.5, screenHeight * 0.5);

	return true;
}

void NameInput::Update(float deltaTime)
{
	counter += deltaTime;
	if (counter > 0.5f)
	{
		renderingLine = !renderingLine;
		counter = 0.0f;
	}
}

void NameInput::Render(D3D* d3d)
{
	nameRender->SetText((char*)mName.c_str(), d3d);
	nameRender->Render(d3d);

	if (renderingLine && mName.size() < 3)
	{
		lineRender->Render(d3d);
	}
}

void NameInput::Shutdown()
{
	if (nameRender)
	{
		nameRender->Shutdown();
		nameRender = 0;
	}

	if (lineRender)
	{
		lineRender->Shutdown();
		lineRender = 0;
	}
}

string NameInput::InputName()
{
	char key = CheckKeyboardState();

	if (key == ',')
	{
		held = false;
	}

	if (key != ',' && !held)
	{
		held = true;

		if (key == '-')
		{
			// Erase a character
			if (mName.size() > 0)
			{
				string originalStr = mName;
				mName = originalStr.substr(0, originalStr.size() - 1);

				lineRender->OffsetPosition(-(10 * lineRender->GetLetterScale()), 0);
			}
		}
		else
		{
			// Add a character
			if (mName.size() < 3)
			{
				mName += key;

				lineRender->OffsetPosition(10 * lineRender->GetLetterScale(), 0);
			}
		}
	}

	return mName;
}

char NameInput::CheckKeyboardState()
{
	if (GetAsyncKeyState('A'))
	{
		return 'A';
	}
	if (GetAsyncKeyState('B'))
	{
		return 'B';
	}
	if (GetAsyncKeyState('C'))
	{
		return 'C';
	}
	if (GetAsyncKeyState('D'))
	{
		return 'D';
	}
	if (GetAsyncKeyState('E'))
	{
		return 'E';
	}
	if (GetAsyncKeyState('F'))
	{
		return 'F';
	}
	if (GetAsyncKeyState('G'))
	{
		return 'G';
	}
	if (GetAsyncKeyState('H'))
	{
		return 'H';
	}
	if (GetAsyncKeyState('I'))
	{
		return 'I';
	}
	if (GetAsyncKeyState('J'))
	{
		return 'J';
	}
	if (GetAsyncKeyState('K'))
	{
		return 'K';
	}
	if (GetAsyncKeyState('L'))
	{
		return 'L';
	}
	if (GetAsyncKeyState('M'))
	{
		return 'M';
	}
	if (GetAsyncKeyState('N'))
	{
		return 'N';
	}
	if (GetAsyncKeyState('O'))
	{
		return 'O';
	}
	if (GetAsyncKeyState('P'))
	{
		return 'P';
	}
	if (GetAsyncKeyState('Q'))
	{
		return 'Q';
	}
	if (GetAsyncKeyState('R'))
	{
		return 'R';
	}
	if (GetAsyncKeyState('S'))
	{
		return 'S';
	}
	if (GetAsyncKeyState('T'))
	{
		return 'T';
	}
	if (GetAsyncKeyState('U'))
	{
		return 'U';
	}
	if (GetAsyncKeyState('V'))
	{
		return 'V';
	}
	if (GetAsyncKeyState('W'))
	{
		return 'W';
	}
	if (GetAsyncKeyState('X'))
	{
		return 'X';
	}
	if (GetAsyncKeyState('Y'))
	{
		return 'Y';
	}
	if (GetAsyncKeyState('Z'))
	{
		return 'Z';
	}
	if (GetAsyncKeyState('0'))
	{
		return '0';
	}
	if (GetAsyncKeyState('1'))
	{
		return '1';
	}
	if (GetAsyncKeyState('2'))
	{
		return '2';
	}
	if (GetAsyncKeyState('3'))
	{
		return '3';
	}
	if (GetAsyncKeyState('4'))
	{
		return '4';
	}
	if (GetAsyncKeyState('5'))
	{
		return '5';
	}
	if (GetAsyncKeyState('6'))
	{
		return '6';
	}
	if (GetAsyncKeyState('7'))
	{
		return '7';
	}
	if (GetAsyncKeyState('8'))
	{
		return '8';
	}
	if (GetAsyncKeyState('9'))
	{
		return '9';
	}
	if (GetAsyncKeyState(VK_BACK))
	{
		return '-';		// '-' signifies "backspace"
	}

	return ',';			// ',' signifies "no input"
}