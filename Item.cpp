//implementation file for items

#include <cstring>
#include <iostream>
#include "Item.h"
#include "Room.h"
using namespace std;

//constructs the basic item stuff
Item::Item(const char _name[255], const char _description[255], Room* _room, bool _takable, bool _consumable, bool _target, bool useonenemy) {
	strcpy(name, _name);
	strcpy(description, _description);
	strcpy(type, "default");
	takable = _takable;
	consumable = _consumable;
	targetRequired = _target;
	useOnEnemy = useonenemy;
	if (_room != NULL) { //set the room if we gave one
		setRoom(_room);
	}
	//itemsH.push_back(this); //store a pointer to this item in the items vector
}
//returns the name of the item
char* Item::getName() {
	return name;
}
//returns the description of the item
char* Item::getDescription() {
	return description;
}
//returns the type of the item (eg. "movement" or "key")
char* Item::getType() {
	return type;
}
//returns if the item is takable
bool Item::getTakable() {
	return takable;
}
//returns description of why you can't take the item
char* Item::getDenial() {
	return denyDescription;
}
//returns the price of the item
int Item::getPrice() {
	return price;
}
//returns how many of this item are left in stock (in rooms with buyable items)
int Item::getStock() {
	return stock;
}
//returns if this item needs a target (if you need to clarify who to USE it on)
bool Item::getTargetNeeded() {
	return targetRequired;
}
//returns if the item gets used up after USE-ing it
bool Item::getConsumable() {
	return consumable;
}
//returns if the item is meant to be USEd on the enemy team during battle (like a weapon)
bool Item::getForEnemy() {
	return useOnEnemy;
}
//sets the description of why you can't take the item
void Item::setDenial(const char denial[255]) {
	strcpy(denyDescription, denial);
}
//changes the item's current room
void Item::setRoom(Room* _room) {
	if (room != NULL) { //if the item was in a room, we first make sure the room knows the item is gone
		room->removeItem(this);
	}
	room = _room; //sets the room
	room->setItem(this); //tells the room the item is there now
}
//removes the item from its room (like when TAKE-ing it) and nullifies the room pointer
void Item::unRoom() {
	if (room == NULL) {
		return;
	}
	room->removeItem(this);
	room = NULL;
}
//for BUYing an item and putting it in the inventory
void Item::buy(int& mony, vector<Item*>* inventory) {
	if (mony < price) { //error text if you can't afford it
		cout << "\nYou can't afford the " << name << "!";
		return;
	}
	mony -= price; //subtracts the cost
	stock--; //decrements the amount left in stock of this item
	inventory->push_back(Duplicate()); //copies the item and adds it to the inventory
	cout << "\nYou bought the " << name << "! You now have " << mony << " mon"; //success text + how many monies you now have
	if (mony == 1) { //proper grammar yay
		cout << "y.";
	} else {
		cout << "ies.";
	} //if the item says something after being bought (like the shopkeeper saying "thank you for your purchase!" or something)
	if (strcmp(buyDescription,"")) {
		cout << "\n" << buyDescription; //prints the buying description
	}
}
//makes the item for sale
void Item::setStock(int _stock, int _price, const char buydesc[255]) {
	stock = _stock; //set how much of the item are for sale
	price = _price; //set the price of the item
	strcpy(buyDescription, buydesc); //set the buy description
}
//virtual function for duplicating the item
Item* Item::Duplicate() {
	return new Item(*this);
}
//virtual destructor function, does literally nothing but I need it so I can call delete
Item::~Item() {

}

//xp items for adding xp to an npc
XpItem::XpItem(const char _name[255], const char _description[255], Room* _room, int _xp) : Item(_name, _description, _room, true, true, true) {
	xp = _xp;
	strcpy(type, "xp"); //sets the type
}
Item* XpItem::Duplicate() { //returns a new xp item as an Item*
	return new XpItem(*this);
}
//returns how much xp it gives
int XpItem::getXp() {
	return xp;
}

//sp items for replenishing sp for an npc
SpItem::SpItem(const char _name[255], const char _description[255], Room* _room, int _sp) : Item(_name, _description, _room, true, true, true) {
	sp = _sp;
	strcpy(type, "sp"); //sets the type
}
Item* SpItem::Duplicate() { //returns a new sp item as an Item*
	return new SpItem(*this);
}
//returns how much sp it gives
int SpItem::getSp() {
	return sp;
}

//hp items for healing npcs
HpItem::HpItem(const char _name[255], const char _description[255], Room* _room, int _hp) : Item(_name, _description, _room, true, true, true) {
	hp = _hp;
	strcpy(type, "hp"); //sets the type
}
Item* HpItem::Duplicate() { //returns a new hp item as an Item*
	return new HpItem(*this);
}
//returns how much hp it gives
int HpItem::getHp() {
	return hp;
}

//revive items for healing and reviving teammates
ReviveItem::ReviveItem(const char _name[255], const char _description[255], Room* _room, int _hp) : Item(_name, _description, _room, true, true, true) {
	hp = _hp;
	strcpy(type, "hp"); //sets the type
}
Item* ReviveItem::Duplicate() { //returns a new revive item as an Item*
	return new ReviveItem(*this);
}
//returns how much hp it restores
int ReviveItem::getHp() {
	return hp;
}

//effect items for adding effects to npcs
EffectItem::EffectItem(const char _name[255], const char _description[255], Room* _room, Effect* _effect) : Item(_name, _description, _room, true, true) {
	effect = _effect;
	strcpy(type, "effect"); //sets the type
}
//returns the effect it gives
Effect* EffectItem::getEffect() {
	return effect;
}
Item* EffectItem::Duplicate() { //returns a new effect item as an Item*
	return new EffectItem(*this);
}

//material items, which basically do nothing
MaterialItem::MaterialItem(const char _name[255], const char _description[255], Room* _room) : Item(_name, _description, _room, true, true) {
	strcpy(type, "material"); //sets the type
}

//BURGER items, for the bad/lame ending
BURGERItem::BURGERItem(const char _name[255], const char _description[255], Room* _room) : Item(_name, _description, _room, true, true) {
	strcpy(type, "BURGER"); //sets the type
}
Item* BURGERItem::Duplicate() { //returns a new BURGER item as an Item*
	return new BURGERItem(*this);
}

//education items, for learning new moves
EducationItem::EducationItem(const char _name[255], const char _description[255], Room* _room, Attack* _attack) : Item(_name, _description, _room, true, true) {
	attacks.push_back(_attack);
	strcpy(type, "education"); //sets the type
}
//adds an attack to the item for it to teach
void EducationItem::setAttack(Attack* attack) {
	attacks.push_back(attack);
}
//returns a vector of all the attacks this item teaches
vector<Attack*>& EducationItem::getAttacks() {
	return attacks;
}
Item* EducationItem::Duplicate() { //returns a new education item as an Item*
	return new EducationItem(*this);
}

//caller items, for summoning npcs to the current room
CallerItem::CallerItem(const char _name[255], const char _description[255], Room* _room, NPC* npc) : Item(_name, _description, _room, true) {
	npc_called = npc;
	strcpy(type, "caller"); //sets the type
}
//returns the npc this caller calls
NPC* CallerItem::getCalledNPC() {
	return npc_called;
}

//key items, for unlocking and unblocking blocked exits
KeyItem::KeyItem(const char _name[255], const char _description[255], const char _useText[255], Room* _room, char* _unlockType, bool _consumable, Attack* _attack) : Item(_name, _description, _room, true, _consumable, true, true) {
	unlockType = _unlockType;
	strcpy(useText, _useText);
	strcpy(type, "key"); //sets the type
	attack = _attack; //sets the attack if it has one
}
//returns the key's targeted room
Room* KeyItem::getTarget() {
	return targetRoom;
}
//returns the key's attack
Attack* KeyItem::getAttack() {
	return attack;
}
//returns what kind of exit this key unlocks
char* KeyItem::getUnlockType() {
	return unlockType;
}
//returns the text for using the key
char* KeyItem::getUseText() {
	return useText;
}
//sets a remote location for the key to unblock
void KeyItem::setTarget(Room* target) {
	targetRoom = target;
}
Item* KeyItem::Duplicate() { //returns a new key item as an Item*
	return new KeyItem(*this);
}

//movement items, for moving through blocked exits
MovementItem::MovementItem(const char _name[255], const char _description[255], const char _useText[255], Room* _room, char* _unlockType, bool _takable, Attack* _attack) : Item(_name, _description, _room, _takable, false, true, true) {
	unlockType = _unlockType;
	strcpy(useText, _useText);
	strcpy(type, "movement"); //sets the type
	attack = _attack; //sets the attack if it has one
}
//returns the mover's attack
Attack* MovementItem::getAttack() {
	return attack;
}
//returns what kind of exit this mover goes through
char* MovementItem::getUnlockType() {
	return unlockType;
}
//gets the text printed after using this item
char* MovementItem::getUseText() {
	return useText;
}
Item* MovementItem::Duplicate() { //returns a new movement item as an Item*
	return new MovementItem(*this);
}

//paver exits, for paving new exits in a room
PaverItem::PaverItem(const char _name[255], const char _description[255], const char _useText[255], Room* _room, Room* _usableRoom, char* _direction, Room* _destination) : Item(_name, _description, _room, false, true, true) {
	destination = _destination;
	direction = _direction;
	usableRoom = _usableRoom;
	strcpy(type, "paver"); //sets the type
	strcpy(useText, _useText);
}
//gets where the new exit will go
Room* PaverItem::getDestination() {
	return destination;
}
//gets the direction the paver item paves in
char* PaverItem::getDirection() {
	return direction;
}
//gets if it is usable in the given room
bool PaverItem::getUsable(Room* _room) {
	return room == usableRoom;
}
//gets the text that is printed when using the paver
char* PaverItem::getUseText() {
	return useText;
}

//manhole items, pretty much just material items but you can throw them at enemies, and then two manhole covers in the game reveal an exit below after TAKE-ing it
ManholeItem::ManholeItem(const char _name[255], const char _description[255], Room* _room, Attack* _attack, Room* _destination, char* _dir) : Item(_name, _description, _room, true, true, true, true) {
	destination = _destination; //sets the destination and direction if applicable
	direction = _dir;
	strcpy(type, "manhole"); //sets the type
	attack = _attack; //sets the attack
}
//gets the room that the item will reveal an exit to after taking it
Room* ManholeItem::getRoom() {
	Room* dest = destination; //makes a new reference so we can nullify it AND return it
	destination = NULL; //nullifies the destination so we don't place it everywhere we drop the item
	return dest;
}
//gets the direction the item reveals an exit in (only DOWN is ever used, but I still need to reference it in the item object for the exit setting to work properly)
char* ManholeItem::getDirection() {
	return direction;
}
//gets the item's attack
Attack* ManholeItem::getAttack() {
	return attack;
}

//info items, for printing information
InfoItem::InfoItem(const char _name[255], const char _description[255], const char _text[255], Room* _room) : Item(_name, _description, _room, false, false) {
	strcpy(text, _text); //sets the text that it says
	strcpy(type, "info"); //sets the type
}
//gets the text that is printed by using the item
char* InfoItem::getText() {
	return text;
}

//treasure items, for getting monies and also maybe fighting an enemy if it's trapped
TreasureItem::TreasureItem(const char _name[255], const char _description[255], Room* _room, int _mony, NPC* _mimic) : Item(_name, _description, _room, false, true) {
	strcpy(type, "treasure"); //sets the type
	mimic = _mimic; //sets the trap enemy if there is one
	mony = _mony;
}
//gets the trap enemy if there is one
NPC* TreasureItem::getMimic() {
	return mimic;
}
//gets how many monies you get by USE-ing the treasure item
int TreasureItem::getMony() {
	return mony;
}

//conveyor switches, for switching the direction of conveyor belt rooms in one factory
ConveyorSwitch::ConveyorSwitch(const char _name[255], const char _description[255], Room* _room) : Item(_name, _description, _room, false) {
	strcpy(type, "switch"); //sets the type
}
//adds the conveyor to the switch's list of conveyors
void ConveyorSwitch::setConveyor(Room* room) {
	conveyors.push_back(room);
}
//returns a list of all rooms this item affects
vector<Room*> ConveyorSwitch::getConveyors() {
	return conveyors;
}