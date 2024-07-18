class SpeedEvent: public Archetype {
	public:

	static constexpr const char* name = "Phigros Judgeline Speed Event";
	defineImports(startTime);
	defineImports(endTime);
	defineImports(value);
	defineImports(next);
	Variable<EntityMemoryId> bpm;
	Variable<EntityMemoryId> baseFloorPosition;
	Variable<EntitySharedMemoryId> judgelineId;

	SonolusApi spawnTime() { return startTime * timeMagic / bpm; }
	SonolusApi despawnTime() { return endTime * timeMagic / bpm; }
	
	int preprocessOrder = 114514;
	SonolusApi preprocess() {
		FUNCBEGIN
		bpm = EntityDataArray[judgelineId].get(5);
		baseFloorPosition = EntitySharedMemoryArray[judgelineId].get(5);
		EntitySharedMemoryArray[judgelineId].set(5, baseFloorPosition + (endTime * timeMagic / bpm - startTime * timeMagic / bpm) * value);
		return VOID;
	}

	SonolusApi updateSequential() {
		FUNCBEGIN
		EntitySharedMemoryArray[judgelineId].set(0, value);
		EntitySharedMemoryArray[judgelineId].set(5, baseFloorPosition + (times.now - startTime * timeMagic / bpm) * value);
		return VOID;
	}
};
