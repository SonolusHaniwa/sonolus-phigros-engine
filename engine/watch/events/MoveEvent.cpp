class MoveXEvent: public Archetype {
	public:

	static constexpr const char* name = "Phigros Judgeline Move X Event";
	defineImports(startTime);
	defineImports(endTime);
	defineImports(start);
	defineImports(end);
	defineImports(easing);
	defineImports(easingLeft);
	defineImports(easingRight);
	defineImports(bezier);
	defineImports(bezierP1);
	defineImports(bezierP2);
	defineImports(bezierP3);
	defineImports(bezierP4);
	defineImports(next);
	Variable<EntityMemoryId> bpm;
	Variable<EntitySharedMemoryId> judgelineId;

	SonolusApi spawnTime() { return startTime * timeMagic / bpm; }
	SonolusApi despawnTime() { return endTime * timeMagic / bpm; }
	
	int preprocessOrder = 114514;
	SonolusApi preprocess() {
		FUNCBEGIN
		bpm = EntityDataArray[judgelineId].get(5);
		return VOID;
	}

	SonolusApi updateSequential() {
		FUNCBEGIN
		var x = If(
			bezier,
			getBezierValue(
				bezierP1, bezierP2, bezierP3, bezierP4,
				startTime * timeMagic / bpm, endTime * timeMagic / bpm, 
				start, end, times.now, easingLeft, easingRight
			), getEaseValue(
				easing, 
				startTime * timeMagic / bpm, endTime * timeMagic / bpm, 
				start, end, times.now, easingLeft, easingRight
			)
		);
		IF (mirror) x = 1.0 - x; FI
		x = x * stage.w + stage.l;
		EntitySharedMemoryArray[judgelineId].set(1, x);
		return VOID;
	}
};

class MoveYEvent: public Archetype {
	public:

	static constexpr const char* name = "Phigros Judgeline Move Y Event";
	defineImports(startTime);
	defineImports(endTime);
	defineImports(start);
	defineImports(end);
	defineImports(easing);
	defineImports(easingLeft);
	defineImports(easingRight);
	defineImports(bezier);
	defineImports(bezierP1);
	defineImports(bezierP2);
	defineImports(bezierP3);
	defineImports(bezierP4);
	defineImports(next);
	Variable<EntityMemoryId> bpm;
	Variable<EntitySharedMemoryId> judgelineId;

	SonolusApi spawnTime() { return startTime * timeMagic / bpm; }
	SonolusApi despawnTime() { return endTime * timeMagic / bpm; }
	
	int preprocessOrder = 114514;
	SonolusApi preprocess() {
		FUNCBEGIN
		bpm = EntityDataArray[judgelineId].get(5);
		return VOID;
	}

	SonolusApi updateSequential() {
		FUNCBEGIN
		var y = If(
			bezier,
			getBezierValue(
				bezierP1, bezierP2, bezierP3, bezierP4,
				startTime * timeMagic / bpm, endTime * timeMagic / bpm, 
				start, end, times.now, easingLeft, easingRight
			), getEaseValue(
				easing, 
				startTime * timeMagic / bpm, endTime * timeMagic / bpm, 
				start, end, times.now, easingLeft, easingRight
			)
		);
		y = y * stage.h + stage.b;
		EntitySharedMemoryArray[judgelineId].set(2, y);
		return VOID;
	}
};