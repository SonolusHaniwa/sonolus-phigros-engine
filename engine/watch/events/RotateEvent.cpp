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
	var bpm;
	var judgelineId = var(EntitySharedMemoryId, 0);

	SonolusApi spawnTime() { return startTime * timeMagic / bpm; }
	SonolusApi despawnTime() { return endTime * timeMagic / bpm; }
	
	int preprocessOrder = 114514;
	SonolusApi preprocess() {
		bpm = EntityDataArray[judgelineId].generic[5];
	}

	SonolusApi updateSequential() {
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
