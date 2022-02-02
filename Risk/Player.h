#pragma once

#include <string>
using namespace std;

class Player
{
public:
	Player() {};
	Player(string);
	Player(string name_in, int troops_in, int id_in); //creates player with given name, number of troops, and id
	bool operator==(const Player&) const;
	int get_id() const;
	string getName();
//REGIONS
	int getRegionsOwned();
	void incRegions();
	void decRegions();
//TROOPS
	int getTroops();
	void setTroops(int troops_in);
	int getDeployable();
	void setDeployable(int troops_in);


	void recruit(int troops_in); //gives player new troops to deploy
private:
	string name = "";
	int troops = 0;
	int deployable = 0;
	int regionsOwned = 0;
	const int id = 0; 
};

