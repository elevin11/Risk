// Risk.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include <string>
#include "Continent.h"
#include "DisplayMap.h"
#include "Game.h"
#include "Map.h"
#include "MapGenerator.h"
#include "Region.h"
#include "Tile.h"
using namespace std;



void print_help()
{
	cout << "Commands: " << endl;
	cout << "new <REGION NAME> " << endl;
	cout << "print" << endl;
	cout << "add player <PLAYER_NAME>" << endl;
	cout << "check defeat / victory" << endl;
	cout << "begin" << endl;
	cout << "generate" << endl;
	cout << "help" << endl;
	cout << "end" << endl;
}



int main()
{
	vector<Region*> list;
	bool input = true;


	//create map for testing, code custom maps later
	DisplayMap midwest("Midwest");

	cout << "Welcome to Risk!" << endl;
	print_help();

	string buff;
	getline(cin, buff); //flushing input to make below work




	Region mich("michigan", 0);
	Region ohio("ohio", 1);
	Region wisc("wisconsin", 2);
	Region indi("indiana", 3);

	midwest.addRegion(mich, 0, 0);
	midwest.addRegion(ohio, 0, 1);
	midwest.addRegion(wisc, 1, 0);
	midwest.addRegion(indi, 1, 1);

	midwest.link("michigan", "ohio");
	midwest.link("michigan", "wisconsin");
	midwest.link("indiana", "ohio");
	midwest.link("indiana", "wisconsin");

	vector<Region*> cont_a;
	cont_a.push_back(&mich);
	cont_a.push_back(&indi);
	midwest.addContinent("Buttland", 10, cont_a);


	Game newGame(0, 1, midwest);

	newGame.addPlayer("eric");
	newGame.addPlayer("char");

	Player * eric_ptr = newGame.getPlayer(0);
	Player * char_ptr = newGame.getPlayer(1);

	midwest.getRegion("wisconsin")->setOwner(*eric_ptr);
	eric_ptr->incRegions();
	midwest.getRegion("ohio")->setOwner(*eric_ptr);
	eric_ptr->incRegions();

	midwest.getRegion("michigan")->setOwner(*char_ptr);
	char_ptr->incRegions();
	midwest.getRegion("indiana")->setOwner(*char_ptr);
	char_ptr->incRegions();

	eric_ptr->setDeployable(6);
	eric_ptr->setTroops(6);
	char_ptr->setDeployable(6);
	char_ptr->setTroops(6);

	newGame.deployTroops(*eric_ptr, wisc, 4);
	newGame.deployTroops(*eric_ptr, ohio, 2);
	newGame.deployTroops(*char_ptr, mich, 3);
	newGame.deployTroops(*char_ptr, indi, 3);





	while (input)
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

		if (parse.size() == 0)
		{
			continue;
		}

		if ( parse[0] == "new")
		{
			if (parse.size() > 1)
			{
				string newName = parse[1];
				int newID;
				int col_in;
				int row_in;

				if (parse.size() > 3)
				{
					try
					{
						row_in = stoi(parse[2]);
						col_in = stoi(parse[3]);
					}
					catch (exception& e)
					{
						cout << "invalid coordinates" << endl;
						continue;
					}
				}
				else
				{
					cout << "invalid number of arguments" << endl;
					continue;
				}

				newID = newGame.getNumRegions();

				Region* newRegion = new Region(newName, newID);

				midwest.addRegion(*newRegion, row_in, col_in);

			}


		}
	

		if (parse[0] == "print")
		{
			midwest.print();
			newGame.listPlayers();
		}


//ADD PLAYER
		if (parse.size() > 1 && parse[0] == "add" && parse[1] == "player")
		{
			string newPlayerName;
			if (parse.size() > 2)
			{
				newPlayerName = parse[2];
				
			}

			newGame.addPlayer(newPlayerName);
			cout << "Player " << newPlayerName << " added" << endl;
		}

		if (parse.size() > 1 && parse[0] == "delete" )
		{
			if(parse[1] == "all")
			{
				for (int i = newGame.getNumPlayers() - 1; i >= 0;  --i)
				{
					newGame.removePlayer(i);
				}
				newGame.removeAllRegions();
				

			}
			else
			{
				//delete by name
			}
		}

		
		if (parse[0] == "check")
		{
			if (parse.size() > 1)
			{
				if (parse[1] == "defeat")
				{
					newGame.checkDefeated();
				}
				else if (parse[1] == "victory")
				{
					bool vic = newGame.checkVictors();
					if (vic)
					{
						cout << "victor  exists" << endl;
					}
					else
					{
						cout << "no victor" << endl;
					}
				}
			}
		}

		if (parse[0] == "begin")
		{
			bool over = false;
			while (!over) //game loop while no victor exists
			{
				newGame.turnCycle();
				over = newGame.checkVictors();
				if (over)
				{
					cout << "FIRST CHECK FOR VICTORS PASSES" << endl;
				}
			}
			for (int i = 0; i < newGame.getNumPlayers(); ++i)
			{
				if (newGame.checkForVictory(*newGame.getPlayer(i)))
				{
					cout << newGame.getPlayer(i)->getName() << " is victorious! " << endl;
					break;
				}
			}
			cout << "GAME OVER" << endl << endl;
		}

		if (parse[0] == "generate")
		{
			if (parse.size() > 2)
			{
				int rows_in = stoi(parse[1]);
				int cols_in = stoi(parse[2]);

				vector<string> states;
				states.push_back("michigan");
				states.push_back("wisconsin");
				states.push_back("ohio");
				states.push_back("indiana");
				states.push_back("illinois");
				states.push_back("kentucky");
				states.push_back("missouri");

				vector<Player*> players_in_game;
				for (int i = 0; i < newGame.getNumPlayers(); ++i)
				{
					players_in_game.push_back(newGame.getPlayer(i));
				}

				MapGenerator generator(states, players_in_game);
				DisplayMap* random_map = generator.generate("My New Map", rows_in, cols_in);
				random_map->print();
			}
			else
			{
				cout << "Not enough arguments to generate map" << endl;
			}

		}



		if (parse[0] == "help")
		{
			print_help();
		}

		if (parse[0] == "end")
		{
			input = false;
		}

	}

	return 0;
}



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
