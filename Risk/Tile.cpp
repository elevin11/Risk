#include "Tile.h"

Tile::Tile(string name_in, vector<Tile*>& neighbors_in, int id_in)
	:id(id_in)
{
	if (neighbors_in.size() != 4)
	{
		throw invalid_argument("invalid number of neighbors");
	}
	
	name = name_in;
	for (int i = 0; i < neighbors_in.size(); ++i)
	{
		neighbors.push_back(neighbors_in[i]);
	}

}
