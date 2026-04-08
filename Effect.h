//header file for effects

#ifndef EFFECT
#define EFFECT

#include <cstring>
#include <vector>

class NPC;

struct Effect; //forward declare itself so it can forward declare attacksH
namespace Helper { //forward declare Helper to add effects to effectsH
	extern std::vector<Effect*> effectsH;
}

struct Effect {
	const char* name; //the name of the effect, (e.g. "POISON")
	int duration; //how long the effect lasts

	int damage; //how much damage the effect does per tick
	int spleak; //how much sp this effect leaks per tick

	//buffs (multiplicative)
	float attackbuff;
	float defensebuff;
	float toughbuff;
	float piercebuff;
	float spusage = 1; //multiplies sp needed to use moves
	float damagebuff = 1; //buffs overall damage TAKEN, so we take that much more
	float speedbuff = 1;

	bool freeze = false; //if the effect causes the target to get frozen
	bool hypnotize = false; //if the effect causes the target to get hypnotized (attack own party)
	bool remove = false; //if the attack removes the target from the fight temporarily
	bool tiring = false; //if the effect makes the affected tired and not be able to move (to differentiate from frozen pretty much)
	int falldamage = 0; //if remove == true, how much damage the affected npc takes on return
	bool spreadfalldamage = false; //if fall damage should affect surroundings

	int guardset = 0; //if this effect sets guard on the target

	NPC* npc = NULL; //the affected npc, used by instances

	float lifesteal = 0; //what % of damage taken is given to the attacker
	float spsteal = 0; //what % of sp taken is given to the attacker
	NPC* absorber = NULL; //who takes the lifestolen hp
	bool turntick = false; //if we tick after the npc turn as opposed to at the start of each round

	bool invincible = false; //if this effect makes the affected npc invincible
	bool evasive = false; //invincible but due to amazing dodging skills

	bool stacks = false; //if the effect stacks instead of just being extended when reapplied

	int multipositioning = 0; //how many shallow copies of the affected npc to make in battle, used by the SUPERSMOOTHIE

	Effect* attackeffect = NULL; //what attack effect to give to the target

	Effect() {} //default constructor to avoid random errors

	//constructs the effect
	Effect(const char* _name, int _duration, int _damage = 0, int _spleak = 0, float _attackbuff = 1, float _defensebuff = 1, float _toughbuff = 1, float _piercebuff = 1, float _speedbuff = 1) {
		name = _name;
		duration = _duration;
		damage = _damage;
		spleak = _spleak;
		attackbuff = _attackbuff;
		defensebuff = _defensebuff;
		toughbuff = _toughbuff;
		piercebuff = _piercebuff;
		speedbuff = _speedbuff;
		Helper::effectsH.push_back(this); //store a pointer to this effect in the effects vector
	}
};
#endif // !EFFECT