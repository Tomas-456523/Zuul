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
			xpReward += npc->getXpReward();
			monyReward += npc->getMonyReward();
		}
	}
	xpReward *= enemyLevel;
	monyReward *= enemyLevel;
}
void Battle::printTeam(vector<NPC*> team) {
	for (NPC* npc : team) {
		cout << "\n" << npc->getName() << " (" << npc->getHealth() << "/" << npc->getHealthMax() << ") - LEVEL " << npc->getLevel();
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
	printTeam(playerTeam);
}
void Battle::analyze() {

}
void Battle::printHelp() {
	cout << "\n";
	cout << flavorText[rand() % 8];
	cout << "\nValid commands:";
	for (int i = 0; i < 7; i++) {
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
bool Battle::playerTurn() {
	bool continuing = true;
	char command[255] = "";

	char commandWord[255];
	char commandExtension[255];

	cout << "\nWhat will you do?\n> ";
	cin.getline(command, 255);
	AllCaps(command);

	ParseCommand(command, commandWord, commandExtension);

	if (!strcmp(commandWord, "USE")) {

	} else if (!strcmp(commandWord, "INVENTORY")) {

	} else if (!strcmp(commandWord, "PARTY")) {

	} else if (!strcmp(commandWord, "ATTACKS")) {

	} else if (!strcmp(commandWord, "ANALYZE")) {

	} else if (!strcmp(commandWord, "HELP")) {
		printHelp();
	} else if (!strcmp(commandWord, "RUN")) {
		continuing = !runAway();
	} else if (!strcmp(commandWord, "[attack]")) {
		//you have to check if it matches an attack name in our attack vector, and then use that attack to attack
	} else {
		cout << "\nInvalid command \"" << commandWord << "\" (type HELP for help).";
	}

	CinIgnoreAll();

	return continuing;
}
void Battle::npcTurn() {

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
		if (current->getPlayerness()) {
			continuing = playerTurn();
		} else {
			//choose random move
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