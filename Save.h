//header file for the world saves

#include <vector>
#include <cstring>
#include <fstream>
#include <iterator>
#include <ctime>
#include "Helper.h"
#include "Room.h"
#include "NPC.h"
#include "Item.h"

struct Save {
	/* SAVE DATA FORMAT:
	*  
	*  Starts with "BQ2" and ends with "="
	*  Sections are seperated by | and individual things in each section are seperated by ,
	*  Items, NPCs, and Rooms are accessed by their ID in their corresponding Helper vector (e.g. roomsH)
	*  Each section starts with a denoting letter that shows what section it is
	*  Storing it in the exact order it was done in is extremely important so that we don't try to get not-yet-existant items, overwrite room descriptions with outdated text, etc.
	*  We do not care about attempted actions, only successful ones.
	*  
	*  Section V: A character representing the save system version (for futureproofing)
	*  Section N: Player name in plaintext, except if there is a | or = or , in the name we store it with a preceding \, same thing with \'s to account for this (e.g. B|O\B= would be stored as NB\|O\\B\=)
	*  Section M: Monies
	*  Section W: Everything that can cause world changes in the order they happened in.
	*  - Items that were bought and therefore duplicated, denoted by b[item]
	*  - Item usage in which room, denoted by u[item]r[room] (including taking take to use world change items and taking manhole covers)
	*      - If it was a choice orb item, we also store which choice was chosen appended with a . (e.g. u[item]r[room].b)
	*  - Items that were used up in battle, denoted by x[item]
	*  - NPCs that were defeated, denoted by d[npc]
	*  - NPCs that caused respawn changes, denoted by r[npc]
	*  - NPCs that were talked to, denoted by a[npc], with an optional modifier for different types of dialogue (.r for recruitment dialogue, .j for rejection dialogue, .d for dismissal dialogue, .i for dismissal rejection, .o for opening, .g for gym)
	*  - Enter changes, denoted by e[room]
	*  - Room welcomes that were triggered, denoted by w[room]
	*  - Dropped escape orbs, denoted by o[item]
	*  - Dropped and taken hose items, denoted by h[item]r[room] and j[item]r[room] respectively
	*  - If the player was caught by a pursuer, denoted by p[npc]
	*  - Opening a temple in a temple entrance room, denoted by t[room]
	*  Section U: Only exists while pursuer is pursuing, tracks their room
	*  Section R: Stuff related to the player team
	*  - First, the teammate this chunk is tracking:
	*    a - Bernard	l - Jilly	h - Henry Jerry
	*    f - Floria		m - Mike	j - Michelin	g - Graham
	*    e - Egadwick	v - Viola	x - Carlos		r - Richie
	*    k - Absolom	c - Cacty	p - Plum		b - Ratman
	*  - Then level, xp, room, and finally, time left in gym (0 if not in gym)
	*  - Everything connected to the NPC is seperated by .
	*  - So, this section would look something like: R'f[level].[xp].[room].[gym time]
	*  Section P: What the player's party actually is, using the above codes (e.g. ajbhl) would mean a party of Self, Michelin, Ratman, Henry Jerry, and Jilly in that order
	*  Section E: Enemy types that have been encountered
	*  Section I: Every item's room, or -1 if it's in the inventory (saved in order, so the items in the rooms and in the inventory remain in the same order) (-1 instead of 0 because 0 is a valid room ID, being limbo, so -1 translates to NULL room here)
	*  - Denoted by [item].[room]
	*  Section L: Exists after taming lobster, stores its name in plaintext with the same exceptions as the player name, and also the lobster's room after that (L[lobster name],[room])
	*  Section T: The fast travel exits that have been found so far, and where they lead to connected by . because two of the stations have two adjacent rooms, stored as an id in the tunnel directions map which can be reversed via iterator ([direction].[room])
	*  - The order doesn't matter in this section because the exit maps in Room sorts the exits in alphabetical order anyway
	*  Section B: Exists after depositing money in the bank, stores current bank balance and last time used bank ([balance],[time])
	*  Section S: All the world states as 0 or 1 (26 states as of now, not including NEVER), since some states are changed by things other than world changes
	*  Section Q: How long the player has played on this save file, stored in seconds
	*  
	*  Example save: BQ2|V0|NBOB THE BUILDER|M10|Wu123r23,x3244,e300,a123.o|U156|Ra1.3.123.0|Pavk|E43,23,67,50|C1,2,3,4,5,6,7|I12.0,234.-1,324.0,53.2,13.0,4324.3,456.201|LCOOL LOBSTER,110|T1.123,2.12,0.112|B10,2|S01010101010101111010010101|Q2198139=

	//MARK: roaming npc positions are not saved?
	*/

	/*stats we might want to save
	times moved in a direction
	attacks launched
	items used
	damage dealt
	damage recieved
	enemies incapacitated
	enemies discovered /
	enemy encounters defeated /
	bosses defeated /
	times gone in an invalid direction
	teammates recruited /
	moves learned /
	times used each command

	Times QUIT without saving: idk

	*/

	size_t savesize = 1000; //we double the save size when we need to, starts with a reasonable length of 1000
	char* data; //the save data

	static char* sectionW; //build section W as we play
	static char* sectionT; //build section T as we play

	static vector<Save*> getSaves

	static set<NPC*> encountered; //all the npcs we've ever fought
	static set<NPC*> recruited; //all the npcs we've ever recruited

	//returns an empty save with zero data for making a new game
	static Save NewGame() {
		return Save("BQ2=");
	}

	//
	void reset() {
		delete data;
		savesize = 1000;
		data = new char[savesize]();
		data[0] = '\0';
	}

	//adds a chunk to the save data and extending the save data length if needed
	void addChunk(const char* chunk) {
		while (strlen(data) + strlen(chunk) >= savesize) {
			savesize *= 2;
			char* newdata = new char[savesize]();
			strcpy(newdata, data);
			delete data;
			data = newdata;
		}
		strcat(data, chunk);
	}

	//gets the variables in main and serializes it into save data
	static void SaveGame(Save* save, std::vector<Item*>* inventory, int monies, double& playtime, time_t lastsave) {
		save->reset(); //reset the current save string so we can rebuild it, easier to edit that way
		//add beginning and section V version number (we are in version 0)
		save->addChunk("BQ2|V0|N");
		//Section N, add the name MARK: DOES NOT DO THE CHECKS YET!
		save->addChunk(npcsH[0]->getName());
		if (monies > 0) { //Section M, monies, only if we actually have any
			char sectionM[10];
			snprintf(sectionM, 10, "|M%d", monies);
			save->addChunk(sectionM);
		}
		//Section W, this is built during gameplay, so we just add it if we've done any changes
		if (strlen(sectionW)) {
			save->addChunk("|W");
			save->addChunk(sectionW);
		}
		//Section U, track pursuer if we have one
		if (NPC* pursuer = npcsH[0]->getPursuer()) {
			char sectionU[20];
			snprintf(sectionU, 20, "|U%d", pursuer->getRoom()->getID());
			save->addChunk(sectionU);
		}
		//Section R, track everything related to the teammates
		save->addChunk("|R");
		for (set<NPC*>::iterator teamiterator = recruited.begin(); teamiterator != recruited.end(); teamiterator++) {
			NPC* teammate = *teamiterator;
			char sectionR[100];
			//the teammate section  included the identifying char, then the level, xp, room id, and gym starting time
			snprintf(sectionR, 100, "%c%d.%d.%d", npcChar[teammate], teammate->getLevel(), teammate->getXP(), teammate->getRoom()->getID(), teammate->getGymStart());
			save->addChunk(sectionR);
			if (next(teamiterator) != recruited.end()) save->addChunk(","); //dividing comma
		}
		//Section P, save the player's party
		save->addChunk("|P"); //add the P section identifier then all their representing chars in order
		for (NPC* npc : npcsH[0]->getParty()) save->addChunk(&npcChar[npc]);
		//Section E, save all encountered enemy types
		save->addChunk("|E");
		for (set<NPC*>::iterator enemyiterator = encountered.begin(); enemyiterator != encountered.end(); enemyiterator++) {
			char sectionE[10];
			snprintf(sectionE, 10, "%d", (*enemyiterator)->getID());
			save->addChunk(sectionE);
			if (next(enemyiterator) != encountered.end()) save->addChunk(","); //dividing comma
		}
		//Section I, saves every item's position
		save->addChunk("|I");
		for (vector<Item*>::iterator itemiterator = itemsH.begin(); itemiterator != itemsH.end(); itemiterator++) {
			Item* item = *itemiterator;
			int roomid = (item->getRoom() ? item->getRoom()->getID() : -1)
			char sectionI[21];
			snprintf(sectionI, 21, "%d.%d", item->getID(), roomid);
			save->addChunk(sectionI);
			if (next(itemiterator) != itemsH.end()) save->addChunk(","); //dividing comma
		}
		//Section L, store lobster name and room MARK: implement this

		//Section T, store opened temple entrances, which is built during gameplay so we just record it
		if (strlen(sectionT)) {
			save->addChunk("|T");
			save->addChunk(sectionT);
		}
		//Section B, store the last time used the bank and current bank balance MARK: implement this

		//Section S, store all the world states except for never
		save->addChunk("|S");
		for (size_t i = 0; i < WorldState.size()-1; i++) {
			save->addChunk(WorldState[i] ? "1" : "0");
		}
		//Section Q, store how long the player has played on this file
		playtime += difftime(time(NULL), lastsave); //this might lose one second per save (command), but that's basically nothing so that's fine
		char sectionQ[100];
		snprintf(sectionQ, 100, "|Q%i", playtime)
		save->addChunk(sectionQ);
		//cap it off with a =
		save->addChunk("=");
	}

	//gets stuff which it can edit, and modifies the world to match the given save data
	static void LoadGame(const Save& save, NPC* player, std::vector<Item*>* inventory, int& monies) {
		//check for invalid saves (won't catch everything but you shouldn't be modifying save data anyway, this is mostly for accidentally copy/pasting wrong)
		//check version and that it starts with BQ2 and ends with =
		//we can always just stop loading partway into the loading, no need to do it all at the beginning

		//reset all the extern variables in Helper and Stats static trackers

	}

	Save(const char* _data) { //make a new save with the given save data
		while (strlen(_data) > savesize) {
			savesize *= 2;
		}
		data = new char[savesize]();
		data[0] = '\0';
		strcat(data, _data);
	}
};