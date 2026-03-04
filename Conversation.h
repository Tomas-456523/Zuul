//header file for conversations

#ifndef CONVERSATION
#define CONVERSATION

#include <queue>
#include "Helper.h"

struct Conversation {
    std::queue<const char*> lines;
    std::queue<const char*> altlines;

    int skipcondition; //if we should skip this, based on WorldConditions in Helper
    bool altdialogue; //if skipcondition is true, if we should use alternate dialogue instead of just skipping the conversation

    bool getOutdated() {
        if (skipcondition && *skipcondition && !altdialogue) return true;
        return false;
    }

    std::queue<const char*> getDialogue() {
        if (skipcondition && *skipcondition)
    }
}

#endif