#ifndef ENGINETUTORIALDATA_H
#define ENGINETUTORIALDATA_H

using namespace std;

class EngineTutorialData {
    public:

    vector<pair<string, int> > skin_sprites;
    vector<pair<string, int> > effect_clips;
    vector<pair<string, int> > particle_effects;
    vector<pair<string, int> > instruction_texts;
    vector<pair<string, int> > instruction_icons;
    int preprocess;
    int navigate;
    int update;
    vector<EngineDataNode> nodes;

    Json::Value toJsonObject() {
        Json::Value res;
        res["skin"]["sprites"].resize(0); res["effect"]["clips"].resize(0); res["particle"]["effects"].resize(0);
        res["instruction"]["texts"].resize(0); res["instruction"]["icons"].resize(0); res["nodes"].resize(0);
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
        for (int i = 0; i < instruction_texts.size(); i++) {
            res["instruction"]["texts"][i]["name"] = instruction_texts[i].first;
            res["instruction"]["texts"][i]["id"] = instruction_texts[i].second;
        }
        for (int i = 0; i < instruction_icons.size(); i++) {
            res["instruction"]["icons"][i]["name"] = instruction_icons[i].first;
            res["instruction"]["icons"][i]["id"] = instruction_icons[i].second;
        }
        res["preprocess"] = preprocess;
        res["navigate"] = navigate;
        res["update"] = update;
        for (int i = 0; i < nodes.size(); i++) res["nodes"].append(nodes[i].toJsonObject());
        return res;
    }
};

#endif