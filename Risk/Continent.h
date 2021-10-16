#include <string>
#include <vector>
#include "Region.h"
using namespace std;

#pragma once
class Continent
{
public:
	Continent() { };
	Continent(string, int, vector<Region *>) {}; //constructs with given name, value, and list of regions
	int getValue(); 
	string getRegionNames(); //returns list of region names;
private:
	string name;
	int value; //number of troops from controlling this continent
	vector<Region *> regions;
};

