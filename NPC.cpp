#include <cstring>
#include <algorithm>
#include <random>
#include <cmath>
#include <iostream>
#include <utility>
#include "NPC.h"
#include "Helper.h"
//#include "Fighter.h"
using namespace std;
using namespace Helper;

NPC::NPC(const char _title[255], const char _name[255], const char _description[255], Room* room, int _health, int _defense, int _attack, int _toughness, int _pierce, int _speed, int _sp, int _level, bool _isleader, bool _player) {
	strcpy(title, _title);
	strcpy(name, _name);
	strcpy(description, _description);
	home = room;
	currentRoom = room;
	room->setNPC(this);
	//self = Fighter(name, description, _health, _defense, _attack, _toughness, _pierce, _speed);
	health = _health;
	maxHealth = _health;
	defense = _defense;
	attack = _attack;
	toughness = _toughness;
	pierce = _pierce;
	speed = _speed;
	isLeader = _isleader;
	isPlayer = _player;
	sp = 0;
	maxSP = _sp;
	if (isLeader) {
		party.push_back(&*this);
	}
	if (_level != 0 && _level <= 100) {
		setLevel(_level);
	}
	if (_level > 100) {
		level = _level;
	}
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
char* NPC::getRecruitedDialogue() {
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
int NPC::getSP() {
	return sp;
}
int NPC::getSPMax() {
	return maxSP;
}
int NPC::getLevel() {
	return level;
}
int NPC::xpForNextLevel() {
	return level * level + 9 - xp;
}
int NPC::xpForLevel(int level) {
	return ((level-1)*(2*level-1)*level/6) + 9*level;
}
vector<NPC*>* NPC::getParty() {
	return &party;
}
bool NPC::getLeader() {
	return isLeader;
}
bool NPC::getHypnotized() {
	return hypnotized;
}
bool NPC::getEscapable() {
	return escapable;
}
Attack* NPC::getCheapestAttack() {
	return cheapest_attack;
}
int NPC::getXpReward() {
	return level*level+5;
}
int NPC::getMonyReward() {
	return level*10;
}
Attack* NPC::getBasicAttack() {
	return standard_attack;
}
vector<Attack*> NPC::getSpecialAttacks() {
	return special_attacks;
}
map<Attack*, int> NPC::getWeights() {
	return attackWeight;
}
bool NPC::getLevelUp() {
	return leveledUp;
}
bool NPC::getEnemy() {
	return isEnemy;
}
bool NPC::getDefeated() {
	return defeated;
}
Item* NPC::getGift() {
	return gift;
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
void NPC::setRecruitedDialogue(const char _dialogue[255]) {
	strcpy(recruitedDialogue, _dialogue);
}
void NPC::setDismissalDialogue(const char _dialogue[255]) {
	strcpy(dismissalDialogue, _dialogue);
}
void NPC::addBattleCry() {

}
void NPC::setRecruitable(bool _recruitable) {
	recruitable = _recruitable;
}
void NPC::Recruit() {
	if (conversations.size() > 0) {
		printDialogue();
	}
	cout << "\n" << name << " - \"" << recruitmentDialogue << "\"";
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
void NPC::setParty(NPC* npc1, NPC* npc2, NPC* npc3, NPC* npc4) {
	for (int i = 1; i < party.size();) {
		delete party[i];
		party.erase(party.begin() + i);
	}

	NPC* npcs[] = { npc1, npc2, npc3, npc4 };
	for (NPC* _npc : npcs) {
		if (_npc != NULL) {
			NPC* npc = new NPC(*_npc);
			party.push_back(npc);
		}
	}
}
void NPC::setName(const char _name[255]) {
	strcpy(name, _name);
}
void NPC::addXp(int _xp) {
	xp += _xp;
	while (xpForNextLevel() <= 0) {
		xp = -xpForNextLevel();
		levelUp(true);
	}
}
//when an npc levels up, we add either 0 or 1 to each stat, plus a base
void NPC::levelUp(bool trackLevelUp) {
	maxHealth += healthScale + rand() % 2;
	health = maxHealth;
	defense += defenseScale + rand() % 2;
	attack += attackScale + rand() % 2;
	toughness += toughnessScale + rand() % 2;
	pierce += pierceScale + rand() % 2; //maybe I shouldn't upgrade this one?
	speed += speedScale + rand() % 2;
	maxSP += spScale + rand() % 2;
	sp = maxSP;
	level++;
	leveledUp = trackLevelUp;
}
void NPC::setHypnotized(bool _hypnotized) {
	hypnotized = _hypnotized;
}
void NPC::setLeader(bool _leader, int _level, Room* room) {
	isLeader = _leader;
	isEnemy = true;
	if (isLeader) {
		setLevel(_level);
		party.push_back(&*this);
		setRoom(room);
	}
}
void NPC::setEnemy(bool _enemy) {
	isEnemy = _enemy;
}
void NPC::blockExit(char* _exitBlocking, char* type, const char reason[255]) {
	exitBlocking = _exitBlocking;
	currentRoom->blockExit(exitBlocking, type, reason);
}
int NPC::damage(float power, float pierce, int hits) {
	float damagePierce = 10; //how much regular damage affects defense alongside pierce (inverse). Afterall, armor will defend you against a sword, but it will literally do nothing if you get hit by a semi truck
	int totalhits = Clamp(hits - guard, 0, 999);
	guard -= hits;
	float defenseF = defense;
	float toughnessF = toughness;
	int damage = (int)ceil(power * (10.0f/(10.0f + ClampF(defenseF - ((power/damagePierce + pierce)*10.0f/(10.0f + toughnessF)),0,defenseF)))) * totalhits;
	int totalDamage = Clamp(damage,0,health);
	if (hits > 1) {
		cout << "\n" << name << " was hit " << hits << " times!";
	}
	if (hits < 1) {
		cout << "\nThe attack missed!";
	}
	if (totalhits < hits) {
		int blocks = hits - totalhits;
		cout << "\n" << name;
		if (blocks == 1) {
			cout << " blocked the attack!";
		} else {
			cout << " blocked " << blocks << " hits!";
		}
		if (guard <= 0) {
			cout << "\n" << name << "'s guard was broken!";
		}
	}
	if (totalhits > 0) {
		health -= totalDamage;
		cout << "\n" << name << " took " << totalDamage << " damage!\n" << name << " now has " << health << "/" << maxHealth << " HP.";
	}
	return totalDamage;
}
void NPC::setLevel(int _level) {
	for (int i = 0; i < _level; i++) {
		levelUp();
	}
	//addXp(xpForLevel(_level));
}
void NPC::setScale(int _health, int _defense, int _attack, int _toughness, int _pierce, int _speed, int _sp) {
	healthScale = _health;
	defenseScale = _defense;
	attackScale = _attack;
	toughnessScale = _toughness;
	pierceScale = _pierce;
	speedScale = _speed;
	spScale = _sp;
}
void NPC::setBasicAttack(Attack* attack) {
	standard_attack = attack;
}
void NPC::addSpecialAttack(Attack* attack) {
	special_attacks.push_back(attack);
}
void NPC::setEscapable(bool _escapable) {
	escapable = _escapable;
}
void NPC::setLevelUp(bool _leveledUp) {
	leveledUp = _leveledUp;
}
void NPC::setGuard(int _guard) {
	guard = _guard;
}
void NPC::setLink(NPC* npc) {
	linkedNPC = npc;
}
void NPC::setGift(Item* item) {
	gift = item;
}
void NPC::calculateWeights() {
	int maxWeight = 90;
	int totalSpCost = 0;
	for (Attack* attack : special_attacks) {
		totalSpCost += attack->cost;
	}
	for (Attack* attack : special_attacks) {
		attackWeight[attack] = maxWeight*attack->cost/totalSpCost;
	}
}
void NPC::alterSp(int amount) {
	sp = Clamp(sp + amount, 0, maxSP);
}
void NPC::defeat() {
	if (exitBlocking != NULL) {
		currentRoom->unblockExit(exitBlocking);
		exitBlocking = NULL;
	}
	if (linkedNPC != NULL) {
		linkedNPC->setRecruitable(true);
		for (int i = 0; i < linkedDialogue.size(); i++) {
			//linkedNPC->addConversation(linkedDialogue[i].first(), linkedDialogue[i].second());
		}
		linkedNPC = NULL;
	}
	defeated = true;
	//set recruitable some other thing
}
void NPC::undefeat() {
	defeated = false;
}
void NPC::addSuffix(const char suffix[3]) {
	strcat(name, suffix);
}
void NPC::addConversation(NPC* speaker, const char dialogue[255], bool newConversation) {
	if (newConversation || !conversations.size()) {
		conversations.emplace();
	}
	conversations.back().emplace_back(speaker, dialogue);
}
void NPC::addLinkedConvo(NPC* speaker, const char dialogue[255]) {
	//linkedDialogue.push_back((speaker, dialogue));
}
void NPC::printDialogue() {
	if (recruited) {
		cout << "\n" << name << " - \"" << recruitedDialogue << "\"";
	} else if (conversations.size() > 0) {
		cout << "\n";
		vector<pair<NPC*, const char*>>& convo = conversations.front();
		for (int i = 0; i < convo.size(); i++) {
			cout << convo[i].first->getName() << " - \"" << convo[i].second << "\"";
			if (i+1 < convo.size()) {
				CinPause();
			}
		}
		conversations.pop();
	} else {
		cout << "\n" << name << " - \"" << dialogue << "\"";
	}
}
NPC::~NPC() {

}