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
	Item(const char _name[255], const char _description[255], Room* room, bool _takable = true);
	~Item();

	char* getName();
	char* getDescription();
	bool getTakable();
	char* getDenial();
	char* buy(int* mony);

	void setDenial(char denial[255]);
	void setBuyDesc(char buydesc[255]);
private:
	char name[255];
	char description[255];

	bool takable;
	char denyDescription[255]; //description of why you can't take the item
	char buyDescription[255];
	int price;
};
#endif