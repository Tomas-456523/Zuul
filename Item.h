//header file for items

#ifndef ITEM
#define ITEM

using namespace std;

class Item {
public:
	Item(const char _name[255], const char _description[255], bool _takable = true);
	~Item();
private:
	char name[255];
	char description[255];

	bool takable;
};
#endif