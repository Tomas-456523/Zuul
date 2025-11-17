/* Tomas Carranza Echaniz
*  This program is going to be the most peak Zuul that has ever zuuled
* 
*/

/*
FEATURES I NEED TO IMPLEMENT
- BETTER DIALOGUE
- TURN-BASED COMBAT
- SAVE SYSTEM (OOOONNNNNNLLLLLYYY IF YOU HAVE TIME)
- DYNAMIC HELP SYSTEM?
- ITEMS
- FINALIZE THE PLOT (almost done)
- ACHIEVEMENTS (ehhhhhh idk anymore consider it if time permits)
- IMPROVE SHOPPING SYSTEM
- QUEST SYSTEM (like, to make npcs recruitable)
- FISHING MINIGAME
- BLOCKED EXITS
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
#include <vector>
#include <cstring>
#include <algorithm>
#include "NPC.h"
#include "Room.h"
#include "Item.h"
#include "Battle.h"
#include "Helper.h"
#include "Attack.h"
#include "Effect.h"

using namespace std;
using namespace Helper;

//sets up the entire game world, including rooms, npcs, and items, and returns the starting room
NPC* SetupWorld(vector<Room*>* rooms) {
	//set up directions
	char* NORTH = new char[12];
	char* SOUTH = new char[12];
	char* WEST = new char[12];
	char* EAST = new char[12];
	char* NORTHWEST = new char[12];
	char* NORTHEAST = new char[12];
	char* SOUTHWEST = new char[12];
	char* SOUTHEAST = new char[12];
	//unique directions
	char* OUT = new char[12];
	char* IN_TENT_1 = new char[12];
	char* IN_TENT_2 = new char[12];
	char* IN_TENT_3 = new char[12];
	char* IN_TENT = new char[12];
	char* UPSTAIRS = new char[12];
	char* DOWNSTAIRS = new char[12];

	strcpy(NORTH, "NORTH");
	strcpy(SOUTH, "SOUTH");
	strcpy(WEST, "WEST");
	strcpy(EAST, "EAST");

	strcpy(NORTHWEST, "NORTHWEST");
	strcpy(NORTHEAST, "NORTHEAST");
	strcpy(SOUTHWEST, "SOUTHWEST");
	strcpy(SOUTHEAST, "SOUTHEAST");

	strcpy(OUT, "OUT");
	strcpy(IN_TENT_1, "IN TENT 1");
	strcpy(IN_TENT_2, "IN TENT 2");
	strcpy(IN_TENT_3, "IN TENT 3");
	strcpy(IN_TENT, "IN TENT");
	strcpy(UPSTAIRS, "UPSTAIRS");
	strcpy(DOWNSTAIRS, "DOWNSTAIRS");

	//set up blockage reaons
	char* ENEMY = new char[12];
	char* CHASM = new char[12];
	char* RIVER = new char[12];

	strcpy(ENEMY, "ENEMY");
	strcpy(CHASM, "CHASM");
	strcpy(RIVER, "RIVER");

	//I send all the template enemy NPCs to limbo, since I need to set a room for them
	Room* limbo = new Room("not supposed to be in this room; seriously how did you get here?");

	//for copy paste DELETE THIS LATER PLEASE: Room* village = new Room("");
	Room* village = new Room("in Tactical Tent Village. It's a beautiful day; perfect for staying indoors and gaming.");
	Room* villageleft = new Room("at the westernmost end of the village, where the tallest tent stands. It's only two stories, but it's comparatively a tent mansion.");
	Room* tenthome = new Room("in the developer's house.");
	Room* tentstore = new Room("in the village convenience store. No other store is more convenient, or so they say.");
	Room* tentmansion = new Room("in the tent mansion's living room. There are way too many clocks here.");
	Room* tentlab = new Room("in tent lab. [flavor text floavor text flavor text]");
	//tunnels are shut down due to a lobster infestation
	Room* tentstation = new Room("in the village train station. The tunnels were closed off recently for [SOME REASON].");

	Room* casino = new Room("in a casino. You should really leave before you develop a gambling addiction.");

	//put a fishing rod here and a fishing minigame :)
	Room* docks = new Room("at the village docks. Nobody owns a boat; why do we have this.");

	Room* forestentrance = new Room("at the entrance of the woodlands.");
	Room* forest = new Room("deep in the woodlands. Smells like pine trees along the way.");
	forest->setWelcome("WANING WOODLANDS", "THE FINAL FOREST", "The slowly decaying corner of\nthe world, reminiscent of the\ntimes before evil.");
	Room* forest2 = new Room("");

	Room* deerclearing = new Room("in the deer clearing, where deer frequently gather.");
	Room* foresttemple = new Room("in the temple of [SOMETHING]. [SOMETHING SOMETHING].");

	Room* BURGERRESTAURANT = new Room("in the BURGER RESTAURANT. The BURGER MAN is waiting for you to order a BURGER.");
	Room* BURGERPRISON = new Room("in the BURGER PRISON redacted from existence. There is one singular cell, holding one singular man.");

	NPC* self = new NPC("\0", "SELF", "It's a me.", village, 20, 5, 6, 0, 0, 10, 0, true, true);
	self->setDialogue("Huh?");
	self->Recruit();

	NPC* archie = new NPC("VILLAGE ELDER", "ARCHIE", "The elder of Tactical Tent Village. He stands there all day and night like a statue.", village, 1, 0, 1, 0, 0, 0, 50);
	archie->setDialogue("So you are going on a BURGER QUEST too, I hear? Just keep heading NORTH, and you'll soon reach BURGERSBURG. Safe travels, child!");
	archie->setRejectionDialogue("I am sorry. Though I would love to join you on your BURGER QUEST, I must stay here and watch over the village, for my recruitable variable is set to false.");

	//NPC* treeelder = new NPC("TREE ELDER", "TREE", "An ancient tree outdating BURGERs");
	
	//REPLACE PLACEHOLDER STATS
	NPC* graham = new NPC("GAMBLER", "GRAHAM", "A sorry gambling addict who is trillions in debt. He'll pay it off as soon as he wins; any day now.", casino, 30, 10, 5, 0, 2, 20, 2);
	graham->setDialogue("What's that? I should quit gambling? Haven't you heard that 99% of gamblers quit right before hitting it big?\"\nGAMBLING MACHINE - \"You lose 1000000 monies.\"\nGRAHAM - \"Aw dang it.");
	graham->setRejectionDialogue("Nah, sorry man. I'm just about to win the jackpot. I can feel it!\"\nGAMBLING MACHINE - \"You lose 1000000 monies.\"\nGRAHAM - \"Aw dang it.");

	NPC* burgerman = new NPC("", "BURGER MAN", "The manager of the BURGER RESTAURANT. He has a BURGER for a head and an uncanny stature.", BURGERRESTAURANT, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647);
	//NPC* henryjerry = new NPC("BURGER QUEST 1 PROTAGONIST", "HENRY JERRY", "The sleep-deprived protagonist from the first game who was used as a puppet of BURGER. He wears a formal suit and seems traumatized.", limbo, 10, 2, 4, 1, 0, 4, 5);

	NPC* burgerheretic = new NPC("BURGER HERETIC", "ARCHIBALD", "A shriveled man imprisoned for resisting the global domination of BURGER.", BURGERPRISON, 1000000, 5000000, 900000000, 100000, 0, 700000, 80000000);
	burgerheretic->setDialogue("I have not seen anybody in ages. [PLOT DEVICE QUEST INSTRUCTIONS]");
	burgerheretic->setRejectionDialogue("I would love to join you on your quest. But as long as the BURGER MENACE endures, so shall these bars you see in front of me.");

	//it's a me
	//I should sell the developer gun
	NPC* developer = new NPC("DEVELOPER", "TOMAS", "The guy who made the game, except not really that guy because yeah.", tenthome, 1, 0, 1, 0, 0, 0, 0);
	developer->setDialogue("Yo wassup.");
	developer->setRejectionDialogue("Nah, sorry. I don't think I would make a good teammate because I made my stats really low. I gotta stay humble, you know?");

	//maybe make a secret code for the thing so regular people can't use it
	Item* devgun = new Item("DEVELOPER GUN", "I made this in order to make playtesters' lives easier.", tenthome, false, 0);
	devgun->setDenial("TOMAS - No you have to buy that first. But that's for playtesters, so pls don't buy that if you aren't.");
	devgun->setBuyDesc("TOMAS - I trust that you are a playtester...");

	NPC* jimmyjohn = new NPC("SHOPKEEPER", "JIMMY JOHN", "The owner of the village convenience store. He has an italian accent.", tentstore, 500, 400, 99999, 800, 99999, 50, 30);
	jimmyjohn->setDialogue("Welcome to my convenience store! None is more convenient!");
	jimmyjohn->setRejectionDialogue("I'm sorry I cannot. Who will take care of my store?");
	//probably give the store actual stock here

	//set up room exits
	village->setExit(SOUTH, docks);
	village->setExit(EAST, forestentrance);
	village->setExit(WEST, villageleft);
	village->setExit(IN_TENT_1, tentstore);
	village->setExit(IN_TENT_2, tentstation);
	village->setExit(IN_TENT_3, tenthome);

	villageleft->setExit(EAST, village);
	villageleft->setExit(IN_TENT, tentmansion);

	tentmansion->setExit(OUT, villageleft);
	tentmansion->setExit(UPSTAIRS, tentlab);
	tentlab->setExit(DOWNSTAIRS, tentmansion);
	
	tentstore->setExit(OUT, village);
	tentstation->setExit(OUT, village);
	tenthome->setExit(OUT, village);
	docks->setExit(NORTH, village);

	forestentrance->setExit(WEST, village);
	forestentrance->setExit(NORTH, forest);
	forest->setExit(SOUTH, forestentrance);

	Room* tunnel = new Room("in the tunnels that span the continent.");

	NPC* tunnel_lobster = new NPC("", "TUNNEL LOBSTER", "An immense, savage crustacean who inhabits the tunnels below.", tunnel, 200, 20, 10, 20, 10, 50, 0, true);
	//you should also get a prompt to name it like
	//                                                     (type your lobster's name here!)
	//SELF - "Oh nice a pet lobster. I think I'll name you 
	//Successfully tamed TUNNEL LOBSTER [FLORIAN]!
	// 
	//and then set the lobster's title to TUNNEL LOBSTER and the name to whatever was typed
	// 
	//if you just enter nothing self says
	//SELF - "Ok nevermind I guess I won't name you."
	//Successfully tamed TUNNEL LOBSTER!
	// 
	//reset the description to tamed instead of savage
	tunnel_lobster->setDialogue("HHhHHHhhHHhHhhHhHHhHhHHh (lobster noises).");
	tunnel_lobster->setRejectionDialogue("HhhHhHhHhhhhHHhHHh (lobster noises probably meaning no).");

	//set up generic non-npc enemies
	NPC* prickly_hog = new NPC("", "PRICKLY HOG", "A small but ferocious hog with sharp prickles.", limbo, 10, 10, 5, 0, 20, 15);
	Attack* headbutt = new Attack("headbutted", -5, 5, 0, 1, 1, 1);
	Attack* homing_prickle = new Attack("launched homing prickles at", 5, 3, 5, 2, 4, 3);
	prickly_hog->setStandardAttack(headbutt);
	prickly_hog->addSpecialAttack(homing_prickle);

	NPC* greater_hog = new NPC("", "GREATER HOG", "A larger and more territorial hog with sharp prickles.", limbo, 20, 0, 0, 0, 0, 0);
	greater_hog->setStandardAttack(headbutt);
	greater_hog->addSpecialAttack(homing_prickle);

	//set up npc enemies
	NPC* forestguard = new NPC("", "GRASSMAN", "A really leafy humanoid who hates real humans.", forestentrance, 0, 0, 0, 0, 0, 0, 0, true);
	forestguard->setDialogue("(angry bush noises)");
	forestguard->setRejectionDialogue("(angry bush noises)");

	//forestentrance->blockExit(NORTH, ENEMY, "guarded by the GRASSMAN.");
	forestguard->blockExit(NORTH, ENEMY, "guarded by the GRASSMAN.");

	/*Fighter savage_hog = Fighter("SAVAGE HOG", "A towering hog elder with sharp prickles.", 10, 10, 5, 0, 20, 15);


	Fighter disease_amalgamate = Fighter("DISEASE AMALGAMATION", "A writhing mass of disease.", 500, 0, 80, 0, 999, 20);

	Fighter jim_shady = Fighter("JIM SHADY", "The real Jim Shady.", 50, 10, 99999, 10, 99999, 0);
	Fighter slim_jim = Fighter("SLIM JIM", "A Jim obsessed with athleticism and velocity.", 150, 0, 99999, 0, 99999, 99999); 
	//this guy should appear in groups
	Fighter jimmy = Fighter("JIMMY", "A small but deadly Jim.", 1, 0, 9999, 0, 9999, 40);
	Fighter jim = Fighter("JIM", "The one and only Jim.", 1000, 200, 999999, 400, 999999, 99999);*/

	//ENEMIES BLOCK EXITS, AND THEY RESPAWN AFTER YOU LEAVE THE ROOM, HOWEVER THE EXIT THEY WERE BLOCKING WILL NO LONGER BE BLOCKED
	//YOU COULD USE THIS MECHANIC BY SAYING HOW YOU GO IN A ROOM BUT THEN AN ENEMY IS BLOCKING THE WAY BACK LIKE THEY WERE CORNERING YOU OR SOMETHING
	//add a small random value to speed and attack each turn

	return self; //returns the player character
}

void PrintRoomData(Room* currentRoom) {
	currentRoom->printWelcome();
	cout << "\nYou are " << currentRoom->getDescription();
	currentRoom->printExits();
	currentRoom->printNPCs();
	currentRoom->printItems();
	currentRoom->printBlocks();
}

void travel(Room*& currentRoom, char* direction, vector<NPC*>* party) {
	Room* roomCanidate = currentRoom->getExit(direction);
	if (roomCanidate == NULL) {
		if (strcmp(direction, "NORTH") && strcmp(direction, "SOUTH") && strcmp(direction, "WEST") && strcmp(direction, "EAST")) {
			cout << "\nInvalid direction \"" << direction << "\".";
		} else {
			cout << "\nThere is no exit in that direction.";
		}
		return;
	} else if (currentRoom->getBlocked(direction)) {
		currentRoom->printBlock(direction);
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
	if (!item->getTakable()) {
		cout << "\n" << item->getDenial();
		return;
	}
	item->unRoom();
	inventory->push_back(item);
	cout << "\nYou took the " << itemname << ".";
}

void dropItem(Room* currentRoom, vector<Item*>* inventory, char* itemname) {
	Item* item = getItemInVector(*inventory, itemname);
	if (item == NULL) {
		cout << "\nYou have no \"" << itemname << "\"."; //I know it's grammatically inaccurate but it looks way better
		return;
	}
	item->setRoom(currentRoom);
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
		cout << "\n" << npcname << " - \"Huh?\"\n\nYou are already in your own party? ...";
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
		cout << "\n" << npcname << " - \"Huh? You can't dismiss me bro I'm the main character!\"\n" << npcname << " was not dismissed.";
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

void printInventory(vector<Item*>* inventory, int monies) {
	cout << "\nYou have " << monies << " monies, and you";
	if (inventory->size() < 1) {
		cout << " have no items!";
		return;
	}
	cout << "r items are:";
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
		cout << npc->getName() << " - LEVEL " << npc->getLevel();
	}
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
	cout << "\n  HEALTH - " << npc->getHealthMax();
	cout << "\t  DEFENSE - " << npc->getDefense();
	cout << "\n  ATTACK - " << npc->getAttack();
	cout << "\t  TOUGHNESS - " << npc->getToughness();
	cout << "\n  PIERCE - " << npc->getPierce();
	cout << "\t  SPEED - " << npc->getSpeed();
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

void buy(Room* currentRoom, vector<Item*>* inventory, char* name, int& mony) {
	Item* item = getItemInVector(currentRoom->getItems(), name);
	if (item == NULL) {
		Item* item = getItemInVector(*inventory, name);
		if (item != NULL) {
			cout << "\nYou already own this item!";
			return;
		}
		cout << "\nThere is no \"" << name << "\" here.";
		return;
	} else if (item->getTakable()) {
		cout << "\nNobody is selling the " << name << "; you can just take it.";
		return;
	}
	item->buy(mony, inventory);
}

void fight(Room* currentRoom, vector<NPC*>* party, vector<Item*>* inventory, char* name, int& mony) {
	NPC* npc = getNPCInVector(currentRoom->getNpcs(), name);
	if (npc == NULL) {
		cout << "\nThere is nobody named \"" << name << "\" here.";
		return;
	}
	if (npc->getPlayerness()) {
		cout << "\n" << name << " - \"Uhhhh you want me to fight myself?\"";
		return;
	}
	if (!npc->getLeader()) {
		cout << "\nYou can't fight " << name << "!";
		return;
	}

	//Battle battle = Battle(NULL, NULL, NULL, NULL, true);
	//bool victory = battle.FIGHT();
	//if (victory) {
	//	//add experience
	//}
	//some stuff
}

void printHelp(char validCommands[14][255], char flavorText[16][255]) {
	cout << "\n";
	srand(time(NULL));
	cout << flavorText[rand() % 16];
	cout << "\nValid commands:";
	for (int i = 0; i < 14; i++) {
		cout << "\n" << validCommands[i];
	}
}

int main() {
	vector<Room*>* rooms = new vector<Room*>;
	
	//sets up the game world and places the player at the current room
	NPC* self = SetupWorld(rooms);
	Room* currentRoom = self->getHome();

	vector<Item*>* inventory = new vector<Item*>;
	vector<NPC*>* party = self->getParty();

	//party->push_back(self);

	int mony = 0; //monies are the currency in the BURGER QUEST multiverse.

	//command stuff used to be initialized here

	char flavorText[16][255] = {
		"What even is a BURGER?",
		"You consider the state of the economy.",
		"You are blue dabadeedabadai.",
		"You are beginning to believe.",
		"You forgor.",
		"You accidentally find spoilers for the ending of BURGER QUEST 2. It is very concerning...",
		"You realize you don't have an oven.",
		"You spot a billboard advertising the new BURGER RESTAURANT in BURGERSBURG.",
		"You ask a passerby what the valid commands are. The guy looks at you really confused.",
		"You stop it and get some help.",
		"We have been trying to reach you about your car's extended warranty.",
		"You spot a quick brown fox jumping over a lazy dog.",
		"You say hello to the world.",
		"You win the lottery. It's not a BURGER so who cares.",
		"You have a cake and eat it too.",
		"You take a potato chip... and eat it."
	};

	char validCommands[14][255] = {
		"GO [direction]",
		"TAKE [item]",
		"DROP [item]",
		"USE [item] (ON [npc])",
		"RECRUIT [npc]",
		"DISMISS [npc]",
		"ASK [npc]",
		"INVENTORY",
		"PARTY",
		"ANALYZE [npc/item]",
		"FIGHT [npc]",
		"BUY [item]",
		"HELP",
		"QUIT"
	};

	cout << "Welcome to BURGER QUEST 2: ELECTRIC BOOGALOO\nYou're going on a quest to get a BURGER.\nType HELP for help.\n";

	cout << "\n             (type your name here!)\nYour name is ";

	char name[255];
	cin.getline(name, 255);

	AllCaps(&name[0]);

	if (!strcmp(name, "")) {
		cout << "\nSELF - \"Ok I guess I just don't have a name then.\"";
		CinPause();
	} else {
		self->setName(name);
	} 
	if (!strcmp(name, "BERNARD")) {
		cout << "\nBERNARD - \"Oh wow that's my actual name!\"";
		CinPause();
	} else if (!strcmp(name, "HELP")) {
		cout << "\nHELP - \"BRO are you serious? Now my name is Help... :(\"";
		CinPause();
	} else if (!strcmp(name, "HELP FOR HELP") || !strcmp(name, "YOUR NAME") || !strcmp(name, "YOUR NAME HERE") || !strcmp(name, "YOUR NAME HERE!")) {
		cout << "\n" << name << " - \"Well, you're very good at following instructions...\"";
		CinPause();
	//probably just make this an annoying string of text that you can only realistically copy/paste in
	} else if (!strcmp(name, "BAGEL ADMINISTRATOR")) {
		cout << "\nDeveloper mode activated!";
		CinPause();
	} //have devmode bool, and do additional command checks for DEFEAT [npc], UNLOCK [exit], FORCERECRUIT [npc], GETMONEY, etc.
	
	CinIgnoreAll();

	PrintRoomData(currentRoom);

	bool continuing = true;
	while (continuing) {
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
			printInventory(inventory, mony);
		} else if (!strcmp(commandWord, "PARTY")) {
			printParty(party);
		} else if (!strcmp(commandWord, "ANALYZE")) {
			analyze(currentRoom, &commandExtension[0], party, inventory);
		} else if (!strcmp(commandWord, "FIGHT")) {
			fight(currentRoom, party, inventory, &commandExtension[0], mony);
		} else if (!strcmp(commandWord, "BUY")) {
			buy(currentRoom, inventory, &commandExtension[0], mony);
		} else if (!strcmp(commandWord, "HELP")) {
			printHelp(validCommands, flavorText);
		} else if (!strcmp(commandWord, "QUIT")) {
			continuing = false;
		} else {
			cout << "\nInvalid command \"" << commandWord << "\" (type HELP for help).";
		}

		CinIgnoreAll();
	}
	//gives a friendly farewell to the player
	cout << "\nEnjoy your next 24 hours.\n";
}