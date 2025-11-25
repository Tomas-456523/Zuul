//header file for items

#ifndef ITEM
#define ITEM

//ITEM TYPES WE WILL NEED:
//Key / Weapon
//Movement / Weapon
//Info
//FISHING ROD OH NO
//treasure chest (can be trapped or not)

#include <vector>
using namespace std;

class Room;
class NPC;
struct Attack;
struct Effect;

class Item {
public:
	Item(const char _name[255], const char _description[255], Room* _room = NULL, bool _takable = true, bool _target = false);
	virtual ~Item();

	char* getName();
	char* getDescription();
	char* getType();
	bool getTakable();
	char* getDenial();
	char* getBuyDescription();
	int getPrice();
	int getStock();
	
	void buy(int& mony, vector<Item*>* inventory);
	void setDenial(const char denial[255]);
	//void setBuyDesc(const char buydesc[255]);
	void setRoom(Room* _room);
	void unRoom();

	void setStock(int _stock, int _price, const char buydesc[255] = "");
	//void unStock();

	virtual Item* Duplicate();
protected:
	char name[255];
	char description[255];
	char type[255];

	bool takable;
	char denyDescription[255] = ""; //description of why you can't take the item
	char buyDescription[255] = "";
	int price;
	int stock = 0;

	bool consumable = false;

	bool targetRequired;

	Room* room;
};

class XpItem : Item {
public:
	XpItem();
	virtual ~XpItem();

	int getXp();
private:
	int xp;
};

class SpItem : Item {
public:
	SpItem();
	virtual ~SpItem();

	int getSp();
private:
	int sp;
};

class HpItem : Item {
public:
	HpItem(const char _name[255], const char _description[255], Room* _room, int _hp);
	virtual ~HpItem();

	int getHp();

	virtual Item* Duplicate() override;
private:
	int hp;
};

//this should be very rare and non-renewable
class ReviveItem : Item {
public:
	ReviveItem();
	virtual ~ReviveItem();
};

class MonyItem : Item {
public:
	MonyItem();
	virtual ~MonyItem();

	int getMony();
private:
	int mony;
};

class EffectItem : Item {
public:
	EffectItem();
	virtual ~EffectItem();

	Effect* getEffect();
private:
	Effect* effect;
};

class MaterialItem : Item {
public:
	MaterialItem(const char _name[255], const char _description[255], Room* _room);
	virtual ~MaterialItem();
};

class BURGERItem : Item {
public:
	BURGERItem();
	virtual ~BURGERItem();
};

class EducationItem : Item {
public:
	EducationItem();
	virtual ~EducationItem();

	void setAttack(Attack* attack);
	vector<Attack*> getAttacks();
private:
	vector<Attack*> attacks;
};

//you can buy caller from desert town
//you can use caller in any station (rooms should have station bool)
//caller summons lobster whether tamed or not
//toll items set station bool to true
//toll items unlock locked exits and immediately block them again lol
//tent station is unlocked always
class CallerItem : Item {
public:
	CallerItem(NPC* npc);
	virtual ~CallerItem();

	void callNPC(); //call the npc to the caller's current room
private:
	NPC* npc_called; //the guy you're calling
};

class TollItem : Item {
public:
	TollItem();
	virtual ~TollItem();
};

class KeyItem : Item {
public:
	KeyItem(char* _unlockType, Attack* _attack = NULL);
	virtual ~KeyItem();

	Room* getTarget();
private:
	char* unlockType;
	Attack* attack;

	Room* targetRoom = NULL; //keys can be targeted keys, so no matter where you are, using them will remotely unblock the exit
};

//paves a new path
class PaverItem : Item {
public:
	PaverItem(Room* _usableRoom, char* _direction, Room* _destination);
	virtual ~PaverItem();

	Room* getRoom(); //gets the room that it's usable in
	void setExit(); //sets the exit in the designated room
private:
	Room* usableRoom; //the room it's usable in
	char* direction; //the direction that it creates the new exit in
	Room* destination; //the room that the new exit leads to
};
#endif