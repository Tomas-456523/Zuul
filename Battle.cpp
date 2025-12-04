#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include "Battle.h"
#include "NPC.h"
#include "Item.h"
#include "Helper.h"

using namespace std;
using namespace Helper;

Battle::Battle(vector<NPC*>* _playerTeam, vector<NPC*>* _enemyTeam, vector<Item*>* _inventory, int& mony, bool _escapable) {
	//clone npcs from both teams
	for (NPC* npc : *_playerTeam) {
		playerTeam.push_back(new NPC(*npc));
	}
	for (NPC* npc : *_enemyTeam) {
		enemyTeam.push_back(new NPC(*npc));
	}
	everyone.insert(everyone.begin(), playerTeam.begin(), playerTeam.end());
	everyone.insert(everyone.end(), enemyTeam.begin(), enemyTeam.end());
	sortBySpeed(everyone);

	inventory = _inventory;
	escapable = _escapable;
	for (NPC* npc : *_playerTeam) {
		if (npc->getPlayerness()) {
			player = npc;
			break;
		}
	}
	//give the enemies xp corresponding with the enemy's level, in order to scale them to that level
	int enemyLevel;
	for (NPC* npc : enemyTeam) {
		if (npc->getLeader()) {
			npc->setEnemy(true);
			enemyLevel = npc->getLevel();
			xpReward += npc->getXpReward();
			monyReward += npc->getMonyReward();
			break;
		}
	}
	for (NPC* npc : enemyTeam) {
		if (!npc->getLeader()) {
			//add xp to the npc to scale them to the leader's level
			npc->setLevel(enemyLevel);
			npc->setEnemy(true);
			xpReward += npc->getXpReward();
			monyReward += npc->getMonyReward();
		}
	}
	xpReward *= enemyLevel;
	monyReward *= enemyLevel;
	vector<char*> names;
	vector<int> amount;
	vector<int> count;
	for (NPC* npc : enemyTeam) {
		char* name = new char[255];
		strcpy(name, npc->getName());
		bool namefound = false;
		for (int i = 0; i < names.size(); i++) {
			if (!strcmp(name, names[i])) {
				amount[i]++;
				namefound = true;
				break;
			}
		}
		if (!namefound) {
			names.push_back(name);
			amount.push_back(1);
			count.push_back(0);
		}
	}
	for (NPC* npc : enemyTeam) {
		for (int i = 0; i < names.size(); i++) {
			if (!strcmp(npc->getName(), names[i])) {
				if (amount[i] > 1) {
					char suffix[3] = "  "; //do not edit this string
					suffix[1] = '0' + ++count[i];
					npc->addSuffix(suffix);
				}
				break;
			}
		}
	}
	for (char* name : names) {
		delete[] name;
	}
}
//if time permits you should make a Helper function that does the first bit modularly
bool Battle::useItem(char* itemname) {
	Item* item = getItemInVector(*inventory, commandExtensionP);
	char itemName[255] = "";
	char npcName[255] = "";
	NPC* npc = NULL;
	if (item == NULL) {
		ParseWithON(itemname, &itemName[0], &npcName[0]);
		item = getItemInVector(*inventory, itemName);
		if (item != NULL) {
			npc = getNPCInVector(*party, npcName);
			if (!item->getTargetNeeded()) {
				cout << "\nThe " << itemName << " doesn't need a target!";
				return false;
			} else if (npc == NULL) {
				cout << "\nThere is nobody named \"" << npcName << "\" in your party!";
				return false;
			}
		} else {
			itemname = itemName; //makes the null item print not say "You have no [item] ON [npc]"
		}
	}
	if (item == NULL) {
		cout << "\nYou have no \"" << itemname << "\".";
		return false;
	} else if (item->getTargetNeeded() && npc == NULL) {
		if (party->size() > 1) {
			cout << "\nThis item needs a target!";
			return false;
		}
		npc = (*party)[0]; //if we only have the player in the party we just use them because who else would we be targetting
	}

	if (!strcmp(item->getType(), "hp")) {
		HpItem* hp = (HpItem*)item;

	} else if (!strcmp(item->getType(), "sp")) {
		SpItem* sp = (SpItem*)item;

	} if (!strcmp(item->getType(), "revive")) {
		ReviveItem* revive = (ReviveItem*)item;

	} if (!strcmp(item->getType(), "effect")) {
		EffectItem* affecter = (EffectItem*)item;

	} else if (!strcmp(item->getType(), "BURGER")) {

	//some key items have attacks
	} else if (!strcmp(item->getType(), "key")) {
		KeyItem* key = (KeyItem*)item;
	//some movement items have attacks
	} else if (!strcmp(item->getType(), "movement")) {
		MovementItem* mover = (MovementItem*)item;
	
	//dont think I have any pickupable ones but if there is just uncomment this
	//} else if (!strcmp(item->getType(), "paver")) {
	//you can quickly read info items you have, in case you need to use it in battle for whatever reason
	} else if (!strcmp(item->getType(), "info")) {
		InfoItem* info = (InfoItem*)item;
		cout << "\n" << info->getText();
		return false;
	} else {
		cout << "\nThe " << itemname << " can't be used in battle!";
		return false;
	}
	if (item->getConsumable()) {
		deleteItem(NULL, inventory, item);
	}
	return true;
}
void Battle::printTeam(vector<NPC*> team, bool printLevel, bool printSP) {
	for (NPC* npc : team) {
		cout << "\n" << npc->getName() << " " << npc->getHealth() << "/" << npc->getHealthMax() << " HP";
		if (printSP) {
			cout << " " << npc->getSP() << "/" << npc->getSPMax() << " SP";
		}
		if (printLevel) {
			cout << " - LEVEL " << npc->getLevel();
		}
	}
}
void Battle::printInventory() {
	cout << "\nYou";
	if (inventory->size() < 1) {
		cout << " have no items!";
		return;
	}
	cout << "r items are:";
	for (Item* item : *inventory) {
		cout << "\n" << item->getName();
	}
}
void Battle::printParty() {
	cout << "\nMembers of your party:";
	printTeam(playerTeam, true, true);
}
void Battle::printEnemies() {
	cout << "\nEnemy party:";
	printTeam(enemyTeam, true);
}
void Battle::printAttacks(NPC* npc) {
	cout << "\nBasic attack:\n";
	Attack* attack = npc->getBasicAttack();
	cout << attack->name << " - " << attack->trueDesc << " - Generates " << -attack->cost << " SP";
	if (npc->getSpecialAttacks().size() > 0) {
		cout << "\nSpecial attacks:";
	}
	for (Attack* attack : npc->getSpecialAttacks()) {
		if (attack->minLevel <= npc->getLevel()) {
			cout << "\n" << attack->name << " - " << attack->trueDesc << " - Costs " << attack->cost << " SP";
		}
	}
}
void Battle::analyze(char* name) {
	NPC* npc = getNPCInVector(everyone, name);
	if (npc != NULL) {
		printNPCData(npc);
		return;
	}
	Item* item = getItemInVector(*inventory, name);
	if (item != NULL) {
		printItemData(item);
		return;
	}
	cout << "\nThere is no item or person named \"" << name << "\" here!";
}
void Battle::printHelp() {
	cout << "\n";
	cout << flavorText[rand() % 8];
	cout << "\nValid commands:";
	for (int i = 0; i < 9; i++) {
		cout << "\n" << validCommands[i];
	}
}
bool Battle::runAway() {
	if (escapable) {
		cout << "\nYou successfully ran away!";
		CinPause();
	} else {
		cout << "\nThere is no escape.";
	}
	return escapable;
}
queue<NPC*> Battle::reorder() {
	queue<NPC*> orderly_fashion;
	for (NPC* npc : everyone) {
		orderly_fashion.emplace(npc);
	}
	return orderly_fashion;
}
/*void Battle::logEffect(Effect& effect) {
	allEffects.push_back(effect);
}*/
//tick the individual effect
void Battle::tickEffect(Effect& effect, vector<Effect*>& choppingBlock) {
	NPC* npc = effect.npc;
	//we decrement effect durations, and add them to the chopping block if duration is at 0 (we don't delete immediately in order to not create dangling pointers in allEffects)
	if (--effect.duration <= 0) {
		choppingBlock.push_back(&effect);
		return;
	}
	if (effect.damage != 0) {
		npc->directDamage(effect.damage, effect.name);
	}
	if (effect.guardset != 0) {
		npc->setGuard(effect.guardset);
	}
}
void Battle::tickEffects() {
	//I have to build this here every turn or else modifying the npc's effect vectors would dangle the pointers
	//vector<Effect*> allEffects;
	vector<Effect*> choppingBlock;
	for (NPC* npc : everyone) {
		for (Effect& effect : npc->getEffects()) {
			tickEffect(effect, choppingBlock);
		}
	}
	for (Effect* effect : choppingBlock) {
		cout << effect->npc->getName() << "'s " << effect->name << " wore off!";
		effect->npc->removeEffect(*effect);
		CinPause();
	}
}
//i should slightly randomly alter attack
void Battle::carryOutAttack(Attack* attack, NPC* attacker, NPC* target) {
	attacker->alterSp(-attack->cost);
	cout << "\n" << attacker->getName() << " used " << attack->name << "!\n" << attacker->getName() << " " << attack->description << " " << target->getName() << "!";
	vector<NPC*> tarparty;
	if (target->getEnemy()) {
		tarparty = enemyTeam;
	} else {
		tarparty = playerTeam;
	}
	if (attack->targets == 3 && tarparty.size() > 1) {
		cout << "\n" << attacker->getName() << "'s surrounding teammates were also hit!";
	}
	CinPause();
	int tarPos = distance(tarparty.begin(), find(tarparty.begin(), tarparty.end(), target));
	for (int i = 0; i < tarparty.size(); i++) {
		if (tarPos - attack->targets/2 <= i && i < tarPos + attack->targets - attack->targets/2) {
			tarparty[i]->damage(attack->power + attacker->getAttack(), attack->pierce + attacker->getPierce(), rand() % (attack->maxhits + 1 - attack->minhits) + attack->minhits);
			if (attack->appliedeffect != NULL) {
				tarparty[i]->setEffect(attack->appliedeffect);
			}
		}
	}
	//target->damage(attack->power+attacker->getAttack(), attack->pierce+attacker->getPierce(), rand()%(attack->maxhits+1, attack->minhits) + attack->minhits);
	CinPause();
}
//interpret and carry out player attacks, and return whether we successfully launched an attack
bool Battle::ParseAttack(NPC* plr, char* commandP, char* commandWordP, char* commandExtensionP, int checkMax) {
	for (int i = 0; i <= checkMax; i++) {
		if (i > 0) { //if it's the first one, we've already parsed it the same, no need to parse
			ParseCommand(commandP, commandWordP, commandExtensionP, i);
		}

		NPC* target = getNPCInVector(enemyTeam, commandExtensionP);

		if (target == NULL) {
			if (i == checkMax) {
				cout << "\nInvalid target \"" << commandExtensionP << "\".";
				return false;
			}
			continue;
		}
		if (!strcmp(commandWordP, plr->getBasicAttack()->name)) {
			carryOutAttack(plr->getBasicAttack(), plr, target);
			return true;
		}
		for (Attack* attack : plr->getSpecialAttacks()) {
			if (attack->minLevel <= plr->getLevel() && !strcmp(commandWordP, attack->name)) {
				carryOutAttack(attack, plr, target);
				return true;
			}
		}
	}
	
	cout << "\nInvalid command or attack \"" << commandWordP << "\" (type HELP for help, or ATTACKS for list of attacks).";
	return false;
}
bool Battle::playerTurn(NPC* plr) {
	bool continuing = true;
	bool keepFighting = true;

	while (continuing) {
		char command[255] = "";

		char commandWord[255];
		char commandExtension[255];

		cout << "\n> ";
		cin.getline(command, 255);
		AllCaps(command);

		ParseCommand(command, commandWord, commandExtension);

		if (!strcmp(commandWord, "USE")) {
			continuing = useItem(commandExtension);
		} else if (!strcmp(commandWord, "INVENTORY")) {
			printInventory();
		} else if (!strcmp(commandWord, "PARTY")) {
			printParty();
		} else if (!strcmp(commandWord, "ENEMIES")) {
			printEnemies();
		} else if (!strcmp(commandWord, "ATTACKS")) {
			printAttacks(player);
		} else if (!strcmp(commandWord, "ANALYZE")) {
			analyze(commandExtension);
		} else if (!strcmp(commandWord, "HELP")) {
			printHelp();
		} else if (!strcmp(commandWord, "RUN")) {
			keepFighting = continuing = !runAway();
		} else {
			continuing = !ParseAttack(plr, command, commandWord, commandExtension);
		}
	}

	CinIgnoreAll();

	return keepFighting;
}
void Battle::npcTurn(NPC* npc) {
	cout << "\n" << npc->getName() << "'s turn!";
	CinPause();
	int r = rand()%100;
	int limit = 0;
	Attack* attack = NULL;
	for (Attack* _attack : npc->getSpecialAttacks()) {
		if (_attack->cost > npc->getSP()) {
			continue;
		}
		limit += npc->getWeights()[_attack];
		if (r <= limit) {
			attack = _attack;
			break;
		}
	}
	if (attack == NULL) {
		attack = npc->getBasicAttack();
	}
	NPC* target = NULL;
	bool attackPlayer = npc->getEnemy();
	if (attack->targetAlly) {
		attackPlayer = !attackPlayer;
	}
	if (npc->getHypnotized()) {
		attackPlayer = !attackPlayer;
	}
	while (target == NULL) {
		if (attackPlayer) {
			target = playerTeam[rand() % playerTeam.size()];
		} else {
			target = enemyTeam[rand() % enemyTeam.size()];
		}
		if (target->getHealth() <= 0) {
			target = NULL;
		}
	}
	carryOutAttack(attack, npc, target);
}
//begins the battle process and returns 0 if the player team lost, 1 if they won, and 2 if they ran away
int Battle::FIGHT() {
	bool continuing = true;
	cout << "\nBATTLE BEGIN!";
	printTeam(playerTeam);
	cout << "\n<<< VERSUS >>>";
	printTeam(enemyTeam);
	CinPause();

	queue<NPC*> turn;
	NPC* current;

	while (continuing) {
		if (turn.size() <= 0) {
			turn = reorder();
			tickEffects();
		}

		current = turn.front();
		if (current->getHealth() <= 0) {
			//do a backflip idk
		} else if (current->getFrozen()) {
			cout << "\n" << player->getName() << " tried to move but is frozen in place!";
		} else if (current->getPlayerness()) {
			cout << "\n" << player->getName() << "'s turn!\nWhat will you do?";
			continuing = playerTurn(current);
		} else {
			npcTurn(current);
		}
		
		turn.pop();
		//check the player team and enemy team for if they've lost all hp, and returns win or loss based on that result
		if (aliveCount(playerTeam) <= 0) {
			return 0; //lose
		} else if (aliveCount(enemyTeam) <= 0) {
			return 1; //win
		}
	}
	return 2; //return false because the player ran away
}
int Battle::getXpReward() {
	return xpReward;
}
int Battle::getMonyReward() {
	return monyReward;
}
Battle::~Battle() {
	//delete npc copies
}