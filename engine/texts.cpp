class Texts {
    public:

    int Tap = 0;
    int Drag = 1;
    int Hold = 2;
    int HoldRelease = 3;
    int Flick = 4;
}Texts;

auto texts = defineInstructionTexts<class Texts>({
    { InstructionText.Tap, Texts.Tap },
    { InstructionText.Hold, Texts.Drag },
    { InstructionText.HoldAndFollow, Texts.Hold },
    { InstructionText.Release, Texts.HoldRelease },
    { InstructionText.Flick, Texts.Flick }
});