#include <cstring>
#include <iostream>
#include "Item.h"
#include "Room.h"
using namespace std;

Item::Item(const char _name[255], const char _description[255], Room* room, bool _takable) {
	strcpy(name, _name);
	strcpy(description, _description);
	takable = _takable;
	room->setItem(this);
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
void Item::setDenial(char denial[255]) {
	strcpy(denyDescription, denial);
}
void Item::setBuyDesc(char buydesc[255]) {
	strcpy(buyDescription, buydesc);
}
void Item::buy(int& mony) {
	if (mony < price) {
		cout << "\nYou can't afford the " << name << "!";
		return;
	}
	mony -= price;
	takable = true;
	cout << "\nYou bought the " << name << "! You now have " << mony << " mon";
	if (mony == 1) {
		cout << "y.";
	} else {
		cout << "ies.";
	}
}
Item::~Item() {

}