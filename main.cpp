/* Tomas Carranza Echaniz
*  12/5/25
*  This program is an epic text-based RPG where you can GO between rooms, TAKE, DROP, and USE items, ASK, RECRUIT,
*  DISMISS, and FIGHT npcs, and you're on a QUEST TO get a BURGER. There's a bunch of other commands as well.
*  
*  In case you missed the first BURGER QUEST, you can play it at https://codehs.com/sandbox/id/zuul-kb8vTu
*/
#include <iostream>
#include <iomanip>
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
#include "Conversation.h"
#include "Stats.h"

using namespace std;
using namespace Helper; //my Helper namespace has a bunch of helpful functions that I also use in other files

//sets up the entire game world, including rooms, npcs, and items, and returns the player character
NPC* SetupWorld() {
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
	const char* INSIDE = "INSIDE";
	const char* OUTSIDE = "OUTSIDE";
	const char* UPSTAIRS = "UPSTAIRS";
	const char* DOWNSTAIRS = "DOWNSTAIRS";
	const char* UNDERGROUND = "UNDERGROUND";
	const char* ABOVEGROUND = "ABOVEGROUND";
	const char* FORWARD = "FORWARD";
	//fast travel directions
	const char* TO_THE_VILLAGE = "TO THE VILLAGE";
	const char* TO_THE_DESERT = "TO THE DESERT";
	const char* TO_THE_HIGHLANDS = "TO THE HIGHLANDS";
	const char* TO_BURGERSBURG = "TO BURGERSBURG";
	const char* TO_THE_BASEMENT = "TO THE BASEMENT";

	//set the direction reverser map in Helper
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

	//set up blockage reaons
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

	//I send all the template enemy NPCs and also shop items to limbo, since I need to set a room for them MARK: make rooms (WW)
	Room* limbo = new Room("not supposed to be in this room; seriously how did you get here?");

	//create all WANING WOODLANDS rooms
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
	Room* ninjacapitol = new Room("in the chief ninja's abode. There are many weapons and scrolls up on the walls.");
	Room* ninjapantry = new Room("in the ninja storage unit. The ninjas live on a strict diet of ninjaberries and ninjasteak and ninjafish and the diet isn't actually that strict.");
	Room* ninjaforge = new Room("in the ninja forge. There are many molds for making weapons here.");
	Room* foresttempleentrance = new Room("in the sunny glade, at the sealed entrance of the ancient forest temple.");
	Room* foresttemplestairs = new Room("on the steps that go into the ancient forest temple.");
	Room* foresttemple = new Room("in the forest temple.");
	//temple stuff
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
	Room* deserttemplestairs = new Room("on the steps that go into the ancient desert temple.");
	Room* deserttemple = new Room("in the desert temple.");
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
	Room* volcanotempleentrance = new Room("in a massive cavern, at the door of an ancient volcanic temple. The rock burns bright red.");
	Room* volcanotemplestairs = new Room("on the steps that go into the ancient volcanic temple.");
	Room* volcanotemple = new Room("in the temple of PATIENCE.");
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
	Room* newstreet2 = new Room("in an alleyway. You see a THIEF rooting through the dumpster. Is there really nothing better to steal?");
	Room* newstreet3 = new Room("at the entrance to a convenience store. The door is intact! And all the windows are broken.");
	Room* newstreet4 = new Room("at the city orphanage, scheduled for demolition.");
	Room* newstreet5 = new Room("at the edge of the regular district. The buildings beyond look really well-maintained.");
	Room* mainstreet1 = new Room("on main street. There's a few cars above lodged in the side of the buildings.");
	Room* mainstreet2 = new Room("on main street. A lopsided helicopter struggles to spin its rotors.");
	Room* mainstreet3 = new Room("on main street. The traffic lights are all broken, but the random fires provide cozy lighting.");
	Room* mainstreet4 = new Room("on main street. There's a stairway here leading down to the BURGERSBURG train station.");
	Room* mainstreet5 = new Room("at the end of main street. The BURGER RESTAURANT looms ahead of you, welcoming you into the elevator.");
	Room* coolstreet1 = new Room("near the entrance of the city. The top of a building is sliced clean off.");
	Room* coolstreet2 = new Room("at the entrance to an apartment building. This one's door is open.");
	Room* coolstreet3 = new Room("at a functional hot dog stand.");
	Room* coolstreet4 = new Room("at the entrance to a dark alley. Eh could be darker.");
	Room* coolstreet5 = new Room("next to some copy-pasted looking townhouses. Surely homebuilders can be more creative?");
	Room* rightstreet1 = new Room("in the crumbling corner of the city. The lava sea radiates light onto exposed infrastructure.");
	Room* rightstreet2 = new Room("still in the city. You hear a random explosion, business as usual.");
	Room* rightstreet3 = new Room("at a really nice brick building. You see a guy in a black rat costume fighting an old lady."); //make sure this gets renamed after beating Ratman
	Room* rightstreet4 = new Room("at a small glass store. You see two guys carrying a large pane of glass."); //add group of people npc for joke
	Room* rightstreet5 = new Room("at the entrance to the BURGERSBURG fire department. It probably hasn't seen much use recently.");
	Room* richneighborhood1 = new Room("in the rich people corner of town. Each huge building corresponds to just one person.");
	Room* richneighborhood2 = new Room("in the rich neighborhood. The security systems put you on guard; as soon as you step on a lawn, you'd probably be blown to smithereens.");
	Room* richneighborhood3 = new Room("at a secluded corner of the rich neighborhood. Some guy's standing outside. What a daredevil.");
	Room* richneighborhood4 = new Room("at the doorway of a huge layered complex, the BURGER corporation's headquarters.");
	//city interiors
	Room* burgfish = new Room("in the fish building. A warm light shines down the stairs.");
	Room* burgchurch = new Room("in a hidden church. Everyone seems joyful despite the state of the city.");
	Room* burgstore = new Room("in an abandoned store. Nobody bothers to stock the shelves anymore.");
	Room* casino = new Room("in the casino. Sounds of slot machines and flashing lights overload your senses.\nYou should really leave before you develop a gambling addiction.");
	Room* darkalley = new Room("in a dark alley, a characteristic of those about to be mugged.");
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
	Room* ceoroom = new Room("in the BURGER CEO's office. The desk stands in front of the BURGER SAFE, where all the company valuables are held.");
	Room* burgsafe = new Room("in the BURGER SAFE. Countless monies and company documents are piled up here.");

	Room* elevator = new Room("in the elevator of the BURGER RESTAURANT. It's a really fancy circular elevator, with a 360 degree view of the city.");
	Room* elevatortop = new Room("in the elevator, elevated all the way to the tippity top.");
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
	BURGERRESTAURANT->setWelcome({{NULL, "BURGER MAN - \"HELLO VALUED CUSTOMER!\""}, {NULL, "BURGER MAN - \"WELCOME TO MY BURGER RESTAURANT!\""}});
	Room* elevatorbottom = new Room("in the elevator, deep down in the restricted level of the BURGER RESTAURANT.");
	elevatorbottom->setWelcome({{NULL, "The elevator shoots downwards."},
								{NULL, "..."},
								{NULL, "..."},
								{NULL, "..."},
								{NULL, "Hard rock speeds by."},
								{NULL, "..."},
								{NULL, "..."},
								{NULL, "..."},
								{NULL, "It's so far down; the temperature starts rising."},
								{NULL, "..."},
								{NULL, "..."},
								{NULL, "..."},
								{NULL, "The rock outside is glowing red hot..."},
								{NULL, "The elevator dings."}});
	Room* burgerbasement = new Room("in the BURGER BASEMENT. The walls are a bluish gray, and the lights emit a constant hum.");
	Room* BURGERPRISON = new Room("in the BURGER PRISON. There one singular damp cell. It smells like BURGERs.");
	Room* basestation = new Room("in a deep train tunnel near the BURGER PRISON. Where do trains need to go this deep?");
	basestation->setStation();

	Room* tunnels = new Room("in the train tunnels that span the continent. The acoustics here are great!");
	tunnels->setStation();

	//Create NPCs and items MARK: make npcs, items, etc.
	NPC* self = new NPC("\0", "SELF", "The protagonist of BURGER QUEST 2, with a cool scarf and blond anime hair.\nIt's a me.", limbo, 90, Stats(20, 5, 6, 0, 0, 10, 9), Stats(1, 0, 1, 0, 0, 1, 1), true, true);
	self->addRecruitedDialogue("Huh?");
	self->Recruit();
	self->addXp(3); //make it so the first enemy gives you just enough xp to level up

	Attack* punch = new Attack("PUNCH", "punched", true, -5, 10, 0, 1, 1, 1);
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
	punchflurry->addDescription("Unleash a barrage of 6 to 7 punches. (5 ATTACK PER HIT)");
	self->addSpecialAttack(punchflurry);
	Attack* shurikenthrow = new Attack("SHURIKEN THROW", "threw a spread of shurikens at", false, 2, 7, 5, 0, 2, 3);
	shurikenthrow->addDescription("Throw a spread of shurikens at the target, with varying success since you're just chucking them. (7 ATTACK, 5 PIERCE, 0-2 hits)");
	Item* shurikens = new WeaponItem("SHURIKENS", "A bundle of ninja shurikens with a note attached:\n\"Congratulations on defeating our ninja scout. Take these shurikens and train in the ninja ways,\nand maybe one day you'll become a true ninja.\"", ninjaland, shurikenthrow);
	Attack* bonedrill = new Attack("BONE CONE", "launched a drill of bone at", false, 10, 8, 5, 5, 6, 1);
	bonedrill->addDescription("Spin the conic bone, drilling into the target. (8 ATTACK, 5 PIERCE, 6 hits)");
	Item* bonecone = new WeaponItem("BONE CONE", "A cone-shaped bone, looks kind of like a drill.", desertgrave, bonedrill);
	Attack* scarfsmack = new Attack("SCARF SMACK", "smacked", false, 15, 25, 0, 2, 2, 1); //no contact because the scarf stretches far from Bernard
	scarfsmack->afterdesc = "with his scarf";
	scarfsmack->addDescription("Smack the target with each end of your stretchy scarf. (25 ATTACK, 2 hits)");
	Item* bigscarf = new WeaponItem("BIG SCARF", "A scarf considerably bigger and stretchier than your current one, good for doing scarf attacks.", limbo, scarfsmack);
	Item* factchest = new TreasureItem("TREASURE CHEST", "A big treasure chest, possibly full of treasure.", factorytreasure, 50, bigscarf);
	Attack* energize = new Attack("ENERGIZE", "energized", false, 14, 0, 0, 1, 1, 1, true, 15);
	Effect* energized = new Effect("ENERGIZED", 1, 0, 0, 2.0f, 0, 0, 0);
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
	Effect* onfire = new Effect("ON FIRE", 3, 5, 0, 1, 0.8f);
	pdeadlyspinferno->addEffect(onfire);
	pdeadlyspinferno->addDescription("Fly at the target and their surroundings in a deadly flaming tornado, also leaving them on fire. (10 ATTACK, 20 PIERCE, 5 hits)");
	Attack* precisionstrike = new Attack("PRECISION STRIKE", "threw a precise energy ellipsoid at", false, 12, 35, 15, 1, 1, 1, false, 12);
	precisionstrike->addDescription("Throw a heavy mass of energy speedily towards the target. (35 ATTACK, 15 PIERCE)");
	Attack* ballisticmissile = new Attack("BALLISTIC MISSILE", "threw a missile of energy at", false, 19, 50, 25, 1, 1, 1, false, 18);
	ballisticmissile->addDescription("Throw a dense missile of energy straight towards the target. (50 ATTACK, 25 PIERCE)");
	Attack* spbomb = new Attack("SP BOMB", "lobbed the SP BOMB at", false, 0, 0, 0, 1, 1, 9, false, 25);
	spbomb->addDescription("Gather up the collective SP of the entire team into a huge ball of energy and lob it at the enemy team. (SP ATTACK, 0 PIERCE)");
	spbomb->spbomb = true; //sp bomb do indeed be sp bomb

	//Flower Girl Floria is primarily a healer with some other stuff for variety as well MARK: Floria
	NPC* floria = new NPC("FLOWER GIRL", "FLORIA", "Your little sister who gets along well with nature, especially flowers.\nShe has a flower-shaped hat.", flowerfield2, 5, Stats(10, 5, 4, 0, 5, 5, 9), Stats(1, 0, 1, 0, 1, 0, 1));
	floria->addConversation({{floria, "Hey big brother! Aren't these flowers just so lovely? :>"},
							 {self, "NO THESE FLOWERS SUCK THEY TRIED TO EAT ME."},
							 {NULL, "FLORIA - :>"},
							 {NULL, "FLORIA - ¦>"},
							 {NULL, "FLORIA - :>"}});
	floria->setDialogue("I just love flowers!");
	floria->addGymDialogue({{floria, "I love running in circles around the gym!"}, {floria, "Exercise is so fun!"}});
	Conversation floriarecruit1 = {{self, "Hey I'm going on a BURGER QUEST wanna join?"}, {floria, "Yes! I hope we see some new flowers on the way!"}};
	floriarecruit1.skipcondition = TEMPLEQUEST;
	Conversation floriarecruit2 = {{self, "Hey I'm going on a QUEST to destroy BURGERs wanna join?"}, {floria, "Yes! I hope we see some new flowers on the way!"}};
	floriarecruit1.alt = &floriarecruit1;
	floriarecruit2.skipcondition = BURGERMENDEF;
	Conversation floriarecruit3 = {{self, "Hey wanna join my team?"}, {floria, "Yes! I hope we see some new flowers!"}};
	floriarecruit2.alt = &floriarecruit3;
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
	Attack* nitroheal = new Attack("NITROSYNTHESIS", "restored", false, 8, -2147483648, 20, 1, 1, 1, true, 12);
	nitroheal->afterdesc = " to peak health";
	nitroheal->addDescription("Use flower power to heal a teammate to peak health.");
	floria->addSpecialAttack(nitroheal);
	Attack* hypercapacitate = new Attack("HYPERCAPACITATE", "used flower power to recapacitate", false, 25, -2147483648, 20, 1, 1, 1, true, 15);
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
	NPC* egadwick = new NPC("SCIENCE GRAMPS", "EGADWICK", "Your grandpa who lives in a secluded corner of the village.\nHe's always advancing science to the dismay of high school chemistry students.", tentlab, 5, Stats(15, 2, 3, 10, 10, 2, 9), Stats(0, 0, 1, 1, 1, 0, 1));
	egadwick->setDialogue({{egadwick, "Ah hello kiddo. How's it going?"}, {self, "Pretty good."}, {egadwick, "Ah, that's good to hear."}});
	egadwick->addGymDialogue({{egadwick, "Eh, exercise isn't really my thing."}, {egadwick, "I gain experience by working out my mind!"}});
	Conversation egadwreject1 = {{self, "Yo Gramps wanna join my BURGER QUEST?"},
								{egadwick, "Sorry, kiddo, I'm afraid there's a dangerous robot on the loose in the forest."},
								{egadwick, "It was supposed to be for gardening but its definition of \"plant\" is a bit broad..."},
								{egadwick, "So now it's trying to cut my hair."},
								{self, "I can go destroy it if you want."},
								{egadwick, "Yes! That would be great, thanks!"}};
	egadwreject1.skipcondition = TEMPLEQUEST;
	Conversation egadwreject2 = {{self, "Yo Gramps wanna help me destroy BURGERs?"},
								{egadwick, "Sorry, kiddo, I'm afraid there's a dangerous robot on the loose in the forest."},
								{egadwick, "It was supposed to be for gardening but its definition of \"plant\" is a bit broad..."},
								{egadwick, "So now it's trying to cut my hair."},
								{self, "I can go destroy it if you want."},
								{egadwick, "Yes! That would be great, thanks!"}};
	egadwreject1.alt = &egadwreject1;
	egadwreject2.skipcondition = BURGERMENDEF;
	Conversation egadwreject3 = {{self, "Yo Gramps wanna join my team?"},
								{egadwick, "Sorry, kiddo, I'm afraid there's a dangerous robot on the loose in the forest."},
								{egadwick, "It was supposed to be for gardening but its definition of \"plant\" is a bit broad..."},
								{egadwick, "So now it's trying to cut my hair."},
								{self, "I can go destroy it if you want."},
								{egadwick, "Yes! That would be great, thanks!"}};
	egadwreject2.alt = &egadwreject3;
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
	Effect* overclocked = new Effect("OVERCLOCKED", 3, 0, 0, 1.5f, 1, 1, 1, 1.5f);
	overclock->addEffect(overclocked);
	egadwick->addSpecialAttack(overclock);
	Attack* shieldacid = new Attack("SHIELD ACID", "threw shield-melting acid at", false, 4, 0, 0, 0, 0, 0, false, 8);
	shieldacid->addDescription("Throw a beaker of shield-melting acid at the target.");
	Effect* acidified = new Effect("ACIDIFIED", 3, 10, 0, 1, 0.5f, 0.5f);
	shieldacid->addEffect(acidified);
	egadwick->addSpecialAttack(shieldacid);
	Attack* emp = new Attack("EMP", "threw an EMP at", false, 16, 20, 20, 1, 1, 3, false, 10, 20);
	emp->addDescription("Throw an EMP, frying some of the target's and surrounding enemies' SP.");
	egadwick->addSpecialAttack(emp);
	Attack* hyperclock = new Attack("HYPERCLOCK", "hyperclocked", false, 13, 0, 0, 0, 0, 0, true, 12);
	hyperclock->addDescription("Over-overclock a teammate, greatly boosting their attack and speed.");
	Effect* hyperclocked = new Effect("HYPERCLOCKED", 3, 0, 0, 2.0f, 1, 1, 1, 2.0f);
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
	Effect* uberclocked = new Effect("UBERCLOCKED", 3, 0, 0, 4.0f, 1, 1, 1, 4.0f);
	uberclock->addEffect(uberclocked);
	egadwick->addSpecialAttack(uberclock);
	Attack* orbitalstrike = new Attack("ORBITAL STRIKE", "called down an orbital beam towards", false, 30, 70, 100, 1, 1, 7, false, 25);
	orbitalstrike->addDescription("Call down an orbital laser from Edgadwick's brand new satellite.");
	egadwick->addSpecialAttack(orbitalstrike);
	
	//Forest Knight Absolom is primarily a tank with some knightly support as well MARK: Absolom
	NPC* forestknight = new NPC("FOREST KNIGHT", "ABSOLOM", "An old knight decked out in wooden armor, on a quest to vanquish all evil that crosses his path.", forestgrave, 30, Stats(30, 20, 25, 30, 10, 0, 10), Stats(1, 2, 1, 1, 0, 0, 1));
	forestknight->addRejectionDialogue({{self, "Hey knight man wanna join me on my BURGER QUEST?"},
										{forestknight, "A BURGER, you say?"},
										{forestknight, "I shan't assist you; this is an object of sin."},
										{forestknight, "I implore you to find a new, more noble goal for your quest."}});
	forestknight->addConversation({{forestknight, "For years, that fiend has kept me trapped here with the graves of my fallen compatriots."},
								   {forestknight, "No matter what I tried, he specifically countered my every technique."},
								   {forestknight, "Nevertheless, I must thank you, child, for freeing me from that shrimp's grasp."},
								   {self, "Yeah no problem."}});
	Conversation absrecruit1 = {{self, "Hey knight man wanna help me save this kid?"},
							    {forestknight, "Why of course!"},
							    {forestknight, "From what will we be saving them?"},
							    {self, "From the BURGER coropration, they kidnapped them."},
							    {forestknight, "Of course, the fiendish stewards of BURGER."},
							    {forestknight, "I shall assist you in saving this child!"}};
	absrecruit1.skipcondition = TEMPLEQUEST;
	Conversation absrecruit2 = {{self, "Hey knight man wanna help me annihilate BURGERs from existence?"},
							    {forestknight, "Ah, what a splendid goal for a quest!"},
							    {forestknight, "But how do you propose to accomplish this?"},
							    {self, "With this thing I learned about called THE PLOT DEVICE."},
							    {forestknight, "THE PLOT DEVICE? I cannot say I have heard of it,"},
							    {forestknight, "but I believe you when you say it can solve the BURGER problem."},
							    {forestknight, "I shall join you on this quest!"}};
	absrecruit2.skipcondition = BURGERMENDEF;
	Conversation absrecruit3 = {{self, "Hey knight man wanna join me?"},
							    {forestknight, "I do not see why not."}};
	absrecruit1.alt = &absrecruit2;
	absrecruit2.alt = &absrecruit3;
	forestknight->addRecruitmentDialogue(absrecruit1);
	forestknight->addDismissalDialogue({{forestknight, "I shall return to defending the forest."},
										{forestknight, "Farewell, friend!"}});
	
	Attack* forestslash = new Attack("FOREST SLASH", "slashed", true, -5, 15, 10, 1, 1, 1);
	forestslash->afterdesc = " with his forest sword";
	forestknight->setBasicAttack(forestslash);
	Attack* defend = new Attack("PROTECT", "is protecting", false, 10, 0, 0, 1, 1, 3, true, 10);
	forestknight->addSpecialAttack(defend);
	defend->protect = true; //defend start protecting
	Attack* redwoodrend = new Attack("REDWOOD REND", "thrusted his sword at", true, 5, 25, 30, 1, 1, 3, false, 12);
	redwoodrend->afterdesc = " with the might of a redwood";
	forestknight->addSpecialAttack(redwoodrend);
	Attack* warcry = new Attack("WAR CRY", "rallied the team into action", false, 20, 0, 0, 0, 0, 7, true, 15);
	Effect* galvanized = new Effect("GALVANIZED", 5, 0, 0, 2.0f, 2.0f);
	warcry->addEffect(galvanized);
	forestknight->addSpecialAttack(warcry);
	Attack* enrootf = new Attack("ENROOT", "rooted into the soil", false, 10, 0, 0, 0, 0, 0, true, 18);
	Effect* rootedf = new Effect("ROOTED", 5, -10, 0, 0, 3.0f, 3.0f);
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
	NPC* mike = new NPC("MINER MANIAC", "MIKE", "Maniacal miner with a reckless mania for blowing things up.\nA frequent customer of the subterranean dynamite store.", kaboomroom, 6, Stats(22, 5, 20, 0, 20, 12, 9), Stats(0, 0, 1, 0, 1, 0, 1));
	mike->addConversation({{NULL, "MIKE is throwing dynamite at the rocky wall."},
						   {self, "That doesn't look very safe."},
						   {mike, "HAHAHAHA Aren't the blasts just music to your ears?"},
						   {self, "no it's very loud :|"}});
	mike->setDialogue("HAHAHAHA I love the smell of explosions in the morning!");
	Conversation mikerec1 = {{self, "Hey I'm going on a BURGER QUEST wanna join?"}, {mike, "Why not, kid! Blowing up rocks is getting old."}};
	mikerec1.skipcondition = TEMPLEQUEST;
	Conversation mikerec2 = {{self, "Hey I'm going on a QUEST to destroy BURGERs wanna join?"}, {mike, "HAHAHAHA! Let's go blow up some BURGERs, kid!"}};
	mikerec1.alt = &mikerec1;
	mikerec2.skipcondition = BURGERMENDEF;
	Conversation mikerec3 = {{self, "Hey wanna join my team?"}, {mike, "Why not, kid! Blowing up rocks is getting old."}};
	mikerec2.alt = &mikerec3;
	mike->addRecruitmentDialogue(mikerec1);
	mike->addRecruitedDialogue("HAHAHAHAHA! So many things to explode!");
	mike->addDismissalDialogue("Alright Imma head back to my cave!");
	mike->setTalkOnRecruit(true);
	mike->setRecruitable(true);

	Attack* mdynamite = new Attack("DUAL DYNAMITE", "threw dual sticks of dynamite", false, -5, 20, 20, 2, 2, 1);
	mdynamite->focushits = false;
	Attack* drecoil = new Attack("LOOSE DYNAMITE", "accidentally bounced a stick of dynamite towards", false, 0, 10, 20, 1, 1, 1);
	mdynamite->recoilatt = drecoil;
	mdynamite->recoilchance = 0.3333f;
	mike->setBasicAttack(mdynamite);
	Attack* flashbang = new Attack("FLASHBANG", "threw a flashbang at", false, 8, 10, 20, 1, 1, 1);
	Effect* stunned = new Effect("STUNNED", 3);
	stunned->freeze = true;
	flashbang->addEffect(stunned);
	Attack* frecoil = new Attack("SIDE EFFECT", "accidentally stunned", false, 0, 10, 20, 1, 1, 1);
	frecoil->afterdesc = " as well";
	frecoil->addEffect(stunned);
	flashbang->recoilatt = frecoil;
	flashbang->recoilchance = 0.3333f;
	mike->addSpecialAttack(flashbang);
	Attack* bigbundle = new Attack("BIG BUNDLE", "threw a big bundle of dynamite at", false, 13, 40, 20, 1, 1, 3);
	Attack* brecoil = new Attack("LOOSE DYNAMITE", "didn't tie the bundle tightly enough, making some dynamite fall close to", false, 0, 20, 20, 1, 1, 1);
	brecoil->afterdesc = " in the process";
	bigbundle->recoilatt = brecoil;
	bigbundle->recoilchance = 0.3333f;
	mike->addSpecialAttack(bigbundle);
	Attack* bunkerbuster = new Attack("BUNKER BUSTER", "aimed a bunker buster at", false, 11, 30, 100, 1, 1, 1, false, 8);
	Attack* bbrecoil = new Attack("MISAIM", "aimed it too close to", false, 0, 30, 100, 1, 1, 1);
	bunkerbuster->recoilatt = bbrecoil;
	bunkerbuster->recoilchance = 0.3333f;
	mike->addSpecialAttack(bunkerbuster);
	Attack* dedefenser = new Attack("DEDEFENSER", "threw a heavy charge at", false, 15, 10, 20, 1, 1, 1, false, 12);
	Effect* dedefensed = new Effect("DEDEFENSED", 10, 0, 0, 1, 0.5f, 0.25f, 1, 0.5f);
	dedefenser->addEffect(dedefensed);
	Attack* ddrecoil = new Attack("LOOSE DEDEFENSER", "accidentally dropped a dedefenser near", false, 0, 10, 20, 1, 1, 1);
	ddrecoil->addEffect(dedefensed);
	dedefenser->recoilatt = ddrecoil;
	dedefenser->recoilchance = 0.3333f;
	mike->addSpecialAttack(dedefenser);
	Attack* depthcharge = new Attack("DEPTH CHARGE", "threw a depth charge at", false, 20, 60, 35, 1, 1, 5, false, 17);
	Attack* dcrecoil = new Attack("LOOSE DYNAMITE", "accidentally included", false, 0, 30, 25, 1, 1, 1);
	dcrecoil->afterdesc = " in charge's radius";
	depthcharge->recoilatt = dcrecoil;
	depthcharge->recoilchance = 0.67f;
	mike->addSpecialAttack(depthcharge);
	Attack* minesweeper = new Attack("MINESWEEPER", "unleashed a mine-sweeping explosive upon the enemy team", false, 35, 10, 20, 12, 12, 1, false, 20);
	minesweeper->focushits = false;
	Attack* mrecoil = new Attack("MINESWEEPER RECOIL", "hit his team with the minesweeper as well", false, 0, 10, 20, 3, 3, 1);
	minesweeper->recoilatt = mrecoil;
	minesweeper->recoilchance = 1;
	mike->addSpecialAttack(minesweeper);

	//and make him say "Oops."

	//Cactus Cacty is a multi-hit damage dealer with some support/healing abilities MARK: Cacty
	NPC* cacty = new NPC("CACTUS", "CACTY", "Sharp cactus, brown from dehydration. He looks very sad, on the brink of death.", oasis, 12, Stats(25, 20, 23, 10, 15, 5, 9), Stats(1, 1, 1, 0, 1, 0, 1));
	cacty->setDialogue({{NULL, "CACTY - *raspy cactus plead for help*"}});
	cacty->addRejectionDialogue({{NULL, "CACTY - *raspy cactus plead for help*"}, {NULL, "CACTY is too dehydrated to join you."}});
	cacty->addRecruitmentDialogue({{self, "Hey cactus man wanna join me?"}, {NULL, "CACTY - *affirmative cactus noises*"}});

	Attack* loosespines = new Attack("LOOSE SPINES", "'s loose spines flew at the enemy team", false, 0, 8, 15, 1, 3, 1);
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
	Attack* cactcarpet = new Attack("CACTUS CARPET", "threw a carpet of cactus at", false, 8, 15, 15, 1, 1, 3, false, 14, 0, 0.34f);
	Effect* spinyfloor = new Effect("SPINY FLOOR", 4, 15);
	cactcarpet->addEffect(spinyfloor);
	cactcarpet->lifesteal = 0.34f;
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
	NPC* michelin = new NPC("MASTER CHEF", "MICHELIN", "Professional chef on a quest to discover new recipes.", factorykitchen, 10, Stats(22, 5, 20, 0, 20, 12, 9), Stats(0, 0, 1, 0, 1, 0, 1));
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
	michrec1.skipcondition = TEMPLEQUEST;
	Conversation michrec2 = {{self, "Well..."}, {self, "Wanna help me destroy BURGERs?"}, {michelin, "sure why not. :("}};
	michrec1.alt = &michrec2;
	michrec2.skipcondition = BURGERMENDEF;
	Conversation michrec3 = {{self, "Well..."}, {self, "Wanna join my team?"}, {michelin, "sure why not. :("}};
	michrec2.alt = &michrec3;
	michelin->addRecruitmentDialogue(michrec1);
	michelin->addRecruitedDialogue("You know, maybe there's more to life than finding recipies.");
	michelin->addDismissalDialogue({{michelin, "Well, I guess I'll go back to my little kitchen area."}, {michelin, "Gotta start packing my bags to head home."}});
	michelin->setRecruitDialogueChange({{michelin, "Hey what's up"}, {michelin, "Just packing my bags."}, {michelin, "Might take a while cause I've set up a whole freaking kitchen here."}});

	Item* cactiberry = new MaterialItem("CACTIBERRY", "Special berry that is pale green and prickly, adorned with a pink flower.", berryroom);
	Item* radiberry = new MaterialItem("RADIBERRY", "Special radioactive berry that glows neon green. Might wanna handle this one quickly.", factorygarden);
	Item* ninjaberry = new MaterialItem("NINJABERRY", "Special black berry pertaining to the ninjas, complete with a natural belt.", berryroom);

	Effect* multiposition = new Effect("MULTIPOSITION", 2147483647, 0, 0, 2.0f, 0.5f, 0.5f, 2.0f, 7.0f);
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
	Effect* marinated = new Effect("MARINATED", 5, 0, 0, 1, 0.5f);
	Attack* flambe = new Attack("FLAMBE'", "bonked", true, 7, 20, 5, 1, 1, 1);
	flambe->afterdesc = " with a flaming pan";
	Effect* flambed = new Effect("FLAMBE'D", 5, 5, 0, 1, 0.8f);
	flambe->addEffect(flambed);
	flambe->synergies.push_back(marinated);
	flambe->cancel = marinated;
	michelin->addSpecialAttack(flambe);
	Attack* hotsauce = new Attack("HOT SAUCE", "gave hot sauce to", false, 3, 0, 0, 1, 1, 1, true, 11);
	Effect* hotsauced = new Effect("HOT SAUCED", 5, 0, 0, 1.25f, 0.75f);
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
	NPC* carlos = new NPC("HACKERMAN", "CARLOS", "Black hat hacker with no hat and no vitamin D. He has a space invaders hoodie.", factorytower, 12, Stats(25, 20, 23, 10, 15, 5, 9), Stats(1, 1, 1, 0, 1, 0, 1));
	carlos->setDialogue({{carlos, "get out i cant focus with you here"}, {carlos, "im so close to hacking into microsofts mony supply"}});
	carlos->addRejectionDialogue({{carlos, "what"}, {carlos, "get outta my room im tryna focus"}});
	carlos->addRecruitmentDialogue({{self, "Hey wanna join my team?"}, {carlos, "no"}, {self, "Come on you gotta get some vitamin D."}, {carlos, "no i gotta start over stealing microsofts monies >:("}, {self, "If you want monies we get a lot just by fighting random enemies."}, {carlos, "..."}, {carlos, "fine ill join you"}});
	carlos->addRecruitedDialogue("...");
	carlos->addRecruitedDialogue({{carlos, "woah what is this stuff"}, {self, "That's grass."}});
	carlos->setRecruitDialogueChange("gaming");
	carlos->addDismissalDialogue({{carlos, "i dont feel like hacking microsoft all over again"}, {carlos, "guess ill just play fortnite or something"}});

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
	Effect* overflow = new Effect("OVERFLOW", 4, 0, 0, 1, 0.5f);
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
	ddos->addDescription("Direct all of Carlos's botnet's traffic, freezing them in place. (8 ATACK, 40 PIERCE, 3 hits)");
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
	NPC* plum = new NPC("PRINCESS", "PLUM", "Sporty princess in purple attire kidnapped from a distant fungal kingdom.", limbo, 18, Stats(13, 10, 8, 1, 5, 12, 9), Stats(0, 0, 1, 0, 1, 1, 9));
	plum->addDismissalDialogue({{plum, "Well, I'll be heading back to my kingdom!"}, {plum, "Bye bye!"}});
	plum->addRecruitedDialogue({{plum, "Ahh, sure beats a diet of pure fungus!"}});
	plum->addRecruitmentDialogue({{self, "Hey before you head back to your kingdom, wanna join my team?"}, {plum, "It would be my pleasure!"}});
	//some of plum's dialogue logic is tied to Browser below in the enemies section

	Attack* racket = new Attack("RACKET", "whacked", true, -5, 10, 0, 1, 1, 1);
	racket->afterdesc = " with her tennis racket";
	plum->setBasicAttack(racket);
	Attack* bigmushroom = new Attack("BIG MUSHROOM", "gave a red embiggening mushroom to", false, 8, 0, 0, 0, 0, 0, true);
	Effect* supermushroom = new Effect("SUPER", 3, 0, 0, 2.0f, 2.0f);
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
	Effect* metallic = new Effect("METAL", 3, 0, 0, 5.0f, 2.5f);
	metalhat->addEffect(metallic);
	plum->addSpecialAttack(metalhat);
	Attack* lightning = new Attack("LIGHTNING", "used a lightning bolt to shock the enemy team", false, 22, 30, 30, 1, 1, 7, 14);
	lightning->focushits = false;
	Effect* mini = new Effect("MINI", 3, 0, 0, 0.5f, 0.5f);
	lightning->addEffect(mini);
	plum->addSpecialAttack(lightning);
	Attack* blueshell = new Attack("BLUE TURTLE SHELL", "threw a winged blue turtle shell, which went flying at", false, 20, 80, 40, 1, 1, 3, false, 16);
	plum->addSpecialAttack(blueshell);
	Attack* superstar = new Attack("SUPER STAR", "gave a super star to", false, 25, 0, 0, 0, 0, 0, true, 18);
	Effect* invincible = new Effect("INVINCIBLE", 3, 0, 0, 1, 2.0f);
	invincible->invincible = true;
	superstar->addEffect(invincible);
	plum->addSpecialAttack(superstar);
	Attack* lifemushroom = new Attack("LIFE MUSHROOM", "gave a green life mushroom to", false, 30, 0, 0, 0, 0, 0, true, 20);
	lifemushroom->extralives = 1;
	plum->addSpecialAttack(lifemushroom);

	//Gambler Graham is the rng guy MARK: Graham
	NPC* graham = new NPC("GAMBLER", "GRAHAM", "A sorry gambling addict who is trillions in debt.\nHe'll pay it off as soon as he wins; any day now.", casino, 19, Stats(), Stats());
	Conversation gramconvo = {{self, "You should stop gambling."}, {graham, "What?"}, {graham, "Haven't you heard that 99% of gamblers quit right before hitting it big?"}, {NULL, "GAMBLING MACHINE - \"You lose 1000000 monies.\""}, {graham, "Aw dang it."}};
	gramconvo->skipcondition = NOGAMBLING;
	graham->addConversation(gramconvo);
	graham->addRejectionDialogue({{self, "Hey wana join my team?"}, {graham, "Sure man, as soon as I win the jackpot."}, {graham, "I'm just about to win it. I can feel it!"}, {NULL, "GAMBLING MACHINE - \"You lose 1000000 monies.\""}, {graham, "Aw dang it."}});
	graham->addRecruitmentDialogue({{self, "Well now that you can't gamble anymore you wanna join my team?"}, {graham, "Eh..."}, {graham, "Sure, but I'm coming back once they fix this place."}});
	graham->addDismissalDialogue({{graham, "I'm just gonna go check if they've fixed the casino yet."}});
	graham->setRecruitDialogueChange({});

	Item* gramplug = new WorldChangeItem("PLUG", "A plug that is plugged into the wall as plugs are.", casino, {});
	WorldChange& gramplugchanges = ((WorldChangeItem*)gramplug)->getChanges();
	gramplugchanges.recruitLinks.push(graham);
	gramplugchanges.linkedDescriptions.push({graham, "Gambling addict whose access to gambling you destroyed."});
	gramplugchanges.linkedDialogue.push({graham, {{graham, "Aggghhhh I was JUST about to winnnn....."}}});

	//1 blows up

	//2

	//3

	//4

	//5

	//6

	//7

	//8

	//9

	//10 nothing

	//11

	//12

	//13

	//14

	//15 summon guard for team

	///16

	//17

	//18

	//19 big bomb

	//20 instakill target

	//Rich Guy Richie is the summoner MARK: Richie
	NPC* richie = new NPC("RICH GUY", "RICHIE", "Rich guy trying to figure out what to do with his massive inheritence.", richneighborhood3, 20, Stats(), Stats());

	//rejection: No, everyone in this city is either rich and trying to get me to join some BURGER cult,
	//or not rich and trying to rob me.
	//No offense but I don't really trust you.

	//Hey you wanna help me fight these BURGER guys?
	//In that building over there?
	//Yea.
	//You know we have like these advanced security systems right?
	//You're gonna get blown up as soon as you walk onto their doorstep.
	//Nah check it out I got this cloaking device.
	//You turn invisible and visible again.
	//Huh.
	//So I'm going there to save this kid they kidnapped.
	//Oh.
	//Yeah I really want these BURGER guys gone.
	//Like their whole cult thing is one thing.
	//But you just said they kidnapped someone?
	//Yeah.
	//Yeah, lets go gettem.

	//Did you just blow up the BURGER headquarters?
	//I saw you walk in there and the top floor just blew up.
	//Oh that was the CEO.
	//We fought him normally but he kind of just blew up on his own.
	//Huh.
	//Hey you wanna help me find this kid they kidnapped?
	//They what?
	//Well I always wanted these BURGER guys gone.
	//But you just said they kidnapped someone?
	//That's way too far.
	//Yeah, lets go gettem.

	//Hey you wanna help me destroy BURGERs from existence?
	//...
	//What?
	//Yeah, using this THE PLOT DEVICE.
	//...
	//You were the one who blew up the CEO right?
	//Cause I saw you walk in their building and that happened.
	//Well we fought him normaly but he kind of just blew up on his own.
	//You sound like you're making everything up...
	//But you also clearly know what you're doing if you beat Enzo...
	//Yeah why not, I don't have anything better to do.
	//Plus I really want this BURGER cult gone.

	//Hey you wanna join my team?
	//You were the one who blew up the CEO right?
	//Cause I saw you walk in their building and that happened.
	//Well we fought him normaly but he kind of just blew up on his own.
	//Well if you beat Enzo you seem pretty qualified...
	//Why not, I don't have anything better to do.

	//dismissal: Welp, Imma head back to my house.

	Attack* throwmoney = new Attack("THROW MONEY AT THE PROBLEM", "threw a heavy gold bar at", -5, 20, 0, 1, 1, 1);
	richie->setBasicAttack(throwmoney);

	//drone

	//

	//marine mech

	//Bodyguard Buford is a damage dealer tied to Richie MARK: Buford
	NPC* buford = new NPC("BODYGUARD", "BUFORD", "Richie's bodyguard, trained in every martial art.", richneighborhood3, 30, Stats(), Stats());

	//

	//BURGER QUEST 1 Protagonist Henry Jerry is not that good at fighting but he's trying his best MARK: Henry Jerry
	NPC* hj = new NPC("BURGER QUEST 1 PROTAGONIST", "HENRY JERRY", "The protagonist of BURGER QUEST 1 who was used as a puppet of BURGER.\nHe wears a formal business suit and a traumatized expression.", limbo, 1, Stats(10, 2, 4, 1, 0, 4, 5));
	//self, "WHAT!?!?"
	//self, "The BURGER QUEST 1 protagonist, Henry Jerry?!?!"
	//hj, "Yeah that's my name."

	//briefbase
	
	//MARK: other npcs
	NPC* archie = new NPC("VILLAGE ELDER", "ARCHIE", "The elder of Tactical Tent Village.\nHe stands there all day and night like a statue.", village, 50);
	archie->setDialogue("Safe travels!");
	Conversation archcondefault = {{archie, "So you are going on a BURGER QUEST, I hear?"},
							 	   {archie, "Just keep heading NORTH, and you'll soon reach BURGERSBURG."},
								   {archie, "Safe travels!"},
								   {archie, "Make sure to bring me back a BURGER!"}};
	archcondefault.skipcondition = TEMPLEQUEST;
	archie->addConversation(archcondefault);

	Conversation archrej1 = {{self, "Hey wanna join me on my BURGER QUEST?"},
							 {archie, "I am sorry. Though I would love to join you,"},
							 {archie, "I must stay here and watch over the village."},
							 {archie, "Make sure to bring back a BURGER for me!"}};
	archrej1.skipcondition = TEMPLEQUEST;
	Conversation archrej2 = {{self, "Hey wanna help me destroy BURGERs?"},
							{archie, "Destroy BURGERs?"},
							{archie, "Well, I must watch over the village, sorry."}};
	archrej1.alt = &archrej2;
	archrej2.skipcondition = BURGERMENDEF;
	Conversation archrej3 = {{self, "Hey wanna join my team?"},
							{archie, "I appreciate the offer, but I must watch over the village."}};
	archrej2.alt = &archrej3;
	archie->addRejectionDialogue(archrej1);
	
	NPC* fisho = new NPC("FISHERMAN", "FRED", "Your dad, a fisherman who frequently fishes at the village docks.", docks, 25);
	fisho->setDialogue("The fish do be fishing today.");
	Conversation fishrej1 = {{self, "Hey wanna join me on my BURGER QUEST?"},
							 {fisho, "I've already gone on mine."},
							 {fisho, "It might be difficult but I know you're capable of doing this."}};
	fishrej1.skipcondition = TEMPLEQUEST;
	Conversation fishrej2 = {{self, "Hey wanna help me destroy BURGERs?"},
							{fisho, "Uh, I'm busy fishing sorry."}};
	fishrej1.alt = &fishrej2;
	fishrej2.skipcondition = BURGERMENDEF;
	Conversation fishrej3 = {{self, "Hey wanna join my team?"},
							{fisho, "Sorry, kid, I gotta be fishing right now."}};
	fishrej2.alt = &fishrej3;
	fisho->addRejectionDialogue(fishrej1);
	Conversation fishdef = {{fisho, "Hey I heard you're finally on a BURGER QUEST?"},
							{fisho, "The trek has gotten more difficult recently,"},
							{fisho, "but I know you're capable."},
							{fisho, "Here, take this fish I just caught."},
							{fisho, "It's not much but I'm sure it'll be useful to you."},
							{self, "Thanks dad."},
							{fisho, "Of course! Well, have a safe trip!"}};
	fishdef.skipcondition = TEMPLEQUEST;
	fisho->addConversation(fishdef);

	Item* fish = new HpItem("HEALTHY FISH", "A fish given by your dad to support your BURGER QUEST. (heals 15 HP)", limbo, 15);
	fisho->setGift(fish);

	NPC* mango = new NPC("", "MANGOLIA", "Your mom, not really a fan of fish.", tenthouse, 10);
	mango->setDialogue("The fish do be fishing today.");
	Conversation mangorej1 = {{self, "Hey wanna join me on my BURGER QUEST?"},
							  {mango, "Sorry sweetie I can't be going on quests anymore."},
							  {mango, "But I hope yours goes well!"}};
	mangorej1.skipcondition = TEMPLEQUEST;
	Conversation mangorej2 = {{self, "Hey wanna help me destroy BURGERs?"},
							  {mango, "Now I don't wanna hear any of this nonsense."},
							  {mango, "Besides, how would you even do that?"}};
	mangorej1.alt = &mangorej2;
	mangorej2.skipcondition = BURGERMENDEF;
	Conversation mangorej3 = {{self, "Hey wanna join my team?"},
							  {mango, "Sorry sweetie, I don't have time to play right now."}};
	fishrej2.alt = &mangorej3;
	mango->addRejectionDialogue(mangorej1);
	Conversation mangodef = {{mango, "Ah! I've heard you're on your BURGER QUEST."},
							 {mango, "I'm just so proud!"},
							 {mango, "Here, take this cake I made just for this occasion!"},
							 {self, "Thanks mom."},
							 {mango, "Of course sweetie! Have a safe trip!"}};
	mango->addConversation(mangodef);

	Item* cake = new HpItem("HEALTHY CAKE", "A cake your mom made to commemorate your BURGER QUEST. (heals 100 HP)", limbo, 100);
	mango->setGift(cake);

	NPC* burgerman = new NPC("", "BURGER MAN", "The manager of the BURGER RESTAURANT. He has a BURGER for a head and an uncanny stature.", BURGERRESTAURANT, 2147483647, Stats(2147483647, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647));
	burgerman->setLeader(true, 0, NULL, false);
	Attack* burgpunch = new Attack("BURGER PUNCH", "punched", true, 1, 99999);
	burgerman->setBasicAttack(burgpunch);

	//HELLO WELCOME TO MY BURGER RESTAURANT!
	//HOW MAY I TAKE YOUR ORDER?

	//YOU HAVE BEEN MAKING QUITE THE MESS.
	//YOU THOUGHT I WOULDN'T NOTICE?
	//I wasn't thinking about you.
	
	//YOU ARE REALLY STUPID.
	//YOU CAME BACK WITHOUT THE PLOT DEVICE?
	//<<<       BURGER QUEST COMPLETE  T_T       >>>
	//<<< ENDING ACHIEVED: YOU FORGOR THE DEVICE >>>
	
	NPC* burgerprisoner = new NPC("BURGER PRISONER", "ARCHIBALD", "A man imprisoned for resisting the global domination of BURGER.", BURGERPRISON, 35);
	burgerprisoner->setDialogue("Hi how are you doing?");
	burgerprisoner->addRejectionDialogue("I would love to join you on your quest. But as long as the BURGER MENACE endures, so shall these bars you see in front of me.");

	NPC* jimmyjohn = new NPC("SHOPKEEPER", "JIMMY JOHN", "The owner of the village convenience store. He has an italian accent.", tentstore, 30);
	jimmyjohn->setDialogue("Welcome to my convenience store! None is more convenient!");
	jimmyjohn->addRejectionDialogue("I'm sorry I cannot. Who will take care of my store?");

	Effect* minibuff = new Effect("MINIBUFF", 4, 0, 0, 1.25f, 1.25f, 1.25f, 1.25f);
	Effect* buff = new Effect("BUFF", 4, 0, 0, 1.5f, 1.5f, 1.5f, 1.5f);
	Effect* bigbuff = new Effect("BIG BUFF", 4, 0, 0, 2.0f, 2.0f, 2.0f, 2.0f);
	Effect* megabuff = new Effect("MEGABUFF", 2, 0, 0, 2.5f, 2.5f, 2.5f, 2.5f);
	
	Item* mythicmango = new HpItem("MYTHICAL MANGO", "The most nutritious fruit, a big mango that sparkles in the sunlight. (heals all HP)", limbo, 2147483647); //JIMMY JOHN - Ah yes, that's a very rare mango. Make sure to save it until you really need it! And thank you for your patronage

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

	tentstore->setStock(apple, 2147483647, 5, "JIMMY JOHN - Thank you for your patronage! Enjoy your apple!");
	tentstore->setStock(pineapple, 2147483647, 20, "JIMMY JOHN - Thank you for your patronage! Enjoy your pine apple!");
	tentstore->setStock(starapple, 2147483647, 60, "JIMMY JOHN - Thank you for your patronage! Enjoy your star apple!");
	tentstore->setStock(noodles, 2147483647, 5, "JIMMY JOHN - Thank you for your patronage! Enjoy your noodles!");
	tentstore->setStock(lasagna, 2147483647, 25, "JIMMY JOHN - Thank you for your patronage! Enjoy your lasagna!");
	tentstore->setStock(pizza, 2147483647, 50, "JIMMY JOHN - Thank you for your patronage! Enjoy your pizza!");
	tentstore->setStock(energybook, 1, 100, "JIMMY JOHN - I don't understand that book. But if it interests you then great! Oh, and thank you for your patronage!");	

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
	factconvo.skipcondition = SWITCHUSED;
	Conversation factconvo2 = {{factelder, "Child, Have you seen the lava sea?"},
							   {factelder, "It's been abnormally high recently, covering up the road to BURGERSBURG."},
							   {factelder, "But the allure of BURGER is too great!"},
							   {factelder, "Some people try to sail across and get burnt up!"},
							   {factelder, "Some are even so desperate they try to swim across!"},
							   {factelder, "Please! You must lower the lava again!"},
							   {factelder, "These factories have drainage valves in their control rooms!"},
							   {self, "Yea I've already been doing that."},
							   {factelder, "Oh. Well, carry on then."}};
	factconvo.alt = &factconvo2;
	factconvo2.skipcondition = LAVADRAINED;
	Conversation factconvo3 = {{factelder, "Child, Have you seen the lava sea?"},
							   {self, "Yeah I already did this whole quest."},
							   {self, "Lava sea looks pretty lowered now."},
							   {factelder, "Oh. Well, good job!"},
							   {self, "Thanks."}};
	factconvo2.alt = &factconvo3;
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
	//  ""---___\/___ ___\__--""		//  ""---___ /___ ___\__--""		//  ""---___ /___ ___\__--""	//    "--___ /___ ___\__--"
	//          |  |   |  |				//          |  |   |  |				//          |  |   |  |			//          |  |   |  |
	//           \   -   /				//           \   -   /				//           \   -   /			//           \   -   /
	//             """""				//             """""				//             """""			//             """""" \ 
	//            |     |				//            |     |				//            |     |			//            |     |.`\ 
							{developer, "Well that's pretty unfortunate."},
							{self, "Why did you let them name me. T_T"}, //this is annoyed face not crying
							{developer, "Sorry I came up with your name after the naming feature."},
							{developer, "Don't worry you will always truly be BERNARD."},
							{developer, "Well good job getting here."},
							{developer, "Here have this cool amazing new move."},
							{self, "Yoooo nice thanks.\n\n\n"}};
	devconvo.skipcondition = ISBERNARD;
	Conversation devconvo2 = {{developer, "Ayy BERNARD how's it going?"},
							{self, "Pretty good!"},
							{self, "The player actually named me the correct name!"},
							{developer, "Yo nice."},
							{developer, "Well good job getting here."},
							{developer, "Here have this cool amazing new move."},
							{self, "Yoooo nice thanks.\n\n\n"}};
	devconvo.alt = &devconvo2;
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
	Attack* parry = new Attack("PARRY", "is preparing to parry", 7, 0, 0, 1, 1, 1);
	parry->parry = true;
	Item* parryguide = new EducationItem("PARRYING GUIDE", "An in depth guide on how to PARRY enemy attacks.", limbo, parry);
	developer->setGift(parryguide);
	/*burgerman->setLink(developer);
	burgerman->addLinkedConvo(self, "Yo developer man.");
	burgerman->addLinkedConvo(developer, "Hey what's up?");
	burgerman->addLinkedConvo(self, "So I was wondering,");
	burgerman->addLinkedConvo(self, "what's up with all the parallel universes in the first game?");
	burgerman->addLinkedConvo(self, "Also who was the other BURGER MAN?");
	burgerman->addLinkedConvo(developer, "Oh uhhhhh... honestly idk maybe I'll just retcon that.");
	burgerman->addLinkedConvo(developer, "Wait no I got this.");
	burgerman->addLinkedConvo(developer, "So the parallel universes are just the same universe but going through them increments the number.");
	burgerman->addLinkedConvo(developer, "Wait that makes no sense because of the news.");
	burgerman->addLinkedConvo(developer, "Uhhhhhhhhhhhh...");
	burgerman->addLinkedConvo(developer, "No clue but maybe I'll come up with something later and edit this conversation.");
	burgerman->addLinkedConvo(developer, "Anyway about the other BURGER MAN,");
	burgerman->addLinkedConvo(developer, "You know how HENRY JERRY's company invented time travel?");
	burgerman->addLinkedConvo(self, "Uh huh.");
	burgerman->addLinkedConvo(developer, "Yeah so the BURGER MAN used the time machine.");
	burgerman->addLinkedConvo(developer, "So he ordered the BURGER from himself.");
	burgerman->addLinkedConvo(self, "Dang that's crazy.");*/
	
	//talk about the game after beating final boss
	//and dlc plans, crowbars
	//jim shady commentary?
	//favorite character
	//least favorite character
	//what are monies
	//have you tried... the gym, the simulator, the triple chest in the sewers, the bank, BURGER QUEST 1
	//have you fought greer yet
	//can you give me feedback (and say link to form)
	//the funny thing with defend
	//save system stuff
	//commentary about the world
	//commentary about character designs nobody will ever see cause it's text based (link to see picture?)
	//scope creep
	//future projects
	//saying dancing
	//favorite game
	//talk about the mountain
	//big cat
	//limbo
	//burger quest 2 ideation
	//talk about bosses
	//zuul
	//burger all caps lore
	//kept thinking michelin is named jim
	//viola after fire fight
	//development time
	//so nice seeing game systems evolve
	//ascii art time
	//npcs saying generic player names

	Attack* pshrimplebeam = new Attack("SHRIMPLE BEAM", "fired a pressurized jet of water at", false, 25, 100, 100, 1, 1, 1);
	pshrimplebeam->instakill = true;
	pshrimplebeam->addDescription("Fire a jet of pressurized water at the target, instantly destoying non-boss enemies. (100 ATTACK, 100 PIERCE)");
	Item* shrimplegun = new WeaponItem("SHRIMPLE GUN", "An advanced red water gun granting non-shrimp wielders the ability to use the SHRIMPLE BEAM.", mountainlake, pshrimplebeam);

	NPC* gymbro = new NPC("GYM BRO", "JIM NASIUM", "Obsessed with being in peak physique, there's scarcely a moment when he isn't seen in the gym.\nHe isn't a shrimp, just to clarify.", desertgymfixed, 25);
	gymbro->addGymDialogue("YYYEEEEEEEEEEAAAAAAAAAAAAAHHHHHHHHHHHHHHHHH WEIGHT LIFTING!!!!!!!!!!!!!!!!!");
	gymbro->addRejectionDialogue("Sorry dude, I gotta stay on THAT GRIND to get THEM GAINS.");
	gymbro->setGymStart(1); //he will always catch up to your level

	mike->addGymDialogue({{mike, "HA this place is great for practicing my throwing range!"}, {gymbro, "PSST!"}, {gymbro, "Hey, kid."}, {self, "Yeah?"}, {gymbro, "Can you please hurry and RECRUIT this guy back?"}, {gymbro, "He's freaking scary."}});
	mike->addGymDialogue({{mike, "Check out how far I can throw this dynamite!"}});

	Attack* forkthrow = new Attack("FORK THROW", "threw a fork at", false, 0, 1, 0, 1, 1, 1);
	Attack* pickthrow = new Attack("PICKAXE THROW", "threw a pickaxe at", false, 0, 1, 2, 1, 1, 1);
	Attack* knifethrow = new Attack("KNIFE THROW", "threw a knife at", false, 0, 1, 1, 1, 1, 1);
	Attack* coverthrow = new Attack("HEAVY FRISBEE", "threw a heavy manhole cover at", false, 0, 80, 0, 1, 1, 1);

	Item* telescope = new InfoItem("TELESCOPE", "A large, robust, telescope for observing space.", "You looked through the telescope. You see an orbital office building!", tentlab);
	
	Item* fork = new ManholeItem("FORK","\"An implement with two or more prongs used for lifting food to the mouth or holding it when cutting.\"\n- Oxford Languages", forestfork, forkthrow);
	Item* pickaxe = new ManholeItem("PICKAXE","\"A tool consisting of a long handle set at right angles in the middle of a curved iron or steel bar with a point at one end and a chisel edge or point at the other, used for breaking up hard ground or rock.\"\n- Oxford Languages", mineshaft2, pickthrow);
	Item* knife = new ManholeItem("KNIFE", "\"An instrument composed of a blade fixed into a handle, used for cutting or as a weapon.\"\n- Oxford Languages", volcano5, knifethrow);

	Item* mhcover1 = new ManholeItem("MANHOLE COVER", "A heavy metal cover to the sewers. You could probably use this like a frisbee.", volcano2, coverthrow, sewerentrance1, DOWN);
	Item* mhcover2 = new ManholeItem("MANHOLE COVER", "A heavy metal cover to the sewers. You could probably use this like a frisbee.", volcano6, coverthrow, sewer2, DOWN);

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

	Item* BURGER = new BURGERItem("BURGER", "It's a BURGER and it smells like a BURGER.", limbo, IN_ELEVATOR);
	BURGERRESTAURANT->setStock(BURGER, 2147483647, 10, "BURGER MAN - \"ENJOY YOUR BURGER!\"");

	Item* skateboard = new InfoItem("SKATEBOARD", "It's a pretty cool skateboard for doing cool skateboard things.", "You did a kickflip. Very cool.", limbo);
	skateboard->setTakable();
	desertshopfixed->setStock(skateboard, 1, 100, "MERRO - \"Thank you for your monies.\"");

	Item* sunscreen = new KeyItem("SUNSCREEN", "Bottle of sunscreen for resisting the heat.", {{NULL, "You applied the sunscreen."}, {NULL, "No amount of radiation should bother you now!"}}, limbo, HEAT, true);
	KeyItem* _sunscreen = (KeyItem*)sunscreen;
	_sunscreen->setTarget(volcanoentrance);
	desertshopfixed->setStock(sunscreen, 1, 20, "MERRO - \"Thank you for your monies.\"");

	Item* vitaminb = new SpItem("VITAMIN B", "A small supplement bottle of pure Vitamin B. (Restores 15 SP)", limbo, 15);
	desertshopfixed->setStock(vitaminb, 2147483647, 20, "MERRO - \"Thank you for your monies.\"");
	Item* iron = new SpItem("IRON", "A small bottle of iron supplement. (Restores 30 SP)", limbo, 30);
	desertshopfixed->setStock(iron, 2147483647, 40, "MERRO - \"Thank you for your monies.\"");
	Item* magnesium = new SpItem("MAGNESIUM", "A small bottle of magnesium supplement. (Restores all SP)", limbo, 2147483647);
	desertshopfixed->setStock(magnesium, 2147483647, 100, "MERRO - \"Thank you for your monies.\"");

	Item* reviveroot = new ReviveItem("REVIVE ROOT", "A small root vegetable known for completely healing any injury. (Recapacitates teammates)", limbo, 2147483647);
	desertshopfixed->setStock(reviveroot, 2147483647, 300, "MERRO - \"Thank you for your monies.\"");
	
	Item* rotrevroot = new ReviveItem("ROTTEN REVIVE ROOT", "A spoiled revive root, still capable of healing,\nthough not to the extent of its fresh version.", burgstore, 10);

	Item* hotdog = new HpItem("HOT DOG", "A classic urban hot dog with mustard. (heals 2 HP)", limbo, 2);
	coolstreet3->setStock(hotdog, 2147483647, 2, "HARRY - \"Ayy thanks for the purchase enjoy your hot dog!\"");

	NPC* hotdogguy = new NPC("HOT DOG VENDOR", "HARRY", "Manager of the lucrative BURGERSBURG hot dog stand.", coolstreet3, 14);
	hotdogguy->addConversation({{self, "Hey!"},
								{self, "Got any grapes?"},
								{hotdogguy, "No, I just sell hot dogs."},
								{hotdogguy, "But they're hot and they're fresh and they're all homemade!"},
								{hotdogguy, "You want a frank?"},
								{self, "No thank."}});
	hotdogguy->setDialogue("Ayyy what's up man you want a hot dog?");
	hotdogguy->addRejectionDialogue("Sorry no, this hot dog stand is my life's calling.");

	//it's TECH DEMO MAN!
	NPC* techdemoman = new NPC("", "TECH DEMO MAN", "Mechanical superhero for testing the game in the past!\n\"Since the game is unbalanced, I'll just make an even more unbalanced NPC!\"\n-Tomas", tenthome, 0, Stats(200000, 200000, 20, 20000, 10, 15, 20));
	techdemoman->setDialogue("HELLO THERE THIS IS MY DIALOGUE!");
	techdemoman->addRejectionDialogue("NO THIS IS NO LONGER THE TECH DEMO.");

	Item* cloakingdevice = new WorldChangeItem("CLOAKING DEVICE", "Specialized cloaking device for getting past advanced security systems.", limbo, {{NULL, "You equipped the CLOAKING DEVICE."}, {NULL, "No security system can spot you now!"}});
	cloakingdevice->setTakable(true);
	WorldChange& cloakingchange = ((WorldChangeItem*)cloakingdevice)->getChanges();
	cloakingchange.exitUnblocks.push({richneighborhood1, NORTHEAST});
	cloakingchange.exitUnblocks.push({richneighborhood2, NORTH});
	cloakingchange.exitUnblocks.push({richneighborhood3, NORTHWEST});
	cloakingchange.recruitLinks.push(richie);
	cloakingchange.recruitLinks.push(forestknight);
	
	NPC* child = new NPC("CHILD", "JILLY", "The daughter of MATILDA who was kidnapped by the BURGER corporation, even younger than your sister.", limbo, 3);

	NPC* matilda = new NPC("WORRIED MOTHER", "MATILDA", "A frequent churchgoer with distress very visible on her face.", burgchurch, 10);
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
	matilda->setGift(cloakingdevice);
	//rejection
	//dialogue

	//jilly is surprisingly good at fighting and when you USE the bag she jumps out of it and kicks Bernard in the gut
	//"OW"
	//"Take that you meanie!"
	//"BRO WHAT THE HECK I'M TRYING TO SAVE YOU"
	//"Oh okay!"
	//"Thank you mister!"

	//I have just finished fighting the CEO
	//I should be getting close!
	//Oh.

	//
	
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
						  {jim, "Yeah."},
						  {self, "Cool."}});
	jim->setDialogue({{jim, "..."}, {self, "..."}});
	jim->addRejectionDialogue("I have to manage my apartment.");
	jim->setGift(greenkey);
	
	NPC* skeleseller = new NPC("SKELETON", "KELVIN", "He appears to be a skeleton on the floor.", kaboomroom, 5);
	skeleseller->setDialogue("Well hello there! Could I interest you in some explosives?");
	skeleseller->addRejectionDialogue("Sorry fella, my ligaments are long gone. No walking for me!");
	Attack* pdynamite = new Attack("DYNAMITE", "threw a stick of dynamite at", false, 0, 20, 20, 1, 1, 1);
	Item* dynamite = new KeyItem("DYNAMITE", "Explosives for exploding stuff.", {{NULL, "You threw the dynamite at the the rubble."}, {NULL, "*KABOOM!*"}, {NULL, "The exit has been unblocked!"}}, limbo, RUBBLE, true, pdynamite);
	kaboomroom->setStock(dynamite, 2147483647, 5, "KELVIN - \"Do you need some explosives? Here you go! I assume you know what you're doing...\"");

	NPC* bob = new NPC("", "BOB", "A small child wearing a newsboy cap. He has a hard time making human friends and prefers plants.", limbo, 0);
	Conversation bobcon = {{bob, "Oh my poor friend Cacty!"},
						   {bob, "He's so dehydrated!"},
						   {bob, "If only the underground spring never dried up!"},
						   {self, "The what?"},
						   {bob, "The underground spring that fueled the oasis!"},
						   {bob, "Clearly there's something wrong with it because the oasis is dry!"}};
	bobcon.skipcondition = VALVEUSED;
	bob->addConversation(bobcon);
	bob->setDialogue({{bob, "Oh my poor friend Cacty!"},
					  {bob, "He's so dehydrated!"}});
	bob->addRejectionDialogue("No my mama says I'm too young to go adventuring.");
	
	NPC* franklin = new NPC("TOWN ELDER", "FRANKLIN", "The town elder of the desert town, and an avid tennis player. He has a big mustache.", limbo, 5);
	franklin->addConversation({{franklin, "Say, I had a very good view of your fight with Viola."},
							   {franklin, "I simply must thank you for saving our humble town!"},
							   {franklin, "Here, have this desert delicacy!"},
							   {self, "Oh thanks."}});
	Item* desertdelicacy = new HpItem("DESERT DELICACY", "A very nice desert pie, featuring rare desert fruit. (heals all HP)", limbo, 2147483647);
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

	NPC* merchant = new NPC("MERCHANT", "MERRO", "Merchant and owner of the desert store. He really wants your monies.", limbo, 12);
	merchant->setDialogue("Welcome, my friend, to my store.");
	merchant->addRejectionDialogue("No I want to sell things.");

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

	//make coolant attack that slows down enemies
	Item* sandcoolant = new KeyItem("SAND COOLANT", "Bottle of coolant handy for cooling down sand of the scorching variety.", {{NULL, "You poured some sand coolant onto the scorching sands."}, {NULL, "The sands cooled down!"}}, deserttempleentrance, SAND, false);
	Item* powerpole = new MovementItem("POLE VAULT", "Very long stick, useful for travelling over chasms.", {{NULL, "You used the pole to go over the chasm!"}}, desertpole, CHASM, true);
	Item* minecart = new MovementItem("SPARE MINECART", "A spare minecart unclamped from the tracks.", {{NULL, "You hopped into the SPARE MINECART and went to the other side of the track."}}, mineshaftside, TRACK, false);

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
	Item* downbutton = new PaverItem("DOWN BUTTON", "An elevator button for going downwards. It's the same style as the BURGER RESTAURANT elevator's buttons.", {{NULL, "There is nowhere to put the DOWN BUTTON!"}, {NULL, "..."}, {NULL, "You shove the DOWN BUTTON into the wall."}, {NULL, "The elevator can go DOWN now!"}}, burgsafe, elevator, DOWN, elevatorbottom);

	Item* forklift = new KeyItem("FORKLIFT", "Cool thing for lifting stuff such as collapsed roof material.", {{NULL, "You used the FORKLIFT to move the collapsed ceiling material out of the way."}}, heavymachineryroom, STUFF, false);
	//you can use the scissor lift to get to the ninja village I guess, I'll keep it because it technically makes sense and it's funny
	Item* scissorlift = new KeyItem("SCISSOR LIFT", "Cool thing for going UP and DOWN straight horizontal directions.", {{NULL, "You toggled the extension of the SCISSOR LIFT."}}, heavymachineryroom, HIGH, false);
	scissorlift->setDropToUse(true);

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

	//firefighter

	//

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
	foresttempleentrance->setExit(SOUTHWEST, forestleft);
	foresttempleentrance->setExit(SOUTHEAST, forestright);
	foresttempleentrance->setExit(NORTHWEST, flowerfield);
	foresttempleentrance->setExit(NORTH, forestfork);
	//foresttempleentrance->setExit(IN_TEMPLE, foresttemplestairs);
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
	//deserttempleentrance->setExit(IN_TEMPLE, deserttemplestairs);
	deserttempleentrance->setExit(EAST, desertdune);
	deserttempleentrance->setExit(NORTHEAST, deserthill);
	deserttemplestairs->setExit(OUTSIDE, deserttempleentrance);
	deserttemplestairs->setExit(WEST, deserttemple);
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
	//volcanotempleentrance->setExit(IN_TEMPLE, volcanotemplestairs);
	volcanotemplestairs->setExit(OUTSIDE, volcanotempleentrance);
	volcanotemplestairs->setExit(WEST, volcanotemple);
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
	leftstreet3->setExit(NORTH, leftstreet4);
	leftstreet3->setExit(SOUTH, leftstreet2);
	leftstreet3->setExit(EAST, newstreet3);
	leftstreet4->setExit(NORTH, leftstreet5);
	leftstreet4->setExit(SOUTH, leftstreet3);
	leftstreet4->setExit(EAST, newstreet4);
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
	newstreet3->setExit(NORTH, newstreet4);
	newstreet3->setExit(SOUTH, newstreet2);
	newstreet3->setExit(EAST, mainstreet3);
	newstreet3->setExit(WEST, leftstreet3);
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
	mainstreet5->setExit(SOUTH, mainstreet4);
	mainstreet5->setExit(WEST, newstreet5);
	mainstreet5->setExit(EAST, coolstreet5);
	mainstreet5->setExit(INSIDE, elevator);
	coolstreet1->setExit(NORTH, coolstreet2);
	coolstreet1->setExit(SOUTHWEST, BURGERSBURG);
	coolstreet1->setExit(EAST, rightstreet1);
	coolstreet1->setExit(WEST, mainstreet1);
	coolstreet2->setExit(NORTH, coolstreet3);
	coolstreet2->setExit(SOUTH, coolstreet1);
	coolstreet2->setExit(EAST, rightstreet2);
	coolstreet2->setExit(WEST, mainstreet2);
	coolstreet3->setExit(NORTH, coolstreet4);
	coolstreet3->setExit(SOUTH, coolstreet2);
	coolstreet3->setExit(EAST, rightstreet3);
	coolstreet3->setExit(WEST, mainstreet3);
	coolstreet4->setExit(NORTH, coolstreet5);
	coolstreet4->setExit(SOUTH, coolstreet3);
	coolstreet4->setExit(EAST, rightstreet4);
	coolstreet4->setExit(WEST, mainstreet4);
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
	ceoelevator1->setExit(UP, ceoelevator2);
	ceoelevator1->setExit(DOWN, ceoelevator0);
	ceoelevator2->setExit(UP, ceoelevator3);
	ceoelevator2->setExit(DOWN, ceoelevator1);
	ceoelevator3->setExit(UP, ceoelevator4);
	ceoelevator3->setExit(DOWN, ceoelevator2);
	ceoelevator4->setExit(OUTSIDE, ceoroom);
	ceoelevator4->setExit(DOWN, ceoelevator3);
	ceoroom->setExit(IN_ELEVATOR, ceoelevator4);
	ceoroom->setExit(IN_SAFE, burgsafe);
	burgsafe->setExit(OUT, ceoroom);
	elevator->setExit(UP, elevatortop);
	elevator->setExit(OUTSIDE, mainstreet5);
	elevatortop->setExit(OUT, BURGERRESTAURANT);
	elevatortop->setExit(DOWN, elevator);
	BURGERRESTAURANT->setExit(IN_ELEVATOR, elevatortop);
	elevatorbottom->setExit(UP, elevator);
	elevatorbottom->setExit(OUT, burgerbasement);
	burgerbasement->setExit(IN_ELEVATOR, elevatorbottom);
	burgerbasement->setExit(SOUTH, BURGERPRISON);
	BURGERPRISON->setExit(NORTH, burgerbasement);
	basestation->setExit(NORTH, BURGERPRISON);
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
	Effect* theburn = new Effect("THE BURN", 3, 0, 0, 2.0f, 2.0f, 2.0f);
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
	Attack* spatula = new Attack("UPSLASH", "flipped", true, 6, 10, 0, 1, 1, 1);
	spatula->afterdesc = " into the air with a spatula";
	Effect* spatulad = new Effect("UPSLASHED", 0); //doesn't cancel their turn but may interrupt healers wanting to heal them
	spatulad->remove = true;
	spatulad->falldamage = 20;
	spatula->addEffect(spatulad);
	ninjachef->addSpecialAttack(spatula);
	Attack* ninjafeast = new Attack("NINJA FEAST", "prepared a ninja feast for his team", false, 8, -30, 0, 1, 1, 7, true);
	ninjachef->addSpecialAttack(ninjafeast);
	Attack* ninjasmoothie = new Attack("NINJA SMOOTHIE", "prepared a ninja smoothie for", false, 3, 20, 5, 1, 1, 1, true, 11);
	Effect* smoothied = new Effect("SMOOTHIED", 3, 0, 0, 1.5f, 1, 1, 1.5f);
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
	jimshady->setEffect(engarde, false);

	NPC* jimmyshimmy = new NPC("", "JIMMY SHIMMY", "A juvenile shrimp who likes to help out his fellow shrimps.", limbo, 0, Stats(20, 0, 10, 0, 20, 15, 9));
	Attack* shrimpleshimmy = new Attack("SHRIMPLE SHIMMY", "shimmied all over", true, 0, 5, 0, 3, 4, 1);
	jimmyshimmy->setBasicAttack(shrimpleshimmy);
	/*Effect* flinch = new Effect("FLINCH", 1);
	shrimpleshimmy->addEffect(flinch);*/

	/*NPC* carnplant = new NPC("", "CARNIVOROUS PLANT", "Really big plant who likes eating meat.", limbo, 0, Stats(20, 5, 7, 5, 5, 12, 10));
	Attack* bite = new Attack("BITE", "bit", true, -5, 10, 5, 1, 1, 1);
	Attack* nutrientabsorb = new Attack("NUTRIENT ABSORB", "sucked the nutrients out of", false, 10, 10, 5, 1, 1, 1, 0.5f);
	carnplant->setBasicAttack(bite);
	carnplant->addSpecialAttack(nutrientabsorb);*/

	NPC* flowerfiend = new NPC("", "FLOWER FIEND", "Really big carnivorous flower, probably the FLOWER FRIEND your sister talks about.", limbo, 0, Stats(20, 0, 7, 0, 0, 12, 9));
	Attack* vinewhip = new Attack("VINE WHIP", "used its vines to whip", true, -6, 10, 0, 1, 1, 1);
	Attack* crunch = new Attack("CRUNCH", "used its flowery fangs to crunch", true, 2, 18, 7, 1, 1, 1);
	Effect* flowerpower = new Effect("FLOWER POWER", 3, 0, 0, 2.0f);
	Attack* flowerempower = new Attack("FLOWER EMPOWER", "used its flower power to buff", false, 15, 10, 5, 1, 1, 1, true, 10);
	flowerempower->addEffect(flowerpower);
	Attack* solarbeam = new Attack("SOLAR BEAM", "used its petals to channel solar light towards", false, 18, 30, 10, 1, 1, 10, false, 10);
	flowerfiend->setBasicAttack(vinewhip);
	flowerfiend->addSpecialAttack(crunch);
	//flowerfiend->addSpecialAttack(nutrientabsorb);
	flowerfiend->addSpecialAttack(flowerempower);
	flowerfiend->addSpecialAttack(solarbeam);

	NPC* egadbot= new NPC("ROGUE ROBOT", "EGARDENBOT", "Short trapezoidal copper robot designed to be an expert gardener,\nbefore going rogue and trimming everything else as well.", limbo, 0, Stats(20, 30, 5, 5, 10, 20, 9));
	Attack* snip = new Attack("SNIP", "snipped scissors at", true, -5, 7, 5, 1, 1, 1);
	egadbot->setBasicAttack(snip);
	Attack* timber = new Attack("TIMBER", "snipped down a tree, directed at", false, 10, 20, 0, 1, 1, 1);
	egadbot->addSpecialAttack(timber);

	NPC* savagehog = new NPC("", "MAMMOTH HOG", "Savage, mammoth elder hog with very sharp prickles.", limbo, 0, Stats(90, 20, 10, 10, 10, 10, 9), Stats(0, 0, 1, 1, 1, 0, 1));
	Attack* charge = new Attack("CHARGE", "charged at", true, -5, 10, 20, 1, 1, 1);
	Attack* savageroar = new Attack("SAVAGE ROAR", "roared savagely at", false, 5, 0, 0, 1, 1, 7);
	Effect* intimidated = new Effect("INTIMIDATED", 4, 0, 0, 0.5f);
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
	mimic->setBoss(true);

	NPC* sandman = new NPC("", "SANDMAN", "A really sandy humanoid continuously flowing with sand.", limbo, 0, Stats(20, 5, 8, 0, 0, 10, 9));
	Effect* sanded = new Effect("SAND IN THE EYES", 3, 0, 0, .5f, .5f);
	Attack* sandthrow = new Attack("POCKET SAND", "threw sand at", false, -3, 5, 0, 1, 1, 1);
	sandthrow->afterdesc = "'s eyes";
	sandthrow->addEffect(sanded);
	Attack* sandpunch = new Attack("SAND PUNCH", "threw a sandy punch at", true, 1, 12, 0, 1, 1, 1);
	sandman->setBasicAttack(sandthrow);
	sandman->addSpecialAttack(sandpunch);

	//NPC* pyramid = new NPC("", "PYRAMON", "Floa");

	NPC* skeleviking = new NPC("", "SKELEVIKING", "A lost skeleton with a horned hat and shield.", limbo, 0, Stats(5, 0, 30, 0, 30, 15, 9));
	Effect* shieldup = new Effect("SHIELD UP", 2147483647);
	shieldup->guardset = 5;
	skeleviking->setEffect(shieldup, false);
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
	Effect* jamming = new Effect("JAMMING OUT", 10, 0, 0, 2.5f, 1.5f, 1.5f, 1, 50.0f);
	rocknroll->selfeffect = jamming;
	Attack* saltcure = new Attack("SALT CURE", "coughed up crystals of curing salt onto", false, 15, 5, 0, 1, 1, 1);
	Effect* saltcured = new Effect("SALT CURED", 2147483647);
	saltcured->spusage = 2.0f; //salt cured means you take double damage and use double sp
	saltcured->damagebuff = 2.0f;
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
	NPC* tkviola = new NPC("TELEKINETIC KIDNAPPER", "VIOLA", "Telekinetic teenager responsible for the disappearence of the desert town.\nHer hair floats upwards and she hovers a few feet above the ground.", limbo, 0, Stats(100, 0, 10, 0, 10, 20, 19), Stats(1, 0, 1, 0, 2, 0, 1));
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
	Effect* intensegravitied = new Effect("INTENSE GRAVITY", 3, 0, 0, 1, 0.5f);
	intensegravity->addEffect(intensegravitied);
	tkviola->addSpecialAttack(intensegravity);
	Attack* blackhole = new Attack("BLACK HOLE", "formed a black hole at", false, 25, 35, 100, 1, 1, 7, false, 20);
	blackhole->addDescription("Form a black hole encompassing the enemies for heavy damage. (35 DAMAGE, 100 PIERCE)");
	tkviola->addSpecialAttack(blackhole);

	NPC* greer = new NPC("BURGER EXECUTIVE", "GREER", "Greedy, high-ranking BURGER official sent to manage all the desert's remaining water.", minespring, 0, Stats(100, 40, 20, 0, 0, 20, 9), Stats(1, 2, 1, 0, 1, 0, 1));
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

	Effect* extrafire = new Effect("EXTRA ON FIRE", 3, 10, 0, 1, 0.65f);
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
	Effect* poisoned = new Effect("POISONED", 5, 10, 0, 1, 0.75f);
	poiscurry->addEffect(poisoned);
	poiscurry->focushits = false;
	Attack* poisomit = new Attack("POISOMIT", "coughed up a ball of sticky corrosive sludge at", false, 2, 15, 30, 1, 1, 1);
	Effect* sludged = new Effect("SLUDGED", 10, 5, 0, 1, 0.75f, 0.5f, 1, 0.25f);
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
	Effect* smogged = new Effect("SMOGGED", 5, 8, 0, 0.75f, 0.75f);
	exhaust->addEffect(smogged);
	factgolem->setBasicAttack(swing);
	factgolem->addSpecialAttack(fistcannon);
	factgolem->addSpecialAttack(furnaceblast);
	factgolem->addSpecialAttack(exhaust);

	NPC* lavadile = new NPC("", "LAVADILE", "Juvenile lavagator, big from a healthy diet of ores.", limbo, 0, Stats(40, 20, 25, 30, 25, 15, 9));
	Effect* ouchie = new Effect("OUCHIE", 2, 0, 0, 1, 0.8f);
	Attack* cbite = new Attack("BITE", "bit down on", true, -5, 15, 15, 1, 1, 1);
	cbite->addEffect(ouchie);
	lavadile->setBasicAttack(cbite);
	Attack* clavomit = new Attack("LAVOMIT", "coughed up lava at", false, 5, 30, 20, 1, 1, 1);
	clavomit->addEffect(onfire);
	lavadile->addSpecialAttack(clavomit);

	NPC* lavagator = new NPC("", "LAVAGATOR", "Enormous alligator inhabitant of the laval sewer systems with retro shades.", limbo, 0, Stats(150, 25, 35, 35, 35, 20, 9));
	lavagator->setBoss(true); //miniboss
	Effect* gcrunched = new Effect("CRUNCHED", 3, 0, 0, 1, 0.6f);
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

	NPC* lavaguardian = new NPC("", "LAVA GUARDIAN", "Huge guardian with radiant molten armor and weapons.\nHe appears to have wandered onto the bridge while the lava level was high, and now guards the gate to BURGERSBURG.", limbo, 0, Stats(200, 50, 30, 20, 20, 10, 9), Stats(2, 1, 1, 0, 0, 0, 1));
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
	Effect* blinded = new Effect("BLINDED", 2, 0, 0);
	blinded->freeze = true;
	solarflare->addEffect(blinded);
	lavaguardian->addSpecialAttack(solarflare);
	Attack* gammarayburst = new Attack("GAMMA RAY BURST", "unleashed a burst of gamma radiation upon", false, 20, 40, 100, 1, 1, 3);
	lavaguardian->addSpecialAttack(gammarayburst);

	NPC* newtab = new NPC("", "NEW TAB", "Internet tabs who loyally serve their internet browser masters.", limbo, 0, Stats(10, 10, 10, 10, 10, 10, 9));
	Attack* askew = new Attack("ASKEW", "hit", true, -5, 10, 0, 1, 1, 1);
	askew->afterdesc = " askew";
	Effect* offbalance = new Effect("OFF BALANCE", 1, 0, 0, 0.8f);
	askew->addEffect(offbalance);
	newtab->setBasicAttack(askew);
	Attack* doabarrelroll = new Attack("DO A BARREL ROLL", "did a barrel roll at", true, 5, 10, 0, 4, 5, 1);
	newtab->addSpecialAttack(doabarrelroll);
	Attack* adblock = new Attack("ADBLOCK", "is blocking attacks thrown at", false, 10, 0, 0, 1, 1, 1, true, 10);
	adblock->protect = true;
	adblock->prioritizeleader = true;
	newtab->addSpecialAttack(adblock);

	NPC* browser = new NPC("EVIL KING", "BROWSER", "Giant spiked internet browser with cool red hair and a penchant for kidnapping princesses.", limbo, 0, Stats(210, 20, 20, 30, 10, 20, 9), Stats(1, 0, 1, 1, 1, 0, 1));
	browser->setBoss(true);
	Attack* medge = new Attack("MICROSOFT EDGE", "sliced", true, -5, 10, 20, 1, 1, 1); //made of fine chromium
	medge->afterdesc = " with his MICROSOFT EDGE";
	browser->setBasicAttack(medge);
	Attack* gsearch = new Attack("GOOGLE SEARCH", "googled", false, 5, 0, 0, 1, 1, 1);
	gsearch->afterdesc = "'s weaknesses";
	Effect* searched = new Effect("SEARCHED", 5, 0, 0, 1, 0.5f);
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
	Effect* foxfire = new Effect("FOX FIRE", 5, 10, 0, 1, 0.5f);
	firefox->addEffect(foxfire);
	browser->addSpecialAttack(firefox);
	plum->setDialogue({{plum, "AHHHHHHHHHH HELP ME I'VE BEEN KIDNAPPED!"}, {browser, "GWAHAHAHAHAHA!"}});
	castlethrone->setWelcome({{plum, "AHHHHHHHHHH HELP ME I'VE BEEN KIDNAPPED!"}, {browser, "GWAHAHAHAHAHA!"}});
	
	NPC* snowman = new NPC("", "SNOWMAN", "Really snowy humanoid who is very intent on beating you up.", limbo, 0, Stats(1, 1, 1, 1, 1, 1, 9), Stats(0, 0, 0, 0, 0, 0, 1));
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
	Effect* mcbond = new Effect("MOTHER-CHILD BOND", 2147483647, 0, 0, 1.125f); //MARK: we need to make sure this goes away when the hatchling is incapacitated
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
	popbomb->summonamount = 1;
	bewlizard->setBasicAttack(popbomb);
	Attack* bsod = new Attack("BURST STREAM OF DESTRUCTION", "fired a stream of destruction at", false, 25, 30, 50, 1, 1, 3);
	bsod->afterdesc = " from its mouth";
	bewlizard->addSpecialAttack(bsod);

	NPC* thief = new NPC("", "THIEF", "Citizen of BURGERSBURG turned to thievery due to desperate times.", limbo, 0, Stats(10, 0, 20, 0, 20, 25, 9));
	//stab
	//

	NPC* axeman = new NPC("", "AXEMAN", "A really deranged human whose head was exchanged for the head of an axe.", limbo, 0, Stats(40, 15, 35, 18, 50, 30, 9));
	//chop

	NPC* disamalg = new NPC("", "DISEASE AMALGAMATION", "Writhing mass of pathogens featuring many colors.", limbo, 0, Stats(100, 40, 5, 5, 30, 0, 9));
	//disease
	//

	NPC* crimmind = new NPC("", "CRIMINAL MASTERMIND", "Psychic criminal genius with a huge brain and a body shriveled from floating everywhere.", limbo, 0, Stats());
	//
	//suspend
	//chuck

	NPC* minipanzer = new NPC("", "MINIPANZER", "Vertically challenged criminal in possession of a tiny tank, very utile for robbing banks.", limbo, 0, Stats());
	//shell
	//roll over
	//supershell

	NPC* bagelfenagler = new NPC("", "BAGEL FENAGLER", "Hulking brute franknsteined into existence who will fenagle your bagels.\nIt wears a suit, and a sack over its head with a painted smiley face.", limbo, 0, Stats());
	//fenagle

	NPC* paveshark = new NPC("", "PAVEMENT SHARK", "Tough shark who stalks its prey through the streets of BURGERSBURG.", limbo, 0, Stats());
	//

	NPC* naturaldisaster = new NPC("", "NATURAL DISASTER", "Twister with an abnormally long lifespan and a collection of junk from all across BURGERSBURG.", limbo, 0, Stats());
	//opening: tailwind
	//thing fling
	//twister
	//rumble
	//split

	NPC* shadowcreature = new NPC("", "SHADOW CREATURE", "Pitch black humanoid who stalks the shadows of BURGERSBURG's citizens.", limbo, 0, Stats());

	NPC* grandma = new NPC("GRANDMA", "MARGE", "Maniacal grandma, mortal nemesis of Ratman.\nShe is singlehandedly responsible for 10% of BURGERSBURG's robberies.", limbo, 0, Stats());
	//She looks like a poor grandma getting beat up by Ratman.

	//axe
	//purse pow
	//slipper

	//Ratman is Batman MARK: Ratman (enemy)
	NPC* ratman = new NPC("", "RATMAN", "Rich vigilante wearing a dark rat suit and a cape, and a yellow utility belt.", limbo, 0, Stats(), Stats());
	//He's not the hero this city deserves, and he's not the hero this city needs, but he's the hero this city has.

	//ratarang

	//prep time


	NPC* richperson = new NPC("", "RICH PERSON", "A really rich BURGER shareholder who loves only his money.", limbo, 0, Stats(15, 1, 5, 0, 0, 9, 9));
	Attack* brassknuckles = new Attack("BRASS KNUCKLES", "swung at", true, -5, 20, 0, 1, 1, 1);
	brassknuckles->afterdesc = " with brass knuckles";
	Attack* chairthrow = new Attack("CHAIR", "threw a nearby chair at", false, 6, 40, 0, 1, 1, 1);
	Effect* ouchknees = new Effect("OUCH MY KNEES", 2, 0, 0, 1, 0.9f);
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
	Effect* enjoined = new Effect("ENJOINED", 1);
	enjoined->freeze = true;
	Attack* injunction = new Attack("INJUNCTION", "injuncted", false, 7, 0, 0, 1, 1, 1);
	injunction->afterdesc = "'s right to battle";
	injunction->addEffect(enjoined);
	burgerlawyer->setBasicAttack(briefcasebash);
	burgerlawyer->addSpecialAttack(legaldefense);
	burgerlawyer->addSpecialAttack(injunction);
	
	NPC* burgeragent = new NPC("", "BURGER AGENT", "Security guard of the BURGER corporation, dripped out in suit and sunglasses.", limbo, 0, Stats());
	//beatdown
	//
	//

	NPC* burgerbutler = new NPC("", "BURGER BUTLER", "Robot butler of the BURGER corporation. Looks like a big suited rice cooker on wheels.", limbo, 0, Stats(100, 50, 15, 30, 0, 0, 9));
	burgerbutler->setBoss(true); //miniboss
	//
	//
	//
	//
	
	NPC* ceo = new NPC("BURGER CEO", "ENZO", "The CEO of the whole BURGER COROPORATION.\nHe's rejected his humanity in favor of the lethal efficiency of machines.", limbo, 0, Stats(1000, 25, 25, 50, 50, 25, 9), Stats(0, 0, 1, 0, 1, 1, 1));
	ceo->setBoss(true);
	Effect* lockedon = new Effect("LOCKED ON", 4); //does nothing
	Attack* lockon = new Attack("LOCK ON", "locked onto", false, -5, 0, 0, 1, 1, 1);
	lockon->addEffect(lockedon);
	Effect* defensemode = new Effect("DEFENSE MODE", 2147483647, 1, 2.0f);
	Effect* assaultmode = new Effect("ASSAULT MODE", 2147483647, 2.0f, 1);
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
	ceo->setEffect(defensemode, false);
	ceo->setBasicAttack(lockon);
	ceo->addSpecialAttack(minigun);
	ceo->addSpecialAttack(flamethrower);
	ceo->addSpecialAttack(bombard);
	ceo->addSpecialAttack(megagun);

	//burger warden

	NPC* burgerscientist = new NPC("BURGER SCIENTIST", "", "Genius husk responsible for the BURGER personnel's augmentations and himself's.\nSlowly swapping his organs for mechanical parts, his true self is long dead.", limbo, 0, Stats());

	//burger man

	//entering temples requires having max size party
	//temples scale to your level
	//you can't exit until you beat the boss (or you use the give up exit which resets everything?)
	NPC* senseofself = new NPC("", "SENSE OF SELF", "", limbo, 0, Stats(), Stats());
	//temple of humility in the forest, gives output antenna of humility
	//forest temple is pretty standard, with some branching paths and combat and choices that allow you to buff yourself or buff your teammates
	//and probably a puzzle or two (prolly one)
	//sos starts by copying your attacks but better,
	//then tempts teammates into joining him (prize), effectiveness depends on how many times you helped yourself
	//you can use your attack APOLAGIZE TO to ask teammates to come back
	//near the end any remaining hypnotized teammates are sent back by sos and he tells you
	//hey it was all just to show you you can't rely on these ppl
	//join me and WE can defeat BURGER together!
	//if you join him he just instakills you after you defeat your other teammates
	//you can also REPENT if you haven't defeated them all yet
	//then you get the antenna

	NPC* thedark = new NPC("", "THE DARK", "", limbo, 0, Stats(), Stats());
	//temple of hope in the desert, gives big red button of hope
	//you get seperated from your teammates and you have to fight enemies that decay your stats
	//probably some hard puzzles
	//and you get reunited at the end
	//the dark decays your stats with its attacks
	//and when stats are low enough it can tempt teammates into despair
	//and you can ENCOURAGE teammates to unfreeze them (cause despair is just freeze)
	//after that you get the button

	NPC* firewithfire = new NPC("", "FIRE WITH FIRE", "", limbo, 0, Stats(), Stats());
	//temple of patience in the volcano area, gives plotometer of patience
	//you do puzzles and fight fire enemies
	//then you fight fire with fire
	//he says stuff that tempts you and your teammates into wrath
	//wrath gives x1.5 attack but wrath attacks also increase phase meter of fwf
	//you can CALM DOWN but you lose the boost
	//fwf increases in heat up to two times after getting wrathfully hit enough, and in the third phase all attacks add to his attack (so you can't just attack wrathfully with no consequences then)
	//teammates calm down naturally after a few turns (and increase wrath meter less), but you can CALM DOWN them so they calm down faster (5 by default probably)
	//then you get the plotometer

	//Attack* ATTACK = new Attack("NAME", "DESCRIPTION", DOESCONTACT, COST, POWER, PIERCE, MINHITS, MAXHITS, TARGETS);
	//Effect EFFECT = new Effect("NAME", duration, damage, spleak, attack, defense, tough, pierce);
	//NPC* npc = new NPC("TITLE", "NAME", "DESCRIPTION", limbo, hp, def, att, tou, pie, spe, ski);

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
	govguard->addLinkedConvo(govguard, {{govguard, "Well done, young one."}, {govguard, "You are a most impressive ninja."}, {govguard, "You've earned yourself access to our most secret ninja scroll."}, {self, "Oh thanks. :D"}});
	govguard->setTalkOnDefeat();
	Item* ninjascroll = new EducationItem("ADVANCED NINJA SCROLL", "An old golden scroll detailing advanced ninja techniques. Most ninjas never even see this scroll their whole lives.", limbo, smokebomb);
	EducationItem* _ninjascroll = (EducationItem*)ninjascroll;
	_ninjascroll->setAttack(pacupunctuken);
	_ninjascroll->setAttack(pdeadlyspinferno);
	govguard->setGift(ninjascroll, true);

	/*NPC* forestrando = new NPC(*grassman);
	forestrando->setLeader(true, 5, forestleft);
	forestrando->setDialogue("*angry bush noises*");
	forestrando->addRejectionDialogue("*angry bush noises*");*/

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

	/*NPC* plantguard = new NPC(*carnplant);
	plantguard->setLeader(true, 4, foresttempleentrance);
	plantguard->blockExit(NORTHWEST, ENEMY, "blocked by the CARNIVOROUS PLANT.");
	plantguard->setDialogue("*snapping biting noises*");
	plantguard->addRejectionDialogue("*snapping biting noises*");*/

	NPC* flowerguard = new NPC(*flowerfiend);
	flowerguard->setLeader(true, 3, flowerfield);
	flowerguard->blockExit(WEST, ENEMY, "blocked by the FLOWER FIEND.");
	flowerguard->setDialogue({{NULL, "FLOWER FIEND - *flowery shriek*"}});
	flowerguard->addRejectionDialogue({{NULL, "FLOWER FIEND - *flowery shriek*"}});

	NPC* mimic1 = new NPC(*mimic);
	mimic1->setLeader(true, 30, NULL);
	mimic1->setDialogue({{NULL, "MIMIC - *UNHINGED ROAR*"}}); //you see it's funny because chests have hinges
	mimic1->addRejectionDialogue({{NULL, "MIMIC - *UNHINGED ROAR*"}});
	mimic1->setEscapable(false);
	mimic1->setForceBattle();
	mimic1->setExtraMonies(1000); //you get lots of monies for beating the mimic
	mimic1->setBoss(true); //of the mini variety

	Item* fakechest = new TreasureItem("TREASURE CHEST", "A big treasure chest, possibly full of treasure.", treasuregrove, 0, NULL, mimic1);
	Item* rr2 = new ReviveItem(*(ReviveItem*)(reviveroot));
	Item* realchest = new TreasureItem("TREASURE CHEST", "A big treasure chest, possibly full of treasure.", treasurecliff, 300, rr2); //put a memory crowbar here?
	
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
	NPC* florian = new NPC(*tunnellobster);
	florian->setLobster(tunnels);
	florian->setLeader(true, 10, desertstation, false);
	florian->setTunnelDirection(tentstation, TO_THE_VILLAGE);
	florian->setTunnelDirection(desertstation, TO_THE_DESERT);
	florian->setTunnelDirection(deserttunnel, TO_THE_DESERT);
	florian->setTunnelDirection(volcanostation, TO_THE_HIGHLANDS);
	florian->setTunnelDirection(burgstation, TO_BURGERSBURG);
	florian->setTunnelDirection(basestation, TO_THE_BASEMENT);
	florian->setDialogue("HHhHHHhhHHhHhhHhHHhHhHHh (angry lobster noises).");
	florian->addRejectionDialogue("HhhHhHhHhhhhHHhHHh (lobster noises probably meaning no).");
	florian->addLinkedDesc(florian, "Your big pet crustacean who inhabits the tunnels below.");
	florian->addLinkedDialogue(florian, {{florian, "HHhhHhHHhHhHhHHHhHHhhHhh (happy lobster noises)."}});

	Item* lobstercaller = new CallerItem("LOBSTER WHISTLE", "Used for summoning lobsters by playing a lobstery melody.", limbo, florian);
	desertstation->setBackup(lobstercaller);
	Item* backupcaller1 = new CallerItem("LOBSTER WHISTLE", "Used for summoning lobsters by playing a lobstery melody.", limbo, florian);
	volcanostation->setBackup(backupcaller1);
	Item* backupcaller2 = new CallerItem("LOBSTER WHISTLE", "Used for summoning lobsters by playing a lobstery melody.", limbo, florian);
	burgstation->setBackup(backupcaller2);
	Item* backupcaller3 = new CallerItem("LOBSTER WHISTLE", "Used for summoning lobsters by playing a lobstery melody.", limbo, florian);
	basestation->setBackup(backupcaller3);

	//set up teammate viola MARK: Viola
	NPC* viola = new NPC(*tkviola);
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
	viola->setWorldCondition(VIOLADEF);

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
	fact3guard3->setParty({slagman, poizard, poizar});
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
	mimic1->setLeader(true, 50, NULL);
	mimic1->setDialogue({{NULL, "MIMIC - *UNHINGED ROAR*"}});
	mimic1->addRejectionDialogue({{NULL, "MIMIC - *UNHINGED ROAR*"}});
	mimic1->setEscapable(false);
	mimic1->setForceBattle();
	mimic1->setExtraMonies(1500); //you get lots of monies for beating the mimic
	mimic1->setBoss(true); //mini

	NPC* mimic3 = new NPC(*mimic);
	mimic1->setLeader(true, 50, NULL);
	mimic1->setDialogue({{NULL, "MIMIC - *UNHINGED ROAR*"}});
	mimic1->addRejectionDialogue({{NULL, "MIMIC - *UNHINGED ROAR*"}});
	mimic1->setEscapable(false);
	mimic1->setForceBattle();
	mimic1->setExtraMonies(1500); //you get lots of monies for beating the mimic
	mimic1->setBoss(true); //mini

	Item* fakechest1 = new TreasureItem("TREASURE CHEST 1", "A big treasure chest, possibly full of treasure.", sewertreasure, 0, NULL, mimic2);
	Item* fakechest2 = new TreasureItem("TREASURE CHEST 2", "A big treasure chest, possibly full of treasure.", sewertreasure, 0, NULL, mimic3);
	Item* rr3 = new ReviveItem(*(ReviveItem*)(reviveroot));
	Item* realchest2 = new TreasureItem("TREASURE CHEST 3", "A big treasure chest, possibly full of treasure.", sewertreasure, 500, rr3); //put a memory crowbar here?

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
	lavaguard->addRejectionDialogue("*ethereal breathing*");
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
	
	NPC* evilgrandma = new NPC(*grandma);
	evilgrandma->setLeader(true, 22, rightstreet3, false);

	NPC* theratman = new NPC(*ratman);
	theratman->setLeader(true, 22, rightstreet3);
	
	//keycard
	NPC* richpeople = new NPC(*richperson); //RICH PEOPLE GAUNTLET!
	richpeople->setLeader(true, 23, ballroom, false);
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
	richpeople->addConversation({{self, "HEY!"}, {NULL, "RICH PEOPLE - *indistinct chatter*"}, {self, "GUYS!"}, {NULL, "You jump onto a table and grab a glass and a fork."}, {NULL, "You clink the fork against the glass a few times."}, {NULL, "The room goes silent."}, {richperson, "Who the heck are you?"}, {self, "I'm saving this kid you guys kidnapped. >:("}, {self, "So..."}, {self, "(wait what am I saying exactly?)"}, {self, "Could one of you guys direct me towards where you kidnap people to?"}, {self, "..."}, {richpeople, "..."}, {richperson, "LET'S GET HIM!"}});
	richpeople->setDialogue({{self, "I'm gonna save that kid you guys kidnapped >:("}, {richperson, "LET'S GET HIM!"}});
	richpeople->setRejectionDialogue({{NULL, "Like, are you trying to recruit all of them at once?"}, {NULL, "That's not gonna work. :|"}});
	richpeople->setForceBattle();
	richpeople->addLinkedRoom({ballroom, "in the BURGER ballroom, full of incapacitated rich people."});

	//richpeople->setGift(keycard, true);

	NPC* enzo = new NPC(*ceo);
	enzo->setMask("BURGER CEO", "ENZO", "The CEO of the whole BURGER COROPORATION, sitting ominously behind his desk.");
	enzo->setLeader(true, 30, ceoroom, false);
	ceo->blockExit(IN_SAFE, ENEMY, "guarded by the BURGER CEO.");
	enzo->addConversation({{self, "Why'd you guys kidnap that kid? >:("},
						   {ceo, "So that's what you busted into this joint for?"},
						   {ceo, "Over some kid? Hah hah hah hah!"},
						   {self, "Answer my question! >:("},
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
	enzo->addLinkedConvo({enzo, {{NULL, "Enzo breaks down into pieces."},
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
								{self, "O_O"}}});
	enzo->setTalkOnDefeat();
	ceo->setDialogue({{ceo, "You got a screw loose if ya think you can beat me!"}, {ceo, "HAH HAH HAH HAH!"}});
	ceo->setRejectionDialogue("You're asking me to join ya? Are ya crazy?");
	ceo->addLinkedRoom({ceoroom, "in the BURGER CEO's office, now missing the roof and walls.\nThe BURGER SAFE is still intact."});
	ceo->addLinkedRoom({ballroom, "in the BURGER ballroom. The action has died down and the room is pretty quiet."});
	ceo->addLinkedRoom({ceolobby, "in the ornate lobby of the building, featuring both an elevator and fancy stairs.\nThe yearly RICH PEOPLE reunion has concluded."});
	ceo->addLinkedRoom({ceolobby2, "on the second floor of the building, still the lobby."});
	ceo->addLinkedRoom({richneighborhood4, "at the doorway of the BURGER corporation's headquarters."});
	ceo->setForceBattle();
	ceo->setEscapable(false);
	ceo->setWorldCondition(BEATCEO);

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
	sewercenter3->blockExit(DOWN, RUBBLE, "blocked by the ground. It looks pretty fragile and rubbley, maybe don't stand on it.");
	sewercenter4->blockExit(WEST, LOCK, "locked with a GREEN LOCK.");
	volcano2->blockExit(NORTH, LAVA, "covered by a sea of lava. Looks like it's THE END OF THE ROAD!");
	volcano4->blockExit(NORTHEAST, LAVA, "covered by a sea of lava. Looks like it's THE END OF THE ROAD!");
	volcano6->blockExit(NORTHWEST, LAVA, "covered by a sea of lava. Looks like it's THE END OF THE ROAD!");
	volcano7->blockExit(NORTH, LAVA, "covered by a sea of lava. Looks like it's THE END OF THE ROAD!");
	sewerentrance1->blockExit(DOWN, LAVA, "submerged in lava.");
	sewerentrance2->blockExit(DOWN, LAVA, "submerged in lava.");
	sewer2->blockExit(UP, COVER, "covered by a manhole cover."); //these two must be unblocked by taking the manhole cover from above (because the cover perfectly fits the hole, so logically you can't take it from below)
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
	richneighborhood1->blockExit(NORTHEAST, TEMPLE, "guarded by high-tech security systems.");
	richneighborhood2->blockExit(NORTH, TEMPLE, "guarded by high-tech security systems.");
	richneighborhood3->blockExit(NORTHWEST, TEMPLE, "guarded by high-tech security systems.");

	return self; //returns the player character
}

//prints all the properties of the given room MARK: print room data
void PrintRoomData(Room* currentRoom) {
	currentRoom->printWelcome(); //some rooms have messages they print on arrival
	cout << "\nYou are " << currentRoom->getDescription();
	currentRoom->printExits();
	currentRoom->printNPCs();
	currentRoom->printItems();
	currentRoom->printStock(); //prints buyable items in this room
	currentRoom->printBlocks(); //prints which exits are blocked
}

//move the player and co. to a new room based on direction, or also just teleopring to forceDest if given MARK: travel
void travel(Room*& currentRoom, const char* direction, vector<NPC*>* party, vector<Item*>* inventory, bool forceTravel = false, Room* forceDest = NULL) {
	Room* roomCanidate = NULL; //the room we're trying to go to
	if (forceDest != NULL) { //we just teleoprt to this room if given
		roomCanidate = forceDest;
	} else { //if no teleport destination is given, we try to get the room in the given direction
		roomCanidate = currentRoom->getExit(direction);
	}
	//we print an error message based on the reason roomCanidate is NULL
	if (roomCanidate == NULL) {
		//if the player gave a direction that doesn't match the commonly given ones, we call it an invalid direction
		if (strcmp(direction, "NORTH") && strcmp(direction, "SOUTH") && strcmp(direction, "EAST") && strcmp(direction, "WEST") && strcmp(direction, "NORTHEAST") && strcmp(direction, "NORTHWEST") && strcmp(direction, "SOUTHEAST") && strcmp(direction, "SOUTHWEST") && strcmp(direction, "UP") && strcmp(direction, "DOWN")) {
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
	//we move our party to the room
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
			}
		}
	}
	Item* roomgift = roomCanidate->popBackup(); //check if the item hasa gift
	if (roomgift && !getItemInVector(*inventory, roomgift->getName())) { //if gift exists and we don't already have the item from elsewhere
		roomCanidate->setItem(roomgift); //put the item in the room
	}
	//we move ourself to the next room
	currentRoom = roomCanidate;
	PrintRoomData(currentRoom); //prints the data of the current room
}

//initiates battle with an npc MARK: fight
void fight(Room* currentRoom, vector<NPC*>* party, vector<Item*>* inventory, const char* name, int& mony) {
	NPC* npc = getNPCInVector(currentRoom->getNpcs(), name); //try to find the given npc in the room
	if (npc == NULL) { //try to find npc in adjacent exits
		npc = getNPCInVector(currentRoom->getNpcs(true), name);
	}
	if (npc == NULL || npc->getDefeated()) { //print error message if they're not here (or defeated and not supposed to be there technically)
		cout << "\nThere is nobody named \"" << name << "\" here.";
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
		npc->printDialogue();
		cout << "\n";
	} //print any dialogue that is specifically printed right before battle
	npc->printOpeningDialogue();
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
		cout << "YOU WIN!";
		CinPause();
		mony += battle.getMonyReward(); //adds the monies to the player's balance
		//prints how much monies were earned and the new current total. I don't care about grammar here because the reward is literally never just 1
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
		if (npc->getTalkOnDefeat()) npc->printDialogue(); //print defeat dialogue
		if (Item* item = npc->takeGift()) { //if the guy has a gift to give upon defeat we get the gift and add it to the inventory
			inventory->push_back(item);
			cout << name << " gave you the " << item->getName() << "!"; //says that you got the thing
			CinPause();
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
				cout << "SELF - \"Ok nevermind I guess I won't name you.\"";
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
			
			CinIgnoreAll(); //clear extra text and potential error flags

			npc->undefeat(); //make sure the lobster isn't defeated after the battle so you can try using it
			npc->setLeader(false); //make it not a leader so you can't fight it and so you can use it
		}
	}
	//prints the room data after battle so that the player can reorient themselves
	PrintRoomData(currentRoom);
}

//takes an item from the current room and adds it to the inventory MARK: take item
void takeItem(Room* currentRoom, vector<Item*>* inventory, const char* itemname, NPC* player) { //I don't want to have to pass player here but we need to teach player attacks when taking weapon items
	//finds the item in the room based on the name
	Item* item = getItemInVector(currentRoom->getItems(), itemname);
	//prints the reason you can't take the item based on the circumstances
	if (item == NULL) {
		//if the player tried to take an item on sale we say you can't steal
		if (getItemInVector(currentRoom->getStock(), itemname) != NULL) {
			cout << "\nThe " << itemname << " is for sale! You can't just take it.";
			return;
		//you can't take an item if you already took it
		} else if (getItemInVector(*inventory, itemname) != NULL) {
			cout << "\nYou're already carrying this item!";
			return;
		}
		//otherwise the player is trying to take something that isn't even there
		cout << "\nThere is no \"" << itemname << "\" here.";
		return;
	} //you're not allowed to take items if they're being guarded
	if (NPC* guard = item->getGuard()) {
		cout << "The " << itemname << " is being guarded by " << guard->getName() << ".";
		return;
	}
	//you're not allowed to take items not marked as takable
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
		WeaponItem* weapon = (WeaponItem*)item;
		Attack* attack = weapon->getAttack();
		player->addSpecialAttack(attack);
		cout << player->getName() << " can now use " << attack->name << "!\n" << attack->name << " - " << attack->trueDesc;
		CinPause();
	} else if (!strcmp(item->getType(), "worldchange")) {
		WorldChangeItem* changer = (WorldChangeItem*)item;
		if (!changer->getTakeToUse()) return;
		applyWorldChange(changer->getChanges()); //do the changes
		printConversation(&changer->getUseText(), false); //prints what the player did and what it accomplished
		deleteItem(currentRoom, inventory, item); //we shouldn't keep world change items because the taking was the using and they're consumables
	}
}

//drops an item from the inventory into the current room MARK: drop item
void dropItem(Room* currentRoom, vector<Item*>* inventory, const char* itemname, NPC* player) { //pass player for the same reason as takeItem
	Item* item = getItemInVector(*inventory, itemname); //finds the item in the inventory
	if (item == NULL) { //gives error message if we have no itemname
		cout << "\nYou have no \"" << itemname << "\"."; //I know ". is grammatically inaccurate but it looks way better than ."
		return;
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
	}
}

//uses an item, with functionality based on type MARK: use item
void useItem(Room*& currentRoom, vector<Item*>* inventory, vector<NPC*>* party, const char* itemname, int& mony) {
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
				return;
			} else if (npc == NULL) { //print if no npc matching the name was found, also since we're not in battle you can only use targeted items on your party
				cout << "\nThere is nobody named \"" << npcName << "\" in your party!";
				return;
			}
		//if the item IS null and the name of the item isn't nothing (player didn't "USE " or "USE" <-- like that's the command we're checking for)
		} else if (strcmp(itemName, "")) {
			itemname = itemName; //makes the null item print not say "You have no [item] ON [npc]"
		}
	}
	if (item == NULL) { //print that no item called itemname was found
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
		if (!AOrB("Are you sure?", "YES", "NO")) {
			return;
		}
		cout << "\nYou ate the BURGER...";
		CinPause();
		cout << "It tasted alright.";
		CinPause();
		cout << "\n\t<<<      BURGER QUEST COMPLETE ?      >>>";
		cout << "\n\t<<< ENDING ACHIEVED: TEST DEMO ENDING >>>"; //isn't it a TECH demo?
		CinPause();
		cout << "\nThank you for playing BURGER QUEST 2: ELECTRIC BOOGALOO!";
		if (!AOrB("Would you like to keep playing?", "YES", "NO")) {
			cout << "\nAlright then cya!\n";
			exit(0);
		}
		BURGERItem* boiga = (BURGERItem*)item; //get burger as burgeritem to get the elevator direction
		travel(currentRoom, boiga->getDirection(), party, inventory, true); //leave to the elevator so the player can keep playing
	//teaches the player character new attacks
	} else if (!strcmp(item->getType(), "education")) {
		EducationItem* edu = (EducationItem*)item; //converts to the corresponding subclass
		bool learned = false; //if we learned something which we can use immediately given our current level
		NPC* player = (*party)[0];
		cout << "\n";
		//adds all the item's attacks to the player's vector of attacks
		for (Attack* attack : edu->getAttacks()) {
			player->addSpecialAttack(attack);
			if (attack->minLevel <= player->getLevel()) { //print the description and name of any attacks we have just learned
				cout << player->getName() << " learned " << attack->name << "!\n" << attack->name << " - " << attack->trueDesc;
				learned = true; //we have, in fact, learned an attack just now
				CinPause();
			}
		}
		if (!learned) { //prints motivational message if none of these attacks are usable yet
			cout << "You're not LEVELED UP enough to use any of these new attacks.\nKeep training and you'll get there eventually!";
		}
	//summons the tunnel lobster to the current train station
	} else if (!strcmp(item->getType(), "caller")) {
		if (!currentRoom->getStation()) { //gives error if not used in a train station
			cout << "\nThe " << itemname << " must be used in a train station!";
			return;
		} //converts to the corresponding subclass
		CallerItem* caller = (CallerItem*)item;
		npc = caller->getCalledNPC(); //makes the npc easier to reference
		if (!npc->getLeader()) { //if the lobster is tamed
			if (npc->getRoom() == currentRoom) { //if the lobster is already here he just dances
				cout << "\n" << npc->getName() << " did a lobstery dance.";
				return;
			} //moves the lobster to the station
			npc->setRoom(currentRoom);
			cout << "\n" << npc->getName() << " burst out of the rubble!"; //flavor text
		} else { //if the lobster is still untamed (undefeated)
			if (npc->getRoom() == currentRoom) { //if the lobster is already here prints flavor text
				cout << "\n" << npc->getName() << " shrieked angrily!";
			} else { //moves the lobster here if not here
				npc->setRoom(currentRoom);
				cout << "\nA " << npc->getName() << " angrily burst out of the rubble!";
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
	} else if (!strcmp(item->getType(), "key")) {
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
	//treasure chest items either give money or are trapped and start a battle
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
			if (Item* newitem = treasure->getItem()) { //if the treasure had an item
				cout << "\nYou ";
				if (treasure->getMony()) cout << "also ";
				newitem->unRoom(); //removes the item from the room
				inventory->push_back(newitem); //adds it to the inventory
				cout << "got a " << newitem->getName() << "!";
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
		cout << "\nYou got the " << product->getName() << "!";
	//you can't use materials; they get a unique error message
	} else if (!strcmp(item->getType(), "material")) {
		cout << "\nYou can't use the " << itemname << "!";
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
	if (npc == NULL) { //error message if nobody in the current room is named npcname
		cout << "\nThere is nobody named \"" << npcname << "\" here.";
		return;
	} //you can't recruit yourself because you're obviously in your own party
	if (npc->getPlayerness()) {
		cout << "\n" << npcname << " - \"Huh?\"\n\nYou are already in your own party? ...";
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
	} //you're not allowed to have more than 4 party members (including yourself) otherwise that would be very unbalanced
	if (party->size() == maxParty) {
		cout << "\nYour party is full!";
		return;
	}
	//adds the npc to your party
	party->push_back(npc);
	npc->printRecruitmentDialogue(); //print the recruitment dialogue
	npc->Recruit(); //sets the npc to recruited
	cout << "\n" << npcname << " was added to your party! (Party size: " << party->size() << "/" << maxParty << ")"; //prints success text
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
		cout << "\nThere is nobody named \"" << npcname << "\" in your party.";
		return;
	} //you can't dismiss yourself/the main character because that makes no sense
	if (npc->getPlayerness()) {
		cout << "\n" << npcname << " - \"Huh? You can't dismiss me bro I'm the main character!\"\n" << npcname << " was not dismissed.";
		return;
	} //you can't dismiss someone who isn't recruited anyway
	if (!npc->getRecruited()) {
		cout << "\n" << npcname << " is not in your party...";
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
		cout << "\n" << npcname << " was removed from your party and returned to what they were doing before.";
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
	if (npc == NULL) { //error message if no such npc is in the current room
		cout << "\nThere is nobody named \"" << npcname << "\" here.";
		return;
	} //tells the npc to print their dialogue
	npc->printDialogue();
	//some npcs give gifts after talking so we check for that here
	Item* item = npc->takeGift();
	if (item != NULL) { //adds the gift to the inventory
		inventory->push_back(item);
		CinPause();
		cout << npcname << " gave you the " << item->getName() << "!"; //says that you got the thing
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
	if (npc != NULL) { //prints the data of the npc that was found
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
	} //error message for invalid name
	cout << "\nThere is no item or person named \"" << name << "\" here.";
}

//buys an item from the current room's catalogue //MARK: buy
void buy(Room* currentRoom, vector<Item*>* inventory, const char* name, int& mony) {
	Item* item = getItemInVector(currentRoom->getStock(), name); //finds the item in the current room's stock
	if (item == NULL) { //gives error message based on other conditions
		if (getItemInVector(currentRoom->getItems(), name) != NULL) { //if the item isn't for sale and it's just on the ground or something
			cout << "\nNobody is selling the " << name << "; you can just take it.";
			return;
		} else if (getItemInVector(*inventory, name) != NULL) { //if you're trying to buy your own item which you already own and are holding
			cout << "\nYou already own this item!";
			return;
		} //error message if no item found of the given name
		cout << "\nThere is no \"" << name << "\" here.";
		return;
	} //buys the item, subtracts the cost from the player's mony, and adds it to the inventory
	item->buy(mony, inventory);
	if (item->getStock() <= 0) { //if we bought all of the item, we delete it from the store
		currentRoom->removeStock(item);
	}
}

//prints all the available commands MARK: help
void printHelp(const char* validCommands[16], const char* flavorText[16]) {
	cout << "\n"; //prints a random flavor text
	cout << flavorText[rand() % 16];
	cout << "\nValid commands:"; //prints all the valid commands
	for (int i = 0; i < 16; i++) {
		cout << "\n" << validCommands[i];
	}
}

//the main function where everything is called MARK: main
int main() {
	cout << "\nBURGER QUEST 2:"
			"\nELECTRIC BOOGALOO"
			"\n"
			"\nVersion 1.0"
			"\n(c) 2026 Tomas Carranza Echaniz"
			"\n"
			"\nPaste existing save data, or press ENTER to begin a new quest!"
			"\n> " << fixed << setprecision(2);
	
	srand(time(NULL)); //seeds random
		
	//sets up the game world and places the player at the current room
	NPC* self = SetupWorld();
	Room* currentRoom = self->getHome(); //sets the current room to the player's starting position

	vector<Item*>* inventory = new vector<Item*>; //the inventory of items
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
	const char* validCommands[16] = {
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
		"QUIT"
	};

	//welcome message
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

	PrintRoomData(currentRoom); //prints the data of the starting room

	bool continuing = true; //we continue until continuing is set to false (when the player quits)
	while (continuing) { //the main loop!
		char command[255] = ""; //the charray that the player inputs into

		char commandWord[255]; //the first word of the player input (the command)
		char commandExtension[255]; //the rest of the player's command (minus the space)

		cout << "\n> "; //The > signifies it's time to type in a command. If there is no >, it's a cutscene or dialogue or something like that and you just have to ENTER until you get to the >.
		cin.getline(command, 255);
		AllCaps(command); //capitalizes the command for easier parsing

		ParseCommand(command, commandWord, commandExtension); //seperates the command into the command and the extension

		if (currentRoom->getGym()) { //if we're in a gym, update all the traning teammates' levels before doing the action
			currentRoom->scaleNPCs(self->getLevel()-1);
		}

		if (!strcmp(commandWord, "GO")) { //for going in a direction
			travel(currentRoom, &commandExtension[0], party, inventory);
		} else if (!strcmp(commandWord, "TAKE")) { //for taking an item
			takeItem(currentRoom, inventory, &commandExtension[0], self);
		} else if (!strcmp(commandWord, "DROP")) { //for dropping an item
			dropItem(currentRoom, inventory, &commandExtension[0], self);
		} else if (!strcmp(commandWord, "USE")) { //for using an item (or tunnel lobster)
			useItem(currentRoom, inventory, party, &commandExtension[0], mony);
		} else if (!strcmp(commandWord, "RECRUIT")) { //for recruiting npcs
			recruitNPC(currentRoom, &commandExtension[0], party);
		} else if (!strcmp(commandWord, "DISMISS")) { //for dismissing npcs
			dismissNPC(currentRoom, &commandExtension[0], party);
		} else if (!strcmp(commandWord, "ASK")) { //for getting the dialogue of npcs
			printNPCDialogue(currentRoom, &commandExtension[0], inventory, party, mony);
		} else if (!strcmp(commandWord, "INVENTORY")) { //for printing the contents of your inventory and your monies
			printInventory(inventory, mony);
		} else if (!strcmp(commandWord, "PARTY")) { //for printing everyone in your party and their level
			printParty(party);
		} else if (!strcmp(commandWord, "ATTACKS")) { //for printing the player's attacks
			printAttacks(self);
		} else if (!strcmp(commandWord, "ROOM")) { //for reprinting the contents of the current room (it was annoying having to scroll back up after doing a bunch of stuff in order to check the room data)
			PrintRoomData(currentRoom);
		} else if (!strcmp(commandWord, "ANALYZE")) { //for getting the data of an item or npc
			analyze(currentRoom, &commandExtension[0], party, inventory);
		} else if (!strcmp(commandWord, "FIGHT")) { //for initiating battle with npcs
			fight(currentRoom, party, inventory, &commandExtension[0], mony);
		} else if (!strcmp(commandWord, "BUY")) { //for buying items for sale
			buy(currentRoom, inventory, &commandExtension[0], mony);
		} else if (!strcmp(commandWord, "HELP")) { //for getting a list of valid commands
			printHelp(validCommands, flavorText);
		} else if (!strcmp(commandWord, "QUIT")) { //for quitting the game
			continuing = false;
		} else { //prints an error message if the player typed something that isn't an actual command //if (strcmp(commandWord, "")) 
			cout << "\nInvalid command \"" << commandWord << "\" (type HELP for help).";
		}

		CinIgnoreAll(); //clears extra or faulty input
	}
	
	//gives a friendly farewell to the player
	cout << "\nCya!\n";
	
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
	delete inventory;
}
