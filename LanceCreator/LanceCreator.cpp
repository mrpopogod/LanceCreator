// LanceCreator.cpp : Defines the entry point for the console application.
//

#include "LanceCreator.h"
#include "stdafx.h"
#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <random>
#include <chrono>
#include "ModelForce.h"
#include "Force.h"
#include "Model.h"
#include "Mech.h"
#include "FileParser.h"

using namespace std;

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
    double under_percentage = -1.0;
    list<Model> models;
    string filenames = "";
    parse_args(argc, argv, max_bv, force_size, num_model_forces, under_percentage, filenames, models);
    
    int max_attempts = 5000;
    set<Force> force_set;
    list<Model> models_with_duplicates;
    set<ModelForce> model_force_set;

    // Create the duplicates that we use to force create all the possible basic forces based on models
    cout << "Setting up mech list" << endl;
    add_duplicates(models, models_with_duplicates);

    // Randomly make N lances, making sure they COULD fit within our requirements
    cout << "Creating lance candidates" << endl;
    create_model_forces(models_with_duplicates, num_model_forces, force_size, max_bv, under_percentage, max_attempts, model_force_set);

    // Go through each model force and explode it
    cout << "Creating all variants" << endl;
    generate_variant_combinations(model_force_set, force_size, force_set);

    // Cut anything that doesn't fit within our BV range
    cout << "Trimming to target BV" << endl;
    trim_to_bv(force_set, max_bv, under_percentage);

    cout << "Outputting forces" << endl;
    print_forces(model_force_set, force_set);

    system("pause");

    return 0;
}

void generate_variant_combinations(std::set<ModelForce>& model_force_set, int force_size, std::set<Force>& force_set)
{
    for (auto& model_force : model_force_set)
    {
        auto first_model = model_force.first_model();
        auto vhead1 = first_model->first_variant();
        auto vend1 = first_model->end_variant();
        for (auto viter1 = vhead1; viter1 != vend1; viter1++)
        {
            if (force_size == 1)
            {
                Force force = Force::build_force({ *viter1 });
                force_set.insert(force);
                continue;
            }

            auto second_model = first_model;
            second_model++;
            auto vhead2 = second_model->first_variant();
            auto vend2 = second_model->end_variant();
            for (auto viter2 = vhead2; viter2 != vend2; viter2++)
            {
                if (force_size == 2)
                {
                    Force force = Force::build_force({ *viter1, *viter2 });
                    force_set.insert(force);
                    continue;
                }

                auto third_model = second_model;
                third_model++;
                auto vhead3 = third_model->first_variant();
                auto vend3 = third_model->end_variant();
                for (auto viter3 = vhead3; viter3 != vend3; viter3++)
                {
                    if (force_size == 3)
                    {
                        Force force = Force::build_force({ *viter1, *viter2, *viter3 });
                        force_set.insert(force);
                        continue;
                    }

                    auto fourth_model = third_model;
                    fourth_model++;
                    auto vhead4 = fourth_model->first_variant();
                    auto vend4 = fourth_model->end_variant();
                    for (auto viter4 = vhead4; viter4 != vend4; viter4++)
                    {
                        if (force_size == 4)
                        {
                            Force force = Force::build_force({ *viter1, *viter2, *viter3, *viter4 });
                            force_set.insert(force);
                            continue;
                        }

                        auto fifth_model = fourth_model;
                        fifth_model++;
                        auto vhead5 = fifth_model->first_variant();
                        auto vend5 = fifth_model->end_variant();
                        for (auto viter5 = vhead5; viter5 != vend5; viter5++)
                        {
                            if (force_size == 5)
                            {
                                Force force = Force::build_force({ *viter1, *viter2, *viter3, *viter4, *viter5 });
                                force_set.insert(force);
                                continue;
                            }

                            auto sixth_model = fifth_model;
                            sixth_model++;
                            auto vhead6 = sixth_model->first_variant();
                            auto vend6 = sixth_model->end_variant();
                            for (auto viter6 = vhead6; viter6 != vend6; viter6++)
                            {
                                if (force_size == 6)
                                {
                                    Force force = Force::build_force({ *viter1, *viter2, *viter3, *viter4, *viter5, *viter6 });
                                    force_set.insert(force);
                                }
                                else 
                                {
                                    cout << "Only supports forces up to size 6" << endl;
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

void print_usage()
{
    cout << "Usage: " << endl
        << "LanceCreator --fileNames name1,name2 [--maxBV bv] [--forceSize size] [--numForces num] [--underPercentage percentage]" << endl << endl
        << "fileNames - A comma separated list of json files describing model sets." << endl
        << "maxBV - The maximum BV a force can have (default 5000)" << endl
        << "forceSize - The numer of units in a force (default 4)" << endl
        << "numForces - The number of compositions of units in a force to generate and run through permutations of variants on (default 3)" << endl
        << "underPercentage - The lowest percentage of maxBV the force must be (e.g. 0.95 means a force will have 95% of maxBV or more (default 0.95)" << endl;
}

void create_model_forces(std::list<Model>& models_with_duplicates, int num_model_forces, int force_size, int max_bv, float under_percentage, int max_attempts, std::set<ModelForce>& model_force_set)
{
    int attempts = 0;
    int generated_forces = 0;
    default_random_engine generator;
    generator.seed(chrono::system_clock::now().time_since_epoch().count());
    while (generated_forces < num_model_forces)
    {
        if (attempts > max_attempts)
        {
            cout << "Was only able to construct " << generated_forces << " model forces" << endl;
            break;
        }

        attempts++;
        list<Model> working_list = models_with_duplicates;
        while (working_list.size() > force_size)
        {
            uniform_int_distribution<int> distribution(0, working_list.size());
            int pos = distribution(generator);
            auto iter = working_list.begin();
            for (int j = 1; j < pos; j++)
            {
                iter++;
            }

            working_list.erase(iter);
        }

        ModelForce model_force(working_list);
        if (!model_force.can_fit_in_bv((int) (max_bv * under_percentage), max_bv))
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
        }
    }
}

void add_duplicates(std::list<Model>& models, std::list<Model>& models_with_duplicates)
{
    for (auto& model : models)
    {
        for (int i = 0; i < model.count(); i++)
        {
            models_with_duplicates.push_back(model);
        }
    }
}

void parse_args(int argc, char** argv, int& max_bv, int& force_size, int& num_model_forces, double& under_percentage, std::string& filenames, std::list<Model>& models)
{
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
        else if (strcmp(argv[i], "--fileNames") == 0)
        {
            if (i + 1 < argc)
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
        print_usage();
        exit(1);
    }
    else
    {
        stringstream ss(filenames);
        string s;
        while (getline(ss, s, ','))
        {
            FileParser p(s);
            p.parse_model_list(models);
        }
    }
}

void print_forces(std::set<ModelForce>& model_force_set, std::set<Force>& force_set)
{
    ofstream file;
    file.open("lances.csv");
    file << "Model forces selected:" << endl;
    for (auto& model_force : model_force_set)
    {
        file << model_force.model_string() << endl;
    }

    for (auto& force : force_set)
    {
        file << force.mech_string() << "," << force.bv() << endl;
    }

    file.close();
}

void trim_to_bv(std::set<Force>& force_set, int max_bv, float under_percentage)
{
    // can't use foreach because we're modifying the set
    for (auto iter = force_set.begin(); iter != force_set.end(); )
    {
        if (iter->bv() > max_bv)
        {
            iter = force_set.erase(iter);
        }
        else if (iter->bv() < max_bv * under_percentage)
        {
            iter = force_set.erase(iter);
        }
        else
        {
            iter++;
        }
    }
}