#include "Force.h"
#include <sstream>

void Force::init()
{
    mechs.sort(Mech::compare_mech);
    stringstream ss;
    for (auto& mech : mechs)
    {
        bv_val += mech.bv();
        ss << mech.name() << ",";
    }

    mech_string_val = ss.str();
    mech_string_val.pop_back();
}

int Force::bv() const
{
    return bv_val;
}

string Force::mech_string() const
{
    return mech_string_val;
}

bool Force::operator<(const Force& rhs) const
{
    return mech_string_val < rhs.mech_string_val;
}

Force Force::build_force(initializer_list<Mech> mechs)
{
    list<Mech> mech_list;
    for (auto& mech : mechs)
    {
        mech_list.push_back(mech);
    }

    Force force(mech_list);

    return force;
}