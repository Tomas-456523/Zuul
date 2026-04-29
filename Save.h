//header file for the world saves

#include <vector.h>
#include "Helper.h"
#include "Room.h"
#include "NPC.h"
#include "Item.h"

struct Save {
	char data[1000]; //the save data
	
	/* SAVE DATA FORMAT:
	*  
	*  Starts with "BQ2" and ends with "="
	*  Sections are seperated by | and individual things in each section are seperated by ,
	*  Items, NPCs, and Rooms are accessed by their ID in their corresponding Helper vector (e.g. roomsH)
	*  Each section starts with a denoting letter followed by a '
	*  
	*  Section V: A character representing the save system version (for futureproofing)
	*  Section N: Player name in plaintext
	*  Section M: Monies
	*  Section W: Everything that can cause world changes in the order they happened in.
	*  - Items that were bought and therefore duplicated, denoted by b[item]
	*  - Item usage in which room, denoted by u[item]r[room] (including taking take to use world change items)
	*  - Items that were used up in battle, denoted by x[item]
	*  - NPCs that were defeated, denoted by d[npc]
	*  - NPCs that caused respawn changes, denoted by r[npc]
	*  - NPCs that were talked to, denoted by t[npc]
	*  - Enter changes, denoted by e[room]
	*  - Room welcomes that were triggered, denoted by w[room]
	*  - Dropped escape orbs, denoted by e[item]
	*  - Dropped and taken hose items, denoted by h[item] and j[item] respectively
	*  Section R: Stuff related to the player team
	*  - First, the teammate this chunk is tracking:
	*    a - Bernard	l - Jilly	h - Henry Jerry
	*    f - Floria		m - Mike	j - Michelin	g - Graham
	*    e - Egadwick	v - Viola	x - Carlos		r - Richie
	*    k - Absolom	c - Cacty	p - Plum		b - Ratman
	*  - Then level, xp, room, and finally, time left in gym (0 if not in gym)
	*  - Everything connected to the NPC is seperated by .
	*  - So, this section would look something like:
	*    R'f[level].[xp].[gym time]
	*    
	*  Section I: Every item's room, or NULL if it's in the inventory (must be in the same order it was saved in)
	*  - Denoted by [item].[room]
	*   
	*  Section L: Exists after taming lobster, stores its name in plaintext
	*  Section B: Exists after depositing money in the bank, stores last time used bank and current bank balance ([time],[balance])
	*  
	*  Example save: BQ2|V'0|N'BOB THE BUILDER|M'10|W'u123r23,x3244,e300|R'a1.3.123.0|I'12,234,324,53,12,4324,456|L'COOL LOBSTER|B'10,2=
	*/ 

	//returns an empty save for making a new game
	static Save NewGame() {
		return Save("BQ2=");
	}

	//gets the variables in main and serializes it into save data
	static Save SaveGame(std::vector<Item*>* inventory, int monies) {

	}

	//gets stuff which it can edit, and modifies the world to match the given save data
	static void LoadGame(const Save& save, NPC* player, std::vector<Item*>* inventory, int& monies) {
		//check for invalid saves (won't catch everything but you shouldn't be modifying save data anyway, this is mostly for accidentally copy/pasting wrong)
	}

	Save(const char* _data) { //make a new save with the given save data
		strcpy(data, _data);
	}
};