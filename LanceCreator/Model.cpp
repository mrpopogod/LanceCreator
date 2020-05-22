#include "Model.h"
#include <algorithm>

string Model::name() const
{
    return name_val;
}

int Model::count() const
{
    return count_val;
}

int Model::min_bv() const
{
    return min_bv_val;
}

int Model::max_bv() const
{
    return max_bv_val;
}

list<Mech>::const_iterator Model::first_variant() const
{
    return variants.begin();
}

list<Mech>::const_iterator Model::end_variant() const
{
    return variants.end();
}

bool Model::compare_model(Model & lhs, Model & rhs)
{
    return lhs.name_val < rhs.name_val;
}

void Model::init()
{
    for (auto& mech : variants)
    {
        min_bv_val = min(min_bv_val, mech.bv());
        max_bv_val = max(max_bv_val, mech.bv());
    }
}
