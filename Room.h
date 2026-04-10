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
	Room(const char* _description);
	~Room();

	const char* getDescription(); //get the description of the room
	vector<Item*>& getItems(); //gets the room's items
	vector<Item*>& getStock(); //gets the items for sale in this room
	vector<NPC*>& getNpcs(bool alt = false); //gets the npcs in this room
	vector<const char*>& getBlocks(); //gets the blocked exits in this room
	Room* getExit(const char* direction); //get the room in the given direction from this room
	Room* getRedirect(); //get if the room redirects to another one
	bool getStation(); //gets if this room is a station
	bool getGym(); //gets if this room is a gym
	bool getTempleEntrance(); //gets if this room is a temple entrance
	bool getBlocked(const char* direction); //gets if the given exit is blocked
	const char* getBlockReason(const char* direction); //gets why the exit is blocked
	Item* popBackup(); //take the backup from the room to check if we should put it there

	void printExits(); //functions for printing the stuff in the room
	void printItems();
	void printStock();
	void printNPCs();
	void printWelcome();
	void printBlocks();
	void printBlock(const char* direction);

	void setItem(Item* item); //puts an item in the room
	void shareItems(Room* room); //share item vectors with this room
	void setNPC(NPC* npc, bool alt = false); //puts an npc in the room
	void removeNPC(NPC* npc, bool alt = false); //removes the thing from the room
	void removeItem(Item* item);
	void setExit(const char* direction, Room* room, const char* blocktype = NULL, const char* _reason = NULL); //sets an exit to another room
	void setRedirect(Room* room); //sets a redirect to another room
	void setStation(bool stat = true); //sets if this room is a station
	void setGym(bool _gym = true); //sets if this room is a gym
	void setConveyor(Room* altroom, const char* conveyorexit); //sets if this room is a conveyor + references the FORWARD exit in conveyor rooms
	void setTempleEntrance(const char* exit, Room* temple, Conversation opentext); //makes this room a temple entrance, so we can ASK NICELY to open it
	void setDescription(const char* _description); //reset the description, used by items that change things
	void setWelcome(Conversation text); //set welcome text for the area
	void setStock(Item* item, int stock, int price, const char* buydesc = ""); //adds an item for sale
	void removeStock(Item* item); //removes an item from sale
	void setBackup(Item* item); //sets a backup item for the room
	void openTemple(); //set the temple entrance if this is a temple entrance

	void switchConveyor(); //switches the direction of the conveyor

	void blockExit(const char* direction, const char* blocktype, const char* reason); //blocks an exit
	void unblockExit(const char* direction); //unblocks the exit
	vector<const char*> unblockAll(const char* type); //unblocks all the blocked exits of the given block type

	void scaleNPCs(int level); //update all npc levels at the gym capped at the given cap

	void undefeatEnemies(); //makes the enemies in the room not defeated anymore
private:
	const char* description; //describes the player's surroundings in this room (eg. (You are ) "in an amazingly incredible room full of stuff.")
	
	Conversation welcomeText; //message that is played when the player first gets here

	bool welcome = false; //if this room has a welcome message for an area

	vector<Item*> items; //items in this room
	vector<Item*>* sharedItems = NULL; //we might actually share items across rooms, such as in multiple rooms representing the same elevator
	vector<Item*> stock; //items for sale in this room
	vector<NPC*> npcs; //npcs in this room
	vector<NPC*> altNpcs; //npcs in an adjacent room that are blocking an exit in this room
	//input direction pointer and returns the room that is in that direction
	map<const char*, Room*, charComparer> exits;
	vector<const char*> blockedExits; //the exits that are blocked
	map<const char*, const char*, charComparer> blockReason; //input direction pointer and returns the reason why the exit is blocked
	map<const char*, const char*, charComparer> blockType; //input direction pointer and returns the block type (eg. CHASM)

	Room* redirect = NULL; //the room thatgoing to this room redirects you to (eg. going to the abandoned town redirects you to the fixed town after doing stuff)

	const char* conveyorExit = NULL; //the conveyor belt exit (FORWARD)
	Room* altRoom = NULL; //used by conveyor belts, their direction switches to altRoom when a switch item is used

	bool station; //if it's a train station
	bool gym; //if it's the gym
	bool faraway; //if you can't dismiss teammates here

	bool templeentrance = false; //if it's a temple entrance and you can ASK NICELY to set the entrance
	pair<const char*, Room*> templesettings = {NULL, NULL};
	Conversation templeopenconvo; //text that is printed when the temple is opened

	pair<size_t> enterchangecondition = {NEVER, NEVER}; //stuff that happens if you enter this room and the first condition is true but not the second one
	WorldChange enterchange;
	Conversation enterchangetext; //prints this when it happens, easier to manage than welcome text because I didn't have a way to remove welcomes

	Item* backup; //item that is placed here only if the player doesn't have one already
};
#endif