/* Tomas Carranza Echaniz
*  12/5/25
*  This program is an epic text-based RPG where you can GO between rooms, TAKE, DROP, and USE items, ASK, RECRUIT,
*  DISMISS, and FIGHT npcs, and you're on a QUEST TO get a BURGER. There's a bunch of other commands as well.
*  
*  In case you missed the first BURGER QUEST, you can play it at https://codehs.com/sandbox/id/zuul-kb8vTu/run
*/
#include <iostream>
#include <iomanip>
#include <vector>
#include <cstring>
#include <algorithm>
#include <ctime>
#include "NPC.h"
#include "Room.h"
#include "Item.h"
#include "Battle.h"
#include "Helper.h"
#include "Attack.h"
#include "Effect.h"
#include "Conversation.h"
#include "Stats.h"
#include "Save.h"

using namespace std;
using namespace Helper; //my Helper namespace has a bunch of helpful functions that I also use in other files

//sets up the entire game world, including rooms, npcs, and items, and returns the player character
NPC* SetupWorld(vector<Item*>* inventory) {
	//set up directions MARK: make directions
	const char* NORTH = "NORTH";
	const char* SOUTH = "SOUTH";
	const char* WEST = "WEST";
	const char* EAST = "EAST";
	const char* NORTHWEST = "NORTHWEST";
	const char* NORTHEAST = "NORTHEAST";
	const char* SOUTHWEST = "SOUTHWEST";
	const char* SOUTHEAST = "SOUTHEAST";
	const char* UP = "UP";
	const char* DOWN = "DOWN";
	const char* IN = "IN";
	const char* OUT = "OUT";
	const char* IN_TENT_1 = "IN TENT 1";
	const char* IN_TENT_2 = "IN TENT 2";
	const char* IN_TENT_3 = "IN TENT 3";
	const char* IN_TENT = "IN TENT";
	const char* IN_HOUSE_1 = "IN HOUSE 1";
	const char* IN_HOUSE_2 = "IN HOUSE 2";
	const char* IN_HOUSE_3 = "IN HOUSE 3";
	const char* IN_FACTORY = "IN FACTORY";
	const char* IN_CASTLE = "IN CASTLE";
	const char* IN_ALLEY = "IN ALLEY";
	const char* IN_ELEVATOR = "IN ELEVATOR";
	const char* IN_SAFE = "IN SAFE";
	const char* IN_TEMPLE = "IN TEMPLE";
	const char* IN_ROOM_1 = "IN ROOM 1";
	const char* IN_ROOM_2 = "IN ROOM 2";
	const char* IN_ROOM_3 = "IN ROOM 3";
	const char* IN_ROOM = "IN ROOM";
	const char* IN_BACK_ROOM = "IN BACK ROOM";
	const char* INSIDE = "INSIDE";
	const char* OUTSIDE = "OUTSIDE";
	const char* UPSTAIRS = "UPSTAIRS";
	const char* DOWNSTAIRS = "DOWNSTAIRS";
	const char* UNDERGROUND = "UNDERGROUND";
	const char* ABOVEGROUND = "ABOVEGROUND";
	const char* FORWARD = "FORWARD";
	const char* TO_THE_TOP = "TO THE TOP";
	const char* TO_GROUND_LEVEL = "TO GROUND LEVEL";
	const char* TO_THE_BOTTOM = "TO THE BOTTOM";
	const char* TO_FLOOR_1 = "TO FLOOR 1";
	const char* TO_FLOOR_2 = "TO FLOOR 2";
	const char* IN_THE_HOLE = "IN THE HOLE";
	//fast travel directions
	const char* TO_THE_VILLAGE = "TO THE VILLAGE";
	const char* TO_THE_DESERT = "TO THE DESERT";
	const char* TO_THE_HIGHLANDS = "TO THE HIGHLANDS";
	const char* TO_BURGERSBURG = "TO BURGERSBURG";
	const char* TO_THE_BASEMENT = "TO THE BASEMENT";

	//set the direction reverser map in Helper MARK: reverse directions
	ReverseDirection[NORTH] = SOUTH;
	ReverseDirection[SOUTH] = NORTH;
	ReverseDirection[EAST] = WEST;
	ReverseDirection[WEST] = EAST;
	ReverseDirection[UP] = DOWN;
	ReverseDirection[DOWN] = UP;
	ReverseDirection[ABOVEGROUND] = UNDERGROUND;
	ReverseDirection[UNDERGROUND] = ABOVEGROUND;
	ReverseDirection[NORTHEAST] = SOUTHWEST;
	ReverseDirection[NORTHWEST] = SOUTHEAST;
	ReverseDirection[SOUTHEAST] = NORTHWEST;
	ReverseDirection[SOUTHWEST] = NORTHEAST;

	//set up blockage reaons MARK: blockage reasons
	const char* ENEMY = "ENEMY";
	const char* CHASM = "CHASM";
	const char* RIVER = "RIVER";
	const char* TEMPLE = "TEMPLE";
	const char* RUBBLE = "RUBBLE";
	const char* TUNNEL = "TUNNEL";
	const char* TRACK = "TRACK";
	const char* LOCK = "LOCK";
	const char* NINJA = "NINJA";
	const char* HIGH = "HIGH";
	const char* SAND = "SAND";
	const char* HEAT = "HEAT";
	const char* LAVA = "LAVA";
	const char* STUFF = "STUFF";
	const char* COVER = "COVER";
	const char* FIRE = "FIRE";

	//I send all the template enemy NPCs and also shop items to limbo, since I need to set a room for them MARK: make rooms
	Room* limbo = new Room("not supposed to be in this room... if you break something by editing your save that's not my fault.\nHere I put all the stuff that's not in any accessible room,\nstuff like the original copy of things you can buy,\nor enemy templates.");

	//the player! defined here so the temple opening dialogues can use the player name
	NPC* self;

	//create all WANING WOODLANDS rooms MARK: WW
	Room* village = new Room("in Tactical Tent Village, your home village of tipi tents.\nIt's a beautiful day; perfect for staying indoors and gaming.");
	Room* villageleft = new Room("at the westernmost end of the village, where the second-tallest tent stands.\nIt's only two stories, but it's comparatively a tent mansion.");
	Room* tentstore = new Room("in the village convenience store. No other store is more convenient, or so they say.");
	Room* tentmansion = new Room("in the tent mansion's living room. There are way too many clocks here.");
	Room* tentlab = new Room("in the tent lab. There's a ton of machinery, and many generic science beakers with colored liquids.");
	Room* tentstation = new Room("in the village train station. The tunnels were closed off recently due to the lobster infestation.");
	tentstation->setStation();
	Room* tentchurch = new Room("in the village church. It's a really big tent, complete with stained glass and everything.\nNobody really goes here anymore, since the priest mysteriously disappeared a long time ago.");
	Room* docks = new Room("at the village docks. Your tent house is set up here because your dad likes fishing.");
	Room* tenthouse = new Room("in your family's house, in what could be considered the multiuse bed/dining/living room.");
	Room* forestentrance = new Room("at the entrance of the woodlands.");
	Room* forest = new Room("deep in the woodlands. Smells like pine trees along the way.");
	forest->setWelcome({{NULL, "Welcome to WANING WOODLANDS!"},
						{NULL, "<<< THE FINAL FOREST >>>"},
						{NULL, "The slowly decaying corner of the world, where you have lived your whole life."},
						{NULL, "The grass is green and the chirds are birping."},
						{NULL, "Now, your BURGER QUEST begins!"}});
	Room* forestleft = new Room("walking down the left path of the woodlands. It feels like a deer is watching you.");
	Room* forestright = new Room("walking down the right path of the woodlands. It feels like a ninja is watching you.");
	Room* deerclearing = new Room("in the deer clearing, where deer frequently gather."); //talk to the deer to get the key
	Room* ninjaland = new Room("underneath the ninja village. It's probably supposed to be hidden but you looked up and there it was.");
	Room* ninjavillage = new Room("in the ninja village. There's no bridges connecting the houses, you must simply jump between them like a true ninja.");
	Room* ninjacapitol = new Room("in the chief ninja's abode. There are many ninja weapons and scrolls up on the walls.");
	Room* ninjapantry = new Room("in the ninja storage unit. The ninjas live on a strict diet of ninjaberries and ninjasteak and ninjafish and the diet isn't actually that strict.");
	Room* ninjaforge = new Room("in the ninja forge. There are many ninja weapons and molds on the wall.");
	Room* foresttempleentrance = new Room("in a wide sunny glade, at the entrance of an ancient forest temple.\nIt's built in an octagonal shape with dark green bricks.");
	Room* foresttemplestairs = new Room("on the steps going into the ancient forest temple.");
	foresttempleentrance->setTempleEntrance(IN_TEMPLE, foresttemplestairs,
		{{self, "Hi forest temple can you please open?"},
		 {NULL, "The temple rumbles..."},
		 {NULL, "The temple door starts lowering into the ground..."},
		 {NULL, "Ancient wind from within the temple blows against the trees..."},
		 {NULL, "The door grinds to a halt."},
		 {NULL, "The forest temple has been opened!"},
		 {NULL, "You can go IN TEMPLE now!"}});
	Room* flowerfield = new Room("in the aromatic flower fields. Your sister likes hanging out here.");
	Room* flowerfield2 = new Room("deep in the flower fields. A really nice river flows over here.");
	Room* forestfork = new Room("at another fork in the road. I hope we don't come across a spoon in the road.");
	Room* forestgarden = new Room("a strange section of the forest. All the trees have been neatly trimmed and organized to an unnatural extent.");
	Room* forestgate = new Room("at the great forest wall. It seperates the inner forest and the rest of the world.");
	Room* forestwall = new Room("walking along the southern side of the wooded wall. It's very misty here.");
	Room* forestgrave = new Room("in a sparse wooden graveyard.");
	Room* forestspork = new Room("at a spork in the road, but no sporks are to be seen anywhere.");
	Room* forestnice = new Room("at a really nice looking area of the forest. The lighting is very cool.");
	Room* treasuregrove = new Room("in the treasure grove, where the grass is golden and treasure chests sometimes appear.");
	Room* treasurecliff = new Room("in the treasure cliff at the edge of the desert, where there is actually treasure.");
	Room* bossgrove = new Room("in the BOSS GROVE, where the boss of WANING WOODLANDS is known to appear.");
	Room* fdintermission1 = new Room("on the path between the woodlands and the wastelands.");
	Room* fdintermission2 = new Room("on the path between the woodlands and the wastelands. The foliage is sparse here. BURGERSBURG can be seen faintly in the distance.");
	Room* fdintermission3 = new Room("on the path between the woodlands and the wastelands. Dead trees surround you. The BURGER RESTAURANT is just barely visible from here.");
	//Create all DESOLATE DESERT rooms MARK: DD
	Room* desert = new Room("in the wastelands. There is no sign of the color green anywhere.");
	desert->setWelcome({{NULL, "Welcome to DESOLATE DESERT!"},
						{NULL, "<<< THE WASTELANDS BEYOND >>>"},
						{NULL, "The world beyond your forest, where the life has been sucked out of the dirt."},
						{NULL, "The sun beats down with no clouds in sight."},
						{NULL, "Surely there must be someone friendly around here?"}});
	Room* deserttempleentrance = new Room("on a large dune where the point of an ancient desert temple pokes out of the sand.");
	Room* deserttemplestairs = new Room("on the steps going into the ancient desert temple.");
	deserttempleentrance->setTempleEntrance(IN_TEMPLE, deserttemplestairs,
		{{self, "Hi desert temple can you please open?"},
		 {NULL, "The temple rumbles..."},
		 {NULL, "The temple door starts lowering into the ground..."},
		 {NULL, "The sand beneath your feet is shaking..."},
		 {NULL, "Sand from within the temple blasts against your face..."},
		 {NULL, "The door grinds to a halt."},
		 {NULL, "The desert temple has been opened!"},
		 {NULL, "You can go IN TEMPLE now!"}});
	//temple stuff
	Room* desertdune = new Room("at a low point in the desert. A rare shadow is present where you can rest.");
	Room* desertplain = new Room("at a really flat area of the desert.");
	Room* desertgrave = new Room("deep in the desert. There's some sort of bovine skeleton here.");
	Room* desertpole = new Room("deep in the desert. All you can see for miles is sand.");
	Room* deserthill = new Room("on a very high dune. It'd be fun to roll all the way down, but your hair would be very annoying to clean afterwards.");
	Room* deserttown = new Room("in an abandoned desert town. Everything still seems to be organized normally, though.");
	Room* desertshop = new Room("in an abandoned store. The products are on the verge of expiration.");
	Room* desertgym = new Room("in an abandoned gym. There's all the equipment you could ask for here, but no rock wall.");
	Room* deserthouse = new Room("in some house with zero purpose. It only exists because the town would feel weird with only two houses."); //I have no idea what to put in this house
	Room* desertshopfixed = new Room("in the desert store. It smells like pastries and medicine.");
	Room* desertgymfixed = new Room("in the desert gym. Now that it's back in business it's time to get on that GRIND!\nSimply DISMISS your teammates here to make them stay in shape while you're gone!");
	desertgymfixed->setGym();
	Room* oasis = new Room("in an oasis, presumably the town's source of water. The water is long gone and the plants are dry husks.");
	Room* canyon = new Room("in a small canyon that cuts into a plateau. There's an old-timey entrance to a train station here.");
	Room* thatcliff = new Room("on this cliff over here."); //Viola goes here after being defeated
	Room* canyon1 = new Room("in a long, shaded canyon, forming a fork in the road. You know, these would actually be really annoying forks if you think about it.");
	Room* canyon2 = new Room("at the end of the canyon, though a path seems to have been made upwards.");
	Room* canyon3 = new Room("at the end of the canyon. There's much evidence of mining here, and a mineshaft entrance here leads underground.");
	Room* cliff0 = new Room("very high up on a cliff; the temperature almost feels normal!");
	Room* cliff1 = new Room("on a trail leading up to the top of the cliffside. You can see the desert town from here.");
	Room* cliff2 = new Room("on the highest cliff. The desert townsfolk are suspended in the air around a girl with a purple dress.");
	Room* mineshaft = new Room("underground in the mineshaft. It was too hot outside, but now it's very cold :(");
	Room* mineshaft2 = new Room("at a pickaxe in the road. That's not a utensil...");
	Room* mineshaft3 = new Room("deep in the mineshaft. The rock looks extra rocky here.");
	Room* minelight = new Room("pretty high up, though still underground. You can see the light at the end of the tunnel just up above; how lovely.");
	Room* mineexit = new Room("at the high-up exit of the mineshaft. SO MANY STAIRS...");
	Room* mineshaftside = new Room("in a side tunnel of the mineshaft. There's a bunch of loose minecarts here.");
	Room* kaboomroom = new Room("in a very unstable looking room. It's filled to the brim with dynamite, which is concerning considering that maniac that hangs out here.");
	Room* berryroom = new Room("in a hidden room full of subterranean cactus.");
	Room* volcanoentrance = new Room("at the northernmost end of the mineshaft. Burning light radiates through the mineshaft entrance.");
	Room* desertstation = new Room("in a train station, which seems to have been accidentally intercepted by the mineshaft.");
	desertstation->setStation();
	Room* deserttunnel = new Room("in a long, dark train tunnel. You've never seen one that goes on for so long with no collapse.");
	deserttunnel->setStation();
	Room* minespring = new Room("at the underground spring under the town oasis. There is a mess of pipes managing the water.");

	//Create all HELLISH HIGHLANDS rooms MARK: HH
	Room* volcano = new Room("in the scorched highlands just before BURGERSBURG. It's very hot.");
	volcano->setWelcome({{NULL, "WELCOME TO HELLISH HIGHLANDS!"},
						 {NULL, "<<< THE END OF THE ROAD >>>"},
						 {NULL, "Radiation from the lava burns against your skin."},
						 {NULL, "The BURGER RESTAURANT can be seen clearly, its image heavily distorted by the heat."},
						 {NULL, "You're almost there."}});
	Room* volcano1 = new Room("in the volcanic highlands. An old, abandoned factory stands to the EAST.");
	Room* volcano2 = new Room("in a volcanic plain. The ashen remains of some plants can be seen here.");
	Room* volcano3 = new Room("in the scorched remains of an old factory town.\nThere's stairs leading down to a train station.");
	Room* volcanostation = new Room("in the factory town train station. Some lava has settled here, but the tunnels are probably still usable.");
	volcanostation->setStation();
	Room* volcano4 = new Room("on a scorched path. It reminds you of pepperoni pizza.");
	Room* volcano5 = new Room("at a... knife in the road.");
	Room* volcano6 = new Room("in a volcanic valley. Another factory stands here, more charred than the rest, yet still holding up.");
	Room* volcano7 = new Room("at the very edge of the volcanic valley. An huge white bridge forms a road to the city.");
	//first factory, very small
	Room* factory1 = new Room("in the factory, on a small grated bridge over a pool of lava.");
	Room* factoryplat = new Room("on a platform overlooking the factory. The control room is just down the hall.");
	Room* factorykitchen = new Room("in a makeshift kitchen made from a factory observation room.\nWhatever was made here before, it now only churns out slag.");
	Room* controlroom1 = new Room("in the factory control room. The machinery is in rather good condtion.");
	//second factory
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
	Room* factorybalcony2 = new Room("on a balcony overlooking the central area.\nYou can also see a garden room of sorts from here, though there is no path to it.");
	Room* factorygarden = new Room("in a garden which is not wheelchair-accessible.\nThe plants look very unhealthy; they seem to have been experimenting with nuclear technology here.");
	Room* conveyor3 = new Room("on an assembly line! This one makes a path to the control room.");
	Room* controlroom2 = new Room("in the factory control room. The machinery looks usable depsite the lava.");
	conveyor1->setConveyor(factorycenter, FORWARD);
	conveyor2->setConveyor(factorystorage, FORWARD);
	conveyor3->setConveyor(factorycenter, FORWARD);
	conveyor4->setConveyor(factorystorage, FORWARD);
	conveyor5->setConveyor(factorystorage, FORWARD);
	//third factory
	Room* factory3 = new Room("in the charred factory. The first floor is a large storage room, though its contents are long burnt to a crispy crisp.");
	Room* factorynw = new Room("in the factory storage room. There's what appears to be the remains of a chair here.");
	Room* factoryne = new Room("in the factory storage room. You can see the assembly line through a corner window, still going strong after so much time submerged in lava.");
	Room* factoryse = new Room("in the factory storage room. You see heavy machinery poking out of the EASTern room. I love heavy machinery!");
	Room* heavymachineryroom = new Room("in a room full of heavy machinery. Some of them are still operational.");
	Room* factoryroofsw = new Room("poking out of the factory. The roof has caved in here.");
	Room* factoryroofnw = new Room("high up in the factory. You can see the control room just up ahead.");
	Room* controlroom3 = new Room("in the factory control room. This one's machinery is very rusty. Very icky.");
	Room* factoryroofne = new Room("high up in the factory. You see a short hallway. It's a really nice hallway.");
	Room* factoryhallway = new Room("at the end of the hallway. A ladder leads high upwards.");
	Room* factorytower = new Room("at the top of the factory tower. It has a very big computer setup with many monitors and smells like energy drink.");
	Room* factoryroofse = new Room("high up in the factory. You see a corridor full of employee of the month awards.");
	Room* factorytreasure = new Room("at the end of the corridor. JIM SHADY is the employee of the year.");
	//the castle
	Room* castleentrance = new Room("at the entrance of a large, crystaline castle. It has a very angular design.");
	Room* castlehall = new Room("in the main hall of the castle. Ornate pillars and paintings of internet browsers pave the way towards the throne room.");
	Room* castlethrone = new Room("in the throne room of the castle. The ceiling is reeeeeeally high up, and the throne almost reaches the top!");
	//the sewers
	Room* sewerentrance1 = new Room("on a ladder leading down to the sewer system.");
	Room* sewerentrance2 = new Room("deep down on the ladder to the sewer system.");
	Room* sewer = new Room("in the sewer system below the wastelands. Smells like you should see a pulmonologist.");
	Room* sewer2 = new Room("in the sewer system below the wastelands. Lava flows through the canal; better watch your step!");
	Room* sewercenter1 = new Room("in the huge central room of the sewers. The architecture here has green accents."); //south
	Room* sewercenter2 = new Room("in the huge central room of the sewers. A massive lake of lava lies under your feet."); //north
	Room* sewercenter3 = new Room("in the huge central room of the sewers. Natural stone is exposed here."); //east
	Room* sewercenter4 = new Room("in the huge central room of the sewers. A large green doorway stands here."); //west
	Room* sewerplant = new Room("in the main control room of the sewers. How does one maintain this lava maintenence system?");
	Room* sewermineswest = new Room("at the easternmost end of a long minecart track. You can't see the end.");
	Room* sewerminessouth = new Room("in a peculiar offshoot from the sewers which someone mined out.");
	Room* mineshortcut = new Room("in stable room, full of mining supplies and explosives.");
	Room* sewertreasure = new Room("in a rocky cave full of bones and fancy ores.");
	//the volcano temple
	Room* volcanotempleentrance = new Room("in a massive cavern, at the door of an ancient volcanic temple.\nThe rock burns bright red.");
	Room* volcanotemplestairs = new Room("on the steps going into the ancient volcanic temple.");
	volcanotempleentrance->setTempleEntrance(IN_TEMPLE, volcanotemplestairs,
		{{self, "Hi volcano temple can you please open?"},
		 {NULL, "The temple rumbles..."},
		 {NULL, "The temple door starts lowering into the ground..."},
		 {NULL, "Cracks start to form in the rock around you..."},
		 {NULL, "The rumbling brings forth lava from the ground..."},
		 {NULL, "The door grinds to a halt."},
		 {NULL, "The volcano temple has been opened!"},
		 {NULL, "You can go IN TEMPLE now!"}});
	//the bridge to BURGERSBURG
	Room* bridge1 = new Room("on the final bridge to BURGERSBURG. The lava ocean crackles beneath your feet.");
	Room* bridge2 = new Room("halfway through the bridge. An eternal night looms over the city.");
	Room* bridge3 = new Room("on the far end of the bridge. The gate into BURGERSBURG stands right up ahead.");

	//Create MOUNT MINBUS rooms. This place is basically just a small boss arena MARK: MM
	Room* mountain = new Room("on a mountain, far, far away from anywhere you know. Dark clouds block view of both the sky and land.");
	mountain->setWelcome({{NULL, "WELCOME TO MOUNT MINBUS!"},
						  {NULL, "<<< THE EDGE OF THE WORLD >>>"},
						  {NULL, "Frigid winds blast your face and dark clouds cover both land and sky."},
						  {NULL, "Ok have fun!"}});
	Room* mountainmine = new Room("far away from the start of the track. A barrage of snow blows in from the exit.");
	Room* mountain2 = new Room("on a little clearing on the mountain. The UPwards path has a ski lift!");
	Room* mountainside = new Room("along a side path of the mountain. The snow is snowier than before.");
	Room* mountainlake = new Room("at a lake full of baby shrimp. The shrimple aura keeps the water from freezing.");
	Room* mountain3 = new Room("at a sheer cliff. Whoever's in charge of this place should really invest in guardrails.");
	Room* mountainpeak = new Room("at the peak of the mountain, watching over a sea of clouds.\nYou can't even see the BURGER RESTAURANT from here; it really is so far away.\nThere's a tent here in the style of your home village.");
	Room* tenthome = new Room("in the developer's house. He is working on BURGER QUEST 2 DLC.");

	//Create all BURGERSBURG rooms MARK: BB
	Room* BURGERSBURG = new Room("at the gate of BURGERSBURG. The BURGER RESTAURANT is just down main street.");
	BURGERSBURG->setWelcome({{NULL, "Welcome to BURGERSBURG!"},
							 {NULL, "<<< THE CAPITAL OF SIN >>>"},
							 {NULL, "Massive skyscrapers blot out the sun, and the BURGER RESTAURANT towers above them all."},
							 {NULL, "All but a few buildings are boarded up."},
							 {NULL, "You don't want this city knowing where you live."},
							 {NULL, "We are not responsible for theft or damage to vehicles or contents!"}});
	Room* leftstreet1 = new Room("in the poor side of the city. It's relatively tranquil here since everything's already been stolen.");
	Room* leftstreet2 = new Room("next to a building with a fish sign. There is an uncharacteristic sense of calm here.");
	Room* leftstreet3 = new Room("on the sidewalk, avoiding a burning crack in the road.");
	Room* leftstreet4 = new Room("at the entrance to a glowing casino. One of the few functioning buildings here.");
	Room* leftstreet5 = new Room("at the edge of the regular district. The streets past this point look well-maintained.");
	Room* newstreet1 = new Room("at a construction site. This thing is never getting completed.");
	Room* newstreet2 = new Room("at the entrance to a convenience store. The door is intact! And all the windows are broken.");
	Room* newstreet3 = new Room("in an area behind the buildings, where there's an entrance to a dark alley.\nEven the dumpsters have all been stolen from.");
	Room* newstreet4 = new Room("at the city orphanage, scheduled for demolition.");
	Room* newstreet5 = new Room("at the edge of the regular district. The buildings beyond look really well-maintained.");
	Room* mainstreet1 = new Room("on main street. There's a few cars above lodged in the side of the buildings.");
	Room* mainstreet2 = new Room("on main street. A lopsided helicopter struggles to spin its rotors.");
	Room* mainstreet3 = new Room("on main street. The traffic lights are all broken, but the random fires provide cozy lighting.");
	Room* mainstreet4 = new Room("on main street. There's a stairway here leading down to the BURGERSBURG train station.");
	Room* mainstreet5 = new Room("at the end of main street. The BURGER RESTAURANT looms ahead of you, welcoming you inside.");
	Room* coolstreet1 = new Room("near the entrance of the city. The top of a building is sliced clean off.");
	Room* coolstreet2 = new Room("at the entrance to an apartment building. This one's door is open.");
	Room* coolstreet3 = new Room("at a functional hot dog stand.");
	Room* coolstreet4 = new Room("at the entrance to a dark alley. Looks pretty dark.");
	Room* coolstreet5 = new Room("next to some copy-pasted looking townhouses. Surely homebuilders can be more creative?");
	Room* rightstreet1 = new Room("in the crumbling corner of the city. The lava sea radiates light onto exposed infrastructure.");
	Room* rightstreet2 = new Room("still in the city. You hear a random explosion, business as usual.");
	Room* rightstreet3 = new Room("at a really nice brick building. You see a guy in a black rat costume fighting an old lady.");
	Room* rightstreet4 = new Room("at a small glass store. You see two guys carrying a large pane of glass.");
	Room* rightstreet5 = new Room("at the entrance to the BURGERSBURG fire department. It hasn't seen much use recently.");
	Room* richneighborhood1 = new Room("in the rich people corner of town. Each huge building corresponds to just one person.");
	Room* richneighborhood2 = new Room("in the rich neighborhood. The security systems put you on guard;\nas soon as you step on a lawn, you'd probably be blown to smithereens.");
	Room* richneighborhood3 = new Room("at a secluded corner of the rich neighborhood. The building here has a front yard.");
	Room* richneighborhood4 = new Room("at the doorway of a huge layered complex, the BURGER corporation's headquarters.");
	//city interiors
	Room* burgfish = new Room("in the fish building. A warm light shines down the stairs.");
	Room* burgchurch = new Room("in a hidden church. It's more minimalistic than the one back home.");
	Room* burgstore = new Room("in an abandoned store. Nobody bothers to stock the shelves anymore.");
	Room* casino = new Room("in the casino. Sounds of slot machines and flashing lights overload your senses.\nYou should really leave before you develop a gambling addiction.");
	Room* casinoback = new Room("in the casino's back room. There's a staircase heading to the basement,\nand crates full of gambling addicts' monies.");
	Room* casinobase = new Room("in the casino's basement, housing the massive energy core required to power all the lights.");
	Room* darkalley = new Room("in a dark alley, a characteristic of those about to be mugged.");
	Room* bankalley = new Room("in a dark alley. There is a really shady bank set up here.\nIf you'd like to deposit your monies, simply USE BANK."); //shady as in it's dark
	Room* shrimpartment1 = new Room("in an apartment building. There's a spiral staircase going all the way up.");
	Room* shrimpartment2 = new Room("on the second floor. The wallpaper has a shrimp pattern.");
	Room* shrimpartment3 = new Room("on the third floor. There is a cat poster here.");
	Room* shrimproof = new Room("on the apartment rooftop. You have a nice view of the city lights.");
	Room* firedepartment = new Room("in the fire department. The fire alarm is sadly whimpering.");
	Room* burgstation = new Room("in the BURGERSBURG train station.\nOddly enough, this one seems in the best condition out of all the ones you've seen.");
	burgstation->setStation();
	Room* ceolobby = new Room("in the ornate lobby of the building, featuring both an elevator and fancy stairs.\nThe yearly RICH PEOPLE reunion is ongoing.");
	Room* ceolobby2 = new Room("on the second floor of the building, still the lobby! There's some huge doors here.");
	Room* ballroom = new Room("in the BURGER ballroom, surrounded by a sea of rich people.");
	Room* ceoelevator1 = new Room("in the elevator, at ground level.");
	Room* ceoelevator2 = new Room("in the elevator, on the second floor.");
	Room* ceoelevator3 = new Room("in the elevator, at the top level with the CEO's office.");
	ceoelevator2->shareItems(ceoelevator1); //they have the same items since it's the same elevator going up and down
	ceoelevator3->shareItems(ceoelevator1);
	Room* ceoroom = new Room("in the BURGER CEO's office. The desk stands in front of the BURGER SAFE, where all the company valuables are held.");
	Room* burgsafe = new Room("in the BURGER SAFE. Countless monies and company documents are piled up here.");
	Room* elevatorentrance = new Room("in the entrance of the BURGER RESTAURANT. It has glass walls curving up and BURGER furniture.\nThe elevator is not currently on ground level.");
	Room* elevator = new Room("in the elevator of the BURGER RESTAURANT. It's a really fancy circular elevator, with a 360 degree view of the city.");
	Room* elevatortop = new Room("in the elevator, elevated all the way to the tippity top.");
	elevatortop->shareItems(elevator);
	elevatortop->setWelcome({{NULL, "The elevator shoots upwards."},
							 {NULL, "..."},
							 {NULL, "..."},
							 {NULL, "..."},
							 {NULL, "Neon BURGER billboards attached to the elevator shaft pass by."},
							 {NULL, "You pass the highest peaks of the BURGERSBURG skyline."},
							 {NULL, "..."},
							 {NULL, "..."},
							 {NULL, "..."},
							 {NULL, "You pass the cloud level, leaving behind the gloom of BURGERSBURG."},
							 {NULL, "The sun is shining over the horizon."},
							 {NULL, "..."},
							 {NULL, "..."},
							 {NULL, "..."},
							 {NULL, "The elevator dings."},
							 {NULL, "You have arrived at your destination."}});
	Room* BURGERRESTAURANT = new Room("in the BURGER RESTAURANT. You can see the sun barely shining over the horizon.\nThe BURGER MAN is waiting for you to order a BURGER.");
	Conversation burgwelcome = {{NULL, "BURGER MAN - \"HELLO VALUED CUSTOMER!\""}, {NULL, "BURGER MAN - \"WELCOME TO MY BURGER RESTAURANT!\""}};
	burgwelcome.skipcondition = {BEATCEO};
	BURGERRESTAURANT->setWelcome(burgwelcome);
	Room* elevatorbottom = new Room("in the elevator, deep down in the restricted level of the BURGER RESTAURANT.");
	elevatorbottom->shareItems(elevator);
	elevatorbottom->setWelcome({{NULL, "The elevator shoots downwards."},
								{NULL, "..."},
								{NULL, "..."},
								{NULL, "..."},
								{NULL, "Hard rock speeds by."},
								{NULL, "..."},
								{NULL, "..."},
								{NULL, "..."},
								{NULL, "It's so far down; the AC struggles to keep up with the rising temperature."},
								{NULL, "..."},
								{NULL, "..."},
								{NULL, "..."},
								{NULL, "The rock outside is glowing red hot..."},
								{NULL, "The elevator dings."}});
	Room* burgerbasement = new Room("at the entrance of the BURGER BASEMENT. The walls are a tealish gray, and the lights emit a constant hum.");
	Room* burgbasenw = new Room("in a sort of BURGER storage room, on a metal platform above a pit of BURGERs.");
	Room* burgbasene = new Room("at the entrance of a storage room. There is no physical door, just the doorway with a metallic outline.");
	Room* burgbasew = new Room("at the entrance of a basement room, labeled the cooler room.");
	Room* burgbasec = new Room("in the middle of the BURGER BASEMENT, featuring many control panels and security cameras.\nYou'd like to press all the buttons but they all need key access.");
	Room* burgbasee = new Room("at a side room of the basement, with a window overlooking the BURGER assembly line.");
	Room* burgbasesw = new Room("at an unremarkable corner of the BURGER BASEMENT.");
	Room* burgbases = new Room("at the entrance of a side room. It looks very high-tech in there.");
	Room* burgbasese = new Room("at a staircase heading down to the BURGER production platform.");
	Room* burgstorage = new Room("in the storage room. This seems to be where they kidnap people to.");
	Room* burgboiler = new Room("in the cooler room. The air conditioner is working overtime, complete with its own air conditioner.");
	Room* burglab = new Room("in the BURGER LABORATORY. There's many nasty looking needles and contraptions.\nThere's a UNIHORN here in quantumn chains.");
	Room* burgplatn = new Room("on the BURGER production platform, suspended inside an enormous cavern.\nYou see hanging factories endlessly churning out BURGERs.");
	Room* burgplate = new Room("next to the BURGER assembly line. There's some BURGER cultists cursing BURGERs as they pass by.");
	Room* burgplats = new Room("on the platform. There's some diagrams describing BURGERs here and a room to the side.");
	Room* BURGERPRISON = new Room("in the BURGER PRISON, full of cells and torture devices.");
	Room* basestation = new Room("in a deep train tunnel near the BURGER PRISON. The rock is very hot here.");
	basestation->setStation();
	
	//Create the temple rooms MARK: TEMPLES!
	Room* forestbuffer1 = new Room("at the bottom of the temple stairs. You must TAKE the ENTRY ORB to enter the temple.");
	Room* forestbuffer2 = new Room("fully in the forest temple. You must DROP the ESCAPE ORB if you wish to leave.");
	forestbuffer2->shareItems(forestbuffer1); //share items because it's the same room
	Room* foresttemple = new Room("in the forest temple.\nYou are presented with a CHOICE ORB.");
	Room* forestbranchw = new Room("on the western branch of the forest temple. Plants and purple smog seep through the walls.");
	Room* forestbranche = new Room("on the eastern branch of the forest temple. Plants and purple smog seep through the walls.");
	Room* foresttemple2 = new Room("in the forest temple, at a purple lake.\nYou are presented with a CHOICE ORB.");
	Room* forestbranchw2 = new Room("on the western branch of the forest temple. Carnivorous moss lines the walls and floor.");
	Room* forestbranche2 = new Room("on the eastern branch of the forest temple. Carnivorous moss lines the walls and floor.");
	Room* foresttemple3 = new Room("in the forest temple. Large purple flowers are flowering here.\nYou are presented with a CHOICE ORB.");
	Room* forestbranchw3 = new Room("on the western branch of the forest temple. The structural integrity of this hallway is questionable.");
	Room* forestbranche3 = new Room("on the eastern branch of the forest temple. The structural integrity of this hallway is questionable.");
	Room* foresttemple4 = new Room("in a long hallway of the forest temple. You don't see any flora past this point.");
	Room* foresttemple5 = new Room("at the end of the hallway. Lots of purple smog flows from the room to the NORTH.");
	Room* foresttempleboss = new Room("in a very spacious arena, filled with dense purple smog.\nThe smog is concentrated in the center.");
	Room* dirtplain = new Room("in the dirt plain where the forest temple entrance used to be.\nSoon, the grass will grow here again.");

	Room* deserttemple = new Room("in the desert temple.");

	Room* volcanotemple = new Room("in the volcano temple.");

	//Create the finale and post-game rooms MARK: TA
	NPC* burgermenace;
	Room* abyss = new Room("right outside the BURGER RESTAURANT in the depths of the world.");
	abyss->setWelcome({{NULL, "You are unbelievably deep underground."},
					   {NULL, "The high temperatures of the deep have been replaced with coldness."},
					   {NULL, "The BURGER RESTAURANT's advertisements illuminate the ground around you,"},
					   {NULL, "but you can't see the edge of the cavern at all."},
					   {NULL, "Welcome to..."},
					   {NULL, "<<< THE ABYSS >>>"}});
	Room* abyss2 = new Room("on the most walkable path forward. The rock around you looks like the moon.");
	Room* abyss3 = new Room("still on the path. The air around you is still."); //I wanted to say the air is still around you but that sounds like a confirmation that you're still in a place with air
	Room* abyss4 = new Room("at the edge of a steep ledge.");
	Room* abyss5 = new Room("in a crater of the abyss. The path forward continues.");
	Room* abyss6 = new Room("in the middle of the crater. You see a rock that looks like a BURGER.");
	Room* abyss7 = new Room("at the other end of the crater. The rocks look very climbable.");
	Room* abyss8 = new Room("past the abyssal crater. The BURGER RESTAURANT looks faint from here.");
	Room* abyss9 = new Room("at the bottom of a cliff; it's a pretty tall cliff.");
	Room* abyss10 = new Room("on the highest stretch of ground you can see.\nThe end of the path is right up ahead.");
	Room* aboss = new Room("at a sheer drop-off, staring down THE BURGER MENACE.");
	aboss->setWelcome({{NULL, "You're at the end of the abyssal path, at a sheer drop-off."},
					   {NULL, "You look over the edge."},
					   {NULL, "You only see the wall of the cliff heading into darkness."},
					   {NULL, "A faint light appears from the darkness..."},
					   {NULL, "Suddenly, from the depths flew up..."},
					   {NULL, "<<< THE BURGER MENACE >>>"},
					   {burgermenace, "I DIDN'T APPRECIATE WHAT YOU DID TO MY BURGER MAN,"},
					   {burgermenace, "SO I BROUGHT YOU DOWN HERE TO HAVE A CHAT FACE TO FACE."},
					   {self, "So everything that happened is your fault? >:|"},
					   {burgermenace, "IF YOU SAY SO."},
					   {burgermenace, "BUT YOUR DEATH,"},
					   {burgermenace, "YOU HAVE BROUGHT UPON YOURSELF."},
					   {NULL, "THE BURGER MENACE throws a BURGER TENDRIL at you like a speeding train."},
					   {NULL, "You put your hands on your hips."},
					   {self, ">:|"},
					   {NULL, "Suddenly..."},
					   {NULL, "You and your team are filled with the power of PLOT!"},
					   {NULL, "Your stats rose by 100000%!"},
					   {NULL, "You dodge the BURGER TENDRIL!"},
					   {burgermenace, "PROCRASTINATE DYING IF YOU WANT."},
					   {burgermenace, "YOU ARE NOTHING TO ME."}});
					   //BATTLE BEGIN!

	Room* timemachine = new Room("in the time machine! It's much bigger on the inside, complete with a donut-shaped layout.");
	Room* simulatorroom = new Room("in the simulator room, for simulating battles.");
	Room* vendingroom = new Room("in the vending machine room. It's a whole warehouse of vending machines for anything you can think of!");
	Room* statsroom = new Room("in the statistics room, for getting the statistics of your life.");
	
	Room* home = new Room("in Henry Jerry's multiuse bed/dining/living room.");
	Room* juiceroom = new Room("in Henry Jerry's portal juice room.");
	Room* yard = new Room("at Henry Jerry's backyard, in the void.");

	Room* tunnels = new Room("in the train tunnels that span the continent. The acoustics here are great!");
	tunnels->setStation();

	//Create NPCs and items MARK: make npcs, items, etc.
	self = new NPC("\0", "SELF", "The protagonist of BURGER QUEST 2, with a cool scarf and blond anime hair.\nIt's a me.", village, 0, Stats(20, 5, 6, 0, 0, 10, 9), Stats(1, 0, 1, 0, 0, 1, 0), true, true);
	npcChar[self] = 'a'; //self's character representation is a because I name him A so it's easier to type his name
	self->addRecruitedDialogue("Huh?"); //player defined above before all the rooms
	self->Recruit();
	self->addXp(3); //make it so the first enemy gives you just enough xp to level up

	Attack* punch = new Attack("PUNCH", "punched", true, -5, 10, 0, 1, 1, 1);
	punch->addDescription("Throw a simple punch at the target. (10 ATTACK)");
	self->setBasicAttack(punch);
	Attack* energyball = new Attack("ENERGY BALL", "threw an energy ball at", false, 3, 12, 10, 1, 1, 1, false, 1);
	energyball->addDescription("Throw a piercing ball of pure kinetic energy at the target. (12 ATTACK, 10 PIERCE)");
	self->addSpecialAttack(energyball);
	Attack* kick = new Attack("KICK", "jumped at", true, 6, 15, 0, 1, 1, 1, false, 3);
	kick->afterdesc = " with a kick";
	kick->addDescription("Launch a flying side kick at the target. (15 ATTACK)");
	self->addSpecialAttack(kick);
	Attack* headbutt = new Attack("HEADBUTT", "flew at", true,  5, 25, 0, 1, 1, 1, false, 5);
	headbutt->afterdesc = " like a missile";
	headbutt->recoil = 5;
	headbutt->addDescription("Deal a strong hit with your head, but it kind of hurts. (25 ATTACK)");
	self->addSpecialAttack(headbutt);
	Attack* bigenergyball = new Attack("BIG ENERGY BALL", "threw a big ball of energy at", false, 10, 20, 10, 1, 1, 1, false, 8);
	bigenergyball->addDescription("Throw a large mass of energy at the target and their surrounding allies. (20 ATTACK, 10 PIERCE)");
	Attack* punchflurry = new Attack("PUNCH FLURRY", "rushed", true, 7, 5, 0, 6, 7, 1, false, 10);
	punchflurry->addDescription("Unleash a barrage of 6 to 7 punches. (5 ATTACK, 6-7 hits)");
	self->addSpecialAttack(punchflurry);
	Attack* shurikenthrow = new Attack("SHURIKEN THROW", "threw a spread of shurikens at", false, 2, 7, 5, 0, 2, 3);
	shurikenthrow->addDescription("Throw a spread of shurikens at the target, with varying success since you're just chucking them. (7 ATTACK, 5 PIERCE, 0-2 hits)");
	Item* shurikens = new WeaponItem("SHURIKENS", "A bundle of ninja shurikens with a note attached:\n\"Congratulations on defeating our ninja scout. Take these shurikens and train in the ninja ways,\nand maybe one day you'll become a true ninja.\"", ninjaland, shurikenthrow);
	Attack* bonedrill = new Attack("BONE CONE", "launched a drill of bone at", false, 10, 8, 5, 5, 6, 1);
	bonedrill->addDescription("Spin the conic bone, drilling into the target. (8 ATTACK, 5 PIERCE, 5-6 hits)");
	Item* bonecone = new WeaponItem("BONE CONE", "A cone-shaped bone, looks kind of like a drill.", desertgrave, bonedrill);
	Attack* scarfsmack = new Attack("SCARF SMACK", "smacked", false, 15, 25, 0, 2, 2, 1); //no contact because the scarf stretches far from Bernard
	scarfsmack->afterdesc = "with his scarf";
	scarfsmack->addDescription("Smack the target with each end of your stretchy scarf. (25 ATTACK, 2 hits)");
	Item* bigscarf = new WeaponItem("BIG SCARF", "A scarf considerably bigger and stretchier than your current one, good for doing scarf attacks.", limbo, scarfsmack);
	Item* factchest = new TreasureItem("TREASURE CHEST", "A big treasure chest, possibly full of treasure.", factorytreasure, 50, {bigscarf});
	Attack* energize = new Attack("ENERGIZE", "energized", false, 14, 0, 0, 1, 1, 1, true, 15);
	Effect* energized = new Effect("ENERGIZED", 1, 0, 0, 2.0, 0, 0, 0);
	energize->addEffect(energized);
	energize->addDescription("Imbue yourself or an teammate with energy, doubling attack for one turn.");
	Attack* smokebomb = new Attack("SMOKE BOMB", "threw two smoke bombs, bringing SMOKEMEN into the battle", false, 13, 0, 0, 0, 0, 0);
	NPC* smokeman = new NPC("", "SMOKEMAN", "A really smoky humanoid who is just vibing.", limbo, 0, Stats(1, 0, 0, 0, 0, 0, 0));
	smokebomb->summon = smokeman;
	smokebomb->summonamount = 2;
	smokebomb->addDescription("Throw two smoke bombs containing smokemen, distracting the enemies.");
	Attack* pacupunctuken = new Attack("ACUPUNCTUKEN", "pinned", false, 12, 2, 15, 4, 4, 1);
	pacupunctuken->afterdesc = " with precise shurikens";
	Effect* pinned = new Effect("PINNED", 3);
	pinned->freeze = true;
	pacupunctuken->addEffect(pinned);
	pacupunctuken->addDescription("Throw precise shurikens at the target's pressure points, rendering them immobile for three turns. (2 ATTACK, 15 PIERCE, 4 hits)");
	Attack* pdeadlyspinferno = new Attack("DEADLY SPINFERNO", "flew at", true, 15, 10, 20, 5, 5, 3);
	pdeadlyspinferno->afterdesc = " in a flaming inferno";
	Effect* onfire = new Effect("ON FIRE", 3, 5, 0, 1, 0.8);
	pdeadlyspinferno->addEffect(onfire);
	pdeadlyspinferno->addDescription("Fly at the target and their surroundings in a deadly flaming tornado, also leaving them on fire. (10 ATTACK, 20 PIERCE, 5 hits)");
	Attack* precisionstrike = new Attack("PRECISION STRIKE", "threw a precise energy ellipsoid at", false, 12, 35, 15, 1, 1, 1, false, 12);
	precisionstrike->addDescription("Throw a heavy mass of energy speedily towards the target. (35 ATTACK, 15 PIERCE)");
	Attack* ballisticmissile = new Attack("BALLISTIC MISSILE", "threw a missile of energy at", false, 19, 50, 25, 1, 1, 1, false, 18);
	ballisticmissile->addDescription("Throw a dense missile of energy straight towards the target. (50 ATTACK, 25 PIERCE)");
	Attack* spbomb = new Attack("SP BOMB", "lobbed the SP BOMB at", false, 0, 0, 0, 1, 1, 9, false, 25);
	spbomb->addDescription("Gather up the collective SP of the entire team into a huge ball of energy and lob it at the enemy team. (SP ATTACK, 0 PIERCE)");
	spbomb->spbomb = true; //sp bomb do indeed be sp bomb
	Attack* spacetimeslice = new Attack("SPACETIME SLICE", "sliced", false, 25, 0, 0, 1, 1, 1, true);
	spacetimeslice->afterdesc = "'s spacetime into two";
	spacetimeslice->copyamount = 1;
	spacetimeslice->addDescription("Slice a teammate's spacetime into two, duplicating them for the duration of the battle.");
	Item* kosmickatana = new WeaponItem("KOSMIC KATANA", "A katana beautifully forged from the corn of a unihorn, sparkling like the stars of the cosmos.\nIt's so sharp it can slice through the spacetime continuum.", limbo, spacetimeslice);
	Attack* uppercut = new Attack("UPPERCUT", "uppercut", true, 7, 15, 0, 1, 1, 1);
	uppercut->afterdesc = " into the air";
	Effect* uppercutted = new Effect("UPPERCUTTED", 0);
	uppercutted->remove = true;
	uppercutted->falldamage = 10;
	uppercut->addEffect(uppercutted);
	uppercut->addDescription("Uppercut the target into the air, interrupting non-bosses' turn if it hits them before they move. (15 ATTACK)");
	Attack* pshrimplebeam = new Attack("SHRIMPLE BEAM", "fired a pressurized jet of water at", false, 25, 100, 100, 1, 1, 1);
	pshrimplebeam->instakill = true;
	pshrimplebeam->addDescription("Fire a jet of pressurized water at the target, instantly destoying non-boss enemies. (100 ATTACK, 100 PIERCE)");
	Item* shrimplegun = new WeaponItem("SHRIMPLE GUN", "An advanced red water gun granting non-shrimp wielders the ability to use the SHRIMPLE BEAM.", mountainlake, pshrimplebeam);
	Attack* parry = new Attack("PARRY", "is preparing to parry", false, 7, 0, 0, 1, 1, 1);
	parry->parry = true;
	Item* parryguide = new EducationItem("PARRYING GUIDE", "An in depth guide on how to PARRY enemy attacks.", limbo, parry);

	//Flower Girl Floria is primarily a healer with some other stuff for variety as well MARK: Floria
	NPC* floria = new NPC("FLOWER GIRL", "FLORIA", "Your little sister who gets along well with nature, especially flowers.\nShe has a flower-shaped hat.", flowerfield2, 5, Stats(10, 5, 4, 0, 5, 5, 9), Stats(1, 0, 1, 0, 1, 0, 0));
	npcChar[floria] = 'f'; //Floria's character representation is f for flower
	floria->addConversation({{floria, "Hey big brother! Aren't these flowers just so lovely? :>"},
							 {self, "NO THESE FLOWERS SUCK THEY TRIED TO EAT ME."},
							 {NULL, "FLORIA - :>"},
							 {NULL, "FLORIA - ¦>"},
							 {NULL, "FLORIA - :>"}});
	floria->setDialogue("I just love flowers!");
	floria->addGymDialogue({{floria, "I love running in circles around the gym!"}, {floria, "Exercise is so fun!"}});
	Conversation floriarecruit1 = {{self, "Hey I'm going on a BURGER QUEST wanna join?"}, {floria, "Yes! I hope we see some new flowers on the way!"}};
	floriarecruit1.skipcondition = {TEMPLEQUEST};
	shared_ptr<Conversation> floriarecruit2 = make_shared<Conversation>(Conversation({{self, "Hey I'm going on a QUEST to destroy BURGERs wanna join?"}, {floria, "Yes! I hope we see some new flowers on the way!"}}));
	floriarecruit1.alt = floriarecruit2;
	floriarecruit2->skipcondition = {BURGERMENDEF};
	shared_ptr<Conversation> floriarecruit3 = make_shared<Conversation>(Conversation({{self, "Hey wanna join my team?"}, {floria, "Yes! I hope we see some new flowers!"}}));
	floriarecruit2->alt = floriarecruit3;
	floria->addRecruitmentDialogue(floriarecruit1);
	floria->addRecruitedDialogue("I must see all the flowers!");
	floria->addDismissalDialogue("I'm going to go back to my flower field!");
	floria->setTalkOnRecruit(true);
	floria->setRecruitable(true);

	Attack* heal = new Attack("PHOTOSYNTHESIS", "sent a healing beam towards", false, -5, -5, 20, 1, 1, 1, true);
	floria->setBasicAttack(heal);
	Attack* rosethorn = new Attack("ROSE THORN", "called upon a rose friend to poke", false, 2, 20, 30, 1, 1, 1, false);
	rosethorn->addDescription("Call a rose friend to poke the target with its thorns. (20 ATTACK, 30 PIERCE)");
	floria->addSpecialAttack(rosethorn);
	Attack* turboheal = new Attack("TURBOSYNTHESIS", "sent a big healing beam towards", false, 4, -20, 20, 1, 1, 1, true, 6);
	turboheal->addDescription("Use flower power to greatly heal a teammate. (20 POWER)");
	floria->addSpecialAttack(turboheal);
	Attack* enroot = new Attack("ENROOT", "started drawing power from the soil", false, 5, 0, 0, 0, 0, 0, true, 8);
	enroot->addDescription("Draw power from the soil, building SP.");
	Effect* rooted = new Effect("ROOTED", 5, 0, -5);
	enroot->addEffect(rooted);
	floria->addSpecialAttack(enroot);
	Attack* recapacitate = new Attack("RECAPACITATE", "used flower power to recapacitate", false, 20, -20, 20, 1, 1, 1, true, 10);
	recapacitate->targetFainted = true;
	recapacitate->addDescription("Use flower power to recapacitate a teammate. (20 POWER)");
	floria->addSpecialAttack(recapacitate);
	Attack* aprilshower = new Attack("APRIL SHOWERS", "called an SP shower from the clouds", false, 25, 0, 0, 0, 0, 7, true, 8);
	aprilshower->addDescription("Call upon the clouds to rain SP upon the team.");
	Effect* spshower = new Effect("SP SHOWER", 6, 0, -6);
	aprilshower->addEffect(spshower);
	floria->addSpecialAttack(aprilshower);
	Attack* nitroheal = new Attack("NITROSYNTHESIS", "restored", false, 8, -999, 20, 1, 1, 1, true, 12);
	nitroheal->afterdesc = " to peak health";
	nitroheal->addDescription("Use flower power to heal a teammate to peak health.");
	floria->addSpecialAttack(nitroheal);
	Attack* hypercapacitate = new Attack("HYPERCAPACITATE", "used flower power to recapacitate", false, 25, -999, 20, 1, 1, 1, true, 15);
	hypercapacitate->targetFainted = true;
	hypercapacitate->addDescription("Use flower power to recapacitate a teammate to full health.");
	floria->addSpecialAttack(hypercapacitate);
	Attack* superpower = new Attack("SUPERPOWER", "unleashed the power of the earth upon", false, 20, 100, 100, 1, 1, 1, false, 17);
	superpower->instakill = true;
	superpower->addDescription("Unleash the power of the earth's core.");
	floria->addSpecialAttack(superpower);
	Attack* mayflower = new Attack("MAY FLOWERS", "brought about an HP bloom", false, 25, 0, 0, 0, 0, 7, true, 20);
	mayflower->addDescription("Bring about an HP bloom to heal the entire team.");
	Effect* hpbloom = new Effect("HP BLOOM", 6, -10);
	mayflower->addEffect(hpbloom);
	floria->addSpecialAttack(mayflower);

	//Science Gramps Egadwick is a support + some heavy damage MARK: Egadwick
	NPC* egadwick = new NPC("SCIENCE GRAMPS", "EGADWICK", "Your grandpa who lives in a secluded corner of the village.\nHe's always advancing science to the dismay of high school chemistry students.", tentlab, 5, Stats(15, 2, 3, 10, 10, 2, 9), Stats(0, 0, 1, 1, 1, 0, 0));
	npcChar[egadwick] = 'e'; //Egadwick's character representation is e for egads
	egadwick->setDialogue({{egadwick, "Ah hello kiddo. How's it going?"}, {self, "Pretty good."}, {egadwick, "Ah, that's good to hear."}});
	egadwick->addGymDialogue({{egadwick, "Eh, exercise isn't really my thing."}, {egadwick, "I gain experience by working out my mind!"}});
	Conversation egadwreject1 = {{self, "Yo Gramps wanna join my BURGER QUEST?"},
								{egadwick, "Sorry, kiddo, I'm afraid there's a dangerous robot on the loose in the forest."},
								{egadwick, "It was supposed to be for gardening but its definition of \"plant\" is a bit broad..."},
								{egadwick, "So now it's trying to cut my hair."},
								{self, "I can go destroy it if you want."},
								{egadwick, "Yes! That would be great, thanks!"}};
	egadwreject1.skipcondition = {TEMPLEQUEST};
	shared_ptr<Conversation> egadwreject2 = make_shared<Conversation>(Conversation({{self, "Yo Gramps wanna help me destroy BURGERs?"},
								{egadwick, "Sorry, kiddo, I'm afraid there's a dangerous robot on the loose in the forest."},
								{egadwick, "It was supposed to be for gardening but its definition of \"plant\" is a bit broad..."},
								{egadwick, "So now it's trying to cut my hair."},
								{self, "I can go destroy it if you want."},
								{egadwick, "Yes! That would be great, thanks!"}}));
	egadwreject1.alt = egadwreject2;
	egadwreject2->skipcondition = {BURGERMENDEF};
	shared_ptr<Conversation> egadwreject3 = make_shared<Conversation>(Conversation({{self, "Yo Gramps wanna join my team?"},
								{egadwick, "Sorry, kiddo, I'm afraid there's a dangerous robot on the loose in the forest."},
								{egadwick, "It was supposed to be for gardening but its definition of \"plant\" is a bit broad..."},
								{egadwick, "So now it's trying to cut my hair."},
								{self, "I can go destroy it if you want."},
								{egadwick, "Yes! That would be great, thanks!"}}));
	egadwreject2->alt = egadwreject3;
	egadwick->addRejectionDialogue(egadwreject1);
	egadwick->addRejectionDialogue("Sorry, kiddo, there's the whole gardening robot situation, remember?");
	egadwick->addRecruitmentDialogue({{egadwick, "Ah, I haven't been adventuring in decades."}, {egadwick, "Thanks for the invitation, kiddo!"}});
	egadwick->addRecruitedDialogue({{egadwick, "So, what science class are you in right now?"}, {self, "I don't go to school."}, {egadwick, "Ah."}});
	egadwick->addRecruitedDialogue("I love science!");
	egadwick->addDismissalDialogue({{egadwick, "Great hanging out with you, kiddo!"},  {egadwick, "I'll be in my tent if you need me!"}});
	
	//egadwick attacks
	Attack* scienceblaster = new Attack("SCIENCE BLASTER", "blasted", false, -5, 10, 15, 1, 1, 1);
	scienceblaster->afterdesc = " with his science blaster";
	egadwick->setBasicAttack(scienceblaster);
	Attack* vitamins = new Attack("VITAMIN SUPPLEMENT", "prescribed", false, 4, 0, 0, 0, 0, 0, true, 3);
	vitamins->afterdesc = " a vitamin supplement";
	vitamins->addDescription("Prescribe a teammate vitamins, boosting their HP and SP.");
	Effect* supplemented = new Effect("SUPPLEMENTED", 3, -10, -10);
	vitamins->addEffect(supplemented);
	egadwick->addSpecialAttack(vitamins);
	Attack* overclock = new Attack("OVERCLOCK", "overclocked", false, 8, 0, 0, 0, 0, 0, true, 5);
	overclock->addDescription("Overclock a teammate, boosting their attack and speed.");
	Effect* overclocked = new Effect("OVERCLOCKED", 3, 0, 0, 1.5, 1, 1, 1, 1.5);
	overclock->addEffect(overclocked);
	egadwick->addSpecialAttack(overclock);
	Attack* shieldacid = new Attack("SHIELD ACID", "threw shield-melting acid at", false, 4, 0, 0, 0, 0, 0, false, 8);
	shieldacid->addDescription("Throw a beaker of shield-melting acid at the target.");
	Effect* acidified = new Effect("ACIDIFIED", 3, 10, 0, 1, 0.5, 0.5);
	shieldacid->addEffect(acidified);
	egadwick->addSpecialAttack(shieldacid);
	Attack* emp = new Attack("EMP", "threw an EMP at", false, 16, 20, 20, 1, 1, 3, false, 10, 20);
	emp->addDescription("Throw an EMP, frying some of the target's and surrounding enemies' SP.");
	egadwick->addSpecialAttack(emp);
	Attack* hyperclock = new Attack("HYPERCLOCK", "hyperclocked", false, 13, 0, 0, 0, 0, 0, true, 12);
	hyperclock->addDescription("Over-overclock a teammate, greatly boosting their attack and speed.");
	Effect* hyperclocked = new Effect("HYPERCLOCKED", 3, 0, 0, 2.0, 1, 1, 1, 2.0);
	hyperclock->addEffect(hyperclocked);
	egadwick->addSpecialAttack(hyperclock);
	Attack* rocketscience = new Attack("ROCKET SCIENCE", "launched a volley of rockets", false, 20, 10, 20, 4, 8, 3, false, 15);
	rocketscience->addDescription("Launch a volley of rockets at the enemy team.");
	rocketscience->focushits = false;
	egadwick->addSpecialAttack(rocketscience);
	Attack* weatherforecast = new Attack("WEATHER FORECAST", "predicted an SP shower", false, 25, 0, 0, 0, 0, 7, true, 17);
	weatherforecast->addDescription("Predict a shower of SP that will rain upon the team.");
	weatherforecast->addEffect(spshower);
	egadwick->addSpecialAttack(weatherforecast);
	Attack* uberclock = new Attack("UBERCLOCK", "uberclocked", false, 16, 0, 0, 0, 0, 0, true, 20);
	uberclock->addDescription("Over-over-overclock a teammate, very greatly boosting their attack and speed.");
	Effect* uberclocked = new Effect("UBERCLOCKED", 3, 0, 0, 4.0, 1, 1, 1, 4.0);
	uberclock->addEffect(uberclocked);
	egadwick->addSpecialAttack(uberclock);
	Attack* orbitalstrike = new Attack("ORBITAL STRIKE", "called down an orbital beam towards", false, 30, 70, 100, 1, 1, 7, false, 25);
	orbitalstrike->addDescription("Call down an orbital laser from Edgadwick's brand new satellite.");
	egadwick->addSpecialAttack(orbitalstrike);
	
	//Forest Knight Absolom is primarily a tank with some knightly support as well MARK: Absolom
	NPC* forestknight = new NPC("FOREST KNIGHT", "ABSOLOM", "An old knight decked out in wooden armor, on a quest to vanquish all evil that crosses his path.", forestgrave, 30, Stats(30, 20, 25, 30, 10, 0, 10), Stats(1, 2, 1, 1, 0, 0, 0));
	npcChar[forestknight] = 'k'; //Absolom's character representation is k for knight
	forestknight->addRejectionDialogue({{self, "Hey knight man wanna join me on my BURGER QUEST?"},
										{forestknight, "A BURGER, you say?"},
										{forestknight, "I shan't assist you; this is an object of sin."},
										{forestknight, "I implore you to find a new, more noble goal for your quest."}});
	forestknight->addConversation({{forestknight, "For years, that fiend has kept me trapped here with the graves of my fallen compatriots."},
								   {forestknight, "No matter what I tried, he countered my every technique."},
								   {forestknight, "Nevertheless, I must thank you, child, for freeing me from that shrimp's grasp."},
								   {self, "Yeah no problem."}});
	Conversation absrecruit1 = {{self, "Hey knight man wanna help me save this kid?"},
							    {forestknight, "Why of course!"},
							    {forestknight, "From what will we be saving them?"},
							    {self, "From the BURGER coropration, they kidnapped them."},
							    {forestknight, "Of course, the fiendish stewards of BURGER."},
							    {forestknight, "I shall assist you in saving this child!"}};
	absrecruit1.skipcondition = {TEMPLEQUEST};
	shared_ptr<Conversation> absrecruit2 = make_shared<Conversation>(Conversation({{self, "Hey knight man wanna help me annihilate BURGERs from existence?"},
							    {forestknight, "Ah, what a splendid goal for a quest!"},
							    {forestknight, "But how do you propose to accomplish this?"},
							    {self, "With this thing I learned about called THE PLOT DEVICE."},
							    {forestknight, "THE PLOT DEVICE? I cannot say I have heard of it,"},
							    {forestknight, "but I believe you when you say it can solve the BURGER problem."},
							    {forestknight, "I shall join you on this quest!"}}));
	absrecruit2->skipcondition = {BURGERMENDEF};
	shared_ptr<Conversation> absrecruit3 = make_shared<Conversation>(Conversation({{self, "Hey knight man wanna join me?"},
							    {forestknight, "I do not see why not."}}));
	absrecruit1.alt = absrecruit2;
	absrecruit2->alt = absrecruit3;
	forestknight->addRecruitmentDialogue(absrecruit1);
	forestknight->addDismissalDialogue({{forestknight, "I shall return to defending the forest."},
										{forestknight, "Farewell, friend!"}});
	forestknight->addGymDialogue({{forestknight, "This is quite the splendid little gym!"}, {forestknight, "A good knight must always stay in shape!"}});
	forestknight->addBlock(elevator, TO_THE_TOP);
	forestknight->addBlock(elevatorbottom, TO_THE_TOP);
	forestknight->setBlockMessage({{forestknight, "The BURGER RESTAURANT?"}, {forestknight, "I shall not be seen there."}, {forestknight, "Let us continue on a better quest!"}});
	forestknight->setBlockUnless(TEMPLEQUEST);
	forestknight->setDialogue({{forestknight, "What a beautiful forest..."}, {forestknight, "I shall protect it 'till my last breath!"}});
	
	Attack* forestslash = new Attack("FOREST SLASH", "slashed", true, -5, 15, 10, 1, 1, 1);
	forestslash->afterdesc = " with his forest sword";
	forestknight->setBasicAttack(forestslash);
	Attack* defend = new Attack("PROTECT", "is protecting", false, 10, 0, 0, 1, 1, 1, true, 10);
	defend->protect = true; //defend start protecting
	forestknight->addSpecialAttack(defend);
	Attack* redwoodrend = new Attack("REDWOOD REND", "thrusted his sword at", true, 5, 25, 30, 1, 1, 3, false, 12);
	redwoodrend->afterdesc = " with the might of a redwood";
	forestknight->addSpecialAttack(redwoodrend);
	Attack* warcry = new Attack("WAR CRY", "rallied the team into action", false, 20, 0, 0, 0, 0, 7, true, 15);
	Effect* galvanized = new Effect("GALVANIZED", 5, 0, 0, 2.0, 2.0);
	warcry->addEffect(galvanized);
	forestknight->addSpecialAttack(warcry);
	Attack* enrootf = new Attack("ENROOT", "rooted into the soil", false, 10, 0, 0, 0, 0, 0, true, 18);
	Effect* rootedf = new Effect("ROOTED", 5, -10, 0, 0, 3.0, 3.0);
	enrootf->addEffect(rootedf);
	forestknight->addSpecialAttack(enrootf);
	Attack* sequoiasmash = new Attack("SEQUOIA SMASH", "crashed down his sword onto", true, 18, 50, 5, 1, 1, 1, false, 20);
	sequoiasmash->afterdesc = " with the weight of a sequoia";
	forestknight->addSpecialAttack(sequoiasmash);
	Attack* splinter = new Attack("SPLINTER", "swung splinters from his sword at", false, 12, 0, 0, 1, 2, 3, true, 25);
	Effect* splintered = new Effect("SPLINTERED", 5, 10);
	splinter->addEffect(splintered);
	forestknight->addSpecialAttack(splinter);
	Attack* blitz = new Attack("BLITZ", "rushed at", true, 25, 5, 15, 10, 10, 1, false, 31);
	blitz->afterdesc = " with a rapid flurry of sword strikes";
	orbitalstrike->addDescription("Rush at the target with a rapid flurry of strikes.");
	forestknight->addSpecialAttack(blitz);

	//Miner Maniac Mike is a good damage teammate with the risk of friendly fire MARK: Mike
	NPC* mike = new NPC("MINER MANIAC", "MIKE", "Maniacal miner with a reckless mania for blowing things up.\nA frequent customer of the subterranean dynamite store.", kaboomroom, 6, Stats(22, 5, 20, 0, 20, 12, 9), Stats(0, 0, 1, 0, 1, 0, 0));
	npcChar[mike] = 'm'; //Mike's character representation is m for Mike and miner and maniac to a lesser extent
	mike->addConversation({{NULL, "MIKE is throwing dynamite at the rocky wall."},
						   {self, "That doesn't look very safe."},
						   {mike, "HAHAHAHA Aren't the blasts just music to your ears?"},
						   {self, "no it's very loud :|"}});
	mike->setDialogue("HAHAHAHA I love the smell of explosions in the morning!");
	Conversation mikerec1 = {{self, "Hey I'm going on a BURGER QUEST wanna join?"}, {mike, "Why not, kid! Blowing up rocks is getting old."}};
	mikerec1.skipcondition = {TEMPLEQUEST};
	shared_ptr<Conversation> mikerec2 = make_shared<Conversation>(Conversation({{self, "Hey I'm going on a QUEST to destroy BURGERs wanna join?"}, {mike, "HAHAHAHA! Let's go blow up some BURGERs, kid!"}}));
	mikerec1.alt = mikerec2;
	mikerec2->skipcondition = {BURGERMENDEF};
	shared_ptr<Conversation> mikerec3 = make_shared<Conversation>(Conversation({{self, "Hey wanna join my team?"}, {mike, "Why not, kid! Blowing up rocks is getting old."}}));
	mikerec2->alt = mikerec3;
	mike->addRecruitmentDialogue(mikerec1);
	mike->addRecruitedDialogue("HAHAHAHAHA! So many things to explode!");
	mike->addDismissalDialogue("Alright Imma head back to my cave!");
	mike->setTalkOnRecruit(true);
	mike->setRecruitable(true);

	Attack* mdynamite = new Attack("DUAL DYNAMITE", "threw dual sticks of dynamite", false, -5, 20, 20, 2, 2, 1);
	mdynamite->focushits = false;
	Attack* drecoil = new Attack("LOOSE DYNAMITE", "accidentally bounced a stick of dynamite towards", false, 0, 10, 20, 1, 1, 1);
	mdynamite->recoilatt = drecoil;
	mdynamite->recoilchance = 1.0/3;
	mike->setBasicAttack(mdynamite);
	Attack* flashbang = new Attack("FLASHBANG", "threw a flashbang at", false, 8, 10, 20, 1, 1, 1);
	Effect* stunned = new Effect("STUNNED", 3);
	stunned->freeze = true;
	flashbang->addEffect(stunned);
	Attack* frecoil = new Attack("SIDE EFFECT", "accidentally stunned", false, 0, 10, 20, 1, 1, 1);
	frecoil->afterdesc = " as well";
	frecoil->addEffect(stunned);
	flashbang->recoilatt = frecoil;
	flashbang->recoilchance = 1.0/3;
	mike->addSpecialAttack(flashbang);
	Attack* bigbundle = new Attack("BIG BUNDLE", "threw a big bundle of dynamite at", false, 13, 40, 20, 1, 1, 3);
	Attack* brecoil = new Attack("LOOSE DYNAMITE", "didn't tie the bundle tightly enough, making some dynamite fall close to", false, 0, 20, 20, 1, 1, 1);
	brecoil->afterdesc = " in the process";
	bigbundle->recoilatt = brecoil;
	bigbundle->recoilchance = 1.0/3;
	mike->addSpecialAttack(bigbundle);
	Attack* bunkerbuster = new Attack("BUNKER BUSTER", "aimed a bunker buster at", false, 11, 30, 100, 1, 1, 1, false, 8);
	Attack* bbrecoil = new Attack("MISAIM", "aimed it too close to", false, 0, 30, 100, 1, 1, 1);
	bunkerbuster->recoilatt = bbrecoil;
	bunkerbuster->recoilchance = 1.0/3;
	mike->addSpecialAttack(bunkerbuster);
	Attack* dedefenser = new Attack("DEDEFENSER", "threw a heavy charge at", false, 15, 10, 20, 1, 1, 1, false, 12);
	Effect* dedefensed = new Effect("DEDEFENSED", 10, 0, 0, 1, 0.5, 0.25, 1, 0.5);
	dedefenser->addEffect(dedefensed);
	Attack* ddrecoil = new Attack("LOOSE DEDEFENSER", "accidentally dropped a dedefenser near", false, 0, 10, 20, 1, 1, 1);
	ddrecoil->addEffect(dedefensed);
	dedefenser->recoilatt = ddrecoil;
	dedefenser->recoilchance = 1.0/3;
	mike->addSpecialAttack(dedefenser);
	Attack* depthcharge = new Attack("DEPTH CHARGE", "threw a depth charge at", false, 20, 60, 35, 1, 1, 5, false, 17);
	Attack* dcrecoil = new Attack("MISINPUT", "accidentally included", false, 0, 30, 25, 1, 1, 1);
	dcrecoil->afterdesc = " in the charge's radius";
	depthcharge->recoilatt = dcrecoil;
	depthcharge->recoilchance = 2.0/3;
	mike->addSpecialAttack(depthcharge);
	Attack* minesweeper = new Attack("MINESWEEPER", "unleashed a mine-sweeping explosive upon the enemy team", false, 29, 10, 20, 12, 12, 1, false, 20);
	minesweeper->focushits = false;
	Attack* mrecoil = new Attack("MINESWEEPER RECOIL", "hit his team with the minesweeper as well", false, 0, 10, 20, 3, 3, 1);
	minesweeper->recoilatt = mrecoil;
	minesweeper->recoilchance = 1;
	mike->addSpecialAttack(minesweeper);

	//Cactus Cacty is a multi-hit damage dealer with some support/healing abilities MARK: Cacty
	NPC* cacty = new NPC("CACTUS", "CACTY", "Sharp cactus, brown from dehydration. He looks very sad, on the brink of death.", oasis, 12, Stats(25, 20, 23, 10, 15, 5, 9), Stats(1, 1, 1, 0, 1, 0, 0));
	npcChar[cacty] = 'c'; //Cacty's character representation is c for cactus
	cacty->setDialogue({{NULL, "CACTY - *raspy cactus plead for help*"}});
	cacty->addRejectionDialogue({{NULL, "CACTY - *raspy cactus plead for help*"}, {NULL, "CACTY is too dehydrated to join you."}});
	cacty->addRecruitmentDialogue({{self, "Hey cactus man wanna join me?"}, {NULL, "CACTY - *affirmative cactus noises*"}});
	cacty->addRecruitedDialogue({{NULL, "CACTY - *adventuring cactus noises*"}});
	cacty->addGymDialogue({{NULL, "CACTY - *exercising cactus noises*"}});

	Attack* loosespines = new Attack("CACTUS SPINES", " pricked", false, 0, 8, 15, 1, 3, 1);
	loosespines->afterdesc = " with his spines";
	cacty->setRecoilAttack(loosespines);
	Attack* cactbomb = new Attack("CACTUS BOMB", "threw a cactus bomb at", false, -5, 3, 15, 4, 6, 1);
	cacty->setBasicAttack(cactbomb);
	Attack* plantlifeplant = new Attack("LIFE PLANT", "planted a life plant", false, 7, 0, 0, 0, 0, 0);
	NPC* lifeplant = new NPC("", "LIFE PLANT", "Inanimate cactus which distracts the enemies and heals the team when destroyed.", limbo, 0, Stats(1, 0, 0, 0, 0, 0, 0));
	plantlifeplant->summon = lifeplant;
	plantlifeplant->summonamount = 1;
	cacty->addSpecialAttack(plantlifeplant);
	Attack* spinewave = new Attack("SPINE WAVE", "popped a wave of spines at", false, 7, 6, 15, 3, 3, 3);
	cacty->addSpecialAttack(spinewave);
	Attack* cactcarpet = new Attack("CACTUS CARPET", "threw a carpet of cactus at", false, 8, 15, 15, 1, 1, 3, false, 14, 0, 0.34);
	Effect* spinyfloor = new Effect("SPINY FLOOR", 4, 15);
	cactcarpet->addEffect(spinyfloor);
	cactcarpet->lifesteal = 0.34;
	cacty->addSpecialAttack(cactcarpet);
	Attack* acupuncture = new Attack("ACUPUNCTURE", "expertly healed", false, 7, -8, 15, 3, 3, 1, true);
	acupuncture->afterdesc = " with spines";
	cacty->addSpecialAttack(acupuncture);
	Attack* prespoints = new Attack("PRESSURE POINTS", "fired precise needles at", false, 8, 10, 20, 1, 1, 1);
	prespoints->afterdesc = "'s pressure points";
	prespoints->addEffect(pinned);
	cacty->addSpecialAttack(prespoints);
	Attack* dualcacti = new Attack("DUAL CACTI", "threw two cactus bombs at", false, 12, 4, 15, 3, 4, 3, false, 16);
	cacty->addSpecialAttack(dualcacti);
	Attack* superspine = new Attack("SUPER SPINE", "fired a huge spine at", false, 15, 40, 100, 1, 1, 1, false, 18);
	cacty->addSpecialAttack(superspine);

	//Master Chef Michelin is a healer/attacker hybrid MARK: Michelin
	NPC* michelin = new NPC("MASTER CHEF", "MICHELIN", "Professional chef on a quest to discover new recipes.", factorykitchen, 10, Stats(22, 5, 20, 0, 20, 12, 9), Stats(0, 0, 1, 0, 1, 0, 0));
	npcChar[michelin] = 'j'; //Michelin's character representation is j for Jim
	michelin->addConversation({{michelin, "Oh hi."},
							   {self, "hi"},
							   {michelin, "Hey can you help me with something?"},
							   {self, "Sure."},
							   {michelin, "My whole life I've made all the same recipes."},
							   {michelin, "Everything's grown bland to me."},
							   {michelin, "But recently I've gotten word of the SUPERSMOOTHIE,"},
							   {michelin, "formed from blending three special berries."},
							   {michelin, "Doesn't that sound new and exciting!?"},
							   {self, "It does sound pretty cool."},
							   {michelin, "Well anyway I've traveled here 'cause I've learned of three special berries that can be found in this region."},
							   {michelin, "The CACTIBERRY from the desert,"},
							   {michelin, "the RADIBERRY growing somewhere here in the factories,"},
							   {michelin, "and the NINJABERRY from some ninja village somewhere."},
							   {self, "Woah the ninja village?"},
							   {self, "You need ninja abilities to get there."},
							   {self, "I don't think I can help you actually sorry."},
							   {michelin, "Ah no worries I got this guide to being a ninja."},
							   {michelin, "I couldn't understand it myself, but you can try if you want."},
							   {self, "Yeah sure I'll take it thanks."},
							   {michelin, "Well when you get all three berries just USE blender over there."},
							   {self, "Sounds good."}});
	Item* ninjaguide = new KeyItem("GUIDE TO BEING A NINJA", "A book detailing the ninja techniques that will help you get into the ninja village.", {{NULL, "You open the guide."}, {self, "Chapter 1: The Ninja Way"}, {self, "Ninjahood was invented by..."}, {self, "..."}, {self, "I ain't reading allat."}, {self, "Ok... Table of Contents..."}, {self, "Aha!"}, {self, "Chapter 84: Ninja Movement"}, {NULL, "The next day..."}, {self, "Alright! Now I can do ninja mevement!"}, {NULL, "You have mastered the ninja abilities and can now enter the ninja village!"}}, limbo, NINJA);
	((KeyItem*)ninjaguide)->setTarget(ninjaland);
	michelin->setGift(ninjaguide);
	michelin->setDialogue({{michelin, "When you get all three berries just USE that blender over there."}, {michelin, "You got this!"}, {NULL, "MICHELIN - *two thumbs up* :D"}});
	michelin->addRejectionDialogue({ {michelin, "Can't sorry I gotta focus on the task at hand."}, {michelin, "Maybe once we're done with this."}});
	Conversation michrec1 = {{self, "Well..."}, {self, "Wanna join my BURGER QUEST?"}, {michelin, "sure why not. :("}};
	michrec1.skipcondition = {TEMPLEQUEST};
	shared_ptr<Conversation> michrec2 = make_shared<Conversation>(Conversation({{self, "Well..."}, {self, "Wanna help me destroy BURGERs?"}, {michelin, "sure why not. :("}}));
	michrec1.alt = michrec2;
	michrec2->skipcondition = {BURGERMENDEF};
	shared_ptr<Conversation> michrec3 = make_shared<Conversation>(Conversation({{self, "Well..."}, {self, "Wanna join my team?"}, {michelin, "sure why not. :("}}));
	michrec2->alt = michrec3;
	michelin->addRecruitmentDialogue(michrec1);
	michelin->addRecruitedDialogue("You know, maybe there's more to life than finding recipies.");
	michelin->addDismissalDialogue({{michelin, "Well, I guess I'll go back to my little kitchen area."}, {michelin, "Gotta start packing my bags to head home."}});
	michelin->setRecruitDialogueChange({{michelin, "Hey what's up"}, {michelin, "Just packing my bags."}, {michelin, "Might take a while cause I've set up a whole freaking kitchen here."}});
	michelin->addGymDialogue({{michelin, "I don't really work out that much,"}, {michelin, "but I know I should so I might as well start now."}});
	michelin->addGymDialogue({{michelin, "excercising"}, {michelin, "yeah"}});

	Item* cactiberry = new MaterialItem("CACTIBERRY", "Special berry that is pale green and prickly, adorned with a pink flower.", berryroom);
	Item* radiberry = new MaterialItem("RADIBERRY", "Special radioactive berry that glows neon green. Might wanna handle this one quickly.", factorygarden);
	Item* ninjaberry = new MaterialItem("NINJABERRY", "Special black berry pertaining to the ninjas, complete with a natural belt.", berryroom);

	Effect* multiposition = new Effect("MULTIPOSITION", 2147483647, 0, 0, 2.0, 0.5, 0.5, 2.0, 7.0);
	multiposition->multipositioning = 3;
	Item* SUPERSMOOTHIE = new EffectItem("SUPERSMOOTHIE", "Prismatic smoothie blended from three special berries. Not really meant for consumption, it might have some multipositional effects.", limbo, multiposition);

	Item* blender = new BlenderItem("BLENDER", "High quality high torque blender for blending stuff into smoothies.", factorykitchen, 
		{{michelin, "YOO you got the berries?"},
		 {self, "Yep."},
		 {michelin, "Alright! Let's see what the deal is with this SUPERSMOOTHIE!"},
		 {NULL, "You put the berries in the blender."},
		 {NULL, "The berries start blending."},
		 {NULL, "..."},
		 {NULL, "Ding!"},
		 {NULL, "You got the SUPERSMOOTHIE!"},
		 {NULL, "MICHELIN - *gasp*"},
		 {michelin, "IT'S BEAUTIFUL!"},
		 {NULL, "You gave the SUPERSMOOTHIE to MICHELIN."},
		 {NULL, "MICHELIN takes a sip of the SUPERSMOOTHIE."},
		 {michelin, "EW BRO THIS THING IS ATROCIOUS"},
		 {michelin, "AHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"},
		 {self, ":|"},
		 {michelin, "HIYNGIEINFGFUGFIBFIU"},
		 {michelin, "QWBDNIbjed"},
		 {michelin, "WHY WOULD ANYBODY WANT TO MAKE THIS"},
		 {michelin, "..."},
		 {michelin, "Well this was a waste of time..."},
		 {michelin, ":("},
		 {michelin, "Well you worked for this."},
		 {michelin, "You want it?"},
		 {self, "Sure."}},
		{"CACTIBERRY", "RADIBERRY", "NINJABERRY"}, SUPERSMOOTHIE);
	WorldChange& blenderchanges = ((BlenderItem*)blender)->getChanges();
	blenderchanges.recruitLinks.push(michelin);
	blenderchanges.linkedDialogue.push({michelin, {{michelin, "Man what a waste of time :("}}});

	Attack* castiron = new Attack("CAST IRON", "bonked", true, -5, 15, 0, 1, 1, 1);
	castiron->afterdesc = " with his cast iron pan";
	michelin->setBasicAttack(castiron);
	Attack* qualitymeal = new Attack("5 STAR MEAL", "prepared a 5-star meal for", false, 5, -25, 0, 1, 1, 1, true);
	michelin->addSpecialAttack(qualitymeal);
	Effect* marinated = new Effect("MARINATED", 5, 0, 0, 1, 0.5);
	Attack* flambe = new Attack("FLAMBE'", "bonked", true, 7, 20, 5, 1, 1, 1);
	flambe->afterdesc = " with a flaming pan";
	Effect* flambed = new Effect("FLAMBE'D", 5, 5, 0, 1, 0.8);
	flambe->addEffect(flambed);
	flambe->synergies.push_back(marinated);
	flambe->cancel = marinated;
	michelin->addSpecialAttack(flambe);
	Attack* hotsauce = new Attack("HOT SAUCE", "gave hot sauce to", false, 3, 0, 0, 1, 1, 1, true, 11);
	Effect* hotsauced = new Effect("HOT SAUCED", 5, 0, 0, 1.25, 0.75);
	hotsauce->addEffect(hotsauced);
	hotsauce->risky = true; //don't do this if the teammate has too low health because otherwise this would be dumb to do
	hotsauce->addDescription("Give a teammate hot sauce, boosting attack but lowering defense.");
	michelin->addSpecialAttack(hotsauce);
	Attack* feast = new Attack("FEAST", "prepared a feast for the team", false, 15, -30, 0, 1, 1, 7, true, 13);
	feast->focushits = false;
	feast->addDescription("Prepare a feast for the whole team, for much healing. (30 POWER)");
	michelin->addSpecialAttack(feast);
	Attack* marinate = new Attack("MARINATE", "doused", false, 10, 0, 0, 1, 1, 1, false, 12);
	marinate->afterdesc = " with alcohol";
	marinate->addEffect(marinated);
	michelin->addSpecialAttack(marinate);
	marinate->addDescription("Douse the target with alcohol, halving their defense and increasing damage taken from FLAMBE'.");
	Attack* michmeal = new Attack("MICHELIN STAR MEAL", "prepared a michelin-star meal for", false, 12, -55, 0, 1, 1, 1, true, 15);
	michmeal->addDescription("Prepare a teammate a super high-quality meal, for much healing. (55 POWER)");
	michelin->addSpecialAttack(michmeal);
	Attack* congratulation = new Attack("CONGRATULATION", "cooked", false, 40, 100, 100, 1, 1, 1, false, 20);
	congratulation->afterdesc = " congratulation";
	congratulation->instakill = true;
	congratulation->addDescription("Cook the target not just well done, but CONGRATULATION.");
	michelin->addSpecialAttack(congratulation);

	//Hackerman Carlos is a disruptor MARK: Carlos
	NPC* carlos = new NPC("HACKERMAN", "CARLOS", "Black hat hacker with no hat and no vitamin D. He has a space invaders hoodie.", factorytower, 12, Stats(25, 20, 23, 10, 15, 5, 9), Stats(1, 1, 1, 0, 1, 0, 0));
	npcChar[carlos] = 'x'; //Carlos' character representation is x for the common xX_epic_gamertag_Xx gamertag naming convention
	carlos->setDialogue({{carlos, "get out i cant focus with you here"}, {carlos, "im so close to hacking into microsofts mony supply"}});
	carlos->addRejectionDialogue({{carlos, "what"}, {carlos, "get outta my room im tryna focus"}});
	carlos->addRecruitmentDialogue({{self, "Hey wanna join my team?"}, {carlos, "no"}, {self, "Come on you gotta get some vitamin D."}, {carlos, "no i gotta start over stealing microsofts monies >:("}, {self, "If you want monies we get a lot just by fighting random enemies."}, {carlos, "..."}, {carlos, "fine ill join you"}});
	carlos->addRecruitedDialogue({{carlos, "woah what is this stuff"}, {self, "That's grass."}});
	carlos->addRecruitedDialogue("...");
	carlos->setRecruitDialogueChange("gaming");
	carlos->addDismissalDialogue({{carlos, "i dont feel like hacking microsoft all over again"}, {carlos, "guess ill just play fortnite or something"}});
	carlos->addGymDialogue({{carlos, "..."}, {carlos, "theres too much sunlight here"}});

	Item* carlosplug = new WorldChangeItem("PLUG", "A plug that is plugged into the wall as plugs are.", factorytower, {{NULL, "All the monitors in the room turn off."}, {carlos, "BRO WHAT THE HECK"}, {self, "Oh my bad."}, {self, "I didn't realize it would do that."}, {carlos, "I WAS LIKE SECONDS FROM STEALING ALL OF MICROSOFTS MONIES"}, {carlos, "THAT WAS LIKE A MONTH LONG PROCESS"}, {self, "You shouldn't steal monies."}, {carlos, "SHUT UP"}, {carlos, "GIMME BACK THAT PLUG"}, {self, "Alright."}, {NULL, "You returned the PLUG to CARLOS."}}, true);
	WorldChange& carlosplugchanges = ((WorldChangeItem*)carlosplug)->getChanges();
	carlosplugchanges.recruitLinks.push(carlos);
	carlosplugchanges.linkedDescriptions.push({carlos, "Black hat hacker with no hat and no vitamin D, whose attempt to hack Microsoft's monies you foiled."});
	carlosplugchanges.linkedDialogue.push({carlos, {{carlos, "broooooo i was so close... >:("}}});
	
	Effect* infected = new Effect("INFECTED", 5, 10, 10);
	Attack* backtrack = new Attack("BACKTRACK", " sent malware to the attacker's traced location", false, 0, 0, 0, 1, 1, 1);
	backtrack->addEffect(infected);
	carlos->setRecoilAttack(backtrack);
	Attack* hack = new Attack("HACK", "hacked into", false, -5, 0, 0, 1, 1, 1, false, 0, 5); //no damage but steals sp which I think is pretty interesting
	hack->afterdesc = "'s SP supply";
	carlos->setBasicAttack(hack);
	Attack* trojan = new Attack("TROJAN", "sent a trojan to", false, 7, 10, 20, 1, 1, 1);
	trojan->addEffect(infected);
	carlos->addSpecialAttack(trojan);
	Attack* stacksmash = new Attack("STACK SMASH", "stack smashed", false, 14, 25, 30, 1, 1, 1);
	Effect* overflow = new Effect("OVERFLOW", 4, 0, 0, 1, 0.5);
	stacksmash->addEffect(overflow);
	carlos->addSpecialAttack(stacksmash);
	Attack* socialengineering = new Attack("SOCIAL ENGINEERING", "socially engineered", false, 14, 0, 0, 1, 1, 1, false, 12);
	socialengineering->afterdesc = " into fighting for him";
	Effect* compromised = new Effect("COMPROMISED", 2);
	compromised->hypnotize = true;
	socialengineering->addEffect(compromised);
	carlos->addSpecialAttack(socialengineering);
	socialengineering->addDescription("Socially engineer an enemy into fighting for your team.");
	Attack* ddos = new Attack("DDOS", "directed his botnet's traffic towards", false, 12, 8, 40, 3, 3, 1, false, 14);
	Effect* overloaded = new Effect("OVERLOADED", 3);
	overloaded->freeze = true;
	ddos->addEffect(overloaded);
	carlos->addSpecialAttack(ddos);
	ddos->addDescription("Direct all of Carlos's botnet's traffic, freezing them in place. (8 ATTACK, 40 PIERCE, 3 hits)");
	Attack* forkbomb = new Attack("FORK BOMB", "sent a forking bomb towards the enemy team", false, 14, 25, 40, 1, 1, 7, false, 15);
	forkbomb->focushits = false;
	carlos->addSpecialAttack(forkbomb);
	forkbomb->addDescription("Send a forking bomb, covering the entire enemy team. (25 ATTACK, 40 PIERCE)");
	Attack* cryptojack = new Attack("CRYPTOJACK", "set up an SP and HP mine in", false, 15, 0, 0, 1, 1, 1, false, 17);
	Effect* cryptojacked = new Effect("CRYPTOJACKED", 5, 5, 5);
	cryptojacked->lifesteal = cryptojacked->spsteal = 1;
	cryptojack->addEffect(cryptojacked);
	carlos->addSpecialAttack(cryptojack);
	cryptojack->addDescription("Set up a bitcoin mine in the target, stealing their HP and SP for 5 turns.");
	Attack* cascadingfailure = new Attack("CASCADING FAILURE", "triggered a cascading failure in the enemy team", false, 20, 20, 40, 1, 1, 7, false, 20);
	cascadingfailure->focushits = false;
	cascadingfailure->synergies.push_back(infected);
	cascadingfailure->synergies.push_back(overflow);
	cascadingfailure->synergies.push_back(compromised);
	cascadingfailure->synergies.push_back(overloaded);
	cascadingfailure->synergies.push_back(cryptojacked);
	carlos->addSpecialAttack(cascadingfailure);
	cascadingfailure->addDescription("Trigger a cascading failure in the enemy team, with the attack's effectiveness multiplied for every one of Carlos's statuses present. (20 ATTACK, 40 PIERCE)");

	//Princess Plum is support MARK: Plum
	NPC* plum = new NPC("PRINCESS", "PLUM", "Sporty princess in purple attire kidnapped from a distant fungal kingdom.", limbo, 18, Stats(13, 10, 8, 1, 5, 12, 9), Stats(0, 0, 1, 0, 1, 1, 0));
	npcChar[plum] = 'p'; //Plum's character representation is p for Plum
	plum->addDismissalDialogue({{plum, "Well, I'll be heading back to my kingdom!"}, {plum, "Bye bye!"}});
	plum->addRecruitedDialogue({{plum, "Ahh, sure beats a diet of pure fungus!"}});
	plum->addRecruitmentDialogue({{self, "Hey before you head back to your kingdom, wanna join my team?"}, {plum, "It would be my pleasure!"}});
	plum->addGymDialogue({{plum, "This is a very nice gym!"}, {plum, "I love playing tennis!"}});
	//some of plum's dialogue logic is tied to Browser below in the enemies section

	Attack* racket = new Attack("RACKET", "whacked", true, -5, 10, 0, 1, 1, 1);
	racket->afterdesc = " with her tennis racket";
	plum->setBasicAttack(racket);
	Attack* bigmushroom = new Attack("BIG MUSHROOM", "gave a red embiggening mushroom to", false, 8, 0, 0, 0, 0, 0, true);
	Effect* supermushroom = new Effect("SUPER", 3, 0, 0, 2.0, 2.0);
	bigmushroom->addEffect(supermushroom);
	plum->addSpecialAttack(bigmushroom);
	Attack* turtleshell = new Attack("TURTLE SHELL", "kicked a green turtle shell towards", false, 6, 30, 0, 1, 1, 1);
	plum->addSpecialAttack(turtleshell);
	Attack* burnyblossom = new Attack("BURNY BLOSSOM", "gave a burny blossom to", false, 12, 0, 0, 0, 0, 0, true, 5);
	Effect* fireing = new Effect("FIRE", 4);
	fireing->attackeffect = onfire;
	burnyblossom->addEffect(fireing);
	plum->addSpecialAttack(burnyblossom);
	Attack* bobbybomb = new Attack("BOBBY BOMB", "threw a Bobby Bomb at", false, 10, 40, 20, 1, 1, 3, false, 7);
	plum->addSpecialAttack(bobbybomb);
	Attack* metalhat = new Attack("METAL HAT", "gave a metal hat to", false, 14, 0, 0, 0, 0, 0, true, 10);
	Effect* metallic = new Effect("METAL", 3, 0, 0, 5.0, 2.5);
	metalhat->addEffect(metallic);
	plum->addSpecialAttack(metalhat);
	Attack* lightning = new Attack("LIGHTNING", "used a lightning bolt to shock the enemy team", false, 22, 30, 30, 1, 1, 7, 14);
	lightning->focushits = false;
	Effect* mini = new Effect("MINI", 3, 0, 0, 0.5, 0.5);
	lightning->addEffect(mini);
	plum->addSpecialAttack(lightning);
	Attack* blueshell = new Attack("BLUE TURTLE SHELL", "threw a winged blue turtle shell, which went flying at", false, 20, 80, 40, 1, 1, 3, false, 16);
	plum->addSpecialAttack(blueshell);
	Attack* superstar = new Attack("SUPER STAR", "gave a super star to", false, 25, 0, 0, 0, 0, 0, true, 18);
	Effect* invincible = new Effect("INVINCIBLE", 3, 0, 0, 1, 2.0);
	invincible->invincible = true;
	superstar->addEffect(invincible);
	plum->addSpecialAttack(superstar);
	Attack* lifemushroom = new Attack("LIFE MUSHROOM", "gave a green life mushroom to", false, 30, 0, 0, 0, 0, 0, true, 20);
	lifemushroom->extralives = 1;
	plum->addSpecialAttack(lifemushroom);

	//Gambler Graham is the rng guy MARK: Graham
	NPC* graham = new NPC("GAMBLER", "GRAHAM", "A sorry gambling addict who is trillions in debt.\nHe'll pay it off as soon as he wins; any day now.", casino, 19, Stats(24, 12, 12, 12, 12, 12, 9), Stats(1, 1, 1, 0, 0, 0, 0));
	npcChar[graham] = 'g'; //Graham's character representation is g for Graham
	Conversation gramconvo = {{self, "You should stop gambling."}, {graham, "What?"}, {graham, "Haven't you heard that 99% of gamblers quit right before hitting it big?"}, {NULL, "GAMBLING MACHINE - \"You lose 1000000 monies.\""}, {graham, "Aw dang it."}};
	gramconvo.skipcondition = {NOGAMBLING};
	graham->addConversation(gramconvo);
	graham->setDialogue({{graham, "Come on, come on..."}, {NULL, "GAMBLING MACHINE - \"You lose 1000000 monies.\""}, {graham, "Aw dang it."}});
	graham->addRejectionDialogue({{self, "Hey wana join my team?"}, {graham, "Sure man, as soon as I win the jackpot."}, {graham, "I'm just about to win it. I can feel it!"}, {NULL, "GAMBLING MACHINE - \"You lose 1000000 monies.\""}, {graham, "Aw dang it."}});
	graham->addRecruitmentDialogue({{self, "Well now that you can't gamble anymore you wanna join my team?"}, {graham, "Eh..."}, {graham, "Sure, but I'm coming back once they fix this place."}});
	graham->addRecruitedDialogue("Let's not go gambling :(");
	graham->addDismissalDialogue({{graham, "I'm just gonna go check if they've fixed the casino yet."}});
	graham->setRecruitDialogueChange({{graham, "Come onnnnn....."}, {graham, "What's taking them so long to fix this place?"}});
	graham->addGymDialogue("This place is out of my comfort zone.");
	
	Item* grambutton4 = new WorldChangeItem("TURBO BUTTON", "A blue button with a skull on it.", limbo, {{NULL, "You press the TURBO BUTTON."}, {NULL, "Emergency sirens start blaring!"}, {NULL, "Thick protective shields envelop the energy core!"}, {NULL, "The energy core explodes within the casing!"}, {NULL, "The protective shields almost hit your face."}, {self, "Oh oops O_O"}});
	WorldChange& gramchanges4 = ((WorldChangeItem*)grambutton4)->getChanges();
	gramchanges4.recruitLinks.push(graham);
	gramchanges4.linkedDescriptions.push({graham, "Gambling addict whose access to gambling you destroyed, save for his icosahedral die."});
	gramchanges4.linkedConversations.push({graham, {{graham, "AGGGHHHHH I was just about to win the jackpot!"}, {graham, "The machine just short-circuited!"}, {self, "Yeah I think that was my bad."}, {self, "I broke the power in the back sorry."}, {graham, "..."}, {self, "Well you shouldn't gamble anyway."}, {graham, "......"}}});
	gramchanges4.linkedDialogue.push({graham, {{graham, "Aggghhhh I was JUST about to winnnn....."}}});
	gramchanges4.roomChanges.push({casinobase, "in the casino's basement, featuring cracks and now-spherical protective walls, courtesy of you."});
	gramchanges4.roomChanges.push({casino, "in the casino, full of complaining gamblers and deprived of its flashing lights."});
	gramchanges4.roomChanges.push({leftstreet4, "at the entrance of the casino, more dark than the average block due to the building's shadow."});

	Item* grambutton3 = new WorldChangeItem("TURBO BUTTON", "A blue button with a skull on it.", limbo, {{NULL, "You press the TURBO BUTTON."}, {NULL, "The basement starts shaking with power!"}});
	WorldChange& gramchanges3 = ((WorldChangeItem*)grambutton3)->getChanges();
	gramchanges3.linkedItems.push({grambutton4, casinobase});
	gramchanges3.roomChanges.push({casinobase, "in the casino's basement, tinted blue from the energy core.\nYou can feel its pulsations in your chest."});
	
	Item* grambutton2 = new WorldChangeItem("TURBO BUTTON", "A blue button with a skull on it.", limbo, {{NULL, "You press the TURBO BUTTON."}, {NULL, "The energy core starts throbbing with energy!"}});
	WorldChange& gramchanges2 = ((WorldChangeItem*)grambutton2)->getChanges();
	gramchanges2.linkedItems.push({grambutton3, casinobase});
	gramchanges2.roomChanges.push({casinobase, "in the casino's basement. The energy core is pulsating heavily, shaking the dust in the room."});
	
	Item* grambutton = new WorldChangeItem("TURBO BUTTON", "A blue button with a skull on it.", casinobase, {{self, "I wonder what this does."}, {NULL, "You press the TURBO BUTTON."}, {NULL, "The energy core surges with energy!"}});
	WorldChange& gramchanges = ((WorldChangeItem*)grambutton)->getChanges();
	gramchanges.linkedItems.push({grambutton2, casinobase});
	gramchanges.roomChanges.push({casinobase, "in the casino's basement. The energy core is glowing extra blue."});

	NPC* icosahedrongus = new NPC("", "ICOSAHEDRONGUS", "Floating yellow icosahedral construct from Graham's icosahedral die.", limbo, 0, Stats(20, 10, 20, 10, 20, 20, 9));
	Attack* laser = new Attack("LASER", "fired a laser at", false, -5, 12, 12, 1, 1, 1);
	laser->afterdesc = " from one of its vertices";
	Attack* bonk = new Attack("BONK", "flew into", true, 8, 24, 24, 1, 1, 1);
	bonk->afterdesc = "'s head";
	Effect* hypnotized = new Effect("HYPNOTIZED", 2);
	hypnotized->hypnotize = true;
	Attack* hypnotize = new Attack("HYPNOTIZE", "emitted hypnotizing waves at", false, 15, 0, 0, 1, 1, 1);
	hypnotize->addEffect(hypnotized);
	icosahedrongus->setBasicAttack(laser);
	icosahedrongus->addSpecialAttack(bonk);
	icosahedrongus->addSpecialAttack(hypnotize);
	
	Attack* nat1 = new Attack("DICE ROLL", "rolled a NAT 1!", false, 0, 30, 50, 1, 1, 3, true); //1 - Graham explodes also damages nearby teammates
	nat1->afterdesc = " blew up!";
	nat1->targetself = true;
	nat1->instakill = true;
	graham->addSpecialAttack(nat1);
	Attack* nat2 = new Attack("DICE ROLL", "rolled a 2!", false, 0, -999, 0, 1, 1, 1); //2 - enemy gets fully healed
	nat2->afterdesc = " was fully healed";
	graham->addSpecialAttack(nat2);
	Attack* nat3 = new Attack("DICE ROLL", "rolled a 3! A flash of light stuns the team!", false, 0, 0, 0, 1, 1, 7, true); //3 - freeze team
	nat3->focushits = false;
	Effect* blinded = new Effect("BLINDED", 1);
	blinded->freeze = true;
	nat3->addEffect(blinded);
	graham->addSpecialAttack(nat3);
	Attack* nat4 = new Attack("DICE ROLL", "rolled a 4! Lightning struck", false, 0, 23, 35, 1, 1, 3, true); //4 - lightning strikes the team
	graham->addSpecialAttack(nat4);
	Attack* nat5 = new Attack("DICE ROLL", "rolled a 5!", false, 0, 0, 0, 1, 1, 1); //5 - buff opponent
	nat5->afterdesc = " turned into gold";
	Effect* golden = new Effect("GOLDEN", 5, 0, 0, 2.75, 2.75);
	nat5->addEffect(golden);
	graham->addSpecialAttack(nat5);
	Attack* nat6 = new Attack("DICE ROLL", "rolled a 6! An anvil appeared over", false, 0, 0, 0, 1, 1, 1, true); //6 - put anvil over teammate, will hit in 2 turns
	Effect* anvilabove = new Effect("ANVIL ABOVE", 2);
	anvilabove->falldamage = 50;
	nat6->addEffect(anvilabove);
	graham->addSpecialAttack(nat6);
	Attack* nat7 = new Attack("DICE ROLL", "rolled a 7", false, 0, 0, 0, 0, 0, 0); //7 - give enemy team an icosahedrongus
	nat7->summon = icosahedrongus;
	nat7->enemysummon = true;
	nat7->summonamount = 1;
	graham->addSpecialAttack(nat7);
	Attack* nat8 = new Attack("DICE ROLL", "rolled an 8!", false, 0, 0, 0, 1, 1, 1, true); //8 - fling teammate into the air
	nat8->afterdesc = " was surged up into the sky";
	Effect* falling = new Effect("FALLING", 2);
	falling->remove = true;
	falling->falldamage = 35;
	nat8->addEffect(falling);
	graham->addSpecialAttack(nat8);
	Attack* nat9 = new Attack("DICE ROLL", "rolled a 9!", false, 0, 15, 0, 1, 1, 1, true); //9 - team takes small damage
	nat9->afterdesc = " took damage";
	graham->addSpecialAttack(nat9);
	Attack* nat10 = new Attack("DICE ROLL", "rolled a 10! Nothing happened", false, 0, 0, 0, 0, 0, 0); //10 - do nothing
	graham->addSpecialAttack(nat10);
	Attack* nat11 = new Attack("DICE ROLL", "rolled an 11!", false, 0, 15, 0, 1, 1, 1); //11 - enemy takes small damage
	nat11->afterdesc = " took damage";
	graham->addSpecialAttack(nat11);
	Attack* nat12 = new Attack("DICE ROLL", "rolled a 12!", false, 0, -17, 0, 1, 1, 1, true); //12 - team gets small heal
	nat12->afterdesc = " recovered HP";
	graham->addSpecialAttack(nat12);
	Attack* nat13 = new Attack("DICE ROLL", "rolled a 13! An anvil appeared over", false, 0, 0, 0, 1, 1, 1); //13 put anvil over opponent, will hit in 2 turns
	nat13->addEffect(anvilabove);
	graham->addSpecialAttack(nat13);
	Attack* nat14 = new Attack("DICE ROLL", "rolled a 14!", false, 0, 0, 0, 1, 1, 1); //14 - freeze enemy
	nat14->afterdesc = " was frozen in a cube of ice";
	Effect* frozen = new Effect("FROZEN", 3);
	frozen->freeze = true;
	nat14->addEffect(frozen);
	graham->addSpecialAttack(nat14);
	Attack* nat15 = new Attack("DICE ROLL", "rolled a 15! Icosahedral energy barriers form around the team!", false, 0, 0, 0, 1, 1, 7, true); //15 - summon guard for team
	nat15->focushits = false;
	nat15->targuard = 1;
	graham->addSpecialAttack(nat15);
	Attack* nat16 = new Attack("DICE ROLL", "rolled a 16!", false, 0, 0, 0, 0, 0, 0, true); //16 - summon icosahedrongus for team
	nat16->summon = icosahedrongus;
	nat16->summonamount = 1;
	graham->addSpecialAttack(nat16);
	Attack* nat17 = new Attack("DICE ROLL", "rolled a 17!", false, 0, -999, 0, 1, 1, 1, true); //17 - teammate gets fully healed
	nat17->afterdesc = " was fully healed";
	graham->addSpecialAttack(nat17);
	Attack* nat18 = new Attack("DICE ROLL", "rolled an 18! A big burst of energy appeared at", false, 0, 30, 50, 1, 1, 3); //18 - big bomb
	graham->addSpecialAttack(nat18);
	Attack* nat19 = new Attack("DICE ROLL", "rolled a 19!", false, 0, 0, 0, 0, 0, 0, true); //19 - make teammate invincible
	nat19->afterdesc = " is invincible";
	nat19->addEffect(invincible); //from plum's move, also buffs attack cause there's no teammate buff roll anyway
	graham->addSpecialAttack(nat19);
	Attack* nat20 = new Attack("DICE ROLL", "rolled a NAT 20!", false, 0, 100, 100, 1, 1, 1); //20 - target explodes
	nat20->afterdesc = " blew up!";
	nat20->instakill = true;
	graham->addSpecialAttack(nat20);

	//Rich Guy Richie is the summoner MARK: Richie
	NPC* richie = new NPC("RICH GUY", "RICHIE", "Rich guy trying to figure out what to do with his massive inheritence.", richneighborhood3, 20, Stats(10, 0, 15, 0, 0, 14, 9), Stats(1, 0, 1, 0, 0, 1, 0));
	npcChar[richie] = 'r'; //Richie's character representation is r for Richie
	Conversation richrej = {{richie, "No, everyone in this city is either rich and trying to get me to join some BURGER cult,"}, {richie, "or not rich and trying to rob me."}, {richie, "No offense but I don't really trust you."}};
	shared_ptr<Conversation> richrej2 = make_shared<Conversation>(Conversation({{self, "Hey you wanna help me fight these BURGER guys?"},
		{richie, "In that building over there?"},
		{self, "Yea."},
		{richie, "You know we have like these advanced security systems in this neighborhood right?"},
		{richie, "You're gonna get blown up as soon as you walk onto their doorstep."},
		{self, "Nah I got this cloaking device."},
		{richie, "..."},
		{richie, "You look pretty uncloaked to me."}}));
	richrej.skipcondition = {JILLYQUEST};
	richrej.alt = richrej2;
	richie->addRejectionDialogue(richrej);
	Conversation richrec = {{self, "Hey you wanna help me fight these BURGER guys?"},
		{richie, "In that building over there?"},
		{self, "Yea."},
		{richie, "You know we have like these advanced security systems in this neighborhood right?"},
		{richie, "You're gonna get blown up as soon as you walk onto their doorstep."},
		{self, "Nah I got this cloaking device."},
		{NULL, "You turn invisible and visible again."},
		{richie, "Huh."},
		{self, "So I'm going there to save this kid they kidnapped."},
		{richie, "Oh."},
		{richie, "Yeah I really want these BURGER guys gone."},
		{richie, "Like their whole cult thing is one thing."},
		{richie, "But you just said they kidnapped someone?"},
		{self, "Yeah."},
		{richie, "Yeah, lets go gettem."}};
	shared_ptr<Conversation> richrec2 = make_shared<Conversation>(Conversation({{richie, "Did you blow up the BURGER headquarters?"},
		{richie, "I saw you walk in there and the top floor just blew up."},
		{self, "Oh that was the CEO."},
		{self, "We fought him normally but he kind of just blew up on his own."},
		{richie, "Huh."},
		{self, "Hey you wanna help me find this kid they kidnapped?"},
		{richie, "They what?"},
		{richie, "Well I always wanted these BURGER guys gone."},
		{richie, "But you just said they kidnapped someone?"},
		{richie, "That's way too far."},
		{richie, "Yeah, lets go gettem."}}));
	shared_ptr<Conversation> richrec3 = make_shared<Conversation>(Conversation({{self, "Hey you wanna help me destroy BURGERs from existence?"},
		{richie, "..."},
		{richie, "What?"},
		{self, "Yeah, using this THE PLOT DEVICE."},
		{richie, "..."},
		{richie, "You were the one who blew up the CEO right?"},
		{richie, "Cause I saw you walk in their building and that happened."},
		{self, "Well we fought him normaly but he kind of just blew up on his own."},
		{richie, "You sound like you're making everything up..."},
		{richie, "But you also clearly know what you're doing if you beat Enzo..."},
		{richie, "Yeah why not, I don't have anything better to do."},
		{richie, "Plus I really want this BURGER cult gone."}}));
	shared_ptr<Conversation> richrec4 = make_shared<Conversation>(Conversation({{self, "Hey you wanna join my team?"},
		{richie, "You were the one who blew up the CEO right?"},
		{richie, "Cause I saw you walk in their building and that happened."},
		{self, "Well we fought him normaly but he kind of just blew up on his own."},
		{richie, "Well if you beat Enzo you seem pretty qualified..."},
		{richie, "Why not, I don't have anything better to do."}}));
	richrec.alt = richrec2;
	richrec.skipcondition = {BEATCEO};
	richrec2->alt = richrec3;
	richrec2->skipcondition = {TEMPLEQUEST};
	richrec3->alt = richrec4;
	richrec3->skipcondition = {BURGERMENDEF};
	richie->addRecruitmentDialogue(richrec);
	richie->addDismissalDialogue("Welp, Imma head back to my house.");
	Conversation richdial = {{richie, "Hey man don't come any closer."}, {richie, "Or you're gonna get blown up by my security systems."}};
	shared_ptr<Conversation> richdial2 = make_shared<Conversation>(Conversation({{richie, "I know you beat the CEO and everything,"}, {richie, "but still watch out for my security systems."}, {richie, "Or you're gonna get blown up."}}));
	richdial.alt = richdial2;
	richdial.skipcondition = {BEATCEO};
	richie->setDialogue(richdial);
	richie->setRecruitDialogueChange("Whattup.");
	richie->addRecruitedDialogue("Whattup.");
	richie->addGymDialogue({{richie, "I mean I have a gym at home but I guess this works."}, {richie, "Just offering."}});
	richie->addBlock(elevator, TO_THE_TOP);
	richie->addBlock(elevatorbottom, TO_THE_TOP);
	richie->setBlockMessage({{richie, "Umm why are we going there?"}, {richie, "We were like fighting BURGER and stuff."}});
	richie->setBlockUnless(TEMPLEQUEST);

	NPC* drone = new NPC("", "DRONE", "Combat drone that Richie bought to help in battle.", limbo, 0, Stats(16, 0, 18, 0, 20, 30, 9));
	Attack* dronegun = new Attack("DRONE GUN", "fired at", false, -5, 4, 10, 3, 3, 1);
	Attack* ram = new Attack("RAM", "rammed into", true, 8, 17, 0, 1, 1, 1);
	drone->setBasicAttack(dronegun);
	drone->addSpecialAttack(ram);

	NPC* healsprinkler = new NPC("", "HEAL SPRINKLER", "Portable station that sprinkles health onto the team, bought by Richie to aid in battle.", limbo, 0, Stats(30, 0, 10, 0, 0, 0, 9));
	Attack* sprinkle = new Attack("SPRINKLE", "sprinkled health on the team", false, 0, -7, 0, 1, 1, 7, true);
	sprinkle->focushits = false;
	healsprinkler->setBasicAttack(sprinkle);

	NPC* guardbot = new NPC("", "ROBOCOP", "Robotic policeman for protecting citizens, bought by Richie to assist in battle.\nTheir effectiveness didn't last long in BURGERSBURG, but are now sold by collectors rarely.", limbo, 0, Stats(40, 0, 16, 0, 8, 13, 9));
	Effect* swatshield = new Effect("SWAT SHIELD", 2147483647);
	swatshield->guardset = 1;
	guardbot->setEffect(swatshield, NULL);
	Attack* baton = new Attack("BATON", "thwacked", true, -5, 15, 0, 1, 1, 1);
	baton->afterdesc = " with its baton";
	Attack* pepperspray = new Attack("PEPPER SPRAY", "sprayed pepper spray at", false, 7, 10, 0, 1, 1, 1);
	Effect* peppersprayed = new Effect("PEPPER SPRAYED", 3, 3, 0, 0.5, 0.75);
	pepperspray->addEffect(peppersprayed);
	guardbot->addSpecialAttack(pepperspray);
	Attack* protectandserve = new Attack("PROTECT AND SERVE", "is protecting", false, 10, 0, 0, 1, 1, 1, true);
	protectandserve->protect = true;
	guardbot->addSpecialAttack(protectandserve);

	NPC* marinemech = new NPC("", "MARINE MECH", "Advanced and expensive military droid bought by Richie as a great help in battle.", limbo, 0, Stats(50, 0, 30, 0, 35, 30, 9));
	Attack* openfire = new Attack("OPEN FIRE", "opened fire upon", false, -5, 4, 20, 5, 5, 1);
	Attack* rockemsockem = new Attack("ROCKEM SOCKEM", "rocked and socked", true, 5, 15, 0, 2, 2, 1);
	Attack* pgmissile = new Attack("PRECISION GUIDED MISSILE", "fired a precision guided missile at", false, 10, 20, 25, 1, 1, 3);
	marinemech->setBasicAttack(openfire);
	marinemech->addSpecialAttack(rockemsockem);
	marinemech->addSpecialAttack(pgmissile);
	
	Attack* throwmoney = new Attack("THROW MONEY AT THE PROBLEM", "threw a heavy gold bar at", false, -5, 20, 0, 1, 1, 1);
	richie->setBasicAttack(throwmoney);
	Attack* buydrone = new Attack("AMAZON PRIME", "bought a DRONE on Amazon", false, 5, 0, 0, 0, 0, 0);
	buydrone->summon = drone;
	buydrone->summonamount = 1;
	richie->addSpecialAttack(buydrone);
	Attack* buyheal = new Attack("AMAZON PRIME", "bought a HEAL SPRINKLER on Amazon", false, 10, 0, 0, 0, 0, 0);
	buyheal->summon = healsprinkler;
	buyheal->summonamount = 1;
	richie->addSpecialAttack(buyheal);
	Attack* buybot = new Attack("AMAZON PRIME", "bought a ROBOCOP on Amazon", false, 15, 0, 0, 0, 0, 0);
	buybot->summon = guardbot;
	buybot->summonamount = 1;
	richie->addSpecialAttack(nat7);
	Attack* buymech = new Attack("AMAZON PRIME", "bought a MARINE MECH on Amazon", false, 20, 0, 0, 0, 0, 0);
	buymech->summon = marinemech;
	buymech->summonamount = 1;
	richie->addSpecialAttack(buymech);

	//Bodyguard Buford is a damage dealer tied to Richie MARK: Buford
	NPC* buford = new NPC("BODYGUARD", "BUFORD", "Richie's bodyguard, trained in every martial art.", richneighborhood3, 30, Stats(40, 50, 20, 50, 0, 25, 9), Stats());
	richie->setGuardian(buford);
	buford->setGuarding(richie);
	buford->setDialogue("...");
	buford->addGymDialogue("...");
	buford->addRejectionDialogue({{buford, "..."}, {NULL, "BUFORD is already recruited by RICHIE!"}});
	Attack* counter = new Attack("COUNTER", "counterattacked", false, -5, 10, 0, 1, 1, 1); //this very much is a contact move but it has to be marked as non-contact lest there be an infinite recoil loop
	buford->setRecoilAttack(counter);
	Attack* martialart = new Attack("MARTIAL ART", "combo'd", true, -5, 3, 0, 6, 7, 1);
	martialart->afterdesc = " with one of his many martial arts";
	buford->setBasicAttack(martialart);
	Attack* scissorkick = new Attack("SCISSOR KICK", "threw out a scissor kick", true, 5, 15, 0, 2, 2, 1);
	scissorkick->focushits = false;
	buford->addSpecialAttack(scissorkick);
	Attack* sweeptheleg = new Attack("SWEEP THE LEG", "sweeped", true, 8, 20, 0, 1, 1, 1);
	Effect* sweeped = new Effect("SWEEPED", 0);
	sweeped->tiring = true;
	sweeptheleg->addEffect(sweeped);
	buford->addSpecialAttack(sweeptheleg);
	Attack* suplex = new Attack("SUPLEX", "suplexed", true, 8, 0, 0, 1, 1, 1);
	suplex->afterdesc = " into the air";
	Effect* suplexed = new Effect("SUPLEXED", 0);
	suplexed->remove = true;
	suplex->addEffect(suplexed);
	suplexed->falldamage = 15;
	buford->addSpecialAttack(suplex);
	Attack* discombobulate = new Attack("DISCOMBOBULATE", "thunderclapped", true, 15, 20, 0, 1, 1, 1);
	Effect* discombobulated = new Effect("DISCOMBOBULATED", 2);
	discombobulated->freeze = true;
	discombobulate->addEffect(discombobulated);
	buford->addSpecialAttack(discombobulate);
	Attack* fiveforty = new Attack("540", "kicked", true, 18, 35, 0, 1, 1, 1);
	fiveforty->afterdesc = " in the head with a 540 jump round house";
	buford->addSpecialAttack(fiveforty);

	//BURGER QUEST 1 Protagonist Henry Jerry is not that good at fighting but he's trying his best MARK: Henry Jerry
	NPC* hj = new NPC("BURGER QUEST 1 PROTAGONIST", "HENRY JERRY", "The protagonist of BURGER QUEST 1 who was used as a puppet of BURGER.\nHe wears a formal business suit and a traumatized expression.", limbo, 1, Stats(10, 2, 4, 1, 0, 4, 5));
	npcChar[hj] = 'h'; //Henry Jerry's character representation is h for Henry Jerry
	//something about bright light and lighning or something
	//{NULL, "The BURGER MAN's BURGERy shell fades to ashes."},
	//{NULL, "You can make out a figure left behind in the cloud..."},
	//{NULL, "..."},
	//{self, "WHAT!?!?"},
	//{self, "The BURGER QUEST 1 protagonist, Henry Jerry?!?!"},
	//{hj, "Yeah that's my name."},
	//{NULL, "You feel a heavy rumbling in the restaurant..."},
	//{NULL, "*rumbling*"},
	//{NULL, "BURGER TENDRILS shoot up around the BURGER RESTAURANT from deep below!"},
	//{NULL, "The BURGER TENDRILS wrap around the restaurant until you can see them out the window."},
	//{NULL, "The BURGER RESTAURANT gets jerked downwards!"},
	//{NULL, "You all slam against the roof."},
	//{NULL, "..."},
	//{NULL, "..."},
	//{NULL, "..."},
	//{NULL, "You fall below the cloud level."},
	//{NULL, "..."},
	//{NULL, "..."},
	//{NULL, "..."},
	//{NULL, "You see the city skyline and the ground pass by outside."},
	//{NULL, "..."},
	//{NULL, "..."},
	//{NULL, "..."},
	//{NULL, "The rock outside starts glowing redder and redder."},
	//{NULL, "..."},
	//{NULL, "..."},
	//{NULL, "..."},
	//{NULL, "Suddenly, the outside goes pitch black!"},
	//{NULL, "..."},
	//{NULL, "..."},
	//{NULL, "..."},
	//{NULL, "..."},
	//{NULL, "..."},
	//{NULL, "..."},
	//{NULL, "The BURGER RESTAURANT crashes onto solid ground!"},
	//{NULL, "You all faceplant violently."},
	//{NULL, "You have arrived at your destination."}
	
	//{hj, "Shoot, bro."},
	//{hj, "The news guy looks so much older than I remember."},
	//{hj, "And my house is so decayed..."},
	//{hj, "How many years did I miss?"},
	//{self, "Probably like 67."},
	//{hj, "Hahahahaha 67"},
	//{self, Hahahhahaha},
	//{hj, Hahahhahaha},
	//{self, Hahahhahaha},
	//{self, 67},
	//{hj, Hahahhahaha},

	//{hj, "You know, there's an ANTI-SKELETON DEVICE lying around in my old office."},
	//{hj, "Shame the portal's busted because we could probably actually pick it up now,"},
	//{hj, "with this game's brand new multi-word parsing."}

	//{hj, "Back in my day,"},
	//{hj, "we didn't have any of these fancy commands,"},
	//{hj, "like FIGHT"},
	//{hj, "or RECRUIT"},
	//{hj, "or ASK"},
	//{hj, "or SAVE..."},
	//{self, "That sounds like simpler times."}

	//I need to get a hobby.
	//And a new job.
	//Because everyone is skeletons now at my old job.

	//recruited: {hj, "Wow so this is what the world looks like."}
	//in abyss: {hj, "This place reminds me of the negative first universe."},
	//			{hj, "But like it has actual ground."}

	//{hj, "Uhhhhhhhhhhhhhhhhhhh..."},
	//{hj, "I have a headache..."}

	//{hj, "Man, I did a lot of bad things as the BURGER MAN..."},
	//{hj, "Can I go with you?"},
	//{hj, "I want to do something against BURGER..."},
	//{self, "Yeah I was about to ask you."},
	//{hj, "Well that works out pretty well then."}

	//{self, "Hey wanna join my team?"},
	//{hj, "?"},
	//{hj, "Isn't the game already over?"},
	//{self, "Yeah but you can still walk around"},
	//{self, "and 100% the game and stuff."},
	//{hj, "Sure why not"}

	//{hj, "I'm just gonna head back to the restaurant I guess."},
	//{hj, "It has a good view, that's one good thing about it."}

	//briefbase
	
	//MARK: other npcs
	NPC* archie = new NPC("VILLAGE ELDER", "ARCHIE", "The elder of Tactical Tent Village.\nHe stands there all day and night like a statue.", village, 50);
	archie->setDialogue("Safe travels!");
	Conversation archcondefault = {{archie, "So you are going on a BURGER QUEST, I hear?"},
							 	   {archie, "Just keep heading NORTH, and you'll soon reach BURGERSBURG."},
								   {archie, "Safe travels!"},
								   {archie, "Make sure to bring me back a BURGER!"}};
	archcondefault.skipcondition = {TEMPLEQUEST};
	archie->addConversation(archcondefault);

	Conversation archrej1 = {{self, "Hey wanna join me on my BURGER QUEST?"},
							 {archie, "I am sorry. Though I would love to join you,"},
							 {archie, "I must stay here and watch over the village."},
							 {archie, "Make sure to bring back a BURGER for me!"}};
	archrej1.skipcondition = {TEMPLEQUEST};
	shared_ptr<Conversation> archrej2 = make_shared<Conversation>(Conversation({{self, "Hey wanna help me destroy BURGERs?"},
							{archie, "Destroy BURGERs?"},
							{archie, "Well, I must watch over the village, sorry."}}));
	archrej1.alt = archrej2;
	archrej2->skipcondition = {BURGERMENDEF};
	shared_ptr<Conversation> archrej3 = make_shared<Conversation>(Conversation({{self, "Hey wanna join my team?"},
							{archie, "I appreciate the offer, but I must watch over the village."}}));
	archrej2->alt = archrej3;
	archie->addRejectionDialogue(archrej1);
	
	NPC* fisho = new NPC("FISHERMAN", "FRED", "Your dad, a fisherman who frequently fishes at the village docks.", docks, 25);
	fisho->setDialogue("The fish do be fishing today.");
	Conversation fishrej1 = {{self, "Hey wanna join me on my BURGER QUEST?"},
							 {fisho, "I've already gone on mine."},
							 {fisho, "It might be difficult but I know you're capable of doing this."}};
	fishrej1.skipcondition = {TEMPLEQUEST};
	shared_ptr<Conversation> fishrej2 = make_shared<Conversation>(Conversation({{self, "Hey wanna help me destroy BURGERs?"},
							{fisho, "Uh, I'm busy fishing sorry."}}));
	fishrej1.alt = fishrej2;
	fishrej2->skipcondition = {BURGERMENDEF};
	shared_ptr<Conversation> fishrej3 = make_shared<Conversation>(Conversation({{self, "Hey wanna join my team?"},
							{fisho, "Sorry, kid, I gotta be fishing right now."}}));
	fishrej2->alt = fishrej3;
	fisho->addRejectionDialogue(fishrej1);
	Conversation fishdef = {{fisho, "Hey I heard you're finally on a BURGER QUEST?"},
							{fisho, "The trek has gotten more difficult recently,"},
							{fisho, "but I know you're capable."},
							{fisho, "Here, take this fish I just caught."},
							{fisho, "It's not much but I'm sure it'll be useful to you."},
							{self, "Thanks dad."},
							{fisho, "Of course! Well, have a safe trip!"}};
	//MARK: alternates for DESTROY BURGER QUEST and postgame
	fishdef.skipcondition = {TEMPLEQUEST};
	fisho->addConversation(fishdef);

	Item* fish = new HpItem("HEALTHY FISH", "A fish given by your dad to support your BURGER QUEST. (heals 15 HP)", limbo, 15);
	fisho->setGift(fish);

	NPC* mango = new NPC("", "MANGOLIA", "Your mom, not really a fan of fish.", tenthouse, 10);
	mango->setDialogue("Oh hi sweetie! I hope everything is going swell!");
	Conversation mangorej1 = {{self, "Hey wanna join me on my BURGER QUEST?"},
							  {mango, "Sorry sweetie I can't be going on quests anymore."},
							  {mango, "But I hope yours goes well!"}};
	mangorej1.skipcondition = {TEMPLEQUEST};
	shared_ptr<Conversation> mangorej2 = make_shared<Conversation>(Conversation({{self, "Hey wanna help me destroy BURGERs?"},
							  {mango, "Now I don't wanna hear any of this nonsense."},
							  {mango, "Besides, how would you even do that?"}}));
	mangorej1.alt = mangorej2;
	mangorej2->skipcondition = {BURGERMENDEF};
	shared_ptr<Conversation> mangorej3 = make_shared<Conversation>(Conversation({{self, "Hey wanna join my team?"},
							  {mango, "Sorry sweetie, I don't have time to play right now."}}));
	mangorej2->alt = mangorej3;
	mango->addRejectionDialogue(mangorej1);
	Conversation mangodef = {{mango, "Ah! I've heard you're on your BURGER QUEST."},
							 {mango, "I'm just so proud!"},
							 {mango, "Here, take this cake I made just for this occasion!"},
							 {self, "Thanks mom."},
							 {mango, "Of course sweetie! Have a safe trip!"}};
	//MARK: alternates for DESTROY BURGER QUEST and postgame
	mango->addConversation(mangodef);

	Item* cake = new HpItem("HEALTHY CAKE", "A cake your mom made to commemorate your BURGER QUEST. (heals 100 HP)", limbo, 100);
	mango->setGift(cake);
	
	NPC* florian; //declare florian here so the callers can reference him here so that once world change can set the backup as backupcaller5
	Item* lobstercaller = new CallerItem("LOBSTER WHISTLE", "Used for summoning lobsters by playing a lobstery melody.", {{NULL, "You played a lobstery melody with the LOBSTER WHISTLE"}}, limbo, florian);
	desertstation->setBackup(lobstercaller);
	Item* backupcaller1 = new CallerItem("LOBSTER WHISTLE", "Used for summoning lobsters by playing a lobstery melody.", {{NULL, "You played a lobstery melody with the LOBSTER WHISTLE"}}, limbo, florian);
	volcanostation->setBackup(backupcaller1);
	Item* backupcaller2 = new CallerItem("LOBSTER WHISTLE", "Used for summoning lobsters by playing a lobstery melody.", {{NULL, "You played a lobstery melody with the LOBSTER WHISTLE"}}, limbo, florian);
	burgstation->setBackup(backupcaller2);
	Item* backupcaller3 = new CallerItem("LOBSTER WHISTLE", "Used for summoning lobsters by playing a lobstery melody.", {{NULL, "You played a lobstery melody with the LOBSTER WHISTLE"}}, limbo, florian);
	burgplats->setBackup(backupcaller3); //this one is for the escape sequence if they never got a whistle
	Item* backupcaller4 = new CallerItem("LOBSTER WHISTLE", "Used for summoning lobsters by playing a lobstery melody.", {{NULL, "You played a lobstery melody with the LOBSTER WHISTLE"}}, limbo, florian);
	basestation->setBackup(backupcaller4, false); //this one is endless because it's the only way to leave the basement after beating the game
	Item* backupcaller5 = new CallerItem("LOBSTER WHISTLE", "Used for summoning lobsters by playing a lobstery melody.\nYou have this because I have to account for players such as yourself,\nwho've ignored all the train stations, and even the whistle before the prison.", {{NULL, "You played a lobstery melody with the LOBSTER WHISTLE"}}, limbo, florian);

	NPC* burgcustomer = new NPC("BURGER CUSTOMER", "CHARLY", "A frequent customer of the BURGER RESTAURANT, wearing BURGER merchandise.", elevatorentrance, 5);
	burgcustomer->addConversation({{burgcustomer, "Man I just love BURGERs!"},
								   {burgcustomer, "I'm so glad to live in the same city as it."},
								   {burgcustomer, "You should see my son, though."},
								   {burgcustomer, "He's an even bigger fan than me!"}});
	burgcustomer->setDialogue("BURGERs are so cool!");
	burgcustomer->addRejectionDialogue({{burgcustomer, "I mean I like BURGERs,"},
										{burgcustomer, "but I just got one."},
										{burgcustomer, "I just kind of want to go home."}});

	Item* cloakingdevice = new WorldChangeItem("CLOAKING DEVICE", "Specialized cloaking device for getting past advanced security systems.", limbo, {{NULL, "You equipped the CLOAKING DEVICE."}, {NULL, "No security system can spot you now!"}});
	cloakingdevice->setTakable(true);
	WorldChange& cloakingchange = ((WorldChangeItem*)cloakingdevice)->getChanges();
	cloakingchange.exitUnblocks.push({richneighborhood1, NORTHEAST});
	cloakingchange.exitUnblocks.push({richneighborhood2, NORTH});
	cloakingchange.exitUnblocks.push({richneighborhood3, NORTHWEST});
	cloakingchange.recruitLinks.push(richie);
	cloakingchange.worldcon = CLOAKED;
	
	NPC* child = new NPC("", "JILLY", "The daughter of Matilda who was kidnapped by the BURGER corporation, even younger than your sister.", limbo, 3, Stats(10, 2, 8, 3, 0, 7, 9));
	npcChar[child] = 'l'; //Jilly's character representation is l for the two l's in her name
	Attack* dillydally = new Attack("DILLYDALLY", "is watching you battle", false, -5, 0, 0, 0, 0, 0);
	Attack* flyingkick = new Attack("FLYING KICK", "flew at", true, 6, 20, 0, 1, 1, 1);
	flyingkick->afterdesc = " with a kick";
	Attack* juppercut = new Attack("UPPERCUT", "uppercut", true, 15, 25, 0, 1, 1, 1);
	Effect* juppercutted = new Effect("UPPERCUTTED", 2);
	juppercutted->remove = true;
	juppercutted->falldamage = 20;
	juppercut->afterdesc = " into the sky";
	flyingkick->addEffect(juppercutted);
	child->setBasicAttack(dillydally);
	child->addSpecialAttack(flyingkick);
	child->addSpecialAttack(juppercut);
	child->setDialogue({{child, "Are we going back home, mister?"}, {self, "Yeah just a second."}});
	child->addRecruitmentDialogue({{self, "Alright I'm taking you back to your mom now."}, {child, "YAY!"}, {child, "Thank you mister!"}});
	child->addDismissalDialogue({{self, "Actually can you just stay here a little longer?"}, {child, "Okay."}, {NULL, "JILLY went to a hiding spot in the storage room."}});
	Conversation jdr = {{child, "What?"}, {child, "But you said you'd take me back to my mom!"}, {child, "Come on! It's not too far away!"}};
	shared_ptr<Conversation> jdr2 = make_shared<Conversation>(Conversation({{self, "Actually can you just stay here a little longer?"}, {child, "Okay."}, {NULL, "JILLY went to a hiding spot in the storage room."}, {NULL, "..."}, {NULL, "JILLY came back!"}, {child, "O_O"}}));
	shared_ptr<Conversation> jdr3 = make_shared<Conversation>(Conversation({{child, "No way!"}, {child, "I don't wanna go with that monster!"}}));
	shared_ptr<Conversation> jdr4 = make_shared<Conversation>(Conversation({{child, "No way!"}, {child, "I am not going back with that monster!"}, {child, "Come on! My mom's not too far away!"}}));
	jdr.skipcondition = {TEMPLEQUEST};
	jdr2->skipcondition = {BURGERCHASE};
	jdr3->skipcondition = {ESCAPEDBASE};
	jdr.alt = jdr2;
	jdr2->alt = jdr3;
	jdr3->alt = jdr4;
	child->addDismissalRejection(jdr);
	child->addRecruitedDialogue("I can't wait to get back home!");
	child->setBlockMessage({{child, "Uhhh I don't want to go there."}, {child, "I thought we were going back to my mom? :("}});
	child->addBlock(BURGERSBURG, SOUTH); //can't leave the city with Jilly
	child->addBlock(tunnels, TO_THE_VILLAGE);
	child->addBlock(tunnels, TO_THE_DESERT);
	child->addBlock(tunnels, TO_THE_HIGHLANDS);
	child->addBlock(tunnels, TO_THE_BASEMENT);
	child->addBlock(richneighborhood4, NORTH);
	child->addBlock(coolstreet2, INSIDE); //can't go in weird places with Jilly
	child->addBlock(rightstreet5, INSIDE);
	child->addBlock(newstreet2, INSIDE);
	child->addBlock(leftstreet4, INSIDE);
	child->addBlock(mainstreet5, INSIDE);
	child->addBlock(mainstreet4, DOWN);
	child->addBlock(coolstreet4, IN_ALLEY);
	child->addBlock(newstreet3, IN_ALLEY);
	child->addBlock(elevator, TO_THE_TOP); //can't bring Jilly to the restaurant
	child->addBlock(elevatorbottom, TO_THE_TOP);
	child->addBlock(elevator, TO_THE_BOTTOM); //if you bring her this far just fully bring her home at that point, I did specifically say "when you're ready"
	child->addBlock(elevatorentrance, IN_ELEVATOR);
	child->setRecruitable(true);
	child->setRecruitCondition(SAVINGJILLY);
	child->setFifth(true);

	NPC* matilda = new NPC("WORRIED MOTHER", "MATILDA", "A frequent churchgoer with distress very visible on her face.", burgchurch, 0);
	matilda->addConversation({{self, "You look distressed."},
							  {matilda, "My daughter Jilly..."},
							  {matilda, "She was kidnapped."},
							  {self, "I can help!"},
							  {self, "Just direct me towards the kidnappers!"},
							  {NULL, "MATILDA - *faintly smiles*"},
							  {matilda, "That's sweet of you."},
							  {NULL, "..."},
							  {self, "No like I'm serious."},
							  {self, "We beat up this big lava knight thing on the way here."},
							  {self, "He was like at least triple the height of this building."},
							  {matilda, "I appreciate your humor, but..."},
							  {self, "Look I can manipulate energy!"},
							  {NULL, "You make an ENERGY BALL above your hand."},
							  {matilda, "Oh."},
							  {matilda, "..."},
							  {matilda, "Well..."},
							  {matilda, "You really think you can help?"},
							  {self, "Yep! :D"},
							  {matilda, "You might just be the answer to my prayers!"},
							  {matilda, "..."},
							  {matilda, "The kidnappers were headed towards the BURGER headquarters to the NORTH."},
							  {matilda, "You'll need this to get past their security systems."},
							  {matilda, "I was holding on to it in hopes of finding someone who could help."},
							  {self, "Oh thanks."},
							  {self, "I will get your kid back!"}});
	matilda->setDialogue({{matilda, "Ohhh my poor Jilly..."}, {matilda, "She must be so scared..."}, {matilda, "Thank you so much for offering to help!"}, {matilda, "I'll be praying for you from here!"}});
	matilda->setGift(cloakingdevice);
	matilda->setTalkMakeChanges(false);
	matilda->setWorldCondition(JILLYQUEST);
	matilda->addRecruitLink(forestknight); //this works just fine because you can never have already saved Jilly if you just start the quest to do that
	//there are some ratman recruit links but they're done below when ratman is actually defined
	Conversation matrej1 = {{self, "Hey wanna join my BURGER QUEST?"},
							{matilda, "..."},
							{matilda, "BURGERs aren't good, you know."},
							{self, "So they're a little unhealthy..."},
							{matilda, "No, they're bad for your soul."},
							{matilda, "You shouldn't try it."},
							{matilda, "Where are your parents?"}};
	matrej1.skipcondition = {JILLYQUEST};
	shared_ptr<Conversation> matrej2 = make_shared<Conversation>(Conversation({{self, "Hey wanna help me save your kid?"},
							{matilda, "I appreciate the offer,"},
							{matilda, "But I haven't any fighting ability like you."},
							{matilda, "I'll be praying for you from here!"}}));
	matrej1.alt = matrej2;
	matrej2->skipcondition = {JILLYSAVED};
	shared_ptr<Conversation> matrej3 = make_shared<Conversation>(Conversation({{self, "Hey wanna get a celebratory BURGER?"},
							{matilda, "What?"},
							{matilda, "You don't know the truth about BURGERs?"},
							{matilda, "They'll hurt your soul!"},
							{matilda, "Don't try any!"},
							{matilda, "Please!"}}));
	matrej2->alt = matrej3;
	matrej3->skipcondition = {TEMPLEQUEST};
	shared_ptr<Conversation> matrej4 = make_shared<Conversation>(Conversation({{self, "Hey wanna help me destroy BURGERs?"},
							{matilda, "What?"},
							{matilda, "Well that would be great,"},
							{matilda, "but I haven't any fighting ability like you."},
							{matilda, "I'll be praying for you from here!"}}));
	matrej3->alt = matrej4;
	matrej4->skipcondition = {BURGERMENDEF};
	shared_ptr<Conversation> matrej5 = make_shared<Conversation>(Conversation({{self, "Hey wanna join my team?"},
							{matilda, "Sorry, I need to take care of my Jilly."},
							{matilda, "Besides, I haven't any fighting ability like you."},
							{matilda, "Safe travels!"}}));
	matrej4->alt = matrej5;

	Item* jillydrawing = new MaterialItem("JILLY'S DRAWING", "A masterfully crafted crayon drawing depicting when you found Jilly.", limbo);

	NPC* theratman;

	shared_ptr<WorldChange> jillyreunion = make_shared<WorldChange>(); //when Jilly and Matilda are reunited!
	jillyreunion->linkedTitles.push({matilda, ""}); //she is no longer worried
	jillyreunion->linkedDescriptions.push({matilda, "A frequent churchgoer filled with joy after you saved her Jilly."});
	jillyreunion->linkedDescriptions.push({child, "The daughter of Matilda who you saved from the BURGER corporation, with very good potential in combat."});
	jillyreunion->linkedDialogue.push({matilda, {{matilda, "Oh I'm so happy to be reunited with my Jilly!"}, {matilda, "Thank you so much!"}}});
	jillyreunion->decruitLinks.push(child);
	jillyreunion->linkedDialogue.push({child, {{child, "Thank you for bringing me back to my mom, mister!"}, {child, "I hope you like my drawing!"}}});
	jillyreunion->dismissLinks.push({self->getParty(), child});
	jillyreunion->inventoryLinks.push({inventory, jillydrawing});
	jillyreunion->linkedAttacks.push({self, uppercut});
	jillyreunion->conditionalDecruits.push({forestknight, TEMPLEQUEST});
	jillyreunion->conditionalDecruits.push({richie, TEMPLEQUEST});
	jillyreunion->conditionalDecruits.push({theratman, TEMPLEQUEST});

	WorldChange jillylockin; //lock in because you now HAVE to save Jilly to progress cause you've decided to do that
	jillylockin.clingyLinks.push({child, NEVER});
	jillylockin.linkedEnterChanges.push({burgchurch, jillyreunion});
	jillylockin.linkedWelcomes.push({burgchurch, {{child, "MOM!"}, {matilda, "JILLY!"},
												  {NULL, "Jilly flies into Matilda's arms."},
												  {NULL, "Tears of joy flow from Matilda's eyes.!"},
												  {matilda, "Oh thank you so much!"},
												  {matilda, "Thank you so much for reuniting me with my Jilly!"},
												  {matilda, "I'm sorry, I wish I had something to repay you with."},
												  {child, "Thank you mister for saving me!"},
												  {child, "I made you this drawing!"},
												  {NULL, "Jilly hands you a drawing of her uppercutting you."},
												  {NULL, "You got the JILLY'S DRAWING!"},
												  {self, "Oh thanks."}}});
	//MARK: make sure you also can't dismiss Jilly when the BURGER MAN appears

	elevator->setEnterChanges(jillylockin, SAVINGJILLY);
	tunnels->setEnterChanges(jillylockin, SAVINGJILLY);

	Item* jillybag = new WorldChangeItem("SACK", "A tied-up sack with something wriggling inside.\nYou should untie it by USE-ing it.", burgstorage, 
		{{NULL, "You untie the SACK."},
		{NULL, "JILLY flies out of the sack!"},
		{NULL, "Jilly uppercuts you into the roof."},
		{self, "OW"}, {child, "Take that you meanie!"},
		{self, "BRO WHAT THE HECK I'M TRYING TO SAVE YOU"},
		{child, "Oh okay!"}, {child, "Thank you mister!"},
		{self, "No problem."},
		{NULL, "RECRUIT Jilly once you're ready to bring her back to her mom."}});
	jillybag->setDenial("You try to take the bag but it punches you in the gut.");
	WorldChange& jbchanges = ((WorldChangeItem*)jillybag)->getChanges();
	jbchanges.defeatRooms.push({child, burgstorage});

	Conversation burgabtconv = {{NULL, "You finally have your BURGER!"},
								{NULL, "This is what you came for."},
								{NULL, "Do you want to eat the BURGER?"}};
	shared_ptr<Conversation> burgabtconv2 = make_shared<Conversation>(Conversation({{NULL, "You could keep saving Jilly."},
								{NULL, "But isn't this BURGER what you really came for?"},
								{NULL, "Do you want to eat the BURGER?"}}));
	shared_ptr<Conversation> burgabtconv3 = make_shared<Conversation>(Conversation({{NULL, "Well, you saved Jilly."},
								{NULL, "But wasn't this BURGER what you really came for?"},
								{NULL, "Do you want to eat the BURGER?"}}));
	shared_ptr<Conversation> burgabtconv4 = make_shared<Conversation>(Conversation({{NULL, "This BURGER is what you originally came for."},
								{NULL, "You've since come to understand what it truly is."},
								{NULL, "Do you really want to eat the BURGER?"}}));
	burgabtconv.skipcondition = {JILLYQUEST, TEMPLEQUEST};
	burgabtconv2->skipcondition = {JILLYSAVED, TEMPLEQUEST};
	burgabtconv3->skipcondition = {TEMPLEQUEST};
	burgabtconv.alt = burgabtconv2;
	burgabtconv2->alt = burgabtconv3;
	burgabtconv3->alt = burgabtconv4;

	Conversation burgerconvo = {{NULL, "\nYou take a bite of the BURGER..."},
								{NULL, "Your BURGER QUEST has finally come to an end."},
								{NULL, "..."},
								{NULL, "..."},
								{NULL, "..."},
								{NULL, "It tasted pretty good."},
								{NULL, "..."},
								{NULL, "Was it really worth it?"},
								{NULL, "You did the same as the world,"},
								{NULL, "and the BURGER has hardened your heart."},
								{NULL, "\n\t<<< BURGER QUEST COMPLETE? >>>"
									   "\n\t<<<    ENDING ACHIEVED:    >>>"
									   "\n\t<<<    CONSUMER MINDSET    >>>"},
								{NULL, "\nWow what a lame and unfulfilling ending..."},
								{NULL, "Maybe there's another path you could take..."}};

	Conversation burghint = {{NULL, "Wow what a lame and unfulfilling ending..."},
							 {NULL, "Maybe there's another path you could take?"}};
	shared_ptr<Conversation> burghint2 = make_shared<Conversation>(Conversation({{NULL, "\nWow what a lame and unfulfilling ending..."},
																				 {NULL, "Maybe you should focus on the task at hand..."}}));
	shared_ptr<Conversation> burghint3 = make_shared<Conversation>(Conversation({{NULL, "\nWow what a lame and unfulfilling ending..."},
																				 {NULL, "Maybe there's still another path you could take?"}}));
	shared_ptr<Conversation> burghint4 = make_shared<Conversation>(Conversation({{NULL, "\nWow what a lame and unfulfilling ending..."},
																				 {NULL, "Maybe you should focus on the task at hand..."}}));
	burghint.skipcondition = {JILLYQUEST, TEMPLEQUEST};
	burghint2->skipcondition = {JILLYSAVED, TEMPLEQUEST};
	burghint3->skipcondition = {TEMPLEQUEST};
	burghint.alt = burghint2;
	burghint2->alt = burghint3;
	burghint3->alt = burghint4;
	
	Item* BURGER = new BURGERItem("BURGER", "It's a BURGER.", limbo, burgerconvo, burgabtconv, burghint);
	Item* freeboiga = new BURGERItem(*(BURGERItem*)(BURGER));

	NPC* burgerman = new NPC("", "BURGER MAN", "The manager and mascot of BURGERs. He has a BURGER for a head and a stick figure body.", BURGERRESTAURANT, 10000, Stats(6000000, 60000, 30000, 60000, 60000, 60000, 9000), Stats(20, 2, 1, 2, 2, 2, 0));
	//burgerman->setLeader(true, 0, BURGERRESTAURANT, false);
	burgerman->setNoFight(false); //you can kind of fight the BURGER MAN but not really, leader status won't reset if trying to account for try to fight without THE PLOT DEVICE and then you get bad ending, then try to fight in the true ending
	BURGERRESTAURANT->setStock(BURGER, 2147483647, 10, {{burgerman, "ENJOY YOUR BURGER!"}});
	BURGER->setFreebie({{burgerman, "YOU CAN'T AFFORD A BURGER?"}, {burgerman, "I BELIEVE THERE SHOULD BE NO FINANCIAL BARRIERS TO ENJOYING A BURGER."}, {burgerman, "HERE, HAVE ONE ON THE HOUSE!"}, {self, "Oh thanks!"}});
	burgerman->setDialogue({{burgerman, "HELLO! WELCOME TO MY BURGER RESTAURANT!"}, {burgerman, "HOW MAY I TAKE YOUR ORDER?"}});
	Conversation burgdeciet = {{burgerman, "YOU HAVE BEEN MAKING QUITE THE MESS."},
							   {self, "I'm sorry :("},
							   {self, "But like your employees kidnapped this kid,"},
							   {self, "so I had to save her."},
							   {burgerman, "REALLY?"},
							   {burgerman, "THIS TRULY SADDENS ME."},
							   {burgerman, "SUCH CORRUPTION IN MY OWN COMPANY?"},
							   {burgerman, "I MUST THANK YOU, THEN."},
							   {burgerman, "FOR DEALING WITH IT FOR ME."},
							   {self, "The CEO guy said you told him to do that though?"},
							   {burgerman, "WOW."},
							   {burgerman, "NOT ONLY DOES HE ORDER THE KIDNAPPING OF A CHILD,"},
							   {burgerman, "BUT HE SHIFTS THE BLAME TO ME?"},
							   {burgerman, "YOU CAN'T TRUST THE WORD OF A KIDNAPPER..."},
							   {burgerman, "REST ASSURED, I WILL FIND BETTER EMPLOYEES,"},
							   {burgerman, "TO ENSURE A MOST POSITIVE EXPERIENCE FOR ALL."},
							   {burgerman, "HERE, HAVE A BURGER ON THE HOUSE."},
							   {burgerman, "A TOKEN OF MY GRATITUDE."},
							   {self, "Oh thanks."}};
	burgdeciet.skipcondition = {TEMPLEQUEST};
	shared_ptr<WorldChange> freeburgchange = make_shared<WorldChange>();
	freeburgchange->worldcon = FREEBURGER;
	burgdeciet.convochanges = freeburgchange; //update BURGER MAN catch dialogue to account for the free BURGER he gave the player
	Conversation burgcatch = {{burgerman, "The BURGER MAN grabs you by the neck."},
							  {burgerman, "YOU HAVE BEEN MAKING QUITE THE MESS."},
							  {burgerman, "YOU THOUGHT I WOULDN'T NOTICE?"},
							  {self, "I wasn't thinking about you :|"},
							  {burgerman, "The BURGER MAN knocks you unconscious."},
							  {NULL, "..."},
							  {NULL, "..."},
							  {NULL, "..."},
							  {NULL, "You wake up, disoriented."},
							  {NULL, "Your hands are in chains and you're lying on hard concrete."}};
	shared_ptr<Conversation> burgcatch2 = make_shared<Conversation>(Conversation({{burgerman, "The BURGER MAN grabs you by the neck."},
							  {burgerman, "WOW."},
							  {burgerman, "YOU WANT TO DEFEAT ME?"},
							  {burgerman, "AFTER I GAVE YOU A BURGER ON THE HOUSE?"},
							  {self, "I don't believe you anymore >:|"},
							  {burgerman, "The BURGER MAN knocks you unconscious."},
							  {NULL, "..."},
							  {NULL, "..."},
							  {NULL, "..."},
							  {NULL, "You wake up, disoriented."},
							  {NULL, "Your hands are in chains and you're lying on hard concrete."}}));
	burgcatch.skipcondition = {FREEBURGER};
	burgcatch.alt = burgcatch2;
	WorldChange burgermanappear; //when you encounter the BURGER MAN in the basement
	burgermanappear.worldcon = {BURGERCHASE};
	burgermanappear.pursueLinks.push({burgerman, self});
	burgbasese->setEnterChanges(burgermanappear, TEMPLEQUEST);
	
	//final questgiver
	NPC* burgerprisoner = new NPC("BURGER PRISONER", "ARCHIBALD", "A prisoner gray with age, chained to the wall for resisting the global domination of BURGER.", BURGERPRISON, 35);
	burgerprisoner->setDialogue({{burgerprisoner, "I may not be able to assist physically,"}, {burgerprisoner, "but I will be praying for you from here."}});
	Conversation prisrej = {{burgerprisoner, "Well,"},
							{burgerprisoner, "I can't really leave here."},
							{NULL, "ARCHIBALD shakes his chains for emphasis."}};
	shared_ptr<Conversation> prisrej2 = make_shared<Conversation>(Conversation({{burgerprisoner, "I would love to join you on your quest."},
							  {burgerprisoner, "But I think you can see I'm not going anywhere..."},
							  {NULL, "ARCHIBALD shakes his chains for emphasis."}}));
	prisrej.skipcondition = {TEMPLEQUEST};
	prisrej.alt = prisrej2;
	burgerprisoner->addRejectionDialogue(prisrej);

	//set up catching stuff which is used later in the basement part
	WorldChange burgcatchanges;
	burgcatchanges.defeatRooms.push({self, BURGERPRISON});
	Conversation escapehint = {{burgerprisoner, "You're finally awake."}, {burgerprisoner, "You had me worried for a moment, there."},
							   {self, "Dang."}, {self, "Guess we gotta wait for a new protagonist :|"},
							   {burgerprisoner, "Don't give up yet!"}, {burgerprisoner, "You've gotten this far!"},
							   {self, "No sorry I was joking mostly."},
							   {burgerprisoner, "Yeah. Well..."},
							   {burgerprisoner, "..."},
							   {burgerprisoner, "I know there's tunnels near this room."},
							   {burgerprisoner, "I got here long ago when they were still finishing the restaurant,"},
							   {burgerprisoner, "and I remember trains arriving here with materials."},
							   {burgerprisoner, "Though, it was covered up by your wall, there, long ago..."},
							   {burgerprisoner, "Probably something to do with security,"},
							   {burgerprisoner, "and also a lobster infestation that occured around that time."},
							   {burgerprisoner, "Maybe if we could somehow access those tunnels..."},
							   {self, "..."},
							   {self, "Train tunnels and lobsters, you say?"}};
	escapehint.skipcondition = {ESCAPEDBASE};
	burgcatchanges.linkedConversations.push({burgerprisoner, escapehint});
	burgcatchanges.linkedDialogue.push({burgerprisoner, {{burgerprisoner, "..."}, {burgerprisoner, "So what is it you had to say about train tunnels and lobsters?"}}});
	burgcatchanges.linkedBackups.push(make_tuple(BURGERPRISON, backupcaller5, inventory)); //if the player never got any lobster caller we do have to make sure they can get one (less cool moment if they need this one, but oh well such is life)
	burgcatchanges.linkedStations.push(BURGERPRISON);
	burgcatchanges.worldcon = IMPRISONED;
	burgerman->setCatchText(burgcatch);
	burgerman->setPursueStuff({{burgbasenw, burgbasew, burgbasesw}, {burgerbasement, burgbasec, burgbases}, {burgbasene, burgbasee, burgbasese}}, burgcatchanges);
	burgerman->setPursueSpecial(elevatorbottom, OUT, {{NULL, "You press the button to go up."},
													  {NULL, "The elevator doors start closing."},
													  {NULL, "..."},
													  {NULL, "The BURGER MAN puts his foot in the door."},
													  {NULL, "The elevator door catches on the BURGER MAN's foot."},
													  {NULL, "The doors open again."},
													  {self, ":|"}});

	//MARK: custom fight deny text, deny links
	//A fight with the BURGER MAN would end in surefire defeat.
	//Use THE PLOT DEVICE!

	Conversation priscon = {{burgerprisoner, "Hm?"},
							{self, "Hi what are you doing in prison?"},
							{burgerprisoner, "I haven't seen anybody in ages..."},
							{burgerprisoner, "Nobody concerned for my well-being, at least."},
							{burgerprisoner, "I was put here for resisting the global domination of BURGER."},
							{self, "Global domination?"},
							{burgerprisoner, "Yes, they're much more than their fast food facade."},
							{burgerprisoner, "I'm sure you can tell from this basement."},
							{self, "I mean... yeah."},
							{burgerprisoner, "So how'd you even get down here?"},
							{self, "Well I was going on a BURGER QUEST,"},
							{self, "but then I started saving this kid that was kidnapped,"},
							{self, "so then I went to their headquarters,"},
							{self, "and I found this button there,"},
							{self, "which looked like the BURGER RESTAURANT's"},
							{self, "so I put the button in the wall and I came down here."},
							{burgerprisoner, "Well, surely you can see what this company truly is by now."},
							{self, "I guess..."},
							{burgerprisoner, "All the BURGER corporation is, is an extension of the BURGER MAN"},
							{burgerprisoner, "which he uses to bring people to his restaurant."},
							{burgerprisoner, "BURGER has the world in a chokehold and it doesn't even realize."},
							{burgerprisoner, "I'm sure you know about the dying forests and the growing wastelands."},
							{self, "Yeah."},
							{burgerprisoner, "That's all BURGER's fault."},
							{burgerprisoner, "Parching the land to guide the people to the city by thirst and hunger,"},
							{burgerprisoner, "where they will be more easily tempted by the BURGER RESTAURANT."},
							{self, "Oh."},
							{burgerprisoner, "Or how about the volcanic highlands surrounding the city?"},
							{burgerprisoner, "All BURGER."},
							{self, "Were they why the lava level was so high?"},
							{burgerprisoner, "Probably."},
							{burgerprisoner, "They don't even care if the people get a BURGER."},
							{burgerprisoner, "As long as they idolize BURGER, it makes them stray from God."},
							{burgerprisoner, "Even if they die on the way,"},
							{burgerprisoner, "that just means they have less time to realize the damage BURGER causes,"},
							{burgerprisoner, "and repent from that path."},
							{self, "..."},
							{burgerprisoner, "But there is hope, I tell you."},
							{burgerprisoner, "They didn't put me in here for holding up a protest sign."},
							{burgerprisoner, "I know a way to deal a heavy blow against BURGER's grasp on the world."},
							{burgerprisoner, "You had to fight through much to get here."},
							{burgerprisoner, "I know you'd be capable of doing it."},
							{burgerprisoner, "If you're willing, I can tell you how."},
							{NULL, "\nYou are on a quest to get a BURGER."},
							{NULL, "But now you have the opportunity to change your quest."}, //You have the opportunity to change your quest now.
							{NULL, "Do you want to accept this DESTROY BURGER QUEST? (YES or NO)"}};
	shared_ptr<Conversation> prisyes = make_shared<Conversation>(Conversation({{self, "..."},
							{self, "Alright I'll do it."},
							{burgerprisoner, "Okay!"},
							{burgerprisoner, "You know the BURGER MAN, right?"},
							{burgerprisoner, "At the top of the restaurant?"},
							{burgerprisoner, "BURGERs arrived at the same time he did."},
							{burgerprisoner, "He's the one in charge of the whole operation."},
							{burgerprisoner, "I'm sure that if you cut it off at the source,"},
							{burgerprisoner, "the rest of the corporation will fall like dominoes."},
							{self, "That's it?"},
							{burgerprisoner, "No."},
							{burgerprisoner, "Just wanna be clear,"},
							{burgerprisoner, "DO NOT try to fight the BURGER MAN."},
							{burgerprisoner, "No worldly force can beat him."},
							{burgerprisoner, "Only with God's help would anyone stand a chance."},
							{burgerprisoner, "That's why you have to get a special device,"},
							{burgerprisoner, "specially engineered to do God's will."},
							{burgerprisoner, "In other words, it does whatever the plot requires."}, 
							{burgerprisoner, "It's..."},
							{NULL, "<<< THE PLOT DEVICE! >>>"},
							{self, "The plot device?"},
							{burgerprisoner, "Yeah!"},
							{burgerprisoner, "Now it's not possible to be destroyed,"},
							{burgerprisoner, "but it was disassembled long ago,"},
							{burgerprisoner, "and scattered across three temples."},
							{self, "You mean the ancient temples?"},
							{self, "They're sealed shut by ancient technology!"},
							{self, "No matter what anybody's tried, they've never made it inside!"},
							{burgerprisoner, "Oh to open them you just gotta ASK NICELY."},
							{self, "Oh I see."},
							{burgerprisoner, "Well, putting the components together is a long lost craft,"},
							{burgerprisoner, "but I believe I know how to do so."},
							{burgerprisoner, "So once you're done just bring them to me!"},
							{self, "Sounds good."}}));
	shared_ptr<Conversation> prisno = make_shared<Conversation>(Conversation({{self, "No,"},
																			  {self, "I'm sorry."},
																			  {self, "I just want a BURGER."},
																			  {burgerprisoner, "Well,"},
																			  {burgerprisoner, "I respect your free will."},
																			  {burgerprisoner, "But if you change your mind,"},
																			  {burgerprisoner, "you know where to find me."}}));
	priscon.branch1 = {"YES", prisyes};
	priscon.branch2 = {"NO", prisno};
	shared_ptr<Conversation> prisnoagain = make_shared<Conversation>(Conversation({{self, "No sorry."}, {burgerprisoner, "Alright."}}));
	shared_ptr<Conversation> prisagain = make_shared<Conversation>(Conversation({{burgerprisoner, "Well have you changed your mind, then?"},
																			   {NULL, "\nYou are on a quest to get a BURGER."},
																			   {NULL, "But now you have the opportunity to change your quest."},
																			   {NULL, "Do you want to accept this DESTROY BURGER QUEST? (YES or NO)"}}));
	prisagain->branch1 = {"YES", prisyes};
	prisagain->branch2 = {"NO", prisnoagain};
	prisno->relay = {burgerprisoner, prisagain};
	prisnoagain->relay = {burgerprisoner, prisagain};
	relaysH.push_back(prisagain); //make sure this conversation don't expire
	burgerprisoner->addConversation(priscon);
	shared_ptr<Conversation> prisfollowup = make_shared<Conversation>(Conversation({{burgerprisoner, "..."}, //remind the player they were saving Jilly
		{burgerprisoner, "Hang on, you said they kidnapped a kid right?"},
		{self, "Yeah."},
		{burgerprisoner, "Well did you save them yet?"},
		{self, "Not really."},
		{burgerprisoner, "Well, I'd say you go do that first."},
		{burgerprisoner, "That kid's safety is important,"},
		{burgerprisoner, "especially when dealing with a company of this nature."},
		{burgerprisoner, "The temples aren't going anywhere, anyway."}}));
	prisfollowup->skipcondition = {JILLYSAVED};
	prisyes->next = prisfollowup;	

	WorldChange lobappearchange;
	Conversation lobsavedialogue = {{burgerprisoner, "Well how about that."}};
	shared_ptr<Conversation> lobsavedialogue2 = make_shared<Conversation>(Conversation({{burgerprisoner, "I may not be able to assist physically,"}, {burgerprisoner, "but I will be praying for you from here."}})); //just the original dialogue again
	lobsavedialogue.skipcondition = {TAMEDLOBSTER};
	lobsavedialogue.alt = lobsavedialogue2;
	lobappearchange.linkedDialogue.push({burgerprisoner, lobsavedialogue});
	lobappearchange.exitPavings.push(make_tuple(BURGERPRISON, basestation, IN_THE_HOLE, OUT));
	lobappearchange.roomChanges.push({BURGERPRISON, "in the BURGER PRISON, full of cells and torture devices, and now featuring a huge hole in the wall."});
	//basement escape changes, can be here because lobster is required to escape now
	lobappearchange.worldcon = ESCAPEDBASE;
	lobappearchange.dismissableLinks.push({child, SAVINGJILLY}); //when you escape you can dismiss jilly now unless you were already saving her (this is if you weren't saving her and then you go back to save her after escaping the basement)
	lobappearchange.defeatRooms.push({burgerman, BURGERRESTAURANT});
	lobappearchange.pursueLinks.push({burgerman, NULL}); //this also updates player's pursuer to NULL

	shared_ptr<WorldChange> templequest = make_shared<WorldChange>();
	templequest->worldcon = {TEMPLEQUEST};
	templequest->linkedDegifts.push(burgerman);
	templequest->removeStock.push({BURGERRESTAURANT, BURGER});
	templequest->exitBlocks.push(make_tuple(BURGERRESTAURANT, IN_ELEVATOR, ENEMY, "sealed shut by the BURGER RESTAURANT controls."));
	templequest->linkedDescriptions.push({burgerman, "The manager and mascot of BURGER. He has a BURGER for a head and an uncanny stature."});
	templequest->recruitLinks.push(forestknight);
	templequest->recruitLinks.push(richie);
	templequest->recruitLinks.push(theratman);
	templequest->clingyLinks.push({child, NEVER});
	templequest->defeatRooms.push({burgerman, burgbasene});
	Conversation forgorending = {{burgerman, "YOU ARE REALLY STUPID."},
								 {burgerman, "YOU CAME BACK WITHOUT THE PLOT DEVICE?"},
								 {NULL, "\nThe BURGER MAN punches through your head."},
								 {NULL, "\n\t<<< BURGER QUEST COMPLETE T_T >>>"
								        "\n\t<<<      ENDING ACHIEVED:     >>>"
								        "\n\t<<<   YOU FORGOR THE DEVICE   >>>"}};
	shared_ptr<WorldChange> forgorchanges = make_shared<WorldChange>(); //to mark the game as ended and move the player outside the restaurant so they're not softlocked
	forgorchanges->worldcon = GAMEEND;
	forgorchanges->defeatRooms.push({self, elevatortop});
	forgorending.convochanges = forgorchanges;
	templequest->linkedDialogue.push({burgerman, forgorending}); //cannot be heard in the basement because he just catches you if you ASK him
	prisyes->convochanges = templequest;

	NPC* jimmyjohn = new NPC("SHOPKEEPER", "JIMMY JOHN", "The owner of the village convenience store. He has an italian accent.", tentstore, 30);
	jimmyjohn->setDialogue("Welcome to my convenience store! None is more convenient!");
	jimmyjohn->addRejectionDialogue("I'm sorry I cannot. Who will take care of my store?");

	Effect* minibuff = new Effect("MINIBUFF", 4, 0, 0, 1.25, 1.25, 1.25, 1.25);
	Effect* buff = new Effect("BUFF", 4, 0, 0, 1.5, 1.5, 1.5, 1.5);
	Effect* bigbuff = new Effect("BIG BUFF", 4, 0, 0, 2.0, 2.0, 2.0, 2.0);
	Effect* megabuff = new Effect("MEGABUFF", 2, 0, 0, 2.5, 2.5, 2.5, 2.5);
	
	Item* mythicmango = new HpItem("MYTHICAL MANGO", "The most nutritious fruit, a big mango that sparkles in the sunlight. (heals all HP)", limbo, 999); //JIMMY JOHN - Ah yes, that's a very rare mango. Make sure to save it until you really need it! And thank you for your patronage

	//tent store stock
	Item* apple = new HpItem("HEALTHY APPLE", "A healthy red apple. (heals 10 HP)", limbo, 10);
	Item* pineapple = new HpItem("HEALTHY PINE APPLE", "A crunchy and durable pine apple. Very annoying to eat, but apparently very healthy! (heals 25 HP)", limbo, 25);
	Item* starapple = new HpItem("HEALTHY STAR APPLE", "A sparkling star-shaped fruit with enough nutrients to last you 10 days. (heals 50 HP)", limbo, 50);
	Item* noodles = new EffectItem("MIGHTY NOODLES", "Some homemade noodles sprinkled with mighty spices. (MINIBUFF effect)", limbo, minibuff);
	Item* lasagna = new EffectItem("MIGHTY LASAGNA", "A homemade lasagna made with mighty flour. (BUFF effect)", limbo, buff);
	Item* pizza = new EffectItem("MIGHTY PIZZA", "A homemade pizza topped with mighty pepperoni. (BIG BUFF EFFECT)", limbo, bigbuff);
	Item* energybook = new EducationItem("ADVANCED GUIDE TO ENERGY MANIPULATION", "A book full of energy manipulation techniques. You could learn some cool attacks from this.", limbo, precisionstrike);
	EducationItem* _energybook = (EducationItem*)energybook; //we have to convert the book to EducationItem in order to add the attacks because regular Item*s have no setAttack function
	_energybook->setAttack(ballisticmissile);
	_energybook->setAttack(bigenergyball);
	_energybook->setAttack(energize);
	_energybook->setAttack(spbomb);

	tentstore->setStock(apple, 2147483647, 5, {{jimmyjohn, "Thank you for your patronage! Enjoy your apple!"}});
	tentstore->setStock(pineapple, 2147483647, 20, {{jimmyjohn, "Thank you for your patronage! Enjoy your pine apple!"}});
	tentstore->setStock(starapple, 2147483647, 60, {{jimmyjohn, "Thank you for your patronage! Enjoy your star apple!"}});
	tentstore->setStock(noodles, 2147483647, 5, {{jimmyjohn, "Thank you for your patronage! Enjoy your noodles!"}});
	tentstore->setStock(lasagna, 2147483647, 25, {{jimmyjohn, "Thank you for your patronage! Enjoy your lasagna!"}});
	tentstore->setStock(pizza, 2147483647, 50, {{jimmyjohn, "Thank you for your patronage! Enjoy your pizza!"}});
	tentstore->setStock(energybook, 1, 100, {{jimmyjohn, "I don't understand that book."}, {jimmyjohn, "But if it interests you then great!"}, {jimmyjohn, "Oh, and thank you for your patronage!"}});

	NPC* mrdeer = new NPC("", "MR DEER", "Your friend MR. DEER. He's a deer.", deerclearing, 5);
	Item* deerkey = new KeyItem("DEER KEY", "The key to the great forest wall.", {{NULL, "You put the DEER KEY in the keyhole."}, {NULL, "The forest gate has been unlocked!"}}, limbo, LOCK);
	mrdeer->setGift(deerkey);
	mrdeer->addConversation({{self, "Hello Mr. Deer!"},
							 {NULL, "MR DEER - *salutatory deer noises*"}, //we have to use this weird fake name printing so we can have the * without the "
							 {self, "I'm going on a BURGER QUEST, but I can't get past the great forest wall without your key."},
							 {NULL, "MR DEER - *warning deer noises*"},
							 {NULL, "MR DEER - *concerned deer noise*"},
							 {NULL, "MR DEER - *thinking deer noises*"},
							 {NULL, "..."},
							 {self, "(I hope he gives me the key...)"},
							 {NULL, "MR DEER - *reluctantly affirmative deer noise*"},
							 {self, "AYYY thank you so much Mr. Deer!"}});
	mrdeer->setDialogue({{NULL, "MR DEER - *deer noises*"}});
	mrdeer->addRejectionDialogue({{NULL, "MR DEER - *no thank you deer noise*"}});

	NPC* factelder = new NPC("FACTORY ELDER", "FELVIN", "Antique mechanical elder who overlooks the factory.\nHe's bolted to the wall and has a metallic mustache.", factory1, 100, Stats(200, 80, 120, 10, 50, 10, 30));
	Conversation factconvo = {{factelder, "Child, Have you seen the lava sea?"},
							  {factelder, "It's been abnormally high recently, covering up the road to BURGERSBURG."},
							  {factelder, "But the allure of BURGER is too great!"},
							  {factelder, "Some people try to sail across and get burnt up!"},
							  {factelder, "Some are even so desperate they try to swim across!"},
							  {self, "Oh dang."},
							  {factelder, "Please! You must lower the lava again!"},
							  {factelder, "These factories have drainage valves in their control rooms!"}};
	factconvo.skipcondition = {SWITCHUSED};
	shared_ptr<Conversation> factconvo2 = make_shared<Conversation>(Conversation({{factelder, "Child, Have you seen the lava sea?"},
							   {factelder, "It's been abnormally high recently, covering up the road to BURGERSBURG."},
							   {factelder, "But the allure of BURGER is too great!"},
							   {factelder, "Some people try to sail across and get burnt up!"},
							   {factelder, "Some are even so desperate they try to swim across!"},
							   {factelder, "Please! You must lower the lava again!"},
							   {factelder, "These factories have drainage valves in their control rooms!"},
							   {self, "Yea I've already been doing that."},
							   {factelder, "Oh. Well, carry on then."}}));
	factconvo.alt = factconvo2;
	factconvo2->skipcondition = {LAVADRAINED};
	shared_ptr<Conversation> factconvo3 = make_shared<Conversation>(Conversation({{factelder, "Child, Have you seen the lava sea?"},
							   {self, "Yeah I already did this whole quest."},
							   {self, "Lava sea looks pretty lowered now."},
							   {factelder, "Oh. Well, good job!"},
							   {self, "Thanks."}}));
	factconvo2->alt = factconvo3;
	factelder->addConversation(factconvo);
	factelder->addConversation({{self, "So what's it like being on a wall your whole life?"}, {factelder, "Kind of boring, but I have plenty of time to come up with a lot of stories."}, {self, "I see."}});
	factelder->setDialogue("You must drain the lava before more lives are lost!");
	factelder->addRejectionDialogue("I only oversee this factory; I cannot leave the wall.");

	NPC* developer = new NPC("DEVELOPER", "TOMAS", "It's me the guy who made the game.", tenthome, 67, Stats(67, 67, 67, 67, 67, 67, 0));
	developer->setDialogue("Yo wassup.");
	Conversation devconvo = {{developer, "Ayy BERNARD how's it going?"},
							{self, "Pretty good I think but the player actually named me that."},
							{NULL, "\n^"
								   "\n|"
								   "\n|"
								   "\n|"},
	// (\               				// (\               				// (\               			// (\               
	// /(\                .,			// /(\                .,			// /(\                .,		// /(\               .,
	// \_/               / |			// \_/               / |			// \_/               / |		// \_/              / |
	//      \-._    ___ /  |			//      \-._  ^ ___ /  |			//      \-._  ^ ___ /  |		//      \-._  /\___/  |
	//  _____\,\`"-_____"  `-"""/		//  _____\, `"-_____"  `-"""/		//  _____\, `  _____   `-"""/	//    ___\, `' _____  `-"""/
	//  ""---___\/___ ___\__--""		//  ""---___ /___ ___\__--""		//  ""---___ /___ ___\__--""	//    "--__  /___ ___\ _--"
	//          |  |   |  |				//          |  |   |  |				//          |  |   |  |			//         '|  |   |  |'
	//           \   -   /				//           \   -   /				//           \   -   /			//           \   -   /
	//             """""				//             """""				//             """""			//             """""" \ 
	//            |     |				//            |     |				//            |     |			//            |     |.`\ .
							{developer, "Well that's pretty unfortunate."},
							{self, "Why did you let them name me. T_T"}, //this is annoyed face not crying
							{developer, "Sorry I came up with your name after the naming feature."},
							{developer, "Don't worry you will always truly be BERNARD."},
							{developer, "Well good job getting here."},
							{developer, "Here have this cool amazing new move."},
							{self, "Yoooo nice thanks.\n\n\n"}}; //MARK: is there a reason there's three newlines here?
	devconvo.skipcondition = {ISBERNARD};
	shared_ptr<Conversation> devconvo2 = make_shared<Conversation>(Conversation({{developer, "Ayy BERNARD how's it going?"},
							{self, "Pretty good!"},
							{self, "The player actually named me the correct name!"},
							{developer, "Yo nice."},
							{developer, "Well good job getting here."},
							{developer, "Here have this cool amazing new move."},
							{self, "Yoooo nice thanks.\n\n\n"}})); //MARK: is there a reason there's three newlines here?
	devconvo.alt = devconvo2;
	developer->addConversation(devconvo);
	developer->addConversation({{self, "Yo developer man."},
								{developer, "Yeah?"},
								{self, "Why is everything text?"},
								{developer, "Because the camera man is on vacation."},
								{developer, "Only the narrator could cover his shift."},
								{self, "Oh I see."}});
	developer->addConversation({{self, "Can I recruit TECH DEMO MAN?"},
								{developer, "No."}});
	developer->addRejectionDialogue("No I don't think that would make sense.");
	developer->setGift(parryguide);

	/*{{self, "Yo developer man."},
	 {developer, "Hey what's up?"},
	 {self, "wdym ???"},
	 {self, "So I was wondering,"},
	 {self, "what's up with all the parallel universes in the first game?"},
	 {self, "Also who was the other BURGER MAN?"},
	 {developer, "Oh uhhhhh... honestly idk maybe I'll just retcon that."},
	 {developer, "Wait no I got this."},
	 {developer, "So the parallel universes are just the same universe but going through them increments the number."},
	 {developer, "Wait that makes no sense because of the news."},
	 {developer, "Uhhhhhhhhhhhh..."},
	 {developer, "Well I've edited this conversation in the future and I've decided that it's the same universe,"},
	 {developer, "but it's different subtly such as the news due to quantumn superposition."},
	 {developer, "So going to a different universe is actually the same universe,"},
	 {developer, "except you're viewing a different quantumn state of the timeline/universe,"},
	 {developer, "which is why you see subtle differences such as the news."},
	 {self, "Oh I see that makes perfect sense!"},
	 {developer, "Anyway tying into the other BURGER MAN,"},
	 {developer, "I guess universe -1 was some sort of prison for the final boss guy."},
	 {developer, "And then he invented BURGERs,"},
	 {developer, "and you know how Henry Jerry's company invented time travel?"},
	 {self, "Uh huh."},
	 {developer, "Yeah so using a bootstrap paradox,"},
	 {developer, "the BURGER MAN tempted Henry Jerry into finding the BURGER,"},
	 {developer, "and time traveled back to the future once all was said and done."},
	 {developer, "So he ordered the BURGER from himself."},
	 {self, "Dang that's crazy."}};
	
	//talk about the game after beating final boss
	//and dlc plans, crowbars
	//jim shady commentary?
	//favorite character
	//least favorite character
	{{self, "So what actually are monies?"},
	 {developer, "???"},
	 {self, "wdym ???"},
	 {developer, "Bro you're carrying monies around you tell me."},
	 {self, "Well you made me say that so you could explain what monies are. T_T"},
	 {developer, "Yep that's true!"},
	 {developer, "Well so I'm thinking monies are like gold coins."},
	 {developer, "So yeah there you have it."},
	 {developer, "I'm pretty sure that name was based off of this Desmos bookmark,"},
	 {developer, "for some graph that was for visualizing currency inflation,"},
	 {developer, "like it starts at a certain value and gets divided by x,"},
	 {developer, "where x is how many of that currency exists."},
	 {developer, "I don't really know if that's exactly how it works but that's what the graph was so yeah."},
	 {developer, "And anyway the bookmark was called \"economonies\" so that's where I assume monies came from."}};
	//have you tried... the gym, the simulator, the triple chest in the sewers, the bank, BURGER QUEST 1
	{{developer, "Have you ever played BURGER QUEST? (the first one)"},
	 {developer, "The sequel is way better but if you're curious you can try it on this link: https://codehs.com/sandbox/id/zuul-kb8vTu/run"},
	 {developer, "You're probably on a terminal so you should do shift+insert to copy paste."}}
	{{developer, "I was walking to my C++ class today,"},
	 {developer, "and I saw a bicycle-powered blender."},
	 {developer, "So you pedal and that generates electricity and the blender blends the stuff."},
	 {developer, "I think it's a very good idea."}}
	{{developer, "Have you tried the gym yet?"},
	 {developer, "It's very useful for having benched teammates catch up to your level."},
	 {developer, "And also it leaves them all in one place as a bonus side-effect."}}
	{{developer, "Have you found the bank yet?"},
	 {developer, "Idk if you'll need it but there wasn't any reason not to make it."},
	 {developer, "Except time but I made it already so yeah. (didn't take too long tho)"},
	 {developer, "Well that bank is very reliable."}}
	//have you fought greer yet
	//can you give me feedback (and say link to form)
	{{developer, "Idk if you've encountered teammates with defend attack,"},
	 {developer, "like they tank hits for someone else after using a defend move,"},
	 {developer, "but something funny about that is when I was making that type of move,"},
	 {developer, "I realized they wouldn't check if it was a beneficial attack,"},
	 {developer, "so like if for example a teammate was going to heal someone,"},
	 {developer, "but that someone was being defended by another teammate,"},
	 {developer, "the defender would like get in the way of the attack and be like,"},
	 {NULL, "DEFENDER - \"NOOOOOOO! I WILL DEFEND YOU, SOMEONE!\""},
	 {developer, "And then they would take the heal for the person they're defending."},
	 {self, "How silly behavior."}}
	//save system stuff
	//commentary about the world
	{{developer, "It's weird how I have a very specific vision for how the world looks,"},
	 {developer, "like I have character designs and everything,"},
	 {developer, "but nobody will ever see them cause it's a text-based game :|"},
	 {developer, "Maybe I'll make this a graphics-based game someday and then you can see all that..."},
	 {devdloper, "but that's a very considerably sized maybe."}}
	//scope creep
	//future projects
	{{developer, "Hey check it out I can say dancing!"},
	 {NULL, " _/\n  \\O\n  / \\"},
	 {NULL, "\n_|_O\n    \\"},
	 {NULL, "\n   O\n /' '\\"},
	 {NULL, "\n   O_|_\n  /"},
	 {NULL, "    \\_\n   O/\n  / \\"},
	 {NULL, "  \\ /\n   O\n  / \\"},
	 {NULL, " _/\n  \\O\n  / \\"},
	 {NULL, "\n_|_O\n    \\"},
	 {NULL, "\n   O\n /' '\\"},
	 {NULL, "\n   O_|_\n  /"},
	 {NULL, "    \\_\n   O/\n  / \\"},
	 {NULL, "  \\ /\n   O\n  / \\"},
	 {NULL, " _/\n  \\O\n  / \\"},
	 {NULL, "\n_|_O\n    \\"},
	 {NULL, "\n   O\n /' '\\"},
	 {NULL, "\n   O_|_\n  /"},
	 {NULL, "    \\_\n   O/\n  / \\"},
	 {NULL, "  \\ /\n   O\n  / \\"},
	 {NULL, " _/\n  \\O\n  / \\"},
/*	 _/										    \_		  \ /		 _/										    \_		  \ /		 _/										    \_		  \ /		 _/	
	  \O		_|_O	   O		   O_|_	   O/		   O		  \O		_|_O	   O		   O_|_	   O/		   O		  \O		_|_O	   O		   O_|_	   O/		   O		  \O
	  / \		    \	 /' '\		  /  	  / \		  / \		  / \		    \	 /' '\		  /  	  / \		  / \		  / \		    \	 /' '\		  /  	  / \		  / \		  / \ * /
	{developer, "He's breakdancing!"}}
	{{developer, "I've noticed,"},
	 {developer, "this game has a noticable lack of dinosaurs."}}
	//favorite game
	//talk about the mountain {developer, ""}
	{{developer, "You know there was going to be a \"BIG CAT\" as the first enemy on the mountain,"},
	 {developer, "it was like a golden lion and it had a silver mane and it was really big,"},
	 {developer, "but it was also really boring so I replaced it with that snowman that dies instantly."},
	 {developer, "Sorry I mean \"gets incapacitated\" instantly."}};
	//limbo
	//burger quest 2 ideation
	//talk about bosses
	{{developer, "I found one of the first ideas for this game; it was pretty interesting."},
	 {developer, "This was from a Zuul assignment in my C++ class after last year's Python class,"},
	 {developer, "which also had a Zuul assignment (BURGER QUEST 1!)."},
	 {developer, "So I knew I wanted to make the sequel,"},
	 {developer, "and I found in my chemistry notebook what seems to be my first idea"},
	 {developer, "because I wrote it before the one that led to the current story,"},
	 {developer, "and so it was that you left a bad review on the BURGER RESTAURANT"},
	 {developer, "and they put you in BURGER prison for doing that,"},
	 {developer, "so the game would be escaping the prison."},
	 {developer, "I'm glad I wrote that down because I completely forgot that."}}
	{{developer, "This document that I used to plan this game says that this game is supposed to take place in..."},
	 {NULL, "\"GENERIC FANTASY LAND\""},
	 {developer, "cause I'm pretty sure it was supposed to be generic but with BURGERs,"},
	 {developer, "but that was boring so I changed it to a cool amazing innovative fantasty land instead."},
	 {NULL, "(well at least I hope it's cool amazing and innovative)"},
	 {developer, "Idk what this place is called now, it's just the earth but fictional"},
	 {developer, "so yea just the earth."}}
	//burger all caps lore
	//kept thinking michelin is named jim
	//viola after fire fight
	//so nice seeing game systems evolve
	//ascii art time
	{{developer, "Have you noticed how nobody actually says your name?"},
	 {developer, "They all say \"kiddo\", \"kid\", \"mister\", etc."},
	 {developer, "It's a very clever way of not having to modify any dialogue to match the name!"}};
	{{developer, "A duration of 2 for enemy freezing effects (like the enemy is the one doing the affecting) is really the sweet spot."},
	 {developer, "1 would be too short and meaningless but 3 might get annoying,"},
	 {developer, "So 2 is good for general purposes."},
	 {developer, "I mean probably I haven't actually tested my game as of writing this :P"}}*/

	NPC* gymbro = new NPC("GYM BRO", "JIM NASIUM", "Obsessed with being in peak physique, there's scarcely a moment when he isn't seen in the gym.\nHe isn't a shrimp, just to clarify.", desertgymfixed, 25);
	gymbro->addGymDialogue("YYYEEEEEEEEEEAAAAAAAAAAAAAHHHHHHHHHHHHHHHHH WEIGHT LIFTING!!!!!!!!!!!!!!!!!");
	gymbro->addRejectionDialogue("Sorry dude, I gotta stay on THAT GRIND to get THEM GAINS.");
	gymbro->setGymStart(1); //he will always catch up to your level

	mike->addGymDialogue({{mike, "HA this place is great for practicing my throwing range!"}, {gymbro, "PSST!"}, {gymbro, "Hey, kid."}, {self, "Yeah?"}, {gymbro, "Can you please hurry and RECRUIT this guy back?"}, {gymbro, "He's freaking scary."}});
	mike->addGymDialogue({{mike, "Check out how far I can throw this dynamite!"}, {mike, "HAHAHA!"}});

	Attack* forkthrow = new Attack("FORK THROW", "threw a fork at", false, 0, 1, 0, 1, 1, 1);
	Attack* pickthrow = new Attack("PICKAXE THROW", "threw a pickaxe at", false, 0, 1, 2, 1, 1, 1);
	Attack* knifethrow = new Attack("KNIFE THROW", "threw a knife at", false, 0, 1, 1, 1, 1, 1);
	Attack* coverthrow = new Attack("HEAVY FRISBEE", "threw a heavy manhole cover at", false, 0, 80, 0, 1, 1, 1);

	Item* telescope = new InfoItem("TELESCOPE", "A large, robust, telescope for observing space.", "You looked through the telescope. You see an orbital office building!", tentlab);
	telescope->setDenial("The TELESCOPE is actually one of those observatory telescopes, so not the kind you can carry around.");
	
	Item* fork = new ManholeItem("FORK","\"An implement with two or more prongs used for lifting food to the mouth or holding it when cutting.\"\n- Oxford Languages", forestfork, forkthrow);
	Item* pickaxe = new ManholeItem("PICKAXE","\"A tool consisting of a long handle set at right angles in the middle of a curved iron or steel bar with a point at one end and a chisel edge or point at the other, used for breaking up hard ground or rock.\"\n- Oxford Languages", mineshaft2, pickthrow);
	Item* knife = new ManholeItem("KNIFE", "\"An instrument composed of a blade fixed into a handle, used for cutting or as a weapon.\"\n- Oxford Languages", volcano5, knifethrow);

	Item* mhcover1 = new ManholeItem("MANHOLE COVER", "A heavy metal cover to the sewers. You could probably use this like a frisbee.", volcano2, coverthrow, sewerentrance1, DOWN);
	Item* mhcover2 = new ManholeItem("MANHOLE COVER", "A heavy metal cover to the sewers. You could probably use this like a frisbee.", volcano6, coverthrow, sewer2, DOWN);

	Item* memcover = new ManholeItem("MANHOLE COVER", "A heavy metal cover to the sewers. You could probably use this like a frisbee.", leftstreet5, coverthrow, limbo, DOWN);
	memcover->setTakable(false);
	memcover->setDenial("The MANHOLE COVER is completely sealed. You need to wait until a future DLC to be able to open it!");

	Item* switch1 = new ConveyorSwitch("SWITCH", "A metal switch sticking out of the ground, meant for controlling the factory assembly lines.", switchroom);
	Item* switch2 = new ConveyorSwitch("SWITCH", "A metal switch sticking out of the ground, meant for controlling the factory assembly lines.", switchroom2);
	ConveyorSwitch* switchhelper1 = (ConveyorSwitch*)switch1;
	ConveyorSwitch* switchhelper2 = (ConveyorSwitch*)switch2;
	switchhelper1->setConveyor(conveyor1);
	switchhelper1->setConveyor(conveyor2);
	switchhelper1->setConveyor(conveyor3);
	switchhelper1->setConveyor(conveyor4);
	switchhelper1->setConveyor(conveyor5);
	switchhelper2->setConveyor(conveyor1);
	switchhelper2->setConveyor(conveyor2);
	switchhelper2->setConveyor(conveyor3);
	switchhelper2->setConveyor(conveyor4);
	switchhelper2->setConveyor(conveyor5);

	//{{NULL, "The BURGER MENACE has been subdued."},
	//{NULL, "All around the world, all the BURGERs fade to ashes."},
	//{NULL, "The BURGER RESTAUANT has been left to rot in the depths."},
	//{NULL, "The BURGER corporation has been deprived of its leadership..."},
	//{NULL, "and its source of revenue."},
	//{NULL, "BURGER's grasp on the world has been completely dissolved."},
	//{NULL, "\n\t<<< BURGER QUEST COMPLETE! >>>"
	//       "\n\t<<<    ENDING ACHIEVED:    >>>"
	//       "\n\t<<<      BURG NO MORE      >>>"},
	
	//{NULL, "\nHENRY JERRY - \"Well, in the end, this has truly been a BURGER QUEST too.\""}, //I have to do the NULL "fake speaker" trick here so I can have an extra newline at the start
	//{self, "Say that again?"},

	//{NULL, "\nThank you so much for playing BURGER QUEST 2: ELECTRIC BOOGALOO all the way to the end!"},
	//{NULL, "I hope this has been an enjoyable and meaningful experience."},
	//{NULL, "May we meet again on the road ahead!"},
	//{NULL, "- Tomas"}}

	//{{hj, "Well I'm gonna go home I guess."},
	//{self, "Alright cya."},
	//{NULL, "HENRY JERRY went back to his house."},
	//{self, "..."},
	//{self, "Wait how are we supposed to get back?"}}

	NPC* merchant = new NPC("MERCHANT", "MERRO", "Merchant and owner of the desert store. He really wants your monies.", limbo, 12);
	merchant->setDialogue("Welcome, my friend, to my store.");
	merchant->addRejectionDialogue("No I want to sell things.");

	Item* skateboard = new InfoItem("SKATEBOARD", "It's a pretty cool skateboard for doing cool skateboard things.", "You did a kickflip. Very cool.", limbo);
	skateboard->setTakable();
	desertshopfixed->setStock(skateboard, 1, 100, {{merchant, "Thank you for your monies."}});

	Item* sunscreen = new KeyItem("SUNSCREEN", "Bottle of sunscreen for resisting the heat.", {{NULL, "You applied the sunscreen."}, {NULL, "No amount of radiation should bother you now!"}}, limbo, HEAT, true);
	KeyItem* _sunscreen = (KeyItem*)sunscreen;
	_sunscreen->setTarget(volcanoentrance);
	desertshopfixed->setStock(sunscreen, 1, 20, {{merchant, "Thank you for your monies."}});

	Item* vitaminb = new SpItem("VITAMIN B", "A small supplement bottle of pure Vitamin B. (Restores 15 SP)", limbo, 15);
	desertshopfixed->setStock(vitaminb, 2147483647, 20, {{merchant, "Thank you for your monies."}});
	Item* iron = new SpItem("IRON", "A small bottle of iron supplement. (Restores 30 SP)", limbo, 30);
	desertshopfixed->setStock(iron, 2147483647, 40, {{merchant, "Thank you for your monies."}});
	Item* magnesium = new SpItem("MAGNESIUM", "A small bottle of magnesium supplement. (Restores all SP)", limbo, 2147483647);
	desertshopfixed->setStock(magnesium, 2147483647, 100, {{merchant, "Thank you for your monies."}});

	Item* reviveroot = new ReviveItem("REVIVE ROOT", "A small root vegetable known for completely healing any injury. (Recapacitates teammates)", limbo, 999);
	desertshopfixed->setStock(reviveroot, 2147483647, 300, {{merchant, "Thank you for your monies."}});
	
	Item* rotrevroot = new ReviveItem("ROTTEN REVIVE ROOT", "A spoiled revive root, still capable of healing, though not to the extent of its fresh version.\nIt looks juicy and squishy and nasty. (Recapacitates teammates with 10 HP)", burgstore, 10);

	Item* crowbar = new MaterialItem("MYSTERY EGG", "This egg is containing a special item and will hatch in a future update.", limbo);
	Item* memorycrowbar1 = new MaterialItem("MYSTERY EGG", "This egg is containing a special item and will hatch in a future update.", limbo);
	Item* memorycrowbar2 = new MaterialItem("MYSTERY EGG", "This egg is containing a special item and will hatch in a future update.", limbo);

	Item* burgchest = new TreasureItem("TREASURE CHEST", "A big treasure chest, possibly full of treasure.", burgboiler, 0, {crowbar});
	burgchest->setDenial("The treasure chest is really heavy! You need to USE it to open it, instead.");

	NPC* hotdogguy = new NPC("HOT DOG VENDOR", "HARRY", "Manager of the lucrative BURGERSBURG hot dog stand.", coolstreet3, 14);
	hotdogguy->addConversation({{self, "Hey!"},
								{self, "Got any grapes?"},
								{hotdogguy, "No, I just sell hot dogs."},
								{hotdogguy, "But they're hot and they're fresh and they're all homemade!"},
								{hotdogguy, "You want a frank?"},
								{self, "No thank."}});
	hotdogguy->setDialogue("Ayyy what's up man you want a hot dog?");
	hotdogguy->addRejectionDialogue("Sorry no, this hot dog stand is my life's calling.");

	Item* hotdog = new HpItem("HOT DOG", "A classic urban hot dog with mustard. (heals 2 HP)", limbo, 2);
	coolstreet3->setStock(hotdog, 2147483647, 2, { {hotdogguy, "Ayy thanks for the purchase enjoy your hot dog!"} });

	//it's TECH DEMO MAN!
	NPC* techdemoman = new NPC("", "TECH DEMO MAN", "Mechanical superhero for testing the game in the past!\n\"Since the game is unbalanced, I'll just make an even more unbalanced NPC!\"\n-Tomas", tenthome, 0, Stats(200000, 200000, 20, 20000, 10, 15, 20));
	techdemoman->setDialogue("HELLO THERE THIS IS MY DIALOGUE!");
	techdemoman->addRejectionDialogue("NO THIS IS NO LONGER THE TECH DEMO.");

	NPC* fr = new NPC("FATHER", "DAMIAN", "The priest in charge of the hidden church.", burgchurch, 2);
	Conversation frcon = {{self, "Are you her husband?"},
						  {fr, "No?"},
						  {self, "Oh but like your title says 'father' and you're in the same room as that mother."},
						  {fr, "Well, I'm a priest."},
						  {fr, "I act as a sort of spiritual father to this community."},
						  {self, "I see."},
						  {fr, "I take it you're not familiar with churches?"},
						  {self, "Well I have one back home but the priest went missing a while ago."},
						  {fr, "I see."},
						  {fr, "So, am I correct in assuming you don't know about Jesus, then?"},
						  {self, "Yeah no clue."},
						  {fr, "Well, would you like to know?"},
						  {NULL, "Would you like to hear about Jesus? (YES or NO)"}};
	shared_ptr<Conversation> fryes = make_shared<Conversation>(Conversation(
		{{fr, "Well..."},
		 {fr, "All of us have sinned,"},
		 {fr, "and this sin keeps us from Heaven,"},
		 {fr, "so we're destined for Hell."},
		 {fr, "But,"},
		 {fr, "because God loves us so much,"},
		 {fr, "He wanted to save us from that."},
		 {fr, "So, He sent His Son, Jesus, down to earth,"},
		 {fr, "where He died on the cross and took our punishment for us."},
		 {fr, "..."},
		 {fr, "Then, on the third day, He resurrected from the dead,"},
		 {fr, "and in doing so He defeated death and sin."},
		 {fr, "So now, through His sacrifice and resurrection,"},
		 {fr, "all who believe in Him and follow Him through His grace get to share in His victory,"},
		 {fr, "and will have eternal life with Him in Heaven."},
		 //{self, "I see."},
		 {fr, "Well, thanks for hearing me out."},
		 {fr, "I'd encourage you to think about that."}}));
	shared_ptr<Conversation> frno = make_shared<Conversation>(Conversation({{fr, "Well, if you ever want to know, just ASK. I'll be right here."}}));
	frcon.branch1 = {"YES", fryes};
	frcon.branch2 = {"NO", frno};
	shared_ptr<Conversation> fragain = make_shared<Conversation>(Conversation({{fr, "So are you curious about Jesus, then?"}, {NULL, "Would you like to hear about Jesus? (YES or NO)"}}));
	fragain->branch1 = {"YES", fryes};
	fragain->branch2 = {"NO", frno};
	frno->relay = {fr, fragain};
	relaysH.push_back(fragain); //make sure this one conversation doesn't expire
	fr->addConversation(frcon);
	Conversation frrej1 = {{self, "Hey do you want to get a BURGER with me?"},
							{fr, "a BURGER?"}, //have to make the a lowercase so it doesn't look like he's shouting "A BURGER?!?!?!!?!?"
							{fr, "That organization isn't what it advertises itself to be."},
							{fr, "They tempt people into idolizing fast food,"},
							{fr, "then ultimately leave them spiritually dead once they finally get it."},
							{fr, "It might bring temporary pleasure, but the pleasure is nothing compared to what it costs."},
							{fr, "Don't allow BURGERs to fool you too."}};
	frrej1.skipcondition = {JILLYQUEST};
	shared_ptr<Conversation> frrej2 = make_shared<Conversation>(Conversation({{self, "Hey do you want to help me save her kid?"},
							{fr, "I would like to, but my vocation is to guide this community;"},
							{fr, "I haven't been given any fighting ability."},
							{fr, "My prayers are with you, though!"}}));
	frrej1.alt = frrej2;
	frrej2->skipcondition = {JILLYSAVED};
	shared_ptr<Conversation> frrej3 = make_shared<Conversation>(Conversation({{self, "Hey now that I saved her kid do you want to get a BURGER with me?"},
							{fr, "You should know, BURGERs aren't what they're advertised to be."},
							{fr, "They tempt people into idolizing fast food,"},
							{fr, "then ultimately leave them spiritually dead once they finally get it."},
							{fr, "It might bring temporary pleasure, but the pleasure is nothing compared to what it costs."},
							{fr, "Don't allow BURGERs to fool you too."}}));
	frrej2->alt = frrej3;
	frrej3->skipcondition = {TEMPLEQUEST};
	shared_ptr<Conversation> frrej4 = make_shared<Conversation>(Conversation({{self, "Hey do you want to help me destroy BURGERs?"},
							{fr, "I would like to, but my vocation is to guide this community;"},
							{fr, "I haven't been given any fighting ability."},
							{fr, "My prayers are with you, though!"}}));
	frrej3->alt = frrej4;
	frrej4->skipcondition = {BURGERMENDEF};
	shared_ptr<Conversation> frrej5 = make_shared<Conversation>(Conversation({{self, "Hey wanna join my team?"},
							{fr, "Sorry, my vocation is to guide this community."},
							{fr, "I don't have any time to be adventuring."},
							{fr, "Safe travels! My prayers are with you!"}}));
	frrej4->alt = frrej5;
	fr->addRejectionDialogue(frrej1);
	fr->setDialogue({{fr, "Well, that's all I've got to say."}, {fr, "I'd encourage you to think about it."}});
	
	Item* greenkey = new KeyItem("GREEN KEY", "A big key in a similar shade of green to the volcanic sewers' accents.", {{NULL, "You put the GREEN KEY in the keyhole."}, {NULL, "The doorway has been unlocked!"}}, limbo, LOCK);
	NPC* jim = new NPC("", "JIM", "A self-aware shrimp wearing a dark hood, tired of his shrimple brother's shenanigans.", shrimproof, 0, Stats(50, 10, 10, 35, 15, 10, 9));
	jim->addConversation({{jim, "I am the self-aware Jim."},
						  {jim, "I wish my shrimple brothers would be self-aware as well."},
						  {jim, "Have you encountered them?"},
						  {self, "A few times."},
						  {jim, "Well I apolagize on their behalf if they've been a nusicence."},
						  {jim, "I guard the key to our place of origin."},
						  {jim, "I was told to wait for someone with blond anime hair and a scarf,"},
						  {jim, "and you seem to match the description."},
						  {jim, "Here you go."},
						  {self, "Oh thanks."}});
	jim->addConversation({{self, "So, you live here?"},
						  {jim, "Yeah I'm the landlord."},
						  {self, "Cool."}});
	jim->setDialogue({{jim, "..."}, {self, "..."}});
	jim->addRejectionDialogue("I have to manage my apartment.");
	jim->setGift(greenkey);

	NPC* qthief = new NPC("", "QUANTUMN THIEF", "Citizen of BURGERSBURG whose description you will never read in game as long as I programmed it correctly.", darkalley, 25, Stats(10, 0, 20, 0, 20, 25, 9));
	qthief->setQuantumn();
	qthief->blockExit(OUT, ENEMY, "blocked by the QUANTUMN THIEF");
	
	NPC* skeleseller = new NPC("SKELETON", "KELVIN", "He appears to be a skeleton on the floor.", kaboomroom, 5);
	skeleseller->setDialogue("Well hello there! Could I interest you in some explosives?");
	skeleseller->addRejectionDialogue("Sorry fella, my ligaments are long gone. No walking for me!");
	Attack* pdynamite = new Attack("DYNAMITE", "threw a stick of dynamite at", false, 0, 20, 20, 1, 1, 1);
	Item* dynamite = new KeyItem("DYNAMITE", "Explosives for exploding stuff.", {{NULL, "You threw the dynamite at the the rubble."}, {NULL, "*KABOOM!*"}, {NULL, "The exit has been unblocked!"}}, limbo, RUBBLE, true, pdynamite);
	kaboomroom->setStock(dynamite, 2147483647, 5, {{skeleseller, "Do you need some explosives? Here you go! I assume you know what you're doing..."}});

	NPC* bob = new NPC("", "BOB", "A small child wearing a newsboy cap. He has a hard time making human friends and prefers plants.", limbo, 0);
	Conversation bobcon = {{bob, "Oh my poor friend Cacty!"},
						   {bob, "He's so dehydrated!"},
						   {bob, "If only the underground spring never dried up!"},
						   {self, "The what?"},
						   {bob, "The underground spring that fueled the oasis!"},
						   {bob, "Clearly there's something wrong with it because the oasis is dry!"}};
	bobcon.skipcondition = {VALVEUSED};
	bob->addConversation(bobcon);
	bob->setDialogue({{bob, "Oh my poor friend Cacty!"},
					  {bob, "He's so dehydrated!"}});
	bob->addRejectionDialogue("No my mama says I'm too young to go adventuring.");
	
	NPC* franklin = new NPC("TOWN ELDER", "FRANKLIN", "The town elder of the desert town, and an avid tennis player. He has a big mustache.", limbo, 5);
	franklin->addConversation({{franklin, "Say, I had a very good view of your fight with Viola."},
							   {franklin, "I simply must thank you for saving our humble town!"},
							   {franklin, "Here, have this desert delicacy!"},
							   {self, "Oh thanks!"}});
	Item* desertdelicacy = new HpItem("DESERT DELICACY", "A very nice desert pie, featuring rare desert fruit. (heals all HP)", limbo, 999);
	franklin->setGift(desertdelicacy);
	franklin->addConversation({{franklin, "I don't like sand."},
							   {franklin, "It's coarse and rough and irritating and it gets everywhere."},
							   {franklin, "You look like you're from the forest, is that right?"},
							   {self, "Yep that's right."},
							   {franklin, "Well believe it or not, this here desert used to be just like your forest."},
							   {self, "Really?"},
							   {franklin, "Yep."},
							   {franklin, "As I grew up, though, all the soil dried up and all the plants died."},
							   {franklin, "Now all we have left are the plants at the oasis..."}});
	franklin->addConversation({{self, "So like does this town have a name?"},
							   {franklin, "..."},
							   {franklin, "Not really."},
							   {self, "How about Practical Pueblo?"},
							   {franklin, "I'll bring it up with the HOA."}});
	franklin->addRejectionDialogue("I must watch over the town, I'm afraid I haven't got time for adventuring.");
	franklin->setDialogue("I'm happy to see this town back in its lively state!");

	/*NPC* olivia = new NPC("", "OLIVIA", "Lady who lives in this house.", limbo, 7);
	olivia->setDialogue({})*/

	//*...*
	//____ is frozen in the air.

	Item* valve = new WorldChangeItem("WATER VALVE", "A valve on the pipe managing the spring's water. It's currently redirecting the water away from the oasis.", minespring, {{NULL, "You turn the valve counterclockwise."}, {NULL, "The spring's water is now flowing to the oasis!"}});
	WorldChange& valvechanges = ((WorldChangeItem*)valve)->getChanges();
	valvechanges.worldcon = VALVEUSED;
	valvechanges.linkedConversations.push({bob, {{bob, "The oasis has filled up with water!"}, {bob, "Do you know how this is?"}, {self, "Yeah I just turned some valve underground."}, {bob, "Remarkable!"}}});
	valvechanges.linkedConversations.push({cacty, {{NULL, "CACTY - *grateful cactus noises*"}, {self, "What how did you know I did that?"}, {NULL, "CACTY - *explanatory cactus noises*"}, {self, "Oh I see."}}});
	valvechanges.linkedDialogue.push({bob, {{bob, "Oh I'm so happy my friend Cacty is hydrated now!"}}});
	valvechanges.linkedDialogue.push({cacty, {{NULL, "CACTY - *happy cactus noises*"}}});
	valvechanges.linkedDescriptions.push({cacty, "Sharp cactus, hydrated back to health. He has a remarkable pink flower and rooty legs to move around."});
	valvechanges.roomChanges.push({oasis, "in the town oasis, now fully restored! Some signs of greenery are starting to return."});
	valvechanges.recruitLinks.push(cacty);
	//valvechanges.roomChanges.push({basement, "something about how BURGER production has ceased"});

	Attack* cooldown = new Attack("COOL DOWN", "poured sand coolant on", false, 0, 0, 0, 1, 1, 1);
	Effect* cooled = new Effect("COOLED", 6, 0, 0, 1, 1, 1, 1, 0.75);
	cooldown->addEffect(cooled);
	Attack* polevault = new Attack("POLE VAULT", "pole vaulted into the air", false, 0, 0, 0, 0, 0, 0); //dodge move!
	Effect* polevaulting = new Effect("POLE VAULTING", 0);
	polevaulting->remove = true;
	polevault->selfeffect = polevaulting;
	Attack* mineram = new Attack("MINECART RAM", "rammed", true, 0, 20, 0, 1, 1, 1); //decent attack but doesn't gain any sp
	mineram->afterdesc = " with the SPARE MINECART";

	Item* sandcoolant = new KeyItem("SAND COOLANT", "Bottle of coolant handy for cooling down sand of the scorching variety.", {{NULL, "You poured some sand coolant onto the scorching sands."}, {NULL, "The sands cooled down!"}}, deserttempleentrance, SAND, false, cooldown);
	Item* powerpole = new MovementItem("POLE VAULT", "Very long stick, useful for travelling over chasms.", {{NULL, "You used the pole to go over the chasm!"}}, desertpole, CHASM, true, polevault);
	Item* minecart = new MovementItem("SPARE MINECART", "A spare minecart unclamped from the tracks.", {{NULL, "You hopped into the SPARE MINECART and went to the other side of the track."}}, mineshaftside, TRACK, false, mineram);

	Item* minecart1 = new MovementItem("WEST MINECART", "Cart used in mining and going over tracks.", {{NULL, "You hopped into the WEST MINECART and went to the other side of the track."}}, mineshaft3, TRACK, false);
	Item* minecart2 = new MovementItem("EAST MINECART", "Cart used in mining and going over tracks.", {{NULL, "You hopped into the EAST MINECART and went to the other side of the track."}}, mineshaftside, TRACK, false);
	Item* minecart3 = new MovementItem("NORTH MINECART", "Cart used in mining and going over tracks.", {{NULL, "You hopped into the NORTH MINECART and went to the other side of the track."}}, mineshaft2, TRACK, false);
	Item* minecart4 = new MovementItem("SOUTH MINECART", "Cart used in mining and going over tracks.", {{NULL, "You hopped into the SOUTH MINECART and went to the other side of the track."}}, mineshaft3, TRACK, false);
	minecart1->setTakable(false);
	minecart2->setTakable(false);
	minecart3->setTakable(false);
	minecart4->setTakable(false);
	minecart1->setDenial("This minecart is clamped onto the minecart tracks!");
	minecart2->setDenial("This minecart is clamped onto the minecart tracks!");
	minecart3->setDenial("This minecart is clamped onto the minecart tracks!");
	minecart4->setDenial("This minecart is clamped onto the minecart tracks!");

	Item* detonator = new PaverItem("DETONATOR", "A device wired to some explosives up ahead.", {{NULL, "You pushed down onto the DETONATOR lever."}, {NULL, "*KABOOM!*"}, {NULL, "An exit SOUTH has been opened!"}}, mineshortcut, mineshortcut, SOUTH, mineshaftside);

	Item* downbutton = new WorldChangeItem("DOWN BUTTON", "Restricted elevator button for going alllllll the way down.\nIt's in the same style as the BURGER RESTAURANT elevator buttons.", burgsafe, {{NULL, "There is nowhere to put the DOWN BUTTON!"}, {NULL, "..."}, {NULL, "You shove the DOWN BUTTON into the wall."}, {NULL, "The elevator can go TO THE BOTTOM now!"}});
	downbutton->setTakable(true);
	WorldChangeItem* db = (WorldChangeItem*)downbutton;
	db->setUsableRooms({elevator, elevatortop});
	WorldChange& dbchanges =db->getChanges();
	dbchanges.exitPavings.push(make_tuple(elevator, elevatorbottom, TO_THE_BOTTOM, TO_GROUND_LEVEL));
	dbchanges.exitPavings.push(make_tuple(elevatortop, elevatorbottom, TO_THE_BOTTOM, TO_THE_TOP));
	
	Attack* forkliftup = new Attack("FORKLIFT UP", "forklifted", true, 15, 0, 0, 1, 1, 1); //uppercut preview basically
	forkliftup->afterdesc = " into the air";
	Effect* forklifted = new Effect("FORKLIFTED", 1);
	forklifted->remove = true;
	forkliftup->addEffect(forklifted);

	Item* forklift = new KeyItem("FORKLIFT", "Cool thing for lifting stuff such as collapsed roof material.", {{NULL, "You used the FORKLIFT to move the collapsed ceiling material out of the way."}}, heavymachineryroom, STUFF, false, forkliftup);
	Item* scissorlift = new KeyItem("SCISSOR LIFT", "Cool thing for going UP and DOWN straight horizontal directions.", {{NULL, "You toggled the extension of the SCISSOR LIFT."}}, heavymachineryroom, HIGH, false);
	scissorlift->setDropToUse(true);

	Attack* scissorup = new Attack("SCISSOR LIFT", "scissor lifted into the air", false, 0, 0, 0, 0, 0, 0); //dodge move! (with slightly longer duration than pole vault)
	Effect* scissorlifted = new Effect("SCISSOR LIFTED", 1);
	scissorlifted->remove = true;
	scissorup->selfeffect = scissorlifted;

	Item* scissorliftsw = new KeyItem("SCISSOR LIFT", "Cool thing for going UP and DOWN straight horizontal directions.", {{NULL, "You toggled the extension of the SCISSOR LIFT."}}, factoryroofsw, HIGH, false);
	Item* scissorliftnw = new KeyItem("SCISSOR LIFT", "Cool thing for going UP and DOWN straight horizontal directions.", {{NULL, "You toggled the extension of the SCISSOR LIFT."}}, factoryroofnw, HIGH, false);
	Item* scissorliftse = new KeyItem("SCISSOR LIFT", "Cool thing for going UP and DOWN straight horizontal directions.", {{NULL, "You toggled the extension of the SCISSOR LIFT."}}, factoryroofse, HIGH, false);
	Item* scissorliftne = new KeyItem("SCISSOR LIFT", "Cool thing for going UP and DOWN straight horizontal directions.", {{NULL, "You toggled the extension of the SCISSOR LIFT."}}, factoryroofne, HIGH, false);
	scissorliftsw->setTakable(false);
	scissorliftnw->setTakable(false);
	scissorliftse->setTakable(false);
	scissorliftne->setTakable(false);

	Item* rbtree = new InfoItem("RED-BLACK TREE", "A very cool tree with red and black fruits, rebalancing itself as it grows.", "You tried to grab one of the fruits but the tree rebalanced itself and you fell.", mountainpeak);
	rbtree->setDenial("And how do you plan to TAKE an entire tree, exactly?");

	Item* controlpanel1 = new WorldChangeItem("CONTROL PANEL", "A huge array of buttons for controlling the factory.\nThankfully, they're all neatly labelled.", controlroom1, {{NULL, "You pull the drainage lever."}, {NULL, "You see the lava level lowering outside!"}});
	WorldChange& panelchanges1 = ((WorldChangeItem*)controlpanel1)->getChanges();
	panelchanges1.exitUnblocks.push({volcano2, NORTH});
	panelchanges1.exitUnblocks.push({volcano2, DOWN});
	panelchanges1.worldcon = SWITCHUSED;

	Item* controlpanel2 = new WorldChangeItem("CONTROL PANEL", "A huge array of buttons for controlling the factory.\nThankfully, they're all neatly labelled.", controlroom2, {{NULL, "You pull the drainage lever."}, {NULL, "You see the lava level lowering outside!"}});
	WorldChange& panelchanges2 = ((WorldChangeItem*)controlpanel2)->getChanges();
	panelchanges2.exitUnblocks.push({sewerentrance1, DOWN});
	panelchanges2.exitUnblocks.push({volcano4, NORTHEAST});
	
	Item* controlpanel3 = new WorldChangeItem("CONTROL PANEL", "A huge array of buttons for controlling the factory.\nThankfully, they're all neatly labelled.", controlroom3, {{NULL, "You pull the drainage lever."}, {NULL, "You see the lava level lowering outside!"}});
	WorldChange& panelchanges3 = ((WorldChangeItem*)controlpanel3)->getChanges();
	panelchanges3.exitUnblocks.push({sewerentrance2, DOWN});
	panelchanges3.exitUnblocks.push({volcano6, NORTHWEST});
	panelchanges3.exitUnblocks.push({volcano6, DOWN});
	
	Item* masterswitch = new WorldChangeItem("MASTER DRAINAGE BUTTON", "Huge red button, which will fully drain the lava from the highlands.", sewerplant, {{NULL, "You jump onto the MASTER DRAINAGE BUTTON."}, {NULL, "The sewers rumble."}, {NULL, "*sewer rumbling noises*"}, {NULL, "The lava below has become unstuck!"}, {NULL, "The lava is flowing properly now; the lava sea is at its natural level."}});
	WorldChange& masterschanges = ((WorldChangeItem*)masterswitch)->getChanges();
	masterschanges.exitUnblocks.push({volcano7, NORTH});
	masterschanges.linkedConversations.push({factelder, {{factelder, "So you've drained the lava sea!"}, {self, "Yep."}, {factelder, "Very good! Very good."}, {factelder, "..."}, {factelder, "Hm..."}, {factelder, "Don't you think their intense desire for a BURGER seems a bit strange?"}, {factelder, "Unnatural even?"}, {factelder, "I would advise you to be wary."}}});
	masterschanges.linkedDialogue.push({factelder, {{factelder, "Well done with the lava!"}, {factelder, "Stay safe, child."}}});
	masterschanges.worldcon = LAVADRAINED;

	NPC* panepeople = new NPC("", "TWO GUYS CARRYING PANE OF GLASS", "They are two guys who are carrying a pane of glass, as you typically see on the street.", rightstreet4, 7);
	panepeople->addLinkedRoom(rightstreet4, "at the glass store. They have very tough windows, so thieves have to steal the glass through the door.");
	panepeople->addDefeatRoom(panepeople, limbo);
	panepeople->setTalkMakeChanges();
	panepeople->setDialogue({{NULL, "You walk up to the guys to talk to them."}, {NULL, "GLASS STORE OWNER - \"HEY!\""}, {NULL, "GLASS STORE OWNER - \"STOP STEALING MY GLASS!\""}, {NULL, "GUY 1 - \"Hahahaha! You'll never catch us!\""}, {NULL, "The two guys run away with the pane of glass."}, {self, "..."}});
	panepeople->addRejectionDialogue({{NULL, "You walk up to the guys to recruit them."}, {NULL, "GLASS STORE OWNER - \"HEY!\""}, {NULL, "GLASS STORE OWNER - \"STOP STEALING MY GLASS!\""}, {NULL, "GUY 1 - \"Hahahaha! You'll never catch us!\""}, {NULL, "The two guys run away with the pane of glass."}, {self, "..."}});

	NPC* banker = new NPC("SHADY BANKER", "SHAUN", "He is a very reliable banker with a constant smile who has set up this bank in this shady alley.", bankalley, 16);
	banker->addConversation({{banker, "Hiii, frieeend."}, {banker, "Feel free to leave your monies heeeere with meeee."}, {banker, "They will be saaaafe and sooound..."}});
	banker->setDialogue({{banker, "Hiii, frieeend."}, {banker, "Are you looking to manage your accooount?"}});
	banker->addRejectionDialogue({{banker, "No, friend."}, {banker, "I'm a banker, not an adventurer."}, {banker, "How can I take care of everyone's monies if I leeave?"}});
	banker->setBanker();

	NPC* firefighter = new NPC("FIREFIGHTER", "DANIEL", "Firefighter of BURGERSBURG who has lost hope and is lying on the floor.", firedepartment, 19);
	firefighter->addConversation({{firefighter, "Hm?"}, 
								  {self, "Are you a firefighter?"},
								  {firefighter, "Yep."},
								  {self, "There was a fire like right outside here."},
								  {firefighter, "Eh, who cares."},
								  {firefighter, "No matter how many fires you put out in this city,"},
								  {firefighter, "five more just pop up in its place."},
								  {self, ":/"},
								  {firefighter, "Feel free to use the hose,"},
								  {firefighter, "but I myself don't care enough to try anymore."}});
	firefighter->setDialogue("What's even the point of anything?");
	firefighter->addRejectionDialogue({{firefighter, "For what?"}, {firefighter, "I can't do anything to help you."}});
		
	Attack* hosedown = new Attack("HOSE DOWN", "hosed down", false, 0, 20, 15, 1, 1, 1);
	hosedown->afterdesc = " with the fire hose";
	Effect* soaked = new Effect("SOAKED", 3, 0, 0, 1, 1, 1, 1, 0.5);
	hosedown->addEffect(soaked);

	Item* firehose = new HoseItem("FIRE HOSE", "A hose for spraying water at fires, powered by the water tank back at the fire station.", {{NULL, "You sprayed water at the fire."}, {NULL, "The fire has been put out!"}}, firedepartment, FIRE, false, hosedown);
	HoseItem* hose = (HoseItem*)firehose;
	hose->addBlocker(BURGERSBURG, SOUTH, "You tried to go SOUTH but got stopped by your fully extended FIRE HOSE.", NULL);
	hose->addBlocker(elevator, TO_THE_TOP, "You pressed the button to go TO THE TOP but the door reopened after closing on your FIRE HOSE.", "You pressed the button to go TO THE TOP but the door reopened after closing on the FIRE HOSE on the ground.");
	hose->addBlocker(elevator, TO_THE_BOTTOM, "You tried to go TO THE BOTTOM but the door reopened after closing on your FIRE HOSE.", "You tried to go TO THE BOTTOM but the door reopened after closing on the FIRE HOSE on the ground.");
	hose->addBlocker(ceoelevator1, TO_FLOOR_2, "You pressed the button to go TO FLOOR 2 but the door reopened after closing on your FIRE HOSE.", "You pressed the button to go TO FLOOR 2 but the door reopened after closing on the FIRE HOSE on the ground.");
	hose->addBlocker(ceoelevator1, TO_THE_TOP, "You pressed the button to go TO THE TOP but the door reopened after closing on your FIRE HOSE.", "You pressed the button to go TO THE TOP but the door reopened after closing on the FIRE HOSE on the ground.");
	hose->addBlocker(ceoelevator2, TO_FLOOR_1, "You pressed the button to go TO FLOOR 1 but the door reopened after closing on your FIRE HOSE.", "You pressed the button to go TO FLOOR 1 but the door reopened after closing on the FIRE HOSE on the ground.");
	hose->addBlocker(ceoelevator2, TO_THE_TOP, "You pressed the button to go TO THE TOP but the door reopened after closing on your FIRE HOSE.", "You pressed the button to go TO THE TOP but the door reopened after closing on the FIRE HOSE on the ground.");
	hose->setStationBlock("You started going into the tunnels but got pulled off your lobster by your fully extended FIRE HOSE!");

	WorldChange hsdropceoele1; //changes if you drop the hose in the floor 1 ceo elevator
	hsdropceoele1.exitBlocks.push(make_tuple(ceolobby2, IN_ELEVATOR, FIRE, "closed because the hose you dropped above isn't letting it move.")); //fire exit because you just dropped the thing that can unblock that type
	hose->addDropChange(ceoelevator1, hsdropceoele1);
	WorldChange hsdropceoele2; //changes if you drop the hose in the floor 2 ceo elevator
	hsdropceoele1.exitBlocks.push(make_tuple(ceolobby, IN_ELEVATOR, FIRE, "closed because the hose you dropped below isn't letting it move."));
	hose->addDropChange(ceoelevator2, hsdropceoele2);
	WorldChange hstakeceoele1; //changes if you take the hose from the floor 1 ceo elevator
	hstakeceoele1.exitUnblocks.push({ceolobby2, IN_ELEVATOR});
	hose->addDropChange(ceoelevator1, hstakeceoele1);
	WorldChange hstakeceoele2; //changes if you take the hose from the floor 2 ceo elevator
	hstakeceoele2.exitUnblocks.push({ceolobby, IN_ELEVATOR});
	hose->addDropChange(ceoelevator2, hstakeceoele2);
	WorldChange hsdropburgele; //changes if you drop the hose in the restaurant elevator
	hsdropburgele.exitBlocks.push(make_tuple(burgerbasement, IN_ELEVATOR, FIRE, "closed because the hose you dropped above isn't letting it move."));
	hose->addDropChange(elevator, hsdropburgele);
	WorldChange hstakeburgele; //changes if you take the hose from the restaurant elevator
	hstakeburgele.exitUnblocks.push({burgerbasement, IN_ELEVATOR});
	hose->addDropChange(elevator, hstakeburgele);

	NPC* ninjasmith = new NPC("", "NINJA SMITH", "An expert smith and ninja, maker of all the ninja's fine weapons.", ninjaforge, 20, Stats(61, 21, 43, 32, 50, 30, 9));
	Conversation smithconv = {{ninjasmith, "I have made many weapon here."},
		{ninjasmith, "But there is one I wish to make that has eluded me whole life."},
		{ninjasmith, "I have plan to make Kosmic Katana,"},
		{ninjasmith, "so sharp it cut through space itself."},
		{self, "Oh wow."},
		{ninjasmith, "To be so sharp, forging material need to be corn of unihorn."},
		{ninjasmith, "You ever see one?"},
		{self, "no"},
		{ninjasmith, "This is the problem."},
		{ninjasmith, "Unihorn very rare creature."},
		{ninjasmith, "No like to be seen."},
		{ninjasmith, "I hope to find one someday,"},
		{ninjasmith, "make Kosmic Katana."},
		{self, "Well I guess I'll be on the lookout for a unihorn."},
		{ninjasmith, "Thank you very much."}};
	shared_ptr<Conversation> smithconv2 = make_shared<Conversation>(Conversation({{ninjasmith, "I have made many weapon here."},
		 {ninjasmith, "But there is one I wish to make that has eluded me whole life."},
		 {ninjasmith, "I have plan to make Kosmic Katana,"},
		 {ninjasmith, "so sharp it cut through space itself."},
		 {self, "Oh wow."},
		 {ninjasmith, "To be so sharp, forging material need to be corn of unihorn."},
		 {ninjasmith, "You ever see one?"},
		 {self, "Yeah in some basement."},
		 {ninjasmith, "What?"},
		 {ninjasmith, "You have seen one?"},
		 {ninjasmith, "Unihorn no like to be seen."},
		 {ninjasmith, "You are very fortunate to have seen it."},
		 {ninjasmith, "If you happened to get corn of the unihorn,"},
		 {ninjasmith, "Please bring it to forge."},
		 {ninjasmith, "I very much hope to make Kosmic Katana."}}));
	smithconv.skipcondition = {SEENUNIHORN};
	smithconv.alt = smithconv2;
	smithconv2->skipcondition = {MADEKATANA};
	ninjasmith->addConversation(smithconv);
	ninjasmith->setDialogue({{ninjasmith, "If you ever see unihorn, please ask for unihorn corn."}, {ninjasmith, "I very much hope to make Kosmic Katana."}});
	ninjasmith->setTalkOnDefeat();
	ninjasmith->addRejectionDialogue("I no abandon ninja way.");
	ninjasmith->guardItem(ninjaberry);

	NPC* unihorn = new NPC("", "UNIHORN", "A mythical and majestic unihorn, dark bluish with pale hair, restrained by quantumn chains.", burglab, 24, Stats(100, 30, 30, 40, 40, 50, 9));
	unihorn->setDialogue({{NULL, "UNIHORN - *pained neighs*"}});
	unihorn->addRejectionDialogue({{NULL, "UNIHORN - *pained neighs*"}});
	WorldChange seeunihorn; //when you go in the lab you have seen the unihorn
	seeunihorn.worldcon = SEENUNIHORN;
	//this could theoretically replace the ninjafurnace's ninja smith linked dialogue, except you need to see the unihorn to use the furnace so it actually doesn't matter
	seeunihorn.linkedDialogue.push({ninjasmith, {{ninjasmith, "If you ever get unihorn corn,"}, {ninjasmith, "please bring here to forge."}, {ninjasmith, "I very much hope to make Kosmic Katana."}}});
	burglab->setEnterChanges(seeunihorn);

	Item* releaseswitch = new MaterialItem("RELEASE SWITCH", "A switch for releasing the quantumn chains around that unihorn.", burglab); //fake functionality because you can never use it

	Item* unihorncorn = new MaterialItem("UNIHORN CORN", "An exotic metal corn cob made of dark teal metal with extraordinary properties.\nIt was given to you in gratitude by the unihorn.", limbo);

	Item* ninjafurnace = new BlenderItem("NINJA FORGE", "Forge of the ninja tribe for smithing the finest tools and weapons with very hot heat.", ninjaforge, 
		{{ninjasmith, "You have unihorn corn?"},
		 {self, "Yep."},
		 {ninjasmith, "Amazing!"},
		 {ninjasmith, "Never in life I expect to be able to make Kosmic Katana."},
		 {ninjasmith, "But here is unihorn corn we need."},
		 {NULL, "You hand the UNIHORN CORN over to the NINJA SMITH."},
		 {NULL, "He turns the heat of the NINJA FORGE up to the max."},
		 {NULL, "Many hours pass as as he carefully shapes the metal of the corn."},
		 {NULL, "You stay to watch him artistically forge the Kosmic Katana."},
		 {NULL, "Many hours later..."},
		 {ninjasmith, "I have finish."},
		 {NULL, "The NINJA SMITH admires the KOSMIC KATANA, holding it up."},
		 {ninjasmith, "It is beautiful."},
		 {ninjasmith, "I never could have imagine how it turn out."},
		 {ninjasmith, "..."},
		 {ninjasmith, "Here, you take katana."},
		 {self, "Really?"},
		 {ninjasmith, "Yes."},
		 {ninjasmith, "I only want to make the katana."},
		 {ninjasmith, "I have no use for it beyond wall decoration."},
		 {NULL, "The NINJA SMITH hands over the KOSMIC KATANA to you."},
		 {self, "Oh wow thank you!"},
		 {ninjasmith, "In finding the unihorn corn, you have earned it."},
		 {ninjasmith, "I know you will make good use of it."}},
		{"UNIHORN CORN"}, kosmickatana);
	WorldChange& forgechanges = ((BlenderItem*)ninjafurnace)->getChanges();
	forgechanges.linkedDialogue.push({ninjasmith, {{ninjasmith, "I am very happy with how kosmic katana turn out."}, {ninjasmith, "Thank you for helping me accomplish dream."}}});
	forgechanges.worldcon = MADEKATANA;

	Item* elevatorbutton = new WorldChangeItem("ELEVATOR BUTTON", "The button for calling the elevator of the BURGER RESTAURANT.", elevatorentrance,
		{{NULL, "You call the elevator with the ELEVATOR BUTTON."},
		 {NULL, "The display above the elevator door shows an arrow going up."},
		 {NULL, "..."},
		 {NULL, "..."},
		 {NULL, "..."},
		 {self, ":|"},
		 {NULL, "You press the button again for a slightly longer duration."},
		 {NULL, "..."},
		 {NULL, "..."},
		 {NULL, "..."},
		 {NULL, "You hear an intensifying magnetic hum."},
		 {NULL, "..."},
		 {NULL, "..."},
		 {NULL, "..."},
		 {NULL, "You see the elevator rise from the ground!"},
		 {NULL, "The elevator dings."},
		 {NULL, "You can go IN ELEVATOR now!"}});
	WorldChange& ebchanges = ((WorldChangeItem*)elevatorbutton)->getChanges();
	ebchanges.exitPavings.push(make_tuple(elevatorentrance, elevator, IN_ELEVATOR, OUT));
	ebchanges.roomChanges.push({elevatorentrance, "in the entrance of the BURGER RESTAURANT. It has glass walls curving up and BURGER furniture."});
	
	Item* outputantenna = new MaterialItem("OUTPUT ANTENNA OF HUMILITY", "A component of THE PLOT DEVICE; humbly carries out the will of God.", limbo);
	Item* bigredbutton = new MaterialItem("BIG RED BUTTON OF HOPE", "A component of THE PLOT DEVICE which the user must press with hope that God's will be done.", limbo);
	Item* plotometer = new MaterialItem("PLOTOMETER OF PATIENCE", "A component of THE PLOT DEVICE; patiently waits for the right time to act.", limbo);

	//Create exits between rooms MARK: set exits
	village->setExit(SOUTH, docks);
	village->setExit(EAST, forestentrance);
	village->setExit(WEST, villageleft);
	village->setExit(IN_TENT_1, tentstore);
	village->setExit(IN_TENT_2, tentstation);
	village->setExit(IN_TENT_3, tentchurch);
	villageleft->setExit(EAST, village);
	villageleft->setExit(IN_TENT, tentmansion);
	tentmansion->setExit(OUTSIDE, villageleft);
	tentmansion->setExit(UPSTAIRS, tentlab);
	tentlab->setExit(DOWNSTAIRS, tentmansion);
	tentstore->setExit(OUTSIDE, village);
	tentstation->setExit(OUTSIDE, village);
	tentchurch->setExit(OUTSIDE, village);
	docks->setExit(NORTH, village);
	docks->setExit(IN_TENT, tenthouse);
	tenthouse->setExit(OUTSIDE, docks);
	forestentrance->setExit(WEST, village);
	forestentrance->setExit(NORTH, forest);
	forest->setExit(SOUTH, forestentrance); //forest exits
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
	ninjavillage->setExit(IN_HOUSE_1, ninjapantry);
	ninjavillage->setExit(IN_HOUSE_2, ninjaforge);
	ninjavillage->setExit(IN_HOUSE_3, ninjacapitol);
	ninjapantry->setExit(OUTSIDE, ninjavillage);
	ninjaforge->setExit(OUTSIDE, ninjavillage);
	ninjacapitol->setExit(OUTSIDE, ninjavillage);
	ninjavillage->setExit(DOWN, ninjaland);
	foresttempleentrance->setExit(SOUTHWEST, forestleft);
	foresttempleentrance->setExit(SOUTHEAST, forestright);
	foresttempleentrance->setExit(NORTHWEST, flowerfield);
	foresttempleentrance->setExit(NORTH, forestfork);
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
	desert->setExit(SOUTH, fdintermission3); //desert exits
	desert->setExit(NORTHWEST, deserttempleentrance);
	desert->setExit(NORTHEAST, desertdune);
	desert->setExit(EAST, desertplain);
	deserttempleentrance->setExit(EAST, desertdune);
	deserttempleentrance->setExit(NORTHEAST, deserthill);
	deserttemplestairs->setExit(OUTSIDE, deserttempleentrance);
	deserttemple->setExit(EAST, deserttemplestairs);
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
	desertshop->setExit(OUTSIDE,deserttown);
	desertgym->setExit(OUTSIDE,deserttown);
	deserthouse->setExit(OUTSIDE,deserttown);
	desertshopfixed->setExit(OUTSIDE,deserttown);
	desertgymfixed->setExit(OUTSIDE,deserttown);
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
	mineshaft->setExit(ABOVEGROUND, canyon3);
	mineshaft->setExit(EAST, minespring);
	mineshaft->setExit(WEST, mineshaft2);
	minespring->setExit(NORTH, mineshaft);
	mineshaft2->setExit(SOUTH, deserttunnel);
	mineshaft2->setExit(NORTH, mineshaft3);
	deserttunnel->setExit(SOUTHWEST, desertstation);
	deserttunnel->setExit(NORTH, mineshaft2);
	desertstation->setExit(NORTHEAST, deserttunnel);
	desertstation->setExit(ABOVEGROUND, canyon);
	mineshaft3->setExit(WEST, mineshaftside);
	mineshaft3->setExit(SOUTH, mineshaft2);
	mineshaft3->setExit(NORTH, minelight);
	mineshaftside->setExit(EAST, mineshaft3);
	mineshaftside->setExit(SOUTHWEST, kaboomroom);
	kaboomroom->setExit(NORTHEAST, mineshaftside);
	kaboomroom->setExit(NORTHWEST, berryroom);
	berryroom->setExit(SOUTHEAST, kaboomroom);
	minelight->setExit(UP, mineexit);
	minelight->setExit(SOUTH, mineshaft3);
	minelight->setExit(NORTH, volcanoentrance);
	mineexit->setExit(ABOVEGROUND, cliff0);
	mineexit->setExit(DOWN, minelight);
	cliff0->setExit(UNDERGROUND, mineexit);
	cliff0->setExit(EAST, cliff1);
	cliff1->setExit(WEST, cliff0);
	cliff1->setExit(DOWN, canyon2);
	cliff1->setExit(UP, cliff2);
	cliff2->setExit(DOWN, cliff1);
	thatcliff->setExit(DOWN, canyon);
	volcanoentrance->setExit(SOUTH, minelight);
	volcanoentrance->setExit(ABOVEGROUND, volcano);
	volcano->setExit(UNDERGROUND, volcanoentrance); //volcano exits
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
	castlehall->setExit(OUTSIDE, castleentrance);
	castlehall->setExit(SOUTH, castlethrone);
	castlethrone->setExit(NORTH, castlehall);
	factory1->setExit(OUTSIDE, volcano1);
	factory1->setExit(EAST, factorykitchen);
	factory1->setExit(UP, factoryplat);
	factorykitchen->setExit(WEST, factory1);
	factoryplat->setExit(EAST, controlroom1);
	factoryplat->setExit(DOWN, factory1);
	controlroom1->setExit(WEST, factoryplat);
	volcanostation->setExit(OUTSIDE, volcano3);
	factory2->setExit(OUTSIDE, volcano3);
	factory2->setExit(NORTHWEST, switchroom);
	factory2->setExit(NORTHEAST, conveyor1);
	switchroom->setExit(SOUTHEAST, factory2);
	conveyor1->setExit(FORWARD, factory2);
	factorycenter->setExit(SOUTHWEST, conveyor1);
	factorycenter->setExit(NORTHWEST, conveyor2);
	factorycenter->setExit(NORTHEAST, conveyor3);
	conveyor3->setExit(FORWARD, controlroom2);
	controlroom2->setExit(SOUTHWEST, conveyor3);
	conveyor2->setExit(FORWARD, factorycenter);
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
	factory3->setExit(OUTSIDE, volcano6);
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
	sewercenter3->setExit(EAST, sewertreasure);
	sewertreasure->setExit(WEST, sewercenter3);
	volcanotempleentrance->setExit(UP, sewercenter3);
	volcanotemplestairs->setExit(OUTSIDE, volcanotempleentrance);
	volcanotemple->setExit(EAST, volcanotemplestairs);
	sewercenter4->setExit(NORTHEAST, sewercenter2);
	sewercenter4->setExit(SOUTHEAST, sewercenter1);
	sewercenter4->setExit(WEST, sewermineswest);
	sewermineswest->setExit(EAST, sewercenter4);
	sewermineswest->setExit(WEST, mountainmine);
	sewerplant->setExit(NORTH, sewercenter2);
	sewerplant->setExit(SOUTH, sewercenter1);
	mountainmine->setExit(EAST, sewermineswest);
	mountainmine->setExit(ABOVEGROUND, mountain);
	mountain->setExit(UNDERGROUND, mountainmine);
	mountain->setExit(UP, mountain2);
	mountain2->setExit(WEST, mountainside);
	mountain2->setExit(UP, mountain3);
	mountain2->setExit(DOWN, mountain);
	mountainside->setExit(WEST, mountainlake);
	mountainside->setExit(EAST, mountain2);
	mountainlake->setExit(EAST, mountainside);
	mountain3->setExit(UP, mountainpeak);
	mountain3->setExit(DOWN, mountain2);
	mountainpeak->setExit(IN_TENT, tenthome);
	mountainpeak->setExit(DOWN, mountain3);
	tenthome->setExit(OUTSIDE, mountainpeak);
	bridge1->setExit(NORTH, bridge2);
	bridge1->setExit(SOUTH, volcano7);
	bridge2->setExit(NORTH, bridge3);
	bridge2->setExit(SOUTH, bridge1);
	bridge3->setExit(NORTH, BURGERSBURG);
	bridge3->setExit(SOUTH, bridge2);
	BURGERSBURG->setExit(NORTH, mainstreet1); //city exits
	BURGERSBURG->setExit(NORTHWEST, newstreet1);
	BURGERSBURG->setExit(NORTHEAST, coolstreet1);
	BURGERSBURG->setExit(SOUTH, bridge3);
	leftstreet1->setExit(NORTH, leftstreet2);
	leftstreet1->setExit(EAST, newstreet1);
	leftstreet2->setExit(NORTH, leftstreet3);
	leftstreet2->setExit(SOUTH, leftstreet1);
	leftstreet2->setExit(EAST, newstreet2);
	leftstreet2->setExit(INSIDE, burgfish);
	burgfish->setExit(OUTSIDE, leftstreet2);
	burgfish->setExit(DOWNSTAIRS, burgchurch);
	burgchurch->setExit(UPSTAIRS, burgfish);
	leftstreet3->setExit(NORTH, leftstreet4);
	leftstreet3->setExit(SOUTH, leftstreet2);
	leftstreet3->setExit(EAST, newstreet3);
	leftstreet4->setExit(NORTH, leftstreet5);
	leftstreet4->setExit(SOUTH, leftstreet3);
	leftstreet4->setExit(EAST, newstreet4);
	leftstreet4->setExit(INSIDE, casino);
	casino->setExit(OUTSIDE, leftstreet4);
	casino->setExit(IN_BACK_ROOM, casinoback);
	casinoback->setExit(OUT, casino);
	casinoback->setExit(DOWN, casinobase);
	casinobase->setExit(UP, casinoback);
	leftstreet5->setExit(NORTH, richneighborhood1);
	leftstreet5->setExit(SOUTH, leftstreet4);
	leftstreet5->setExit(EAST, newstreet5);
	newstreet1->setExit(NORTH, newstreet2);
	newstreet1->setExit(SOUTHEAST, BURGERSBURG);
	newstreet1->setExit(EAST, mainstreet1);
	newstreet1->setExit(WEST, leftstreet1);
	newstreet2->setExit(NORTH, newstreet3);
	newstreet2->setExit(SOUTH, newstreet1);
	newstreet2->setExit(EAST, mainstreet2);
	newstreet2->setExit(WEST, leftstreet2);
	newstreet2->setExit(INSIDE, burgstore);
	burgstore->setExit(OUTSIDE, newstreet2);
	newstreet3->setExit(NORTH, newstreet4);
	newstreet3->setExit(SOUTH, newstreet2);
	newstreet3->setExit(EAST, mainstreet3);
	newstreet3->setExit(WEST, leftstreet3);
	newstreet3->setExit(IN_ALLEY, darkalley);
	darkalley->setExit(OUT, newstreet3);
	newstreet4->setExit(NORTH, newstreet5);
	newstreet4->setExit(SOUTH, newstreet3);
	newstreet4->setExit(EAST, mainstreet4);
	newstreet4->setExit(WEST, leftstreet4);
	newstreet5->setExit(NORTH, richneighborhood2);
	newstreet5->setExit(SOUTH, newstreet4);
	newstreet5->setExit(EAST, mainstreet5);
	newstreet5->setExit(WEST, leftstreet5);
	mainstreet1->setExit(NORTH, mainstreet2);
	mainstreet1->setExit(SOUTH, BURGERSBURG);
	mainstreet1->setExit(EAST, coolstreet1);
	mainstreet1->setExit(WEST, newstreet1);
	mainstreet2->setExit(NORTH, mainstreet3);
	mainstreet2->setExit(SOUTH, mainstreet1);
	mainstreet2->setExit(WEST, newstreet2);
	mainstreet2->setExit(EAST, coolstreet2);
	mainstreet3->setExit(NORTH, mainstreet4);
	mainstreet3->setExit(SOUTH, mainstreet2);
	mainstreet3->setExit(WEST, newstreet3);
	mainstreet3->setExit(EAST, coolstreet3);
	mainstreet4->setExit(NORTH, mainstreet5);
	mainstreet4->setExit(SOUTH, mainstreet3);
	mainstreet4->setExit(WEST, newstreet4);
	mainstreet4->setExit(EAST, coolstreet4);
	mainstreet4->setExit(DOWN, burgstation);
	burgstation->setExit(UP, mainstreet4);
	mainstreet5->setExit(SOUTH, mainstreet4);
	mainstreet5->setExit(WEST, newstreet5);
	mainstreet5->setExit(EAST, coolstreet5);
	mainstreet5->setExit(INSIDE, elevatorentrance);
	coolstreet1->setExit(NORTH, coolstreet2);
	coolstreet1->setExit(SOUTHWEST, BURGERSBURG);
	coolstreet1->setExit(EAST, rightstreet1);
	coolstreet1->setExit(WEST, mainstreet1);
	coolstreet2->setExit(NORTH, coolstreet3);
	coolstreet2->setExit(SOUTH, coolstreet1);
	coolstreet2->setExit(EAST, rightstreet2);
	coolstreet2->setExit(WEST, mainstreet2);
	coolstreet2->setExit(INSIDE, shrimpartment1);
	shrimpartment1->setExit(OUTSIDE, coolstreet2);
	shrimpartment1->setExit(UP, shrimpartment2);
	shrimpartment2->setExit(DOWN, shrimpartment1);
	shrimpartment2->setExit(UP, shrimpartment3);
	shrimpartment3->setExit(DOWN, shrimpartment2);
	shrimpartment3->setExit(UP, shrimproof);
	shrimproof->setExit(DOWN, shrimpartment3);
	coolstreet3->setExit(NORTH, coolstreet4);
	coolstreet3->setExit(SOUTH, coolstreet2);
	coolstreet3->setExit(EAST, rightstreet3);
	coolstreet3->setExit(WEST, mainstreet3);
	coolstreet4->setExit(NORTH, coolstreet5);
	coolstreet4->setExit(SOUTH, coolstreet3);
	coolstreet4->setExit(EAST, rightstreet4);
	coolstreet4->setExit(WEST, mainstreet4);
	coolstreet4->setExit(IN_ALLEY, bankalley);
	bankalley->setExit(OUT, coolstreet4);
	coolstreet5->setExit(SOUTH, coolstreet4);
	coolstreet5->setExit(EAST, rightstreet5);
	coolstreet5->setExit(WEST, mainstreet5);
	rightstreet1->setExit(NORTH, rightstreet2);
	rightstreet1->setExit(WEST, coolstreet1);
	rightstreet2->setExit(NORTH, rightstreet3);
	rightstreet2->setExit(SOUTH, rightstreet1);
	rightstreet2->setExit(WEST, coolstreet2);
	rightstreet3->setExit(NORTH, rightstreet4);
	rightstreet3->setExit(SOUTH, rightstreet2);
	rightstreet3->setExit(WEST, coolstreet3);
	rightstreet4->setExit(NORTH, rightstreet5);
	rightstreet4->setExit(SOUTH, rightstreet3);
	rightstreet4->setExit(WEST, coolstreet4);
	rightstreet5->setExit(SOUTH, rightstreet4);
	rightstreet5->setExit(WEST, coolstreet5);
	rightstreet5->setExit(INSIDE, firedepartment);
	firedepartment->setExit(OUTSIDE, rightstreet5);
	richneighborhood1->setExit(EAST, richneighborhood2);
	richneighborhood1->setExit(NORTHEAST, richneighborhood4);
	richneighborhood1->setExit(SOUTH, leftstreet5);
	richneighborhood2->setExit(NORTH, richneighborhood4);
	richneighborhood2->setExit(SOUTH, newstreet5);
	richneighborhood2->setExit(EAST, richneighborhood3);
	richneighborhood2->setExit(WEST, richneighborhood1);
	richneighborhood3->setExit(NORTHWEST, richneighborhood4);
	richneighborhood3->setExit(WEST, richneighborhood2);
	richneighborhood4->setExit(SOUTH, richneighborhood2);
	richneighborhood4->setExit(SOUTHEAST, richneighborhood3);
	richneighborhood4->setExit(SOUTHWEST, richneighborhood1);
	richneighborhood4->setExit(INSIDE, ceolobby);
	ceolobby->setExit(OUTSIDE, richneighborhood4);
	ceolobby->setExit(IN_ELEVATOR, ceoelevator1);
	ceolobby->setExit(UP, ceolobby2);
	ceolobby2->setExit(IN_ROOM, ballroom);
	ceolobby2->setExit(IN_ELEVATOR, ceoelevator2);
	ceolobby2->setExit(DOWN, ceolobby);
	ballroom->setExit(OUT, ceolobby2);
	ceoelevator1->setExit(TO_FLOOR_2, ceoelevator2);
	ceoelevator1->setExit(TO_THE_TOP, ceoelevator3);
	ceoelevator1->setExit(OUT, ceolobby);
	ceoelevator2->setExit(TO_FLOOR_1, ceoelevator1);
	ceoelevator2->setExit(TO_THE_TOP, ceoelevator3);
	ceoelevator2->setExit(OUT, ceolobby2);
	ceoelevator3->setExit(TO_FLOOR_1, ceoelevator1);
	ceoelevator3->setExit(TO_FLOOR_2, ceoelevator2);
	ceoelevator3->setExit(OUT, ceoroom);
	ceoroom->setExit(IN_ELEVATOR, ceoelevator3);
	ceoroom->setExit(IN_SAFE, burgsafe);
	burgsafe->setExit(OUT, ceoroom);
	elevatorentrance->setExit(OUTSIDE, mainstreet5);
	elevator->setExit(TO_THE_TOP, elevatortop);
	elevator->setExit(OUT, elevatorentrance);
	elevatortop->setExit(OUT, BURGERRESTAURANT);
	elevatortop->setExit(TO_GROUND_LEVEL, elevator);
	BURGERRESTAURANT->setExit(IN_ELEVATOR, elevatortop);
	elevatorbottom->setExit(TO_GROUND_LEVEL, elevator);
	elevatorbottom->setExit(TO_THE_TOP, elevatortop);
	elevatorbottom->setExit(OUT, burgerbasement);
	burgerbasement->setExit(IN_ELEVATOR, elevatorbottom);
	burgerbasement->setExit(SOUTH, burgbasec);
	burgerbasement->setExit(EAST, burgbasene);
	burgerbasement->setExit(WEST, burgbasenw);
	burgbasenw->setExit(EAST, burgerbasement);
	burgbasenw->setExit(SOUTH, burgbasew);
	burgbasene->setExit(WEST, burgerbasement);
	burgbasene->setExit(SOUTH, burgbasee);
	burgbasene->setExit(IN_ROOM, burgstation);
	burgbasew->setExit(NORTH, burgbasenw);
	burgbasew->setExit(SOUTH, burgbasesw);
	burgbasew->setExit(EAST, burgbasec);
	burgbasew->setExit(IN_ROOM, burgboiler);
	burgbasec->setExit(NORTH, burgerbasement);
	burgbasec->setExit(SOUTH, burgbases);
	burgbasec->setExit(EAST, burgbasee);
	burgbasec->setExit(WEST, burgbasew);
	burgbasee->setExit(NORTH, burgbasene);
	burgbasee->setExit(SOUTH, burgbasese);
	burgbasee->setExit(WEST, burgbasec);
	burgbases->setExit(IN_ROOM, burglab);
	burgbases->setExit(NORTH, burgbasec);
	burgbases->setExit(EAST, burgbasese);
	burgbases->setExit(WEST, burgbasesw);
	burgbasesw->setExit(EAST, burgbases);
	burgbasesw->setExit(NORTH, burgbasew);
	burgbasese->setExit(WEST, burgbases);
	burgbasese->setExit(NORTH, burgbasee);
	burgbasese->setExit(DOWNSTAIRS, burgplatn);
	burgplatn->setExit(UPSTAIRS, burgbasese);
	burgplatn->setExit(EAST, burgplate);
	burgplatn->setExit(SOUTH, burgplats);
	burgplats->setExit(NORTHEAST, burgplate);
	burgplats->setExit(NORTH, burgplatn);
	burgplats->setExit(IN_ROOM, BURGERPRISON);
	burgplate->setExit(WEST, burgplatn);
	burgplate->setExit(SOUTHWEST, burgplats);
	burgboiler->setExit(OUT, burgbasew);
	burgstorage->setExit(OUT, burgbasene);
	burglab->setExit(OUT, burgbases);
	BURGERPRISON->setExit(OUT, burgplats);
	basestation->setExit(NORTH, BURGERPRISON);
	foresttemplestairs->setExit(SOUTH, forestbuffer1);
	forestbuffer1->setExit(NORTH, foresttemplestairs);
	forestbuffer2->setExit(SOUTH, foresttemple);
	foresttemple->setExit(SOUTH, forestbuffer2);
	forestbranchw->setExit(NORTHEAST, foresttemple);
	forestbranchw->setExit(SOUTHEAST, foresttemple2);
	forestbranche->setExit(NORTH, foresttemple);
	forestbranche->setExit(SOUTH, foresttemple2);
	forestbranchw2->setExit(NORTH, foresttemple2);
	forestbranchw2->setExit(SOUTH, foresttemple3);
	forestbranche2->setExit(NORTHWEST, foresttemple2);
	forestbranche2->setExit(SOUTHWEST, foresttemple3);
	forestbranchw3->setExit(NORTH, foresttemple3);
	forestbranchw3->setExit(EAST, foresttemple4);
	forestbranche3->setExit(NORTHWEST, foresttemple3);
	forestbranche3->setExit(SOUTHEAST, foresttemple4);
	foresttemple4->setExit(EAST, foresttemple5);
	foresttemple5->setExit(WEST, foresttemple4);
	foresttemple5->setExit(NORTH, foresttempleboss);
	foresttempleboss->setExit(SOUTH, foresttemple5);
	tunnels->setExit(TO_THE_VILLAGE, tentstation);
	tunnels->setExit(TO_THE_DESERT, desertstation);

	//set up enemies enemies MARK: enemies (internal)
	NPC* pricklyhog = new NPC("", "PRICKLY HOG", "A small but ferocious hog with sharp prickles.", limbo, 0, Stats(10, 10, 5, 0, 10, 15, 9));
	Attack* hogheadbutt = new Attack("HEADBUTT", "headbutted", true, -5, 5, 0, 1, 1, 1);
	headbutt->recoil = 5;
	Attack* homing_prickle = new Attack("HOMING PRICKLE", "launched homing prickles at", false, 6, 3, 5, 2, 4, 3);
	pricklyhog->setBasicAttack(hogheadbutt);
	pricklyhog->addSpecialAttack(homing_prickle);

	NPC* greaterhog = new NPC("", "GREATER HOG", "A larger and more territorial hog with sharp prickles and tusks.", limbo, 0, Stats(20, 10, 10, 2, 20, 20, 9));
	greaterhog->setBasicAttack(hogheadbutt);
	greaterhog->addSpecialAttack(homing_prickle);

	NPC* grassman = new NPC("", "GRASSMAN", "A really grassy humanoid who hates real humans.", limbo, 0, Stats(16, 0, 5, 0, 2, 5, 9));
	Attack* grassstrike = new Attack("GRASS STRIKE", "grassily striked", true, -5, 15, 0, 1, 1, 1);
	Attack* lawnmower = new Attack("LAWNMOWER", "threw a lawnmower at", false, 10, 50, 50, 1, 1, 1, false, 2);
	grassman->setBasicAttack(grassstrike);
	grassman->addSpecialAttack(lawnmower);

	NPC* buffgrassman = new NPC("", "BUFF GRASSMAN", "A really grassy humanoid who has been hitting the gym.", limbo, 0, Stats(32, 0, 8, 0, 2, 4, 9));
	Effect* theburn = new Effect("THE BURN", 3, 0, 0, 2.0, 2.0, 2.0);
	Attack* benchpress = new Attack("BENCH PRESS", "worked out with", false, 10, 0, 0, 1, 1, 5, true);
	buffgrassman->setBasicAttack(grassstrike);
	buffgrassman->addSpecialAttack(benchpress);

	NPC* enemydeer = new NPC("", "ENEMY DEER", "A bipedal deer in a fighting stance.", limbo, 0, Stats(10, 6, 10, 1, 5, 5, 9));
	Attack* deercombo = new Attack("DEER COMBO", "beat up", true, 0, 5, 0, 4, 4, 1);
	enemydeer->setBasicAttack(deercombo);

	NPC* ninjascout = new NPC("", "NINJA SCOUT", "A junior member of the ninja village, often sent on easy missions.", limbo, 0, Stats(20, 2, 8, 0, 5, 20, 9));
	Attack* shurikenninja = new Attack("SHURIKEN", "expertly threw shurikens at", false, 0, 8, 5, 1, 2, 3);
	ninjascout->setBasicAttack(shurikenninja);

	NPC* ninja = new NPC("", "NINJA", "A complete ninja, trained in the ninja ways and living the ninja lifestyle.", limbo, 0, Stats(40, 10, 25, 0, 25, 30, 9));
	Attack* shurikenninja2 = new Attack("SHURIKEN", "expertly threw shurikens at", false, -5, 10, 15, 1, 2, 3);
	ninja->setBasicAttack(shurikenninja2);
	Attack* katana = new Attack("KATANA", "sliced", true, 5, 25, 30, 1, 1, 1);

	NPC* ninjachef = new NPC("", "NINJA CHEF", "An expert ninja who is also an expert in cooking. He guards the ninja pantry.", limbo, 0, Stats(80, 20, 20, 20, 20, 25, 9));
	Attack* shuripan = new Attack("SHURIPAN", "threw spinning pans at", false, -5, 15, 0, 1, 2, 3);
	shuripan->afterdesc = " like shurikens";
	Attack* spatula = new Attack("SPATULA", "flipped", true, 6, 10, 0, 1, 1, 1);
	spatula->afterdesc = " into the air with a spatula";
	Effect* spatulad = new Effect("SPATULA'D", 0); //doesn't cancel their turn but may interrupt healers wanting to heal them
	spatulad->remove = true;
	spatulad->falldamage = 20;
	spatula->addEffect(spatulad);
	ninjachef->addSpecialAttack(spatula);
	Attack* ninjafeast = new Attack("NINJA FEAST", "prepared a ninja feast for his team", false, 8, -30, 0, 1, 1, 7, true);
	ninjachef->addSpecialAttack(ninjafeast);
	Attack* ninjasmoothie = new Attack("NINJA SMOOTHIE", "prepared a ninja smoothie for", false, 3, 20, 5, 1, 1, 1, true, 11);
	Effect* smoothied = new Effect("SMOOTHIED", 3, 0, 0, 1.5, 1, 1, 1.5);
	ninjasmoothie->addEffect(smoothied);
	ninjachef->addSpecialAttack(ninjasmoothie);

	//a true ninja doesn't reveal his name despite being a unique character
	NPC* ninjachief = new NPC("", "NINJA CHIEF", "The chief ninja of the ninja village, with the most experience and the highest rank of diamond belt.", limbo, 0, Stats(120, 40, 35, 20, 25, 60, 9));
	ninjachief->setBoss(true);
	Attack* ninjakick = new Attack("NINJA KICK", "jumped at", true, -5, 20, 0, 1, 1, 1);
	ninjakick->afterdesc = " with a flying side kick";
	ninjachief->setBasicAttack(ninjakick);
	Attack* kiloshuriken = new Attack("KILOSHURIKEN", "threw dual helicopter rotors at the team", false, 6, 30, 20, 2, 2, 3);
	ninjachief->addSpecialAttack(kiloshuriken);
	Attack* acupunctuken = new Attack("ACUPUNCTUKEN", "pinned", false, 8, 2, 15, 4, 4, 1);
	acupunctuken->afterdesc = " with precise shurikens";
	acupunctuken->addEffect(pinned);
	ninjachief->addSpecialAttack(acupunctuken);
	Attack* deadlyspinferno = new Attack("DEADLY SPINFERNO", "flew at", false, 15, 10, 20, 5, 5, 3);
	deadlyspinferno->afterdesc = " in a flaming inferno";
	deadlyspinferno->addEffect(onfire);
	ninjachief->addSpecialAttack(deadlyspinferno);

	NPC* jimshady = new NPC("", "JIM SHADY", "An envious and spiky shrimp. This Jim Shady is just imitating.", limbo, 0, Stats(50, 10, 10, 35, 15, 10, 9));
	Attack* shrimplebeam = new Attack("SHRIMPLE BEAM", "fired a pressurized jet of water at", false, 0, 100, 100, 1, 1, 1);
	shrimplebeam->instakill = true;
	jimshady->setBasicAttack(shrimplebeam);
	Effect* engarde = new Effect("EN GARDE!", 2147483647);
	engarde->guardset = 1;
	jimshady->setEffect(engarde, NULL);

	NPC* jimmyshimmy = new NPC("", "JIMMY SHIMMY", "A juvenile shrimp who likes to help out his fellow shrimps.", limbo, 0, Stats(20, 0, 10, 0, 20, 15, 9));
	Attack* shrimpleshimmy = new Attack("SHRIMPLE SHIMMY", "shimmied all over", true, 0, 5, 0, 3, 4, 1);
	jimmyshimmy->setBasicAttack(shrimpleshimmy);
	/*Effect* flinch = new Effect("FLINCH", 1);
	shrimpleshimmy->addEffect(flinch);*/

	NPC* flowerfiend = new NPC("", "FLOWER FIEND", "Really big carnivorous flower, probably the FLOWER FRIEND your sister talks about.", limbo, 0, Stats(20, 0, 7, 0, 0, 12, 9));
	Attack* vinewhip = new Attack("VINE WHIP", "used its vines to whip", true, -6, 10, 0, 1, 1, 1);
	Attack* crunch = new Attack("CRUNCH", "used its flowery fangs to crunch", true, 2, 18, 7, 1, 1, 1);
	Effect* flowerpower = new Effect("FLOWER POWER", 3, 0, 0, 2.0);
	Attack* flowerempower = new Attack("FLOWER EMPOWER", "used its flower power to buff", false, 15, 10, 5, 1, 1, 1, true, 10);
	Attack* nutrientabsorb = new Attack("NUTRIENT ABSORB", "sucked the nutrients out of", false, 10, 10, 5, 1, 1, 1, true, 10, 0, 0.5);
	flowerempower->addEffect(flowerpower);
	Attack* solarbeam = new Attack("SOLAR BEAM", "used its petals to channel solar light towards", false, 18, 30, 10, 1, 1, 10, false, 10);
	flowerfiend->setBasicAttack(vinewhip);
	flowerfiend->addSpecialAttack(crunch);
	flowerfiend->addSpecialAttack(nutrientabsorb);
	flowerfiend->addSpecialAttack(flowerempower);
	flowerfiend->addSpecialAttack(solarbeam);

	NPC* egadbot= new NPC("ROGUE ROBOT", "EGARDENBOT", "Short trapezoidal copper robot designed to be an expert gardener,\nbefore going rogue and trimming everything else as well.", limbo, 0, Stats(20, 30, 5, 5, 10, 20, 9));
	Attack* snip = new Attack("SNIP", "snipped scissors at", true, -5, 7, 5, 1, 1, 1);
	egadbot->setBasicAttack(snip);
	Attack* timber = new Attack("TIMBER", "snipped down a tree, directed at", false, 10, 20, 0, 1, 1, 1);
	egadbot->addSpecialAttack(timber);

	NPC* savagehog = new NPC("", "MAMMOTH HOG", "Savage, mammoth elder hog with very sharp prickles.", limbo, 0, Stats(90, 20, 10, 10, 10, 10, 9), Stats(0, 0, 1, 1, 1, 0, 0));
	Attack* charge = new Attack("CHARGE", "charged at", true, -5, 10, 20, 1, 1, 1);
	Attack* savageroar = new Attack("SAVAGE ROAR", "roared savagely at", false, 5, 0, 0, 1, 1, 7);
	Effect* intimidated = new Effect("INTIMIDATED", 4, 0, 0, 0.5);
	savageroar->addEffect(intimidated);
	Attack* pricklestorm = new Attack("PRICKLE STORM", "launched a storm of prickles at", false, 10, 1, 5, 1, 3, 7);
	savagehog->setBasicAttack(charge);
	savagehog->addSpecialAttack(savageroar);
	savagehog->addSpecialAttack(pricklestorm);
	savagehog->setBoss(true);

	NPC* mimic = new NPC("", "MIMIC", "Big carnivorous treasure chest full of treasure and bones.", limbo, 0, Stats(50, 30, 15, 20, 20, 30, 9));
	Attack* chomp = new Attack("CHOMP", "chomped", true, -5, 10, 10, 1, 1, 1);
	Attack* stomp = new Attack("STOMP", "jumped onto", true, 5, 30, 0, 1, 1, 1);
	Attack* monymeteor = new Attack("MONY METEOR", "spit a heavy mass of monies at", false, 10, 50, 0, 1, 1, 3);
	mimic->setBasicAttack(chomp);
	mimic->addSpecialAttack(stomp);
	mimic->addSpecialAttack(monymeteor);
	mimic->setBoss(true); //of the mini variety

	NPC* sandman = new NPC("", "SANDMAN", "A really sandy humanoid continuously flowing with sand.", limbo, 0, Stats(20, 5, 8, 0, 0, 10, 9));
	Effect* sanded = new Effect("SAND IN THE EYES", 3, 0, 0, .5, .5);
	Attack* sandthrow = new Attack("POCKET SAND", "threw sand at", false, -3, 5, 0, 1, 1, 1);
	sandthrow->afterdesc = "'s eyes";
	sandthrow->addEffect(sanded);
	Attack* sandpunch = new Attack("SAND PUNCH", "threw a sandy punch at", true, 1, 12, 0, 1, 1, 1);
	sandman->setBasicAttack(sandthrow);
	sandman->addSpecialAttack(sandpunch);

	NPC* skeleviking = new NPC("", "SKELEVIKING", "A lost skeleton with a horned hat and shield.", limbo, 0, Stats(5, 0, 30, 0, 30, 15, 9));
	Effect* shieldup = new Effect("SHIELD UP", 2147483647);
	shieldup->guardset = 5;
	skeleviking->setEffect(shieldup, NULL);
	Attack* vslash = new Attack("VIKING SLASH", "hit", true, -5, 5, 0, 1, 1, 1);
	vslash->afterdesc = " with his sword";
	skeleviking->setBasicAttack(vslash);
	Attack* vbonecone = new Attack("BONE CONE", "launched his helmet's bone cones at", false, 5, 1, 0, 4, 4, 1);
	skeleviking->addSpecialAttack(vbonecone);
	Attack* vtornado = new Attack("VIKING TORNADO", "spun like a tornado at", true, 10, 8, 0, 3, 4, 3);
	skeleviking->addSpecialAttack(vtornado);

	NPC* rumbleweed = new NPC("", "RUMBLEWEED", "Tumbling weed of the desert known to cause tremors among its path.", limbo, 0, Stats(10, 2, 20, 1, 15, 30, 9));
	Attack* tumble = new Attack("TUMBLE", "rolled into", true, -5, 10, 10, 1, 1, 1);
	Effect* brambled = new Effect("BRAMBLED", 3, 5);
	tumble->addEffect(brambled);
	Attack* rumble = new Attack("RUMBLE", "shook the ground under", false, 2, 5, 0, 3, 3, 7);
	rumbleweed->setBasicAttack(tumble);
	rumbleweed->addSpecialAttack(rumble);

	NPC* rockbug = new NPC("", "ROCKBUG", "Sizable geometric bug who normally lives in the rock, but becomes very aggressive when disturbed.", limbo, 0, Stats(15, 30, 5, 20, 0, 2, 9));
	Attack* rocknroll = new Attack("ROCK AND ROLL", "rolled up into a boulder, speeding at", true, -5, 20, 0, 1, 1, 1);
	Effect* jamming = new Effect("JAMMING OUT", 10, 0, 0, 2.5, 1.5, 1.5, 1, 50.0);
	rocknroll->selfeffect = jamming;
	Attack* saltcure = new Attack("SALT CURE", "coughed up crystals of curing salt onto", false, 15, 5, 0, 1, 1, 1);
	Effect* saltcured = new Effect("SALT CURED", 2147483647);
	saltcured->spusage = 2.0; //salt cured means you take double damage and use double sp
	saltcured->damagebuff = 2.0;
	saltcure->addEffect(saltcured);
	rockbug->setBasicAttack(rocknroll);
	rockbug->addSpecialAttack(saltcure);

	/*NPC* rascal = new NPC("", "MINE RASCAL", "Little thingy who lives in the mines.", limbo, 0, Stats());
	rascal->setBasicAttack(genericattack);
	rascal->addSpecialAttack(genericspecial);
	rascal->addSpecialAttack(genericcc);*/

	NPC* skeleminer = new NPC("", "SKELEMINER", "Miner of a previous generation (with those lamped mining hats!), ceaselessly mining away at the walls.", limbo, 0, Stats(20, 10, 15, 5, 5, 13, 9));
	Attack* pickaxerang = new Attack("PICKAXERANG", "threw his pickaxe at", false, -5, 10, 15, 1, 1, 7);
	Attack* sdynamite = new Attack("DYNAMITE", "threw a stick of dynamite at", false, 3, 20, 20, 1, 1, 1);
	Attack* pickstrike = new Attack("MINE", "struck", true, 6, 25, 40, 1, 1, 1);
	pickstrike->afterdesc = " with his pickaxe";
	skeleminer->setBasicAttack(pickaxerang);
	skeleminer->addSpecialAttack(sdynamite);
	skeleminer->addSpecialAttack(pickstrike);

	NPC* dreadnaut = new NPC("", "DREADNOUGHT", "Hefty armored bug complete with a tank cannon. A true wonder of nature.", limbo, 0, Stats(100, 30, 20, 30, 20, 5, 9));
	Attack* dpincer = new Attack("PINCER", "pinced", true, -5, 10, 10, 1, 1, 1);
	Attack* rapidfire = new Attack("RAPID FIRE", "fired upon", false, 5, 1, 100, 15, 15, 1);
	Attack* tankshell = new Attack("TANK SHELL", "fired its cannon at", false, 10, 30, 20, 1, 1, 3);
	dreadnaut->setBasicAttack(dpincer);
	dreadnaut->addSpecialAttack(rapidfire);
	dreadnaut->addSpecialAttack(tankshell);
	dreadnaut->setBoss(true); //more of a miniboss

	//the lobster is your pet and fast travel MARK: Tunnel Lobster (enemy)
	NPC* tunnellobster = new NPC("", "TUNNEL LOBSTER", "An immense, savage crustacean who inhabits the tunnels below.", limbo, 0, Stats(200, 20, 15, 20, 15, 50, 9));
	Attack* lpincer = new Attack("PINCER", "pinced", true, -5, 10, 10, 1, 1, 1);
	Attack* tailsmack = new Attack("TAIL SMACK", "smacked", true, 6, 25, 0, 1, 1, 3);
	tailsmack->afterdesc = " with its tail";
	Attack* lobroar = new Attack("LOBSTERY ROAR", "unleashed a lobstery roar, shaking loose chunks of the ceiling", false, 8, 20, 0, 5, 5, 1);
	lobroar->focushits = false;
	Attack* trainrush = new Attack("TRAIN RUSH", "rushed at", true, 13, 40, 0, 1, 1, 1);
	trainrush->afterdesc = "like a train";
	tunnellobster->setBasicAttack(lpincer);
	tunnellobster->addSpecialAttack(tailsmack);
	tunnellobster->addSpecialAttack(lobroar);
	tunnellobster->addSpecialAttack(trainrush);
	tunnellobster->setBoss(true);

	//Gravity Girl Viola is mostly attack with some support but a boss fight first MARK: Viola (enemy)
	NPC* tkviola = new NPC("TELEKINETIC KIDNAPPER", "VIOLA", "Telekinetic teenager responsible for the disappearence of the desert town.\nHer hair floats upwards and she hovers a few feet above the ground.", limbo, 0, Stats(100, 0, 10, 0, 20, 20, 19), Stats(1, 0, 1, 0, 2, 0, 0));
	tkviola->setBoss(true);
	Attack* wave = new Attack("WAVE", "flung a gravitational wave at", false, -5, 10, 20, 1, 1, 1);
	tkviola->setBasicAttack(wave);
	Attack* smackdown = new Attack("SMACKDOWN", "lifted", false, 5, 20, 0, 1, 1, 1);
	smackdown->afterdesc = " into the air and smacked them back down";
	Effect* suspended = new Effect("SUSPENDED", 3, 0, 0);
	smackdown->synergies.push_back(suspended); //because Viola is already holding the npc in the air so it's easier to do more damage
	tkviola->addSpecialAttack(smackdown);
	Attack* tkguard = new Attack("INVOLUNTARY GUARD", "set up a guard using a townsperson", false, 7, 0, 0, 0, 0, 0);
	tkguard->guardset = 1;
	tkviola->addSpecialAttack(tkguard);
	Attack* fling = new Attack("FLING", "flung", false, 5, 0, 0, 1, 1, 1);
	fling->afterdesc = " into the stratosphere";
	Effect* flung = new Effect("FLUNG", 1);
	flung->remove = true;
	flung->falldamage = 30;
	fling->cancel = suspended; //throwing someone into the air makes them no longer frozen in place
	fling->addEffect(flung);
	tkviola->addSpecialAttack(fling);
	Attack* suspend = new Attack("SUSPEND", "suspended", false, 7, 0, 0, 1, 1, 1);
	suspend->afterdesc = " in the air";
	suspended->freeze = true;
	suspend->addEffect(suspended);
	tkviola->addSpecialAttack(suspend);
	Attack* gutpunch = new Attack("GUT PUNCH", "delivered a telekinetic punch to",false,  12, 25, 50, 1, 1, 1, false, 11);
	gutpunch->afterdesc = "'s gut";
	gutpunch->addDescription("Deliver a telekinetic punch to the target's gut. (25 ATTACK, 50 PIERCE)");
	tkviola->addSpecialAttack(gutpunch);
	Attack* spatialpop = new Attack("SPATIAL POP", "popped a spacetime bubble at", false, 10, 20, 30, 1, 1, 3, false, 12);
	spatialpop->addDescription("Form and pop a spacetime bubble damaging three adjacent enemies. (20 DAMAGE, 30 PIERCE)");
	tkviola->addSpecialAttack(spatialpop);
	Attack* forcefield = new Attack("FORCE FIELD", "created an outwards force around", false, 15, 0, 0, 0, 0, 7, true, 15);
	forcefield->addDescription("Create an outwards force around the team, doubling defense.");
	Effect* forcefielded = new Effect("FORCE FIELD", 3, 0, 0, 1, 2);
	forcefield->addEffect(forcefielded);
	tkviola->addSpecialAttack(forcefield);
	Attack* intensegravity = new Attack("INTENSE GRAVITY", "intensified gravity around", false, 15, 0, 0, 0, 0, 7, false, 17);
	intensegravity->addDescription("Intensify gravity around the enemy team, halving their defense.");
	Effect* intensegravitied = new Effect("INTENSE GRAVITY", 3, 0, 0, 1, 0.5);
	intensegravity->addEffect(intensegravitied);
	tkviola->addSpecialAttack(intensegravity);
	Attack* blackhole = new Attack("BLACK HOLE", "formed a black hole at", false, 25, 35, 100, 1, 1, 7, false, 20);
	blackhole->addDescription("Form a black hole encompassing the enemies for heavy damage. (35 DAMAGE, 100 PIERCE)");
	tkviola->addSpecialAttack(blackhole);

	NPC* greer = new NPC("BURGER EXECUTIVE", "GREER", "Greedy, high-ranking BURGER official sent to manage all the desert's remaining water.", minespring, 0, Stats(100, 40, 20, 0, 0, 20, 9), Stats(1, 2, 1, 0, 1, 0, 0));
	greer->setBoss(true);
	Attack* gun = new Attack("GUN", "shot", false, -5, 30, 50, 1, 1, 1);
	greer->setBasicAttack(gun);
	Attack* pressurevalve = new Attack("PRESSURIZED VALVE", "opened a pressurized valve, shooting a huge burst of water at", false, 5, 20, 0, 1, 1, 1);
	Effect* runningback = new Effect("RUNNING BACK", 2);
	runningback->remove = true;
	pressurevalve->addEffect(runningback);
	greer->addSpecialAttack(pressurevalve);
	Attack* gasleak = new Attack("GAS LEAK", "shot a gas main near", false, 5, 0, 0, 0, 0, 3);
	Effect* gassed = new Effect("GASSED", 3, 10, 0);
	gassed->spusage = 2;
	gasleak->addEffect(gassed);
	greer->addSpecialAttack(gasleak);
	Attack* scaldingsteam = new Attack("SCALDING STEAM", "shot a valve full of hot steam near", false, 5, 25, 50, 1, 1, 3);
	Effect* scalded = new Effect("SCALDED", 3);
	scalded->damagebuff = 2;
	scaldingsteam->addEffect(scalded);
	greer->addSpecialAttack(scaldingsteam);

	Effect* extrafire = new Effect("EXTRA ON FIRE", 3, 10, 0, 1, 0.65);
	Attack* burn = new Attack("BURN", "burned", false, 0, 0, 0, 0, 1); //contact attacks for the lavamen
	burn->addEffect(onfire);
	Attack* reallyburn = new Attack("REALLY BURN", "really burned", false, 0, 0, 0, 0, 1);
	reallyburn->addEffect(extrafire);

	NPC* magman = new NPC("", "LAVAMAN", "A really laval humanoid burning bright with radiation.", limbo, 0, Stats(20, 0, 25, 0, 25, 10, 9)); //laval is a real word but it's kind of hard to find on google, you have to clarify "laval in the context of lava"
	magman->setRecoilAttack(burn);
	Attack* lavawhack = new Attack("LAVA WHACK", "lavally whacked", true, -5, 15, 5, 1, 1, 1);
	lavawhack->addEffect(onfire);
	Attack* meteor = new Attack("METEOR", "shot a superheated boulder at", false, -5, 25, 0, 1, 1, 3);
	meteor->afterdesc = " from inside itself"; //I was thinking something like when iron man shoots a laser from his arc reactor except it's the lavaman using the rock inside itself
	magman->setBasicAttack(lavawhack);
	magman->addSpecialAttack(meteor);

	NPC* lavasoldier = new NPC("", "LAVA SOLDIER", "Lavaman rocking molten armor and a homemade bow and arrows and spear from the depths of the lava sea.", limbo, 0, Stats(25, 15, 25, 15, 30, 5, 9));
	Attack* lavaspear = new Attack("LAVA SPEAR", "speared", false, -5, 20, 5, 1, 1, 1);
	lavaspear->afterdesc = " with a lava spear";
	lavaspear->addEffect(onfire);
	Attack* lavarrows = new Attack("LAVARROWS", "shot explosive arrows", false, 8, 10, 20, 3, 3, 3); //no fire because they just explode
	lavarrows->focushits = false;
	Attack* closecombat = new Attack("CLOSE COMBAT", "engaged in close combat with", true, 8, 15, 20, 1, 1, 3);
	lavasoldier->setBasicAttack(lavaspear);
	lavasoldier->addSpecialAttack(lavarrows);
	lavasoldier->addSpecialAttack(closecombat);

	NPC* largelavaman = new NPC("", "LARGE LAVAMAN", "A really big laval humanoid who towers over his peers.\nThey have no finesse, preferring brutal strikes.", limbo, 0, Stats(60, 0, 35, 0, 25, 0, 9));
	largelavaman->setRecoilAttack(burn);
	Attack* lavaslam = new Attack("LAVA SLAM", "lavally slammed down on", true, -5, 20, 5, 1, 1, 1);
	lavaslam->addEffect(onfire);
	Attack* haymaker = new Attack("HAYMAKER", "threw a haymaker at", true, 8, 45, 5, 1, 1, 1);
	haymaker->addEffect(onfire);
	largelavaman->setBasicAttack(lavaslam);
	largelavaman->addSpecialAttack(haymaker);

	NPC* lavizard = new NPC("", "LAVIZARD", "Cute little lava gecko who frequently ingests lava to aid digestion.", limbo, 0, Stats(10, 5, 30, 5, 10, 30, 9));
	Attack* scurry = new Attack("SCURRY", "scurried all around the team", true, -5, 10, 0, 4, 5, 1);
	scurry->focushits = false;
	Attack* lavomit = new Attack("LAVOMIT", "coughed up a ball of lava at", false, 1, 20, 20, 1, 1, 1);
	lavomit->addEffect(onfire);
	lavizard->setBasicAttack(scurry);
	lavizard->addSpecialAttack(lavomit);

	NPC* poizard = new NPC("", "POIZARD", "Poisonous counterpart to the lavizard, painted a bright purple.", limbo, 0, Stats(15, 10, 30, 10, 10, 20, 9));
	Attack* poiscurry = new Attack("SCURRY", "poisonously scurried all around the team", true, -5, 10, 0, 2, 3, 1);
	Effect* poisoned = new Effect("POISONED", 5, 10, 0, 1, 0.75);
	poiscurry->addEffect(poisoned);
	poiscurry->focushits = false;
	Attack* poisomit = new Attack("POISOMIT", "coughed up a ball of sticky corrosive sludge at", false, 2, 15, 30, 1, 1, 1);
	Effect* sludged = new Effect("SLUDGED", 10, 5, 0, 1, 0.75, 0.5, 1, 0.25);
	poisomit->addEffect(sludged);
	poizard->setBasicAttack(poiscurry);
	poizard->addSpecialAttack(poisomit);

	NPC* slagman = new NPC("", "SLAGMAN", "A really slaggy humanoid formed from the factories' slag. They burn far brighter than their laval counterparts.", limbo, 0, Stats(30, 20, 35, 20, 10, 15, 9));
	slagman->setRecoilAttack(reallyburn);
	Attack* slagjab = new Attack("SLAG JAB", "slaggily jabbed at", true, -5, 20, 10, 1, 1, 1);
	slagjab->addEffect(extrafire);
	Attack* metalmeteor = new Attack("METAL METEOR", "shot a solid metal sphere at", false, 10, 40, 0, 1, 1, 1);
	metalmeteor->afterdesc = " from inside itself"; //same commentary comment as lavamen
	Attack* slagvomit = new Attack("SLAG VOMIT", "puked slag all over the team", false, 15, 20, 10, 1, 1, 7);
	slagvomit->focushits = false;
	slagvomit->addEffect(extrafire);
	slagman->setBasicAttack(slagjab);
	slagman->addSpecialAttack(metalmeteor);
	slagman->addSpecialAttack(slagvomit);

	NPC* superslagman = new NPC("", "SUPER SLAGMAN", "A really slaggy humanoid burning white-hot. Their slurry composure gives them a sick cape!", limbo, 0, Stats(50, 25, 40, 15, 15, 50, 9));
	superslagman->setRecoilAttack(reallyburn);
	Attack* slagjchu = new Attack("JAB CROSS HOOK UPPERCUT", "jabbed, crossed, hooked, and uppercutted", true, -5, 10, 10, 4, 4, 1);
	slagjchu->addEffect(extrafire);
	Attack* metalbeam = new Attack("METAL BEAM", "chucked a beam perpendicularly at", false, 8, 40, 0, 1, 1, 3);
	Attack* slagblast = new Attack("SLAG BLAST", "blasted a blast of slag at", false, 14, 30, 10, 1, 1, 7);
	slagblast->addEffect(extrafire);
	superslagman->setBasicAttack(slagjchu);
	superslagman->addSpecialAttack(metalbeam);
	superslagman->addSpecialAttack(slagblast);

	NPC* factgolem = new NPC("", "FACTORY GOLEM", "Hulking construct with a furnace core. They ceaslessly work even when submerged in lava, and double as security!", limbo, 0, Stats(140, 30, 30, 40, 10, 3, 9)); //mini mini boss?
	Attack* swing = new Attack("HEAVY HAYMAKER", "swung a heavy haymaker at", true, -5, 40, 0, 1, 1, 1);
	Attack* fistcannon = new Attack("FIST CANNON", "fired its fists like missiles", false, 3, 30, 0, 2, 2, 1); //I was gonna say heat-seeking fists but that doens't really make sense given the area it's in
	fistcannon->focushits = false;
	Attack* furnaceblast = new Attack("FURNACE BLAST", "blasted flames at", false, 6, 40, 20, 1, 1, 3);
	furnaceblast->afterdesc = " from its furnace core";
	furnaceblast->addEffect(onfire);
	Attack* exhaust = new Attack("EXHAUST", "exhausted heavy fumes", false, 6, 30, 30, 1, 1, 7);
	exhaust->focushits = false;
	Effect* smogged = new Effect("SMOGGED", 5, 8, 0, 0.75, 0.75);
	exhaust->addEffect(smogged);
	factgolem->setBasicAttack(swing);
	factgolem->addSpecialAttack(fistcannon);
	factgolem->addSpecialAttack(furnaceblast);
	factgolem->addSpecialAttack(exhaust);

	NPC* lavadile = new NPC("", "LAVADILE", "Juvenile lavagator, big from a healthy diet of ores.", limbo, 0, Stats(40, 20, 25, 30, 25, 15, 9));
	Effect* ouchie = new Effect("OUCHIE", 2, 0, 0, 1, 0.8);
	Attack* cbite = new Attack("BITE", "bit down on", true, -5, 15, 15, 1, 1, 1);
	cbite->addEffect(ouchie);
	lavadile->setBasicAttack(cbite);
	Attack* clavomit = new Attack("LAVOMIT", "coughed up lava at", false, 5, 30, 20, 1, 1, 1);
	clavomit->addEffect(onfire);
	lavadile->addSpecialAttack(clavomit);

	NPC* lavagator = new NPC("", "LAVAGATOR", "Enormous alligator inhabitant of the laval sewer systems with retro shades.", limbo, 0, Stats(150, 25, 35, 35, 35, 20, 9));
	lavagator->setBoss(true); //miniboss
	Effect* gcrunched = new Effect("CRUNCHED", 3, 0, 0, 1, 0.6);
	Attack* gcrunch = new Attack("CRUNCH", "crunched down on", true, -5, 20, 20, 1, 1, 1);
	gcrunch->addEffect(gcrunched);
	lavagator->setBasicAttack(gcrunch);
	Attack* deathroll = new Attack("DEATH ROLL", "rotated", true, 5, 8, 20, 4, 4, 1);
	deathroll->synergies.push_back(gcrunched);
	lavagator->addSpecialAttack(deathroll);
	Attack* gatorgun = new Attack("GATOR GUN", "fired a flaming beam at", true, 15, 30, 40, 1, 1, 3);
	gatorgun->afterdesc = " from its gatory core";
	gatorgun->addEffect(extrafire);
	lavagator->addSpecialAttack(gatorgun);

	NPC* lavaguardian = new NPC("", "LAVA GUARDIAN", "Huge guardian with radiant molten armor and weapons.\nHe appears to have wandered onto the bridge while the lava level was high, and now guards the gate to BURGERSBURG.", limbo, 0, Stats(200, 50, 30, 20, 20, 10, 9), Stats(2, 1, 1, 0, 0, 0, 0));
	lavaguardian->setBoss(true);
	Attack* contactexplosion = new Attack("EXPLOSION", "blew up in", false, 0, 15, 20, 1, 1, 1);
	contactexplosion->afterdesc = "'s face";
	lavaguardian->setRecoilAttack(contactexplosion);
	Attack* firesword = new Attack("FIRE SWORD", "slashed", true, -5, 20, 20, 1, 1, 1);
	firesword->afterdesc = " with its flaming sword";
	firesword->addEffect(onfire);
	lavaguardian->setBasicAttack(firesword);
	Attack* uplash = new Attack("UPSLASH", "slashed", true, 5, 20, 20, 1, 1, 1);
	uplash->afterdesc = " upwards into the air";
	Effect* uplashed = new Effect("UPSLASHED", 0); //doesn't cancel their turn but may interrupt healers wanting to heal them
	uplashed->remove = true;
	uplashed->falldamage = 30;
	uplash->addEffect(uplashed);
	lavaguardian->addSpecialAttack(uplash);
	Attack* eruption = new Attack("ERUPTION", "explosively crashed its sword down on", true, 8, 30, 20, 1, 1, 3);
	lavaguardian->addSpecialAttack(eruption);
	Attack* solarflare = new Attack("SOLAR FLARE", "heightened its radiance to a blinding extent", false, 18, 10, 20, 1, 1, 7);
	solarflare->focushits = false;
	solarflare->addEffect(blinded); //defined in graham's nat 3
	lavaguardian->addSpecialAttack(solarflare);
	Attack* gammarayburst = new Attack("GAMMA RAY BURST", "unleashed a burst of gamma radiation upon", false, 20, 40, 100, 1, 1, 3);
	lavaguardian->addSpecialAttack(gammarayburst);

	NPC* newtab = new NPC("", "NEW TAB", "Internet tabs who loyally serve their internet browser masters.", limbo, 0, Stats(10, 10, 10, 10, 10, 10, 9));
	Attack* askew = new Attack("ASKEW", "hit", true, -5, 10, 0, 1, 1, 1);
	askew->afterdesc = " askew";
	Effect* offbalance = new Effect("OFF BALANCE", 1, 0, 0, 0.8);
	askew->addEffect(offbalance);
	newtab->setBasicAttack(askew);
	Attack* doabarrelroll = new Attack("DO A BARREL ROLL", "did a barrel roll at", true, 5, 10, 0, 4, 5, 1);
	newtab->addSpecialAttack(doabarrelroll);
	Attack* adblock = new Attack("ADBLOCK", "is blocking attacks thrown at", false, 10, 0, 0, 1, 1, 1, true, 10);
	adblock->protect = true;
	adblock->prioritizeleader = true;
	newtab->addSpecialAttack(adblock);

	NPC* browser = new NPC("EVIL KING", "BROWSER", "Giant spiked internet browser with cool red hair and a penchant for kidnapping princesses.", limbo, 0, Stats(210, 20, 20, 30, 10, 20, 9), Stats(1, 0, 1, 1, 1, 0, 0));
	browser->setBoss(true);
	Attack* medge = new Attack("MICROSOFT EDGE", "sliced", true, -5, 10, 20, 1, 1, 1); //made of fine chromium
	medge->afterdesc = " with his MICROSOFT EDGE";
	browser->setBasicAttack(medge);
	Attack* gsearch = new Attack("GOOGLE SEARCH", "googled", false, 5, 0, 0, 1, 1, 1);
	gsearch->afterdesc = "'s weaknesses";
	Effect* searched = new Effect("SEARCHED", 5, 0, 0, 1, 0.5);
	gsearch->addEffect(searched);
	browser->addSpecialAttack(gsearch);
	Attack* opennewtab = new Attack("OPEN NEW TABS", "opened some new tabs", false, 8, 0, 0, 0, 0, 0);
	opennewtab->summon = newtab;
	opennewtab->summonamount = 3;
	browser->addSpecialAttack(opennewtab);
	Attack* yahoo = new Attack("YAHOO!", "went crazy", true, 7, 10, 15, 6, 6, 1);
	yahoo->focushits = false;
	browser->addSpecialAttack(yahoo);
	Attack* iexplorer = new Attack("INTERNET EXPLORER", "is buffering a powerful blast towards", false, 12, 0, 0, 1, 1, 1);
	Effect* buffering = new Effect("BUFFERING", 2);
	buffering->falldamage = 80;
	iexplorer->addEffect(buffering);
	browser->addSpecialAttack(iexplorer);
	Attack* firefox = new Attack("FIREFOX", "breathed fire upon", false, 14, 20, 30, 3, 3, 3);
	Effect* foxfire = new Effect("FOX FIRE", 5, 10, 0, 1, 0.5);
	firefox->addEffect(foxfire);
	browser->addSpecialAttack(firefox);
	plum->setDialogue({{plum, "AHHHHHHHHHH HELP ME I'VE BEEN KIDNAPPED!"}, {browser, "GWAHAHAHAHAHA!"}});
	castlethrone->setWelcome({{plum, "AHHHHHHHHHH HELP ME I'VE BEEN KIDNAPPED!"}, {browser, "GWAHAHAHAHAHA!"}});
	
	NPC* snowman = new NPC("", "SNOWMAN", "A really snowy humanoid who is very intent on beating you up.", limbo, 0, Stats(1, 1, 1, 1, 1, 1, 9), Stats(0, 0, 0, 0, 0, 0, 1));
	Attack* oopsy = new Attack("SNOW BASH", "tripped", false, 50, 15, 0, 1, 1, 1, true);
	oopsy->focushits = false;
	Attack* snowbash = new Attack("SNOW BASH", "socked", true, 1, 300, 0, 1, 1, 1); //if he's high level enough he doesn't die instantly! so maybe he can be interesting in the simulator
	snowbash->afterdesc = "snowily";
	Effect* frozed = new Effect("FROZEN", 5);
	snowbash->addEffect(frozed);
	snowman->setOpener(oopsy);
	snowman->setBasicAttack(oopsy);
	snowman->addSpecialAttack(snowbash);

	NPC* realjimshady = new NPC("", "THE REAL JIM SHADY", "He's Jim Shady, yes he's the real Shady.", limbo, 0, Stats(500, 20, 20, 35, 30, 5, 9));
	realjimshady->setBoss(true);
	Attack* shrimplecomplex = new Attack("SHRIMPLE COMPLEX", "rushed the team with the speed of a true rap shrimp", true, 0, 1, 500, 100, 100, 1);
	shrimplecomplex->focushits = false;
	Effect* tired = new Effect("RECOVERING", 1);
	shrimplecomplex->selfeffect = tired;
	tired->tiring = true;
	realjimshady->setOpener(shrimplecomplex);
	realjimshady->setBasicAttack(shrimplebeam);

	NPC* hatchling = new NPC("", "HATCHLING", "Just-hatched lizard with an instant parental bond and fighting instincts.", limbo, 0, Stats(10, 0, 7, 0, 1, 17, 4));
	Attack* filialinstinct = new Attack("FILIAL INSTINCT", "instinctually aided", false, 0, 0, 0, 1, 1, 1, true, 10);
	Effect* mcbond = new Effect("MOTHER-CHILD BOND", 2147483647, 0, 0, 1.125);
	mcbond->bond = true; //so we can check to remove the effect when the hatchling is incapacitated
	filialinstinct->addEffect(mcbond);
	filialinstinct->prioritizeleader = true;
	mcbond->stacks = true;
	hatchling->setOpener(filialinstinct);
	Attack* nip = new Attack("NIP", "tried its best to bite", true, 0, 7, 1, 1, 1, 1); //1 pierce cause tiny baby teeth
	hatchling->setBasicAttack(nip);

	NPC* bewlizard = new NPC("", "BLUE EYES WHITE LIZARD", "Legendary white lizard with blue eyes. It's known to multiply very quickly.", limbo, 0, Stats(400, 17, 24, 10, 14, 24, 24));
	bewlizard->setBoss(true);
	Attack* popbomb = new Attack("POPULATION BOMB", "pooped some eggs at the team", false, -5, 20, 0, 2, 2, 1);
	popbomb->focushits = false;
	popbomb->summon = hatchling;
	popbomb->summonamount = 2;
	bewlizard->setBasicAttack(popbomb);
	Attack* bsod = new Attack("BURST STREAM OF DESTRUCTION", "fired a stream of destruction at", false, 25, 30, 50, 1, 1, 3);
	bsod->afterdesc = " from its mouth";
	bewlizard->addSpecialAttack(bsod);

	NPC* thief = new NPC("", "THIEF", "Citizen of BURGERSBURG turned to thievery due to desperate times.", limbo, 0, Stats(10, 0, 20, 0, 20, 25, 9));
	Attack* steal = new Attack("STEAL", "swiped", true, -5, 10, 15, 1, 1, 1, false, 0, 5);
	steal->afterdesc = "'s SP";
	Attack* stab = new Attack("STAB", "stabbed", true, 5, 20, 15, 1, 1, 1);
	steal->afterdesc = " with a thief knife";
	thief->setBasicAttack(steal);
	thief->addSpecialAttack(stab);

	NPC* axeman = new NPC("", "AXEMAN", "A really deranged human whose head was exchanged for the head of an axe.", limbo, 0, Stats(40, 15, 35, 18, 50, 30, 9));
	Attack* chop = new Attack("CHOP", "swung his head at", true, -5, 15, 20, 1, 1, 1);
	Effect* chopped = new Effect("CHOPPED", 2147483647, 0, 0, 1, 1, 0.5, 1, 0.5);
	Attack* karatechop = new Attack("KARATE CHOP", "karate chopped", true, 2, 5, 0, 1, 1, 1);
	karatechop->afterdesc = "'s neck";
	karatechop->addEffect(chopped);
	Attack* axeflip = new Attack("AXEFLIP", "frontflipped at", true, 5, 30, 25, 1, 1, 1);
	axeman->setBasicAttack(chop);
	axeman->addSpecialAttack(karatechop);
	axeman->addSpecialAttack(axeflip);

	NPC* disamalg = new NPC("", "DISEASE AMALGAMATION", "Writhing mass of pathogens featuring many colors.", limbo, 0, Stats(100, 40, 5, 5, 30, 0, 9));
	Effect* tetanus = new Effect("TETANUS", 5, 8);
	Effect* commoncold = new Effect("COMMON COLD", 3, 2, 0, 1, 0.8);
	Effect* asthenia = new Effect("ASTHENIA", 5, 0, 0, 0.75, 0.5);
	Effect* brainbug = new Effect("BRAIN BUG", 5, 0, 5);
	Effect* bedridden = new Effect("BEDRIDDEN", 3);
	bedridden->tiring = true;
	Attack* contagious = new Attack("CONTAGIOUS", "spread its disease to", false, 0, 0, 0, 1, 1, 1);
	contagious->addEffect(tetanus);
	Attack* sneeze = new Attack("SNEEZE", "sneezed disease at", false, -5, 0, 0, 1, 1, 1);
	sneeze->addEffect(commoncold);
	Attack* sneeze2 = new Attack("SNEEZE", "sneezed disease at", false, 5, 0, 0, 1, 1, 1);
	sneeze2->addEffect(asthenia);
	Attack* cough = new Attack("COUGH", "coughed up disease at", false, 5, 0, 0, 1, 1, 1);
	cough->addEffect(brainbug);
	Attack* superbug = new Attack("SUPERBUG", "spit out a superbug at", false, 15, 0, 0, 1, 1, 1);
	superbug->addEffect(bedridden);
	disamalg->setRecoilAttack(contagious);
	disamalg->setBasicAttack(sneeze);
	disamalg->addSpecialAttack(sneeze2);
	disamalg->addSpecialAttack(cough);
	disamalg->addSpecialAttack(superbug);

	NPC* crimmind = new NPC("", "CRIMINAL MASTERMIND", "Psychic criminal genius with a huge brain and a body shriveled from floating everywhere.", limbo, 0, Stats(50, 0, 25, 10, 20, 25, 9));
	Attack* chuck = new Attack("CHUCKED", "telekinetically chucked some debris at", false, -5, 15, 0, 1, 1, 1);
	Effect* stretched = new Effect("STRETCHED", 3, 15);
	stretched->freeze = true;
	Attack* stretch = new Attack("STRETCHED", "is stretching", false, 7, 20, 20, 1, 1, 1);
	stretch->afterdesc = " apart in the air";
	stretch->addEffect(stretched);
	Attack* tossaside = new Attack("TOSS ASIDE", "telekinetically tossed", false, 5, 30, 0, 1, 1, 1);
	tossaside->afterdesc = " into a wall";
	Effect* tossed = new Effect("TOSSED ASIDE", 0);
	tossed->remove = true;
	tossaside->addEffect(tossed);
	tossaside->synergies.push_back(stretched);
	tossaside->cancel = suspended;
	crimmind->setBasicAttack(chuck);
	crimmind->addSpecialAttack(stretch);
	crimmind->addSpecialAttack(tossaside);

	NPC* minipanzer = new NPC("", "MINIPANZER", "Vertically challenged criminal in possession of a tiny tank, very utile for robbing banks.", limbo, 0, Stats(30, 100, 30, 50, 10, 10, 9));
	Attack* shell = new Attack("SHELL", "fired a shell at", false, -5, 20, 20, 1, 1, 1);
	Attack* rollover = new Attack("RUN OVER", "ran", true, 4, 50, 0, 1, 1, 1);
	rollover->afterdesc = " over";
	Attack* flammpanzer = new Attack("FLAMMPANZER", "spewed a stream of napalm at", false, 8, 20, 10, 1, 1, 1);
	flammpanzer->addEffect(extrafire);
	Attack* supershell = new Attack("SUPERSHELL", "fired a supershell at", false, 10, 30, 20, 1, 1, 3);
	minipanzer->setBasicAttack(shell);
	minipanzer->addSpecialAttack(rollover);
	minipanzer->addSpecialAttack(flammpanzer);
	minipanzer->addSpecialAttack(supershell);

	NPC* bagelfenagler = new NPC("", "BAGEL FENAGLER", "Hulking brute franknsteined into existence who will fenagle your bagels.\nIt wears a suit, and a sack over its head with a painted smiley face.", limbo, 0, Stats(120, 20, 30, 0, 0, 5, 9));
	Attack* slam = new Attack("SLAM", "slammed down its fists on", true, -5, 25, 0, 1, 1, 1);
	Attack* smackaside = new Attack("SMACK ASIDE", "smacked", true, 5, 20, 0, 2, 2, 1);
	smackaside->afterdesc = " into a wall";
	Effect* smacked = new Effect("SMACKED ASIDE", 0);
	smacked->remove = true;
	smackaside->addEffect(smacked);
	Attack* fenagle = new Attack("FENAGLE", "fenagled", true, 4, 0, 0, 1, 1, 1);
	fenagle->afterdesc = " with its sack";
	fenagle->take = true;
	bagelfenagler->setBasicAttack(slam);
	bagelfenagler->addSpecialAttack(smackaside);
	bagelfenagler->addSpecialAttack(fenagle);

	NPC* paveshark = new NPC("", "PAVEMENT SHARK", "Tough pavement-gray shark who stalks its prey through the streets of BURGERSBURG.", limbo, 0, Stats(20, 15, 35, 25, 50, 30, 9));
	paveshark->setShark(); //this is a shark
	Effect* submerged = new Effect("SUBMERGED", 2147483647, 0, 0, 1, 4);
	Attack* sharkbite = new Attack("SHARK BITE", "sharkily bit", true, -5, 20, 50, 1, 1, 1);
	sharkbite->selfcancel = submerged;
	sharkbite->affectselfbeforeattack = true;
	Attack* submerge = new Attack("SUBMERGE", "submerged into the pavement", false, 3, 0, 0, 0, 0, 0);
	submerge->selfeffect = submerged;
	Attack* breach = new Attack("BREACH", "jumped biting out of the pavement at", true, 5, 30, 50, 1, 1, 1);
	breach->synergies.push_back(submerged);
	breach->selfcancel = submerged;
	breach->affectselfbeforeattack = true;
	paveshark->setBasicAttack(sharkbite);
	paveshark->addSpecialAttack(submerge);
	paveshark->addSpecialAttack(breach);

	NPC* naturaldisaster = new NPC("", "NATURAL DISASTER", "Twister with an abnormally long lifespan and a collection of junk from all across BURGERSBURG.", limbo, 0, Stats(100, 30, 30, 0, 30, 50, 9));
	Effect* tailwinded = new Effect("TAILWIND", 2147483647, 0, 0, 1, 1, 1, 1, 2.5);
	Attack* tailwind = new Attack("TAILWIND", "is boosting its team's speed", false, 0, 0, 0, 1, 1, 9, true);
	tailwind->focushits = false;
	tailwind->addEffect(tailwinded);
	Attack* thingfling = new Attack("THING FLING", "flung around random debris", false, -5, 20, 0, 3, 5, 1);
	thingfling->focushits = false;
	Attack* updraft = new Attack("UPDRAFT", "sucked", false, 6, 0, 0, 1, 1, 1);
	updraft->afterdesc = " into its updraft";
	Effect* updrafted = new Effect("UPDRAFTED", 2);
	updrafted->remove = true;
	updrafted->falldamage = 40;
	updraft->addEffect(updrafted);
	Attack* earthquake = new Attack("EARTHQUAKE", "shook the ground underneath", false, 4, 30, 0, 1, 1, 7);
	Attack* supercell = new Attack("SUPERCELL", "called down another twister from a supercell", false, 12, 0, 0, 0, 0, 0);
	supercell->copyamount = 1; //duplicates itself to match the health so you don't have to fight another natural disaster all over again
	supercell->targetself = true;
	naturaldisaster->setOpener(tailwind);
	naturaldisaster->setBasicAttack(thingfling);
	naturaldisaster->addSpecialAttack(updraft);
	naturaldisaster->addSpecialAttack(supercell);

	NPC* gamblemonster = new NPC("GAMBLE MONSTER", "GAMBLONGO", "He's Gamblongo the Gamble monster and he owns the casino. He looks like a floating blob of puree with eyes.", limbo, 0, Stats(100, 20, 30, 40, 50, 70, 90), Stats(2, 2, 2, 2, 2, 2, 1));
	Attack* gonna = new Attack("ANTICIPATION", "is charging an intense attack", false, -5, 0, 0, 0, 0, 0, 0);
	Attack* get = new Attack("DEATH", "got you", false, 90, 12345678, 12345678, 1, 1, 15);
	get->focushits = false;
	gamblemonster->setBasicAttack(gonna);
	gamblemonster->addSpecialAttack(get);
	
	NPC* grandma = new NPC("EVIL GRANDMA", "MARGE", "Maniacal grandma, mortal nemesis of Ratman.\nShe is singlehandedly responsible for 10% of BURGERSBURG's robberies.", limbo, 0, Stats(150, 10, 30, 0, 30, 37, 9));
	Attack* waraxe = new Attack("WAR AXE", "axed", true, -5, 10, 20, 1, 1, 1);
	waraxe->afterdesc = " with a war axe";
	Attack* dualflail = new Attack("FLAIL", "flailed a huge dual flail around", false, 5, 18, 20, 2, 2, 1);
	waraxe->focushits = false;
	Attack* areyouhungry = new Attack("ARE YOU HUNGRY?", "fired flaming freshly baked cookies from a machine gun", false, 10, 2, 5, 8, 8, 1);
	areyouhungry->focushits = false;
	areyouhungry->addEffect(onfire);
	Attack* slipper = new Attack("SLIPPER", "threw a heat-seeking slipped at", false, 13, 20, 30, 1, 1, 1);
	Effect* severelyconcussed = new Effect("SEVERELY CONCUSSED", 3);
	severelyconcussed->tiring = true;
	slipper->addEffect(severelyconcussed);
	grandma->setBasicAttack(waraxe);
	grandma->addSpecialAttack(dualflail);
	grandma->addSpecialAttack(areyouhungry);
	grandma->addSpecialAttack(slipper);

	//Ratman is Batman MARK: Ratman (enemy)
	NPC* ratman = new NPC("", "RATMAN", "Rich vigilante wearing a dark rat suit and a cape, and a yellow utility belt.", limbo, 0, Stats(25, 30, 24, 20, 15, 23, 10), Stats(0, 1, 1, 0, 1, 0, 0));
	Effect* prepared = new Effect("PREPARED AGAINST", 3);
	ratman->setTargetEffect(prepared);
	Effect* shrouded = new Effect("SHROUDED", 5); //5 but probably gets canceled before that
	shrouded->evasive = true;
	Attack* ratarang = new Attack("RATARANG", "threw a ratarang at", false, -5, 12, 10, 1, 1, 1);
	ratarang->synergies.push_back(prepared);
	ratman->setBasicAttack(ratarang);
	Attack* mma = new Attack("MIXED MARTIAL ARTS", "engaged", true, 8, 12, 0, 3, 3, 1);
	mma->afterdesc = " in close combat";
	mma->selfcancel = shrouded;
	ratman->addSpecialAttack(mma);
	Attack* preptime = new Attack("PREP TIME", "prepared against", false, 10, 0, 0, 1, 1, 1);
	preptime->addEffect(prepared);
	preptime->redundanteffect = false; //don't prepare multiple times
	ratman->addSpecialAttack(preptime);
	Attack* smokepellet = new Attack("SMOKE PELLET", "threw a smoke pellet on the ground", false, 10, 0, 0, 0, 0, 0);
	smokepellet->selfeffect = shrouded;
	ratman->addSpecialAttack(smokepellet);
	Attack* explosivegel = new Attack("EXPLOSIVE GEL", "sprayed explosive gel on", false, 12, 0, 0, 1, 1, 1);
	explosivegel->synergies.push_back(prepared);
	Effect* abttoexplode = new Effect("READY TO BLOW", 1);
	abttoexplode->falldamage = 30;
	abttoexplode->spreadfalldamage = true;
	explosivegel->addEffect(abttoexplode);
	explosivegel->selfcancel = shrouded;
	ratman->addSpecialAttack(explosivegel);
	Attack* grapplegun = new Attack("GRAPPLE GUN", "grappled", false, 12, 7, 0, 1, 1, 1);
	grapplegun->afterdesc = "'s legs and flung them away";
	Effect* grappled = new Effect("GRAPPLED AWAY", 1);
	grappled->remove = true;
	grapplegun->addEffect(grappled);
	ratman->addSpecialAttack(grapplegun);
	Attack* taserfinger = new Attack("TASER FINGER", "tased", false, 16, 12, 10, 1, 1, 1);
	taserfinger->synergies.push_back(prepared);
	taserfinger->afterdesc = " with his taser finger";
	Effect* tased = new Effect("TASED", 3);
	tased->freeze = true;
	taserfinger->addEffect(tased);
	taserfinger->selfcancel = shrouded;
	ratman->addSpecialAttack(taserfinger);
	Attack* sharkrepellant = new Attack("SHARK REPELLANT", "sprayed shark repellant at", false, 5, 100, 100, 1, 1, 1); //use 5 sp cause I don't want it to affect weights in normal fights too much
	sharkrepellant->instakill = true;
	sharkrepellant->targetshark = true;
	sharkrepellant->selfcancel = shrouded;
	ratman->addSpecialAttack(explosivegel);

	NPC* richperson = new NPC("", "RICH PERSON", "A really rich BURGER shareholder who loves only his monies.", limbo, 0, Stats(15, 1, 5, 0, 0, 9, 9));
	Attack* brassknuckles = new Attack("BRASS KNUCKLES", "swung at", true, -5, 20, 0, 1, 1, 1);
	brassknuckles->afterdesc = " with brass knuckles";
	Attack* chairthrow = new Attack("CHAIR", "threw a nearby chair at", false, 6, 40, 0, 1, 1, 1);
	Effect* ouchknees = new Effect("OUCH MY KNEES", 2, 0, 0, 1, 0.9);
	Attack* canecrunch = new Attack("CANE CRUNCH", "hit", true, 15, 30, 0, 1, 1, 1);
	canecrunch->afterdesc = "'s knees with his rich person cane";
	richperson->setBasicAttack(brassknuckles);
	richperson->addSpecialAttack(chairthrow);
	richperson->addSpecialAttack(canecrunch);

	NPC* richcyborg = new NPC("", "RICH CYBORG", "A really rich BURGER shareholder who has traded some of his weak human flesh for metal.", limbo, 0, Stats(30, 30, 20, 25, 15, 18, 9));
	Attack* metalknuckles = new Attack("METAL KNUCKLES", "struck", true, -5, 20, 0, 1, 1, 1);
	metalknuckles->afterdesc = " with metallic fists";
	Attack* fingergun = new Attack("FINGER GUN", "shot at", false, 5, 15, 40, 2, 2, 1);
	fingergun->afterdesc = "with his integrated weaponry";
	Attack* runover = new Attack("RUN OVER", "flew into", true, 12, 35, 0, 1, 1, 1);
	runover->afterdesc = " with his integrated jet pack";
	richcyborg->setBasicAttack(metalknuckles);
	richcyborg->addSpecialAttack(fingergun);
	richcyborg->addSpecialAttack(runover);

	NPC* burgerlawyer = new NPC("", "BURGER LAWYER", "A really corrupt lawyer, rich from covering up the BURGER corporation's crimes.", limbo, 0, Stats(20, 15, 10, 5, 0, 5, 9));
	Attack* briefcasebash = new Attack("BRIEFCASE BASH", "bashed", true, -5, 25, 0, 1, 1, 1);
	briefcasebash->afterdesc = " with his briefcase";
	Effect* legalimmunity = new Effect("LEGAL IMMUNITY", 2);
	legalimmunity->invincible = true;
	Attack* legaldefense = new Attack("LEGAL DEFENSE", "legally defended", false, 7, 0, 0, 1, 1, 1, true);
	legaldefense->addEffect(legalimmunity);
	Effect* enjoined = new Effect("ENJOINED", 2);
	enjoined->freeze = true;
	Attack* injunction = new Attack("INJUNCTION", "injuncted", false, 7, 0, 0, 1, 1, 1);
	injunction->afterdesc = "'s right to battle";
	injunction->addEffect(enjoined);
	burgerlawyer->setBasicAttack(briefcasebash);
	burgerlawyer->addSpecialAttack(legaldefense);
	burgerlawyer->addSpecialAttack(injunction);
	
	NPC* burgeragent = new NPC("", "BURGER AGENT", "Security guard of the BURGER corporation, dripped out in suit and sunglasses.", limbo, 0, Stats(45, 25, 20, 20, 10, 18, 9));
	Attack* nightstick = new Attack("NIGHTSTICK", "thwacked", true, -5, 20, 25, 1, 1, 1);
	nightstick->afterdesc = " with a nightstick";
	Attack* beatdown = new Attack("BEATDOWN", "beat up", true, 4, 10, 0, 3, 3, 1); //funny cause up and down are opposites but here they mean the same thing
	Effect* tackled = new Effect("TACKLED", 0);
	Effect* tackling = new Effect("TACKLING", 0);
	tackled->remove = true;
	tackling->remove = true;
	Attack* tackle = new Attack("TACKLE", "tackled", true, 5, 24, 0, 1, 1, 1);
	tackle->addEffect(tackled);
	tackle->selfeffect = tackling;
	burgeragent->setBasicAttack(nightstick);
	burgeragent->addSpecialAttack(beatdown);
	burgeragent->addSpecialAttack(tackle);

	NPC* burgerbutler = new NPC("", "BURGER BUTLER", "Robot butler of the BURGER corporation. Looks like a big suited rice cooker on wheels.", limbo, 0, Stats(100, 50, 15, 30, 0, 0, 9));
	burgerbutler->setBoss(true); //miniboss
	Attack* butlerhands = new Attack("BUTLER HANDS", "threw out two punches with its robotic extendable hands", true, -5, 15, 0, 2, 2, 1);
	butlerhands->focushits = false;
	Attack* tableflip = new Attack("TABLE FLIP", "flipped one of the fancy tables onto", false, 7, 23, 0, 1, 1, 3);
	Attack* chandelier = new Attack("CHANDELIER", "ripped a chandelier from the ceiling and crashed it down onto", true, 20, 40, 0, 1, 1, 1);
	Attack* vacuum = new Attack("VACUUM", "sucked up", true, 13, 20, 15, 1, 1, 1);
	vacuum->afterdesc = " with its vacuum";
	vacuum->take = true;
	burgerbutler->setBasicAttack(butlerhands);
	burgerbutler->addSpecialAttack(tableflip);
	burgerbutler->addSpecialAttack(chandelier);
	burgerbutler->addSpecialAttack(vacuum);
	
	NPC* ceo = new NPC("BURGER CEO", "ENZO", "The CEO of the whole BURGER COROPORATION.\nHe's rejected his humanity in favor of the lethal efficiency of machines.", limbo, 0, Stats(1000, 25, 25, 50, 50, 25, 9), Stats(0, 0, 1, 0, 1, 1, 0));
	ceo->setBoss(true);
	Effect* lockedon = new Effect("LOCKED ON", 4); //does nothing
	Attack* lockon = new Attack("LOCK ON", "locked onto", false, -5, 0, 0, 1, 1, 1);
	lockon->addEffect(lockedon);
	Effect* defensemode = new Effect("DEFENSE MODE", 2147483647, 1, 2.0);
	Effect* assaultmode = new Effect("ASSAULT MODE", 2147483647, 2.0, 1);
	Attack* minigun = new Attack("MINIGUN", "gunned down", false, 4, 6, 15, 6, 6, 1);
	minigun->afterdesc = " with his minigun";
	minigun->synergies.push_back(lockedon);
	minigun->selfeffect = defensemode;
	minigun->selfcancel = assaultmode;
	minigun->affectselfbeforeattack = true;
	Attack* flamethrower = new Attack("FLAMETHROWER", "torched", false, 8, 20, 10, 2, 2, 1);
	flamethrower->afterdesc = " with his flamethrower";
	flamethrower->synergies.push_back(lockedon);
	flamethrower->selfeffect = defensemode;
	flamethrower->selfcancel = assaultmode;
	flamethrower->affectselfbeforeattack = true;
	flamethrower->addEffect(onfire);
	Attack* bombard = new Attack("BOMBARD", "bombarded the team with his missile launchers", false, 13, 20, 20, 4, 4, 3);
	bombard->focushits = false;
	bombard->synergies.push_back(lockedon);
	bombard->selfeffect = assaultmode;
	bombard->selfcancel = defensemode;
	bombard->affectselfbeforeattack = true;
	Attack* megagun = new Attack("MEGAGUN", "gunned down", false, 15, 20, 0, 3, 3, 1);
	megagun->afterdesc = " with big bullets";
	megagun->synergies.push_back(lockedon);
	megagun->selfeffect = assaultmode;
	megagun->selfcancel = defensemode;
	megagun->affectselfbeforeattack = true;
	ceo->setEffect(defensemode, NULL);
	ceo->setBasicAttack(lockon);
	ceo->addSpecialAttack(minigun);
	ceo->addSpecialAttack(flamethrower);
	ceo->addSpecialAttack(bombard);
	ceo->addSpecialAttack(megagun);

	NPC* burgerwarden = new NPC("", "BURGER WARDEN", "Burly masked man with a BURGER insignia tasked with guarding the BURGER BASEMENT.", limbo, 0, Stats(300, 25, 23, 40, 30, 10, 9));
	burgerwarden->setBoss(true);
	Attack* spikedfists = new Attack("SPIKED FISTS", "punched", true, -5, 20, 30, 1, 1, 1);
	spikedfists->afterdesc = " with a heavy spiked fist";
	Attack* whip = new Attack("SCOURGE", "whipped", false, 5, 30, 40, 1, 1, 1);
	whip->afterdesc = " with a scourge";
	Effect* whipped = new Effect("SCOURGED", 2);
	whipped->tiring = true;
	whip->addEffect(whipped);
	Attack* guillotine = new Attack("GUILLOTINE", "flung a guillotine blade at", false, 7, 100, 100, 1, 1, 1);
	guillotine->instakill = true;
	burgerwarden->setBasicAttack(spikedfists);
	burgerwarden->addSpecialAttack(whip);
	burgerwarden->addSpecialAttack(guillotine);

	NPC* burgercultist = new NPC("", "BURGER CULTIST", "Worshipper of BURGER aligned with its evil values. They wear a yellow robe and triangular hood.", limbo, 0, Stats(50, 15, 35, 5, 60, 0, 9));
	//burger magic (damage)
	//sorcery (sp leak)
	//curse (very heavy dot)

	NPC* carnplant = new NPC("", "CARNIVOROUS PLANT", "Really big plant who likes eating meat.", limbo, 0, Stats(25, 6, 15, 5, 15, 7, 9));
	Effect* macerated = new Effect("MACERATED", 4, 9, 0, 1, 1, 1, 1, 0.25);
	Attack* snaptrap = new Attack("SNAP TRAP", "snapped its fangs upon", true, -5, 15, 15, 1, 1, 1);
	snaptrap->synergies.push_back(macerated);
	Attack* pitcherfluid = new Attack("PITCHER FLUID", "coughed up sticky pitcher fluid at", true, 6, 8, 30, 1, 1, 3);
	pitcherfluid->addEffect(macerated);
	Attack* sundew = new Attack("SUNDEW TENDRIL", "wrapped a sundew tendril around", true, 9, 5, 20, 1, 1, 1, false, 0, 0, 1);
	Effect* sundewed = new Effect("SUNDEWED", 5, 12);
	sundewed->lifesteal = 1;
	sundew->addEffect(sundewed);
	carnplant->setBasicAttack(snaptrap);
	carnplant->addSpecialAttack(pitcherfluid);
	carnplant->addSpecialAttack(sundew);

	NPC* smogfish = new NPC("", "SMOGFISH", "Floating many-finned fish of purple smog who fights in the form of others.", limbo, 0, Stats(10, 0, 10, 0, 0, 18, 9));
	Attack* copycat = new Attack("COPYCAT", "transformed into", false, 0, 0, 0, 0, 0, 1);
	copycat->transformtotar = true;
	copycat->prioritizenonleader = true; //don't transform into the player if possible to make sense of self stand out more
	smogfish->setBasicAttack(copycat);

	NPC* junglenaut = new NPC("", "JUNGLENAUT", "Armored juggernaut of the jungle, puppeted by vines wrapped round about and through its helmet.", limbo, 0, Stats(250, 30, 10, 30, 4, 25, 9));
	Attack* fullsteam = new Attack("FULL STEAM", "charged full steam at", true, -5, 22, 0, 1, 1, 1);
	Effect* knockedaway = new Effect("KNOCKED AWAY", 0);
	knockedaway->remove = true;
	fullsteam->addEffect(knockedaway);
	Attack* vinelash = new Attack("VINE LASH", "lashed its vines around", true, 7, 19, 5, 2, 2, 1);
	vinelash->focushits = false;
	Attack* constrict = new Attack("CONSTRICT", "constricted", true, 12, 10, 0, 1, 1, 1);
	constrict->afterdesc = " with its vines";
	Effect* constricted = new Effect("CONSTRICTED", 2, 10);
	constricted->freeze = true;
	Attack* piledrive = new Attack("PILEDRIVE", "piledrove", true, 12, 30, 6, 1, 1, 1);
	Effect* piledriven = new Effect("FRACTURED", 4, 0, 0, 1, 0.5);
	piledrive->addEffect(piledriven);
	constrict->addEffect(constricted);
	junglenaut->setBasicAttack(fullsteam);
	junglenaut->addSpecialAttack(vinelash);
	junglenaut->addSpecialAttack(constrict);
	junglenaut->addSpecialAttack(piledrive);

	/*Attack* punch = new Attack("PUNCH", "punched", true, -5, 10, 0, 1, 1, 1);
	punch->addDescription("Throw a simple punch at the target. (10 ATTACK)");
	self->setBasicAttack(punch);

	Attack* energyball = new Attack("ENERGY BALL", "threw an energy ball at", 3, 12, 10, 1, 1, 1, false, 1);
	energyball->addDescription("Throw a piercing ball of pure kinetic energy at the target. (12 ATTACK, 10 PIERCE)");
	self->addSpecialAttack(energyball);

	Attack* kick = new Attack("KICK", "jumped at", true, 6, 15, 0, 1, 1, 1, false, 3);
	kick->afterdesc = " with a kick";
	kick->addDescription("Launch a flying side kick at the target. (15 ATTACK)");
	self->addSpecialAttack(kick);

	Attack* headbutt = new Attack("HEADBUTT", "flew at", true,  5, 25, 0, 1, 1, 1, false, 5);
	headbutt->afterdesc = " like a missile";
	headbutt->recoil = 5;
	headbutt->addDescription("Deal a strong hit with your head, but it kind of hurts. (25 ATTACK)");
	self->addSpecialAttack(headbutt);

	Attack* bigenergyball = new Attack("BIG ENERGY BALL", "threw a big ball of energy at", false, 10, 20, 10, 1, 1, 1, false, 8);
	bigenergyball->addDescription("Throw a large mass of energy at the target and their surrounding allies. (20 ATTACK, 10 PIERCE)");

	Attack* punchflurry = new Attack("FLURRY RUSH", "rushed", true, 7, 5, 0, 6, 7, 1, false, 10);
	punchflurry->addDescription("Unleash a barrage of 6 to 7 punches. (5 ATTACK, 6-7 hits)");
	self->addSpecialAttack(punchflurry);*/

	//energize
	
	//sp bomb

	//entering temples requires having max size party
	//temples scale to your level
	//you can't exit until you beat the boss (or you use the give up exit which resets everything?)
	//Stats(20, 5, 6, 0, 0, 10, 9), Stats(1, 0, 1, 0, 0, 1, 0)
	NPC* senseofself = new NPC("", "SENSE OF SELF", "He looks like yourself, with a cool scarf and blond anime hair except taller.", limbo, 0, Stats(5500, 12, 20, 5, 0, 12, 9), Stats(2, 0, 1, 1, 0, 1, 0));
	senseofself->setBoss(true);
	//temple of humility in the forest, gives output antenna of humility
	//forest temple is pretty standard, with some branching paths and combat and choices that allow you to buff yourself or buff your teammates
	//and probably a puzzle or two (prolly one)
	//sos starts by copying your attacks but better,
	//then tempts teammates into joining him (prize), effectiveness depends on how many times you helped yourself
	//"He only cares about himself."
	//"He doesn't deserve you!"
	//"Why don't you join the better side?"
	//you can use your attack APOLAGIZE TO to ask teammates to come back
	//near the end any remaining hypnotized teammates are sent back by sos and he tells you

	//MARK: make the description change depending on if he's on your side or not
	//disguised desc: He's another you! What could be better than that?

	//MARK: IMPORTANT:
	//force use temptation at two points (90% hp and 77% hp)
	//before the second point, do not target player if theyre below 25% hp
	//phase 1: until 90% hp: just show off with cooler version of basic plr attacks
	//phase 2: until 30% hp: bulk of the battle with previous moveset but also temptation
	//phase 3: do the final player temptation and switch to more advanced player moveset (energize player while on their team, do not target player if on that team)
	//if plr gets to 0 hp, sustain them at 1 hp (sos's energize should do that) so they can do the choice
	//MARK: DO NOT TARGET NPC AGAIN IF THE EFFECT FAILED DUE TO IMMUNITY

	//4 different 2 sp use temptation attacks, one base, 3 from choice orbs
	//NULL, "He only cares about himself; don't you remember how he chose to buff himself over you?"
	//NULL, "Why don't you join the better side? We got plenty of team buffs here!"

	//NULL, "He only cares about himself, don't you remember?"
	//NULL, "He chose to debuff you guys 'cause he could NEVER take one for the team..."
	//NULL, "Why don't you join the better side? You'll never get debuffed here!"

	//NULL, "Why are you with him?"
	//NULL, "He can't help but be the star of the show!"
	//NULL, "Keeping the cool big buff for himself at your expense!"
	//NULL, "Join the better side! There's plenty of stardom to share here!"

	//basic:
	//NULL, "Why are you on his side?"
	//NULL, "You could do so much better; he doesn't deserve you!"
	//NULL, "Come join the better side!"

	//cool punch

	//cool energy ball

	//temptation

	//super side kick

	//turbo punch flurry

	//super swaggy enegry ball

	//hindsight 20/20
	//senseofself, "Well they're all incapacitated."
	//senseofself, "All the credit goes to meee! >B)"
	//self, "what?"
	//NULL, "You blow up behind SENSE OF SELF as he looks away"
	//NULL, "because it's cooler not to look at explosions."

	//NULL, "SENSE OF SELF shrinks down to your height."
	//OR
	//NULL, "SENSE OF SELF catches you as you fall."
	//SENSE OF SELF used ENERGIZE!
	//SENSE OF SELF is maintaining SELF's hp!

	//senseofself, "Hey man,"
	//senseofself, "I'm sorry about this whole 'you but better' shtick."
	//senseofself, "I just wanted to show you,"
	//senseofself, "You can't rely on these people!"
	//senseofself, "Look how quick they were to betray you!"
	//senseofself, "You don't need them!"
	//senseofself, "But what could be better..."
	//senseofself, "than two of you?!"
	//senseofself, "Help me finish them off, and we can defeat BURGER together!"
	//NULL, "Join SENSE OF SELF? (YES or NO)"
	//hey it was all just to show you you can't rely on these ppl
	//join me and WE can defeat BURGER together!
	//if you join him he just instakills you after you defeat your other teammates
	//you can also GO BACK if you haven't defeated them all yet
	//then you get the antenna

	//{{forestknight, "Quiet, fiend!"}, {forestknight, "I will not betray my compatriots!"}, {senseofself, "Whatever, you're the least of your team anyway..."}}

	NPC* shadowcreature = new NPC("", "SHADOW CREATURE", "Lanky creature of darkness that chips away at people's strength.", limbo, 0, Stats(30, 0, 10, 0, 20, 30, 9));
	Effect* decayed;
	//shadow slap

	NPC* masky = new NPC("", "MASKY", "Plain white mask floating in the air who warps the world depending on its fluctuating mood.\nIts resting expression is ¦|", limbo, 0, Stats(20, 25, 7, 45, 4, 11, 9));
	//:D - heal teammate, buff teammate (unchip stats), or make teammate radiant
	//D: - damage enemy, big chip enemy stats, or turn them into stone

	NPC* pyramid = new NPC("", "PYRAMON", "Floating layered pyramid from the stars that sees all that is right in front of it, much like a normal eye.", limbo, 0, Stats(250, 30, 10, 30, 4, 25, 9));
	//pyradon
	//laser
	//hypnotize or something

	NPC* thedark = new NPC("", "THE DARK", "The face of the darkness that haunts people's nightmares.", limbo, 0, Stats(8000, 10, 5, 50, 10, 0, 9), Stats(2, 1, 0, 2, 0, 0, 0));
	thedark->setBoss(true);
	//temple of hope in the desert, gives big red button of hope
	//you get seperated from your teammates and you have to fight enemies that decay your stats
	//you need to get three light orbs to open the way down by doing puzzles and fighting and stuff
	//also each light orb you put in returns one teammate to you
	//the dark decays your stats with its attacks
	//and when stats are low enough it can tempt teammates into despair
	//and you can ENCOURAGE teammates to unfreeze them (cause despair is just freeze)
	//after that you get the button

	//jumpscare

	NPC* bolide;
	//impact

	NPC* firefly;
	//firethrower
	//

	NPC* monkeystatue;
	NPC* infernobo;
	//infernobo
	//
	//
	//nova cannon

	NPC* firewithfire = new NPC("", "FIRE WITH FIRE", "Humanoid formed of flowing fire, who really loves getting people mad.", limbo, 0, Stats(6000, 15, 10, 0, 10, 14, 9), Stats(2, 0, 1, 0, 1, 1, 0));
	NPC* firewithfir2 = new NPC("", "FIRE WITH FIRE", "Humanoid formed of flowing fire, burning a bright blue.", limbo, 0,               Stats(6000, 15, 20, 0, 15, 21, 9), Stats(2, 0, 2, 0, 1, 2, 0));
	NPC* firewithfir3 = new NPC("", "FIRE WITH FIRE", "Humanoid formed of flowing fire, burning an intense purple hue.", limbo, 0,       Stats(6000, 15, 30, 0, 20, 28, 9), Stats(2, 0, 3, 0, 2, 3, 0));
	firewithfire->setBoss(true);
	//temple of patience in the volcano area, gives plotometer of patience
	//you do puzzles and fight fire enemies
	//then you fight fire with fire
	//he says stuff that tempts you and your teammates into wrath
	//wrath gives x1.5 attack but wrath attacks also increase phase meter of fwf
	//you can CALM DOWN but you lose the boost
	//fwf increases in heat up to two times after getting wrathfully hit enough, and in the third phase all attacks add to his attack (so you can't just attack wrathfully with no consequences then)
	//teammates calm down naturally after a few turns (and increase wrath meter less), but you can CALM DOWN them so they calm down faster (5 by default probably)
	//then you get the plotometer

	//burny burn (recoil)

	//firefight "fought"" with fire"
	//ragebait

	//firefight
	//spark "flicked an explosive spark at"
	//ragebait

	//spark
	//ragebait
	//flame

	//MARK: floria would just heal more? prolly don't allow beneficial moves
	Attack* ragebait;
	//self - {{firewithfire, "Look at this shorty."}, {self, "WHAT"}, {firewithfire, "Yeah like why are you so short?"}, {firewithfire, "What even are you? 3'4?"}, {self, "NO! >:|"}, {firewithfire, "True, generous estimate."}, {self, "SHUT UP >:|"}}
	//floria - {{firewithfire, "Hey Floria your hat looks stupid."}, {firewithfire, "You're stupid."}, {floria, "WHAT?"}, {floria, "YOU MEANIE!"}}
	//egadwick - {{firewithfire, "Who let this fossil into the team?"}, {firweithfire, "So much age and yet all his life amounts to is making science textbooks more annoying to read."}, {firewithfire, "What a nerd..."}, {egadwick, "You don't understand the marvelousness of science!"}}
	//absolom - {{firewithfire, "Here we have the mighty forest knight..."}, {firewithfire, "No stronger than a shrimp, though!"}, {firewithfire, "BAHAHAHAHAHAHAHA!"}, {forestknight, "Silence, fiend!"}, {forestknight, "You shall never tempt me!"}, {firewithfire, "Hmmmm! >:("}, {forestknight, "Come on, friends!"}, {forestknight, "Don't fall for his provocations!"}}
	//viola - {{firewithfire, ""}} comment about kidnapping
	//mike - {{firewithfire, ""}} taunt about mine collapse?
	//cacty - {{NULL, "FIRE WITH FIRE - *insults Cacty's mother in cactus language*"}, {NULL, "CACTY - *furious cactus noises*"}}
	//michelin - {{firewithfire, ""}} insults chef abilities
	//carlos - {{firewithfire, ""}} insults failure to hack mircosoft
	//plum - {{firewithfire, "Oh you must be the famous fungus princess."}} etc.
	//graham - {{firewithfire, "Look at this bozo."}, {firewithfire, "Everyone has a clear role in this team, but you?"}, {firewithfire, "What's even your purpose?"}, {firewithfire, "The gambling addict?"}, {firewithfire, "So useless BAHAHAHA"}, {graham, "Hey.... you shut up! >:("}}
	//richie - {{firewithfire, "Here we have the rich dirtbag of the group..."}, {richie, ">:O"}, {firewithfire, "Bro stop hogging all your monies to yourself!"}, {richie, "I'll have you know I donate! >:("}, {firewithfire, "Yeah I'm sure you do..."}, {firewithfire, "Even if you did for you it's the same as donating penny shavings..."}, {richie, "STOP! >:("}}
	//ratman - {{firewithfire, "Ratman!"}, {firewithfire, "..."}, {firewithfire, "Yeah you're just a weirdo in a rat costume."}, {ratman, "I will not tolerate this slander."}, {ratman, "Because I'm Ratman."}}
	Effect* wrath;
	
	//10000, Stats(6000000, 60000, 30000, 60000, 60000, 60000, 9000), Stats(20, 2, 1, 2, 2, 2, 0)
	burgermenace = new NPC("", "THE BURGER MENACE", "", limbo, 0, Stats(), Stats());
	//burger tendril
	//

	//burger boom

	NPC* adversary = new NPC("", "THE ADVERSARY", "", limbo, 0, Stats(), Stats());
	//


	//MARK: make sure all the 0 hit and 0 target moves actually make sense with that
	//MARK: probably no attack should have 0 damage and non-0 hits
	//MARK: all 0 target attacks should be for affecting self or summoning stuff like that, nothing to do with the other team (except makinhg a summon for them)
	//MARK: make sure NO ATTACKS accidentally don't have a bool as the third slot

	//set up overworld enemies  MARK: enemies (world)
	NPC* forestguard = new NPC(*grassman);
	forestguard->setLeader(true, 1, forestentrance);
	forestguard->blockExit(NORTH, ENEMY, "guarded by the GRASSMAN.");
	forestguard->setDialogue({{NULL, "GRASSMAN - *angry bush noises*"}});
	forestguard->addRejectionDialogue({{NULL, "GRASSMAN - *angry bush noises*"}});

	NPC* deerguard = new NPC(*enemydeer);
	deerguard->setLeader(true, 3, forestleft);
	deerguard->blockExit(WEST, ENEMY, "guarded by the ENEMY DEER.");
	deerguard->setDialogue({{NULL, "ENEMY DEER - *angry deer noises*"}});
	deerguard->addRejectionDialogue({{NULL, "ENEMY DEER - *angry deer noises*"}});

	NPC* ninjaguard = new NPC(*ninjascout);
	ninjaguard->setLeader(true, 5, forestright);
	ninjaguard->blockExit(NORTHEAST, ENEMY, "guarded by the NINJA SCOUT.");
	ninjaguard->setDialogue("You will never get past me!!!!!!! >:D");
	ninjaguard->addRejectionDialogue("No!!! I will always be a ninja!!!!!! >:D");

	NPC* ninjaguard1 = new NPC(*ninja);
	ninjaguard1->setLeader(true, 15, ninjavillage, false);
	ninjaguard1->blockExit(IN_HOUSE_1, ENEMY, "guarded by the NINJA.");
	ninjaguard1->setDialogue("You won't get past me, outsider.");
	ninjaguard1->addRejectionDialogue("I will never abandon the ninja way.");

	NPC* ninjaguard2 = new NPC(*ninja);
	ninjaguard2->setLeader(true, 15, ninjavillage, false);
	ninjaguard2->blockExit(IN_HOUSE_2, ENEMY, "guarded by the NINJA.");
	ninjaguard2->setDialogue("Fight me if you wish to see the true power of the ninja way!");
	ninjaguard2->addRejectionDialogue("I will never abandon the ninja way.");

	NPC* ninjaguard3 = new NPC(*ninja);
	ninjaguard3->setLeader(true, 15, ninjavillage, false);
	ninjaguard3->blockExit(IN_HOUSE_3, ENEMY, "guarded by the NINJA.");
	ninjaguard3->setDialogue("How could you stand before our ninja chief if you can't even beat me?");
	ninjaguard3->addRejectionDialogue("I will never abandon the ninja way.");

	NPC* pantryguard = new NPC(*ninjachef);
	pantryguard->setLeader(true, 17, ninjapantry);
	pantryguard->addConversation({{self, "Hi ninja man can I have that ninjaberry?"}, {ninjachef, "Only if you prove yourself in combat."}, {self, "Sounds good."}});
	pantryguard->setDialogue("Prove yourself in combat to prove yourself worthy of the ninjaberry.");
	pantryguard->addLinkedDialogue(pantryguard, {{pantryguard, "Well done, young one."}, {pantryguard, "You have proven yourself worthy of the ninjaberry."}, {self, "Thanks."}});
	pantryguard->setTalkOnDefeat();
	pantryguard->addRejectionDialogue("I cannot abandon the ninja way; I must continue my culinary service to my ninja clan.");
	pantryguard->guardItem(ninjaberry);

	NPC* govguard = new NPC(*ninjachief);
	govguard->setLeader(true, 19, ninjacapitol);
	govguard->setParty({ninja, ninja});
	govguard->setDialogue({{ninjachief, "You have improved your ninja abilities at an accelerated rate."}, {ninjachief, "Now, fight me as your final test."}});
	govguard->addConversation({{ninjachief, "You have improved your ninja abilities at an accelerated rate."}, {ninjachief, "Most impressive."}, {ninjachief, "Now, fight me as your final test."}}); //just so he says that before the fight
	govguard->addRejectionDialogue("I must continue to govern the ninja village.");
	govguard->addLinkedDialogue(govguard, {{govguard, "Well done, young one."}, {govguard, "You are a most impressive ninja."}});
	govguard->addLinkedConvo(govguard, {{govguard, "Well done, young one."}, {govguard, "You are a most impressive ninja."}, {govguard, "You've earned yourself access to our most secret ninja scroll."}, {self, "Oh thanks :D"}});
	govguard->setTalkOnDefeat();
	Item* ninjascroll = new EducationItem("ADVANCED NINJA SCROLL", "An old golden scroll detailing advanced ninja techniques. Most ninjas never even see this scroll their whole lives.", limbo, smokebomb);
	EducationItem* _ninjascroll = (EducationItem*)ninjascroll;
	_ninjascroll->setAttack(pacupunctuken);
	_ninjascroll->setAttack(pdeadlyspinferno);
	govguard->setGift(ninjascroll, true);

	NPC* forestguard2 = new NPC(*buffgrassman);
	forestguard2->setLeader(true, 2, foresttempleentrance);
	forestguard2->setParty({pricklyhog});
	forestguard2->blockExit(NORTH, ENEMY, "guarded by the BUFF GRASSMAN.");
	forestguard2->setDialogue({{NULL, "BUFF GRASSMAN - *burly bush noises*"}});
	forestguard2->addRejectionDialogue({{NULL, "*BUFF GRASSMAN - haughty bush noises*"}});

	NPC* hogguard = new NPC(*greaterhog);
	hogguard->setLeader(true, 3, forestfork);
	hogguard->blockExit(NORTHEAST, ENEMY, "guarded by the GREATER HOG.");
	hogguard->setDialogue({{NULL, "GREATER HOG - *angry squeal*"}});
	hogguard->addRejectionDialogue({{NULL, "GREATER HOG - *angry squeal*"}});

	NPC* hogguard2 = new NPC(*greaterhog);
	hogguard2->setLeader(true, 3, forestspork);
	hogguard2->setParty({pricklyhog, pricklyhog});
	hogguard2->blockExit(NORTHWEST, ENEMY, "guarded by the GREATER HOG.");
	hogguard2->setDialogue({{NULL, "GREATER HOG - *angry squeal*"}});
	hogguard2->addRejectionDialogue({{NULL, "GREATER HOG - *angry squeal*"}});

	NPC* forestguard3 = new NPC(*buffgrassman);
	forestguard3->setLeader(true, 4, forestnice);
	forestguard3->setParty({grassman, grassman});
	forestguard3->blockExit(EAST, ENEMY, "guarded by the BUFF GRASSMAN.");
	forestguard3->setDialogue({{NULL, "BUFF GRASSMAN - *burly bush noises*"}});
	forestguard3->addRejectionDialogue({{NULL, "*BUFF GRASSMAN - haughty bush noises*"}});

	NPC* jimshady1 = new NPC(*jimshady);
	jimshady1->setLeader(true, 5, forestwall);
	jimshady1->blockExit(EAST, ENEMY, "blocked by JIM SHADY.");
	jimshady1->setDialogue("I'm JIM SHADY, yes I'm the REAL SHADY!");
	jimshady1->addConversation({{jimshady1, "I'm JIM SHADY, yes I'm the REAL SHADY!"},
								{self, "No you aren't."},
								{jimshady1, "Shut up."}});
	jimshady1->addRejectionDialogue("No go away.");

	NPC* roguerobot = new NPC(*egadbot);
	roguerobot->setLeader(true, 3, forestgarden);
	roguerobot->addRecruitLink(egadwick);
	roguerobot->addLinkedConvo(egadwick, {{egadwick, "I'm no longer detecting signals from my robot. Did you by chance stop it?"},
									   {self, "Yep I did."},
									   {egadwick, "Oh thank goodness! Thanks a bunch, kiddo!"},
									   {egadwick, "Now I can safely be in the great outdoors!"}});
	roguerobot->setDialogue({{NULL, "EGARDENBOT - *rogue snipping noises*"}});
	roguerobot->addRejectionDialogue({{NULL, "EGARDENBOT - *rogue snipping noises*"}});

	NPC* flowerguard = new NPC(*flowerfiend);
	flowerguard->setLeader(true, 3, flowerfield);
	flowerguard->blockExit(WEST, ENEMY, "blocked by the FLOWER FIEND.");
	flowerguard->setDialogue({{NULL, "FLOWER FIEND - *flowery shriek*"}});
	flowerguard->addRejectionDialogue({{NULL, "FLOWER FIEND - *flowery shriek*"}});

	NPC* mimic1 = new NPC(*mimic);
	mimic1->setLeader(true, 30, NULL, false);
	mimic1->setDialogue({{NULL, "MIMIC - *UNHINGED ROAR*"}}); //you see it's funny because chests have hinges
	mimic1->addRejectionDialogue({{NULL, "MIMIC - *UNHINGED ROAR*"}});
	mimic1->setEscapable(false);
	mimic1->setForceBattle();
	mimic1->setExtraMonies(1000); //you get lots of monies for beating the mimic

	Item* fakechest = new TreasureItem("TREASURE CHEST", "A big treasure chest, possibly full of treasure.", treasuregrove, 0, {}, mimic1);
	fakechest->setDenial("The treasure chest is really heavy! You need to USE it to open it, instead.");
	Item* rr2 = new ReviveItem(*(ReviveItem*)(reviveroot));
	Item* realchest = new TreasureItem("TREASURE CHEST", "A big treasure chest, possibly full of treasure.", treasurecliff, 300, {rr2, memorycrowbar1});
	realchest->setDenial("The treasure chest is really heavy! You need to USE it to open it, instead.");
	
	NPC* forestboss = new NPC(*savagehog);
	forestboss->setLeader(true, 5, bossgrove, true, true);
	forestboss->setParty({pricklyhog, greaterhog});
	forestboss->blockExit(NORTH, ENEMY, "blocked off by the MAMMOTH HOG!");
	forestboss->setEscapable(false);
	forestboss->setDialogue({{NULL, "MAMMOTH HOG - *SAVAGE ROAR*"}});
	forestboss->addRejectionDialogue({{NULL, "MAMMOTH HOG - *ROAR MEANING NO*"}});
	forestboss->setForceBattle();

	NPC* dgraveguard = new NPC(*skeleviking);
	dgraveguard->setLeader(true, 2, desertgrave);
	dgraveguard->blockExit(SOUTHWEST, ENEMY, "blocked by the SKELEVIKING.");
	dgraveguard->setDialogue({{NULL, "SKELEVIKING - *angry silence*"}});
	dgraveguard->addRejectionDialogue({{NULL, "SKELEVIKING - *angry silence*"}});

	NPC* desertguard = new NPC(*sandman);
	desertguard->setLeader(true, 5, desert);
	desertguard->setParty({sandman, sandman});
	desertguard->blockExit(NORTHWEST, ENEMY, "blocked by the SANDMAN.", true);
	desertguard->setDialogue({{NULL, "SANDMAN - *angry falling sand noises*"}});
	desertguard->addRejectionDialogue({{NULL, "SANDMAN - *angry falling sand noises*"}});

	NPC* desertguard2 = new NPC(*sandman);
	desertguard2->setLeader(true, 6, desertdune);
	desertguard2->setParty({rumbleweed, rumbleweed});
	desertguard2->blockExit(SOUTHEAST, ENEMY, "blocked by the SANDMAN.", true);
	desertguard2->setDialogue({{NULL, "SANDMAN - *angry falling sand noises*"}});
	desertguard2->addRejectionDialogue({{NULL, "SANDMAN - *angry falling sand noises*"}});

	NPC* jimshady2 = new NPC(*jimshady);
	jimshady2->setLeader(true, 8, desert);
	jimshady2->blockExit(NORTHEAST, ENEMY, "blocked by JIM SHADY.", true);
	jimshady2->setDialogue("I'm JIM SHADY, yes I'm the REAL SHADY!");
	jimshady2->addConversation({{jimshady2, "I'm JIM SHADY, yes I'm the REAL SHADY!"},
								{self, "No you still aren't."},
								{jimshady2, "Nobody asked you."}});
	jimshady2->addRejectionDialogue("No. Begone.");

	NPC* canyonguard = new NPC(*skeleminer);
	canyonguard->setLeader(true, 6, canyon3);
	canyonguard->setParty({rockbug});
	canyonguard->blockExit(UNDERGROUND, ENEMY, "blocked by the SKELEMINER.");
	canyonguard->setDialogue({{NULL, "SKELEMINER - *angry rattling*"}});
	canyonguard->addRejectionDialogue({{NULL, "SKELEMINER - *angry rattling*"}});

	NPC* mineguard = new NPC(*skeleminer);
	mineguard->setLeader(true, 6, deserttunnel);
	mineguard->setParty({rockbug, rockbug});
	mineguard->blockExit(SOUTHWEST, ENEMY, "blocked by the SKELEMINER.", true);
	mineguard->setDialogue({{NULL, "SKELEMINER - *angry rattling*"}});
	mineguard->addRejectionDialogue({{NULL, "SKELEMINER - *angry rattling*"}});
	
	NPC* mineguard2 = new NPC(*skeleminer);
	mineguard2->setLeader(true, 8, mineshaft2);
	mineguard2->setParty({sandman});
	//mineguard2->blockExit(NORTH, ENEMY, "blocked by the SKELEMINER.");
	mineguard2->setDialogue({{NULL, "SKELEMINER - *angry rattling*"}});
	mineguard2->addRejectionDialogue({{NULL, "SKELEMINER - *angry rattling*"}});
	mineguard2->guardItem(minecart3);

	NPC* boomguard = new NPC(*dreadnaut);
	boomguard->setLeader(true, 8, mineshaftside);
	boomguard->setParty({rockbug, rockbug});
	boomguard->blockExit(SOUTHWEST, ENEMY, "blocked by the DREADNOUGHT.");
	boomguard->setDialogue({{NULL, "DREADNOUGHT - *mechanical invertabrate noises*"}});
	boomguard->addRejectionDialogue({{NULL, "DREADNOUGHT - *mechanical rejection*"}});
	
	//MARK: Lobster
	florian = new NPC(*tunnellobster);
	florian->setLobster(tunnels);
	florian->setLeader(true, 10, desertstation, false);
	florian->setTunnelDirection(tentstation, TO_THE_VILLAGE);
	florian->setTunnelDirection(desertstation, TO_THE_DESERT);
	florian->setTunnelDirection(deserttunnel, TO_THE_DESERT);
	florian->setTunnelDirection(volcanostation, TO_THE_HIGHLANDS);
	florian->setTunnelDirection(burgstation, TO_BURGERSBURG);
	florian->setTunnelDirection(BURGERPRISON, TO_THE_BASEMENT);
	florian->setTunnelDirection(basestation, TO_THE_BASEMENT);
	florian->setDialogue("HHhHHHhhHHhHhhHhHHhHhHHh (angry lobster noises)");
	florian->addRejectionDialogue("HhhHhHhHhhhhHHhHHh (lobster noises probably meaning no)");
	florian->addLinkedDesc(florian, "Your big pet crustacean who inhabits the tunnels below.");
	florian->addLinkedDialogue(florian, {{NULL, "You pet your lobster."}, {self, ":D"}, {florian, "HHhhHhHHhHhHhHHHhHHhhHhh (happy lobster noises)"}});
	florian->setWorldCondition(TAMEDLOBSTER);

	//set up teammate viola MARK: Viola
	NPC* viola = new NPC(*tkviola);
	npcChar[viola] = 'v'; //Viola's character representation is v for Viola
	viola->setLeader(true, 10, cliff2);
	viola->addConversation({{self, "Hey did you kidnap everyone in that town over there?"},
							{viola, "So what if I did?"},
							{self, "Lady you can't just go kidnapping people."},
							{viola, "OH YEAH? FIGHT ME!"},
							{self, "Ok."}});
	viola->setDialogue("AHAHAHAHAHAHAHA!");
	viola->addGymDialogue("It's leg day oh nooo :(");
	viola->addRejectionDialogue({{self, "Hey wanna join me on my BURGER QUEST?"},
								 {viola, "..."},
								 {viola, "no"}});
	viola->addRecruitmentDialogue({{self, "Yo, wanna join my team?"},
								   {viola, "Really?"},
								   {viola, "After what I did?"},
								   {self, "Yeah."},
								   {viola, "Um..."},
								   {viola, "Alright..."}});
	viola->addRecruitedDialogue("It feels nice to walk. I hadn't done that in a while.");
	viola->addDismissalDialogue({{viola, "Well, I guess I'll go watch over the town."},
								 {viola, "It's the least I could do..."}});
	viola->addRedirect(desertshop, desertshopfixed);
	viola->addRedirect(desertgym, desertgymfixed);
	viola->addLinkedRoom(deserttown, "in the repopulated desert village. It's very lively here; looks like it's right back to business as usual.");
	viola->addLinkedRoom(deserthouse, "in some house which still has zero purpose.");
	viola->addRecruitLink(viola);
	viola->addLinkedStats(viola, Stats(20, 0, 10, 0, 10, 20, 19));
	viola->addLinkedConvo(viola, {{viola, "I'm sorry I'll free everyone..."},
								  {viola, "It's just that I'm shy and I have a hard time making friends that's why I kidnapped them..."},
								  {self, "That's not a very good reason."},
								  {viola, "Yeah I know..."},
								  {viola, "I'm just going to go to that cliff over there..."},
								  {NULL, "VIOLA went to that cliff over there."}});
	viola->addLinkedTitle(viola, "GRAVITY GIRL");
	viola->addLinkedDesc(viola, "Telekinetic teenager in regret of her prior actions. She is more physically grounded now.");
	viola->addLinkedDialogue(viola, {{viola, "I can't believe I let all that power go to my head..."}});
	viola->addDefeatRoom(viola, thatcliff);
	viola->addDefeatRoom(merchant, desertshopfixed);
	viola->addDefeatRoom(franklin, deserttown);
	viola->addDefeatRoom(bob, oasis);
	viola->setRecruitDialogueChange("I think I'm doing a good job protecting the town so far.");
	viola->addLinkedRoom(cliff2, "on the highest cliff, with a good view of the volcanic, even wastier wastelands beyond the desert");
	viola->addAttackRemoval(viola, tkguard);
	viola->setTalkOnDefeat();
	viola->setForceBattle();
	viola->setEscapable(false);

	NPC* springguard = new NPC(*greer);
	springguard->setLeader(true, 20, NULL, false);
	springguard->alterSp(-100); //so greer has to start by using gun so it's funnier
	springguard->addConversation({{greer, "Oi kid!"},
								  {greer, "Whaddaya doin' down here, huh?"},
								  {self, "I want to turn that valve over there."},
								  {greer, "Scram, kid!"},
								  {greer, "This is official BOIGA business!"},
								  {self, "No >:|"},
								  {greer, "Ohhh we got a tough guy, huh?"},
								  {greer, "Well if youse wanna turn da valve, youse gonna hafta go through me!"},
								  {self, "Alright then."}});
	springguard->addRejectionDialogue("Now why would I wanna join youse, huh?");
	springguard->setDialogue("Scram, kid!");
	springguard->addLinkedConvo(springguard, {{greer, "Youse gotta be kiddin' me..."},
											  {greer, "Beaten by a kid?"},
											  {greer, "I can't go back to da boss with a failya like dis!"},
											  {greer, "I'm not looking forward to dis meetin'..."},
											  {NULL, "GREER disappeared into the darkness..."},
											  {NULL, "The VALVE is no longer being guarded!"}});
	springguard->setTalkOnDefeat();
	springguard->setForceBattle();
	springguard->setEscapable(false);
	springguard->addLinkedRoom(oasis, "in the town oasis, now fully restored! Some signs of greenery are starting to appear.");
	springguard->guardItem(valve);

	NPC* volcanoguard = new NPC(*magman);
	volcanoguard->setLeader(true, 10, volcano1);
	volcanoguard->blockExit(NORTHWEST, ENEMY, "guarded by the LAVAMAN.");
	volcanoguard->setDialogue({{NULL, "LAVAMAN - *angry flowing sizzling noises*"}});
	volcanoguard->addRejectionDialogue({{NULL, "LAVAMAN - *angry flowing sizzling noises*"}});

	NPC* volcanoguard2 = new NPC(*magman);
	volcanoguard2->setLeader(true, 12, volcano3);
	volcanoguard2->setParty({lavizard, lavizard});
	volcanoguard2->blockExit(NORTHEAST, ENEMY, "guarded by the LAVAMAN.");
	volcanoguard2->setDialogue({{NULL, "LAVAMAN - *angry flowing sizzling noises*"}});
	volcanoguard2->addRejectionDialogue({{NULL, "LAVAMAN - *angry flowing sizzling noises*"}});

	NPC* volcanoguard3 = new NPC(*largelavaman);
	volcanoguard3->setLeader(true, 14, volcano5);
	volcanoguard3->setParty({magman, magman, lavizard});
	volcanoguard3->blockExit(NORTH, ENEMY, "guarded by the LARGE LAVAMAN.");
	volcanoguard3->setDialogue({{NULL, "LARGE LAVAMAN - *LAVAL ROAR*"}});
	volcanoguard3->addRejectionDialogue({{NULL, "LARGE LAVAMAN - *LAVAL ROAR*"}});

	NPC* fact1guard = new NPC(*slagman);
	fact1guard->setLeader(true, 11, factoryplat);
	fact1guard->setParty({magman, magman});
	fact1guard->blockExit(EAST, ENEMY, "guarded by the SLAGMAN.");
	fact1guard->setDialogue({{NULL, "SLAGMAN - *HISSSSSSSS*"}});
	fact1guard->addRejectionDialogue({{NULL, "SLAGMAN - *HISSSSSSSSSS*"}});

	NPC* fact2guard = new NPC(*lavasoldier);
	fact2guard->setLeader(true, 12, factory2);
	fact2guard->setParty({magman});
	fact2guard->blockExit(NORTHEAST, ENEMY, "guarded by the LAVA SOLDIER.");
	fact2guard->setDialogue({{NULL, "LAVA SOLDIER - *lavally challenges you to a battle*"}});
	fact2guard->addRejectionDialogue({{NULL, "LAVA SOLDIER - *proud laval noises*"}});

	NPC* fact2guard2 = new NPC(*slagman);
	fact2guard2->setLeader(true, 12, factorybalcony1);
	fact2guard2->setParty({lavasoldier, lavasoldier}); //reverse so it's 1 soldier 2 slagmen?
	fact2guard2->blockExit(NORTHEAST, ENEMY, "guarded by the SLAGMAN.", true);
	fact2guard2->setDialogue({{NULL, "SLAGMAN - *HISSSSSSSS*"}});
	fact2guard2->addRejectionDialogue({{NULL, "SLAGMAN - *HISSSSSSSSSS*"}});

	NPC* fact2guard3 = new NPC(*slagman);
	fact2guard3->setLeader(true, 13, factorystorage);
	fact2guard3->setParty({slagman});
	fact2guard3->blockExit(EAST, ENEMY, "guarded by the SLAGMAN.", true);
	fact2guard3->setDialogue({{NULL, "SLAGMAN - *HISSSSSSSS*"}});
	fact2guard3->addRejectionDialogue({{NULL, "SLAGMAN - *HISSSSSSSSSS*"}});

	NPC* golemguard = new NPC(*factgolem);
	golemguard->setLeader(true, 15, switchroom2);
	golemguard->blockExit(SOUTHEAST, ENEMY, "guarded by the FACTORY GOLEM.", true);
	golemguard->setDialogue({{NULL, "FACTORY GOLEM - *DEEP SYNTH NOISES*"}});
	golemguard->addRejectionDialogue({{NULL, "FACTORY GOLEM - *DEEP SYNTH REJECTION*"}});

	NPC* fact3guard = new NPC(*lavasoldier);
	fact3guard->setLeader(true, 14, factorynw);
	fact3guard->setParty({slagman, lavizard, lavizard});
	fact3guard->blockExit(EAST, ENEMY, "guarded by the LAVA SOLDIER.");
	fact3guard->setDialogue({{NULL, "LAVA SOLDIER - *lavally challenges you to a battle*"}});
	fact3guard->addRejectionDialogue({{NULL, "LAVA SOLDIER - *proud laval noises*"}});

	NPC* fact3guard2 = new NPC(*poizard);
	fact3guard2->setLeader(true, 14, factoryse);
	fact3guard2->setParty({lavizard, lavizard});
	fact3guard2->blockExit(EAST, ENEMY, "guarded by the LAVA SOLDIER.");
	fact3guard2->setDialogue({{NULL, "POIZARD - *squeakily threatens you*"}});
	fact3guard2->addRejectionDialogue({{NULL, "POIZARD - *squeakily rejects you*"}});

	NPC* jimshady3 = new NPC(*jimshady);
	jimshady3->setLeader(true, 15, factoryroofse);
	jimshady3->blockExit(EAST, ENEMY, "blocked by JIM SHADY.");
	jimshady3->setDialogue("I'm JIM SHADY, yes I'm the REAL SHADY!");
	jimshady3->addConversation({{jimshady1, "I'm JIM SHADY, yes I'm the REAL SHADY!"},
								{self, "No..."},
								{self, "Jim you should stop trying to be someone else."},
								{jimshady1, "Nobody asked you."},
								{self, "Ok. T_T"}});
	jimshady3->addRejectionDialogue("No.");

	NPC* carlosguard = new NPC(*superslagman);
	carlosguard->setLeader(true, 14, factoryroofne);
	carlosguard->setParty({lavasoldier, lavasoldier});
	carlosguard->blockExit(EAST, ENEMY, "guarded by the SUPER SLAGMAN.");
	carlosguard->setDialogue({{NULL, "SUPER SLAGMAN - *HHIIIIIIIISSSSSSSSSSSSSSSSSSSS*"}});
	carlosguard->addRejectionDialogue({{NULL, "SUPER SLAGMAN - *HHHHIIIIIIIIIIIISSSSSSSSSSSSSSSSSSSSSSSSSS*"}});

	NPC* fact3guard3 = new NPC(*poizard);
	fact3guard3->setLeader(true, 15, factoryroofnw);
	fact3guard3->setParty({slagman, poizard, poizard});
	fact3guard3->blockExit(NORTH, ENEMY, "guarded by the SUPER SLAGMAN.");
	fact3guard3->setDialogue({{NULL, "SUPER SLAGMAN - *HHIIIIIIIISSSSSSSSSSSSSSSSSSSS*"}});
	fact3guard3->addRejectionDialogue({{NULL, "SUPER SLAGMAN - *HHHHIIIIIIIIIIIISSSSSSSSSSSSSSSSSSSSSSSSSS*"}});

	NPC* shortcutguard = new NPC(*skeleminer); //skeleminer duo!
	shortcutguard->setLeader(true, 15, sewer);
	shortcutguard->setParty({skeleminer});
	shortcutguard->blockExit(SOUTHWEST, ENEMY, "blocked by the SKELEMINER.");
	shortcutguard->setDialogue({{NULL, "SKELEMINER - *angry rattling*"}});
	shortcutguard->addRejectionDialogue({{NULL, "SKELEMINER - *angry rattling*"}});

	NPC* sewerguards = new NPC(*largelavaman);
	sewerguards->setLeader(true, 15, sewercenter1);
	sewerguards->setParty({magman});
	sewerguards->blockExit(NORTHWEST, ENEMY, "guarded by the LARGE LAVAMAN.", true);
	sewerguards->setDialogue({{NULL, "LARGE LAVAMAN - *LAVAL ROAR*"}});
	sewerguards->addRejectionDialogue({{NULL, "LARGE LAVAMAN - *LAVAL ROAR*"}});

	NPC* sewerguardn = new NPC(*largelavaman);
	sewerguardn->setLeader(true, 15, sewercenter2);
	sewerguardn->blockExit(SOUTHEAST, ENEMY, "guarded by the LARGE LAVAMAN.", true);
	sewerguardn->setDialogue({{NULL, "LARGE LAVAMAN - *LAVAL ROAR*"}});
	sewerguardn->addRejectionDialogue({{NULL, "LARGE LAVAMAN - *LAVAL ROAR*"}});

	NPC* crocguards = new NPC(*lavadile);
	crocguards->setLeader(true, 15, sewercenter1);
	crocguards->blockExit(NORTH, ENEMY, "guarded by the LAVADILE.", true);
	crocguards->setDialogue({{NULL, "LAVADILE - *growllll*"}});
	crocguards->addRejectionDialogue({{NULL, "LAVADILE - *growllllll*"}});

	NPC* crocguardn = new NPC(*lavadile);
	crocguardn->setLeader(true, 15, sewercenter1);
	crocguardn->setParty({lavadile});
	crocguardn->blockExit(NORTH, ENEMY, "guarded by the LAVADILE.", true);
	crocguardn->setDialogue({{NULL, "LAVADILE - *growllll*"}});
	crocguardn->addRejectionDialogue({{NULL, "LAVADILE - *growllllll*"}});

	NPC* mimic2 = new NPC(*mimic);
	mimic2->setLeader(true, 50, NULL, false);
	mimic2->setDialogue({{NULL, "MIMIC - *UNHINGED ROAR*"}});
	mimic2->addRejectionDialogue({{NULL, "MIMIC - *UNHINGED ROAR*"}});
	mimic2->setEscapable(false);
	mimic2->setForceBattle();
	mimic2->setExtraMonies(1500); //you get lots of monies for beating the mimic

	NPC* mimic3 = new NPC(*mimic);
	mimic3->setLeader(true, 50, NULL, false);
	mimic3->setDialogue({{NULL, "MIMIC - *UNHINGED ROAR*"}});
	mimic3->addRejectionDialogue({{NULL, "MIMIC - *UNHINGED ROAR*"}});
	mimic3->setEscapable(false);
	mimic3->setForceBattle();
	mimic3->setExtraMonies(1500); //you get lots of monies for beating the mimic

	Item* fakechest1 = new TreasureItem("TREASURE CHEST 1", "A big treasure chest, possibly full of treasure.", sewertreasure, 0, {}, mimic2);
	Item* fakechest2 = new TreasureItem("TREASURE CHEST 2", "A big treasure chest, possibly full of treasure.", sewertreasure, 0, {}, mimic3);
	Item* rr3 = new ReviveItem(*(ReviveItem*)(reviveroot));
	Item* realchest2 = new TreasureItem("TREASURE CHEST 3", "A big treasure chest, possibly full of treasure.", sewertreasure, 500, {rr3, memorycrowbar2});
	fakechest1->setDenial("The treasure chest is really heavy! You need to USE it to open it, instead.");
	fakechest2->setDenial("The treasure chest is really heavy! You need to USE it to open it, instead.");
	realchest2->setDenial("The treasure chest is really heavy! You need to USE it to open it, instead.");

	NPC* drainguard = new NPC(*lavagator);
	drainguard->setLeader(true, 15, sewerplant);
	drainguard->setDialogue({{NULL, "LAVAGATOR - *GATORY ROAR*"}});
	drainguard->addRejectionDialogue({{NULL, "LAVAGATOR - *GATORY ROAR*"}});
	drainguard->guardItem(masterswitch);
	
	//put a treasure chest in the mines (just monies), and guard the room to it with a rockbug or something (or put the skeleminer duo here? and make a molten miner where the duo is right now?)

	NPC* lavaguard = new NPC(*lavaguardian);
	lavaguard->setLeader(true, 16, bridge3, false);
	lavaguard->blockExit(NORTH, ENEMY, "blocked by the LAVA GUARDIAN.");
	lavaguard->setDialogue("*ethereal breathing*");
	lavaguard->addRejectionDialogue({{self, "Hey you wanna join me on my BURGER QUEST?"}, {lavaguardian, "*ethereal breathing*"}, {self, "..."}});
	lavaguard->setEscapable(false);

	NPC* tabguard = new NPC(*newtab);
	tabguard->setLeader(true, 12, castlehall, false);
	tabguard->setParty({newtab, newtab});
	tabguard->blockExit(SOUTH, ENEMY, "blocked by the NEW TAB");
	tabguard->setDialogue({{NULL, "NEW TAB - *angry internet argument noises*"}});
	tabguard->addRejectionDialogue({{NULL, "NEW TAB - *angry internet argument noises*"}});
	
	NPC* kingbrowser = new NPC(*browser);
	kingbrowser->setLeader(true, 18, castlethrone, true);
	kingbrowser->setRespawnReq(plum); //respawns only when plum is there and not recruited
	kingbrowser->setDialogue("GWAHAHAHAHAHA!");
	kingbrowser->addRejectionDialogue({{browser, "You think I wanna join you?"}, {browser, "GWAHAHAHAHAHA!"}});
	kingbrowser->setEscapable(false);
	kingbrowser->addRecruitLink(plum);
	kingbrowser->addLinkedConvo(kingbrowser, {{plum, "Thank you for saving me, young knight!"}, {self, "No problemo."}}); //only say the first time
	kingbrowser->setTalkOnDefeat();
	kingbrowser->addLinkedConvo(plum, {{plum, "I need to invest in better security."}, {plum, "All I have right now is some plumber."}}); //only say the first time
	kingbrowser->addLinkedDialogue(plum, {{plum, "This Browser..."}, {plum, "I need to invest in better security..."}}); //add every time
	kingbrowser->startNewChanges(true); //do these next ones every single defeat
	kingbrowser->addLinkedDialogue(plum, {{plum, "This Browser..."}, {plum, "I need to invest in better security..."}});
	kingbrowser->addRecruitLink(plum);
	WorldChange& kbrespawnchanges = kingbrowser->editRespawnChanges(); //add these only the first respawn
	kbrespawnchanges.decruitLinks.push(plum);
	kbrespawnchanges.linkedWelcomes.push({castlethrone, {{plum, "AHHHHHHHHHH HELP ME I'VE BEEN KIDNAPPED!"}, {browser, "GWAHAHAHAHAHA!"}, {self, "What? Again?"}, {self, "How?"}, {plum, "Well you know."}, {plum, "Browser just came in his airship and kidnapped me."}, {self, "I see."}}}); //only add the first respawn
	kbrespawnchanges.linkedDialogue.push({plum, {{plum, "AHHHHHHHHHH HELP ME I'VE BEEN KIDNAPPED!"}, {browser, "GWAHAHAHAHAHA!"}, {self, "T_T"}}}); //add on every respawn
	WorldChange& kbrespawnchanges2 = kingbrowser->editRespawnChanges(); //add these every time
	kbrespawnchanges.decruitLinks.push(plum);
	kbrespawnchanges2.linkedDialogue.push({plum, {{plum, "AHHHHHHHHHH HELP ME I'VE BEEN KIDNAPPED!"}, {browser, "GWAHAHAHAHAHA!"}, {self, "T_T"}}});

	NPC* mountainguard = new NPC(*snowman);
	mountainguard->setLeader(true, 15, mountain, false);
	mountainguard->blockExit(UP, ENEMY, "blocked by the SNOWMAN.");
	mountainguard->setDialogue({{NULL, "SNOWMAN - *angry snow crunching noises*"}});
	mountainguard->addRejectionDialogue({{NULL, "SNOWMAN - *angry snow crunching noises*"}});

	NPC* mountainguard2 = new NPC(*bewlizard);
	mountainguard2->setLeader(true, 25, mountain3);
	mountainguard2->alterSp(-100); //the lizard starts at 0 sp
	mountainguard2->blockExit(UP, ENEMY, "blocked by the BLUE EYES WHITE LIZARD.");
	mountainguard2->setDialogue({{NULL, "BLUE EYES WHITE LIZARD - *warning chirp growls*"}});
	mountainguard2->addRejectionDialogue({{NULL, "BLUE EYES WHITE LIZARD - *warning chirp growls*"}});

	NPC* jimshady0 = new NPC(*realjimshady);
	jimshady0->setLeader(true, 25, mountainside);
	jimshady0->setParty({jimmyshimmy, jimmyshimmy});
	jimshady0->blockExit(WEST, ENEMY, "blocked by THE REAL JIM SHADY.");
	jimshady0->addConversation({{realjimshady, "Hi."}, {realjimshady, "My name is-"}, {NULL, "What?"}, {realjimshady, "My name is-"}, {NULL, "Who?"}, {realjimshady, "My name is"}, {NULL, "*chka chka*"}, {realjimshady, "JIM SHADY."}});
	jimshady0->addRejectionDialogue("No.");
	jimshady0->setDialogue("I'm JIM SHADY, yes I'm the REAL SHADY.");
	jimshady0->setForceBattle(true);
	jimshady0->addLinkedConvo(jimshady0, {{self, "So what's it like being the real Jim Shady?"}, {jimshady0, "Pretty real."}});

	NPC* leftguard1 = new NPC(*paveshark);
	leftguard1->setLeader(true, 16, leftstreet3);
	leftguard1->setParty({paveshark, paveshark});
	leftguard1->blockExit(NORTH, ENEMY, "guarded by the PAVEMENT SHARK.", true);
	leftguard1->setDialogue({{NULL, "PAVEMENT SHARK - *angry shark noises*"}});
	leftguard1->addRejectionDialogue({{NULL, "PAVEMENT SHARK - *angry shark noises*"}});

	NPC* leftguard2 = new NPC(*bagelfenagler);
	leftguard2->setLeader(true, 17, leftstreet4);
	leftguard2->setParty({crimmind, paveshark, paveshark});
	leftguard2->blockExit(NORTH, ENEMY, "guarded by the BAGEL FENAGLER.", true);
	leftguard2->setDialogue("Uhhh huuhhhhhh huuhhhhhh  hhuuhhhhhhhhhhh... :)");
	leftguard2->addRejectionDialogue("Uh uhhhhhh... :)");

	NPC* newguard1 = new NPC(*crimmind);
	newguard1->setLeader(true, 16, newstreet1);
	newguard1->setParty({axeman});
	newguard1->blockExit(NORTH, ENEMY, "guarded by the CRIMINAL MASTERMIND.", true);
	newguard1->setDialogue("I am 4 parallel universes ahead of you.");
	newguard1->addRejectionDialogue("I will not stoop down to your level.");

	NPC* newguard2 = new NPC(*disamalg);
	newguard2->setLeader(true, 17, newstreet2);
	newguard2->setParty({paveshark, paveshark});
	newguard2->blockExit(NORTH, ENEMY, "guarded by the DISEASE AMALGAMATION.", true);
	newguard2->setDialogue({{NULL, "DISEASE AMALGAMATION - *slimy writhing disease noises*"}});
	newguard2->addRejectionDialogue({{NULL, "DISEASE AMALGAMATION - *slimy writhing disease noises*"}});

	NPC* newguard3 = new NPC(*naturaldisaster);
	newguard3->setLeader(true, 18, newstreet3);
	newguard3->setParty({paveshark, paveshark}); //sharknado!
	newguard3->blockExit(NORTH, ENEMY, "guarded by the NATURAL DISASTER.", true);
	newguard3->setDialogue({{NULL, "NATURAL DISASTER - *angry jet engine noises*"}});
	newguard3->addRejectionDialogue({{NULL, "NATURAL DISASTER - *angry jet engine noises*"}});

	NPC* mainguard = new NPC(*thief);
	mainguard->setLeader(true, 13, mainstreet2);
	mainguard->setParty({thief, thief});
	mainguard->blockExit(NORTH, ENEMY, "guarded by the THIEF.", true);
	mainguard->setDialogue("I'm gonna rob ya, kid! >:D");
	mainguard->addRejectionDialogue("Nah stealing's all I do! >:D");

	NPC* mainguard2 = new NPC(*bagelfenagler);
	mainguard2->setLeader(true, 16, mainstreet4);
	mainguard2->setParty({axeman});
	mainguard2->blockExit(NORTH, ENEMY, "guarded by the BAGEL FENAGLER.", true);
	mainguard2->setDialogue("Uhhhhhhhh huhhh huh huuuuuhhhhhh... :)");
	mainguard2->addRejectionDialogue("Uh uh uhhhh... :)");

	NPC* coolguard = new NPC(*disamalg);
	coolguard->setLeader(true, 16, coolstreet1);
	coolguard->setParty({crimmind, thief});
	coolguard->blockExit(NORTH, ENEMY, "guarded by the DISEASE AMALGAMATION.", true);
	coolguard->setDialogue({{NULL, "DISEASE AMALGAMATION - *slimy writhing disease noises*"}});
	coolguard->addRejectionDialogue({{NULL, "DISEASE AMALGAMATION - *slimy writhing disease noises*"}});

	NPC* coolguard2 = new NPC(*naturaldisaster);
	coolguard2->setLeader(true, 18, coolstreet2);
	coolguard2->setParty({disamalg, crimmind, thief});
	coolguard2->blockExit(NORTH, ENEMY, "guarded by the NATURAL DISASTER.", true);
	coolguard2->setDialogue({{NULL, "NATURAL DISASTER - *angry jet engine noises*"}});
	coolguard2->addRejectionDialogue({{NULL, "NATURAL DISASTER - *angry jet engine noises*"}});

	NPC* coolguard3 = new NPC(*minipanzer);
	coolguard3->setLeader(true, 19, coolstreet4);
	coolguard3->setParty({crimmind, axeman});
	coolguard3->blockExit(NORTH, ENEMY, "guarded by the MINIPANZER.", true);
	coolguard3->setDialogue({{NULL, "MINIPANZER - *muffled maniacal laughter*"}});
	coolguard3->addRejectionDialogue({{NULL, "You make out some faint muffled speech from the MINIPANZER:"}, {minipanzer, "nh br m gnna rb ya"}});

	NPC* rightguard = new NPC(*minipanzer);
	rightguard->setLeader(true, 18, rightstreet1);
	rightguard->setParty({axeman, axeman});
	rightguard->blockExit(NORTH, ENEMY, "guarded by the MINIPANZER.", true);
	rightguard->setDialogue({{NULL, "MINIPANZER - *muffled maniacal laughter*"}});
	rightguard->addRejectionDialogue({{NULL, "You make out some faint muffled speech from the MINIPANZER:"}, {minipanzer, "nh br m gnna rb ya"}});

	NPC* oneguardl = new NPC(*thief);
	oneguardl->setLeader(true, 14, newstreet1);
	oneguardl->setParty({thief});
	oneguardl->blockExit(WEST, ENEMY, "guarded by the THIEF.", true);
	oneguardl->setDialogue("I'm gonna rob ya, kid! >:D");
	oneguardl->addRejectionDialogue("Nah kid, I'm gonna rob ya! >:D");

	NPC* oneguardr = new NPC(*axeman);
	oneguardr->setLeader(true, 16, coolstreet1);
	oneguardr->setParty({thief, thief});
	oneguardr->blockExit(EAST, ENEMY, "guarded by the AXEMAN.", true);
	oneguardr->setDialogue("AHAAHHAHAHAHHA!");
	oneguardr->addRejectionDialogue("HAHAHHAHAHAHHAHAHAH!");

	NPC* twoguard = new NPC(*minipanzer);
	twoguard->setLeader(true, 18, rightstreet2);
	twoguard->setParty({axeman});
	twoguard->blockExit(WEST, ENEMY, "guarded by the MINIPANZER.", true);
	twoguard->setDialogue({{NULL, "MINIPANZER - *muffled maniacal laughter*"}});
	twoguard->addRejectionDialogue({{NULL, "You make out some faint muffled speech from the MINIPANZER:"}, {minipanzer, "nh br m gnna rb ya"}});

	NPC* threeguard = new NPC(*disamalg);
	threeguard->setLeader(true, 17, coolstreet3);
	threeguard->setParty({disamalg, axeman});
	threeguard->blockExit(WEST, ENEMY, "guarded by the DISEASE AMALGAMATION.", true);
	threeguard->setDialogue({{NULL, "DISEASE AMALGAMATION - *slimy writhing disease noises*"}});
	threeguard->addRejectionDialogue({{NULL, "DISEASE AMALGAMATION - *slimy writhing disease noises*"}});

	NPC* fourguard = new NPC(*bagelfenagler);
	fourguard->setLeader(true, 20, coolstreet4);
	fourguard->setParty({naturaldisaster, minipanzer, minipanzer});
	fourguard->blockExit(EAST, ENEMY, "guarded by the BAGEL FENAGLER.", true);
	fourguard->setDialogue("Huhhhh huhhhhh huhhhhh huhhhhhhh... :)");
	fourguard->addRejectionDialogue("Hhhmmmmmmmmmm... :)");

	NPC* fiveguard = new NPC(*crimmind);
	fiveguard->setLeader(true, 20, newstreet5);
	fiveguard->setParty({crimmind, minipanzer, axeman});
	fiveguard->blockExit(EAST, ENEMY, "guarded by the CRIMINAL MASTERMIND.", true);
	fiveguard->setDialogue("I am 4 parallel universes ahead of you.");
	fiveguard->addRejectionDialogue("I will not stoop down to your level.");

	NPC* fiveguard2 = new NPC(*disamalg);
	fiveguard2->setLeader(true, 21, coolstreet5);
	fiveguard2->setParty({crimmind, axeman});
	fiveguard2->blockExit(EAST, ENEMY, "guarded by the DISEASE AMALGAMATION.", true);
	fiveguard2->setDialogue({{NULL, "DISEASE AMALGAMATION - *slimy writhing disease noises*"}});
	fiveguard2->addRejectionDialogue({{NULL, "DISEASE AMALGAMATION - *slimy writhing disease noises*"}});

	NPC* richguardv = new NPC(*crimmind);
	richguardv->setLeader(true, 21, richneighborhood2);
	richguardv->setParty({crimmind, minipanzer});
	richguardv->blockExit(SOUTH, ENEMY, "guarded by the CRIMINAL MASTERMIND.", true);
	richguardv->setDialogue("I am 4 parallel universes ahead of you.");
	richguardv->addRejectionDialogue("I will not stoop down to your level.");

	NPC* richguardh = new NPC(*disamalg);
	richguardh->setLeader(true, 20, richneighborhood1);
	richguardh->setParty({axeman, crimmind});
	richguardh->blockExit(EAST, ENEMY, "guarded by the DISEASE AMALGAMATION.", true);
	richguardh->setDialogue({{NULL, "DISEASE AMALGAMATION - *slimy writhing disease noises*"}});
	richguardh->addRejectionDialogue({{NULL, "DISEASE AMALGAMATION - *slimy writhing disease noises*"}});

	NPC* apguard1 = new NPC(*thief);
	apguard1->setLeader(true, 20, shrimpartment2);
	apguard1->setParty({thief});
	apguard1->blockExit(UP, ENEMY, "guarded by the THIEF.");
	apguard1->addConversation({{thief, "angry noises"}, {self, "What?"}, {thief, "Uhhh..."}, {thief, "I'm gonna rob ya, kid! >:D"}});
	apguard1->setDialogue("I'm gonna rob ya, kid! >:D");
	apguard1->addRejectionDialogue("angry noises");

	NPC* apguard2 = new NPC(*axeman);
	apguard2->setLeader(true, 20, shrimpartment3);
	apguard2->setParty({crimmind, minipanzer, thief});
	apguard2->blockExit(UP, ENEMY, "guarded by the AXEMAN.");
	apguard2->setDialogue("HHHHHHHHAAAAAAAAAAAAAAAAAAHHHAAAHAHHAHAHAHHAAAA!");
	apguard2->addRejectionDialogue("AHAHAHHAAHHAHAHAHAAAAAAAAAAA!");

	NPC* gamblongo = new NPC(*gamblemonster);
	gamblongo->setLeader(true, 20, casinoback, false);
	gamblongo->blockExit(DOWN, ENEMY, "blocked by GAMBLONGO.");
	gamblongo->addConversation({{gamblongo, "ahahaha"}, {gamblongo, "i am gamblongo the gamble monster and i own this casino"}, {self, "??????????"}, {gamblongo, "if you wish to enter the basement you will have to defeat me in combat"}});
	gamblongo->addConversation({{gamblongo, "i am inevitable"}});
	gamblongo->setDialogue({{gamblongo, "ahahaha i am gamblongo the gamble monster"}});

	NPC* evilgrandma = new NPC(*grandma);
	
	evilgrandma->setLeader(true, 22, rightstreet3, false);
	evilgrandma->setMask("GRANDMA", "MARGE", "She looks like a poor grandma getting beat up and robbed by Ratman.");
	evilgrandma->setTalkOnDefeat();
	evilgrandma->addOpeningDialogue({{grandma, "HAH! I wasn't convincing enough, was I?"},
									 {grandma, "You're a smart cookie, you!"},
									 {grandma, "Bless your heart."},
									 {grandma, "Looks like I'll have to do this the old-fashioned way!"}});
	evilgrandma->addLinkedConvo(grandma, {{grandma, "AHHHH COME ON!"},
										  {grandma, "And I would have gotten away with it too,"},
										  {grandma, "if it weren't for you meddling kids!!"},
										  {ratman, "Good work, kid."},
										  {ratman, "This grandma has been terrorizing the city for many years."},
										  {ratman, "But now she's going straight to the BURGERSBURG asylum."},
										  {ratman, "Because I'm Ratman."},
										  {NULL, "RATMAN grappling hooks away carrying a tied up MARGE."}});
	
	theratman = new NPC(*ratman); //MARK: Ratman
	npcChar[theratman] = 'b'; //Ratman's character representation is b for Batman
	theratman->setLeader(true, 22, rightstreet3);
	theratman->setDialogue("I'm Ratman.");
	theratman->addRecruitedDialogue("I'm Ratman.");
	theratman->addGymDialogue("I'm Ratman.");
	theratman->setThief(); //beating Ratman means Marge steals all your monies
	theratman->addOpeningDialogue({{NULL, "Na na na na na na na na"},
								   {NULL, "na na na na na na na na"},
								   {NULL, "na na na na na na na na"},
								   {NULL, "RATMAN!"}});
	theratman->setTalkOnDefeat();
	theratman->addLinkedConvo(theratman, {{grandma, "Thank you so much!"},
										  {grandma, "Hahahaha"},
										  {grandma, "HAHAHAHAHAHAHAHA!"},
										  {grandma, "THANK YOU FOR BEING SO DUMB!"},
										  {NULL, "MARGE athletically sprints away with all your monies."},
										  {ratman, "Why'd you get in the way, kid?"},
										  {self, "Well it looked like you were beating up a poor grandma."},
										  {self, "Why didn't you clarify better what you were doing?"},
										  {ratman, "Because I'm Ratman."},
										  {self, "T_T"},
										  {NULL, "RATMAN grappling hooks away."}});
	theratman->addDeleaderLink(theratman); //if you fight Ratman
	theratman->addRoamLink(theratman);
	theratman->addDefeatRoom(evilgrandma, limbo);
	theratman->setWorldCondition(BEATRATMAN);
	theratman->addLinkedDesc(theratman, "The dark knight, the caped crusader, etc.\nHe's not the hero this city deserves, and he's not the hero this city needs, but he's the hero this city has.");
	theratman->addRecruitLink(theratman, JILLYQUEST); //MARK: UNLESS Jilly is already saved, then add new recruit links in temple quest
	theratman->addBlock(elevator, TO_THE_TOP);
	theratman->addBlock(elevatorbottom, TO_THE_TOP);
	theratman->setBlockMessage({{theratman, "I thought we weren't just going to get fast food."}, {theratman, "I got serious buiness to attend to, kid."}, {theratman, "Because I'm Ratman."}});
	theratman->setBlockUnless(TEMPLEQUEST);
	evilgrandma->addDeleaderLink(theratman); //if you fight Marge
	evilgrandma->addRoamLink(theratman);
	evilgrandma->addDefeatRoom(grandma, limbo);
	evilgrandma->setWorldCondition(BEATMARGE);
	evilgrandma->addLinkedDesc(theratman, "The dark knight, the caped crusader, etc.\nHe's not the hero this city deserves, and he's not the hero this city needs, but he's the hero this city has.");
	evilgrandma->addRecruitLink(theratman, JILLYQUEST); //MARK: UNLESS Jilly is already saved, then add new recruit links in temple quest
	matilda->addRecruitLink(theratman, BEATRATMAN); //we can just add both for many possible conditions :D
	matilda->addRecruitLink(theratman, BEATMARGE);
	theratman->addRejectionDialogue({{self, "Hey rat man wanna join me on my BURGER QUEST?"},
									 {ratman, "I don't have time to get a BURGER, kid."},
									 {ratman, "I have a city to protect."},
									 {ratman, "Because I'm Ratman."}});
	Conversation batrec1 = {{self, "Hey rat man wanna help me save this kid?"},
							{ratman, "I will help you save this child."},
							{ratman, "Because I'm Ratman."}};
	batrec1.skipcondition = {TEMPLEQUEST};
	shared_ptr<Conversation> batrec2 = make_shared<Conversation>(Conversation({{self, "Hey rat man wanna help me destroy BURGERs?"},
							{ratman, "Sorry, kid."},
							{ratman, "I've got bigger matters to deal with than a fast food corporation."},
							{self, "No they're actually evil and stuff."},
							{self, "They're like trying to destroy all the forests and stuff."},
							{self, "And also they kidnapped a child."},
							{self, "And also they like secretly took over the world."},
							{ratman, "I stand corrected."},
							{ratman, "This sounds like serious business."},
							{ratman, "I will help you destroy BURGERs"},
							{ratman, "Because I'm Ratman."}}));
	batrec2->skipcondition = {BURGERMENDEF};
	shared_ptr<Conversation> batrec3 = make_shared<Conversation>(Conversation({{self, "Hey rat man wanna join me?"},
							{ratman, "I've noticed a decline in crime recently."},
							{ratman, "I could probably fit in some adventuring."},
							{ratman, "So I will join your team."},
							{ratman, "Because I'm Ratman."}}));
	batrec1.alt = batrec2;
	batrec2->alt = batrec3;
	theratman->addDismissalDialogue({{ratman, "I shall continue to protect the city."}, {ratman, "Because I'm Ratman."}, {NULL, "RATMAN grappling hooks away."}});
	theratman->setRoamRooms({mainstreet1, mainstreet2, mainstreet3, mainstreet4, mainstreet5, coolstreet1, coolstreet2, coolstreet3, coolstreet4, coolstreet5, rightstreet1, rightstreet2, rightstreet3, rightstreet4, rightstreet5});
	rightstreet3->setWelcome({{grandma, "AHHHHHHHH!"},
							  {grandma, "HELP ME, DEARIE!"},
							  {grandma, "This bad man is beating me up!"},
							  {grandma, "And stealing my purse!"},
							  {self, "Why are you beating up this grandma? >:|"},
							  {ratman, "Because I'm Ratman."}});

	NPC* burgerguards = new NPC(*burgeragent);
	burgerguards->setLeader(true, 23, richneighborhood4, false);
	burgerguards->setMask("", "BURGER GUARDS", "Security guards of the BURGER corporation, dripped out in suit and sunglasses.");
	burgerguards->setParty({crimmind, minipanzer, thief});
	burgerguards->blockExit(INSIDE, ENEMY, "guarded by the BURGER GUARDS.");
	burgerguards->addConversation({{self, "Hey I'm trying to rescue this kid can you move please?"}, {burgeragent, "Kid, I'm not sure how you got here,"}, {burgeragent, "but I'm gonna give you the opportunity to leave."}, {self, "No."}, {NULL, "BURGER AGENT - *sigh*"}});
	burgerguards->setDialogue({{burgeragent, "Kid, go home."}, {self, "No."}});
	burgerguards->addRejectionDialogue({{self, "Hey you guys wanna join my team?"}, {burgeragent, "What?"}, {self, "I said 'You wanna join my team.' "}, {burgeragent, "No."}, {self, "What about you?"}, {NULL, "BURGER AGENT 2 - \"No.\""}, {self, "ok"}});
	burgerguards->addRejectionDialogue({{burgerguards, "No."}});
	
	Item* keycard = new WorldChangeItem("EXECUTIVE KEYCARD", "Golden elevator keycard given to BURGER executives for going up to the CEO's office.", limbo,
		{{NULL, "You slid the EXECUTIVE KEYCARD across the keycard slot."},
		 {NULL, "KEYCARD SLOT - *error beep*"},
		 {self, "Oh it's the other way."},
		 {NULL, "You flip the EXECUTIVE KEYCARD around and slide it across the keycard slot."},
		 {NULL, "KEYCARD SLOT - *confirmation beep*"},
		 {NULL, "You can go TO THE TOP now!"}});
	keycard->setTakable(true);
	WorldChangeItem* kc = (WorldChangeItem*)keycard;
	kc->setUsableRooms({ceoelevator1, ceoelevator2});
	WorldChange& kcchanges = kc->getChanges();
	kcchanges.exitUnblocks.push({ceoelevator1, TO_THE_TOP});
	kcchanges.exitUnblocks.push({ceoelevator2, TO_THE_TOP});

	NPC* richpeople = new NPC(*richperson); //RICH PEOPLE GAUNTLET!
	richpeople->setLeader(true, 22, ballroom, false);
	richpeople->setMask("", "RICH PEOPLE", "A huge gathering of rich people, discussing rich people things while drinking expensive wine and eating expensive cheese.");
	richpeople->setParty({richperson, richperson}); //rich person x3
	richpeople->setParty({richperson, richperson, richcyborg}, true); //rich person x2 + rich cyborg
	richpeople->setParty({richcyborg, burgerlawyer}, true); //rich cyborg + burger lawyer
	richpeople->setParty({burgeragent, richperson, richperson}, true); //burger agent + rich person x2
	richpeople->setParty({burgerbutler}, true); //burger butler
	richpeople->setParty({burgerlawyer, burgerlawyer, richcyborg}, true); //burger lawyer x2 + rich cyborg
	richpeople->setParty({burgeragent, burgeragent}, true); //burger agent x2
	richpeople->setParty({burgerlawyer, burgerlawyer, burgerlawyer}, true); //burger lawyer x3
	richpeople->setParty({richcyborg, burgeragent, burgerlawyer}, true); //rich cyborg + burger agent + burger lawyer
	richpeople->setParty({richperson, richperson, richperson, richperson, richperson, richperson, richperson, richperson}, true); //rich person x8
	richpeople->setParty({burgerbutler, burgerbutler}, true); //burger butler x2
	richpeople->addConversation({{self, "HEY!"},
								 {NULL, "RICH PEOPLE - *indistinct chatter*"},
								 {self, "GUYS!"}, {NULL, "You jump onto a table and grab a glass and a fork."},
								 {NULL, "You clink the fork against the glass a few times."}, {NULL, "The room goes silent."},
								 {richperson, "Who the heck are you?"},
								 {self, "I'm saving this kid you guys kidnapped. >:|"},
								 {self, "So..."}, {self, "(wait what am I saying exactly?)"},
								 {self, "Could one of you guys direct me towards where you kidnap people to?"},
								 {self, "..."},
								 {richpeople, "..."},
								 {richperson, "LET'S GET HIM!"}});
	richpeople->setDialogue({{self, "I'm gonna save that kid you guys kidnapped >:|"}, {richperson, "LET'S GET HIM!"}});
	richpeople->addRejectionDialogue({{NULL, "Like, are you trying to recruit all of them at once?"}, {NULL, "That's not gonna work. :|"}});
	richpeople->setForceBattle();
	richpeople->addLinkedRoom(ballroom, "in the BURGER ballroom, full of incapacitated rich people.");
	richpeople->addLinkedItem(keycard, ballroom); //beat the rich people to get the keycard
	richpeople->setEscapable(false);

	NPC* enzo = new NPC(*ceo);
	enzo->setMask("BURGER CEO", "ENZO", "The CEO of the whole BURGER COROPORATION, sitting ominously behind his desk.");
	enzo->setLeader(true, 30, ceoroom, false);
	enzo->blockExit(IN_SAFE, ENEMY, "guarded by ENZO.");
	enzo->addConversation({{self, "Why'd you guys kidnap that kid? >:|"},
						   {ceo, "So that's what you busted into this joint for?"},
						   {ceo, "Over some kid? Hah hah hah hah!"},
						   {self, "Answer my question! >:|"},
						   {ceo, "Eh, don't even remember that."},
						   {ceo, "I must've sent one of my lackeys to do that some time ago."},
						   {ceo, "I just do whatever the BURGER MAN tells me to do."},
						   {ceo, "Never ask questions."},
						   {ceo, "It's the least I could do, with all the fame and riches he's given us."},
						   {ceo, "All for the low low price of our souls!"},
						   {self, "That deal kinda sucks. :|"},
						   {ceo, "Well, I beg to differ."},
						   {ceo, "Anyway, ya did quite the number on the personnel."},
						   {ceo, "That kinda threat ain't good for business."},
						   {NULL, "Mechanical noises start to whirr from within Enzo..."},
						   {ceo, "Guess I gotta take care of ya myself."},
						   {NULL, "The metal plates forming Enzo's skin start to shift..."},
						   {NULL, "Enzo bursts into a metallic mass of heavy weaponry!"},
						   {ceo, "Let's see how much that humanity of yours'll help ya out now!"},
						   {ceo, "HAH HAH HAH HAH!"}});
	enzo->addLinkedConvo(enzo, {{NULL, "Enzo breaks down into pieces."},
							   {NULL, "You walk up to his core."},
							   {enzo, "WHAT?"},
							   {enzo, "WHAT?"},
							   {enzo, "YOU?"},
							   {enzo, "HOW DID YOU?"},
							   {self, "Well clearly all this robot stuff wasn't worth it."},
							   {self, "So can you tell me where that kid is?"},
							   {enzo, "YOU THINK I'M JUST GONNA BE ALL BUDDY BUDDY WITH YA?"},
							   {enzo, "CAUSE YA BEAT ME UP, I'M GONNA SEE THE ERROR IN MY WAYS?"},
							   {self, "Well like..."},
							   {self, "It did work once \\_(:|)_/"},
							   {enzo, "GO HOME, KID."},
							   {enzo, "You don't know what yer messing with."},
							   {NULL, "Enzo's core starts to glow."},
							   {self, "O_O"},
							   {NULL, "You jump away from Enzo."},
							   {NULL, "Enzo self-destructed!"},
							   {NULL, "The top of the BURGER headquarters went flying in pieces!"},
							   {self, "O_O"}});
	enzo->setTalkOnDefeat();
	enzo->setDialogue({{ceo, "You got a screw loose if ya think you can beat me!"}, {ceo, "HAH HAH HAH HAH!"}});
	enzo->addRejectionDialogue("You're asking me to join ya? Are ya crazy?");
	enzo->addLinkedRoom(ceoroom, "in the BURGER CEO's office, now missing the roof and walls.\nThe BURGER SAFE is still intact.");
	enzo->addLinkedRoom(ballroom, "in the BURGER ballroom. The action has died down and the room is pretty quiet.");
	enzo->addLinkedRoom(ceolobby, "in the ornate lobby of the building, featuring both an elevator and fancy stairs.\nThe yearly RICH PEOPLE reunion has concluded.");
	enzo->addLinkedRoom(ceolobby2, "on the second floor of the building, still the lobby.");
	enzo->addLinkedRoom(richneighborhood4, "at the doorway of the BURGER corporation's headquarters.");
	enzo->addDefeatRoom(burgcustomer, limbo);
	enzo->addLinkedConvo(burgerman, burgdeciet);
	enzo->addLinkedGift(burgerman, freeboiga);
	enzo->setForceBattle();
	enzo->setEscapable(false);
	enzo->setWorldCondition(BEATCEO);

	NPC* burgerscientist = new NPC("BURGER SCIENTIST", "IVOR", "Genius robotic husk responsible for the BURGER personnel's augmentations and himself's.\nSlowly swapping his organs for mechanical parts, his true self is long dead.", limbo, 0, Stats(150, 20, 30, 35, 45, 30, 9));
	burgerscientist->setNoFight(); //FIGHT or ASK does the same thing and you can't actually fight him
	burgerscientist->setLeader(true, 29, burglab);
	burgerscientist->setTalkMakeChanges();
	burgerscientist->addDefeatRoom(unihorn, limbo);
	burgerscientist->addLinkedItem(unihorncorn, burglab);
	burgerscientist->guardItem(releaseswitch);
	burgerscientist->addLinkedItem(releaseswitch, limbo);
	burgerscientist->addLinkedRoom(burglab, "in the BURGER LABORATORY. There's many nasty looking needles and contraptions.");
	burgerscientist->addConversation({{self, "Hey what are you doing to this unihorn? >:|"},
									  {burgerscientist, "..."},
									  {burgerscientist, "Take it, I don't care."},
									  {burgerscientist, "I'm already done with it."},
									  {NULL, "IVOR flips the RELEASE SWITCH."},
									  {NULL, "The quantumn chains restraining the unihorn fall off!"},
									  {NULL, "The unihorn looks at you."},
									  {NULL, "UNIHORN - *grateful neigh*"},
									  {NULL, "The unihorn jumps at the wall and passes through."},
									  {NULL, "It left behind a UNIHORN CORN in gratitude."}});
	burgerscientist->setDialogue({{burgerscientist, "..."}, {NULL, "IVOR is toiling away at some sort of quantumn machinery."}});
	burgerscientist->addRejectionDialogue({{self, "Hey wanna join my team?"}, {burgerscientist, "..."}, {NULL, "IVOR is toiling away at some sort of quantumn machinery and does not care about your offer."}});
	Conversation bsjconv = {{burgerscientist, "I see you've found the child."},
	 {burgerscientist, "She was going to be my next subject after the unihorn."},
	 {burgerscientist, "We detected unusual signs of latent energy production from within her."},
	 {burgerscientist, "You are a similar case, though you were far too developed at the time of detection,"},
	 {burgerscientist, "rendering successful attempts at capturing you improbable."},
	 {burgerscientist, "I understand you intend to return her to her mother."},
	 {burgerscientist, "I will offer you 1,000,000,000 monies in exchange for leaving the child to me."},
	 {self, "No. >:|"},
	 {burgerscientist, "Very well."}};
	shared_ptr<Conversation> bsjconv2 = make_shared<Conversation>(Conversation({{burgerscientist, "I see you have the child with you."},
	 {NULL, "JILLY hides behind you."},
	 {burgerscientist, "She was going to be my next subject after the unihorn."},
	 {burgerscientist, "We detected unusual signs of latent energy production from within her."},
	 {burgerscientist, "You are a similar case, though you were far too developed at the time of detection,"},
	 {burgerscientist, "rendering successful attempts at capturing you improbable."},
	 {burgerscientist, "With this in mind, I will offer you 1,000,000,000 monies in exchange for the child."},
	 {self, "No. >:|"},
	 {burgerscientist, "Very well."}}));
	shared_ptr<Conversation> bsjconv3 = make_shared<Conversation>(Conversation({{burgerscientist, "I see you've taken the child back to her mother."},
	 {burgerscientist, "She was going to be my next subject after the unihorn."},
	 {burgerscientist, "We detected unusual signs of latent energy production from within her."},
	 {burgerscientist, "You are a similar case, though you were far too developed at the time of detection,"},
	 {burgerscientist, "rendering successful attempts at capturing you improbable."},
	 {burgerscientist, "With this in mind, I will offer you 1,000,000,000 monies in exchange for bringing the child to me."},
	 {self, "No. >:|"},
	 {burgerscientist, "Very well."}}));
	bsjconv.skipcondition = {SAVINGJILLY};
	bsjconv.alt = bsjconv2;
	bsjconv2->skipcondition = {JILLYSAVED};
	bsjconv2->alt = bsjconv3;
	jbchanges.linkedConversations.push({burgerscientist, bsjconv});

	NPC* basementguard = new NPC(*burgerwarden);
	basementguard->setLeader(true, 28, burgbasese);
	basementguard->blockExit(DOWNSTAIRS, ENEMY, "guarded by the BURGER WARDEN.");
	basementguard->setDialogue("...");
	basementguard->addRejectionDialogue("...");

	NPC* burgercultists = new NPC(*burgercultist); //MARK: FINISH THESE GUYS
	burgercultists->setMask("", "BURGER CULTISTS", "A group of hooded figures in yellow robes cursing the BURGERs on the assembly line.");
	burgercultists->setLeader(true, 25, burgplate);
	burgercultists->setParty({burgercultist, burgercultist, burgercultist, burgercultist});
	burgercultists->setDialogue("...");
	burgercultists->addRejectionDialogue("...");
	burgercultists->addLinkedRoom(burgplate, ""); //MARK: what the heck is this?







	//choice orb effects MARK: forest temple stuff
	Effect* fbuff = new Effect("TEMPLE BUFF", 2147483647, 0, 0, 1.5, 1.5, 1.5, 1.5, 1.5);
	Effect* debuff = new Effect("TEMPLE DEBUFF", 2147483647, 0, 0, .75, .75, .75, .75, .75);
	Effect* superswaggy = new Effect("SUPER SWAGGY", 2147483647, 0, 0, 3, 3, 3, 3, 3);
	Effect* prettyswaggy = new Effect("SWAGGY", 2147483647, 0, 0, 1.75, 1.75, 1.75, 1.75, 1.75);
	
	//left path guards
	NPC* ftlguard1 = new NPC(*carnplant); //carnplant x2, smogfish (introduce smogfish)
	ftlguard1->setLeader(true, 0, forestbranchw, false);
	ftlguard1->setParty({carnplant, smogfish});
	ftlguard1->blockExit(SOUTHEAST, ENEMY, "guarded by the CARNIVOROUS PLANT.");
	ftlguard1->setScaleFight();
	ftlguard1->setFightEffects(NULL, fbuff); //buff self but not teammates
	ftlguard1->setDialogue({{NULL, "CARNIVOROUS PLANT - *snapping biting noises*"}});
	ftlguard1->addRejectionDialogue({{NULL, "CARNIVOROUS PLANT - *snapping hissing noises*"}});
	ftlguard1->addLinkedDialogue(ftlguard1, {{NULL, "Your TEMPLE BUFF faded..."}});
	ftlguard1->setTalkOnDefeat();

	NPC* ftlguard2 = new NPC(*junglenaut); //junglenaut + carnplant x2 (introduce the junglenaut + buildup)
	ftlguard2->setLeader(true, 0, forestbranchw2, false);
	ftlguard2->setParty({carnplant, carnplant});
	ftlguard2->blockExit(SOUTH, ENEMY, "guarded by the JUNGLENAUT.");
	ftlguard2->setScaleFight();
	ftlguard1->setFightEffects(NULL, debuff); //debuff self but not teammates
	ftlguard2->setDialogue({{NULL, "JUNGLENAUT - *twisting vine noises*"}});
	ftlguard2->addRejectionDialogue({{NULL, "JUNGLENAUT - *twisting vine rejection*"}});
	ftlguard2->addLinkedDialogue(ftlguard2, {{NULL, "Your TEMPLE DEBUFF faded..."}});
	ftlguard2->setTalkOnDefeat();

	NPC* ftlguard3 = new NPC(*junglenaut); //junglenaut, smogfish x3 (final test with the big target + 3 of your own team basically)
	ftlguard3->setLeader(true, 0, forestbranchw3, false);
	ftlguard3->setParty({smogfish, smogfish, smogfish});
	ftlguard3->blockExit(EAST, ENEMY, "guarded by the JUNGLENAUT.");
	ftlguard3->setScaleFight();
	ftlguard3->setFightEffects(prettyswaggy, prettyswaggy); //share buff with whole team including self
	ftlguard3->setDialogue({{NULL, "JUNGLENAUT - *twisting vine noises*"}});
	ftlguard3->addRejectionDialogue({{NULL, "JUNGLENAUT - *twisting vine rejection*"}});
	ftlguard3->addLinkedDialogue(ftlguard3, {{NULL, "Your team's SWAGGY faded..."}});
	ftlguard3->setTalkOnDefeat();
	
	//right path guards
	NPC* ftrguard1 = new NPC(*carnplant); //carnplant x2, smogfish (introduce smogfish)
	ftrguard1->setLeader(true, 0, forestbranche, false);
	ftrguard1->setParty({carnplant, smogfish});
	ftrguard1->blockExit(SOUTH, ENEMY, "guarded by the CARNIVOROUS PLANT.");
	ftrguard1->setScaleFight();
	ftrguard1->setFightEffects(fbuff, NULL); //buff teammates but not self
	ftrguard1->setDialogue({{NULL, "CARNIVOROUS PLANT - *snapping biting noises*"}});
	ftrguard1->addRejectionDialogue({{NULL, "CARNIVOROUS PLANT - *snapping hissing noises*"}});
	ftrguard1->addLinkedDialogue(ftrguard1, {{NULL, "Your teammates' TEMPLE BUFF faded..."}});
	ftrguard1->setTalkOnDefeat();

	NPC* ftrguard2 = new NPC(*junglenaut); //junglenaut + carnplant x2 (introduce the junglenaut + buildup)
	ftrguard2->setLeader(true, 0, forestbranche2, false);
	ftrguard2->setParty({carnplant, carnplant});
	ftrguard2->blockExit(SOUTHWEST, ENEMY, "guarded by the JUNGLENAUT.");
	ftrguard2->setScaleFight();
	ftrguard2->setFightEffects(debuff, NULL); //debuff teammates but not self
	ftrguard2->setDialogue({{NULL, "JUNGLENAUT - *twisting vine noises*"}});
	ftrguard2->addRejectionDialogue({{NULL, "JUNGLENAUT - *twisting vine rejection*"}});
	ftrguard2->addLinkedDialogue(ftrguard2, {{NULL, "Your teammates' TEMPLE DEBUFF faded..."}});
	ftrguard2->setTalkOnDefeat();


	NPC* ftrguard3 = new NPC(*junglenaut); //junglenaut, smogfish x3 (final test with the big target + 3 of your own team basically)
	ftrguard3->setLeader(true, 0, forestbranche3, false);
	ftrguard3->setParty({smogfish, smogfish, smogfish});
	ftrguard3->blockExit(SOUTHEAST, ENEMY, "guarded by the JUNGLENAUT.");
	ftrguard3->setScaleFight();
	ftrguard3->setFightEffects(NULL, superswaggy); //keep all the buff to yourself
	ftrguard3->setDialogue({{NULL, "JUNGLENAUT - *twisting vine noises*"}});
	ftrguard3->addRejectionDialogue({{NULL, "JUNGLENAUT - *twisting vine rejection*"}});
	ftrguard3->addLinkedDialogue(ftrguard3, {{NULL, "Your SUPER SWAGGY faded..."}});
	ftrguard3->setTalkOnDefeat();
	
	//the boss!
	NPC* ftboss = new NPC(*senseofself);
	ftboss->setMask("", "SENSE OF SELF", "A slowly swirling mass of thin purple smog.");
	ftboss->setLeader(true, 0, foresttempleboss, false);
	ftboss->setScaleFight();
	ftboss->addConversation({{NULL, "You approach the cloud of smog."},
							 {NULL, "The smog drifts away from the center of the room..."},
							 {NULL, "You see a figure standing in the smog..."},
							 {self, "What the heck?"},
							 {self, "Who are you?"},
							 {senseofself, "I'm you,"},
							 {senseofself, "but better!"}});
	ftboss->addLinkedConvo(senseofself, {{NULL, "SENSE OF SELF starts sublimating into smog..."},
							{senseofself, "Wwhhaatt?"},
							{senseofself, "Ii'mm bbetterr tthaan yyoooouuuuuuu......"},
							{NULL, "The smog fades from the room..."},
							{NULL, "You see the OUTPUT ANTENNA OF HUMILITY left behind on the ground."},
							{NULL, "Your ESCAPE ORB hardened into a STONE ORB!"}});
	ftboss->setTalkOnDefeat();
	ftboss->setForceBattle();
	ftboss->addLinkedItem(outputantenna, foresttempleboss);
	Item* feorb; //we implement this last after the rest of the forest temple stuff
	ftboss->setLinkedOrb(feorb);
	ftboss->addPaveLink(forestbuffer2, foresttemplestairs, NORTH, SOUTH); //make it so you can just walk out of the temple now
	ftboss->addRedirect(forestbuffer1, forestbuffer2); //cause they're the same room
	ftboss->addLinkedRoom(foresttempleboss, "in the forest temple arena, cleared of any traces of smog.");
	ftboss->setEscapable(false);
	ftboss->setWorldCondition(CANDISMISS); //cause you just finished the temple so you can dismiss teammates again

	shared_ptr<WorldChange> ftsink = make_shared<WorldChange>(); //when the forest temple sinks into the ground
	ftsink->exitPavings.push(make_tuple(dirtplain, foresttempleentrance, NORTH, SOUTH)); //very cool, there is a dirt plain here now
	ftsink->exitPavings.push(make_tuple(dirtplain, forest, SOUTH, NORTH));
	ftsink->exitPavings.push(make_tuple(dirtplain, forestright, EAST, WEST));
	ftsink->exitPavings.push(make_tuple(dirtplain, forestleft, WEST, EAST));
	ftsink->redirectRooms.push({foresttemplestairs, foresttempleentrance}); //set all these redirects so that they push all the items out of the temple, in case the player dropped some items or didn't take the output antenna (for some reason)
	ftsink->redirectRooms.push({forestbuffer1, foresttempleentrance});
	ftsink->redirectRooms.push({forestbuffer2, foresttempleentrance});
	ftsink->redirectRooms.push({foresttemple, foresttempleentrance});
	ftsink->redirectRooms.push({forestbranchw, foresttempleentrance});
	ftsink->redirectRooms.push({forestbranche, foresttempleentrance});
	ftsink->redirectRooms.push({foresttemple2, foresttempleentrance});
	ftsink->redirectRooms.push({forestbranchw2, foresttempleentrance});
	ftsink->redirectRooms.push({forestbranche2, foresttempleentrance});
	ftsink->redirectRooms.push({foresttemple3, foresttempleentrance});
	ftsink->redirectRooms.push({forestbranchw3, foresttempleentrance});
	ftsink->redirectRooms.push({forestbranche3, foresttempleentrance});
	ftsink->redirectRooms.push({foresttemple4, foresttempleentrance});
	ftsink->redirectRooms.push({foresttemple5, foresttempleentrance});
	ftsink->redirectRooms.push({foresttempleboss, foresttempleentrance});
	ftboss->addEnterChanges(foresttempleentrance, ftsink);
	ftboss->addLinkedWelcome(foresttempleentrance, {{NULL, "The ground rumbles..."},
		{NULL, "The forest temple starts sinking into the ground!"},
		{NULL, "Leaves are shaken from the trees around you..."},
		{NULL, "The ground closes up above the temple!"},
		{NULL, "A dirt plain lies where the forest temple once stood."}});
	
	Item* choice1; //forwards reference the orb so it can move itself with the world changes
	WorldChange choicea1;
	choicea1.exitPavings.push(make_tuple(foresttemple, forestbranchw, SOUTHWEST, NORTHEAST));
	choicea1.exitPavings.push(make_tuple(foresttemple2, forestbranchw, NORTHWEST, SOUTHEAST));
	choicea1.roomChanges.push({foresttemple, "in the forest temple where you were presented with your first choice."});
	choicea1.linkedItems.push({choice1, limbo});
	WorldChange choiceb1;
	choiceb1.exitPavings.push(make_tuple(foresttemple, forestbranche, SOUTH, NORTH));
	choiceb1.exitPavings.push(make_tuple(foresttemple2, forestbranche, NORTH, SOUTH));
	choiceb1.roomChanges.push({foresttemple, "in the forest temple where you were presented with your first choice."});
	choiceb1.linkedItems.push({choice1, limbo});
	choice1 = new ChoiceOrb("CHOICE ORB", "A shiny floating orange orb waiting to give you a choice.", foresttemple, choicea1, choiceb1,
								{{NULL, "CHOICE ORB - \"I present you with a choice:\""},
								 {NULL, "CHOICE ORB - \"A - Give yourself a TEMPLE BUFF.\""},
								 {NULL, "CHOICE ORB - \"B - Let your teammates have the TEMPLE BUFF.\""},
								 {NULL, "Which choice will you choose? (A or B)"}},
								{{NULL, "CHOICE ORB - \"In your next battle, you shall have your TEMPLE BUFF.\""},
								 {NULL, "An exit opened to the SOUTHWEST!"}},
								{{NULL, "CHOICE ORB - \"In your next battle, your teammates shall have their TEMPLE BUFF.\""},
								 {NULL, "An exit opened to the SOUTH!"}});
	choice1->setDenial("You try to TAKE the CHOICE ORB but it smacks you in the head.\nYou need to USE it instead.");

	Item* choice2;
	WorldChange choicea2;
	choicea2.exitPavings.push(make_tuple(foresttemple2, forestbranchw2, SOUTH, NORTH));
	choicea2.exitPavings.push(make_tuple(foresttemple3, forestbranchw2, NORTH, SOUTH));
	choicea2.roomChanges.push({foresttemple2, "in the forest temple, at a purple lake."});
	choicea2.linkedItems.push({choice2, limbo});
	WorldChange choiceb2;
	choiceb2.exitPavings.push(make_tuple(foresttemple2, forestbranche2, SOUTHEAST, NORTHWEST));
	choiceb2.exitPavings.push(make_tuple(foresttemple3, forestbranche2, NORTHEAST, SOUTHWEST));
	choiceb2.roomChanges.push({foresttemple2, "in the forest temple, at a purple lake."});
	choiceb2.linkedItems.push({choice2, limbo});
	choice2 = new ChoiceOrb("CHOICE ORB", "A shiny floating orange orb waiting to give you a choice.", foresttemple2, choicea2, choiceb2,
								{{NULL, "CHOICE ORB - \"I present you with a choice:\""},
								 {NULL, "CHOICE ORB - \"A - Give yourself a TEMPLE DEBUFF.\""},
								 {NULL, "CHOICE ORB - \"B - Let your teammates have the TEMPLE DEBUFF.\""},
								 {NULL, "Which choice will you choose? (A or B)"}},
								{{NULL, "CHOICE ORB - \"In your next battle, you shall have your TEMPLE DEBUFF.\""},
								 {NULL, "An exit opened to the SOUTH!"}},
								{{NULL, "CHOICE ORB - \"In your next battle, your teammates shall have their TEMPLE DEBUFF.\""},
								 {NULL, "An exit opened to the SOUTHEAST!"}});
	choice2->setDenial("You try to TAKE the CHOICE ORB but it smacks you in the head.\nYou need to USE it instead.");
	
	Item* choice3;
	WorldChange choicea3;
	choicea3.exitPavings.push(make_tuple(foresttemple3, forestbranchw3, SOUTH, NORTH));
	choicea3.exitPavings.push(make_tuple(foresttemple4, forestbranchw3, WEST, EAST));
	choicea3.roomChanges.push({foresttemple3, "in the forest temple. Large purple flowers are flowering here."});
	choicea3.linkedItems.push({choice3, limbo});
	WorldChange choiceb3;
	choiceb3.exitPavings.push(make_tuple(foresttemple3, forestbranche3, SOUTHEAST, NORTHWEST));
	choiceb3.exitPavings.push(make_tuple(foresttemple4, forestbranche3, NORTHEAST, SOUTHWEST));
	choiceb3.roomChanges.push({foresttemple3, "in the forest temple. Large purple flowers are flowering here."});
	choiceb3.linkedItems.push({choice3, limbo});
	choice3 = new ChoiceOrb("CHOICE ORB", "A shiny floating orange orb waiting to give you a choice.", foresttemple3, choicea3, choiceb3,
								{{NULL, "CHOICE ORB - \"I present you with a choice:\""},
								 {NULL, "CHOICE ORB - \"A - Share a buff with your team.\""},
								 {NULL, "CHOICE ORB - \"B - Keep the buff to yourself, hugely increasing your power.\""},
								 {NULL, "Which choice will you choose? (A or B)"}},
								{{NULL, "CHOICE ORB - \"In your next battle, your team shall share the SWAGGY effect.\""},
								 {NULL, "An exit opened to the SOUTH!"}},
								{{NULL, "CHOICE ORB - \"In your next battle, you shall have your SUPER SWAGGY effect.\""},
								 {NULL, "An exit opened to the SOUTHWEST!"}});
	choice3->setDenial("You try to TAKE the CHOICE ORB but it smacks you in the head.\nYou need to USE it instead.");

	WorldChange ftodropchanges; //the changes that happen when dropping the forest temple escape orb in order to reset it
	ftodropchanges.exitDepavings.push({foresttemple, SOUTH}); //reset the choice exit pavings
	ftodropchanges.exitDepavings.push({foresttemple, SOUTHWEST});
	ftodropchanges.exitDepavings.push({foresttemple2, NORTH});
	ftodropchanges.exitDepavings.push({foresttemple2, NORTHWEST});
	ftodropchanges.exitDepavings.push({foresttemple2, SOUTH});
	ftodropchanges.exitDepavings.push({foresttemple2, SOUTHEAST});
	ftodropchanges.exitDepavings.push({foresttemple3, NORTH});
	ftodropchanges.exitDepavings.push({foresttemple3, NORTHEAST});
	ftodropchanges.exitDepavings.push({foresttemple3, SOUTH});
	ftodropchanges.exitDepavings.push({foresttemple3, SOUTHEAST});
	ftodropchanges.exitDepavings.push({foresttemple4, WEST});
	ftodropchanges.exitDepavings.push({foresttemple4, NORTHWEST});
	ftodropchanges.roomChanges.push({foresttemple, "in the forest temple.\nYou are presented with a CHOICE ORB."}); //reset the descriptions
	ftodropchanges.roomChanges.push({foresttemple2, "in the forest temple, at a purple lake.\nYou are presented with a CHOICE ORB."});
	ftodropchanges.roomChanges.push({foresttemple3, "in the forest temple. Large purple flowers are flowering here.\nYou are presented with a CHOICE ORB."});
	ftodropchanges.linkedItems.push({choice1, foresttemple});
	ftodropchanges.linkedItems.push({choice2, foresttemple2});
	ftodropchanges.linkedItems.push({choice3, foresttemple3});

	//f(orest temple) e(ntry/escape) orb
	feorb = new EscapeOrb("ENTRY ORB", "ESCAPE ORB", "STONE ORB",
								"A shiny purple orb which you must TAKE in order to enter the forest temple.",
								"A fragile purple orb which you must DROP in order to exit the forest temple.",
								"A hard stone orb, the petrified version of the forest temple's entry/escape orb.",
								forestbuffer1, forestbuffer2, ftboss, {ftlguard1, ftlguard2, ftlguard3, ftrguard1, ftrguard2, ftrguard3}, ftodropchanges);

	//MARK: desert temple stuff
	//shadow shadow

	//shadow shadow masky

	//shadow masky masky

	//pyramon shadow masky

	//pyramon shadow shadow

	//pyramon pyramon masky masky shadow shadow

	//pyramon pyramon pyramon masky masky masky

	//dtboss

	//MARK: volcano temple stuff

	//

	//

	//

	//

	//vtboss

	//we have this so that logically you couldn't possibly have a chance of beating the BURGER MAN before getting THE PLOT DEVICE while still having the final boss who is controlling him be beatable
	Effect* powerofplot = new Effect("POWER OF PLOT", 2147483647, 0, 0, 1000, 1000, 1000, 1000, 1000);

	//finalboss

	//block exits MARK: block exits
	forestgate->blockExit(NORTH, LOCK, "blocked by a large branchy gate. There is a large keyhole in the center with deer antlers.");
	treasuregrove->blockExit(NORTH, CHASM, "blocked by a steep ravine.");
	treasurecliff->blockExit(SOUTH, CHASM, "blocked by a steep ravine.");
	ninjaland->blockExit(UP, NINJA, "too high. You need NINJA ABILITIES to scale the trees.");
	desert->blockExit(EAST, SAND, "blocked by scorching sands.");
	desertplain->blockExit(WEST, SAND, "blocked by scorching sands.");
	deserttempleentrance->blockExit(EAST, SAND, "blocked by scorching sands.");
	desertdune->blockExit(WEST, SAND, "blocked by scorching sands.");
	mineshaft->blockExit(EAST, TRACK, "blocked by a deep pit. A MINECART TRACK is set over it.");
	minespring->blockExit(NORTH, TRACK, "blocked by a deep pit. A MINECART TRACK is set over it.");
	desertstation->blockExit(ABOVEGROUND, RUBBLE, "blocked by rocky rubble.");
	canyon->blockExit(UNDERGROUND, RUBBLE, "blocked by rocky rubble.");
	mineshaft3->blockExit(WEST, TRACK, "blocked by a deep pit. A MINECART TRACK is set over it.");
	mineshaftside->blockExit(EAST, TRACK, "blocked by a deep pit. A MINECART TRACK is set over it.");
	mineshaft2->blockExit(NORTH, TRACK, "blocked by a deep pit. A MINECART TRACK is set over it.");
	mineshaft3->blockExit(SOUTH, TRACK, "blocked by a deep pit. A MINECART TRACK is set over it.");
	canyon2->blockExit(UP, RUBBLE, "blocked by rocky rubble.");
	cliff1->blockExit(DOWN, RUBBLE, "blocked by rocky rubble.");
	minelight->blockExit(UP, RUBBLE, "blocked by rocky rubble.");
	mineexit->blockExit(DOWN, RUBBLE, "blocked by rocky rubble.");
	kaboomroom->blockExit(NORTHWEST, RUBBLE, "blocked by rocky rubble.");
	minelight->blockExit(NORTH, TRACK, "blocked by a deep pit. A MINECART TRACK is set over it.");
	volcanoentrance->blockExit(SOUTH, TRACK, "blocked by a deep pit. A MINECART TRACK is set over it.");
	volcanoentrance->blockExit(NORTH, HEAT, "too hot! You'd probably melt from the radiation unless you got some SUNSCREEN!");
	sewercenter3->blockExit(DOWN, RUBBLE, "blocked by rocky rubble.");
	sewercenter4->blockExit(WEST, LOCK, "locked with a GREEN LOCK.");
	volcano2->blockExit(NORTH, LAVA, "covered by a sea of lava. Looks like it's THE END OF THE ROAD!");
	volcano4->blockExit(NORTHEAST, LAVA, "covered by a sea of lava. Looks like it's THE END OF THE ROAD!");
	volcano6->blockExit(NORTHWEST, LAVA, "covered by a sea of lava. Looks like it's THE END OF THE ROAD!");
	volcano7->blockExit(NORTH, LAVA, "covered by a sea of lava. Looks like it's THE END OF THE ROAD!");
	sewerentrance1->blockExit(DOWN, LAVA, "submerged in lava.");
	sewerentrance2->blockExit(DOWN, LAVA, "submerged in lava.");
	sewer2->blockExit(UP, COVER, "covered by a manhole cover."); //these two must be unblocked by taking the manhole cover from above (because the cover perfectly fits the hole, so logically you can't take it from below (I guess you could just push it but there's no push command))
	sewerentrance1->blockExit(UP, COVER, "covered by a manhole cover.");
	volcano2->blockExit(DOWN, LAVA, "submerged in lava."); //these two wil not show until the manhole cover is removed
	volcano6->blockExit(DOWN, LAVA, "submerged in lava.");
	sewerminessouth->blockExit(SOUTH, TRACK, "blocked by a deep pit. A MINECART TRACK is set over it.");
	mineshortcut->blockExit(NORTH, TRACK, "blocked by a deep pit. A MINECART TRACK is set over it.");
	factorybalcony2->blockExit(EAST, CHASM, "blocked by a lack of path over the central room.");
	factorygarden->blockExit(WEST, CHASM, "blocked by a lack of path.");
	factory3->blockExit(UP, HIGH, "too high up.");
	factory3->blockExit(EAST, STUFF, "blocked by collapsed roof material.");
	factorynw->blockExit(UP, HIGH, "too high up.");
	factoryne->blockExit(UP, HIGH, "too high up.");
	factoryse->blockExit(UP, HIGH, "too high up.");
	factoryse->blockExit(WEST, STUFF, "blocked by collapsed roof material.");
	factoryroofsw->blockExit(DOWN, HIGH, "too far down to jump.");
	factoryroofnw->blockExit(DOWN, HIGH, "too far down to jump.");
	factoryroofne->blockExit(DOWN, HIGH, "too far down to jump.");
	factoryroofse->blockExit(DOWN, HIGH, "too far down to jump.");
	sewermineswest->blockExit(WEST, TRACK, "blocked by a deep pit. A MINECART TRACK is set over it.");
	mountainmine->blockExit(EAST, TRACK, "blocked by a deep pit. A MINECART TRACK is set over it.");
	mainstreet2->blockExit(EAST, FIRE, "blocked by a blazing fire!");
	coolstreet2->blockExit(WEST, FIRE, "blocked by a blazing fire!");
	coolstreet2->blockExit(INSIDE, FIRE, "blocked by a blazing fire!");
	newstreet3->blockExit(EAST, FIRE, "blocked by a blazing fire!");
	mainstreet3->blockExit(WEST, FIRE, "blocked by a blazing fire!");
	leftstreet4->blockExit(EAST, FIRE, "blocked by a blazing fire!");
	newstreet4->blockExit(WEST, FIRE, "blocked by a blazing fire!");
	newstreet4->blockExit(NORTH, FIRE, "blocked by a blazing fire!");
	newstreet5->blockExit(SOUTH, FIRE, "blocked by a blazing fire!");
	mainstreet4->blockExit(EAST, FIRE, "blocked by a blazing fire!");
	coolstreet4->blockExit(WEST, FIRE, "blocked by a blazing fire!");
	coolstreet3->blockExit(NORTH, FIRE, "blocked by a blazing fire!");
	coolstreet4->blockExit(SOUTH, FIRE, "blocked by a blazing fire!");
	rightstreet4->blockExit(NORTH, FIRE, "blocked by a blazing fire!");
	rightstreet5->blockExit(SOUTH, FIRE, "blocked by a blazing fire!");
	richneighborhood1->blockExit(NORTHEAST, TEMPLE, "guarded by high-tech security systems.");
	richneighborhood2->blockExit(NORTH, TEMPLE, "guarded by high-tech security systems.");
	richneighborhood3->blockExit(NORTHWEST, TEMPLE, "guarded by high-tech security systems.");

	//MARK:

	return self; //returns the player character
}

//prints all the properties of the given room MARK: print room data
void PrintRoomData(Room* currentRoom, NPC* player = NULL) {
	currentRoom->printWelcome(); //some rooms have messages they print on arrival
	cout << "\nYou are " << currentRoom->getDescription();
	currentRoom->printExits();
	currentRoom->printNPCs();
	currentRoom->printItems();
	currentRoom->printStock(); //prints buyable items in this room
	currentRoom->printBlocks(); //prints which exits are blocked
	if (player && player->getPursuer()) { //print pursuer logic if needed
		NPC* pursuer = player->getPursuer();
		pair<int, int> pcoords = pursuer->getPurPos(currentRoom);
		if (pcoords.first < 0 || pcoords.second < 0) return; //don't give hint if not in the main grid (so no spoilers)
		pair<int, int> bcoords = pursuer->getPurPos(pursuer->getRoom());
		bool aligned = pcoords.first == bcoords.first || pcoords.second == bcoords.second;
		if (!aligned) { //vague reminder
			cout << "\nThe " << pursuer->getName() << " is near.";
			return;
		} //clear when direct line of sight
		int dist;
		const char* direction;
		if (pcoords.first != bcoords.first) {
			dist = bcoords.first - pcoords.first;
			direction = (dist < 0 ? "WEST" : "EAST");
		} else if (pcoords.second != bcoords.second) {
			dist = bcoords.second - pcoords.second;
			direction = (dist < 0 ? "NORTH" : "SOUTH");
		} else return; //when the pursuer is HERE that gets printing in printNPCs so printing it here would be redundant
		cout << "\nThe " << pursuer->getName() << " is " << abs(dist) << " room" << (abs(dist) > 1 ? "s" : "") << " to the " << direction << ".";
	}
}

//move the player and co. to a new room based on direction, or also just teleopring to forceDest if given MARK: travel
void travel(Room* currentRoom, const char* direction, vector<NPC*>* party, vector<Item*>* inventory, bool forceTravel = false, Room* forceDest = NULL) {
	Room* roomCanidate = NULL; //the room we're trying to go to
	if (forceDest != NULL) { //we just teleport to this room if given
		roomCanidate = forceDest;
	} else { //if no teleport destination is given, we try to get the room in the given direction
		roomCanidate = currentRoom->getExit(direction);
	}
	//we print an error message based on the reason roomCanidate is NULL
	if (roomCanidate == NULL) {
		if (!strcmp(direction, "")) { //give a better-looking error message than Invalid direction ""
			cout << "\nGo where?";
		//if the player gave a direction that doesn't match the commonly given ones, we call it an invalid direction
		} else if (!getCardinal(direction)) {
			cout << "\nInvalid direction \"" << direction << "\".";
		} else { //if there was a valid direction
			cout << "\nThere is no exit in that direction.";
		}
		return;
	//if there is a valid room to go to, but the exit is blocked
	//we go there anyway if we force the move (done by movement items)
	} else if (!forceTravel && currentRoom->getBlocked(direction)) {
		currentRoom->printBlock(direction);
		return;
	} else { //block movement if we have a hose item that does that
		Item* hose = getItemTypeInVector(*inventory, "hose");
		if (!hose) hose = getItemTypeInVector(currentRoom->getItems(), "hose");
		if (hose) {
			const char* blockreason = ((HoseItem*)hose)->getBlocked(currentRoom, direction); //slightly different error based on if you're holding it or not
			if (blockreason) {
				cout << blockreason; //print why we can't move
				return;
			}
		}
	}
	if (WorldState[IMPRISONED]) { 
		cout << "\nYou tried to leave but got pulled back by your chains.";
		return;
	}
	bool npcblocky = false; //if a teammate is blocking the movement
	for (NPC* npc : *party) {
		if (npc->getBlocked(currentRoom, direction)) {
			if (npcblocky) CinPause(); //make a pause from the previous npc's conversation
			npc->printBlockDialogue(false);
			npcblocky = true;
		}
	}
	if (npcblocky) return; //don't return cause a teammate is blocking moving
	if (NPC* pursuer = (*party)[0]->getPursuer()) { //handle pursuers, move every time player travel()s
		NPC* self = (*party)[0]; //get player for convenience
		if (!getCardinal(direction)) pursuer->setRoom(currentRoom); //pursuer teleports to outside room if you go in a side room
		//because of that, when you go in the elevator, the pursuer is guaranteed to be right outside
		//MARK: pursuer should not move if player is moving towards them
		pair<Room*, const char*>& special = pursuer->getSpecial();
		//catch the player when they're going up in the elevator
		if (currentRoom == special.first && direction == special.second) {
			pursuer->printCatchDialogue(true);
			pursuer->doCatchChanges();
			return;
		//catch the player if they're in the same room and trying to leave BUT let them go into the elevator for false hope or something
		} if (pursuer->getRoom() == currentRoom && roomCanidate != special.first) {
			pursuer->printCatchDialogue();
			pursuer->doCatchChanges();
			return;
		} //pursuer movement logic
		pair<int, int> pcoords = pursuer->getPurPos(currentRoom);
		pair<int, int> bcoords = pursuer->getPurPos(pursuer->getRoom());
		bool aligned = pcoords.first == bcoords.first || pcoords.second == bcoords.second;
		pair<Room*, const char*>& pdat = pursuer->getPurPlayerData();
		if (aligned) { //detect last player location seen and direction saw player moving
			pdat = {currentRoom, direction};
		} else if (pursuer->getRoom() == pdat.first) { //reset player data if the pursuer gets to the place they last saw the player
			pdat = {NULL, NULL};
		}
		if (!pdat.first && !pdat.second) { //pursuer lost track of player, go guard elevator
			if (bcoords.first == 0) pursuer->setRoom(pursuer->getRoom()->getExit("EAST")); //go to center column if not there
			else if (bcoords.first == 2) pursuer->setRoom(pursuer->getRoom()->getExit("WEST"));
			else if (bcoords.second != 0) pursuer->setRoom(pursuer->getRoom()->getExit("NORTH")); //go north if not at elevator entrance
		} else if (pursuer->getRoom() != roomCanidate) { //move towards the player they aren't moving to the pursuer's room (for some reason), just let them get there if so
			pair<int, int> dcoords = pursuer->getPurPos(pdat.first); //get last know player pos as coordinates
			if (bcoords.first < dcoords.first) pursuer->setRoom(pursuer->getRoom()->getExit("EAST")); //these are all mutually exclusive due to how this is set up
			if (bcoords.first > dcoords.first) pursuer->setRoom(pursuer->getRoom()->getExit("WEST"));
			if (bcoords.second < dcoords.second) pursuer->setRoom(pursuer->getRoom()->getExit("SOUTH"));
			if (bcoords.second > dcoords.second) pursuer->setRoom(pursuer->getRoom()->getExit("NORTH"));
		}
	}
	//rooms may redirect you to go somewhere else
	if (roomCanidate->getRedirect() != NULL) {
		roomCanidate = roomCanidate->getRedirect();
	}
	//we recapacitate all the defeated enemies in the room, so the rooms don't become all desolate after defeating them all
	roomCanidate->undefeatEnemies();
	//we level up all the NPCs if it's the gym to match the player's level-1. This is so teammates don't fall behind on level after getting benched
	if (roomCanidate->getGym()) {
		roomCanidate->scaleNPCs((*party)[0]->getLevel()-1);
	}
	//we move our party to the room, including the player
	for (NPC* npc : (*party)) {
		npc->setRoom(roomCanidate);
	}
	//if we're moving between stations, we also move our lobster if it's there, because you're riding the lobster so it makes sense
	//a side effect is that the lobster also moves between the two desert station rooms, but it's like a pet so that makes sense, no need to fix it
	if (currentRoom->getStation() && roomCanidate->getStation()) {
		//tries to find the lobster in the station
		for (NPC* npc : currentRoom->getNpcs()) {
			//if it's the lobster AND it isn't in enemy form still
			if (npc->getLobster() && !npc->getLeader()) {
				npc->setRoom(roomCanidate); //move the lobster
				break; //break because there's only one lobster
			} //also make roaming npcs roam
			if (npc->getRoaming() && !npc->getRecruited() && !npc->getLeader()) {
				npc->roam();
			}
		}
	}
	Item* roomgift = roomCanidate->popBackup(); //check if the item has a gift
	if (roomgift && !getItemInVector(*inventory, roomgift->getName())) { //if gift exists and we don't already have the item from elsewhere
		roomCanidate->setItem(roomgift); //put the item in the room
	} //do any changes the room might have to make
	roomCanidate->doEnterChanges();
	PrintRoomData(roomCanidate, (*party)[0]); //prints the data of the new current room
}

//initiates battle with an npc MARK: fight
void fight(Room* currentRoom, vector<NPC*>* party, vector<Item*>* inventory, const char* name, int& mony) {
	NPC* npc = getNPCInVector(currentRoom->getNpcs(), name); //try to find the given npc in the room
	if (npc == NULL) { //try to find npc in adjacent exits
		npc = getNPCInVector(currentRoom->getNpcs(true), name);
	}
	if (npc && npc->getQuantumn()) { //we just defeat quantumn enemies and ignore that they were ever there
		npc->defeat();
		npc = NULL;
	}
	if (npc == NULL || npc->getDefeated()) { //print error message if they're not here (or defeated and not supposed to be there technically)
		cout << "\nThere is nobody named \"" << name << "\" here.";
		return;
	}
	if (NPC* pursuer = npc->getPursuing()) { //if trying to fight the pursuer they just catch the player
		pursuer->printCatchDialogue();
		pursuer->doCatchChanges();
		return;
	}
	if (npc->getPlayerness()) { //you can't fight yourself
		cout << "\n" << name << " - \"Uhhhh you want me to fight myself?\"";
		return;
	}
	if (!npc->getLeader()) { //you can only fight npcs marked as "leader", basically another word for enemy in the overworld (except for the player who has already been confirmed to not be the given npc)
		cout << "\nYou can't fight " << name << "!";
		return;
	}
	if (npc->getConvoSize()) { //I want the player to hear all the dialogue instead of blindly fighting everyone, so we make sure if the npc has dialogue that it is said
		npc->printDialogue(true);
		cout << "\n";
	}
	if (npc->popNoFight()) { //fakeout fight, also removes leader status if marked as doing that
		return;
	}
	NPC* roamio = NULL; //add roaming npc to the party if one is here
	for (NPC* _roamio : currentRoom->getNpcs()) {
		if (_roamio->getRoaming() && !_roamio->getRecruited() && npc != _roamio) {
			roamio = _roamio;
			party->push_back(roamio);
			cout << roamio->getName() << " is fighting alongside you!";
			break;
		}
	}
	//print any dialogue that is specifically printed right before battle
	npc->printOpeningDialogue();
	if (npc->getScaleFight()) npc->setLevel((*party)[0]->getLevel()); //scale the fight to the player's level if marked as necessary
	//creates the Battle!
	Battle battle = Battle(party, npc->getParty(), inventory, mony, npc->getEscapable());
	//initiates the battle and returns an int that represents the outcome of the battle
	int battlestatus = battle.FIGHT();
	if (battlestatus == 0) { //lose
		cout << "\nDEFEAT.";
		CinPause();
		int monyLoss = mony - mony/2; //the player loses half their monies, rounding up
		mony -= monyLoss;
		cout << "\nYou lost " << monyLoss << " mon";
		if (monyLoss == 1) { //prints ending based on quantity because I love proper grammar (when it's reasonably convenient to program)
			cout << "y.";
		} else {
			cout << "ies.";
		}
		CinPause();
	} else if (battlestatus == 1) { //win
		cout << "\nVICTORY!";
		CinPause();
		mony += battle.getMonyReward(); //adds the monies to the player's balance
		//prints how much monies were earned and the new current total. I don't care about grammar here because the reward is never just 1
		cout << "\nYou earned " << battle.getMonyReward() << " monies! Now you have " << mony << " monies!";
		CinPause();
		//gives all npcs the xp reward
		for (NPC* teammate : *party) {
			teammate->addXp(battle.getXpReward());
		}
		//prints how much xp everyone got
		cout << "\nYou";
		if (party->size() > 1) { //it's kind of strange to say "your party" if it's just you, so we do a check for that
			cout << "r party members";
		}
		cout << " gained " << battle.getXpReward() << " XP!";
		CinPause();
		//prints if any teammates leveled up and prints if so
		for (NPC* teammate : *party) {
			printLvlUpData(teammate);
		}
		//sets the npc as defeated
		npc->defeat();
		//moves the npc to the room it was defeated in, if we were fighting it from a different room
		if (npc->getRoom(true) == currentRoom) {
			npc->setRoom(currentRoom);
		}
		if (npc->getTalkOnDefeat()) npc->printDialogue(true); //print defeat dialogue
		if (Item* item = npc->takeGift()) { //if the guy has a gift to give upon defeat we get the gift and add it to the inventory
			inventory->push_back(item);
			cout << name << " gave you the " << item->getName() << "!"; //says that you got the thing
			CinPause();
			//also apply any attacks to the player if they were given a weapon item
			if (!strcmp(item->getType(), "weapon")) {
				applyWeaponAttack((*party)[0], item);
				CinPause();
			}
		}

		//some NPCs have special fight endings after defeating them, so we do those checks here
		if (npc->getLobster()) { //if it was the lobster
			cout << "\nThe TUNNEL LOBSTER, now defeated, appears docile.";
			CinPause();
			cout << "TUNNEL LOBSTER - \"HhHhhhHHhhHhhHhHhhHhhhHHhh (docile lobster noises)\"";
			CinPause();
			//gives the prompt to name the lobster!
			cout << "\n" << setw(strlen((*party)[0]->getName())) << ""; //spacing
			cout <<                                    "                                                 (type your lobster's name here!)";
			cout << "\n" << (*party)[0]->getName() << " - \"Oh nice a pet lobster! I think I'll name you ";

			//gets the player input and puts it in the name charray
			char name[255];
			cin.getline(name, 255);

			AllCaps(&name[0]); //capitalizes the name
			
			if (!strcmp(name, "")) { //Bernard really wanted to name the lobster so he expresses his disappointment if the player just ENTERed immediately and didn't type a name
				cout << (*party)[0]->getName() << " - \"Ok nevermind I guess I won't name you.\"";
				CinPause();
			} else { //if the player did give a name, we set the lobster's title and name to TUNNEL LOBSTER [name]
				npc->setName(name);
				npc->setTitle("TUNNEL LOBSTER");
			} //Florian is the cannonical name so Bernard remarks on that
			if (!strcmp(name, "FLORIAN")) {
				cout << "\n" << (*party)[0]->getName() << " - \"Yeah that's what I was thinking too!\"";
				CinPause();	//      \/ \/ \/ reference to the initial naming help joke
			} else if (!strcmp(name, "HELP") || !strncmp(name, "YOUR LOBSTER'S NAME", 19)|| !strncmp(name, "YOUR LOBSTERS NAME", 18)) { //if the player followed the instructions too literally
				cout << "\n" << (*party)[0]->getName() << " - \"...\"";
				CinPause();
			}
			
			cout << "\nSuccessfully tamed TUNNEL LOBSTER";
			if (strcmp(name, "")) {//if we actually named the lobster, we print the rest of the name. We can get away with always printing "TUNNEL LOBSTER" because the original name and new title are the same
				cout << " " << npc->getName();
			}
			cout << "!";
			//gives instructions on how to use your new pet lobster, should be understandable enough
			cout << "\nUtilize the abandoned train tunnels by USE-ing " << npc->getName() << ".";
			CinPause();

			if (WorldState[IMPRISONED]) { //if this is the escape sequence
				WorldState[IMPRISONED] = false;
				if (strlen(name)) cout << "\n" << name;
				else cout << "\nYour new pet lobster";
				cout << name << " snips off your shackles.";
				CinPause();
				Conversation c = {{(*party)[0], "Nice!"},
					{(*party)[0], "Hey are you coming?"},
					{NULL, "ARCHIBALD - \"No,\""},
					{NULL, "ARCHIBALD - \"I don't want to attract unnecessary attention from the BURGER MAN.\""},
					{NULL, "ARCHIBALD - \"Just focus on your quest; don't worry about me!\""},
					{(*party)[0], "Alright."}};
				printConversation(&c, true);
			}
			
			CinIgnoreAll(); //clear extra text and potential error flags

			npc->undefeat(); //make sure the lobster isn't defeated after the battle so you can try using it
			npc->setLeader(false); //make it not a leader so you can't fight it and so you can use it
		} else if (npc->getThief()) {
			mony = 0; //if you were fighting someone marked "thief" you lose all your monies (doesn't necessarily mean they were the thief, just means you lose all your monies)
		}
	}
	if (roamio) party->erase(remove(party->begin(), party->end(), roamio), party->end());
	//prints the room data after battle so that the player can reorient themselves
	PrintRoomData(currentRoom);
}

//takes an item from the current room and adds it to the inventory MARK: take item
void takeItem(Room* currentRoom, vector<Item*>* inventory, const char* itemname, NPC* player) { //I don't want to have to pass player here but we need to teach player attacks when taking weapon items
	//finds the item in the room based on the name
	Item* item = getItemInVector(currentRoom->getItems(), itemname);
	//prints the reason you can't take the item based on the circumstances
	if (item == NULL) {
		if (!strcmp(itemname, "")) { //better error message than There is no "" here. imo
			cout << "\nTake what?"
		//if the player tried to take an item on sale we say you can't steal
		} else if (getItemInVector(currentRoom->getStock(), itemname) != NULL) {
			cout << "\nThe " << itemname << " is for sale! You can't just take it.";
		//you can't take an item if you already took it
		} else if (getItemInVector(*inventory, itemname) != NULL) {
			cout << "\nYou're already carrying this item!";
		} else { //otherwise the player is trying to take something that isn't even there
			cout << "\nThere is no \"" << itemname << "\" here.";
		}
		return;
	} //you're not allowed to take items if they're being guarded
	if (NPC* guard = item->getGuard()) {
		cout << "The " << itemname << " is being guarded by " << guard->getName() << ".";
		return;
	}
	if (!strcmp(item->getType(), "escapeorb") && player->getParty()->size() < 4) { //escape orbs must be taken with 4 people so that the temples can actually use their themes and stuff 
		size_t partysize = player->getParty()->size();	//here we say the together for emphasis unless it's just you
		cout << "\nYou tried to take the ENTRY ORB " << (partysize > 1 ? "together " : "") << "with your party of " << partysize << ".";
		CinPause();
		cout << "The ENTRY ORB didn't budge!";
		CinPause();
		cout << "You need a full team of 4 people to take it out of its slot!";
		return;
	} //you're not allowed to take items not marked as takable
	if (!item->getTakable()) {
		cout << "\n";
		if (strcmp(item->getDenial(), "")) { //if there is a custom denial we use it
			cout << item->getDenial();
		} else { //otherwise say the generic denial
			cout << "You can't take the " << itemname << "!";
		}
		return;
	}
	item->unRoom(); //removes the item from the room
	inventory->push_back(item); //adds it to the inventory
	cout << "\nYou took the " << itemname << ".";
	//taking manhole covers reveals a new exit below! (only the first time you take it)
	if (!strcmp(item->getType(), "manhole")) {
		//gets the item in its true form (the subclass)
		ManholeItem* cover = (ManholeItem*)item;
		if (cover->getRoom() != NULL) { //if there is a room to reveal, we set an exit downwards (there's no horizontal manholes in this game)
			currentRoom->setExit(cover->getDirection(), cover->getRoom());
			cover->getRoom()->unblockExit(ReverseDirection[cover->getDirection()]); //also unblock the exit from below
			cout << "\nAn exit DOWNwards was revealed!";
		}
	} else if (!strcmp(item->getType(), "weapon")) { //weapon items enable the use of their move
		CinPause();
		applyWeaponAttack(player, item);
	} else if (!strcmp(item->getType(), "worldchange")) {
		WorldChangeItem* changer = (WorldChangeItem*)item;
		if (!changer->getTakeToUse()) return;
		applyWorldChange(changer->getChanges()); //do the changes
		printConversation(&changer->getUseText(), false); //prints what the player did and what it accomplished
		deleteItem(currentRoom, inventory, item); //we shouldn't keep world change items because the taking was the using and they're consumables
	} else if (!strcmp(item->getType(), "escapeorb")) { //escape orbs make you enter the temple when taken
		CinPause();
		EscapeOrb* orb = (EscapeOrb*)item;
		cout << "\nThe temple door behind you shut!";
		CinPause();
		cout << "The ENTRY ORB turned into an ESCAPE ORB!";
		orb->take();
		CinPause(); //pause after the transformation message
		WorldState[CANDISMISS] = false; //can't dismiss teammates in temple
		travel(currentRoom, NULL, player->getParty(), inventory, true, orb->getDestination()); //go into the temple
	}
}

//drops an item from the inventory into the current room MARK: drop item
void dropItem(Room* currentRoom, vector<Item*>* inventory, const char* itemname, NPC* player) { //pass player for the same reason as takeItem
	Item* item = getItemInVector(*inventory, itemname); //finds the item in the inventory
	if (item == NULL) { //gives error message if we have no itemname
		if (!strcmp(itemname, "")) { //cool error message, very natural response
			cout << "\nDrop what?";
		} else { //if the player actually did try to drop something weird
			cout << "\nYou have no \"" << itemname << "\"."; //I know ". is grammatically inaccurate but it looks way better than ."
		}
		return;
	}
	if (!strcmp(item->getType(), "escapeorb")) { //the escape/entry orbs require extra confirmation before being dropped due to their effects
		EscapeOrb* orb = (EscapeOrb*)item;
		if (!orb->getInert()) {
			cout << "\nAll your progress in the temple will be reset.";
			CinPause();
			cout << "Are you sure you want to drop your " << item->getName() << "?";
			if (!AOrB(NULL, "YES", "NO")) return;
		}
	}
	item->setRoom(currentRoom); //puts the item in the current room
	//erases the item from the inventory
	inventory->erase(remove(inventory->begin(), inventory->end(), item), inventory->end());
	cout << "\nYou dropped the " << itemname << ".";
	if (!strcmp(item->getType(), "weapon")) { //weapon items enable the use of their move
		CinPause();
		WeaponItem* weapon = (WeaponItem*)item;
		Attack* attack = weapon->getAttack();
		player->removeSpecialAttack(attack);
		cout << player->getName() << " can no longer use " << attack->name << ".";
		CinPause();
	} else if (!strcmp(item->getType(), "escapeorb")) { //escape orbs reset their temples and teleport you out
		CinPause();
		EscapeOrb* orb = (EscapeOrb*)item;
		if (!orb->getInert()) { //make it reset the temple plus a pause, but you can just drop stone orbs normally
			cout << "\nThe " << item->getName() << " shattered!";
			orb->drop();
			CinPause(); //pause after the shatter message
			WorldState[CANDISMISS] = true; //can dismiss teammates outside of temple
			travel(currentRoom, NULL, player->getParty(), inventory, true, orb->getEntrance()); //go back to the temple entrance
		}
	}
}

//uses an item, with functionality based on type MARK: use item
void useItem(Room* currentRoom, vector<Item*>* inventory, vector<NPC*>* party, const char* itemname, int& mony) {
	//in addition to items, you can also USE the tunnel lobster for fast travel
	//it's probably bad practice to have this here, but it's functional practice! :)
	NPC* lobster = NULL; //we loop through the rooms npcs to find the lobster. We don't use the getNPCInVector function because then you can't use it if the lobster name matches an npc name
	for (NPC* npc : currentRoom->getNpcs()) {
		if (npc->getLobster() && !strcmp(npc->getName(), itemname)) { //if it's the lobster and the names match, set the lobster to that npc
			lobster = npc;
			break; //break because the lobster was found
		}
	} //if the lobster was found, we start the lobster USE-ing process
	if (lobster != NULL) { //checks the lobsteriness of the potential lobster
		if (lobster->getLeader()) { //we can't use the lobster if it's still angry and undefeated
			cout << "\nYou can't use that untamed lobster!";
			return;
		} //we can't use the lobster twice in a row, because that wouldn't make any sense
		if (currentRoom == lobster->getHome()) {
			cout << "\nYou are already in the tunnels!";
			return;
		}
		if (Item* hose = getItemTypeInVector(*inventory, "hose")) { //block usage of tunnels if we have a hose item
			cout << ((HoseItem*)hose)->getStationBlock();
			return;
		} //prints flavor text based on if you have teammates or if it's just you
		cout << "\n" << itemname << " carried you";
		if (party->size()) { //check for partymates
			cout << "r party";
		}
		cout << " to the train station tunnels!";
		CinPause();
		//travels to the tunnels!
		travel(currentRoom, NULL, party, inventory, true, lobster->getHome());
		return;
	}
	if (!strcmp(itemname, "BANK")) { //if using the bank
		NPC* banker = NULL; //we loop through the rooms npcs to find the banker
		for (NPC* npc : currentRoom->getNpcs()) {
			if (npc->getBanker()) {
				banker = npc;
				break; //break because the banker was found
			}
		}
		if (banker) { //if we found a banker do the banking
			banker->depositMonies(mony);
			return; //return so we don't print error saying "There is no BANK here!"
		}
	}
	//this is actually using item code
	Item* item = getItemInVector(*inventory, itemname); //finds the item in our inventory
	char itemName[255] = "";
	char npcName[255] = "";
	NPC* npc = NULL; //npc we may or may not target
	if (item == NULL) { //finds the item in the room if not in the inventory
		item = getItemInVector(currentRoom->getItems(), itemname);
	} //if the item is still null, we check if the player was trying to use an item ON an npc
	if (item == NULL) {
		ParseWithON(itemname, &itemName[0], &npcName[0]); //parses the rest of the command, seperated by " ON "
		item = getItemInVector(*inventory, itemName); //gets the item in the inventory
		if (item == NULL && getItemInVector(currentRoom->getItems(), itemName) != NULL) { //if the item needs to clarify who to use it on, we're not allowed to use it unless we're holding it
			cout << "\nYou can't use the " << itemName << " if you aren't holding it!";
			return;
		}
		if (item != NULL) { //we try to find the npc we're targetting
			npc = getNPCInVector(*party, npcName);
			if (!item->getTargetNeeded()) { //no need to clarify the target if one isn't required (we check for this because I thought the commands would look weird otherwise, like "USE SWITCH ON BOB", like what does Bob have to do with this? It's a switch!)
				cout << "\nThe " << itemName << " doesn't need a target!";
			} else if (npc == NULL) { //print if no npc matching the name was found, also since we're not in battle you can only use targeted items on your party
				cout << "\nThere is nobody named \"" << npcName << "\" in your party!";
			}
			return;
		//if the item IS null and the name of the item isn't nothing (player didn't "USE " or "USE" <-- like that's the command we're checking for)
		} else if (strcmp(itemName, "")) {
			itemname = itemName; //makes the null item print not say "You have no [item] ON [npc]"
		}
	}
	if (!strcmp(itemname, "")) { //error message if the player didn't clarify what exactly to use
		cout << "\nUse what?";
		return;
	} else if (item == NULL) { //print that no item called itemname was found
		cout << "\nYou have no \"" << itemname << "\".";
		return;
	} else if (NPC* guard = item->getGuard()) { //can't use items being guarded
		cout << "\nThe " << itemname << " is being guarded by " << guard->getName() << ".";
		return;
	} else if (item->getDropToUse() && !item->getRoom()) { //if item room is NULL that means it's in the inventory
		cout << "\nThe " << itemname << " must be on the ground to use it.";
		return;
	} else if (item->getTargetNeeded() && npc == NULL && strcmp(item->getType(), "key") && strcmp(item->getType(), "movement")) { //if the item needed a target but no " ON " was given we give error text
		if (party->size() > 1) { //if the party isn't only the player
			cout << "\nThis item needs a target!";
			return;
		}
		npc = (*party)[0]; //if we only have the player in the party we just use them because who else would we be targetting
	}

	//gives xp to the target
	if (!strcmp(item->getType(), "xp")) {
		XpItem* xp = (XpItem*)item; //converts to the corresponding subclass
		cout << npc->getName() << " gained " << xp->getXp() << " XP!";
		npc->addXp(xp->getXp()); //adds the xp
	//beats the game until I rework this
	} else if (!strcmp(item->getType(), "BURGER")) {
		BURGERItem* boiga = (BURGERItem*)item; //get BURGER as BURGERItem
		printConversation(&boiga->getConfirmText(), false);
		if (!AOrB(NULL, "YES", "NO")) return;
		printConversation(&boiga->getUseText(), true);
		printConversation(&boiga->getHintText(), true);
		WorldState[GAMEEND] = true; //mark that the game ended because this item ends the game
	//teaches the player character new attacks
	} else if (!strcmp(item->getType(), "education")) {
		EducationItem* edu = (EducationItem*)item; //converts to the corresponding subclass
		NPC* player = (*party)[0];
		vector<Attack*> attacks = edu->getAttacks(); //get all the item's attacks to add them to the player's vector of attacks
		vector<Attack*> futureattacks;
		for (Attack* attack : attacks) { //move all currently unusable attacks to the future attacks vector
			if (attack->minLevel > player->getLevel()) {
				futureattacks.push_back(attack);
				attacks.erase(remove(attacks.begin(), attacks.end(), attack), attacks.end());
			}
		}
		size_t amount = attacks.size(); //get this so we can easily check if we shouldn't CinPause() at the end
		for (size_t i = 0; i < amount; i++) { //add and describe all the new attacks we learned
			Attack* attack = attacks[i]; //get the attack to add
			player->addSpecialAttack(attack);
			//print the description and name of any attacks we have just learned
			cout << "\n" << player->getName() << " learned " << attack->name << "!\n" << attack->name << " - " << attack->trueDesc;
			if (i < amount) CinPause(); //do not print the final pause cause then the > looks bad
		}
		for (Attack* attack : futureattacks) player->addSpecialAttack(attack); //add all the remaining attacks to the pool for use in the future but do not announce them
		if (attacks.empty()) { //prints motivational message if none of these attacks are usable yet
			cout << "\nYou're not LEVELED UP enough to use any of these new attacks.\nKeep training and you'll get there eventually!";
		} else if (!futureattacks.empty()) { //print this message so the player knows they will also learn more attacks in the future
			cout << "\nYou're not LEVELED UP enough to use " << (futureattacks.size() > 1 ? "some" : "one") << " of these new attacks.\nKeep training and you'll get there eventually!";
		}
	//summons the tunnel lobster to the current train station
	} else if (!strcmp(item->getType(), "caller")) {
		if (!currentRoom->getStation()) { //gives error if not used in a train station
			cout << "\nThe " << itemname << " must be used in a train station!";
			return;
		} //converts to the corresponding subclass
		CallerItem* caller = (CallerItem*)item;
		printConversation(&caller->getUseText(), true); //prints what the caller did
		npc = caller->getCalledNPC(); //makes the npc easier to reference
		if (!npc->getLeader()) { //if the lobster is tamed
			if (npc->getRoom() == currentRoom) { //if the lobster is already here he just dances
				cout << "\n" << npc->getName() << " did a lobstery dance.";
				return;
			} //moves the lobster to the station
			npc->setRoom(currentRoom);
			if (WorldState[IMPRISONED]) { //your tamed lobster saves you if you're imprisoned
				cout << "\nYou feel the ground rumbling..."; //these aren't part of the conversation so we can print the lobster name
				CinPause();
				cout << npc->getName() << " burst through the wall!";
				CinPause();
				Conversation c = {{NULL, "You all get sent flying along with the rubble."},
					{NULL, "You're no longer chained to the wall!"},
					{npc, "HHHhHHHhHHhHHhHHhHh (triumphant lobster noises)"},
					{(*party)[0], "MY LOBSTER!!!! :D"}};
				printConversation(&c, true);
				if (strcmp(npc->getName(), "TUNNEL LOBSTER")) cout << npc->getName();
				else cout << "Your TUNNEL LOBSTER";
				cout << " snips off your shackles.";
				CinPause();
				c = {{(*party)[0], "Nice!"},
					{(*party)[0], "Hey are you coming?"},
					{NULL, "ARCHIBALD - \"No,\""},
					{NULL, "ARCHIBALD - \"I don't want to attract unnecessary attention from the BURGER MAN.\""},
					{NULL, "ARCHIBALD - \"Just focus on your quest; don't worry about me!\""},
					{(*party)[0], "Alright."}};
				printConversation(&c, true);
				npc->doLobsterChanges(); //make the prison reflect the fact that a big lobster just burst through the wall
				WorldState[IMPRISONED] = false;
			} else cout << "\n" << npc->getName() << " burst out of the rubble!"; //flavor text
		} else { //if the lobster is still untamed (undefeated)
			if (npc->getRoom() == currentRoom) { //if the lobster is already here prints flavor text
				cout << "\n" << npc->getName() << " shrieked angrily!";
			} else { //moves the lobster here if not here
				npc->setRoom(currentRoom);
				if (WorldState[IMPRISONED]) {
					cout << "\nYou feel the ground rumbling...";
					CinPause();
					cout << "\nA " << npc->getName() << " angrily burst through the wall!";
					CinPause();
					Conversation c = {{NULL, "You all get sent flying along with the rubble."},
						{NULL, "You're no longer chained to the wall!"},
						{(*party)[0], "Let's go!"},
						{npc, "HHHHhHHHhHHhHhHHhHHhHh (angry lobster noises)"},
						{(*party)[0], "Oh shoot :|"}};
					printConversation(&c, false); //no final pause cause we do one down there
					npc->doLobsterChanges(); //make the prison reflect the fact that a big lobster just burst through the wall
				} else cout << "\nA " << npc->getName() << " angrily burst out of the rubble!";
			}
			CinPause(); //the caller initiates a battle if lobster is untamed
			fight(currentRoom, party, inventory, npc->getName(), mony);
		}
		//sets the tunnel exit back to the station. This way, you can only go to a station if you've already been there
		//a side effect is that the desert fast travel is whichever one you fast travelled from first (since there's two), but they're right next to each other so it doesn't really matter
		npc->getHome()->setExit(npc->getTunnelDirection(currentRoom), currentRoom);
	//donation?????
	} else if (!strcmp(item->getType(), "toll")) {
		TollItem* toll = (TollItem*)item;
	//used for unblocking blocked exits
	} else if (!strcmp(item->getType(), "key") || !strcmp(item->getType(), "hose")) {
		KeyItem* key = (KeyItem*)item; //converts to the corresponding subclass
		vector<const char*> exitsUnlocked; //the exits that were unlocked in the key using process, so we can unlock the other side also
		
		bool used = false; //if we ended up actually using the key
		if (key->getTargets().size()) { //some keys are remote keys, and unlock specified targets
			//unblocks all the exits in all the rooms. We don't check reverse direction because I can just manually put the other room in the target rooms vector
			for (Room* target : key->getTargets()) {
				target->unblockAll(key->getUnlockType());
			}
			used = true; //if I made a remote key its definitely unlocking something, so used = true 100% here of the time
		} else { //unblocks exits in the current room
			exitsUnlocked = currentRoom->unblockAll(key->getUnlockType());
			used = exitsUnlocked.size() > 0; //we check the size of unblocked exits as a basis for if we used the item
			//unblocks the other sides of the exits, because there's some double sided blocks
			for (const char* exit : exitsUnlocked) {
				Room* thatroom = currentRoom->getExit(exit); //gets the room on the other side of the exit
				//unblocks the reverse direction exit if it's blocked
				if (thatroom->getBlocked(ReverseDirection[exit])) {
					thatroom->unblockExit(ReverseDirection[exit]);
				}
			}
		} //if we didn't use it, we print an error message
		if (!used) {
			cout << "\nThere is nothing to use the " << itemname << " on here.";
			return;
		} //prints a description of what the key item did (because it's not only literal keys)
		printConversation(&key->getUseText(), false);
	//movement items are used to go through a blocked exit despite the fact that it's blocked, for example a boat over a river
	} else if (!strcmp(item->getType(), "movement")) {
		MovementItem* mover = (MovementItem*)item; //converts to the corresponding subclass
		for (const char* exit : currentRoom->getBlocks()) { //tries to find a blocked exit that matches the movement item's block type
			if (currentRoom->getBlockReason(exit) == mover->getUnlockType()) {
				printConversation(&mover->getUseText(), true); //prints what exactly the movement item did
				travel(currentRoom, exit, party, inventory, true); //force travels to the found room
				return; //returns so we don't teleport to another room (and movement items don't get used up anyway, so no need for the deletion check)
			}
		} //prints error message if no matching blocked exit was found
		cout << "\nYou can't use the " << itemname << " here.";
	//paver items create a new exit
	} else if (!strcmp(item->getType(), "paver")) {
		PaverItem* paver = (PaverItem*)item; //converts to the corresponding subclass
		//if the item isn't usable here we give an error message
		if (!paver->getUsable(currentRoom)) {
			cout << "\nYou can't use the " << itemname << " here!";
			return;
		}
		//sets the exit to the room in the given direction
		currentRoom->setExit(paver->getDirection(), paver->getDestination());
		//sets the exit back to the current room in the reverse of the given direction
		paver->getDestination()->setExit(const_cast<char*>(ReverseDirection[paver->getDirection()]), currentRoom);
		//prints what exactly the paver item did
		printConversation(&paver->getUseText(), false);
	//info items print some info
	} else if (!strcmp(item->getType(), "info")) {
		InfoItem* info = (InfoItem*)item; //converts to the corresponding subclass
		cout << "\n" << info->getText(); //prints the info
	//treasure chest items either give monies or are trapped and start a battle
	} else if (!strcmp(item->getType(), "treasure")) {
		TreasureItem* treasure = (TreasureItem*)item; //converts to the corresponding subclass
		cout << "\nYou opened the " << itemname << "...";
		CinPause();
		if (NPC* mimic = treasure->getMimic()) { //if the item has a mimic
			mimic->setRoom(currentRoom);
			cout << "\nYou were met with a " << mimic->getName() << "!";
			CinPause();
			fight(currentRoom, party, inventory, mimic->getName(), mony);
		} else {
			if (int monies = treasure->getMony()) { //if the treasure has monies
				mony += monies; //adds the mony to the player's mony balance
				cout << "\nYou got " << monies << " monies! You now have " << mony << " monies!"; //says how much they got and new balance
			}
			if (treasure->getItems().size()) { //if the treasure had an item
				vector<Item*>& items = treasure->getItems();
				size_t amount = items.size();
				cout << "\nYou ";
				if (treasure->getMony()) cout << "also ";
				cout << "got a ";
				for (size_t i = 0; i < amount; i++) {
					items[i]->unRoom(); //removes the item from the room
					inventory->push_back(items[i]); //adds it to the inventory
					cout << items[i]->getName();
					if (i < amount) cout << " and a ";
				}
				cout << "!";
				//also make sure that weapon items give their attack
				for (size_t i = 0; i < amount; i++) {
					if (!strcmp(items[i]->getType(), "weapon")) {
						CinPause(); //pauses the last weapon printing, and also all the previous stuff!
						applyWeaponAttack((*party)[0], items[i]);
					}
				}
			}
		}
	//switch items are in one factory and switch the direction of all the conveyor belts
	} else if (!strcmp(item->getType(), "switch")) {
		ConveyorSwitch* cswitch = (ConveyorSwitch*)item; //converts to the corresponding subclass
		//switches all the conveyors
		for (Room* cveyor : cswitch->getConveyors()) {
			cveyor->switchConveyor();
		} //describes what just happened
		cout << "\nYou pulled the switch to the other side. All the assembly lines have switched directions!";
	//world change items do changes to the world, just like when npcs get defeated and do all that stuff
	} else if (!strcmp(item->getType(), "worldchange")) {
		WorldChangeItem* changer = (WorldChangeItem*)item;
		if (changer->getTakeToUse()) { //you can't use takeToUse world change items normally
			cout << "\nYou can't use the " << itemname << "!";
			return;
		}
		if (!changer->getUsable(currentRoom)) {
			cout << "\nThere's nothing to use the " << itemname << " for here!";
			return;
		}
		applyWorldChange(changer->getChanges()); //do the changes
		printConversation(&changer->getUseText(), false); //prints what the player did and what it accomplished
	//blender items blend a group of items into a new item
	} else if (!strcmp(item->getType(), "blender")) {
		BlenderItem* blender = (BlenderItem*)item;
		vector<const char*> missingingredients;
		vector<Item*> foundingredients;
		for (const char* ingredient : blender->getIngredients()) { //check if weave all the ingredients by sorting them into have and not have
			bool wegotit = false;
			for (Item* item : *inventory) {
				if (!strcmp(item->getName(), ingredient)) {
					foundingredients.push_back(item);
					wegotit = true;
					break;
				}
			}
			if (!wegotit) missingingredients.push_back(ingredient);
		}
		if (!missingingredients.empty()) { //print which ingredients we still need
			cout << "\nYou don't have the required ingredients to use the " << itemname << "!\nMissing ingredients: " << missingingredients[0];
			int len = missingingredients.size();
			if (len > 2) cout << ",";
			cout << " ";
			for (int i = 1; i < len - 1; i++) { //print the rest of the items minus the last one if there are 3+ items
				cout << missingingredients[i] << ", "; //seperate with comma
			}
			if (len > 1) { //print the final item with "and" included (if >1 items)
				cout << "and " << missingingredients[len - 1];
			}
			return; //return so we don't give anything
		}
		for (Item* item : foundingredients) { //delete all the items being used up
			deleteItem(currentRoom, inventory, item);
		}
		Item* product = blender->getProduct();
		product->unRoom(); //removes the item from the room
		inventory->push_back(product); //adds it to the inventory
		printConversation(&blender->getUseText(), true); //prints the blender's use text
		applyWorldChange(blender->getChanges());
		cout << "\nYou got the " << product->getName() << "!";
	//choice orbs give you a choice between choice A and choice B
	} else if (!strcmp(item->getType(), "choiceorb")) {
		((ChoiceOrb*)item)->CHOICE(); //do the choosing
	//you can't use materials; they get a unique error message
	} else if (!strcmp(item->getType(), "material")) {
		cout << "\nYou can't use the " << itemname << "!";
		return;
	//escape orbs get a unique error message as well
	} else if (!strcmp(item->getType(), "escapeorb")) {
		((EscapeOrb*)item)->printUseError();
		return;
	} else { //other types of items must be used in battles
		cout << "\nThe " << itemname << " can only be used in battle!";
		return;
	} //if the item is one-use only we delete the item
	if (item->getConsumable()) {
		deleteItem(currentRoom, inventory, item);
	}
}

//recruit an npc into the player party MARK: recruit
void recruitNPC(Room* currentRoom, const char* npcname, vector<NPC*>* party, int maxParty = 4) {
	NPC* npc = getNPCInVector(currentRoom->getNpcs(), npcname); //find the npc we're trying to recruit
	if (npc == NULL) { //try to find npc in adjacent exits
		npc = getNPCInVector(currentRoom->getNpcs(true), npcname);
	}
	if (npc && npc->getQuantumn()) { //we just defeat quantumn enemies and ignore that they were ever there
		npc->defeat();
		npc = NULL;
	}
	if (npc == NULL) { //error message if nobody in the current room is named npcname
		if (!strcmp(npcname, "")) cout << "\nRecruit who?"; //or if the player typed nothing
		else cout << "\nThere is nobody named \"" << npcname << "\" here.";
		return;
	} //you can't recruit yourself because you're obviously in your own party
	if (npc->getPlayerness()) {
		cout << "\n" << npcname << " - \"Huh?\"";
		CinPause();
		cout << "\nYou are already in your own party? ...";
		return;
	} //if the npc isn't recruitable we give error message and the npc says something
	if (!npc->getRecruitable()) {
		npc->printRejectionDialogue();
		cout << "\n" << npcname << " was not added to your party.";
		return;
	} //you can't rerecruit npcs, we don't want them being extra recruited
	if (npc->getRecruited()) {
		cout << "\n" << npcname << " is already in your party...";
		return;
	} //I guess a better name for fifth teammates would be extra teammates but the only two of these in the game never overlap anyway so it works
	size_t newpartysize = 1; //count the size of the new party, starting with 1 due to the new teammate which we're trying to add
	for (NPC* npc : *party) {
		if (!npc->getFifth()) newpartysize++;
	} //you're not allowed to have more than 4 party members (including yourself) otherwise that would be very unbalanced
	if (newpartysize > maxParty && !npc->getFifth()) {
		cout << "\nYour party is full!";
		return;
	}
	//adds the npc to your party
	party->push_back(npc);
	npc->printRecruitmentDialogue(); //print the recruitment dialogue
	npc->Recruit(); //sets the npc to recruited
	cout << "\n" << npcname << " was added to your party!"; //prints success text
	if (!npc->getFifth()) cout << " (Party size: " << newpartysize << "/" << maxParty << ")";
	if (currentRoom->getGym()) { //if we're in a gym, print the fruits of the npc's training
		printLvlUpData(npc);
		npc->setGymStart(0); //no longer training so we reset training time
	}
}

//decruit npcs from your party MARK: dismiss
void dismissNPC(Room* currentRoom, const char* npcname, vector<NPC*>* party) {
	NPC* npc = getNPCInVector(currentRoom->getNpcs(), npcname); //find the npc to dismiss
	if (npc == NULL) { //try to find npc in adjacent exits
		npc = getNPCInVector(currentRoom->getNpcs(true), npcname);
	}
	if (npc == NULL) { //error text if no npc named npcname was found
		if (!strcmp(npcname, "")) cout << "\nDismiss who?"; //or if the player typed nothing
		cout << "\nThere is nobody named \"" << npcname << "\" in your party.";
		return;
	} //you can't dismiss yourself/the main character because that makes no sense
	if (npc->getPlayerness()) {
		cout << "\n" << npcname << " - \"Huh? You can't dismiss me bro I'm the main character!\"";
		CinPause();
		cout << "\n" << npcname << " was not dismissed.";
		return;
	} //you can't dismiss someone who isn't recruited anyway
	if (!npc->getRecruited()) {
		cout << "\n" << npcname << " is not in your party...";
		return;
	} //you can't dismiss the npc because I said so
	if (!npc->getDismissable()) {
		npc->printDismissalRejection();
		cout << "\n" << npcname << " was not dismissed.";
		return;
	} //you can't dismiss the npc because it's in a temple where you need your team or the final boss prelude where you wouldn't be able to get them back
	if (!WorldState[CANDISMISS]) {
		cout << "\nYour team has no way to go back home right now!";
		return;
	} //removes the npc from your party
	party->erase(remove(party->begin(), party->end(), npc), party->end());
	//if you dismiss a partymate in the gym, it leaves them there so they can train, so we check for that
	bool gym = currentRoom->getGym();
	if (gym) {
		cout << "\n" << npcname << " is now on the GRIND at the gym, and will now train to stay at your level!";
		npc->setGymStart(time(NULL)); //track when they started training
	} else { //in every other room, they just say something and go back home
		npc->printDismissalDialogue();
		cout << "\n" << npcname << " left your party.";
	}
	//sets the npc to dismissed
	npc->Dismiss(!gym);
}

//prints anything the targeted npc has to say MARK: print dialogue
void printNPCDialogue(Room* currentRoom, const char* npcname, vector<Item*>* inventory, vector<NPC*>* party, int& mony) {
	NPC* npc = getNPCInVector(currentRoom->getNpcs(), npcname); //finds the npc named npcname
	if (npc == NULL) { //try to find npc in adjacent exits
		npc = getNPCInVector(currentRoom->getNpcs(true), npcname);
	}
	if (npc && npc->getQuantumn()) { //we just defeat quantumn enemies and ignore that they were ever there
		npc->defeat();
		npc = NULL;
	}
	if (npc == NULL) { //open the temple if we were just ASKing NICELY and it's the temple quest and we're at a temple entrance
		if (WorldState[TEMPLEQUEST] && currentRoom->getTempleEntrance() && !strcmp(npcname, "NICELY")) {
			currentRoom->openTemple();
		} else if (!strcmp(npcname, "")) { //error if the player just didn't give a name
			cout << "\nAsk who?";
		} else { //error message if no such npc is in the current room
			cout << "\nThere is nobody named \"" << npcname << "\" here.";
		}
		return;
	} 
	if (NPC* pursuer = npc->getPursuing()) { //if trying to ask the pursuer they just catch the player
		pursuer->printCatchDialogue();
		pursuer->doCatchChanges();
		return;
	} //tells the npc to print their dialogue normally
	npc->printDialogue(false);
	
	//some npcs give gifts after talking so we check for that here
	Item* item = npc->takeGift();
	if (item != NULL) { //adds the gift to the inventory
		inventory->push_back(item);
		CinPause();
		cout << npcname << " gave you the " << item->getName() << "!"; //says that you got the thing
		//also apply any attacks to the player if they were given a weapon item
		if (!strcmp(item->getType(), "weapon")) {
			CinPause();
			applyWeaponAttack((*party)[0], item);
		}
	}
	//some npcs fight you immediately after talking so if that's the case we initiate battle here
	if (npc->getForceBattle()) {
		cout << "\n";
		fight(currentRoom, party, inventory, npcname, mony);
	}
}

//prints the player's monies and inventory items MARK: print inventory
void printInventory(vector<Item*>* inventory, int monies) {
	cout << "\nYou have " << monies << " monies, and you"; //prints mony amount
	if (inventory->size() < 1) { //if we have no items, we say that
		cout << " have no items!";
		return;
	} //finishes the message
	cout << "r items are:";
	for (Item* item : *inventory) { //prints the names of each item
		cout << "\n" << item->getName();
	}
}

//prints the player's party MARK: print party
void printParty(vector<NPC*>* party) {
	cout << "\nMembers of your party:";
	for (NPC* npc : *party) { //prints everyone's title if they have one, and then their name and level
		cout << "\n" << npc->getTitle();
		if (strlen(npc->getTitle()) > 0) { //puts a space between the title and name if they have a title
			cout << " ";
		}
		cout << npc->getName() << " - LEVEL " << npc->getLevel();
	}
}

//analyzes either an item or npc of the given name MARK: analyze
void analyze(Room* currentRoom, const char* name, vector<NPC*>* party, vector<Item*>* inventory) {
	NPC* npc = getNPCInVector(currentRoom->getNpcs(), name); //tries to find an npc in the room or party
	if (npc == NULL) { //try to find npc in adjacent exits
		npc = getNPCInVector(currentRoom->getNpcs(true), name);
	}
	if (npc && npc->getQuantumn()) { //we just defeat quantumn enemies and ignore that they were ever there
		npc->defeat();
		npc = NULL;
	}
	if (npc != NULL) { //prints the data of the npc that was found
		if (npc->getScaleFight()) npc->setLevel((*party)[0]->getLevel()); //if the npc scales to the player's level make sure it's that level before printing the stats
		printNPCData(npc);
		return;
	} //if no npc was found, we try to find an item in the current room
	Item* item = getItemInVector(currentRoom->getItems(), name);
	if (item == NULL) { //if there was no item there, we look for the item in our inventory
		item = getItemInVector(*inventory, name);
	} //the item may also be for sale so we check that too
	if (item == NULL) {
		item = getItemInVector(currentRoom->getStock(), name);
	} //prints the data of the item if one was found
	if (item != NULL) {
		printItemData(item);
		return;
	} //error message for not specifying what to analyze
	if (!strcmp(name, "")) cout << "\nAnalyze what?";
	//error message for invalid name
	else cout << "\nThere is no item or person named \"" << name << "\" here.";
}

//buys an item from the current room's catalogue //MARK: buy
void buy(Room* currentRoom, vector<Item*>* inventory, const char* name, int& mony) {
	Item* item = getItemInVector(currentRoom->getStock(), name); //finds the item in the current room's stock
	if (item == NULL) { //gives error message based on other conditions
		if (!strcmp(npcname, "")) { //if the player didn't give a name for what to buy
			cout << "\nBuy what?";
		} else if (getItemInVector(currentRoom->getItems(), name) != NULL) { //if the item isn't for sale and it's just on the ground or something
			cout << "\nNobody is selling the " << name << "; you can just take it.";
		} else if (getItemInVector(*inventory, name) != NULL) { //if you're trying to buy your own item which you already own and are holding
			cout << "\nYou already own this item!";
		} else { //error message if no item found of the given name
			cout << "\nThere is no \"" << name << "\" here.";
		}
		return;
	} //buys the item, subtracts the cost from the player's mony, and adds it to the inventory
	item->buy(mony, inventory);
	if (item->getStock() <= 0) { //if we bought all of the item, we delete it from the store
		currentRoom->removeStock(item);
	}
}

//prints all the available commands MARK: help
void printHelp(const char** validCommands, const char** flavorText, size_t commandAmount, size_t flavorAmount) {
	cout << "\n"; //prints a random flavor text if we passed any
	if (flavorText) cout << flavorText[rand() % flavorAmount];
	cout << "\nCommands:"; //prints all the valid commands
	for (int i = 0; i < commandAmount; i++) {
		cout << "\n" << validCommands[i];
	}
}

//save the game so we can store it as a file, and return if we want to keep going after saving MARK: save world
bool saveWorld(Save*& save, vector<Item*>* inventory, int monies, time_t& savetime, const char* andwhat) {
	cout << "\nSaving..."; //print loading because it looks nice in case there's some lag

	//MARK: save the game

	savetime = time(NULL); //update the time we last saved because we just saved

	if (strcmp(andwhat, "AND QUIT")) {
		cout << "\rSuccessfully saved your progress!"; //success message! unless we were also quitting
		if (strcmp(andwhat, "")) { //if we typed something in addition to SAVE but it wasn't AND QUIT
			CinPause();
			cout << "\nYou seem to have also typed something uninterpretable after SAVE.\nAre you trying to SAVE AND QUIT? (YES or NO)"; //in case there was a typo or something, idk why else you would type something after SAVE
			if (!AOrB(NULL, "YES", "NO")) {
				cout << "\nAlrighty then."; //carry on message
				return true;
			} //the YES path continues down to that return false down there
		}
	}
	cout << "\rLoading..."; //it's loading the loading screen (and deleting the world since we're leaving the game in this path)
	return false;
}

//make sure the player is really sure about quitting without saving, and return if we are continuing MARK: confirm quit
bool confirmQuit(time_t savetime) {
	long long seconds = difftime(time(NULL), savetime);
	long long minutes = seconds/60; //use minutes if possible, precise reports of game time
	seconds %= 60; //get rid of the extra seconds
	long long hours = minutes/60; //go further to hours if possible
	minutes %= 60; //get rid of the extra minutes

	//make the player understand what they're doing so they don't accidentally do something dumb
	cout << "\nYou will lose ";
	if (hours) cout << hours << "h ";
	if (minutes || hours) cout << minutes << "m "; //print minutes even if 0 if hours did print for better looking times
	cout << seconds << "s of progress!\nAre you sure you want to quit without saving? (YES or NO)";

	if (!AOrB(NULL, "YES", "NO")) { //get the player choice
		cout << "\nAlrighty then."; //carry on message
		return true;
	}
	cout << "\rLoading..."; //it's loading the loading screen (and deleting the world since we're leaving the game in this path)
	return false;
}

//the main game function for exploring the world MARK: play
void play(Save*& save) {
	vector<Item*> inventory; //the inventory of items
		
	//sets up the game world and places the player at the current room
	NPC* self = SetupWorld(&inventory);

	double playtime = 0; //how long the player has played on this save
	
	vector<NPC*>* party = self->getParty(); //a pointer to the player's party

	int mony = 0; //monies are the currency in the BURGER QUEST universe.

	//flavor text printed by printHelp()
	const char* flavorText[16] = {
		"What even is a BURGER?",
		"You consider the state of the economy.",
		"You are blue dabadeedabadai.",
		"You are beginning to believe.",
		"You forgor.",
		"You accidentally find spoilers for the ending of BURGER QUEST 2. It is very concerning...",
		"You realize you don't have an oven.",
		"You spot a billboard advertising the BURGER RESTAURANT in BURGERSBURG.",
		"You ask a passerby what the valid commands are. The guy looks at you really confused.",
		"You stop it and get some help.",
		"We have been trying to reach you about your car's extended warranty.",
		"You spot a quick brown fox jumping over a lazy dog.",
		"You say hello to the world.",
		"I could really go for a burger right now. Too bad they only sell BURGERs here.",
		"You have a cake and eat it too.",
		"You take a potato chip... and eat it."
	};

	//a list of the valid commands (and extensions) to be printed by printHelp()
	const char* validCommands[17] = {
		"GO [direction]",
		"TAKE [item]",
		"DROP [item]",
		"USE [item] (ON [npc])",
		"RECRUIT [npc]",
		"DISMISS [npc]",
		"ASK [npc]",
		"INVENTORY",
		"PARTY",
		"ATTACKS",
		"ROOM",
		"ANALYZE [npc/item]",
		"FIGHT [npc]",
		"BUY [item]",
		"HELP",
		"SAVE (AND QUIT)",
		"QUIT"
	};

	if (!save) { //if no existing save was provided, do the new game process, starting with the welcome message!
		cout << "Welcome to BURGER QUEST 2: ELECTRIC BOOGALOO!\nYou're going on a quest to get a BURGER (not to be confused with a burger).\nType HELP for help.\n";

		//you get to name yourself!
		cout << "\n             (type your name here!)\nYour name is ";

		//gets the player's input and puts it into the player name
		char name[255];
		cin.getline(name, 255);

		AllCaps(&name[0]); //capitalizes the name because everything is capitalized (no it isn't)

		if (!strcmp(name, "")) { //the main character complains if you didn't name him
			cout << "\nSELF - \"Ok I guess I just don't have a name then.\"";
			CinPause();
		} else { //otherwise we set the name to whatever the player inputted
			self->setName(name);
		} 
		if (!strcmp(name, "BERNARD")) { //Bernard is the character's canonical name
			cout << "\nBERNARD - \"Oh wow that's my actual name!\"";
			WorldState[ISBERNARD] = true;
			CinPause();
		} else if (!strcmp(name, "HELP")) { //Help complains if you followed the previous instructions and typed HELP (for help)
			cout << "\nHELP - \"BRO are you serious? Now my name is Help... :(\"";
			CinPause();
		//if the player typed anything by following the instructions too literally, the main character remarks on that
		} else if (!strcmp(name, "YOUR NAME HERE!)")) {
			cout << "\n" << name << " - \"You don't include the parenthesis cause it's closing the one from before...\"";
			CinPause();
		} else if (!strncmp(name, "HELP FOR HELP", 13) || !strncmp(name, "YOUR NAME", 9)) {
			cout << "\n" << name << " - \"Well, you're very good at following instructions...\"";
			CinPause();
		}

		CinIgnoreAll(); //clears extra characters or invalid input
	} else { //welcome the player back if they are using existing save data
		cout << "\nWelcome back!";
		CinPause();
	}

	PrintRoomData(self->getRoom()); //prints the data of the starting room

	time_t savetime = time(NULL); //the last time this session was saved, to get total save file time and how much progress will be lost when quitting without saving

	bool promptline = true; //if the prompting > should be in a newline, only false after typing nothing
	bool continuing = true; //we continue until this is set to false (when the player quits or gets an ending)
	while (continuing) { //the main loop!
		char command[255] = ""; //the charray that the player inputs into

		char commandWord[255]; //the first word of the player input (the command)
		char commandExtension[255]; //the rest of the player's command (minus the space)

		if (promptline) cout << "\n";
		cout << "> "; //The > signifies it's time to type in a command. If there is no >, it's a cutscene or dialogue or something like that and you just have to ENTER until you get to the >.
		cin.getline(command, 255);
		AllCaps(command); //capitalizes the command for easier parsing

		promptline = true; //make sure the next > will probably be in a new line

		ParseCommand(command, commandWord, commandExtension); //seperates the command into the command and the extension

		Room* currentRoom = self->getRoom(); //sets the current room to the player's position

		if (currentRoom->getGym()) { //if we're in a gym, update all the traning teammates' levels before doing the action
			currentRoom->scaleNPCs(self->getLevel()-1);
		}

		if (!strcmp(commandWord, "GO")) { //for going in a direction
			travel(currentRoom, &commandExtension[0], party, &inventory);
		} else if (!strcmp(commandWord, "TAKE")) { //for taking an item
			takeItem(currentRoom, &inventory, &commandExtension[0], self);
		} else if (!strcmp(commandWord, "DROP")) { //for dropping an item
			dropItem(currentRoom, &inventory, &commandExtension[0], self);
		} else if (!strcmp(commandWord, "USE")) { //for using an item (or tunnel lobster)
			useItem(currentRoom, &inventory, party, &commandExtension[0], mony);
		} else if (!strcmp(commandWord, "RECRUIT")) { //for recruiting npcs
			recruitNPC(currentRoom, &commandExtension[0], party);
		} else if (!strcmp(commandWord, "DISMISS")) { //for dismissing npcs
			dismissNPC(currentRoom, &commandExtension[0], party);
		} else if (!strcmp(commandWord, "ASK")) { //for getting the dialogue of npcs
			printNPCDialogue(currentRoom, &commandExtension[0], &inventory, party, mony);
		} else if (!strcmp(commandWord, "INVENTORY")) { //for printing the contents of your inventory and your monies
			printInventory(&inventory, mony);
		} else if (!strcmp(commandWord, "PARTY")) { //for printing everyone in your party and their level
			printParty(party);
		} else if (!strcmp(commandWord, "ATTACKS")) { //for printing the player's attacks
			printAttacks(self);
		} else if (!strcmp(commandWord, "ROOM")) { //for reprinting the contents of the current room (it was annoying having to scroll back up after doing a bunch of stuff in order to check the room data)
			PrintRoomData(currentRoom, self);
		} else if (!strcmp(commandWord, "ANALYZE")) { //for getting the data of an item or npc
			analyze(currentRoom, &commandExtension[0], party, &inventory);
		} else if (!strcmp(commandWord, "FIGHT")) { //for initiating battle with npcs
			fight(currentRoom, party, &inventory, &commandExtension[0], mony);
		} else if (!strcmp(commandWord, "BUY")) { //for buying items for sale
			buy(currentRoom, &inventory, &commandExtension[0], mony);
		} else if (!strcmp(commandWord, "HELP")) { //for getting a list of valid commands
			printHelp(validCommands, flavorText, 17, 16);
		} else if (!strcmp(commandWord, "SAVE")) { //for saving and also possibly quitting
			continuing = saveWorld(save, &inventory, mony, savetime, &commandExtension[0]);
		} else if (!strcmp(commandWord, "QUIT")) { //for quitting the game without saving
			continuing = confirmQuit(savetime);
		} else if (!strlen(command)) { //don't print error if the player just entered nothing
			promptline = false;
		} else { //prints an error message if the player typed something that isn't an actual command
			cout << "\nInvalid command \"" << commandWord << "\" (type HELP for help).";
		}

		CinIgnoreAll(); //clears extra or faulty input

		if (WorldState[GAMEEND]) continuing = false; //if we got an ending, we quit the game
	}
	
	//deletes all the rooms
	for (Room* room : roomsH) {
		delete room;
	} //deletes all the npcs
	while (!npcsH.empty()) { //these ones call erase remove in their destructor so we do this while loop instead
		delete npcsH.back();
	} //deletes all the items
	while (!itemsH.empty()) {
		delete itemsH.back();
	} //deletes all the attacks
	for (Attack* attack : attacksH) {
		delete attack;
	} //deletes all the effects
	for (Effect* effect : effectsH) {
		delete effect;
	}
	relaysH.clear(); //get rid of the shared pointers
	npcID = 0; //we just deleted all the npcs so reset the npc ID
}

//print a list of all the player's saves MARK: print saves
void printSaves(vector<Save*>& saves) {
	//for each save, print their number, the player level, their monies, and total game time
	//example: SAVE 1 - SELF, Lvl 15, 12980 Monies, 5h 56m 7s
	//error text: SAVE 8 - Save data is faulty.
	//we still load faulty saves (from the file system) to preserve save number
	for (size_t i = 0; i < saves.size(); i++) {
		Save* save = saves[i];
		cout << "\nSAVE " << i << " - ";
	}
}

//make a new game and manage the NULL member of the saves vector if the player saved it MARK: new game
void newGame(vector<Save*>& saves) {
	saves.push_back(NULL); //push a null save so we can put the Save* there if we make one
	Save*& savey = saves.back(); //get a reference to the NULL thingy
	play(savey); //play the game with the reference to the NULL save
	if (!savey) saves.pop_back(); //if we didn't save before quitting, we get rid of the NULL thingy
}

//load an existing save MARK: load save
void loadSave() {

}

//delete one of the saves and rearrange the files accordingly MARK: delete save
void deleteSave() {
	
}

//import save data and try to make a save file for it MARK: import save
void importSave() {
	int i = 2398; //MARK: make this actually be determined
	cout << "\nSuccessfully imported your save data into SAVE " << i << "!";
}

//export save data in a copy-pastable way MARK: export save
void exportSave() {
	cout << "\nHere is your save data!\n\n";

	//MARK: print save data

	cout << "\n\nMake sure to copy everything between the BQ2 and = (including the BQ2 and =)."
			"\nYou're probably in a terminal, so prefer Ctrl + Insert or Ctrl + Shift + C over Ctrl + C."
			"\nIMPORT this save data whenever you want to use it!"
			"\nI am not responsible for issues with your game if you manually edit your save file."
			"\nStore it in a safe location!";
}

//loads all the saves the player has in their files MARK: load saves
//this only works if all the files are in the format saveN.bq2 and there are no gaps, which should be the case unless the player was messing around with the files, which isn't my fault
void loadSaves(vector<Save*>& saves, bool reload = false) {
	for (Save* save : saves) delete save; //delete all the old saves
	saves.clear(); //reset the vector in case we were reloading
	char filename[255]; //we use this to get the current name of the file
	//load all the saves in the order of the number in their name
	for (size_t i = 0;; i++) {
		snprintf(filename, 255, "save%d.bq2", i); //get what the name of the current file should be
		ifstream savefile(filename); //try to get the file
		if (!savefile) break; //stop getting files if no such file was found
		streamsize filelen = savefile.tellg(); //get the length of the save file data
		char* savedata = new char[filelen+1]; //allocate a charray to write the data into
		savefile.read(savedata, filelen); //write the save file data into the charray so the program can use it
		savedata[filelen] = '\0'; //null terminate the data
		saves.push_back(new Save(savedata)); //make a new save file with the new data
		delete[] savedata;
	}
	if (reload) { //tells the player what just happened and how many files were found during the reloading
		if (saves.size()) cout << "\nSaves successfully reloaded! Found " << saves.size() << " saves.";
		else cout << "\nNo save files were found."; //lets the player know there were no save files found
	}
}

//the title screen! MARK: main (title screen)
int main() {
	srand(time(NULL)); //seeds random

	//a list of the valid commands (and extensions) to be printed by printHelp()
	const char* validCommands[8] = {
		"SAVES",
		"NEW GAME",
		"LOAD [save file]",
		"DELETE [save file]"
		"IMPORT [save data]",
		"EXPORT [save file]",
		"RELOAD",
		"HELP",
		"EXIT"
	};

	cout << "\nBURGER QUEST 2:"
			"\nELECTRIC BOOGALOO"
			"\n"
			"\nVersion Alpha 0.8"
			"\n(c) 2026 Tomas Carranza Echaniz"
			"\n"
			"\nPress ENTER to begin." << fixed << setprecision(2);

	CinPause();

	vector<Save*> saves; //all the player's saves
	loadSaves(saves); //get all the player's saves

	printHelp(validCommands, NULL, 7, 0); //prints what to do right off the bat
	cout << "\nWhat would you like to do?"; //beginning prompt
	
	bool promptline = true;
	bool continuing = true; //we continue until the player quits
	while (continuing) { //the main loop!
		char command[255] = ""; //the charray that the player inputs into

		char commandWord[255]; //the first word of the player input (the command)
		char commandExtension[255]; //the rest of the player's command (minus the space)

		if (promptline) cout << "\n";
		cout << "> "; //The > signifies it's time to type in a command. If there is no >, it's a cutscene or dialogue or something like that and you just have to ENTER until you get to the >.
		cin.getline(command, 255);
		AllCaps(command); //capitalizes the command for easier parsing

		promptline = true; //make sure the next > will probably be in a new line

		ParseCommand(command, commandWord, commandExtension); //seperates the command into the command and the extension

		if (!strcmp(commandWord, "SAVES")) { //for printing out all the save files
			
		} else if (!strcmp(command, "NEW GAME")) { //for starting a new save file
			
		} else if (!strcmp(commandWord, "LOAD")) { //for loading an existing save file
			
		} else if (!strcmp(commandWord, "DELETE")) { //for deleting a file
			
		} else if (!strcmp(commandWord, "IMPORT")) { //for importing save data from elsewhere (this and export are in case I ever put this on a website or something where you can't access the files easily)
			
		} else if (!strcmp(commandWord, "EXPORT")) { //for exporting save data in a copy-pastable way
			
		} else if (!strcmp(commandWord, "RELOAD")) { //for reloading your save files to match the os files
			loadSaves(saves, true);
		} else if (!strcmp(commandWord, "HELP")) { //for asking what the valid commands are
			printHelp(validCommands, NULL, 9, 0);
		} else if (!strcmp(commandWord, "EXIT")) { //for exiting the game
			continuing = false;
		} else if (!strlen(command)) { //don't print error if the player just entered nothing
			promptline = false;
		} else { //prints an error message if the player typed something that isn't an actual command
			cout << "\nInvalid command \"" << commandWord << "\" (type HELP for help).";
		}

		CinIgnoreAll(); //clears extra or faulty input
	}

	//gives a friendly farewell to the player
	cout << "\nCya!\n";

	//delete all the saves because they are allocated data and their contents are saves as files anyway
	for (Save* save : saves) {
		delete save;
	}
}
