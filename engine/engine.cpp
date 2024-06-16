#include"skins.cpp"
#include"effects.cpp"
#include"particles.cpp"

#if play
using namespace playData;
#endif

string Scope = "phigros";
#include"configuration/options.cpp"
#include"configuration/ui.cpp"
#include"constants.cpp"
#include"easing.cpp"

#if play
#include"play/Initialization.cpp"
#include"play/Judgeline.cpp"
#include"play/SpeedEvent.cpp"
#include"play/MoveEvent.cpp"
#include"play/RotateEvent.cpp"
#include"play/DisappearEvent.cpp"
#include"play/notes/NormalNote.cpp"
#include"play/notes/DragNote.cpp"
#include"play/notes/HoldNote.cpp"
#include"play/notes/FlickNote.cpp"
#endif
