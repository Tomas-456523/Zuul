#include "Helper.h"
#include <cstring>
#include <iostream>
#include <limits>
#include <algorithm>
using namespace std;

namespace Helper {
	void CinIgnoreAll() {
		if (!cin) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}
	void CinPause() { //pauses until the user inputs enter, and also ignores any characters inputted meanwhile
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	//takes a pointer to a char array as input, and capitalizes all the characters in it. Used for easier string comparison.
	void AllCaps(char* text) {
		for (int i = 0; i < strlen(text); i++) {
			text[i] = toupper(text[i]);
		}
	}
	//takes commandP, puts the first word into commandWordP, and the rest into commandExtensionP. some attacks used in Battle may be >1 word, so there's also an option to skip a certain amount of spaces
	void ParseCommand(char* commandP, char* commandWordP, char* commandExtensionP, int skipSpaces) {
		int i = 0;
		while ((skipSpaces > 0 || commandP[i] != ' ') && commandP[i] != '\0') {
			commandWordP[i] = commandP[i];
			if (commandP[i] == ' ') {
				skipSpaces--;
			}
			i++;
		}
		commandWordP[i] = '\0';
		if (commandP[i] == '\0') {
			strcpy(commandExtensionP,"");
			return;
		}
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
			if (!npc->getDefeated() && !strcmp(npc->getName(), npcname)) {
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
	void printNPCData(NPC* npc) {
		cout << "\n" << npc->getTitle();
		if (strlen(npc->getTitle()) > 0) {
			cout << " ";
		}
		cout << npc->getName() << " - " << npc->getDescription() << "\n  LEVEL " << npc->getLevel(); //if recruited should say something like LEVEL 30 (1932874 xp to LEVEL UP)
		if (npc->getRecruited()) {
			cout << " (" << npc->xpForNextLevel() << " xp to LEVEL UP)";
		}
		npc->printEffects();
		cout << "\n  HEALTH - " << npc->getHealthMax();
		cout << "\t  DEFENSE - " << npc->getDefense();
		cout << "\n  ATTACK - " << npc->getAttack();
		cout << "\t  TOUGHNESS - " << npc->getToughness();
		cout << "\n  PIERCE - " << npc->getPierce();
		cout << "\t  SPEED - " << npc->getSpeed();
		cout << "\n  SKILL - " << npc->getSPMax();
	}

	void printItemData(Item* item) {
		cout << "\n" << item->getName() << " - " << item->getDescription();
		if (!item->getTakable()) {
			cout << " Costs " << item->getPrice() << " mon";
			if (item->getPrice() == 1) {
				cout << "y.";
			} else {
				cout << "ies.";
			}
		}
	}
	int Clamp(int num, int min, int max) {
		if (num < min) {
			return min;
		}
		if (num > max) {
			return max;
		}
		return num;
	}
	float ClampF(float num, float min, float max) {
		if (num < min) {
			return min;
		}
		if (num > max) {
			return max;
		}
		return num;
	}
	//sorts vector of npcs from fastest to slowest
	bool speedCompare(NPC* a, NPC* b) {
		return a->getSpeed() > b->getSpeed();
	}
	void sortBySpeed(vector<NPC*>& team) {
		sort(team.begin(), team.end(), speedCompare);
	}
	//get how many npcs in a team have at least 1 hp
	int aliveCount(vector<NPC*> team) {
		int i = 0;
		for (NPC* npc : team) {
			if (npc->getHealth() > 0) {
				i++;
			}
		}
		return i;
	}
	map<const char*, const char*> ReverseDirection;
}