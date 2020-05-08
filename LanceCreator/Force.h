#pragma once

#include <string>
#include <list>
#include <initializer_list>
#include "Mech.h"

using namespace std;

class Force
{
public:
    Force(list<Mech> mechs)
        : mechs(mechs),
        bv(0),
        mech_string("UNSET")
    {}

    ~Force() {}

    void init();
    bool operator<(const Force& rhs) const;
    static Force build_force(initializer_list<Mech> mechs);

private:
    list<Mech> mechs;
    int bv;
    string mech_string;
};

