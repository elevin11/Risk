#include <string>
#include <vector>
#include "Player.h"
#include "Region.h"
using namespace std;

#pragma once
class Continent
{
public:
	Continent() { };
	Continent(string, int, vector<Region *>); //constructs with given name, value, and list of regions
	bool updateOwner(); //returns true if owner is changed
	Player* getOwner();
	int getValue(); 
	string getName();
private:
	string name;
	int value = 0; //number of troops from controlling this continent
	vector<Region *> regions;
	Player* owner = nullptr;
};

