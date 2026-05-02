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

using namespace Helper; //I got tired of typing Helper::

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
	*  - Worth mentioning, these aren't npcs but have their own letter:
	*    t - Florian	n - Banker	u - Buford
	*  - Then level, xp, room, stats (attacks launched, beneficial attacks launched, damage dealt, damage healed, damage recieved, health recovered, knockouts, times revived npc), and finally, time left in gym (0 if not in gym)
	*  - Everything connected to the NPC is seperated by .
	*  - So, this section would look something like: Rf[level].[xp].[room].[attacks].[helps].[damage dealt].[health dealt].[damage taken].[health recovered].[kos].[revives].[gym time]
	*  Section P: What the player's party actually is, using the above codes (e.g. ajbhl) would mean a party of Self, Michelin, Ratman, Henry Jerry, and Jilly in that order
	*  Section E: Enemy types that have been encountered
	*  Section I: Every item's room, or -1 if it's in the inventory (saved in order, so the items in the rooms and in the inventory remain in the same order) (-1 instead of 0 because 0 is a valid room ID, being limbo, so -1 translates to NULL room here)
	*  - Denoted by [item].[room]
	*  Section L: Exists after taming lobster, stores its name in plaintext with the same exceptions as the player name, and also the lobster's room after that (L[lobster name],[room])
	*  Section T: The room that each tunnel direction leads to, in the order of their index in the tunnel lobster tunnel directions, -1 if the exit hasn't been discovered yet
	*  - The order doesn't matter in this section because the exit maps in Room sorts the exits in alphabetical order anyway
	*  Section B: Exists after depositing money in the bank, stores current bank balance and last time used bank ([balance],[time])
	*  Section S: All the world states as 0 or 1 (26 states as of now, not including NEVER), since some states are changed by things other than world changes
	*  Section Q: Misc data, like how long the player has played on this save file, stored in seconds, then how many sessions there have been in this save, then how many times the player typed an invalid command, times gone in an invalid direction, times interacting with invalid npc, times interacted with invalid item, times entered nothing
	*  Section C: 18 integers representing how many times used each command (validly), so something like C[go],[take],[drop],[use],[recruit],[dismiss],[ask],[inventory],[party],[attacks],[room],[analyze],[fight],[buy],[help],[save],[enemies],[run]
	*  
	*  Example save: BQ2|V0|NBOB THE BUILDER|M10|Wu123r23,x3244,e300,a123.o|U156|Ra1.3.123.0|Pavk|E43,23,67,50|C1,2,3,4,5,6,7|I12.0,234.-1,324.0,53.2,13.0,4324.3,456.201|LCOOL LOBSTER,110|T1.123,2.12,0.112|B10,2|S01010101010101111010010101|Q2198139=
	*/

	/* MARK: TO DO LIST FOR SECTION WWWWWWW
	*  - Items that were bought and therefore duplicated, denoted by b[item]
	*  - Item usage in which room, denoted by u[item].[room] (including taking take to use world change items and taking manhole covers)
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
	*  - Opening a temple in a temple entrance room, denoted by t[room]*/

	/*MARK: ALSO
	all the npc data in section R
	invalid commands (figure it out in battle attacks)
	invalid npcs (same)

	*/

	size_t savesize = 1000; //we double the save size when we need to, starts with a reasonable length of 1000
	char* data; //the save data

	size_t savenum;

	//
	void reset() {
		delete[] data;
		savesize = 1000;
		data = new char[savesize]();
		data[0] = '\0';
	}

	//adds a chunk to the given char and extending the data length if needed
	void addChunk(const char* chunk, char* dest = NULL) {
		if (!dest) dest = data; //default to affecting the data
		while (strlen(dest) + strlen(chunk) >= savesize) {
			savesize *= 2;
			char* newdata = new char[savesize]();
			strcpy(newdata, dest);
			delete[] dest;
			dest = newdata;
		}
		strcat(dest, chunk);
	}

	//format the given name to have a \ before |, =, \, and , (the returned name must be deleted afterwards)
	static char* formatName(const char* _name) {
		char* name = new char[strlen(_name)*2+1](); //make a new name with enough space to fit the name with a \ before all its characters if necessary
		size_t len = strlen(_name); //get the length so we don't do strlen too much
		size_t p = 0; //the current position we're writing in since it can drift apart from i after addings \'s
		for (size_t i = 0; i < len; i++) { //checks all the characters of the _name individually
			char c = _name[i]; //get the char for easy access
			if (c == '\\' || c == '|' || c == '=' || c == ',') name[p++] = '\\'; //adds the \ if c is one of these given characters
			name[p++] = c; //then add c itself
		}
		name[p] = '\0'; //null terminate the thingy
		return name; //return the name we got!
	}

	//gets the variables in main and serializes it into save data
	static void SaveGame(Save* save, int monies, time_t lastsave) {
		save->reset(); //reset the current save string so we can rebuild it, easier to edit that way
		//add beginning and section V version number (we are in version 0)
		save->addChunk("BQ2|V0|N");
		//Section N, add the name
		{ char* name = formatName(Helper::npcsH[0]->getName()); //format the name to have escape codes so we can have special characters in the name
		save->addChunk(name);
		delete[] name; } //delete the name because it was made with new
		//Section M, monies, only if we actually have any
		if (monies > 0) {
			char sectionM[20];
			snprintf(sectionM, 20, "|M%d", monies);
			save->addChunk(sectionM);
		}
		//Section W, this is built during gameplay, so we just add it if we've done any changes
		if (strlen(Helper::sectionW)) {
			save->addChunk("|W");
			save->addChunk(Helper::sectionW);
		}
		//Section U, track pursuer if we have one
		if (NPC* pursuer = Helper::npcsH[0]->getPursuer()) {
			char sectionU[20];
			snprintf(sectionU, 20, "|U%d", pursuer->getRoom()->getID());
			save->addChunk(sectionU);
		}
		//Section R, track everything related to the teammates
		save->addChunk("|R");
		for (set<NPC*>::iterator teamiterator = Helper::recruited.begin(); teamiterator != Helper::recruited.end(); teamiterator++) {
			NPC* teammate = *teamiterator;
			char sectionR[100];
			//the teammate section included the identifying char, then the level, xp, room id, and gym starting time
			snprintf(sectionR, 100, "%c%d.%d.%d.%d.%d.%lld.%lld.%lld.%lld.%d.%d.%lld", npcChar[teammate], teammate->getLevel(), teammate->getXP(), teammate->getRoom()->getID(), attackslaunched[teammate], helpslaunched[teammate], damagedealt[teammate], healthhealed[teammate], damagerecieved[teammate], healthrecovered[teammate], knockouts[teammate], revives[teammate], (long long)teammate->getGymStart());
			save->addChunk(sectionR);
			if (next(teamiterator) != Helper::recruited.end()) save->addChunk(","); //dividing comma
		}
		//Section P, save the player's party
		{char team[12] = "|P"; //build the team here starting with the P section identifier then all their representing chars in order
		size_t plen = Helper::npcsH[0]->getParty()->size();
		for (size_t i = 0; i < plen; i++) { //add all the party characters to the string
			team[i+2] = Helper::npcChar[(*Helper::npcsH[0]->getParty())[i]];
		}
		team[plen+2] = '\0'; //null terminate it!
		save->addChunk(team);} //add the team chunk
		//Section E, save all encountered enemy types
		save->addChunk("|E");
		for (set<NPC*>::iterator enemyiterator = Helper::encountered.begin(); enemyiterator != Helper::encountered.end(); enemyiterator++) {
			char sectionE[10];
			snprintf(sectionE, 10, "%d", (*enemyiterator)->getID());
			save->addChunk(sectionE);
			if (next(enemyiterator) != Helper::encountered.end()) save->addChunk(","); //dividing comma
		}
		//Section I, saves every item's position
		save->addChunk("|I");
		for (vector<Item*>::iterator itemiterator = Helper::itemsH.begin(); itemiterator != Helper::itemsH.end(); itemiterator++) {
			Item* item = *itemiterator;
			int roomid = (item->getRoom() ? item->getRoom()->getID() : -1);
			char sectionI[21];
			snprintf(sectionI, 21, "%d.%d", item->getID(), roomid);
			save->addChunk(sectionI);
			if (next(itemiterator) != Helper::itemsH.end()) save->addChunk(","); //dividing comma
		}
		//Section L, store lobster name and room
		save->addChunk("|L");
		NPC* lobster = Helper::charNPC['t']; //get the lobster NPC* for easier use
		if (strlen(lobster->getTitle())) { //if the lobster doesn't have a title, we don't record a name (because that's how the game knows to not change the original name), also accounting for people who named their tunnel lobster TUNNEL LOBSTER for some reason
			char* lname = formatName(lobster->getName()); //same process as player name
			save->addChunk(lname);
			delete[] lname;
		}
		char lobroom[11]; //get the lobster' room (plus the dividing comma since it's convenient)
		snprintf(lobroom, 11, ",%d", lobster->getRoom()->getID());
		save->addChunk(lobroom);
		//Section T, store opened temple entrances, which is built during gameplay so we just record it
		if (strlen(Helper::sectionT)) {
			save->addChunk("|T");
			for (size_t i = 0; i < 5; i++) { //there are five tunnel directions so do one check for each one
				if (sectionT[i]) { //if the direction has been discovered
					char dir[10]; //put the direction into the thingy
					snprintf(dir, 10, "%d");
					save->addChunk(dir); //add the thingy to the save in section T which is the current section of course
				} else { //the tunnel direction has not been discovered yet so we add a sentinel -1 representing NULL
					save->addChunk("-1");
				}
				if (i < 4) save->addChunk(","); //add the dividing comma if it's not the last one
			}
			save->addChunk(sectionT);
		}
		//Section B, store current bank balance and the last time used the bank
		NPC* banker = Helper::charNPC['n']; //get the banker
		if (!banker->getConvoSize()) {
			char bankstuff[100]; //print the bank data into this charray
			snprintf(bankstuff, 100, "|B%d,%lld", banker->getBalance(), (long long)banker->getDepositTime());
			save->addChunk(bankstuff); //add the banking section!
		}
		//Section S, store all the world states except for never
		save->addChunk("|S");
		for (size_t i = 0; i < Helper::NEVER; i++) {
			save->addChunk(Helper::WorldState[i] ? "1" : "0");
		}
		//Section Q, store how long the player has played on this file plus some silly error data
		playtime += difftime(time(NULL), lastsave); //this might lose one second per save (command), but that's basically nothing so that's fine
		char sectionQ[100];
		snprintf(sectionQ, 100, "|Q%.0f,%d,%d,%d,%d,%d,%d", playtime, sessions, invalidcommand, invalidmove, invalidnpc, invaliditem, nothingtosay);
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

		//while checking used items, track "discontinuity" which is incremented each time an item is consumed. every time we check an index in itemsH that is greater than itemsH.size()-1, subtract that index by the discontinuity, and that should give the actual item
	}

	Save(const char* _data, size_t num) { //make a new save with the given save data and identifying number
		while (strlen(_data) > savesize) {
			savesize *= 2;
		}
		data = new char[savesize]();
		data[0] = '\0';
		strcat(data, _data);
		//MARK: truncate whitespace?
	}
};