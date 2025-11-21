#ifndef BATTLE
#define BATTLE

#include <queue>
#include "Battle.h"
#include "NPC.h"
#include "Item.h"
#include "Effect.h"
using namespace std;

class NPC;
class Item;

class Battle {
public:
	Battle(vector<NPC*>* _playerTeam, vector<NPC*>* _enemyTeam, vector<Item*>* _inventory, int& mony, bool _escapable = true);
	~Battle();

	int FIGHT();

	bool useItem(char* commandExtensionP);
	void printTeam(vector<NPC*> team, bool printLevel = true, bool printSP = false);
	void printInventory();
	void printParty();
	void printAttacks(NPC* npc);
	void analyze();
	void printHelp();
	bool runAway();
	bool ParseAttack(char* commandP, char* commandWordP, char* commandExtensionP, bool checkDouble = true);
	bool playerTurn();
	void npcTurn(NPC* npc);

	queue<NPC*> reorder();
private:
	vector<NPC*> playerTeam;
	vector<NPC*> enemyTeam;
	vector<NPC*> everyone;

	vector<Effect> allEffects;

	vector<Item*>* inventory;

	NPC* player;
	int mony;

	bool escapable;

	int xpReward;
	int monyReward;

	char flavorText[8][255] = {
		"You are locked the heck in.",
		"You ask your opponent what the valid command words are. They look at you really confused.",
		"You got this.",
		"You are beginning to believe.",
		"You forgor.",
		"You all take your legally mandated lunch break.",
		"You wonder why everyone just agreed to attack each other in an orderly fashion.",
		"You get distracted by a squirrel."
	};

	char validCommands[8][255] = {
		"[attack] [npc]",
		"USE [item]",
		"INVENTORY",
		"PARTY",
		"ATTACKS",
		"ANALYZE [npc/item]",
		"HELP",
		"RUN"
	};
};

#endif