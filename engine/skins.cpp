using namespace std;

class Sprites {
	public:

	int NormalJudgeline = 1000;
	int AllPerfectJudgeline = 1001;
	int FullComboJudgeline = 1002;
	int NormalNote = 1;
	int HLNote = 2;
	int FallbackNormalNote = 2001;
	int NormalDrag = 3;
	int HLDrag = 4;
	int FallbackNormalDrag = 2003;
	int NormalHoldHead = 5;
	int HLHoldHead = 6;
	int NormalFlick = 7;
	int HLFlick = 8;
	int FallbackNormalFlick = 2005;
	int NormalHoldBody = 9;
	int HLHoldBody = 10;
	int PauseButton = 11;
	int Blocker = 12;
	int ComboText = 13;
	int Combo0 = 14;
	int Combo1 = 15;
	int Combo2 = 16;
	int Combo3 = 17;
	int Combo4 = 18;
	int Combo5 = 19;
	int Combo6 = 20;
	int Combo7 = 21;
	int Combo8 = 22;
	int Combo9 = 23;
	int tip = 24;
}Sprites;

auto skins = defineSkins<class Sprites>({
	{SkinSpriteName.JudgmentLine, Sprites.NormalJudgeline},
	{"Judgeline AllPerfect", Sprites.AllPerfectJudgeline},
	{"Judgeline FullCombo", Sprites.FullComboJudgeline},
	{"Phigros Normal Note", Sprites.NormalNote},
	{"Phigros Normal Drag", Sprites.NormalDrag},
	{"Phigros Normal Flick", Sprites.NormalFlick},
	{SkinSpriteName.NoteHeadCyan, Sprites.FallbackNormalNote},
	{SkinSpriteName.NoteHeadYellow, Sprites.FallbackNormalDrag},
	{SkinSpriteName.NoteHeadBlue, Sprites.NormalHoldHead},
	{SkinSpriteName.NoteHeadRed, Sprites.FallbackNormalFlick},
	{SkinSpriteName.NoteTailCyan, Sprites.HLNote},
	{SkinSpriteName.NoteTailYellow, Sprites.HLDrag},
	{SkinSpriteName.NoteTailBlue, Sprites.HLHoldHead},
	{SkinSpriteName.NoteTailRed, Sprites.HLFlick},
	{SkinSpriteName.NoteConnectionBlue, Sprites.NormalHoldBody},
	{SkinSpriteName.NoteConnectionYellow, Sprites.HLHoldBody},
	{"Pause", Sprites.PauseButton},
	{"Blocker", Sprites.Blocker},
	{"combo", Sprites.ComboText},
	{"0", Sprites.Combo0},
	{"1", Sprites.Combo1},
	{"2", Sprites.Combo2},
	{"3", Sprites.Combo3},
	{"4", Sprites.Combo4},
	{"5", Sprites.Combo5},
	{"6", Sprites.Combo6},
	{"7", Sprites.Combo7},
	{"8", Sprites.Combo8},
	{"9", Sprites.Combo9},
	{"tip", Sprites.tip}
});
