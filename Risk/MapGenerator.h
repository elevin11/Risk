#pragma once
#include "DisplayMap.h"
#include "Map.h"
#include "Player.h"

#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
using namespace std;

class MapGenerator
{
public:
	MapGenerator(vector<string> region_names_in, vector<Player*> players_in);
	DisplayMap * generate(string name_in, int rows_in, int columns_in); //returns a randomly generated map with given dimensions. Regions randomly assigned owners based on player list
private:
	vector<string> region_names;
	vector<Player*> players;
};

