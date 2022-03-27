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
	else if (parse[0] == "save")
	{
		save();
	}
	else if (parse[0] == "load")
	{
		if (parse.size() < 2)
		{
			cout << "invalid number of inputs" << endl;
		}
		else
		{
			current = load(parse[1]);
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

void MapEditor::save()
{
	string name = current->getName();
	int max_name = current->getMaxNameLength();
	int num_rows = current->getMaxRows();
	int num_columns = current->getNumColumns();

	ofstream file("saved_maps/" + name + ".txt");

	if (file.is_open())
	{
		file << name << endl;
		file << to_string(max_name) << endl;
		file << to_string(num_rows) << endl;
		file << to_string(num_columns) << endl;

		for (int i = 0; i < num_rows; ++i)
		{
			for (int j = 0; j < num_columns; ++j)
			{
				Region* current_region = current->getRegion(i, j);

				if (current_region != nullptr)
				{
					string line = to_string(i) + " " + to_string(j) + " " + current_region->getName() + " "; //sets coordinates and name

					//check for neighbor to east
					if (j < num_columns - 1 && current->getRegion(i, j + 1) != nullptr)
					{
						if (current_region->isNeighbor(*current->getRegion(i, j + 1)))
						{
							line += "E ";
						}
					}
					else
					{
						line += "e ";
					}

					//check for neighbor to west
					if (j > 0 && current->getRegion(i, j - 1) != nullptr)
					{
						if (current_region->isNeighbor(*current->getRegion(i, j - 1)))
						{
							line += "W ";
						}
					}
					else
					{
						line += "w ";
					}

					//check for neighbor to north
					if (i > 0 && current->getRegion(i - 1, j) != nullptr)
					{
						if (current_region->isNeighbor(*current->getRegion(i - 1, j)))
						{
							line += "N ";
						}
					}
					else
					{
						line += "n ";
					}

					//check for neighbor to south
					if (i < num_rows - 1 && current->getRegion(i + 1, j) != nullptr)
					{
						if (current_region->isNeighbor(*current->getRegion(i + 1, j)))
						{
							line += "S ";
						}
					}
					else
					{
						line += "s ";
					}

					file << line << endl;

				}
			}
		}

		file.close();
	}
	else
	{
		cout << "Error: could not open file" << endl;
	}

}

DisplayMap* MapEditor::load(string map_name)
{ 
	ifstream file( ".\\saved_maps\\" + map_name + ".txt");
	string line;
	vector<string> parse;

	string new_map_name;
	int new_map_max_name = 0;
	int new_map_rows = 0;
	int new_map_cols = 0;
	Region* new_region = nullptr;
	DisplayMap* new_map = nullptr;

	if (file.is_open())
	{
 		int line_count = 0;
		int region_count = 0;

		//first four lines contain map name, max region name lenght, and row + column size
		while (getline(file, line))
		{
			stringstream stream(line);
			string temp;

			while (stream >> temp)
			{
				parse.push_back(temp);
			}

			if (line_count == 0)
			{
				new_map_name = parse[0];
			}
			else if (line_count == 1)
			{
				new_map_max_name = stoi(parse[0]);
			}
			else if (line_count == 2)
			{
				new_map_rows = stoi(parse[0]);
			}
			else if (line_count == 3)
			{
				new_map_cols = stoi(parse[0]);
				new_map = new DisplayMap(new_map_name, new_map_rows, new_map_cols); //now have enough info to construct map
			}
			else if (line_count > 3) //remaining lines encode regions to populate map with - row, column, name, and neighbors
			{
				int new_region_row = stoi(parse[0]);
				int new_region_col = stoi(parse[1]);
				string new_region_name = parse[2];

				new_region = new Region(new_region_name, region_count);
				new_map->addRegion(*new_region, new_region_row, new_region_col);
				++region_count;
			}

			++line_count;
			line.clear();
			parse.clear();
		}
		file.close();
	}
	else
	{
		cout << "Error: could not open file" << endl;
		return nullptr; 
	}

	//link together neightbors
	file.open(".\\saved_maps\\" + map_name + ".txt");
	if (file.is_open())
	{
		int line_count = 0;
		bool east_neighbor = false;
		bool west_neighbor = false;
		bool north_neighbor = false;
		bool south_neighbor = false;

		while (getline(file, line))
		{
			stringstream stream(line);
			string temp;
			//cout << line_count << "  " << line << endl;

			while (stream >> temp)
			{
				parse.push_back(temp);
			}

			if (line_count > 3) //region encoding start on fourth line
			{
				
				//determine which directions region has links to neighbors
				if (parse[3] == "E")
				{
					east_neighbor = true;
				}
				else
				{
					east_neighbor = false;
				}
				
				if (parse[4] == "W")
				{
					west_neighbor = true;
				}
				else
				{
					west_neighbor = false;
				}
				
				if (parse[5] == "N")
				{
					north_neighbor = true;
				}
				else
				{
					north_neighbor = false;
				}
				
				if (parse[6] == "S")
				{
					south_neighbor = true;
				}
				else
				{
					south_neighbor = false;
				}


				//set pointer to current region
				int new_region_row = stoi(parse[0]);
				int new_region_col = stoi(parse[1]);
				Region * this_region = new_map->getRegion(new_region_row, new_region_col);
				//cout << "region found - " << new_map->getRegion(new_region_row, new_region_row)->getName() + " with id " << new_map->getRegion(new_region_row, new_region_row)->getID() << endl;


				//set links to neigbhbors for each direction
				if (east_neighbor)
				{
					this_region->setNeighbor(*new_map->getRegion(new_region_row, new_region_col + 1));
				}
				if (west_neighbor)
				{
					this_region->setNeighbor(*new_map->getRegion(new_region_row, new_region_col - 1));
				}
				if (north_neighbor)
				{
					this_region->setNeighbor(*new_map->getRegion(new_region_row - 1, new_region_col ));
				}
				if (south_neighbor)
				{
					this_region->setNeighbor(*new_map->getRegion(new_region_row + 1, new_region_col));
				}
			}

			++line_count;
			line.clear();
			parse.clear();
		}

		file.close();
	}
	else
	{
		cout << "Error: could not re-open file" << endl;
		return nullptr;
	}

	return new_map;
}
