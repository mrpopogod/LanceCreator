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
        bv_val(0),
        mech_string_val("UNSET")
    {
        init();
    }

    ~Force() {}
    
    int bv() const;
    string mech_string() const;
    bool operator<(const Force& rhs) const;
    static Force build_force(initializer_list<Mech> mechs);

private:
    list<Mech> mechs;
    int bv_val;
    string mech_string_val;

    void init();
};

