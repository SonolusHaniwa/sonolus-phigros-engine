#ifndef ENGINEENUMS_H
#define ENGINEENUMS_H

using namespace std;

class NameText {
    public:

    string Keywords = "#KEYWORDS";
    string Name = "#NAME";
    string Rating = "#RATING";
    string MinimumRating = "#RATING_MINIMUM";
    string MaximumRating = "#RATING_MAXIMUM";
    string Title = "#TITLE";
    string Subtitle = "#SUBTITLE";
    string Artists = "#ARTISTS";
    string Author = "#AUTHOR";
    string Genre = "#GENRE";
    string Category = "#CATEGORY";
    string Language = "#LANGUAGE";
    string Difficulty = "#DIFFICULTY";
    string Length = "#LENGTH";
    string MinimumLength = "#LENGTH_MINIMUM";
    string MaximumLength = "#LENGTH_MAXIMUM";
    string LevelSpeed = "#SPEED";
    string AutoPlay = "#AUTO";
    string MirrorLevel = "#MIRROR";
    string Random = "#RANDOM";
    string Hidden = "#HIDDEN";
    string StrictJudgment = "#JUDGMENT_STRICT";
    string LooseJudgment = "#JUDGMENT_LOOSE";
    string SFX = "#EFFECT";
    string AutoSFX = "#EFFECT_AUTO";
    string StageVisibility = "#STAGE";
    string StagePosition = "#STAGE_POSITION";
    string StageSize = "#STAGE_SIZE";
    string StageRotation = "#STAGE_ROTATION";
    string StageTransparency = "#STAGE_ALPHA";
    string StageAnimation = "#STAGE_ANIMATION";
    string StageTilt = "#STAGE_TILT";
    string VerticalStageCover = "#STAGE_COVER_VERTICAL";
    string HorizontalStageCover = "#STAGE_COVER_HORIZONTAL";
    string StageCoverTransparency = "#STAGE_COVER_ALPHA";
    string LockStageAspectRatio = "#STAGE_ASPECTRATIO_LOCK";
    string StageEffect = "#STAGE_EFFECT";
    string StageEffectPosition = "#STAGE_EFFECT_POSITION";
    string StageEffectSize = "#STAGE_EFFECT_SIZE";
    string StageEffectRotation = "#STAGE_EFFECT_ROTATION";
    string StageEffectTransparency = "#STAGE_EFFECT_ALPHA";
    string LaneVisibility = "#LANE";
    string LanePosition = "#LANE_POSITION";
    string LaneSize = "#LANE_SIZE";
    string LaneRotation = "#LANE_ROTATION";
    string LaneTransparency = "#LANE_ALPHA";
    string LaneAnimation = "#LANE_ANIMATION";
    string LaneEffect = "#LANE_EFFECT";
    string LaneEffectPosition = "#LANE_EFFECT_POSITION";
    string LaneEffectSize = "#LANE_EFFECT_SIZE";
    string LaneEffectRotation = "#LANE_EFFECT_ROTATION";
    string LaneEffectTransparency = "#LANE_EFFECT_ALPHA";
    string JudgmentLineVisibility = "#JUDGELINE";
    string JudgmentLinePosition = "#JUDGELINE_POSITION";
    string JudgmentLineSize = "#JUDGELINE_SIZE";
    string JudgmentLineRotation = "#JUDGELINE_ROTATION";
    string JudgmentLineTransparency = "#JUDGELINE_ALPHA";
    string JudgmentLineAnimation = "#JUDGELINE_ANIMATION";
    string JudgmentLineEffect = "#JUDGELINE_EFFECT";
    string JudgmentLineEffectPosition = "#JUDGELINE_EFFECT_POSITION";
    string JudgmentLineEffectSize = "#JUDGELINE_EFFECT_SIZE";
    string JudgmentLineEffectRotation = "#JUDGELINE_EFFECT_ROTATION";
    string JudgmentLineEffectTransparency = "#JUDGELINE_EFFECT_ALPHA";
    string SlotVisibility = "#SLOT";
    string SlotPosition = "#SLOT_POSITION";
    string SlotSize = "#SLOT_SIZE";
    string SlotRotation = "#SLOT_ROTATION";
    string SlotTransparency = "#SLOT_ALPHA";
    string SlotAnimation = "#SLOT_ANIMATION";
    string SlotEffect = "#SLOT_EFFECT";
    string SlotEffectPosition = "#SLOT_EFFECT_POSITION";
    string SlotEffectSize = "#SLOT_EFFECT_SIZE";
    string SlotEffectRotation = "#SLOT_EFFECT_ROTATION";
    string SlotEffectTransparency = "#SLOT_EFFECT_ALPHA";
    string NoteVisibility = "#NOTE";
    string NoteSpeed = "#NOTE_SPEED";
    string RandomNoteSpeed = "#NOTE_SPEED_RANDOM";
    string NotePosition = "#NOTE_POSITION";
    string NoteSize = "#NOTE_SIZE";
    string NoteRotation = "#NOTE_ROTATION";
    string NoteTransparency = "#NOTE_ALPHA";
    string NoteAnimation = "#NOTE_ANIMATION";
    string NoteEffect = "#NOTE_EFFECT";
    string NoteEffectPosition = "#NOTE_EFFECT_POSITION";
    string NoteEffectSize = "#NOTE_EFFECT_SIZE";
    string NoteEffectRotation = "#NOTE_EFFECT_ROTATION";
    string NoteEffectTransparency = "#NOTE_EFFECT_ALPHA";
    string MarkerVisibility = "#MARKER";
    string MarkerPosition = "#MARKER_POSITION";
    string MarkerSize = "#MARKER_SIZE";
    string MarkerRotation = "#MARKER_ROTATION";
    string MarkerTransparency = "#MARKER_ALPHA";
    string MarkerAnimation = "#MARKER_ANIMATION";
    string ConnectorVisibility = "#CONNECTOR";
    string ConnectorPosition = "#CONNECTOR_POSITION";
    string ConnectorSize = "#CONNECTOR_SIZE";
    string ConnectorRotation = "#CONNECTOR_ROTATION";
    string ConnectorTransparency = "#CONNECTOR_ALPHA";
    string ConnectorAnimation = "#CONNECTOR_ANIMATION";
    string SimultaneousLineVisibility = "#SIMLINE";
    string SimultaneousLinePosition = "#SIMLINE_POSITION";
    string SimultaneousLineSize = "#SIMLINE_SIZE";
    string SimultaneousLineRotation = "#SIMLINE_ROTATION";
    string SimultaneousLineTransparency = "#SIMLINE_ALPHA";
    string SimultaneousLineAnimation = "#SIMLINE_ANIMATION";
}NameText;

class UnitText {
    public:

    string Percentage = "#PERCENTAGE";
    string Second = "#SECOND";
    string Millisecond = "#MILLISECOND";
}UnitText;

class InstructionText {
    public:

    string Tap = "#TAP";
    string TapAndHold = "#TAP_HOLD";
    string TapAndRelease = "#TAP_RELEASE";
    string TapAndFlick = "#TAP_FLICK";
    string TapAndSlide = "#TAP_SLIDE";
    string Hold = "#HOLD";
    string HoldAndSlide = "#HOLD_SLIDE";
    string HoldAndFollow = "#HOLD_FOLLOW";
    string Release = "#RELEASE";
    string Flick = "#FLICK";
    string Slide = "#SLIDE";
    string SlideAndFlick = "#SLIDE_FLICK";
    string Avoid = "#AVOID";
    string Jiggle = "#JIGGLE";
}InstructionText;

class InstructionIconName {
    public:
    
    string Hand = "#HAND";
    string Arrow = "#ARROW";
}InstructionIconName;

class OptionType {
    public:

    string Toggle = "toggle";
    string Slider = "slider";
    string Select = "select";
}OptionType;

class Metric {
    public:

    string arcade = "arcade";
    string accuracy = "accuracy";
    string life = "life";
    string perfectRate = "perfectRate";
    string errorHeatmap = "errorHeatmap";
}Metric;

class AnimationEase {
    public:

    string Linear = "Linear";
    string InSine = "InSine";
    string InQuad = "InQuad";
    string InCubic = "InCubic";
    string InQuart = "InQuart";
    string InExpo = "InExpo";
    string InCirc = "InCirc";
    string InBack = "InBack";
    string InElastic = "InElastic";
    string OutSine = "OutSine";
    string OutQuad = "OutQuad";
    string OutCubic = "OutCubic";
    string OutQuart = "OutQuart";
    string OutExpo = "OutExpo";
    string OutCirc = "OutCirc";
    string OutBack = "OutBack";
    string OutElastic = "OutElastic";
    string InOutSine = "InOutSine";
    string InOutQuad = "InOutQuad";
    string InOutCubic = "InOutCubic";
    string InOutQuart = "InOutQuart";
    string InOutExpo = "InOutExpo";
    string InOutCirc = "InOutCirc";
    string InOutBack = "InOutBack";
    string InOutElastic = "InOutElastic";
    string OutInSine = "OutInSine";
    string OutInQuad = "OutInQuad";
    string OutInCubic = "OutInCubic";
    string OutInQuart = "OutInQuart";
    string OutInExpo = "OutInExpo";
    string OutInCirc = "OutInCirc";
    string OutInBack = "OutInBack";
    string OutInElastic = "OutInElastic";
    string None = "None";
}AnimationEase;

class ErrorStyle {
    public:

    string none = "none";
    string plus = "plus";
    string minus = "minus";
    string arrowUp = "arrowUp";
    string arrowDown = "arrowDown";
    string arrowLeft = "arrowLeft";
    string arrowRight = "arrowRight";
    string triangleUp = "triangleUp";
    string triangleDown = "triangleDown";
    string triangleLeft = "triangleLeft";
    string triangleRight = "triangleRight";
}ErrorStyle;

class ErrorPlacement {
    public:

    string both = "both";
    string left = "left";
    string right = "right";
}ErrorPlacement;

class RuntimeFunction {
    public:

    string Constant = "Constant";
    string Execute = "Execute";
    string If = "If";
    string Switch = "Switch";
    string SwitchWithDefault = "SwitchWithDefault";
    string SwitchInteger = "SwitchInteger";
    string SwitchIntegerWithDefault = "SwitchIntegerWithDefault";
    string While = "While";
    string Add = "Add";
    string Subtract = "Subtract";
    string Multiply = "Multiply";
    string Divide = "Divide";
    string Mod = "Mod";
    string Power = "Power";
    string Log = "Log";
    string Equal = "Equal";
    string NotEqual = "NotEqual";
    string Greater = "Greater";
    string GreaterOr = "GreaterOr";
    string Less = "Less";
    string LessOr = "LessOr";
    string And = "And";
    string Or = "Or";
    string Not = "Not";
    string Abs = "Abs";
    string Sign = "Sign";
    string Min = "Min";
    string Max = "Max";
    string Ceil = "Ceil";
    string Floor = "Floor";
    string Round = "Round";
    string Frac = "Frac";
    string Trunc = "Trunc";
    string Degree = "Degree";
    string Radian = "Radian";
    string Sin = "Sin";
    string Cos = "Cos";
    string Tan = "Tan";
    string Sinh = "Sinh";
    string Cosh = "Cosh";
    string Tanh = "Tanh";
    string Arcsin = "Arcsin";
    string Arccos = "Arccos";
    string Arctan = "Arctan";
    string Arctan2 = "Arctan2";
    string Clamp = "Clamp";
    string Lerp = "Lerp";
    string LerpClamped = "LerpClamped";
    string Unlerp = "Unlerp";
    string UnlerpClamped = "UnlerpClamped";
    string Remap = "Remap";
    string RemapClamped = "RemapClamped";
    string Smoothstep = "Smoothstep";
    string Random = "Random";
    string RandomInteger = "RandomInteger";
    string Get = "Get";
    string GetShifted = "GetShifted";
    string Set = "Set";
    string SetShifted = "SetShifted";
    string Draw = "Draw";
    string DrawCurvedL = "DrawCurvedL";
    string DrawCurvedR = "DrawCurvedR";
    string DrawCurvedLR = "DrawCurvedLR";
    string DrawCurvedB = "DrawCurvedB";
    string DrawCurvedT = "DrawCurvedT";
    string DrawCurvedBT = "DrawCurvedBT";
    string Play = "Play";
    string PlayScheduled = "PlayScheduled";
    string Spawn = "Spawn";
    string SpawnParticleEffect = "SpawnParticleEffect";
    string MoveParticleEffect = "MoveParticleEffect";
    string DestroyParticleEffect = "DestroyParticleEffect";
    string HasSkinSprite = "HasSkinSprite";
    string HasEffectClip = "HasEffectClip";
    string HasParticleEffect = "HasParticleEffect";
    string Judge = "Judge";
    string JudgeSimple = "JudgeSimple";
    string EaseInSine = "EaseInSine";
    string EaseInQuad = "EaseInQuad";
    string EaseInCubic = "EaseInCubic";
    string EaseInQuart = "EaseInQuart";
    string EaseInQuint = "EaseInQuint";
    string EaseInExpo = "EaseInExpo";
    string EaseInCirc = "EaseInCirc";
    string EaseInBack = "EaseInBack";
    string EaseInElastic = "EaseInElastic";
    string EaseOutSine = "EaseOutSine";
    string EaseOutQuad = "EaseOutQuad";
    string EaseOutCubic = "EaseOutCubic";
    string EaseOutQuart = "EaseOutQuart";
    string EaseOutQuint = "EaseOutQuint";
    string EaseOutExpo = "EaseOutExpo";
    string EaseOutCirc = "EaseOutCirc";
    string EaseOutBack = "EaseOutBack";
    string EaseOutElastic = "EaseOutElastic";
    string EaseInOutSine = "EaseInOutSine";
    string EaseInOutQuad = "EaseInOutQuad";
    string EaseInOutCubic = "EaseInOutCubic";
    string EaseInOutQuart = "EaseInOutQuart";
    string EaseInOutQuint = "EaseInOutQuint";
    string EaseInOutExpo = "EaseInOutExpo";
    string EaseInOutCirc = "EaseInOutCirc";
    string EaseInOutBack = "EaseInOutBack";
    string EaseInOutElastic = "EaseInOutElastic";
    string EaseOutInSine = "EaseOutInSine";
    string EaseOutInQuad = "EaseOutInQuad";
    string EaseOutInCubic = "EaseOutInCubic";
    string EaseOutInQuart = "EaseOutInQuart";
    string EaseOutInQuint = "EaseOutInQuint";
    string EaseOutInExpo = "EaseOutInExpo";
    string EaseOutInCirc = "EaseOutInCirc";
    string EaseOutInBack = "EaseOutInBack";
    string EaseOutInElastic = "EaseOutInElastic";
    string IsDebug = "IsDebug";
    string DebugPause = "DebugPause";
    string DebugLog = "DebugLog";
    string JumpLoop = "JumpLoop";
    string Paint = "Paint";
    string PlayLooped = "PlayLooped";
    string PlayLoopedScheduled = "PlayLoopedScheduled";
    string StopLooped = "StopLooped";
    string StopLoopedScheduled = "StopLoopedScheduled";
    string BeatToBPM = "BeatToBPM";
    string BeatToStartingBeat = "BeatToStartingBeat";
    string BeatToStartingTime = "BeatToStartingTime";
    string BeatToTime = "BeatToTime";
    string Block = "Block";
    string Break = "Break";
    string Copy = "Copy";
    string DecrementPostPointed = "DecrementPostPointed";
    string DecrementPostShifted = "DecrementPostShifted";
    string DecrementPost = "DecrementPost";
    string DecrementPrePointed = "DecrementPrePointed";
    string DecrementPreShifted = "DecrementPreShifted";
    string DecrementPre = "DecrementPre";
    string DoWhile = "DoWhile";
    string Execute0 = "Execute0";
    string GetPointed = "GetPointed";
    string IncrementPostPointed = "IncrementPostPointed";
    string IncrementPostShifted = "IncrementPostShifted";
    string IncrementPost = "IncrementPost";
    string IncrementPrePointed = "IncrementPrePointed";
    string IncrementPreShifted = "IncrementPreShifted";
    string IncrementPre = "IncrementPre";
    string Negate = "Negate";
    string Rem = "Rem";
    string SetAddPointed = "SetAddPointed";
    string SetAddShifted = "SetAddShifted";
    string SetAdd = "SetAdd";
    string SetDividePointed = "SetDividePointed";
    string SetDivideShifted = "SetDivideShifted";
    string SetDivide = "SetDivide";
    string SetModPointed = "SetModPointed";
    string SetModShifted = "SetModShifted";
    string SetMod = "SetMod";
    string SetMultiplyPointed = "SetMultiplyPointed";
    string SetMultiplyShifted = "SetMultiplyShifted";
    string SetMultiply = "SetMultiply";
    string SetSubtractPointed = "SetSubtractPointed";
    string SetSubtractShifted = "SetSubtractShifted";
    string SetSubtract = "SetSubtract";
    string SetPointed = "SetPointed";
    string SetPowerPointed = "SetPowerPointed";
    string SetPowerShifted = "SetPowerShifted";
    string SetPower = "SetPower";
    string SetRemPointed = "SetRemPointed";
    string SetRemShifted = "SetRemShifted";
    string SetRem = "SetRem";
    string StackEnter = "StackEnter";
    string StackGetFramePointer = "StackGetFramePointer";
    string StackGetFrame = "StackGetFrame";
    string StackGetPointer = "StackGetPointer";
    string StackGet = "StackGet";
    string StackGrow = "StackGrow";
    string StackInit = "StackInit";
    string StackLeave = "StackLeave";
    string StackPop = "StackPop";
    string StackPush = "StackPush";
    string StackSetFramePointer = "StackSetFramePointer";
    string StackSetFrame = "StackSetFrame";
    string StackSetPointer = "StackSetPointer";
    string StackSet = "StackSet";
    string TimeToScaledTime = "TimeToScaledTime";
    string TimeToStartingScaledTime = "TimeToStartingScaledTime";
    string TimeToStartingTime = "TimeToStartingTime";
    string TimeToTimeScale = "TimeToTimeScale";
    string Print = "Print";
}RuntimeFunction;

class SkinSpriteName {
    public:

    string NoteHeadNeutral = "#NOTE_HEAD_NEUTRAL";
    string NoteHeadRed = "#NOTE_HEAD_RED";
    string NoteHeadGreen = "#NOTE_HEAD_GREEN";
    string NoteHeadBlue = "#NOTE_HEAD_BLUE";
    string NoteHeadYellow = "#NOTE_HEAD_YELLOW";
    string NoteHeadPurple = "#NOTE_HEAD_PURPLE";
    string NoteHeadCyan = "#NOTE_HEAD_CYAN";

    string NoteTickNeutral = "#NOTE_TICK_NEUTRAL";
    string NoteTickRed = "#NOTE_TICK_RED";
    string NoteTickGreen = "#NOTE_TICK_GREEN";
    string NoteTickBlue = "#NOTE_TICK_BLUE";
    string NoteTickYellow = "#NOTE_TICK_YELLOW";
    string NoteTickPurple = "#NOTE_TICK_PURPLE";
    string NoteTickCyan = "#NOTE_TICK_CYAN";

    string NoteTailNeutral = "#NOTE_TAIL_NEUTRAL";
    string NoteTailRed = "#NOTE_TAIL_RED";
    string NoteTailGreen = "#NOTE_TAIL_GREEN";
    string NoteTailBlue = "#NOTE_TAIL_BLUE";
    string NoteTailYellow = "#NOTE_TAIL_YELLOW";
    string NoteTailPurple = "#NOTE_TAIL_PURPLE";
    string NoteTailCyan = "#NOTE_TAIL_CYAN";

    string NoteConnectionNeutral = "#NOTE_CONNECTION_NEUTRAL";
    string NoteConnectionRed = "#NOTE_CONNECTION_RED";
    string NoteConnectionGreen = "#NOTE_CONNECTION_GREEN";
    string NoteConnectionBlue = "#NOTE_CONNECTION_BLUE";
    string NoteConnectionYellow = "#NOTE_CONNECTION_YELLOW";
    string NoteConnectionPurple = "#NOTE_CONNECTION_PURPLE";
    string NoteConnectionCyan = "#NOTE_CONNECTION_CYAN";

    string NoteConnectionNeutralSeamless = "#NOTE_CONNECTION_NEUTRAL_SEAMLESS";
    string NoteConnectionRedSeamless = "#NOTE_CONNECTION_RED_SEAMLESS";
    string NoteConnectionGreenSeamless = "#NOTE_CONNECTION_GREEN_SEAMLESS";
    string NoteConnectionBlueSeamless = "#NOTE_CONNECTION_BLUE_SEAMLESS";
    string NoteConnectionYellowSeamless = "#NOTE_CONNECTION_YELLOW_SEAMLESS";
    string NoteConnectionPurpleSeamless = "#NOTE_CONNECTION_PURPLE_SEAMLESS";
    string NoteConnectionCyanSeamless = "#NOTE_CONNECTION_CYAN_SEAMLESS";

    string SimultaneousConnectionNeutral = "#SIMULTANEOUS_CONNECTION_NEUTRAL";
    string SimultaneousConnectionRed = "#SIMULTANEOUS_CONNECTION_RED";
    string SimultaneousConnectionGreen = "#SIMULTANEOUS_CONNECTION_GREEN";
    string SimultaneousConnectionBlue = "#SIMULTANEOUS_CONNECTION_BLUE";
    string SimultaneousConnectionYellow = "#SIMULTANEOUS_CONNECTION_YELLOW";
    string SimultaneousConnectionPurple = "#SIMULTANEOUS_CONNECTION_PURPLE";
    string SimultaneousConnectionCyan = "#SIMULTANEOUS_CONNECTION_CYAN";

    string SimultaneousConnectionNeutralSeamless = "#SIMULTANEOUS_CONNECTION_NEUTRAL_SEAMLESS";
    string SimultaneousConnectionRedSeamless = "#SIMULTANEOUS_CONNECTION_RED_SEAMLESS";
    string SimultaneousConnectionGreenSeamless = "#SIMULTANEOUS_CONNECTION_GREEN_SEAMLESS";
    string SimultaneousConnectionBlueSeamless = "#SIMULTANEOUS_CONNECTION_BLUE_SEAMLESS";
    string SimultaneousConnectionYellowSeamless = "#SIMULTANEOUS_CONNECTION_YELLOW_SEAMLESS";
    string SimultaneousConnectionPurpleSeamless = "#SIMULTANEOUS_CONNECTION_PURPLE_SEAMLESS";
    string SimultaneousConnectionCyanSeamless = "#SIMULTANEOUS_CONNECTION_CYAN_SEAMLESS";

    string DirectionalMarkerNeutral = "#DIRECTIONAL_MARKER_NEUTRAL";
    string DirectionalMarkerRed = "#DIRECTIONAL_MARKER_RED";
    string DirectionalMarkerGreen = "#DIRECTIONAL_MARKER_GREEN";
    string DirectionalMarkerBlue = "#DIRECTIONAL_MARKER_BLUE";
    string DirectionalMarkerYellow = "#DIRECTIONAL_MARKER_YELLOW";
    string DirectionalMarkerPurple = "#DIRECTIONAL_MARKER_PURPLE";
    string DirectionalMarkerCyan = "#DIRECTIONAL_MARKER_CYAN";

    string SimultaneousMarkerNeutral = "#SIMULTANEOUS_MARKER_NEUTRAL";
    string SimultaneousMarkerRed = "#SIMULTANEOUS_MARKER_RED";
    string SimultaneousMarkerGreen = "#SIMULTANEOUS_MARKER_GREEN";
    string SimultaneousMarkerBlue = "#SIMULTANEOUS_MARKER_BLUE";
    string SimultaneousMarkerYellow = "#SIMULTANEOUS_MARKER_YELLOW";
    string SimultaneousMarkerPurple = "#SIMULTANEOUS_MARKER_PURPLE";
    string SimultaneousMarkerCyan = "#SIMULTANEOUS_MARKER_CYAN";

    string StageMiddle = "#STAGE_MIDDLE";

    string StageLeftBorder = "#STAGE_LEFT_BORDER";
    string StageRightBorder = "#STAGE_RIGHT_BORDER";
    string StageTopBorder = "#STAGE_TOP_BORDER";
    string StageBottomBorder = "#STAGE_BOTTOM_BORDER";
    string StageLeftBorderSeamless = "#STAGE_LEFT_BORDER_SEAMLESS";
    string StageRightBorderSeamless = "#STAGE_RIGHT_BORDER_SEAMLESS";
    string StageTopBorderSeamless = "#STAGE_TOP_BORDER_SEAMLESS";
    string StageBottomBorderSeamless = "#STAGE_BOTTOM_BORDER_SEAMLESS";

    string StageTopLeftCorner = "#STAGE_TOP_LEFT_CORNER";
    string StageTopRightCorner = "#STAGE_TOP_RIGHT_CORNER";
    string StageBottomLeftCorner = "#STAGE_BOTTOM_LEFT_CORNER";
    string StageBottomRightCorner = "#STAGE_BOTTOM_RIGHT_CORNER";

    string Lane = "#LANE";
    string LaneSeamless = "#LANE_SEAMLESS";

    string LaneAlternative = "#LANE_ALTERNATIVE";
    string LaneAlternativeSeamless = "#LANE_ALTERNATIVE_SEAMLESS";

    string JudgmentLine = "#JUDGMENT_LINE";
    string NoteSlot = "#NOTE_SLOT";

    string StageCover = "#STAGE_COVER";

    string GridNeutral = "#GRID_NEUTRAL";
    string GridRed = "#GRID_RED";
    string GridGreen = "#GRID_GREEN";
    string GridBlue = "#GRID_BLUE";
    string GridYellow = "#GRID_YELLOW";
    string GridPurple = "#GRID_PURPLE";
    string GridCyan = "#GRID_CYAN";
}SkinSpriteName;

class EffectClipName {
    public:

    string Miss = "#MISS";
    string Perfect = "#PERFECT";
    string Great = "#GREAT";
    string Good = "#GOOD";

    string Hold = "#HOLD";

    string MissAlternative = "#MISS_ALTERNATIVE";
    string PerfectAlternative = "#PERFECT_ALTERNATIVE";
    string GreatAlternative = "#GREAT_ALTERNATIVE";
    string GoodAlternative = "#GOOD_ALTERNATIVE";

    string HoldAlternative = "#HOLD_ALTERNATIVE";

    string Stage = "#STAGE";
}EffectClipName;

class ParticleEffectName {
    public:

    string NoteCircularTapNeutral = "#NOTE_CIRCULAR_TAP_NEUTRAL";
    string NoteCircularTapRed = "#NOTE_CIRCULAR_TAP_RED";
    string NoteCircularTapGreen = "#NOTE_CIRCULAR_TAP_GREEN";
    string NoteCircularTapBlue = "#NOTE_CIRCULAR_TAP_BLUE";
    string NoteCircularTapYellow = "#NOTE_CIRCULAR_TAP_YELLOW";
    string NoteCircularTapPurple = "#NOTE_CIRCULAR_TAP_PURPLE";
    string NoteCircularTapCyan = "#NOTE_CIRCULAR_TAP_CYAN";

    string NoteCircularAlternativeNeutral = "#NOTE_CIRCULAR_ALTERNATIVE_NEUTRAL";
    string NoteCircularAlternativeRed = "#NOTE_CIRCULAR_ALTERNATIVE_RED";
    string NoteCircularAlternativeGreen = "#NOTE_CIRCULAR_ALTERNATIVE_GREEN";
    string NoteCircularAlternativeBlue = "#NOTE_CIRCULAR_ALTERNATIVE_BLUE";
    string NoteCircularAlternativeYellow = "#NOTE_CIRCULAR_ALTERNATIVE_YELLOW";
    string NoteCircularAlternativePurple = "#NOTE_CIRCULAR_ALTERNATIVE_PURPLE";
    string NoteCircularAlternativeCyan = "#NOTE_CIRCULAR_ALTERNATIVE_CYAN";

    string NoteCircularHoldNeutral = "#NOTE_CIRCULAR_HOLD_NEUTRAL";
    string NoteCircularHoldRed = "#NOTE_CIRCULAR_HOLD_RED";
    string NoteCircularHoldGreen = "#NOTE_CIRCULAR_HOLD_GREEN";
    string NoteCircularHoldBlue = "#NOTE_CIRCULAR_HOLD_BLUE";
    string NoteCircularHoldYellow = "#NOTE_CIRCULAR_HOLD_YELLOW";
    string NoteCircularHoldPurple = "#NOTE_CIRCULAR_HOLD_PURPLE";
    string NoteCircularHoldCyan = "#NOTE_CIRCULAR_HOLD_CYAN";

    string NoteLinearTapNeutral = "#NOTE_LINEAR_TAP_NEUTRAL";
    string NoteLinearTapRed = "#NOTE_LINEAR_TAP_RED";
    string NoteLinearTapGreen = "#NOTE_LINEAR_TAP_GREEN";
    string NoteLinearTapBlue = "#NOTE_LINEAR_TAP_BLUE";
    string NoteLinearTapYellow = "#NOTE_LINEAR_TAP_YELLOW";
    string NoteLinearTapPurple = "#NOTE_LINEAR_TAP_PURPLE";
    string NoteLinearTapCyan = "#NOTE_LINEAR_TAP_CYAN";

    string NoteLinearAlternativeNeutral = "#NOTE_LINEAR_ALTERNATIVE_NEUTRAL";
    string NoteLinearAlternativeRed = "#NOTE_LINEAR_ALTERNATIVE_RED";
    string NoteLinearAlternativeGreen = "#NOTE_LINEAR_ALTERNATIVE_GREEN";
    string NoteLinearAlternativeBlue = "#NOTE_LINEAR_ALTERNATIVE_BLUE";
    string NoteLinearAlternativeYellow = "#NOTE_LINEAR_ALTERNATIVE_YELLOW";
    string NoteLinearAlternativePurple = "#NOTE_LINEAR_ALTERNATIVE_PURPLE";
    string NoteLinearAlternativeCyan = "#NOTE_LINEAR_ALTERNATIVE_CYAN";

    string NoteLinearHoldNeutral = "#NOTE_LINEAR_HOLD_NEUTRAL";
    string NoteLinearHoldRed = "#NOTE_LINEAR_HOLD_RED";
    string NoteLinearHoldGreen = "#NOTE_LINEAR_HOLD_GREEN";
    string NoteLinearHoldBlue = "#NOTE_LINEAR_HOLD_BLUE";
    string NoteLinearHoldYellow = "#NOTE_LINEAR_HOLD_YELLOW";
    string NoteLinearHoldPurple = "#NOTE_LINEAR_HOLD_PURPLE";
    string NoteLinearHoldCyan = "#NOTE_LINEAR_HOLD_CYAN";

    string LaneCircular = "#LANE_CIRCULAR";
    string LaneLinear = "#LANE_LINEAR";

    string SlotCircular = "#SLOT_CIRCULAR";
    string SlotLinear = "#SLOT_LINEAR";

    string JudgeLineCircular = "#JUDGE_LINE_CIRCULAR";
    string JudgeLineLinear = "#JUDGE_LINE_LINEAR";
}ParticleEffectName;

class Scroll {
    public:

    int LeftToRight = 0;
    int TopToBottom = 1;
    int RightToLeft = 2;
    int BottomToTop = 3;
}Scroll;

class PrintColor {
    public:

    int Theme = -1;
    int Neutral = 0;
    int Red = 1;
    int Green = 2;
    int Blue = 3;
    int Yellow = 4;
    int Purple = 5;
    int Cyan = 6;
}PrintColor;

class PrintFormat {
    public:

    int Number = 0;
    int Percentage = 1;
    int Time = 10;
    int Score = 11;
    int BPM = 20;
    int TimeScale = 21;
    int BeatCount = 30;
    int MeasureCount = 31;
    int EntityCount = 32;
}PrintFormat;

#endif