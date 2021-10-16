#pragma once
#include "Map.h"

enum direction { North, West, South, East };

class DisplayMap :
    public Map
{
public:
    DisplayMap(string);
    void addRegion(Region& newRegion, int x_coord, int y_coord); //adds given region to map in location of coordinates
private:
    string name; //name of map
    vector<vector<Region*>> regions; //list of regions, each must have unique name
    int numRegions = 0;
};

