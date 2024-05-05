#ifndef ENGINEWATCHDATA_H
#define ENGINEWATCHDATA_H

using namespace std;

class EngineWatchDataArchetypeCallback {
    public:

    double index = 0;
    double order = 0;

    EngineWatchDataArchetypeCallback(){}
    EngineWatchDataArchetypeCallback(double index, double order): index(index), order(order){};
    EngineWatchDataArchetypeCallback(Json::Value arr){
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

class EngineWatchDataArchetype {
    public:

    string name;
    bool hasInput;
    EngineWatchDataArchetypeCallback preprocess;
    EngineWatchDataArchetypeCallback spawnTime;
    EngineWatchDataArchetypeCallback despawnTime;
    EngineWatchDataArchetypeCallback initialize;
    EngineWatchDataArchetypeCallback updateSequential;
    EngineWatchDataArchetypeCallback updateParallel;
    EngineWatchDataArchetypeCallback terminate;
    EngineWatchDataArchetypeCallback updateSpawn;    
    vector<pair<string, int> > imports;

    EngineWatchDataArchetype(){}
    EngineWatchDataArchetype(string name, vector<pair<string, int> > imports, bool hasInput,
        EngineWatchDataArchetypeCallback preprocess = EngineWatchDataArchetypeCallback(),
        EngineWatchDataArchetypeCallback spawnTime = EngineWatchDataArchetypeCallback(),
        EngineWatchDataArchetypeCallback despawnTime = EngineWatchDataArchetypeCallback(),
        EngineWatchDataArchetypeCallback initialize = EngineWatchDataArchetypeCallback(),
        EngineWatchDataArchetypeCallback updateSequential = EngineWatchDataArchetypeCallback(),
        EngineWatchDataArchetypeCallback updateParallel = EngineWatchDataArchetypeCallback(),
        EngineWatchDataArchetypeCallback terminate = EngineWatchDataArchetypeCallback(),
        EngineWatchDataArchetypeCallback updateSpawn = EngineWatchDataArchetypeCallback()):
        name(name), imports(imports), hasInput(hasInput), preprocess(preprocess), spawnTime(spawnTime), despawnTime(despawnTime),
        initialize(initialize), updateSequential(updateSequential), updateParallel(updateParallel),
        terminate(terminate), updateSpawn(updateSpawn){};
    EngineWatchDataArchetype(Json::Value arr) {
        name = arr["name"].asString();
        preprocess = EngineWatchDataArchetypeCallback(arr["preprocess"]);
        spawnTime = EngineWatchDataArchetypeCallback(arr["spawnTime"]);
        despawnTime = EngineWatchDataArchetypeCallback(arr["despawnTime"]);
        initialize = EngineWatchDataArchetypeCallback(arr["initialize"]);
        updateSequential = EngineWatchDataArchetypeCallback(arr["updateSequential"]);
        updateParallel = EngineWatchDataArchetypeCallback(arr["updateParallel"]);
        terminate = EngineWatchDataArchetypeCallback(arr["terminate"]);
        updateSpawn = EngineWatchDataArchetypeCallback(arr["updateSpawn"]);
        for (int i = 0; i < arr["imports"].size(); i++) imports.push_back(make_pair(arr["imports"][i]["name"].asString(), arr["imports"][i]["index"].asInt()));
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["name"] = name;
        res["hasInput"] = hasInput;
        res["preprocess"] = preprocess.toJsonObject();
        res["spawnTime"] = spawnTime.toJsonObject();
        res["despawnTime"] = despawnTime.toJsonObject();
        res["initialize"] = initialize.toJsonObject();
        res["updateSequential"] = updateSequential.toJsonObject();
        res["updateParallel"] = updateParallel.toJsonObject();
        res["terminate"] = terminate.toJsonObject();
        res["updateSpawn"] = updateSpawn.toJsonObject();
        res["imports"].resize(0);
        for (int i = 0; i < imports.size(); i++) {
            res["imports"][i]["name"] = imports[i].first;
            res["imports"][i]["index"] = imports[i].second;
        }
        res["data"] = res["imports"];
        return res;
    }
};

class EngineWatchData {
    public:

    vector<pair<string, int> > skin_sprites = {};
    vector<pair<string, int> > effect_clips = {};
    vector<pair<string, int> > particle_effects = {};
    vector<EngineDataBucket> buckets = {};
    vector<EngineWatchDataArchetype> archetypes = {};
    int updateSpawn = 0;
    vector<EngineDataNode> nodes = {};

    Json::Value toJsonObject() {
        Json::Value res;
        res["skin"]["sprites"].resize(0);
        res["effect"]["clips"].resize(0);
        res["particle"]["effects"].resize(0);
        res["archetypes"].resize(0); res["nodes"].resize(0); res["buckets"].resize(0);
        for (int i = 0; i < skin_sprites.size(); i++) {
            res["skin"]["sprites"][i]["name"] = skin_sprites[i].first;
            res["skin"]["sprites"][i]["id"] = skin_sprites[i].second;
        }
        for (int i = 0; i < effect_clips.size(); i++) {
            res["effect"]["clips"][i]["name"] = effect_clips[i].first;
            res["effect"]["clips"][i]["id"] = effect_clips[i].second;
        }
        for (int i = 0; i < particle_effects.size(); i++) {
            res["particle"]["effects"][i]["name"] = particle_effects[i].first;
            res["particle"]["effects"][i]["id"] = particle_effects[i].second;
        }
        for (int i = 0; i < buckets.size(); i++) res["buckets"].append(buckets[i].toJsonObject());
        for (int i = 0; i < archetypes.size(); i++) res["archetypes"].append(archetypes[i].toJsonObject());
        res["updateSpawn"] = updateSpawn;
        for (int i = 0; i < nodes.size(); i++) res["nodes"].append(nodes[i].toJsonObject());
        return res;
    }
};

#endif
