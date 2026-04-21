//the header file for battles

#ifndef BATTLE
#define BATTLE

#include <queue>
#include "Battle.h"
#include "NPC.h"
#include "Item.h"
#include "Effect.h"

class NPC;
class Item;

//custom comparator used by the turn queue to sort all the fighters by speed
struct speedComparer {
	bool operator()(NPC* a, NPC* b) const {
		return a->getSpeed()*a->getSpeedMultiplier() > b->getSpeed()*b->getSpeedMultiplier();
	}
};

class Battle {
public:
	Battle(vector<NPC*>* _playerTeam, vector<NPC*>* _enemyTeam, vector<Item*>* _inventory, int& mony, bool _escapable = true);
	~Battle();

	int FIGHT(); //starts the battle process and returns an int based on the outcome

	bool useItem(const char* itemname); //use an item in the player's inventory
	void printTeam(vector<NPC*>& team, bool printLevel = true, bool printSP = false, bool printFainted = true); //print everyone in the given team
	void printInventory(); //print the contents of the inventory
	void printParty(); //print everyone in the player's party
	void printEnemies(); //print all the opponents
	void analyze(const char* name); //print the data of the given item or npc
	void printHelp(); //print the valid command words and extensions
	bool runAway(); //try to escape the battle
	void checkEffects(); //checks all the tracked effects for if they're at duration 0
	void attachEffect(NPCEffect* effect); //add the effect to the alleffects vector and make sure no duplicates occur
	void handleKnockout(NPC* npc); //handles stuff related to when the npc gets incapacitated
	void carryOutDamage(NPC* npc, double damage, double pierce); //handles the damaging of the npc and stuff related to that
	void hitTargets(NPC* attacker, Attack* attack, vector<NPC*>& tarparty, int tarPos); //hit the target, and surroundings if needed
	void carryOutAttack(Attack* attack, NPC* attacker, NPC* target, bool recoil = false); //affects the given target based on the given attack
	bool ParseAttack(NPC* plr, char* commandP, char* commandWordP, char* commandExtensionP, int checkMax = 2); //interpret and carry out an attack command given by the player
	vector<NPC*>& getTarTeam(NPC* npc, Attack* attack); //find which team the attack is supposed to target based on the given npc
	vector<NPC*> getTargets(NPC* npc, Attack* attack); //get a vector of valid targets that we can target with the attack
	bool playerTurn(NPC* plr); //the player chooses what to do here
	Attack* chooseAttack(NPC* npc); //choose an attack for the npc based on their precalculated attack weights
	void npcTurn(NPC* npc); //the npc chooses an attack to do
	
	void addNPC(NPC* npc, NPC* parent = NULL, bool altteam = false); //creates a new npc mid-battle

	int getXpReward(); //gets how much xp was earned as a result of a victory
	int getMonyReward(); //gets how many monies were earned as a result of a victory

	void speedSort(NPC* npc); //handle speed changing logic, and also
	void reorder(); //reput all the npcs into the turn queue, reset stuff for start of round
private:
	vector<NPC*> playerTeam; //the npcs in the player's team
	vector<NPC*> enemyTeam; //all the opponents
	vector<NPC*> everyone; //a list of everyone involved in the battle

	priority_queue<NPC*, vector<NPC*>, speedComparer> turn; //queue for finding whose turn it is
	map<NPC*, int> knownspeeds; //maps npcs to their speed so we can detect if the speed has changed, and not do the turn if it's a different speed
	set<NPC*> went; //everyone who already went this round, in order to account for speed shenanigans and also multiposition

	vector<NPCEffect*> alleffects; //a list of all the effects affecting their npcs
	vector<tuple<NPC*, NPC*, Effect*>> bonds; //effects given to npc 1 to npc 2 so when npc 1 is incapacitated, we know to remove the effect from npc 2

	vector<Item*>* inventory; //pointer to the player's inventory

	NPC* player; //the original player object

	bool escapable; //if you can run from this battle

	int xpReward = 0; //how much xp is given to the player party for winning
	int monyReward = 0; //how many monies are given to the player party for winning

	const char* flavorText[8] = { //flavor text which is randomly printed when asking for HELP
		"You are locked the heck in.",
		"You ask your opponent what the valid command words are. They look at you really confused.",
		"You got this.",
		"You are beginning to believe.",
		"You forgor.",
		"You all take your legally mandated lunch break.",
		"You wonder why everyone just agreed to attack each other in an orderly fashion.",
		"You get distracted by a squirrel."
	};

	const char* validCommands[9] = { //the valid commands that are printed when asking for HELP
		"[attack] [npc]",
		"USE [item] (ON [npc])",
		"INVENTORY",
		"PARTY",
		"ENEMIES",
		"ATTACKS",
		"ANALYZE [npc/item]",
		"HELP",
		"RUN"
	};

	const char* idleText[5] = { //flavor text for dummies not doing anything
		"hanging in there.",
		"hanging out.",
		"still there.",
		"being incredible.",
		"keeping it frosty."
	};
};

#endif