#include <cstring>
#include "NPC.h"
using namespace std;

NPC::NPC(const char _title[255], const char _name[255], const char _description[255], Room* room, int _health, int _defense, int _attack, int _toughness, int _pierce, int _speed, int _level, bool _player) {
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
	level = _level;
	isPlayer = _player;
}
char* NPC::getTitle() {
	return &title[0];
}
char* NPC::getName() {
	return &name[0];
}
char* NPC::getDescription() {
	return &description[0];
}
char* NPC::getDialogue() {
	return dialogue;
}
char* NPC::getRejectionDialogue() {
	return rejectionDialogue;
}
char* NPC::getRecruitmentDialogue() {
	return recruitmentDialogue;
}
char* NPC::getDismissalDialogue() {
	return dismissalDialogue;
}
bool NPC::getRecruited() {
	return recruited;
}
bool NPC::getRecruitable() {
	return recruitable;
}
bool NPC::getPlayerness() {
	return isPlayer;
}
Room* NPC::getHome() {
	return home;
}
int NPC::getHealth() {
	return health;
}
int NPC::getHealthMax() {
	return maxHealth;
}
int NPC::getDefense() {
	return defense;
}
int NPC::getAttack() {
	return attack;
}
int NPC::getToughness() {
	return toughness;
}
int NPC::getPierce() {
	return pierce;
}
int NPC::getSpeed() {
	return speed;
}
int NPC::getLevel() {
	return level;
}
int NPC::xpForNextLevel(int _level) {
	return _level * _level + 9;
}
void NPC::setDialogue(const char _dialogue[255]) {
	strcpy(dialogue, _dialogue);
}
void NPC::setRejectionDialogue(const char _dialogue[255]) {
	strcpy(rejectionDialogue, _dialogue);
}
void NPC::setRecruitmentDialogue(const char _dialogue[255]) {
	strcpy(recruitmentDialogue, _dialogue);
}
void NPC::setDismissalDialogue(const char _dialogue[255]) {
	strcpy(dismissalDialogue, _dialogue);
}
void NPC::addBattleCry() {

}
void NPC::setAttackDescription() {

}
void NPC::setRecruitable(bool _recruitable) {
	recruitable = _recruitable;
}
void NPC::Recruit() {
	recruited = true;
}
void NPC::Dismiss() {
	recruited = false;
	setRoom(home);
}
void NPC::setRoom(Room* _room) {
	currentRoom->removeNPC(this);
	currentRoom = _room;
	currentRoom->setNPC(this);
}
void NPC::addExp(int _exp) {
	exp += _exp;
	//level up based on brackets, idk
}
NPC::~NPC() {

}