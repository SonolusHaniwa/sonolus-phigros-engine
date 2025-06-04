string scope = "phigros";

class Options {
    public:
    
    var StrictMode = ToggleOption(
        Text.JudgmentStrict,
        "",
        true,
        false,
        "",
        false
    );
    var Mirror = ToggleOption(
        Text.Mirror,
        "",
        false,
        false,
        "",
        false
    );
    var LevelSpeed = SliderOption(
        Text.Speed,
        "",
        true,
        false,
        "",
        1.0,
        0.5,
        2.0,
        0.05,
        Text.PercentageUnit
    );
    var NoteSize = SliderOption(
        Text.NoteSize,
        "",
        false,
        false,
        scope,
        1.0,
        0.8,
        1.2,
        0.005
    );
    var HasSFX = ToggleOption(
        Text.Effect,
        "",
        false,
        false,
        scope,
        true
    );
    var HasSimul = ToggleOption(
        "Highlight Simultaneous Notes",
        "",
        false,
        false,
        scope,
        true
    );
    var HasIndicator = ToggleOption(
        "FC/AP Indicator",
        "",
        false,
        false,
        scope,
        true
    );
    var Background = SliderOption(
        "Background Brightness",
        "",
        false,
        false,
        scope,
        0.5,
        0.2,
        0.8,
        0.05
    );
    var SystemPause = ToggleOption(
        "Use Sonolus Pause Button",
        "",
        false,
        false,
        scope,
        false
    );
    var SystemCombo = ToggleOption(
        "Use Sonolus Combo",
        "",
        false,
        false,
        scope,
        false
    );
    var SystemPrimaryMetric = ToggleOption(
        "Use Sonolus Primary Metric",
        "",
        false,
        false,
        scope,
        false
    );
    var JudgelineId = ToggleOption(
        "Display Judgeline Index & Direction",
        "",
        false,
        false,
        scope,
        false
    );
    var AutoSFX = ToggleOption(
        Text.EffectAuto,
        "",
        false,
        false,
        scope,
        false
    );
}Options;