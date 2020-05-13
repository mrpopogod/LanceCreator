#include "Mech.h"

// name, then variant
bool Mech::compare_mech(Mech & lhs, Mech & rhs)
{
    if (lhs.model_name == rhs.model_name)
    {
        return lhs.variant_name < rhs.variant_name;
    }
    else
    {
        return lhs.model_name < rhs.model_name;
    }
}

string Mech::name() const
{
    return model_name + " " + variant_name;
}

int Mech::bv() const
{
    return bv_val;
}
