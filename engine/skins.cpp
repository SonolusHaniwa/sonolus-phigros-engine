using namespace std;

class Sprites {
	public:

	int Judgeline = 0;
	int NormalNote = 1;
	int HLNote = 2;
	int NormalDrag = 3;
	int HLDrag = 4;
	int NormalHoldHead = 5;
	int HLHoldHead = 6;
	int NormalFlick = 7;
	int HLFlick = 8;
	int NormalHoldBody = 9;
	int HLHoldBody = 10;
}Sprites;

auto skins = defineSkins<class Sprites>({
	{"Judgeline", Sprites.Judgeline},
	{"Normal Note", Sprites.NormalNote},
	{"Normal Drag", Sprites.NormalDrag},
	{"Normal Hold Head", Sprites.NormalHoldHead},
	{"Normal Flick", Sprites.NormalFlick},
	{"Critical Note", Sprites.HLNote},
	{"Critical Drag", Sprites.HLDrag},
	{"Critical Hold Head", Sprites.HLHoldHead},
	{"Critical Flick", Sprites.HLFlick},
	{"Normal Hold", Sprites.NormalHoldBody},
	{"Critical Hold", Sprites.HLHoldBody}
});
