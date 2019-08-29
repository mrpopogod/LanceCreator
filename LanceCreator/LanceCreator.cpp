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
    Mech first;
    Mech second;
    Mech third;
    Mech fourth;

    int bv() {
        return first.bv + second.bv + third.bv + fourth.bv;
    }
};

Mech mechs[] = {
    { "Commando", 541 },
    { "Spider", 622 },
    { "Jenner", 875 },
    { "Panther", 769 },
    { "Assassin", 749 },
    { "Cicada", 659 },
    { "Clint", 770 },
    { "Hermes II", 784 },
    { "Whitworth", 982 },
    { "Vindicator", 1024 },
    { "Enforcer", 1032 },
    { "Hunchback", 1041 },
    { "Trebuchet", 1191 },
    { "Dervish", 1146 },
    { "Dragon", 1125 },
    { "Quickdraw", 1192 },
    { "Catapult", 1399 },
    { "Jagermech", 901 },
    { "Grasshopper", 1427 },
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
    int max_bv = 5000;
    map<int, list<Lance>> lance_map;
    list<Lance> lance_list;
    for (int i = 0; i < 24; i++)
    {
        for (int j = i + 1; j < 24; j++)
        {
            for (int k = j + 1; k < 24; k++)
            {
                for (int l = k + 1; l < 24; l++)
                {
                    Lance lance;
                    lance.first = mechs[i];
                    lance.second = mechs[j];
                    lance.third = mechs[k];
                    lance.fourth = mechs[l];
                    if (lance.bv() < max_bv * 0.9) {
                        continue;
                    }
                    else if (lance.bv() > max_bv) {
                        continue;
                    }

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
        file << iter->first.name << ","
            << iter->second.name << "," 
            << iter->third.name << "," 
            << iter->fourth.name << "," 
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
            file << liter->first.name << ","
                << liter->second.name << ","
                << liter->third.name << ","
                << liter->fourth.name << endl;
        }

        file << endl;
    }

    file.close();

    system("pause");

    return 0;
}

