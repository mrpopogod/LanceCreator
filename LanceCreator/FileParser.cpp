
#include "FileParser.h"

FileParser::FileParser(const string& file)
{
    FILE* fp = fopen(file.c_str(), "rb");
    char readBuffer[65536];
    FileReadStream is(fp, readBuffer, sizeof(readBuffer));

    d.ParseStream(is);
    fclose(fp);
}

void FileParser::parse_model_list(list<Model>& model_list)
{
    for (auto& v : d.GetArray())
    {
        model_list.push_back(parse_model(v));
    }
}

Model FileParser::parse_model(Value& value)
{
    string name = value["name"].GetString();
    int count = value["count"].GetInt();
    list<Mech> mechs;
    for (auto& v : value["variants"].GetArray())
    {
        string model_name = value["name"].GetString();
        string variant_name = v["variant"].GetString();
        int bv = v["bv"].GetInt();
        Mech mech(model_name, variant_name, bv);
        mechs.push_back(mech);
    }

    Model model(name, count, mechs);

    return model;
}