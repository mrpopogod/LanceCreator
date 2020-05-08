#include "ModelForce.h"
#include <sstream>

void ModelForce::init()
{
    models.sort(Model::compare_model);
    stringstream ss;
    for (auto& mech : models)
    {
        ss << mech.name() << ",";
    }

    model_string = ss.str();
    model_string.pop_back();
}

bool ModelForce::can_fit_in_bv(int low_bv, int high_bv)
{
    if (min_bv_val == INT_MAX || max_bv_val == INT_MIN)
    {
        min_bv_val = 0;
        max_bv_val = 0;
        for (auto& model : models)
        {
            min_bv_val += model.min_bv();
            max_bv_val += model.max_bv();
        }
    }

    return (max_bv_val >= low_bv) && (min_bv_val <= high_bv);
}

bool ModelForce::operator<(const ModelForce & rhs) const
{
    return model_string < rhs.model_string;
}
