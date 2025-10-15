#include "NPC.h"
#include <cstring>
using namespace std;

NPC::NPC(char _name[255], char _description[255]) {
	strcpy(name, _name);
	strcpy(description, _description);
}
NPC::~NPC() {

}