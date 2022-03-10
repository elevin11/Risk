#pragma once
#include "DisplayMap.h"
#include "Map.h"

#include <sstream>
#include <string>
#include <vector>
using namespace std;


class MapEditor
{
public:
	MapEditor();
	void loop();					//start main editor loop
	void display();					//displays map editor title + options + view of current map (if applicable)
	bool getCommand();				//processes user input, returns whether input is finished
	vector<string> getUserInput();	//gets command from the user and outputs list split by spaces
	DisplayMap* newMap(string map_name);//creates empty new map with given name
	DisplayMap* newMap(string map_name, int rows, int columns);//creates empty new map with given name + dimensions
	

//LOAD + SAVE SYSTEM
	void save();//saves current map to disk
	DisplayMap* load(string map_name);//loads new map from disk
private:
	DisplayMap* current;	//Map currently being viewed/worked on
};

