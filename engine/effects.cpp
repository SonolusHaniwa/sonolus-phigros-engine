using namespace std;

class Clips {
    public:

	int Note = 1;
	int Flick = 4;
	int Hold = 3;
	int Drag = 2;
}Clips;

auto effects = defineEffects<class Clips>({
    {EffectClipName.Perfect, Clips.Note},
    {EffectClipName.PerfectAlternative, Clips.Flick},
    {EffectClipName.Hold, Clips.Hold},
//    {"Sekai Tick", Clips.Drag},
});
