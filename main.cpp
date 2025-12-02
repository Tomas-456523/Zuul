/* Tomas Carranza Echaniz
*  This program is going to be the most peak Zuul that has ever zuuled
* 
*/

/*
FEATURES I NEED TO IMPLEMENT
- BETTER DIALOGUE
- TURN-BASED COMBAT
- SAVE SYSTEM (OOOONNNNNNLLLLLYYY IF YOU HAVE TIME)
- DYNAMIC HELP SYSTEM?
- ITEMS
- FINALIZE THE PLOT (almost done)
- ACHIEVEMENTS (ehhhhhh idk anymore consider it if time permits)
- QUEST SYSTEM (like, to make npcs recruitable)
- FISHING MINIGAME
- RECAP COMMAND?
*/
/*
If you get the lame ending it gives a reference to the "Don't be lame clause"
says:
Wow, that was lame...
Maybe you should explore more to get a less lame ending...

	    <<< BURGER QUEST COMPLETE ? >>>    
		<<< THANK YOU FOR PLAYING ! >>>

ENDING ACHIEVED: LAME ENDING

... and then you get booted to the main menu like in hollow knight
*/
#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
#include "NPC.h"
#include "Room.h"
#include "Item.h"
#include "Battle.h"
#include "Helper.h"
#include "Attack.h"
#include "Effect.h"

using namespace std;
using namespace Helper;

//sets up the entire game world, including rooms, npcs, and items, and returns the player character
NPC* SetupWorld(vector<Room*>* rooms) {
	//set up directions
	char* NORTH = new char[12];
	char* SOUTH = new char[12];
	char* WEST = new char[12];
	char* EAST = new char[12];
	char* NORTHWEST = new char[12];
	char* NORTHEAST = new char[12];
	char* SOUTHWEST = new char[12];
	char* SOUTHEAST = new char[12];
	char* UP = new char[12];
	char* DOWN = new char[12];
	char* OUT = new char[12];
	char* IN_TENT_1 = new char[12];
	char* IN_TENT_2 = new char[12];
	char* IN_TENT_3 = new char[12];
	char* IN_TENT = new char[12];
	char* IN_HOUSE_1 = new char[12];
	char* IN_HOUSE_2 = new char[12];
	char* IN_HOUSE_3 = new char[12];
	char* IN_FACTORY = new char[12];
	char* IN_CASTLE = new char[12];
	char* UPSTAIRS = new char[12];
	char* DOWNSTAIRS = new char[12];
	char* UNDERGROUND = new char[12];
	char* FORWARD = new char[12];

	//set the text for the directions
	strcpy(NORTH, "NORTH");
	strcpy(SOUTH, "SOUTH");
	strcpy(WEST, "WEST");
	strcpy(EAST, "EAST");
	strcpy(NORTHWEST, "NORTHWEST");
	strcpy(NORTHEAST, "NORTHEAST");
	strcpy(SOUTHWEST, "SOUTHWEST");
	strcpy(SOUTHEAST, "SOUTHEAST");
	strcpy(UP, "UP");
	strcpy(DOWN, "DOWN");
	strcpy(OUT, "OUT");
	strcpy(IN_TENT_1, "IN TENT 1");
	strcpy(IN_TENT_2, "IN TENT 2");
	strcpy(IN_TENT_3, "IN TENT 3");
	strcpy(IN_TENT, "IN TENT");
	strcpy(IN_HOUSE_1, "IN HOUSE 1");
	strcpy(IN_HOUSE_2, "IN HOUSE 2");
	strcpy(IN_HOUSE_3, "IN HOUSE 3");
	strcpy(IN_FACTORY, "IN FACTORY");
	strcpy(IN_CASTLE, "IN CASTLE");
	strcpy(UPSTAIRS, "UPSTAIRS");
	strcpy(DOWNSTAIRS, "DOWNSTAIRS");
	strcpy(UNDERGROUND, "UNDERGROUND");
	strcpy(FORWARD, "FORWARD");

	//set up blockage reaons
	char* ENEMY = new char[12];
	char* CHASM = new char[12];
	char* RIVER = new char[12];
	char* TEMPLE = new char[12];
	char* RUBBLE = new char[12];
	char* TUNNEL = new char[12];
	char* LOCK = new char[12];
	char* MISC = new char[12];
	
	//set up blockage reason text
	strcpy(ENEMY, "ENEMY");
	strcpy(CHASM, "CHASM");
	strcpy(RIVER, "RIVER");
	strcpy(TEMPLE, "TEMPLE");
	strcpy(RUBBLE, "RUBBLE");
	strcpy(TUNNEL, "TUNNEL");
	strcpy(LOCK, "LOCK");
	strcpy(MISC, "MISC");

	//I send all the template enemy NPCs and also shop items, since I need to set a room for them
	Room* limbo = new Room("not supposed to be in this room; seriously how did you get here?");

	//for copy paste DELETE THIS LATER PLEASE: Room* village = new Room("");
	//create all WANING WOODLANDS rooms MARK: woodlands
	Room* village = new Room("in Tactical Tent Village. It's a beautiful day; perfect for staying indoors and gaming.");
	Room* villageleft = new Room("at the westernmost end of the village, where the tallest tent stands. It's only two stories, but it's comparatively a tent mansion.");
	Room* tentstore = new Room("in the village convenience store. No other store is more convenient, or so they say.");
	Room* tentmansion = new Room("in the tent mansion's living room. There are way too many clocks here.");
	//put a telescope here, and maybe a launchpad outside which may or may bit do something
	Room* tentlab = new Room("in the tent lab. There's a ton of machinery, and many generic science beakers with colored liquids.");
	//(put a sign here) tunnels are shut down due to a lobster infestation
	Room* tentstation = new Room("in the village train station. The tunnels were closed off recently due to a lobster infestation.");
	//make the church get repaired post game and Archibald goes here
	Room* tentchurch = new Room("in the village church. It's a really big tent, complete with stained glass and everything. Nobody really goes here anymore, especially since the priest mysteriously disappeared a long time ago.");
	//NO FISHINH MINIGAME WHATSOEVER (ok so this place is useless then?)
	Room* docks = new Room("at the village docks. Nobody owns a boat; why do we have this.");

	Room* forestentrance = new Room("at the entrance of the woodlands.");
	Room* forest = new Room("deep in the woodlands. Smells like pine trees along the way.");
	forest->setWelcome("Welcome to WANING WOODLANDS!");
	forest->setWelcome("<<< THE FINAL FOREST >>>");
	forest->setWelcome("The slowly decaying corner of the world, where you have lived your whole life.");
	forest->setWelcome("But now, your BURGER QUEST begins!");
	
	Room* forestleft = new Room("walking down the left path of the woodlands. It feels like a deer is watching you.");
	Room* forestright = new Room("walking down the right path of the woodlands. It feels like a ninja is watching you.");

	Room* deerclearing = new Room("in the deer clearing, where deer frequently gather."); //talk to the deer to get the key
	Room* ninjaland = new Room("underneath the ninja village. It's probably supposed to be hidden but you looked up and there it was.");
	Room* ninjavillage = new Room("in the ninja village. There's no bridges connecting the houses, you must simply jump between them like a true ninja.");
	Room* ninjacapitol = new Room("in the chief ninja's abode. There are many weapons and scrolls up on the walls.");
	Room* ninjapantry = new Room("in the ninja storage unit. The ninjas live on a strict diet of NINJABERRIES and ninjasteak and ninjafish and the diet isn't actually that strict.");
	Room* ninjaforge = new Room("in the ninja forge. There are many molds for making weapons here, and a large shuriken mold is currently in use.");

	NPC* mrdeer = new NPC("", "MR. DEER", "Your friend MR. DEER. He's a deer.", deerclearing, 5, 2, 6, 0, 4, 20, 0, 5);
	Item* deerkey = new KeyItem("DEER KEY", "The key to the great forest wall.", "put the DEER KEY in the keyhole. The gate has been unlocked!", limbo, LOCK);
	mrdeer->setGift(deerkey);
	mrdeer->addConversation(self, "Hello MR. DEER!");
	mrdeer->addConversation(mrdeer, "(salutatory deer noises)");
	mrdeer->addConversation(self, "I'm going on a BURGER QUEST, but I can't get past the great forest wall without your key.");
	mrdeer->addConversation(mrdeer, "(warning deer noises)");
	mrdeer->addConversation(mrdeer, "(concerned deer noise)");
	mrdeer->addConversation(mrdeer, "(thinking deer noises)");
	mrdeer->addConversation(mrdeer, "...");
	mrdeer->addConversation(self, "I hope he gives me the key...");
	mrdeer->addConversation(mrdeer, "(reluctantly affirmative deer noise)");
	mrdeer->addConversation(self, "AYYY thank you so much MR. DEER!");

	//make sure the desc gets updated after plot device quest
	Room* foresttempleentrance = new Room("in the glade where the ancient forest temple stands.");
	Room* foresttemplestairs = new Room("on the steps that go into the ancient forest temple.");
	Room* foresttemple = new Room("in the temple of [SOMETHING]. [SOMETHING SOMETHING].");

	Room* flowerfield = new Room("in the aromatic flower fields. Your sister likes hanging out here.");
	Room* flowerfield2 = new Room("deep in the flower fields. A really nice river flows over here.");

	Room* forestfork = new Room("at another fork in the road. I hope we don't come across a spoon in the road.");
	Item* fork = new MaterialItem("FORK","\"An implement with two or more prongs used for lifting food to the mouth or holding it when cutting.\"\n- Oxford Languages", forestfork);

	Room* forestgarden = new Room("a strange section of the forest. All the trees have been neatly trimmed and organized to an unnatural extent.");

	Room* forestgate = new Room("at the great forest wall. It seperates the inner forest and the rest of the world.");
	Room* forestwall = new Room("walking along the southern side of the wooded wall. It's very misty here.");
	Room* forestgrave = new Room("in a sparse wooden graveyard.");

	Room* forestspork = new Room("at a spork in the road, but no sporks are to be seen anywhere.");

	Room* forestnice = new Room("at a really nice looking area of the forest. The lighting is very cool.");
	Room* treasuregrove = new Room("in the treasure grove, where the grass is golden and treasure chests sometimes appear.");
	Room* treasurecliff = new Room("in the treasure cliff at the edge of the desert, where there is actualy treasure.");

	Room* bossgrove = new Room("in the BOSS GROVE, where the boss of WANING WOODLANDS is known to appear.");

	Room* fdintermission1 = new Room("on the path between the woodlands and the wastelands.");
	Room* fdintermission2 = new Room("on the path between the woodlands and the wastelands. The foliage is sparse here. BURGERSBURG can be seen faintly in the distance.");
	Room* fdintermission3 = new Room("on the path between the woodlands and the wastelands. Dead trees surround you. The BURGER RESTAURANT is just barely visible from here.");

	//MARK: desert
	Room* desert = new Room("in the wastelands. There is no sign of life anywhere (except you!).");
	desert->setWelcome("Welcome to DESOLATE DESERT!");
	desert->setWelcome("<<< THE WASTELANDS BEYOND >>>");
	desert->setWelcome("The world beyond your forest, where the life has been sucked out of the dirt.");
	desert->setWelcome("The sun beats down with no clouds in sight.");
	desert->setWelcome("Surely there must be someone friendly around here?");
	
	Room* deserttempleentrance = new Room("on a large dune where an ancient desert temple pokes out of the sand. I'm sure nobody's ever been in this one, either.");
	Room* deserttemplestairs = new Room("on the steps that go into the ancient desert temple.");

	Room* desertdune = new Room("at a low point in the desert. A rare shadow is present where you can rest.");
	Room* desertplain = new Room("at a really flat area of the desert.");
	Room* desertgrave = new Room("deep in the desert. There's some sort of bovine skeleton here.");
	Room* desertpole = new Room("deep in the desert. All you can see for miles is sand.");
	Room* deserthill = new Room("on a very high dune. It'd be fun to roll all the way down, but your hair would be very annoying to clean afterwards.");

	Room* deserttown = new Room("in the abandoned desert village. It's strange; where did everyone go?");
	Room* desertshop = new Room("in an abandoned store. It would feel cozy if not for the smell of the expired products.");
	Room* desertgym = new Room("in an abandoned gym. There's all the equipment you could ask for here, but no rock wall.");
	Room* deserthouse = new Room("in some house with zero purpose. It only exists because the town would feel weird with only two houses.");

	Room* oasis = new Room("in an oasis, presumably the town's source of water, though the water is long gone.");
	Room* canyon = new Room("in a small canyon that cuts into a plateau. There's an entrance to an old mineshaft here.");
	Room* thatcliff = new Room("on this cliff over here.");

	Room* canyon1 = new Room("in a long, shaded canyon, forming YET ANOTHER fork in the road. You know, these would actually be really annoying forks if you think about it.");
	Room* canyon2 = new Room("at the end of the canyon, though a path seems to have been made upwards.");
	Room* canyon3 = new Room("at the end of the canyon. There's much evidence of mining here, and a mineshaft entrance here leads underground.");

	Room* cliff1 = new Room("very high up on a cliff; the temperature almost feels normal!");
	Room* cliff2 = new Room("on the highest cliff. You have a very good view of the volcanic, even wastier wastelands beyond the desert.");

	Room* mineshaft = new Room("underground in the mineshaft. It was too hot outside, but now it's very cold :(");
	Room* mineshaft2 = new Room("at a pickaxe in the road. That's not a utensil...");
	Item* pickaxe = new MaterialItem("PICKAXE","\"A tool consisting of a long handle set at right angles in the middle of a curved iron or steel bar with a point at one end and a chisel edge or point at the other, used for breaking up hard ground or rock.\"\n- Oxford Languages", mineshaft2);
	Room* mineshaft3 = new Room("deep in the mineshaft. A huge face spans the wall; he looks very interesting.");
	Room* minelight = new Room("pretty high up, though still underground. You can see the light at the end of the tunnel just up above; how lovely.");
	Room* mineexit = new Room("at the high-up exit of the mineshaft. SO MANY STAIRS...");
	Room* mineshaftside = new Room("in a side tunnel of the mineshaft. All the ores are left untouched; what were the miners looking for?"); //water that's what
	Room* kaboomroom = new Room("in a very unstable looking room. It's filled to the brim with dynamite, which is concerning considering that maniac that hangs out here.");

	Room* volcanoentrance = new Room("at the northernmost end of the mineshaft. Burning light and ash enter through the mineshaft entrance.");
	//volcano welcome: the burger restaurant's image is seen, distorted by the heat

	Room* desertstation = new Room("in a train station, which seems to have been accidentally intercepted by the mineshaft.");
	Room* deserttunnel = new Room("in a long, dark train tunnel. You've never seen one that goes on for so long with no collapse.");

	Room* minespring = new Room("at an underground spring, directly under the town oasis. There's many dead miners here.");

	//MARK: volcano
	Room* volcano = new Room("in the scorched highlands just before BURGERSBURG. It's very hot.");
	volcano->setWelcome("WELCOME TO HELLISH HIGHLANDS!");
	volcano->setWelcome("<<< THE END OF THE ROAD >>>"); //THE RING OF FIRE?
	volcano->setWelcome("The air burns your skin and ash fills your lungs.");
	volcano->setWelcome("The BURGER RESTAURANT can be seen clearly, but its image is heavily distorted by the heat.");
	volcano->setWelcome("You're almost there.");

	Room* volcano1 = new Room("in the volcanic highlands. An old, abandoned factory stands to the EAST.");
	Room* volcano2 = new Room("in a volcanic plain. The ashen remains of some plants can be seen here.");
	Room* volcano3 = new Room("in the scorched remains of an old factory town. I don't think there's any hope of this one's citizens coming back.\nThere's stairs leading down to a train station.");
	Room* volcanostation = new Room("in the factory town train station. Some lava has settled here, but the tunnels are probably still usable?");
	Room* volcano4 = new Room("on a scorched path. It reminds you of pepperoni pizza.");
	Room* volcano5 = new Room("at a... knife in the road.");
	Item* knife = new MaterialItem("KNIFE","\"An instrument composed of a blade fixed into a handle, used for cutting or as a weapon.\"\n- Oxford Languages", volcano5);
	Room* volcano6 = new Room("in a volcanic valley. Another factory stands here, more charred than the rest, yet still holding up.");
	Room* volcano7 = new Room("at the very edge of the volcanic valley. An huge old bridge forms a road to the city.");

	Room* factory1 = new Room("in the factory, on a small grated bridge over a pool of lava.");
	Room* factorykitchen = new Room("in a makeshift kitchen made from a factory observation room. Whatever was made here before, it now only churns out slag.");
	Room* controlroom1 = new Room("in the factory control room. The machinery is in rather good condtion.");

	Room* factory2 = new Room("in a factory. This one seems more suited for producing actual products.");
	Room* switchroom = new Room("in an assembly line observation room.");
	Room* conveyor1 = new Room("on an assembly line! You have to dodge the machinery lest you become a pancake.");
	Room* factorycenter = new Room("in the central room of the factory. There's some charred furniture here.");
	Room* conveyor2 = new Room("on an assembly line! This one makes a path to the upper factory.");
	Room* factorystorage = new Room("in second story of the factory. It looks like they handle packaging here.");
	Room* factorybalcony1 = new Room("on a balcony overlooking the entrance area.");
	Room* switchroom2 = new Room("in the observation area, for observing the second floor.");
	Room* conveyor4 = new Room("on an assembly line! This one makes a path between the main second floor rooms.");
	Room* conveyor5 = new Room("on an assembly line! This one makes a path to a central balcony area.");
	Room* factorybalcony2 = new Room("on a balcony overlooking the central area. You can also see a garden room of sorts from here, though there is no path to it.");
	Room* factorygarden = new Room("in a garden which is not wheelchair-accessible. The plants look very unhealthy; they seem to have been experimenting with nuclear technology here.");
	Room* conveyor3 = new Room("on an assembly line! This one makes a path to the control room.");
	Room* controlroom2 = new Room("in the factory control room. The machinery looks usable depsite the lava; props to whoever made it!");

	Room* factory3 = new Room("in the charred factory. The first floor is a large storage room, though its contents are long burnt to a crispy crisp.");
	Room* factorynw = new Room("in the factory storage room. There's what appears to be the remains of a chair here.");
	Room* factoryne = new Room("in the factory storage room. You can see the assembly line through a corner window, still going strong after so much time submerged in lava.");
	Room* factoryse = new Room("in the factory storage room. You see heavy machinery poking out of the EASTern room. I love heavy machinery!");
	Room* heavymachineryroom = new Room("in a room full of heavy machinery. Some of them are, incredibly, still operational.");
	Room* factoryroofsw = new Room("poking out of the factory. The roof has caved in here.");
	Room* factoryroofnw = new Room("high up in the factory. You can see the control room just up ahead.");
	Room* controlroom3 = new Room("in the factory control room. This one's machinery is very rusty. Very icky.");
	Room* factoryroofne = new Room("high up in the factory. You see a short hallway. It's a really nice hallway.");
	Room* factoryhallway = new Room("at the end of the hallway. A ladder leads high upwards.");
	Room* factorytower = new Room("at the top of the factory tower. It's full of computers and it smells like energy drink.");
	Room* factoryroofse = new Room("high up in the factory. You see a corridor full of employee of the month awards.");
	Room* factorytreasure = new Room("at the end of the corridor. JIM SHADY is the employee of the year. Good for him.");

	Room* castleentrance = new Room("at the entrance of a large, crystaline castle. It has a very angular design.");
	Room* castlehall = new Room("in the main hall of the castle. Ornate pillars and paintings of internet browsers pave the way towards the throne room.");
	Room* castlethrone = new Room("in the throne room of the castle. The ceiling is reeeeeeally high up, and the throne almost reaches the top!");
	castlethrone->setWelcome("PLUM - AHHHHHHHHHH HELP ME I'VE BEEN KIDNAPPED!");
	castlethrone->setWelcome("BROWSER - GWAHAHAHAHAHA!");

	Room* sewerentrance1 = new Room("on a ladder leading down to the sewer system.");
	Room* sewerentrance2 = new Room("deep down on the ladder to the sewer system.");
	Room* sewer = new Room("in the sewer system below the wastelands. Smells like you should see a pulmonologist.");
	Room* sewer2 = new Room("in the sewer system below the wastelands. Lava flows through the canal; better watch your step!");
	Room* sewercenter1 = new Room("in the huge central room of the sewers. The architecture here has green accents."); //south
	Room* sewercenter2 = new Room("in the huge central room of the sewers. A massive lake of lava lies under your feet."); //north
	Room* sewercenter3 = new Room("in the huge central room of the sewers. A natural stone wall and floor is exposed here."); //east
	Room* sewercenter4 = new Room("in the huge central room of the sewers. A large, green doorway stands here."); //west
	Room* sewerplant = new Room("in the main control room of the sewers. How does one maintain this lava maintenence system?");
	Room* sewermineswest = new Room("at the esternmost end of a long minecart track. You can't see the end.");
	Room* sewerminessouth = new Room("in a peculiar offshoot from the sewers which someone mined out.");
	Room* mineshortcut = new Room("in stable room, full of mining supplies and explosives.");

	Room* volcanotempleentrance = new Room("in a massive cavern, at the door of an ancient volcanic temple. The rock burns bright red.");

	Room* bridge1 = new Room("on the final bridge to BURGERSBURG. The lava ocean crackles beneath your feet.");
	Room* bridge2 = new Room("halfway through the bridge. An eternal night looms over the city.");
	Room* bridge3 = new Room("on the far end of the bridge. The gate into BURGERSBURG stands right up ahead.");

	//MARK: mountain
	Room* mountain = new Room("on a mountain, far, far away from anywhere you know. Dark clouds block view of both the sky and land.");
	mountain->setWelcome("WELCOME TO MOUNT MINBUS!");
	mountain->setWelcome("<<< THE EDGE OF THE WORLD >>>");
	mountain->setWelcome("Frigid winds blast your face and dark clouds cover both land and sky.");
	mountain->setWelcome("Ok have fun!");

	Room* mountainmine = new Room("far away from the start of the track. A barrage of snow blows in from the exit.");
	Room* mountain2 = new Room("on a little clearing on the mountain. The upwards path has a ski lift!");
	Room* mountainlake = new Room("at a lake full of baby shrimp. The shrimple aura keeps the water from freezing.");
	Room* mountain3 = new Room("at a sheer cliff. Whoever's in charge of this place should really invest in guardrails.");
	Room* mountainpeak = new Room("at the peak of the mountain, watching over a sea of clouds. You can't even see the BURGER RESTAURANT from here; it really is so far away.\nThere's a tent here in the style of your home village.");

	//it's a me
	Room* tenthome = new Room("in the developer's house.");
	NPC* developer = new NPC("DEVELOPER", "TOMAS", "The guy who made the game, except not really that guy because yeah.", tenthome, 1, 0, 1, 0, 0, 0, 0, 0);
	developer->setDialogue("Yo wassup.");
	developer->setRejectionDialogue("Nah, sorry. I don't think I would make a good teammate because I made my stats really low. I gotta stay humble, you know?");

	//MARK: BURGERSBURG
	Room* BURGERSBURG = new Room("at the gate of BURGERSBURG. The BURGER RESTAURANT is just down main street.");
	BURGERSBURG->setWelcome("Welcome to BURGERSBURG!");
	BURGERSBURG->setWelcome("<<< THE CAPITAL OF SIN >>>");
	BURGERSBURG->setWelcome("Massive skyscrapers blot out the sun, and the BURGER RESTAURANT towers above them all.");
	BURGERSBURG->setWelcome("All but a few buildings are boarded up.");
	BURGERSBURG->setWelcome("You don't want this city knowing where you live.");
	BURGERSBURG->setWelcome("We are not responsible for theft or damage to vehicles or contents!");

	Room* mainstreet1 = new Room("on main street. There's a few lopsided cars on fire here.");
	Room* mainstreet2 = new Room("on main street. You hear an explosion somewhere in the distance.");
	Room* mainstreet3 = new Room("on main street. You see a shady alley to the right."); //behind broken building "It's so close, but the BURGER RESTAURANT is hard to see with all the smoke
	Room* mainstreet4 = new Room("on main street. The traffic lights are all broken, but the random fires provide cozy lighting.");
	Room* mainstreet5 = new Room("at the end of main street. The BURGER RESTAURANT looms ahead of you.");

	Room* leftstreet1 = new Room("in the poor side of the city. It's relatively tranquil here since everything's already been stolen.");
	Room* leftstreet2 = new Room("next to a building with a fish sign. There is an uncharacteristic sense of calm here.");
	Room* leftstreet3 = new Room(".");
	Room* leftstreet4 = new Room("at the entrance to a glowing casino. One of the few functioning buildings here.");
	Room* leftstreet5 = new Room(".");

	//newstreet

	//coolstreet

	Room* rightstreet1 = new Room("in the crumbling corner of the city. The lava sea radiates light onto exposed infrastructure.");
	Room* rightstreet2 = new Room("next to a building with a fish sign. There is an uncharacteristic sense of calm here.");
	Room* rightstreet3 = new Room(".");
	Room* rightstreet4 = new Room(".");
	Room* rightstreet5 = new Room("at the entrance to the BURGERSBURG fire department. It probably hasn't seen much use recently.");

	Room* richneighborhood1 = new Room("in the rich people corner of town. They own nice little suburban houses.");
	Room* richneighborhood2 = new Room("in the rich neighborhood. You feel on guard with all the security systems looking at you. As soon as you step on a lawn, you'd probably be blown to smithereens.");
	Room* richneighborhood3 = new Room("at a secluded corner of the rich neighborhood. Some guy's standing outside. What a daredevil.");
	Room* richneighborhood4 = new Room("at the doorway of a huge bureaucratic-looking building. It's probably the second-tallest building in the city.");

	Room* burgfish = new Room("in the fish building. A warm light shines down the stairs.");
	Room* burgchurch = new Room("in a hidden church community. [descriptive text].");

	Room* burgstore = new Room("in an abandoned store. Nobody bothers to stock the shelves anymore."); //up exit but bernard gets scared?
	Room* casino = new Room("in the casino. Sounds of slot machines and flashing lights overload your senses. You should really leave before you develop a gambling addiction.");
	Room* darkalley = new Room("in a dark alley, a characteristic of those about to be mugged. Eh could be darker.");
	Room* shrimpartment1 = new Room("in an apartment building. There's a spiral staircase going all the way up.");
	Room* shrimpartment2 = new Room("on the second floor. The flowery wallpaper is peeling off.");
	Room* shrimpartment3 = new Room("on the third floor. You see an open doorway leading to an unfurnished room; your instincts pull you away from it.");
	Room* shrimproof = new Room("on the apartment rooftop. The pigeons are gone.");
	Room* firedepartment = new Room("in the fire department. The fire alarm is going off SOMEONE TURN IT OFF PLEASE MY EARS");

	Room* elevator = new Room("in the elevator of the BURGER RESTAURANT. It's one of those heavy-duty elevators.");
	Room* elevatortop = new Room("in the elevator, elevated all the way to the top. Once you go through the door, there is no going back.");
	elevatortop->setWelcome("");
	elevatortop->setWelcome("");
	elevatortop->setWelcome("");
	elevatortop->setWelcome("");
	elevatortop->setWelcome("");
	elevatortop->setWelcome("");
	//bunch of exposition

	Room* BURGERRESTAURANT = new Room("at the tippity top the BURGER RESTAURANT. You can see the sun barely shining under the horizon.\nThe BURGER MAN is waiting for you to order a BURGER.");
	Room* BURGERPRISON = new Room("in the BURGER PRISON. There one singular damp cell. It smells like BURGERs.");

	
	//MARK: set up NPCs
	//Attack copy/paste: Attack* ATTACK = new Attack("NAME", "DESCRIPTION", COST, POWER, PIERCE, MINHITS, MAXHITS, TARGETS);
	////////////////////////
	//DELETE THIS ATTACK, ONLY FOR TESTING
	Attack* finishhim = new Attack("", "DESCRIPTION", 0, 99999999, 99999999, 1, 1, 1);
	////////////////
	Attack* punch = new Attack("PUNCH", "punched", -5, 2, 0, 1, 1, 1);
	punch->addDescription("Throw a simple punch at the target.");

	Attack* energyball = new Attack("ENERGY BALL", "threw an energy ball at", 5, 10, 2, 1, 1, 1, 0);
	energyball->addDescription("Throw a ball of pure kinetic energy at the target.");

	Attack* precisionstrike = new Attack("PRECISION STRIKE", "", 10, 35, 15, 1, 1, 1, 10);
	precisionstrike->addDescription("Launch a heavy mass of energy speedily towards the target.");

	//SET START ROOM TO VILLAGE
	NPC* self = new NPC("\0", "SELF", "It's a me.", forestentrance, 20, 5, 6, 0, 0, 10, 5, 0, true, true);
	self->setScale(1, 1, 1, 0, 0, 1, 1);
	self->setDialogue("Huh?");
	self->Recruit();
	self->setBasicAttack(punch);
	self->addSpecialAttack(finishhim);
	self->addSpecialAttack(precisionstrike);

	NPC* floria = new NPC("FLOWER GIRL", "FLORIA", "Your little sister who gets along well with nature, especially flowers. She has a flower-shaped hat.", flowerfield2, 16, 5, 4, 0, 1, 5, 2);
	floria->addConversation(floria, "Hey big brother! Aren't these flowers just so lovely?");
	floria->addConversation(self, "NO THESE FLOWERS SUCK THEY TRIED TO EAT ME.");
	floria->setDialogue("I just love flowers!");
	floria->setRecruitmentDialogue("Yay! I hope we see some new flowers!");
	floria->setRecruitedDialogue("I must see all the flowers!");
	floria->setDismissalDialogue("I'm going to go back to my flower field!");

	NPC* egadwick = new NPC("SCIENCE GRAMPS", "EGADWICK", "Your grandpa who lives in a secluded corner of the village. He's always advancing science to the dismay of high school chemistry students.", tentlab, 15, 2, 3, 10, 10, 2, 10, 10);
	egadwick->setDialogue("Ah hello kiddo. How");
	egadwick->setRejectionDialogue("No, sorry kiddo. I made a robot for gardening but now it's trying to cut my gorgeous hair and it's on the loose in the forest. If you could destroy it I could probably go.");
	egadwick->setRecruitmentDialogue("Ah, I haven't been adventuring in decades. Thanks for the invitation, kiddo!");
	egadwick->setRecruitedDialogue("I love science.");
	egadwick->setDismissalDialogue("Great hanging out with you, kiddo! Well, I guess I'll go work on a better robot!");

	NPC* archie = new NPC("VILLAGE ELDER", "ARCHIE", "The elder of Tactical Tent Village. He stands there all day and night like a statue.", village, 1, 0, 1, 0, 0, 0, 0, 50);
	archie->setDialogue("So you are going on a BURGER QUEST, I hear? Just keep heading NORTH, and you'll soon reach BURGERSBURG. Safe travels, child!");
	archie->setRejectionDialogue("I am sorry. Though I would love to join you on your BURGER QUEST, I must stay here and watch over the village. Make sure to bring back a BURGER for me!");

	//NPC* treeelder = new NPC("TREE ELDER", "TREE", "An ancient tree outdating BURGERs");


	//REPLACE PLACEHOLDER STATS
	NPC* graham = new NPC("GAMBLER", "GRAHAM", "A sorry gambling addict who is trillions in debt. He'll pay it off as soon as he wins; any day now.", casino, 30, 10, 5, 0, 2, 20, 0, 2);
	graham->setDialogue("What's that? I should quit gambling? Haven't you heard that 99% of gamblers quit right before hitting it big?\"\nGAMBLING MACHINE - \"You lose 1000000 monies.\"\nGRAHAM - \"Aw dang it.");
	graham->setRejectionDialogue("Nah, sorry man. I'm just about to win the jackpot. I can feel it!\"\nGAMBLING MACHINE - \"You lose 1000000 monies.\"\nGRAHAM - \"Aw dang it.");

	NPC* burgerman = new NPC("", "BURGER MAN", "The manager of the BURGER RESTAURANT. He has a BURGER for a head and an uncanny stature.", BURGERRESTAURANT, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647);
	NPC* henryjerry = new NPC("BURGER QUEST 1 PROTAGONIST", "HENRY JERRY", "The sleep-deprived protagonist from the first game who was used as a puppet of BURGER. He wears a formal suit and seems traumatized.", limbo, 10, 2, 4, 1, 0, 4, 5, 1);

	NPC* burgerprisoner = new NPC("BURGER PRISONER", "ARCHIBALD", "A shriveled man imprisoned for resisting the global domination of BURGER.", BURGERPRISON, 1000000, 5000000, 900000000, 100000, 0, 700000, 200000, 80000000);
	burgerprisoner->setDialogue("I have not seen anybody in ages. [PLOT DEVICE QUEST INSTRUCTIONS]");
	burgerprisoner->setRejectionDialogue("I would love to join you on your quest. But as long as the BURGER MENACE endures, so shall these bars you see in front of me.");

	NPC* jimmyjohn = new NPC("SHOPKEEPER", "JIMMY JOHN", "The owner of the village convenience store. He has an italian accent.", tentstore, 500, 400, 99999, 800, 99999, 50, 50, 30);
	jimmyjohn->setDialogue("Welcome to my convenience store! None is more convenient!");
	jimmyjohn->setRejectionDialogue("I'm sorry I cannot. Who will take care of my store?");

	//NPC* fisho = new NPC("FISHERMAN", "FRED", "A middle-aged fisherman who frequently fishes at the village docks.", docks, 30, 10, );

	Effect* minibuff = new Effect();
	//tent store stock
	Item* ibuprofen = new HpItem("IBUPROFEN", "Relieves pain and inflammation and stuff. (heals 10 HP)", limbo, 10);
	tentstore->setStock(ibuprofen, 2147483647, 10, "JIMMY JOHN - Thank you for your patronage!");
	Item* energybook = new EducationItem("ENERGY MANIPULATION BEGINNER'S GUIDE", "A book full of energy manipulation techniques. You could learn some cool attacks from this.", limbo, energyball);
	//energybook->setAttack();
	//energybook->setAttack();
	tentstore->setStock(energybook, 1, 100, "JIMMY JOHN - Thank you for your patronage!");

	NPC* wallelder = new NPC("WALL ELDER", "WELBY", "An ancient elder whose rocky face spans the wall. There may be more to him, but all you can see is his face.", mineshaft3, 15000, 15000, 15000, 15000, 0, 2000, 25000);
	wallelder->addConversation(wallelder, "Child, are you on a BURGER QUEST?");
	wallelder->addConversation(self, "Indeed I am."); //what if the player has already started plot device quest? they probably haven't tho
	wallelder->addConversation(wallelder, "Do not be fooled by the allure of BURGER. Do you not know why you crave it so?");
	wallelder->addConversation(wallelder, "BURGER is formed from the essence of evil. Have you seen the desert above?");
	wallelder->addConversation(self, "Uh huh.");
	wallelder->addConversation(wallelder, "It used to be a beautiful forest, full of life like the one to the south.");
	wallelder->addConversation(wallelder, "But the BURGER MAN parched the woods and reduced it to the wastes seen today, and he will continue to do so until nature has been expunged from this world.");
	wallelder->addConversation(wallelder, "For by destroying the means to live, he guides the people by hunger and thirst to his city, where they will be more easily tempted by the BURGER RESTAURANT.");
	wallelder->addConversation(self, "Dang that's crazy.");
	wallelder->addConversation(wallelder, "If nothing else, remember this. The lies of this world are placed high UP on shining pedestals, while its truths are buried DOWN below.");
	wallelder->setDialogue("Always beware the temptation of BURGER.");
	wallelder->setRejectionDialogue("I embedded myself in this heavy rock ages ago so that temptation could not possibly drag me to the BURGER RESTAURANT. I cannot move nor join you.");

	Attack* shurikenthrow = new Attack("SHURIKEN THROW", "Throw a spread of shurikens at the target, with varying success.", 0, 7, 5, 0, 2, 3);
	Item* shuriken = new EducationItem("SHURIKEN", "A ninja shuriken with a note attached: \"Congratulations on defeating our ninja scout. Take this shuriken and train in the ninja ways, and maybe one day you'll become a true ninja.\"", ninjaland, shurikenthrow);
	
	//MARK: set up room exits
	village->setExit(SOUTH, docks);
	village->setExit(EAST, forestentrance);
	village->setExit(WEST, villageleft);
	village->setExit(IN_TENT_1, tentstore);
	village->setExit(IN_TENT_2, tentstation);
	village->setExit(IN_TENT_3, tentchurch);
	villageleft->setExit(EAST, village);
	villageleft->setExit(IN_TENT, tentmansion);
	tentmansion->setExit(OUT, villageleft);
	tentmansion->setExit(UPSTAIRS, tentlab);
	tentlab->setExit(DOWNSTAIRS, tentmansion);
	tentstore->setExit(OUT, village);
	tentstation->setExit(OUT, village);
	tentstation->setExit(WEST, limbo);
	tentstation->setExit(EAST, limbo);
	tentchurch->setExit(OUT, village);
	docks->setExit(NORTH, village);
	forestentrance->setExit(WEST, village);
	forestentrance->setExit(NORTH, forest);
	forest->setExit(SOUTH, forestentrance);
	forest->setExit(NORTHWEST, forestleft);
	forest->setExit(NORTHEAST, forestright);
	forestleft->setExit(WEST, deerclearing);
	forestleft->setExit(NORTHEAST, foresttempleentrance);
	forestleft->setExit(SOUTHEAST, forest);
	deerclearing->setExit(EAST, forestleft);
	forestright->setExit(NORTHEAST, ninjaland);
	forestright->setExit(NORTHWEST, foresttempleentrance);
	forestright->setExit(SOUTHWEST, forest);
	ninjaland->setExit(SOUTHWEST, forestright);
	ninjaland->setExit(UP, ninjavillage);
	ninjavillage->setExit(IN_HOUSE_1, ninjacapitol);
	ninjavillage->setExit(IN_HOUSE_2, ninjapantry);
	ninjavillage->setExit(IN_HOUSE_3, ninjaforge);
	foresttempleentrance->setExit(SOUTHWEST, forestleft);
	foresttempleentrance->setExit(SOUTHEAST, forestright);
	foresttempleentrance->setExit(NORTHWEST, flowerfield);
	foresttempleentrance->setExit(NORTH, forestfork);
	foresttempleentrance->setExit(SOUTH, foresttemplestairs);
	flowerfield->setExit(WEST, flowerfield2);
	flowerfield->setExit(SOUTHEAST, foresttempleentrance);
	flowerfield2->setExit(EAST, flowerfield);
	forestfork->setExit(NORTHWEST, forestgarden);
	forestfork->setExit(NORTHEAST, forestgate);
	forestfork->setExit(SOUTH, foresttempleentrance);
	forestgarden->setExit(SOUTHEAST, forestfork);
	forestgate->setExit(EAST, forestwall);
	forestgate->setExit(SOUTHWEST, forestfork);
	forestgate->setExit(NORTH, forestspork);
	forestwall->setExit(EAST, forestgrave);
	forestwall->setExit(WEST, forestgate);
	forestgrave->setExit(WEST, forestwall);
	forestspork->setExit(SOUTH, forestgate);
	forestspork->setExit(NORTHWEST, bossgrove);
	forestspork->setExit(NORTHEAST, forestnice);
	forestnice->setExit(SOUTHWEST, forestspork);
	forestnice->setExit(EAST, treasuregrove);
	treasuregrove->setExit(WEST, forestnice);
	treasuregrove->setExit(NORTH, treasurecliff);
	treasurecliff->setExit(SOUTH, treasuregrove);
	bossgrove->setExit(SOUTHEAST, forestspork);
	bossgrove->setExit(NORTH, fdintermission1);
	fdintermission1->setExit(NORTH, fdintermission2);
	fdintermission1->setExit(SOUTH, bossgrove);
	fdintermission2->setExit(NORTH, fdintermission3);
	fdintermission2->setExit(SOUTH, fdintermission1);
	fdintermission3->setExit(NORTH, desert);
	fdintermission3->setExit(SOUTH, fdintermission2);
	desert->setExit(SOUTH, fdintermission3);
	desert->setExit(NORTHWEST, deserttempleentrance);
	desert->setExit(NORTHEAST, desertdune);
	desert->setExit(EAST, desertplain);
	deserttempleentrance->setExit(WEST, deserttemplestairs);
	deserttempleentrance->setExit(EAST, desertdune);
	deserttempleentrance->setExit(NORTHEAST, deserthill);
	desertdune->setExit(WEST, deserttempleentrance);
	desertdune->setExit(NORTHEAST, desertgrave);
	desertdune->setExit(SOUTHWEST, desert);
	desertdune->setExit(SOUTHEAST, desertplain);
	desertgrave->setExit(SOUTHWEST, desertdune);
	desertplain->setExit(EAST, desertpole);
	desertplain->setExit(WEST, desert);
	desertplain->setExit(NORTHWEST, desertdune);
	desertpole->setExit(WEST, desertplain);
	deserthill->setExit(NORTH, deserttown);
	deserthill->setExit(SOUTHWEST, deserttempleentrance);
	deserttown->setExit(NORTH, canyon1);
	deserttown->setExit(SOUTH, deserthill);
	deserttown->setExit(EAST, oasis);
	deserttown->setExit(WEST, canyon);
	deserttown->setExit(IN_HOUSE_1, desertshop);
	deserttown->setExit(IN_HOUSE_2, desertgym);
	deserttown->setExit(IN_HOUSE_3, deserthouse);
	desertshop->setExit(OUT,deserttown);
	desertgym->setExit(OUT,deserttown);
	deserthouse->setExit(OUT,deserttown);
	oasis->setExit(WEST, deserttown);
	canyon->setExit(UP, thatcliff);
	canyon->setExit(EAST, deserttown);
	canyon->setExit(UNDERGROUND, desertstation);
	canyon1->setExit(SOUTH, deserttown);
	canyon1->setExit(NORTHWEST, canyon2);
	canyon1->setExit(NORTHEAST, canyon3);
	canyon2->setExit(SOUTHEAST, canyon1);
	canyon2->setExit(UP, cliff1);
	canyon3->setExit(SOUTHWEST, canyon1);
	canyon3->setExit(UNDERGROUND, mineshaft);
	mineshaft->setExit(OUT, canyon3);
	mineshaft->setExit(EAST, minespring);
	mineshaft->setExit(WEST, mineshaft2);
	minespring->setExit(NORTH, mineshaft);
	mineshaft2->setExit(SOUTH, deserttunnel);
	mineshaft2->setExit(NORTH, mineshaft3);
	deserttunnel->setExit(NORTHEAST, limbo);
	deserttunnel->setExit(SOUTHWEST, desertstation);
	desertstation->setExit(NORTHEAST, deserttunnel);
	desertstation->setExit(SOUTHWEST, limbo);
	desertstation->setExit(OUT, canyon);
	mineshaft3->setExit(WEST, mineshaftside);
	mineshaft3->setExit(SOUTH, mineshaft2);
	mineshaft3->setExit(NORTH, minelight);
	mineshaftside->setExit(EAST, mineshaft3);
	mineshaftside->setExit(SOUTHWEST, kaboomroom);
	kaboomroom->setExit(NORTHEAST, mineshaftside);
	minelight->setExit(UP, mineexit);
	minelight->setExit(SOUTH, mineshaft3);
	minelight->setExit(NORTH, volcanoentrance);
	mineexit->setExit(OUT, cliff1);
	mineexit->setExit(DOWN, minelight);
	cliff1->setExit(UNDERGROUND, mineexit);
	cliff1->setExit(DOWN, canyon2);
	cliff1->setExit(UP, cliff2);
	cliff2->setExit(DOWN, cliff1);
	thatcliff->setExit(DOWN, canyon);
	volcanoentrance->setExit(SOUTH, minelight);
	volcanoentrance->setExit(OUT, volcano);
	volcano->setExit(UNDERGROUND, volcanoentrance);
	volcano->setExit(NORTH, volcano1);
	volcano1->setExit(SOUTH, volcano);
	volcano1->setExit(IN_FACTORY, factory1);
	volcano1->setExit(NORTHWEST, volcano2);
	volcano2->setExit(SOUTHEAST, volcano1);
	volcano2->setExit(NORTH, volcano3);
	volcano3->setExit(SOUTH, volcano2);
	volcano3->setExit(DOWN, volcanostation);
	volcano3->setExit(IN_FACTORY, factory2);
	volcano3->setExit(NORTHEAST, volcano4);
	volcano4->setExit(SOUTHWEST, volcano3);
	volcano4->setExit(NORTHEAST, volcano5);
	volcano5->setExit(NORTH, volcano6);
	volcano5->setExit(SOUTHWEST, volcano4);
	volcano6->setExit(SOUTH, volcano5);
	volcano6->setExit(NORTHWEST, volcano7);
	volcano6->setExit(IN_FACTORY, factory3);
	volcano7->setExit(NORTH, bridge1);
	volcano7->setExit(SOUTHWEST, castleentrance);
	volcano7->setExit(SOUTHEAST, volcano6);
	castleentrance->setExit(NORTHEAST, volcano7);
	castleentrance->setExit(IN_CASTLE, castlehall);
	castlehall->setExit(OUT, castleentrance);
	castlehall->setExit(SOUTH, castlethrone);
	castlethrone->setExit(NORTH, castlehall);
	factory1->setExit(OUT, volcano1);
	factory1->setExit(EAST, factorykitchen);
	factory1->setExit(UP, controlroom1);
	factorykitchen->setExit(WEST, factory1);
	controlroom1->setExit(DOWN, factory1);
	volcanostation->setExit(OUT, volcano3);
	factory2->setExit(OUT, volcano3);
	factory2->setExit(NORTHWEST, switchroom);
	factory2->setExit(NORTHEAST, conveyor1);
	switchroom->setExit(SOUTHEAST, factory2);
	conveyor1->setExit(FORWARD, factorycenter);
	factorycenter->setExit(SOUTHWEST, conveyor1);
	factorycenter->setExit(NORTHWEST, conveyor2);
	factorycenter->setExit(NORTHEAST, conveyor3);
	conveyor3->setExit(FORWARD, controlroom2);
	controlroom2->setExit(SOUTHWEST, conveyor3);
	conveyor2->setExit(FORWARD, factorystorage);
	factorystorage->setExit(SOUTH, conveyor2);
	factorystorage->setExit(NORTH, conveyor4);
	factorystorage->setExit(EAST, conveyor5);
	factorystorage->setExit(WEST, factorybalcony1);
	factorybalcony1->setExit(EAST, factorystorage);
	factorybalcony1->setExit(DOWN, switchroom);
	factorybalcony1->setExit(NORTHEAST, switchroom2);
	conveyor4->setExit(FORWARD, switchroom2);
	conveyor5->setExit(FORWARD, factorybalcony2);
	switchroom2->setExit(SOUTHWEST, factorybalcony1);
	switchroom2->setExit(SOUTH, conveyor4);
	switchroom2->setExit(SOUTHEAST, factorybalcony2);
	factorybalcony2->setExit(NORTHWEST, switchroom2);
	factorybalcony2->setExit(WEST, conveyor5);
	factorybalcony2->setExit(DOWN, factorycenter);
	factorybalcony2->setExit(EAST, factorygarden);
	factorygarden->setExit(WEST, factorybalcony2);
	factorygarden->setExit(DOWN, factorycenter);
	factory3->setExit(OUT, volcano6);
	factory3->setExit(NORTH, factorynw);
	factory3->setExit(EAST, factoryse);
	factory3->setExit(UP, factoryroofsw);
	factorynw->setExit(EAST, factoryne);
	factorynw->setExit(SOUTH, factory3);
	factorynw->setExit(UP, factoryroofnw);
	factoryne->setExit(WEST, factorynw);
	factoryne->setExit(SOUTH, factoryse);
	factoryne->setExit(UP, factoryroofne);
	factoryse->setExit(NORTH, factoryne);
	factoryse->setExit(WEST, factory3);
	factoryse->setExit(EAST, heavymachineryroom);
	factoryse->setExit(UP, factoryroofse);
	heavymachineryroom->setExit(WEST, factoryse);
	factoryroofsw->setExit(DOWN, factory3);
	factoryroofnw->setExit(DOWN, factorynw);
	factoryroofnw->setExit(NORTH, controlroom3);
	controlroom3->setExit(SOUTH, factoryroofnw);
	factoryroofne->setExit(DOWN, factoryne);
	factoryroofne->setExit(EAST, factoryhallway);
	factoryhallway->setExit(WEST, factoryroofne);
	factoryhallway->setExit(UP, factorytower);
	factorytower->setExit(DOWN, factoryhallway);
	factoryroofse->setExit(DOWN, factoryse);
	factoryroofse->setExit(EAST, factorytreasure);
	factorytreasure->setExit(WEST, factoryroofse);
	sewerentrance1->setExit(DOWN, sewerentrance2);
	sewerentrance1->setExit(UP, volcano2);
	sewerentrance2->setExit(DOWN, sewer);
	sewerentrance2->setExit(UP, sewerentrance1);
	sewer->setExit(NORTH, sewercenter1);
	sewer->setExit(UP, sewerentrance2);
	sewer->setExit(SOUTHWEST, sewerminessouth);
	sewerminessouth->setExit(NORTHEAST, sewer);
	sewerminessouth->setExit(SOUTH, mineshortcut);
	mineshortcut->setExit(NORTH, sewerminessouth);
	sewer2->setExit(SOUTHWEST, sewercenter2);
	sewer2->setExit(UP, volcano6);
	sewercenter1->setExit(SOUTH, sewer);
	sewercenter1->setExit(NORTH, sewerplant);
	sewercenter1->setExit(NORTHWEST, sewercenter4);
	sewercenter1->setExit(NORTHEAST, sewercenter3);
	sewercenter2->setExit(NORTHEAST, sewer2);
	sewercenter2->setExit(SOUTH, sewerplant);
	sewercenter2->setExit(SOUTHWEST, sewercenter4);
	sewercenter2->setExit(SOUTHEAST, sewercenter3);
	sewercenter3->setExit(NORTHWEST, sewercenter2);
	sewercenter3->setExit(SOUTHWEST, sewercenter1);
	sewercenter3->setExit(DOWN, volcanotempleentrance);
	volcanotempleentrance->setExit(UP, sewercenter3);
	sewercenter4->setExit(NORTHEAST, sewercenter2);
	sewercenter4->setExit(SOUTHEAST, sewercenter1);
	sewercenter4->setExit(WEST, sewermineswest);
	sewermineswest->setExit(EAST, sewercenter4);
	sewermineswest->setExit(WEST, mountainmine);
	sewerplant->setExit(NORTH, sewercenter2);
	sewerplant->setExit(SOUTH, sewercenter1);
	mountainmine->setExit(EAST, sewermineswest);
	mountainmine->setExit(OUT, mountain);
	mountain->setExit(UNDERGROUND, mountainmine);
	mountain->setExit(UP, mountain2);
	mountain2->setExit(WEST, mountainlake);
	mountain2->setExit(UP, mountain3);
	mountain2->setExit(DOWN, mountain);
	mountainlake->setExit(EAST, mountain2);
	mountain3->setExit(UP, mountainpeak);
	mountain3->setExit(DOWN, mountain2);
	mountainpeak->setExit(IN_TENT, tenthome);
	mountainpeak->setExit(DOWN, mountain3);
	tenthome->setExit(OUT, mountainpeak);
	bridge1->setExit(NORTH, bridge2);
	bridge1->setExit(SOUTH, volcano7);
	bridge2->setExit(NORTH, bridge1);
	bridge2->setExit(SOUTH, bridge3);
	bridge3->setExit(NORTH, BURGERSBURG);
	bridge3->setExit(SOUTH, bridge2);
	

	Room* tunnel = new Room("in the train tunnels that span the continent.");

	//MARK: set up enemies

	NPC* tunnel_lobster = new NPC("", "TUNNEL LOBSTER", "An immense, savage crustacean who inhabits the tunnels below.", tunnel, 200, 20, 10, 20, 10, 50, 10, 30, true);
	//you should also get a prompt to name it like
	//                                                     (type your lobster's name here!)
	//SELF - "Oh nice a pet lobster. I think I'll name you 
	//Successfully tamed TUNNEL LOBSTER [FLORIAN]!
	// 
	//and then set the lobster's title to TUNNEL LOBSTER and the name to whatever was typed
	// 
	//if you just enter nothing self says
	//SELF - "Ok nevermind I guess I won't name you."
	//Successfully tamed TUNNEL LOBSTER!
	// 
	//reset the description to tamed instead of savage
	tunnel_lobster->setDialogue("HHhHHHhhHHhHhhHhHHhHhHHh (lobster noises).");
	tunnel_lobster->setRejectionDialogue("HhhHhHhHhhhhHHhHHh (lobster noises probably meaning no).");

	//set up generic non-npc enemies
	NPC* prickly_hog = new NPC("", "PRICKLY HOG", "A small but ferocious hog with sharp prickles.", limbo, 10, 10, 5, 0, 20, 15, 5);
	Attack* headbutt = new Attack("HEADBUTT", "headbutted", -5, 5, 0, 1, 1, 1);
	Attack* homing_prickle = new Attack("HOMING PRICKLE", "launched homing prickles at", 5, 3, 5, 2, 4, 3);
	prickly_hog->setBasicAttack(headbutt);
	prickly_hog->addSpecialAttack(homing_prickle);

	NPC* greater_hog = new NPC("", "GREATER HOG", "A larger and more territorial hog with sharp prickles.", limbo, 20, 0, 0, 0, 0, 0, 0);
	greater_hog->setBasicAttack(headbutt);
	greater_hog->addSpecialAttack(homing_prickle);

	NPC* grassman = new NPC("", "GRASSMAN", "A really grassy humanoid who hates real humans.", limbo, 16, 0, 5, 0, 2, 5, 5);
	Attack* grassstrike = new Attack("GRASS STRIKE", "grassily striked", 0, 3, 0, 1, 1, 1);
	grassman->setBasicAttack(grassstrike);

	NPC* jimshady = new NPC("", "JIM SHADY", "An envious and spiky shrimp. This JIM SHADY is just imitating.", limbo, 50, 20, 10, 5, 15, 20, 10);
	Attack* shirmplebeam = new Attack("SHRIMPLE BEAM", "fired a pressurized jet of water at", 0, 99999, 99999, 1, 1, 1);
	jimshady->setBasicAttack(shirmplebeam);

	NPC* carnplant = new NPC("", "CARNIVOROUS PLANT", "Really big plant who likes eating meat.", limbo, 20, 5, 7, 5, 5, 12, 10);
	Attack* bite = new Attack("BITE", "bit", -5, 10, 5, 1, 1, 1);
	Attack* nutrientabsorb = new Attack("NUTRIENT ABSORB", "sucked the nutrients out of", 10, 10, 5, 1, 1, 1, 0.5f);
	carnplant->setBasicAttack(bite);
	carnplant->addSpecialAttack(nutrientabsorb);

	NPC* flowerfiend = new NPC("", "FLOWER FIEND", "Enormous carnivorous flower with lashing vines. Probably the FLOWER FRIEND your sister talks about.", flowerfield, 50, 5, 7, 5, 5, 12, 24, 7, true);
	flowerfiend->setParty(carnplant, carnplant);
	Attack* crunch = new Attack("CRUNCH", "used its flowery fangs to crunch", -7, 15, 7, 1, 1, 1);
	//Attack* flowerpower = new Attack("FLOWER POWER", "used its planty power to buff", 16, 10, 5, 1, 1, 1);
	Attack* solarbeam = new Attack("SOLAR BEAM", "used its petals to channel solar light onto", 24, 30, 10, 1, 1, 1);
	flowerfiend->setBasicAttack(crunch);
	flowerfiend->addSpecialAttack(nutrientabsorb);
	//flowerfiend->addSpecialAttack(flowerpower);
	flowerfiend->addSpecialAttack(solarbeam);

	NPC* carnplant = new NPC("", "CARNIVOROUS PLANT", "Really big plant who likes eating meat.", limbo, 20, 5, 7, 5, 5, 12, 10);

	NPC* roguerobot = new NPC("ROGUE ROBOT", "EGARDENBOT 1.0", "Short trapezoidal copper robot designed to be an expert gardener, before going rogue and trimming EVERYTHING.", forestgarden, 20, 15, 5, 5, 10, 20, 15, 5, true);
	Attack* snip = new Attack("SNIP", "snipped scissors at", -5, 7, 5, 1, 1, 1);
	roguerobot->setBasicAttack(snip);
	roguerobot->setLink(egadwick);
	roguerobot->addLinkedConvo(egadwick, "I'm no longer detecting signals from my robot. Did you by chance stop it?");
	roguerobot->addLinkedConvo(self, "Yep I did.");
	roguerobot->addLinkedConvo(egadwick, "Oh thank goodness! Thanks a bunch, kiddo!");
	roguerobot->addLinkedConvo(egadwick, "Now I can safely be in the great outdoors!");

	//Attack* ATTACK = new Attack("NAME", "DESCRIPTION", COST, POWER, PIERCE, MINHITS, MAXHITS, TARGETS);
	//NPC* npc = new NPC("TITLE", "NAME", "DESCRIPTION", limbo, hp, def, att, tou, pie, spe, ski);

	//for npcs you can actually fight, make sure to set their level as 0 at construction, then set the level manually after setting the scale, otherwise stats will be off

	//set up npc enemies
	NPC* forestguard = new NPC(*grassman);
	forestguard->setLeader(true, 1, forestentrance);
	forestguard->setDialogue("(angry bush noises)");
	forestguard->setRejectionDialogue("(angry bush noises)");
	//forestguard->setParty(grassman, grassman, grassman, prickly_hog);

	//MARK: block exits
	tentstation->blockExit(EAST, TUNNEL, "blocked by endless rubble.");
	tentstation->blockExit(WEST, TUNNEL, "blocked by endless rubble.");
	forestgate->blockExit(NORTH, LOCK, "blocked by a large branchy gate. There is a large keyhole in the center with deer antlers.");
	foresttempleentrance->blockExit(SOUTH, TEMPLE, "sealed shut by ancient technology. No matter what anyone has tried, nobody has ever made it in.");
	treasuregrove->blockExit(NORTH, CHASM, "blocked by a large chasm.");
	treasurecliff->blockExit(SOUTH, CHASM, "blocked by a large chasm.");
	ninjaland->blockExit(UP, MISC, "too high. You need ninja abilities to scale the trees.");

	//volcano factory exit is blocked by the caved-in roof

	//exits blocked by enemies
	forestguard->blockExit(NORTH, ENEMY, "guarded by the GRASSMAN.");

	return self; //returns the player character
}

void PrintRoomData(Room* currentRoom) {
	currentRoom->printWelcome();
	cout << "\nYou are " << currentRoom->getDescription();
	currentRoom->printExits();
	currentRoom->printNPCs();
	currentRoom->printItems();
	currentRoom->printStock();
	currentRoom->printBlocks();
}

void travel(Room*& currentRoom, char* direction, vector<NPC*>* party) {
	Room* roomCanidate = currentRoom->getExit(direction);
	if (roomCanidate == NULL) {
		if (strcmp(direction, "NORTH") && strcmp(direction, "SOUTH") && strcmp(direction, "WEST") && strcmp(direction, "EAST")) {
			cout << "\nInvalid direction \"" << direction << "\".";
		} else {
			cout << "\nThere is no exit in that direction.";
		}
		return;
	} else if (currentRoom->getBlocked(direction)) {
		currentRoom->printBlock(direction);
		return;
	}
	roomCanidate->undefeatEnemies();
	currentRoom = roomCanidate;
	for (NPC* npc : (*party)) {
		npc->setRoom(currentRoom);
	}
	PrintRoomData(currentRoom);
}

void takeItem(Room* currentRoom, vector<Item*>* inventory, char* itemname) {
	Item* item = getItemInVector(currentRoom->getItems(), itemname);
	if (item == NULL) {
		if (getItemInVector(currentRoom->getStock(), itemname) != NULL) {
			cout << "\nThe " << itemname << " is for sale! You can't just take it.";
			return;
		} else if (getItemInVector(*inventory, itemname) != NULL) {
			cout << "\nYou're already carrying this item!";
			return;
		}
		cout << "\nThere is no \"" << itemname << "\" here.";
		return;
	}
	if (!item->getTakable()) {
		cout << "\n" << item->getDenial();
		return;
	}
	item->unRoom();
	inventory->push_back(item);
	cout << "\nYou took the " << itemname << ".";
}

void dropItem(Room* currentRoom, vector<Item*>* inventory, char* itemname) {
	Item* item = getItemInVector(*inventory, itemname);
	if (item == NULL) {
		cout << "\nYou have no \"" << itemname << "\"."; //I know ". is grammatically inaccurate but it looks way better than ."
		return;
	}
	item->setRoom(currentRoom);
	inventory->erase(remove(inventory->begin(), inventory->end(), item), inventory->end());
	cout << "\nYou dropped the " << itemname << ".";
}

void deleteItem(Room* currentRoom, vector<Item*>* inventory, Item* item) {

}

//MARK: useitem
//use commands may be formatted "USE ITEM", or "USE ITEM ON NPC", or "USE ITEM NAME ON NPC", so have fun coding that
void useItem(Room* currentRoom, vector<Item*>* inventory, char* itemname, int& mony) {
	Item* item = getItemInVector(*inventory, itemname);
	if (item == NULL) {
		item = getItemInVector(currentRoom->getItems(), itemname);
	}
	if (item == NULL) {
		cout << "\nYou have no \"" << itemname << "\".";
		return;
	}
	NPC* target = NULL;
	if (item->getTargetNeeded()) {
		//get the target
	}
	if (!strcmp(item->getType(), "xp")) {

	} else if (!strcmp(item->getType(), "mony")) {

	} else if (!strcmp(item->getType(), "BURGER")) {

	} else if (!strcmp(item->getType(), "education")) {

	} else if (!strcmp(item->getType(), "caller")) {

	} else if (!strcmp(item->getType(), "toll")) {

	} else if (!strcmp(item->getType(), "key")) {
		KeyItem* key = (KeyItem*)item;
		vector<char*> exitsUnlocked;
		Room* targetRoom = currentRoom;
		bool used = false;
		if (key->getTarget() != NULL) {
			targetRoom = key->getTarget();
			exitsUnlocked = targetRoom->unblockAll(key->getUnlockType());
			used = true;
		} else {
			exitsUnlocked = currentRoom->unblockAll(key->getUnlockType());
			used = exitsUnlocked.size() > 0;
		}
		if (!used) {
			cout << "\nThere is nothing to use the " << itemname << " on here.";
			return;
		}
		cout << "\n" << key->getUseText();

		//change room description

		//exactly one blocked exit in this game is also blocked from the other side, so I do this check and assume no exits in the game have mismatching blocks
		for (char* exit : exitsUnlocked) {
			targetRoom->getExit(exit)->unblockAll(key->getUnlockType());
		}
		//if (deletable) {
		deleteItem(currentRoom, inventory, item);
		//}
	} else if (!strcmp(item->getType(), "paver")) {

	} else if (!strcmp(item->getType(), "info")) {

	} else if (!strcmp(item->getType(), "material")) {
		cout << "\nYou can't use the " << itemname << "!";
	} else {
		cout << "\nThe " << itemname << " can only be used in battle!";
	}
}

void recruitNPC(Room* currentRoom, char* npcname, vector<NPC*>* party, int maxParty = 4) {
	NPC* npc = getNPCInVector(currentRoom->getNpcs(), npcname);
	if (npc == NULL) {
		cout << "\nThere is nobody named \"" << npcname << "\" here.";
		return;
	}
	if (npc->getPlayerness()) {
		cout << "\n" << npcname << " - \"Huh?\"\n\nYou are already in your own party? ...";
		return;
	}
	if (!npc->getRecruitable()) {
		cout << "\n" << npcname << " - \"" << npc->getRejectionDialogue() << "\"";
		cout << "\n" << npcname << " was not added to your party.";
		return;
	}
	if (npc->getRecruited()) {
		cout << "\n" << npcname << " is already in your party...";
		return;
	}
	if (party->size() == maxParty) {
		cout << "\nYour party is full!";
	}
	party->push_back(npc);
	npc->Recruit();
	cout << "\n" << npcname << " was added to your party!" << "(party size: " << party->size() << "/" << maxParty << ")";
}

void dismissNPC(Room* currentRoom, char* npcname, vector<NPC*>* party) {
	NPC* npc = getNPCInVector(currentRoom->getNpcs(), npcname);
	if (npc == NULL) {
		cout << "\nThere is nobody named \"" << npcname << "\" here.";
		return;
	}
	if (npc->getPlayerness()) {
		cout << "\n" << npcname << " - \"Huh? You can't dismiss me bro I'm the main character!\"\n" << npcname << " was not dismissed.";
		return;
	}
	if (!npc->getRecruited()) {
		cout << "\n" << npcname << " is not in your party...";
		return;
	}
	party->erase(remove(party->begin(), party->end(), npc), party->end());
	cout << "\n" << npcname << " - \"" << npc->getDismissalDialogue() << "\"";
	cout << "\n" << npcname << " was removed from your party and returned to what they were doing before.";
}

void printNPCDialogue(Room* currentRoom, char* npcname) {
	NPC* npc = getNPCInVector(currentRoom->getNpcs(), npcname);
	if (npc == NULL) {
		cout << "\nThere is nobody named \"" << npcname << "\" here.";
		return;
	}
	npc->printDialogue();
}

void printInventory(vector<Item*>* inventory, int monies) {
	cout << "\nYou have " << monies << " monies, and you";
	if (inventory->size() < 1) {
		cout << " have no items!";
		return;
	}
	cout << "r items are:";
	for (Item* item : *inventory) {
		cout << "\n" << item->getName()/* << " - " << item->getDescription()*/;
	}
}

void printParty(vector<NPC*>* party) {
	cout << "\nMembers of your party:";
	for (NPC* npc : *party) {
		cout << "\n" << npc->getTitle();
		if (strlen(npc->getTitle()) > 0) {
			cout << " ";
		}
		cout << npc->getName() << " - LEVEL " << npc->getLevel();
	}
}

void analyze(Room* currentRoom, char* name, vector<NPC*>* party, vector<Item*>* inventory) {
	NPC* npc = getNPCInVector(currentRoom->getNpcs(), name);
	if (npc == NULL) {
		npc = getNPCInVector(*party, name);
	}
	if (npc != NULL) {
		printNPCData(npc);
		return;
	}
	Item* item = getItemInVector(currentRoom->getItems(), name);
	if (item == NULL) {
		item = getItemInVector(*inventory, name);
	}
	if (item == NULL) {
		item = getItemInVector(currentRoom->getStock(), name);
	}
	if (item != NULL) {
		printItemData(item);
		return;
	}
	cout << "\nThere is no item or person named \"" << name << "\" here.";
}

void buy(Room* currentRoom, vector<Item*>* inventory, char* name, int& mony) {
	Item* item = getItemInVector(currentRoom->getStock(), name);
	if (item == NULL) {
		if (getItemInVector(currentRoom->getItems(), name) != NULL) {
			cout << "\nNobody is selling the " << name << "; you can just take it.";
			return;
		} else if (getItemInVector(*inventory, name) != NULL) {
			cout << "\nYou already own this item!";
			return;
		}
		cout << "\nThere is no \"" << name << "\" here.";
		return;
	}
	item->buy(mony, inventory);
	if (item->getStock() <= 0) {
		currentRoom->removeStock(item);
	}
}

void fight(Room* currentRoom, vector<NPC*>* party, vector<Item*>* inventory, char* name, int& mony) {
	NPC* npc = getNPCInVector(currentRoom->getNpcs(), name);
	if (npc == NULL) {
		cout << "\nThere is nobody named \"" << name << "\" here.";
		return;
	}
	if (npc->getPlayerness()) {
		cout << "\n" << name << " - \"Uhhhh you want me to fight myself?\"";
		return;
	}
	if (!npc->getLeader()) {
		cout << "\nYou can't fight " << name << "!";
		return;
	}

	Battle battle = Battle(party, npc->getParty(), inventory, mony, npc->getEscapable());
	int battlestatus = battle.FIGHT();
	if (battlestatus == 0) { //lose
		cout << "\nDEFEAT.";
		CinPause();
		int monyLoss = mony - mony/2;
		mony -= monyLoss;
		cout << "\nYou lost " << monyLoss << " mon";
		if (monyLoss == 1) {
			cout << "y.";
		} else {
			cout << "ies.";
		}
		CinPause();
	} else if (battlestatus == 1) { //win
		cout << "\nYOU WIN!";
		CinPause();
		mony += battle.getMonyReward(); 
		//prints how much monies were earned and the new current total. I don't care about grammar here because the reward is literally never just 1
		cout << "\nYou earned " << battle.getMonyReward() << " monies! Now you have " << mony << " monies!";
		CinPause();
		for (NPC* teammate : *party) {
			teammate->addXp(battle.getXpReward());
		}
		cout << "\nYou";
		if (party->size() > 1) {
			cout << "r party members";
		}
		cout << " gained " << battle.getXpReward() << " XP!";
		CinPause();
		for (NPC* teammate : *party) {
			if (teammate->getLevelUp()) {
				cout << "\n" << teammate->getName() << " leveled up! " << teammate->getName() << " is now Level " << teammate->getLevel() << "!";
				//I really want this to show stat changes
				teammate->setLevelUp(false);
				CinPause();
			}
		}
		npc->defeat();
	}
	PrintRoomData(currentRoom);
}

void printHelp(char validCommands[15][255], char flavorText[16][255]) {
	cout << "\n";
	cout << flavorText[rand() % 16];
	cout << "\nValid commands:";
	for (int i = 0; i < 15; i++) {
		cout << "\n" << validCommands[i];
	}
}

int main() {
	srand(time(NULL));
	
	vector<Room*>* rooms = new vector<Room*>;
	
	//sets up the game world and places the player at the current room
	NPC* self = SetupWorld(rooms);
	Room* currentRoom = self->getHome();

	vector<Item*>* inventory = new vector<Item*>;
	vector<NPC*>* party = self->getParty();

	//party->push_back(self);

	int mony = 0; //monies are the currency in the BURGER QUEST multiverse.

	//command stuff used to be initialized here

	char flavorText[16][255] = {
		"What even is a BURGER?",
		"You consider the state of the economy.",
		"You are blue dabadeedabadai.",
		"You are beginning to believe.",
		"You forgor.",
		"You accidentally find spoilers for the ending of BURGER QUEST 2. It is very concerning...",
		"You realize you don't have an oven.",
		"You spot a billboard advertising the new BURGER RESTAURANT in BURGERSBURG.",
		"You ask a passerby what the valid commands are. The guy looks at you really confused.",
		"You stop it and get some help.",
		"We have been trying to reach you about your car's extended warranty.",
		"You spot a quick brown fox jumping over a lazy dog.",
		"You say hello to the world.",
		"I could really go for a burger right now. Too bad they only sell BURGERs here.",
		"You have a cake and eat it too.",
		"You take a potato chip... and eat it."
	};

	char validCommands[15][255] = {
		"GO [direction]",
		"TAKE [item]",
		"DROP [item]",
		"USE [item] (ON [npc])",
		"RECRUIT [npc]",
		"DISMISS [npc]",
		"ASK [npc]",
		"INVENTORY",
		"PARTY",
		"ROOM",
		"ANALYZE [npc/item]",
		"FIGHT [npc]",
		"BUY [item]",
		"HELP",
		"QUIT"
	};

	cout << "Welcome to BURGER QUEST 2: ELECTRIC BOOGALOO\nYou're going on a quest to get a BURGER (not to be confused with a burger).\nType HELP for help.\n";

	cout << "\n             (type your name here!)\nYour name is ";

	char name[255];
	//uncomment this, I just don't want to type the name every time I want to test something
	//cin.getline(name, 255);

	AllCaps(&name[0]);

	if (!strcmp(name, "")) {
		cout << "\nSELF - \"Ok I guess I just don't have a name then.\"";
		//uncomment this too
		//CinPause();
	} else {
		self->setName(name);
	} 
	if (!strcmp(name, "BERNARD")) {
		cout << "\nBERNARD - \"Oh wow that's my actual name!\"";
		CinPause();
	} else if (!strcmp(name, "HELP")) {
		cout << "\nHELP - \"BRO are you serious? Now my name is Help... :(\"";
		CinPause();
	} else if (!strcmp(name, "HELP FOR HELP") || !strcmp(name, "YOUR NAME") || !strcmp(name, "YOUR NAME HERE") || !strcmp(name, "YOUR NAME HERE!")) {
		cout << "\n" << name << " - \"Well, you're very good at following instructions...\"";
		CinPause();
	//probably just make this an annoying string of text that you can only realistically copy/paste in
	} else if (!strcmp(name, "BAGEL ADMINISTRATOR")) {
		cout << "\nDeveloper mode activated!";
		CinPause();
	} //have devmode bool, and do additional command checks for DEFEAT [npc], UNLOCK [exit], FORCERECRUIT [npc], GETMONEY, etc.
	
	CinIgnoreAll();

	PrintRoomData(currentRoom);

	bool continuing = true;
	while (continuing) {
		char command[255] = "";

		char commandWord[255];
		char commandExtension[255];

		cout << "\n> ";
		cin.getline(command, 255);
		AllCaps(command);

		ParseCommand(command, commandWord, commandExtension);

		if (!strcmp(commandWord, "GO")) {
			travel(currentRoom, &commandExtension[0], party);
		} else if (!strcmp(commandWord, "TAKE")) {
			takeItem(currentRoom, inventory, &commandExtension[0]);
		} else if (!strcmp(commandWord, "DROP")) {
			dropItem(currentRoom, inventory, &commandExtension[0]);
		} else if (!strcmp(commandWord, "USE")) {
			useItem(currentRoom, inventory, &commandExtension[0], mony);
		} else if (!strcmp(commandWord, "RECRUIT")) {
			recruitNPC(currentRoom, &commandExtension[0], party);
		} else if (!strcmp(commandWord, "DISMISS")) {
			dismissNPC(currentRoom, &commandExtension[0], party);
		} else if (!strcmp(commandWord, "ASK")) {
			printNPCDialogue(currentRoom, &commandExtension[0]);
		} else if (!strcmp(commandWord, "INVENTORY")) {
			printInventory(inventory, mony);
		} else if (!strcmp(commandWord, "PARTY")) {
			printParty(party);
		} else if (!strcmp(commandWord, "ROOM")) {
			PrintRoomData(currentRoom);
		} else if (!strcmp(commandWord, "ANALYZE")) {
			analyze(currentRoom, &commandExtension[0], party, inventory);
		} else if (!strcmp(commandWord, "FIGHT")) {
			fight(currentRoom, party, inventory, &commandExtension[0], mony);
		} else if (!strcmp(commandWord, "BUY")) {
			buy(currentRoom, inventory, &commandExtension[0], mony);
		} else if (!strcmp(commandWord, "HELP")) {
			printHelp(validCommands, flavorText);
		} else if (!strcmp(commandWord, "QUIT")) {
			continuing = false;
		} else {
			cout << "\nInvalid command \"" << commandWord << "\" (type HELP for help).";
		}

		CinIgnoreAll();
	}
	//gives a friendly farewell to the player
	cout << "\nEnjoy your next 24 hours.\n";

}
