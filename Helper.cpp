#include "Helper.h"
#include <cstring>
#include <iostream>
#include <limits>
using namespace std;

namespace Helper {
	void CinIgnoreAll() {
		if (!cin) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}
	//takes a pointer to a char array as input, and capitalizes all the characters in it. Used for easier string comparison.
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
}