//this is the implementation file for Battles, which controls combat with enemies
//this is pretty much main.cpp 2: electric boogaloo

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

//creates the battle instance
Battle::Battle(vector<NPC*>* _playerTeam, vector<NPC*>* _enemyTeam, vector<Item*>* _inventory, int& mony, bool _escapable) {
	//clone npcs from both teams so I don't have to reset them later
	for (NPC* npc : *_playerTeam) {
		playerTeam.push_back(new NPC(*npc));
	}
	for (NPC* npc : *_enemyTeam) {
		enemyTeam.push_back(new NPC(*npc));
	}
	//add everyone to a list of everyone for convenience
	everyone.insert(everyone.begin(), playerTeam.begin(), playerTeam.end());
	everyone.insert(everyone.end(), enemyTeam.begin(), enemyTeam.end());
	sortBySpeed(everyone); //sorts everyone by their speed stat. That's the order they move in

	inventory = _inventory; //references the inventory from main
	escapable = _escapable; //if you can escape this battle, usually only false for boss fights

	player = (*_playerTeam)[0]; //get a reference to the player from main

	//give the enemies xp corresponding with the enemy's level, in order to scale them to that level
	//sets the enemy level to the enemy leader, which should be the first one in the party since they put themselves in their own party when set to a leader
	int enemyLevel = enemyTeam[0]->getLevel();
	for (NPC* npc : enemyTeam) { //sets every enemy to the found level and sets them to be an enemy
		npc->setLevel(enemyLevel);
		npc->setEnemy(true);
		xpReward += npc->getXpReward(); //adds their xp and mony rewards to the total
		monyReward += npc->getMonyReward();
	}

	//the rest of the process is just de-duplicating enemy names (for example, if there's three BOBs, it renames them to BOB, BOB 2, and BOB 3);
	vector<char*> names; //each name
	vector<int> amount; //how many times that name appears
	vector<int> count; //how many times we've counted that name while renaming them (like, we have BOB, and then we rename him to BOB 2. That means count == 2)
	for (NPC* npc : enemyTeam) {
		char* name = new char[255]; //gets the name of the enemy
		strcpy(name, npc->getName());
		bool namefound = false; //searches names to see if we've already saved that name before
		for (int i = 0; i < names.size(); i++) {
			if (!strcmp(name, names[i])) {
				amount[i]++; //increments how many "name"s there are
				namefound = true; //we have indeed found that name now, and we break since we've found it
				break;
			}
		} //we add the new name to the list, and that it appears once
		if (!namefound) {
			names.push_back(name);
			amount.push_back(1);
			count.push_back(0); //we haven't begun checking this yet, so it's set to 0
		}
	} //renames all the duplicate enemies
	for (NPC* npc : enemyTeam) {
		for (int i = 0; i < names.size(); i++) { //we check all the names to see if one matches this enemy's name
			if (!strcmp(npc->getName(), names[i])) {
				if (amount[i] > 1) { //no need to rename the first one, so we only continue if it isn't
					char suffix[10]; //the suffic to append to the enemy's name
					//sets suffix to " ", and then increments the current name's count and sets the rest of it to the current number
					snprintf(suffix, 10, " %d", ++count[i]); //I need 10 characters in the array because otherwise g++ complains
					npc->addSuffix(suffix); //adds the number suffix to the enemy's name
				}
				break; //breaks because we found the name
			}
		}
	} //deallocates the names we were using
	for (char* name : names) {
		delete[] name;
	}
}
//creates a new npc and adds it to the battle
void Battle::addNPC(NPC* npc) {
	vector<NPC*>* team = NULL; //gets which side the enemy is on
	if (npc->getEnemy()) { //we must manually set enemy earlier or else EVERY new npc will go to the player team
		team = &enemyTeam;
	} else {
		team = &playerTeam;
	}
	NPC* newguy = new NPC(*npc);
	int count = 1; //we include newguy so the name count starts at 1
	for (int i = 0; i < team->size(); i++) { //renames the enemy according to the amount of the same named enemy present. (eg. there is a BOB here already and we add a new BOB. BOB sees there's another BOB and renames himself BOB 2)
		char* name = strstr((*team)[i]->getName(), newguy->getName());
		if (name == NULL || name != &(*team)[i]->getName()[0]) { //if we didn't find the name or it the name was found after the beginning, the npc has a different name so we continue
			continue;
		}
		char charafter = (*team)[i]->getName()[strlen(newguy->getName())]; //check the char in the npc name after the newguy's name
		if (charafter != ' ' && charafter != '\0') { //skip this name if the next character isn't space or null terminator, because that means the name doesn't correspond and it's some other name that starts with the new guy's name
			continue;
		}
		count++; //we found a matching name so increment the count
	}
	if (count > 1) { //we only add a suffix if there's >1 of that enemy. Otherwise it's pretty obvious it's BOB 1
		char suffix[12]; //generate the suffix corresponding to the amount of similar names found
		snprintf(suffix, 12, " %d", count); //it has to be 12 and not 10 this time because the compiler was complaining
		newguy->addSuffix(suffix); //apply the suffix
	}
	newguy->setLevel((*team)[0]->getLevel()); //update the level to match the team
	team->push_back(newguy);
	everyone.push_back(newguy);
}
//ticks an effect (if it does anything every turn, it does that, and decrements its duration)
void Battle::tickEffect(Effect& effect) {
	NPC* npc = effect.npc; //gets the affected npc
	//decrement even if the npc is unconscious because a fire will eventually go out if you're on fire even if you're asleep
	if (--effect.duration <= 0) {
		return;
	}
	if (!npc->getHealth()) { //we don't affect the npc if they're unconscious
		return;
	} //applies damage (or healing)
	if (effect.damage != 0) {
		npc->directDamage(effect.damage, effect.name);
	} //applies sp alterations
	if (effect.spleak != 0) {
		npc->alterSp(-effect.spleak, effect.name);
	} //applies the guard
	if (effect.guardset != 0) {
		npc->setGuard(effect.guardset);
	}
}
//ticks all the effects that everyone has
void Battle::tickEffects() {
	for (NPC* npc : everyone) { //ticks everyone's effects
		for (Effect& effect : npc->getEffects()) {
			tickEffect(effect);
		}
	}
	//checks everyone's effects to see if they wore off
	for (NPC* npc : everyone) { //we must delete them here in order to not create dangling pointers
		vector<Effect>& effects = npc->getEffects(); //gets reference to the npc's effects
		for (int i = 0; i < effects.size();) {
			if (effects[i].duration <= 0) {
				cout << npc->getName() << "'s " << effects[i].name << " wore off!";
				npc->removeEffect(effects[i]); //removes the effect
				CinPause();
				continue; //skip the increment because the rest of the vector just shifted left from deleting the current effect
			}
			i++;
		}
	}
}
//carries out the attack (makes it hit the target)
void Battle::carryOutAttack(Attack* attack, NPC* attacker, NPC* target) {
	attacker->alterSp(-attack->cost); //removes sp from the attacker
	if (attack->spbomb) { //if it's this one move, we have to check due to its unique functionality
		attack->power = 0; //starts at 0 damage
		for (NPC* npc : playerTeam) { //removes everyone's sp and adds it to the sp bomb damage total
			attack->power += npc->getSP();
			npc->alterSp(-npc->getSP());
		}
	}
	//says what happened
	cout << "\n" << attacker->getName() << " used " << attack->name << "!\n" << attacker->getName() << " " << attack->description << " " << target->getName() << "!";
	vector<NPC*> tarparty; //gets which party is being targeted based on if the target is an enemy or not
	if (target->getEnemy()) {
		tarparty = enemyTeam;
	} else {
		tarparty = playerTeam;
	}
	//says if we hit multiple targets
	if (attack->targets > 1 && tarparty.size() > 1) {
		cout << "\n" << attacker->getName() << "'s surrounding teammates were also affected!";
	}
	CinPause();
	//gets the position of the target in the team vector
	int tarPos = distance(tarparty.begin(), find(tarparty.begin(), tarparty.end(), target));
	//hits all the targets, we must iterate in order to account for multi-target attacks
	for (int i = 0; i < tarparty.size(); i++) {
		if (tarPos - attack->targets / 2 <= i && i < tarPos + attack->targets - attack->targets / 2) { //if they're within range
			int effectiveAttack = 0; //if the attack power is 0, it doesn't do damage and ignores the npc's attack stat
			if (attack->power) { //we set the effective attack to this if the attack does damage
				effectiveAttack = attacker->getAttack();
			}
			//damages the target                                                                                //some moves hit a random amount of times within a certain range
			tarparty[i]->damage(attack->power + effectiveAttack, attack->pierce + attacker->getPierce(), rand() % (attack->maxhits + 1 - attack->minhits) + attack->minhits);
			if (attack->appliedeffect != NULL) { //adds an effect if the attack had one
				tarparty[i]->setEffect(attack->appliedeffect);
			}
		}
	}
	CinPause();
}
//uses the specified item from the inventory, and returns if the player's turn is over based on if we successfully used an item 
bool Battle::useItem(char* itemname) {
	Item* item = getItemInVector(*inventory, itemname); //finds the item; no need to check currentRoom this time!
	char itemName[255] = "";
	char npcName[255] = "";
	NPC* npc = NULL; //the target of the item, if necessary
	if (item == NULL) { //if no item was found, we check if the player was trying to target someone
		ParseWithON(itemname, &itemName[0], &npcName[0]); //seperates the item name accounting for " ON "
		item = getItemInVector(*inventory, itemName); //finds the item using the new item name
		if (item != NULL) { //if an item was found like this
			if (item->getForEnemy()) { //finds the target based on the item's intended target
				npc = getNPCInVector(enemyTeam, npcName);
			} else {
				npc = getNPCInVector(playerTeam, npcName);
			} //gives error if a target was provided but none was needed
			if (!item->getTargetNeeded()) {
				cout << "\nThe " << itemName << " doesn't need a target!";
				return false;
			} //gives error message if no target was found
			if (npc == NULL) {
				if (item->getForEnemy()) { //if it was for targeting the enemy
					cout << "\nThere is nobody named \"" << npcName << "\" on the enemy team!";
				} else { //if it was for targeting your team
					cout << "\nThere is nobody named \"" << npcName << "\" in your party!";
				}
				return false;
			}
		} else {
			itemname = itemName; //makes the null item print not say "You have no [item] ON [npc]"
		}
	} //error if you have no item of the given name
	if (item == NULL) {
		cout << "\nYou have no \"" << itemname << "\".";
		return false;
	//if the item needed a target but none was given
	} else if (item->getTargetNeeded() && npc == NULL) {
		if (item->getForEnemy()) {
			if (enemyTeam.size() > 1) { //error message which we skip if there's only one enemy
				cout << "\nThis item needs a target!";
				return false;
			}
			npc = enemyTeam[0]; //set the target to the only possible one
		} else {
			if (playerTeam.size() > 1) { //error message which we skip if there's only the player
				cout << "\nThis item needs a target!";
				return false;
			}
			npc = playerTeam[0]; //set the target to the only possible one
		}
		
	}

	//hp items heal the target
	if (!strcmp(item->getType(), "hp")) {
		HpItem* hp = (HpItem*)item; //converts to the corresponding subclass
		if (!npc->getHealth()) { //you're not allowed to heal unconscious teammates, that's what revives are for
			cout << npc->getName() << " is too damaged for the " << itemname << " to work!";
			return false;
		} //directly applies the health to the target
		npc->directDamage(-hp->getHp());
	//sp items restore sp for the target
	} else if (!strcmp(item->getType(), "sp")) {
		SpItem* sp = (SpItem*)item; //converts to the corresponding subclass
		npc->alterSp(sp->getSp()); //alters the sp of the target
	//revive items are just healing items but you can only use them on incapacitated teammates
	} if (!strcmp(item->getType(), "revive")) {
		ReviveItem* revive = (ReviveItem*)item; //converts to the corresponding subclass
		if (npc->getHealth()) { //gives error message if used on capacitated teammate
			cout << "The " << itemname << " must be saved for dire circumstances!";
			return false;
		} //gives success message and revives the teammate
		cout << npc->getName() << " was recapacitated!";
		npc->directDamage(-revive->getHp());
	//effect items apply an effect to the target
	} if (!strcmp(item->getType(), "effect")) {
		EffectItem* affecter = (EffectItem*)item; //converts to the corresponding subclass
		if (!npc->getHealth()) { //gives error message because you're not allowed to affect incapacitated teammates
			cout << npc->getName() << " is incapacitated! The " << itemname << " won't work!";
			return false;
		} //sets the effect on the target
		npc->setEffect(affecter->getEffect());
	//
	//} else if (!strcmp(item->getType(), "BURGER")) {

	//some key items have attacks, so you can use them here
	} else if (!strcmp(item->getType(), "key")) {
		KeyItem* key = (KeyItem*)item; //converts to the corresponding subclass
		if (key->getAttack() == NULL) { //if the key item has no attack
			cout << "The " << itemname << " can't be used in battle!";
			return false;
		} //carries out the attack on the target
		carryOutAttack(key->getAttack(), playerTeam[0], npc);
	//some movement items have attacks as well
	} else if (!strcmp(item->getType(), "movement")) {
		MovementItem* mover = (MovementItem*)item; //converts to the corresponding subclass
		if (mover->getAttack() == NULL) { //if the movement item has no attack
			cout << "The " << itemname << " can't be used in battle!";
			return false;
		} //carries out the attack on the target
		carryOutAttack(mover->getAttack(), playerTeam[0], npc);
	//manhole items' names are misleading. They're just random stuff on the ground that you can find and throw at enemies as an attack
	} else if (!strcmp(item->getType(), "manhole")) {
		ManholeItem* cover = (ManholeItem*)item; //converts to the corresponding subclass
		//they all have attacks so we don't check for it, we just launch the attack onto the target
		carryOutAttack(cover->getAttack(), playerTeam[0], npc);
	//info items give information as usual
	} else if (!strcmp(item->getType(), "info")) {
		InfoItem* info = (InfoItem*)item; //converts to the corresponding subclass
		cout << "\n" << info->getText(); //prints the info item's information
		return false;
	} else { //otherwise the player tried to use an item that is only usable in the overworld so we give an error message
		cout << "\nThe " << itemname << " can't be used in battle!";
		return false;
	} //if the item gets used up after use, we delete it! (since deleteItem checks inventory first, we can get away with passing NULL for room)
	if (item->getConsumable()) {
		deleteItem(NULL, inventory, item);
	}
	return true; //if we got here everything went well so we return true to move on from the player's turn
}
//prints the given team's data
void Battle::printTeam(vector<NPC*>& team, bool printLevel, bool printSP, bool printFainted) {
	for (NPC* npc : team) { //we skip incapacitated npcs if we're ignoring fainted npcs
		if (!printFainted && !npc->getHealth()) {
			continue;
		} //prints their name and how much health they have left out of their maximum health
		cout << "\n" << npc->getName() << " " << npc->getHealth() << "/" << npc->getHealthMax() << " HP";
		if (printSP) { //prints their sp and how much they have left out of their maximum if printSP is true
			cout << " " << npc->getSP() << "/" << npc->getSPMax() << " SP";
		} //prints the level of the npc
		if (printLevel) {
			cout << " - LEVEL " << npc->getLevel();
		}
	}
}
//prints all the items in the inventory
void Battle::printInventory() {
	cout << "\nYou";
	if (inventory->size() < 1) { //if we have no items
		cout << " have no items!";
		return;
	} //prints the rest of the sentence if we do have items
	cout << "r items are:";
	for (Item* item : *inventory) { //prints all the items like normal
		cout << "\n" << item->getName();
	}
}
//prints all the members of the player party
void Battle::printParty() {
	cout << "\nMembers of your party:";
	printTeam(playerTeam, true, true); //we print their sp and level, and we do print fainted teammates
}
//prints all the members of the enemy party
void Battle::printEnemies() {
	cout << "\nEnemy party:";
	printTeam(enemyTeam, true, false, false); //we do not print their sp but yes their level, and not the fainted enemies
}
//prints an analysis of the given item or npc
void Battle::analyze(char* name) {
	NPC* npc = getNPCInVector(everyone, name); //finds the npc in the list of everyone
	if (npc != NULL) { //prints the data!
		printNPCData(npc);
		return;
	} //finds the item in the inventory, no ground items here!
	Item* item = getItemInVector(*inventory, name);
	if (item != NULL) { //prints the data!
		printItemData(item);
		return;
	} //error message if nothing matches the given name
	cout << "\nThere is no item or person named \"" << name << "\" here!";
}
//prints some flavor text and all the valid commands
void Battle::printHelp() {
	cout << "\n";
	cout << flavorText[rand() % 8]; //prints a random flavor text
	cout << "\nValid commands:";
	for (int i = 0; i < 9; i++) { //prints all the valid commands
		cout << "\n" << validCommands[i];
	}
}
//tries to escape the battle
bool Battle::runAway() {
	if (escapable) {
		cout << "\nYou successfully ran away!";
		CinPause();
	} else {
		cout << "\nThere is no escape.";
	}
	return escapable;
}
//puts everyone in a queue based on their speed
void Battle::reorder(queue<NPC*>& orderly_fashion) {
	for (NPC* npc : everyone) { //everyone vector is already sorted by speed so we just use that
		orderly_fashion.emplace(npc); //place the npc at the back of the line!
	}
}
//interpret and carry out player attacks, and return whether we successfully launched an attack
bool Battle::ParseAttack(NPC* plr, char* commandP, char* commandWordP, char* commandExtensionP, int checkMax) {
	//we have to check multiple times, since attacks may have 0 or more spaces in them
	for (int i = 0; i <= checkMax; i++) {
		if (i > 0) { //if it's the first one, we've already parsed it the same, no need to parse again
			ParseCommand(commandP, commandWordP, commandExtensionP, i);
		}
		//finds the target using what is currently thought to be the name
		NPC* target = getNPCInVector(enemyTeam, commandExtensionP);
		//if no target was given in the string and there's only one enemy, we of course just target that enemy
		//target == NULL if this if statement runs, because I don't remember naming any enemy ""
		if (!strcmp(commandExtensionP, "") && enemyTeam.size() <= 1) {
			target = enemyTeam[0];
		}

		if (target == NULL) { //if no target was found
			if (i == checkMax) { //prints an error if we've already checked as many times as permitted
				cout << "\nInvalid target \"" << commandExtensionP << "\".";
				return false;
			}
			continue; //goes to the next iteration if we still have more checks to run, with one more space this time!
		} //if it was the basic attack we launch that
		if (!strcmp(commandWordP, plr->getBasicAttack()->name)) {
			carryOutAttack(plr->getBasicAttack(), plr, target);
			return true;
		} //otherwise we try to find a special attack matching the name found
		for (Attack* attack : plr->getSpecialAttacks()) {
			//we only launch the attack if the player is leveled up enough to use it
			if (attack->minLevel <= plr->getLevel() && !strcmp(commandWordP, attack->name)) {
				if (plr->getSP() < attack->cost) { //we don't launch the attack if we don't have enough sp
					cout << "\nYou don't have enough SP for this attack! (" << plr->getSP() << "/" << attack->cost << ")";
					return false;
				}
				carryOutAttack(attack, plr, target);
				return true;
			}
		}
	}
	
	//prints error message and returns false because no attack was launched successfully, so the player must type something elsse
	cout << "\nInvalid command or attack \"" << commandWordP << "\" (type HELP for help, or ATTACKS for list of attacks).";
	return false;
}
//the player's controls. Returns whether the player did a valid action or not
bool Battle::playerTurn(NPC* plr) {
	bool continuing = true; //keep going until this is set to false
	bool keepFighting = true; //if the player wants to keep fighting (not run away)

	while (continuing) { //loops until a valid action is taken
		char command[255] = ""; //the command that the player inputs into

		char commandWord[255]; //the command word
		char commandExtension[255]; //the rest of the command

		cout << "\n> "; //prompt to input something
		cin.getline(command, 255); //gets the player input
		AllCaps(command); //capitalizes the input

		ParseCommand(command, commandWord, commandExtension); //parses the command, splitting it into the command word and the rest (and a space)

		if (!strcmp(commandWord, "USE")) { //for using an item
			continuing = useItem(commandExtension); //may potentially end the player turn if valid item is used
		} else if (!strcmp(commandWord, "INVENTORY")) { //for printing a list of items in the inventory
			printInventory();
		} else if (!strcmp(commandWord, "PARTY")) { //for printing a list of all teammates
			printParty();
		} else if (!strcmp(commandWord, "ENEMIES")) { //for printing a list of all opponents
			printEnemies();
		} else if (!strcmp(commandWord, "ATTACKS")) { //for printing a list of all available attacks
			printAttacks(player);
		} else if (!strcmp(commandWord, "ANALYZE")) { //for printing analysis of item or npc
			analyze(commandExtension);
		} else if (!strcmp(commandWord, "HELP")) { //for printing all available commands and flavor text
			printHelp();
		} else if (!strcmp(commandWord, "RUN")) { //for trying to run away
			keepFighting = continuing = !runAway(); //ends the player turn (and the whole battle) if we successfully run away
		} else { //otherwise the player may have been trying to use an attack, so we try to parse it, and we end the player turn if an attack was successfully launched!
			continuing = !ParseAttack(plr, command, commandWord, commandExtension);
		}
	}

	CinIgnoreAll(); //clears extra input and invalid input

	return keepFighting; //returns if the player wants to keep fighting (true) or run away (false)

}
//npcs decide what to do on their turn here
void Battle::npcTurn(NPC* npc) {
	cout << "\n" << npc->getName() << "'s turn!"; //prints whose turn it is
	CinPause();
	//this is the attack choosing process. Each attack has a certain weight based on the total sp cost of all special moves. We choose a move randomly based on that weight, and if no moves are chosen we do the basic attack. This system makes it so we prioritize more expensive and probably better moves, and makes it so the npcs try to "build up" sp for the bigger moves rather than just always using the cheapest one
	int r = rand()%100; //we choose a random number from 0 to 99
	int limit = 0; //the limit from 0 to 100 that the total attack weight must add up to
	Attack* attack = NULL; //the attack that was chosen
	for (Attack* _attack : npc->getSpecialAttacks()) { //checks each attack for if we should choose it
		if (_attack->cost > npc->getSP()) { //if it's too expensive with the current sp we move on to the next attack
			continue;
		}
		if (_attack->minLevel > npc->getLevel()) { //if the npc isn't experienced enough to use the move, we don't count it
			continue;
		}
		//we add the weight of the attack to the limit
		limit += npc->getWeights()[_attack];
		if (r <= limit) { //we check if the limit has reached r. If so, that's the attack we choose
			attack = _attack;
			break; //break because we chose the attack
		}
	}
	//defaults to basic attack if no special attack was chosen
	if (attack == NULL) {
		attack = npc->getBasicAttack();
	}
	
	bool attackPlayer = npc->getEnemy(); //we attack the player if it's an enemy
	if (attack->targetAlly) { //we reverse the targeting if the attack actually targets allies (like healing moves)
		attackPlayer = !attackPlayer;
	}
	if (npc->getHypnotized()) { //we reverse the targeting if the npc is hypnotized
		attackPlayer = !attackPlayer;
	}

	//defaults to basic attack if attack is meant to target fainted npcs (revive moves) but no potential target is incapacitated
	if ((!attackPlayer && attack->targetFainted && aliveCount(playerTeam) == playerTeam.size()) ||
		(!attackPlayer && attack->targetFainted && aliveCount(enemyTeam) == enemyTeam.size())) {
		attack = npc->getBasicAttack();
	}
	
	NPC* target = NULL; //try to find the target by randomly throwing darts until one hits
	while (target == NULL) {
		if (attackPlayer) { //finds a random target in the player team
			target = playerTeam[rand() % playerTeam.size()];
		} else { //finds a random target in the enemy team
			target = enemyTeam[rand() % enemyTeam.size()];
		} //if the attack targets > 0 hp npcs, the check fails if the target canidate has 0 hp
		if (!attack->targetFainted) {
			if (target->getHealth() <= 0) {
				target = NULL;
			}
		} else { //if the attack targets 0 hp npcs, the check fails if the target canidate has > 0 hp
			if (target->getHealth() > 0) {
				target = NULL;
			}
		}
	} //launches the attack!
	carryOutAttack(attack, npc, target);
}
//begins the battle process and returns 0 if the player team lost, 1 if they won, and 2 if they ran away
int Battle::FIGHT() {
	cout << "\nBATTLE BEGIN!"; //shows everyone involved in the battle plus flavor text
	printTeam(playerTeam);
	cout << "\n<<< VERSUS >>>";
	printTeam(enemyTeam);
	CinPause(); //gives the player time to orient themselves

	queue<NPC*> turn; //queue for finding whose turn it is
	NPC* current; //the current npc whose turn it is

	bool continuing = true; //continues until this is set to false, only if the player successfully runs away
	while (continuing) { //the main battle loop!
		if (turn.size() <= 0) { //we put everyone in order again if the turn queue is empty
			reorder(turn);
			tickEffects(); //we also tick effects once every round
		}

		current = turn.front(); //gets the npc whose turn it is at the front of the queue
		if (current->getHealth() <= 0) { //the npc doesn't do anything if out of health. I do this empty if statement because i still need to do stuff after all the else ifs and I don't like nesting
			//do a backflip idk
		} else if (current->getFrozen()) { //prints how the npc wanted to move but couldn't due to freezing
			cout << "\n" << player->getName() << " tried to move but is frozen in place!";
		} else if (current->getPlayerness()) { //starts the player turn!
			cout << "\n" << player->getName() << "'s turn!\nWhat will you do?";
			continuing = playerTurn(current);
		} else { //does the npc's turn
			npcTurn(current);
		}
		
		turn.pop(); //removes the npc from the front of the queue because their turn is over!
		//check the player team and enemy team for if they've lost all hp, and returns win or loss based on that result
		if (aliveCount(playerTeam) <= 0) {
			return 0; //lose
		} else if (aliveCount(enemyTeam) <= 0) {
			return 1; //win
		}
	}
	return 2; //return 2 because the player ran away
}
//gets how much xp to reward the player team for winning
int Battle::getXpReward() {
	return xpReward;
}
//gets how many monies to reward the player for winning
int Battle::getMonyReward() {
	return monyReward;
}
//deletes the npc copies because they were only for this instance of battle
Battle::~Battle() {
	for (NPC* npc : everyone) {
		delete npc;
	}
}