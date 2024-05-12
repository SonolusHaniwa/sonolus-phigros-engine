using namespace std;

class Effects {
    public:

	int perfect = 0;
	int great = 1;
}Effects;

auto particles = defineParticles<class Effects>({
	{ParticleEffectName.NoteLinearHoldYellow, Effects.perfect},
	{ParticleEffectName.NoteLinearHoldBlue, Effects.great},
});
