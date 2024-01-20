#ifndef ENGINEPREVIEWDATA_H
#define ENGINEPREVIEWDATA_H

using namespace std;

class EnginePreviewDataArchetypeCallback {
    public:

    double index = 0;
    double order = 0;

    EnginePreviewDataArchetypeCallback(){}
    EnginePreviewDataArchetypeCallback(double index, double order): index(index), order(order){};
    EnginePreviewDataArchetypeCallback(Json::Value arr){
        if (arr.isNull() == true) return;
        index = arr["index"].asInt(), order = arr["order"].asInt();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["index"] = index;
        res["order"] = order;
        return res;
    }
};

class EnginePreviewDataArchetype {
    public:

    string name;
    EnginePreviewDataArchetypeCallback preprocess;
    EnginePreviewDataArchetypeCallback render;
    vector<pair<string, int> > data;

    EnginePreviewDataArchetype(){}
    EnginePreviewDataArchetype(string name, vector<pair<string, int> > data,
        EnginePreviewDataArchetypeCallback preprocess = EnginePreviewDataArchetypeCallback(),
        EnginePreviewDataArchetypeCallback render = EnginePreviewDataArchetypeCallback()): 
        name(name), data(data), preprocess(preprocess), render(render){};
    EnginePreviewDataArchetype(Json::Value arr) {
        name = arr["name"].asString();
        preprocess = EnginePreviewDataArchetypeCallback(arr["preprocess"]);
        render = EnginePreviewDataArchetypeCallback(arr["render"]);
        for (int i = 0; i < arr["data"].size(); i++) data.push_back(make_pair(arr["data"][i]["name"].asString(), arr["data"][i]["index"].asInt()));
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["name"] = name;
        res["preprocess"] = preprocess.toJsonObject();
        res["render"] = render.toJsonObject();
        res["data"].resize(0);
        for (int i = 0; i < data.size(); i++) {
            res["data"][i]["name"] = data[i].first;
            res["data"][i]["index"] = data[i].second;
        }
        return res;
    }
};

class EnginePreviewData {
    public:

    vector<pair<string, int> > skin_sprites = {};
    vector<EnginePreviewDataArchetype> archetypes = {};
    vector<EngineDataNode> nodes = {};

    Json::Value toJsonObject() {
        Json::Value res;
        res["skin"]["sprites"].resize(0);
        res["archetypes"].resize(0); res["nodes"].resize(0);
        for (int i = 0; i < skin_sprites.size(); i++) {
            res["skin"]["sprites"][i]["name"] = skin_sprites[i].first;
            res["skin"]["sprites"][i]["id"] = skin_sprites[i].second;
        }
        for (int i = 0; i < archetypes.size(); i++) res["archetypes"].append(archetypes[i].toJsonObject());
        for (int i = 0; i < nodes.size(); i++) res["nodes"].append(nodes[i].toJsonObject());
        return res;
    }
};

#endif