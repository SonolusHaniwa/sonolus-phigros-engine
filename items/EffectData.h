#pragma once

class EffectData {
	public:
    vector<pair<string, string> > clips;

	Json::Value toJsonObject() {
		Json::Value res;
		res["clips"].resize(0);
		for (int i = 0; i < clips.size(); i++) {
            Json::Value single;
            single["name"] = clips[i].first;
            single["filename"] = clips[i].second;
            res["clips"].append(single);
        } return res;
	}
};

void packEffect(string audioDir, string dataDir) {
	cout << "Packing Effect Audios..." << endl;
    EffectData data;
    for (int i = 0; i < engineData.effect_clips.size(); i++) {
        string name = engineData.effect_clips[i].first;
        string filename = to_string(i);
        data.clips.push_back(make_pair(name, filename));
        ifstream fin("./effect/" + name + ".mp3");
        fin.seekg(0, ios::end); int len = fin.tellg();
        char* ch = new char[len]; fin.seekg(0, ios::beg);
        fin.read(ch, len); fin.close(); string res = string(ch, len); delete[] ch;
        writeFileIntoZip(audioDir, filename, res);
    }

	cout << "Writing Effect Data..." << endl;
	string json = json_encode(data.toJsonObject());
	buffer dataBuffer = compress_gzip(json);
	ofstream fout(dataDir);
	char* ch = new char[dataBuffer.v.size()];
	for (int i = 0; i < dataBuffer.v.size(); i++) ch[i] = dataBuffer.v[i];
	fout.write(ch, dataBuffer.v.size()); delete[] ch;
	fout.close();
}
