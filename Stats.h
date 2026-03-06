//header file for stats

#ifndef STATS
#define STATS

#include <algorithm>

struct Stats {
    int hpmax, defense, attack, toughness, pierce, speed, spmax;

    Stats() : hpmax(0), defense(0), attack(0), toughness(0), pierce(0), speed(0), spmax(0) {}
    Stats(int hp, int def, int att, int tou, int pie, int spe, int sp) : hpmax(hp), defense(def), attack(att), toughness(tou), pierce(pie), speed(spe), spmax(sp) {}

    Stats& operator+=(const Stats& other) {
        hpmax += other.hpmax;
        defense += other.defense;
        attack += other.attack;
        toughness += other.toughness;
        pierce += other.pierce;
        speed += other.speed;
        spmax += other.spmax;
        return *this;
    }

    int& operator[](int index) {
        switch (index) {
        case 0: return hpmax;
        case 1: return defense;
        case 2: return attack;
        case 3: return toughness;
        case 4: return pierce;
        case 5: return speed;
        case 6: return spmax;
        }
    }

    friend Stats operator+(Stats left, const Stats& right) {
        return left += right;
    }

    Stats getStatScale(const Stats& basestats) { //calculate default scaling stats based on base stats
        Stats scale = Stats(); //all stats start as 0
        
        //ok well here we have to figure out a system for creating default stats
        //probably three highest stats plus some bias away from health and pierce
        scale.spmax = 1; //sp max is guaranteed to be level + 10 because the values are pretty arbitrary otherwise

        return scale;
    }

    size_t hashStat(const int level, const int id, const int stat) {
        return 0;
    }

    static Stats makeLvlStats(const int level, const int id) {
        Stats stats = Stats();
        stats.hpmax     += hashStat(level, id, 1) % 2;
        stats.defense   += hashStat(level, id, 2) % 2;
        stats.attack    += hashStat(level, id, 3) % 2;
        stats.toughness += hashStat(level, id, 4) % 2;
        stats.pierce    += 0; //pierce doesn't update unless I manually set pierce scale, keeping it like the other ones just felt weird to me; you don't get more spiky the more you level up
        stats.speed     += hashStat(level, id, 6) % 2;
        stats.spmax     += 1;
        return stats;
    }
};

#endif