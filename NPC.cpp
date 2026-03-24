//implementation file for npcs

#include <cstring>
#include <algorithm>
#include <random>
#include <cmath>
#include <iostream>
#include <utility>
#include "NPC.h"
#include "Helper.h"
#include "Conversation.h"
using namespace std;
using namespace Helper;

//constructs the npc!
NPC::NPC(const char* _title, const char* _name, const char* _description, Room* room, int _level, Stats _basestats, Stats _scale, bool _isleader, bool _player) {
	title = _title;
	strcpy(name, _name);
	description = _description;
	home = room;
	currentRoom = room;
	room->setNPC(this); //make sure the room knows the npc is there
	isLeader = _isleader;
	isPlayer = _player;
	if (isLeader) { //adds itself to its own party if it's a leader npc (fightable or the player)
		party.push_back(&*this);
	}
	npcsH.push_back(this); //store a pointer to this npc in the npcs vector
	id = npcID++; //get this npc's id and increment it for the next one

	if (!_basestats.empty()) basestats = _basestats; //set base stats if specified ones were passed
	else basestats = Stats::genBaseStats(id); //if none were passed, generate some new ones
	stats = basestats; //effective stats start as base stats
	if (!_scale.empty()) scale = _scale;
	else Stats::getStatScale(basestats); //make a default scaling based on the base stats
	health = stats.hpmax; //xp starts at max
	sp = stats.spmax / 3; //start battle at a third of max sp and has to be built up

	if (_level != 0 && _level <= 100) { //sets its given level if it isn't too high, otherwise it freezes the program at the start
		setLevel(_level);
	}
	if (_level > 100) { //instead we just manually set it
		level = _level;
	}
}
NPC::NPC(const NPC& other) { //copy constructor, we do not need to set the stats because we only copy during battle when stats should be the same and during setup world where all the templates are level 0 anyway
	*this = other;
	npcsH.push_back(this); //store a pointer to this npc in the npcs vector
	id = npcID++; //get this npc's id and increment it for the next one
}
//a bunch of functions for getting npc varaibles
const char* NPC::getTitle() {
	return title;
}
const char* NPC::getName() {
	return name;
}
const char* NPC::getDescription() {
	return description;
}
void NPC::printRejectionDialogue() {
	printDialogue(&rejectionDialogue.front());
	if (rejectionDialogue.size() != 1) rejectionDialogue.pop();
}
void NPC::printRecruitmentDialogue() {
	if (speakOnRecruit && !conversations.empty()) { //if we have to print normal dialogue before regular dialogue
		printDialogue();
		speakOnRecruit = false; //only do this once
		CinPause();
	}
	if (recruitmentDialogue.empty()) return;
	printDialogue(&recruitmentDialogue.front());
	recruitmentDialogue.pop();
}
void NPC::printDismissalDialogue() {
	if (dismissalDialogue.empty()) return;
	printDialogue(&dismissalDialogue.front());
	dismissalDialogue.pop();
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
Room* NPC::getRoom(bool alt) {
	if (!alt) {
		return currentRoom;
	}
	return altRoom;
}
int NPC::getHealth() {
	return health;
}
int NPC::getHealthMax() {
	return stats.hpmax;
}
int NPC::getDefense() {
	return stats.defense;
}
int NPC::getAttack() {
	return stats.attack;
}
int NPC::getToughness() {
	return stats.toughness;
}
int NPC::getPierce() {
	return stats.pierce;
}
int NPC::getSpeed() {
	return stats.speed;
}
int NPC::getSP() {
	return sp;
}
int NPC::getSPMax() {
	return stats.spmax;
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
int NPC::getXpReward() {
	if (xpReward) return  xpReward;
	return level*level+5; //arbitrary formula to reward xp
}
int NPC::getMonyReward() {
	if (monyReward) return monyReward;
	return level*2;
}
Attack* NPC::getBasicAttack() {
	return standard_attack;
}
Attack* NPC::getRecoilAttack() {
	return recoilattack;
}
Attack* NPC::getGuardAttack() {
	return guardattack;
}
Attack* NPC::getOpener() {
	return opener;
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
Stats NPC::getStatChanges() {
	Stats changes = statChangesSum;
	statChangesSum = Stats(); //reset stat changes because we don't need to track them anymore
	return changes; //return the stats we recovered
}
vector<Attack*> NPC::getNewAttacks() {
	vector<Attack*> attacks = newAttacks;
	newAttacks.clear(); //reset new attacks because we don't need to track them anymore
	return attacks; //return the attacks we recovered
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
int NPC::getRecovering() {
	return recovering;
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
bool NPC::getBoss() {
	return isBoss;
}
float NPC::getAttMultiplier() {
	return attackMultiplier;
}
float NPC::getDefMultiplier() {
	return defenseMultiplier;
}
float NPC::getToughMultiplier() {
	return toughMultiplier;
}
float NPC::getPierceMultiplier() {
	return pierceMultiplier;
}
float NPC::getSPUseMultiplier() {
	return spUseMultiplier;
}
float NPC::getDamageMultiplier() {
	return damageMultiplier;
}
NPC* NPC::getGuardian() {
	return guardian;
}
NPC* NPC::getGuarding() {
	return guarding;
}
NPC* NPC::getParrying() {
	return parrying;
}
bool NPC::getInvincible() {
	return invincible;
}
time_t NPC::getGymStart() {
	return gymStart;
}
int NPC::getGuard() {
	if (guard < 0) return 0;
	return guard;
}
bool NPC::getAway() { //get if the npc is away from the battle
	return away;
}
Effect* NPC::getEffect(const char* effect) {
	for (Effect& ef : effects) { //check if we already have the effect
		if (!strcmp(effect, ef.name)) {
			return &ef;
		}
	}
	return NULL;
}
void NPC::setWorldCondition(size_t cond) {//set a world condition for this npc to edit on defeat
	changes.worldcon = cond;
}
const char* NPC::getTunnelDirection(Room* room) { //gets the direction back to the lobster's current position from the tunnel
	return tunnelLinks[room];
}
Item* NPC::takeGift() { //takes the gift from the npc and nullifies it because there's only one gift
	if (battleReward) return NULL; //don't give gift if you have to fight first
	Item* item = gift;
	gift = NULL;
	return item;
}
//a bunch of functions to set npc variables
void NPC::setDialogue(const Conversation& _dialogue) {
	dialogue = _dialogue;
}
void NPC::addGymDialogue(const Conversation& _dialogue) {
	gymDialogue.push(_dialogue);
}
void NPC::addRejectionDialogue(const Conversation& _dialogue) {
	rejectionDialogue.push(_dialogue);
}
void NPC::addRecruitmentDialogue(const Conversation& _dialogue) {
	recruitmentDialogue.push(_dialogue);
}
void NPC::addRecruitedDialogue(const Conversation& _dialogue) {
	recruitedDialogue.push(_dialogue);
}
void NPC::addDismissalDialogue(const Conversation& _dialogue) {
	dismissalDialogue.push(_dialogue);
}
void NPC::addConversation(const Conversation& _dialogue) { //add a conversation to the npc's dialogue
	conversations.push(_dialogue); //create a pair of speaker-dialogue in the conversations queue
}
void NPC::setRecruitDialogueChange(const Conversation& _dialogue) {
	newDialogue = _dialogue;
}
//same thing but it accepts one line for simple dialogues
void NPC::setDialogue(const char* _dialogue) {
	dialogue = {{this, _dialogue}};
}
void NPC::addGymDialogue(const char* _dialogue) {
	gymDialogue.push(Conversation({{this, _dialogue}}));
}
void NPC::addRejectionDialogue(const char* _dialogue) {
	rejectionDialogue.push(Conversation({{this, _dialogue}}));
}
void NPC::addRecruitmentDialogue(const char* _dialogue) {
	recruitmentDialogue.push(Conversation({{this, _dialogue}}));
}
void NPC::addRecruitedDialogue(const char* _dialogue) {
	recruitedDialogue.push(Conversation({{this, _dialogue}}));
}
void NPC::addDismissalDialogue(const char* _dialogue) {
	dismissalDialogue.push(Conversation({{this, _dialogue}}));
}
void NPC::addConversation(const char* _dialogue) { //add a conversation to the npc's dialogue
	conversations.push(Conversation({{this, _dialogue}})); //create a pair of speaker-dialogue in the conversations queue
}
void NPC::setRecruitDialogueChange(const char* _dialogue) {
	newDialogue = {{this, _dialogue}};
}
void NPC::setRecruitable(bool _recruitable) {
	recruitable = _recruitable;
}
void NPC::Recruit() { //recruits the npc
	if (!newDialogue.empty()) {
		dialogue = newDialogue;
	}
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
void NPC::setParty(NPC* npc1, NPC* npc2, NPC* npc3, NPC* npc4, NPC* npc5, NPC* npc6, NPC* npc7, NPC* npc8, NPC* npc9, NPC* npc10, NPC* npc11, NPC* npc12, NPC* npc13, NPC* npc14, NPC* npc15, NPC* npc16, NPC* npc17, NPC* npc18, NPC* npc19) {
	NPC* npcs[] = { npc1, npc2, npc3, npc4 }; //makes an array of npcs so we can add them in a for loop
	for (NPC* _npc : npcs) { //loops through the list and adds all the npcs to the party
		if (_npc != NULL) {
			NPC* npc = new NPC(*_npc); //duplicates the npc because there might be multiple of the same one
			party.push_back(npc);
		}
	}
}
void NPC::setName(const char* _name) {
	strcpy(name, _name);
}
void NPC::setTitle(const char* _title) {
	title = _title;
}
void NPC::setDescription(const char* _description) {
	description = _description;
}
void NPC::addXp(int _xp) { //adds xp and checks for level up
	xp += _xp;
	while (xpForNextLevel() <= 0) {
		xp = -xpForNextLevel();
		levelUp(true); //level up and tracks it
	}
}
//when an npc levels up, we add either 0 or 1 to each stat, plus a base
void NPC::levelUp(bool trackLevelUp, int instant) { //we can optionally instantly set it to a certain level, for very high levels
	int oldlevel = level;
	Stats statsup; //how much each stat just went up
	if (instant) { //instantly go to the given level if one was given that wasn't the default 0
		statsup = Stats::avgLvLUp(instant-level) + scale*(instant-oldlevel); //get the average random stat changes for how much we leveled up plus the guaranteed scale for each level up
		level = instant; //set the level
	} else { //normally go up one single level
		statsup = Stats::makeLvlStats(level, id) + scale; //deterministically determine the stats we just got from the level up, plus the baseline stat scale
		level++; //increments the level
	}
	statChangesSum += statsup; //track unprinted stat changes so we can print them later
	stats += statsup; //apply the stat changes
	health = stats.hpmax; //we must start battle at max health so we update current health to match the max
	sp = stats.spmax / 3; //start battle at a third of max sp
	
	leveledUp = trackLevelUp; //registers that we've leveled up so that we can print it later
	for (Attack* attack : special_attacks) { //checks if we got a new attack
		if (attack->minLevel <= level && attack->minLevel > oldlevel) {
			if (trackLevelUp) newAttacks.push_back(attack); //save the new attack so we can print that we got it later
			calculateWeights(); //recalculate weights to account for the new attack
		}
	}
}
//makes this npc a leader npc
void NPC::setLeader(bool _leader, int _level, Room* room, bool respawn, bool boss) {
	isLeader = _leader;
	isEnemy = true; //if we manually set it as leader, it's an enemy
	respawns = respawn; //some enemies don't respawn
	isBoss = boss;
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
void NPC::setBoss(bool boss) {
	isBoss = boss;
}
void NPC::setExtraXP(int xp) {
	xpReward = xp;
}
void NPC::setExtraMonies(int monies) {
	monyReward = xp;
}
void NPC::setTalkOnRecruit(bool talk) {
	speakOnRecruit = talk;
}
void NPC::setGuardian(NPC* npc) {
	guardian = npc;
}
void NPC::setGuarding(NPC* npc) {
	guarding = npc;
}
void NPC::setParrying(NPC* _parrying) {
	parrying = _parrying;
}
void NPC::setInvincible(bool _invincible) {
	invincible = _invincible;
}
void NPC::setGymStart(time_t start) {
	gymStart = start;
}
void NPC::setTunnelDirection(Room* room, const char* direction) { //sets the tunnel direction based on the room the lobster goes through
	tunnelLinks[room] = direction;
}
void NPC::blockExit(const char* _exitBlocking, const char* type, const char* reason, bool bothsides) { //sets that this enemy is blocking an exit
	exitBlocking = _exitBlocking;
	currentRoom->blockExit(exitBlocking, type, reason); //tells the room the exit is blocked
	if (bothsides) { //blocks the corresponding exit from the other side if this npc is blocking the exit from both rooms
		altRoom = currentRoom->getExit(exitBlocking);
		altRoom->blockExit(ReverseDirection[exitBlocking], type, reason);
		altRoom->setNPC(this, true);
	}
}
void NPC::printDamage(int damage, const char* status) { //prints the damage the npc took and why if a reason is given
	if (stats.hpmax <= 999) damage = min(damage, 999); //cap damage printing at 999, unless we actually have reason to print that high
	if (damage > 0) {
		cout << "\n" << name << " took " << damage << " damage!";
	} else if (damage < 0) {
		cout << "\n" << name << " recovered " << -damage << " HP!";
	} else {
		cout << "\n" << name << " was not affected.";
	}
	if (status != NULL) { //prints the status that caused it
		cout << " due to " << status;
	}
	cout << "\n" << name << " now has " << health << "/" << stats.hpmax << " HP."; //prints how much health it now has
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
	guard = Clamp(guard - hits, 0, 99999); //hits lower guard
	float defenseF = stats.defense; //converts stats to floats for easier damage calculation
	float toughnessF = stats.toughness;
	//calculates the damage
	int damage = (int)ceil(power * (10.0f/(10.0f + ClampF(defenseF - ((power/damagePierce + pierce)*10.0f/(10.0f + toughnessF)),0,defenseF)))) * totalhits;
	int totalDamage = Clamp(damage,health-stats.hpmax,health); //clamps the total damage from how much it could heal to how much it can damage before reaching 0 hp
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
		printDamage(damage);
	}
	return totalDamage;
}
//directly applies damage while ignoring defense and all that
void NPC::directDamage(int damage, const char* status) {
	int totalDamage = Clamp(damage,health-stats.hpmax,health); //clamps the total damage from how much it could heal to how much it can damage before reaching 0 hp
	health -= totalDamage;
	printDamage(damage, status); //prints the damage taken and why it was taken
}
void NPC::setLevel(int _level) { //manually sets the level of the npc (will not level down)
	int lvlguard = 100; //guards the level up past a certain amount to avoid the program freezing on large level settings
	if (_level - level > lvlguard) { //if we're leveling up too much, we just instantly go to that level
		levelUp(false, _level);
	} else { //if the level is a reasonable amount, just call the level up process normally for maximum pseudorandomness
		for (int i = level; i < _level; i++) {
			levelUp(false); //levels up that many times in order to get the stat increases
		}
	}
}
//set the scaling of the npc (minimum stat increase per level up)
void NPC::setScale(Stats _scale) {
	scale = _scale;
}
void NPC::setBaseStats(Stats _stats) { //reset base stats and calculate new current stats based on it
	stats -= basestats; //remove current base from stats
	basestats = _stats; //reset base
	stats += basestats; //add new base to the total stats
}
void NPC::setBasicAttack(Attack* attack) {
	standard_attack = attack;
}
void NPC::setRecoilAttack(Attack* attack) {
	recoilattack = attack;
}
void NPC::setGuardAttack(Attack* attack) {
	guardattack = attack;
}
void NPC::setOpener(Attack* attack) {
	opener = attack;
}
void NPC::addSpecialAttack(Attack* attack) {
	special_attacks.push_back(attack);
}
void NPC::removeSpecialAttack(Attack* attack) {
	special_attacks.erase(remove(special_attacks.begin(), special_attacks.end(), attack), special_attacks.end());
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
void NPC::addRecruitLink(NPC* npc) { //links this npc to be set to recuritable later
	changes.recruitLinks.push(npc);
}
void NPC::addLinkedRoom(Room* room, const char* desc) { //room's description gets changed to this after this npc is defeated
	changes.roomChanges.push({room, desc});
}
void NPC::addLinkedDialogue(NPC* speaker, const Conversation& dialogue) {
	changes.linkedDialogue.push({speaker, dialogue});
}
void NPC::addLinkedTitle(NPC* npc, const char* title) {
	changes.linkedTitles.push({npc, title});
}
void NPC::addLinkedDesc(NPC* npc, const char* desc) {
	changes.linkedDescriptions.push({npc, desc});
}
void NPC::addLinkedConvo(NPC* speaker, const Conversation& dialogue) { //add a conversation to add to the linked npc
	changes.linkedConversations.push({speaker, dialogue});
}
void NPC::addLinkedStats(NPC* npc, Stats stats) {
	changes.linkedStats.push({npc, stats});
}
void NPC::addAttackRemoval(NPC* npc, Attack* attack) {
	changes.removeAttacks.push({npc, attack});
}
void NPC::addDefeatRoom(NPC* npc, Room* room) {
	changes.defeatRooms.push({npc, room});
}
void NPC::setGift(Item* item, bool fightfirst) { //set a gift to give to the player after talking
	gift = item;
	battleReward = fightfirst;
}
void NPC::addRedirect(Room* room1, Room* room2) { //makes room1 redirect to room2 after being defeated
	changes.redirectRooms.push({room1, room2});
}
void NPC::guardItem(Item* item) { //guard the given item until defeat
	item->setGuard(this);
	changes.guardedItems.push(item);
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
	}
	attackMultiplier *= effect.attackbuff; //edit stat multipliers
	defenseMultiplier *= effect.defensebuff;
	toughMultiplier *= effect.toughbuff;
	pierceMultiplier *= effect.piercebuff;
	damageMultiplier *= effect.damagebuff;
	spUseMultiplier *= effect.spusage;

	if (battle) { //print stat changes if in battle
		if (effect.defensebuff != 1) { //prints the stat changes
			cout << "\n" << name << "'s DEFENSE went " << (effect.defensebuff > 1 ? "up" : "down") << " by a factor of " << effect.defensebuff << "!";
		}
		if (effect.attackbuff != 1) {
			cout << "\n" << name << "'s ATTACK went " << (effect.defensebuff > 1 ? "up" : "down") << " by a factor of " << effect.attackbuff << "!";
		}
		if (effect.toughbuff != 1) {
			cout << "\n" << name << "'s TOUGHNESS went " << (effect.defensebuff > 1 ? "up" : "down") << " by a factor of " << effect.toughbuff << "!";
		}
		if (effect.piercebuff != 1) {
			cout << "\n" << name << "'s PIERCE went " << (effect.defensebuff > 1 ? "up" : "down") << " by a factor of " << effect.piercebuff << "!";
		}
		if (effect.damagebuff != 1) {
			cout << "\n" << name << " now takes " << damageMultiplier << " times as much damage!";
		}
		if (effect.spusage != 1) {
			cout << "\n" << name << "'s moves now use " << spUseMultiplier << " times SP!";
		}
	}
	if (effect.freeze) { //adds freeze to the npc
		if (!freeze && battle) { //if we weren't already frozen
			cout << "\n" << name << " was frozen in place!";
		}
		freeze++;
	}
	if (effect.hypnotize && battle) { //adds hypnosis to the npc
		if (!hypnosis) { //if the npc wasn't already hynotized
			cout << "\n" << name << " is now fighting for the enemy!";
		}
		hypnosis++;
	}
	if (effect.tiring) { //adds recovering to the npc
		if (!recovering) { //if the npc wasn't already recovering
			cout << "\n" << name << " is recovering from the attack!";
		}
		recovering++;
	}
	if (effect.remove) {
		away = true;
	}
	if (effect.guardset) {
		setGuard(effect.guardset);
	}
	effect.npc = this; //tell the effect it's affecting this npc
	effects.push_back(effect); //adds the effect to the effect tracker
}
void NPC::removeEffect(Effect& effect, bool announce) { //removes an effect from the npc
	for (int i = 0; i < effects.size(); i++) {
		if (!strcmp(effect.name, effects[i].name)) { //if the names match, no conflicting effect names as far as I know
			attackMultiplier /= effects[i].attackbuff; //remove the stat multipliers
			defenseMultiplier /= effects[i].defensebuff;
			toughMultiplier /= effects[i].toughbuff;
			pierceMultiplier /= effects[i].piercebuff;
			damageMultiplier /= effects[i].damagebuff;
			spUseMultiplier /= effects[i].spusage;
			if (effect.freeze) { //decrements freeze if applicable
				freeze--;
				if (!freeze && announce) { //prints if we're no longer frozen
					cout << "\n" << name << " broke free!";
				}
			}
			if (effect.hypnotize) { //decrements hypnosis if applicable
				hypnosis--;
				if (!hypnosis && announce) { //prints if we're no longer hypnotized
					cout << "\n" << name << " snapped out of it!";
				}
			}
			if (effect.tiring) { //decrements hypnosis if applicable
				recovering--;
				if (!recovering && announce) { //prints if we're no longer recovering
					cout << "\n" << name << " has recovered!";
				}
			}
			if (effects[i].defensebuff != 1 && announce) { //prints the stat changes
				cout << "\n" << name << "'s DEFENSE went " << (effect.defensebuff > 1 ? "up" : "down") << " by a factor of " << effect.defensebuff << "!";
			}
			if (effects[i].attackbuff != 1 && announce) {
				cout << "\n" << name << "'s ATTACK went " << (effect.defensebuff > 1 ? "up" : "down") << " by a factor of " << effect.attackbuff << "!";
			}
			if (effects[i].toughbuff != 1 && announce) {
				cout << "\n" << name << "'s TOUGHNESS went " << (effect.defensebuff > 1 ? "up" : "down") << " by a factor of " << effect.toughbuff << "!";
			}
			if (effects[i].piercebuff != 1 && announce) {
				cout << "\n" << name << "'s PIERCE went " << (effect.defensebuff > 1 ? "up" : "down") << " by a factor of " << effect.piercebuff << "!";
			}
			if (effects[i].damagebuff != 1 && announce) {
				//MARK: please edit this
				cout << "\n" << name << " now takes " << damageMultiplier << " times as much damage!";
			}
			if (effects[i].spusage != 1 && announce) {
				cout << "\n" << name << "'s moves now use " << 1.0f/effect.spusage << " times SP!";
			}
			if (effects[i].remove) {
				away = false;
				cout << "\n" << name << " is back!";
				if (effects[i].falldamage) {
					damage(effects[i].falldamage, 0, 1);
				}
			}
			effects.erase(effects.begin() + i); //erase the effect from the vector
			return; //return because no need to check the rest because we just removed it

			/*
			if (effects[i].defensebuff) { //says by how much stats dropped if applicable
				//cout << "\n" << name << "'s DEFENSE went down to " << defense << "!";
			}
			if (effects[i].attackbuff) {
				//cout << "\n" << name << "'s ATTACK went down to " << attack << "!";
			}
			if (effects[i].toughbuff) {
				//cout << "\n" << name << "'s TOUGHNESS went down to " << toughness << "!";
			}
			if (effects[i].piercebuff) {
				//cout << "\n" << name << "'s PIERCE went down to " << pierce << "!";
			}*/
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
void NPC::alterSp(int amount, const char* status) {
	if (amount > 0) { //if there is positive change
		int alterAmount = Clamp(amount, 0, stats.spmax - sp);
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
		if (altRoom != NULL) {
			altRoom->unblockExit(ReverseDirection[exitBlocking]);
			altRoom->removeNPC(this, true);
		}
		exitBlocking = NULL;
	}
	forcebattle = false; //don't force battles after dialogue anymore if we did that (for example, viola so you can't just fight her again)
	if (battleReward) battleReward = false; //now we can give the gift
	applyWorldChange(changes); //apply all the world changes associated with this npc
}
void NPC::undefeat() { //tells the enemy it's not defeated
	defeated = false;
}
void NPC::addSuffix(const char* suffix) { //adds a suffix to the end of the npc's name
	strcat(name, suffix);
}
//prints the npc's dialogue, prioritizing thisone if it's passed
void NPC::printDialogue(Conversation* thisone) {
	Conversation conversation; //uninitialized, please initialize
	if (thisone) { //if we passed thisone, print that one
		conversation = *thisone;
	} else if (recruited) { //prints recruited dialogue if recruited
		conversation = recruitedDialogue.front();
		if (recruitedDialogue.size() > 1) {
			recruitedDialogue.pop();
		}
	} else if (conversations.size() > 0) { //if there's a limited conversation to be had
		do { //do while because conversation starts uninitialized with garbage data
			conversation = conversations.front(); //gets the current conversation
			conversations.pop();
		} while (conversation.getOutdated()); //try again if the conversation is outdated, atm only regular conversations can be outdated
		//NOTE: we MUST always have a queued dialogue after outdated dialogues
	} else if (currentRoom->getGym()) { //gym dialogue if they're in the gym
		conversation = gymDialogue.front();
		if (gymDialogue.size() > 1) {
			gymDialogue.pop();
		}
	} else { //regular dialogue
		conversation = dialogue;
	}
	printConversation(&conversation); //courtesy of Helper
}
NPC::~NPC() { //removes self from npcs vector when destroyed
	npcsH.erase(remove(npcsH.begin(), npcsH.end(), this), npcsH.end());
}