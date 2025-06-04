FuncNode PI  = acos(-1);
FuncNode lockAspectRatio = 1;
FuncNode interfaceGap = 0.05;
FuncNode targetAspectRatio = 16.0 / 9.0;
FuncNode judgelineHeight = 0.01;
FuncNode baseNoteWidth = 0.48;
FuncNode minSFXDistance = 0.02;
FuncNode noteRatio = 2;
FuncNode dragRatio = 2;
FuncNode flickRatio = 2;
FuncNode hlNoteRatio = 2;
FuncNode hlDragRatio = 2;
FuncNode hlFlickRatio = 2;
FuncNode holdHeadRatio = 989.0 / 50.0;
FuncNode holdBodyRatio = 989.0 / 1950.0;
FuncNode hlHoldHeadRatio = 1062.0 / 97.0;
FuncNode hlHoldBodyRatio = 1062.0 / 1951.0;
FuncNode hlHoldOffset = 1062.0 / 49.0;
FuncNode fallBackNoteRatio = 8;
FuncNode judgeDistanceLimit = baseNoteWidth * 0.875;
FuncNode effectDurationTime = 0.4;
FuncNode effectWidth = baseNoteWidth * 0.75;
FuncNode holdTailTime = 0.18;
FuncNode comboTextRatio = 297.0 / 62.0;
FuncNode combo0Ratio = 46.0 / 62.0;
FuncNode combo1Ratio = 31.0 / 60.0;
FuncNode combo2Ratio = 41.0 / 61.0;
FuncNode combo3Ratio = 41.0 / 62.0;
FuncNode combo4Ratio = 49.0 / 60.0;
FuncNode combo5Ratio = 41.0 / 61.0;
FuncNode combo6Ratio = 46.0 / 62.0;
FuncNode combo7Ratio = 42.0 / 60.0;
FuncNode combo8Ratio = 47.0 / 62.0;
FuncNode combo9Ratio = 46.0 / 62.0;
FuncNode floorPositionLimit = -0.001;
FuncNode timeBarHeight = 0.01;
FuncNode timeMagic = 1.875;
FuncNode judgelineIdTextHeight = 0.05;
FuncNode tipRatio = 2015.0 / 157.0;
FuncNode scheduledSFXTime = 0.5;

#if defined(play) || defined(watch)
FuncNode strictMode = Options.StrictMode;
FuncNode mirror = Options.Mirror;
FuncNode levelSpeed = Options.LevelSpeed;
FuncNode noteWidth2 = baseNoteWidth * Options.NoteSize;
FuncNode hasSFX = Options.HasSFX;
FuncNode hasSimul = Options.HasSimul;
FuncNode hasIndicator = Options.HasIndicator && HasSkinSprite(Sprites.AllPerfectJudgeline) && HasSkinSprite(Sprites.FullComboJudgeline);
FuncNode background = Options.Background;
FuncNode sonolusPause = Options.SystemPause;
FuncNode sonolusCombo = Options.SystemCombo;
FuncNode sonolusPrimaryMetric = Options.SystemPrimaryMetric;
FuncNode hasJudgelineId = Options.JudgelineId;
FuncNode autoSFX = Options.AutoSFX;
#endif

class screen {
    public:

    FuncNode aspectRatio = RuntimeEnvironment.screenAspectRatio;
    FuncNode l = -1 * aspectRatio;
    FuncNode b = -1;
    FuncNode r = aspectRatio;
    FuncNode t = 1;
    FuncNode w = aspectRatio * 2;
    FuncNode h = 2;
}screen;

#if defined(play) || defined(watch) || defined(tutorial)

class stage {
	public:

	FuncNode w = If(
        screen.aspectRatio < targetAspectRatio,
        screen.w,
        screen.h * targetAspectRatio
    );
    FuncNode h = screen.h;
    FuncNode l = -1 * w / 2;
    FuncNode r = w / 2;
    FuncNode t = h / 2;
    FuncNode b = -1 * h / 2;
}stage;
#endif

#if defined(play) || defined(watch)
FuncNode noteWidth = noteWidth2 * stage.w / 3.55;
class judgment {
	public:

    FuncNode perfect = If(strictMode, 0.04, 0.08);
    FuncNode great = If(strictMode, 0.075, 0.16);
    FuncNode good = If(strictMode, 0.14, 0.18);
}judgment;
#endif

class score {
	public:

	FuncNode perfect = 1;
	FuncNode great = 0.65;
	FuncNode good = 0;
}score;

#if defined(play) || defined(watch)
var allocateJudgelineId;
var combo;
var maxCombo;
var judgeStatus;
var notes;
var accscore;
var maxTime;
var lastUpdatedId;
#elif defined(tutorial)
var spawnedEffect;
#endif