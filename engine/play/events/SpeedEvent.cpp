class SpeedEvent: public Archetype {
	public:

	static constexpr const char* name = "Phigros Judgeline Speed Event";
	defineImports(startTime);
	defineImports(endTime);
	defineImports(start);
	defineImports(end);
	defineImports(next);
	Variable<EntityMemoryId> bpm;
	Variable<EntitySharedMemoryId> judgelineId;
	Variable<EntitySharedMemoryId> baseFloorPosition;

	SonolusApi spawnOrder() { return startTime * timeMagic / bpm; }
	SonolusApi shouldSpawn() { return times.now >= startTime * timeMagic / bpm; }

	int preprocessOrder = 114;
	SonolusApi preprocess() {
		FUNCBEGIN
		bpm = EntityDataArray[judgelineId].get(5);
		baseFloorPosition = EntitySharedMemoryArray[judgelineId].get(5);
		EntitySharedMemoryArray[judgelineId].set(5, baseFloorPosition + (endTime - startTime) * timeMagic / bpm * (start + end) / 2);
		return VOID;
	}

	SonolusApi updateSequential() {
		FUNCBEGIN
		var curr = (times.now - startTime * timeMagic / bpm) * (end - start) / ((endTime - startTime) * timeMagic / bpm) + start;
		EntitySharedMemoryArray[judgelineId].set(0, curr);
		EntitySharedMemoryArray[judgelineId].set(5, baseFloorPosition + (times.now - startTime * timeMagic / bpm) * (start + curr) / 2);
		IF (times.now > endTime * timeMagic / bpm) EntityDespawn.set(0, 1); FI
		return VOID;
	}
};
