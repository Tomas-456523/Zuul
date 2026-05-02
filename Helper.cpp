//the implementation file for my Helper namespace, has a bunch of helpful functions that I might use in multiple files

#include "Helper.h"
#include "Attack.h"
#include "NPC.h"
#include "Item.h"
#include "Room.h"
#include "Save.h"
#include <cstring>
#include <iostream>
#include <limits>
#include <algorithm>
#include <cmath>
using namespace std;

namespace Helper {
	//if there's extra or faulty input, this handy function will ignore that for you without having to type numeric limits every time
	void CinIgnoreAll(bool force) {
		if (!cin || force) {
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
	void ParseWithON(const char* commandExtensionP, char* itemNameP, char* npcNameP) {
		const char* on = strstr(commandExtensionP, " ON "); //finds the first char of the substring " ON " in the given command extension
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
	NPC* getNPCInVector(vector<NPC*>& the_vector, const char* npcname) {
		for (NPC* npc : the_vector) { //if the npc isn't defeated and the name matches, we return that
			if (!npc->getDefeated() && !strcmp(npc->getName(), npcname)) {
				return npc;
			}
		}
		return NULL; //no valid npc was found so return null
	}
	//finds an item in the given vector that has the given name
	Item* getItemInVector(vector<Item*>& the_vector, const char* itemname) {
		for (Item* item : the_vector) { //if the item's name matches, we return that
			if (!strcmp(item->getName(), itemname)) {
				return item;
			}
		}
		return NULL; //no valid item was found so return null
	}
	//finds an item in the given vector that has the given type
	Item* getItemTypeInVector(vector<Item*>& the_vector, const char* itemtype) {
		for (Item* item : the_vector) { //if the item's type matches, we return that
			if (!strcmp(item->getType(), itemtype)) {
				return item;
			}
		}
		return NULL; //no valid item was found so return null
	}
	//finds an attack in the given vector that has the given name
	Attack* getAttackInVector(const vector<Attack*>& the_vector, const char* attackname) {
		for (Attack* attack : the_vector) { //if the attack's name matches, we return that
			if (!strcmp(attack->name, attackname)) {
				return attack;
			}
		}
		return NULL; //no valid attack was found so return null
	}
	//prints the data of the given npc
	void printNPCData(NPC* npc, bool battle) {
		cout << "\n" << npc->getTitle(); //prints the title of the npc if they have one
		if (strlen(npc->getTitle()) > 0) {
			cout << " "; //seperate the title and the name with a space if there is a title
		} //prints the name and description of the npc plus their level
		cout << npc->getName() << " - " << npc->getDescription() << "\n  LEVEL " << npc->getLevel();
		if (npc->getRecruited()) { //prints the xp to level up for recruited npcs. Non-recruitable npcs don't level up anyway
			cout << " (" << npc->xpForNextLevel() << " xp to LEVEL UP)";
		} //prints any effects the npc may have (eg. "POISON")
		npc->printEffects();
		//prints all the stats of the npc
		cout << "\n  HEALTH - ";
		if (battle) cout << npc->getHealth() << "/";
		cout << npc->getHealthMax();
		cout << "\t  DEFENSE - " << npc->getDefense() * npc->getDefMultiplier();
		cout << "\n  ATTACK - " << npc->getAttack() * npc->getAttMultiplier();
		cout << "\t  TOUGHNESS - " << npc->getToughness() * npc->getToughMultiplier();
		cout << "\n  PIERCE - " << npc->getPierce() * npc->getPierceMultiplier();
		cout << "\t  SPEED - " << npc->getSpeed() * npc->getSpeedMultiplier();
		cout << "\n  SP - ";
		if (battle) cout << npc->getSP() << "/";
		cout << npc->getSPMax();
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
		if (npc->getSpecialAttacks()[0]->minLevel <= npc->getLevel()) { //if we have any usable special attacks, assumes attacks are added in level order
			cout << "\nSpecial attacks:";
		} //prints all the special attacks and how much sp they cost
		for (Attack* attack : npc->getSpecialAttacks()) {
			//we only print attacks if the npc is leveled up enough to use them
			if (attack->minLevel <= npc->getLevel()) {
				cout << "\n" << attack->name << " - " << attack->trueDesc << " - Costs " << attack->cost << " SP";
			}
		}
		commandcount[9]++; //increment successful attacksing
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
	//returns the given double from min to max, seperate from the above one for convenience
	double ClampD(double num, double min, double max) {
		if (num < min) { //return the minimum if below it
			return min;
		}
		if (num > max) { //return the maximum if above it
			return max;
		}
		return num; //the number is actually perfectly fine so we return it as-is
	}
	//rounds the given double to the nearest integer
	int Round(double num) {
		return (int)round(num);
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
			if (npc->getHealth() > 0 && (npc->getBasicAttack() || !npc->getSpecialAttacks().empty())) { //they also don't count if they have no attacks (e.g. a life plant, it doesn't do anything so it doesn't count)
				i++;
			}
		}
		return i; //returns the found count
	}
	//get new vector with only the npcs above 0 hp
	vector<NPC*> getAlive(const vector<NPC*>& team) {
		vector<NPC*> alive; //*capacitated
		for (NPC* npc : team) { //push all the npcs above 0 hp into the thingy
			if (npc->getHealth() > 0) alive.push_back(npc);
		}
		return alive;
	}
	//loops until the player chooses either option A or B, and returns true if A was chosen and false if B was chosen
	bool AOrB(const char* prompt, const char* A, const char* B) {
		bool promptline = true; //if the prompting > should be in a newline, only false for typing nothing
		while (true) { //loops until valid response, after which the loop is returned out of
			char command[255] = ""; //the charray that the player inputs into

			if (prompt) cout << "\n" << prompt;
			if (promptline) cout << "\n";
			else nothingtosay++; //player said nothing so increment the number
			cout << "> "; //The amazing >
			cin.getline(command, 255); //gets the player input

			promptline = true; //reset promptline status after using the previous status

			AllCaps(command); //capitalizes the command for easier parsing

			if (!strcmp(command, A)) { //choose option A
				return true;
			} else if (!strcmp(command, B)) { //choose option B
				return false;
			} else if (!strcmp(command, "")) { //don't add an extra ugly newline or Invalid response "" if the player typed nothing
				promptline = false;
			} else { //prints the invalid input
				cout << "\nInvalid response \"" << command << "\".";
				invalidcommand++; //this was an invalid response so we increment the invalid command counter
			}

			CinIgnoreAll(); //clears extra or faulty input
		}
	}
	//prints a conversation, must be here because having this as Conversation's function was causing a plethora of compiler errors
	void printConversation(const Conversation* _convo, bool finalpause) { //pass a pointer so it's easier to do all the alt stuff
		cout << "\n";
		const Conversation* current = _convo;
		while (current->goToAlt()) { //keep going until we didn't find a new alt conversation
			current = current->alt.get(); //go to the alt
		}
		const vector<pair<NPC*, const char*>>& convo = current->lines;
		for (int i = 0; i < convo.size(); i++) { //prints all the dialogue in the conversation
			if (convo[i].first != NULL) {
				cout << convo[i].first->getName() << " - \"" << convo[i].second << "\"";
			} else {
				cout << convo[i].second;
			}
			if ((finalpause && !current->branch1.first && !current->next) || i + 1 < convo.size()) { //if it's not the last one (or if we manually set it to print the final pause since some situations print more stuff afterwards (AND we don't have branching paths)) we do a pause, so the last one lets the player type
				CinPause();
			}
		}
		if (current->branch1.first) { //if one of the four components isn't NULL I'm assuming I made all of them equal something otherwise why would I put a branch
			if (AOrB(NULL, current->branch1.first, current->branch2.first)) {
				printConversation(current->branch1.second.get(), finalpause);
			} else {
				printConversation(current->branch2.second.get(), finalpause);
			}
		} else if (Conversation* next = current->next.get()) {
			printConversation(next, finalpause);
		}
		if (shared_ptr<Conversation> relay = current->relay.second.lock()) { //relay the relaying conversation to the npc
			current->relay.first->addConversation(*relay);
		}
		if (shared_ptr<WorldChange> convochanges = current->convochanges) { //do changes that the conversation makes
			applyWorldChange(*convochanges);
		}
	}
	//print the level up data tracked by this npc
	void printLvlUpData(NPC* npc) {
		if (npc->getLevelUp()) {
			cout << npc->getName() << " leveled up! " << npc->getName() << " is now Level " << npc->getLevel() << "!";
			npc->setLevelUp(false); //marks level up as false so we don't say we leveled up every time we finish a battle
			CinPause();
			Stats statChanges = npc->getStatChanges();
			cout << "  HEALTH - " << npc->getHealthMax(); //prints all the new stats of the npc
			if (statChanges[0]) cout << " (+" << statChanges[0] << ")";
			cout << "\t  DEFENSE - " << npc->getDefense();
			if (statChanges[1]) cout << " (+" << statChanges[1] << ")";
			cout << "\n  ATTACK - " << npc->getAttack();
			if (statChanges[2]) cout << " (+" << statChanges[2] << ")";
			cout << "\t  TOUGHNESS - " << npc->getToughness();
			if (statChanges[3]) cout << " (+" << statChanges[3] << ")";
			cout << "\n  PIERCE - " << npc->getPierce();
			if (statChanges[4]) cout << " (+" << statChanges[4] << ")";
			cout << "\t  SPEED - " << npc->getSpeed();
			if (statChanges[5]) cout << " (+" << statChanges[5] << ")";
			cout << "\n  MAX SP - " << npc->getSPMax();
			if (statChanges[6]) cout << " (+" << statChanges[6] << ")";
			CinPause();
			for (Attack* att : npc->getNewAttacks()) { //print any new attacks learned and what they do
				cout << "\n" << npc->getName() << " learned " << att->name << "!\n" << att->name << " - " << att->trueDesc;
				CinPause();
			}
		}
	}
	//changes the world based on the instructions passed, also the item/npc's world changes will be drained as we pass by reference
	void applyWorldChange(WorldChange& changes) {
		while (!changes.recruitLinks.empty()) {
			NPC* npc = changes.recruitLinks.front();
			npc->setRecruitable(true);
			if (npc->getLeader()) {
				npc->setLeader(false);
				npc->setBoss(false); //falsify boss just in case (so bosses like viola aren't immune to instakill attacks like shrimple beam)
				npc->undefeat();
			}
			changes.recruitLinks.pop();
		}
		while (!changes.dismissLinks.empty()) {
			pair<vector<NPC*>*, NPC*>& data = changes.dismissLinks.front();
			data.first->erase(remove(data.first->begin(), data.first->end(), data.second), data.first->end());
			data.second->Dismiss();
			changes.dismissLinks.pop();
		}
		while (!changes.conditionalRecruits.empty()) { //make unrecruitable all the npcs unless the condition paired to them is true
			pair<NPC*, size_t>& data = changes.conditionalRecruits.front();
			if (WorldState[data.second]) {
				data.first->setRecruitable(true);
				if (data.first->getLeader()) {
					data.first->setLeader(false);
					data.first->setBoss(false); //falsify boss just in case (so bosses like viola aren't immune to instakill attacks like shrimple beam)
					data.first->undefeat();
				}
			}
			changes.conditionalRecruits.pop();
		}
		while (!changes.linkedConversations.empty()) {
			pair<NPC*, Conversation>& data = changes.linkedConversations.front();
			data.first->addConversation(data.second);
			changes.linkedConversations.pop();
		}
		while (!changes.linkedDialogue.empty()) {
			pair<NPC*, Conversation>& data = changes.linkedDialogue.front();
			data.first->setDialogue(data.second);
			changes.linkedDialogue.pop();
		}
		while (!changes.linkedTitles.empty()) {
			pair<NPC*, const char*>& data = changes.linkedTitles.front();
			data.first->setTitle(data.second);
			changes.linkedTitles.pop();
		}
		while (!changes.linkedDescriptions.empty()) {
			pair<NPC*, const char*>& data = changes.linkedDescriptions.front();
			data.first->setDescription(data.second);
			changes.linkedDescriptions.pop();
		}
		while (!changes.roomChanges.empty()) {
			pair<Room*, const char*>& data = changes.roomChanges.front();
			data.first->setDescription(data.second);
			changes.roomChanges.pop();
		}
		while (!changes.defeatRooms.empty()) {
			pair<NPC*, Room*>& data = changes.defeatRooms.front();
			if (data.first->getPlayerness()) { //if it's the player, move the whole party
				for (NPC* npc : (*data.first->getParty())) {
					npc->setRoom(data.second);
				}
			} else { //if it's not the player, move them and also set their home room to that
				data.first->setRoom(data.second);
				data.first->setHome(data.second);
			}
			changes.defeatRooms.pop();
		}
		while (!changes.redirectRooms.empty()) {
			pair<Room*, Room*>& data = changes.redirectRooms.front();
			data.first->setRedirect(data.second);
			changes.redirectRooms.pop();
		}
		while (!changes.linkedStats.empty()) { //change all changing stats
			pair<NPC*, Stats>& data = changes.linkedStats.front();
			data.first->setBaseStats(data.second);
			changes.linkedStats.pop();
		}
		while (!changes.removeAttacks.empty()) { //change all changing stats
			pair<NPC*, Attack*>& data = changes.removeAttacks.front();
			data.first->removeSpecialAttack(data.second);
			data.first->calculateWeights(); //we just removed a move so we have to recalculate the weights to account for it
			changes.removeAttacks.pop();
		}
		while (!changes.linkedAttacks.empty()) { //change all changing stats
			pair<NPC*, Attack*>& data = changes.linkedAttacks.front();
			data.first->addSpecialAttack(data.second);
			if (data.first->getPlayerness()) { //print what the new attack does if it's the player
				cout << "\n" << data.first->getName() << " learned " << data.second->name << "!\n" << data.second->name << " - " << data.second->trueDesc;
				CinPause();
			} else { //we just added a move so we have to recalculate the weights to account for it
				data.first->calculateWeights();
			}
			changes.linkedAttacks.pop();
		}
		while (!changes.guardedItems.empty()) { //unguard all the items
			changes.guardedItems.front()->setGuard(NULL);
			changes.guardedItems.pop();
		}
		while (!changes.exitUnblocks.empty()) { //unblock the exits
			pair<Room*, const char*>& data = changes.exitUnblocks.front();
			data.first->unblockExit(data.second);
			changes.exitUnblocks.pop();
		}
		while (!changes.exitBlocks.empty()) { //block the exits
			tuple<Room*, const char*, const char*, const char*>& data = changes.exitBlocks.front();
			get<0>(data)->blockExit(get<1>(data), get<2>(data), get<3>(data));
			changes.exitBlocks.pop();
		}
		while (!changes.exitPavings.empty()) { //block the exits
			tuple<Room*, Room*, const char*, const char*>& data = changes.exitPavings.front();
			get<0>(data)->setExit(get<2>(data), get<1>(data)); //first room goes to second room via first exit
			get<1>(data)->setExit(get<3>(data), get<0>(data)); //second room goes to first room via second exit
			changes.exitPavings.pop();
		}
		while (!changes.exitDepavings.empty()) { //remove the exits
			pair<Room*, const char*>& data = changes.exitDepavings.front();
			data.first->removeExit(data.second);
			changes.exitDepavings.pop();
		}
		while (!changes.decruitLinks.empty()) { //make unrecruitable all the npcs
			NPC* data = changes.decruitLinks.front();
			data->setRecruitable(false);
			changes.decruitLinks.pop();
		}
		while (!changes.deleaderLinks.empty()) {
			NPC* data = changes.deleaderLinks.front();
			data->setLeader(false);
			data->setBoss(false); //falsify boss just in case
			data->undefeat();
			changes.deleaderLinks.pop();
		}
		while (!changes.conditionalDecruits.empty()) { //make unrecruitable all the npcs unless the condition paired to them is true
			pair<NPC*, size_t>& data = changes.conditionalDecruits.front();
			if (!WorldState[data.second]) data.first->setRecruitable(false);
			changes.conditionalDecruits.pop();
		}
		while (!changes.linkedWelcomes.empty()) { //unguard all the items
			pair<Room*, Conversation>& data = changes.linkedWelcomes.front();
			data.first->setWelcome(data.second);
			changes.linkedWelcomes.pop();
		}
		while (!changes.linkedItems.empty()) {
			pair<Item*, Room*>& data = changes.linkedItems.front();
			data.first->setRoom(data.second);
			changes.linkedItems.pop();
		}
		while (!changes.inventoryLinks.empty()) {
			pair<vector<Item*>*, Item*>& data = changes.inventoryLinks.front();
			data.first->push_back(data.second);
			changes.inventoryLinks.pop();
		}
		while (!changes.roamLinks.empty()) { //this is assuming we previously set roamrooms prior to setting roaming
			NPC* npc = changes.roamLinks.front();
			npc->setRoaming();
			npc->roam();
			changes.roamLinks.pop();
		}
		while (!changes.pursueLinks.empty()) {
			pair<NPC*, NPC*>& data = changes.pursueLinks.front();
			if (NPC* pursued = data.first->getPursuing()) { //nullify the pursuer because they're no longer pursuing this one
				pursued->setPursuer(NULL);
			}
			data.first->setPursuing(data.second); //first npc pursues second npc
			if (data.second) data.second->setPursuer(data.first); //if there was a second npc, tell it it's being pursued by the first one
			changes.pursueLinks.pop();
		}
		while (!changes.dismissableLinks.empty()) {
			pair<NPC*, size_t>& data = changes.dismissableLinks.front();
			if (!WorldState[data.second]) {
				data.first->setDismissable(true);
			}
			changes.dismissableLinks.pop();
		}
		while (!changes.clingyLinks.empty()) {
			pair<NPC*, size_t>& data = changes.clingyLinks.front();
			if (!WorldState[data.second]) {
				data.first->setDismissable(false);
			}
			changes.clingyLinks.pop();
		}
		while (!changes.linkedGifts.empty()) { //give gifts
			pair<NPC*, Item*>& data = changes.linkedGifts.front();
			data.first->setGift(data.second);
			changes.linkedGifts.pop();
		}
		while (!changes.linkedDegifts.empty()) { //remove gifts
			NPC* data = changes.linkedDegifts.front();
			data->setGift(NULL); //gifts don't know they're gifts so we don't have to edit their logic and stuff after setting the gift to NULL
			changes.linkedDegifts.pop();
		}
		while (!changes.linkedBackups.empty()) { //place an item in the room if it's not in the inventory
			tuple<Room*, Item*, vector<Item*>*>& data = changes.linkedBackups.front();
			if (!getItemInVector(*get<2>(data), get<1>(data)->getName())) {
				get<0>(data)->setItem(get<1>(data));
			}
			changes.linkedBackups.pop();
		}
		while (!changes.linkedEnterChanges.empty()) {
			pair<Room*, shared_ptr<WorldChange>>& data = changes.linkedEnterChanges.front();
			data.first->setEnterChanges(*data.second);
			changes.linkedEnterChanges.pop();
		}
		while (!changes.removeStock.empty()) { //remove gifts
			pair<Room*, Item*>& data = changes.removeStock.front();
			data.first->removeStock(data.second, false); //remove the item and don't print we did that
			changes.removeStock.pop();
		}
		while (!changes.linkedStations.empty()) { //make the linked stations be stations
			changes.linkedStations.front()->setStation();
			changes.linkedStations.pop();
		}
		if (Item* orb = changes.linkedOrb) { //petrify the linked escape/entry orb
			((EscapeOrb*)orb)->petrify();
		}
		if (changes.worldcon != NEVER) { //NEVER will never be true, but otherwise we set that this thing has been done
			WorldState[changes.worldcon] = true;
		}
	}
	//gives the player the attack the weapon item has, weapon items can be gotten through different places so we have this helper
	void applyWeaponAttack(NPC* player, Item* item) {
		WeaponItem* weapon = (WeaponItem*)item;
		Attack* attack = weapon->getAttack();
		player->addSpecialAttack(attack);
		cout << player->getName() << " can now use " << attack->name << "!\n" << attack->name << " - " << attack->trueDesc;
	}
	//get if the direction given is a cardinal direction
	bool getCardinal(const char* direction) {
		return !strcmp(direction, "NORTH") || !strcmp(direction, "SOUTH") || !strcmp(direction, "EAST") || !strcmp(direction, "WEST") || !strcmp(direction, "NORTHEAST") || !strcmp(direction, "NORTHWEST") || !strcmp(direction, "SOUTHEAST") || !strcmp(direction, "SOUTHWEST") || !strcmp(direction, "UP") || !strcmp(direction, "DOWN"); //never seat eoggy waffles
	}
	//get the title screen so we can print it. This is so I can modify it from one place since it gets printed in the beginning but also endings
	const char* getTitle() {
		return "BURGER QUEST 2:"
			 "\nELECTRIC BOOGALOO"
			 "\n"
			 "\nVersion Alpha 1.0"
			 "\n(c) 2026 Tomas Carranza Echaniz"
			 "\n";
	}
	//set up all the npc maps, including reversing the npc to char map so we can get the npc from the char
	void buildNPCData() {
		for (map<NPC*, char>::iterator npcchar = npcChar.begin(); npcchar != npcChar.end(); npcchar++) {
			charNPC[npcchar->second] = npcchar->first; //reverse the npc map for convenience
			//set up the npc statistic maps if it isn't the banker or the tunnel lobster because they don't fight (well the lobster fights like once but this is more for the player team)
			if (npcchar->second != 'n' && npcchar->second != 't') {
				attackslaunched[npcchar->first] = 0;
				helpslaunched[npcchar->first] = 0;
				damagedealt[npcchar->first] = 0;
				healthhealed[npcchar->first] = 0;
				healthrecovered[npcchar->first] = 0;
				damagerecieved[npcchar->first] = 0;
				knockouts[npcchar->first] = 0;
				revives[npcchar->first] = 0;
			}
		}
	}
	//reset all the external maps and thingymadoodles to not be full of garbage data
	void emptyExterns() {
		ReverseDirection.clear();
		npcChar.clear();
		chaNPC.clear();
		memset(commandcount, 0, sizeof(commandcount));
		attackslaunched.clear();
		helpslaunched.clear();
		damagedealt.clear();
		healthhealed.clear();
		healthrecovered.clear();
		damagerecieved.clear();
		knockouts.clear();
		revives.clear();
		encountered.clear();
		recruited.clear();
		roomsH.clear();
		npcsH.clear();
		itemsH.clear();
		attacksH.clear();
		effectsH.clear();
		relaysH.clear();
		invalidcommand = 0; //reset all these stats also
		invalidnpc = 0;
		invalidmove = 0;
		invaliditem = 0;
		nothingtosay = 0;
		actionwhat = 0;
		sessions = 0;
		playtime = 0;
		npcID = 0; //reset ids as well!
		itemID = 0;
		roomID = 0;
		//all world states are reset to false
		for (size_t i = 0; i <= NEVER) WorldState[i] = 0;
		delete[] sectionW; //delete the world change tracker
		memset(sectionT, NULL, sizeof(sectionT)); //use sizeof since pointer size may vary by system
	}
	//go to the parents until we reach the template
	const NPC* getBase(const NPC* npc) {
		for (; npc->getParent(); npc = npc->getParent()); //go to the parent until there is no parent
		return npc; //this one has no parent so this is the base npc so we return this
	}
	//loads all the saves the player has in their files, we call this every time we do one of the other commands to make sure it's up to date with anything modified externally MARK: load saves
	//this only works if all the files are in the format saveN.bq2 and there are no gaps, which should be the case unless the player was messing around with the files, which isn't my fault
	void loadSaves() {
		for (Save* save : saves) delete save; //delete all the old saves
		saves.clear(); //reset the vector in case we were reloading
		char filename[255]; //we use this to get the current name of the file
		//load all the saves in the order of the number in their name
		for (size_t i = 1;; i++) {
			snprintf(filename, 255, "save%zu.bq2", i); //get what the name of the current file should be
			ifstream savefile(filename); //try to get the file
			if (!savefile) break; //stop getting files if no such file was found
			savefile.seekg(0, ios::end); //go to the end of the file so we can get the length via tellg
			streamsize filelen = savefile.tellg(); //get the length of the save file data
			savefile.seekg(0, ios::beg); //go to the beginning of the file so we can read it normally
			char* savedata = new char[filelen+1]; //allocate a charray to write the data into
			savefile.read(savedata, filelen); //write the save file data into the charray so the program can use it
			savedata[filelen] = '\0'; //null terminate the data
			saves.push_back(new Save(savedata, saves.size()+1)); //make a new save file with the new data
			delete[] savedata;
		}
	}

	//all the player's saves
	vector<Save*> saves;

	//map to find the opposite of the given direction (e.g. ReverseDirection[SOUTH] == NORTH)
	map<const char*, const char*> ReverseDirection;

	//stuff that we need to be able to update as we play the game, that the save system uses, or npc statistics

	//map to find the char that represents each recruitable npc (e.g. npcChar[self] == a)
	map<NPC*, char> npcChar;
	map<char, NPC*> charNPC; //map to get the npc from the char

	char* sectionW; //build section W as we play
	Room* sectionT[5] = {NULL};

	int commandcount[18] = {0}; //count how many times we used each command
	int invalidcommand = 0;
	int invalidnpc = 0;
	int invalidmove = 0;
	int invaliditem = 0;
	long long nothingtosay = 0; //how many times player entered nothing into a >
	int actionwhat = 0; //how many times the player tried to use a command but never specified who or what the command was directed towards
	int sessions = 0; //how many times the player has booted up this save
	double playtime = 0; //total time the player has played in this save for

	//track these for every teammate
	map<NPC*, int> attackslaunched; //how many times each npc attacked
	map<NPC*, int> helpslaunched; //how many times each npc did a beneficial attack
	map<NPC*, long long> damagedealt; //total damage this npc has dealt
	map<NPC*, long long> healthhealed; //total healing this npc has done to other npcs
	map<NPC*, long long> damagerecieved; //how much damage this npc has tanked
	map<NPC*, long long> healthrecovered; //how much this npc has healed in total
	map<NPC*, int> knockouts; //how many npcs this npc incapacitated
	map<NPC*, int> revives; //how many npcs this npc recapacitated

	set<NPC*> encountered; //all the npcs we've ever fought
	set<NPC*> recruited; //all the npcs we've ever recruited

	vector<Room*> roomsH; //vectors of everything in memory so we can deallocate them all later
	vector<NPC*> npcsH;
	vector<Item*> itemsH;
	vector<Attack*> attacksH;
	vector<Effect*> effectsH;
	vector<shared_ptr<Conversation>> relaysH;

	bool WorldState[NEVER+1] = {false}; //every world state starts as false (NEVER + 1 because the last enumerator is equal enum size minus 1, since it starts at 0)

	int npcID = 0; //npcID starts at 0, since it's the npcsH index
	int itemID = 0; //same thing for these vectors
	int roomID = 0;
}
