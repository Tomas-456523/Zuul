#include "Room.h"
#include "NPC.h"
#include "Item.h"
#include <cstring>
using namespace std;

Room::Room(char _description[255]) {
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
Room* Room::getExit(char direction) {
	return exits[direction];
}
void Room::setItem(Item* item) {
	items.push_back(item);
}
void Room::setNpcs(NPC* npc) {
	npcs.push_back(npc);
}
void Room::setExit(char direction, Room* room) {
	exits[direction] = room;
}