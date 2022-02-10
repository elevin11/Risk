#pragma once
#include "Map.h"
#include "Player.h"
#include "Region.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <random>
#include <string>
#include <vector>
using namespace std;

class Game
{
public:
 //   Game() {};
 //   Game(int);
    Game(int players, int troops, Map & input_map); //number of players, number of starting  troops, map
    void turn(Player& player_in);
    void turnCycle();

    //PLAYER FUNCTIONS
    void addPlayer(string);
    void listPlayers();
    int getNumPlayers();
    void removePlayer(int player_id);
    Player* getPlayer(int player_id);
    bool checkForDefeat(Player& player_in);
    void checkDefeated();
    bool checkForVictory(Player& player_in);
    bool checkVictors();
    

    //REGION FUNCTIONS
    void claimRegions(); // players take turns claiming regions until all are assigned
    void assignRegion(Player &, Region&); //assigns given region to given player

    //DEPLOY FUNCTIONS
    void deployTroops(Player& player_in, Region& region_in, int troops_in);
    void deployPhase(Player& player_in, int new_troops);
    void deployCycle();
    int continentBonus(Player& player_in, Continent& cont_in);

    //ATTACK FUNCTIONS
    void attack(Player& attacker, Region& source, Region& target);
    void attackPhase(Player& player_in);
    void attackCycle();

    //MOVE FUNCTIONS
    void moveTroops(Region& source, Region& target, int troops);
    void fortifyPhase(Player& player);
    void fortifyCycle();
   
  

    //MAP FUNCTIONS
    int getNumRegions();//return number of regions in game map
    void removeRegion(string);
    void removeAllRegions();
    bool checkControlPath(Region& source, Region& target, Player& player);
    void findControlPath(Region& source, Region& target, Player& player, bool * visited);

    //MISC
    vector<string> getUserInput();
    struct compDeployable_LT
    {
        bool operator()( Player* lhs, Player* rhs) const
        {
            return lhs->getDeployable() < rhs->getDeployable();
        }
    };


private:
    int num_players = 0;
    int assigned_regions = 0;
    int starting_troops = 0;
    Map& map;
    vector<Player *> players;
    int current = 0;
    
};

