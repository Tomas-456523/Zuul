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

	Effect(const char _name[255], int _duration) {
		strcpy(name, _name);
		duration = _duration;
	}

	void setDuration(int _duration) {
		duration = _duration;
	}

	/*void tick(NPC* npc) {
		duration--;
		if (duration <= 0) {
			//remove the effect from the npc
			return;
		}
		if (damage != 0) {
			npc->directDamage(damage); //but we don't print damage reason...
		}
	}*/
};

#endif // !EFFECT