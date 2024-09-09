double PI  = acos(-1);
double lockAspectRatio = 1;
double interfaceGap = 0.05;
double targetAspectRatio = 16.0 / 9.0;
double judgelineHeight = 0.01;
double baseNoteWidth = 0.48;
double minSFXDistance = 0.02;
double noteRatio = 2;
double dragRatio = 2;
double flickRatio = 2;
double hlNoteRatio = 2;
double hlDragRatio = 2;
double hlFlickRatio = 2;
double holdHeadRatio = 989.0 / 50.0;
double holdBodyRatio = 989.0 / 1950.0;
double hlHoldHeadRatio = 1062.0 / 97.0;
double hlHoldBodyRatio = 1062.0 / 1951.0;
double hlHoldOffset = 1062.0 / 49.0;
double fallBackNoteRatio = 8;
double judgeDistanceLimit = baseNoteWidth * 0.875;
double effectDurationTime = 0.4;
double effectWidth = baseNoteWidth * 0.75;
double holdTailTime = 0.18;
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
double tipRatio = 2015.0 / 157.0;
double scheduledSFXTime = 0.5;

#if play || watch
let strictMode = LevelOption.get(Options.StrictMode);
let mirror = LevelOption.get(Options.Mirror);
let levelSpeed = LevelOption.get(Options.LevelSpeed);
let noteWidth2 = baseNoteWidth * LevelOption.get(Options.NoteSize);
let hasSFX = LevelOption.get(Options.HasSFX);
let hasSimul = LevelOption.get(Options.HasSimul);
let hasIndicator = LevelOption.get(Options.HasIndicator) && HasSkinSprite(Sprites.AllPerfectJudgeline) && HasSkinSprite(Sprites.FullComboJudgeline);
let background = LevelOption.get(Options.Background);
let sonolusPause = LevelOption.get(Options.SystemPause);
let sonolusCombo = LevelOption.get(Options.SystemCombo);
let sonolusPrimaryMetric = LevelOption.get(Options.SystemPrimaryMetric);
let hasJudgelineId = LevelOption.get(Options.JudgelineId);
let autoSFX = LevelOption.get(Options.AutoSFX);
#endif

#if play || watch || tutorial
class stage {
	public:

	let w = If(screen.aspectRatio < targetAspectRatio,
        screen.w,
        screen.h * targetAspectRatio);
    let h = screen.h;
    let l = -1 * w / 2;
    let r = w / 2;
    let t = h / 2;
    let b = -1 * h / 2;
}stage;
#endif

#if play || watch
let noteWidth = noteWidth2 * stage.w / 3.55;
class judgment {
	public:

    let perfect = If(strictMode, 0.04, 0.08);
    let great = If(strictMode, 0.075, 0.16);
    let good = If(strictMode, 0.14,0.18);
}judgment;
#endif

class score {
	public:

	let perfect = 1;
	let great = 0.65;
	let good = 0;
}score;

#if play || watch
Variable<LevelMemoryId> allocateJudgelineId;
Variable<LevelMemoryId> combo;
Variable<LevelMemoryId> maxCombo;
Variable<LevelMemoryId> judgeStatus;
Variable<LevelMemoryId> notes;
Variable<LevelMemoryId> accscore;
Variable<LevelMemoryId> maxTime;
Variable<LevelMemoryId> lastUpdatedId;
#elif tutorial
Variable<TutorialMemoryId> spawnedEffect;
#endif