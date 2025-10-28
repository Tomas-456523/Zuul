#ifndef BATTLE
#define BATTLE

#include "Battle.h"
#include "NPC.h"
#include "Item.h"
using namespace std;

class NPC;
class Item;

class Battle {
public:
	Battle(vector<NPC*>* _playerTeam, vector<NPC*>* _enemyTeam, vector<Item*>* _inventory, int& mony, bool _escapable = true);
	~Battle();

	void FIGHT();

	void printTeam(vector<NPC*>* team);
	void printInventory();
	void printParty();
	void analyze();
	void printHelp();
	bool runAway();
	bool playerTurn();
private:
	vector<NPC*>* playerTeam;
	vector<NPC*>* enemyTeam;
	vector<NPC*> everyone;

	vector<Item*>* inventory;

	NPC* player;
	int mony;

	bool escapable;

	char flavorText[8][255] = {
		"You are locked the heck in.",
		"You ask your opponent what the valid command words are. They look at you really confused.",
		"You got this.",
		"You are beginning to believe.",
		"You forgor.",
		"You all take your legally mandated lunch break.",
		"You wonder why everyone just stands there and takes the hit.",
		"You get distracted by a squirrel."
	};

	char validCommands[7][255] = {
		"ATTACK [npc]",
		"USE [item]",
		"INVENTORY",
		"PARTY",
		"ANALYZE [npc/item]",
		"HELP",
		"RUN"
	};
};

#endif