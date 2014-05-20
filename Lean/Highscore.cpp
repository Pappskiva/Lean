#include "Highscore.h"


Highscore::Highscore()
{
	first = 0;
	last = 0;
	current = 0;
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
void Highscore::PrintHighscore(char* fontInfoFileName, WCHAR* fontTextureFileName, int sentenceMaxLength,
	D3D* d3d, int screenW, int screenH, m4 baseViewMatrix)
{
	LoadHighscoreFromText("../Lean/data/HighscoreList.txt");
	if (first == NULL)
	{
		//skriver ut "Highscore listan är tom"
	}
	else
	{
		int i = 1;
		int xPos, yPos;
		xPos = 75;
		yPos = 50;
		current = first;
		while (current->next != NULL)
		{
			string name = current->name;		
			char points[16];
			_itoa_s(current->value, points, 10);

			string temp = i + ": " + name + "    " + points;

			sentence = new SentenceClass;
			sentence->Initialize(fontInfoFileName, fontTextureFileName, sentenceMaxLength, d3d, screenW, screenH, baseViewMatrix);

			//Skriver ut namn
			char* textToSend = (char*)temp.c_str();
			sentence->SetText(textToSend, d3d);
			sentence->SetPosition(xPos, yPos);
			sentence->Render(d3d);

			i++;
			yPos += 20;
		}
	}
}
void Highscore::Shutdown()
{
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
				current->next->prev = first;
				current->prev = NULL;
				first->next = current->next;
				first->prev = current;
				current->next = first;
				first = current;
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