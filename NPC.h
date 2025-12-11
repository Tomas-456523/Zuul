//header file for NPCs
//this name is misleading because the yes player character also uses this class

#ifndef NPC_H
#define NPC_H

#include <vector>
#include <queue>
#include <utility>
#include <map>
#include "Room.h"
#include "Effect.h"
#include "Attack.h"
using namespace std;

class Room; //forward declares room because these two classes reference each other
class Item;

class NPC { //for game testing purposes, all stats default to 20
public: //you need to set stats on creation
	NPC(const char _title[255], const char _name[255], const char _description[255], Room* room, int _health = 20, int _defense = 20, int _attack = 20, int _toughness = 20, int _pierce = 20, int _speed = 20, int _sp = 20, int _level = 0, bool _isleader = false, bool _player = false);
	~NPC();

	//a bunch of functions to get npc variables
	char* getTitle(); //gets the title of the character
	char* getName(); //gets the name of the character
	char* getDescription(); //gets the description of the character
	char* getDialogue(); //gets the current dialogue
	char* getRejectionDialogue(); //gets the rejection dialogue for the npc
	char* getRecruitmentDialogue(); //gets the recruitment dialogue for the npc
	char* getRecruitedDialogue();
	char* getDismissalDialogue(); //gets the dismissal dialogue for the npc
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
	int getSP(); //gets the current sp of the npc
	int getSPMax(); //gets the maximum sp of the npc
	Room* getHome(); //gets the home location of the npc
	Room* getRoom(bool alt = false); //get current room or the adjecent one it's blocking
	int getLevel(); //gets the level of the npc
	int xpForNextLevel();
	int xpForLevel(int level); //for the level from 0
	vector<NPC*>* getParty();
	bool getLeader(); //if npc is a leader
	bool getEscapable();
	int getXpReward(); //reward for beating them
	int getMonyReward();
	bool getEnemy(); //if npc is enemy
	Attack* getBasicAttack();
	vector<Attack*> getSpecialAttacks();
	map<Attack*, int> getWeights();
	bool getLevelUp(); //if we leveled up recently
	vector<int>& getStatChanges(); //the stat changes from the last level up
	Attack* getNewAttack();
	bool getDefeated(); //if npc has been defeated since entering the room
	Item* takeGift(); //return and nullify the gift item
	vector<Effect>& getEffects();
	int getHypnotized(); //hypnosis amount
	int getFrozen(); //freeze amount
	int getConvoSize(); //how many conversations are left to say
	bool getRespawn(); //if they respawn

	//bunch of functions for affecting npc variables
	void setDialogue(const char _dialogue[255]); //sets the dialogue for the npc
	void setGymDialogue(const char _dialogue[255]); //sets what the npc says in the gym
	void setRejectionDialogue(const char _dialogue[255]); //sets the rejection dialogue for the npc
	void setRecruitmentDialogue(const char _dialogue[255]); //sets the recruitment dialogue for the npc
	void setRecruitedDialogue(const char _dialogue[255]); //sets the recruited dialogue for the npc
	void setRecruitDialogueChange(const char _recruitment[255], const char _normal[255]); //sets new recruitment and normal dialogue after recruiting
	void setDismissalDialogue(const char _dialogue[255]); //sets the dismissal dialogue for the npc
	void setRecruitable(bool _recruitable); //set if you can recruit them
	void Recruit(); //set recurited to true
	void Dismiss(bool gohome = true); //dismiss them and go home if specified
	void setRoom(Room* _room); //move the npc
	void setHome(Room* room);
	void setParty(NPC* npc1 = NULL, NPC* npc2 = NULL, NPC* npc3 = NULL, NPC* npc4 = NULL);
	void setName(const char _name[255]);
	void setTitle(const char _title[255]);
	void setScale(int _health, int _defense, int _attack, int _toughness, int _pierce, int _speed, int _sp);
	void addXp(int _xp);
	void levelUp(bool trackLevelUp = false);
	void setLeader(bool _leader, int _level = 0, Room* room = NULL, bool respawn = true);
	int damage(float power, float pierce, int hits = 1);
	void directDamage(int damage, char* status = NULL);
	void setLevel(int _level); //only used for enemy parties
	void setBasicAttack(Attack* attack);
	void addSpecialAttack(Attack* attack);
	void blockExit(char* _exitBlocking, char* type, const char reason[255], bool bothsides = false);
	void setEscapable(bool _escapable);
	void calculateWeights();
	void setEnemy(bool _enemy);
	void alterSp(int amount, char* status = NULL);
	void setLevelUp(bool _leveledUp);
	void addSuffix(const char suffix[3]); //add suffix to end of npc name
	void setGuard(int _guard); //set guard to block attacks
	void setLink(NPC* npc, const char dialogue[255] = ""); //linked npc to affect on defeat
	void setLinkedRoom(Room* room, const char desc[255]); //room to affect on defeat
	void setGift(Item* item); //item to give when talking
	void setRedirect(Room* room1, Room* room2); //set room to redirect upon defeat
	void setEffect(Effect* effect, bool battle = true); //add an effect to the npc
	void removeEffect(Effect& effect);

	void addConversation(NPC* speaker, const char dialogue[255], bool newConversation = false); //add a conversation line to the npc, and make a new conversation if specified
	void addLinkedConvo(NPC* speaker, const char dialogue[255]);
	void printDialogue();
	void printDamage(int damage, char* status = NULL);
	void printEffects();

	bool getTalkOnDefeat(); //gets if the npc talks after being defeated
	void setTalkOnDefeat(bool talk = true); //sets if the npc talks after being defeated

	bool getForceBattle(); //gets if the npc forces battle after talking
	void setForceBattle(bool force = true); //set if the npc forces battle after talking

	bool getLobster(); //get if it's the lobster
	void setLobster(Room* tunnels, bool lobster = true); //set if it's a lobster plus set its home to the tunnels since it actually starts somewhere else

	char* getTunnelDirection(Room* room); //gets the direction back to the given room from the tunnels (for lobster only)
	void setTunnelDirection(Room* room, char* direction); //sets tunnel directions (for lobster only)

	void setDefeatNPC(const char newTitle[255], const char newDesc[255], const char newDialogue[255], Room* newRoom); //sets what the npc changes to after being defeated

	void defeat(); //set the enemy to defeated and do a bunch of defeated processes if applicable
	void undefeat(); //set the enemy to not defeated
protected:
	char title[255]; //the title of the character (eg. VILLAGE ELDER)
	char name[255]; //the name
	char description[255]; //npc's description when analyzed

	Room* home; //where the npc goes after being dismissed
	Room* currentRoom;

	vector<NPC*> party; //the npc's party if it is a leader

	Attack* standard_attack; //the npc's normal attack for generating sp
	vector<Attack*> special_attacks; //the npc's special attacks that cost sp

	vector<Effect> effects; //the effects affecting this npc
	map<Attack*, int> attackWeight; //the weight of the npc's attacks

	queue<vector<pair<NPC*,const char*>>> conversations; //npcs can have discussions with the player character, and they're stored as a queue of vectors of pairs of dialogue and the npc that spoke it

	char dialogue[255]; //dialogue that the npc says when asked, and no conversations are left
	char recruitedDialogue[255]; //dialogue that the npc says while currently recruited
	char rejectionDialogue[255]; //dialogue that the npc says when rejecting recruitment offer
	char recruitmentDialogue[255]; //dialogue that the npc says when recruited
	char dismissalDialogue[255]; //dialogue that the npc says when dismissed
	char gymDialogue[255]; //dialogue the character says when at the gym

	char newRecruitmentDialogue[255]; //the recruitment and normal dialogues get changes to this after being recruited for the first time
	char newDialogue[255];

	Item* gift; //item that the npc holds and gifts to the player after talking

	bool recruitable = false;
	bool recruited = false; //if the npc is in the player party
	bool isPlayer;
	bool isLeader;
	bool isEnemy = false;

	bool isLobster = false; //if it's the lobster
	map<Room*, char*> tunnelLinks; //tunnel links for setting them to get back from the tunnels  if it's the lobster

	//npc stats
	int health; //current health
	int maxHealth;
	int defense;
	int attack;
	int toughness; //how much defense resists attack and pierce
	int pierce; //how much defense is ignored
	int speed; //speed determines the order of movement in battle
	int sp; //sp stands for skill points
	int maxSP;
	
	int level = 0;
	int xp = 0; //how much xp the npc has stored up

	bool leveledUp = false; //if the npc leveled up
	vector<int> statChanges = {0, 0, 0, 0, 0, 0, 0};
	Attack* newAttack; //the newest attack we got

	int healthScale = 0; //how much each stat increases (minimum) each level up
	int defenseScale = 0;
	int attackScale = 0;
	int toughnessScale = 0;
	int pierceScale = 0;
	int speedScale = 0;
	int spScale = 0;

	int hypnosis = 0; //how hypnotized the npc is
	int freeze = 0; //how frozen the npc is
	bool defeated = false; //if the npc is defeated (appears in the room)
	bool respawns = true; //if the npc appears again after battle

	bool forcebattle = false; //if we force the player to battle after talking
	bool talkOnDefeat = false; //if the npc talks when defeated
	bool defeatChange = false; //if the npc changes after defeating them

	char* exitBlocking = NULL; //enemy npcs may block an exit until they are defeated
	Room* altRoom = NULL; //enemies block exits from both sides, so they have to be in two rooms at the same time, unfortunately
	NPC* linkedNPC = NULL; //we set this npc to recruitable when robot is defeated
	vector<pair<NPC*, const char*>> linkedConversation; //we add this conversation to the linked npc when defeated
	char linkedDialogue[255]; //we set the linked npc's dialogue to this when defeated (if it isn't "")

	Room* linkedRoom = NULL; //we edit the description of this room after being defeated
	char linkedDescription[255]; //the description gets set to this

	//some npcs change when defeated so they set their variables to this stuff:
	char defeatDialogue[255];
	char defeatTitle[255];
	char defeatDescription[255];
	Room* defeatRoom;

	bool escapable = true; //if you can escape from this enemy in a battle
	int guard = 0; //how many hits the npc can block before guard is broken

	pair<Room*, Room*> redirectRoom; //after defeat, the first room is redirected to the second room

	//int xpReward;
	//int monyReward;
};
#endif