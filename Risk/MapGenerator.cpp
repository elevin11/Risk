#include "MapGenerator.h"

MapGenerator::MapGenerator(vector<string> region_names_in, vector<Player*> players_in)
{
	srand(time(nullptr));

	for (int i = 0; i < region_names_in.size(); ++i)
	{
		region_names.push_back(region_names_in[i]);
	}
	for (int i = 0; i < players_in.size(); ++i)
	{
		players.push_back(players_in[i]);
	}

}

DisplayMap * MapGenerator::generate(string name_in, int rows_in, int columns_in)
{
	cout << "DisplayMap called " << endl;

	if (region_names.empty())
	{
		cout << "ERROR: List of region names is empty" << endl;
		return nullptr;
	}
	else if (players.empty())
	{
		cout << "ERROR: List of players is empty" << endl;
		return nullptr;
	}


	DisplayMap* new_map = new DisplayMap(name_in, rows_in, columns_in);
	
	int num_tiles = rows_in * columns_in;
	int num_regions = (rand() % num_tiles) + 1; 
	
	for (int i = 0; i < num_regions; ++i)
	{
		int new_region_row = rand() % rows_in;
		int new_region_col = rand() % columns_in;

		//If region exists, reroll coordinates
		if (new_map->checkForRegion(new_region_row, new_region_col) )
		{
			i = i - 1;
			continue;
		}
		else //Else create new region and add it to map at given coordinates
		{
			//get regions name from list
			int name_idx = i % region_names.size();
			int repeat_times = (i - name_idx) / int(region_names.size());

			string name = region_names[name_idx]; //set region name

			// if multiple regions with same name, add number to end of it
			if (repeat_times > 0)
			{
				name += " " + to_string(repeat_times + 1);
			}

			Region* new_region = new Region(name, new_map->getNumRegions());
			new_map->addRegion(*new_region, new_region_row, new_region_col);
		}
	}

	//cycle through players and assign them regions just generated
	//add continents

	return new_map;
	
}

