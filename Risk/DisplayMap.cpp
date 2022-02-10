#include "DisplayMap.h"

DisplayMap::DisplayMap(string name_in)
{
	name = name_in;

	vector<Region*> newColumn;
	Region* newRegion;

	newRegion = new Region("-", -1);
	newColumn.push_back(newRegion);

	regions.push_back(newColumn);
	maxRows = 1;

}

DisplayMap::DisplayMap(string name_in, int rows_in, int columns_in)
{
	name = name_in;
	if (rows_in < 0)
		rows_in = 1;
	if (columns_in < 0)
		columns_in = 1;

	for (int i = 0; i < columns_in; ++i)
	{
		vector<Region*> newColumn;
		Region* newRegion;
		for (int j = 0; j < rows_in; ++j)
		{
			newRegion = new Region("-", -1);
			newColumn.push_back(newRegion);
		}
		regions.push_back(newColumn);
		//check and set max row height if necessary
		if (newColumn.size() > maxRows)
		{
			maxRows = newColumn.size();
		}
	}
}

void DisplayMap::addRegion(Region& newRegion, int row, int col)
{
	if (col >= regions.size()) //create new column for region
	{
		int colsToAdd = col - regions.size() + 1; //counts number of columns to be added

		for (int i = 0; i < colsToAdd; ++i) //fills in empty columns
		{
			vector<Region*> newColumn;
			Region* newRegion;

			for (int j = 0; j < maxRows; ++j)
			{
				newRegion = new Region("-", -1);
				newColumn.push_back(newRegion);
			}
			regions.push_back(newColumn);

			//check and set max row height if necessary
			if (newColumn.size() > maxRows)
			{
				maxRows = newColumn.size();
			}
		}

	}

	if (row >= maxRows) //create new row for region
	{
		int rowsToAdd = row - regions[0].size() + 1; //number of rows to be added
		Region* newRegion;

		for (int i = 0; i < regions.size(); ++i) //iterates through columns and resizes with blank tiles
		{
			for (int j = 0; j < rowsToAdd; ++j) 
			{
				newRegion = new Region("-", -1);
				regions[i].push_back(newRegion);
			}
		}

		maxRows = row;
	}

	//has already been resized with blank tiles, now add new region
	if (regions[col][row]->getID() == -1) //check if tile is blank - only increase region count if not overriding existing region
	{
		changeNumRegions(1);
	}
	regions[col][row] = &newRegion;
}



Region* DisplayMap::getRegion(string search_name)
{
	int x;
	int y;

	for (x = 0; x < regions.size(); ++x)
	{
		for (y = 0; y < regions[x].size(); ++y)
		{
			if (search_name.compare( regions[x][y]->getName() ) == 0)
			{
				return regions[x][y];
			}
		}
	}

	return nullptr;
}

void DisplayMap::print()
{

	int x = 0;
	int y = 0;

	auto max = max_element(regions.begin(), regions.end(), compSize_LT());
	int max_col = (*max).size();

	cout << name << ": " << endl;
	cout << getNumRegions() << " regions in map" << endl;

	//prints out column numbers
	cout << "    ";
	for (int i = 0; i < regions.size(); ++i)
	{
		cout << i;
		if (i < 10)
		{
			for (int j = 0; j < maxName - 1; ++j)
			{
				cout << " ";
			}
		}
		else if (i < 100)
		{
			for (int j = 0; j < maxName - 2; ++j)
			{
				cout << " ";
			}
		}
		else
		{
			for (int j = 0; j < maxName - 3; ++j)
			{
				cout << " ";
			}
		}
	}


	for (y = 0; y < maxRows; ++y) //this should be main loop - each iteration corresponds to a row of tiles
	{
		cout << endl;
		cout << y << "   ";
		for (x = 0; x < regions.size(); ++x) //print out region names
		{
			
			if (y < regions[x].size())
			{
				//prints out 16 chars - region name + space for remainder
				cout << regions[x][y]->getName();
				int name_length = regions[x][y]->getName().length();
				for (int i = 0; i < maxName - name_length; ++i)
				{
					cout << " ";
				}
			}
			else
			{
				cout << "-";
				for (int i = 0; i < maxName - 1; ++i)
				{
					cout << " ";
				}
			}
		}
		cout << endl;
		cout << " " << "   ";
		for (x = 0; x < regions.size(); ++x) //print out owner names
		{
			if (y < regions[x].size())
			{
				if (regions[x][y]->getID() == -1) //checks for blank tile
				{
					cout << "-";
					for (int i = 0; i < maxName - 1; ++i)
					{
						cout << " ";
					}
				}
				else
				{
					//prints out 16 chars - region name + space for remainder or dashes if linked
					cout << regions[x][y]->getOwnerName();
					int name_length = regions[x][y]->getOwnerName().length();

					if (x < regions.size() - 1)
					{
						if (regions[x][y]->isNeighbor(*regions[x + 1][y] )) 
						{
							cout << " ";
							for (int i = 1; i < maxName - name_length - 1; ++i)
							{
								cout << "-";
							}
							cout << " ";
						}
						else
						{
							for (int i = 0; i < maxName - name_length; ++i)
							{
								cout << " ";
							}
						}
					}




				 }			
			}
			else
			{
				cout << "-";
				for (int i = 0; i < maxName - 1; ++i)
				{
					cout << " ";
				}
			}
		}
		cout << endl;

		cout << " " << "   ";
		for (x = 0; x < regions.size(); ++x) //print out number of troops
		{
			if (y < regions[x].size())
			{
				if (regions[x][y]->getID() == -1) //checks for blank tile
				{
					cout << "-";
					for (int i = 0; i < maxName - 1; ++i)
					{
						cout << " ";
					}
				}
				else
				{
					//prints out 16 chars - region name + space for remainder
					cout << regions[x][y]->getTroops();
					int name_length = 0;
					if (0 <= regions[x][y]->getTroops() < 10)
					{
						name_length = 1;
					}
					else if (10 <= regions[x][y]->getTroops() < 100)
					{
						name_length = 2;
					}
					else
					{
						name_length = 3;
					}
					for (int i = 0; i < maxName - name_length; ++i)
					{
						cout << " ";
					}
				}
			}
			else
			{
				cout << "-";
				for (int i = 0; i < maxName - 1; ++i)
				{
					cout << " ";
				}
			}
		}
		cout << endl;
		cout << " " << "   ";
		for (x = 0; x < regions.size(); ++x) //print out links
		{
			if (y < regions[x].size())
			{
				if (y < regions[x].size() - 1)
				{
					if (regions[x][y]->isNeighbor(*regions[x][y + 1]))
					{
						cout << "|";
						for (int i = 0; i < maxName - 1; ++i)
						{
							cout << " ";
						}
					}
					else
					{
						for (int i = 0; i < maxName; ++i)
						{
							cout << " ";
						}
					}

				}


			}
			else
			{
				cout << "-";
				for (int i = 0; i < maxName - 1; ++i)
				{
					cout << " ";
				}
			}
		}
	}
	cout << endl;

	cout << "Continents: " << endl;
	for (int i = 0; i < getNumContinents(); ++i)
	{
		cout << getContinentList()[i]->getName() << " - " << getContinentList()[i]->getValue() << " - ";
		if (getContinentList()[i]->getOwner() != nullptr)
		{
			cout << getContinentList()[i]->getOwner()->getName() << endl;
		}
		else
		{
			cout << "none" << endl;
		}

	}

	cout << endl;
}

bool DisplayMap::checkForRegion(string region_name)
{
	Region* test_region = getRegion(region_name);
	if (test_region != nullptr)
	{
		return true;
	}

	return false;
}

bool DisplayMap::checkForRegion(int row, int col)
{
	Region* test_region = nullptr;
	if (col >= regions.size() || row > maxRows) 
	{
		return false;
	}
	else
	{
		test_region = regions[col][row];
		if (test_region->getID() != -1)
		{
			return true;
		}
	}
	return false;
}

bool DisplayMap::checkDuplicate(Region& region_in)
{
	for (int i = 0; i < regions.size(); ++i)
	{
		for (int j = 0; j < regions[i].size(); ++j)
		{
			if (regions[i][j]->getName() == region_in.getName())
			{
				return true;
			}
		}
	}
	return false;
}

void DisplayMap::link(string region_1, string region_2)
{
	Region* firstRegion = getRegion(region_1);
	Region* secondRegion = getRegion(region_2);

	if (firstRegion == nullptr || secondRegion == nullptr)
	{
		cout << "Could not link regions, invalid name" << endl;
		return;
	}
	
	pair<int, int> coord_1 = getCoordinates(region_1);
	pair<int, int> coord_2 = getCoordinates(region_2);

	int x_dif = abs(coord_1.first - coord_2.first);
	int y_dif = abs(coord_1.second - coord_2.second);

	if ( (x_dif == 1 && y_dif == 0 )|| (y_dif == 1 && x_dif == 0) )
	{
		(*firstRegion).setNeighbor(*secondRegion);
	}
	else
	{
		cout << "cannot link, regions not next to each other" << endl;
		return;
	}
	
	
}

pair<int, int> DisplayMap::getCoordinates(string region_in)
{
	int x;
	int y;

	for (x = 0; x < regions.size(); ++x)
	{
		for (y = 0; y < regions[x].size(); ++y)
		{
			if (region_in == regions[x][y]->getName())
			{
				 return pair<int, int>(x, y);
			}
		}
	}
	
	cout << "region not found" << endl;
	return pair<int, int>(-1, -1);
}
