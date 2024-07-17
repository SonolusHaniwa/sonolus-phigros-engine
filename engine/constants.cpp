double PI  = acos(-1);
double lockAspectRatio = 1;
double interfaceGap = 0.05;
double extraWidth = 1;
double targetAspectRatio = 1920.0 / 1080.0;
double judgelineHeight = 0.01;
double baseNoteWidth = 0.48;
double minSFXDistance = 0.02;
// double noteRatio = 989.0 / 100.0;
// double dragRatio = 989.0 / 60.0;
// double flickRatio = 989.0 / 200.0;
// double hlNoteRatio = 1089.0 / 200.0;
// double hlDragRatio = 1089.0 / 160.0;
// double hlFlickRatio = 1089.0 / 300.0;
double noteRatio = 1;
double dragRatio = 1;
double flickRatio = 1;
double hlNoteRatio = 1;
double hlDragRatio = 1;
double hlFlickRatio = 1;
double holdHeadRatio = 989.0 / 50.0;
double holdBodyRatio = 989.0 / 1950.0;
double hlHoldHeadRatio = 1062.0 / 97.0;
double hlHoldBodyRatio = 1062.0 / 1951.0;
double hlHoldOffset = 1062.0 / 49.0;
double judgeDistanceLimit = baseNoteWidth * 0.875;
double effectDurationTime = 0.4;
double effectWidth = baseNoteWidth * 0.75;
double holdEffectSpawnDistance = 0.08;
double holdTailTime = 0.16;
double comboTextRatio = 297.0 / 62.0;
double combo0Ratio = 46.0 / 62.0;
double combo1Ratio = 31.0 / 60.0;
double combo2Ratio = 41.0 / 61.0;
double combo3Ratio = 41.0 / 62.0;
double combo4Ratio = 49.0 / 60.0;
double combo5Ratio = 41.0 / 61.0;
double combo6Ratio = 46.0 / 62.0;
double combo7Ratio = 42.0 / 60.0;
double combo8Ratio = 47.0 / 62.0;
double combo9Ratio = 46.0 / 62.0;
double floorPositionLimit = -0.001;
double timeBarHeight = 0.01;
double timeMagic = 1.875;
double judgelineIdTextHeight = 0.05;

#if play || watch
let strictMode = LevelOption.get(Options.StrictMode);
let noteWidth = baseNoteWidth * LevelOption.get(Options.NoteSize);
let hasSFX = LevelOption.get(Options.HasSFX);
let hasSimul = LevelOption.get(Options.HasSimul);
let hasIndicator = LevelOption.get(Options.HasIndicator);
let background = LevelOption.get(Options.Background);
let sonolusPause = LevelOption.get(Options.SystemPause);
let sonolusCombo = LevelOption.get(Options.SystemCombo);
let sonolusPrimaryMetric = LevelOption.get(Options.SystemPrimaryMetric);
let hasJudgelineId = LevelOption.get(Options.JudgelineId);
let autoSFX = LevelOption.get(Options.AutoSFX);
#endif

#if play || watch
class stage {
	public:

	Variable<EntityMemoryId> w = If(lockAspectRatio == 0 || screen.aspectRatio < targetAspectRatio,
        screen.w * extraWidth,
        screen.h * targetAspectRatio);
    Variable<EntityMemoryId> h = If(lockAspectRatio == 0 || screen.aspectRatio > targetAspectRatio,
        screen.h,
        screen.w / targetAspectRatio);
    Variable<EntityMemoryId> l = -1 * w / 2;
    Variable<EntityMemoryId> r = w / 2;
    Variable<EntityMemoryId> t = h / 2;
    Variable<EntityMemoryId> b = -1 * h / 2;
}stage;
#endif

let t = If(
	lockAspectRatio && screen.aspectRatio < targetAspectRatio,
	screen.w / targetAspectRatio * 0.5,
	screen.t
);
let b = t - stage.h;

class judgment {
	public:

    let perfect = If(strictMode, 0.04, 0.08);
    let great = If(strictMode, 0.075, 0.16);
    let good = If(strictMode, 0.14,0.18);
}judgment;

class score {
	public:

	let perfect = 1;
	let great = 0.65;
	let good = 0;
}score;

Variable<LevelMemoryId> allocateJudgelineId;
Variable<LevelMemoryId> combo;
Variable<LevelMemoryId> maxCombo;
Variable<LevelMemoryId> judgeStatus;
Variable<LevelMemoryId> notes;
Variable<LevelMemoryId> accscore;
Variable<LevelMemoryId> maxTime;