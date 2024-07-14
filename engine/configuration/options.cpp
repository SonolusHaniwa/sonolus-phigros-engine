using namespace std;

class Options {
    public:

    int StrictMode = 0;
    int NoteSize = 1;
    int HasSFX = 2;
    int HasSimul = 3;
    int HasIndicator = 4;
    int Background = 5;
    int SystemPause = 6;
    int SystemCombo = 7;
    int SystemPrimaryMetric = 8;
    int JudgelineId = 9;
}Options;

auto options = defineOptions<class Options>({
    {
        name: NameText.StrictJudgment,
        scope: Scope,
        standard: 1,
        type: OptionType.Toggle,
        def: false
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
    }
});