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
	void PrintHighscore(char* fontInfoFileName, WCHAR* fontTextureFileName, int sentenceMaxLength,
		D3D* d3d, int screenW, int screenH, m4 baseViewMatrix);
	void Shutdown();

private:

	SentenceClass* sentence;

	void LoadHighscoreFromText(char* filename);
	void SaveHighscoreToText(char* filename);
	void SortHighscore();

	Node* first;
	Node* last;
	Node* current;
	//2 dimensionell array = 2* antal som finns i highscore listan +1
	
};

