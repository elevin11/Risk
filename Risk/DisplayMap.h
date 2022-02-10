#pragma once
#include "Map.h"
#include <cmath>
#include <utility>

enum direction { North, West, South, East };

class DisplayMap :
    public Map
{
public:
    DisplayMap(string); //defaults to 1x1 blank map
    DisplayMap(string, int rows_in, int columns_in);
    void addRegion(Region& newRegion, int x_coord, int y_coord); //adds given region to map in location of coordinates - if name = '-', adds blank region
    Region* getRegion(string); //returns region with given name
    void print();
    bool checkForRegion(string); //checks by name
    bool checkForRegion(int row, int col); //checks by coordinate
    bool checkDuplicate(Region&);
    void link(string, string);
    pair<int, int> getCoordinates(string);



    struct compSize_LT
    {
        bool operator()(vector<Region*>  lhs, vector<Region*> rhs) const
        {
            return lhs.size() < rhs.size();
        }
    };
private:
    string name; //name of map
    vector<vector<Region*>> regions; //list of columns, each of which is vector of region pointers
 //   int numRegions = 0;
    int maxName = 16;
    int maxRows = 0;
};

