class SpeedEvent: public Archetype {
	public:

	string name = "Phigros Judgeline Speed Event";
	defineImport(startTime);
	defineImport(endTime);
	defineImport(start);
	defineImport(end);
	defineImport(next);
	var bpm;
	var judgelineId = var(EntitySharedMemoryId, 0);
	var baseFloorPosition = var(EntitySharedMemoryId, 1);

	SonolusApi spawnTime() { return startTime * timeMagic / bpm; }
	SonolusApi despawnTime() { return endTime * timeMagic / bpm; }
	
	int preprocessOrder = 114;
	SonolusApi preprocess() {
		bpm = EntityDataArray[judgelineId].generic[5];
		baseFloorPosition = EntitySharedMemoryArray[judgelineId].generic[5];
		startTime = Max(startTime, 0);
		EntitySharedMemoryArray[judgelineId].generic[5] = baseFloorPosition + (endTime - startTime) * timeMagic / bpm * (start + end) / 2;
	}

	SonolusApi updateSequential() {
		var curr = (times.now - startTime * timeMagic / bpm) * (end - start) / ((endTime - startTime) * timeMagic / bpm) + start;
		EntitySharedMemoryArray[judgelineId].generic[0] = curr;
		EntitySharedMemoryArray[judgelineId].generic[5] = baseFloorPosition + (times.now - startTime * timeMagic / bpm) * (start + curr) / 2;
	}
};
