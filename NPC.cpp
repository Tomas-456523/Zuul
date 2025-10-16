#include "NPC.h"
#include <cstring>
using namespace std;

NPC::NPC(const char _title[255], const char _name[255], const char _description[255], Room* room, int _health, int _defense, int _attack, int _toughness, int _pierce, int _speed, bool _player) {
	strcpy(title, _title);
	strcpy(name, _name);
	strcpy(description, _description);
	home = room;
	currentRoom = room;
	room->setNPC(this);
	maxHealth = _health;
	health = _health;
	defense = _defense;
	attack = _attack;
	toughness = _toughness;
	pierce = _pierce;
	speed = _speed;
	isPlayer = _player;
}
char* NPC::getName() {
	return &name[0];
}
bool NPC::getRecruited() {
	return recruited;
}
NPC::~NPC() {

}