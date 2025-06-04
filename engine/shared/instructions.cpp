class iTexts {
    public:

    InstructionText Tap = defineInstructionText(Text.Tap);
    InstructionText Hold = defineInstructionText(Text.Hold);
    InstructionText HoldAndFollow = defineInstructionText(Text.HoldFollow);
    InstructionText Release = defineInstructionText(Text.Release);
    InstructionText Flick = defineInstructionText(Text.Flick);
}iTexts;

class iIcons {
    public:

    InstructionIcon Hand = defineInstructionIcon(InstructionIconName.Hand);
}iIcons;