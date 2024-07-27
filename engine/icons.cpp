class Icons {
    public:

    bool hand = 0;
}Icons;

auto icons = defineInstructionIcons<class Icons>({
    { InstructionIconName.Hand, Icons.hand }
});