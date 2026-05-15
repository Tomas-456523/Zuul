//header file for these thingies, en extension of Helper.h, but must be seperate file due to Conversation needing to know these but not being able to include Helper.h due to the whole circular dependency situation

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
		LOBSTERPRISON,
		ESCAPEDBASE,
		CANDISMISS, //if you're currently allowed to dismiss your teammates
		GOTPLOTDEVICE,
		BURGERMANDEF,
		BURGERMENDEF,
		POSTGAME,
		GAMEEND, //when the player reaches an ending, this is not carried over when loading saves
		NEVER //never set never to true
	};

	extern bool WorldState[NEVER+1]; //the state of the world! (must be as sized according to the last condition in WorldCondition, works since enumerators are technically ints!)
}

#endif