#include "Game.h"

//Game::Game(int players_in)
//{
//    num_players = players_in;
//};

Game::Game(int players_in, int troops_in, Map & map_in)
	: map(map_in)
{
	num_players = players_in;
 //   map = map_in;
	starting_troops = troops_in;

	
}


void Game::turn(Player& player)
{
	cout << player.getName() << "'s turn" << endl;
	//deploy
	int newTroops = player.getRegionsOwned() / 3;

	map.updateAllContinents();
	cout << player.getName() << " controls the following continents: " << endl;

	vector<Continent*> conts = map.getContinentList();
	vector<Continent*>::iterator it;
	for (it = conts.begin(); it != conts.end(); ++it)
	{
		if (&player == (*it)->getOwner())
		{
			cout << (*it)->getName() << " - " << (*it)->getValue() << " troops" << endl;
			newTroops += (*it)->getValue();
		}
	}


	if (newTroops < 3)
	{
		newTroops = 3;
	}

	cout << player.getName() << " receives " << newTroops << " new troops" << endl;
	deployPhase(player, newTroops);


	//attack
	attackPhase(player);

	//fortify
	fortifyPhase(player);
}

void Game::turnCycle()
{
	for (int i = 0; i < players.size(); ++i)
	{
		if (checkForDefeat(*players[i]))
		{
			continue;
		}
		else
		{
			turn(*players[i]);
			cout << endl;
		}
	}
}

void Game::addPlayer(string name_in)
{
	Player * newPlayer = new Player(name_in, starting_troops, num_players);
	cout << "player " << newPlayer->get_id() << " added" << endl;

	players.push_back(newPlayer);

	++num_players;

}

void Game::listPlayers()
{
	for (int i = 0; i < players.size(); ++i)
	{
		cout << "Player " << players[i]->get_id() << " - " << players[i]->getName() << endl;
		cout << "controls " << players[i]->getRegionsOwned() << " regions" << endl;
		cout << "commands " << players[i]->getTroops() << " troops" << endl;
		cout << "has " << players[i]->getDeployable() << " troops to deploy" << endl;
		cout << endl;
	}
}

int Game::getNumPlayers()
{
	return num_players;
}

void Game::removePlayer(int player_id)
{
	Player* rmv = players[player_id];
	players.erase(players.begin() + player_id);
	--num_players;
	if (player_id < current)
	{
		--current;
	}
	delete rmv;
}

Player* Game::getPlayer(int player_id)
{
	if (player_id < players.size())
	{
		return players[player_id];
	}
	return nullptr;
}

bool Game::checkForDefeat(Player& player_in)
{
	if (player_in.getRegionsOwned() == 0)
	{
		return true;
	}
	return false;
}

void Game::checkDefeated()
{
	for (int i = 0; i < players.size(); ++i)
	{
		if (checkForDefeat(*players[i]))
		{
			cout << players[i]->getName() << " defeated" << endl;
		}
	}
}

bool Game::checkForVictory(Player& player_in)
{
	for (int i = 0; i < players.size(); ++i)
	{
		if (*players[i] == player_in) //skip player checking for victory
		{
			continue;
		}
		if (!checkForDefeat(*players[i])) //if another player found
		{
			return false;
		}
	}
	return true;
}

bool Game::checkVictors()
{
	int i = 0;
	bool victor = false;
	while (!victor && i < players.size())
	{
		victor = checkForVictory(*players[i]);
		if (victor)
		{
			return true;
		}
		++i;
	}
	return false;
}

void Game::claimRegions()
{
	//print intro and list players
	cout << "CLAIM REGIONS" << endl;
	listPlayers();

	int numPlayers = players.size();

	if (numPlayers == 0)
	{
		cout << "ERROR: No players in game. Exiting claim sequence. " << endl << endl;
		return;
	}




	//sets troops for each player
	if (numPlayers > map.getNumRegions())
	{
		cout << map.getNumRegions() << " region in map" << endl;
		cout << "Too many players for this map " << endl << endl;
		return;
	}
	int unassigned = map.getNumRegions() - assigned_regions;

	int	min_troops = (unassigned + 1) / numPlayers; //each player needs this many troops for all regions to be assigned



	//find minimum deployable among players
	//if min deployable < min_troops, give each player min_troops

	int min_deployable;
	auto min = min_element(players.begin(), players.end(), compDeployable_LT());
	min_deployable = (*min)->getDeployable();



	if (min_deployable < min_troops)
	{
		cout << "Not enough starting troops. All players given " << min_troops << " more troops." << endl;
		for (int i = 0; i < numPlayers; ++i)
		{
			players[i]->recruit(min_troops);
		}
	}

	cout << "Each player starts with " << starting_troops << " troops." << endl << endl;

	for (int i = 0; i < numPlayers; ++i)
	{
		cout << players[i]->getName() << " troops = " << players[i]->getTroops() << " deployable = " << players[i]->getDeployable() << endl;
	}
	cout << endl;
 
  

	//players take turns selecting a region until all regions have been assigned
	while (assigned_regions < map.getNumRegions())
	{

		Player* currentPlayer = players[current];

		
		Region* chosenRegion = nullptr;
		
		cout << "Player " << currentPlayer->get_id() << " choose a region" << endl;
		cout << assigned_regions << " of " << map.getNumRegions() << " chosen" << endl;




		while( chosenRegion == nullptr)
		{

			vector<string> parse = getUserInput();

			//check for empty input string
			if (parse.size() < 1)
			{
				continue;
			}

			string regionName = parse[0];
			

			
			
			//BUG: checkForRegion returns true, but chosenRegion still gets assigned nullptr
			if (map.checkForRegion(regionName)) //true if chosenRegion != nullptr
			{
				chosenRegion = map.getRegion(regionName);	
			}
			else
			{ 
				cout << "Region name = " << regionName << endl;
				cout << map.getNumRegions() << endl;
				cout << regionName.size() << endl;

				cout << "please enter a valid region name" << endl;
				cout << regionName << " is not valid" << endl;
				chosenRegion = nullptr;
				continue;
			}


			if (chosenRegion->getOwner() != nullptr) //true if chosenRegion has an owner
			{
				cout << "region already controlled by " << chosenRegion->getOwnerName() << endl;
				chosenRegion = nullptr;
			}


		}



		assignRegion(*currentPlayer, *chosenRegion);    
		cout << chosenRegion->getOwnerName() << " controls " << chosenRegion->getName() << endl;

		++current;
		if (current >= players.size())
		{
			current = 0;
		}

		cout << endl;

	}
	
	cout << "ALL REGIONS CLAIMED" << endl;
}

void Game::assignRegion(Player& player, Region& region)
{
	if (region.getOwner() != nullptr)
	{
		cout << "Region already assigned" << endl;
	}
	else if (player.getDeployable() <= 0)
	{
		cout << "Cannot assign " << region.getName() << ", " << player.getName() << " does not have  enough troops." << endl;
	}
	else
	{
		region.setOwner(player);
		deployTroops(player, region, 1);

		++assigned_regions;
		player.incRegions();
	}
}

void Game::deployTroops(Player& player, Region& region, int troops)
{
	if ( region.getOwner() == nullptr || !(player == *(region.getOwner()) ) )
	{
		cout << player.getName() << " does not control " << region.getName() << endl;
	}

	if (troops > player.getDeployable())
	{
		cout << player.getName() << " does not have enough troops " << endl;		
	}
	if (region.getOwner() == nullptr || !(player == *region.getOwner()) || troops > player.getDeployable())
	{
		return;
	}

	region.setTroops(region.getTroops() + troops);
	player.setDeployable(player.getDeployable() - troops);
	cout << troops << " troops deployed to " << region.getName() << endl;
	
}

void Game::deployPhase(Player& player, int new_troops)
{
	player.recruit(new_troops);
	int to_deploy = player.getDeployable();


	cout << player.getName() << " deploy phase" << endl;
	cout << player.getDeployable() << " troops to deploy" << endl;
	cout << "Enter region and number of troops to deploy there" << endl << endl;

	while (to_deploy > 0)
	{
		vector<string> parse = getUserInput();

		//check for empty input string
		if (parse.size() < 1)
		{
			continue;
		}

		string regionName = parse[0];
		int deployNum = 0;

		if (parse[0] == "exit")
		{
			return;
		}
		else if (parse[0] == "print")
		{
			map.print();
			listPlayers();
			continue;
		}
		if (map.getRegion(regionName) == nullptr)
		{
			cout << "invalid region name " << endl;
			continue;
		}

		try
		{
			deployNum = stoi(parse[1]);
		}
		catch (exception& e)
		{
			cout << "invalid number of troops" << endl;
			continue;
		}

		Region* targetRegion = map.getRegion(regionName);

		if (deployNum > 0 && targetRegion != nullptr)
		{
			deployTroops(player, *targetRegion, deployNum);
			to_deploy = player.getDeployable();
		}
		if (targetRegion == nullptr)
		{
			cout << "Invalid target region" << endl;
		}
	}

	cout << player.getName() << " deploy phase complete." << endl << endl;
}

void Game::deployCycle()
{
	for (int i = 0; i < players.size(); ++i)
	{
		if (checkForDefeat(*players[i]))
		{
			continue;
		}
		Player* currentPlayer = players[i];

		int newTroops = currentPlayer->getRegionsOwned() / 3;
		if (newTroops < 3)
		{
			newTroops = 3;
		}

		cout << currentPlayer->getName() << " receives " << newTroops << " new troops" << endl;
		deployPhase(*currentPlayer, newTroops);
	}
}

void Game::attack(Player& attacker, Region& source, Region& target)
{
	//ERROR CHECKING
	if (*(target.getOwner()) == attacker)
	{
		cout << "Cannot attack your own region" << endl;
		return;
	}
	if (source.getTroops() < 2)
	{
		cout << "Not enough troops in " << source.getName() << " to attack." << endl;
		return;
	}
	if ( !( *(source.getOwner()) == attacker ) ) //check if attacker doesn't control source region
	{
		cout << "Do not control " << source.getName() << endl;
		return;
	}
	if ( !source.isNeighbor(target) ) //check if regions are not linked
	{
		cout << source.getName() << " does not neighbor " << target.getName() << endl;
		return;
	}
	int attack_total = source.getTroops() - 1;
	int defense_total = target.getTroops();

	cout << attacker.getName() << " attacks " << target.getName() << "(" << target.getOwnerName() << ")" << " with " << attack_total << " troops from " << source.getName() << endl;

	source.setTroops(1);

	int attack = 0;
	int defense = 0;
	int contact = 0;

	while (attack_total > 0 && defense_total > 0)
	{
		
		cout << " Contact " << contact << endl;
		cout << attacker.getName() << "(A) has " << attack_total << " troops" << endl;
		cout << target.getOwnerName() << "(D) has " << defense_total << " troops" << endl;

		if (attack_total >= 3)
		{
			attack = 3;
		}
		else
		{
			attack = attack_total;
		}

		if (defense_total >= 2)
		{
			defense = 2;
		}
		else
		{
			defense = defense_total;
		}

		//take troops for dice roll out of battle total
		attack_total -= attack;
		defense_total -= defense;

		//generate dice rolls
		vector<int> attack_dice;
		vector<int> defense_dice;

		random_device dev;
		mt19937 rng(dev());
		uniform_int_distribution<std::mt19937::result_type> dist6(1, 6);

		for (int i = 0; i < attack; ++i)
		{
			attack_dice.push_back(dist6(rng));
		}
		for (int i = 0; i < defense; ++i)
		{
			defense_dice.push_back(dist6(rng));
		}
		
		//sorts attack, defense dice rolls highest first
		sort(attack_dice.begin(), attack_dice.end(), greater<int>() );
		sort(defense_dice.begin(), defense_dice.end(), greater<int>() );

		cout << "A: ";
		for (int i = 0; i < attack_dice.size(); ++i)
		{
			cout << attack_dice[i] << " ";
		}
		cout << endl;
		cout << "D: ";
		for (int i = 0; i < defense_dice.size(); ++i)
		{
			cout << defense_dice[i] << " ";
		}
		cout << endl;

		//compare succesfull dice rolls and subtract troops from attack/defense
		int dice_comps = min(attack, defense);
		int a_killed = 0;
		int d_killed = 0;

		for (int i = 0; i < dice_comps; ++i)
		{
			if (attack_dice[i] > defense_dice[i])
			{
				--defense;
				++d_killed;
			}
			else
			{
				--attack;
				++a_killed;
			}
		}

		cout << dice_comps << " dice comparisons. " << a_killed << " attacking troops killed, " << d_killed << " defending troops killed. " << endl;

		//update troop totals
		int a_troops_update = attacker.getTroops() - a_killed;
		int d_troops_update = target.getOwner()->getTroops() - d_killed;
		attacker.setTroops(a_troops_update);
		target.getOwner()->setTroops(d_troops_update);

		//add surviving troops from dice rolls back to battle totals
		attack_total += attack;
		defense_total += defense;

		++contact;

		cout << endl;
	}

	//if attacker has surviving troops, they take the territory
	//attack_total > 0 implies attacker won last dice roll. 
	if (attack_total > 0)
	{
		target.setTroops(attack); //troops to occupy target
		attack_total -= attack;

		cout << attack << " troops occupy " << target.getName() << " after successful attack." << endl;
		cout << "How many troops to move into " << target.getName() << "? Maximum " << attack_total << endl;

		int troop_movement = 0;
		int troopsToMove = 0;

		bool done = false;
		while (!done)
		{
			vector<string> parse = getUserInput();


			try
			{
				troop_movement = stoi(parse[0]);
			}
			catch (exception& e)
			{
				cout << "invalid number of troops" << endl;
				continue;
			}

			if (troop_movement > attack_total || troop_movement < 0)
			{
				cout << "Please enter a number between 0 and  " << attack_total << endl;
				continue;
			}

			done = true;
		}

		
		int troop_remain = attack_total - troop_movement;

		attacker.incRegions();
		target.getOwner()->decRegions();

		target.incTroops(troop_movement);
		source.incTroops(troop_remain);
		target.setOwner(attacker);


		cout << troop_movement << " troops moved from " << source.getName() << " to " << target.getName() << endl;
		cout << troop_remain + 1 << " troops remain in " << source.getName() << ". " << endl;
		cout << attacker.getName() << " now controls " << target.getName() << ". " << endl;
	}
	else if (defense_total > 0)
	{
		target.setTroops(defense_total); //remaining defensive troops in target
		cout << target.getOwnerName() << " successfully defends " << target.getName() << " from " << attacker.getName() << endl;
	}
	else
	{
		cout << "ERROR: Both players lost all troops" << endl;
	}



}

void Game::attackPhase(Player& player_in)
{

	cout << "Player " << player_in.get_id() << " attack phase." << endl;
	cout << "Enter the name of the source and target region for your attack. When finished, enter \"done\" " << endl;
	bool done = false;

	while (!done)
	{
		vector<string> parse = getUserInput();

		
		if (parse.size() < 1)//check for empty input string
		{
			continue;
		}
		else if (parse[0] == "done")//
		{
			done = true;
		}
		else if (parse[0] == "print")
		{
			map.print();
			//listPlayers();
		}
		else if (parse.size() < 2)
		{
			cout << "ERROR: not enough arguments" << endl;
		}
		else
		{
			string src_name = parse[0];
			string tgt_name = parse[1];

			if (map.getRegion(src_name) == nullptr || map.getRegion(tgt_name) == nullptr)
			{
				cout << "invalid region name(s)" << endl;
			}
			else
			{
				Region* src = map.getRegion(src_name);
				Region* tgt = map.getRegion(tgt_name);

				attack(player_in, *src, *tgt);
			}
		}

	}	
}

void Game::attackCycle()
{
	cout << "BEGIN ATTACK CYCLE" << endl << endl;
	for (int i = 0; i < players.size(); ++i)
	{
		if (checkForDefeat(*players[i]))
		{
			continue;
		}
		cout << players[i]->getName() << "'s attack  phase. " << endl;
		attackPhase(*players[i]);
		cout << endl;
	}

	cout << "END ATTACK CYCLE" << endl;
}

void Game::moveTroops(Region& source, Region& target, int troops)
{
	if (troops > source.getTroops()  ) 
	{
		cout << "Not enough troops in " << source.getName() << endl;
		return;
	}

	source.decTroops(troops);
	target.incTroops(troops);

	cout << troops << " troops moved from " << source.getName() << " to " << target.getName() << endl;
}

void Game::fortifyPhase(Player& player)
{
	cout << player.getName() << "'s fortification phase" << endl;
	cout << "Enter source region, target region, and number of troops to move" << endl;
	cout << "Enter \"done\" to skip this phase." << endl;

	bool done = false;

	while (!done)
	{
		vector<string> parse = getUserInput();

		if (parse.size() < 1)//check for empty input string
		{
			continue;
		}
		else if (parse[0] == "done")//
		{
			done = true;
		}
		else if (parse[0] == "print")
		{
			map.print();
			//listPlayers();
		}
		else if (parse.size() < 3)
		{
			cout << "ERROR: not enough arguments" << endl;
		}
		else
		{
			string src_name = parse[0];
			string tgt_name = parse[1];

			int troopsToMove = 0;
			try
			{
				troopsToMove = stoi(parse[2]);
			}
			catch(exception &e)
			{ 
				cout << "invalid number of troops" << endl;
				continue;
			}
			

			if (map.getRegion(src_name) == nullptr || map.getRegion(tgt_name) == nullptr)
			{
				cout << "invalid region name(s)" << endl;
			}
			else if( troopsToMove < 0)
			{
				cout << "invalid number of troops" << endl;
			}
			else
			{
				Region* src = map.getRegion(src_name);
				Region* tgt = map.getRegion(tgt_name);

				if( checkControlPath(*src, *tgt, player))
				{
					if (troopsToMove < src->getTroops()) //strictly less to leave 1 troop in src
					{
						moveTroops(*src, *tgt, troopsToMove);
						done = true;
					}
					else
					{
						cout << "Not enough troops in " << src->getName() << endl;
					}

				}
				else
				{
					cout << "No path of territories under your control from " << src->getName() << " to " << tgt->getName() << endl;
				}

			}
		}
	}
}

void Game::fortifyCycle()
{
	cout << "BEGIN FORTIFY CYCLE" << endl << endl;

	for (int i = 0; i < players.size(); ++i)
	{
		if (checkForDefeat(*players[i]))
		{
			continue;
		}

		fortifyPhase(*players[i]);
		cout << endl;
	}

	cout << "END FORTIFY CYCLE" << endl;
}

int Game::getNumRegions()
{
	return map.getNumRegions();
}

void Game::removeRegion(string name)
{
	map.removeRegion(name);
	Region* region = map.getRegion(name);
	if (region != nullptr && region->getOwner() != nullptr)
	{
		--assigned_regions;
	}
}

void Game::removeAllRegions()
{
	map.removeAllRegions();
	assigned_regions = 0;
}

bool Game::checkControlPath(Region& source, Region& target, Player& player)
{
	int num_regions = map.getNumRegions();
	bool* visited = new bool[num_regions];

	for (int i = 0; i < num_regions; ++i)
	{
		visited[i] = false;
	}

	findControlPath(source, target, player, visited);

	return visited[target.getID()];
}

void Game::findControlPath(Region& source, Region& target, Player& player, bool* visited)
{
	if ( !( *(source.getOwner()) == player) ) //if player does not control source return withohut marking visited
	{
		return;
	}	

	visited[source.getID()] = true; //mark source as visited
	if (source.getID() == target.getID()) //if source == target then path exists
	{
		return;
	}

	vector<Region*> adjacent = source.getNeighbors(); //make list of neighbors

	for (int i = 0; i < adjacent.size(); ++i) //iterate through neighbors
	{	
		if (visited[adjacent[i]->getID()] == false) //if neighbor hasn't been visited
		{
			//visited[adjacent[i]->getID()] == true;
			findControlPath(*adjacent[i], target, player, visited); //recursive call on neighbor
		}
	}
	return;
}

vector<string> Game::getUserInput()
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

