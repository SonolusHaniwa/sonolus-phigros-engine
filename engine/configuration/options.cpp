using namespace std;

class Options {
    public:

    int StrictMode = 0;
    int LevelSpeed = 1;
    int NoteSize = 2;
    int HasSFX = 3;
    int HasSimul = 4;
    int HasIndicator = 5;
    int Background = 6;
    int SystemPause = 7;
    int SystemCombo = 8;
    int SystemPrimaryMetric = 9;
    int JudgelineId = 10;
    int AutoSFX = 11;
}Options;

auto options = defineOptions<class Options>({
    {
        name: NameText.StrictJudgment,
        scope: Scope,
        standard: 1,
        type: OptionType.Toggle,
        def: false
    }, {
        name: NameText.LevelSpeed,
        scope: Scope,
        standard: 1,
        type: OptionType.Slider,
        def: 1,
        min: 0.5,
        max: 2,
        step: 0.05,
        unit: UnitText.Percentage,
    }, {
        name: NameText.NoteSize,
        scope: Scope,
        standard: 0,
        type: OptionType.Slider,
        def: 1.0,
        min: 0.8,
        max: 1.2,
        step: 0.005,
    }, {
        name: NameText.SFX,
        scope: Scope,
        standard: 0,
        type: OptionType.Toggle,
        def: 1
    }, {
        name: "Highlight Simultaneous Notes",
        scope: Scope,
        standard: 0,
        type: OptionType.Toggle,
        def: 1
    }, {
        name: "FC/AP Indicator",
        scope: Scope,
        standard: 0,
        type: OptionType.Toggle,
        def: 1
    }, {
        name: "Background Brightness",
        scope: Scope,
        standard: 0,
        type: OptionType.Slider,
        def: 0.5,
        min: 0.2,
        max: 0.8,
        step: 0.05,
    }, {
        name: "Use Sonolus Pause Button",
        scope: Scope,
        standard: 0,
        type: OptionType.Toggle,
        def: 0
    }, {
        name: "Use Sonolus Combo",
        scope: Scope,
        standard: 0,
        type: OptionType.Toggle,
        def: 0
    }, {
        name: "Use Sonolus Primary Metric",
        scope: Scope,
        standard: 0,
        type: OptionType.Toggle,
        def: 0
    }, {
        name: "Display Judgeline Index & Direction",
        scope: Scope,
        standard: 0,
        type: OptionType.Toggle,
        def: 0
    }, {
        name: "Auto SFX",
        scope: Scope,
        standard: 0,
        type: OptionType.Toggle,
        def: 0
    }
});