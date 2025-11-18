#ifndef HELPER
#define HELPER

#include "NPC.h"
#include "Item.h"
#include <vector>

namespace Helper {
	void CinIgnoreAll();
	void CinPause();
	void AllCaps(char* text);
	void ParseCommand(char* commandP, char* commandWordP, char* commandExtensionP);
	NPC* getNPCInVector(vector<NPC*> the_vector, char* npcname);
	Item* getItemInVector(vector<Item*> the_vector, char* itemname);
	int Clamp(int num, int min, int max);
	void sortBySpeed(vector<NPC*>& team);
}
#endif