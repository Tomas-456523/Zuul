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
Room::Room(const char _description[255]) {
	strcpy(description, _description);
	//roomsH.push_back(this); //store a pointer to this room in the rooms vector
}
//destructor for the room
Room::~Room() {

}
//a bunch of functions for getting room variables
char* Room::getDescription() {
	return &description[0];
}
vector<Item*>& Room::getItems() {
	return items;
}
vector<Item*>& Room::getStock() {
	return stock;
}
vector<NPC*>& Room::getNpcs() {
	return npcs;
}
vector<char*>& Room::getBlocks() {
	return blockedExits;
}
Room* Room::getExit(char* direction) { //returns the room that corresponds to the given exit
	map<char*, Room*, charComparer>::iterator exiterator = exits.find(direction); //makes sure the exit actually exists. If we didn't check this, going in an invalid direction would create ghost exits
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
bool Room::getBlocked(char* direction) { //get if the exit is blocked
	for (char* exit : blockedExits) {
		if (!strcmp(exit, direction)) { //if the exit was in the vetcor then it's blocked
			return true;
		}
	}
	return false;
}
char* Room::getBlockReason(char* direction) { //get why it's blocked
	if (getBlocked(direction)) {
		return blockReason[direction];
	}
	return NULL; //null because it's not actually blocked
}

//prints all the exits from this room; I tried just returning the map, but it was annoying because of the custom comparator so I just do this
void Room::printExits() {
	cout << "\nExits: ";
	int i = 0;
	//iterates through the map using exiterator
	for (map<char*, Room*, charComparer>::iterator exiterator = exits.begin(); exiterator != exits.end(); ++exiterator) {
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
	int i = 0; //how many npcs are listed
	for (NPC* npc : npcs) {
		if (!npc->getRecruited() && !npc->getDefeated()) { //only count non-recruited and non-defeated npcs
			i++;
		}
	}
	if (i < 1) { //return if no npcs
		return;
	}
	cout << "\nNPCs:";
	int j = 0; //count how many npcs we've printed
	for (NPC* npc : npcs) {
		if (!npc->getRecruited() && !npc->getDefeated()) {
			if (strlen(npc->getTitle()) > 0) { //if npc has a title, add a space for formatting (if no title, space gets added immediately after anyway)
				cout << " ";
			}
			cout << npc->getTitle() << " " << npc->getName();
			if (++j != i) {
				cout << ","; //seperate by comma if not the last one
			}
		}
	}
}
void Room::printBlocks() {
	for (char* direction : blockedExits) {
		cout << "\nThe " << direction << " exit is " << blockReason[direction];
	}
}
void Room::printBlock(char* direction) {
	cout << "\nThe " << direction << " exit is " << blockReason[direction];
}
void Room::printWelcome() {
	if (!welcome) {
		return;
	}
	cout << "\n";
	for (const char* text : welcomeText) {
		cout << text;
		CinPause();
	}
	welcome = false; //we only do the welcome once
}
void Room::setItem(Item* item) {
	items.push_back(item);
}
void Room::setNPC(NPC* npc) {
	npcs.push_back(npc);
}
void Room::removeNPC(NPC* npc) {
	npcs.erase(remove(npcs.begin(), npcs.end(), npc), npcs.end());
}
void Room::removeItem(Item* item) {
	items.erase(remove(items.begin(), items.end(), item), items.end());
}
//sets an exit to another room, and blocks it if specified
void Room::setExit(char* direction, Room* room, char* blocktype, char* _reason) {
	exits[direction] = room;
	if (blocktype != NULL) { //blocks the exit
		blockExit(direction, blocktype, _reason);
	}
}
void Room::setRedirect(Room* room) {
	redirect = room;
}
void Room::setStation(bool stat) {
	station = stat;
}
void Room::setGym(bool _gym) {
	gym = _gym;
}
void Room::setConveyor(Room* altroom, char* conveyorexit) {
	altRoom = altroom;
	conveyorExit = conveyorexit;
}
void Room::setDescription(const char _description[255]) {
	strcpy(description, _description);
}
void Room::setWelcome(const char text[255]) {
	welcomeText.push_back(text);
	welcome = true;
}
void Room::setStock(Item* item, int amount, int price, const char buydesc[255]) {
	item->setStock(amount, price, buydesc);
	stock.push_back(item);
}
void Room::removeStock(Item* item) { //we uncatalogue the item when it runs out
	cout << "You bought the last " << item->getName() << "! It's sold out!"; //lets the player know it's sold out
	stock.erase(remove(stock.begin(), stock.end(), item), stock.end()); //remove it from the list of stock
}
void Room::switchConveyor() { //swaps the altRoom and the FORWARD exit, effectively reversing the conveyor movement
	swap(altRoom, exits[conveyorExit]);
}
//block the exit, and save why it's blocked and a description of the blockage
void Room::blockExit(char* direction, char* blocktype, const char _reason[255]) {
	blockedExits.push_back(direction);
	blockType[direction] = blocktype;
	char* reason = new char[255];
	strcpy(reason, _reason);
	blockReason[direction] = reason;
}
void Room::unblockExit(const char* direction) { //erase the block so it's not tracked and therefore not blocked anymore
	blockedExits.erase(remove(blockedExits.begin(), blockedExits.end(), direction), blockedExits.end());
}
vector<char*> Room::unblockAll(char* type) { //unblock all the blocked exits of the given block type
	vector<char*> matches; //found exits of that block type
	for (char* exit : blockedExits) { //unblocks them
		if (blockType[exit] == type) {
			unblockExit(exit);
			matches.push_back(exit);
		}
	}
	return matches; //returns exits that were unblocked
}
void Room::scaleNPCs(int level) { //levels up every npc in the room to the given level
	for (NPC* npc : npcs) {
		npc->setLevel(level);
	}
}
void Room::undefeatEnemies() {
	for (NPC* npc : npcs) {
		//probbaly do a check if you should undefeat them
		if (npc->getDefeated() && npc->getRespawn()) {
			npc->undefeat();
		}
	}
}