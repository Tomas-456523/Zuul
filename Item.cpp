#include <cstring>
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
Item::~Item() {

}