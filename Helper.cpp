//the implementation file for my Helper namespace, has a bunch of helpful functions that I might use in multiple files

#include "Helper.h"
#include <cstring>
#include <iostream>
#include <limits>
#include <algorithm>
using namespace std;

namespace Helper {
	//if there's extra or faulty input, this handy function will ignore that for you without having to type numeric limits every time
	void CinIgnoreAll() {
		if (!cin) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}
	//pauses until the user inputs enter, and also ignores any characters inputted meanwhile. Used for dramatic effect, and so the player doesn't get nuked with a wall of text during battle
	void CinPause() {
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	//takes a pointer to a char array as input, and capitalizes all the characters in it. Used for easier string comparison and consistent case among names
	void AllCaps(char* text) {
		for (int i = 0; i < strlen(text); i++) {
			text[i] = toupper(text[i]);
		}
	}
	//takes commandP, puts the first word into commandWordP, and the rest into commandExtensionP. some attacks used in Battle may be >1 word, so there's also an option to skip a certain amount of spaces
	void ParseCommand(char* commandP, char* commandWordP, char* commandExtensionP, int skipSpaces) {
		int i = 0; //the current char we're checking
		//keeps going until we run out of spaces to skip or we're at the end of the charray
		while ((skipSpaces > 0 || commandP[i] != ' ') && commandP[i] != '\0') {
			commandWordP[i] = commandP[i]; //puts the character in the command word portion
			if (commandP[i] == ' ') {
				skipSpaces--; //decrements skip spaces because a space was found
			}
			i++; //advances the check position
		}
		commandWordP[i] = '\0'; //caps the end of the command word with a null terminator
		if (commandP[i] == '\0') { //return and set the extension to blank if we're at the end of the command
			strcpy(commandExtensionP,"");
			return;
		}
		i++; //skip the space
		int j = i; //i - j == the current position for the command extension
		while (commandP[i] != '\0') { //sets the command extension to the rest of command
			commandExtensionP[i - j] = commandP[i];
			i++; //increment the checking position
		}
		commandExtensionP[i - j] = '\0'; //null terminates the command extension
	}
	//takes a command extension and splits it based on " ON ", then puts it into item name and npc name. For example "USE JUICE ON BOB" would take "JUICE ON BOB" and split it into "JUCIE" (item) and "BOB" (npc)
	void ParseWithON(char* commandExtensionP, char* itemNameP, char* npcNameP) {
		char* on = strstr(commandExtensionP, " ON "); //finds the first char of the substring " ON " in the given command extension
		if (on == NULL) { //we just return blanks if the given text has no " ON "
			strcpy(itemNameP, "");
			strcpy(npcNameP, "");
			return;
		}
		int i = 0; //the position of the command extension we're currently checking
		while (&commandExtensionP[i] != on) { //puts the command extension chars into itemName until we find the " ON "
			itemNameP[i] = commandExtensionP[i];
			i++; //increment the checking position
		}
		itemNameP[i] = '\0'; //null terminates the item name
		i += 4; //skip the " ON " in the command extension
		int j = i; //i - j == the current position for the npc name
		while (commandExtensionP[i] != '\0') { //put the rest of the command extension into npc name until we reach the end
			npcNameP[i - j] = commandExtensionP[i];
			i++; //increment the checking position
		}
		npcNameP[i - j] = '\0'; //null terminates the npc name
	}
	//finds an npc in the given vector who has the given name
	NPC* getNPCInVector(vector<NPC*>& the_vector, char* npcname) {
		for (NPC* npc : the_vector) { //if the npc isn't defeated and the name matches, we return that
			if (!npc->getDefeated() && !strcmp(npc->getName(), npcname)) {
				return npc;
			}
		}
		return NULL; //no valid npc was found so return null
	}
	//finds an item in the given vector that has the given name
	Item* getItemInVector(vector<Item*>& the_vector, char* itemname) {
		for (Item* item : the_vector) { //if the item's name matches, we return that
			if (!strcmp(item->getName(), itemname)) {
				return item;
			}
		}
		return NULL; //no valid item was found so return null
	}
	//prints the data of the given npc
	void printNPCData(NPC* npc) {
		cout << "\n" << npc->getTitle(); //prints the title of the npc if they have one
		if (strlen(npc->getTitle()) > 0) {
			cout << " "; //seperae the title and the name with a space if there is a title
		} //prints the name and description of the npc plus their level
		cout << npc->getName() << " - " << npc->getDescription() << "\n  LEVEL " << npc->getLevel();
		if (npc->getRecruited()) { //prints the xp to level up for recruited npcs. Non-recruitable npcs don't level up anyway
			cout << " (" << npc->xpForNextLevel() << " xp to LEVEL UP)";
		} //prints any effects the npc may have (eg. "POISON")
		npc->printEffects();
		cout << "\n  HEALTH - " << npc->getHealthMax(); //prints all the stats of the npc
		cout << "\t  DEFENSE - " << npc->getDefense();
		cout << "\n  ATTACK - " << npc->getAttack();
		cout << "\t  TOUGHNESS - " << npc->getToughness();
		cout << "\n  PIERCE - " << npc->getPierce();
		cout << "\t  SPEED - " << npc->getSpeed();
		cout << "\n  SKILL - " << npc->getSPMax();
	}
	//prints the data of the given item, usually just the name and description
	void printItemData(Item* item) {
		cout << "\n" << item->getName() << " - " << item->getDescription();
		if (item->getStock()) { //if the item is for sale, we print its price as well
			cout << " Costs " << item->getPrice() << " mon";
			if (item->getPrice() == 1) { //proper grammar based on singular or plural hooray
				cout << "y.";
			} else {
				cout << "ies.";
			}
		}
	}
	//prints the attacks of the given npc, their descriptions, and how they affect sp
	void printAttacks(NPC* npc) {
		cout << "\nBasic attack:\n"; //the npc's basic attack (the punch!) and how much sp it generates (5!)
		Attack* attack = npc->getBasicAttack();
		cout << attack->name << " - " << attack->trueDesc << " - Generates " << -attack->cost << " SP";
		if (npc->getSpecialAttacks().size() > 0) { //if we have any special attacks
			cout << "\nSpecial attacks:";
		} //prints all the special attacks and how much sp they cost
		for (Attack* attack : npc->getSpecialAttacks()) {
			//we only print attacks if the npc is leveled up enough to use them
			if (attack->minLevel <= npc->getLevel()) {
				cout << "\n" << attack->name << " - " << attack->trueDesc << " - Costs " << attack->cost << " SP";
			}
		}
	}	
	//deletes the item from existence and removes it from the inventory or current room based on where it was
	void deleteItem(Room* currentRoom, vector<Item*>* inventory, Item* item) {
		for (Item* _item : *inventory) { //first check if it's in the inventory
			if (_item == item) {
				delete item; //delete the item and remove it from the inventory
				inventory->erase(remove(inventory->begin(), inventory->end(), item), inventory->end());
				return; //return because the item was found
			}
		} //if the item was on the ground of the current room, we de-room it and delete it then
		item->unRoom();
		delete item;
	}
	//returns the given integer from min to max
	int Clamp(int num, int min, int max) {
		if (num < min) { //return the minimum if below it
			return min;
		}
		if (num > max) { //return the maximum if above it
			return max;
		}
		return num; //the number is actually perfectly fine so we return it as-is
	}
	//returns the given float from min to max, seperate from the above one for convenience
	float ClampF(float num, float min, float max) {
		if (num < min) { //return the minimum if below it
			return min;
		}
		if (num > max) { //return the maximum if above it
			return max;
		}
		return num; //the number is actually perfectly fine so we return it as-is
	}
	//returns if a is faster than b
	bool speedCompare(NPC* a, NPC* b) {
		return a->getSpeed() > b->getSpeed();
	}
	//sorts vector of npcs from fastest to slowest
	void sortBySpeed(vector<NPC*>& team) {
		sort(team.begin(), team.end(), speedCompare);
	}
	//get how many npcs in a team have at least 1 hp
	int aliveCount(vector<NPC*>& team) {
		int i = 0; //starts at 0 and adds 1 every time someone has >0 hp
		for (NPC* npc : team) {
			if (npc->getHealth() > 0) {
				i++;
			}
		}
		return i; //returns the found count
	}
	//loops until the player chooses either option A or B, and returns true if A was chosen and false if B was chosen
	bool AOrB(const char* prompt, const char* A, const char* B) {
		while (true) { //loops until valid response, after which the loop is returned out of
			char _command[255] = ""; //the charray that the player inputs into

			char command[255]; //the command truncated to 254 characters (to not cause a buffer overflow)
			cout << "\n" << prompt << "\n> "; //The amazing >
			cin.getline(command, 255); //gets the player input

			ParseCommand(_command, command, _command, 255); //gets the first 254 characters of _command. Clears _command as a side effect but we don't need it afterwards anyway so yeah

			AllCaps(command); //capitalizes the command for easier parsing

			if (!strcmp(command, A)) { //choose option A
				return true;
			} else if (!strcmp(command, B)) { //choose option B
				return false;
			} else { //prints the invalid input
				cout << "\nInvalid response \"" << command << "\".";
			}

			CinIgnoreAll(); //clears extra or faulty input
		}
	}

	//map to find the opposite of the given direction (eg. ReverseDirection[SOUTH] == NORTH)
	map<const char*, const char*> ReverseDirection;

	//vectors of everything in memory so we can deallocate them all later
	/*vector<char*> charsH;
	vector<Room*> roomsH;
	vector<NPC*> npcsH;
	vector<Item*> itemsH;
	vector<Attack*> attacksH;
	vector<Effect*> effectsH;*/
}