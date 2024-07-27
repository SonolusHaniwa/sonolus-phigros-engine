const function<FuncNode()> Preprocesses[] = {
    Initialization
};
const vector<pair<function<FuncNode(let, let)>, double> > Segments[] = {
    {
        { NormalNoteFall, 4.0 },
        { NormalNoteFrozen, 4.0 },
        { NormalNoteHit, 1.0 }
    }, {
        { DragNoteFall, 4.0 },
        { DragNoteFrozen, 4.0 },
        { DragNoteHit, 1.0 }
    }, {
        { HoldNoteFall, 4.0 },
        { HoldNoteFrozen, 4.0 },
        { HoldNoteFall2, 4.0 }
    }, {
        { FlickNoteFall, 4.0 },
        { FlickNoteFrozen, 4.0 },
        { FlickNoteHit, 1.0 }
    }
};
const function<FuncNode(let, let)> Statics[] = {
    drawPause,
    drawBorder,
    drawJudgeline,
    drawTimeline,
    drawScore
};

vector<pair<FuncNode, FuncNode> > duration, baseTime, nextSegment;
int segmentNumber = 0, groupNumber = 0; double sumTime = 0; 
auto unused_preprocess_unused = [](){
    int id = 0;
    for (auto segmentGroup: Segments) {
        int tmpid = id; groupNumber++;
        for (auto segment: segmentGroup) {
            segmentNumber++;
            duration.push_back({id, segment.second}), 
            baseTime.push_back({id, sumTime}), 
            sumTime += segment.second; id++;
        }
        for (int i = tmpid; i < id; i++) nextSegment.push_back({i, id});
    }
    return true;
}();

SonolusApi TutorialPreprocess() {
    FUNCBEGIN
    for (auto preprocess : Preprocesses) preprocess();
    return VOID;
}

SonolusApi TutorialNavigate() {
    FUNCBEGIN
    FOR (i, 0, If(navigation.direction > 0, 1, groupNumber - 1), 1) {
        tutorialNavigation = SwitchWithDefault(tutorialNavigation, nextSegment, 0);
        tutorialNavigation = tutorialNavigation % segmentNumber;
    } DONE
    tutorialStartTime = times.now;
    instruction = -1;
    return VOID;
}

SonolusApi TutorialUpdate() {
    FUNCBEGIN
    IF (times.now - tutorialStartTime > SwitchWithDefault(tutorialNavigation, duration, 1)) {
        tutorialNavigation = tutorialNavigation + 1;
        tutorialNavigation = tutorialNavigation % segmentNumber;
        tutorialStartTime = times.now;
    } FI

    for (auto Static: Statics) 
        Static(SwitchWithDefault(tutorialNavigation, baseTime, 1) + times.now - tutorialStartTime, sumTime);

    vector<pair<FuncNode, FuncNode> > funcs; int id = 0;
    for (auto group: Segments) for (auto segment: group)
        funcs.push_back({id++, 
            [&](){
                FUNCBEGIN
                segment.first(times.now - tutorialStartTime, segment.second);
                return VAR;
            }()
        });
    Execute0({ SwitchWithDefault(tutorialNavigation, funcs, 0) });
    return VOID;
}

bool tutorialDefine = [](){
    definePreprocess(TutorialPreprocess);
    defineNavigate(TutorialNavigate);
    defineUpdate(TutorialUpdate);
    return true;
}();