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

	bool freeze = false; //if the effect causes the target to get frozen
	bool hypnotize = false; //if the effect causes the target to get hypnotized (attack own party)

	int guardset = 0; //if this effect sets guard on the target

	NPC* npc = NULL; //the affected npc, used by instances

	Effect() {} //default constructor to avoid random errors

	//constructs the effect
	Effect(const char* _name, int _duration, int _damage = 0, int _spleak = 0, float _attackbuff = 0, float _defensebuff = 0, float _toughbuff = 0, float _piercebuff = 0) {
		name = _name;
		duration = _duration;
		damage = _damage;
		spleak = _spleak;
		attackbuff = _attackbuff;
		defensebuff = _defensebuff;
		toughbuff = _toughbuff;
		piercebuff = _piercebuff;
		Helper::effectsH.push_back(this); //store a pointer to this effect in the effects vector
	}
};
#endif // !EFFECT