// LanceCreator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <cstdio>
#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <algorithm>
#include <climits>
#include <random>
#include <chrono>
#include <initializer_list>

using namespace std;
using namespace rapidjson;

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
            for (auto& mech : variants)
            {
                min_bv_val = min(min_bv_val, mech.bv);
            }
        }

        return min_bv_val;
    }

    int max_bv()
    {
        if (max_bv_val == INT_MIN)
        {
            for (auto& mech : variants)
            {
                max_bv_val = max(max_bv_val, mech.bv);
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
        for (auto& mech : models)
        {
            ss << mech.name << ",";
        }

        model_string = ss.str();
        model_string.pop_back();
    }

    int min_bv()
    {
        int bv = 0;
        for (auto& mech : models)
        {
            bv += mech.min_bv();
        }

        return bv;
    }

    int max_bv()
    {
        int bv = 0;
        for (auto& mech : models)
        {
            bv += mech.max_bv();
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
        for (auto& mech : mechs)
        {
            bv += mech.bv;
            ss << mech.model_name << mech.variant_name;
        }

        mech_string = ss.str();
    }

    bool operator<(const Force& rhs) const
    {
        return mech_string < rhs.mech_string;
    }
};

list<Model> is_models = {
    { 1, "Commando", { Mech({"Commando", "1B", 616}), Mech({"Commando", "1C", 458}), Mech({"Commando", "1D", 558}), Mech({"Commando", "2D", 541}), Mech({"Commando", "3A", 540}), Mech({"Commando", "5S", 557 }) } },
    { 1, "Catapult", { Mech({"Catapult", "C1", 1399}), Mech({"Catapult", "A1", 1285}), Mech({"Catapult", "C4", 1358}), Mech({"Catapult", "K2", 1319}), Mech({"Catapult", "C1b", 1570}) } },
    { 1, "BattleMaster", { Mech({"BattleMaster", "1G", 1519}), Mech({"BattleMaster", "1Gb", 1825}), Mech({"BattleMaster", "1D", 1522}), Mech({"BattleMaster", "1S", 1507}), Mech({"BattleMaster", "3M", 1697}), Mech({"BattleMaster", "3S", 1441}) } },
    { 1, "Awesome", { Mech({"Awesome", "8Q", 1605}), Mech({"Awesome", "8R", 1470}), Mech({"Awesome", "8T", 1593}), Mech({"Awesome", "8V", 1510}), Mech({"Awesome", "9M", 1812}) } },
    { 1, "Locust", { Mech({"Locust", "1V", 432}), Mech({"Locust", "1E", 553}), Mech({"Locust", "1M", 424}), Mech({"Locust", "1S", 440}), Mech({"Locust", "1V", 642}), Mech({"Locust", "3D", 436}), Mech({"Locust", "3M", 522}), Mech({"Locust", "3S", 483}), Mech({"Locust", "3V", 490}) } },
    { 1, "Shadow Hawk", { Mech({"Shadow Hawk", "2H", 1064}), Mech({"Shadow Hawk", "2H", 1354}), Mech({"Shadow Hawk", "2K", 1147}), Mech({"Shadow Hawk", "2D", 899}), Mech({"Shadow Hawk", "2D2", 1049}), Mech({"Shadow Hawk", "5M", 1430}) } },
    { 1, "Griffin", { Mech({"Griffin", "1N", 1272}), Mech({"Griffin", "1DS", 1285}), Mech({"Griffin", "1S", 1253}), Mech({"Griffin", "2N", 1606}), Mech({"Griffin", "3M", 1521}) } },
    { 2, "Wolverine", { Mech({"Wolverine", "6R", 1101}), Mech({"Wolverine", "6K", 1248}), Mech({"Wolverine", "6M", 1291}), Mech({"Wolverine", "7D", 1314}), Mech({"Wolverine", "7H", 1301}), Mech({"Wolverine", "7K", 1331}), Mech({"Wolverine", "7M", 1673}) } },
    { 1, "Thunderbolt", { Mech({"Thunderbolt", "5S", 1335}), Mech({"Thunderbolt", "5L", 1546}), Mech({"Thunderbolt", "5LS", 1305}), Mech({"Thunderbolt", "5Sb", 1618}), Mech({"Thunderbolt", "5SE", 1414}), Mech({"Thunderbolt", "5SS", 1337}), Mech({"Thunderbolt", "7M", 1495}), Mech({"Thunderbolt", "9S", 1494}), Mech({"Thunderbolt", "9SE", 1439}) } }
};

list<Model> old_models = {
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
    { 1, "Enforcer", { Mech({"Enforcer", "4R", 1032}), Mech({"Enforcer", "5D", 1308}) } },
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
    { 1, "Centurion", { Mech({"Centurion", "A", 945}), Mech({"Centurion", "AH", 945}), Mech({"Centurion", "AL", 1057}), Mech({"Centurion", "D", 1130}) } },
    { 1, "Wolfhound", { Mech({"Wolfhound", "1", 949}), Mech({"Wolfhound", "1A", 967}), Mech({"Wolfhound", "1B", 967}), Mech({"Wolfhound", "2", 1061}) } },
    { 1, "Flea", { Mech({"Flea", "4", 432}), Mech({"Flea", "15", 430}), Mech({"Flea", "16", 506}), Mech({"Flea", "17", 510}) } },
    { 1, "Vulcan", { Mech({"Vulcan", "2T", 642}), Mech({"Vulcan", "5T", 942}), Mech({"Vulcan", "5M", 811}), Mech({"Vulcan", "5S", 883}) } },
    { 1, "Hatchetman", { Mech({"Hatchetman", "3F", 854}), Mech({"Hatchetman", "5S", 1039}) } },
    { 1, "Orion", { Mech({"Orion", "K", 1429}), Mech({"Orion", "V", 1298}), Mech({"Orion", "VA", 1329}), Mech({"Orion", "M", 1414}),  Mech({"Orion", "MA", 1501}) } },
    { 1, "Charger", { Mech({"Charger", "1A1", 981}), Mech({"Charger", "1L", 980}), Mech({"Charger", "1A5", 1468}), Mech({"Charger", "1A9", 1397}), Mech({"Charger", "3K", 1656}) } },
    { 1, "Firestarter", { Mech({"Firestarter", "A", 773}), Mech({"Firestarter", "K", 773}), Mech({"Firestarter", "H", 773}), Mech({"Firestarter", "M", 798}), Mech({"Firestarter", "S1", 820}) } },
    { 1, "Blackjack", { Mech({"Blackjack", "1X", 964}), Mech({"Blackjack", "1", 949}), Mech({"Blackjack", "1DB", 1015}), Mech({"Blackjack", "1DC", 917}), Mech({"Blackjack", "3", 1271}), Mech({"Blackjack", "2", 1148}) } },
    { 1, "Victor", { Mech({"Victor", "9A", 1236}), Mech({"Victor", "9A1", 1306}), Mech({"Victor", "9B", 1378}), Mech({"Victor", "9S", 1360}), Mech({"Victor", "9D", 1717}) } },
    { 1, "Shogun", { Mech({"Shogun", "2H", 2087}), Mech({"Shogun", "2E", 1823}), Mech({"Shogun", "2F", 1804}) } },
    { 1, "Stalker", { Mech({"Stalker", "3F", 1559}), Mech({"Stalker", "3H", 1624}), Mech({"Stalker", "3Fb", 2029}), Mech({"Stalker", "4N", 1558}), Mech({"Stalker", "5M", 1655}), Mech({"Stalker", "5S", 1423}) } },
    { 1, "Cataphract", { Mech({"Cataphract", "1X", 1316}), Mech({"Cataphract", "2X", 1344}), Mech({"Cataphract", "3D", 1325}), Mech({"Cataphract", "3L", 1545}) } },
    { 1, "Raven", { Mech({"Raven", "2X", 887}), Mech({"Raven", "4X", 820}), Mech({"Raven", "3L", 708}) } },
    { 1, "Guillotine", { Mech({"Guillotine", "3N", 1418}), Mech({"Guillotine", "4L", 1400}), Mech({"Guillotine", "4P", 1376}), Mech({"Guillotine", "5M", 1472}) } }
};

Force build_force(initializer_list<Mech> mechs)
{
    Force force;
    for (auto& mech : mechs)
    {
        force.mechs.push_back(mech);
    }

    force.sort();
    force.calculate_bv();
    return force;
}

void print_usage()
{
    cout << "Usage: " << endl
        << "LanceCreator [--maxBV bv] [--forceSize size] [--numForces num] [--underPercentage percentage] [--modelSet set]" << endl << endl
        << "maxBV - The maximum BV a force can have" << endl
        << "forceSize - The numer of units in a force" << endl
        << "numForces - The number of compositions of units in a force to generate and run through permutations of variants on" << endl
        << "underPercentage - The lowest percentage of maxBV the force must be (e.g. 0.95 means a force will have 95% of maxBV or more" << endl
        << "modelSet - The model set to use, valid values are OLD, IS" << endl;
}

Document load_json_from_file(string& filename) 
{
    FILE* fp = fopen(filename.c_str(), "rb");
    char readBuffer[65536];
    FileReadStream is(fp, readBuffer, sizeof(readBuffer));

    Document d;
    d.ParseStream(is);
    fclose(fp);

    return d;
}

Model parse_model(Value& value)
{
    Model model;
    model.name = value["name"].GetString();
    model.count = value["count"].GetInt();
    for (auto& v : value["variants"].GetArray())
    {
        Mech mech;
        mech.model_name = v["model"].GetString();
        mech.variant_name = v["variant"].GetString();
        mech.bv = v["bv"].GetInt();
        model.variants.push_back(mech);
    }

    return model;
}

list<Model> parse_models(Document& document)
{
    list<Model> models;
    for (auto& v : document.GetArray())
    {
        models.push_back(parse_model(v));
    }

    return models;
}

// Generate a series of potential forces using the configured set of mech models.
// Supports up to a force size of 6, so can do lances, stars, and L2s.
// Set the models list to be one of the pregenerated lists
// Starts by generating a number of forces that are buildable with the available models,
// then creates all the combinations of the canonical variants of those models (e.g. AS7-D vs. AS7-K)
// By scoping the number of combinations down we avoid an untenable combinatoric explosion.
// All forces will be within a particular specified BV, never over, and never under the cutoff (so
// we don't have to hit the exact BV but don't give up too much as well).
int main(int argc, char** argv)
{
    int max_bv = -1;
    int force_size = -1;
    int num_model_forces = -1;
    float under_percentage = -1.0;
    list<Model> *models = nullptr;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--help") == 0)
        {
            print_usage();
            exit(0);
        }
        else if (strcmp(argv[i], "--maxBV") == 0)
        {
            if (i + 1 < argc)
            {
                i++;
                max_bv = atoi(argv[i]);
                if (max_bv < 1)
                {
                    cout << "Max BV must be a positive number" << endl;
                    exit(1);
                }
            }
            else
            {
                print_usage();
                exit(1);
            }
        }
        else if (strcmp(argv[i], "--forceSize") == 0)
        {
            if (i + 1 < argc)
            {
                i++;
                force_size = atoi(argv[i]);
                if (force_size < 1 || force_size > 6)
                {
                    cout << "Force Size must be between 1 and 6" << endl;
                    exit(1);
                }
            }
            else
            {
                print_usage();
                exit(1);
            }
        }
        else if (strcmp(argv[i], "--numForces") == 0)
        {
            if (i + 1 < argc)
            {
                i++;
                num_model_forces = atoi(argv[i]);
                if (num_model_forces < 1 || num_model_forces > 5)
                {
                    cout << "Num Forces must be between 1 and 5" << endl;
                    exit(1);
                }
            }
            else
            {
                print_usage();
                exit(1);
            }
        }
        else if (strcmp(argv[i], "--underPercentage") == 0)
        {
            if (i + 1 < argc)
            {
                i++;
                under_percentage = atof(argv[i]);
                if (under_percentage < 0.0f || under_percentage > 1.0f)
                {
                    cout << "Under percentage must be between 0.0 and 1.0" << endl;
                    exit(1);
                }
            }
            else
            {
                print_usage();
                exit(1);
            }
        }
        else if (strcmp(argv[i], "--modelSet") == 0)
        {
            if (i + 1 < argc)
            {
                i++;
                if (strcmp(argv[i], "OLD") == 0)
                {
                    models = &old_models;
                }
                else if (strcmp(argv[i], "IS") == 0)
                {
                    models = &is_models;
                }
                else
                {
                    cout << "Model set must be one of OLD, IS" << endl;
                    exit(1);
                }
            }
            else
            {
                print_usage();
                exit(1);
            }
        }
        else
        {
            cout << "Unrecognized option: " << argv[i] << endl << endl;
            print_usage();
            exit(1);
        }
    }

    if (max_bv < 0)
    {
        max_bv = 5000;
    }
    
    if (force_size < 0)
    {
        force_size = 4;
    }
    
    if (under_percentage < 0.0)
    {
        under_percentage = 0.95f;
    }
    
    if (num_model_forces < 0)
    {
        num_model_forces = 3;
    }

    if (models == nullptr)
    {
        models = &is_models;
    }
    
    int max_attempts = 5000;
    set<Force> force_set;
    list<Model> models_with_duplicates;
    set<ModelForce> model_force_set;

    // Create the duplicates that we use to force create all the possible basic forces based on models
    for (auto& model : *models)
    {
        for (int i = 0; i < model.count; i++)
        {
            models_with_duplicates.push_back(model);
        }
    }

    // Randomly make N lances, making sure they COULD fit within our requirements
    int attempts = 0;
    int generated_forces = 0;
    default_random_engine generator;
    generator.seed(chrono::system_clock::now().time_since_epoch().count());
    while (generated_forces < num_model_forces)
    {
        attempts++;
        list<Model> working_list = models_with_duplicates;
        ModelForce model_force;
        for (int i = 0; i < force_size; i++) 
        {
            uniform_int_distribution<int> distribution(0, working_list.size());
            int pos = distribution(generator);
            auto iter = working_list.begin();
            for (int j = 1; j < pos; j++)
            {
                iter++;
            }

            model_force.models.push_back(*iter);
            working_list.erase(iter);
        }

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
            auto inserted = model_force_set.insert(model_force);
            if (inserted.second)
            {
                generated_forces++;
            }
            else if (attempts > max_attempts)
            {
                cout << "Was only able to construct " << generated_forces << " model forces" << endl;
                break;
            }
        }
    }

    // Go through each model force and explode it
    for (auto& model_force : model_force_set)
    {
        auto first_model = model_force.models.begin();
        auto vhead1 = first_model->variants.begin();
        auto vend1 = first_model->variants.end();
        for (auto viter1 = vhead1; viter1 != vend1; viter1++)
        {
            if (force_size == 1)
            {
                Force force = build_force({ *viter1 });
                force_set.insert(force);
                continue;
            }

            auto second_model = first_model;
            second_model++;
            auto vhead2 = second_model->variants.begin();
            auto vend2 = second_model->variants.end();
            for (auto viter2 = vhead2; viter2 != vend2; viter2++)
            {
                if (force_size == 2)
                {
                    Force force = build_force({ *viter1, *viter2 });
                    force_set.insert(force);
                    continue;
                }

                auto third_model = second_model;
                third_model++;
                auto vhead3 = third_model->variants.begin();
                auto vend3 = third_model->variants.end();
                for (auto viter3 = vhead3; viter3 != vend3; viter3++)
                {
                    if (force_size == 3)
                    {
                        Force force = build_force({ *viter1, *viter2, *viter3 });
                        force_set.insert(force);
                        continue;
                    }

                    auto fourth_model = third_model;
                    fourth_model++;
                    auto vhead4 = fourth_model->variants.begin();
                    auto vend4 = fourth_model->variants.end();
                    for (auto viter4 = vhead4; viter4 != vend4; viter4++)
                    {
                        if (force_size == 4)
                        {
                            Force force = build_force({ *viter1, *viter2, *viter3, *viter4 });
                            force_set.insert(force);
                            continue;
                        }

                        auto fifth_model = fourth_model;
                        fifth_model++;
                        auto vhead5 = fifth_model->variants.begin();
                        auto vend5 = fifth_model->variants.end();
                        for (auto viter5 = vhead5; viter5 != vend5; viter5++)
                        {
                            if (force_size == 5)
                            {
                                Force force = build_force({ *viter1, *viter2, *viter3, *viter4, *viter5 });
                                force_set.insert(force);
                                continue;
                            }

                            auto sixth_model = fifth_model;
                            sixth_model++;
                            auto vhead6 = sixth_model->variants.begin();
                            auto vend6 = sixth_model->variants.end();
                            for (auto viter6 = vhead6; viter6 != vend6; viter6++)
                            {
                                if (force_size == 6)
                                {
                                    Force force = build_force({ *viter1, *viter2, *viter3, *viter4, *viter5, *viter6 });
                                    force_set.insert(force);
                                }
                                else
                                {
                                    cout << "Force sizes above 6 are not supported" << endl;
                                    exit(1);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // cut down by BV (so can't use foreach)
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
    file << "Model forces selected:" << endl;
    for (auto& model_force : model_force_set)
    {
        file << model_force.model_string << endl;
    }

    for (auto& force : force_set)
    {
        for (auto& mech : force.mechs)
        {
            file << mech.model_name << "-" << mech.variant_name << ",";
        }

        file << force.bv << endl;
    }

    file.close();

    system("pause");

    return 0;
}

