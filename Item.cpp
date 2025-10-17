#include <cstring>
#include "Item.h"
using namespace std;

Item::Item(const char _name[255], const char _description[255], bool _takable) {
	strcpy(name, _name);
	strcpy(description, _description);
	takable = _takable;
}
Item::~Item() {

}