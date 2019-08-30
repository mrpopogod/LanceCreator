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

struct Mech {
    string model_name;
    string variant_name;
    int bv;

    bool operator==(const Mech& rhs) const
    {
        return model_name == rhs.model_name && variant_name == rhs.variant_name && bv == rhs.bv;
    }
};

// name, then variant
bool compare_mech(Mech& lhs, Mech& rhs)
{
    if (lhs.model_name == rhs.model_name)
    {
        return lhs.variant_name < rhs.variant_name;
    }
    else
    {
        return lhs.model_name < rhs.model_name;
    }
}

struct Model {
	int count;
    string name;
	list<Mech> variants;

    bool operator==(const Model& rhs) const
    {
        return count == rhs.count && name == rhs.name && variants == rhs.variants;
    }
};

bool compare_model(Model& lhs, Model& rhs)
{
    return lhs.name < rhs.name;
}

struct ModelForce {
    list<Model> models;

    void sort()
    {
        models.sort(compare_model);
    }

    bool operator==(const ModelForce& rhs) const
    {
        return (models == rhs.models);
    }

    bool operator<(const ModelForce& rhs) const
    {
        return models.front().name < rhs.models.front().name;
    }
};

struct Force {
	list<Mech> mechs;
    int bv = 0;

    void sort()
    {
        mechs.sort(compare_mech);
    }

    void calculate_bv()
    {
        bv = 0;
        for (auto iter = mechs.begin(); iter != mechs.end(); iter++)
        {
            bv += iter->bv;
        }
    }

	bool operator==(const Force& rhs) const
	{
		return (mechs == rhs.mechs);
	}

    bool operator<(const Force& rhs) const
    {
        return bv < rhs.bv;
    }
};

list<Model> models = {
    { 2, "Commando", { Mech({"Commando", "1B", 616}), Mech({"Commando", "1C", 458}), Mech({"Commando", "1D", 558}), Mech({"Commando", "2D", 541}), Mech({"Commando", "3A", 540}), Mech({"Commando", "5S", 557 }) } },
    { 2, "Spider", { Mech({"Spider", "5D", 524}), Mech({"Spider", "5K", 503}), Mech({"Spider", "5V", 622}), Mech({"Spider", "7M", 621}) } },
    { 2, "Jenner", { Mech({"Jenner", "D", 875}), Mech({"Jenner", "F", 1011}), Mech({"Jenner", "K", 889}) } },
    { 2, "Panther", { Mech({"Panther", "8Z", 741}), Mech({"Panther", "9R", 769}), Mech({"Panther", "10K", 838}) } },
    { 1, "Assassin", { Mech({"Assassin", "21", 749}), Mech({"Assassin", "23", 740}) } },
    { 2, "Cicada", { Mech({"Cicada", "2A", 659}), Mech({"Cicada", "2B", 626}), Mech({"Cicada", "3C", 771}), Mech({"Cicada", "3F", 1329}), Mech({"Cicada", "3M", 812}) } },
    { 2, "Clint", { Mech({"Clint", "2-3T", 770}), Mech({"Clint", "2-3U", 1081}), Mech({"Clint", "2-4T", 619}) } },

};

// Lance version.  At some point create a Star and L2 version (ideally I would do it in a way that lets me set
// just a number, but as for now it's harder than I'd like to do so.
int main()
{
    int max_bv = 3000;
    set<Force> force_set;
    list<Model> models_with_duplicates;
    set<ModelForce> model_force_set;

    // Create the duplicates that we use to force create all the possible basic forces based on models
	for (auto model_iter = models.begin(); model_iter != models.end(); model_iter++)
	{
        for (int i = 0; i < model_iter->count; i++)
        {
            models_with_duplicates.push_back(*model_iter);
        }
	}

    // Create the model forces; each of these would then be exploded into the actual specific variants
    for (auto iter1 = models_with_duplicates.begin(); iter1 != models_with_duplicates.end(); iter1++)
    {
        auto iter2 = iter1;
        iter2++;
        for (iter2; iter2 != models_with_duplicates.end(); iter2++)
        {
            auto iter3 = iter2;
            iter3++;
            for (iter3; iter3 != models_with_duplicates.end(); iter3++)
            {
                auto iter4 = iter3;
                iter4++;
                for (iter4; iter4 != models_with_duplicates.end(); iter4++)
                {
                    ModelForce model_force;
                    model_force.models.push_back(*iter1);
                    model_force.models.push_back(*iter2);
                    model_force.models.push_back(*iter3);
                    model_force.models.push_back(*iter4);
                    model_force.sort();
                    model_force_set.insert(model_force);
                }
            }
        }
    }

    // Go through each model force and explode it
    for (auto force_iter = model_force_set.begin(); force_iter != model_force_set.end(); force_iter++)
    {
        auto model_iter = force_iter->models.begin();
        auto vhead1 = model_iter->variants.begin();
        auto vend1 = model_iter->variants.end();
        model_iter++;
        auto vhead2 = model_iter->variants.begin();
        auto vend2 = model_iter->variants.end();
        model_iter++;
        auto vhead3 = model_iter->variants.begin();
        auto vend3 = model_iter->variants.end();
        model_iter++;
        auto vhead4 = model_iter->variants.begin();
        auto vend4 = model_iter->variants.end();
        for (auto viter1 = vhead1; viter1 != vend1; viter1++)
        {
            for (auto viter2 = vhead2; viter2 != vend2; viter2++)
            {
                for (auto viter3 = vhead3; viter3 != vend3; viter3++)
                {
                    for (auto viter4 = vhead4; viter4 != vend4; viter4++)
                    {
                        // create specific force
                        Force force;
                        force.mechs.push_back(*viter1);
                        force.mechs.push_back(*viter2);
                        force.mechs.push_back(*viter3);
                        force.mechs.push_back(*viter4);
                        force.sort();
                        force.calculate_bv();
                        force_set.insert(force);
                    }
                }
            }
        }
    }

    // cut down by BV
    for (auto iter = force_set.begin(); iter != force_set.end(); )
    {
        if (iter->bv > max_bv)
        {
            iter = force_set.erase(iter);
        }
        else if (iter->bv < max_bv * 0.95)
        {
            iter = force_set.erase(iter);
        }
        else
        {
            iter++;
        }
    }

    ofstream file;
    file.open("lances.csv");

    file << "Lances small to big" << endl;
    for (auto iter = force_set.begin(); iter != force_set.end(); iter++)
    {
		for (auto mech_iter = iter->mechs.begin(); mech_iter != iter->mechs.end(); mech_iter++)
		{
			file << mech_iter->model_name << "-" << mech_iter->variant_name << ",";
		}
        
        file << iter->bv << endl;
    }

    file.close();

    system("pause");

    return 0;
}

