#pragma once

#include "Player.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;

class Region
{
public:
    Region() {};
    Region(string, int); //constructs Region with given name and id
    Region(string, vector<Region*> &, int id_in); //constructs Region with given name and list of neighbors;
    ~Region();

    string getName(); //returns name of Region
    void setName(string); //sets name of Region
    const int getID();

    void setNeighbor(Region &); //Adds border between given Region and this one
    bool isNeighbor(Region &); //Returns whether given Region is neighbor of this one
    string getNeighborNames(); //returns list of neighors' names
    vector<Region *> getNeighbors();
    //test

    void setTroops(int);
    void incTroops(int);
    void decTroops(int);

    void setOwner(Player&);
    Player* getOwner();
    string getOwnerName();
    int getTroops();

private:
    string name = "";
    vector<Region *> neighbors;
    Player* owner = nullptr;
    int troops = 0;
    const int id = 0;
};


