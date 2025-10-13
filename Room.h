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
	Room();
	~Room();

private:
	vector<Item*> items;
	vector<NPC*> npcs;
	map<char[255], Room> exits;
};
#endif