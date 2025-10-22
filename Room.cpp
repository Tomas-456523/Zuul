#include "Room.h"
#include "NPC.h"
#include "Item.h"
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

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
vector<NPC*> Room::getNpcs() {
	return npcs;
}
Room* Room::getExit(char* direction) {
	return exits[direction];
}
//prints all the exits from this room; I tried just returning the map, but it was annoying because of the custom comparator so I just do this
void Room::printExits() {
	cout << "\nExits:";
	//iterates through the map using exiterator (get it? exit + iterator AAHAHHAHAHHAHAHAHHAHAHA so amazing)
	for (map<char*, Room*, charComparer>::iterator exiterator = exits.begin(); exiterator != exits.end(); ++exiterator) {
		cout << " " << exiterator->first;
	}
}
void Room::printItems() {
	int len = items.size();
	//if there are 0 items here, don't print anything
	if (len < 1) {
		return;
	}
	cout << "There is " << items[0]->getName();

	if (len > 2) {
		cout << ",";
	}
	cout << " ";

	for (int i = 1; i < len - 1; i++) {
		cout << items[i]->getName() << " ";
		if (len > 2) {
			cout << ",";
		}
		cout << " ";
	}
	
	if (len > 1) {
		cout << " and ";
	}
	cout << items[len-1]->getName();
	cout << "here.";
}
void Room::printNPCs() {
	if (npcs.size() < 1) {
		return;
	}
	cout << "\nNPCs:";
	for (NPC* npc : npcs) {
		if (!npc->getRecruited() && !npc->getPlayerness()) {
			cout << " " << npc->getTitle() << " " << npc->getName();
		}
	}
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
void Room::setExit(char* direction, Room* room) {
	exits[direction] = room;
}
