//header file for rooms

#ifndef ROOM
#define ROOM

#include <vector>
#include <map>
#include "NPC.h"
#include "Item.h"
using namespace std;

class NPC;

class Room {
public:
	Room(char _description[255]);
	~Room();

	char* getDescription();
	vector<Item*> getItems();
	vector<NPC*> getNpcs();
	Room* getExit(char direction);

	void setItem(Item* item);
	void setNpcs(NPC* npc);
	void setExit(char direction, Room* room);
private:
	char description[255];

	vector<Item*> items;
	vector<NPC*> npcs;
	map<char, Room*> exits;
};
#endif