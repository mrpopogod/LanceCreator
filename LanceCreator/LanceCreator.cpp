// LanceCreator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <list>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <algorithm>
#include <climits>

using namespace std;

struct Mech {
    string model_name;
    string variant_name;
    int bv;
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
    int min_bv_val = INT_MAX;
    int max_bv_val = INT_MIN;

    int min_bv()
    {
        if (min_bv_val == INT_MAX)
        {
            for (auto iter = variants.begin(); iter != variants.end(); iter++)
            {
                min_bv_val = min(min_bv_val, iter->bv);
            }
        }

        return min_bv_val;
    }

    int max_bv()
    {
        if (max_bv_val == INT_MIN)
        {
            for (auto iter = variants.begin(); iter != variants.end(); iter++)
            {
                max_bv_val = max(max_bv_val, iter->bv);
            }
        }

        return max_bv_val;
    }
};

bool compare_model(Model& lhs, Model& rhs)
{
    return lhs.name < rhs.name;
}

struct ModelForce {
    list<Model> models;
    string model_string;

    void sort()
    {
        models.sort(compare_model);
        stringstream ss;
        for (auto iter = models.begin(); iter != models.end(); iter++)
        {
            ss << iter->name;
        }

        model_string = ss.str();
    }

    int min_bv()
    {
        int bv = 0;
        for (auto iter = models.begin(); iter != models.end(); iter++)
        {
            bv += iter->min_bv();
        }

        return bv;
    }

    int max_bv()
    {
        int bv = 0;
        for (auto iter = models.begin(); iter != models.end(); iter++)
        {
            bv += iter->max_bv();
        }

        return bv;
    }

    bool operator<(const ModelForce& rhs) const
    {
        return model_string < rhs.model_string;
    }
};

struct Force {
	list<Mech> mechs;
    string mech_string;
    int bv = 0;

    void sort()
    {
        mechs.sort(compare_mech);
    }

    void calculate_bv()
    {
        bv = 0;
        stringstream ss;
        for (auto iter = mechs.begin(); iter != mechs.end(); iter++)
        {
            bv += iter->bv;
            ss << iter->model_name << iter->variant_name;
        }

        mech_string = ss.str();
    }

    bool operator<(const Force& rhs) const
    {
        return mech_string < rhs.mech_string;
    }
};

list<Model> models = {
    { 1, "Commando", { Mech({"Commando", "1B", 616}), Mech({"Commando", "1C", 458}), Mech({"Commando", "1D", 558}), Mech({"Commando", "2D", 541}), Mech({"Commando", "3A", 540}), Mech({"Commando", "5S", 557 }) } },
    { 2, "Spider", { Mech({"Spider", "5D", 524}), Mech({"Spider", "5K", 503}), Mech({"Spider", "5V", 622}), Mech({"Spider", "7M", 621}) } },
    { 2, "Jenner", { Mech({"Jenner", "D", 875}), Mech({"Jenner", "F", 1011}), Mech({"Jenner", "A", 712}), Mech({"Jenner", "K", 889}) } },
    { 2, "Panther", { Mech({"Panther", "8Z", 741}), Mech({"Panther", "9R", 769}), Mech({"Panther", "10K", 838}) } },
    { 1, "Assassin", { Mech({"Assassin", "21", 749}), Mech({"Assassin", "23", 740}) } },
    { 2, "Cicada", { Mech({"Cicada", "2A", 659}), Mech({"Cicada", "2B", 626}), Mech({"Cicada", "3C", 771}), Mech({"Cicada", "3F", 1329}), Mech({"Cicada", "3M", 812}) } },
    { 2, "Clint", { Mech({"Clint", "2-3T", 770}), Mech({"Clint", "2-3U", 1081}) } },
    { 2, "Hermes II", { Mech({"Heremes II", "2S", 784}), Mech({"Hermes II", "2M", 910}), Mech({"Hermes II", "4K", 976}), Mech({"Hermes II", "5S", 857}) } },
    { 1, "Whitworth", { Mech({"Whitworth", "1", 981}), Mech({"Whitworth", "1S", 917}), Mech({"Whitworth", "2", 932}) } },
    { 2, "Vindicator", { Mech({"Vindicator", "1R", 1024}), Mech({"Vindicator", "1SIC", 1020}), Mech({"Vindicator", "3L", 1105}) } },
    { 1 , "Enforcer", { Mech({"Enforcer", "4R", 1032}), Mech({"Enforcer", "5D", 1308}) } },
    { 1, "Hunchback", { Mech({"Hunchback", "4G", 1041}), Mech({"Hunchback", "4H", 1062}), Mech({"Hunchback", "4J", 1143}), Mech({"Hunchback", "4N", 1087}), Mech({"Hunchback", "4P", 1138}), Mech({"Hunchback", "4SP", 1043}), Mech({"Hunchback", "5M", 1056}) } },
    { 2, "Trebuchet", { Mech({"Trebuchet", "3C", 1342}), Mech({"Trebuchet", "5N", 1191}), Mech({"Trebuchet", "5J", 1191}), Mech({"Trebuchet", "5S", 984}), Mech({"Trebuchet", "7M", 1398}) } },
    { 2, "Dervish", { Mech({"Dervish", "6M", 1146}), Mech({"Dervish", "7D", 1412}) } },
    { 2, "Dragon", { Mech({"Dragon", "1N", 1125}), Mech({"Dragon", "1C", 1215}), Mech({"Dragon", "5N", 1223}) } },
    { 2, "Quickdraw", { Mech({"Quickdraw", "4G", 1192}), Mech({"Quickdraw", "4H", 1242}), Mech({"Quickdraw", "5A", 1196}), Mech({"Quickdraw", "5K", 1265}), Mech({"Quickdraw", "5M", 1237}) } },
    { 2, "Catapult", { Mech({"Catapult", "C1", 1399}), Mech({"Catapult", "A1", 1285}), Mech({"Catapult", "C4", 1358}), Mech({"Catapult", "K2", 1319}), Mech({"Catapult", "C1b", 1570}) } },
    { 1, "JagerMech", { Mech({"JagerMech", "S", 901}), Mech({"JagerMech", "A", 1122}), Mech({"JagerMech", "DD", 965}) } },
    { 2, "Grasshopper", { Mech({"Grasshopper", "5H", 1427}), Mech({"Grasshopper", "5N", 1511}), Mech({"Grasshopper", "5J", 1358}) } },
    { 2, "Awesome", { Mech({"Awesome", "8Q", 1605}), Mech({"Awesome", "8R", 1470}), Mech({"Awesome", "8T", 1593}), Mech({"Awesome", "8V", 1510}), Mech({"Awesome", "9M", 1812}) } },
    { 2, "Zeus", { Mech({"Zeus", "6S", 1374}), Mech({"Zeus", "5S", 1499}), Mech({"Zeus", "5T", 1729}), Mech({"Zeus", "6T", 1436}), Mech({"Zeus", "9S", 1639}), Mech({"Zeus", "9SS", 1769}) } },\
    { 2, "Cyclops", { Mech({"Cyclops", "10-Z", 1317}), Mech({"Cyclops", "10-q", 1584}), Mech({"Cyclops", "11-A", 1547}) } },
    { 2, "Banshee", { Mech({"Banshee", "3E", 1422}), Mech({"Banshee", "3M", 1595}), Mech({"Banshee", "3Q", 1394}), Mech({"Banshee", "3S", 1751}), Mech({"Banshee", "3MC", 1487}) } },
    { 2, "Atlas", { Mech({"Atlas", "D", 1897}), Mech({"Atlas", "RS", 1849}), Mech({"Atlas", "A", 1787}), Mech({"Atlas", "K", 2175}) } },
    { 1, "BattleMaster", { Mech({"BattleMaster", "1G", 1519}), Mech({"BattleMaster", "1Gb", 1825}), Mech({"BattleMaster", "1D", 1522}), Mech({"BattleMaster", "1S", 1507}), Mech({"BattleMaster", "3M", 1697}), Mech({"BattleMaster", "3S", 1441}) } },

};

// Lance version.  At some point create a Star and L2 version (ideally I would do it in a way that lets me set
// just a number, but as for now it's harder than I'd like to do so.
int main()
{
    int max_bv = 5000;
    float under_percentage = 0.95f;
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
    // Also cuts any lances that can't fit within the bounds (max too low or min too high)
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
                    // Could optimize lines, but this is most readable regarding the conditions
                    if (model_force.min_bv() > max_bv)
                    {
                        continue;
                    }
                    else if (model_force.max_bv() < max_bv * under_percentage)
                    {
                        continue;
                    }
                    else
                    {
                        model_force_set.insert(model_force);
                    }
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
        else if (iter->bv < max_bv * under_percentage)
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

