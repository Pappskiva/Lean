#pragma once

#include <fstream>
#include <string>
#include "SentenceClass.h"
using namespace std;


class Highscore
{
private:
	struct Node
	{
		string name;
		int value;
		Node* next;
		Node* prev;
	};
public:
	Highscore();
	~Highscore();
	void SaveScore(string name, int totalScore);
	void PrintHighscore(SentenceClass* sentence, D3D* d3d, int screenWidth, int screenHeight);
	void Shutdown();
	int CheckTopTen();

private:

	void LoadHighscoreFromText(char* filename);
	void SaveHighscoreToText(char* filename);
	void SortHighscore();

	Node* first;
	Node* last;
	Node* current;

	char* m_filename;
};

