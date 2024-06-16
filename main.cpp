#include<bits/stdc++.h>
#include"sonolus.h"
using namespace std;

const string dist = "./dist";
#include"engine/engine.cpp"

vector<string> explode(const char* seperator, const char* source) {
	string src = source; vector<string> res;
	while (src.find(seperator) != string::npos) {
		int wh = src.find(seperator);
		res.push_back(src.substr(0, src.find(seperator)));
		src = src.substr(wh + string(seperator).size());
	} res.push_back(src);
	return res;
}

string readFile(string path) {
    ifstream fin(path);
    fin.seekg(0, ios::end);
    int len = fin.tellg();
    if (len == -1) {
        cout << "No such file or directory: \"" << path << "\"" << endl;
        exit(1);
    } fin.seekg(0, ios::beg);
    char* buffer = new char[len];
    fin.read(buffer, len);
    fin.close(); return string(buffer, len);
}

#include"convert.h"

void helpText(int argc, char** argv) {
    cout << "Usage: " << endl;
    cout << endl;
    cout << "Create Engine Scripts: " << endl;
    #ifdef play
    cout << "    EngineConfig & EnginePlayData: " << argv[0] << endl;
    #elif tutorial
    cout << "    EngineConfig & EngineTutorialData: " << argv[0] << endl;
    #elif preview
    cout << "    EngineConfig & EnginePreviewData: " << argv[0] << endl;
    #elif watch
    cout << "    EngineConfig & EngineWatchData: " << argv[0] << endl;
    #endif
    cout << "Chart Convertor: " << endl;
    cout << "    Official -> LevelData:  " << argv[0] << " json2data <chart.json> <LevelData>" << endl;
    exit(1);
}
    
int main(int argc, char** argv) {
    // 谱面转换测试
    if (argc >= 2) {
        if (string(argv[1]) == "json2data") {
            if (argc != 4) helpText(argc, argv);
            string chart = readFile(argv[2]);
            string LevelData = fromPGS(chart);
            cout << "Compressing..." << endl;
            buffer data = compress_gzip(LevelData, 9);
            cout << "Compress Finished." << endl;
            ofstream fout(argv[3]);
            for (int i = 0; i < data.size(); i++) fout << data.v[i];
            fout.close();
            return 0;
        } else if (string(argv[1]) == "pec2json") {
            if (argc != 4) helpText(argc, argv);
            string chart = readFile(argv[2]);
            string LevelData = fromPEC(chart);
            ofstream fout(argv[3]);
            fout.write(LevelData.c_str(), LevelData.size());
            fout.close();
            return 0;
        }
    }
    
    engineConfiguration.ui = configurationUI;
#ifdef play
    buffer data, configuration;
    build<
        // Replace with your archetypes here
        Initialization,
        Judgeline,
        SpeedEvent,
        MoveXEvent,
        MoveYEvent,
        RotateEvent,
        DisappearEvent,
        NormalNote,
        DragNote,
        HoldNote,
        FlickNote
    >(configuration, data);
    ofstream fout((dist + "/EngineConfiguration"));
    for (int i = 0; i < configuration.size(); i++) fout << configuration.v[i];
    fout.close(); fout.open((dist + "/EngineData"));
    // ofstream fout((dist + "/EngineData"));
    for (int i = 0; i < data.size(); i++) fout << data.v[i];
// #elif tutorial
//     buffer data, configuration;
//     build(configuration, data);
//     ofstream fout((dist + "/EngineConfiguration"));
//     for (int i = 0; i < configuration.size(); i++) fout << configuration.v[i];
//     fout.close(); fout.open((dist + "/EngineTutorialData"));
//     for (int i = 0; i < data.size(); i++) fout << data.v[i];
#elif preview
    buffer data, configuration;
    build<
        // Replace with your archetypes here
        Judgeline
    >(configuration, data);
    ofstream fout((dist + "/EngineConfiguration"));
    for (int i = 0; i < configuration.size(); i++) fout << configuration.v[i];
    fout.close(); fout.open((dist + "/EnginePreviewData"));
    for (int i = 0; i < data.size(); i++) fout << data.v[i];
#elif watch
	buffer data, configuration;
	build<
		// Replace with your archetypes here
		Archetype
	>(configuration, data);
    ofstream fout((dist + "/EngineConfiguration"));
    for (int i = 0; i < configuration.size(); i++) fout << configuration.v[i];
    fout.close(); fout.open((dist + "/EngineWatchData"));
    for (int i = 0; i < data.size(); i++) fout << data.v[i];
#endif

	// 生成 Skin
	packSkin(dist + "/SkinTexture", dist + "/SkinData");
    // 生成 Effect
    packEffect(dist + "/EffectAudio", dist + "/EffectData");
}
