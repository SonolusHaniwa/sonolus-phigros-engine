class MoveXEvent: public Archetype {
	public:

	static constexpr const char* name = "Phigros Judgeline Move X Event";
	defineImports(startTime);
	defineImports(endTime);
	defineImports(start);
	defineImports(end);
	defineImports(easing);
	defineImports(next);
	Variable<EntityMemoryId> bpm;
	Variable<EntitySharedMemoryId> judgelineId;

	SonolusApi spawnTime() { return startTime * timeMagic / bpm; }
	SonolusApi despawnTime() { return endTime * timeMagic / bpm; }
	
	int preprocessOrder = 114514;
	SonolusApi preprocess() {
		FUNCBEGIN
		startTime = startTime / levelSpeed;
		endTime = endTime / levelSpeed;
		bpm = EntityDataArray[judgelineId].get(5);
		return VOID;
	}

	SonolusApi updateSequential() {
		FUNCBEGIN
		var x = getEaseValue(easing, startTime * timeMagic / bpm, endTime * timeMagic / bpm, start, end, times.now);
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
	defineImports(next);
	Variable<EntityMemoryId> bpm;
	Variable<EntitySharedMemoryId> judgelineId;

	SonolusApi spawnTime() { return startTime * timeMagic / bpm; }
	SonolusApi despawnTime() { return endTime * timeMagic / bpm; }
	
	int preprocessOrder = 114514;
	SonolusApi preprocess() {
		FUNCBEGIN
		startTime = startTime / levelSpeed;
		endTime = endTime / levelSpeed;
		bpm = EntityDataArray[judgelineId].get(5);
		return VOID;
	}

	SonolusApi updateSequential() {
		FUNCBEGIN
		var y = getEaseValue(easing, startTime * timeMagic / bpm, endTime * timeMagic / bpm, start, end, times.now);
		y = y * stage.h + stage.b;
		EntitySharedMemoryArray[judgelineId].set(2, y);
		return VOID;
	}
};