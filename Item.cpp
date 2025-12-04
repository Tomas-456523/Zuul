#include <cstring>
#include <iostream>
#include "Item.h"
#include "Room.h"
using namespace std;

Item::Item(const char _name[255], const char _description[255], Room* _room, bool _takable, bool _consumable, bool _target) {
	strcpy(name, _name);
	strcpy(description, _description);
	strcpy(type, "default");
	takable = _takable;
	consumable = _consumable;
	targetRequired = _target;
	//price = _price;
	if (_room != NULL) {
		setRoom(_room);
	}
}
char* Item::getName() {
	return name;
}
char* Item::getDescription() {
	return description;
}
char* Item::getType() {
	return type;
}
bool Item::getTakable() {
	return takable;
}
char* Item::getDenial() {
	return denyDescription;
}
int Item::getPrice() {
	return price;
}
int Item::getStock() {
	return stock;
}
bool Item::getTargetNeeded() {
	return targetRequired;
}
bool Item::getConsumable() {
	return consumable;
}
void Item::setDenial(const char denial[255]) {
	strcpy(denyDescription, denial);
}
/*void Item::setBuyDesc(const char buydesc[255]) {
	strcpy(buyDescription, buydesc);
}*/
void Item::setRoom(Room* _room) {
	if (room != NULL) {
		room->removeItem(this);
	}
	room = _room;
	room->setItem(this);
}
void Item::unRoom() {
	room->removeItem(this);
	room = NULL;
}
void Item::buy(int& mony, vector<Item*>* inventory) {
	if (mony < price) {
		cout << "\nYou can't afford the " << name << "!";
		return;
	}
	mony -= price;
	stock--;
	inventory->push_back(Duplicate());
	cout << "\nYou bought the " << name << "! You now have " << mony << " mon";
	if (mony == 1) {
		cout << "y.";
	} else {
		cout << "ies.";
	}
	if (strcmp(buyDescription,"")) {
		cout << "\n" << buyDescription;
	}
}
void Item::setStock(int _stock, int _price, const char buydesc[255]) {
	stock = _stock;
	price = _price;
	strcpy(buyDescription, buydesc);
}
/*void Item::unStock() {
	stock = 0;
}*/
Item* Item::Duplicate() {
	return new Item(*this);
}
Item::~Item() {

}
//MARK: xp
XpItem::XpItem(const char _name[255], const char _description[255], Room* _room, int _xp) : Item(_name, _description, _room, true, true, true) {
	xp = _xp;
	strcpy(type, "xp");
}
Item* XpItem::Duplicate() {
	return new XpItem(*this);
}
int XpItem::getXp() {
	return xp;
}
XpItem::~XpItem() {

}
//MARK: sp
SpItem::SpItem(const char _name[255], const char _description[255], Room* _room, int _sp) : Item(_name, _description, _room, true, true, true) {
	sp = _sp;
	strcpy(type, "sp");
}
Item* SpItem::Duplicate() {
	return new SpItem(*this);
}
int SpItem::getSp() {
	return sp;
}
SpItem::~SpItem() {

}
//MARK: hp
HpItem::HpItem(const char _name[255], const char _description[255], Room* _room, int _hp) : Item(_name, _description, _room, true, true, true) {
	hp = _hp;
	strcpy(type, "hp");
}
Item* HpItem::Duplicate() {
	return new HpItem(*this);
}
int HpItem::getHp() {
	return hp;
}
HpItem::~HpItem() {

}
//MARK: material
MaterialItem::MaterialItem(const char _name[255], const char _description[255], Room* _room) : Item(_name, _description, _room, true, true) {
	strcpy(type, "material");
}
MaterialItem::~MaterialItem() {

}
//MARK: education
EducationItem::EducationItem(const char _name[255], const char _description[255], Room* _room, Attack* _attack) : Item(_name, _description, _room, true, true) {
	attacks.push_back(_attack);
	strcpy(type, "education");
}
void EducationItem::setAttack(Attack* attack) {
	attacks.push_back(attack);
}
vector<Attack*> EducationItem::getAttacks() {
	return attacks;
}
Item* EducationItem::Duplicate() {
	return new EducationItem(*this);
}
EducationItem::~EducationItem() {

}
//MARK: caller
CallerItem::CallerItem(const char _name[255], const char _description[255], Room* _room, NPC* npc) : Item(_name, _description, _room, true) {
	npc_called = npc;
	strcpy(type, "caller");
}
CallerItem::~CallerItem() {

}
NPC* CallerItem::getCalledNPC() {
	return npc_called;
}
//MARK: key
KeyItem::KeyItem(const char _name[255], const char _description[255], const char _useText[255], Room* _room, char* _unlockType, bool _consumable, Attack* _attack) : Item(_name, _description, _room, true, _consumable, true) {
	unlockType = _unlockType;
	strcpy(useText, _useText);
	strcpy(type, "key");
	attack = _attack;
}
Room* KeyItem::getTarget() {
	return targetRoom;
}
Attack* KeyItem::getAttack() {
	return attack;
}
char* KeyItem::getUnlockType() {
	return unlockType;
}
char* KeyItem::getUseText() {
	return useText;
}
void KeyItem::setTarget(Room* target) {
	targetRoom = target;
}
Item* KeyItem::Duplicate() {
	return new KeyItem(*this);
}
KeyItem::~KeyItem() {

}

//MARK: movement
MovementItem::MovementItem(const char _name[255], const char _description[255], const char _useText[255], Room* _room, char* _unlockType, bool _takable, Attack* _attack) : Item(_name, _description, _room, _takable, false, true) {
	unlockType = _unlockType;
	strcpy(useText, _useText);
	strcpy(type, "movement");
	attack = _attack;
}
Room* MovementItem::getTarget() {
	return targetRoom;
}
Attack* MovementItem::getAttack() {
	return attack;
}
char* MovementItem::getUnlockType() {
	return unlockType;
}
char* MovementItem::getUseText() {
	return useText;
}
void MovementItem::setTarget(Room* target) {
	targetRoom = target;
}
Item* MovementItem::Duplicate() {
	return new MovementItem(*this);
}
MovementItem::~MovementItem() {

}
//MARK: paver
PaverItem::PaverItem(const char _name[255], const char _description[255], const char _useText[255], Room* _room, char* _direction, Room* _destination) : Item(_name, _description, _room, false, true) {
	destination = _destination;
	direction = _direction;
	strcpy(type, "paver");
	strcpy(useText, _useText);
}
PaverItem::~PaverItem() {

}
Room* PaverItem::getDestination() {
	return destination;
}
char* PaverItem::getDirection() {
	return direction;
}
char* PaverItem::getUseText() {
	return useText;
}
//MARK: manhole cover
ManholeItem::ManholeItem(const char _name[255], const char _description[255], Room* _room, Room* _destination, Attack* _attack) : Item(_name, _description, _room, true, true, true) {
	destination = _destination;
	strcpy(type, "manhole");
	attack = _attack;
}
ManholeItem::~ManholeItem() {

}
Room* ManholeItem::getRoom() {
	Room* dest = destination;
	destination = NULL;
	return dest;
}
//MARK: info
InfoItem::InfoItem(const char _name[255], const char _description[255], const char _text[255], Room* _room) : Item(_name, _description, _room, false, false) {
	strcpy(text, _text);
	strcpy(type, "info");
}
char* InfoItem::getText() {
	return text;
}
InfoItem::~InfoItem() {

}