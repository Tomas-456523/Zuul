//header file for conversations

#ifndef CONVERSATION
#define CONVERSATION

#include <vector>
#include <utility>
#include <iostream>
#include <initializer_list>
#include "WorldState.h"

class NPC;

struct Conversation {
	std::vector<std::pair<NPC*, const char*>> lines;
	Conversation* alt; //alternate dialogue if the skipcondition is true

	int skipcondition; //if we should skip this, based on WorldConditions in Helper
	bool altdialogue; //if skipcondition is true, if we should use alternate dialogue instead of just skipping the conversation

	bool getOutdated() {
		if (Helper::WorldState[skipcondition] && !altdialogue) return true;
		return false;
	}

	bool empty() {
		return (Helper::WorldState[skipcondition] ? alt->empty() : lines.empty());
	}

	Conversation() : skipcondition(Helper::NEVER), altdialogue(false), alt(0) {}

	Conversation(std::initializer_list<std::pair<NPC*, const char*>> _lines) : lines(_lines), skipcondition(Helper::NEVER), altdialogue(false), alt(0) {}
};

#endif