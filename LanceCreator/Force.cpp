#include "Force.h"
#include <sstream>

void Force::init()
{
    mechs.sort(Mech::compare_mech);
    stringstream ss;
    for (auto& mech : mechs)
    {
        bv += mech.get_bv();
        ss << mech.get_name();
    }

    mech_string = ss.str();
}

bool Force::operator<(const Force& rhs) const
{
    return mech_string < rhs.mech_string;
}

Force Force::build_force(initializer_list<Mech> mechs)
{
    list<Mech> mech_list;
    for (auto& mech : mechs)
    {
        mech_list.push_back(mech);
    }

    Force force(mech_list);
    force.init();

    return force;
}