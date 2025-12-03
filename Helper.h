#ifndef HELPER
#define HELPER

#include "NPC.h"
#include "Item.h"
#include <vector>
#include <map>

namespace Helper {
	void CinIgnoreAll();
	void CinPause();
	void AllCaps(char* text);
	void ParseCommand(char* commandP, char* commandWordP, char* commandExtensionP, int skipSpaces = 0);
	void ParseWithON(char* commandExtensionP, char* itemNameP, char* npcNameP);
	NPC* getNPCInVector(vector<NPC*> the_vector, char* npcname);
	Item* getItemInVector(vector<Item*> the_vector, char* itemname);
	void printNPCData(NPC* npc);
	void printItemData(Item* item);
	int Clamp(int num, int min, int max);
	float ClampF(float num, float min, float max);
	void sortBySpeed(vector<NPC*>& team);
	int aliveCount(vector<NPC*> team);
	extern map<const char*, const char*> ReverseDirection; //NAMESPACES CAN HAVE VARIABLES???? THATS INCREDIBLE I LOVE THAT SO MUCH!!!!
}
#endif