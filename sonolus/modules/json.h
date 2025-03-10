#ifndef JSON_H
#define JSON_H

#include<jsoncpp/json/json.h>

using namespace std;

string json_encode(Json::Value val) {
    Json::FastWriter writer;
    return writer.write(val);
}

string json_pretty_encode(Json::Value val) {
    Json::StyledWriter writer;
    return writer.write(val);
}

Json::Value json_decode(string json) {
    Json::Reader reader;
    Json::Value res;
    reader.parse(json, res);
    return res;
}

bool json_decode(string json, Json::Value& res) {
    Json::Reader reader;
    return reader.parse(json, res);
}

#endif
