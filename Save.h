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
		return data[5]; //MARK: FIX!
	}

	const char* getSection(char section) { //MARK: make this please
		const char* p = data;
		return NULL;
	}

	void printMainString() { //this is very placeholder MARK: fix this
		const char* p = strchr(data, 'N')+1;
		
		bool escaping = false;
		char* name = new char[255];

		for (size_t i = 0;; p++) {
			if (!escaping && *p == '\\') {
				escaping = true;
			} else if (!escaping && (*p == ',' || *p == '=' || *p == '|')) {
				name[i] = '\0';
				break; //we have reached the end of the name and we're in some other section now
			} else {
				name[i++] = *p;
				escaping = false;
			}
		}

		p = strchr(p, 'M')+1;
		int monies;
		if (p) monies = 0;
		else monies = ParseNum(p);
		p = strchr(p, 'R')+1;
		p = strchr(p, 'a')+1;
		int level = ParseNum(p);
		p = strchr(p, 'Q')+1;

		long long seconds = ParseNum(p);
		long long minutes = seconds/60; //use minutes if possible, precise reports of game time
		seconds %= 60; //get rid of the extra seconds
		long long hours = minutes/60; //go further to hours if possible
		minutes %= 60; //get rid of the extra minutes

		cout << name << ", Lvl " << level << ", " << monies << " mon" << (monies == 1 ? "y" : "ies") << ", ";
		if (hours) cout << hours << "h ";
		if (minutes || hours) cout << minutes << "m ";
		cout << seconds << "s";

		delete[] name;
	}

	//makes sure the save's data is valid and perfectly parsable with no issues, so I can be sure the string is good when I start loading
	bool getValid() {
		return true;
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
		//Section S, store all the world states except for never
		addChunk(save->data, "|S", save->savesize);
		for (size_t i = 0; i < Helper::NEVER; i++) {
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

	//parse name and advance save data pointer past it in the process, assumes it's given a valid save
	static void ParseName(NPC* named, const char*& data) {
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
	static long long ParseNum(const char*& data) {
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

		const char* data = save->data;

		//after validating the save, assume it's all perfectly parsable (not necessarily correct, just parsable)
		data += 4; //skip the BQ2|

		set<int> discontinuity; //the item id discontinuities, because we delete items sometimes so

		while (*data != '=') { //keep loading stuff until we reach the end
			if (*data == '|') { //just go past the dividers
				data++;
			} else if (*data == 'V') { //ignore version section, if it's a future version just check outside the function
				data += 2;
			} else if (*data == 'N') { //n for name
				if (*(++data) != '|') ParseName(player, data);
			} else if (*data == 'M') { //m for monies
				monies = ParseNum(++data);
			} else if (*data == 'W') { //w for world change and everything that can cause it, most important section
				while(*data != '|') {
					if (*(++data) == 'b') { //duplicate item because it was bought
						itemsH[ParseNum(++data)]->loadBuy(); //dupe the item and its position will be handled later, logs in W automatically, don't adjust because we only buy (dupe) items that are in limbo and can't be interacted with
					} else if (*data == 'u') { //use item in world
						int id = ParseNum(++data); //get the id unadjusted so we can update discontinuities more conveniently later if we delete the item
						Item* item = itemsH[adjustItemID(id, discontinuity)]; //get the item adjusted for item deletions
						Room* room = roomsH[ParseNum(++data)]; //get the room this item was used in, which is sometimes important
						bool log = true; //if we should releog this change, don't log if it didn't actually do anything
						//MARK: keep in mind at this point we don't know where any of the items are or if they're in the inventory
						if (!strcmp(item->getType(), "BURGER")) {
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
							//skip for now MARK: do this
						} else if (!strcmp(item->getType(), "manhole")) { //uncover manhole item exits
							ManholeItem* cover = (ManholeItem*)item;
							room->setExit(cover->getDirection(), cover->getRoom());
							cover->getRoom()->unblockExit(ReverseDirection[cover->getDirection()]); //also unblock the exit from below
							cover->nullifyRoom();
						}
						//ignore xp items because we aready track xp in section R, but they do get used up

						//BURGER items don't actually do anything to the world so we just ignore those, otherwise save it to the log so subsequent loadings can do this again
						if (log) {
							trackItemUse(item, room, false); //don't track the use variable because we manually set that later anyway
						}

						if (item->getConsumable()) { //delete the item if it was a consumable
							item->unRoom();
							discontinuity.insert(id); //log the discontinuity because we're about to delete the item
							delete item;
						}
					} else if (*data == 'x') { //delete item because it was used in battle (we do not log x for non-consumable items used in battle), they don't have any effects outside of battle so we just delete them
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
						npcsH[ParseNum(++data)]->undefeat(); //undefeat() also logs it in Wautomatically
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
					} else if (*data == 'o') {
						//o - ignore for now drop escape orb MARK: WE NEED TO HANDLE ESCAPE ORBS
					} else if (*data == 'p') { //player got caught by pursuer
						npcsH[ParseNum(++data)]->doCatchChanges();
					} else if (*data == 't') { //open temples
						roomsH[ParseNum(++data)]->openTemple(false); //openTemple logs that the temple was opened automatically
					}
				}
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
					player->setParty({charNPC[*data]});
				}
			} else if (*data == 'E') { //e for encountered enemies
				while (*data != '|' && *data != '=') {
					encountered.insert(npcsH[ParseNum(++data)]);
				}
			} else if (*data == 'I') { //i for items, set their rooms
				while (*data != '|' && *data != '=') {
					Item* item = itemsH[adjustItemID(ParseNum(++data), discontinuity)];
					int roomid = ParseNum(++data);
					if (roomid == -1) { //-1 means NULL room which means they go in the inventory
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
		while (strlen(_data)+1 > savesize) {
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