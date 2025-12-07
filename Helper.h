//header file for my Helper namespace

#ifndef HELPER
#define HELPER

#include "NPC.h"
#include "Item.h"
#include <vector>
#include <map>

namespace Helper {
	void CinIgnoreAll(); //clears extra or faulty input
	void CinPause(); //pauses until the player presses ENTER
	void AllCaps(char* text); //capitalizes the given text
	void ParseCommand(char* commandP, char* commandWordP, char* commandExtensionP, int skipSpaces = 0); //takes commandP, puts the first word in commandWordP, and the rest in commandExtensionP
	void ParseWithON(char* commandExtensionP, char* itemNameP, char* npcNameP); //takes commandExtensionP, puts everything before " ON " in itemNameP, and everything after in npcNameP
	NPC* getNPCInVector(vector<NPC*>& the_vector, char* npcname); //returns the npc in the given vector that has the given name
	Item* getItemInVector(vector<Item*>& the_vector, char* itemname); //returns the item in the given vector that has the given name
	void printNPCData(NPC* npc); //prints the data of the npc
	void printItemData(Item* item); //prints the data of the item
	void deleteItem(Room* currentRoom, vector<Item*>* inventory, Item* item); //deletes the item and removes it from the inventory or current room based on where it was
	int Clamp(int num, int min, int max); //returns num with a minimum return value of min and a maximum of max
	float ClampF(float num, float min, float max); //same as Clamp but for floats instead of ints
	void sortBySpeed(vector<NPC*>& team); //sorts the vector of npcs by speed
	int aliveCount(vector<NPC*>& team); //returns how many npcs in the given team have >0 hp
	
	//I actually have to use std:: here since you shouldn't use namespaces in h files
	extern std::map<const char*, const char*> ReverseDirection; //map to find the opposite of the given direction, tied to the Helper object
	
	/*extern std::vector<char*> charsH; //a bunch of vectors for every object created in the game, so they can all be deleted later
	extern std::vector<Room*> roomsH;
	extern std::vector<NPC*> npcsH;
	extern std::vector<Item*> itemsH;
	extern std::vector<Attack*> attacksH;
	extern std::vector<Effect*> effectsH;*/
}
#endif