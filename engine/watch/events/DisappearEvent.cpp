class DisappearEvent: public Archetype {
	public:

	static constexpr const char* name = "Phigros Judgeline Disappear Event";
	defineImports(startTime);
	defineImports(endTime);
	defineImports(start);
	defineImports(end);
	defineImports(easing);
	defineImports(easingLeft);
	defineImports(easingRight);
	defineImports(bezier);
	defineImports(bezierP1);
	defineImports(bezierP2);
	defineImports(bezierP3);
	defineImports(bezierP4);
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
			If(
				bezier,
				getBezierValue(
					bezierP1, bezierP2, bezierP3, bezierP4,
					startTime * timeMagic / bpm, endTime * timeMagic / bpm, 
					start, end, times.now, easingLeft, easingRight
				),
				getEaseValue(
					easing, 
					startTime * timeMagic / bpm, endTime * timeMagic / bpm, 
					start, end, times.now, easingLeft, easingRight
				)
			) + EntitySharedMemoryArray[judgelineId].get(4)
		);
		return VOID;
	}
};
