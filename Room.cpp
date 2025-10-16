#include "Room.h"
#include "NPC.h"
#include "Item.h"
#include <cstring>
#include <iostream>
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
	cout << "\nExits:"
	for (map<char*, Room*, charComparer>::iterator exiterator = exits.begin(); exiterator != exits.end(); ++exiterator) {
		cout << " " << exiterator->first;
	}
}
void Room::printItems() {
	int len = items.size();
	if (len == 1) {
		//You know what just make this later when you actually need it
	}
}
void Room::printNPCs() {
	cout << "\nNPCs:";
	for (NPC* npc : npcs) {
		if (!npc.getRecruited()) {
			cout << " " << npc->getName();
		}
	}
}
void Room::setItem(Item* item) {
	items.push_back(item);
}
void Room::setNPC(NPC* npc) {
	npcs.push_back(npc);
}
void Room::setExit(char* direction, Room* room) {
	exits[direction] = room;
}