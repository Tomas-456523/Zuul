#ifndef HELPER
#define HELPER

#include "NPC.h"
#include "Item.h"
#include <vector>

namespace Helper {
	void CinIgnoreAll();
	void CinPause();
	void AllCaps(char* text);
	void ParseCommand(char* commandP, char* commandWordP, char* commandExtensionP, int skipSpaces = 0);
	NPC* getNPCInVector(vector<NPC*> the_vector, char* npcname);
	Item* getItemInVector(vector<Item*> the_vector, char* itemname);
	void printNPCData(NPC* npc);
	void printItemData(Item* item);
	int Clamp(int num, int min, int max);
	float ClampF(float num, float min, float max);
	int Ceil(float num);
	void sortBySpeed(vector<NPC*>& team);
	int aliveCount(vector<NPC*> team);
}
#endif