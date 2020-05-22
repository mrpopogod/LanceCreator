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

    model_string_val = ss.str();
    model_string_val.pop_back();

    min_bv_val = 0;
    max_bv_val = 0;
    for (auto& model : models)
    {
        min_bv_val += model.min_bv();
        max_bv_val += model.max_bv();
    }
}

string ModelForce::model_string() const
{
    return model_string_val;
}

bool ModelForce::can_fit_in_bv(int low_bv, int high_bv)
{
    return (max_bv_val >= low_bv) && (min_bv_val <= high_bv);
}

list<Model>::const_iterator ModelForce::first_model() const
{
    return models.begin();
}

bool ModelForce::operator<(const ModelForce & rhs) const
{
    return model_string_val < rhs.model_string_val;
}
