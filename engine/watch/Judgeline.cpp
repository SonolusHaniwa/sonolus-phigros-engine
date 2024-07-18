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
	Variable<EntityMemoryId> allocatedId;

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
		allocatedId = allocateJudgelineId.get();
		allocateJudgelineId = allocateJudgelineId + 1;
        var id = speedEvent.get(); jump(id, 3);
        id = moveXEvent.get(); jump(id, 5);
        id = moveYEvent.get(); jump(id, 5);
        id = rotateEvent.get(); jump(id, 5);
        id = disappearEvent.get(); jump(id, 5);
		return VOID;
	}

	SonolusApi updateParallel() {
		FUNCBEGIN
		IF (times.now < 0) Return(0); FI
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

		IF (hasJudgelineId) {
			var tmpId = allocatedId.get();
			var width = 0;
            var h = judgelineIdTextHeight;
			var numberDistance = h * 0.15;
            var w = -1 * numberDistance;
			WHILE (tmpId) {
                var digit = tmpId % 10;
                w = w + h * Switch(digit, {
                    {0, combo0Ratio}, {1, combo1Ratio}, {2, combo2Ratio},
                    {3, combo3Ratio}, {4, combo4Ratio}, {5, combo5Ratio},
                    {6, combo6Ratio}, {7, combo7Ratio}, {8, combo8Ratio}, {9, combo9Ratio}
                }) + numberDistance;
				tmpId = Floor(tmpId / 10);
            } DONE
			var cx = x + Cos(rotate - PI / 2) * (numberDistance * 3 + h / 2), cy = y + Sin(rotate - PI / 2) * (numberDistance * 3 + h / 2);
			var r = w / 2;
			tmpId = allocatedId.get();
			WHILE (tmpId) {
                var digit = tmpId % 10;
				var R = r, L = R - h * Switch(digit, {
					{0, combo0Ratio}, {1, combo1Ratio}, {2, combo2Ratio},
					{3, combo3Ratio}, {4, combo4Ratio}, {5, combo5Ratio},
					{6, combo6Ratio}, {7, combo7Ratio}, {8, combo8Ratio}, {9, combo9Ratio}
				});
				r = L - numberDistance;
				var vector2Length = L, vector3Length = R, vector4Length = h / 2;
				var x7 = cx + Cos(rotate) * vector2Length, y7 = cy + Sin(rotate) * vector2Length;
				var x8 = cx + Cos(rotate) * vector3Length, y8 = cy + Sin(rotate) * vector3Length;
				var x9 = x7 - Cos(rotate + PI / 2) * vector4Length, y9 = y7 - Sin(rotate + PI / 2) * vector4Length;
				var x10 = x7 + Cos(rotate + PI / 2) * vector4Length, y10 = y7 + Sin(rotate + PI / 2) * vector4Length;
				var x11 = x8 + Cos(rotate + PI / 2) * vector4Length, y11 = y8 + Sin(rotate + PI / 2) * vector4Length;
				var x12 = x8 - Cos(rotate + PI / 2) * vector4Length, y12 = y8 - Sin(rotate + PI / 2) * vector4Length;
				Draw(Switch(digit, {
                    {0, Sprites.Combo0}, {1, Sprites.Combo1}, {2, Sprites.Combo2},
                    {3, Sprites.Combo3}, {4, Sprites.Combo4}, {5, Sprites.Combo5},
                    {6, Sprites.Combo6}, {7, Sprites.Combo7}, {8, Sprites.Combo8}, {9, Sprites.Combo9}
                }), x9, y9, x10, y10, x11, y11, x12, y12, 19999 - allocatedId, 1);
				tmpId = Floor(tmpId / 10);
			} DONE
		} FI
		return VOID;
	}
};
