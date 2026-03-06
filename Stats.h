//header file for stats

#ifndef STATS
#define STATS

#include <algorithm>

struct Stats {
    int hpmax, defense, attack, toughness, pierce, speed, spmax;

    Stats() : hpmax(0), defense(0), attack(0), toughness(0), pierce(0), speed(0), spmax(0) {}
    Stats(int hp, int def, int att, int tou, int pie, int spe, int sp) : hpmax(hp), defense(def), attack(att), toughness(tou), pierce(pie), speed(spe), spmax(sp) {}

    Stats& operator+(Stats left, const Stats& right) {
        left += right;
        return left;
    }

    Stats getStatScale(Stats basestats) { //calculate default scaling stats based on base stats
        return Stats();
    }
};

#endif