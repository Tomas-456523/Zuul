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
- QUEST SYSTEM (like, to make npcs recruitable)
- FISHING MINIGAME
- RECAP COMMAND?
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

//sets up the entire game world, including rooms, npcs, and items, and returns the player character
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

	//set the text for the directions
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
	char* TEMPLE = new char[12];
	
	//set up blockage reason text
	strcpy(ENEMY, "ENEMY");
	strcpy(CHASM, "CHASM");
	strcpy(RIVER, "RIVER");
	strcpy(TEMPLE, "TEMPLE");

	//I send all the template enemy NPCs and also shop items, since I need to set a room for them
	Room* limbo = new Room("not supposed to be in this room; seriously how did you get here?");

	//for copy paste DELETE THIS LATER PLEASE: Room* village = new Room("");
	//create all WANING WOODLANDS rooms
	Room* village = new Room("in Tactical Tent Village. It's a beautiful day; perfect for staying indoors and gaming.");
	Room* villageleft = new Room("at the westernmost end of the village, where the tallest tent stands. It's only two stories, but it's comparatively a tent mansion.");
	Room* tentstore = new Room("in the village convenience store. No other store is more convenient, or so they say.");
	Room* tentmansion = new Room("in the tent mansion's living room. There are way too many clocks here.");
	//put a telescope here, and maybe a launchpad outside which may or may bit do something
	Room* tentlab = new Room("in the tent lab. There's a ton of machinery, and many generic science beakers with colored liquids.");
	//(put a sign here) tunnels are shut down due to a lobster infestation
	Room* tentstation = new Room("in the village train station. The tunnels were closed off recently for [SOME REASON].");
	//make the church get repaired post game and Archibald goes here
	Room* tentchurch = new Room("in the village church. Nobody really goes here anymore, especially since the priest disappeared a long time ago.");
	//put a fishing rod here and a fishing minigame :) NONONONNONNONONONONONONONONONNNNO THATS ENOUGH SCOPE
	Room* docks = new Room("at the village docks. Nobody owns a boat; why do we have this.");

	Room* forestentrance = new Room("at the entrance of the woodlands.");
	Room* forest = new Room("deep in the woodlands. Smells like pine trees along the way.");
	forest->setWelcome("Welcome to WANING WOODLANDS!", "<<< THE FINAL FOREST >>>", "The slowly decaying corner of the world, vaguely reminiscent of the times before evil.");
	
	Room* forestleft = new Room("walking down the left path of the woodlands. It feels like a deer is watching you.");
	Room* forestright = new Room("walking down the right path of the woodlands. It feels like a ninja is watching you.");

	Room* deerclearing = new Room("in the deer clearing, where deer frequently gather."); //talk to the deer to get the key
	Room* ninjaland = new Room("underneath the ninja village. It's probably supposed to be hidden but you looked up and there it was.");
	//make sure the desc gets updated after plot device quest
	Room* fortementrance = new Room("in the glade where the ancient forest temple stands. No matter what anyone has tried, nobody has ever made it in.");
	Room* foreststairs = new Room("on the steps that go into the ancient forest temple.");
	Room* foresttemple = new Room("in the temple of [SOMETHING]. [SOMETHING SOMETHING].");

	Room* flowerfield = new Room("in the aromatic flower fields. Your sister likes hanging out here.");
	Room* flowerfield2 = new Room("deep in the flower fields. A really nice river flows over here.");
	//fix this
	flowerfield2->setWelcome("FLORIA - Hey! Aren't these flowers just so lovely?","[main character] - NO THESE FLOWERS SUCK THEY TRIED TO EAT ME.","");

	Room* forcrossroad = new Room("at another fork in the road. I hope we don't come across a spoon in the road.");
	Item* fork = new Item("FORK","\"An implement with two or more prongs used for lifting food to the mouth or holding it when cutting.\"\n - Oxford Languages", forcrossroad);

	Room* garden = new Room("a strange section of the forest. All the trees have been neatly trimmed and organized to an unnatural extent.");

	Room* forestgate = new Room("at the border between the woodlands and the rest of the world, seperated by a huge wall. It was meant to keep evil at bay, but it didn't really work.");
	Room* forestgrave = new Room("in a sparse wooden graveyard.");

	Room* outsidegate = new Room("at a spork in the road, but no sporks are to be seen anywhere.");

	Room* treasuretree = new Room("in the treasure grove, where the grass is golden and treasure chests sometimes appear.");
	Room* treasurecliff = new Room("in the treasure cliff at the edge of the desert, where there is actualy treasure.");

	Room* bossgrove = new Room("in the BOSS GROVE, where the boss of WANING WOODLANDS is known to appear.");

	Room* fdintermission1 = new Room("on the path between the woodlands and the wastelands.");
	Room* fdintermission2 = new Room("on the path between the woodlands and the wastelands. The foliage is sparse here. BURGERSBURG can be seen faintly in the distance.");
	Room* fdintermission3 = new Room("on the path between the woodlands and the wastelands. Dead trees surround you. The BURGER RESTAURANT is just barely visible from here.");

	Room* desert = new Room("in the wastelands. There is no sign of life anywhere (except you!).");
	desert->setWelcome("Welcome to DESOLATE DESERT", "<<< THE WASTELANDS BEYOND >>>", "The world beyond your forest, where life has been sucked out of the dirt. Surely there must be someone around here?");

	//NPC* wallelder = new NPC("WALL ELDER", "WELBY", "An ancient elder whose rocky face spans the wall. There may be more to him, but all you can see is his face.");

	//Attack copy/paste: Attack* ATTACK = new Attack("NAME", "DESCRIPTION", COST, POWER, PIERCE, MINHITS, MAXHITS, TARGETS);
	////////////////////////
	//DELETE THIS ATTACK, ONLY FOR TESTING
	Attack* finishhim = new Attack("", "DESCRIPTION", 0, 99999999, 99999999, 1, 1, 1);
	////////////////
	Attack* punch = new Attack("PUNCH", "punched", -5, 2, 0, 1, 1, 1);
	punch->addDescription("Throw a simple punch at the target.");

	Attack* energyball = new Attack("ENERGY BALL", "threw an energy ball at", 5, 10, 2, 1, 1, 1, 0);
	energyball->addDescription("Throw a ball of pure kinetic energy at the target.");

	Attack* precisionstrike = new Attack("PRECISION STRIKE", "", 10, 35, 15, 1, 1, 1, 10);
	precisionstrike->addDescription("Launch a heavy mass of energy speedily towards the target.");

	//SET START ROOM TO VILLAGE
	NPC* self = new NPC("\0", "SELF", "It's a me.", forestentrance, 20, 5, 6, 0, 0, 10, 5, 0, true, true);
	self->setScale(1, 1, 1, 0, 0, 1, 1);
	self->setDialogue("Huh?");
	self->Recruit();
	self->setBasicAttack(punch);
	self->addSpecialAttack(energyball);
	self->addSpecialAttack(finishhim);
	self->addSpecialAttack(precisionstrike);

	NPC* archie = new NPC("VILLAGE ELDER", "ARCHIE", "The elder of Tactical Tent Village. He stands there all day and night like a statue.", village, 1, 0, 1, 0, 0, 0, 0, 50);
	archie->setDialogue("So you are going on a BURGER QUEST too, I hear? Just keep heading NORTH, and you'll soon reach BURGERSBURG. Safe travels, child!");
	archie->setRejectionDialogue("I am sorry. Though I would love to join you on your BURGER QUEST, I must stay here and watch over the village, for my recruitable variable is set to false.");

	//NPC* treeelder = new NPC("TREE ELDER", "TREE", "An ancient tree outdating BURGERs");

	NPC* egadwick = new NPC("SCIENCE GRAMPS", "EGADWICK", "Your grandpa who lives in a secluded corner of the village. He's always advancing science to the dismay of high school chemistry students.", tentlab, 15, 2, 3, 10, 10, 2, 10, 10);
	egadwick->setDialogue("Ah hello kiddo.[][][][][][][]");
	egadwick->setRejectionDialogue("No, sorry kiddo. I made a robot for gardening but now it's trying to cut my gorgeous hair and it's on the loose in the forest. If you can destroy it I could probably go.");
	egadwick->setRecruitmentDialogue("Ah, I haven't been adventuring in decades. Thanks for the invitation, kiddo!");

	Room* casino = new Room("in a casino. You should really leave before you develop a gambling addiction.");
	Room* BURGERRESTAURANT = new Room("in the BURGER RESTAURANT. The BURGER MAN is waiting for you to order a BURGER.");
	Room* BURGERPRISON = new Room("in the BURGER PRISON redacted from existence. There is one singular cell, holding one singular man.");

	//REPLACE PLACEHOLDER STATS
	NPC* graham = new NPC("GAMBLER", "GRAHAM", "A sorry gambling addict who is trillions in debt. He'll pay it off as soon as he wins; any day now.", casino, 30, 10, 5, 0, 2, 20, 0, 2);
	graham->setDialogue("What's that? I should quit gambling? Haven't you heard that 99% of gamblers quit right before hitting it big?\"\nGAMBLING MACHINE - \"You lose 1000000 monies.\"\nGRAHAM - \"Aw dang it.");
	graham->setRejectionDialogue("Nah, sorry man. I'm just about to win the jackpot. I can feel it!\"\nGAMBLING MACHINE - \"You lose 1000000 monies.\"\nGRAHAM - \"Aw dang it.");

	NPC* burgerman = new NPC("", "BURGER MAN", "The manager of the BURGER RESTAURANT. He has a BURGER for a head and an uncanny stature.", BURGERRESTAURANT, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647);
	//NPC* henryjerry = new NPC("BURGER QUEST 1 PROTAGONIST", "HENRY JERRY", "The sleep-deprived protagonist from the first game who was used as a puppet of BURGER. He wears a formal suit and seems traumatized.", limbo, 10, 2, 4, 1, 0, 4, 5);

	NPC* burgerheretic = new NPC("BURGER HERETIC", "ARCHIBALD", "A shriveled man imprisoned for resisting the global domination of BURGER.", BURGERPRISON, 1000000, 5000000, 900000000, 100000, 0, 700000, 200000, 80000000);
	burgerheretic->setDialogue("I have not seen anybody in ages. [PLOT DEVICE QUEST INSTRUCTIONS]");
	burgerheretic->setRejectionDialogue("I would love to join you on your quest. But as long as the BURGER MENACE endures, so shall these bars you see in front of me.");

	//it's a me
	Room* tenthome = new Room("in the developer's house.");
	NPC* developer = new NPC("DEVELOPER", "TOMAS", "The guy who made the game, except not really that guy because yeah.", tenthome, 1, 0, 1, 0, 0, 0, 0, 0);
	developer->setDialogue("Yo wassup.");
	developer->setRejectionDialogue("Nah, sorry. I don't think I would make a good teammate because I made my stats really low. I gotta stay humble, you know?");

	NPC* jimmyjohn = new NPC("SHOPKEEPER", "JIMMY JOHN", "The owner of the village convenience store. He has an italian accent.", tentstore, 500, 400, 99999, 800, 99999, 50, 50, 30);
	jimmyjohn->setDialogue("Welcome to my convenience store! None is more convenient!");
	jimmyjohn->setRejectionDialogue("I'm sorry I cannot. Who will take care of my store?");

	//tent store stock
	Item* ibuprofen = new Item("IBUPROFEN", "Relieves pain and inflammation and stuff. (heals 10 HP)", limbo, 10);
	tentstore->setStock(ibuprofen, 2147483647, 10, "JIMMY JOHN - Thank you for your patronage!");

	//set up room exits
	village->setExit(SOUTH, docks);
	village->setExit(EAST, forestentrance);
	village->setExit(WEST, villageleft);
	village->setExit(IN_TENT_1, tentstore);
	village->setExit(IN_TENT_2, tentstation);
	village->setExit(IN_TENT_3, tentchurch);

	villageleft->setExit(EAST, village);
	villageleft->setExit(IN_TENT, tentmansion);

	tentmansion->setExit(OUT, villageleft);
	tentmansion->setExit(UPSTAIRS, tentlab);
	tentlab->setExit(DOWNSTAIRS, tentmansion);
	
	tentstore->setExit(OUT, village);
	tentstation->setExit(OUT, village);
	tentchurch->setExit(OUT, village);
	docks->setExit(NORTH, village);

	forestentrance->setExit(WEST, village);
	forestentrance->setExit(NORTH, forest);
	forest->setExit(SOUTH, forestentrance);

	Room* tunnel = new Room("in the tunnels that span the continent.");

	NPC* tunnel_lobster = new NPC("", "TUNNEL LOBSTER", "An immense, savage crustacean who inhabits the tunnels below.", tunnel, 200, 20, 10, 20, 10, 50, 10, 30, true);
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
	NPC* prickly_hog = new NPC("", "PRICKLY HOG", "A small but ferocious hog with sharp prickles.", limbo, 10, 10, 5, 0, 20, 15, 5);
	Attack* headbutt = new Attack("HEADBUTT", "headbutted", -5, 5, 0, 1, 1, 1);
	Attack* homing_prickle = new Attack("HOMING PRICKLE", "launched homing prickles at", 5, 3, 5, 2, 4, 3);
	prickly_hog->setBasicAttack(headbutt);
	prickly_hog->addSpecialAttack(homing_prickle);

	NPC* greater_hog = new NPC("", "GREATER HOG", "A larger and more territorial hog with sharp prickles.", limbo, 20, 0, 0, 0, 0, 0, 0);
	greater_hog->setBasicAttack(headbutt);
	greater_hog->addSpecialAttack(homing_prickle);

	NPC* grassman = new NPC("", "GRASSMAN", "A really grassy humanoid who hates real humans.", limbo, 16, 0, 5, 0, 2, 5, 5);
	Attack* grassslap = new Attack("GRASSSLAP", "grassily slapped", 0, 3, 0, 1, 1, 1);
	grassman->setBasicAttack(grassslap);

	//for npcs you can actually fight, make sure to set their level as 0 at construction, then set the level manually after setting the scale, otherwise stats will be off

	//set up npc enemies
	NPC* forestguard = new NPC(*grassman);
	forestguard->setLeader(true, 1, forestentrance);
	forestguard->setDialogue("(angry bush noises)");
	forestguard->setRejectionDialogue("(angry bush noises)");
	//forestguard->setParty(grassman, grassman, grassman, prickly_hog);

	//forestentrance->blockExit(NORTH, ENEMY, "guarded by the GRASSMAN.");
	forestguard->blockExit(NORTH, ENEMY, "guarded by the GRASSMAN.");

	/*Fighter savage_hog = Fighter("SAVAGE HOG", "A towering hog elder with sharp prickles.", 10, 10, 5, 0, 20, 15);


	Fighter disease_amalgamate = Fighter("DISEASE AMALGAMATION", "A writhing mass of disease.", 500, 0, 80, 0, 999, 20);

	Fighter jim_shady = Fighter("JIM SHADY", "This Jim Shady is just imitating.", 50, 10, 99999, 10, 99999, 0);
	Fighter slim_jim = Fighter("SLIM JIM", "A Jim obsessed with athleticism and velocity.", 150, 0, 99999, 0, 99999, 99999); 
	//this guy should appear in groups
	Fighter jimmy = Fighter("JIMMY SHIMMY", "A small but deadly Jim.", 1, 0, 9999, 0, 9999, 40);
	Fighter jim = Fighter("THE REAL", "JIM SHADY", "He's Jim Shady. The REAL Jim Shady.", 1000, 200, 999999, 400, 999999, 99999);*/

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
	currentRoom->printStock();
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
	roomCanidate->undefeatEnemies();
	currentRoom = roomCanidate;
	for (NPC* npc : (*party)) {
		npc->setRoom(currentRoom);
	}
	PrintRoomData(currentRoom);
}

void takeItem(Room* currentRoom, vector<Item*>* inventory, char* itemname) {
	Item* item = getItemInVector(currentRoom->getItems(), itemname);
	if (item == NULL) {
		if (getItemInVector(currentRoom->getStock(), itemname) != NULL) {
			cout << "\nThe " << itemname << " is for sale! You can't just take it.";
			return;
		} else if (getItemInVector(*inventory, itemname) != NULL) {
			cout << "\nYou're already carrying this item!";
			return;
		}
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
		cout << "\nYou have no \"" << itemname << "\"."; //I know ". is grammatically inaccurate but it looks way better than ."
		return;
	}
	item->setRoom(currentRoom);
	inventory->erase(remove(inventory->begin(), inventory->end(), item), inventory->end());
	cout << "\nYou dropped the " << itemname << ".";
}

//use commands may be formatted "USE ITEM", or "USE ITEM ON NPC", or "USE ITEM NAME ON NPC", so have fun coding that
void useItem(Room* currentRoom, vector<Item*>* inventory, char* commandExtensionP) {
	/*if (!strcmp(item->getType(), "thingy")) {
    	thingyItem* thingy = (thingyItem*)item;
    	thingy->doTheThing();
	}*/
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
	if (item == NULL) {
		item = getItemInVector(currentRoom->getStock(), name);
	}
	if (item != NULL) {
		printItemData(item);
		return;
	}
	cout << "\nThere is no item or person named \"" << name << "\" here.";
}

void buy(Room* currentRoom, vector<Item*>* inventory, char* name, int& mony) {
	Item* item = getItemInVector(currentRoom->getStock(), name);
	if (item == NULL) {
		if (getItemInVector(currentRoom->getItems(), name) != NULL) {
			cout << "\nNobody is selling the " << name << "; you can just take it.";
			return;
		} else if (getItemInVector(*inventory, name) != NULL) {
			cout << "\nYou already own this item!";
			return;
		}
		cout << "\nThere is no \"" << name << "\" here.";
		return;
	}
	item->buy(mony, inventory);
	if (item->getStock() <= 0) {
		currentRoom->removeStock(item);
	}
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

	Battle battle = Battle(party, npc->getParty(), inventory, mony, npc->getEscapable());
	int battlestatus = battle.FIGHT();
	if (battlestatus == 0) { //lose
		cout << "\nDEFEAT.";
		CinPause();
		int monyLoss = mony - mony/2;
		mony -= monyLoss;
		cout << "\nYou lost " << monyLoss << " mon";
		if (monyLoss == 1) {
			cout << "y.";
		} else {
			cout << "ies.";
		}
		CinPause();
	} else if (battlestatus == 1) { //win
		cout << "\nYOU WIN!";
		CinPause();
		mony += battle.getMonyReward(); 
		//prints how much monies were earned and the new current total. I don't care about grammar here because the reward is literally never just 1
		cout << "\nYou earned " << battle.getMonyReward() << " monies! Now you have " << mony << " monies!";
		CinPause();
		for (NPC* teammate : *party) {
			teammate->addXp(battle.getXpReward());
		}
		cout << "\nYou";
		if (party->size() > 1) {
			cout << "r party members";
		}
		cout << " gained " << battle.getXpReward() << " XP!";
		CinPause();
		for (NPC* teammate : *party) {
			if (teammate->getLevelUp()) {
				cout << "\n" << teammate->getName() << " leveled up! " << teammate->getName() << " is now Level " << teammate->getLevel() << "!";
				//I really want this to show stat changes
				teammate->setLevelUp(false);
				CinPause();
			}
		}
		npc->defeat();
	}
	PrintRoomData(currentRoom);
}

void printHelp(char validCommands[15][255], char flavorText[16][255]) {
	cout << "\n";
	cout << flavorText[rand() % 16];
	cout << "\nValid commands:";
	for (int i = 0; i < 15; i++) {
		cout << "\n" << validCommands[i];
	}
}

int main() {
	srand(time(NULL));
	
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
		"I could really go for a burger right now. Too bad they only sell BURGERs here.",
		"You have a cake and eat it too.",
		"You take a potato chip... and eat it."
	};

	char validCommands[15][255] = {
		"GO [direction]",
		"TAKE [item]",
		"DROP [item]",
		"USE [item] (ON [npc])",
		"RECRUIT [npc]",
		"DISMISS [npc]",
		"ASK [npc]",
		"INVENTORY",
		"PARTY",
		"ROOM",
		"ANALYZE [npc/item]",
		"FIGHT [npc]",
		"BUY [item]",
		"HELP",
		"QUIT"
	};

	cout << "Welcome to BURGER QUEST 2: ELECTRIC BOOGALOO\nYou're going on a quest to get a BURGER.\nType HELP for help.\n";

	cout << "\n             (type your name here!)\nYour name is ";

	char name[255];
	//uncomment this, I just don't want to type the name every time I want to test something
	//cin.getline(name, 255);

	AllCaps(&name[0]);

	if (!strcmp(name, "")) {
		cout << "\nSELF - \"Ok I guess I just don't have a name then.\"";
		//uncomment this too
		//CinPause();
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
		} else if (!strcmp(commandWord, "ROOM")) {
			PrintRoomData(currentRoom);
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