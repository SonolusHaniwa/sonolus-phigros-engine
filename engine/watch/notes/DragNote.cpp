class DragNote: public Archetype {
	public:

	static constexpr const char* name = "Phigros Drag Note";
	bool hasInput = true;
	defineImports(time);
	defineImports(positionX);
	defineImports(holdTime);
	defineImports(speed);
	defineImports(floorPosition);
	defineImports(isAbove);
	defineImports(isMulti);
	defineImports(isFake);
	defineImports(judgeline);
	defineImports(bpm);
	defineImports(accuracy);
	defineImports(judgeResult);
	Variable<EntityMemoryId> positionY;
	Variable<EntityMemoryId> effectX1;
	Variable<EntityMemoryId> effectY1;
	Variable<EntityMemoryId> effectX2;
	Variable<EntityMemoryId> effectY2;
	Variable<EntityMemoryId> effectX3;
	Variable<EntityMemoryId> effectY3;
	Variable<EntityMemoryId> effectX4;
	Variable<EntityMemoryId> effectY4;
	Variable<EntityMemoryId> appearTime;
	Variable<EntitySharedMemoryId> nextNote; // 下一个按键信息
	Variable<EntitySharedMemoryId> judgeTime; // 判定时间
	Variable<EntitySharedMemoryId> currentCombo; // 当前 Combo 数
	Variable<EntitySharedMemoryId> currentMaxCombo; // 当前最大 Combo 数
	Variable<EntitySharedMemoryId> currentJudgeStatus; // 当前判定结果
	Variable<EntitySharedMemoryId> currentAccScore; // 当前准度得分
	Variable<EntitySharedMemoryId> comboId; // Combo 排行

	BlockPointer<EntitySharedMemoryArrayId> line = EntitySharedMemoryArray[judgeline];

	SonolusApi spawnTime() { return appearTime; }
	SonolusApi despawnTime() { return If(isReplay,
		If(Abs(time + accuracy + 1) < 0.001, time + judgment.good, time + accuracy),
		time
	); }

	int preprocessOrder = 514;
	SonolusApi preprocess() {
		FUNCBEGIN
		time = time * timeMagic / bpm;
		notes = notes + 1;
		isMulti = isMulti && hasSimul;
		maxTime = Max(maxTime, time);
		var id = EntityDataArray[judgeline].get(0);
		WHILE (id) {
			var deltaFloorPosition = Abs(floorPosition) - EntitySharedMemoryArray[id].get(1);
			IF (deltaFloorPosition <= 10 / 3 / speed) BREAK; FI
			appearTime = EntityDataArray[id].get(0) * timeMagic / bpm + (deltaFloorPosition - 10 / 3 / speed) / EntityDataArray[id].get(2);
			id = EntityDataArray[id].get(3);
		} DONE
		appearTime = Max(0, Min(appearTime, time - 0.5));
		IF (isReplay) {
			judgeTime = If(Abs(time + accuracy + 1) < 0.001, time + judgment.good, time + accuracy);
			IF (judgeResult != 0 && hasSFX && !autoSFX) PlayScheduled(Clips.Drag, time + accuracy, minSFXDistance); FI
			IF (autoSFX && hasSFX) PlayScheduled(Clips.Drag, time, minSFXDistance); FI
			Spawn(getArchetypeId(UpdateJudgment), {EntityInfo.get(0)});
		} ELSE {
			judgeTime = time.get();
			IF (hasSFX) PlayScheduled(Clips.Drag, time, minSFXDistance); FI
			Spawn(getArchetypeId(UpdateJudgment), {EntityInfo.get(0)});
		} FI
		return VOID;
	}

	// SonolusApi complete(let hitTime) {
	// 	FUNCBEGIN
	// 	IF (Abs(hitTime - time) <= judgment.good) {
	// 		IF (hasSFX) Play(Clips.Drag, minSFXDistance); FI
	// 		judgeStatus = Min(judgeStatus, 2); combo = combo + 1;
	// 		accscore = accscore + score.perfect;
	// 		SpawnParticleEffect(Effects.perfect, 
	// 			effectX1, effectY1, effectX2, effectY2,
	// 			effectX3, effectY3, effectX4, effectY4,
	// 			effectDurationTime);
	// 		EntityInput.set(0, 1); 
	// 		EntityInput.set(1, hitTime - time);
	// 		EntityInput.set(2, Buckets.drag);
	// 		EntityInput.set(3, hitTime - time);
	// 	} FI
	// 	IF (Abs(hitTime - time) > judgment.good) {
	// 		judgeStatus = Min(judgeStatus, 0); combo = 0;
	// 	} FI
	// 	EntityDespawn.set(0, 1);
	// 	return VOID;
	// }
	int updateSequentialOrder = 114514;
	SonolusApi updateSequential() {
		FUNCBEGIN
		// IF (currentCombo >= 1137 && currentCombo <= 1139) Debuglog(EntityInfo.get(0)); FI
		IF (times.now < 0) Return(0); FI
		IF (isAbove) positionY = floorPosition - line.get(5);
		ELSE positionY = floorPosition + line.get(5); FI
		return VOID;
	}

	SonolusApi terminate() {
		FUNCBEGIN
		IF (times.skip) Return(0); FI
		IF (isReplay && judgeResult == 0) Return(0); FI
		SpawnParticleEffect(If(judgeResult == 2 || !isReplay, Effects.perfect, Effects.great), 
			effectX1, effectY1, effectX2, effectY2,
			effectX3, effectY3, effectX4, effectY4,
			effectDurationTime);
		return VOID;
	}

	SonolusApi updateParallel() {
		FUNCBEGIN
		IF (times.now < 0) Return(0); FI
		var currentFloorPosition = If(isAbove, positionY, -1 * positionY);
		IF (times.now < time && currentFloorPosition < floorPositionLimit) Return(0); FI
		var dx = positionX * stage.w * 0.05625;
		var dy = positionY * speed * stage.h * 0.6;
		
		var rotate = line.get(3);
		var r = Power({dx * dx + dy * dy, 0.5});
		var angle = Arctan2(dy, dx);
		var newAngle = angle + rotate;
		var x = r * Cos(newAngle) + line.get(1), y = r * Sin(newAngle) + line.get(2);
		var x0 = dx * Cos(rotate) + line.get(1), y0 = dx * Sin(rotate) + line.get(2);
		
		var vec1Length = noteWidth, vec1X = vec1Length * Cos(rotate), vec1Y = vec1Length * Sin(rotate);
		var x1 = x - vec1X, y1 = y - vec1Y, x2 = x + vec1X, y2 = y + vec1Y;
		var vec2Length = noteWidth / If(isMulti, hlDragRatio, dragRatio);
		var vec2X = vec2Length * Cos(rotate + PI / 2), vec2Y = vec2Length * Sin(rotate + PI / 2);
		var x3 = x1 - vec2X, y3 = y1 - vec2Y;
		var x4 = x1 + vec2X, y4 = y1 + vec2Y;
		var x5 = x2 + vec2X, y5 = y2 + vec2Y;
		var x6 = x2 - vec2X, y6 = y2 - vec2Y;
		// 粒子效果不用转
		effectX1 = x0 - effectWidth, effectY1 = y0 - effectWidth;
		effectX2 = x0 - effectWidth, effectY2 = y0 + effectWidth;
		effectX3 = x0 + effectWidth, effectY3 = y0 + effectWidth;
		effectX4 = x0 + effectWidth, effectY4 = y0 - effectWidth;
		
		Draw(If(isMulti, Sprites.HLDrag, Sprites.NormalDrag), x3, y3, x4, y4, x5, y5, x6, y6, 10000 + 1000 - time + EntityInfo.get(0) / 10000, If(times.now > time, Max(1 - (times.now - time) / judgment.great, 0), 1));
		return VOID;
	}
};