#include "Map.h"

Map::Map(string name_in)
{
	name = name_in;
}

Map::Map(string name_in, vector<Region*> regions_in)
{
	name = name_in;
	regions = regions_in;
}

void Map::addRegion(Region& newRegion)
{
	if (!checkDuplicate(newRegion))
	{
		regions.push_back(&newRegion);
		++numRegions;
	}
	else
	{
		cout << "Map already contains a region by this name" << endl;
	}
}

void Map::addRegion(Region& newRegion, vector<Region*> newNeigbhors)
{
	if (!checkDuplicate(newRegion))
	{
		regions.push_back(&newRegion);
		++numRegions;

		for (vector<Region*>::iterator it = newNeigbhors.begin(); it != newNeigbhors.end(); ++it)
		{
			link(newRegion.getName(), (*it)->getName());
		}
	}
	else
	{
		cout << "Map already contains a region by this name" << endl;
	}
}

void Map::removeRegion(string name)
{
	Region* rmv = getRegion(name);
	vector<Region*>::iterator it;

	for (it = regions.begin(); it != regions.end(); ++it)
	{
		if ((*it)->getName() == rmv->getName())
		{
			regions.erase(it);
			--numRegions;
		}
	}
}

void Map::removeAllRegions()
{
//	vector<Region*>::iterator it;
	regions.clear();
	numRegions = 0;

}

bool Map::checkDuplicate(Region& region_in)
{
	for (vector<Region*>::iterator it = regions.begin(); it != regions.end(); ++it)
	{
		if (region_in.getName() == (*it)->getName())
		{
			return true;
		}
	}
	return false;
}

bool Map::checkForRegion(string region_name)
{
	Region* test_region = getRegion(region_name);
	if (test_region != nullptr)
	{
		return true;
	}

	return false;
}



Region * Map::getRegion(string search_name)
{
		vector<Region*>::iterator it;
		for (it = regions.begin(); it != regions.end(); ++it)
		{
			if (search_name.compare((**it).getName()) == 0)
			{
				return *it;
			}
		}

		return nullptr;
}

int Map::getNumRegions()
{
	return numRegions;
}

void Map::print()
{

	for (int i = 0; i < regions.size(); ++i)
	{
		cout << regions[i]->getName() << ":" << endl;
		cout << "   " << regions[i]->getNeighborNames() << endl;
		cout << "Controlled by " << regions[i]->getOwnerName() << " with " << regions[i]->getTroops() << " troops" << endl;
		cout << endl << endl;
	}
}

void Map::link(string region_1, string region_2)
{
	Region* firstRegion = getRegion(region_1);
	Region* secondRegion =getRegion(region_2);

	if (firstRegion == nullptr || secondRegion == nullptr)
	{
		cout << "Could not link regions, invalid name" << endl;
		return;
	}
	(*firstRegion).setNeighbor(*secondRegion);
}
