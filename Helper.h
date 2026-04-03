//header file for my Helper namespace

#ifndef HELPER
#define HELPER

#include "WorldState.h"
#include <vector>
#include <map>

class Room;
class NPC;
class Item;
struct Attack;
struct Effect;
struct Conversation;
struct WorldChange;

namespace Helper {
	void CinIgnoreAll(); //clears extra or faulty input
	void CinPause(); //pauses until the player presses ENTER
	void AllCaps(char* text); //capitalizes the given text
	void ParseCommand(char* commandP, char* commandWordP, char* commandExtensionP, int skipSpaces = 0); //takes commandP, puts the first word in commandWordP, and the rest in commandExtensionP
	void ParseWithON(const char* commandExtensionP, char* itemNameP, char* npcNameP); //takes commandExtensionP, puts everything before " ON " in itemNameP, and everything after in npcNameP
	NPC* getNPCInVector(std::vector<NPC*>& the_vector, const char* npcname); //returns the npc in the given vector that has the given name
	Item* getItemInVector(std::vector<Item*>& the_vector, const char* itemname); //returns the item in the given vector that has the given name
	Item* getItemTypeInVector(std::vector<Item*>& the_vector, const char* itemtype); //returns the item in the given vector that has the given type
	void printNPCData(NPC* npc); //prints the data of the npc
	void printItemData(Item* item); //prints the data of the item
	void printAttacks(NPC* npc); //prints the attacks that the given npc has
	void deleteItem(Room* currentRoom, std::vector<Item*>* inventory, Item* item); //deletes the item and removes it from the inventory or current room based on where it was
	int Clamp(int num, int min, int max); //returns num with a minimum return value of min and a maximum of max
	float ClampF(float num, float min, float max); //same as Clamp but for floats instead of ints
	void sortBySpeed(std::vector<NPC*>& team); //sorts the vector of npcs by speed
	int aliveCount(std::vector<NPC*>& team); //returns how many npcs in the given team have >0 hp
	bool AOrB(const char* prompt, const char* A, const char* B); //prompts the player to type either option A or option B and return true if A is chosen
	void printConversation(const Conversation* _convo, bool finalpause); //print a conversation in conversation order
	void printLvlUpData(NPC* npc); //print the level up data of the given npc
	void applyWorldChange(WorldChange& changes); //apply the changes to the world which are in the given stuff to change
	
	//I actually have to use std:: here since you shouldn't use namespaces in h files
	extern std::map<const char*, const char*> ReverseDirection; //map to find the opposite of the given direction, tied to the Helper object
	
	extern std::vector<Room*> roomsH; //a bunch of vectors for every object created in the game, so they can all be deleted later
	extern std::vector<NPC*> npcsH;
	extern std::vector<Item*> itemsH;
	extern std::vector<Attack*> attacksH;
	extern std::vector<Effect*> effectsH;

	extern int npcID; //ids for the npcs (their index in npcsH) so we can track them in the save system and also for determining their random stat scale deterministically
}
#endif