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
	for (NPC* npc : *_enemyTeam) {
		if (npc->getLeader()) {
			enemyLevel = npc->getLevel();
			xpReward += npc->getXpReward();
			monyReward += npc->getMonyReward();
			break;
		}
	}
	for (NPC* npc : *_enemyTeam) {
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
}
//use commands may be formatted "USE ITEM", or "USE ITEM ON NPC", or "USE ITEM NAME ON NPC", so have fun coding that
bool Battle::useItem(char* commandExtensionP) {
	//use the item
	return false;
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
//interpret and carry out player attacks, and return whether we successfully launched an attack
bool Battle::ParseAttack(NPC* plr, char* commandP, char* commandWordP, char* commandExtensionP, bool checkDouble) {
	NPC* target = getNPCInVector(enemyTeam, commandExtensionP);
	if (target == NULL && !checkDouble) {
		return false;
	}
	if (!strcmp(commandWordP, player->getBasicAttack()->name)) {
		//replace with carryOutAttack function {{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}
		Attack* attack = player->getBasicAttack();
		plr->alterSp(-attack->cost);
		//also add effects
		cout << "\n" << plr->getName() << " used " << attack->name << "!\n" << plr->getName() << " " << attack->description << " " << target->getName() << "!";
		CinPause();
		cout << "\n" << target->getName() << " took " << target->damage(attack->power+plr->getAttack(), attack->pierce+plr->getPierce()) << " damage!\n" << target->getName() << " now has " << target->getHealth() << "/" << target->getHealthMax() << " HP.";
		CinPause();
		return true;
	} else {
		for (Attack* attack : player->getSpecialAttacks()) {
			if (attack->minLevel <= player->getLevel() && !strcmp(commandWordP, attack->name)) {
				//replace with carryOutAttack function {{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}
				plr->alterSp(-attack->cost);
				//also add effects
				cout << "\n" << plr->getName() << " used " << attack->name << "!\n" << plr->getName() << " " << attack->description << " " << target->getName() << "!";
				CinPause();
				cout << "\n" << target->getName() << " took " << target->damage(attack->power+plr->getAttack(), attack->pierce+plr->getPierce()) << " damage!\n" << target->getName() << " now has " << target->getHealth() << "/" << target->getHealthMax() << " HP.";
				CinPause();
				return true;
			}
		}
	}
	if (!checkDouble) {
		cout << "\nInvalid command or attack \"" << commandWordP << "\" (type HELP for help, or ATTACKS for list of attacks).";
		return false;
	//if no enemy was found at all, check for double word attacks
	} else if (true) {
		//clear commandWordP & commandExtensionP?
		ParseCommand(commandP, commandWordP, commandExtensionP, 1);
		return ParseAttack(plr, commandP, commandWordP, commandExtensionP, false);
	}
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
	while (target == NULL) {
		if (npc->getEnemy()) {
			target = playerTeam[rand()%playerTeam.size()];
			if (target->getHealth() <= 0) {
				target = NULL;
			}
		} else {
			target = enemyTeam[rand()%enemyTeam.size()];
			if (target->getHealth() <= 0) {
				target = NULL;
			}
		}
	}
	//replace with carryOutAttack function {{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}
	npc->alterSp(-attack->cost);
	//also add effects
	cout << "\n" << npc->getName() << " used " << attack->name << "!\n" << npc->getName() << " " << attack->description << " " << target->getName() << "!";
	CinPause();
	cout << "\n" << target->getName() << " took " << target->damage(attack->power+npc->getAttack(), attack->pierce+npc->getPierce()) << " damage!\n" << target->getName() << " now has " << target->getHealth() << "/" << target->getHealthMax() << " HP.";
	CinPause();
}
//begins the battle process and returns 0 if the player team lost, 1 if they won, and 2 if they ran away
int Battle::FIGHT() {
	bool continuing = true;
	cout << "\nBATTLE BEGIN!";
	printTeam(playerTeam);
	cout << "\n<<< VERSUS >>>";
	printTeam(enemyTeam);
	CinPause();

	queue<NPC*> turn = reorder();
	NPC* current;

	while (continuing) {
		current = turn.front();
		if (current->getHealth() <= 0) {
			//do a backflip idk
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
		if (turn.size() <= 0) {
			turn = reorder();
			//tick effects
		}
	}
	return 2; //return false because the player ran away
}
Battle::~Battle() {
	//delete npc copies
}