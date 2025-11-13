#ifndef ATTACK
#define ATTACK

#include <vector>
#include "Effect.h"

struct Attack() {
	char description[255]; //describes how the attack was carried out, (e.g. "threw a shuriken")
	int power; //how much damage the attack does, added onto npc's base attack
	int pierce; //how much defense the attack pierces through

	float lifesteal; //what % of damage dealt is stolen and added to the npc's health

	vector<Effect> applied_effects;

	//how many targets it hits (1, 3, or all)
	//min and max amount of times it hits

	Attack() {

	}
}

#endif // !ATTACK

/* everyone should have a basic attack
*  enemies should have 1-3 special attacks
* special attacks use sp (skill points) which regen a % of damage dealt from regular attacks
* ai always uses special attacks when it can, and chooses one randomly
* some attacks inflict status effects, if they already have the effect, it resets the duration
* 
* when applying effects, add a clone of it to the npc's effects vector, unless they already have the same name effect, then just update the duration to the maximum available