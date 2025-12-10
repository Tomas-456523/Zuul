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

//used in my maps that use char*s as keys
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

	char* getDescription(); //get the description of the room
	vector<Item*>& getItems(); //gets the room's items
	vector<Item*>& getStock(); //gets the items for sale in this room
	vector<NPC*>& getNpcs(bool alt = false); //gets the npcs in this room
	vector<char*>& getBlocks(); //gets the blocked exits in this room
	Room* getExit(char* direction); //get the room in the given direction from this room
	Room* getRedirect(); //get if the room redirects to another one
	bool getStation(); //gets if this room is a station
	bool getGym(); //gets if this room is a gym
	bool getBlocked(char* direction); //gets if the given exit is blocked
	char* getBlockReason(char* direction); //gets why the exit is blocked

	void printExits(); //functions for printing the stuff in the room
	void printItems();
	void printStock();
	void printNPCs();
	void printWelcome();
	void printBlocks();
	void printBlock(char* direction);

	void setItem(Item* item); //puts an item in the room
	void setNPC(NPC* npc, bool alt = false); //puts an npc in the room
	void removeNPC(NPC* npc, bool alt = false); //removes the thing from the room
	void removeItem(Item* item);
	void setExit(char* direction, Room* room, char* blocktype = NULL, char* _reason = NULL); //sets an exit to another room
	void setRedirect(Room* room); //sets a redirect to another room
	void setStation(bool stat = true); //sets if this room is a station
	void setGym(bool _gym = true); //sets if this room is a gym
	void setConveyor(Room* altroom, char* conveyorexit); //sets if this room is a conveyor + references the FORWARD exit in conveyor rooms
	void setDescription(const char _description[255]); //reset the description, used by items that change things
	void setWelcome(const char text[255]); //set welcome text for the area
	void setStock(Item* item, int stock, int price, const char buydesc[255] = ""); //adds an item for sale
	void removeStock(Item* item); //removes an item from sale

	void switchConveyor(); //switches the direction of the conveyor

	void blockExit(char* direction, char* blocktype, const char reason[255]); //blocks an exit
	void unblockExit(const char* direction); //unblocks the exit
	vector<char*> unblockAll(char* type); //unblocks all the blocked exits of the given block type

	void scaleNPCs(int level); //sets all the npcs to the given level

	void undefeatEnemies(); //makes the enemies in the room not defeated anymore
private:
	char description[255]; //describes the player's surroundings in this room (eg. (You are ) "in an amazingly incredible room full of stuff.")
	
	vector<const char*> welcomeText; //message that is played when the player first gets here

	bool welcome = false; //if this room has a welcome message for an area

	vector<Item*> items; //items in this room
	vector<Item*> stock; //items for sale in this room
	vector<NPC*> npcs; //npcs in this room
	vector<NPC*> altNpcs; //npcs in an adjacent room that are blocking an exit in this room
	//input direction pointer and returns the room that is in that direction
	map<char*, Room*, charComparer> exits;
	vector<char*> blockedExits; //the exits that are blocked
	map<char*, char*, charComparer> blockReason; //input direction pointer and returns the reason why the exit is blocked
	map<char*, char*, charComparer> blockType; //input direction pointer and returns the block type (eg. CHASM)

	Room* redirect = NULL; //the room thatgoing to this room redirects you to (eg. going to the abandoned town redirects you to the fixed town after doing stuff)

	char* conveyorExit = NULL; //the conveyor belt exit (FORWARD)
	Room* altRoom = NULL; //used by conveyor belts, their direction switches to altRoom when a switch item is used

	bool station; //if it's a train station
	bool gym; //if it's the gym
};
#endif