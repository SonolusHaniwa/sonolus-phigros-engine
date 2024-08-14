#include<bits/stdc++.h>
#include"sonolus/sonolus.h"
using namespace std;

const string dist = "./dist";
#include"engine/engine.cpp"

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
        } else if (string(argv[1]) == "pec2data") {
            if (argc != 4) helpText(argc, argv);
            string chart = readFile(argv[2]);
            string LevelData = fromPGS(fromPEC(chart));
            cout << "Compressing..." << endl;
            buffer data = compress_gzip(LevelData, 9);
            cout << "Compress Finished." << endl;
            ofstream fout(argv[3]);
            for (int i = 0; i < data.size(); i++) fout << data.v[i];
            fout.close();
            return 0;
        } else if (string(argv[1]) == "rpe2json") {
            if (argc != 4) helpText(argc, argv);
            string chart = readFile(argv[2]);
            string LevelData = fromRPE(chart);
            ofstream fout(argv[3]);
            fout.write(LevelData.c_str(), LevelData.size());
            fout.close();
            return 0;
        } else if (string(argv[1]) == "rpe2data") {
            if (argc != 4) helpText(argc, argv);
            string chart = readFile(argv[2]);
            string LevelData = fromPGS(fromRPE(chart));
            cout << "Compressing..." << endl;
            buffer data = compress_gzip(LevelData, 9);
            cout << "Compress Finished." << endl;
            ofstream fout(argv[3]);
            for (int i = 0; i < data.size(); i++) fout << data.v[i];
            fout.close();
            return 0;
        }
    }
    
    engineConfiguration.ui = configurationUI;
#ifdef play
    build(
        dist + "/EngineConfiguration",
        dist + "/EngineData",
        Initialization,
        StageController,
        InputManager,
        FlickInputManager,
        Judgeline,
        SpeedEvent,
        MoveXEvent,
        MoveYEvent,
        RotateEvent,
        DisappearEvent,
        NormalNote,
        DragNote,
        HoldNote,
        FlickNote,
        FakeNormalNote,
        FakeDragNote,
        FakeHoldNote,
        FakeFlickNote
    );
#elif tutorial
    build(
        dist + "/EngineConfiguration",
        dist + "/EngineTutorialData"
    );
#elif preview
    build(
        dist + "/EngineConfiguration",
        dist + "/EnginePreviewData",
        Initialization
    );
#elif watch
	build(
        dist + "/EngineConfiguration",
        dist + "/EngineWatchData",
        Initialization,
        StageController,
        Judgeline,
        SpeedEvent,
        MoveXEvent,
        MoveYEvent,
        RotateEvent,
        DisappearEvent,
        NormalNote,
        DragNote,
        HoldNote,
        FlickNote,
        FakeNormalNote,
        FakeDragNote,
        FakeHoldNote,
        FakeFlickNote,
        UpdateJudgment
    );
#endif

	// 生成 Skin
	packSkin(dist + "/SkinTexture", dist + "/SkinData");
    // 生成 Effect
    packEffect(dist + "/EffectAudio", dist + "/EffectData");
}
