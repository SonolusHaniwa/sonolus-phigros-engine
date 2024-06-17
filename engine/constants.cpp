double PI  = acos(-1);
double lockAspectRatio = 1;
double extraWidth = 1;
double targetAspectRatio = 1920.0 / 1080.0;
double judgelineHeight = 0.01;
double noteWidth = 0.2;
double minSFXDistance = 0.02;
double effectDurationTime = 0.4;
double noteRatio = 989.0 / 100.0;
double dragRatio = 989.0 / 60.0;
double flickRatio = 989.0 / 200.0;
double hlNoteRatio = 1089.0 / 200.0;
double hlDragRatio = 1089.0 / 160.0;
double hlFlickRatio = 1089.0 / 300.0;

#if play || watch
class stage {
	public:

	Variable<EntityMemoryId> w = If(lockAspectRatio == 0 || screen.aspectRatio < targetAspectRatio,
        screen.w * extraWidth,
        screen.h * targetAspectRatio);
    Variable<EntityMemoryId> h = If(lockAspectRatio == 0 || screen.aspectRatio > targetAspectRatio,
        screen.h,
        screen.w / targetAspectRatio);
    Variable<EntityMemoryId> l = -1 * w / 2;
    Variable<EntityMemoryId> r = w / 2;
    Variable<EntityMemoryId> t = h / 2;
    Variable<EntityMemoryId> b = -1 * h / 2;
}stage;
#endif

let t = If(
	lockAspectRatio && screen.aspectRatio < targetAspectRatio,
	screen.w / targetAspectRatio * 0.5,
	screen.t
);
let b = t - stage.h;

class judgment {
	public:

    let perfectPlus = 0.025;
    let perfect = 0.04;
    let great = 0.07;
    let good = 0.1;
    let bad = 0.125;
}judgment;

class score {
	public:

	let perfect = 1;
	let great = 0.8;
	let good = 0.5;
}score;
