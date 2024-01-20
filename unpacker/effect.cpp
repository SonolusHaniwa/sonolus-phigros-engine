#include<bits/stdc++.h>
using namespace std;
#include"../modules/buffer.h"
#include"../modules/gzip.h"
#include"../modules/json.h"
#include"../modules/zip.h"

int main(int argc, char** argv) {
	if (argc < 3) {
		cout << "Usage: " << argv[0] << " [EffectAudio] [EffectData]" << endl;
		return 1;
	}

    cout << "Reading Effect Data..." << endl;
	ifstream fin(argv[2]);
	fin.seekg(0, ios::end); int len = fin.tellg();
	char* ch = new char[len]; fin.seekg(0, ios::beg);
	fin.read(ch, len); string data = "";
	for (int i = 0; i < len; i++) data.push_back(ch[i]);
	string json = decompress_gzip(data).asString();
	Json::Value obj; json_decode(json, obj);

    cout << "Unpacking Effect Audios..." << endl;
    for (int i = 0; i < obj["clips"].size(); i++) {
        string name = obj["clips"][i]["name"].asString();
        string filename = obj["clips"][i]["filename"].asString();
        string content = getFileFromZip(argv[1], filename);
        ofstream fout("../effect/" + name + ".mp3");
        char* ch = new char[content.size()];
        for (int i = 0; i < content.size(); i++) ch[i] = content[i];
        fout.write(ch, content.size()); fout.close(); delete[] ch;
    }
}