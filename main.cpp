/* Tomas Carranza Echaniz
*  This program is going to be the most peak Zuul that has ever zuuled
* 
*/

/*
FEATURES I NEED TO IMPLEMENT
- DIALOGUE
- NPCS
- TURN-BASED COMBAT
- SAVE SYSTEM (OOOONNNNNNLLLLLYYY IF YOU HAVE TIME)
- DYNAMIC HELP SYSTEM?
- ITEMS
- FINALIZE THE PLOT
- ACHIEVEMENTS
- SOME SORT OF SHOPPING SYSTEM
*/
/*
If you get the lame ending it gives a reference to the "Don't be lame clause"
says:
Wow, that was lame...
Maybe you should explore more to get a less lame ending...

	    <<< BURGER QUEST COMPLETE ? >>>    
		<<< THANK YOU FOR PLAYING ! >>>

ENDING ACHIEVED: LAME ENDING

... and then you get booted to the main menu like in hollow knight
*/
#include <iostream>
#include <limits>
#include <vector>
#include <cstring>
#include <algorithm>
#include "NPC.h"
#include "Room.h"
#include "Item.h"
using namespace std;

//sets up the entire game world, including rooms, npcs, and items, and returns the starting room
NPC* SetupWorld(vector<Room*>* rooms) {
	//set up directions
	char* NORTH = new char[6];
	char* SOUTH = new char[6];
	char* WEST = new char[6];
	char* EAST = new char[6];

	strcpy(NORTH, "NORTH");
	strcpy(SOUTH, "SOUTH");
	strcpy(WEST, "WEST");
	strcpy(EAST, "EAST");

	//for copy paste DELETE THIS LATER PLEASE: Room* village = new Room("");
	Room* village = new Room("in Tactical Tent Village. It's a beautiful day; perfect for staying indoors and gaming.");
	Room* docks = new Room("at the village docks. Nobody owns a boat; why do we have this.");
	//please raname the goofy forest before submitting
	Room* forest = new Room("at the entrance of the [goofy] forest. Smells like pine trees along the way.");

	Room* casino = new Room("in a huge casino. You should really leave before you develop a gambling addiction.");

	Room* BURGERRESTAURANT = new Room("in the BURGER RESTAURANT. The BURGER MAN is waiting for you to order a BURGER.");

	NPC* self = new NPC("\0", "SELF", "It's a me.", village, 20, 5, 6, 0, 0, 10, true);
	self->setDialogue("Huh?");

	NPC* archie = new NPC("VILLAGE ELDER", "ARCHIE", "The elder of Tactical Tent Village. He stands there all day and night like a statue.", village, 1, 0, 1, 0, 0, 0);
	//REPLACE AREA WITH WHEREVER THE BURGER HERETIC WILL BE. MAKE ABSOLUTELY CERTAIN YOU DON'T LEAVE THAT IN
	archie->setDialogue("So I hear you are going on a BURGER QUEST? I would advise you to not recieve the BURGER; many are fooled and do not realize the great evil resonating in it. I would implore you to instead seek out my friend in the [AREA]; he can help you to annihilate the source.");
	archie->setRejectionDialogue("I am sorry. I cannot join you on your BURGER QUEST. I must stay here and watch over the village, for my recruitable variable is set to false.");
	
	//REPLACE PLACEHOLDER STATS
	NPC* graham = new NPC("GAMBLER", "GRAHAM", "A sorry gambling addict who is trillions in debt. He'll pay it off as soon as he wins; any day now.", casino, 0, 0, 0, 0, 0, 0);
	graham->setDialogue("What's that? I should quit gambling? Haven't you heard that 99% of gamblers quit just before hitting it big?");


	NPC* burgerman = new NPC("", "BURGER MAN", "The BURGER MAN in charge of this particular BURGER RESTAURANT. He has a BURGER for a head and an uncanny stature.", BURGERRESTAURANT, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647);
	
	Item* object = new Item("OBJECT", "Unidentifiable object whose only purpose is testing items.", village);

	Item* thing = new Item("THING", "Unidentifiable thing whose only purpose is testing items.", docks);
	Item* thingy = new Item("THINGY", "Unidentifiable thingy whose only purpose is testing items.", docks);

	Item* thingamajig = new Item("THINGAMAJIG", "Unidentifiable thingamajig whose only purpose is testing items.", forest);
	Item* thingamajiggy = new Item("THINGAMAJIGGY", "Unidentifiable thingamajiggy whose only purpose is testing items.", forest);
	Item* something = new Item("SOMETHING", "Unidentifiable something whose only purpose is testing items.", forest);

	//set up room exits
	village->setExit(SOUTH, docks);
	village->setExit(EAST, forest);
	docks->setExit(NORTH, village);
	forest->setExit(WEST, village);

	return self; //returns the player character
}

void PrintRoomData(Room* currentRoom) {
	cout << "\nYou are " << currentRoom->getDescription();
	currentRoom->printExits();
	currentRoom->printNPCs();
	currentRoom->printItems();
}

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
		commandExtensionP[i-j] = commandP[i];
		i++;
	}
	commandExtensionP[i-j] = '\0';
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

void travel(Room*& currentRoom, char* direction, vector<NPC*>* party) {
	if (strcmp(direction, "NORTH") && strcmp(direction, "SOUTH") && strcmp(direction, "WEST") && strcmp(direction, "EAST")) {
		cout << "\nInvalid direction \"" << direction << "\".";
		return;
	}
	Room* roomCanidate = currentRoom->getExit(direction);
	if (roomCanidate == NULL) {
		cout << "\nThere is no exit in that direction.";
		return;
	}
	currentRoom = roomCanidate;
	for (NPC* npc : (*party)) {
		npc->setRoom(currentRoom);
	}
	PrintRoomData(currentRoom);
}

void takeItem(Room* currentRoom, vector<Item*>* inventory, char* itemname) {
	Item* item = getItemInVector(currentRoom->getItems(), itemname);
	if (item == NULL) {
		cout << "\nThere is no \"" << itemname << "\" here.";
		return;
	}
	currentRoom->removeItem(item);
	inventory->push_back(item);
	cout << "\nYou took the " << itemname << ".";
}

void dropItem(Room* currentRoom, vector<Item*>* inventory, char* itemname) {
	Item* item = getItemInVector(*inventory, itemname);
	if (item == NULL) {
		cout << "\nYou have no \"" << itemname << "\"."; //I know it's grammatically inaccurate but it looks way better
		return;
	}
	currentRoom->setItem(item);
	inventory->erase(remove(inventory->begin(), inventory->end(), item), inventory->end());
	cout << "\nYou dropped the " << itemname << ".";
}

void useItem(Room* currentRoom, vector<Item*>* inventory, char* itemname) {
	//do stuff with the item
}

void recruitNPC(Room* currentRoom, char* npcname, vector<NPC*>* party) {
	NPC* npc = getNPCInVector(currentRoom->getNpcs(), npcname);
	if (npc == NULL) {
		cout << "\nThere is nobody named \"" << npcname << "\" here.";
		return;
	}
	if (npc->getPlayerness()) {
		cout << "\nSELF - \"Huh?\"\n\nYou are already in your own party? ...";
		return;
	}
	if (!npc->getRecruitable()) {
		cout << "\n" << npcname << " - \"" << npc->getRejectionDialogue() << "\"";
		cout << "\n" << npcname << " was not added to your party.";
		return;
	}
	if (npc->getRecruited()) {
		cout << "\n" << npcname << " is already in your party...";
		return;
	}
	party->push_back(npc);
	cout << "\n" << npcname << " - \"" << npc->getRecruitmentDialogue() << "\"";
	cout << "\n" << npcname << " was added to your party!";
}

void dismissNPC(Room* currentRoom, char* npcname, vector<NPC*>* party) {
	NPC* npc = getNPCInVector(currentRoom->getNpcs(), npcname);
	if (npc == NULL) {
		cout << "\nThere is nobody named \"" << npcname << "\" here.";
		return;
	}
	if (npc->getPlayerness()) {
		cout << "\nSELF - \"Huh? You can't dismiss me bro I'm the main character!\"\nSELF was not dismissed.";
		return;
	}
	if (!npc->getRecruited()) {
		cout << "\n" << npcname << " is not in your party...";
		return;
	}
	party->erase(remove(party->begin(), party->end(), npc), party->end());
	cout << "\n" << npcname << " - \"" << npc->getDismissalDialogue() << "\"";
	cout << "\n" << npcname << " was removed from your party and returned to what they were doing before.";
}

void printNPCDialogue(Room* currentRoom, char* npcname) {
	NPC* npc = getNPCInVector(currentRoom->getNpcs(), npcname);
	if (npc == NULL) {
		cout << "\nThere is nobody named \"" << npcname << "\" here.";
		return;
	}
	cout << "\n" << npcname << " - \"" << npc->getDialogue() << "\"";
}

void printInventory(vector<Item*>* inventory) {
	if (inventory->size() < 1) {
		cout << "\nYou have no items!";
		return;
	}
	cout << "\nYour items are:";
	for (Item* item : *inventory) {
		cout << "\n" << item->getName()/* << " - " << item->getDescription()*/;
	}
}

void printParty(vector<NPC*>* party) {
	cout << "\nMembers of your party:";
	for (NPC* npc : *party) {
		cout << "\n" << npc->getTitle();
		if (strlen(npc->getTitle()) > 0) {
			cout << " ";
		}
		cout << npc->getName();
	}
}

void printNPCData(NPC* npc) {
	cout << "\n" << npc->getTitle();
	if (strlen(npc->getTitle()) > 0) {
		cout << " ";
	}
	cout << npc->getName() << " - " << npc->getDescription();
	cout << "\n  HEALTH - " << npc->getHealthMax();
	cout << "\t  DEFENSE - " << npc->getDefense();
	cout << "\n  ATTACK - " << npc->getAttack();
	cout << "\t  TOUGHNESS - " << npc->getToughness();
	cout << "\n  PIERCE - " << npc->getPierce();
	cout << "\t  SPEED - " << npc->getSpeed();
}

void printItemData(Item* item) {
	cout << "\n" << item->getName() << " - " << item->getDescription();
}

void analyze(Room* currentRoom, char* name, vector<NPC*>* party, vector<Item*>* inventory) {
	NPC* npc = getNPCInVector(currentRoom->getNpcs(), name);
	if (npc == NULL) {
		npc = getNPCInVector(*party, name);
	}
	if (npc != NULL) {
		printNPCData(npc);
		return;
	}
	Item* item = getItemInVector(currentRoom->getItems(), name);
	if (item == NULL) {
		item = getItemInVector(*inventory, name);
	}
	if (item != NULL) {
		printItemData(item);
		return;
	}
	cout << "\nThere is no item or person named \"" << name << "\" here.";
}

void printHelp(char validCommands[13][255], char validExtensions[13][255], char flavorText[16][255]) {
	cout << "\n";
	srand(time(NULL));
	cout << flavorText[rand() % 15];
	cout << "\nValid commands:";
	for (int i = 0; i < 12; i++) {
		cout << "\n" << validCommands[i] << " " << validExtensions[i];
	}
}

int main() {
	vector<Room*>* rooms = new vector<Room*>;
	
	//sets up the game world and places the player at the current room
	NPC* self = SetupWorld(rooms);
	Room* currentRoom = self->getHome();

	vector<Item*>* inventory = new vector<Item*>;
	vector<NPC*>* party = new vector<NPC*>;

	party->push_back(self);

	//command stuff used to be initialized here

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

	char validCommands[13][255] = {
		"GO",
		"TAKE",
		"DROP",
		"USE",
		"RECRUIT",
		"DISMISS",
		"ASK",
		"INVENTORY",
		"PARTY",
		"ANALYZE",
		"FIGHT",
		"HELP",
		"QUIT"		
	};

	char validExtensions[13][255] = {
		"[direction]",
		"[item]",
		"[item]",
		"[item]",
		"[npc]",
		"[npc]",
		"[npc]",
		"",
		"",
		"[npc/item]",
		"[npc]",
		"",
		""
	};

	cout << "Welcome to BURGER QUEST 2: ELECTRIC BOOGALOO\nYou're going on a quest to get a BURGER.\nType HELP for help.\n";
	PrintRoomData(currentRoom);

	//player should name him/herself here

	bool continuing = true;
	while (continuing) {
		//im not sure if I should initialize these variables here, but it makes it so they're cleared every time which is pretty convenient
		char command[255] = "";

		char commandWord[255];
		char commandExtension[255];

		cout << "\n> ";
		cin.getline(command, 255);
		AllCaps(command);

		ParseCommand(command, commandWord, commandExtension);

		if (!strcmp(commandWord, "GO")) {
			travel(currentRoom, &commandExtension[0], party);
		} else if (!strcmp(commandWord, "TAKE")) {
			takeItem(currentRoom, inventory, &commandExtension[0]);
		} else if (!strcmp(commandWord, "DROP")) {
			dropItem(currentRoom, inventory, &commandExtension[0]);
		} else if (!strcmp(commandWord, "USE")) {
			useItem(currentRoom, inventory, &commandExtension[0]);
		} else if (!strcmp(commandWord, "RECRUIT")) {
			recruitNPC(currentRoom, &commandExtension[0], party);
		} else if (!strcmp(commandWord, "DISMISS")) {
			dismissNPC(currentRoom, &commandExtension[0], party);
		} else if (!strcmp(commandWord, "ASK")) {
			printNPCDialogue(currentRoom, &commandExtension[0]);
		} else if (!strcmp(commandWord, "INVENTORY")) {
			printInventory(inventory);
		} else if (!strcmp(commandWord, "PARTY")) {
			printParty(party);
		} else if (!strcmp(commandWord, "ANALYZE")) {
			analyze(currentRoom, &commandExtension[0], party, inventory);
		} else if (!strcmp(commandWord, "FIGHT")) {

		} else if (!strcmp(commandWord, "HELP")) {
			printHelp(validCommands, validExtensions, flavorText);
		} else if (!strcmp(commandWord, "QUIT")) {
			continuing = false;
		} else {
			cout << "\nInvalid command \"" << commandWord << "\" (type HELP for help).";
		}

		CinIgnoreAll();
	}
	//gives a friendly farewell to the player
	cout << "\nEnjoy your next 24 hours.";
}
