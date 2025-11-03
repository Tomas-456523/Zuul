#include <cstring>
#include "NPC.h"
#include "Fighter.h"
using namespace std;

struct Fighter;

NPC::NPC(const char _title[255], const char _name[255], const char _description[255], Room* room, int _health, int _defense, int _attack, int _toughness, int _pierce, int _speed, int _level, bool _player) {
	strcpy(title, _title);
	strcpy(name, _name);
	strcpy(description, _description);
	home = room;
	currentRoom = room;
	room->setNPC(this);
	level = _level;
	self = Fighter(name, description, _health, _defense, _attack, _toughness, _pierce, _speed);
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
	return self.health;
}
int NPC::getHealthMax() {
	return self.maxHealth;
}
int NPC::getDefense() {
	return self.defense;
}
int NPC::getAttack() {
	return self.attack;
}
int NPC::getToughness() {
	return self.toughness;
}
int NPC::getPierce() {
	return self.pierce;
}
int NPC::getSpeed() {
	return self.speed;
}
int NPC::getLevel() {
	return level;
}
int NPC::xpForNextLevel() {
	return level * level + 9 - xp;
}
vector<Fighter>* NPC::getParty() {
	return &party;
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
void NPC::addXp(int _xp) {
	xp += _xp;
	while (xpForNextLevel() <= 0) {
		xp = -xpForNextLevel();
		level++;
	}
	//level up stuff
}
NPC::~NPC() {

}