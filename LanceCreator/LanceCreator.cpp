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
    cxxopts::Options options("Lace Creator", "Utility to randomly roll Battletech forces that fit within a BV");
    options.add_options()
        ("f,fileNames", "JSON Files containing mech descriptions", cxxopts::value<vector<string>>())
        ("b,maxBV", "Max BV for generated forces", cxxopts::value<int>()->default_value("5000"))
        ("s,forceSize", "Number of units in a force, no larger than 6", cxxopts::value<int>()->default_value("4"))
        ("n,numForces", "Number of unique collections of models for building forces", cxxopts::value<int>()->default_value("3"))
        ("u,underPercentage", "What percentage under Max BV is acceptable", cxxopts::value<double>()->default_value("0.95"))
        ("h,help", "Print usage")
        ;

    auto result = options.parse(argc, argv);

    if (result.count("help"))
    {
        print_usage(options);
        exit(0);
    }

    int max_bv = -1;
    int force_size = -1;
    int num_model_forces = -1;
    double under_percentage = -1.0;
    list<Model> models;
    if (!validate_args(result, max_bv, force_size, num_model_forces, under_percentage, models))
    {
        print_usage(options);
        exit(1);
    }
    
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

void generate_variant_combinations(set<ModelForce>& model_force_set, int force_size, set<Force>& force_set)
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

void create_model_forces(list<Model>& models_with_duplicates, int num_model_forces, int force_size, int max_bv, float under_percentage, int max_attempts, set<ModelForce>& model_force_set)
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

void add_duplicates(list<Model>& models, list<Model>& models_with_duplicates)
{
    for (auto& model : models)
    {
        for (int i = 0; i < model.count(); i++)
        {
            models_with_duplicates.push_back(model);
        }
    }
}

void print_usage(cxxopts::Options options)
{
    cout << options.help() << endl;
}

bool validate_args(cxxopts::ParseResult result, int& max_bv, int& force_size, int& num_model_forces, double& under_percentage, list<Model>& models)
{
    max_bv = result["maxBV"].as<int>();
    if (max_bv < 1)
    {
        cout << "Max BV must be a positive number" << endl;
        return false;
    }

    force_size = result["forceSize"].as<int>();
    if (force_size < 1 || force_size > 6)
    {
        cout << "Force Size must be between 1 and 6" << endl;
        return false;
    }

    num_model_forces = result["numForces"].as<int>();
    if (num_model_forces < 1 || num_model_forces > 5)
    {
        cout << "Num Forces must be between 1 and 5" << endl;
        return false;
    }

    under_percentage = result["underPercentage"].as<double>();
    if (under_percentage < 0.0f || under_percentage > 1.0f)
    {
        cout << "Under percentage must be between 0.0 and 1.0" << endl;
        return false;
    }

    if (!result.count("fileNames"))
    {
        cout << "At least one file must be specified" << endl;
        return false;    
    }

    vector<string> file_names_vector = result["fileNames"].as<vector<string>>();
    for (auto& file_name : file_names_vector)
    {
        FileParser p(file_name);
        p.parse_model_list(models);
    }

    return true;
}

void print_forces(set<ModelForce>& model_force_set, set<Force>& force_set)
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

void trim_to_bv(set<Force>& force_set, int max_bv, float under_percentage)
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