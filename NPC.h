//header file for NPCs

//npcs can be recruited (bool for if they want to be recruited)
//when recruited, they follow you around and are not listed in the description of a room
//recruited npcs and their stats can be listed using the PARTY command
//recruited npcs can be dismissed, and they will then go back home
//you can have up to 3 npcs in your party

#ifndef NPC_H
#define NPC_H

#include <vector>
#include <stack>
#include "Room.h"
using namespace std;

class Room; //forward declares room because these two classes reference each other

class NPC {
public:
	NPC();
	~NPC();
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

	bool recruitable = false;
	bool recruited = false;

	int health;
	int defense;
	int attack;
	int speed; //speed determines the order of movement; it's more like reaction time

	int level;
	int maxLevel = 100;
};
#endif