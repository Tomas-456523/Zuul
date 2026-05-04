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
	*  - Items that were bought and therefore duplicated, and the room they were bought in, denoted by b[item].[room]
	*  - Item usage in which room, denoted by u[item].[room] (including taking take to use world change items and taking manhole covers)
	*      - If it was a choice orb item, we also store which choice was chosen appended with a . (e.g. u[item].[room].b)
	*  - Items that were used up in battle, denoted by x[item]
	*  - NPCs that were defeated, denoted by d[npc]
	*  - NPCs that caused respawn changes, denoted by r[npc]
	*  - NPCs that were talked to, denoted by a[npc], with an optional modifier for different types of dialogue (.r for recruitment dialogue, .e for recruited dialogue, .j for rejection dialogue, .d for dismissal dialogue, .i for dismissal rejection, .o for opening, .g for gym)
	*  - Enter changes, denoted by e[room]
	*  - Room welcomes that were triggered, denoted by w[room]
	*  - Dropped escape orbs, denoted by o[item]
	*  - If the player was caught by a pursuer, denoted by p[npc]
	*  - Opening a temple in a temple entrance room, denoted by t[room]
	*  Section U: Only exists while pursuer is pursuing, tracks their room
	*  Section R: Stuff related to the player team
	*  - First, the teammate this chunk is tracking:
	*    a - Bernard	l - Jilly	h - Henry Jerry
	*    f - Floria		m - Mike	j - Michelin	g - Graham
	*    e - Egadwick	v - Viola	x - Carlos		r - Richie
	*    k - Absolom	c - Cacty	p - Plum		b - Ratman
	*  - Worth mentioning, these aren't teammates but have their own letter:
	*    t - Florian	n - Banker	u - Buford (buford's stats are tracked)
	*  - Then level, xp, room, stats (attacks launched, beneficial attacks launched, damage dealt, damage healed, damage recieved, health recovered, knockouts, times got ko'd, a special stat, and sp usage), and finally, time left in gym (0 if not in gym)
	*  - Everything connected to the NPC is seperated by .
	*  - So, this section would look something like: R[level].[xp].[room].[attacks].[helps].[damage dealt].[health dealt].[damage taken].[health recovered].[kos].[ko'd].[special].[sp usage].[gym time]
	*  Section P: What the player's party actually is (apart from the player), using the above codes (e.g. jbhl) would mean a party of Self, Michelin, Ratman, Henry Jerry, and Jilly in that order
	*  Section E: Enemy types that have been encountered
	*  Section I: Every item's room, or -1 if it's in the inventory (saved in order, so the items in the rooms and in the inventory remain in the same order) (-1 instead of 0 because 0 is a valid room ID, being limbo, so -1 translates to NULL room here)
	*  - Denoted by [item].[room]
	*  Section L: Exists after taming lobster, stores its name in plaintext with the same exceptions as the player name, and also the lobster's room after that (L[lobster name],[room])
	*  Section T: The room that each tunnel direction leads to, in the order of their index in the tunnel lobster tunnel directions, -1 if the exit hasn't been discovered yet
	*  - The order of exit setting doesn't matter in this section because the exit maps in Room sorts the exits in alphabetical order anyway
	*  Section B: Exists after depositing money in the bank, stores current bank balance and last time used bank ([balance],[time])
	*  Section S: All the world states as 0 or 1 (26 states as of now, not including NEVER), since some states are changed by things other than world changes
	*  Section Q: Misc data, like how long the player has played on this save file, stored in seconds, then how many sessions there have been in this save, then how many times the player typed an invalid command, times gone in an invalid direction, times interacting with invalid npc, times interacted with invalid item, times entered nothing, biggest sp bomb, successful parries
	*  Section C: 18 integers representing how many times used each command (validly), so something like C[go],[take],[drop],[use],[recruit],[dismiss],[ask],[inventory],[party],[attacks],[room],[analyze],[fight],[buy],[help],[save],[enemies],[run]
	*  
	*  Example save: BQ2|V0|NBOB THE BUILDER|M10|Wu123.23,x3244,e300,a123.o|U156|Ra1.3.123.0|Pavk|E43,23,67,50|C1,2,3,4,5,6,7|I12.0,234.-1,324.0,53.2,13.0,4324.3,456.201|LCOOL LOBSTER,110|T1.123,2.12,0.112|B10,2|S01010101010101111010010101|Q2198139=
	*/

	size_t savesize = 1000; //we double the save size when we need to, starts with a reasonable length of 1000
	char* data; //the save data

	size_t savenum;

	//make sure it's a valid save before calling this
	char getVersion() {
		return data[5];
	}

	//makes sure the save's data is valid and perfectly parsable with no issues, so I can be sure the string is good when I start loading
	bool getValid() {
		if (strncmp(data, "BQ2|V", 4)) return false;
		if (data[strlen(data)-1] != '=') return false;
		
		//unimplemented right now because Im only using my own saves currently made in the game so they should work fine

		//MARK: also make sure each section is only there once
		
		//I changed my mind I should enforce section order
		//at least make sure R is after W, I after W, etc. and make sure all similar cases are accounted for
		//u after w

		return true; //yay everything worked so it's valid!
	}

	//
	void reset() {
		delete[] data;
		savesize = 1000;
		data = new char[savesize]();
		data[0] = '\0';
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

	//gets the variables in main and serializes it into save data MARK: save game
	static void SaveGame(Save* save, int monies, time_t lastsave) {
		save->reset(); //reset the current save string so we can rebuild it, easier to edit that way
		//add beginning and section V version number (we are in version 0)
		addChunk(save->data, "BQ2|V0|N");
		//Section N, add the name
		{ char* name = formatName(Helper::charNPC['a']->getName()); //format the name to have escape codes so we can have special characters in the name
		addChunk(save->data, name);
		delete[] name; } //delete the name because it was made with new
		//Section M, monies, only if we actually have any
		if (monies > 0) {
			char sectionM[20];
			snprintf(sectionM, 20, "|M%d", monies);
			addChunk(save->data, sectionM);
		}
		//Section W, this is built during gameplay, so we just add it if we've done any changes
		if (Helper::sectionW) { //if it's been edited so it's not null
			addChunk(save->data, "|W");
			addChunk(save->data, Helper::sectionW);
		}
		//Section U, track pursuer if we have one
		if (NPC* pursuer = Helper::charNPC['a']->getPursuer()) {
			char sectionU[20];
			snprintf(sectionU, 20, "|U%d", pursuer->getRoom()->getID());
			addChunk(save->data, sectionU);
		}
		//Section R, track everything related to the teammates
		addChunk(save->data, "|R");
		for (set<NPC*>::iterator teamiterator = Helper::Rnpcs.begin(); teamiterator != Helper::Rnpcs.end(); teamiterator++) {
			NPC* teammate = *teamiterator;
			char sectionR[1000]; //this is prolly enough space
			//the teammate section included the identifying char, then the level, xp, room id, attacks launched, helps launched, damage dealt, heals dealt, damage taken, health recovered, knockouts, incapacitations, a special stat which varies by npc, total sp used up by pecial attacks, and gym starting time
			snprintf(sectionR, 1000, "%c%d.%d.%d.%d.%d.%d.%lld.%lld.%lld.%lld.%d.%d.%d.%lld.%lld", npcChar[teammate], teammate->getLevel(), teammate->getXP(), teammate->getRoom()->getID(), attackslaunched[teammate], helpslaunched[teammate], damagedealt[teammate], healthhealed[teammate], damagerecieved[teammate], healthrecovered[teammate], knockouts[teammate], incapacitations[teammate], specialstat[teammate], spusedup[teammate], (long long)teammate->getGymStart());
			addChunk(save->data, sectionR);
			if (next(teamiterator) != Helper::Rnpcs.end()) addChunk(save->data, ","); //dividing comma
		}
		//Section P, save the player's party
		{ char team[12] = "|P"; //build the team here starting with the P section identifier then all their representing chars in order
		size_t plen = Helper::charNPC['a']->getParty()->size();
		for (size_t i = 1; i < plen; i++) { //add all the party characters to the string (skipping the player because of course they're in the party)
			team[i+2] = Helper::npcChar[(*Helper::charNPC['a']->getParty())[i]];
		}
		team[plen+2] = '\0'; //null terminate it!
		addChunk(save->data, team); } //add the team chunk
		//Section E, save all encountered enemy types
		addChunk(save->data, "|E");
		for (set<NPC*>::iterator enemyiterator = Helper::encountered.begin(); enemyiterator != Helper::encountered.end(); enemyiterator++) {
			char sectionE[10];
			snprintf(sectionE, 10, "%d", (*enemyiterator)->getID());
			addChunk(save->data, sectionE);
			if (next(enemyiterator) != Helper::encountered.end()) addChunk(save->data, ","); //dividing comma
		}
		//Section I, saves every item's position
		addChunk(save->data, "|I");
		for (vector<Item*>::iterator itemiterator = Helper::itemsH.begin(); itemiterator != Helper::itemsH.end(); itemiterator++) {
			Item* item = *itemiterator;
			int roomid = (item->getRoom() ? item->getRoom()->getID() : -1);
			char sectionI[21];
			snprintf(sectionI, 21, "%d.%d", item->getID(), roomid);
			addChunk(save->data, sectionI);
			if (next(itemiterator) != Helper::itemsH.end()) addChunk(save->data, ","); //dividing comma
		}
		//Section L, store lobster name and room
		NPC* lobster = Helper::charNPC['t']; //get the lobster NPC* for easier use
		if (!lobster->getLeader()) { //only do section L if the lobster has been tamed
			addChunk(save->data, "|L");
			if (strlen(lobster->getTitle())) { //if the lobster doesn't have a title, we don't record a name (because that's how the game knows to not change the original name), also accounting for people who named their tunnel lobster TUNNEL LOBSTER for some reason
				char* lname = formatName(lobster->getName()); //same process as player name
				addChunk(save->data, lname);
				delete[] lname;
			}
			char lobroom[11]; //get the lobster' room (plus the dividing comma since it's convenient)
			snprintf(lobroom, 11, ",%d", lobster->getRoom()->getID());
			addChunk(save->data, lobroom);
		}
		//Section T, store found tunnel directions
		if (sectionT[0] || sectionT[1] || sectionT[2] || sectionT[3] || sectionT[4]) { //only track if any of them have actually been used
			addChunk(save->data, "|T");
			for (size_t i = 0; i < 5; i++) { //there are five tunnel directions so do one check for each one
				if (sectionT[i]) { //if the direction has been discovered
					char dir[10]; //put the direction into the thingy
					snprintf(dir, 10, "%d", sectionT[i]->getID());
					addChunk(save->data, dir); //add the thingy to the save in section T which is the current section of course
				} else { //the tunnel direction has not been discovered yet so we add a sentinel -1 representing NULL
					addChunk(save->data, "-1");
				}
				if (i < 4) addChunk(save->data, ","); //add the dividing comma if it's not the last one
			}
		}
		//Section B, store current bank balance and the last time used the bank
		NPC* banker = Helper::charNPC['n']; //get the banker
		if (!banker->getConvoSize()) {
			char bankstuff[100]; //print the bank data into this charray
			snprintf(bankstuff, 100, "|B%d,%lld", banker->getBalance(), (long long)banker->getDepositTime());
			addChunk(save->data, bankstuff); //add the banking section!
		}
		//Section S, store all the world states except for never
		addChunk(save->data, "|S");
		for (size_t i = 0; i < Helper::NEVER; i++) {
			addChunk(save->data, Helper::WorldState[i] ? "1" : "0");
		}
		//Section Q, store how long the player has played on this file plus some silly error data plus some other potentially interesting stats
		playtime += difftime(time(NULL), lastsave); //this might lose one second per save (command), but that's basically nothing so that's fine
		char sectionQ[1000];
		snprintf(sectionQ, 1000, "|Q%.0f,%d,%d,%d,%d,%d,%d,%d,%d", playtime, sessions, invalidcommand, invalidmove, invalidnpc, invaliditem, nothingtosay, biggestspbomb, successfulparries);
		addChunk(save->data, sectionQ);
		//cap it off with a =
		addChunk(save->data, "=");
	}

	//parse name and advance save data pointer past it in the process, assumes it's given a valid save
	static bool ParseName(NPC* named, const char*& data) {
		bool escaping = false;
		char* name = new char[255];

		for (size_t i = 0;; data++) {
			if (!escaping && *data == '\\') {
				escaping = true;
			} else if (!escaping && (*data == ',' || *data == '=' || *data == '|')) {
				name[i] = '\0';
				break; //we have reached the end of the name and we're in some other section now
			} else {
				name[i++] = *data;
				escaping = false;
			}
		}
		named->setName(name);
		delete[] name;
	}

	//parse the number in the data currently and advance the pointer past it
	long long ParseNum(const char*& data) {
		int sign = 1;
		if (*data == '-') {
			sign = -1;
			data++;
		}
		long long total = 0;
		for (; isdigit((unsigned char)*data); data++) {
			total = total * 10 + (*data - '0');
		}
		return total * sign;
	}

	//adjust item id to get their index in itemsH, accounting for deleted items
	static int adjustItemID(int id, set<int>& discontinuities) {
		int discount = distance(discontinuities.begin(), discontinuities.lower_bound(id));
		return id - discount;
	}

	//gets stuff which it can edit, and modifies the world to match the given save data MARK: load game
	static void LoadGame(const Save* save, NPC* player, std::vector<Item*>* inventory, int& monies) {
		//check for invalid saves (won't catch everything but you shouldn't be modifying save data anyway, this is mostly for accidentally copy/pasting wrong)
		//check version and that it starts with BQ2 and ends with =

		emptyExterns(); //make sure everything in helper is all reset

		const char* data = save->data;

		//after validating the save, assume it's all perfectly parsable (not necessarily correct, just parsable)
		data += 4; //skip the BQ2|

		set<int> discontinuity; //the item id discontinuities, because we delete items sometimes so

		while (*data != '=') { //keep loading stuff until we reach the end
			if (*data == '|') { //just go past the dividers
				data++;
			} else if (*data == 'V') { //ignore version section, if it's a future version just check outside the function

			} else if (*data == 'N') { //n for name
				if (*(++data) != '|') ParseName(player, ++data);
			} else if (*data == 'M') { //m for monies
				monies = ParseNum(++data);
			} else if (*data == 'W') { //w for world change and everything that can cause it, most important section
				//MARK: make sure to update sectionW log
				if (*(++data) == 'b') { //duplicate item because it was bought
					Item* item = 
					//make it do the conversation changes
				} else if (*data == 'u') { //use item in world
					//MARK: keep in mind at this point we don't know where any of the items are or if they're in the inventory
					//xp
					//burger
					//education
					//caller
					//key/hose
					//paver
					//treasure
					//switch
					//worldchange (take and normal)
					//blender
					//choiceorb (skip for now) MARK: do this
					//manhole (taking)

					//UPDATE DISCONTINUITY
				} else if (*data == 'x') { //delete item because it was used in battle
					//unroom it first
					//UPDATE DISCONTINUITY
				} else if (*data == 'd') { //defeat defeated enemies
					//defeat() logs it automatically
				} else if (*data == 'r') { //handle respawn changes
					//undefeat() also logs it automatically
				} else if (*data == 'a') { //(.r for recruitment dialogue, .e for recruited dialogue, .j for rejection dialogue, .d for dismissal dialogue, .i for dismissal rejection, .o for opening, .g for gym)
					//do the changes, logging is done by npc automatically
				} else if (*data == 'e') { //room enter changes
					//this function also logs the thing automatically
				} else if (*data == 'w') { //do room welcomes
					//printWelcome logs it automatically
				} else if (*data == 'o') {
					//o - ignore for now drop escape orb MARK: WE NEED TO HANDLE ESCAPE ORBS
				} else if (*data == 'p') { //player got caught by pursuer

				} else if (*data == 't') { //open temples
					//openTemple logs that the temple was opened automatically
				}
			} else if (*data == 'U') { //u for second letter of pursuer
				player->getPursuer()->setRoom(roomsH[ParseNum(++data)]); //just set their room
			} else if (*data == 'R') { //r for recruitable, tracks recruitable npcs' stats (and Buford's)
				for (; *data != '|' && *data != '='; data++) {
					NPC* npc = charNPC[*(++data)];
					npc->setLevel(ParseNum(++data));
					npc->addXp(ParseNum(++data));
					npc->setRoom(roomsH[ParseNum(++data)]);
					attackslaunched[npc] = ParseNum(++data);
					helpslaunched[npc] = ParseNum(++data);
					damagedealt[npc] = ParseNum(++data);
					healthhealed[npc] = ParseNum(++data);
					damagerecieved[npc] = ParseNum(++data);
					healthrecovered[npc] = ParseNum(++data);
					knockouts[npc] = ParseNum(++data);
					incapacitations[npc] = ParseNum(++data);
					specialstat[npc] = ParseNum(++data);
					spusedup[npc] = ParseNum(++data);
					npc->setGymStart(ParseNum(++data));
					Rnpcs.push_back(npc);
				}
			} else if (*data == 'P') { //p for party
				while (*(++data) != '|') { //add the teammates until we reach the end
					player->setParty(charNPC(*data));
				}
			} else if (*data == 'E') { //e for encountered enemies
				while (*data != '|' && *data != '=') {
					encountered->insert(npcsH[ParseNum(++data)]);
				}
			} else if (*data == 'I') { //i for items, set their rooms
				while (*data != '|' && *data != '=') {
					Item item = itemsH[adjustItemID(ParseNum(++data), discontinuity)];
					int roomid = ParseNum(++data);
					if (roomid != -1) { //-1 means NULL room which means they go in the inventory
						item->unRoom();
						inventory->push_back(item);
						//make sure the player has their weapon items' attacks
						if (!strcmp(item->getType(), "weapon")) player->addSpecialAttack(((WeaponItem*)item)->getAttack());
					} else { //otherwise set the item to the room
						item->setRoom(roomsH[roomid]);
						//block exits that may be blocked by a hose being in the room
						if (!strcmp(item->getType(), "hose")) ((HoseItem*)item)->drop(roomsH[roomid]);
					} 
				}
			} else if (*data == 'L') { //l for lobster
				NPC* lobster = charNPC['t'];
				if (*(++data) != ',') ParseName(charNPC['t'], ++data);
				else charNPC['t']->setTitle("TUNNEL LOBSTER"); //tunnel lobster only has the title if they have were named
				lobster->setRoom(roomsH[ParseNum(++data)]); //place the lobster in its room
			} else if (*data == 'T') { //t for tunnels, set tunnel directions
				NPC* lobster = charNPC['t'];
				for (size_t i = 0; i < 5; i++) {
					int id = ParseNum(++data);
					if (id < 0) sectionT[i] = NULL; //no exit, just set the thing in sectionT
					else { //yes exit, set the room that it goes to
						Room* room = roomsH[id];
						sectionT[i] = room;
						lobster->paveTunnel(room, i); //set this tunnel exit to go to this room specifically
					}
				}
			} else if (*data == 'B') { //b for bank, set bank data
				charNPC['n']->manualSetBankData(ParseNum(++data), ParseNum(++data)); //set the bank balance and the bank time
			} else if (*data == 'S') { //s for states, set all the world states
				for (size_t i = 0; *(++data) != '|' && *data != '='; i++) { //set all the states except for NEVER because that's not tracked by SaveWorld
					WorldState[i] = *data - '0';
				}
			} else if (*data == 'Q') { //q for it's the randomest letter I could think of, random stats
				playtime = ParseNum(++data); //yeah just set all the stats pretty simple
				sessions = ParseNum(++data);
				invalidcommand = ParseNum(++data);
				invalidmove = ParseNum(++data);
				invalidnpc = ParseNum(++data);
				invaliditem = ParseNum(++data);
				nothingtosay = ParseNum(++data);
				biggestspbomb = ParseNum(++data);
				successfulparries = ParseNum(++data);
			} else if (*data == 'C') { //c for commands, probably (might be a coincidence)
				for (size_t i = 0; i < 18; i++) { //set all the amounts
					commandcount[i] = ParseNum(++data);
				}
			}
		}

		//while checking used items, track "discontinuity" which is incremented each time an item is consumed. every time we check an index in itemsH that is greater than itemsH.size()-1, subtract that index by the discontinuity, and that should give the actual item
	}

	Save(const char* _data, size_t num) { //make a new save with the given save data and identifying number
		while (strlen(_data) > savesize) {
			savesize *= 2;
		}
		data = new char[savesize]();
		data[0] = '\0';
		strcat(data, _data);
		savenum = num;
		//MARK: truncate whitespace?
	}

	~Save() { //delete the data on destruction
		delete[] data;
	}
};