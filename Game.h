/* Tomas Carranza Echaniz
*  5/14/26
*  This is the header file for the game world and gameplay
*
*  The game is constructed by passing a pointer to a Save struct. When starting a NEW GAME, a reference to a NULL Save pointer
*  which is in the saves vector is passed. If the player never saves, the program sees that it is null and removes it from the
*  vector. The game must then be started by calling play(), and from there the SetupWorld function is called, and if a save was
*  provided, it will mutate the world using Save.h's LoadGame function to match the player's progress so far. From there, the
*  player can start using game commands which is handled with all the functions below, though some of the functionality may be
*  in the Helper namespace.
*/

#ifndef GAME
#define GAME

#include "Battle.h"
#include "Room.h"
#include "Save.h"

class Game {
public:
	Game(Save*& _save);
	
	void SetupWorld(); //set up the game world with all its rooms, npcs, items, etc. before the player actually plays in it

	void PrintRoomData(Room* currentRoom, bool track = false); //prints all the properties of the given room
	void travel(Room* currentRoom, const char* direction, bool forceTravel = false, Room* forceDest = NULL); //move the player and co. to a new room based on direction, or also just teleopring to forceDest if given
	void fight(Room* currentRoom, const char* name, bool track = false); //initiates battle with an npc
	void takeItem(Room* currentRoom, const char* itemname); //takes an item from the current room and adds it to the inventory
	void dropItem(Room* currentRoom, const char* itemname); //drops an item from the inventory into the current room
	void useItem(Room* currentRoom, const char* itemname); //uses an item, with functionality based on type
	void recruitNPC(Room* currentRoom, const char* npcname); //recruit an npc into the player party
	void dismissNPC(Room* currentRoom, const char* npcname); //dismiss npcs from your party
	void printNPCDialogue(Room* currentRoom, const char* npcname); //prints anything the targeted npc has to say, and other ASK-related functionalities
	void printInventory(); //prints the player's monies and inventory items
	void printParty(); //prints the player's part
	void analyze(Room* currentRoom, const char* name); //analyzes either an item or npc of the given name
	void buy(Room* currentRoom, const char* name); //buys an item from the current room's catalogue
	bool saveWorld(time_t& savetime, const char* andwhat, bool& laws, bool gameend = false); //save the game so we can store it as a file, and return if we want to keep going after saving
	bool confirmQuit(time_t savetime, bool laws); //make sure the player is really sure about quitting without saving, and return if we are continuing

	void play(); //the main game function and gameplay loop for exploring the world
	
private:
	std::vector<Item*> inventory; //the inventory of items
	int mony = 0; //monies are the currency in the BURGER QUEST universe.
	std::vector<NPC*>* party; //a pointer to the player's party for convenience
	NPC* self; //the player!

	Save*& save; //this is the save that this game instance is managing

	//flavor text printed by printHelp. The comments are what they are instead if the game has been beaten with the true ending.
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