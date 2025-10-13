/* Tomas Carranza Echaniz
*  This program is going to be the most peak Zuul that has ever zuuled
* 
*/

/*
FEATURES I NEED TO IMPLEMENT
- ROOMS
- MOVING
- DIALOGUE
- NPCS
- TURN-BASED COMBAT
- SAVE SYSTEM (OOOONNNNNNLLLLLYYY IF YOU HAVE TIME)
- DYNAMIC HELP SYSTEM?
- ITEMS
- FINALIZE THE PLOT
- ACHIEVEMENTS
*/

/*
COMMANDS:
HELP
INVENTORY
PARTY
ANALYZE [item or npc, ideally they will not have overlapping names]
FIGHT [npc]
TALK TO [npc]
RECRUIT [npc]
TAKE [item]
USE [item]
DROP [item]
QUIT
*/
/*
If you get the lame ending it gives a reference to the "Don't be lame clause"
says:
Wow, that was lame...
Maybe you should explore more to get a less lame ending...

	    <<< BURGER QUEST COMPLETE ? >>>    
		<<< THANK YOU FOR PLAYING ! >>>

ENDING ACHIEVED: LAME ENDING

... and then you get booted to the main menu like in silksong
*/
#include <iostream>
#include <limits>
#include <vector>
#include <cstring>
#include "NPC.h"
#include "Room.h"
#include "Item.h"
using namespace std;

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

char* ParseCommand() {
	return NULL;
}

void printHelp(char validCommands[12][255], char validExtensions[12][255], char flavorText[16][255]) {
	cout << "\n";
	srand(time(NULL));
	cout << flavorText[rand() % 15];
	cout << "\nValid commands:";
	for (int i = 0; i < 12; i++) {
		cout << "\n" << validCommands[i] << " " << validExtensions[i];
	}
}

int main() {
	char command[255];
	char* commandP = &command[0];
	vector<Room*> rooms;

	char flavorText[16][255] = {
		"What even is a BURGER?",
		"You consider the state of the economy.",
		"You are blue dabadeedabadai.",
		"You are beginning to believe.",
		"You forgor.",
		"You accidentally find spoilers for the ending of BURGER QUEST 2. It is very concerning...",
		"You realize you don't have an oven.",
		"You spot a billboard advertising the new BURGER RESTAURANT. When you look away you realize you still don't know what a BURGER looks like.",
		"You ask a passerby what the valid commands are. The guy looks at you really confused.",
		"You stop it and get some help.",
		"We have been trying to reach you about your car's extended warranty.",
		"You spot a quick brown fox jumping over a lazy dog.",
		"You say hello to the world.",
		"You win the lottery. It's not a BURGER so who cares.",
		"You have a cake and eat it too.",
		"You take a potato chip... and eat it."
	};

	char validCommands[12][255] = {
		"GO",
		"TAKE",
		"DROP",
		"USE",
		"RECRUIT",
		"DISMISS",
		"ASK",
		"INVENTORY",
		"PARTY",
		"FIGHT",
		"HELP",
		"QUIT"		
	};

	char validExtensions[12][255] = {
		"[direction]",
		"[item]",
		"[item]",
		"[item]",
		"[npc]",
		"[npc]",
		"[npc]",
		"",
		"",
		"[npc]",
		"",
		""
	};

	cout << "Welcome to BURGER QUEST 2: ELECTRIC BOOGALOO\nYou're going on a quest to get a BURGER.";
	bool continuing = true;
	while (continuing) {
		cout << "\n> ";
		cin.getline(command, 255);

		AllCaps(commandP);
		//PARSE COMMAND

		if (!strcmp(command, "HELP")) {
			printHelp(validCommands, validExtensions, flavorText);
		} else if (!strcmp(command, "QUIT")) {
			continuing = false;
		}

		CinIgnoreAll();
	}
	//gives a friendly farewell to the player
	cout << "Enjoy your next 24 hours.";
}
