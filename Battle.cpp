#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
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
			break;
		}
	}
	for (NPC* npc : *_enemyTeam) {
		if (!npc->getLeader()) {
			//add xp to the npc to scale them to the leader's level
			npc->addXp(1);
		}
	}
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
	srand(time(NULL));
	cout << flavorText[rand() % 8];
	cout << "\nValid commands:";
	for (int i = 0; i < 7; i++) {
		cout << "\n" << validCommands[i];
	}
}
bool Battle::runAway() {
	if (escapable) {
		cout << "\nYou successfully ran away!";
	} else {
		cout << "\nThere is no escape.";
	}
	return escapable;
}
bool Battle::playerTurn() {
	bool continuing = true;
	char command[255] = "";

	char commandWord[255];
	char commandExtension[255];

	cout << "\n> ";
	cin.getline(command, 255);
	AllCaps(command);

	ParseCommand(command, commandWord, commandExtension);

	if (!strcmp(commandWord, "ATTACK")) {

	} else if (!strcmp(commandWord, "USE")) {

	} else if (!strcmp(commandWord, "INVENTORY")) {

	} else if (!strcmp(commandWord, "PARTY")) {

	} else if (!strcmp(commandWord, "ANALYZE")) {

	} else if (!strcmp(commandWord, "HELP")) {

	} else if (!strcmp(commandWord, "RUN")) {
		continuing = runAway();
	} else {
		cout << "\nInvalid command \"" << commandWord << "\" (type HELP for help).";
	}

	CinIgnoreAll();

	return continuing;
}
//begins the battle process and returns true if the player team won
bool Battle::FIGHT() {
	bool continuing = true;
	while (continuing) {
		//get current npc
		//npc turn
		//if npc.getPlayerness() {
			continuing = playerTurn();
		//}
	}
	return false; //return false because the player ran away
}
Battle::~Battle() {
	//delete npc copies
}