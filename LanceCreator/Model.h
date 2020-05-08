#pragma once

#include <string>
#include <list>
#include "Mech.h"

using namespace std;

class Model
{
public:
    Model(string name, list<Mech> variants)
        : name_val(name),
        variants(variants),
        min_bv_val(INT_MAX),
        max_bv_val(INT_MIN)
    {}

    ~Model() {}

    string name();
    int min_bv();
    int max_bv();
    static bool compare_model(Model& lhs, Model& rhs);

private:
    string name_val;
    list<Mech> variants;
    int min_bv_val;
    int max_bv_val;
};

