//header file for effects

#ifndef EFFECT
#define EFFECT

#include <cstring>

class NPC;

struct Effect {
	char name[255]; //the name of the effect, (e.g. "POISON")
	int duration; //how long the effect lasts

	int damage; //how much damage the effect does per tick
	int spleak; //how much sp this effect leaks per tick

	//buffs (additive)
	int attackbuff;
	int defensebuff;
	int toughbuff;
	int piercebuff;

	bool freeze; //if the effect causes the target to get frozen
	bool hypnotize; //if the effect causes the target to get hypnotized (attack own party)

	int guardset; //if this effect sets guard on the target

	NPC* npc; //the affected npc, used by instances

	Effect() {} //default constructor to avoid random errors

	//constructs the effect
	Effect(const char _name[255], int _duration, int _damage = 0, int _spleak = 0, int _attackbuff = 0, int _defensebuff = 0, int _toughbuff = 0, int _piercebuff = 0) {
		strcpy(name, _name);
		duration = _duration;
		damage = _damage;
		spleak = _spleak;
		attackbuff = _attackbuff;
		defensebuff = _defensebuff;
		toughbuff = _toughbuff;
		piercebuff = _piercebuff;
		//Helper::effectsH.push_back(this); //store a pointer to this effect in the effects vector
	}
};
#endif // !EFFECT