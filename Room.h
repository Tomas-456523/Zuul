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

//this makes everything be in alphabetical order unfortunately, but it does allow me to store use strings as keys
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
	bool getBlocked(char* direction);

	void printExits();
	void printItems();
	void printNPCs();
	void printWelcome();
	void printBlocks();
	void printBlock(char* direction);

	void setItem(Item* item);
	void setNPC(NPC* npc);
	void removeNPC(NPC* npc);
	void removeItem(Item* item);
	void setExit(char* direction, Room* room);
	void setDescription(const char _description[255]); //reset the description, used by items that change things
	void setWelcome(const char _welcome[255], const char _title[255], const char _description[255]); //set a welcome to an area

	void blockExit(char* direction, char* blocktype, const char reason[255]);
	void unblockExit(char* direction);
private:
	char description[255];
	char welcomeMessage[255];
	char welcomeTitle[255];
	char welcomeDescription[255];

	bool welcome = false; //if this room has a welcome message for an area

	vector<Item*> items;
	vector<NPC*> npcs;
	//input direction pointer and returns the room that is in that direction
	map<char*, Room*, charComparer> exits;
	vector<char*> blockedExits;
	//input direction pointer and returns the reason why the exit is blocked
	map<char*, char*, charComparer> blockReason;
	map<char*, char*, charComparer> blockType;
};
#endif