#include <cstring>
#include <iostream>
#include "Item.h"
#include "Room.h"
using namespace std;

Item::Item(const char _name[255], const char _description[255], Room* _room, bool _takable, int _price) {
	strcpy(name, _name);
	strcpy(description, _description);
	takable = _takable;
	price = _price;
	setRoom(_room);
}
char* Item::getName() {
	return name;
}
char* Item::getDescription() {
	return description;
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
void Item::setDenial(const char denial[255]) {
	strcpy(denyDescription, denial);
}
void Item::setBuyDesc(const char buydesc[255]) {
	strcpy(buyDescription, buydesc);
}
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
	unRoom();
	inventory->push_back(this);
	takable = true;
	cout << "\nYou bought the " << name << "! You now have " << mony << " mon";
	if (mony == 1) {
		cout << "y.";
	} else {
		cout << "ies.";
	}
	cout << "\n" << buyDescription;
}
Item::~Item() {

}