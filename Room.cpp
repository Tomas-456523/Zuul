//implementation file for rooms

#include "Room.h"
#include "NPC.h"
#include "Item.h"
#include <cstring>
#include <iostream>
#include <algorithm>
#include <utility>
#include "Helper.h"
using namespace std;
using namespace Helper;

//constructs the room and sets its description
Room::Room(const char* _description) {
	description = _description;
	roomsH.push_back(this); //store a pointer to this room in the rooms vector
}
//destructor for the room
Room::~Room() {

}
//a bunch of functions for getting room variables
const char* Room::getDescription() {
	return description;
}
vector<Item*>& Room::getItems() { //used in this same room's functions in case there it shares items with another room
	return sharedItems ? *sharedItems : items;
}
vector<Item*>& Room::getStock() {
	return stock;
}
vector<NPC*>& Room::getNpcs(bool alt) {
	if (!alt) {
		return npcs;
	} else {
		return altNpcs;
	}
}
vector<const char*>& Room::getBlocks() {
	return blockedExits;
}
Room* Room::getExit(const char* direction) { //returns the room that corresponds to the given exit
	map<const char*, Room*, charComparer>::iterator exiterator = exits.find(direction); //makes sure the exit actually exists. If we didn't check this, going in an invalid direction would create ghost exits
	if (exiterator == exits.end()) {
		return NULL;
	}
	return exiterator->second; //return the room
}
Room* Room::getRedirect() {
	return redirect;
}
bool Room::getStation() {
	return station;
}
bool Room::getGym() {
	return gym;
}
bool Room::getTempleEntrance() {
	return templeentrance;
}
bool Room::getBlocked(const char* direction) { //get if the exit is blocked
	for (const char* exit : blockedExits) {
		if (!strcmp(exit, direction)) { //if the exit was in the vetcor then it's blocked
			return true;
		}
	}
	return false;
}
const char* Room::getBlockReason(const char* direction) { //get why it's blocked
	if (getBlocked(direction)) {
		return blockReason[direction];
	}
	return NULL; //null because it's not actually blocked
}
Item* Room::popBackup() {
	Item* _backup;
	if (onebackup) { //if there is only one backup
		_backup = backup;
		backup = NULL; //make gift NULL because we only give one gift
	} else if (!getItemInVector(items, backup->getName())) { //if there is endless backups (and there isn't one just lying on the floor), give a copy of the backup
		_backup = backup->Duplicate();
	}
	return _backup;
}
//do changes that are done when entering the room
void Room::doEnterChanges() { //don't do the change if it has none or if the condition isn't met and we have a condition set to something other than NEVER
	if (!hasenterchanges || !WorldState[enterchangecondition] && enterchangecondition != NEVER) return;
	applyWorldChange(enterchange);
	hasenterchanges = false; //don't do it again
}
//prints all the exits from this room; I tried just returning the map, but it was annoying because of the custom comparator so I just do this
void Room::printExits() {
	cout << "\nExits: ";
	int i = 0;
	//iterates through the map using exiterator
	for (map<const char*, Room*, charComparer>::iterator exiterator = exits.begin(); exiterator != exits.end(); ++exiterator) {
		cout << exiterator->first;
		if (i++ < exits.size()-1) {
			cout << ", "; //seperate using commas except for the last one because there's nothing to seperate
		}
	}
}
//beautiful, gramatically correct (wait nevermind no it isn't) function for printing all items in the room
//1 item: There is THINGY here.
//2 items: There is THINGY and THINGAMABOB here.
//3+ items: There is THINGY, THINGAMABOB, and THINGAMAJIG here.
//and so on
void Room::printItems() {
	vector<Item*>& items = getItems(); //get items like this because spamming getItems() everywhere looked ugly
	int len = items.size();
	if (len < 1) { //if there are 0 items here, don't print anything
		return;
	}
	cout << "\nThere is " << items[0]->getName(); //prints the first item

	if (len > 2) { //if there are more than two items, we need to use commas
		cout << ",";
	}
	cout << " "; //space no matter what

	for (int i = 1; i < len - 1; i++) { //print the rest of the items minus the last one if there are 3+ items
		cout << items[i]->getName() << ", "; //seperate with comma
	}
	
	if (len > 1) { //print the final item with "and" included (if >1 items)
		cout << "and " << items[len - 1]->getName() << " ";
	}
	cout << "here."; //clarifies that it's here and not over there
}
//print all the items for sale
void Room::printStock() {
	int len = stock.size();
	if (len < 1) { //return if nothing for sale
		return;
	}
	cout << "\nItems for sale:";
	int i = 0;
	for (Item* item : stock) {
		int price = item->getPrice();
		cout << " " << item->getName() << " (" << price << " mon"; //prints the name and price, word finished later
		if (price == 1) { //proper grammar based on price of 1 or not
			cout << "y)";
		} else {
			cout << "ies)";
		}
		if (++i != stock.size()) {
			cout << ","; //seperate by comma if not the last one
		}
	}
}
void Room::printNPCs() {
	vector<NPC*> validNPCs;
	for (NPC* npc : npcs) {
		if (!npc->getRecruited() && !npc->getDefeated() && (!npc->getGuarding() || !npc->getGuarding()->getRecruited())) { //only count non-recruited and non-defeated npcs (and if they're not the guard of a recruited npc)
			validNPCs.push_back(npc);
		}
	}
	for (NPC* npc : altNpcs) { //get the adjecent-roomed enemy npcs
		if (!npc->getDefeated()) { //only count non-defeated npcs
			validNPCs.push_back(npc);
		}
	}
	if (!validNPCs.size()) { //return if no npcs
		return;
	}
	cout << "\nNPCs:";
	int i = 0; //count how many npcs we've printed
	for (NPC* npc : validNPCs) {
		if (strlen(npc->getTitle()) > 0) { //if npc has a title, add a space for formatting (if no title, space gets added immediately after anyway)
			cout << " ";
		}
		cout << npc->getTitle() << " " << npc->getName();
		if (++i != validNPCs.size()) {
			cout << ","; //seperate by comma if not the last one
		}
	}
}
void Room::printBlocks() {
	for (const char* direction : blockedExits) { //only print it if it's an actual exit (cause manhole covers, basically)
		if (getExit(direction)) cout << "\nThe " << direction << " exit is " << blockReason[direction];
	}
}
void Room::printBlock(const char* direction) {
	cout << "\nThe " << direction << " exit is " << blockReason[direction];
}
void Room::printWelcome() {
	if (!welcome) return;
	welcome = false; //we only do the welcome once
	if (welcomeText.getOutdated()) return; //don't do outdated welcomes
	printConversation(&welcomeText, true);
}
void Room::setItem(Item* item) {
	getItems().push_back(item);
}
void Room::shareItems(Room* room) {
	sharedItems = &room->getItems();
}
void Room::setNPC(NPC* npc, bool alt) {
	if (!alt) {
		npcs.push_back(npc);
	} else {
		altNpcs.push_back(npc);
	}
}
void Room::removeNPC(NPC* npc, bool alt) {
	if (!alt) {
		npcs.erase(remove(npcs.begin(), npcs.end(), npc), npcs.end());
	} else {
		altNpcs.erase(remove(altNpcs.begin(), altNpcs.end(), npc), altNpcs.end());
	}
}
void Room::removeItem(Item* item) {
	vector<Item*>& items = getItems();
	items.erase(remove(items.begin(), items.end(), item), items.end());
}
//sets an exit to another room, and blocks it if specified
void Room::setExit(const char* direction, Room* room, const char* blocktype, const char* _reason) {
	exits[direction] = room;
	if (blocktype != NULL) { //blocks the exit
		blockExit(direction, blocktype, _reason);
	}
}
//removes an exit to the room
void removeExit(const char* direction) {
	
}
void Room::setRedirect(Room* room) {
	redirect = room;
	for (Item* item : getItems()) { //moves all this room's items to the redirect room, in case the player dropped some items here
		item->setRoom(redirect); //item's setRoom calls setItem and removeItem as well
	}
}
void Room::setStation(bool stat) {
	station = stat;
}
void Room::setGym(bool _gym) {
	gym = _gym;
}
void Room::setTempleEntrance(const char* exit, Room* temple, const Conversation& opentext) {
	templeentrance = true;
	templesettings = {exit, temple};
	templeopenconvo = opentext;
}
void Room::setConveyor(Room* altroom, const char* conveyorexit) {
	altRoom = altroom;
	conveyorExit = conveyorexit;
}
void Room::setDescription(const char* _description) {
	description = _description;
}
void Room::setWelcome(Conversation text) {
	welcomeText = text;
	welcome = true;
}
void Room::setStock(Item* item, int amount, int price, const Conversation& buydesc) {
	item->setStock(amount, price, buydesc);
	stock.push_back(item);
}
void Room::removeStock(Item* item, bool printmessage) { //we uncatalogue the item when it runs out
	if (printmessage) cout << "You bought the last " << item->getName() << "! It's sold out!"; //lets the player know it's sold out
	stock.erase(remove(stock.begin(), stock.end(), item), stock.end()); //remove it from the list of stock
}
void Room::setBackup(Item* item, bool onlyone) {
	backup = item;
	onebackup = onlyone;
}
//make the temple enterable if this is a temple entrance
void Room::openTemple() {
	this->setExit(templesettings.first, templesettings.second);
	printConversation(&templeopenconvo, false);
	templeentrance = false; //so we can't do all this again
}
void Room::setEnterChanges(const WorldChange& changes, size_t condition) {
	enterchange = changes;
	enterchangecondition = condition;
	hasenterchanges = true;
}
void Room::switchConveyor() { //swaps the altRoom and the FORWARD exit, effectively reversing the conveyor movement
	swap(altRoom, exits[conveyorExit]);
}
//block the exit, and save why it's blocked and a description of the blockage
void Room::blockExit(const char* direction, const char* blocktype, const char* reason) {
	blockedExits.push_back(direction);
	blockType[direction] = blocktype;
	blockReason[direction] = reason;
}
void Room::unblockExit(const char* direction) { //erase the block so it's not tracked and therefore not blocked anymore
	blockedExits.erase(remove(blockedExits.begin(), blockedExits.end(), direction), blockedExits.end());
}
vector<const char*> Room::unblockAll(const char* type) { //unblock all the blocked exits of the given block type
	vector<const char*> matches; //found exits of that block type
	for (const char* exit : blockedExits) { //unblocks them
		if (blockType[exit] == type) {
			unblockExit(exit);
			matches.push_back(exit);
		}
	}
	return matches; //returns exits that were unblocked
}
void Room::scaleNPCs(int cap) { //levels up every npc in the gym depending on the time they've been there
	time_t now = time(NULL); //gets what time it is right now
	for (NPC* npc : npcs) {
		time_t start = npc->getGymStart();
		if (!start) continue; //if they have no gym start time, there's nothing to do here
		int level = (now - start)*(cap-npc->getLevel()+1)/600 + npc->getLevel(); //calculate new level based on time difference, 1 level per minute by default, plus a "catching up" factor (level difference / 10)
		npc->setLevel(min(level, cap)); //set the new level, capped at the given cap (player level - 1)
	}
}
void Room::undefeatEnemies() {
	for (NPC* npc : npcs) {
		if (npc->getDefeated() && npc->getRespawn()) { //get respawn also checks if it should respawn based on respawn req (npc that must be in the same room and not recruited)
			npc->undefeat();
		}
	}
}