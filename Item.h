//header file for items

#ifndef ITEM
#define ITEM

using namespace std;

class Item {
public:
	Item();
	~Item();

private:
	char name[255];
	char description[255];
};
#endif