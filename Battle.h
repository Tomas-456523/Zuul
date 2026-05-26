/* Tomas Carranza Echaniz
*  5/25/26
*  This is the header file for battles
*  
*  After constructing a Battle, the combat process is started by the FIGHT() function. It loops through
*  all the npcs in order of speed, and from there branches out to either the NPC or player turn functions
*  depending on the playerness of the currently going character. All attacks converge into carryOutAttack,
*  which calls hitTargets (handles spread attacks), and finally ends up at hitTarget, for applying the
*  attack's damage and further effects to the reciever. carryOutAttack also handles attack effects not
*  directly affecting the target. The player's command, if not matching the preset ones, will then go
*  through ParseAttack, which will check if the player is trying to launch an attck, and give an error if
*  the command turned out to be uninterpretable.
*/

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
		return a->getSpeed()*a->getSpeedMultiplier() < b->getSpeed()*b->getSpeedMultiplier();
	}
};

class Battle {
public:
	Battle(vector<NPC*>* _playerTeam, vector<NPC*>* _enemyTeam, vector<Item*>* _inventory, bool _escapable = true, bool scaleenemies = true);
	~Battle();

	int FIGHT(); //starts the battle process and returns an int based on the outcome

	bool useItem(const char* itemname, NPC* plr); //use an item in the player's inventory
	void printTeam(vector<NPC*>& team, bool printLevel = true, bool printSP = false, bool printFainted = true); //print everyone in the given team
	void printInventory(); //print the contents of the inventory
	void printParty(); //print everyone in the player's party
	void printEnemies(); //print all the opponents
	void analyze(const char* name); //print the data of the given item or npc
	bool runAway(); //try to escape the battle
	void checkEffects(NPC* npc); //checks all the npc's tracked effects for if they're at duration 0
	void attachEffect(NPCEffect* effect); //handle stuff that happens when setting effects
	void detatchEffect(NPCEffect* effect); //handle stuff that happens when removing effects
	void handleKnockout(NPC* npc); //handles stuff related to when the npc gets incapacitated
	void hitTarget(Attack* attack, NPC* attacker, NPC* reciever, int hits, bool parry = false); //hit one of the targets from hitTargets
	void hitTargets(NPC* attacker, Attack* attack, vector<NPC*>& tarparty, int tarPos); //hit the target, and surroundings if needed
	void carryOutAttack(Attack* attack, NPC* attacker, NPC* target, bool recoil = false); //affects the given target based on the given attack
	bool ParseAttack(NPC* plr, char* commandP, char* commandWordP, char* commandExtensionP, int checkMax = 3); //interpret and carry out an attack command given by the player
	vector<NPC*>& getTarTeam(NPC* npc, Attack* attack); //find which team the attack is supposed to target based on the given npc
	vector<NPC*> getTargets(NPC* npc, Attack* attack); //get a vector of valid targets that we can target with the attack
	bool playerTurn(NPC* plr); //the player chooses what to do here
	Attack* chooseAttack(NPC* npc); //choose an attack for the npc based on their precalculated attack weights
	void npcTurn(NPC* npc, bool opener = false); //the npc chooses an attack to do
	void printVersus(size_t wave); //prints the starting text or wave text
	void setupWave(bool pteam, size_t wave, bool scaleteam); //sets up a new wave for the given side
	void checkOpeners(const vector<NPC*>& checks); //check the given npcs for if they have opening moves
	void checkFightEffects(); //check the enemy leader for if they have any team effects to apply to the player team
	bool doBusinessProposition(NPC* sos, NPC* plr, bool faint); //do the business proposition, which is a very specific choice sequence for the forest temple boss, and return what the player chose
	
	NPC* addNPC(NPC* npc, NPC* summoner = NULL, bool altteam = false); //creates a new npc mid-battle
	void numberNPC(NPC* npc, const vector<NPC*>& team); //adds a number to the end of an npc's name to account for duplicates

	void buildReward(NPC* enemy, bool summon = false); //build up the reward for beating the fight depending on the npc passed
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

	map<Effect*, set<NPC*>> donteffect; //track which effects we don't want to apply on these npcs anymore
	bool beenhypno = false; //if anybody has been hypnotized in this battle, used by the forest temple boss in the business proposition

	vector<Item*>* inventory; //pointer to the player's inventory

	NPC* player; //the original player object
	NPC* enemy; //the original enemy object

	Effect* leadeffect = NULL; //if the fight has fight effects, we store them so we can apply them to player team summons
	Effect* teameffect = NULL;

	bool givemonies = true; //flags which are disabled if we manually set the leader to give 0 monies or xp respectively, to be consistent with the intent
	bool givexp = true;

	size_t pwave = 0;
	size_t ewave = 0;

	int enemycount = 0; //tracks how many non-boss enemies have been in the battle, just for rewards calculation

	bool escapable; //if you can run from this battle
	bool scaleEnemies; //if we should scale the enemies to match their world level (for world fights)

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