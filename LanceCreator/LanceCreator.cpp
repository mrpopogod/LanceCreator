// LanceCreator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <list>
#include <map>
#include <iostream>
#include <fstream>

using namespace std;

struct Mech {
    string name;
    int bv;
};

struct Lance {
    Mech light;
    Mech medium;
    Mech heavy;
    Mech assault;

    int bv() {
        return light.bv + medium.bv + heavy.bv + assault.bv;
    }
};

Mech light[] = {
    { "Commando", 541 },
    { "Spider", 622 },
    { "Jenner", 875 },
    { "Panther", 769 }
};

Mech medium[] = {
    { "Assassin", 749 },
    { "Cicada", 659 },
    { "Clint", 770 },
    { "Hermes II", 784 },
    { "Whitworth", 982 },
    { "Vindicator", 1024 },
    { "Enforcer", 1032 },
    { "Hunchback", 1041 },
    { "Trebuchet", 1191 },
    { "Dervish", 1146 }
};

Mech heavy[] = {
    { "Dragon", 1125 },
    { "Quickdraw", 1192 },
    { "Catapult", 1399 },
    { "Jagermech", 901 },
    { "Grasshopper", 1427 }
};

Mech assault[] = {
    { "Awesome", 1605 },
    { "Zeus", 1374 },
    { "Cyclops", 1308 },
    { "Banshee", 1422 },
    { "Atlas", 1897 }
};

bool compare_lance(Lance& lhs, Lance& rhs) {
    return lhs.bv() < rhs.bv();
}

int main()
{
    map<int, list<Lance>> lance_map;
    list<Lance> lance_list;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            for (int k = 0; k < 5; k++)
            {
                for (int l = 0; l < 5; l++)
                {
                    Lance lance;
                    lance.light = light[i];
                    lance.medium = medium[j];
                    lance.heavy = heavy[k];
                    lance.assault = assault[l];
                    lance_list.push_back(lance);
                    lance_map[lance.bv()].push_back(lance);
                }
            }
        }
    }

    lance_list.sort(compare_lance);

    ofstream file;
    file.open("lances.csv");

    file << "Lances small to big" << endl;
    for (auto iter = lance_list.begin(); iter != lance_list.end(); iter++)
    {
        file << iter->light.name << ","
            << iter->medium.name << "," 
            << iter->heavy.name << "," 
            << iter->assault.name << "," 
            << iter->bv() << endl;
    }
    
    file << endl;

    file << "Lances with equal BV" << endl;
    for (auto iter = lance_map.begin(); iter != lance_map.end(); iter++)
    {
        list<Lance> lances = iter->second;
        if (lances.size() < 2) 
        {
            continue;
        }

        file << "BV: " << iter->first << endl;
        for (auto liter = lances.begin(); liter != lances.end(); liter++)
        {
            file << liter->light.name << ","
                << liter->medium.name << ","
                << liter->heavy.name << ","
                << liter->assault.name << endl;
        }

        file << endl;
    }

    file.close();

    system("pause");

    return 0;
}

