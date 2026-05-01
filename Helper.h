//header file for my Helper namespace

#ifndef HELPER
#define HELPER

#include "WorldState.h"
#include <vector>
#include <map>
#include <memory>

class Room;
class NPC;
class Item;
struct Attack;
struct Effect;
struct Conversation;
struct WorldChange;

namespace Helper {
	void CinIgnoreAll(bool force = false); //clears extra or faulty input
	void CinPause(); //pauses until the player presses ENTER
	void AllCaps(char* text); //capitalizes the given text
	void ParseCommand(char* commandP, char* commandWordP, char* commandExtensionP, int skipSpaces = 0); //takes commandP, puts the first word in commandWordP, and the rest in commandExtensionP
	void ParseWithON(const char* commandExtensionP, char* itemNameP, char* npcNameP); //takes commandExtensionP, puts everything before " ON " in itemNameP, and everything after in npcNameP
	NPC* getNPCInVector(std::vector<NPC*>& the_vector, const char* npcname); //returns the npc in the given vector that has the given name
	Item* getItemInVector(std::vector<Item*>& the_vector, const char* itemname); //returns the item in the given vector that has the given name
	Item* getItemTypeInVector(std::vector<Item*>& the_vector, const char* itemtype); //returns the item in the given vector that has the given type
	Attack* getAttackInVector(const std::vector<Attack*>& the_vector, const char* attackname); //returns the attack in the given vector that has the given name
	void printNPCData(NPC* npc, bool battle = false); //prints the data of the npc
	void printItemData(Item* item); //prints the data of the item
	void printAttacks(NPC* npc); //prints the attacks that the given npc has
	void deleteItem(Room* currentRoom, std::vector<Item*>* inventory, Item* item); //deletes the item and removes it from the inventory or current room based on where it was
	int Clamp(int num, int min, int max); //returns num with a minimum return value of min and a maximum of max
	double ClampD(double num, double min, double max); //same as Clamp but for floats instead of ints
	int Round(double num); //do (int)round(num), so I don't have to write that everywhere
	void sortBySpeed(std::vector<NPC*>& team); //sorts the vector of npcs by speed
	int aliveCount(std::vector<NPC*>& team); //returns how many npcs in the given team have >0 hp
	std::vector<NPC*> getAlive(const std::vector<NPC*>& team); //get vector with only the npcs above 0 hp
	bool AOrB(const char* prompt, const char* A, const char* B); //prompts the player to type either option A or option B and return true if A is chosen
	void printConversation(const Conversation* _convo, bool finalpause); //print a conversation in conversation order
	void printLvlUpData(NPC* npc); //print the level up data of the given npc
	void applyWorldChange(WorldChange& changes); //apply the changes to the world which are in the given stuff to change
	void applyWeaponAttack(NPC* player, Item* item); //gives the player the attack the weapon item has, weapon items can be gotten through different places so we have this helper
	bool getCardinal(const char* direction); //get if the direction given is a cardinal direction
	const char* getTitle(); //get the title screen text, this is so I can modify it from one place since it gets printed in the beginning but also endings
	void buildNPCData(); //set up all the npc maps, including reversing the npc to char map so we can get the npc from the char
	NPC* getBase(NPC* npc); //go to the parents until we reach the template
	
	//I actually have to use std:: here since you shouldn't use namespaces in h files
	extern std::map<const char*, const char*> ReverseDirection; //map to find the opposite of the given direction, tied to the Helper object
	extern std::map<NPC*, char> npcChar; //map to find the char that represents each recruitable npc, used by the save system
	extern std::map<char, NPC*> charNPC; //the last map but reversed
	
	extern std::vector<Room*> roomsH; //a bunch of vectors for every object created in the game, so they can all be deleted later
	extern std::vector<NPC*> npcsH;
	extern std::vector<Item*> itemsH;
	extern std::vector<Attack*> attacksH;
	extern std::vector<Effect*> effectsH;
	extern std::vector<std::shared_ptr<Conversation>> relaysH; //conversation relays use weak_ptrs to avoid infinite loops so we store them here so the conversations don't get deleted

	//these are a bunch of stuff that the save system uses that we track as we play the game

	extern char* sectionW; //build section W as we play
	extern char* sectionT; //build section T as we play

	extern int stats[18]; //count how many times we used each command
	extern int invalidmove;
	extern int sessions; //how many times the player has booted up this save

	//track these for every teammate
	extern std::map<NPC*, int> attackslaunched; //how many times each npc attacked
	extern std::map<NPC*, int> helpslaunched; //how many times each npc did a beneficial attack
	extern std::map<NPC*, long long> damagedealt; //total damage this npc has dealt
	extern std::map<NPC*, long long> healthhealed; //total healing this npc has done to other npcs
	extern std::map<NPC*, long long> healthrecovered; //how much this npc has healed in total
	extern std::map<NPC*, long long> damagerecieved; //how much damage this npc has tanked
	extern std::map<NPC*, int> knockouts; //how many npcs this npc incapacitated
	extern std::map<NPC*, int> revives; //how many npcs this npc recapacitated

	extern set<NPC*> encountered; //all the npcs we've ever fought
	extern set<NPC*> recruited; //all the npcs we've ever recruited

	extern int npcID; //ids for the npcs (their index in npcsH) so we can track them in the save system and also for determining their random stat scale deterministically
	extern int roomID; //ids for the items and rooms so it's easier to find their place in the vectors from the save system
	extern int itemID;
}
#endif