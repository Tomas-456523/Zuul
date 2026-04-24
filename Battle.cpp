//this is the implementation file for Battles, which controls combat with enemies
//this is pretty much main.cpp 2: electric boogaloo

#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>
#include "Battle.h"
#include "NPC.h"
#include "Item.h"
#include "Helper.h"
#include "Attack.h"
#include "Effect.h"

using namespace std;
using namespace Helper;

//creates the battle instance MARK: initialize
Battle::Battle(vector<NPC*>* _playerTeam, vector<NPC*>* _enemyTeam, vector<Item*>* _inventory, int& mony, bool _escapable, bool scaleenemies) {
	player = (*_playerTeam)[0]; //get a reference to the team leaders from main in case we need them
	enemy = (*_enemyTeam)[0];

	inventory = _inventory; //references the inventory from main
	escapable = _escapable; //if you can escape this battle, usually only false for boss fights
	scaleEnemies = scaleenemies; //tracks if we should scale the enemies
	
	setupWave(true, 0, false); //set up the player and enemy teams with the 0th (1st) wave, don't scale player team
	setupWave(false, 0, scaleenemies); //we scale the enemies by default since the world fights do that

	if (enemyTeam[0]->getMasked()) { //set to hidden values if the enemy leader was masked
		enemyTeam[0]->setTitle(enemyTeam[0]->getHiddenTitle());
		enemyTeam[0]->setName(enemyTeam[0]->getHiddenName());
		enemyTeam[0]->setDescription(enemyTeam[0]->getHiddenDescription());
	}
}
//resets or just sets the given party according to the given wave number the new wave npcs
void Battle::setupWave(bool pteam, size_t wave, bool scaleteam) {
	vector<NPC*>& team = (pteam ? playerTeam : enemyTeam); //gets the affected team
	NPC* worldleader = (pteam ? player : enemy); //gets the team leader (in the world)
	for (NPC* npc : team) { //delete the old team if there was one
		everyone.erase(remove(everyone.begin(), everyone.end(), npc), everyone.end());
		//MARK: remove their npceffects
		delete npc;
	}
	team.clear(); //empty the old team vector to make room for the new team

	//we copy the npcs so I don't have to reset them later and also there's can be multiple of the same npc
	for (NPC* _npc : *worldleader->getParty(wave)) { //copy all the npcs from the new wave to their corresponding party
		NPC* npc = new NPC(*_npc);
		npc->setEnemy(worldleader->getEnemy()); //make eneminess match the leader
		team.push_back(npc);
		vector<NPC*> guards = npc->getGuardians();
		for (NPC* _guard : guards) { //update all guardian links so they're not pointing to outside battle
			npc->removeGuardian(_guard); //get rid of the one in world
			NPC* guard = new NPC(*_guard);
			guard->setEnemy(worldleader->getEnemy()); //make guardian's eneminess match their team
			npc->setGuardian(guard); //set the Battle copy as the actual guardian
			guard->setGuarding(npc); //update this one to guard the Battle copy and not the one in the world
			team.push_back(guard);
		}
	}

	//add everyone to a list of everyone for convenience
	everyone.insert(everyone.begin(), team.begin(), team.end());	

	//de-duplicating the npc names (for example, if there's three BOBs, it renames them to BOB, BOB 2, and BOB 3);
	vector<char*> names; //each name
	vector<int> amount; //how many times that name appears
	vector<int> count; //how many times we've counted that name while renaming them (like, we have BOB, and then we rename him to BOB 2. That means count == 2)
	for (NPC* npc : team) {
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
	for (NPC* npc : team) {
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

	if (!scaleteam) return; //don't scale the team if we're told not to (also skips reward adding, but that doesn't matter in every context scaleteam is set to false anyway)

	//give the enemies xp corresponding with the enemy's level, in order to scale them to that level
	//sets the enemy level to the enemy leader, which should be the first one in the party since they put themselves in their own party when set to a leader
	int enemyLevel = team[0]->getLevel();
	for (NPC* npc : enemyTeam) { //sets every enemy to the found level and sets them to be an enemy
		npc->setLevel(enemyLevel);
		
		xpReward += npc->getXpReward(); //adds their xp and mony rewards to the total
		monyReward += npc->getMonyReward();
	}
}
//creates a new npc and adds it to the battle MARK: add npc
void Battle::addNPC(NPC* npc, NPC* parent, bool altteam) {
	vector<NPC*>* team = NULL; //gets which side the enemy is on
	if (npc->getEnemy() || altteam && !npc->getEnemy()) { //we must manually set enemy earlier or else EVERY new npc will go to the player team
		team = &enemyTeam;
	} else if (!npc->getEnemy() || altteam && npc->getEnemy()) {
		team = &playerTeam;
	}
	NPC* newguy = new NPC(*npc);
	int count = 1; //we include newguy so the name count starts at 1
	for (int i = 0; i < team->size(); i++) { //renames the enemy according to the amount of the same named enemy present. (eg. there is a BOB here already and we add a new BOB. BOB sees there's another BOB and renames himself BOB 2)
		const char* name = strstr((*team)[i]->getName(), newguy->getName());
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
	newguy->setParent(parent);
	team->push_back(newguy);
	everyone.push_back(newguy);
	if (newguy->getOpener()) { //do the opening attack if they have one
		checkOpeners({newguy});
	}
}
//checks all the effects that the given npc has for if they've run out, for duration 0 moves MARK: check effects
void Battle::checkEffects(NPC* npc) {
	for (Effect* effect : npc->getEffects()) { //check all their effects for if their duration is 0
		NPCEffect* neffect = npc->getEffect(effect, true);
		if (neffect->duration <= 0) {
			npc->removeEffect(effect, NULL); //remove the effect
			detatchEffect(neffect); //handle effect removal stuff
		}
	}
}
//add the effect to the alleffects vector while handling duplicates MARK: attach effect
void Battle::attachEffect(NPCEffect* effect) {
	if (effect->effect->hypnotize) { //when changing hypnosis status, untake anyone they may be taking
		effect->affected->setTaking(NULL);
	}
	if (effect->effect->speedbuff != 1) { //if the speed has changes
		speedSort(effect->affected); //reconfigure the npc's speed and turn stuff to account for the new speed
	}
	for (NPCEffect* neffect : alleffects) { //if there's a duplicate just return cause it's already there
		if (neffect->effect == effect->effect && neffect->affected == effect->affected) {
			return;
		}
	} //psuh the new effect
	alleffects.push_back(effect);
}
//handle effect removal, including removing the effect from the alleffects vector MARK: detatch effect
void Battle::detatchEffect(NPCEffect* effect) {
	if (!effect) return; //removeEffect on NPCs may return NULL for the effect not being there so we check for that
	if (effect->stacks) return; //if the effect is still affecting the npc, we don't do the removal stuff

	if (effect->effect->hypnotize) { //when changing hypnosis status, untake anyone they may be taking
		effect->affected->setTaking(NULL);
	} 
	if (effect->effect->speedbuff != 1) { //if the speed has changes
		speedSort(effect->affected); //reconfigure the npc's speed and turn stuff to account for the new speed
	}
	//stop tracking the effect
	alleffects.erase(remove(alleffects.begin(), alleffects.end(), effect), alleffects.end());

	if (effect->affected->popKO()) handleKnockout(effect->affected); //handle ko stuff if the npc was just incapacitated due to effect fall damage
}
//stuff that happens when an npc gets incapacitated MARK: handle knockout
void Battle::handleKnockout(NPC* npc) {
	if (npc->popExtraLives()) { //if the npc has extra lives, pop one and fully heal them
		cout << "\n" << npc->getName() << " had an extra life!";
		CinPause();
		npc->directDamage(-npc->getHealthMax());
		return;
	} else { //make sure the player understands this npc is incapacitated
		cout << "\n" << npc->getName() << " is incapacitated!";
		CinPause();
	}
	
	npc->setTaking(NULL); //untake taken npcs

	//also if there is only one npc left on this team we make sure they're not taken
	if (aliveCount(npc->getEnemy() ? enemyTeam : playerTeam) == 1) {
		for (NPC* enemy : (npc->getEnemy() ? playerTeam : enemyTeam)) npc->setTaking(NULL);
	}

	//remove bond effects from the parent
	if (npc->getOpener() && npc->getOpener()->appliedeffect && npc->getOpener()->appliedeffect->bond) {
		NPC* leader = (!npc->getParent() ? (npc->getEnemy() ? enemyTeam : playerTeam)[0] : npc->getParent());
		detatchEffect(leader->removeEffect(npc->getOpener()->appliedeffect, npc));
		if (leader->popKO()) handleKnockout(leader); //handle ko stuff if the leader was just incapacitated due to fall damage
	}
}
//does the hitting stuff for only one of the targets MARK: hit target
void Battle::hitTarget(Attack* attack, NPC* attacker, NPC* reciever, int hits, bool parry) {
	double attackProcessing = 0; //processes attack and pierce as doubles for better rounding at the end
	double pierceProcessing = 0;
	if (attack->spbomb) { //sp bombs don't get multipliers or modifiers of any sort, power still increases due to level from sp stat
		attackProcessing = attack->power;
	} else if (attack->instakill && !reciever->getBoss()) { //instakill attacks remove all health except for bosses
		attackProcessing = pierceProcessing = 9999999; //just send as much damage as possible MARK: actually no, send damage equal to the health (direct damage)
	} else { //normal attacks, we multiply the attack power/pierce and attack/pierce stat together so they both matter equally, and divide by 10 just cause I don't want the numbers to be too big
		attackProcessing = attack->power * Round(attacker->getAttack() * attacker->getAttMultiplier()) / 10.0;
		pierceProcessing = attack->pierce * Round(attacker->getPierce() * attacker->getPierceMultiplier()) / 10.0;
	}
	if (parry) attackProcessing *= 2; //parried hits hit with double power!
	for (Effect* effect : attack->synergies) { //increase attack power by 1.5x for all synergies on the reciever
		bool synergized = false; //track if we synergized at all so we can pause once for any amount of synergies found
		if (reciever->getEffect(effect, true)) {
			attackProcessing *=  1.5;
			cout << "\nThe attack synergized with " << reciever->getName() << "'s " << effect->name << "!";
		}
		if (synergized) CinPause(); //final pause
	}
	
	for (int j = 0; j < hits; j++) { //damages the target the given amount of times
		if (hits > 1) cout << "\nHit " << j+1 << "!"; //announce which hit it was if it's multi-hit

		//parry the hit if parrying the attacker
		if (reciever->getParrying() == attacker && !attack->getBeneficial()) {
			cout << "\n" << reciever->getName() << " parried it!";
			CinPause();
			reciever->setParrying(NULL); //don't parry other attacks or hits
			hitTarget(attack, attacker, attacker, 1, true); //send the hit right back at double power!
			hits -= 1; //subtract one hit because we just parried it
			continue; //don't apply the damage of this hit to the npc
		}

		attackProcessing *= (0.9 + ((double)rand()/RAND_MAX) * 0.2); //randomly vary attack power of every hit by 10%
		bool crit = !(rand()%20); //5% chance to crit
		if (crit) attackProcessing *= 1.75; //75% extra attack seems to be a good sweet spot for crit multipliers

		int effectiveAttack = Round(attackProcessing); //convert the processing doubles to ints
		int effectivePierce = Round(pierceProcessing);

		if ((reciever->getInvincible() && attack->power < 0) || (!reciever->getInvincible() && attack->power != 0)) { //hit normally if healing or (damaging and not invincible), never apply damage for 0 power attacks since their point isn't to affect health if that's the case
			int damage = reciever->damage(effectiveAttack, effectivePierce);
			if (attack->lifesteal) attacker->damage(damage * -attack->lifesteal, 0); //heal the attacker based on the lifesteal
		}
		if (crit) { //yeeeeaaaaaahhhhhhh!!!!!! critical hit lets goooooooo excitement (this is the reaction this message invokes) (or "oh shoot" if you got hit)
			cout << "\nCRITICAL " << (attack->power > 0 ? "HIT" : "HEAL") << "!";
			CinPause();
		}
		if (reciever->getInvincible() && attack->power > 0) {
			cout << reciever->getName() << " brushed it off!"; //says they weren't affected cause they're invincible
			CinPause();
		}
	}
	if (!hits) cout << "\nThe attack missed!";
	if (reciever->popKO()) handleKnockout(reciever); //apply knockout stuff to the reciever
	//after this it does stuff related to the reciever(s) of the attack (if it doesn't print what happened, it's probably either said in NPC's functions or by the attack itself)
	if (attack->appliedeffect != NULL) { //adds an effect if the attack had one
		attachEffect(reciever->setEffect(attack->appliedeffect));
	}
	if (attack->cancel != NULL) { //removes the effect this attack cancels out
		if (NPCEffect* canceled = reciever->getEffect(attack->cancel, true)) {
			detatchEffect(reciever->removeEffect(canceled->effect, NULL)); //don't announce the change, like "VIOLA flung BOB!" "BOB broke free!" like no he didn't he doesn't seem very free in the stratosphere
		}
		if (reciever->popKO()) handleKnockout(reciever); //handle ko stuff if the reciever was just incapacitated due to effect fall damage
	}
	for (int i = 0; i < attack->copyamount; i++) { //copy the target for copying attacks
		addNPC(reciever); //duplicate the target with no parent on the same team as the target
	}
	if (attack->targuard) reciever->setGuard(attack->targuard, true); //adds target guards on the target
	if (attack->spleak) reciever->alterSp(-attack->spleak); //alter sp how the attack does that
	if (attack->extralives) { //add extra lives to the target
		reciever->addExtraLives(attack->extralives);
		cout << "\n" << reciever->getName() << " got " << attack->extralives << " extra li" << (attack->extralives == 1 ? "fe" : "ves") << "!";
	}
	if (attack->contact && reciever->getRecoilAttack()) { //attacker gets attacked by the target's recoil attack
		carryOutAttack(reciever->getRecoilAttack(), reciever, attacker, true);
	}
}
//hits the targets (and surroundings depending on attack range), seperate function needed because this must be called multiple times for unfocused moves/attacks MARK: hit targets
void Battle::hitTargets(NPC* attacker, Attack* attack, vector<NPC*>& tarparty, int tarPos) {
	for (int i = 0; i < tarparty.size(); i++) { //hits all the targets, we must iterate in order to account for multi-target attacks
		if (tarPos - attack->targets / 2 <= i && i < tarPos + attack->targets - attack->targets / 2 && tarparty[i]->getHealth()) { //if they're within range and still have health
			NPC* reciever = tarparty[i]; //who will recieve the damage, could be the target or the npc guarding the target
			if (reciever->getAway() || (!attack->targetFainted && !reciever->getHealth())) { //ignore incapacitated affected npcs if we don't target incapacitated and also can't target away npcs
				continue;
			}
			vector<NPC*> guardians = reciever->getGuardians(); //check the reciever's guardians
			for (size_t i = 0; i < guardians.size(); i++) { //check all the guardians for if they can take the hit for the current reciever, and just go with the first one we find if there's multiple
				NPC* guardian = guardians[i];
				//guardians block hits if they're not beneficial and the guardian still has hp and they're not frozen or recovering or not here, of course, and also they won't block hits for opposing side (for example, after getting hypnotized and doing a protect move), also taking hypnotization into account
				if (!attack->getBeneficial() && guardian->getHealth() && !guardian->getFrozen() && !guardian->getRecovering() && !guardian->getAway() && (!guardian->getHypnotized() && guardian->getEnemy() == reciever->getEnemy()|| guardian->getHypnotized() && guardian->getEnemy() != reciever->getEnemy())) {
					cout << guardian->getName() << " is taking the hit for " << reciever->getName() << "!";
					reciever = guardian;
					CinPause();
					break;
				}
			}
			int hits = 1; //some moves hit a random amount of times within a certain range
			if (attack->focushits) hits = rand() % (attack->maxhits + 1 - attack->minhits) + attack->minhits;
			hitTarget(attack, attacker, reciever, hits);
		}
	}
}
//carries out the attack (makes it hit the target) MARK: carry out attack
void Battle::carryOutAttack(Attack* attack, NPC* attacker, NPC* target, bool recoil) {
	attacker->alterSp(-Round(attack->cost*attacker->getSPUseMultiplier())); //removes sp from the attacker
	if (attack->spbomb) { //if it's this one move, we have to do stuff related to its unique functionality
		attack->power = 0; //starts at 0 damage
		for (NPC* npc : playerTeam) { //removes everyone's sp and adds it to the sp bomb damage total
			attack->power += npc->getSP();
			npc->alterSp(-npc->getSP());
		}
		cout << attacker->getName() << " gathered the team's SP into an SP BOMB!";
		CinPause();
		if (!attack->power) { //if nobody has sp
			cout << "\nEveryone has 0 SP! No attack was formed!";
			return;
		}
		cout << "\n" << attack->power << " SP went into the attack!\nIt's ";
		if (attack->power < 12) cout << "a small glowing orb!";
		else if (attack->power < 30) cout << "really big!";
		else if (attack->power < 60) cout << "a boulder of energy!";
		else if (attack->power < 80) cout << "a huge attack!";
		else if (attack->power < 100) cout << "super big!";
		else cout << "massive!";
		CinPause();
	}
	//says what happened depending on if it was normal or due to recoil
	if (!recoil) cout << "\n" << attacker->getName() << " used " << attack->name << "!"; //using normal attack
	else if (attack->focushits) cout << "\n" << target->getName() << " was affected by " << attacker->getName() << "'s " << attack->name << "!"; //attack triggered target's recoil or attack had recoil and hit teammate as well and this is the result of either situation
	else cout << "\n" << attacker->getName() << "'s team was affected by " << attacker->getName() << "'s " << attack->name << "!"; //this was the result of a recoiling regular attack that hits the team unfocusedly

	//print description of the attack and what the attacker did
	cout << "\n" << attacker->getName() << " " << attack->description;
	if (attack->focushits) cout << " " << target->getName() << attack->afterdesc; //prints the target unless it's not focused on a specific target
	cout << "!"; //punctuate the description!
	vector<NPC*> tarparty; //gets which party is being targeted based on if the target is an enemy or not
	if (target->getEnemy()) {
		tarparty = enemyTeam;
	} else {
		tarparty = playerTeam;
	}
	//affects the attacker before attacking if we do that
	if (attack->affectselfbeforeattack) {
		if (attack->selfeffect != NULL) attachEffect(attacker->setEffect(attack->selfeffect));
		if (attack->selfcancel != NULL) {
			detatchEffect(attacker->removeEffect(attack->selfeffect, NULL));
			if (attacker->popKO()) handleKnockout(attacker); //handle ko stuff if the attacker was just incapacitated due to fall damage
		}
	}
	//says if we hit multiple targets
	if (!attack->focushits && attack->targets > 1 && tarparty.size() > 1) {
		cout << "\n" << target->getName() << "'s surrounding teammates were also affected!";
	}
	CinPause();
	if (attack->focushits) { //normal moves which focus damage on one target
		//gets the position of the target in the team vector
		int tarPos = distance(tarparty.begin(), find(tarparty.begin(), tarparty.end(), target));
		hitTargets(attacker, attack, tarparty, tarPos);
	} else { //unfocused moves which hit a random target for every hit
		for (size_t i = 0; i < rand() % (attack->maxhits + 1 - attack->minhits) + attack->minhits; i++) {
			int tarPos = rand() % tarparty.size();
			hitTargets(attacker, attack, tarparty, tarPos);
		}
	}
	if (attack->recoil) { //apply recoil with 0 pierce, because pierce is something intentional
		attacker->damage(attack->recoil * Round(attacker->getAttack() * attacker->getAttMultiplier()) / 10.0, 0);
	}
	if (attack->guardset) attacker->setGuard(attack->guardset, true); //set the guard if the attack does that, add it to the current guard unless it's negative, then it goes from 0 because getGuard returns that for values < 0
	if (attack->protect) { //handle guarding here and not in hitTargets because we it only guards one person anyway
		if (NPC* guarding = attacker->getGuarding()) guarding->removeGuardian(attacker); //can only guard one npc at a time
		target->setGuardian(attacker);
		attacker->setGuarding(target);
	}
	if (attack->parry) attacker->setParrying(target); //parrying attacks are for parrying the target so we do that
	//affects the attacker however applicable
	if (!attack->affectselfbeforeattack) {
		if (attack->selfeffect != NULL) attachEffect(attacker->setEffect(attack->selfeffect));
		if (attack->selfcancel != NULL) {
			detatchEffect(attacker->removeEffect(attack->selfeffect, NULL));
			if (attacker->popKO()) handleKnockout(attacker); //handle ko stuff if the attacker was just incapacitated due to fall damage
		}
	}
	for (int i = 0; i < attack->summonamount; i++) { //add adds for how many this attack summons
		bool forenemy = attack->enemysummon; //enemy and team summons are reversed when hypnotized
		addNPC(attack->summon, attacker, (attacker->getHypnotized() ? !forenemy : forenemy));
	}
}
//check all the given npcs for if they have an opening attack to do MARK: check openers
void Battle::checkOpeners(const vector<NPC*>& checks) {
	for (NPC* npc : checks) {
		if (npc->getOpener()) {
			npcTurn(npc); //do the turn for the npc (who will choose the opener)
			npc->setOpener(NULL); //only do opener once
		}
	}
}
//uses the specified item from the inventory, and returns if the player's turn is over based on if we successfully used an item MARK: use item
bool Battle::useItem(const char* itemname) {
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
			} else if (npc->getAway()) { //can't use item on missing npc
				cout << "\n" << npcName << " is not in the battlefield right now!";
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
	} else if (!strcmp(item->getType(), "revive")) {
		ReviveItem* revive = (ReviveItem*)item; //converts to the corresponding subclass
		if (npc->getHealth()) { //gives error message if used on capacitated teammate
			cout << "The " << itemname << " must be saved for dire circumstances!";
			return false;
		} //gives success message and revives the teammate
		cout << npc->getName() << " was recapacitated!";
		npc->directDamage(-revive->getHp());
	//effect items apply an effect to the target
	} else if (!strcmp(item->getType(), "effect")) {
		EffectItem* affecter = (EffectItem*)item; //converts to the corresponding subclass
		if (!npc->getHealth()) { //gives error message because you're not allowed to affect incapacitated teammates
			cout << npc->getName() << " is incapacitated! The " << itemname << " won't work!";
			return false;
		} //sets the effect on the target
		attachEffect(npc->setEffect(affecter->getEffect()));
		//the SUPERSMOOTHIE has this specific battle-handled effect
		for (int i = 0; i < affecter->getEffect()->multipositioning; i++) {
			playerTeam.push_back(npc); //add a shallow copy to the team lists so they're just in multiple positions, very cool
			everyone.push_back(npc);
		}
	//some key items have attacks, so you can use them here
	} else if (!strcmp(item->getType(), "key") || !strcmp(item->getType(), "hose")) {
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
	} else if (!strcmp(item->getType(), "weapon")) { //weapon items are another way of just using their attack
		WeaponItem* weapon = (WeaponItem*)item;
		carryOutAttack(weapon->getAttack(), playerTeam[0], npc);
	} else { //otherwise the player tried to use an item that is only usable in the overworld so we give an error message
		cout << "\nThe " << itemname << " can't be used in battle!";
		return false;
	} //if the item gets used up after use, we delete it! (since deleteItem checks inventory first, we can get away with passing NULL for room)
	if (item->getConsumable()) {
		deleteItem(NULL, inventory, item);
	}
	return true; //if we got here everything went well so we return true to move on from the player's turn
}
//prints the given team's data MARK: print team
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
//prints all the items in the inventory MARK: print inventory
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
//prints all the members of the player party MARK: print party
void Battle::printParty() {
	cout << "\nMembers of your party:";
	printTeam(playerTeam, true, true); //we print their sp and level, and we do print fainted teammates
}
//prints all the members of the enemy party
void Battle::printEnemies() {
	cout << "\nEnemy party:";
	printTeam(enemyTeam, true, false, false); //we do not print their sp but yes their level, and not the fainted enemies
}
//prints an analysis of the given item or npc MARK: analyze
void Battle::analyze(const char* name) {
	NPC* npc = getNPCInVector(everyone, name); //finds the npc in the list of everyone
	if (npc != NULL) { //prints the data!
		printNPCData(npc, true);
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
//tries to escape the battle MARK: run away
bool Battle::runAway() {
	if (escapable) {
		cout << "\nYou successfully ran away!";
		CinPause();
	} else {
		cout << "\nThere is no escape.";
	}
	return escapable;
}
//sort the npcs based on speed, by tracking their known speed and putting them in the turn queue MARK: speed sort
void Battle::speedSort(NPC* npc) {
	turn.push(npc); //add the npc back into the turn queue
	knownspeeds[npc] = Round(npc->getSpeed()*npc->getSpeedMultiplier()); //track what we now know the speed should be when we do their turn
}
//reset the npc trackers in a new round MARK: reorder
void Battle::reorder() { //put everyone in the priority queue based on their speed
	for (NPC* npc : everyone) speedSort(npc); //sort the npc into the turn queue and track their speed
	went.clear(); //nobody has gone because it's a new round
}
//interpret and carry out player attacks, and return whether we successfully launched an attack MARK: parse attack
bool Battle::ParseAttack(NPC* plr, char* commandP, char* commandWordP, char* commandExtensionP, int checkMax) {
	char tcother[255] = ""; //track the invalid portion of the command when it was partially correct so we can give better error messages (e.g. unsuccessfully using PUNCH FLURRY shouldn't say anything about the move PUNCH)
	char acother[255] = ""; //name is a(ttack) or t(arget) c(andidate) other cause it was the other to the valid candidate

	//we have to check multiple times, since attacks may have 0 or more spaces in them
	for (int i = checkMax-1; i >= 0; i--) {
		if (i < checkMax) { //if it's the first one, we've already parsed it the same, no need to parse again
			ParseCommand(commandP, commandWordP, commandExtensionP, i);
		}
		vector<NPC*>& tarteam = everyone; //based on the attack given we choose either the player team for beneficial attacks or the enemy team otherwise. It defaults to everyone so we can print better errors if no attack is found

		Attack* attack; //the attack we're doing
		
		if (!strcmp(commandWordP, plr->getBasicAttack()->name)) { //it might be the basic attack
			attack = plr->getBasicAttack();
		} else { //it might be one of the special attacks
			attack = getAttackInVector(plr->getSpecialAttacks(), commandWordP); //try to find the attack in the special attacks vector
			if (attack && attack->minLevel > plr->getLevel()) attack = NULL; //you're not supposed to use (or know about) attacks you're underleveled for, so we treat that case the same as not finding an attack
		}

		if (attack) tarteam = (attack->targetAlly ? playerTeam : enemyTeam); //see which team this attack should target

		//finds the target using what is currently thought to be the name
		NPC* target = getNPCInVector(tarteam, commandExtensionP);

		//if no target was given in the string and there's only one possible target, we of course just target that guy
		//target == NULL if this if statement runs, because I don't remember naming anybody ""
		if (!strcmp(commandExtensionP, "") && tarteam.size() <= 1) {
			target = tarteam[0];
		}

		if (attack && target) { //if we found a valid attack and target for that attack, we (try to) launch it!
			if (target->getHealth() <= 0 && !attack->targetFainted) { //if trying to hit incapacitated npc
				cout << "\n" << commandExtensionP << " is incapacitated";
				if (attack->getBeneficial()) cout << "! " << attack->name << " won't have any effect!"; //different text based on if trying to help or attack the incapacitated target
				else cout << "already!";
				return false; //could not launch attack
			}
			if (target->getAway()) { //if the target is not here and targetable at the moment
				for (NPC* npc : tarteam) { //check if there is someone we could target instead
					if (npc->getHealth() > 0 && !npc->getAway() || npc->getPlayerness()) { //if this is the player team there's no reason to hit this away teammate so don't act if that's the case
						cout << "\n" << commandExtensionP << " is not in the battlefield right now!";
						return false; //don't launch the attack if we should hit someone else instead
					}
				} //if there's no one else to target, then just let the player try to hit the away npc because there's nothing better to do while waiting for them to come back
			}
			if (plr->getSP() < attack->cost) { //we don't launch the attack if we don't have enough sp
				cout << "\nYou don't have enough SP for this attack! (" << plr->getSP() << "/" << attack->cost << ")";
				return false; //could not launch attack
			}
			carryOutAttack(attack, plr, target);
			return true; //successfully launched attack!
		} else if (target && strlen(commandWordP) > strlen(tcother)) { //track the invalid attack or command when target was valid (prefer longer ones cause they're probably more meaningful error messages)
			strcpy(tcother, commandWordP);
		} else if (attack && strlen(commandExtensionP) > strlen(acother)) { //track the invalid target when the attack was valid (also prefer longer ones for the same reason)
			strcpy(acother, commandExtensionP);
		}
	} //from here the attack launching was unsuccessful

	//prints error message and returns false because no attack was launched successfully, so the player must type something else
	if (strlen(tcother) && strlen(acother)) cout << "\nInvalid target \"" << acother << "\" or invalid command or attack \"" << tcother << "\". (Type HELP for help)"; //one or the other is wrong just print ambiguous error
	else if (strlen(tcother)) cout << "\nInvalid command or attack \"" << tcother << "\". (Type HELP for help)"; //invalid attack or command error
	else if (strlen(acother)) cout << "\nInvalid target \"" << acother << "\". (Type HELP for help)"; //invalid target error
	else cout << "\nInvalid command or attack \"" << commandP << "\". (Type HELP for help)"; //"their whole input was inilegible" error
		
	return false; //nothing was successfully launched
}
//the player's controls. Returns whether the player did a valid action or not MARK: player turn
bool Battle::playerTurn(NPC* plr) {
	bool promptline = true; //if the prompting > should be in a new line
	bool keepFighting = true; //if the player wants to keep fighting (not run away)

	bool continuing = true;
	while (continuing) { //loops until a valid action is taken
		char command[255] = ""; //the command that the player inputs into

		char commandWord[255]; //the command word
		char commandExtension[255]; //the rest of the command

		if (promptline) cout << "\n"; //print a newline for the > if we should
		cout << "> "; //prompt to input something
		cin.getline(command, 255); //gets the player input
		AllCaps(command); //capitalizes the input

		promptline = true; //make sure the next prompt probably is in a new line

		ParseCommand(command, commandWord, commandExtension); //parses the command, splitting it into the command word and the rest (and a space)

		if (!strcmp(commandWord, "USE")) { //for using an item
			continuing = !useItem(commandExtension); //may potentially end the player turn if valid item is used
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
		} else if (!strlen(command)) { //if the player typed nothing, don't try to do anything
			promptline = false;
		} else { //otherwise the player may have been trying to use an attack, so we try to parse it, and we end the player turn if an attack was successfully launched!
			continuing = !ParseAttack(plr, command, commandWord, commandExtension);
		}
	}

	CinIgnoreAll(); //clears extra input and invalid input

	return keepFighting; //returns if the player wants to keep fighting (true) or run away (false)
}
//find which team this attack hits MARK: get target team
vector<NPC*>& Battle::getTarTeam(NPC* npc, Attack* attack) {
	//target player team if attacker is enemy and vice versa, but flip if the attacker is hypnotized and do another flip check if the attack is meant to target your own team, like healing
	return npc->getEnemy() ^ npc->getHypnotized() ^ attack->targetAlly ? playerTeam : enemyTeam;
}
//get a vector of all the targets that we could target with the attack MARK: get targets
vector<NPC*> Battle::getTargets(NPC* npc, Attack* attack) {
	if (attack->targetself) return {npc}; //just target self if the attack targets self (no filtering because self-targeting attacks are something that should really just always launch)
	
	vector<NPC*> choices = getTarTeam(npc, attack); //get the options of targets that we have depending on who the attacker is and what the attack does
	//check team-wide attack validity rules, just return early if it wouldn't work
	if (attack->take) {
		if (aliveCount(choices) <= 1) return {}; //can't take the last npc on a team or that would be an endless fight of doing nothing
		for (NPC* att : (npc->getEnemy() ^ npc->getHypnotized() ^ attack->targetAlly) ? enemyTeam : playerTeam) { //make sure nobody in our team is already taking an npc (or whatever team we're representing in the taking, also idk why you would take a teammate but might as well account for that here)
			if (att->getTaking()) return {};
		}
	} //use this to build up a list of valid targets
	vector<NPC*> targets;
	//check each target in the team this attack would target and add it if it's a valid target for the attack
	for (NPC* target : choices) {
		if (attack->targetFainted && target->getHealth() > 0) continue; //can't target fainted if bro isn't fainted
		if (attack->power < 0 && target->getHealth() >= target->getHealthMax()) continue; //can't (shouldn't) heal if bro is at full health
		if (attack->take && (target->getBoss() || target->getPlayerness() || target == npc)) continue; //taking attacks can't target bosses, the player (not player otherwise it would be boring gameplay waiting for your teammates to beat the guy), or the attacker (taking yourself would be some real cartoon stuff)
		if (attack->appliedeffect && attack->appliedeffect->remove && target->getBoss()) continue; //can't remove bosses cause that's just messing up everyone else's groove during the fight
		if (attack->appliedeffect && attack->appliedeffect->hypnotize && target->getBoss()) continue; //can't hypnotize bosses because that would be weird for some fights
		if (attack->risky && target->getHealth() < target->getHealthMax()/2) continue; //can't use risky moves if they have below half health because that would be risky
		if (attack->targetshark && !target->getShark()) continue; //shark-targeting attacks must have a shark to target
		if (!attack->redundanteffect && target->getEffect(attack->appliedeffect, true)) continue; //if the attack shouldn't target npcs who already have the applied effect but the guy does have it
		if (attack->protect) { //protect attacks have various conditions
			if (target == npc) continue; //don't protect yourself because that would be wasting SP to do literally nothing (you're already taking your hits for yourself)
			if (npc->getGuarding() && npc->getGuarding()->getHealth() > 0) continue; //no guarding moves if we're already guarding someone still capacitated (don't switch person being guarded, that would be like "oh lol nope yeah I'm actually not defending you anymore have fun")
			if (target->getGuarding()) continue; //don't guard guardians cause that's weird
			for (NPC* guard : target->getGuardians()) if (guard->getHealth() > 0) continue; //don't guard the teammate if they're already being guarded by someone still capacitated because they got this, better focus on dps for now or guarding other teammates
		} //yay the target passed all the tests and is now a valid target option!
		targets.push_back(target);
	}
	//check for further prioritizations from here
	{ //try to filter out npcs that are away because targeting npcs you can't hit when there's a perfectly targetable npc right there would be really dumb
		vector<NPC*> _targets; //store non-away npcs to see if prioritization would remove all valid targets
		for (NPC* target : targets) {
			if (!target->getAway()) _targets.push_back(target); //add the target if they are not away
		}
		if (!_targets.empty()) targets = _targets; //go with the filtered list if we didn't just filer everyone out
	}
	//filter out dummy npcs if targeting our own team, to avoid stuff like uselessly buffing or healing dummies, for example. This section is to ensure dummies are always useful while making teammates not dumb
	//hypnotized npcs can target dummies no matter what to ensure dummies are always useful, blocking heals for other team and blocking hits on same team
	//non-beneficial attacks can also hit dummies no matter what (skip dummy filtering) so that npcs like graham that can damage teammates are also blocked by dummies
	if (choices[0]->getEnemy() == npc->getEnemy() && !npc->getHypnotized() && attack->getBeneficial()) {
		vector<NPC*> _targets; //store non-dummy npcs to see if prioritization would remove all valid targets
		for (NPC* target : targets) {
			if (target->getBasicAttack() || !target->getSpecialAttacks().empty()) _targets.push_back(target); //add the target if they have any attacks, meaning they're not a dummy (as in, the non-moving definition of the word, not unintelligent)
		}
		if (!_targets.empty()) targets = _targets; //go with the filtered list if we didn't just filer everyone out
	}
	if (Effect* teffect = npc->getTargetEffect()) { //if the npc prioritizes hitting targets with this effect, filter out npcs without the effect
		vector<NPC*> _targets; //store the npcs with the effect to see if prioritization would remove all valid targets
		for (NPC* target : targets) {
			if (target->getEffect(teffect, true)) _targets.push_back(target); //add the target if they have the targeted effect
		}
		if (!_targets.empty()) targets = _targets; //go with the filtered list if we didn't just filter everyone out
	}
	if (attack->prioritizeleader) { //leader-prioritizing moves either prioritize their parent or team leader depending on if they're a summon or not, or if they're hypnotized
		//if there is no parent or the parent is not in the same team (meaning the npc is hypnotized or just targetting the other team's leader), we default to the team leader
		NPC* leader = ((!npc->getParent() || !getNPCInVector(choices, npc->getParent()->getName())) ? choices[0] : npc->getParent()); //or just choose the actual parent
		//check if the leader is one of the available targets and choose it if so
		if (getNPCInVector(targets, leader->getName())) return {leader};
	}
	if (attack->prioritizenonleader) { //non-leader-prioritizing moves prioritize not hitting the leader
		//if there is no parent or the parent is not in the same team (meaning the npc is hypnotized or just targetting the other team's leader), we default to the team leader
		NPC* leader = ((!npc->getParent() || !getNPCInVector(choices, npc->getParent()->getName())) ? choices[0] : npc->getParent()); //or just choose the actual parent
		//if removing the leader wouldn't make it an empty list, we try to filter it out
		if (targets.size() != 1) targets.erase(remove(targets.begin(), targets.end(), leader), targets.end());
	}

	return targets; //return the targets!
}
//choose an attack for the npc based on their precalculated attack weights MARK: choose attack
Attack* Battle::chooseAttack(NPC* npc) {
	//this is the attack choosing process. Each attack has a certain weight based on the total sp cost of all special moves. We choose a move randomly based on that weight, and if no moves are chosen we do the basic attack. This system makes it so we prioritize more expensive and probably better moves, and makes it so the npcs try to "build up" sp for the bigger moves rather than just always using the cheapest one
	int r = rand()%100; //we choose a random number from 0 to 99
	int limit = 0; //the limit from 0 to 100 that the total attack weight must add up to
	Attack* attack = NULL; //the attack that was chosen

	for (Attack* _attack : npc->getSpecialAttacks()) { //checks each attack for if we should choose it
		if (Round(_attack->cost*npc->getSPUseMultiplier()) > npc->getSP()) { //if it's too expensive with the current sp we move on to the next attack
			continue;
		}
		if (_attack->minLevel > npc->getLevel()) { //if the npc isn't experienced enough to use the move, we don't count it
			continue;
		}
		//we add the weight of the attack to the limit
		limit += npc->getWeight(_attack);
		if (r < limit) { //we check if the limit has passed r. If so, that's the attack we choose
			return _attack; //return the attack!
		}
	}
	//defaults to basic attack if no special attack was chosen
	return npc->getBasicAttack();
}
//npcs decide what to do on their turn here MARK: npc turn
void Battle::npcTurn(NPC* npc, bool opener) {
	if (!opener) { //don't print that it's their turn if it's just an opening attack
		cout << "\n" << npc->getName() << "'s turn!"; //prints whose turn it is
		CinPause();
	}

	//get the attack to do! defaults to opener if this is an opening turn
	Attack* attack = (opener ? npc->getOpener() : chooseAttack(npc));
	
	vector<NPC*> targets = getTargets(npc, attack); //try to find the targets for the attack

	while (targets.empty()) { //if no targets were found for the attack (while loop is only for npcs with no basic attack, usually basically just an if statement)
		if (opener) return; //just don't do the attack, don't choose new attack if it was an opening attack
		if (attack == npc->getBasicAttack()) break; //if it WAS just the basic attack causing issues, then we just break so we can get to the doing nothing message
		
		if (!npc->getBasicAttack()) attack = chooseAttack(npc); //if the npc has no basic attack (graham), just keep rolling special attacks until it works
		else attack = npc->getBasicAttack(); //just default to the basic attack for normal npcs

		targets = getTargets(npc, attack); //find new targets with the new attack
	}

	if (targets.empty()) { //no targets were found for either the special or basic attack, so they just do nothing
		cout << "\n" << npc->getName() << " isn't sure what to do...";
		return;
	}
	
	NPC* target = targets[rand()%targets.size()]; //choose one of the targets
	carryOutAttack(attack, npc, target); //launches the attack!

	if (attack->recoilatt && ((double)rand()/RAND_MAX < attack->recoilchance)) { //handle recoiling attacks
		vector<NPC*> rtargets = getTargets(npc, attack->recoilatt); //get the targets which is just the npc's own team
		carryOutAttack(attack->recoilatt, npc, rtargets[rand()%rtargets.size()], true); //launches the attack!
		if (!WorldState[RECOILED]) { //clarify that it was an accident if it was the first time
			cout << "\n" << npc->getName() << " - \"Oops.\"";
			CinPause();
		}
		WorldState[RECOILED] = true;
	}
}
//prints text for starting a new wave or the battle as a whole
void Battle::printVersus(size_t wave) { //shows everyone involved in the battle plus flavor text
	if (!wave) cout << "\nBATTLE BEGIN!"; //first wave is just the starting text
	else cout << "\nWAVE " << wave << "!"; //otherwise print which wave it is
	printTeam(playerTeam);
	cout << "\n<<< VERSUS >>>";
	printTeam(enemyTeam);
	CinPause(); //gives the player time to orient themselves
}
//begins the battle process and returns 0 if the player team lost, 1 if they won, and 2 if they ran away MARK: FIGHT
int Battle::FIGHT() {
	printVersus(0); //print all the battle intro text
	
	NPC* current; //the current npc whose turn it is

	checkOpeners(everyone); //check any opening moves the npcs may have

	bool continuing = true;
	while (continuing) { //the main battle loop! continues until continuing is set to false, only if the player successfully runs away
		if (turn.empty()) { //we put everyone in order again if the turn queue is empty
			reorder(); //also clear went and reset known speeds
		}

		current = turn.top(); //gets the npc whose turn it is at the front of the queue
		turn.pop(); //removes the npc from the front of the queue so the next npc is in line the next turn

		if (went.find(current) != went.end()) { //skip the npc for the rest of the round if they already went
			continue;
		} //skip the turn if their speed doesn't match the known speed, to handle speed changes
		if (knownspeeds[current] != Round(current->getSpeed()*current->getSpeedMultiplier())) {
			continue;
		}

		checkEffects(current); //we also check if there are any 0-duration effects to stop tracking before the turn starts
		
		if (current->getHealth() <= 0) { //the npc doesn't do anything if out of health. I do this empty if statement because i still need to do stuff after all the else ifs and I don't like nesting
			//do a backflip idk
		} else if (current->getFrozen()) { //prints how the npc wanted to move but couldn't due to freezing
			cout << "\n" << current->getName() << " is frozen in place!";
			CinPause();
		} else if (current->getRecovering()) {
			cout << "\n" << current->getName() << " is recovering!";
			CinPause();
		} else if (current->getAway()) {
			cout << "\n" << current->getName() << " is currently away!";
			CinPause();
		} else if (!current->getBasicAttack() && current->getSpecialAttacks().empty()) { //say idle text when there are no attacks
			cout << current->getName() << " is " << idleText[rand()%5];
			CinPause();
		} else if (current->getPlayerness() && !current->getHypnotized()) { //starts the player turn!
			cout << "\n" << player->getName() << "'s turn!\nWhat will you do?";
			continuing = playerTurn(current);
		} else { //does the npc's turn
			npcTurn(current);
		}

		for (Effect* effect : current->getEffects()) { //tick all the npc's effects AFTER their turn (this makes durations more intuitive)
			current->tickEffect(effect);
			if (current->popKO()) handleKnockout(current); //handle ko if this just ko'd them
		}

		went.insert(current); //the npc has went now so we track that they went

		bool newwave = false; //check if we're starting a new wave
		
		//check the player team and enemy team for if they've lost all hp, sees if we can go to a new wave, and if not, returns win or loss based on that result
		if (aliveCount(playerTeam) <= 0) {
			if (++pwave < player->getWaves()) { //also checks each team for if they have more waves
				setupWave(true, pwave, false);
				newwave = true;
			} else return 0; //lose
		} else if (aliveCount(enemyTeam) <= 0) {
			if (++ewave < player->getWaves()) {
				setupWave(true, ewave, scaleEnemies);
				newwave = true;
			} else return 1; //win
		}

		if (newwave) { //reset the turn queue if it's a new wave to force a new round
			while (!turn.empty()) turn.pop();
			checkOpeners(everyone); //check any opening moves the new npcs may have
		}
	}
	return 2; //return 2 because the player ran away
}
//gets how much xp to reward the player team for winning MARK: get rewards
int Battle::getXpReward() {
	return xpReward;
}
//gets how many monies to reward the player for winning
int Battle::getMonyReward() {
	return monyReward;
}
//deletes the npc copies because they were only for this instance of battle MARK: destructor
Battle::~Battle() {
	for (NPC* npc : everyone) {
		delete npc;
	}
}