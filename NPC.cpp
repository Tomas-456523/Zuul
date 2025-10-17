#include <cstring>
#include "NPC.h"
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
void NPC::setRecruited(bool _recruited) {
	recruited = _recruited;
}
void NPC::addHealth() {

}
void NPC::addDefense() {

}
void NPC::addAttack() {

}
void NPC::addToughness() {

}
void NPC::addPierce() {

}
void NPC::addSpeed() {

}
NPC::~NPC() {

}