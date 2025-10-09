//header file for rooms

#include <vector>
#include <map>
#include "NPC.h"
using namespace std;

#ifndef ROOM
#define ROOM
class Room {
public:
	Room();
	~Room();

private: //REPLACE CHAR WITH FUTURE ITEM CLASS
	vector<char> items;
	vector<NPC> npcs;
	map<char[255], Room> exits;
};
#endif