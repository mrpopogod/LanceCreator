// LanceCreator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <list>
#include <map>
#include <iostream>
#include <fstream>
#include <set>
#include <algorithm>
#include <climits>

using namespace std;

struct Model {
	int count;
    string name;
	list<Mech> variants;

    int lowest_bv() {
        static int bv = INT_MAX;
        static int size = 0;
        if (variants.size() > size) {
            for (auto iter = variants.begin(); iter != variants.end(); iter++)
            {
                bv = min(bv, iter->bv);
            }
        }

        return bv;
    }
};

struct Mech {
    string model_name;
    string va_list;
	int bv;
};

struct Force {
	list<Mech> mechs;

	int bv() {
		static int bv = 0;
		static int size = 0;
		if (mechs.size() > size) {
			for (auto iter = mechs.begin(); iter != mechs.end(); iter++) 
            {
				bv += iter->bv;
			}

			size = mechs.size();
		}

		return bv;
	}

	bool operator==(const Force& rhs)
	{
        mechs.sort(compare_mech);
		return (mechs == rhs.mechs);
	}
};

list<Model> models = {
	{ 2, { Mech({"Commando", "2D", 541}), Mech({"Commando", "5S", 557 }) } },
	{ 1, { Mech({"Spider", "5V", 622}), Mech({"Spider", "7M", 621}) } }
};

bool compare_force(Force& lhs, Force& rhs) 
{
	return lhs.bv() < rhs.bv();
}

// lowest bv first, then alphabetical
bool compare_mech(Mech& lhs, Mech& rhs)
{
	if (lhs.bv < rhs.bv) {
		return true;
	}

	if (rhs.bv > lhs.bv) {
		return false;
	}

	return lhs.name < rhs.name;
}

// Next iteration idea;
// A structure that contains one or more mechs; it defines number of models I have and all the variants
// Then this loop goes through that structure and populates a series of lances with all combinations 
// (e.g. if I have two Atlas models and define the D, K, and S, it would create a lance with a D, a lance
// with a K, a lance with an S, a lance with a D and a K, with a D and an S, and a K and an S).  This 
// functionality would require building up on each lance as it gets filled out, so probably something recursive.
// And bail out once the lance gets to size 4.
//
// So you have a function that takes in a Lance structure and the current offset in the mech list, it bails out
// if the lance is full, then it goes through the current item in the mech list, adds all appropriate combinations
// to the lance and then recursively calls itself with the next offset and all the lances.

// Here's a new idea of how this algortihm should work.  First we create a set of forces of models.  The list of models 
// will have duplicate entries for having multiple of a model and we do a dumb creation and let the ordering of mechs
// in the force be simple alphabetical and the set remove duplicates.
// Stage two is we go to each model in the force and explode it into multiple forces, one for each variant.
// Stage three is toss all those forces into a set to dedupe
// Stage four is we cut down by BV
// I should just fucking restrict this to lance, then create variants that do star and L2

int main()
{
    int max_bv = 5000;
	int force_size = 4;
    map<int, list<Force>> force_map;
    set<Force> force_set;
    map<Model, int> model_count_map;
	
	for (auto model_iter = models.begin(); model_iter != models.end(); model_iter++)
	{
        model_count_map[*model_iter] = model_iter->count;
	}




    for (int i = 0; i < 24; i++)
    {
        for (int j = i + 1; j < 24; j++)
        {
            for (int k = j + 1; k < 24; k++)
            {
                for (int l = k + 1; l < 24; l++)
                {
                    Force lance;
					lance.mechs.push_back(mechs[i]);
					lance.mechs.push_back(mechs[j]);
					lance.mechs.push_back(mechs[k]);
					lance.mechs.push_back(mechs[l]);
                    if (lance.bv() < max_bv - 150) {
                        continue;
                    }
                    else if (lance.bv() > max_bv) {
                        continue;
                    }

                    force_list.push_back(lance);
                    force_map[lance.bv()].push_back(lance);
                }
            }
        }
    }

    force_list.sort(compare_force);

    ofstream file;
    file.open("lances.csv");

    file << "Lances small to big" << endl;
    for (auto iter = force_list.begin(); iter != force_list.end(); iter++)
    {
		for (auto mech_iter = iter->mechs.begin(); mech_iter != iter->mechs.end(); mech_iter++)
		{
			file << mech_iter->name << ",";
		}

		file << iter->bv() << endl;
    }
    
    file << endl;

    file << "Lances with equal BV" << endl;
    for (auto iter = force_map.begin(); iter != force_map.end(); iter++)
    {
        list<Force> lances = iter->second;
        if (lances.size() < 2) 
        {
            continue;
        }

        file << "BV: " << iter->first << endl;
        for (auto liter = lances.begin(); liter != lances.end(); liter++)
        {
			for (auto mech_iter = liter->mechs.begin(); mech_iter != liter->mechs.end(); mech_iter++)
			{
				file << mech_iter->name << ",";
			}

			file << liter->bv() << endl;
        }

        file << endl;
    }

    file.close();

    system("pause");

    return 0;
}

