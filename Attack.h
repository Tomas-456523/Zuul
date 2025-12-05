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
	bool targetAlly = false;
	bool targetFainted = false;

	int minLevel; //must be at least this level to use the attack

	float lifesteal; //what % of damage dealt is stolen and added to the npc's health

	Effect* appliedeffect = NULL;

	//how many targets it hits (1, 3, or all)
	//min and max amount of times it hits

	Attack() {} //default constructor so NPC doesn't throw error

	Attack(const char _name[255], const char _description[255], int _cost, int _power, int _pierce, int _minhits, int _maxhits, int _targets, bool _targetAlly = false, int _minlevel = 0, float _lifesteal = 0) {
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
	}

	/*void addEffect(Effect effect) {
		applied_effects.push_back(effect);
	}*/

	void addDescription(const char desc[255]) {
		strcpy(trueDesc, desc);
	}
};

#endif // !ATTACK

/* everyone should have a basic attack
*  enemies should have 1-3 special attacks
* special attacks use sp (skill points) which regen a % of damage dealt from regular attacks
* ai always uses special attacks when it can, and chooses one randomly
* some attacks inflict status effects, if they already have the effect, it resets the duration
* 
* when applying effects, add a clone of it to the npc's effects vector, unless they already have the same name effect, then just update the duration to the maximum available
*/