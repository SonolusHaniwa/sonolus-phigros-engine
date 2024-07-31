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
		EntitySharedMemoryArray[judgelineId].set(4,
			getEaseValue(easing, startTime * timeMagic / bpm, endTime * timeMagic / bpm, start, end, times.now)
		);
		return VOID;
	}
};
