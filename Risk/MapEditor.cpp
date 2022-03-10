#include "MapEditor.h"



MapEditor::MapEditor()
{
	current = nullptr;
}

void MapEditor::loop()
{
	cout << "Welcome to map editor" << endl;
	bool done = false;
	while (!done)
	{
		display();
		done = getCommand();
	}
	cout << "Thank you for using map editor" << endl;
}

void MapEditor::display()
{
	cout << "MAP EDITOR" << endl << endl << endl;
	cout << "new <map name>" << endl;
	cout << "add <name> <x_coordinate> <y_coordinate>" << endl;
	cout << "link <name 1> <name 2>" << endl;
	cout << "enter 'done' to quit" << endl;
	cout << endl << endl;

	if (current != nullptr)
	{
		current->print();
	}
	else
	{
		cout << "No map currently open" << endl;
	}
	
}

bool MapEditor::getCommand()
{
	vector<string> parse = getUserInput();
	if (parse.size() < 1) //check for empty input
	{
		return false;
	}
	else if (parse[0] == "new")
	{
		if (parse.size() < 2)
		{
			cout << "not enough inputs" << endl;
		}
		else if (parse.size() == 2) //if no sizes
		{
			string map_name = parse[1];
			current = newMap(map_name);
		}
		else if (parse.size() == 4) //size given
		{
			string map_name = parse[1];
			int rows_in = stoi(parse[2]);
			int cols_in = stoi(parse[3]);
			current = newMap(map_name, rows_in, cols_in);
		}
		else
		{
			cout << "invalid number of inputs " << endl;
		}
	}
	else if (parse[0] == "add")
	{
		if (current == nullptr)
		{
			cout << "no map currently open" << endl;
		}
		else if (parse.size() == 4)
		{
			string region_name = parse[1];
			int x_coord = stoi(parse[2]); //NOTE: Will this cause issues with invalid input? Check this out later
			int y_coord = stoi(parse[3]); //NOTE: See above - also other places you use stoi
			int new_region_id = current->getNumRegions();
			Region* new_region = new Region(region_name, new_region_id);
			current->addRegion(*new_region, x_coord, y_coord);
		}
		else
		{
			cout << "invalid number of inputs " << endl;
		}
	}
	else if (parse[0] == "link")
	{
		if (parse.size() == 3)
		{
			string first_region = parse[1];
			string second_region = parse[2];
			current->link(first_region, second_region);
		}
		else
		{
			cout << "invalid number of inputs " << endl;
		}
	}
	else if (parse[0] == "delete")
	{
		if (current == nullptr)
		{
			cout << "no map currently open" << endl;
		}
		else if (parse.size() == 2)
		{
			string region_name = parse[1];
			current->removeRegion(region_name);
		}
		else
		{
			cout << "invalid number of inputs " << endl;
		}
	}
	else if (parse[0] == "done")
	{
		return true;
	}
	return false;
}

vector<string> MapEditor::getUserInput()
{
	vector<string> parse;
	string command;

	cout << "::";

	command.clear();
	parse.clear();
	cin.clear();


	getline(cin, command);
	stringstream stream(command);
	string temp;

	while (stream >> temp)
	{
		parse.push_back(temp);
	}

	return parse;
}

DisplayMap* MapEditor::newMap(string map_name)
{
	DisplayMap* new_map = new DisplayMap(map_name);
	return new_map;
}

DisplayMap* MapEditor::newMap(string map_name, int rows, int columns)
{
	DisplayMap* new_map = new DisplayMap(map_name, rows, columns);
	return new_map;
}
