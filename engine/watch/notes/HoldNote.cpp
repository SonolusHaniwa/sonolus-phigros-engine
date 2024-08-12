class HoldNote: public Archetype {
	public:

	static constexpr const char* name = "Phigros Hold Note";
	bool hasInput = true;
	defineImports(time);
	defineImports(positionX);
	defineImports(holdTime);
	defineImports(speed);
	defineImports(floorPosition);
	defineImports(endFloorPosition);
	defineImports(isAbove);
	defineImports(isMulti);
	defineImports(isFake);
	defineImports(judgeline);
	defineImports(bpm);
	defineImports(accuracy);
	defineImports(judgeResult);
	defineImports(judgeTime);
	defineImports(judgeResult2);
	Variable<EntityMemoryId> positionY;
	Variable<EntityMemoryId> effectX1;
	Variable<EntityMemoryId> effectY1;
	Variable<EntityMemoryId> effectX2;
	Variable<EntityMemoryId> effectY2;
	Variable<EntityMemoryId> effectX3;
	Variable<EntityMemoryId> effectY3;
	Variable<EntityMemoryId> effectX4;
	Variable<EntityMemoryId> effectY4;
	Variable<EntityMemoryId> lastSpawn;
	Variable<EntityMemoryId> appearTime;
	Variable<EntitySharedMemoryId> nextNote; // 下一个按键信息
	Variable<EntitySharedMemoryId> judgeTime2; // 判定时间
	Variable<EntitySharedMemoryId> currentCombo; // 当前 Combo 数
	Variable<EntitySharedMemoryId> currentMaxCombo; // 当前最大 Combo 数
	Variable<EntitySharedMemoryId> currentJudgeStatus; // 当前判定结果
	Variable<EntitySharedMemoryId> currentAccScore; // 当前准度得分
	Variable<EntitySharedMemoryId> comboId; // Combo 排行

	BlockPointer<EntitySharedMemoryArrayId> line = EntitySharedMemoryArray[judgeline];

	SonolusApi spawnTime() { return appearTime; }
	SonolusApi despawnTime() { return time + holdTime; }

	int preprocessOrder = 514;
	SonolusApi preprocess() {
		FUNCBEGIN
		time = time * timeMagic / bpm;
		holdTime = holdTime * timeMagic / bpm;
		isMulti = isMulti && hasSimul;
		lastSpawn = -1;
		// var id = EntityDataArray[judgeline].get(0);
		// WHILE (id) {
		// 	var deltaFloorPosition = Abs(floorPosition) - EntitySharedMemoryArray[id].get(1);
		// 	IF (deltaFloorPosition <= 10 / 3 / 1) BREAK; FI
		// 	appearTime = EntityDataArray[id].get(0) * timeMagic / bpm + (deltaFloorPosition - 10 / 3 / 1) / EntityDataArray[id].get(2);
		// 	id = EntityDataArray[id].get(3);
		// } DONE
		// appearTime = Max(0, Min(appearTime, time - 0.5));
		IF (!isFake) {
			notes = notes + 1;
			IF (isReplay) {
				judgeTime2 = time + judgeTime;
				IF ((accuracy != 0 || judgeResult != 0) && hasSFX && !autoSFX) PlayScheduled(Clips.Note, time / levelSpeed + accuracy, minSFXDistance); FI
				IF (autoSFX && hasSFX) PlayScheduled(Clips.Note, time / levelSpeed, minSFXDistance); FI
				Spawn(getArchetypeId(UpdateJudgment), {EntityInfo.get(0)});
				Set(EntityInputId, 0, time + accuracy);
				Set(EntityInputId, 1, Buckets.hold);
				Set(EntityInputId, 2, accuracy * 1000);
			} ELSE {
				judgeTime2 = time + holdTime;
				IF (hasSFX) PlayScheduled(Clips.Note, time / levelSpeed, minSFXDistance); FI
				Spawn(getArchetypeId(UpdateJudgment), {EntityInfo.get(0)});
				Set(EntityInputId, 0, time + accuracy);
				Set(EntityInputId, 1, Buckets.hold);
			} FI
		} FI
		return VOID;
	}

	SonolusApi updateSequential() {
		FUNCBEGIN
		IF (times.skip) lastSpawn = -1; FI
		IF (times.now < 0) Return(0); FI
		positionY = If(isAbove, floorPosition - line.get(5), floorPosition + line.get(5));
		IF (isFake) Return(0); FI

		// 画粒子效果
		IF (times.skip) Return(0); FI
		IF (isReplay) {
			IF (judgeResult2 == 0 && times.now >= time + accuracy && times.now <= judgeTime2 && times.now - lastSpawn >= 30 / bpm) {
				SpawnParticleEffect(If(Abs(accuracy) < judgment.perfect, Effects.perfect, Effects.great), 
					effectX1, effectY1, 
					effectX2, effectY2, 
					effectX3, effectY3, 
					effectX4, effectY4, effectDurationTime);
				lastSpawn = times.now;
			} FI
		} ELSE {
			IF (times.now >= time && times.now <= time + holdTime && times.now - lastSpawn >= 30 / bpm) {
				SpawnParticleEffect(Effects.perfect, 
					effectX1, effectY1, 
					effectX2, effectY2, 
					effectX3, effectY3, 
					effectX4, effectY4, effectDurationTime);
				lastSpawn = times.now;
			} FI
		} FI
		return VOID;
	}

	SonolusApi terminate() {
		FUNCBEGIN
		IF (times.skip) {
			lastSpawn = -1;
			Return(0); 
		} FI
		return VOID;
	}

	SonolusApi updateParallel() {
		FUNCBEGIN
		IF (times.now < 0) Return(0); FI
		var currentFloorPosition = If(isAbove, positionY, -1 * positionY);
		IF (times.now < time && currentFloorPosition < floorPositionLimit) Return(0); FI
		IF (currentFloorPosition * 1 > 10 / 3 * 5.85) Return(0); FI
		var dx = positionX * stage.w * 0.05625;
		var dy = positionY * stage.h * 0.6;
		IF (times.now > time) dy = 0; FI
		var dy2 = If(isAbove, endFloorPosition - line.get(5), endFloorPosition + line.get(5)) * stage.h * 0.6;
		
		var rotate = line.get(3);
		var r = Power({dx * dx + dy * dy, 0.5});
		var hr = Power({dx * dx + dy2 * dy2, 0.5});
		var angle = Arctan2(dy, dx);
		var hangle = Arctan2(dy2, dx);
		var newAngle = angle + rotate;
		var hnewAngle = hangle + rotate;
		var x = r * Cos(newAngle) + line.get(1), y = r * Sin(newAngle) + line.get(2);
		var hx = hr * Cos(hnewAngle) + line.get(1), hy = hr * Sin(hnewAngle) + line.get(2);
		var x0 = dx * Cos(rotate) + line.get(1), y0 = dx * Sin(rotate) + line.get(2);
		
		var vec1Length = noteWidth / 2, vec1X = vec1Length * Cos(rotate), vec1Y = vec1Length * Sin(rotate);
		var x1 = x - vec1X, y1 = y - vec1Y, x2 = x + vec1X, y2 = y + vec1Y;
		var hx1 = hx - vec1X, hy1 = hy - vec1Y, hx2 = hx + vec1X, hy2 = hy + vec1Y;
		var vec2Length = noteWidth / 2 / If(isMulti, hlHoldHeadRatio, holdHeadRatio);
		var vec2X = vec2Length * Cos(rotate + PI / 2), vec2Y = vec2Length * Sin(rotate + PI / 2);
		var vec3X = noteWidth / 2 / hlHoldOffset * Cos(rotate - PI / 2), vec3Y = noteWidth / 2 / hlHoldOffset * Sin(rotate - PI / 2);
		var x3 = x1, y3 = y1;
		var x4 = x1 + If(isAbove, 1, -1) * 2 * vec2X, y4 = y1 + If(isAbove, 1, -1) * 2 * vec2Y;
		var x5 = x2 + If(isAbove, 1, -1) * 2 * vec2X, y5 = y2 + If(isAbove, 1, -1) * 2 * vec2Y;
		var x6 = x2, y6 = y2;
		IF (isMulti) {
			x3 = x3 + If(isAbove, 1, -1) * vec3X; y3 = y3 + If(isAbove, 1, -1) * vec3Y;
			x4 = x4 + If(isAbove, 1, -1) * vec3X; y4 = y4 + If(isAbove, 1, -1) * vec3Y;
			x5 = x5 + If(isAbove, 1, -1) * vec3X; y5 = y5 + If(isAbove, 1, -1) * vec3Y;
			x6 = x6 + If(isAbove, 1, -1) * vec3X; y6 = y6 + If(isAbove, 1, -1) * vec3Y;
		} FI
		// 粒子效果不用转
		effectX1 = x0 - effectWidth, effectY1 = y0 - effectWidth;
		effectX2 = x0 - effectWidth, effectY2 = y0 + effectWidth;
		effectX3 = x0 + effectWidth, effectY3 = y0 + effectWidth;
		effectX4 = x0 + effectWidth, effectY4 = y0 - effectWidth;
		
		Draw(If(isMulti, Sprites.HLHoldHead, Sprites.NormalHoldHead), x3, y3, x4, y4, x5, y5, x6, y6, 1000 + 1000 - time + EntityInfo.get(0) / 10000, If(times.now > judgeTime2 && isReplay, 0.4, 1));
		Draw(If(isMulti, Sprites.HLHoldBody, Sprites.NormalHoldBody), x4, y4, hx1, hy1, hx2, hy2, x5, y5, 1000 + 1000 - time + EntityInfo.get(0) / 10000, If(times.now > judgeTime2 && isReplay, 0.4, 1));
		return VOID;
	}
};

class FakeHoldNote: public HoldNote {
	public:

	static constexpr const char* name = "Phigros Fake Hold Note";
	bool hasInput = false;
};