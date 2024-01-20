using namespace std;

class Sprites {
	public:

	int Judgeline = 0;
	int NormalNote = 1;
	int HLNote = 2;
	int NormalDrag = 3;
	int HLDrag = 4;
	int NormalHold = 5;
	int HLHold = 6;
	int NormalFlick = 7;
	int HLFlick = 8;
}Sprites;

auto skins = defineSkins<class Sprites>({
	{"Judgeline", Sprites.Judgeline},
	{"Normal Note", Sprites.NormalNote},
	{"Normal Drag", Sprites.NormalDrag},
	{"Normal Hold", Sprites.NormalHold},
	{"Normal Flick", Sprites.NormalFlick},
});
