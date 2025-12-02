#ifndef EFFECT
#define EFFECT

#include <cstring>

class NPC;

struct Effect {
	char name[255]; //the name of the effect, (e.g. "POISON")
	int duration; //how long the effect lasts

	int damage;
	int attackbuff;
	int defensebuff;
	int toughbuff;
	int piercebuff;
	bool freeze;

	int guardset;

	NPC* npc;

	//status effects
	// - poison
	// - burn
	// - hypnotism
	// - buff (attack+)
	// - hyper (attack++ speed++)
	// - megabuff (attack+++ defense++)
	// - debuff (attack- defense-)
	// - megadebuff (attack-- defense---)
	// - despair (attack--- defense---) (only one teammate at a time)
	// - memory leak (drains sp)
	// - en garde! (for jim shadies)

	Effect() {}

	Effect(const char _name[255], int _duration, int _damage = 0, int _attackbuff = 0, int _defensebuff = 0, int _toughbuff = 0, int _piercebuff = 0) {
		strcpy(name, _name);
		duration = _duration;
		damage = _damage;
		attackbuff = _attackbuff;
		defensebuff = _defensebuff;
		toughbuff = _toughbuff;
		piercebuff = _piercebuff;
	}
};
#endif // !EFFECT