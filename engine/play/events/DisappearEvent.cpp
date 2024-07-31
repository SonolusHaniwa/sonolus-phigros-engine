class DisappearEvent: public Archetype {
	public:

	static constexpr const char* name = "Phigros Judgeline Disappear Event";
	defineImports(startTime);
	defineImports(endTime);
	defineImports(start);
	defineImports(end);
	defineImports(easing);
	defineImports(next);
	Variable<EntityMemoryId> bpm;
	Variable<EntitySharedMemoryId> judgelineId;

	SonolusApi spawnOrder() { return startTime * timeMagic / bpm; }
	SonolusApi shouldSpawn() { return times.now >= startTime * timeMagic / bpm; }

	int preprocessOrder = 114;
	SonolusApi preprocess() {
		FUNCBEGIN
		bpm = EntityDataArray[judgelineId].get(5);
		return VOID;
	}

	SonolusApi updateSequential() {
		FUNCBEGIN
		EntitySharedMemoryArray[judgelineId].set(4,
			getEaseValue(easing, startTime * timeMagic / bpm, endTime * timeMagic / bpm, start, end, times.now)
		);
		IF (times.now > endTime * timeMagic / bpm) EntityDespawn.set(0, 1); FI
		return VOID;
	}
};
