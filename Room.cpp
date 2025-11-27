#include "Room.h"
#include "NPC.h"
#include "Item.h"
#include <cstring>
#include <iostream>
#include <algorithm>
#include "Helper.h"
using namespace std;
using namespace Helper;

Room::Room(const char _description[255]) {
	strcpy(description, _description);
}
Room::~Room() {

}
char* Room::getDescription() {
	return &description[0];
}
vector<Item*> Room::getItems() {
	return items;
}
vector<Item*> Room::getStock() {
	return stock;
}
vector<NPC*> Room::getNpcs() {
	return npcs;
}
vector<char*> Room::getBlocks() {
	return blockedExits;
}
Room* Room::getExit(char* direction) {
	map<char*, Room*, charComparer>::iterator exiterator = exits.find(direction);
	if (exiterator == exits.end()) {
		return NULL;
	}
	return exiterator->second;
}
bool Room::getBlocked(char* direction) {
	for (char* exit : blockedExits) {
		if (!strcmp(exit, direction)) {
			return true;
		}
	}
	return false;
}
char* Room::getBlockReason(char* direction) {
	if (getBlocked(direction)) {
		return blockReason[direction];
	}
	return NULL;
}

//prints all the exits from this room; I tried just returning the map, but it was annoying because of the custom comparator so I just do this
void Room::printExits() {
	cout << "\nExits: ";
	int i = 0;
	//iterates through the map using exiterator (get it? exit + iterator AAHAHHAHAHHAHAHAHHAHAHA so amazing)
	for (map<char*, Room*, charComparer>::iterator exiterator = exits.begin(); exiterator != exits.end(); ++exiterator) {
		cout << exiterator->first;
		if (i++ < exits.size()-1) {
			cout << ", ";
		}
	}
}
//beautiful, gramatically correct (wait nevermind no it isn't) function for printing all items in the room
void Room::printItems() {
	int len = items.size();
	//if there are 0 items here, don't print anything
	if (len < 1) {
		return;
	}
	cout << "\nThere is " << items[0]->getName();

	if (len > 2) {
		cout << ",";
	}
	cout << " ";

	for (int i = 1; i < len - 1; i++) {
		cout << items[i]->getName();
		if (len > 2) {
			cout << ",";
		}
		cout << " ";
	}
	
	if (len > 1) {
		cout << "and " << items[len - 1]->getName() << " ";
	}
	cout << "here.";
}
void Room::printStock() {
	int len = stock.size();
	if (len < 1) {
		return;
	}
	cout << "\nItems for sale:";
	int i = 0;
	for (Item* item : stock) {
		int price = item->getPrice();
		cout << " " << item->getName() << " (" << price << " mon";
		if (price == 1) {
			cout << "y)";
		} else {
			cout << "ies)";
		}
		if (i++ != 0) {
			cout << ",";
		}
	}
}
void Room::printNPCs() {
	int i = 0;
	for (NPC* npc : npcs) {
		if (!npc->getRecruited() && !npc->getDefeated()) {
			i++;
		}
	}
	if (i < 1) {
		return;
	}
	cout << "\nNPCs:";
	int j = 0;
	for (NPC* npc : npcs) {
		if (!npc->getRecruited() && !npc->getDefeated()) {
			if (strlen(npc->getTitle()) > 0) {
				cout << " ";
			}
			cout << npc->getTitle() << " " << npc->getName();
			if (++j != i) {
				cout << ",";
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
	/*cout << "\nWelcome to " << welcomeMessage << "!";
	CinPause();
	cout << "<<< " << welcomeTitle << " >>>";
	CinPause();
	cout << welcomeDescription;
	CinPause();*/
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
//do we ever use the last two arguments? remove if no
void Room::setExit(char* direction, Room* room, char* blocktype, char* _reason) {
	exits[direction] = room;
	if (blocktype != NULL) {
		blockExit(direction, blocktype, _reason);
	}
}
void Room::setDescription(const char _description[255]) {
	strcpy(description, _description);
}
void Room::setWelcome(const char text[255]) {
	welcomeText.push_back(text);
	/*strcpy(welcomeMessage, _welcome);
	strcpy(welcomeTitle, _title);
	strcpy(welcomeDescription, _description);*/
	welcome = true;
}
void Room::setStock(Item* item, int amount, int price, const char buydesc[255]) {
	item->setStock(amount, price, buydesc);
	stock.push_back(item);
}
void Room::removeStock(Item* item) {
	delete item;
	stock.erase(remove(stock.begin(), stock.end(), item), stock.end());
}
void Room::blockExit(char* direction, char* blocktype, const char _reason[255]) {  
	blockedExits.push_back(direction);
	blockType[direction] = blocktype; //do we even need this?
	char* reason = new char[255];
	strcpy(reason, _reason);
	blockReason[direction] = reason;
}
void Room::unblockExit(char* direction) {
	blockedExits.erase(remove(blockedExits.begin(), blockedExits.end(), direction), blockedExits.end());
}
vector<char*> Room::unblockAll(char* type) {
	vector<char*> matches;
	for (char* exit : blockedExits) {
		if (blockType[exit] == type) {
			unblockExit(exit);
			matches.push_back(exit);
		}
	}
	return matches;
}
void Room::undefeatEnemies() {
	for (NPC* npc : npcs) {
		//probbaly do a check if you should undefeat them
		if (npc->getDefeated()) {
			npc->undefeat();
		}
	}
}