class Judgeline: public Archetype {
	public:

	static constexpr const char* name = "Phigros Judgeline";
	defineImports(speedEvent);
	defineImports(moveXEvent);
	defineImports(moveYEvent);
	defineImports(rotateEvent);
	defineImports(disappearEvent);
	defineImports(bpm);
	Variable<EntitySharedMemoryId> speed;
	Variable<EntitySharedMemoryId> x;
	Variable<EntitySharedMemoryId> y;
	Variable<EntitySharedMemoryId> rotate;
	Variable<EntitySharedMemoryId> disappear;
	Variable<EntitySharedMemoryId> floorPosition;
	Variable<EntityMemoryId> speedEventId;
	Variable<EntityMemoryId> moveXEventId;
	Variable<EntityMemoryId> moveYEventId;
	Variable<EntityMemoryId> rotateEventId;
	Variable<EntityMemoryId> disappearEventId;
	Variable<EntityMemoryId> baseFloorPosition;

	SonolusApi spawnOrder() { return 1; }
	SonolusApi shouldSpawn() { return 1; }

	SonolusApi preprocess() {
		FUNCBEGIN
		speedEventId = speedEvent.get();
		moveXEventId = moveXEvent.get();
		moveYEventId = moveYEvent.get();
		rotateEventId = rotateEvent.get();
		disappearEventId = disappearEvent.get();
		speed = 1;
		x = 0, y = 0;
		rotate = 0;
		disappear = 0;
		floorPosition = 0;
		baseFloorPosition = 0;
		return VOID;
	}

	SonolusApi jump(Variable<EntityMemoryId> &id, int nextId, bool isSpeed = false) {
		FUNCBEGIN
		WHILE (id != 0) {
			auto obj = EntityDataArray[id];
			IF (obj.get(1) * timeMagic / bpm > times.now) BREAK; FI
			IF (isSpeed) {
				baseFloorPosition = baseFloorPosition + (obj.get(1) - obj.get(0)) * timeMagic / bpm * obj.get(2); 
			} FI
			id = obj.get(nextId);
		} DONE
		return VOID;
	}
	SonolusApi updateSequential() {
		FUNCBEGIN
		jump(speedEventId, 3, true);
		jump(moveXEventId, 5);
		jump(moveYEventId, 5);
		jump(rotateEventId, 5);
		jump(disappearEventId, 5);
		Debuglog(disappearEventId);
		IF (speedEventId != 0) {
			speed = EntityDataArray[speedEventId].get(2); 
			floorPosition = baseFloorPosition + (times.now - EntityDataArray[speedEventId].get(0) * timeMagic / bpm) * speed;
		} FI
		IF (moveXEventId != 0) {
			auto obj = EntityDataArray[moveXEventId];
			x = getEaseValue(obj.get(4), obj.get(0) * timeMagic / bpm, obj.get(1) * timeMagic / bpm, obj.get(2), obj.get(3), times.now);
			x = x * stage.w + stage.l;
		} FI
		IF (moveYEventId != 0) {
			auto obj = EntityDataArray[moveYEventId];
			y = getEaseValue(obj.get(4), obj.get(0) * timeMagic / bpm, obj.get(1) * timeMagic / bpm, obj.get(2), obj.get(3), times.now);
			y = y * stage.h + stage.b;
		} FI
		IF (rotateEventId != 0) {
			auto obj = EntityDataArray[rotateEventId];
			rotate = getEaseValue(obj.get(4), obj.get(0) * timeMagic / bpm, obj.get(1) * timeMagic / bpm, obj.get(2), obj.get(3), times.now) % 360;
			IF (rotate < 0) rotate = rotate + 360; FI
			rotate = rotate / 180 * PI;
		} FI
		IF (disappearEventId != 0) {
			auto obj = EntityDataArray[disappearEventId];
			disappear = getEaseValue(obj.get(4), obj.get(0) * timeMagic / bpm, obj.get(1) * timeMagic / bpm, obj.get(2), obj.get(3), times.now);
		} FI
		return VOID;
	}

	SonolusApi updateParallel() {
		FUNCBEGIN
		var x1 = 0, y1 = 0, x2 = 0, y2 = 0;
		IF (rotate == PI / 2 || rotate == PI * 3 / 2) x1 = x.get(), y1 = stage.b.get(), x2 = x.get(), y2 = stage.t.get();
		ELSE {
			let k = Tan(rotate);
			x1 = stage.l.get(); y1 = k * (stage.l - x) + y;
			x2 = stage.r.get(); y2 = k * (stage.r - x) + y;
			IF (y1 < stage.b) x1 = 1 / k * (stage.b - y) + x, y1 = stage.b.get(); FI
			IF (y1 > stage.h) x1 = 1 / k * (stage.h - y) + x, y1 = stage.h.get(); FI
			IF (y2 < stage.b) x2 = 1 / k * (stage.b - y) + x, y2 = stage.b.get(); FI
			IF (y2 > stage.h) x2 = 1 / k * (stage.h - y) + x, y2 = stage.h.get(); FI
		} FI
		let vectorLength = judgelineHeight / 2;
		let vectorX = vectorLength * Cos(rotate + PI / 2), vectorY = vectorLength * Sin(rotate + PI / 2);
		let x3 = x1 - vectorX, y3 = y1 - vectorY;
		let x4 = x1 + vectorX, y4 = y1 + vectorY;
		let x5 = x2 + vectorX, y5 = y2 + vectorY;
		let x6 = x2 - vectorX, y6 = y2 - vectorY;
		// IF (EntityInfo.get(0) == 0) 
		// Debuglog(vectorX); Debuglog(vectorY);
		// Debuglog(x3); Debuglog(y3); 
		// Debuglog(x4); Debuglog(y4); 
		// Debuglog(x5); Debuglog(y5); 
		// Debuglog(x6); Debuglog(y6);
		// FI
		Draw(Switch(judgeStatus, {
			{2, Sprites.AllPerfectJudgeline},
			{1, Sprites.FullComboJudgeline},
			{0, Sprites.NormalJudgeline}
		}), x3, y3, x4, y4, x5, y5, x6, y6, 10000, disappear);
		return VOID;
	}
};
