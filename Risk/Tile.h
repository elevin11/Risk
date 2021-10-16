#pragma once
#include "Region.h"

//enum direction { North, West, South, East };

class Tile :
    public Region
{
public:
    Tile(string name_in, vector<Tile*>& neighbors_in, int id_in);
private:
    string name = "";
    vector<Region*> neighbors;
    Player* owner = nullptr;
    int troops = 0;
    const int id = 0;
    
};

