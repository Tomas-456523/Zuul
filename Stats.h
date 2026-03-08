//header file for stats

#ifndef STATS
#define STATS

#include <algorithm>

struct Stats {
    int hpmax, defense, attack, toughness, pierce, speed, spmax;

    Stats() : hpmax(0), defense(0), attack(0), toughness(0), pierce(0), speed(0), spmax(0) {}
    Stats(int hp, int def, int att, int tou, int pie, int spe, int sp) : hpmax(hp), defense(def), attack(att), toughness(tou), pierce(pie), speed(spe), spmax(sp) {}

    Stats& operator+=(const Stats& other) { //when you add stats it just adds the individual stats together
        hpmax += other.hpmax;
        defense += other.defense;
        attack += other.attack;
        toughness += other.toughness;
        pierce += other.pierce;
        speed += other.speed;
        spmax += other.spmax;
        return *this; //return this as value to allow += chains
    }

    friend Stats operator+(Stats left, const Stats& right) { //for doing stats1 + stats2
        return left += right;
    }

    Stats& operator*=(int other) { //when you multiply a stats by an integer it multlies all the stats by that amount
        hpmax *= other;
        defense *= other;
        attack *= other;
        toughness *= other;
        pierce *= other;
        speed *= other;
        spmax *= other;
        return *this; //return this as value to allow *= chains
    }

    friend Stats operator*(Stats left, int right) { //for doing stats*amount
        return left *= right;
    }

    int& operator[](int index) { //make stat getting easier using indexing!
        switch (index) {
        case 0: return hpmax;
        case 1: return defense;
        case 2: return attack;
        case 3: return toughness;
        case 4: return pierce;
        case 5: return speed;
        default: return spmax; //I made this default instead of case 6 because the compiler was warning me
        }
    }

    static Stats avgLvLUp(int levels) { //makes a new stats object equal to the average amount of stats randomly going up for the given amount of levels
        Stats stats = Stats();
        stats.pierce = 0; //pierce doesn't change on level up usually
        stats.spmax  = levels; //spmax always goes up by one per level
        stats.hpmax = stats.defense = stats.attack = stats.toughness = stats.speed = levels / 2; //everything else goes up 1 or 0 times, averaging to 1/2 per level up
        return stats; //return the stats we just made!
    }

    

    static Stats getStatScale(const Stats& basestats) { //calculate default scaling stats based on base stats
        Stats scale = Stats(); //all stats start as 0
        Stats score = basestats; //the score for each stat
        score.hpmax /= 1.5f; //bias away from health and pierce
        score.pierce /= 1.5f;
        size_t win1 = 1, win2 = 2; //defense and attack are default winners in case of a tie, win1 is treated as bigger in the check
        for (size_t i = 0; i < 6; i++) { //look through all the scores (ignoring sp) to find the two highest
            if (score[i] > score[win1]) { //if the score is greater than the first winner, cycle win1 down to win2 and set win1 to the new winner
                win2 = win1;
                win1 = i;
            } else if (i != win1 && score[i] > score[win2]) { //if the score is only greater than win2 (and isn't just win1), just set that one
                win2 = i;
            }
        } //set the winners (and sp) to a scale of 1!
        scale[win1] = scale[win2] = scale.spmax = 1; //sp max defaults to (start amount + level) because the values are pretty arbitrary otherwise
        return scale; //return the scale!
    }

    static size_t hashStat(const int level, const int id, const int stat) { //used by stat generators to make stats that look random yet are deterministic
        size_t hash = id; //start hashing using the id
        //xor the hash with (the level plus a constant that is known to make good random-looking hashes, then added to the hash with the first two bits on the right (to mix the hash bits))
        hash ^= level + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        return hash ^ (stat + 0x9e3779b9 + (hash) + (hash >> 2)); //then do it again but with stat to mix more and include stat in the hashing, then return what we got
    }

    static Stats genBaseStats(const int id) { //deterministically generate base stats based on id for npcs that never enter battle, so I don't have to set like a billion useless stats
        Stats stats = Stats();
        stats.hpmax     = hashStat(4524354, id, 0) % 10 + 10; //this was based off of makeLvlStats, but this doesn't pass level so I added these random numbers just to use something else for a hash
        stats.defense   = hashStat(345, id, 1) % 10 + 4;
        stats.attack    = hashStat(55343, id, 2) % 10 + 1;
        stats.toughness = hashStat(345, id, 3) % 20;
        stats.pierce    = 0; //most people aren't physically sharp
        stats.speed     = hashStat(345353, id, 5) % 10 + 2;
        stats.spmax     = 1;
        return stats;
    }

    static Stats makeLvlStats(const int level, const int id) {
        Stats stats = Stats();
        stats.hpmax     = hashStat(level, id, 0) & 1;
        stats.defense   = hashStat(level, id, 1) & 1;
        stats.attack    = hashStat(level, id, 2) & 1;
        stats.toughness = hashStat(level, id, 3) & 1;
        stats.pierce    = 0; //pierce doesn't update unless I manually set pierce scale, keeping it like the other ones just felt weird to me; you don't get more spiky the more you level up
        stats.speed     = hashStat(level, id, 5) & 1;
        stats.spmax     = 1;
        return stats;
    }

    bool empty() const { //check if the stats instance is empty, meaning all the stats were left unchanged from 0
        return !hpmax && !defense && !attack && !toughness && !pierce && !speed && !spmax;
    }
};

#endif