class SpeedEvent: public Archetype {
	public:

	string name = "Phigros Judgeline Speed Event";
	defineImport(startTime);
	defineImport(endTime);
	defineImport(start);
	defineImport(end);
	defineImport(next);
	var bpm;
	Variable judgelineId = Variable(EntitySharedMemoryId, 0);
	Variable baseFloorPosition = Variable(EntitySharedMemoryId, 1);

	SonolusApi spawnOrder() { return startTime * timeMagic / bpm; }
	SonolusApi shouldSpawn() { return times.now >= startTime * timeMagic / bpm; }

	int preprocessOrder = 114;
	SonolusApi preprocess() {
		bpm = EntityDataArray[judgelineId].generic[5];
		baseFloorPosition = EntitySharedMemoryArray[judgelineId].generic[5];
		startTime = Max(startTime, 0);
		EntitySharedMemoryArray[judgelineId].generic[5] = baseFloorPosition + (endTime - startTime) * timeMagic / bpm * (start + end) / 2;
	}

	SonolusApi updateSequential() {
		if (times.now >= endTime * timeMagic / bpm) {
			despawn.despawn = true;
			return;
		}
		var curr = (times.now - startTime * timeMagic / bpm) * (end - start) / ((endTime - startTime) * timeMagic / bpm) + start;
		EntitySharedMemoryArray[judgelineId].generic[0] = curr;
		EntitySharedMemoryArray[judgelineId].generic[5] = baseFloorPosition + (times.now - startTime * timeMagic / bpm) * (start + curr) / 2;
	}
};
