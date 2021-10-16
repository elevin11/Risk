#include "DisplayMap.h"

DisplayMap::DisplayMap(string name_in)
{
	name = name_in;
}

void DisplayMap::addRegion(Region& newRegion, int x_coord, int y_coord)
{
	if (checkDuplicate(newRegion))
	{
		
		cout << "Map already contains a region by this name" << endl;
		return;
	}
	else if( x_coord > regions.size() || x_coord < 0)
	{
		cout << "Invalid x coordinate" << endl;
		return;
	}
	else if (y_coord > regions[x_coord].size() || y_coord < 0)
	{
		cout << "Invalid y coordinate" << endl;
		return;
	}
	else
	{
		if (x_coord < regions.size() && y_coord < regions[x_coord].size()) //if x, y in existing rows/columns
		{
			regions[x_coord][y_coord] = &newRegion;
			++numRegions;
		}
		else if (x_coord < regions.size()) //if x is less than number of columns, add new row to existing x column
		{
			regions[x_coord].push_back(&newRegion);
			
		}
		else //if x is greater than number of columns, add new column with single row
		{
			vector<Region*> newColumn;
			newColumn.push_back(&newRegion);
			regions.push_back(newColumn);
		}

	}
}
