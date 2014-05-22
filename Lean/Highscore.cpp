#include "Highscore.h"


Highscore::Highscore()
{
	first = NULL;
	last = NULL;
	current = NULL;
	Initialized = false;
}
Highscore::~Highscore(){}

void Highscore::SaveScore(string name, int totalScore)
{
	Node *newNode = new Node;
	newNode->name = name;
	newNode->value = totalScore;
	newNode->next = NULL;
	newNode->prev = NULL;

	LoadHighscoreFromText("../Lean/data/HighscoreList.txt");

	if (first == NULL)
	{
		last = newNode;
		first = newNode;
	}
	else
	{
		newNode->prev = last;
		last->next = newNode;
		last = newNode;
		last->next = NULL;
	}

	SaveHighscoreToText("../Lean/data/HighscoreList.txt");
}

void Highscore::PrintHighscore(D3D* d3d, int screenWidth, int screenHeight)
{
	
	if (!Initialized)
	{
		const char* alignment = "left";
		float letterScale = 1.0f;
		int sentenceMaxLength = 16;
		sentence = new SentenceClass;
		sentence->Initialize(d3d, alignment, letterScale, sentenceMaxLength, screenWidth, screenHeight);
		sentence->SetColor(0.5f, 0.5f, 0.5f);
		Initialized = true;
	}

	bool result = false;;

	LoadHighscoreFromText("../Lean/data/HighscoreList.txt");
	if (first != NULL)
	{
		int i = 0;
		int xPos, yPos;
		xPos = screenWidth /3;
		yPos = screenHeight / 3;
		current = first;
		while (current->next != NULL && i < 11)
		{
			i++;
			yPos += 20;
			string name = current->name;		//Prep for print
			char points[16];
			_itoa_s(current->value, points, 10);

			string printString = to_string(i) + ": " + name + " " + points;//Put everything into one line
			char* textToSend = (char*)printString.c_str();//Convert to char*

			sentence->SetText(textToSend, d3d);
			sentence->SetPosition(xPos, yPos);
			sentence->Render(d3d);

			current = current->next;
		}
	}
	else
	{
		//skriver ut "Highscore listan är tom"
	}
}
void Highscore::Shutdown()
{
	sentence->Shutdown();
	delete sentence;
	sentence = 0;
	while (first != NULL)
	{
		current = first;
		first = current->next;

		current->name = "";
		current->value = 0;
		current->next = NULL;
		current->prev = NULL;
		delete current;
	}
	first = NULL;
	last = NULL;
	current = NULL;

}
void Highscore::LoadHighscoreFromText(char* filename)
{
	Shutdown();

	ifstream text;
	text.open(filename);
	if (text.is_open())
	{
		string tName;
		string tPoint;
		int ttPoint;

		while (getline(text, tName))
		{
			Node* node = new Node;
			node->next = NULL;
			node->prev = NULL;
			node->name = tName;

			getline(text, tPoint);
			ttPoint = stoi(tPoint);//PArsar från string till int apparantly...

			node->value = ttPoint;


			if (first == NULL)
			{
				first = node;
				last = node;
			}
			else
			{
				last->next = node;
				node->prev = last;
				last = node;
			}
		}
	}
	text.close();
}
void Highscore::SaveHighscoreToText(char* filename)
{
	SortHighscore();

	ofstream text;
	text.open(filename);
	if (text.is_open())
	{
		current = first;

		while (current != NULL)
		{
			text << current->name;
			text << "\n"; //Ny rad
			text << current->value;
			text << "\n"; //Ny rad
			current = current->next;
		}
	}

	text.close();
}
void Highscore::SortHighscore()
{
	Node* lastSorted = first;

	while (lastSorted != last)
	{
		current = lastSorted->next;
		while (current->prev != NULL && current->prev->value < current->value)
		{
			if (current->prev == first)//if swapping with the first element 
			{
				if (current == last)
				{
					first->next = current->next;
					first->prev = current;
					current->next = first;
					current->prev = NULL;
					last = first;
					first = current;
				}
				else
				{
					current->next->prev = first;
					current->prev = NULL;
					first->next = current->next;
					first->prev = current;
					current->next = first;
					first = current;
				}
			}
			else if (current == last)//if swapping with the last element 
			{
				lastSorted->next = NULL;
				current->next = lastSorted;
				current->prev = lastSorted->prev;
				current->prev->next = current;
				lastSorted->prev = current;
				last = lastSorted;
			}
			else//Normal swap 
			{
				current->next->prev = current->prev;
				current->prev->next = current->next;
				current->prev->prev->next = current;
				current->prev = current->next->prev->prev;
				current->next = current->next->prev;
				current->next->prev = current;
			}
		}
		if (lastSorted->next != NULL && lastSorted->value >= lastSorted->next->value)
			lastSorted = lastSorted->next;
	}
}