/* Tomas Carranza Echaniz
*  5/14/26
*  This program is an epic text-based RPG where you can GO between rooms, TAKE, DROP, and USE items, ASK, RECRUIT,
*  DISMISS, and FIGHT npcs, and you're on a QUEST TO get a BURGER. There's a bunch of other commands as well.
*
*  This is the main file in which you start the game process after booting up the game.
*  
*  main.cpp handles the main menu for starting the game in Game! You start in the title screen and after pressing ENTER
*  can LOAD an existing save file or start a NEW GAME, and also print a list of their SAVES along with data pertaining
*  to the files in order to identify them. The player can also DELETE saves or EXPORT the save data of a file, printing
*  the data so the player can copy it and IMPORT it into the program on another system. The saves are stored in a global
*  vector which is stored in the Helper namespace, so that we can see the number of saves while saving the game.
*/

#include <iostream>
#include <iomanip>
#include <vector>
#include <cstring>
#include <algorithm>
#include <ctime>
#include <cctype>
#include "NPC.h"
#include "Room.h"
#include "Item.h"
#include "Battle.h"
#include "Helper.h"
#include "Attack.h"
#include "Effect.h"
#include "Conversation.h"
#include "Stats.h"
#include "Save.h"
#include "Game.h"

using namespace std;
using namespace Helper; //my Helper namespace has a bunch of helpful functions that I also use in other files

//print a list of all the player's saves MARK: print saves
void printSaves() {
	loadSaves(); //load the saves from the files to stay up to date
	if (saves.empty()) {
		cout << "\nYou don't have any saves; try starting a NEW GAME!";
		return;
	}
	cout << "\nSaves:";
	for (size_t i = 0; i < saves.size(); i++) {
		Save* save = saves[i];
		cout << "\nSAVE " << i+1 << " - ";
		if (!save->getValid()) cout << "Save data is faulty.";
		else if (save->getVersion() != '0') cout << "You need a newer version to play this save.";
		else {
			save->printMainString();
		}
	}
}

//make a new game and manage the NULL member of the saves vector if the player saved it MARK: new game
void newGame() {
	loadSaves(); //load the saves from the files to stay up to date
	saves.push_back(NULL); //push a null save so we can put the Save* there if we make one
	Save*& savey = saves.back(); //get a reference to the NULL thingy
	Game game = Game(savey); //make a new game with the new save
	game.play(); //play the game with the reference to the NULL save
	if (!savey) saves.pop_back(); //if we didn't save before quitting, we get rid of the NULL thingy
	//we make a file in saveWorld(), so it will be caught by subsequent loadSaves()s
}

//try to interpret what the player typed as a file number MARK: interpret file num
int getFileNum(const char* savename) {
	if (!strncmp(savename, "SAVE ", 5)) savename += 5; //ignore the "SAVE " so we just read the number
	long long num = 0; //build up the number here, long long so we can detect > INT_MAX instead of just overflowing and doing something weird
	bool foundnum = false; //if we found a number so we can differentiate between a "we did nothing" 0 and "the player typed 0" 0
	for (; isdigit((unsigned char)*savename); savename++) { //keeps going until we run out of numbers
		num = num*10 + (*savename-'0'); //move the number digits forward
		foundnum = true; //we found a number! because the thing went at all
		if (num > 2147483647) return -1; //we went past the int limit, this is definitely not a valid save number so return error number
	}
	if (!foundnum || *savename != '\0') return -1; //we return an error number because we either didn't find any numbers or there was stuff after the number(s)
	return num; //return the number we found!
}

//load an existing save MARK: load save
void loadSave(const char* savename) {
	loadSaves(); //load the saves from the files to stay up to date
	if (saves.empty()) { //error because nothing the player could possibly try would load a nonexistant save
		cout << "\nYou don't have any saves to load; try starting a NEW GAME!";
		return;
	} if (!strcmp(savename, "")) { //player said to load but didn't say what
		cout << "\nLoad what?";
		return;
	}
	int num = getFileNum(savename); //get the save file the player is trying to access
	if (num < 0) { //<0 num means error number, so we print that the input was bad + instructions
		cout << "\nInvalid save name \"" << savename << "\". (Must be SAVE [number] or just [number])";
		return;
	} if (!num) { //there is no save 0, so clarify that in case it wasn't clear
		cout << "\nThere's no save 0.";
		return;
	} if (num > saves.size()) { //let the player know they are going over the amount they can choose from
		cout << "\nYou don't have that many saves; there's only " << saves.size() << ".";
		return;
	} if (!saves[num-1]->getValid()) { //bad data so no play
		cout << "\nThis save's data is faulty and cannot be played.";
		return;
	} if (saves[num-1]->getVersion() != '0') { //we be too outdated, no play
		cout << "\nThis save is for a later version of the game. You need to update to a newer version to play it.";
		return;
	} //valid save number! play the game with the save we found!
	Game game = Game(saves[num-1]); //make a game instance with the save, decrement since we start at 1 and not 0
	game.play(); //play the game!
}

//delete one of the saves and rearrange the files accordingly MARK: delete save
void deleteSave(const char* savename) {
	loadSaves(); //load the saves from the files to stay up to date
	if (saves.empty()) { //error because there's nothing to delete
		cout << "\nYou don't have any saves to delete.";
		return;
	} if (!strcmp(savename, "")) { //player said to delete but didn't say what
		cout << "\nDelete what?";
		return;
	}
	int num = getFileNum(savename); //get the save file the player is trying to delete
	if (num < 0) { //<0 num means error number, so we print that the input was bad + instructions
		cout << "\nInvalid save name \"" << savename << "\". (Must be SAVE [number] or just [number])";
		return;
	} if (!num) { //there is no save 0, so clarify that in case it wasn't clear
		cout << "\nThere's no save 0.";
		return;
	} if (num > saves.size()) { //let the player know they are going over the amount they can choose from
		cout << "\nYou don't have that many saves; there's only " << saves.size() << ".";
		return;
	}
	long long seconds = saves[num-1]->getPlaytime();
	long long minutes = seconds/60; //use minutes if possible, precise reports of game time
	seconds %= 60; //get rid of the extra seconds
	long long hours = minutes/60; //go further to hours if possible
	minutes %= 60; //get rid of the extra minutes
	cout << "\nYou have ";
	if (hours) cout << hours << "h ";
	if (minutes || hours) cout << minutes << "m ";
	cout << seconds << "s of playtime on SAVE " << num << "\nAre you sure you want to delete it? (YES or NO)";
	if (!AOrB(NULL, "YES", "NO")) { //make sure the player is really sure about this because it's a very destructive action
		cout << "\nAlrighty then; did not delete SAVE " << num << ".";
		return;
	} //valid save number, so start deleting it
	char filename[255];
	snprintf(filename, 255, "save%d.bq2", num); //get what the name of the file should be
	if (remove(filename)) { //try to delete the file from the system
		cout << "\nFailed to delete the file \"save" << num << ".bq2\"."; //give error message to give the player a heads up
		return; //and return cause we couldn't delete it so we're not sure what to do now
	} //rename all the other files, so that the program can recognize them
	cout << "\nSuccessfully deleted SAVE " << num << "!"; //delete the file corresponding to the save, then success text!
	int total = saves.size();
	char newname[255]; //we use this to rename the files after the deleted one
	for (int i = num+1; i <= total; i++) { //start with the file after the one we just deleted and go until we've renamed the last file which should correspond with the size of the saves vector
		snprintf(filename, 255, "save%d.bq2", i); //decrement their number
		snprintf(newname, 255, "save%d.bq2", i-1);
		if (rename(filename, newname)) { //try to rename the file, and do error stuff if it fails
			CinPause(); //pause and give these instructions if something went wrong
			cout << "\nFailed to rename the file \"save" << i << ".bq2\".\nThe game may not recognize some files; ensure there is no gap in the files' numbers by renaming them.\n(i.e. There must be a save" << i-1 << ".bq2 for the game to recognize save" << i << ".bq2 and onward)";
			return; //return because there was an issue so yeah
		}
	}
	//don't modify the internal saves because they get reloaded on every command anyway
}

//import save data and try to make a save file for it MARK: import save
void importSave(const char* savedata) {
	if (!strcmp(savedata, "")) { //player said to import but didn't say what
		cout << "\nImport what?";
		return;
	}
	//get rid of leading whitespace (I thought it would be fair just for this command, since you might accidentally copy some whitespace along with the text). Trailing whitespace is handled by the save constructor
	while (isspace((unsigned char)*savedata)) savedata++;
	Save savey = Save(savedata, 0);
	if (!savey.getValid()) {
		cout << "\nThis save data is faulty and cannot be imported. Make sure your pasted text starts with \"BQ2\" and ends with \"=\".";
		return;
	}

	loadSaves(); //load the saves from the files to stay up to date
	char filename[255]; //get the name of the new file
	snprintf(filename, 255, "save%zu.bq2", saves.size()+1);
	ofstream newfile(filename); //make the new file and write into it the new data
	if (!newfile) { //print error if something went wrong
		cout << "\nFailed to create new save file.";
		return;
	}
	newfile << savedata;
	cout << "\nSuccessfully imported your save data into SAVE " << saves.size()+1 << "!"; //success text!
	//the new save will be added to the vector at the start of each function via their reload
	if (savey.getVersion() !='0') cout << " (Save data is from a future version; you must update to play on it.)";
}

//export save data in a copy-pastable way MARK: export save
void exportSave(const char* savename) {
	loadSaves(); //load the saves from the files to stay up to date
	if (saves.empty()) { //error because there's nothing to export
		cout << "\nYou don't have any saves to export.";
		return;
	} if (!strcmp(savename, "")) { //player said to export but didn't say what
		cout << "\nExport what?";
		return;
	}
	int num = getFileNum(savename); //get the save file the player is trying to export
	if (num < 0) { //<0 num means error number, so we print that the input was bad + instructions
		cout << "\nInvalid save name \"" << savename << "\". (Must be SAVE [number] or just [number])";
		return;
	} if (!num) { //there is no save 0, so clarify that in case it wasn't clear
		cout << "\nThere's no save 0.";
		return;
	} if (num > saves.size()) { //let the player know they are going over the amount they can choose from
		cout << "\nYou don't have that many saves; there's only " << saves.size() << ".";
		return;
	}
	printSave(saves[num-1]->data); //print the data to export it
}

//the title screen! MARK: main (title screen)
int main() {
	srand(time(NULL)); //seeds random

	//a list of the valid commands (and extensions) to be printed by printHelp
	const char* validCommands[8] = {
		"NEW GAME",
		"LOAD [save file]",
		"SAVES",
		"DELETE [save file]",
		"IMPORT [save data]",
		"EXPORT [save file]",
		"HELP",
		"EXIT"
	};

	cout << "\n" << getTitle() << "\nPress ENTER to begin." << fixed << setprecision(2);

	CinPause();

	loadSaves(); //get all the player's saves

	cout << "\nType HELP for help.\nWhat would you like to do?"; //beginning prompt
	
	bool promptline = true;
	bool continuing = true; //we continue until the player quits
	while (continuing) { //the main loop!
		char command[10000] = ""; //the charray that the player inputs into

		char commandWord[10000]; //the first word of the player input (the command)
		char commandExtension[10000]; //the rest of the player's command (minus the space)

		if (promptline) cout << "\n";
		cout << "> "; //The > signifies it's time to type in a command. If there is no >, it's a cutscene or dialogue or something like that and you just have to ENTER until you get to the >.
		cin.getline(command, 10000);

		promptline = true; //make sure the next > will probably be in a new line

		ParseCommand(command, commandWord, commandExtension); //seperates the command into the command and the extension

		AllCaps(commandWord); //capitalizes the command for easier parsing
		if (strcmp(commandWord, "IMPORT")) { //do not capitalize the imported string because that messes up everything
			AllCaps(command);
			AllCaps(commandExtension);
		}

		if (!strcmp(commandWord, "SAVES")) { //for printing out all the save files
			printSaves();
		} else if (!strcmp(command, "NEW GAME")) { //for starting a new save file
			newGame();
		} else if (!strcmp(commandWord, "LOAD")) { //for loading an existing save file
			loadSave(commandExtension);
		} else if (!strcmp(commandWord, "DELETE")) { //for deleting a file
			deleteSave(commandExtension);
		} else if (!strcmp(commandWord, "IMPORT")) { //for importing save data from elsewhere (this and export are in case I ever put this on a website or something where you can't access the files easily)
			importSave(commandExtension);
		} else if (!strcmp(commandWord, "EXPORT")) { //for exporting save data in a copy-pastable way
			exportSave(commandExtension);
		} else if (!strcmp(commandWord, "HELP")) { //for asking what the valid commands are
			printHelp(validCommands, NULL, 8, 0);
		} else if (!strcmp(commandWord, "EXIT")) { //for exiting the game
			continuing = false;
		} else if (!strlen(command)) { //don't print error if the player just entered nothing
			promptline = false;
		} else { //prints an error message if the player typed something that isn't an actual command
			cout << "\nInvalid command \"" << commandWord << "\" (type HELP for help).";
		}

		CinIgnoreAll(); //clears extra or faulty input
	}

	//gives a friendly farewell to the player
	cout << "\nCya!\n";

	//delete all the saves because they are allocated data and their contents are saved as files anyway
	for (Save* save : saves) {
		delete save;
	}
}
