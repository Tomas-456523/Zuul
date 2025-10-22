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

using namespace std;

class Item {
public:
	Item(const char _name[255], const char _description[255], Room* room, bool _takable = true);
	~Item();

	char* getName();
	char* getDescription();
private:
	char name[255];
	char description[255];

	bool takable;
};
#endif