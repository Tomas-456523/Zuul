#ifndef EFFECT
#define EFFECT

#include <cstring>
#include "NPC.h"

class NPC;

struct Effect {
	char name[255]; //the name of the effect, (e.g. "POISON")
	int duration; //how long the effect lasts

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
	//how many targets it hits (1, 3, or all)
	//min and max amount of times it hits

	Effect() {}

	Effect(const char _name[255], int _duration) {
		strcpy(name, _name);
		duration = _duration;
	}

	void setDuration(int _duration) {
		duration = _duration;
	}

	void tick(NPC* npc) {
		duration--;
		if (duration <= 0) {
			//remove the effect from the npc
		}
	}
};

#endif // !EFFECT