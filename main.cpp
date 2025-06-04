#include<bits/stdc++.h>
#include"sonolus/sonolus.h"
#include"engine/engine.cpp"
#include"levelData.cpp"

string readFile(string path) {
    ifstream fin; fin.open(path);
    fin.seekg(0, ios::end);
    int len = fin.tellg();
    if (len == -1) {
        cout << "No such file or directory: \"" << path << "\"" << endl;
        exit(1);
    } fin.seekg(0, ios::beg);
    char* buffer = new char[len];
    fin.read(buffer, len);
    fin.close(); 
    return string(buffer, len);
}

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
            string data = compress_gzip(LevelData, 9);
            cout << "Compress Finished." << endl;
            ofstream fout; fout.open(argv[3]);
            fout.write(data.c_str(), data.size());
            fout.close();
            return 0;
        } else if (string(argv[1]) == "pec2json") {
            if (argc != 4) helpText(argc, argv);
            string chart = readFile(argv[2]);
            string LevelData = fromPEC(chart);
            ofstream fout; fout.open(argv[3]);
            fout.write(LevelData.c_str(), LevelData.size());
            fout.close();
            return 0;
        } else if (string(argv[1]) == "pec2data") {
            if (argc != 4) helpText(argc, argv);
            string chart = readFile(argv[2]);
            string LevelData = fromPGS(fromPEC(chart));
            cout << "Compressing..." << endl;
            string data = compress_gzip(LevelData, 9);
            cout << "Compress Finished." << endl;
            ofstream fout; fout.open(argv[3]);
            fout.write(data.c_str(), data.size());
            fout.close();
            return 0;
        } else if (string(argv[1]) == "rpe2json") {
            if (argc != 4) helpText(argc, argv);
            string chart = readFile(argv[2]);
            string LevelData = fromRPE(chart);
            ofstream fout; fout.open(argv[3]);
            fout.write(LevelData.c_str(), LevelData.size());
            fout.close();
            return 0;
        } else if (string(argv[1]) == "rpe2data") {
            if (argc != 4) helpText(argc, argv);
            string chart = readFile(argv[2]);
            string LevelData = fromPGS(fromRPE(chart));
            cout << "Compressing..." << endl;
            string data = compress_gzip(LevelData, 9);
            cout << "Compress Finished." << endl;
            ofstream fout; fout.open(argv[3]);
            fout.write(data.c_str(), data.size());
            fout.close();
            return 0;
        }
    }

    UI.SetScope(scope);
    UI.SetMenu(Visibility(1.0, 1.0));
    UI.SetJudgment(
        Visibility(1.0, 1.0), 
        Animation(
            AnimationTween(0.0, 0.0, 0.2, EngineConfigurationAnimationTweenEase.InSine),
            AnimationTween(0.0, 0.0, 0.2, EngineConfigurationAnimationTweenEase.InSine)
        ),
        EngineConfigurationJudgmentErrorStyle.None,
        EngineConfigurationJudgmentErrorPlacement.LeftRight,
        20.0
    );
    UI.SetCombo(
        Visibility(1.0, 1.0),
        Animation(
            AnimationTween(1.0, 1.0, 0.2, EngineConfigurationAnimationTweenEase.InSine),
            AnimationTween(1.0, 1.0, 0.2, EngineConfigurationAnimationTweenEase.Linear)
        )
    );
    UI.SetMetric(
        EngineConfigurationMetric.Arcade,
        Visibility(1.0, 1.0),
        EngineConfigurationMetric.Life,
        Visibility(1.0, 1.0)
    );
    UI.SetProgress(Visibility(1.0, 1.0));
    UI.SetTutorial(
        Visibility(1.0, 1.0),
        Visibility(1.0, 1.0)
    );

    BuildConfiguration();
    #ifdef play
    BuildData(
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
    #endif
    #ifdef tutorial
    BuildData();
    #endif
    #ifdef preview
    BuildData(
        Initialization
    );
    #endif
    #ifdef watch
    BuildData(
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

    PackSkin();
    PackEffect();
}
