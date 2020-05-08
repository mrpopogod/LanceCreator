#include "Model.h"
#include <algorithm>

int Model::min_bv()
{
    if (min_bv_val == INT_MAX)
    {
        for (auto& mech : variants)
        {
            min_bv_val = min(min_bv_val, mech.get_bv());
            max_bv_val = max(max_bv_val, mech.get_bv());
        }
    }

    return min_bv_val;
}

int Model::max_bv()
{
    if (max_bv_val == INT_MIN)
    {
        for (auto& mech : variants)
        {
            min_bv_val = min(min_bv_val, mech.get_bv());
            max_bv_val = max(max_bv_val, mech.get_bv());
        }
    }

    return max_bv_val;
}

bool Model::compare_model(Model & lhs, Model & rhs)
{
    return lhs.name < rhs.name;
}
