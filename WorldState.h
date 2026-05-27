/* Tomas Carranza Echaniz
*  5/26/26
*  This header file is an extension of the Helper namespace, but it's in a seperate file due to 
*  circular dependency with Conversation.h.
*  
*  World conditions are stored in the external WorldState bool array, which is accessed using the
*  enumerators below. They represent various states of the world, such as bosses that have been
*  beaten, items that have been used, events that are currently occuring, etc. These are mainly
*  used by conversations to not say outdated or inappropriate information, but are also used by
*  things such as conditional world changes, checking if we ended the game automatically, etc.
*  It's just a convenient way to track things.
*/

#ifndef WORLDCONDITION
#define WORLDCONDITION

namespace Helper {
	enum WorldCondition {
		ISBERNARD,
		RECOILED,
		TAMEDLOBSTER,
		VIOLAREC,
		VALVEUSED,
		SWITCHUSED,
		LAVADRAINED,
		NOGAMBLING,
		BEATRATMAN,
		BEATMARGE,
		JILLYQUEST,
		CLOAKED,
		BEATCEO,
		FREEBURGER,
		SEENUNIHORN,
		MADEKATANA,
		SAVINGJILLY,
		JILLYSAVED,
		TEMPLEQUEST, //otherwise known as DESTROY BURGER QUEST
		BURGERCHASE,
		IMPRISONED,
		ESCAPEDBASE,
		CANDISMISS, //if you're currently allowed to dismiss your teammates
		GOTPLOTDEVICE,
		BURGERMANDEF,
		BURGERMENDEF,
		GAMEEND, //when the player reaches an ending, this is not carried over when loading saves
		NEVER //this is a sentinel; never set never to true
	};

	extern bool WorldState[NEVER+1]; //the state of the world! (must be as sized according to the last condition in WorldCondition, works since enumerators are technically ints!)
}

#endif