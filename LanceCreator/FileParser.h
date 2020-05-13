#pragma once

#include <string>
#include <list>
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include "Model.h"

using namespace std;
using namespace rapidjson;

class FileParser
{
public:
    FileParser(const string& file);
    ~FileParser() {}

    void parse_model_list(list<Model>& model_list);

private:
    Document d;

    Model parse_model(Value& value);
};
