//implementation file for npcs

#include <cstring>
#include <algorithm>
#include <random>
#include <cmath>
#include <iostream>
#include <utility>
#include "NPC.h"
#include "Helper.h"
using namespace std;
using namespace Helper;

//constructs the npc!
NPC::NPC(const char _title[255], const char _name[255], const char _description[255], Room* room, int _health, int _defense, int _attack, int _toughness, int _pierce, int _speed, int _sp, int _level, bool _isleader, bool _player) {
	strcpy(title, _title);
	strcpy(name, _name);
	strcpy(description, _description);
	home = room;
	currentRoom = room;
	room->setNPC(this); //make sure the room knows the npc is there
	health = _health; //set the stats
	maxHealth = _health;
	defense = _defense;
	attack = _attack;
	toughness = _toughness;
	pierce = _pierce;
	speed = _speed;
	isLeader = _isleader;
	isPlayer = _player;
	sp = 0; //sp starts at 0 in each battle so it has to be built up
	maxSP = _sp;
	if (isLeader) { //adds itself to its own party if it's a leader npc (fightable or the player)
		party.push_back(&*this);
	}
	if (_level != 0 && _level <= 100) { //sets its given level if it isn't too high, otherwise it freezes the program at the start
		setLevel(_level);
	}
	if (_level > 100) { //instead we just manually set it
		level = _level;
	}
	//npcsH.push_back(this); //store a pointer to this npc in the npcs vector
}
//a bunch of functions for getting npc varaibles
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
	return recruitedDialogue;
}
char* NPC::getDismissalDialogue() {
	return dismissalDialogue;
}
bool NPC::getRecruited() { //returns if in the player team
	return recruited;
}
bool NPC::getRecruitable() {
	return recruitable;
}
bool NPC::getPlayerness() { //returns if it is the player
	return isPlayer;
}
Room* NPC::getHome() { //returns the home room of the npc
	return home;
}
Room* NPC::getRoom() {
	return currentRoom;
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
int NPC::xpForNextLevel() { //arbitrary formula for the level curve, it just looked good and that's how I chose it
	return level * level + 9 - xp;
}
int NPC::xpForLevel(int level) { //uses an altered version of the above arbitrary formula to find how much xp until getting to a certain level from level 0
	return ((level-1)*(2*level-1)*level/6) + 9*level;
}
vector<NPC*>* NPC::getParty() { //gets the npc's party for leader npcs
	return &party;
}
bool NPC::getLeader() {
	return isLeader;
}
bool NPC::getEscapable() { //if you can escape battle with this npc
	return escapable;
}
int NPC::getXpReward() { //arbitrary formula to reward xp
	return level*level+5;
}
int NPC::getMonyReward() { 
	return level*2;
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
vector<Effect>& NPC::getEffects() {
	return effects;
}
int NPC::getHypnotized() {
	return hypnosis;
}
int NPC::getFrozen() {
	return freeze;
}
bool NPC::getTalkOnDefeat() {
	return talkOnDefeat;
}
bool NPC::getForceBattle() {
	return forcebattle;
}
int NPC::getConvoSize() {
	return conversations.size();
}
bool NPC::getLobster() {
	return isLobster;
}
bool NPC::getRespawn() {
	return respawns;
}
char* NPC::getTunnelDirection(Room* room) { //gets the direction back to the lobster's current position from the tunnel
	return tunnelLinks[room];
}
Item* NPC::takeGift() { //takes the gift from the npc and nullifies it because there's only one gift
	Item* item = gift;
	gift = NULL;
	return item;
}
//a bunch of functions to set npc variables
void NPC::setDialogue(const char _dialogue[255]) {
	strcpy(dialogue, _dialogue);
}
void NPC::setGymDialogue(const char _dialogue[255]) {
	strcpy(gymDialogue, _dialogue);
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
void NPC::setRecruitable(bool _recruitable) {
	recruitable = _recruitable;
}
void NPC::Recruit() { //recruits the npc
	if (conversations.size() > 0) {
		printDialogue(); //I want the player to hear the dialogue before being recriuted so we print it here if it hasn't been heard yet
	}
	cout << "\n" << name << " - \"" << recruitmentDialogue << "\""; //says a thing after being recruited
	recruited = true;
}
void NPC::Dismiss(bool gohome) { //removes the npc from the party
	recruited = false;
	if (gohome) { //go home if we were told to, only false if dismissed in the gym
		setRoom(home);
	}
}
void NPC::setRoom(Room* _room) { //moves the npc from one room to the next, and lets the rooms know of the change
	currentRoom->removeNPC(this);
	currentRoom = _room;
	currentRoom->setNPC(this);
}
void NPC::setHome(Room* room) {
	home = room;
}
//set a party for enemy leader npcs
void NPC::setParty(NPC* npc1, NPC* npc2, NPC* npc3, NPC* npc4) {
	NPC* npcs[] = { npc1, npc2, npc3, npc4 }; //makes an array of npcs so we can add them in a for loop
	for (NPC* _npc : npcs) { //loops through the list and adds all the npcs to the party
		if (_npc != NULL) {
			NPC* npc = new NPC(*_npc); //duplicates the npc because there might be multiple of the same one
			party.push_back(npc);
		}
	}
}
void NPC::setName(const char _name[255]) {
	strcpy(name, _name);
}
void NPC::setTitle(const char _title[255]) {
	strcpy(title, _title);
}
void NPC::addXp(int _xp) { //adds xp and checks for level up
	xp += _xp;
	while (xpForNextLevel() <= 0) {
		xp = -xpForNextLevel();
		levelUp(true); //level up and tracks it
	}
}
//when an npc levels up, we add either 0 or 1 to each stat, plus a base
void NPC::levelUp(bool trackLevelUp) {
	maxHealth += healthScale + rand() % 2;
	health = maxHealth;
	defense += defenseScale + rand() % 2;
	attack += attackScale + rand() % 2;
	toughness += toughnessScale + rand() % 2;
	//pierce += pierceScale + rand() % 2; //maybe I shouldn't upgrade this one?
	speed += speedScale + rand() % 2;
	maxSP += spScale + rand() % 2;
	level++; //increments the level
	leveledUp = trackLevelUp; //registers that we've leveled up so that we can print it later
	calculateWeights(); //recalculates the weights in case we got a new attack
}
//makes this npc a leader npc
void NPC::setLeader(bool _leader, int _level, Room* room, bool respawn) {
	isLeader = _leader;
	isEnemy = true; //if we manually set it as leader, it's an enemy
	respawns = respawn; //some enemies don't respawn
	if (isLeader) {
		setLevel(_level); //set the level
		party.push_back(&*this); //adds itself to its own party
		if (room != NULL) { //puts itself in the given room
			setRoom(room);
		}
	}
}
void NPC::setEnemy(bool _enemy) { //sets if it is an enemy
	isEnemy = _enemy;
}
void NPC::setLobster(Room* tunnels, bool lobster) { //sets that it's the lobster
	isLobster = lobster;
	home = tunnels; //lobster lives in the tunnels
}
void NPC::setTunnelDirection(Room* room, char* direction) { //sets the tunnel direction based on the room the lobster goes through
	tunnelLinks[room] = direction;
}
void NPC::blockExit(char* _exitBlocking, char* type, const char reason[255]) { //sets that this enemy is blocking an exit
	exitBlocking = _exitBlocking;
	currentRoom->blockExit(exitBlocking, type, reason); //tells the room the exit is blocked
}
void NPC::printDamage(int damage, char* status) { //prints the damage the npc took and why if a reason is given
	if (damage >= 0) {
		cout << "\n" << name << " took " << damage << " damage"; //no "!", it gets printed later
	} else {
		cout << "\n" << name << " recovered " << -damage << " HP"; //no "!"
	}
	if (status != NULL) { //prints the status that caused it
		cout << " due to " << status;
	}
	cout << "!\n" << name << " now has " << health << "/" << maxHealth << " HP."; //prints how much health it now has
	if (health <= 0) { //says that the npc is incapacitated if it now has 0 hp
		CinPause();
		cout << "\n" << name << " is incapacitated!";
	}
}
void NPC::printEffects() { //prints the effects this npc has
	int size = effects.size(); //returns if we don't have any effects
	if (size <= 0) {
		return;
	}
	cout << "\n  Status effects: ";
	int i = 0; //track the effect #
	for (Effect& effect : effects) {
		cout << effect.name;
		if (effect.duration < 1000) { //if it has >1000 ticks, it's probably intended to just last forever, so we don't print a duration
			cout << " (" << effect.duration << " ticks left)";
		}
		if (++i != size) { //print comma for formatting unless it's the last effect
			cout << ", ";
		}
	}
	cout << "\n";
}
//damages the npc and returns the total damage
int NPC::damage(float power, float pierce, int hits) {
	float damagePierce = 10; //how much regular damage affects defense alongside pierce (inverse). Afterall, armor will defend you against a sword, but it will literally do nothing if you get hit by a semi truck
	int totalhits = Clamp(hits - guard, 0, 999); //how many times the attack landed
	guard -= hits; //hits lower guard
	float defenseF = defense; //converts stats to floats for easier damage calculation
	float toughnessF = toughness;
	//calculates the damage
	int damage = (int)ceil(power * (10.0f/(10.0f + ClampF(defenseF - ((power/damagePierce + pierce)*10.0f/(10.0f + toughnessF)),0,defenseF)))) * totalhits;
	int totalDamage = Clamp(damage,health-maxHealth,health); //clamps the total damage from how much it could heal to how much it can damage before reaching 0 hp
	if (hits > 1) { //prints how many times it was hit if it was hit a nonstandard amount of times
		cout << "\n" << name << " was hit " << hits << " times!";
	}
	if (hits < 1) {
		cout << "\nThe attack missed!";
	}
	if (totalhits < hits) { //prints how many hits were blocked by the guard
		int blocks = hits - totalhits;
		cout << "\n" << name;
		if (blocks == 1) {
			cout << " blocked the attack!";
		} else {
			cout << " blocked " << blocks << " hits!";
		}
		if (guard <= 0) { //prints if the guard is now down
			cout << "\n" << name << "'s guard was broken!";
		}
	}
	if (totalhits > 0) { //subtract the damage and print how much was done
		health -= totalDamage;
		printDamage(totalDamage);
	}
	return totalDamage;
}
//directly applies damage while ignoring defense and all that
void NPC::directDamage(int damage, char* status) {
	int totalDamage = Clamp(damage,health-maxHealth,health); //clamps the total damage from how much it could heal to how much it can damage before reaching 0 hp
	health -= totalDamage;
	printDamage(totalDamage, status); //prints the damage taken and why it was taken
}
void NPC::setLevel(int _level) { //manually sets the level of the npc
	for (int i = level; i < _level; i++) {
		levelUp(); //levels up that many times in order to get the stat increases
	}
}
//set the scaling of the npc (minimum stat increase per level up)
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
void NPC::setLevelUp(bool _leveledUp) { //set if we leveled up
	leveledUp = _leveledUp;
}
void NPC::setGuard(int _guard) {
	guard = _guard;
}
void NPC::setLink(NPC* npc, const char dialogue[255]) { //links this npc to another one
	linkedNPC = npc;
	strcpy(linkedDialogue, dialogue); //the npc gets their dialogue changed to this after this one is defeated
}
void NPC::setLinkedRoom(Room* room, const char desc[255]) { //links this npc to a room
	linkedRoom = room;
	strcpy(linkedDescription, desc); //the npc gets their description changed to this after this one is defeated
}
void NPC::setGift(Item* item) { //set a gift to give to the player after talking
	gift = item;
}
void NPC::setRedirect(Room* room1, Room* room2) { //makes room1 redirect to room2 after being defeated
	redirectRoom = make_pair(room1, room2);
}
void NPC::setTalkOnDefeat(bool talk) {
	talkOnDefeat = talk;
}
void NPC::setForceBattle(bool force) { //set if the npc forces battle after talking
	forcebattle = force;
}
void NPC::setEffect(Effect* _effect, bool battle) { //sets an effect on the npc
	Effect effect = *_effect; //depointers the effect so we can have seperate durations
	for (Effect& ef : effects) { //check if we already have the effect
		if (!strcmp(effect.name, ef.name)) {
			if (effect.duration < ef.duration) { //it does nothing if it's already here and with a greater duration
				cout << name << " already has " << ef.name << "!";
				return;
			} //extends the duration otherwise
			cout << name << "'s " << ef.name << " was extended!";
			ef.duration = effect.duration;
			return;
		}
	} //increase stats if in a battle
	if (battle) {
		attack += effect.attackbuff;
		defense += effect.defensebuff;
		toughness += effect.toughbuff;
		pierce += effect.piercebuff;
		if (effect.defensebuff) { //prints the stat changes
			cout << "\n" << name << "'s DEFENSE went up to << " << defense << "!";
		}
		if (effect.attackbuff) {
			cout << "\n" << name << "'s ATTACK went up to << " << attack << "!";
		}
		if (effect.toughbuff) {
			cout << "\n" << name << "'s TOUGHNESS went up to << " << toughness << "!";
		}
		if (effect.piercebuff) {
			cout << "\n" << name << "'s PIERCE went up to << " << pierce << "!";
		}
	}
	if (effect.freeze) { //adds freeze to the npc
		if (!freeze) { //if we weren't already frozen
			cout << "\n" << name << " was frozen in place!";
		}
		freeze++;
	}
	if (effect.hypnotize) { //adds hypnosis to the npc
		if (!hypnosis) { //if the npc wasn't already hynotized
			cout << "\n" << name << " is now fighting for the enemy!";
		}
		hypnosis++;
	}
	effect.npc = this; //tell the effect it's affecting this npc
	effects.push_back(effect); //adds the effect to the effect tracker
}
void NPC::removeEffect(Effect& effect) { //removes an effect from the npc
	for (int i = 0; i < effects.size(); i++) {
		if (!strcmp(effect.name, effects[i].name)) { //if the names match, no conflicting effect names as far as I know
			attack -= effects[i].attackbuff; //removes the stat changes
			defense -= effects[i].defensebuff;
			toughness -= effects[i].toughbuff;
			pierce -= effects[i].piercebuff;
			if (effects[i].defensebuff) { //says by how much stats dropped if applicable
				cout << "\n" << name << "'s DEFENSE went down to " << defense << "!";
			}
			if (effects[i].attackbuff) {
				cout << "\n" << name << "'s ATTACK went down to " << attack << "!";
			}
			if (effects[i].toughbuff) {
				cout << "\n" << name << "'s TOUGHNESS went down to " << toughness << "!";
			}
			if (effects[i].piercebuff) {
				cout << "\n" << name << "'s PIERCE went down to " << pierce << "!";
			}
			if (effect.freeze) { //decrements freeze if applicable
				freeze--;
				if (!freeze) { //prints if we're no longer frozen
					cout << "\n" << name << " is no longer frozen!";
				}
			}
			if (effect.hypnotize) { //decrements hypnosis if applicable
				hypnosis--;
				if (!hypnosis) { //prints if we're no longer hypnotized
					cout << "\n" << name << " snapped out of it!";
				}
			}
			effects.erase(effects.begin() + i); //erase the effect from the vector
			return; //return because no need to check the rest because we just removed it
		}
	}
}
//calculate attack weights
void NPC::calculateWeights() {
	int maxWeight = 90; //maximum weight that all attacks' cumulative weights add up to (out of 100)
	int totalSpCost = 0; //total cost of all attacks
	for (Attack* attack : special_attacks) { //add the cost of every special attack to the total
		if (level >= attack->minLevel) { //only attacks we can use currently
			totalSpCost += attack->cost;
		}
	}
	for (Attack* attack : special_attacks) { //calculate the weight based on the cost out of total cost
		if (level >= attack->minLevel) {
			attackWeight[attack] = maxWeight*attack->cost/totalSpCost;
		}
	}
}
//changes the sp amount
void NPC::alterSp(int amount, char* status) {
	if (amount > 0) { //if there is positive change
		int alterAmount = Clamp(amount, 0, maxSP - sp);
		sp += alterAmount; //alter the amount
		if (status != NULL) { //say why it happened
			cout << "\n" << name << " gained " << alterAmount << " SP due to " << status << "!";
		}
		return;
	} //if we're removing sp
	int alterAmount = Clamp(-amount, 0, sp);
	sp -= alterAmount;
	if (status != NULL) { //say the reason if there is one
		cout << "\n" << name << " leaked " << alterAmount << " SP due to " << status << "!";
	}
}
//stuff that happens when the npc is defeated in battle
void NPC::defeat() {
	defeated = true; //was defeated
	if (exitBlocking != NULL) { //unblock blocked exits
		currentRoom->unblockExit(exitBlocking);
		exitBlocking = NULL;
	}
	if (linkedNPC != NULL) { //do stuff for linked npc
		if (!linkedNPC->getLobster()) { //set npc to recuitable unless it's the lobster
			linkedNPC->setRecruitable(true);
		}
		if (linkedNPC->getLeader()) { //unleader the linked npc
			linkedNPC->setLeader(false);
			linkedNPC->undefeat(); //and undefeat them
		}
		for (int i = 0; i < linkedConversation.size(); i++) { //adds the linked conversation to the linked npc
			linkedNPC->addConversation(linkedConversation[i].first, linkedConversation[i].second, !i); //new conversation if it's the first line (the !i)
		}
		if (strcmp(linkedDialogue, "")) { //if there's linked dialogue we set that too
			linkedNPC->setDialogue(linkedDialogue);
		}
		linkedNPC = NULL; //nullify them because we won't be using the linked npc anymore from here
	}
	if (linkedRoom != NULL) { //change the description of the linked room
		linkedRoom->setDescription(linkedDescription);
	}
	if (redirectRoom.first != NULL) { //set the redirect in the room
		redirectRoom.first->setRedirect(redirectRoom.second);
		redirectRoom = make_pair((Room*)NULL, (Room*)NULL); //nullify the redirect
	}
	if (defeatChange) { //do changes to the npc if applicable
		strcpy(title, defeatTitle);
		strcpy(description, defeatDescription);
		strcpy(dialogue, defeatDialogue);
		if (defeatRoom != NULL) { //move them to this room and now that's their home room
			setRoom(defeatRoom);
			setHome(defeatRoom);
		}
		defeatChange = false; //no need to change again
	}
}
void NPC::undefeat() { //tells the enemy it's not defeated
	defeated = false;
}
//sets stuff that changes when we defeat this guy
void NPC::setDefeatNPC(const char newTitle[255], const char newDesc[255], const char newDialogue[255], Room* newRoom) {
	strcpy(defeatTitle, newTitle);
	strcpy(defeatDescription, newDesc);
	strcpy(defeatDialogue, newDialogue);
	defeatRoom = newRoom;
	defeatChange = true;
}
void NPC::addSuffix(const char suffix[3]) { //adds a suffix to the end of the npc's name
	strcat(name, suffix);
}
void NPC::addConversation(NPC* speaker, const char dialogue[255], bool newConversation) { //add a conversation to the npc's dialogue
	if (newConversation || !conversations.size()) { //if the queue is empty we reserve a space
		conversations.emplace();
	}
	conversations.back().emplace_back(speaker, dialogue); //create a pair of speaker-dialogue in the conversations vector
}
void NPC::addLinkedConvo(NPC* speaker, const char dialogue[255]) { //add a conversation to add to the linked npc
	linkedConversation.emplace_back(speaker, dialogue);
}
//prints the npc's dialogue
void NPC::printDialogue() {
	if (recruited) { //prints recruited dialogue if recruited
		cout << "\n" << name << " - \"" << recruitedDialogue << "\"";
	} else if (conversations.size() > 0) { //if there's a conversation to be had
		cout << "\n";
		vector<pair<NPC*, const char*>>& convo = conversations.front(); //gets the current conversation
		for (int i = 0; i < convo.size(); i++) { //prints all the dialogue in the conversation
			if (convo[i].first != NULL) {
				cout << convo[i].first->getName() << " - \"" << convo[i].second << "\"";
			} else {
				cout << convo[i].second;
			}
			if (i+1 < convo.size()) { //if it's not the last one we do a pause, so the last one lets the player type
				CinPause();
			}
		}
		conversations.pop(); //removes the conversation because it's been said
	} else if (currentRoom->getGym()) { //gym dialogue if they're in the gym
		cout << "\n" << name << " - \"" << gymDialogue << "\"";
	} else { //regular dialogue
		cout << "\n" << name << " - \"" << dialogue << "\"";
	}
}
NPC::~NPC() { //destructor

}