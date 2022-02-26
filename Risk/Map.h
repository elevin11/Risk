#pragma once
#include "Continent.h"
#include "Region.h"

#include <iostream>
#include<string>
#include<vector>
using namespace std;

class Map
{
public:
    Map() {};
    Map(string); 
    Map(string, vector<Region *>); //constructor that builds map with given name from list of regions;

    void addRegion(Region&); //adds given region to map with no neighbors
    void addRegion(Region&, vector<Region*>); //adds given region to map with specified neighbors
    void removeRegion(string name);
    void removeAllRegions();

    virtual void print(); // print lists of regions and their neighbors
    void link(string, string); // connects two regions in map
    virtual bool checkDuplicate(Region&); //checks if given region is already part of map
    virtual bool checkForRegion(string); //checks if region by that name belongs to map
    virtual Region* getRegion(string); //returns region with given name
    virtual int getNumRegions();
    void changeNumRegions(int change);
    

    virtual void addContinent(string, int, vector<Region*>); //adds continent with given name from list of regions - checks if any region in list is already part of map
    int getNumContinents();
    Continent* getContinent(string);
    vector<Continent*> getContinentList();
    void updateAllContinents();

private:
    string name; //name of map
    vector<Region *> regions; //list of regions, each must have unique name
    vector<Continent * > continents; //list of continents, must partition list of regions
    int num_regions = 0;

};

