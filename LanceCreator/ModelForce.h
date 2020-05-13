#pragma once

#include <string>
#include <list>
#include "Model.h"

using namespace std;

class ModelForce
{
public:
    ModelForce(list<Model> models)
        : models(models),
        min_bv_val(INT_MAX),
        max_bv_val(INT_MIN),
        model_string_val("UNSET")
    {
        init();
    }

    ~ModelForce() {}
    
    string model_string() const;
    bool can_fit_in_bv(int low_bv, int high_bv);
    list<Model>::const_iterator first_model() const;
    bool operator<(const ModelForce& rhs) const;

private:
    list<Model> models;
    int min_bv_val;
    int max_bv_val;
    string model_string_val;

    void init();
};

