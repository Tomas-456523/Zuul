//header file for items

#ifndef ITEM
#define ITEM

//ITEM TYPES WE WILL NEED:
//Movement / Weapon
//FISHING ROD OH NO
//npc item (becomes npc, cactus)
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
	bool getTargetNeeded();
	
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
//MARK:xp
class XpItem : public Item {
public:
	XpItem();
	virtual ~XpItem();

	int getXp();
private:
	int xp;
};
//MARK:sp
class SpItem : public Item {
public:
	SpItem();
	virtual ~SpItem();

	int getSp();
private:
	int sp;
};
//MARK: hp
class HpItem : public Item {
public:
	HpItem(const char _name[255], const char _description[255], Room* _room, int _hp);
	virtual ~HpItem();

	int getHp();

	virtual Item* Duplicate() override;
private:
	int hp;
};
//MARK: revuve
//this should be very rare and non-renewable
class ReviveItem : public Item {
public:
	ReviveItem();
	virtual ~ReviveItem();
};
//MARK: mony
class MonyItem : public Item {
public:
	MonyItem();
	virtual ~MonyItem();

	int getMony();
private:
	int mony;
};
//MARK: effect
class EffectItem : public Item {
public:
	EffectItem();
	virtual ~EffectItem();

	Effect* getEffect();
private:
	Effect* effect;
};
//MARK: material
class MaterialItem : public Item {
public:
	MaterialItem(const char _name[255], const char _description[255], Room* _room);
	virtual ~MaterialItem();
};
//MARK: BURGER
class BURGERItem : public Item {
public:
	BURGERItem();
	virtual ~BURGERItem();

	virtual Item* Duplicate() override;
};
//MARK: education
class EducationItem : public Item {
public:
	EducationItem(const char _name[255], const char _description[255], Room* _room, Attack* _attack);
	virtual ~EducationItem();

	void setAttack(Attack* attack); //set extra attacks
	vector<Attack*> getAttacks();

	virtual Item* Duplicate() override;
private:
	vector<Attack*> attacks;
};

//MARK: caller
//you can buy caller from desert town
//you can use caller in any station (rooms should have station bool)
//caller summons lobster whether tamed or not
//toll items set station bool to true
//toll items unlock locked exits and immediately block them again lol
//tent station is unlocked always
class CallerItem : public Item {
public:
	CallerItem(NPC* npc);
	virtual ~CallerItem();

	void callNPC(); //call the npc to the caller's current room
private:
	NPC* npc_called; //the guy you're calling
};
//MARK: toll
class TollItem : public Item {
public:
	TollItem();
	virtual ~TollItem();
};
//MARK: key
class KeyItem : public Item {
public:
	KeyItem(const char _name[255], const char _description[255], const char _useText[255], Room* _room, char* _unlockType, Attack* _attack = NULL);
	virtual ~KeyItem();

	Room* getTarget();
	Attack* getAttack();
	char* getUnlockType();
	char* getUseText();

	void setTarget(Room* target);

	virtual Item* Duplicate() override;
private:
	char* unlockType;
	Attack* attack;

	Room* targetRoom = NULL; //keys can be targeted keys, so no matter where you are, using them will remotely unblock the exit
	char useText[255];
};

//paves a new path MARK: paver
class PaverItem : public Item {
public:
	PaverItem(const char _name[255], const char _description[255], Room* _room, Room* _usableRoom, char* _direction, Room* _destination);
	virtual ~PaverItem();

	Room* getRoom(); //gets the room that it's usable in
	void setExit(); //sets the exit in the designated room
private:
	Room* usableRoom; //the room it's usable in
	char* direction; //the direction that it creates the new exit in
	Room* destination; //the room that the new exit leads to
};
//MARK: info
class InfoItem : public Item {
public:
	InfoItem(const char _name[255], const char _description[255], const char _text[255], Room* _room);
	virtual ~InfoItem();

	char* getText();
private:
	char text[255];
};
#endif