//header file for items

#ifndef ITEM
#define ITEM

//ITEM TYPES WE WILL NEED:
//Stat upgrader
//Key Item
//Weapon
//BURGER
//Info
//no payment; we will use coin system instead

#include "Room.h"
using namespace std;

class Room;

class Item {
public:
	Item(const char _name[255], const char _description[255], Room* _room, bool _takable = true, int _price = 0);
	~Item();

	char* getName();
	char* getDescription();
	bool getTakable();
	char* getDenial();
	char* getBuyDescription();
	int getPrice();
	
	void buy(int& mony, vector<Item*>* inventory);
	void setDenial(const char denial[255]);
	void setBuyDesc(const char buydesc[255]);
	void setRoom(Room* _room);
	void unRoom();
private:
	char name[255];
	char description[255];

	bool takable;
	char denyDescription[255]; //description of why you can't take the item
	char buyDescription[255];
	int price;

	Room* room;
};
#endif