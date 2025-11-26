#include <cstring>
#include <iostream>
#include "Item.h"
#include "Room.h"
using namespace std;

Item::Item(const char _name[255], const char _description[255], Room* _room, bool _takable, bool _target) {
	strcpy(name, _name);
	strcpy(description, _description);
	strcpy(type, "default");
	takable = _takable;
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

HpItem::HpItem(const char _name[255], const char _description[255], Room* _room, int _hp) : Item(_name, _description, _room, true, true) {
	hp = _hp;
	strcpy(type, "hp");
}
Item* HpItem::Duplicate() {
	return new HpItem(*this);
}
HpItem::~HpItem() {

}

MaterialItem::MaterialItem(const char _name[255], const char _description[255], Room* _room) : Item(_name, _description, _room, true) {
	strcpy(type, "material");
}
MaterialItem::~MaterialItem() {

}

EducationItem::EducationItem(const char _name[255], const char _description[255], Room* _room, Attack* _attack) : Item(_name, _description, _room, true) {
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