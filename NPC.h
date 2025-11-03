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
#include "Fighter.h"
using namespace std;

class Room; //forward declares room because these two classes reference each other
struct Fighter;

class NPC {
public: //you need to set stats on creation
	NPC(const char _title[255], const char _name[255], const char _description[255], Room* room, int _health, int _defense, int _attack, int _toughness, int _pierce, int _speed, int _level = 0, bool _player = false);
	~NPC();

	char* getTitle(); //gets the title of the character
	char* getName(); //gets the name of the character
	char* getDescription(); //gets the description of the character
	char* getDialogue(); //gets the current dialogue
	char* getRejectionDialogue(); //gets the rejection dialogue for the npc
	char* getRecruitmentDialogue(); //gets the recruitment dialogue for the npc
	char* getDismissalDialogue(); //gets the dismissal dialogue for the npc
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
	Room* getHome(); //gets the home location of the npc
	int getLevel(); //gets the level of the npc
	int xpForNextLevel();
	vector<Fighter>* getParty();

	void setDialogue(const char _dialogue[255]); //sets the dialogue for the npc
	void setRejectionDialogue(const char _dialogue[255]); //sets the rejection dialogue for the npc
	void setRecruitmentDialogue(const char _dialogue[255]); //sets the recruitment dialogue for the npc
	void setDismissalDialogue(const char _dialogue[255]); //sets the dismissal dialogue for the npc
	void addBattleCry(); //adds a random battle cry to the npc
	void setAttackDescription(); //sets the description of the attack
	void setRecruitable(bool _recruitable);
	void Recruit();
	void Dismiss();
	void setRoom(Room* _room);
	//the game never needs to set stats for the characters past creation, but they can be upgraded with these methods
	void addXp(int _xp);
	void addMember(Fighter member);

protected:
	char title[255]; //the title of the character (eg. VILLAGE ELDER)
	char name[255];
	char description[255];

	Room* home;
	Room* currentRoom;

	vector<Fighter> party;
	Fighter self;

	//dialogue that the npc says when asked
	char dialogue[255];
	char recruitedDialogue[255]; //dialogue that the npc says while recruited
	char rejectionDialogue[255];
	char recruitmentDialogue[255];
	char dismissalDialogue[255];
	//the npcs say a random one of these phrases when attacking
	char battleCry[255];

	char attackDescription[255]; //describes how the npc is attacking (eg. shot a laser at)

	bool recruitable = false;
	bool recruited = false;
	bool isPlayer;
	
	int level;
	int xp;
};
#endif