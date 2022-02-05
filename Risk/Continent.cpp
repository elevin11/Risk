#include "Continent.h"

Continent::Continent(string name_in, int value_in, vector<Region*> members_in)
{
	name = name_in;
	value = value_in;
	vector<Region*>::iterator it;
	for (it = members_in.begin(); it != members_in.end(); ++it)
	{
		regions.push_back(*(it));
	}
}

bool Continent::updateOwner()
{
	if (regions.empty())
	{
		return false;
	}

	Player* test_owner = regions[0]->getOwner(); //set potential new owner
	bool same = true; //will be set to false if one of the continents regions has a different owner

	vector<Region*>::iterator it;
	for (it = regions.begin(); it != regions.end(); ++it) //check if any regions in continent have differnt owner
	{
		if ((*it)->getOwner() != test_owner)
		{
			same = false; 
			break;
		}
	}
	if (same) //if all regions have same owner as first one, set owner of whole continent
	{
		owner = test_owner;
	}
	return same;
}

Player* Continent::getOwner()
{
	return owner;
}

int Continent::getValue()
{
	return value;
}
