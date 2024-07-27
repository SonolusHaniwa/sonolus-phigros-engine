#include"skins.cpp"
#include"effects.cpp"
#include"particles.cpp"
#include"buckets.cpp"
#include"icons.cpp"
#include"texts.cpp"

#if play
using namespace playData;
#elif tutorial
using namespace tutorialData;
#elif preview
using namespace previewData;
#elif watch
using namespace watchData;
#endif

string Scope = "phigros";
#include"configuration/options.cpp"
#include"configuration/ui.cpp"
#include"constants.cpp"
#include"easing.cpp"

#if play
#include"play/Initialization.cpp"
#include"play/StageController.cpp"
#include"play/InputManager.cpp"
#include"play/FlickInputManager.cpp"
#include"play/Judgeline.cpp"
#include"play/events/SpeedEvent.cpp"
#include"play/events/MoveEvent.cpp"
#include"play/events/RotateEvent.cpp"
#include"play/events/DisappearEvent.cpp"
#include"play/notes/NormalNote.cpp"
#include"play/notes/DragNote.cpp"
#include"play/notes/HoldNote.cpp"
#include"play/notes/FlickNote.cpp"
#elif tutorial
#include"tutorial/Hands.cpp"
#include"tutorial/Initialization.cpp"
#include"tutorial/StageController.cpp"
#include"tutorial/Judgeline.cpp"
#include"tutorial/notes/NormalNote.cpp"
#include"tutorial/notes/DragNote.cpp"
#include"tutorial/notes/HoldNote.cpp"
#include"tutorial/notes/FlickNote.cpp"
#include"tutorial/Tutorial.cpp"
#elif preview
#include"preview/Initialization.cpp"
#elif watch
#include"watch/updateSpawn.cpp"
#include"watch/Initialization.cpp"
#include"watch/StageController.cpp"
#include"watch/Judgeline.cpp"
#include"watch/events/SpeedEvent.cpp"
#include"watch/events/MoveEvent.cpp"
#include"watch/events/RotateEvent.cpp"
#include"watch/events/DisappearEvent.cpp"
#include"watch/UpdateJudgment.cpp"
#include"watch/notes/NormalNote.cpp"
#include"watch/notes/DragNote.cpp"
#include"watch/notes/HoldNote.cpp"
#include"watch/notes/FlickNote.cpp"
#include"watch/Preprocess.cpp"
#endif
