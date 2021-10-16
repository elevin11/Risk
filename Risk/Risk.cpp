// Risk.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include <string>
#include "Game.h"
#include "Region.h"
#include "Tile.h"
#include "Map.h"
#include "DisplayMap.h"
using namespace std;



void print_help()
{
	cout << "Commands: " << endl;
	cout << "new <REGION NAME> " << endl;
	cout << "find <REGION NAME>" << endl;
	cout << "link <REGION 1> <REGION 2>" << endl;
	cout << "print" << endl;
	cout << "help" << endl;
	cout << "claim" << endl;
	cout << "add player <PLAYER_NAME>" << endl;
	cout << "delete all" << endl;
	cout << "deploy" << endl;
	cout << "attack" << endl;
	cout << "fortify" << endl;
	cout << "check defeat / victory" << endl;
	cout << "begin" << endl;
	cout << "tile" << endl;
}



int main()
{
	vector<Region*> list;
	bool input = true;


	//create map for testing, code custom maps later
	Map midwest("Midwest");

	cout << "Welcome to Risk!" << endl;
	print_help();

	string buff;
	getline(cin, buff); //flushing input to make below work

	Game newGame(0, 1, midwest);

	newGame.addPlayer("eric");
	newGame.addPlayer("char");


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
				string newName;
				int newID;

				for (int i = 1; i < parse.size(); ++i)
				{
					newName = parse[i];
					newID = newGame.getNumRegions();

					Region* newRegion = new Region(newName, newID);

					midwest.addRegion(*newRegion);
				}

			}


		}


		if (parse[0] == "find")
		{
			if (parse.size() > 1)
			{
				if (midwest.getRegion(parse[1]) != nullptr)
				{
					cout << midwest.getRegion(parse[1])->getName() << endl;
				}
				else
				{
					cout << "Could not find " << parse[1] << endl;
				}
			}
		}
	

		if (parse[0] == "link")
		{
			if (parse.size() >= 3)
			{
				midwest.link(parse[1], parse[2]);

			}
		}

		if (parse[0] == "end")
		{
			input = false;
		}

		if (parse[0] == "print")
		{
			midwest.print();
			newGame.listPlayers();
		}

		if (parse[0] == "claim")
		{
			newGame.claimRegions();
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

		if (parse[0] == "deploy")
		{
			newGame.deployCycle();
		}

		if (parse[0] == "attack")
		{
			newGame.attackCycle();
		}

		if (parse[0] == "fortify")
		{
			newGame.fortifyCycle();
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

		if (parse[0] == "display_map")
		{
			DisplayMap d_map("disp");
			
		}


		if (parse[0] == "help")
		{
			print_help();
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
