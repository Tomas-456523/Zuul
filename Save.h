//header file for the world saves

#ifndef SAVE
#define SAVE

#include <vector>
#include <cstring>
#include <fstream>
#include <iterator>
#include <ctime>
#include <cmath>
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
	*  - Item usage in which room, denoted by u[item].[room] (including taking take to use world change items and taking manhole covers)
	*      - If it was a choice orb item, we also store which choice was chosen appended with a . (e.g. u[item].[room].b)
	*  - Items that were used up in battle, denoted by x[item]
	*  - NPCs that were defeated, denoted by d[npc]
	*  - NPCs that caused respawn changes, denoted by r[npc]
	*  - NPCs that were talked to, denoted by a[npc], with an optional modifier for different types of dialogue (.r for recruitment dialogue, .e for recruited dialogue, .j for rejection dialogue, .d for dismissal dialogue, .i for dismissal rejection, .o for opening, .g for gym)
	*  - Enter changes, denoted by e[room]
	*  - Room welcomes that were triggered, denoted by w[room]
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
	*  Section S: All the world states as 0 or 1 (27 states as of now, not including NEVER or GAMEEND), since some states are changed by things other than world changes
	*  Section Q: Misc data, like how long the player has played on this save file, stored in seconds, then how many sessions there have been in this save, then how many times the player typed an invalid command, times gone in an invalid direction, times interacting with invalid npc, times interacted with invalid item, times entered nothing, biggest sp bomb, successful parries
	*  Section C: 18 integers representing how many times used each command (validly), so something like C[go],[take],[drop],[use],[recruit],[dismiss],[ask],[inventory],[party],[attacks],[room],[analyze],[fight],[buy],[help],[save],[enemies],[run]
	*  
	*  Example save: BQ2|V0|NBOB THE BUILDER|M10|Wu123.23,x3244,e300,a123.o|U156|Ra1.3.123.0|Pavk|E43,23,67,50|C1,2,3,4,5,6,7|I12.0,234.-1,324.0,53.2,13.0,4324.3,456.201|LCOOL LOBSTER,110|T1.123,2.12,0.112|B10,2|S01010101010101111010010101|Q2198139=
	*/

	size_t savesize = 1000; //we double the save size when we need to, starts with a reasonable length of 1000
	char* data; //the save data

	size_t savenum; //identifying number for convenience

	//reset the save data to starting stuff MARK: reset
	void reset() {
		delete[] data;
		savesize = 1000;
		data = new char[savesize](); //make sure we have something to write into
		data[0] = '\0';
	}

	//verify that the given number is valid, usable for signed <=64 bit integers or unsigned <64 bit integers MARK: verify number
	//also mutates the passed pointer because it's convenient for me
	static bool verifyNum(const char*& data, int bits = 32, bool _sign = true) { //can choose limits of the bits, since the save data has ints and long longs
		unsigned long long total = 0; //track the total using an unsigned long
		unsigned long long intlimit = 1ULL << (bits-_sign); //get the maximum by shifting 1 to the left by the amount of bits, which works since binary's digits track powers of 2
		int sign = 1;
		if (*data == '-') { //checks if we are signing the thingy
			if (!_sign) return false;
			sign = -1;
			data++;
		}
		int digits = 0; //also track digits so we don't overflow the total tracker
		int maxdigits = (bits-_sign)*log10(2)+1;
		for (; isdigit((unsigned char)*data); data++) {
			int digit = *data - '0'; //the digit we are adding
			if (total > (intlimit-(sign+1)/2-digit)/10) return false; //do some math to make sure adding this new digit won't over or underflow, and of course if it will, retun false cause it's bad
			total = total * 10 + digit; //move the total's places forward and add the digit to the ones place
			digits++; //increment digits because we added a digit
		}
		if (!digits) return false; //there wasn't any number...
		return true; //we have checked that it's within the limit and everything already so we know it's true!
	}

	//verify that a name is all goot and dandy and stuff, like properly escaped characters MARK: verify name
	static bool verifyName(const char*& data) { //leaves off the pointer at the position after the name
		bool escaping = false;
		size_t namesize = 0; //get the size of the name so we can make sure it's not too long

		for (; *data != '\0'; data++) {
			if (namesize >= 254) { //if the name was too long
				return false;
			} else if (!escaping && *data == '\\') {
				escaping = true;
			} else if (!escaping && (*data == ',' || *data == '=' || *data == '|')) {
				return true; //we reached the end of the name
			} else if (escaping && *data != ',' && *data != '=' && *data != '|' && *data != '\\') {
				return false; //escaping non-escapable character is bad
			} else { //we're adding a normal character and everything seems good so we reset escaping and go to the next char
				escaping = false;
				namesize++; //add one to the name length cause this is a normal character
			}
		}
		return false; //the name goes off the edge of the save data so it's bad
	}

	//makes sure the save's data is valid and perfectly parsable with no issues, so I can be sure the string is good when using it MARK: get valid
	//make sure to check save validity before doing basically anything with the save data, because I assume valid data for most of the functions. Setting functions like reset and SaveGame don't need this check
	//this checks parsability, save data could still be incorrect and segfault or something, but if that happens it should be the player's fault for editing their data manually
	bool getValid() {
		//to start, we at least know the save data is properly null-terminated
		if (*data == '\0') return false; //if it doesn't have anything then it's invalid
		if (strncmp(data, "BQ2|", 4)) return false; //must start and end like this
		if (data[strlen(data)-1] != '=') return false;

		std::set<char> sections; //sections that we have found already
		
		for (const char* p = data+4; *p != '=';) { //start right after the BQ2| until reaching the =
			if (*p != '|') { //checks stuff if it's not a divider
				if (sections.count(*p)) return false; //no duplicate sections
				sections.insert(*p); //track that we have checked this section so we can verify there aren't any duplicates using the check immediately before this
			} else { //skip the dividers, they are already checked during the sections and at the end
				p++;
			}
			if (*p == 'V') {
				p += 2; //skip the version number
			} else if (*p == 'N') {
				if (!verifyName(++p)) return false; //the name was invalid for whatever reason
			} else if (*p == 'M') { //check if the monies amount is valid
				if (!verifyNum(++p, 32)) return false;
			} else if (*p == 'W') {
				if (sections.count('U') || sections.count('R') || sections.count('I') || sections.count('S')) return false; //section W must precede these sections because otherwise it could overwrite stuff they set, or they could be missing data
				for (p++;; p++) {
					if (strchr("bxdrewpt", *p)) { //if it's one of these subsections we just need to check one id, so I can group these together as one check
						if (!verifyNum(++p)) return false; //check item/npc/room id
					} else if (*p == 'u') {
						if (!verifyNum(++p)) return false; //check item id
						if (*p != '.') return false; //check connecting period
						if (!verifyNum(++p)) return false; //check room id
						if (*p == '.') { //check the optional point for choice orbs
							if (*(++p) != 'a' && *p != 'b') return false;
							p++; //MARK: check if this is a choice orb item?
						}
					} else if (*p == 'a') {
						if (!verifyNum(++p)) return false; //check npc id
						if (*p == '.') { //the optional conversation type, check that it's a valid type
							if (!strchr("rejdiog", *(++p))) return false;
							//MARK: BRANCHING PATHS
							p++; //make sure we're at the position after this subsection
						}
					}
					if (*p == '|' || *p == '=') break; //break because it's the end of the world changes section
					if (*p != ',') return false; //check dividing comma
				}
			} else if (*p == 'U') {
				if (!verifyNum(++p, 32)) return false; //check for pursuer room id
			} else if (*p == 'R') { //check teammate stats
				//level, xp, room, attacks launched, beneficial attacks launched, damage dealt, damage healed, damage recieved, health recovered, knockouts, times got ko'd, a special stat, and sp usage, and finally, time left in gym (0 if not in gym)
				const char* validChars = "afeklmvcjxpgrbhu";
				for (p++;; p++) {
					if (!strchr(validChars, *p)) return false; //can only track these specified characters
					for (size_t i = 0; i < 5; i++) { //check level, xp, room id, attacks, and helps
						if (!verifyNum(++p, 32)) return false;
					}
					for (size_t i = 0; i < 4; i++) { //check damage dealt, damage healed, damage recieved, and health recovered
						if (!verifyNum(++p, 64)) return false;
					}
					for (size_t i = 0; i < 3; i++) { //check kos, times ko'd, and special stat
						if (!verifyNum(++p, 32)) return false;
					}
					for (size_t i = 0; i < 2; i++) { //check sp usage and gym time
						if (!verifyNum(++p, 64)) return false;
					}
					if (*p == '|' || *p == '=') break; //break because it's the end of the team stats section
					if (*p != ',') return false; //check dividing comma
				}
			} else if (*p == 'P') { //player party
				const char* validChars = "feklmvcjxpgrbh"; //no buford because he's not recruited technically
				for (p++; *p != '|' && *p != '='; p++) { //start at the first char and go until we reach the end of the party section
					if (!strchr(validChars, *p)) return false; //check that all the party members are recruitable teammates
				}
			} else if (*p == 'E') { //enemy types
				for (;;) {
					if (!verifyNum(++p, 32)) return false; //check npc id
					if (*p == '|' || *p == '=') break; //we've reached the end of section E
					if (*p != ',') return false; //check dividing comma
				}
			} else if (*p == 'I') { //items
				for (;;) {
					if (!verifyNum(++p, 32)) return false; //check item id
					if (*p != '.') return false; //check connecting period
					if (!verifyNum(++p, 32)) return false; //check item room id
					if (*p == '|' ||*p == '=') break; //we've reached the end of section I
					if (*p != ',') return false; //check dividing comma
				}
			} else if (*p == 'L') { //lobster name then room
				if (!verifyName(++p)) return false; //check lobster name
				if (*p != ',') return false; //check the dividing comma
				if (!verifyNum(++p, 32)) return false; //check lobster room
			} else if (*p == 'T') { //tunnel directions
				for (size_t i = 0; i < 5; i++) {
					if (!verifyNum(++p, 32)) return false; //check room id
					if (i < 4 && *p != ',') return false; //check the dividing comma
				}
			} else if (*p == 'B') { //bank balance then time
				if (!verifyNum(++p, 32)) return false; //check balance
				if (*p != ',') return false; //check the dividing comma
				if (!verifyNum(++p, 64)) return false; //check the deposit time
			} else if (*p == 'S') { //states
				for (size_t i = 0; i < GAMEEND; i++) {
					if (*(++p) != '0' && *p != '1') return false; //must be a bitstring of GAMEEND characters long
				}
				p++; //go past the bitstring
			} else if (*p == 'Q') { //play time, how many sessions there have been in this save, then how many times the player typed an invalid command, times gone in an invalid direction, times interacting with invalid npc, times interacted with invalid item, times entered nothing, biggest sp bomb, successful parries
				if (!verifyNum(++p, 64)) return false; //check playtime, I know it's a double but a 64 bit int should be plenty, it's literally more seconds than the universe is old so yeah
				for (size_t i = 0; i < 5; i++) { //check sessions then all the invalid trackers
					if (*p != ',') return false; //check the dividing comma
					if (!verifyNum(++p, 32)) return false;
				}
				if (*p != ',') return false; //check the dividing comma
				if (!verifyNum(++p, 64)) return false; //check times entered nothing
				for (size_t i = 0; i < 2; i++) { //check biggest sp bomb and parry count
					if (*p != ',') return false; //check the dividing comma
					if (!verifyNum(++p, 32)) return false;
				}
			} else if (*p == 'C') { //18 command counts
				for (size_t i = 0; i < 18; i++) {
					if (!verifyNum(++p, 32)) return false; //check the command count
					if (i < 17 && *p != ',') return false; //check the dividing comma
				}
			} else { //we landed on some weird non-section and non-divider character so that's bad and we return false
				return false;
			}
			if (*p != '|' && *p != '=') return false; //check the divider
		}

		return true; //yay everything worked so it's valid!
	}

	//goes through the thingy to find the position of the given section MARK: get section
	const char* getSection(char section) {
		bool namesection = false; //we can't return anything if we're in the naming section because the player could type weird names
		bool escaping = false; //don't end the naming section if we're escaping

		const char* d = data+3; //get a pointer to the start of data (ignoring the BQ2 at the start) so we can move it forward until the section

		for (; *d != section || namesection ; d++) {
			if (d - data > strlen(data)) return NULL; //the section wasn't in the data, so we return NULL

			//these characters mean we are in a name section, so we have to watch out for fake ending characters now
			if (*d == 'N' || *d == 'L') namesection = true; //these can also show up in the middle of a naming section, but namesection is true there anyway so that's fine

			if (escaping) escaping = false; //we are escaping so don't finish the naming section when reaching a , or |
			else if (*d == '\\') escaping = true; //\ means we're escaping, this isn't a \\ because the preceding check would've caught it
			else if (!escaping && (*d == '|' || *d == ',')) namesection = false; //once we reach the | or , and we're not escaping, we've reached the end of the name section
		}

		return d; //return the position of the section character we landed on
	}

	//get the version of this save system, for future proofing! MARK: get version
	char getVersion() {
		return getSection('V')[1]; //go to section V and the version is the character after that. (96 possible versions should be enough)
	}

	//get the playtime in the save in seconds
	long long getPlaytime() {
		long long seconds = 0; //play time defaults to 0
		const char* sectionQ = getSection('Q');
		if (sectionQ) { //the play time is the first data in section Q, quite conveniently, so we parse that
			seconds = ParseNum(++sectionQ);
		}
		return seconds;
	}

	//print specific save data points that are helpful for knowing which save is which MARK: print save
	void printMainString() {
		char name[255];
		ParseName(name, getSection('N')+1); //get the name of the player character

		int monies = 0; //monies are 0 by default if we don't find the section
		const char* sectionM = getSection('M');
		if (sectionM) monies = ParseNum(++sectionM); //get the player's monies

		int level = 0; //level defaults to 0
		const char* sectionR = getSection('R');
		if (sectionR) { //if we got to section R, the player is definitely in there, so go to the a subsection
			const char* a = strchr(sectionR, 'a');
			level = ParseNum(++a); //the level is the first data there so we read that
		}

		long long seconds = 0; //play time defaults to 0
		const char* sectionQ = getSection('Q');
		if (sectionQ) { //the play time is the first data in section Q, quite conveniently, so we parse that
			seconds = ParseNum(++sectionQ);
		}

		long long minutes = seconds/60; //use minutes if possible, precise reports of game time
		seconds %= 60; //get rid of the extra seconds
		long long hours = minutes/60; //go further to hours if possible
		minutes %= 60; //get rid of the extra minutes

		//print the data!
		cout << name << ", Lvl " << level << ", " << monies << " mon" << (monies == 1 ? "y" : "ies") << ", ";
		if (hours) cout << hours << "h "; //playtime stuff
		if (minutes || hours) cout << minutes << "m ";
		cout << seconds << "s";
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
		addChunk(save->data, "BQ2|V0|N", save->savesize);
		//Section N, add the name
		{ char* name = formatName(Helper::charNPC['a']->getName()); //format the name to have escape codes so we can have special characters in the name
		addChunk(save->data, name, save->savesize);
		delete[] name; } //delete the name because it was made with new
		//Section M, monies, only if we actually have any
		if (monies > 0) {
			char sectionM[20];
			snprintf(sectionM, 20, "|M%d", monies);
			addChunk(save->data, sectionM, save->savesize);
		}
		//Section W, this is built during gameplay, so we just add it if we've done any changes
		if (Helper::sectionW) { //if it's been edited so it's not null
			addChunk(save->data, "|W", save->savesize);
			addChunk(save->data, Helper::sectionW, save->savesize);
		}
		//Section U, track pursuer if we have one
		if (NPC* pursuer = Helper::charNPC['a']->getPursuer()) {
			char sectionU[20];
			snprintf(sectionU, 20, "|U%d", pursuer->getRoom()->getID());
			addChunk(save->data, sectionU, save->savesize);
		}
		//Section R, track everything related to the teammates
		addChunk(save->data, "|R", save->savesize);
		for (set<NPC*>::iterator teamiterator = Helper::Rnpcs.begin(); teamiterator != Helper::Rnpcs.end(); teamiterator++) {
			NPC* teammate = *teamiterator;
			char sectionR[1000]; //this is prolly enough space
			//the teammate section included the identifying char, then the level, xp, room id, attacks launched, helps launched, damage dealt, heals dealt, damage taken, health recovered, knockouts, incapacitations, a special stat which varies by npc, total sp used up by pecial attacks, and gym starting time
			snprintf(sectionR, 1000, "%c%d.%d.%d.%d.%d.%lld.%lld.%lld.%lld.%d.%d.%d.%lld.%lld", npcChar[teammate], teammate->getLevel(), teammate->getXP(), teammate->getRoom()->getID(), attackslaunched[teammate], helpslaunched[teammate], damagedealt[teammate], healthhealed[teammate], damagerecieved[teammate], healthrecovered[teammate], knockouts[teammate], incapacitations[teammate], specialstat[teammate], spusedup[teammate], (long long)teammate->getGymStart());
			addChunk(save->data, sectionR, save->savesize);
			if (next(teamiterator) != Helper::Rnpcs.end()) addChunk(save->data, ",", save->savesize); //dividing comma
		}
		//Section P, save the player's party
		{ char team[12] = "|P"; //build the team here starting with the P section identifier then all their representing chars in order
		size_t plen = Helper::charNPC['a']->getParty()->size();
		for (size_t i = 1; i < plen; i++) { //add all the party characters to the string (skipping the player because of course they're in the party)
			team[i+1] = Helper::npcChar[(*Helper::charNPC['a']->getParty())[i]];
		}
		team[plen+1] = '\0'; //null terminate it!
		addChunk(save->data, team, save->savesize); } //add the team chunk
		//Section E, save all encountered enemy types
		if (encountered.size()) addChunk(save->data, "|E", save->savesize);
		for (set<NPC*>::iterator enemyiterator = Helper::encountered.begin(); enemyiterator != Helper::encountered.end(); enemyiterator++) {
			char sectionE[10];
			snprintf(sectionE, 10, "%d", (*enemyiterator)->getID());
			addChunk(save->data, sectionE, save->savesize);
			if (next(enemyiterator) != Helper::encountered.end()) addChunk(save->data, ",", save->savesize); //dividing comma
		}
		//Section I, saves every item's position
		addChunk(save->data, "|I", save->savesize);
		for (vector<Item*>::iterator itemiterator = Helper::itemsH.begin(); itemiterator != Helper::itemsH.end(); itemiterator++) {
			Item* item = *itemiterator;
			int roomid = (item->getRoom() ? item->getRoom()->getID() : -1);
			char sectionI[21];
			snprintf(sectionI, 21, "%d.%d", item->getID(), roomid);
			addChunk(save->data, sectionI, save->savesize);
			if (next(itemiterator) != Helper::itemsH.end()) addChunk(save->data, ",", save->savesize); //dividing comma
		}
		//Section L, store lobster name and room
		NPC* lobster = Helper::charNPC['t']; //get the lobster NPC* for easier use
		if (!lobster->getLeader()) { //only do section L if the lobster has been tamed
			addChunk(save->data, "|L", save->savesize);
			if (strlen(lobster->getTitle())) { //if the lobster doesn't have a title, we don't record a name (because that's how the game knows to not change the original name), also accounting for people who named their tunnel lobster TUNNEL LOBSTER for some reason
				char* lname = formatName(lobster->getName()); //same process as player name
				addChunk(save->data, lname, save->savesize);
				delete[] lname;
			}
			char lobroom[11]; //get the lobster' room (plus the dividing comma since it's convenient)
			snprintf(lobroom, 11, ",%d", lobster->getRoom()->getID());
			addChunk(save->data, lobroom, save->savesize);
		}
		//Section T, store found tunnel directions
		if (sectionT[0] || sectionT[1] || sectionT[2] || sectionT[3] || sectionT[4]) { //only track if any of them have actually been used
			addChunk(save->data, "|T", save->savesize);
			for (size_t i = 0; i < 5; i++) { //there are five tunnel directions so do one check for each one
				if (sectionT[i]) { //if the direction has been discovered
					char dir[10]; //put the direction into the thingy
					snprintf(dir, 10, "%d", sectionT[i]->getID());
					addChunk(save->data, dir, save->savesize); //add the thingy to the save in section T which is the current section of course
				} else { //the tunnel direction has not been discovered yet so we add a sentinel -1 representing NULL
					addChunk(save->data, "-1", save->savesize);
				}
				if (i < 4) addChunk(save->data, ",", save->savesize); //add the dividing comma if it's not the last one
			}
		}
		//Section B, store current bank balance and the last time used the bank
		NPC* banker = Helper::charNPC['n']; //get the banker
		if (!banker->getConvoSize()) {
			char bankstuff[100]; //print the bank data into this charray
			snprintf(bankstuff, 100, "|B%d,%lld", banker->getBalance(), (long long)banker->getDepositTime());
			addChunk(save->data, bankstuff, save->savesize); //add the banking section!
		}
		//Section S, store all the world states except for never and gameend
		addChunk(save->data, "|S", save->savesize);
		for (size_t i = 0; i < Helper::GAMEEND; i++) {
			addChunk(save->data, (Helper::WorldState[i] ? "1" : "0"), save->savesize);
		}
		//Section Q, store how long the player has played on this file plus some silly error data plus some other potentially interesting stats
		playtime += difftime(time(NULL), lastsave); //this might lose one second per save (command), but that's basically nothing so that's fine
		char sectionQ[1000];
		snprintf(sectionQ, 1000, "|Q%.0f,%d,%d,%d,%d,%d,%lld,%d,%d", playtime, sessions, invalidcommand, invalidmove, invalidnpc, invaliditem, nothingtosay, biggestspbomb, successfulparries);
		addChunk(save->data, sectionQ, save->savesize);
		//Section C, store how many times each command has been used
		addChunk(save->data, "|C", save->savesize);
		for (size_t i = 0; i < 18; i++) { //there are eighteen commands (apart from quit without saving) so do one check for each one
			char sectionC[20]; //put the number into the thingy
			snprintf(sectionC, 20, "%d", commandcount[i]);
			addChunk(save->data, sectionC, save->savesize);
			if (i < 17) addChunk(save->data, ",", save->savesize); //add the dividing comma if it's not the last one
		}
		//cap it off with a =
		addChunk(save->data, "=", save->savesize);
	}

	//parse name and advance save data pointer past it in the process, assumes it's given a valid save MARK: parse name (NPC)
	static void ParseName(NPC* named, const char*& data) {
		bool escaping = false;
		char name[255];

		for (size_t i = 0;; data++) {
			if (!escaping && *data == '\\') {
				escaping = true;
			} else if (!escaping && (*data == ',' || *data == '=' || *data == '|')) {
				name[i] = '\0'; //cap off the name cause we're done here
				break;
			} else {
				name[i++] = *data;
				escaping = false;
			}
		}
		named->setName(name);
	}

	//different overload for the name parsing that writes into the given char* MARK: parse name (char)
	static void ParseName(char dest[255], const char* data) {
		bool escaping = false;

		for (size_t i = 0;; data++) {
			if (!escaping && *data == '\\') {
				escaping = true;
			} else if (!escaping && (*data == ',' || *data == '=' || *data == '|')) {
				dest[i] = '\0'; //cap off the name cause we're done here
				break;
			} else {
				dest[i++] = *data;
				escaping = false;
			}
		}
	}

	//parse the number in the data currently and advance the pointer past it MARK: parse num
	static long long ParseNum(const char*& data) {
		int sign = 1;
		if (*data == '-') { //we have to sign the integer because there's a -
			sign = -1;
			data++; //go past the -
		}
		long long total = 0;
		while (isdigit((unsigned char)*data)) {
			total = total * 10 + (*data - '0');
			data++; //increment the data
		}
		return total * sign;
	}

	//adjust item id to get their index in itemsH, accounting for deleted items MARK: adjust item id
	static int adjustItemID(int id, set<int>& discontinuities) {
		int discount = distance(discontinuities.begin(), discontinuities.lower_bound(id));
		return id - discount;
	}
	
	//section W is pretty big and has its own subsections so I split the loading into this function MARK: load W
	static void loadW(const char*& data, NPC* player, set<int>& discontinuity) {
		while(*data != '|') {
			if (*(++data) == 'b') { //duplicate item because it was bought
				itemsH[ParseNum(++data)]->loadBuy(); //dupe the item and its position will be handled later, logs in W automatically, don't adjust because we only buy (dupe) items that are in limbo and can't be interacted with
			} else if (*data == 'u') { //use item in world
				int id = ParseNum(++data); //get the id unadjusted so we can update discontinuities more conveniently later if we delete the item
				Item* item = itemsH[adjustItemID(id, discontinuity)]; //get the item adjusted for item deletions
				Room* room = roomsH[ParseNum(++data)]; //get the room this item was used in, which is sometimes important
				bool log = true; //if we should releog this change, don't log if it didn't actually do anything
				if (!strcmp(item->getType(), "BURGER") || !strcmp(item->getType(), "info")) {
					log = false;
				} else if (!strcmp(item->getType(), "education")) {
					for (Attack* attack : ((EducationItem*)item)->getAttacks()) { //much simpler than in the main useItem function since we can ignore all the text
						player->addSpecialAttack(attack);
					}
				} else if (!strcmp(item->getType(), "caller")) { //if it's the caller, just don't log it unless it was part of the prison escape sequence which causes world changes
					if (WorldState[IMPRISONED]) { //do the prison stuff if needed
						WorldState[IMPRISONED] = false;
						charNPC['t']->doLobsterChanges();
					} else log = false;
				} else if (!strcmp(item->getType(), "key") || !strcmp(item->getType(), "hose")) {
					KeyItem* key = (KeyItem*)item;
					bool used = false;
					for (Room* r : key->getTargets()) {
						r->unblockAll(key->getUnlockType());
						used = true;
					}
					if (!used) {
						vector<const char*> exitsUnlocked = room->unblockAll(key->getUnlockType());
						for (const char* exit : exitsUnlocked) { //unblocks the other sides of the exits, because there's some double sided blocks
							Room* thatroom = room->getExit(exit); //gets the room on the other side of the exit
							//unblocks the reverse direction exit if it's blocked
							if (thatroom->getBlocked(ReverseDirection[exit])) {
								thatroom->unblockExit(ReverseDirection[exit]);
							}
						}
					}
					
				} else if (!strcmp(item->getType(), "paver")) {
					PaverItem* paver = (PaverItem*)item; //converts to the corresponding subclass
					room->setExit(paver->getDirection(), paver->getDestination()); //sets the exit to the room in the given direction
					paver->getDestination()->setExit(const_cast<char*>(ReverseDirection[paver->getDirection()]), room); //sets the exit back to the current room in the reverse of the given direction
				} else if (!strcmp(item->getType(), "treasure")) { //for treasure items, ignore their monies and items because they're set by sections M and I respectively, just handle the mimics
					if (NPC* mimic = ((TreasureItem*)item)->getMimic()) { //set the mimic to the room the chest was opened in
						mimic->setRoom(room); //technically since fight() is called in useItem(), the player could log subsection d for the mimic before u for the trasure, but as far as I can tell this doesn't cause any conflicts so I think it's fine
					}
				} else if (!strcmp(item->getType(), "switch")) { //switch conveyor directions
					for (Room* conveyor : ((ConveyorSwitch*)item)->getConveyors()) {
						conveyor->switchConveyor();
					}
				} else if (!strcmp(item->getType(), "worldchange")) { //both take to use and normal world change items, basically behave the same apart from how you use them
					applyWorldChange(((WorldChangeItem*)item)->getChanges()); //do the changes
				} else if (!strcmp(item->getType(), "blender")) {
					BlenderItem* blender = (BlenderItem*)item;
					vector<Item*> ingredients;
					for (const char* itemname : blender->getIngredients()) { //find all the ingredients
						ingredients.push_back(getItemInVector(itemsH, itemname));
					}
					for (Item* ingredient : ingredients) { //delete the ingredients
						ingredient->unRoom();
						discontinuity.insert(ingredient->getID()); //log the discontinuity because we're about to delete the item
						delete ingredient;
					} //and then the product is handled by section I
					applyWorldChange(blender->getChanges()); //does the blender's world changes
				} else if (!strcmp(item->getType(), "choiceorb")) {
					((ChoiceOrb*)item)->makeChoice(*(++data)); //make the choice based on the character
					log = false; //because it logs itself
					data++; //go past the choice letter
				} else if (!strcmp(item->getType(), "lightorb")) {
					LightOrb* orb = ((LightOrb*)item);
					orb->setTeammate(NULL, player->getParty(), false); //give the teammate back
					item->setRoom(room); //put the item in the dropoff room so we can do this check down here
					vector<Item*> lightorbs; //get all the light orbs so we can check if we can open the ground yet
					while (Item* lorb = getItemTypeInVector(room->getItems(), "lightorb")) {
						lorb->unRoom();
						lightorbs.push_back(lorb);
					}
					if (lightorbs.size() >= 3) { //open the exit to the boss area!
						orb->paveDown(); //just use the first one, it doesn't matter which one
					}
					for (Item* lorb : lightorbs) { //put the lorbs back
						lorb->setRoom(room);
					}
				} else if (!strcmp(item->getType(), "coldorb")) {
					((ColdOrb*)item)->extinguishFire();
				} else if (!strcmp(item->getType(), "escapeorb")) { //do escape orb drop changes
					((EscapeOrb*)item)->drop();
				} else if (!strcmp(item->getType(), "manhole")) { //uncover manhole item exits
					ManholeItem* cover = (ManholeItem*)item;
					room->setExit(cover->getDirection(), cover->getRoom());
					cover->getRoom()->unblockExit(ReverseDirection[cover->getDirection()]); //also unblock the exit from below
					cover->nullifyRoom();
				}
				//ignore xp items because we aready track xp in section R, but they do get used up

				if (*data == '.') { //it bothered me for some reason that the user could but a .[choice] on a non-choiceorb item so we ignore the choice here
					data += 2; //skip the . and the choice
				}

				//BURGER items don't actually do anything to the world so we just ignore those, otherwise save it to the log so subsequent loadings can do this again
				if (log) {
					trackItemUse(item, room, false); //don't track the use variable because we manually set that later anyway
				}

				if (item->getConsumable()) { //delete the item if it was a consumable
					item->unRoom();
					discontinuity.insert(id); //log the discontinuity because we're about to delete the item
					delete item;
				}
			} else if (*data == 'x') { //delete item because it was used in battle (we do not log x for non-consumable items used in battle, they don't have any effects outside of battle so we just delete them)
				int id = ParseNum(++data); //get the id not adjusted yet so we can add the discontinuity
				Item* item = itemsH[adjustItemID(id, discontinuity)]; //get the item adjusted for item deletions
				item->unRoom(); //unroom it if it's a roomed item
				discontinuity.insert(id); //log the discontinuity because we're about to delete the item
				delete item; //cannot be in the inventory at this point in the loading process so we just delete it
			} else if (*data == 'd') { //defeat defeated enemies
				NPC* npc = npcsH[ParseNum(++data)];
				npc->defeat(); //defeat() logs the defeating in section W automatically
				npc->takeGift(); //take any battle gifts they may have had, room/inventory set in section I
			} else if (*data == 'r') { //handle respawn changes
				npcsH[ParseNum(++data)]->undefeat(); //undefeat() also logs it in W automatically
			} else if (*data == 'a') { //pop dialogue because it's already been heard, plus any world changes the conversation has (.r for recruitment dialogue, .e for recruited dialogue, .j for rejection dialogue, .d for dismissal dialogue, .i for dismissal rejection, .o for opening, .g for gym)
				//MARK: THIS DOES NOT HANDLE BRANCHING DIALOGUE AT ALL
				NPC* npc = npcsH[ParseNum(++data)];
				if (*data != '.') { //normal conversation
					npc->printDialogue(false, NULL, false);
					npc->takeGift(); //take their gift cause we talked to them, I don't think any exhausted dialogues give gifts
				} else if (*(++data) == 'r') { //recruitment dialogue
					npc->printRecruitmentDialogue(false);
				} else if (*data == 'e') { //recruited dialogue
					npc->popRecDialogue();
				} else if (*data == 'j') { //rejection dialogue
					npc->printRejectionDialogue(false);
				} else if (*data == 'd') { //dismissal dialogue
					npc->printDismissalDialogue(false);
				} else if (*data == 'i') { //dismissal rejection
					npc->printDismissalRejection(false);	
				} else if (*data == 'o') { //opening dialogue
					npc->printOpeningDialogue(false);
				} else if (*data == 'g') { //gym dialogue
					npc->popGymDialogue();
				} //logging is done by npc automatically, and so are any conversation changes by printConversation
			} else if (*data == 'e') { //room enter changes
				roomsH[ParseNum(++data)]->doEnterChanges(); //this function also logs the thing automatically
			} else if (*data == 'w') { //do room welcomes
				roomsH[ParseNum(++data)]->printWelcome(false); //printWelcome logs it automatically
			} else if (*data == 'p') { //player got caught by pursuer
				npcsH[ParseNum(++data)]->doCatchChanges();
			} else if (*data == 't') { //open temples
				roomsH[ParseNum(++data)]->openTemple(false); //openTemple logs that the temple was opened automatically
			}
		}
	}

	//gets stuff which it can edit, and modifies the world to match the given save data MARK: load game
	static void LoadGame(const Save* save, NPC* player, std::vector<Item*>* inventory, int& monies) {
		set<int> discontinuity; //the item id discontinuities, because we delete items sometimes so we need this to get actual itemsH index

		for (const char* data = save->data+4; *data != '=';) { //keep loading stuff until we reach the end
			if (*data == '|') { //just go past the dividers
				data++;
			} else if (*data == 'V') { //ignore version section, if it's a future version we should just check outside the function
				data += 2;
			} else if (*data == 'N') { //n for name
				if (*(++data) != '|') ParseName(player, data);
			} else if (*data == 'M') { //m for monies
				monies = ParseNum(++data);
			} else if (*data == 'W') { //w for world change and everything that can cause it, most important section
				loadW(data, player, discontinuity); //it was a pretty big section so I gave it its own function
			} else if (*data == 'U') { //u for second letter of pursuer
				player->getPursuer()->setRoom(roomsH[ParseNum(++data)]); //just set their room
			} else if (*data == 'R') { //r for recruitable, tracks recruitable npcs' stats (and Buford's)
				for (; *data != '|' && *data != '=';) {
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
					Rnpcs.insert(npc);
				}
			} else if (*data == 'P') { //p for party
				while (*(++data) != '|') { //add the teammates until we reach the end
					NPC* teammate = charNPC[*data];
					player->setParty({teammate});
					teammate->Recruit();
				}
			} else if (*data == 'E') { //e for encountered enemies
				while (*data != '|' && *data != '=') {
					encountered.insert(npcsH[ParseNum(++data)]);
				}
			} else if (*data == 'I') { //i for items, set their rooms
				while (*data != '|' && *data != '=') {
					Item* item = itemsH[adjustItemID(ParseNum(++data), discontinuity)];
					int roomid = ParseNum(++data);
					//all the BURGERs faded to ashes after beating the game so we delete all of them if the final boss has been defeated
					if (WorldState[BURGERMENDEF] && item->getType("BURGER")) {
						item->unRoom();
						discontinuity.insert(id); //log the discontinuity because we're about to delete the item
						delete item;
					} else if (roomid == -1) { //-1 means NULL room which means they go in the inventory
						item->unRoom();
						inventory->push_back(item);
						//make sure the player has their weapon items' attacks
						if (!strcmp(item->getType(), "weapon")) player->addSpecialAttack(((WeaponItem*)item)->getAttack());
						//make the escape orbs' name and description match to escape version if it's in the inventory and not stone yet
						else if (!strcmp(item->getType(), "escapeorb") && !((EscapeOrb*)item)->getInert()) ((EscapeOrb*)item)->take();
					} else { //otherwise set the item to the room
						item->setRoom(roomsH[roomid]);
						//block exits that may be blocked by a hose being in the room
						if (!strcmp(item->getType(), "hose")) ((HoseItem*)item)->drop(roomsH[roomid]);
					} 
				}
			} else if (*data == 'L') { //l for lobster
				NPC* lobster = charNPC['t'];
				if (*(++data) != ',') ParseName(charNPC['t'], data);
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
				int balance = ParseNum(++data);
				long long time = ParseNum(++data);
				charNPC['n']->manualSetBankData(balance, time); //set the bank balance and the bank time
			} else if (*data == 'S') { //s for states, set all the world states
				for (size_t i = 0; *(++data) != '|' && *data != '='; i++) { //set all the states except for NEVER and GAMEEND because that's not tracked by SaveWorld
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
	}

	//make a new save with the given save data and identifying number MARK: constructor
	Save(const char* _data, size_t num) {
		while (strlen(_data)+1 > savesize) { //find a good length to fit the amount of data
			savesize *= 2;
		}
		data = new char[savesize](); //make room for the data
		data[0] = '\0'; //null terminate it, and strcat moves the terminator forward
		strcat(data, _data);
		savenum = num; //saves have identifying numbers for convenience
		char* ending = const_cast<char*>(getSection('='));
		if (ending) ending[1] = '\0'; //null terminate the character after = to truncate whitespace, to avoid copy pasting annoyances
	}

	~Save() { //delete the data on destruction MARK: destructor
		delete[] data;
	}
};

#endif