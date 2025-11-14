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
vector<NPC*> Room::getNpcs() {
	return npcs;
}
Room* Room::getExit(char* direction) {
	return exits[direction];
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
void Room::printNPCs() {
	int i = 0;
	for (NPC* npc : npcs) {
		if (!npc->getRecruited()) {
			i++;
		}
	}
	if (i < 1) {
		return;
	}
	cout << "\nNPCs:";
	for (NPC* npc : npcs) {
		if (!npc->getRecruited()) {
			cout << " " << npc->getTitle() << " " << npc->getName();
		}
	}
}
void Room::printWelcome() {
	if (!welcome) {
		return;
	}
	cout << "\nWelcome to " << welcomeMessage << "!";
	CinPause();
	cout << "<<< " << welcomeTitle << " >>>";
	CinPause();
	cout << welcomeDescription;
	CinPause();
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
void Room::setExit(char* direction, Room* room) {
	exits[direction] = room;
}
void Room::setDescription(const char _description[255]) {
	strcpy(description, _description);
}
void Room::setWelcome(const char _welcome[255], const char _title[255], const char _description[255]) {
	strcpy(welcomeMessage, _welcome);
	strcpy(welcomeTitle, _title);
	strcpy(welcomeDescription, _description);
	welcome = true;
}
void Room::blockExit(char* direction, char* blocktype, const char reason[255]) {
	blockedExits.push_back(direction);
	blockType[direction] = blocktype;
	strcpy(blockReason[direction], reason);
}