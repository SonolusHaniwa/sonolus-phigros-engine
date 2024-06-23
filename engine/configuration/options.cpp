using namespace std;

class Options {
    public:

    int NoteSize = 0;
    int HasSFX = 1;
    int HasSimul = 2;
    int HasIndicator = 3;
    int Background = 4;
}Options;

auto options = defineOptions<class Options>({
    {
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
    }
});