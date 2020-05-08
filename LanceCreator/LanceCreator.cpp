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
#include "ModelForce.h"
#include "Force.h"
#include "Model.h"
#include "Mech.h"

using namespace std;
using namespace rapidjson;

void print_usage()
{
    cout << "Usage: " << endl
        << "LanceCreator [--maxBV bv] [--forceSize size] [--numForces num] [--underPercentage percentage] [--modelSet set [--fileNames name1,name2]]" << endl << endl
        << "maxBV - The maximum BV a force can have" << endl
        << "forceSize - The numer of units in a force" << endl
        << "numForces - The number of compositions of units in a force to generate and run through permutations of variants on" << endl
        << "underPercentage - The lowest percentage of maxBV the force must be (e.g. 0.95 means a force will have 95% of maxBV or more" << endl
        << "modelSet - The model set to use, valid values are OLD, IS, FILE" << endl
        << "fileNames - A comma separated list of json files describing model sets.  Only if modelSet is FILE" << endl;
}

Document load_json_from_file(const string& filename) 
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
    string name = value["name"].GetString();
    list<Mech> mechs;
    for (auto& v : value["variants"].GetArray())
    {
        string model_name = value["name"].GetString();
        string variant_name = v["variant"].GetString();
        int bv = v["bv"].GetInt();
        Mech mech(model_name, variant_name, bv);
        mechs.push_back(mech);
    }

    Model model(name, mechs);

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
    list<Model> parsed_models;
    list<Model> *models = nullptr;
    bool load_from_file = false;
    string filenames = "";
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
                if (force_size < 1 || force_size > 8)
                {
                    cout << "Force Size must be between 1 and 8" << endl;
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
                else if (strcmp(argv[i], "FILE") == 0)
                {
                    load_from_file = true;
                }
                else
                {
                    cout << "Model set must be one of OLD, IS, FILE" << endl;
                    exit(1);
                }
            }
            else
            {
                print_usage();
                exit(1);
            }
        }
        else if (strcmp(argv[i], "--fileNames") == 0) 
        {
            if (!load_from_file)
            {
                cout << "Cannot specify filenames if not loading from file" << endl << endl;
                print_usage();
                exit(1);
            }
            else if (i + 1 < argc)
            {
                i++;
                filenames = argv[i];
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

    if (strcmp(filenames.c_str(), "") == 0)
    {
        if (models == nullptr)
        {
            models = &is_models;
        }
    }
    else
    {
        stringstream ss(filenames);
        string s;
        while (getline(ss, s, ',')) 
        {
            Document d = load_json_from_file(s);
            list<Model> file_models = parse_models(d);
            parsed_models.splice(parsed_models.end(), file_models);
        }

        models = &parsed_models;
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
                                
                                auto seventh_model = sixth_model;
                                seventh_model++;
                                auto vhead7 = seventh_model->variants.begin();
                                auto vend7 = seventh_model->variants.end();
                                for (auto viter7 = vhead7; viter7 != vend7; viter7++)
                                {
                                    if (force_size == 7)
                                    {
                                        Force force = build_force({ *viter1, *viter2, *viter3, *viter4, *viter5, *viter6, *viter7 });
                                        force_set.insert(force);
                                    }

                                    auto eigth_model = seventh_model;
                                    eigth_model++;
                                    auto vhead8 = eigth_model->variants.begin();
                                    auto vend8 = eigth_model->variants.end();
                                    for (auto viter8 = vhead8; viter8 != vend8; viter8++)
                                    {
                                        if (force_size == 8)
                                        {
                                            Force force = build_force({ *viter1, *viter2, *viter3, *viter4, *viter5, *viter6, *viter7, *viter8 });
                                            force_set.insert(force);
                                        }
                                        else
                                        {
                                            cout << "Only supports forces up to size 8" << endl;
                                            exit(1);
                                        }
                                    }
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

