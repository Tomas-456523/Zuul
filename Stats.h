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
    }

    friend Stats operator+(Stats left, const Stats& right) {
        return left += right;
    }

    Stats getStatScale(const Stats& basestats) { //calculate default scaling stats based on base stats
        Stats scale = Stats(); //all stats start as 0
        int* stat1 = , stat2, stat3; //the three stats we give a guaranteed increase in
        return scale;
    }
};

#endif