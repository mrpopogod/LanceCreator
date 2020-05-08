#pragma once

#include <string>

using namespace std;

class Mech
{
public:
    Mech(string model_name, string variant_name, int bv)
        : model_name(model_name),
        variant_name(variant_name),
        bv(bv)
    {}

    ~Mech() {}

    string get_name();
    int get_bv();

    static bool compare_mech(Mech& lhs, Mech& rhs);

private:
    string model_name;
    string variant_name;
    int bv;
};

