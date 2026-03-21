//header file for attacks

#ifndef ATTACK
#define ATTACK

#include <vector>
#include <cstring>
#include "Effect.h"
using namespace std;

class NPC;

struct Attack; //forward declare itself so it can forward declare attacksH
namespace Helper { //forward declare Helper to add attacks to attacksH
	extern vector<Attack*> attacksH;
}

struct Attack {
	const char* name; //the name of the attack and how you call it
	const char* description; //describes how the attack was carried out, (e.g. "threw a shuriken at")
	const char* afterdesc = ""; //description after the attack (e.g. threw a shuriken at BOB" with expert precision")
	const char* trueDesc; //actual description of the attack; only for player attacks
	int cost; //how much sp the attack costs to use
	int power; //how much damage the attack does, added onto npc's base attack
	int pierce; //how much defense the attack pierces through
	int targets; //how many targets the attack hits (1 hits just the target, 3 hits their surrounding allies, and 4 hits their entire team, and I don't really use 2)
	int minhits; //the attack can hit from minhits to maxhits amount of times
	int maxhits;
	bool targetAlly = false; //if the attack is supposed to hit your own team
	bool targetFainted = false; //if the attack is supposed to hit 0 hp npcs
	bool instakill = false; //if it instantly defeats non-boss npcs
	bool focushits = true; //if every hit of multihit moves should hit the same target, as opposed to random targets each time
	int recoil = 0; //how much damage the user takes (calculated later using their own attack stat)
	int guardset = 0; //set guard on the attacker

	bool spbomb = false;
	bool protect = false; //if this is the move that makes the mover start defending the target

	int minLevel; //must be at least this level to use the attack

	float lifesteal; //what % of damage dealt is stolen and added to the npc's health
	int spleak; //how much sp the attack removes from the target

	Effect* appliedeffect = NULL; //targets get affected by this effect when hit
	Effect* selfeffect = NULL; //attacker gets affected by this effect after using
	Effect* synergy = NULL; //if the target has this effect, the attack doubles in power
	Effect* cancel = NULL; //attack removes this effect

	Attack* recoilatt = NULL;
	float recoilchance = 0;

	NPC* summon = NULL; //the npc that the attack summons
	int summonamount = 0; //how many of the summon to summon

	bool contact; //if the attacker makes contact with the target
	bool beneficial = false; //if you want to get hit by this

	bool risky = false; //if we shouldn't do this move to a teammate below half health

	Attack() {} //default constructor so NPC doesn't throw error

	//constructs the attack
	//default stats are for testing purposes
	Attack(const char* _name, const char* _description, bool _contact, int _cost = 2, int _power = 20, int _pierce = 20, int _minhits = 1, int _maxhits = 1, int _targets = 1, bool _targetAlly = false, int _minlevel = 0, int _spleak = 0, float _lifesteal = 0) {
		name = _name;
		description = _description;
		contact = _contact;
		cost = _cost;
		power = _power;
		pierce = _pierce;
		targets = _targets;
		minhits = _minhits;
		maxhits = _maxhits;
		targetAlly = _targetAlly;
		minLevel = _minlevel;
		spleak = _spleak;
		lifesteal = _lifesteal;
		Helper::attacksH.push_back(this); //store a pointer to this attack in the attacks vector
	}

	//I don't need these functions since everything's public in a struct but I like having functions, especially since I don't have to define them in seperate header and cpp files

	//sets an effect for the attack to apply on hit
	void addEffect(Effect* effect) {
		appliedeffect = effect;
	}

	//sets the true description of the attack, so that it can be seen in the attacks menu
	void addDescription(const char* desc) {
		trueDesc = desc;
	}
};

#endif // !ATTACK