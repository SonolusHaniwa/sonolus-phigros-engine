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
    vector<pair<string, int> > imports;

    EnginePreviewDataArchetype(){}
    EnginePreviewDataArchetype(string name, vector<pair<string, int> > imports,
        EnginePreviewDataArchetypeCallback preprocess = EnginePreviewDataArchetypeCallback(),
        EnginePreviewDataArchetypeCallback render = EnginePreviewDataArchetypeCallback()): 
        name(name), imports(imports), preprocess(preprocess), render(render){};
    EnginePreviewDataArchetype(Json::Value arr) {
        name = arr["name"].asString();
        preprocess = EnginePreviewDataArchetypeCallback(arr["preprocess"]);
        render = EnginePreviewDataArchetypeCallback(arr["render"]);
        for (int i = 0; i < arr["imports"].size(); i++) imports.push_back(make_pair(arr["imports"][i]["name"].asString(), arr["imports"][i]["index"].asInt()));
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["name"] = name;
        res["preprocess"] = preprocess.toJsonObject();
        res["render"] = render.toJsonObject();
        res["imports"].resize(0);
        for (int i = 0; i < imports.size(); i++) {
            res["imports"][i]["name"] = imports[i].first;
            res["imports"][i]["index"] = imports[i].second;
        }
        res["data"] = res["imports"];
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