#include "Player.h"

Player::Player(string name_in)
{
	name = name_in;
}

Player::Player(string name_in, int troops_in, int id_in) : id(id_in)
{
	name = name_in;
	troops = troops_in;
	deployable = troops_in;
}

bool Player::operator==( const Player& otherPlayer) const
{
	if (otherPlayer.get_id() == id)
	{
		return true;
	}
	
	return false;
}

int Player::get_id() const
{
	return id;
}

string Player::getName()
{
	return name;
}

int Player::getRegionsOwned()
{
	return regions_owned;
}

int Player::getTroops()
{
	return troops;
}

void Player::incRegions()
{
	++regions_owned;
}

void Player::decRegions()
{
	--regions_owned;
}

int Player::getDeployable()
{
	return deployable;
}

void Player::setDeployable(int troops_in)
{

	deployable = troops_in;
}

void Player::setTroops(int troops_in)
{
	troops = troops_in;
}

void Player::recruit(int troops_in)
{
	deployable += troops_in;
	troops += troops_in;
}
