//header file for NPCs
//this name is misleading because the yes player character also uses this class

//npcs can be recruited (bool for if they want to be recruited)
//when recruited, they follow you around and are not listed in the description of a room
//recruited npcs and their stats can be listed using the PARTY command
//recruited npcs can be dismissed, and they will then go back home
//you can have up to 3 npcs in your party, not including yourself
//

#ifndef NPC_H
#define NPC_H

#include <vector>
#include <stack>
#include "Room.h"
using namespace std;

class Room; //forward declares room because these two classes reference each other

class NPC {
public: //you need to set stats on creation
	NPC(char _name[255], char _description[255]);
	~NPC();

	char* getName(); //gets the name of the character
	char* getDescription(); //gets the description of the character
	char* getDialogue(); //gets the current dialogue
	char* getBattleCry(); //gets a random battle cry
	char* getAttackDescription(); //gets the description of the npc's attack
	bool getRecruitable(); //gets the recruitable status of the character
	bool getRecruited(); //gets the recruited status of the npc (if they're already in the party)
	bool getPlayerness(); //gets if the character is a player character (true) or if they're truly an npc (false)
	int getHealth(); //gets the current health of the npc
	int getHealthMax(); //gets the maximum health of the npc
	int getDefense(); //gets the defense of the npc
	int getAttack(); //gets the damage the npc can deal
	int getToughness(); //gets the toughness of the npc
	int getPierce(); //gets the pierce of the npc
	int getSpeed(); //gets the speed of the npc

	void addDialogue(); //adds temporary dialogue to the npc
	void addRandomDialogue(); //adds random dialogue to the npc
	void addBattleCry(); //adds a random battle cry to the npc
	void setAttackDescription(); //sets the description of the attack
	void setRecruitable();
	void setRecruited();
	//the game never needs to set stats for the characters past creation, but they can be upgraded with these methods
	void addHealth(); //adds to the maximum and current health
	void addDefense(); //adds to the defense
	void addAttack(); //adds to the attack
	void addToughness();
	void addPierce();
	void addSpeed();
private:
	char name[255];
	char description[255];

	Room* home;
	Room* currentRoom;

	//the dialogue that the character has to go through before getting to the random dialogue
	stack<char*> initialDialogue;

	//random dialogue will be chosen from this vector after the player has exhausted all initial dialogue
	vector<char*> randomDialogue;
	//the npcs say a random one of these phrases when attacking
	vector<char*> battleCries;

	char attackDescription[255]; //describes how the npc is attacking (eg. shot a laser at)

	bool recruitable = false;
	bool recruited = false;
	bool isPlayer;

	int health;
	int maxHealth;
	int defense;
	int attack;
	int toughness;
	int pierce;
	int speed; //speed determines the order of movement; it's more like reaction time
};
#endif