#pragma once

#include <string>
#include <list>
#include <set>
#include "Model.h"
#include "ModelForce.h"
#include "Force.h"

void print_usage();

void create_model_forces(std::list<Model>& models_with_duplicates, int num_model_forces, int force_size, int max_bv, float under_percentage, int max_attempts, std::set<ModelForce>& model_force_set);

void trim_to_bv(std::set<Force>& force_set, int max_bv, float under_percentage);

void print_forces(std::set<ModelForce>& model_force_set, std::set<Force>& force_set);

void parse_args(int argc, char** argv, int& max_bv, int& force_size, int& num_model_forces, double& under_percentage, std::string& filenames, std::list<Model>& models);

void add_duplicates(std::list<Model>& models, std::list<Model>& models_with_duplicates);

void generate_variant_combinations(std::set<ModelForce>& model_force_set, int force_size, std::set<Force>& force_set);