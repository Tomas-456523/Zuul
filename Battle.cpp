#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include "Battle.h"
#include "NPC.h"
#include "Item.h"

using namespace std;

//THIS IS IMPORTED FROM MAIN.CPP, IDK IF THERE'S A BETTER PRACTICE, MAYBE MAKE A CLASS FOR IT
////////////////////////////////////////////////////////////////////////////////
void CinIgnoreAll() {
	if (!cin) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
}

void AllCaps(char* text) {
	for (int i = 0; i < strlen(text); i++) {
		text[i] = toupper(text[i]);
	}
}

void ParseCommand(char* commandP, char* commandWordP, char* commandExtensionP) {
	int i = 0;
	while (commandP[i] != ' ' && commandP[i] != '\0') {
		commandWordP[i] = commandP[i];
		i++;
	}
	commandWordP[i] = '\0';
	i++;
	int j = i;
	while (commandP[i] != '\0') {
		commandExtensionP[i - j] = commandP[i];
		i++;
	}
	commandExtensionP[i - j] = '\0';
}

NPC* getNPCInVector(vector<NPC*> the_vector, char* npcname) {
	for (NPC* npc : the_vector) {
		if (!strcmp(npc->getName(), npcname)) {
			return npc;
		}
	}
	return NULL;
}

Item* getItemInVector(vector<Item*> the_vector, char* itemname) {
	for (Item* item : the_vector) {
		if (!strcmp(item->getName(), itemname)) {
			return item;
		}
	}
	return NULL;
}
//////////////////////////////////////////////////////////////////////////////////

Battle::Battle(vector<NPC*>* _playerTeam, vector<NPC*>* _enemyTeam, vector<Item*>* _inventory, int& mony, bool _escapable) {
	playerTeam = _playerTeam;
	enemyTeam = _enemyTeam;
	everyone.insert(everyone.begin(), playerTeam->begin(), playerTeam->end());
	everyone.insert(everyone.end(), enemyTeam->begin(), enemyTeam->end());
	inventory = _inventory;
	escapable = _escapable;
	for (NPC* npc : *playerTeam) {
		if (npc->getPlayerness()) {
			player = npc;
			break;
		}
	}
	FIGHT();
}
void Battle::printTeam(vector<NPC*>* team) {
	for (NPC* npc : *team) {
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
void Battle::FIGHT() {
	bool continuing = true;
	while (continuing) {
		//get current npc
		//npc turn
		//if npc.getPlayerness() {
			continuing = playerTurn();
		//}
	}
}
Battle::~Battle() {

}