#include "Region.h"
#include <iostream>
using namespace std;

Region::Region(string name, int id_in)
	: id(id_in)
{
	setName(name);
}

Region::Region(string name, vector<Region*>& neighbors_in, int id_in) 
	: id(id_in)
{
	setName(name);
	vector<Region*>::iterator it;
	for (it = neighbors_in.begin(); it != neighbors_in.end(); ++it)
	{
		setNeighbor(**it);
	}
}

Region::~Region()
{

}

string Region::getName()
{
	return name;
}

const int Region::getID()
{
	return id;
}

void Region::setName(string name_in)
{
	name = name_in;
}

void Region::setNeighbor(Region &new_neighbor)
{
	if (!isNeighbor(new_neighbor))
	{
	//	Region* nbr_ptr = &new_neighbor;
		neighbors.push_back(&new_neighbor);
		if (!new_neighbor.isNeighbor(*this))
		{
			//	Region* nbr_ptr = &new_neighbor;
			new_neighbor.setNeighbor(*this);
		}
	}

}

bool Region::isNeighbor(Region &region_input)
{
	vector<Region *>::iterator it = find(neighbors.begin(), neighbors.end(), &region_input);
	if (it != neighbors.end())
	{
		return true;
	}
	return false;
}

string Region::getNeighborNames()
{
	string neighbor_list;
	vector<Region*>::iterator it;
	for (it = neighbors.begin(); it != neighbors.end(); ++it)
	{
		neighbor_list.append( (*it)->getName() );
		neighbor_list.append(" ");
	}

	return neighbor_list;
}

vector<Region *> Region::getNeighbors()
{
	return neighbors;
	// TODO: insert return statement here
}

void Region::setOwner(Player& newOwner)
{
	owner = &newOwner;
}

void Region::setTroops(int troops_in)
{
	troops = troops_in;
}

void Region::incTroops(int new_troops)
{
	troops += new_troops;
}

void Region::decTroops(int troops_in)
{
	troops -= troops_in;
}

Player* Region::getOwner()
{
	return owner;
}

string Region::getOwnerName()
{
	if (owner != nullptr)
	{
		return owner->getName();
	}
	return "none";
}

int Region::getTroops()
{
	return troops;
}
