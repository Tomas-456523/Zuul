//header file for the game world and gameplay

#ifndef GAME
#define GAME

#include "Battle.h"
#include "Room.h"
#include "Save.h"

class Game {
public:
	Game(Save*& _save);
	
	void SetupWorld(); //set up the game world with all its rooms, npcs, items, etc. before the player actually plays in it

	void PrintRoomData(Room* currentRoom, bool track = false);
	void travel(Room* currentRoom, const char* direction, bool forceTravel = false, Room* forceDest = NULL);
	void fight(Room* currentRoom, const char* name, bool track = false);
	void takeItem(Room* currentRoom, const char* itemname);
	void dropItem(Room* currentRoom, const char* itemname);
	void useItem(Room* currentRoom, const char* itemname);
	void recruitNPC(Room* currentRoom, const char* npcname);
	void dismissNPC(Room* currentRoom, const char* npcname);
	void printNPCDialogue(Room* currentRoom, const char* npcname);
	void printInventory();
	void printParty();
	void analyze(Room* currentRoom, const char* name);
	void buy(Room* currentRoom, const char* name);
	bool saveWorld(time_t& savetime, const char* andwhat, bool& laws, bool gameend = false);
	bool confirmQuit(time_t savetime, bool laws);

	void play();
	
private:
	std::vector<Item*> inventory; //the inventory of items
	int mony = 0; //monies are the currency in the BURGER QUEST universe.
	std::vector<NPC*>* party; //a pointer to the player's party for convenience
	NPC* self; //the player!

	Save*& save; //this is the save that this game instance is managing

	//flavor text printed by printHelp
	const char* flavorText[16] = {
		"What even is a BURGER?", //You practice your breakdancing skills now that you're done with your BURGER QUEST.
		"You consider the state of the economy.",
		"You are blue dabadeedabadai.",
		"You are beginning to believe.",
		"You forgor.",
		"You accidentally find spoilers for the ending of BURGER QUEST 2. It is very concerning...", //You accidentally find spoilers for the ending of BURGER QUEST 2. Good thing you already beat it.
		"You realize you don't have an oven.",
		"You spot a billboard advertising the BURGER RESTAURANT in BURGERSBURG.", //You spot a billboard advertising literally nothing.
		"You ask a passerby what the valid commands are. The guy looks at you really confused.",
		"You stop it and get some help.",
		"We have been trying to reach you about your car's extended warranty.",
		"You spot a quick brown fox jumping over a lazy dog.",
		"You say hello to the world.",
		"I could really go for a burger right now. Too bad they only sell BURGERs here.", //You realize how you've never seen a normal burger.
		"You have a cake and eat it too.",
		"You take a potato chip... and eat it."
	};

	//a list of the valid commands (and extensions) to be printed by printHelp
	const char* validCommands[17] = {
		"GO [direction]",
		"TAKE [item]",
		"DROP [item]",
		"USE [item] (ON [npc])",
		"RECRUIT [npc]",
		"DISMISS [npc]",
		"ASK [npc]",
		"INVENTORY",
		"PARTY",
		"ATTACKS",
		"ROOM",
		"ANALYZE [npc/item]",
		"FIGHT [npc]",
		"BUY [item]",
		"HELP",
		"SAVE (AND QUIT)",
		"QUIT"
	};
};

#endif