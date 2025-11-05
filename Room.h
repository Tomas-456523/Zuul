//header file for rooms

#ifndef ROOM
#define ROOM

#include <vector>
#include <map>
#include <cstring>
#include "NPC.h"
#include "Item.h"
using namespace std;

class NPC;
class Item;

struct charComparer {
	bool operator()(char* a, char* b) const {
		return strcmp(a,b) < 0;
	}
};

class Room {
public:
	Room(const char _description[255]);
	~Room();

	char* getDescription();
	vector<Item*> getItems();
	vector<NPC*> getNpcs();
	Room* getExit(char* direction);

	void printExits();
	void printItems();
	void printNPCs();
	void printWelcome();

	void setItem(Item* item);
	void setNPC(NPC* npc);
	void removeNPC(NPC* npc);
	void removeItem(Item* item);
	void setExit(char* direction, Room* room);
	void setDescription(const char _description[255]); //reset the description, used by items that change things
	void setWelcome(const char _welcome[255]); //set a welcome to an area
private:
	char description[255];
	char welcomeMessage[255];

	bool welcome = false; //if this room has a welcome message for an area

	vector<Item*> items;
	vector<NPC*> npcs;
	map<char*, Room*, charComparer> exits;
};
#endif