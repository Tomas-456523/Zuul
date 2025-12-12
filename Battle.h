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

class Battle {
public:
	Battle(vector<NPC*>* _playerTeam, vector<NPC*>* _enemyTeam, vector<Item*>* _inventory, int& mony, bool _escapable = true);
	~Battle();

	int FIGHT(); //starts the battle process and returns an int based on the outcome

	bool useItem(char* itemname); //use an item in the player's inventory
	void printTeam(vector<NPC*>& team, bool printLevel = true, bool printSP = false, bool printFainted = true); //print everyone in the given team
	void printInventory(); //print the contents of the inventory
	void printParty(); //print everyone in the player's party
	void printEnemies(); //print all the opponents
	void analyze(char* name); //print the data of the given item or npc
	void printHelp(); //print the valid command words and extensions
	bool runAway(); //try to escape the battle
	void tickEffect(Effect& effect); //ticks the given effect
	void tickEffects(); //ticks all effects on every npc
	void BURGERProcess(NPC* self, NPC* burger, int id); //what the burger man uses to talk in battle
	void carryOutAttack(Attack* attack, NPC* attacker, NPC* target); //affects the given target based on the given attack
	bool ParseAttack(NPC* plr, char* commandP, char* commandWordP, char* commandExtensionP, int checkMax = 1); //interpret and carry out an attack command given by the player
	bool playerTurn(NPC* plr); //the player chooses what to do here
	void npcTurn(NPC* npc); //the npc chooses an attack to do
	
	void addNPC(NPC* npc); //creates a new npc mid-battle

	int getXpReward(); //gets how much xp was earned as a result of a victory
	int getMonyReward(); //gets how many monies were earned as a result of a victory

	void reorder(queue<NPC*>& orderly_fashion); //puts all the npcs in the queue in order of speed
private:
	vector<NPC*> playerTeam; //the npcs in the player's team
	vector<NPC*> enemyTeam; //all the opponents
	vector<NPC*> everyone; //a list of everyone involved in the battle

	vector<Item*>* inventory; //pointer to the player's inventory

	NPC* player; //the original player object

	bool escapable; //if you can run from this battle

	int xpReward = 0; //how much xp is given to the player party for winning
	int monyReward = 0; //how many monies are given to the player party for winning

	char flavorText[8][255] = { //flavor text which is randomly printed when asking for HELP
		"You are locked the heck in.",
		"You ask your opponent what the valid command words are. They look at you really confused.",
		"You got this.",
		"You are beginning to believe.",
		"You forgor.",
		"You all take your legally mandated lunch break.",
		"You wonder why everyone just agreed to attack each other in an orderly fashion.",
		"You get distracted by a squirrel."
	};

	char validCommands[9][255] = { //the valid commands that are printed when asking for HELP
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
};

#endif