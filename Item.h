//header file for items

#ifndef ITEM
#define ITEM

#include <vector>
#include <tuple>
#include "WorldChange.h"
#include "Conversation.h"
using namespace std;

class Room;
class NPC;
struct Attack;
struct Effect;

class Item { //the default item
public:
	Item(const char* _name, const char* _description, Room* _room = NULL, bool _takable = true, bool _consumable = false, bool _target = false, bool useonenemy = false);
	virtual ~Item(); //virtual destructor, virtual since there's many item subclasses

	Item(const Item& other);

	const char* getName(); //gets the name of the item
	const char* getDescription(); //gets the description of the item
	Room* getRoom(); //get what room the item is in
	const char* getType(); //gets the type of the item (eg. "key")
	bool getTakable(); //gets if you can take the item
	const char* getDenial(); //gets description of why you can't take the item
	int getPrice(); //gets the price of the item if it's for sale
	int getStock(); //gets how many of the item are left if it's for sale
	bool getTargetNeeded(); //gets if the item needs a target (if you have to specify who to use it on)
	bool getConsumable(); //gets if the item gets deleted after use
	bool getForEnemy(); //gets if it's meant to hit the opponents in battle as opposed to for your team
	NPC* getGuard(); //get who is guarding the item
	bool getDropToUse(); //get if we can't use it while in the inventory

	void buy(int& mony, vector<Item*>* inventory); //buys a copy of the item and adds it to the inventory
	void setDenial(const char* denial); //sets a description of why you can't take the item
	void setRoom(Room* _room); //sets the item's current room
	void unRoom(); //removes the item from the room

	void setStock(int _stock, int _price, const Conversation& buydesc); //makes the item for sale
	void setFreebie(const Conversation& pitydesc); //make one free if you can't afford it
	void setTakable(bool _takable = true); //manually sets the item to takable, for item types that default to something we don't want
	void setDropToUse(bool dropreq); //set if we can't use it while in the inventory
	void setGuard(NPC* npc); //set a guard that prevents you from using the item

	virtual Item* Duplicate(); //duplicates the item, used by buy and overwritten by all buyable item subclasses in order to prevent splitting when duplicating them
protected:
	const char* name; //what the item is called
	const char* description; //describes the item and what it's used for
	const char* type; //what subclass of item it is

	bool takable; //if you can take the item
	const char* denyDescription = ""; //description of why you can't take the item
	Conversation buyDescription; //prints this when buying the item
	int price; //how much the item costs to buy
	int stock = 0; //how much of the item you can buy before it's sold out
	bool pity = false; //if they give you one free one anyway if you can't afford it
	Conversation pityDescription;

	bool consumable = false; //if the item gets used up when USEd

	bool targetRequired; //if the item needs a target specified before using it
	bool useOnEnemy; //if the target has to be an enemy in battle

	NPC* guard = NULL; //npc that stops you from using or taking the item until defeated

	Room* room = NULL; //the room that the item is in

	bool droptouse = false; //if you can't use it if it's in your inventory, pretty much just for scissor lift because just imagine the order of events: you go up, you go north (with your scissor lift in your pocket), you come back, and you're floating in the air now?
};

//xp items for adding xp to an npc
class XpItem : public Item {
public:
	XpItem(const char* _name, const char* _description, Room* _room, int _xp);

	int getXp(); //get how much xp to add

	virtual Item* Duplicate() override; //gets an Item* pointing to a copy of this subitem
private:
	int xp; //how much xp to add
};

//sp items for adding sp to an npc
class SpItem : public Item {
public:
	SpItem(const char* _name, const char* _description, Room* _room, int _sp);

	int getSp(); //gets how much sp to add

	virtual Item* Duplicate() override; //gets an Item* pointing to a copy of this subitem
private:
	int sp; //how much sp to add
};

//hp items for adding hp to an npc
class HpItem : public Item {
public:
	HpItem(const char* _name, const char* _description, Room* _room, int _hp);

	int getHp(); //gets how much hp to add

	virtual Item* Duplicate() override; //gets an Item* pointing to a copy of this subitem
private:
	int hp; //how much hp to add
};

//revive items for reviving npcs
class ReviveItem : public Item {
public:
	ReviveItem(const char* _name, const char* _description, Room* _room, int _hp);

	int getHp(); //gets how much hp to add

	virtual Item* Duplicate() override; //gets an Item* pointing to a copy of this subitem
private:
	int hp; //how much hp to add
};

//effect items for affecting npcs with an effect
class EffectItem : public Item {
public:
	EffectItem(const char* _name, const char* _description, Room* _room, Effect* _effect);

	Effect* getEffect(); //gets the effect to add

	virtual Item* Duplicate() override; //gets an Item* pointing to a copy of this subitem
private:
	Effect* effect; //the effect this item affects with
};

//material items which do nothing except exist
class MaterialItem : public Item {
public:
	MaterialItem(const char* _name, const char* _description, Room* _room);
};

//BURGER items which are used for endgame stuff and are only different than materials due to having a Duplicate() and having a different type
class BURGERItem : public Item {
public:
	BURGERItem(const char* _name, const char* _description, Room* _room, const Conversation& _useText, const Conversation& _confirmText, const Conversation& _hintText);

	const Conversation& getUseText() const;
	const Conversation& getConfirmText() const;
	const Conversation& getHintText() const;
	
	virtual Item* Duplicate() override; //gets an Item* pointing to a copy of this subitem
private:
	Conversation useText;
	Conversation confirmText;
	Conversation hintText;
};

//education items for learning new attacks
class EducationItem : public Item {
public:
	EducationItem(const char* _name, const char* _description, Room* _room, Attack* _attack);

	void setAttack(Attack* attack); //set extra attacks
	vector<Attack*>& getAttacks(); //gets the attacks this item teaches

	virtual Item* Duplicate() override; //gets an Item* pointing to a copy of this subitem
private:
	vector<Attack*> attacks; //the attacks this item teacher
};

//weapon items for using new attacks
class WeaponItem : public Item {
public:
	WeaponItem(const char* _name, const char* _description, Room* _room, Attack* _attack);

	Attack* getAttack(); //gets the attack this item teaches

	virtual Item* Duplicate() override; //gets an Item* pointing to a copy of this subitem
private:
	Attack* attack; //the attacks this item teacher
};

//caller items for calling the lobster
class CallerItem : public Item {
public:
	CallerItem(const char* _name, const char* _description, const Conversation& _useText, Room* _room, NPC* npc);

	NPC* getCalledNPC(); //get the npc it calls
	const Conversation& getUseText() const; //gets the text printed by using it

	virtual Item* Duplicate() override; //gets an Item* pointing to a copy of this subitem
private:
	NPC* npc_called; //the npc you're calling
	Conversation useText; //the text printed by using it
};

//toll items for paying monies
class TollItem : public Item {
public:
	TollItem();
};

//key items for unlocking locked exits
class KeyItem : public Item {
public:
	KeyItem(const char* _name, const char* _description, const Conversation& _useText, Room* _room, const char* _unlockType, bool _consumable = true, Attack* _attack = NULL);

	vector<Room*>& getTargets(); //gets this item's remote unlock rooms if it has one
	Attack* getAttack(); //gets this item's attack in battle
	const char* getUnlockType(); //gets the type of blockage this item clears
	const Conversation& getUseText() const; //gets the text printed by using it

	void setTarget(Room* target); //sets a remote location to unlock

	virtual Item* Duplicate() override; //gets an Item* pointing to a copy of this subitem
private:
	const char* unlockType; //the type of blockage this item clears
	Attack* attack; //this item's attack in battle

	vector<Room*> targetRooms; //keys can be targeted keys, so no matter where you are, using them will remotely unblock the exit. So this has those exits
	Conversation useText; //the text printed by using it
};

//hose items that are keyitems that have some bonus inconvenient blocking functionalities
class HoseItem : public KeyItem {
public:
	HoseItem(const char* _name, const char* _description, const Conversation& _useText, Room* _room, const char* _unlockType, bool _consumable = true, Attack* _attack = NULL);

	void addBlocker(Room* room, const char* direction, const char* reason, const char* floorreason); //block the room in that direction
	void setStationBlock(const char* message);
	void addDropChange(Room* room, WorldChange& changes); //make world changes on take and on drop
	void addTakeChange(Room* room, WorldChange& changes);

	const char* getBlocked(Room* currentroom, const char* direction); //check if this hose is blocking going in this direction from this room, return why if so
	const char* getStationBlock();
	void doDropChanges(Room* currentroom); //do the changes when needed
	void doTakeChanges(Room* currentroom);
private:
	vector<tuple<Room*, const char*, const char*, const char*>> blockers; //you cannot go in this direction in these rooms for that reason if you have the item
	vector<pair<Room*, WorldChange>> dropchanges; //if the hose is dropped in this room make these changes
	vector<pair<Room*, WorldChange>> takechanges; //if the hose is taken in this room make these changes
	const char* stationblock = NULL; //text after getting blocked from fast travel
};

//movement items for moving through locked exits
class MovementItem : public Item {
public:
	MovementItem(const char* _name, const char* _description, const Conversation& _useText, Room* _room, const char* _unlockType, bool _takable = true, Attack* _attack = NULL);

	Attack* getAttack(); //gets the attack it does in battle
	const char* getUnlockType(); //gets the block type the thing moves through
	const Conversation& getUseText() const; //gets the text printed by using it

	virtual Item* Duplicate() override; //gets an Item* pointing to a copy of this subitem
private:
	const char* unlockType; //what block type the thing moves through
	Attack* attack; //the attack by using it in battle

	Conversation useText; //the text printed by using it
};

//paver items for paving new exits
class PaverItem : public Item {
public:
	PaverItem(const char* _name, const char* _description, const Conversation& _useText, Room* _room, Room* _usableRoom, const char* _direction, Room* _destination);

	Room* getDestination(); //gets the room that it leads to
	const char* getDirection(); //gets the direction it leads to
	bool getUsable(Room* _room); //gets if it's usable in the given room
	const Conversation& getUseText() const; //gets the text printed after using the item
private:
	Room* usableRoom; //the room it's usable in
	const char* direction; //the direction that it creates the new exit in
	Room* destination; //the room that the new exit leads to
	Conversation useText; //the text printed by using the item
};

//manhole items, for revealing exits and throwing at enemies
class ManholeItem : public Item {
public:
	ManholeItem(const char* _name, const char* _description, Room* _room, Attack* _attack, Room* _destination = NULL, const char* _dir = NULL);

	Room* getRoom(); //gets the room that it's leads to
	const char* getDirection(); //gets the direction it makes an exit in
	Attack* getAttack(); //gets the throwing attack
private:
	Attack* attack; //you can throw a manhole cover at an enemy, pretty good attack
	Room* destination; //the room that the new exit leads to
	const char* direction; //the direction it makes an exit in
};

//info items for printing text
class InfoItem : public Item {
public:
	InfoItem(const char* _name, const char* _description, const char* _text, Room* _room);

	const char* getText(); //get the text to print
private:
	const char* text; //the text to print
};

//treasure items, for giving monies and maybe fighting an enemy trap
class TreasureItem : public Item {
public:
	TreasureItem(const char* _name, const char* _description, Room* _room, int _mony, initializer_list<Item*> _items = {}, NPC* _mimic = NULL);

	NPC* getMimic(); //get the trap enemy if there is one
	int getMony(); //get how many monies to give
	vector<Item*>& getItems();
private:
	NPC* mimic; //the trap enemy it makes you fight
	int mony; //how many monies it gives
	vector<Item*> items; //the items stored in the thing
};

//conveyor switches, for switching the direction of conveyor belts
class ConveyorSwitch : public Item {
public:
	ConveyorSwitch(const char* _name, const char* _description, Room* _room);

	void setConveyor(Room* room); //adds the room to the list of affected rooms
	vector<Room*> getConveyors(); //returns the list of affected rooms
private:
	vector<Room*> conveyors; //the affected rooms
};

//world change items, for changing the world
class WorldChangeItem : public Item {
public:
	WorldChangeItem(const char* _name, const char* _description, Room* _room, const Conversation& _useText, bool taketouse = false);

	WorldChange& getChanges(); //returns the world changes, as a reference so it's easier to edit
	const Conversation& getUseText() const; //gets the text printed by using it
	bool getTakeToUse(); //get if we need to take it instead of use it to make it do the thing
	bool getUsable(Room* _room); //gets if it's usable in the given room

	void setUsableRooms(initializer_list<Room*> rooms); //make item need to be usable in these rooms only
private:
	WorldChange changes; //the changes to do
	Conversation useText;
	bool takeToUse; //if we need to take it instead of use it to make it do the thing
	vector<Room*> usableRooms; //rooms where this is usable (if it's empty it's usable everywhere)
};

//blender items, for taking ingredients and combining them into one thing
class BlenderItem : public Item {
public:
	BlenderItem(const char* _name, const char* _description, Room* _room, const Conversation& _useText, initializer_list<const char*> _ingredients, Item* _product);

	WorldChange& getChanges(); //returns the world changes, as a reference so it's easier to edit
	const Conversation& getUseText() const; //gets the text printed by using it
	const vector<const char*>& getIngredients(); //get the ingredients needed to make the thing
	Item* getProduct(); //get the product of the blender item
private:
	WorldChange changes; //the changes that happen when this item is used
	Conversation useText;
	vector<const char*> ingredients; //what is used to make the product
	Item* product; //what the blender item makes
};
#endif