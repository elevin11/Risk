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
		++num_regions;
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
		++num_regions;

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
			--num_regions;
		}
	}
}

void Map::removeAllRegions()
{
//	vector<Region*>::iterator it;
	regions.clear();
	num_regions = 0;

}

bool Map::checkDuplicate(Region& region_in)
{
	cout << "Map::checkDuplicate called " << endl;
	cout << regions.size() << " regions in map" << endl;
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
	return num_regions;
}

void Map::changeNumRegions(int change)
{
	num_regions += change;
}

void Map::addContinent(string name_in, int val_in, vector<Region*> regions_in)
{
	//checks that all the given regions already belong in map
	vector<Region*>::iterator it;
	for (it = regions_in.begin(); it != regions_in.end(); ++it)
	{
		if (!checkDuplicate(**it))
		{
			cout << "Error: could not add continent. Region " << (*it)->getName() << " not part of map" << endl;
			return;
		}
	}

	Continent* cont_ptr = new Continent(name_in, val_in, regions_in);

	continents.push_back(cont_ptr);
}

int Map::getNumContinents()
{
	return continents.size();
}

Continent* Map::getContinent(string search_name)
{
	vector<Continent*>::iterator it;
	for (it = continents.begin(); it != continents.end(); ++it)
	{
		if (search_name.compare((**it).getName()) == 0)
		{
			return *it;
		}
	}

	return nullptr;
}

vector<Continent*> Map::getContinentList()
{
	return continents;
}

void Map::updateAllContinents()
{
	for (int i = 0; i < continents.size(); ++i)
	{
		bool update = continents[i]->updateOwner();
	}
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
