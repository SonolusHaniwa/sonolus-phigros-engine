double PI  = acos(-1);
double lockAspectRatio = 1;
double extraWidth = 1;
double targetAspectRatio = 1920.0 / 1080.0;
double judgelineHeight = 0.01;
double baseNoteWidth = 0.24;
double minSFXDistance = 0.02;
double effectDurationTime = 0.4;
double noteRatio = 989.0 / 100.0;
double dragRatio = 989.0 / 60.0;
double flickRatio = 989.0 / 200.0;
double hlNoteRatio = 1089.0 / 200.0;
double hlDragRatio = 1089.0 / 160.0;
double hlFlickRatio = 1089.0 / 300.0;
double holdHeadRatio = 989.0 / 50.0;
double holdBodyRatio = 989.0 / 1950.0;
double hlHoldHeadRatio = 1062.0 / 97.0;
double hlHoldBodyRatio = 1062.0 / 1951.0;
double hlHoldOffset = 1062.0 / 49.0;
double judgeDistanceLimit = baseNoteWidth * 1.5;
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

#if play || watch
let noteWidth = baseNoteWidth * LevelOption.get(Options.NoteSize);
let hasSFX = LevelOption.get(Options.HasSFX);
let hasSimul = LevelOption.get(Options.HasSimul);
let hasIndicator = LevelOption.get(Options.HasIndicator);
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

    let perfect = 0.08;
    let great = 0.16;
    let good = 0.18;
}judgment;

class score {
	public:

	let perfect = 1;
	let great = 0.65;
	let good = 0;
}score;

Variable<LevelMemoryId> combo;
Variable<LevelMemoryId> maxCombo;
Variable<LevelMemoryId> judgeStatus;
Variable<LevelMemoryId> notes;
Variable<LevelMemoryId> accscore;