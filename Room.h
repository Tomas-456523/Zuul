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
	bool operator()(const char* a, const char* b) const {
		return strcmp(a,b) < 0;
	}
};

class Room {
public:
	Room(const char _description[255]);
	~Room();

	char* getDescription();
	vector<Item*> getItems();
	vector<Item*> getStock();
	vector<NPC*> getNpcs();
	vector<char*> getBlocks();
	Room* getExit(char* direction);
	Room* getRedirect();
	bool getStation();
	bool getGym();
	bool getBlocked(char* direction);
	char* getBlockReason(char* direction);

	void printExits();
	void printItems();
	void printStock();
	void printNPCs();
	void printWelcome();
	void printBlocks();
	void printBlock(char* direction);

	void setItem(Item* item);
	void setNPC(NPC* npc);
	void removeNPC(NPC* npc);
	void removeItem(Item* item);
	void setExit(char* direction, Room* room, char* blocktype = NULL, char* _reason = NULL);
	void setRedirect(Room* room);
	void setStation(bool stat = true);
	void setGym(bool _gym = true);
	void setConveyor(Room* altroom, char* conveyorexit);
	void setDescription(const char _description[255]); //reset the description, used by items that change things
	void setWelcome(const char text[255]); //set welcome text for the area
	void setStock(Item* item, int stock, int price, const char buydesc[255] = "");
	void removeStock(Item* item);

	void switchConveyor(); //switches the direction of the conveyor

	void blockExit(char* direction, char* blocktype, const char reason[255]);
	void unblockExit(const char* direction);
	vector<char*> unblockAll(char* type);

	void scaleNPCs(int level);

	void undefeatEnemies();
private:
	char description[255];
	
	vector<const char*> welcomeText;

	bool welcome = false; //if this room has a welcome message for an area

	vector<Item*> items;
	vector<Item*> stock;
	vector<NPC*> npcs;
	//input direction pointer and returns the room that is in that direction
	map<char*, Room*, charComparer> exits;
	vector<char*> blockedExits;
	//input direction pointer and returns the reason why the exit is blocked
	map<char*, char*, charComparer> blockReason;
	map<char*, char*, charComparer> blockType;

	Room* redirect = NULL;

	char* conveyorExit = NULL;
	Room* altRoom = NULL; //uses by conveyor belts, their direction switches to altroom when a switch item is used

	bool station;
	bool gym;
};
#endif