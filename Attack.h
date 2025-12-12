//header file for attacks

#ifndef ATTACK
#define ATTACK

#include <vector>
#include <cstring>
#include "Effect.h"
using namespace std;

struct Attack {
	char name[255]; //the name of the attack and how you call it
	char description[255]; //describes how the attack was carried out, (e.g. "threw a shuriken")
	char trueDesc[255]; //actual description of the attack; only for player attacks
	int cost; //how much sp the attack costs to use
	int power; //how much damage the attack does, added onto npc's base attack
	int pierce; //how much defense the attack pierces through
	int targets; //how many targets the attack hits (1 hits just the target, 3 hits their surrounding allies, and 4 hits their entire team, and I don't really use 2)
	int minhits; //the attack can hit from minhits to maxhits amount of times
	int maxhits;
	bool targetAlly = false; //if the attack is supposed to hit your own team
	bool targetFainted = false; //if the attack is supposed to hit 0 hp npcs

	bool spbomb = false;
	bool burgertalk = false; //if it's the burger man talking
	bool burgerpunch = false; //if it's the burger man's attack

	int minLevel; //must be at least this level to use the attack

	float lifesteal; //what % of damage dealt is stolen and added to the npc's health

	Effect* appliedeffect = NULL; //targets get affected by this effetc when hit

	Attack() {} //default constructor so NPC doesn't throw error

	//constructs the attack
	//default stats are for testing purposes
	Attack(const char _name[255], const char _description[255], int _cost = 2, int _power = 20, int _pierce = 20, int _minhits = 1, int _maxhits = 1, int _targets = 1, bool _targetAlly = false, int _minlevel = 0, float _lifesteal = 0) {
		strcpy(name, _name);
		strcpy(description, _description);
		cost = _cost;
		power = _power;
		pierce = _pierce;
		targets = _targets;
		minhits = _minhits;
		maxhits = _maxhits;
		targetAlly = _targetAlly;
		minLevel = _minlevel;
		lifesteal = _lifesteal;
		if (!strcmp(name, "SP BOMB")) { //we check if it's this one move because of its unique functionality
			spbomb = true;
		}
		//Helper::attacksH.push_back(this); //store a pointer to this attack in the attacks vector
	}

	//I don't need these functions since everything's public in a struct but I like having functions, especially since I don't have to define them in seperate header and cpp files

	//sets an effect for the attack to apply on hit
	void addEffect(Effect* effect) {
		appliedeffect = effect;
	}

	//sets the true description of the attack, so that it can be seen in the attacks menu
	void addDescription(const char desc[255]) {
		strcpy(trueDesc, desc);
	}
};

#endif // !ATTACK