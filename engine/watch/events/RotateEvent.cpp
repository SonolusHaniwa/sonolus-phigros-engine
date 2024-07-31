class RotateEvent: public Archetype {
	public:

	static constexpr const char* name = "Phigros Judgeline Rotate Event";
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
		bpm = EntityDataArray[judgelineId].get(5);
		return VOID;
	}

	SonolusApi updateSequential() {
		FUNCBEGIN
		var rotate = getEaseValue(easing, startTime * timeMagic / bpm, endTime * timeMagic / bpm, start, end, times.now);
		IF (rotate < 0) rotate = rotate + 360; FI
		rotate = rotate / 180 * PI;
		EntitySharedMemoryArray[judgelineId].set(3, rotate);
		return VOID;
	}
};
