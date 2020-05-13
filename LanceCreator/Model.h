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
    {
        init();
    }

    ~Model() {}

    string name() const;
    int count() const;
    int min_bv() const;
    int max_bv() const;
    list<Mech>::const_iterator first_variant() const;
    list<Mech>::const_iterator end_variant() const;
    static bool compare_model(Model& lhs, Model& rhs);

private:
    string name_val;
    list<Mech> variants;
    int min_bv_val;
    int max_bv_val;

    void init();
};

