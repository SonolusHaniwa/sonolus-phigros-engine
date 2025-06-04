class RotateEvent: public Archetype {
	public:

	string name = "Phigros Judgeline Rotate Event";
	defineImport(startTime);
	defineImport(endTime);
	defineImport(start);
	defineImport(end);
	defineImport(easing);
	defineImport(easingLeft);
	defineImport(easingRight);
	defineImport(bezier);
	defineImport(bezierP1);
	defineImport(bezierP2);
	defineImport(bezierP3);
	defineImport(bezierP4);
	defineImport(next);
	Variable bpm;
	Variable judgelineId = Variable(EntitySharedMemoryId, 0);

	SonolusApi spawnOrder() { return startTime * timeMagic / bpm; }
	SonolusApi shouldSpawn() { return times.now >= startTime * timeMagic / bpm; }

	int preprocessOrder = 114;
	SonolusApi preprocess() {
		bpm = EntityDataArray[judgelineId].generic[5];
	}

	SonolusApi updateSequential() {
		if (times.now >= endTime * timeMagic / bpm) {
			despawn.despawn = true;
			return;
		}
		var rotate = If(
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
		) + EntitySharedMemoryArray[judgelineId].generic[3];
		EntitySharedMemoryArray[judgelineId].generic[3] = rotate;
	}
};
