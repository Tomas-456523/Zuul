//header file for items

#ifndef ITEM
#define ITEM

#include <vector>
using namespace std;

class Room;
class NPC;
struct Attack;
struct Effect;

class Item { //the default item
public:
	Item(const char _name[255], const char _description[255], Room* _room = NULL, bool _takable = true, bool _consumable = false, bool _target = false, bool useonenemy = false);
	virtual ~Item(); //virtual destructor, necessary since there's many item subclasses

	char* getName(); //gets the name of the item
	char* getDescription(); //gets the description of the item
	char* getType(); //gets the type of the item (eg. "key")
	bool getTakable(); //gets if you can take the item
	char* getDenial(); //gets description of why you can't take the item
	int getPrice(); //gets the price of the item if it's for sale
	int getStock(); //gets how many of the item are left if it's for sale
	bool getTargetNeeded(); //gets if the item needs a target (if you have to specify who to use it on)
	bool getConsumable(); //gets if the item gets deleted after use
	bool getForEnemy(); //gets if it's meant to hit the opponents in battle as opposed to for your team
	
	void buy(int& mony, vector<Item*>* inventory); //buys a copy of the item and adds it to the inventory
	void setDenial(const char denial[255]); //sets a description of why you can't take the item
	void setRoom(Room* _room); //sets the item's current room
	void unRoom(); //removes the item from the room

	void setStock(int _stock, int _price, const char buydesc[255] = ""); //makes the item for sale
	void setTakable(bool _takable = true); //manually sets the item to takable, for item types that default to something we don't want

	virtual Item* Duplicate(); //duplicates the item, used by buy and overwritten by all buyable item subclasses in order to prevent splitting when duplicating them
protected:
	char name[255]; //what the item is called
	char description[255]; //describes the item and what it's used for
	char type[255]; //what subclass of item it is

	bool takable; //if you can take the item
	char denyDescription[255] = ""; //description of why you can't take the item
	char buyDescription[255] = ""; //prints this when buying the item
	int price; //how much the item costs to buy
	int stock = 0; //how much of the item you can buy before it's sold out

	bool consumable = false; //if the item gets used up when USEd

	bool targetRequired; //if the item needs a target specified before using it
	bool useOnEnemy; //if the target has to be an enemy in battle

	Room* room = NULL; //the room that the item is in
};

//xp items for adding xp to an npc
class XpItem : public Item {
public:
	XpItem(const char _name[255], const char _description[255], Room* _room, int _xp);

	int getXp(); //get how much xp to add

	virtual Item* Duplicate() override; //gets an Item* pointing to a copy of this subitem
private:
	int xp; //how much xp to add
};

//sp items for adding sp to an npc
class SpItem : public Item {
public:
	SpItem(const char _name[255], const char _description[255], Room* _room, int _sp);

	int getSp(); //gets how much sp to add

	virtual Item* Duplicate() override; //gets an Item* pointing to a copy of this subitem
private:
	int sp; //how much sp to add
};

//hp items for adding hp to an npc
class HpItem : public Item {
public:
	HpItem(const char _name[255], const char _description[255], Room* _room, int _hp);

	int getHp(); //gets how much hp to add

	virtual Item* Duplicate() override; //gets an Item* pointing to a copy of this subitem
private:
	int hp; //how much hp to add
};

//revive items for reviving npcs
class ReviveItem : public Item {
public:
	ReviveItem(const char _name[255], const char _description[255], Room* _room, int _hp);

	int getHp(); //gets how much hp to add

	virtual Item* Duplicate() override; //gets an Item* pointing to a copy of this subitem
private:
	int hp; //how much hp to add
};

//effect items for affecting npcs with an effect
class EffectItem : public Item {
public:
	EffectItem(const char _name[255], const char _description[255], Room* _room, Effect* _effect);

	Effect* getEffect(); //gets the effect to add

	virtual Item* Duplicate() override; //gets an Item* pointing to a copy of this subitem
private:
	Effect* effect; //the effect this item affects with
};

//material items which do nothing except exist
class MaterialItem : public Item {
public:
	MaterialItem(const char _name[255], const char _description[255], Room* _room);
};

//BURGER items which are used for endgame stuff and are only different than materials due to having a Duplicate() and having a different type
class BURGERItem : public Item {
public:
	BURGERItem(const char _name[255], const char _description[255], Room* _room, char* direction);

	char* getDirection(); //returns the elevator direction to go bakc there if the player wants to keep playing after getting the ending

	virtual Item* Duplicate() override; //gets an Item* pointing to a copy of this subitem
private:
	char* elevatordir; //the elevator direction to go bakc there if the player wants to keep playing after getting the ending
};

//education items for learning new attacks
class EducationItem : public Item {
public:
	EducationItem(const char _name[255], const char _description[255], Room* _room, Attack* _attack);

	void setAttack(Attack* attack); //set extra attacks
	vector<Attack*>& getAttacks(); //gets the attacks this item teacher

	virtual Item* Duplicate() override; //gets an Item* pointing to a copy of this subitem
private:
	vector<Attack*> attacks; //the attacks this item teacher
};

//caller items for calling the lobster
class CallerItem : public Item {
public:
	CallerItem(const char _name[255], const char _description[255], Room* _room, NPC* npc);

	NPC* getCalledNPC(); //get the npc it calls
private:
	NPC* npc_called; //the npc you're calling
};

//toll items for paying monies
class TollItem : public Item {
public:
	TollItem();
};

//key items for unlocking locked exits
class KeyItem : public Item {
public:
	KeyItem(const char _name[255], const char _description[255], const char _useText[255], Room* _room, char* _unlockType, bool _consumable = true, Attack* _attack = NULL);

	vector<Room*>& getTargets(); //gets this item's remote unlock rooms if it has one
	Attack* getAttack(); //gets this item's attack in battle
	char* getUnlockType(); //gets the type of blockage this item clears
	char* getUseText(); //gets the text printed by using it

	void setTarget(Room* target); //sets a remote location to unlock

	virtual Item* Duplicate() override; //gets an Item* pointing to a copy of this subitem
private:
	char* unlockType; //the type of blockage this item clears
	Attack* attack; //this item's attack in battle

	vector<Room*> targetRooms; //keys can be targeted keys, so no matter where you are, using them will remotely unblock the exit. So this has those exits
	char useText[255]; //the text printed by using it
};

//movement items for moving through locked exits
class MovementItem : public Item {
public:
	MovementItem(const char _name[255], const char _description[255], const char _useText[255], Room* _room, char* _unlockType, bool _takable = true, Attack* _attack = NULL);

	Attack* getAttack(); //gets the attack it does in battle
	char* getUnlockType(); //gets the block type the thing moves through
	char* getUseText(); //gets the text printed by using it

	virtual Item* Duplicate() override; //gets an Item* pointing to a copy of this subitem
private:
	char* unlockType; //what block type the thing moves through
	Attack* attack; //the attack by using it in battle

	char useText[255]; //the text printed by using it
};

//paver items for paving new exits
class PaverItem : public Item {
public:
	PaverItem(const char _name[255], const char _description[255], const char _useText[255], Room* _room, Room* _usableRoom, char* _direction, Room* _destination);

	Room* getDestination(); //gets the room that it leads to
	char* getDirection(); //gets the direction it leads to
	bool getUsable(Room* _room); //gets if it's usable in the given room
	char* getUseText(); //gets the text printed after using the item
private:
	Room* usableRoom; //the room it's usable in
	char* direction; //the direction that it creates the new exit in
	Room* destination; //the room that the new exit leads to
	char useText[255]; //the text printed by using the item
};

//manhole items, for revealing exits and throwing at enemies
class ManholeItem : public Item {
public:
	ManholeItem(const char _name[255], const char _description[255], Room* _room, Attack* _attack, Room* _destination = NULL, char* _dir = NULL);

	Room* getRoom(); //gets the room that it's leads to
	char* getDirection(); //gets the direction it makes an exit in
	Attack* getAttack(); //gets the throwing attack
private:
	Attack* attack; //you can throw a manhole cover at an enemy, pretty good attack
	Room* destination; //the room that the new exit leads to
	char* direction; //the direction it makes an exit in
};

//info items for printing text
class InfoItem : public Item {
public:
	InfoItem(const char _name[255], const char _description[255], const char* _text, Room* _room);

	char* getText(); //get the text to print
private:
	char text[999]; //the text to print
};

//treasure items, for giving monies and maybe fighting an enemy trap
class TreasureItem : public Item {
public:
	TreasureItem(const char _name[255], const char _description[255], Room* _room, int _mony, NPC* _mimic);

	NPC* getMimic(); //get the trap enemy if there is one
	int getMony(); //get how many monies to give
private:
	NPC* mimic; //the trap enemy it makes you fight
	int mony; //how many monies it gives
};

//conveyor switches, for switching the direction of conveyor belts
class ConveyorSwitch : public Item {
public:
	ConveyorSwitch(const char _name[255], const char _description[255], Room* _room);

	void setConveyor(Room* room); //adds the room to the list of affected rooms
	vector<Room*> getConveyors(); //returns the list of affected rooms
private:
	vector<Room*> conveyors; //the affected rooms
};
#endif