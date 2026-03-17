//header file for world chnages, since both npcs and world change items do this

#ifndef WORLDCHANGE
#define WORLDCHANGE

#include <queue>
#include <utility>
#include "WorldState.h"
#include "Conversation.h"
#include "Stats.h"

class Room;
class NPC;
class Item;
struct Attack;

struct WorldChange {
    std::queue<NPC*> recruitLinks; //these npcs are set to recruitable
	std::queue<std::pair<NPC*, Conversation>> linkedConversations; //we add these conversations to the npc
	std::queue<std::pair<NPC*, Conversation>> linkedDialogue; //we set the linked npcs' dialogue to this
	std::queue<std::pair<NPC*, const char*>> linkedTitles; //we reset these npc's titles
	std::queue<std::pair<NPC*, const char*>> linkedDescriptions; //we reset these npc's descriptions

	std::queue<std::pair<NPC*, Room*>> defeatRooms; //move these npcs to these rooms

	std::queue<std::pair<Room*, const char*>> roomChanges; //we edit the description of these rooms to these descriptions
	std::queue<std::pair<Room*, Room*>> redirectRooms; //the first rooms are redirected to the second rooms

	std::queue<std::pair<NPC*, Stats>> linkedStats; //stat changes
	std::queue<std::pair<NPC*, Attack*>> removeAttacks; //attacks to remove from the npc

    std::queue<Item*> guardedItems; //remove these items' guards so they're takable/usable

	size_t worldcon = Helper::NEVER; //world condition that this affects, unless it's NEVER then it doesn't do anything
};

#endif