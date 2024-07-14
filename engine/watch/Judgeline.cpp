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

	SonolusApi spawnTime() { return -999999; }
	SonolusApi despawnTime() { return 999999; }

	SonolusApi jump(var &id, int nextId) {
		FUNCBEGIN
		WHILE (id != 0) {
            EntitySharedMemoryArray[id].set(0, EntityInfo.get(0));
			id = EntityDataArray[id].get(nextId);
		} DONE
		return VOID;
	}

    int preprocessOrder = -114514;
	SonolusApi preprocess() {
		FUNCBEGIN
        var id = speedEvent.get(); jump(id, 3);
        id = moveXEvent.get(); jump(id, 5);
        id = moveYEvent.get(); jump(id, 5);
        id = rotateEvent.get(); jump(id, 5);
        id = disappearEvent.get(); jump(id, 5);
		return VOID;
	}

	SonolusApi updateParallel() {
		FUNCBEGIN
		var x1 = 0, y1 = 0, x2 = 0, y2 = 0;
		IF (rotate == PI / 2 || rotate == PI * 3 / 2) x1 = x.get(), y1 = stage.b.get(), x2 = x.get(), y2 = stage.t.get();
		ELSE {
			let k = Tan(rotate);
            Debuglog(disappear);
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
		// IF (EntityInfo.get(0)) 
        //     Debuglog(vectorX); Debuglog(vectorY);
        //     Debuglog(x3); Debuglog(y3); 
        //     Debuglog(x4); Debuglog(y4); 
        //     Debuglog(x5); Debuglog(y5); 
        //     Debuglog(x6); Debuglog(y6);
		// FI
		Draw(Switch(judgeStatus, {
			{2, Sprites.AllPerfectJudgeline},
			{1, Sprites.FullComboJudgeline},
			{0, Sprites.NormalJudgeline}
		}), x3, y3, x4, y4, x5, y5, x6, y6, 10000, disappear);
		return VOID;
	}
};
