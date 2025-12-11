/* Tomas Carranza Echaniz
*  12/5/25
*  This program is a text-based game where you can GO between rooms, TAKE, DROP, and USE items, ASK, RECRUIT,
*  DISMISS, and FIGHT npcs, and you're on a QUEST TO get a BURGER. You can eat the BURGER to get the bad ending,
*  or you can explore more to get the good ending. There's a bunch of other commands as well.
*  
*  WALKTHOUGH AS OF NOW:
*  go right fight the grassman, then go northwest then west, talk to mr deer to get the key, then go north until you find the gate
*  then use the deer key, then just keep going north until you get to the canyon, then go in the mines and keep going north as much
*  as you can, then leave the mines, and then keep going north until you reach the burger restaurant and then buy a burger and use
*  it and then you win yay. Although, in a few days it'll be much better so please play it then instead if possible.
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
using namespace Helper; //my Helper namespace has a bunch of helpful functions that I also use in other files

//sets up the entire game world, including rooms, npcs, and items, and returns the player character
NPC* SetupWorld() {
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
	char* IN_ALLEY = new char[12];
	char* IN_ELEVATOR = new char[12];
	char* IN_TEMPLE = new char[12];
	char* INSIDE = new char[12];
	char* UPSTAIRS = new char[12];
	char* DOWNSTAIRS = new char[12];
	char* UNDERGROUND = new char[12];
	char* FORWARD = new char[12];
	//fast travel directions
	char* TO_THE_VILLAGE = new char[20];
	char* TO_THE_DESERT = new char[20];
	char* TO_THE_HIGHLANDS = new char[20];
	char* TO_BURGERSBURG = new char[20];
	char* TO_THE_BASEMENT = new char[20];

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
	strcpy(IN_ALLEY, "IN ALLEY");
	strcpy(IN_ELEVATOR, "IN ELEVATOR");
	strcpy(IN_TEMPLE, "IN TEMPLE");
	strcpy(INSIDE, "INSIDE");
	strcpy(UPSTAIRS, "UPSTAIRS");
	strcpy(DOWNSTAIRS, "DOWNSTAIRS");
	strcpy(UNDERGROUND, "UNDERGROUND");
	strcpy(FORWARD, "FORWARD");
	strcpy(TO_THE_VILLAGE, "TO THE VILLAGE");
	strcpy(TO_THE_DESERT, "TO THE DESERT");
	strcpy(TO_THE_HIGHLANDS, "TO THE HIGHLANDS");
	strcpy(TO_BURGERSBURG, "TO BURGERSBURG");
	strcpy(TO_THE_BASEMENT, "TO THE BASEMENT");

	//set the direction reverser map in Helper
	ReverseDirection[NORTH] = SOUTH;
	ReverseDirection[SOUTH] = NORTH;
	ReverseDirection[EAST] = WEST;
	ReverseDirection[WEST] = EAST;
	ReverseDirection[UP] = DOWN;
	ReverseDirection[DOWN] = UP;
	ReverseDirection[OUT] = UNDERGROUND;
	ReverseDirection[UNDERGROUND] = OUT;
	ReverseDirection[NORTHEAST] = SOUTHWEST;
	ReverseDirection[NORTHWEST] = SOUTHEAST;
	ReverseDirection[SOUTHEAST] = NORTHWEST;
	ReverseDirection[SOUTHWEST] = NORTHEAST;

	//set up blockage reaons
	char* ENEMY = new char[12];
	char* CHASM = new char[12];
	char* RIVER = new char[12];
	char* TEMPLE = new char[12];
	char* RUBBLE = new char[12];
	char* TUNNEL = new char[12];
	char* TRACK = new char[12];
	char* LOCK = new char[12];
	char* MISC = new char[12];
	
	//set up blockage reason text
	strcpy(ENEMY, "ENEMY");
	strcpy(CHASM, "CHASM");
	strcpy(RIVER, "RIVER");
	strcpy(TEMPLE, "TEMPLE");
	strcpy(RUBBLE, "RUBBLE");
	strcpy(TUNNEL, "TUNNEL");
	strcpy(TRACK, "TRACK");
	strcpy(LOCK, "LOCK");
	strcpy(MISC, "MISC");

	//I send all the template enemy NPCs and also shop items to limbo, since I need to set a room for them
	Room* limbo = new Room("not supposed to be in this room; seriously how did you get here?");

	//create all WANING WOODLANDS rooms
	Room* village = new Room("in Tactical Tent Village. It's a beautiful day; perfect for staying indoors and gaming.");
	Room* villageleft = new Room("at the westernmost end of the village, where the tallest tent stands. It's only two stories, but it's comparatively a tent mansion.");
	Room* tentstore = new Room("in the village convenience store. No other store is more convenient, or so they say.");
	Room* tentmansion = new Room("in the tent mansion's living room. There are way too many clocks here.");
	Room* tentlab = new Room("in the tent lab. There's a ton of machinery, and many generic science beakers with colored liquids.");
	Room* tentstation = new Room("in the village train station. The tunnels were closed off recently due to a lobster infestation.");
	tentstation->setStation();
	Room* tentchurch = new Room("in the village church. It's a really big tent, complete with stained glass and everything. Nobody really goes here anymore, especially since the priest mysteriously disappeared a long time ago.");
	Room* docks = new Room("at the village docks. Your tent house is set up here because your dad likes fishing.");
	Room* tenthouse = new Room("in your family's house, in what could be considered the multiuse bed/dining/living room.");
	Room* forestentrance = new Room("at the entrance of the woodlands.");
	Room* forest = new Room("deep in the woodlands. Smells like pine trees along the way.");
	forest->setWelcome("Welcome to WANING WOODLANDS!");
	forest->setWelcome("<<< THE FINAL FOREST >>>");
	forest->setWelcome("The slowly decaying corner of the world, where you have lived your whole life.");
	forest->setWelcome("The grass is green and the chirds are birping.");
	forest->setWelcome("Now, your BURGER QUEST begins!");
	Room* forestleft = new Room("walking down the left path of the woodlands. It feels like a deer is watching you.");
	Room* forestright = new Room("walking down the right path of the woodlands. It feels like a ninja is watching you.");
	Room* deerclearing = new Room("in the deer clearing, where deer frequently gather."); //talk to the deer to get the key
	Room* ninjaland = new Room("underneath the ninja village. It's probably supposed to be hidden but you looked up and there it was.");
	Room* ninjavillage = new Room("in the ninja village. There's no bridges connecting the houses, you must simply jump between them like a true ninja.");
	Room* ninjacapitol = new Room("in the chief ninja's abode. There are many weapons and scrolls up on the walls.");
	Room* ninjapantry = new Room("in the ninja storage unit. The ninjas live on a strict diet of NINJABERRIES and ninjasteak and ninjafish and the diet isn't actually that strict.");
	Room* ninjaforge = new Room("in the ninja forge. There are many molds for making weapons here.");
	Room* foresttempleentrance = new Room("in the glade where the ancient forest temple stands.");
	Room* foresttemplestairs = new Room("on the steps that go into the ancient forest temple.");
	Room* foresttemple = new Room("in the temple of [SOMETHING]. [SOMETHING SOMETHING].");
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
	Room* treasurecliff = new Room("in the treasure cliff at the edge of the desert, where there is actualy treasure.");
	Room* bossgrove = new Room("in the BOSS GROVE, where the boss of WANING WOODLANDS is known to appear.");
	Room* fdintermission1 = new Room("on the path between the woodlands and the wastelands.");
	Room* fdintermission2 = new Room("on the path between the woodlands and the wastelands. The foliage is sparse here. BURGERSBURG can be seen faintly in the distance.");
	Room* fdintermission3 = new Room("on the path between the woodlands and the wastelands. Dead trees surround you. The BURGER RESTAURANT is just barely visible from here.");
	//Create all DESOLATE DESERT rooms
	Room* desert = new Room("in the wastelands. There is no sign of life anywhere (except you!).");
	desert->setWelcome("Welcome to DESOLATE DESERT!");
	desert->setWelcome("<<< THE WASTELANDS BEYOND >>>");
	desert->setWelcome("The world beyond your forest, where the life has been sucked out of the dirt.");
	desert->setWelcome("The sun beats down with no clouds in sight.");
	desert->setWelcome("Surely there must be someone friendly around here?");
	Room* deserttempleentrance = new Room("on a large dune where an ancient desert temple pokes out of the sand. I'm sure nobody's ever been in this one, either.");
	Room* deserttemplestairs = new Room("on the steps that go into the ancient desert temple.");
	//temple stuff
	Room* desertdune = new Room("at a low point in the desert. A rare shadow is present where you can rest.");
	Room* desertplain = new Room("at a really flat area of the desert.");
	Room* desertgrave = new Room("deep in the desert. There's some sort of bovine skeleton here.");
	Room* desertpole = new Room("deep in the desert. All you can see for miles is sand.");
	Room* deserthill = new Room("on a very high dune. It'd be fun to roll all the way down, but your hair would be very annoying to clean afterwards.");
	Room* deserttown = new Room("in the abandoned desert village. It's strange; where did everyone go?");
	Room* desertshop = new Room("in an abandoned store. It would feel cozy if not for the smell of the expired products.");
	Room* desertgym = new Room("in an abandoned gym. There's all the equipment you could ask for here, but no rock wall.");
	Room* deserthouse = new Room("in some house with zero purpose. It only exists because the town would feel weird with only two houses.");
	//the desert town gets fixed after defeating VIOLA, but rather than reset all the descriptions manually I just make the abandoned town redirect you to the fixed one
	Room* deserttownfixed = new Room("in the repopulated desert village. It's very lively here; looks like it's right back to business as usual.");
	Room* desertshopfixed = new Room("in the desert store. It smells like pastries and medicine.");
	Room* desertgymfixed = new Room("in the desert gym. Now that it's back in business it's time to get on that GRIND!\nSimply DISMISS your teammates here to make them stay in shape until you come back!");
	desertgymfixed->setGym();
	Room* deserthousefixed = new Room("in some house which still has zero purpose."); //I have no idea what to put in this house
	Room* oasis = new Room("in an oasis, presumably the town's source of water, though the water is long gone and the plants are dry husks.");
	Room* oasisfixed = new Room("in the town oasis, now fully restored! Some signs of greenery are starting to appear."); //oasis gets fixed after beating the BURGER GLUTTON underground
	Room* canyon = new Room("in a small canyon that cuts into a plateau. There's an old-timey entrance to a train station here.");
	Room* thatcliff = new Room("on this cliff over here."); //VIOLA goes here after being defeated
	Room* canyon1 = new Room("in a long, shaded canyon, forming YET ANOTHER fork in the road. You know, these would actually be really annoying forks if you think about it.");
	Room* canyon2 = new Room("at the end of the canyon, though a path seems to have been made upwards.");
	Room* canyon3 = new Room("at the end of the canyon. There's much evidence of mining here, and a mineshaft entrance here leads underground.");
	Room* cliff1 = new Room("very high up on a cliff; the temperature almost feels normal!");
	Room* cliff2 = new Room("on the highest cliff. You have a very good view of the volcanic, even wastier wastelands beyond the desert.");
	Room* mineshaft = new Room("underground in the mineshaft. It was too hot outside, but now it's very cold :(");
	Room* mineshaft2 = new Room("at a pickaxe in the road. That's not a utensil...");
	Room* mineshaft3 = new Room("deep in the mineshaft. A huge face spans the wall; he looks very interesting.");
	Room* minelight = new Room("pretty high up, though still underground. You can see the light at the end of the tunnel just up above; how lovely.");
	Room* mineexit = new Room("at the high-up exit of the mineshaft. SO MANY STAIRS...");
	Room* mineshaftside = new Room("in a side tunnel of the mineshaft. All the ores are left untouched; what were the miners looking for?"); //water that's what
	Room* kaboomroom = new Room("in a very unstable looking room. It's filled to the brim with dynamite, which is concerning considering that maniac that hangs out here.");
	Room* volcanoentrance = new Room("at the northernmost end of the mineshaft. Burning light and ash enter through the mineshaft entrance.");
	Room* desertstation = new Room("in a train station, which seems to have been accidentally intercepted by the mineshaft.");
	desertstation->setStation();
	Room* deserttunnel = new Room("in a long, dark train tunnel. You've never seen one that goes on for so long with no collapse.");
	deserttunnel->setStation();
	Room* minespring = new Room("at an underground spring which channels water into the town oasis. There's many dead miners here.");

	//Create all HELLISH HIGHLANDS rooms
	Room* volcano = new Room("in the scorched highlands just before BURGERSBURG. It's very hot.");
	volcano->setWelcome("WELCOME TO HELLISH HIGHLANDS!");
	volcano->setWelcome("<<< THE END OF THE ROAD >>>");
	volcano->setWelcome("The air burns your skin and ash fills your lungs.");
	volcano->setWelcome("The BURGER RESTAURANT can be seen clearly, but its image is heavily distorted by the heat.");
	volcano->setWelcome("You're almost there.");
	Room* volcano1 = new Room("in the volcanic highlands. An old, abandoned factory stands to the EAST.");
	Room* volcano2 = new Room("in a volcanic plain. The ashen remains of some plants can be seen here.");
	Room* volcano3 = new Room("in the scorched remains of an old factory town. I don't think there's any hope of this one's citizens coming back.\nThere's stairs leading down to a train station.");
	Room* volcanostation = new Room("in the factory town train station. Some lava has settled here, but the tunnels are probably still usable?");
	volcanostation->setStation();
	Room* volcano4 = new Room("on a scorched path. It reminds you of pepperoni pizza.");
	Room* volcano5 = new Room("at a... knife in the road.");
	Room* volcano6 = new Room("in a volcanic valley. Another factory stands here, more charred than the rest, yet still holding up.");
	Room* volcano7 = new Room("at the very edge of the volcanic valley. An huge old bridge forms a road to the city.");
	//first factory, very small
	Room* factory1 = new Room("in the factory, on a small grated bridge over a pool of lava.");
	Room* factorykitchen = new Room("in a makeshift kitchen made from a factory observation room. Whatever was made here before, it now only churns out slag.");
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
	Room* factorybalcony2 = new Room("on a balcony overlooking the central area. You can also see a garden room of sorts from here, though there is no path to it.");
	Room* factorygarden = new Room("in a garden which is not wheelchair-accessible. The plants look very unhealthy; they seem to have been experimenting with nuclear technology here.");
	Room* conveyor3 = new Room("on an assembly line! This one makes a path to the control room.");
	Room* controlroom2 = new Room("in the factory control room. The machinery looks usable depsite the lava; props to whoever made it!");
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
	Room* heavymachineryroom = new Room("in a room full of heavy machinery. Some of them are, incredibly, still operational.");
	Room* factoryroofsw = new Room("poking out of the factory. The roof has caved in here.");
	Room* factoryroofnw = new Room("high up in the factory. You can see the control room just up ahead.");
	Room* controlroom3 = new Room("in the factory control room. This one's machinery is very rusty. Very icky.");
	Room* factoryroofne = new Room("high up in the factory. You see a short hallway. It's a really nice hallway.");
	Room* factoryhallway = new Room("at the end of the hallway. A ladder leads high upwards.");
	Room* factorytower = new Room("at the top of the factory tower. It's full of computers and it smells like energy drink.");
	Room* factoryroofse = new Room("high up in the factory. You see a corridor full of employee of the month awards.");
	Room* factorytreasure = new Room("at the end of the corridor. JIM SHADY is the employee of the year. Good for him.");
	//the castle
	Room* castleentrance = new Room("at the entrance of a large, crystaline castle. It has a very angular design.");
	Room* castlehall = new Room("in the main hall of the castle. Ornate pillars and paintings of internet browsers pave the way towards the throne room.");
	Room* castlethrone = new Room("in the throne room of the castle. The ceiling is reeeeeeally high up, and the throne almost reaches the top!");
	castlethrone->setWelcome("PLUM - AHHHHHHHHHH HELP ME I'VE BEEN KIDNAPPED!");
	castlethrone->setWelcome("BROWSER - GWAHAHAHAHAHA!");
	//the sewers
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
	//the volcano temple
	Room* volcanotempleentrance = new Room("in a massive cavern, at the door of an ancient volcanic temple. The rock burns bright red.");
	//the bridge to BURGERSBURG
	Room* bridge1 = new Room("on the final bridge to BURGERSBURG. The lava ocean crackles beneath your feet.");
	Room* bridge2 = new Room("halfway through the bridge. An eternal night looms over the city.");
	Room* bridge3 = new Room("on the far end of the bridge. The gate into BURGERSBURG stands right up ahead.");

	//Create MOUNT MINBUS rooms. This place is basically just a small boss arena
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
	Room* tenthome = new Room("in the developer's house.");

	//Create all BURGERSBURG rooms
	Room* BURGERSBURG = new Room("at the gate of BURGERSBURG. The BURGER RESTAURANT is just down main street.");
	BURGERSBURG->setWelcome("Welcome to BURGERSBURG!");
	BURGERSBURG->setWelcome("<<< THE CAPITAL OF SIN >>>");
	BURGERSBURG->setWelcome("Massive skyscrapers blot out the sun, and the BURGER RESTAURANT towers above them all.");
	BURGERSBURG->setWelcome("All but a few buildings are boarded up.");
	BURGERSBURG->setWelcome("You don't want this city knowing where you live.");
	BURGERSBURG->setWelcome("We are not responsible for theft or damage to vehicles or contents!");
	Room* leftstreet1 = new Room("in the poor side of the city. It's relatively tranquil here since everything's already been stolen.");
	Room* leftstreet2 = new Room("next to a building with a fish sign. There is an uncharacteristic sense of calm here.");
	Room* leftstreet3 = new Room("on the sidewalk, avoiding a burning crack in the road.");
	Room* leftstreet4 = new Room("at the entrance to a glowing casino. One of the few functioning buildings here.");
	Room* leftstreet5 = new Room("at the edge of the regular district. The streets past this point look well-maintained.");
	Room* newstreet1 = new Room("at a construction site. This thing is never getting completed.");
	Room* newstreet2 = new Room("in an alleyway. You see a THIEF rooting through the dumpster. Is there really nothing better to steal?");
	Room* newstreet3 = new Room("at the entrance to a convenience store. The door is intact! And all the windows are broken.");
	Room* newstreet4 = new Room("at the city orphanage. It's scheduled for demolition.");
	Room* newstreet5 = new Room("at the edge of the regular district. The buildings beyond look really well-maintained.");
	Room* mainstreet1 = new Room("on main street. There's a few lopsided cars on fire here.");
	Room* mainstreet2 = new Room("on main street. You hear an explosion somewhere in the distance.");
	Room* mainstreet3 = new Room("on main street. The traffic lights are all broken, but the random fires provide cozy lighting.");
	Room* mainstreet4 = new Room("on main street. There's a stairway here leading down to the BURGERSBURG train station.");
	Room* mainstreet5 = new Room("at the end of main street. The BURGER RESTAURANT looms ahead of you. BURGER TENDRILS wrap around its base.");
	Room* coolstreet1 = new Room("near the entrance of the city. The top of a building is sliced clean off.");
	Room* coolstreet2 = new Room("at the entrance to an apartment building. This one's door is open.");
	Room* coolstreet3 = new Room("at a functional hot dog stand."); //set up hot dog vendor here
	Room* coolstreet4 = new Room("at the entrance to a dark alley. Eh could be darker.");
	Room* coolstreet5 = new Room("next to some copy-pasted looking townhouses. Surely homebuilders can be more creative?");
	Room* rightstreet1 = new Room("in the crumbling corner of the city. The lava sea radiates light onto exposed infrastructure.");
	Room* rightstreet2 = new Room("in the city. You hear a random explosion, business as usual.");
	Room* rightstreet3 = new Room("a really nice brick building. You see a guy in a black rat costume fighting an old lady."); //make sure this gets renamed after beating Ratman
	Room* rightstreet4 = new Room("at a small glass store. You see two guys carrying a large pane of glass."); //add group of people npc for joke
	Room* rightstreet5 = new Room("at the entrance to the BURGERSBURG fire department. It probably hasn't seen much use recently.");
	Room* richneighborhood1 = new Room("in the rich people corner of town. Each huge building corresponds to just one person.");
	Room* richneighborhood2 = new Room("in the rich neighborhood. The security systems put you on guard; as soon as you step on a lawn, you'd probably be blown to smithereens.");
	Room* richneighborhood3 = new Room("at a secluded corner of the rich neighborhood. Some guy's standing outside. What a daredevil.");
	Room* richneighborhood4 = new Room("at the doorway of a huge layered bureaucratic-looking building. It's probably the second-tallest building in the city.");
	//city interiors
	Room* burgfish = new Room("in the fish building. A warm light shines down the stairs.");
	Room* burgchurch = new Room("in a hidden church. Everyone seems joyful despite the state of the city.");
	Room* burgstore = new Room("in an abandoned store. Nobody bothers to stock the shelves anymore.");
	Room* casino = new Room("in the casino. Sounds of slot machines and flashing lights overload your senses. You should really leave before you develop a gambling addiction.");
	Room* darkalley = new Room("in a dark alley, a characteristic of those about to be mugged.");
	Room* shrimpartment1 = new Room("in an apartment building. There's a spiral staircase going all the way up.");
	Room* shrimpartment2 = new Room("on the second floor. The flowery wallpaper is peeling off.");
	Room* shrimpartment3 = new Room("on the third floor. You see an open doorway leading to an unfurnished room; your instincts pull you away from it.");
	Room* shrimproof = new Room("on the apartment rooftop. You have a nice view of the city lights.");
	Room* firedepartment = new Room("in the fire department. The fire alarm is sadly whimpering.");
	Room* burgstation = new Room("in the BURGERSBURG train station. Oddly enough, this one seems in the best condition out of all the ones you've seen.");
	burgstation->setStation();
	Room* ceolobby = new Room("in the ornate lobby of the building. The yearly RICH PEOPLE reunion is ongoing.");
	Room* ceoelevator0 = new Room("in the elevator of the rich people building, at the lowest level.");
	Room* ceoelevator1 = new Room("in the elevator, on the first level. You see");
	Room* ceoelevator2 = new Room("in the elevator, on the first level. You see");
	Room* ceoelevator3 = new Room("in the elevator, at the top level. You see the CEO's office ahead.");
	Room* ceoroom = new Room("in the BURGER CEO's office. The desk stands in front of the BURGER SAFE, where all the company valuables are held.");
	Room* elevator = new Room("in the elevator of the BURGER RESTAURANT. It's a really fancy circular elevator, with a 360 degree view of the city.");
	Room* elevatortop = new Room("in the elevator, elevated all the way to the top. Once you go through the door, there is no going back.");
	elevatortop->setWelcome("You ventured beyond your forest home,");
	elevatortop->setWelcome("endured through desolate sands,");
	elevatortop->setWelcome("journeyed through volcanic highlands,");
	elevatortop->setWelcome("and braved the crime of the city.");
	elevatortop->setWelcome("It was a long journey, but you're a mere minute away from your destination...");
	elevatortop->setWelcome("The elevator shoots upwards.");
	elevatortop->setWelcome("...");
	elevatortop->setWelcome("...");
	elevatortop->setWelcome("...");
	elevatortop->setWelcome("Neon BURGER billboards attached to the elevator shaft pass by.");
	elevatortop->setWelcome("You pass the highest peaks of the BURGERSBURG skyline.");
	elevatortop->setWelcome("...");
	elevatortop->setWelcome("...");
	elevatortop->setWelcome("...");
	elevatortop->setWelcome("You pass the cloud level, leaving behind the gloom of BURGERSBURG.");
	elevatortop->setWelcome("The sun is shining over the horizon.");
	elevatortop->setWelcome("...");
	elevatortop->setWelcome("...");
	elevatortop->setWelcome("...");
	elevatortop->setWelcome("The elevator dings. You have arrived at your destination...");
	elevatortop->setWelcome("What will you do?");
	Room* BURGERRESTAURANT = new Room("at the tippity top the BURGER RESTAURANT. You can see the sun barely shining under the horizon.\nThe BURGER MAN is waiting for you to order a BURGER.");
	Room* elevatorbottom = new Room("deep down in the Earth, in the restricted level of the BURGER RESTAURANT.");
	elevatorbottom->setWelcome("The elevator shoots downwards.");
	elevatorbottom->setWelcome("...");
	elevatorbottom->setWelcome("...");
	elevatorbottom->setWelcome("...");
	elevatorbottom->setWelcome("Hard rock speeds by.");
	elevatorbottom->setWelcome("...");
	elevatorbottom->setWelcome("...");
	elevatorbottom->setWelcome("...");
	elevatorbottom->setWelcome("It's so far down; the temperature starts rising.");
	elevatorbottom->setWelcome("...");
	elevatorbottom->setWelcome("...");
	elevatorbottom->setWelcome("...");
	elevatorbottom->setWelcome("Temperatures have risen above volcanic levels...");
	elevatorbottom->setWelcome("The elevator dings. Where have you arrived?");
	Room* BURGERPRISON = new Room("in the BURGER PRISON. There one singular damp cell. It smells like BURGERs.");
	Room* basestation = new Room("in a deep train tunnel near the BURGER PRISON. Where do trains need to go this deep?");
	basestation->setStation();

	Room* tunnels = new Room("in the train tunnels that span the continent. The acoustics here are great!");
	tunnels->setStation();

	//Create effects
	
	//Create attacks
	
	//Create NPCs and items
	//Attack copy/paste: Attack* ATTACK = new Attack("NAME", "DESCRIPTION", COST, POWER, PIERCE, MINHITS, MAXHITS, TARGETS);
	////////////////////////
	//DELETE THIS ATTACK, ONLY FOR TESTING
	Attack* finishhim = new Attack("", "DESCRIPTION", 0, 99999999, 99999999, 1, 1, 1);
	////////////////
	Attack* punch = new Attack("PUNCH", "punched", -5, 2, 0, 1, 1, 1);
	punch->addDescription("Throw a simple punch at the target.");

	Attack* energyball = new Attack("ENERGY BALL", "threw an energy ball at", 5, 10, 2, 1, 1, 1);
	energyball->addDescription("Throw a ball of pure kinetic energy at the target.");

	//SET START ROOM TO VILLAGE
	NPC* self = new NPC("\0", "SELF", "It's a me.", village, 20, 5, 6, 0, 0, 10, 5, 0, true, true);
	self->setScale(1, 1, 1, 0, 0, 1, 1);
	self->setDialogue("Huh?");
	self->Recruit();
	self->setBasicAttack(punch);
	self->addSpecialAttack(finishhim);

	NPC* floria = new NPC("FLOWER GIRL", "FLORIA", "Your little sister who gets along well with nature, especially flowers. She has a flower-shaped hat.", flowerfield2, 16, 5, 4, 0, 1, 5, 2);
	floria->addConversation(floria, "Hey big brother! Aren't these flowers just so lovely?");
	floria->addConversation(self, "NO THESE FLOWERS SUCK THEY TRIED TO EAT ME.");
	floria->setDialogue("I just love flowers!");
	floria->setGymDialogue("I love running in circles around the gym! Exercise is so fun!");
	floria->setRecruitmentDialogue("Yay! I hope we see some new flowers!");
	floria->setRecruitedDialogue("I must see all the flowers!");
	floria->setDismissalDialogue("I'm going to go back to my flower field!");

	NPC* egadwick = new NPC("SCIENCE GRAMPS", "EGADWICK", "Your grandpa who lives in a secluded corner of the village. He's always advancing science to the dismay of high school chemistry students.", tentlab, 15, 2, 3, 10, 10, 2, 10, 10);
	egadwick->setDialogue("Ah hello kiddo. How's it going?");
	egadwick->setGymDialogue("Eh, exercise isn't really my thing, kiddo. I can gain experience by working out my mind!");
	egadwick->setRejectionDialogue("No, sorry kiddo. I made a robot for gardening but now it's trying to cut my gorgeous hair and it's on the loose in the forest. If you could destroy it I could probably go.");
	egadwick->setRecruitmentDialogue("Ah, I haven't been adventuring in decades. Thanks for the invitation, kiddo!");
	egadwick->setRecruitedDialogue("I love science. The world has so many interesting specimens.");
	egadwick->setDismissalDialogue("Great hanging out with you, kiddo! Well, I guess I'll go work on a better robot!");

	NPC* archie = new NPC("VILLAGE ELDER", "ARCHIE", "The elder of Tactical Tent Village. He stands there all day and night like a statue.", village, 1, 0, 1, 0, 0, 0, 0, 50);
	archie->setDialogue("Safe travels, child!");
	archie->addConversation(archie, "So you are going on a BURGER QUEST, I hear?");
	archie->addConversation(archie, "Just keep heading NORTH, and you'll soon reach BURGERSBURG.");
	archie->addConversation(archie, "Safe travels, child!");
	archie->addConversation(archie, "Make sure to bring me back a BURGER! Heh heh heh.");
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

	Effect* minibuff = new Effect("MINIBUFF", 4, 0, 0, 5, 5, 5, 5);
	Effect* buff = new Effect("BUFF", 4, 0, 0, 15, 15, 15, 15);
	Effect* bigbuff = new Effect("BIG BUFF", 4, 0, 0, 30, 30, 30, 30);
	Effect* megabuff = new Effect("MEGABUFF", 2, 0, 0, 50, 50, 50, 50);

	Effect* smoothiebuff = new Effect("MULTIPOSITION", 999999, -30, 10, 70, 70, 70, 70);

	Effect* energized = new Effect("ENERGIZED", 2, 0, 0, 50, 0, 0, 0);

	//tent store stock
	Item* apple = new HpItem("HEALTHY APPLE", "A healthy red apple. (heals 10 HP)", limbo, 10);
	Item* pineapple = new HpItem("HEALTHY PINE APPLE", "A crunchy and durable pine apple. Very annoying to eat, but apparently very healthy! (heals 20 HP)", limbo, 20);
	Item* noodles = new EffectItem("MIGHTY NOODLES", "Some healthy and tasty homemade noodles sprinkled with mighty spices. (MINIBUFF effect)", limbo, minibuff);
	Item* lasagna = new EffectItem("MIGHTY LASAGNA", "A healthy and tasty homemade lasagna made with mighty flour. (BUFF effect)", limbo, buff);
	Item* pizza = new EffectItem("MIGHTY PIZZA", "A healthy and tasty homemade pizza topped with mighty pepperoni. (BIG BUFF EFFECT)", limbo, bigbuff);
	//set up the advanced energy attacks
	Attack* precisionstrike = new Attack("PRECISION STRIKE", "threw a precise energy ellipsoid at", 12, 35, 15, 1, 1, 1, false, 12);
	precisionstrike->addDescription("Throw a heavy mass of energy speedily towards the target. (35 ATTACK 15 PIERCE)");
	Attack* ballisticmissile = new Attack("BALLISTIC MISSILE", "threw a missile of energy at", 19, 50, 25, 1, 1, 1, false, 18);
	precisionstrike->addDescription("Throw a dense missile of energy straight towards the target. (50 ATTACK 25 PIERCE)");
	Attack* bigenergyball = new Attack("BIG ENERGY BALL", "threw a big ball of energy at", 10, 20, 10, 1, 1, 1, false, 8);
	bigenergyball->addDescription("Throw a large mass of energy at the target and their surrounding allies. (20 ATTACK 10 PIERCE)");
	Attack* energize = new Attack("ENERGIZE", "energized", 14, 0, 0, 1, 1, 1, true, 21);
	energize->addEffect(energized);
	energize->addDescription("Imbue yourself or an teammate with energy, adding 50 additional power to their next attack.");
	Attack* spbomb = new Attack("SP BOMB", "lobbed a ball of the team's collective SP energy at", 0, 0, 0, 1, 1, 9, false, 25);
	spbomb->addDescription("Gather up the collective SP of the entire team into a ball of energy and lob it at the enemy team. (SP ATTACK, 0 PIERCE)");
	Item* energybook = new EducationItem("ADVANCED GUIDE TO ENERGY MANIPULATION", "A book full of energy manipulation techniques. You could learn some cool attacks from this.", limbo, precisionstrike);
	EducationItem* _energybook = (EducationItem*)energybook; //we have to convert the book to EducationItem in order to add the attacks because regular Item*s have no setAttack function
	_energybook->setAttack(ballisticmissile);
	_energybook->setAttack(bigenergyball);
	_energybook->setAttack(energize);
	_energybook->setAttack(spbomb);

	tentstore->setStock(apple, 2147483647, 10, "JIMMY JOHN - Thank you for your patronage! Enjoy your apple!");
	tentstore->setStock(pineapple, 2147483647, 30, "JIMMY JOHN - Thank you for your patronage! Enjoy your pine apple!");
	tentstore->setStock(noodles, 2147483647, 10, "JIMMY JOHN - Thank you for your patronage! Enjoy your noodles!");
	tentstore->setStock(lasagna, 2147483647, 35, "JIMMY JOHN - Thank you for your patronage! Enjoy your lasagna!");
	tentstore->setStock(pizza, 2147483647, 50, "JIMMY JOHN - Thank you for your patronage! Enjoy your pizza!");
	tentstore->setStock(energybook, 1, 100, "JIMMY JOHN - I don't understand that book. But if it interests you then great! Oh, and thank you for your patronage!");	

	NPC* mrdeer = new NPC("", "MR DEER", "Your friend MR. DEER. He's a deer.", deerclearing, 5, 2, 6, 0, 4, 20, 0, 5);
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
	mrdeer->setDialogue("(deer noises)");
	mrdeer->setRejectionDialogue("(no thank you deer noise)");

	NPC* wallelder = new NPC("WALL ELDER", "WELBY", "An ancient elder whose rocky face spans the wall. There may be more to him, but all you can see is his face.", mineshaft3, 15000, 15000, 15000, 15000, 0, 2000, 500, 25000);
	wallelder->addConversation(wallelder, "Child, are you on a BURGER QUEST?");
	wallelder->addConversation(self, "Indeed I am.");
	wallelder->addConversation(wallelder, "Do not be fooled by the allure of BURGER. Do you not know why you crave it so?");
	wallelder->addConversation(wallelder, "BURGER is formed from the essence of evil. Have you seen the desert above?");
	wallelder->addConversation(self, "Uh huh.");
	wallelder->addConversation(wallelder, "It used to be a beautiful forest, full of life like the one to the south.");
	wallelder->addConversation(wallelder, "But the BURGER MAN ordered the woods parched, and reduced it to the wastes seen today.");
	wallelder->addConversation(wallelder, "He will continue to do so until nature has been expunged from this world.");
	wallelder->addConversation(wallelder, "By destroying the means to live, he guides the people by hunger and thirst to his city, where they will be more easily tempted by the BURGER RESTAURANT.");
	wallelder->addConversation(self, "Dang that's crazy.");
	wallelder->addConversation(wallelder, "If nothing else, remember this. The lies of this world are placed high UP on shining pedestals, while its truths are buried DOWN below.");
	wallelder->setDialogue("Always beware the temptation of BURGER.");
	wallelder->setRejectionDialogue("I have been embedded in this hard rock for ages. I cannot move nor join you.");

	NPC* magmelder = new NPC("MAGMELDER", "MELVIN", "A molten elder who lives in the lava. Lava continuously flows down from him, but his mustache and nose have a visible outline", factory1, 200, 80, 120, 10, 50, 10, 30, 100);
	magmelder->addConversation(magmelder, "Oh it's horrible!");
	magmelder->addConversation(magmelder, "I raised the lava level around the city so that no one could eat a BURGER!");
	magmelder->addConversation(magmelder, "But its allure is too great! I've seen people try to sail across and get burnt up!");
	magmelder->addConversation(magmelder, "Some are even so desperate they try to swim across!");
	magmelder->addConversation(self, "Oh dang.");
	magmelder->addConversation(magmelder, "Please! You must lower the lava again! These factories have drainage valves in their control rooms!");
	magmelder->addConversation(self, "Why can't you just do it yourself?");
	magmelder->addConversation(magmelder, "I would, but I seem to have gotten myself stuck in this pool of lava!");
	magmelder->addConversation(self, "I see.");
	magmelder->setDialogue("You must drain the lava before more lives are lost!");
	magmelder->setRejectionDialogue("Nay! I must dwell in lava and you must dwell on land. These things conflict, do they not?");

	NPC* developer = new NPC("DEVELOPER", "TOMAS", "The guy who made the game, except not really that guy because yeah.", tenthome, 1, 0, 1, 0, 0, 0, 0, 0);
	developer->setDialogue("Yo wassup.");
	developer->addConversation(self, "Yo developer man.");
	developer->addConversation(developer, "Yeah?");
	developer->addConversation(self, "Why is everything text?");
	developer->addConversation(developer, "Because the camera man is on vacation.");
	developer->addConversation(developer, "Only the narrator could cover his shift.");
	developer->addConversation(self, "Oh I see.");
	developer->setRejectionDialogue("Nah, sorry. I don't think I would make a good teammate because I made my stats really low. I gotta stay humble, you know?");
	burgerman->setLink(developer);
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
	burgerman->addLinkedConvo(self, "Dang that's crazy.");

	NPC* forestknight = new NPC("FOREST KNIGHT", "ABSOLOM", "An old knight decked out in wooden armor, on a quest to vanquish all evil that crosses his path.", forestgrave, 20, 20, 25, 30, 10, 0, 10);
	forestknight->setScale(1, 2, 1, 1, 0, 0, 0);
	forestknight->setLevel(30);
	forestknight->setRejectionDialogue("I sense that you are on a BURGER QUEST. I will not assist you in obtaining this object of sin.\nI implore you to find a new, more noble goal for your quest.");
	forestknight->addConversation(forestknight, "For years, that fiend has kept me trapped here with the graves of my fallen compatriots.");
	forestknight->addConversation(forestknight, "Does he not sleep? Does he not eat?");
	forestknight->addConversation(forestknight, "From what I have seen, no. Nevertheless, I must thank you, child, for freeing me from that shrimp's hold.");
	forestknight->addConversation(self, "Yeah no problem.");

	/*(forestknight, "Child, I cannot join you on your quest. I shan't assist you in obtaining the evil BURGER.");
	(self, "Nah bro I'm not doing that anymore.");
	(self, "I'm actually trying to like completely annihilate BURGERs from this world now.");
	(forestknight, "Ah, that's good to hear.");
	(forestknight, "My apologies, child. My quest sensor is a bit slow.");
	(forestknight, "I have not updated to the latest software version in a little while.");*/

	NPC* minermaniac = new NPC("MINER MANIAC", "MIKE", "Crazed miner who likes exploding things. A frequent customer of the subterranean dynamite store.", kaboomroom, 15, 5, 20, 0, 20, 12, 15);
	minermaniac->setScale(0, 0, 0, 0, 0, 0, 1);

	Attack* shurikenthrow = new Attack("SHURIKEN THROW", "threw a spread of shurikens at", 0, 7, 5, 0, 2, 3);
	shurikenthrow->addDescription("Throw a spread of shurikens at the target, with varying success since you're just chucking them.");
	Item* shuriken = new EducationItem("SHURIKEN", "A ninja shuriken with a note attached: \"Congratulations on defeating our ninja scout. Take this shuriken and train in the ninja ways, and maybe one day you'll become a true ninja.\"", ninjaland, shurikenthrow);
	
	NPC* gymbro = new NPC("GYM BRO", "JIM NASIUM", "Obsessed with being in peak physique, there's scarcely a moment when he isn't seen in the gym.", desertgymfixed, 10, 10, 10, 10, 10, 10, 0);
	gymbro->setGymDialogue("YYYEEEEEEEEEEAAAAAAAAAAAAAHHHHHHHHHHHHHHHHH WEIGHT LIFTING!!!!!!!!!!!!!!!!!");
	gymbro->setRejectionDialogue("Sorry dude, I gotta stay on THAT GRIND to get THEM GAINS.");

	Attack* forkthrow = new Attack("FORK THROW", "threw a fork at", 0, 1, 0, 1, 1, 1);
	Attack* pickthrow = new Attack("PICKAXE THROW", "threw a pickaxe at", 0, 1, 2, 1, 1, 1);
	Attack* knifethrow = new Attack("KNIFE THROW", "threw a knife at", 0, 1, 1, 1, 1, 1);
	Attack* coverthrow = new Attack("HEAVY FRISBEE", "threw a heavy manhole cover at", 0, 180, 0, 1, 1, 1);

	Item* telescope = new InfoItem("TELESCOPE", "A large, robust, telescope for observing space.", "You looked through the telescope and saw an orbital office building.", tentlab);
	
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

	Item* BURGER = new BURGERItem("BURGER", "It's a BURGER and it smells like a BURGER.", limbo);
	BURGERRESTAURANT->setStock(BURGER, 2147483647, 10, "BURGER MAN - \"ENJOY YOUR BURGER!\"");

	Item* skateboard = new InfoItem("SKATEBOARD", "It's a pretty cool skateboard for doing cool skateboard things.", "You did a kickflip. Very cool.", limbo);
	skateboard->setTakable();
	desertshopfixed->setStock(skateboard, 1, 100, "");

	//Create exits between rooms
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
	docks->setExit(IN_TENT, tenthouse);
	tenthouse->setExit(OUT, docks);
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
	ninjavillage->setExit(IN_HOUSE_1, ninjacapitol);
	ninjavillage->setExit(IN_HOUSE_2, ninjapantry);
	ninjavillage->setExit(IN_HOUSE_3, ninjaforge);
	foresttempleentrance->setExit(SOUTHWEST, forestleft);
	foresttempleentrance->setExit(SOUTHEAST, forestright);
	foresttempleentrance->setExit(NORTHWEST, flowerfield);
	foresttempleentrance->setExit(NORTH, forestfork);
	foresttempleentrance->setExit(IN_TEMPLE, foresttemplestairs);
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
	deserttempleentrance->setExit(IN_TEMPLE, deserttemplestairs);
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
	deserttownfixed->setExit(NORTH, canyon1);
	deserttownfixed->setExit(SOUTH, deserthill);
	deserttownfixed->setExit(EAST, oasis);
	deserttownfixed->setExit(WEST, canyon);
	deserttownfixed->setExit(IN_HOUSE_1, desertshopfixed);
	deserttownfixed->setExit(IN_HOUSE_2, desertgymfixed);
	deserttownfixed->setExit(IN_HOUSE_3, deserthousefixed);
	desertshopfixed->setExit(OUT,deserttownfixed);
	desertgymfixed->setExit(OUT,deserttownfixed);
	deserthousefixed->setExit(OUT,deserttownfixed);
	oasis->setExit(WEST, deserttown);
	oasisfixed->setExit(WEST, deserttown);
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
	elevator->setExit(UP, elevatortop);
	elevator->setExit(OUT, mainstreet5);
	elevatortop->setExit(OUT, BURGERRESTAURANT);
	elevatortop->setExit(DOWN, elevator);
	BURGERRESTAURANT->setExit(IN_ELEVATOR, elevatortop);

	tunnels->setExit(TO_THE_VILLAGE, tentstation);
	tunnels->setExit(TO_THE_DESERT, desertstation);

	//set up enemies

	NPC* tunnellobster = new NPC("", "TUNNEL LOBSTER", "An immense, savage crustacean who inhabits the tunnels below.", tunnels, 200, 20, 10, 20, 10, 50, 10);
	tunnellobster->setLobster(tunnels);
	tunnellobster->setLeader(true, 10, desertstation, false);
	tunnellobster->setTunnelDirection(tentstation, TO_THE_VILLAGE);
	tunnellobster->setTunnelDirection(desertstation, TO_THE_DESERT);
	tunnellobster->setTunnelDirection(deserttunnel, TO_THE_DESERT);
	tunnellobster->setTunnelDirection(volcanostation, TO_THE_HIGHLANDS);
	tunnellobster->setTunnelDirection(burgstation, TO_BURGERSBURG);
	tunnellobster->setTunnelDirection(basestation, TO_THE_BASEMENT);
	tunnellobster->setLink(tunnellobster);
	tunnellobster->setDefeatNPC("", "An immense, tamed crustacean who inhabits the tunnels below.", "HHhhHhHHhHhHhHHHhHHhhHhh (happy lobster noises).", NULL);
	tunnellobster->setDialogue("HHhHHHhhHHhHhhHhHHhHhHHh (angry lobster noises).");
	tunnellobster->setRejectionDialogue("HhhHhHhHhhhhHHhHHh (lobster noises probably meaning no).");

	Item* lobstercaller = new CallerItem("LOBSTER WHISTLE", "Used for summoning lobsters by playing a lobstery melody.", desertstation, tunnellobster);

	//set up generic non-npc enemies
	NPC* pricklyhog = new NPC("", "PRICKLY HOG", "A small but ferocious hog with sharp prickles.", limbo, 10, 10, 5, 0, 10, 15, 5);
	Attack* headbutt = new Attack("HEADBUTT", "headbutted", -5, 5, 0, 1, 1, 1);
	Attack* homing_prickle = new Attack("HOMING PRICKLE", "launched homing prickles at", 5, 3, 5, 2, 4, 3);
	pricklyhog->setBasicAttack(headbutt);
	pricklyhog->addSpecialAttack(homing_prickle);

	NPC* greaterhog = new NPC("", "GREATER HOG", "A larger and more territorial hog with sharp prickles and tusks.", limbo, 20, 10, 10, 2, 20, 20, 10);
	greaterhog->setBasicAttack(headbutt);
	greaterhog->addSpecialAttack(homing_prickle);

	NPC* grassman = new NPC("", "GRASSMAN", "A really grassy humanoid who hates real humans.", limbo, 16, 0, 5, 0, 2, 5, 5);
	Attack* grassstrike = new Attack("GRASS STRIKE", "grassily striked", -2, 3, 0, 1, 1, 1);
	Attack* lawnmower = new Attack("LAWNMOWER", "threw a lawnmower at", 5, 20, 5, 1, 1, 2, false, 2);
	grassman->setBasicAttack(grassstrike);
	grassman->addSpecialAttack(lawnmower);

	NPC* buffgrassman = new NPC("", "BUFF GRASSMAN", "A really grassy humanoid who has been hitting the gym.", limbo, 32, 0, 8, 0, 2, 4, 6);
	Effect* theburn = new Effect("THE BURN", 3, 0, 0, 10, 10, 10);
	Attack* benchpress = new Attack("BENCH PRESS", "worked out with", 4, 0, 0, 1, 1, 5, true);
	buffgrassman->setBasicAttack(grassstrike);
	buffgrassman->addSpecialAttack(benchpress);

	NPC* enemydeer = new NPC("", "ENEMY DEER", "A bipedal deer in a fighting stance.", limbo, 10, 6, 3, 1, 5, 5, 5);
	Attack* deercombo = new Attack("DEER COMBO", "beat up", 0, 1, 0, 4, 4, 1);
	enemydeer->setBasicAttack(deercombo);

	NPC* ninjascout = new NPC("", "NINJA SCOUT", "A junior member of the ninja village, often sent on easy missions.", limbo, 20, 2, 4, 0, 5, 20, 15);
	Attack* shurikenninja = new Attack("SHURIKEN", "expertly threw shurikens at", 0, 4, 5, 0, 3, 3);
	ninjascout->setBasicAttack(shurikenninja);

	NPC* jimshady = new NPC("", "JIM SHADY", "An envious and spiky shrimp. This JIM SHADY is just imitating.", limbo, 50, 20, 10, 5, 15, 20, 10);
	Attack* shirmplebeam = new Attack("SHRIMPLE BEAM", "fired a pressurized jet of water at", 0, 99999, 99999, 1, 1, 1);
	jimshady->setBasicAttack(shirmplebeam);
	Effect* engarde = new Effect("EN GARDE!", 2147483647);
	engarde->guardset = 1;
	jimshady->setEffect(engarde, false);

	NPC* carnplant = new NPC("", "CARNIVOROUS PLANT", "Really big plant who likes eating meat.", limbo, 20, 5, 7, 5, 5, 12, 10);
	Attack* bite = new Attack("BITE", "bit", -5, 10, 5, 1, 1, 1);
	Attack* nutrientabsorb = new Attack("NUTRIENT ABSORB", "sucked the nutrients out of", 10, 10, 5, 1, 1, 1, 0.5f);
	carnplant->setBasicAttack(bite);
	carnplant->addSpecialAttack(nutrientabsorb);

	NPC* flowerfiend = new NPC("", "FLOWER FIEND", "Enormous carnivorous flower with lashing vines. Probably the FLOWER FRIEND your sister talks about.", limbo, 30, 5, 7, 5, 5, 12, 24);
	Attack* crunch = new Attack("CRUNCH", "used its flowery fangs to crunch", -7, 15, 7, 1, 1, 1);
	Effect* flowerpower = new Effect("FLOWER POWER", 3, 0, 0, 5);
	Attack* flowerempower = new Attack("FLOWER EMPOWER", "used its flower power to buff", 16, 10, 5, 1, 1, 1, true);
	flowerempower->addEffect(flowerpower);
	Attack* solarbeam = new Attack("SOLAR BEAM", "used its petals to channel solar light onto", 24, 30, 10, 1, 1, 1);
	flowerfiend->setBasicAttack(crunch);
	flowerfiend->addSpecialAttack(nutrientabsorb);
	flowerfiend->addSpecialAttack(flowerempower);
	flowerfiend->addSpecialAttack(solarbeam);

	NPC* sandman = new NPC("", "SANDMAN", "A really sandy humanoid continuously flowing with sand.", limbo, 20, 5, 8, 0, 0, 6, 5);
	Effect* sanded = new Effect("SAND IN THE EYES", duration, damage, spleak, attack, defense, tough, pierce);
	Attack* sandthrow = new Attack("POCKET SAND", "threw sand at the eyes of", -2, 5, 0, 1, 1, 1);
	Attack* sandpunch = new Attack("SAND PUNCH", "threw a sandy fist at", 1, 3, 0, 1, 1, 1);
	Attack* lawnmower = new Attack("LAWNMOWER", "threw a lawnmower at", 5, 20, 5, 1, 1, 2, false, 2);
	grassman->setBasicAttack(grassstrike);
	grassman->addSpecialAttack(lawnmower);

	//Attack* ATTACK = new Attack("NAME", "DESCRIPTION", COST, POWER, PIERCE, MINHITS, MAXHITS, TARGETS); <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	//Effect EFFECT = new Effect("NAME", duration, damage, spleak, attack, defense, tough, pierce);
	//NPC* npc = new NPC("TITLE", "NAME", "DESCRIPTION", limbo, hp, def, att, tou, pie, spe, ski);

	//for npcs you can actually fight, make sure to set their level as 0 at construction, then set the level manually after setting the scale, otherwise stats will be off

	//set up npc enemies
	NPC* forestguard = new NPC(*grassman);
	forestguard->setLeader(true, 1, forestentrance);
	forestguard->blockExit(NORTH, ENEMY, "guarded by the GRASSMAN.");
	forestguard->setDialogue("(angry bush noises)");
	forestguard->setRejectionDialogue("(angry bush noises)");

	NPC* deerguard = new NPC(*enemydeer);
	deerguard->setLeader(true, 3, forestleft);
	deerguard->blockExit(WEST, ENEMY, "guarded by the ENEMY DEER.");
	deerguard->setDialogue("(angry deer noises)");
	deerguard->setRejectionDialogue("(angry deer noises)");

	NPC* ninjaguard = new NPC(*ninjascout);
	ninjaguard->setLeader(true, 5, forestright);
	ninjaguard->blockExit(NORTHEAST, ENEMY, "guarded by the NINJA SCOUT.");
	ninjaguard->setDialogue("You will never get past me!!!!!!! >:D");
	ninjaguard->setRejectionDialogue("No!!! I will always be a ninja!!!!!! >:D");

	NPC* forestrando = new NPC(*grassman);
	forestrando->setLeader(true, 5, forestleft);
	forestrando->setDialogue("(angry bush noises)");
	forestrando->setRejectionDialogue("(angry bush noises)");

	NPC* forestguard2 = new NPC(*buffgrassman);
	forestguard2->setLeader(true, 2, foresttempleentrance);
	forestguard2->setParty(pricklyhog);
	forestguard2->blockExit(NORTH, ENEMY, "guarded by the BUFF GRASSMAN.");
	forestguard2->setDialogue("(burly bush noises)");
	forestguard2->setRejectionDialogue("(burly bush noises)");

	NPC* hogguard = new NPC(*greaterhog);
	hogguard->setLeader(true, 3, forestfork);
	hogguard->blockExit(NORTHEAST, ENEMY, "guarded by the GREATER HOG.");
	hogguard->setDialogue("(angry squeal)");
	hogguard->setRejectionDialogue("(angry squeal)");

	NPC* hogguard2 = new NPC(*greaterhog);
	hogguard2->setLeader(true, 4, forestfork);
	hogguard2->setParty(pricklyhog, pricklyhog);
	hogguard2->blockExit(NORTHWEST, ENEMY, "guarded by the GREATER HOG.");
	hogguard2->setDialogue("(angry squeal)");
	hogguard2->setRejectionDialogue("(angry squeal)");

	NPC* forestguard3 = new NPC(*buffgrassman);
	forestguard3->setLeader(true, 5, forestnice);
	forestguard3->setParty(grassman, grassman);
	forestguard3->blockExit(EAST, ENEMY, "guarded by the BUFF GRASSMAN.");
	forestguard3->setDialogue("(burly bush noises)");
	forestguard3->setRejectionDialogue("(angry bush noises)");

	NPC* jimshady1 = new NPC(*jimshady);
	jimshady1->setLeader(true, 5, forestwall);
	jimshady1->blockExit(EAST, ENEMY, "blocked by JIM SHADY.");
	jimshady1->setDialogue("I'm JIM SHADY, yes I'm the REAL SHADY");
	jimshady1->addConversation(jimshady1, "I'm JIM SHADY, yes I'm the REAL SHADY!");
	jimshady1->addConversation(self, "No you aren't.");
	jimshady1->addConversation(jimshady1, "Shut up.");
	jimshady1->setRejectionDialogue("No go away.");

	NPC* roguerobot = new NPC("ROGUE ROBOT", "EGARDENBOT 1.0", "Short trapezoidal copper robot designed to be an expert gardener, before going rogue and trimming EVERYTHING.", forestgarden, 20, 15, 5, 5, 10, 20, 15, 5, true);
	Attack* snip = new Attack("SNIP", "snipped scissors at", -5, 7, 5, 1, 1, 1);
	roguerobot->setBasicAttack(snip);
	roguerobot->setLink(egadwick);
	roguerobot->addLinkedConvo(egadwick, "I'm no longer detecting signals from my robot. Did you by chance stop it?");
	roguerobot->addLinkedConvo(self, "Yep I did.");
	roguerobot->addLinkedConvo(egadwick, "Oh thank goodness! Thanks a bunch, kiddo!");
	roguerobot->addLinkedConvo(egadwick, "Now I can safely be in the great outdoors!");

	NPC* plantguard = new NPC(*carnplant);
	plantguard->setLeader(true, 4, foresttempleentrance);
	plantguard->blockExit(NORTHWEST, ENEMY, "blocked by the CARNIVOROUS PLANT.");
	plantguard->setDialogue("(snapping biting noises)");
	plantguard->setRejectionDialogue("(snapping biting noises)");

	NPC* flowerguard = new NPC(*flowerfiend);
	flowerguard->setLeader(true, 4, flowerfield);
	flowerguard->setParty(carnplant, carnplant);
	flowerguard->blockExit(WEST, ENEMY, "blocked by the FLOWER FIEND.");
	flowerguard->setDialogue("(flowery shriek)");
	flowerguard->setRejectionDialogue("(flowery shriek)");

	NPC* savagehog = new NPC("", "SAVAGE HOG", "Dangerous, mammoth elder hog with very sharp prickles.", bossgrove, 80, 15, 20, 10, 30, 15, 15);
	savagehog->setLeader(true, 5);
	savagehog->setParty(pricklyhog, greaterhog);
	savagehog->blockExit(NORTH, ENEMY, "blocked off by the SAVAGE HOG!");
	savagehog->setDialogue("(SAVAGE ROAR)");
	savagehog->setRejectionDialogue("Nah sorry mate. I'd rather stay home here in the grove with ma hog family.");
	savagehog->setEscapable(false);
	Effect* intimidated = new Effect("INTIMIDATED", 4, 0, 0, -10);
	Attack* charge = new Attack("CHARGE", "charged at", -5, 10, 20, 1, 1, 1);
	Attack* savageroar = new Attack("SAVAGE ROAR", "roared savagely at", 5, 0, 0, 1, 1, 7);
	savageroar->addEffect(intimidated);
	Attack* pricklestorm = new Attack("PRICKLE STORM", "launched a storm of prickles at", 10, 1, 10, 1, 3, 7);
	savagehog->setBasicAttack(charge);
	savagehog->addSpecialAttack(savageroar);
	savagehog->addSpecialAttack(pricklestorm);
	savagehog->setForceBattle();

	//set up teammate viola
	NPC* viola = new NPC("TELEKINETIC KIDNAPPER", "VIOLA", "Telekinetic teenager responsible for the disappearence of the desert town. Her hair floats upwards and she hovers a few feet above the ground.", cliff2, 30, 0, 10, 0, 10, 20, 20, 0, true);
	viola->setScale(0, 0, 1, 0, 1, 0, 2);
	viola->setLeader(true, 20);
	viola->addConversation(self, "Hey did you kidnap everyone in that town over there?");
	viola->addConversation(viola, "So what if I did?");
	viola->addConversation(self, "Lady you can't just go kidnapping people.");
	viola->addConversation(viola, "OH YEAH? FIGHT ME!");
	viola->addConversation(self, "Ok.");
	viola->setDialogue("AHAHAHAHAHAHAHA!");
	viola->setGymDialogue("It's leg day oh nooo :(");
	viola->setRejectionDialogue("Nah, but you can join me and my friends if you want! AHAHAHA!");
	viola->setRecruitmentDialogue("Really? After what I did? Maybe I could do something good by following you. I think I'll go with. Thanks.");
	viola->setRecruitedDialogue("It feels nice to walk. I hadn't done that in a while.");
	viola->setDismissalDialogue("Well I guess I can watch over the town from that cliff over there. That might be good.");
	viola->setRedirect(deserttown, deserttownfixed);
	viola->setLink(viola);
	viola->addLinkedConvo(viola, "I'm sorry. I'll free everyone...");
	viola->addLinkedConvo(viola, "It's just that I'm shy and I have a hard time making friends that's why I kidnapped them...");
	viola->addLinkedConvo(self, "That's not a very good reason.");
	viola->addLinkedConvo(viola, "Yeah I know...");
	viola->addLinkedConvo(viola, "I'm just going to go to that cliff over there...");
	viola->addLinkedConvo(NULL, "VIOLA went to that cliff over there.");
	viola->setDefeatNPC("GRAVITY GIRL", "Telekinetic teenager trying to use her powers for something good.", "I can't believe I let all that power go to my head...", thatcliff);
	viola->setRecruitDialogueChange("Alright! Maybe I can do something good by following you.", "I think I'm doing a good job protecting the town so far.");
	viola->setTalkOnDefeat();
	viola->setForceBattle();
	viola->setEscapable(false);

	NPC* glutton = new NPC("BURGER GLUTTON", "GREER", "Greedy BURGER official sent to keep all the desert's remaining water for himself. He is bloated with water.", minespring, 100, 40, 10, 0, 0, 0, 10);
	glutton->setLeader(true, 30, NULL, false);
	glutton->addConversation(self, "Hey why are you drinking all that water?");
	glutton->addConversation(self, "You don't need that much save it for everyone else!");
	glutton->addConversation(glutton, "Ehehehehe... all da watah is all mine, pal.");
	glutton->addConversation(glutton, "Official BOIGA MAN ordah.");
	glutton->addConversation(self, "You can't just do that what the heck >:(");
	glutton->addConversation(glutton, "If youse want da watah, youse gonna hafta go through me!");
	glutton->addConversation(self, "Alright then.");
	glutton->setRedirect(oasis, oasisfixed);
	glutton->setLink(glutton);
	glutton->setDialogue("Ehehehe... da watah's all fer me!");
	glutton->setRejectionDialogue("Now why would I wanna join youse, huh?");
	glutton->addLinkedConvo(glutton, "Ah you gots me...");
	glutton->addLinkedConvo(glutton, "...");
	glutton->addLinkedConvo(glutton, "I can'ts go back to da boss with a failya like dis...");
	glutton->addLinkedConvo(glutton, "What else is theah to do?");
	glutton->addLinkedConvo(glutton, "Great job lad.");
	glutton->addLinkedConvo(NULL, "GREER disappeared into the darkness...");
	glutton->addLinkedConvo(NULL, "The spring's water can now freely flow into the oasis!");
	glutton->setLinkedRoom(ceoroom, "in the BURGER CEO's office. The desk stands in front of the BURGER SAFE, where all the company valuables are held.\nThere is a rotund corpse in the corner");
	glutton->setTalkOnDefeat();
	glutton->setForceBattle();
	glutton->setEscapable(false);

	NPC* lavaguard = new NPC("", "LAVA GUARDIAN", "Huge guardian with radiant molten armor and weapons. He appears to have been swimming above the bridge when the lava was drained, and now guards the gate to BURGERSBURG.", bridge3, 200, 50, 30, 20, 20, 10, 50);
	lavaguard->setLeader(true, 40, NULL, false);
	lavaguard->setDialogue("(ethereal breathing)");
	lavaguard->setRejectionDialogue("(ethereal breathing)");
	lavaguard->setLink(magmelder, "Oh nooooo............");
	lavaguard->addLinkedConvo(magmelder, "Amazing work! The lava has been drained!");
	lavaguard->addLinkedConvo(magmelder, "But with the way to BURGERSBURG cleared...");
	lavaguard->addLinkedConvo(magmelder, "People can get BURGERs again...");
	lavaguard->addLinkedConvo(magmelder, "Oh nooooo...... We fix one problem and another one comes back......");
	lavaguard->addLinkedConvo(self, "Hm.");
	lavaguard->setEscapable(false);

	//block exits
	tentstation->blockExit(EAST, TUNNEL, "blocked by endless rubble.");
	tentstation->blockExit(WEST, TUNNEL, "blocked by endless rubble.");
	forestgate->blockExit(NORTH, LOCK, "blocked by a large branchy gate. There is a large keyhole in the center with deer antlers.");
	foresttempleentrance->blockExit(IN_TEMPLE, TEMPLE, "sealed shut by ancient technology. No matter what anyone has tried, nobody has ever made it in.");
	treasuregrove->blockExit(NORTH, CHASM, "blocked by a steep ravine.");
	treasurecliff->blockExit(SOUTH, CHASM, "blocked by a steep ravine.");
	ninjaland->blockExit(UP, MISC, "too high. You need ninja abilities to scale the trees.");

	//desert is blocked by scorching sands, prevented with shoes

	//volcano factory exit is blocked by the caved-in roof

	return self; //returns the player character
}

//prints all the properties of the given room
void PrintRoomData(Room* currentRoom) {
	currentRoom->printWelcome(); //some rooms have messages they print on arrival
	cout << "\nYou are " << currentRoom->getDescription();
	currentRoom->printExits();
	currentRoom->printNPCs();
	currentRoom->printItems();
	currentRoom->printStock(); //prints buyable items in this room
	currentRoom->printBlocks(); //prints which exits are blocked
}

//move the player and co. to a new room based on direction, or also just teleopring to forceDest if given
void travel(Room*& currentRoom, char* direction, vector<NPC*>* party, bool forceTravel = false, Room* forceDest = NULL) {
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
	//we move ourself to the next room
	currentRoom = roomCanidate;
	PrintRoomData(currentRoom); //prints the data of the current room
}

//initiates battle with an npc
void fight(Room* currentRoom, vector<NPC*>* party, vector<Item*>* inventory, char* name, int& mony) {
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
	} //creates the Battle! 
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
			if (teammate->getLevelUp()) {
				cout << teammate->getName() << " leveled up! " << teammate->getName() << " is now Level " << teammate->getLevel() << "!";
				teammate->setLevelUp(false); //marks level up as false so we don't say we leveled up every time we finish a battle
				CinPause();
				vector<int>& statChanges = teammate->getStatChanges();
				cout << "\n  HEALTH - " << teammate->getHealthMax(); //prints all the new stats of the npc
				if (statChanges[0]) {
					cout << " (+" << statChanges[0] << ")";
				}
				cout << "\t  DEFENSE - " << teammate->getDefense();
				if (statChanges[1]) {
					cout << " (+" << statChanges[1] << ")";
				}
				cout << "\n  ATTACK - " << teammate->getAttack();
				if (statChanges[2]) {
					cout << " (+" << statChanges[2] << ")";
				}
				cout << "\t  TOUGHNESS - " << teammate->getToughness();
				if (statChanges[3]) {
					cout << " (+" << statChanges[3] << ")";
				}
				cout << "\n  PIERCE - " << teammate->getPierce();
				if (statChanges[4]) {
					cout << " (+" << statChanges[4] << ")";
				}
				cout << "\t  SPEED - " << teammate->getSpeed();
				if (statChanges[5]) {
					cout << " (+" << statChanges[5] << ")";
				}
				cout << "\n  SKILL - " << teammate->getSPMax();
				if (statChanges[6]) {
					cout << " (+" << statChanges[6] << ")";
				}
				CinPause();
				Attack* newAtt = teammate->getNewAttack(); //checks if the teammate learned a new attack
				if (newAtt != NULL) { //print the attack and what it does
					cout << teammate->getName() << " learned " << newAtt->name << "!\n" << newAtt->name << " - " << newAtt->trueDesc;
					CinPause();
				}
			}
		}
		//sets the npc as defeated
		npc->defeat();
		//moves the npc to the room it was defeated in, if we were fighting it from a different room
		if (npc->getRoom(true) == currentRoom) {
			npc->setRoom(currentRoom);
		}
		//some NPCs have special fight endings after defeating them, so we do those checks here
		if (npc->getLobster()) { //if it was the lobster
			cout << "\nThe TUNNEL LOBSTER, now defeated, appears docile.";
			CinPause();
			cout << "TUNNEL LOBSTER - \"HhHhhhHHhhHhhHhHhhHhhhHHhh (docile lobster noises)\"";
			CinPause();
			//prints spaces based on player name length to format the instructions in the parentheses down there \/ \/ \/ \/ \/ \/ \/
			for (int i = 0; i < strlen((*party)[0]->getName()); i++) {
				cout << " ";
			}
			//gives the prompt to name the lobster!
			cout <<                              "\n                                                     (type your lobster's name here!)";
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
			} else if (!strcmp(name, "HELP") || !strcmp(name, "YOUR LOBSTER'S NAME") || !strcmp(name, "YOUR LOBSTER'S NAME HERE") || !strcmp(name, "YOUR LOBSTER'S NAME HERE!")) { //if the player followed the instructions too literally
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
		}
	}
	//prints the room data after battle so that the player can reorient themselves
	PrintRoomData(currentRoom);
}

//takes an item from the current room and adds it to the inventory
void takeItem(Room* currentRoom, vector<Item*>* inventory, char* itemname) {
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
	}
	//you're not allowed to take items not marked as takable
	if (!item->getTakable()) {
		cout << "\n";
		if (strcmp(item->getDenial(), "")) { //if there is a custom denial we use it
			cout <<  item->getDenial();
			return;
		} //otherwise say the generic denial
		cout << "You can't take the " << itemname << "!";
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
			cout << "\nAn exit DOWNwards was revealed!";
		}
	}
}

//drops an item from the inventory into the current room
void dropItem(Room* currentRoom, vector<Item*>* inventory, char* itemname) {
	Item* item = getItemInVector(*inventory, itemname); //finds the item in the inventory
	if (item == NULL) { //gives error message if we have no itemname
		cout << "\nYou have no \"" << itemname << "\"."; //I know ". is grammatically inaccurate but it looks way better than ."
		return;
	}
	item->setRoom(currentRoom); //puts the item in the current room
	//erases the item from the inventory
	inventory->erase(remove(inventory->begin(), inventory->end(), item), inventory->end());
	cout << "\nYou dropped the " << itemname << ".";
}

//uses an item, with functionality based on type
void useItem(Room*& currentRoom, vector<Item*>* inventory, vector<NPC*>* party, char* itemname, int& mony) {
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
		travel(currentRoom, NULL, party, true, lobster->getHome());
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
			if (!item->getTargetNeeded()) { //no need to clarify the target if one isn't required (we check for this because I thought the commands would look weird otherwise, like "USE SWITCH ON BOB", like what does Bob have to do with this? It's a switch!
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
	} else if (item->getTargetNeeded() && npc == NULL) { //if the item needed a target but no " ON " was given we give error text
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
		cout << "You are the winner you win you got the BURGER woo!";
		CinPause();
		cout << "This is the win condition because I need to turn this in on time.";
		CinPause();
		cout << "Congratulations you are now in the post-game!";
		CinPause();
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
			cout << "You're not LEVELED UP enough to use any of these new attacks. Keep training and you'll get there eventually!";
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
		vector<char*> exitsUnlocked; //the exits that were unlocked in the key using process, so we can unlock the other side also
		
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
			for (char* exit : exitsUnlocked) {
				Room* thatroom = currentRoom->getExit(exit); //gets the room on the other side of the exit
				//unblocks the reverse direction exit if it's blocked
				if (thatroom->getBlocked(const_cast<char*>(ReverseDirection[exit]))) {
					thatroom->unblockExit(ReverseDirection[exit]);
				}
			}
		} //if we didn't use it, we print an error message
		if (!used) {
			cout << "\nThere is nothing to use the " << itemname << " on here.";
			return;
		} //prints a description of what the key item did (because it's not only literal keys)
		cout << "\nYou " << key->getUseText();
	//movement items are used to go through a blocked exit despite the fact that it's blocked, for example a boat over a river
	} else if (!strcmp(item->getType(), "movement")) {
		MovementItem* mover = (MovementItem*)item; //converts to the corresponding subclass
		for (char* exit : currentRoom->getBlocks()) { //tries to find a blocked exit that matches the movement item's block type
			if (currentRoom->getBlockReason(exit) == mover->getUnlockType()) {
				cout << "\nYou " << mover->getUseText(); //prints what exactly the movement item did
				CinPause();
				travel(currentRoom, exit, party, true); //force travels to the found room
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
		cout << "\nYou " << paver->getUseText();
	//info items print some info
	} else if (!strcmp(item->getType(), "info")) {
		InfoItem* info = (InfoItem*)item; //converts to the corresponding subclass
		cout << "\n" << info->getText(); //prints the info
	//treasure chest items either give money or are trapped and start a battle
	} else if (!strcmp(item->getType(), "treasure")) {
		TreasureItem* treasure = (TreasureItem*)item; //converts to the corresponding subclass
		/*if (treasure->getMimic() != NULL) {
			fight(currentRoom, party, inventory, treasure->getMimic()->getName(), mony);
		}*/ //hidden bool for npcs?
		mony += treasure->getMony(); //adds the mony to the player's mony balance
		cout << "\nYou opened the " << itemname << " and got " << treasure->getMony() << " monies! You now have " << mony << " monies!"; //says how much they got and new balance
	//switch items are in one factory and switch the direction of all the conveyor belts
	} else if (!strcmp(item->getType(), "switch")) {
		ConveyorSwitch* cswitch = (ConveyorSwitch*)item; //converts to the corresponding subclass
		//switches all the conveyors
		for (Room* cveyor : cswitch->getConveyors()) {
			cveyor->switchConveyor();
		} //describes what just happened
		cout << "\nYou pulled the switch to the other side. All the assembly lines have switched directions!";
	//you can't use materials; they get a unique error message
	} else if (!strcmp(item->getType(), "material")) {
		cout << "\nYou can't use the " << itemname << "!";
		return;
	} else { //other types of items must be used in battles
		cout << "\nThe " << itemname << " can only be used in battle!";
		return;
	}
	//if the item is one-use only we delete the item
	if (item->getConsumable()) {
		deleteItem(currentRoom, inventory, item);
	}
}

//recruit an npc into the player party
void recruitNPC(Room* currentRoom, char* npcname, vector<NPC*>* party, int maxParty = 4) {
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
		cout << "\n" << npcname << " - \"" << npc->getRejectionDialogue() << "\"";
		cout << "\n" << npcname << " was not added to your party.";
		return;
	} //you can't rerecruit npcs, we don't want them being extra recruited
	if (npc->getRecruited()) {
		cout << "\n" << npcname << " is already in your party...";
		return;
	} //you're not allowed to have more than 4 party members (including yourself) otherwise that would be so unbalanced
	if (party->size() == maxParty) {
		cout << "\nYour party is full!";
		return;
	}
	//adds the npc to your party
	party->push_back(npc);
	npc->Recruit(); //sets the npc to recruited
	cout << "\n" << npcname << " was added to your party!" << "(party size: " << party->size() << "/" << maxParty << ")"; //prints success text
}

//decruit npcs from your party
void dismissNPC(Room* currentRoom, char* npcname, vector<NPC*>* party) {
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
	} else { //in every other room, they just say something and go back home
		cout << "\n" << npcname << " - \"" << npc->getDismissalDialogue() << "\"";
		cout << "\n" << npcname << " was removed from your party and returned to what they were doing before.";
	}
	//sets the npc to dismissed
	npc->Dismiss(!gym);
}

//prints anything the targeted npc has to say
void printNPCDialogue(Room* currentRoom, char* npcname, vector<Item*>* inventory, vector<NPC*>* party, int& mony) {
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

//prints the player's monies and inventory items
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

//prints the player's party
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

//analyzes either an item or npc of the given name
void analyze(Room* currentRoom, char* name, vector<NPC*>* party, vector<Item*>* inventory) {
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

//buys an item from the current room's catalogue
void buy(Room* currentRoom, vector<Item*>* inventory, char* name, int& mony) {
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

//prints all the available commands
void printHelp(char validCommands[16][255], char flavorText[16][255]) {
	cout << "\n"; //prints a random flavor text
	cout << flavorText[rand() % 16];
	cout << "\nValid commands:"; //prints all the valid commands
	for (int i = 0; i < 16; i++) {
		cout << "\n" << validCommands[i];
	}
}

//the main function where everything is called
int main() {
	srand(time(NULL)); //seeds random
		
	//sets up the game world and places the player at the current room
	NPC* self = SetupWorld();
	Room* currentRoom = self->getHome(); //sets the current room to the player's starting position

	vector<Item*>* inventory = new vector<Item*>; //the inventory of items
	vector<NPC*>* party = self->getParty(); //a pointer to the player's party

	int mony = 0; //monies are the currency in the BURGER QUEST universe.

	//flavor text printed by printHelp()
	char flavorText[16][255] = {
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
	char validCommands[16][255] = {
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
	cout << "Welcome to BURGER QUEST 2: ELECTRIC BOOGALOO\nYou're going on a quest to get a BURGER (not to be confused with a burger).\nType HELP for help.\n";

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
		CinPause();
	} else if (!strcmp(name, "HELP")) { //Help complains if you followed the previous instructions and typed HELP (for help)
		cout << "\nHELP - \"BRO are you serious? Now my name is Help... :(\"";
		CinPause();
	//if the player typed anything by following the instructions too literally, the main character remarks on that
	} else if (!strcmp(name, "HELP FOR HELP") || !strcmp(name, "YOUR NAME") || !strcmp(name, "YOUR NAME HERE") || !strcmp(name, "YOUR NAME HERE!")) {
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

		if (!strcmp(commandWord, "GO")) { //for going in a direction
			travel(currentRoom, &commandExtension[0], party);
		} else if (!strcmp(commandWord, "TAKE")) { //for taking an item
			takeItem(currentRoom, inventory, &commandExtension[0]);
		} else if (!strcmp(commandWord, "DROP")) { //for dropping an item
			dropItem(currentRoom, inventory, &commandExtension[0]);
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
		} else { //prints an error message if the player typed something that isn't an actual command
			cout << "\nInvalid command \"" << commandWord << "\" (type HELP for help).";
		}

		CinIgnoreAll(); //clears extra or faulty input
	}
	
	//gives a friendly farewell to the player
	cout << "\nEnjoy your next 24 hours.\n";

	//deletes all the text
	/*for (char* text : charsH) {
		delete[] text;
	} //deletes all the rooms
	for (Room* room : roomsH) {
		delete room;
	} //deletes all the items
	for (Item* item : itemsH) {
		delete item;
	} //deletes all the attacks
	for (Attack* attack : attacksH) {
		delete attack;
	} //deletes all the effects
	for (Effect* effect : effectsH) {
		delete effect;
	}*/
}

