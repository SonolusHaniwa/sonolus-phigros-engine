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
	defineExports(accuracy);
	defineExports(judgeResult);
	defineExports(judgeTime);
	defineExports(judgeResult2);
	Variable<EntityMemoryId> positionY;
	Variable<EntityMemoryId> effectX1;
	Variable<EntityMemoryId> effectY1;
	Variable<EntityMemoryId> effectX2;
	Variable<EntityMemoryId> effectY2;
	Variable<EntityMemoryId> effectX3;
	Variable<EntityMemoryId> effectY3;
	Variable<EntityMemoryId> effectX4;
	Variable<EntityMemoryId> effectY4;
	Variable<EntityMemoryId> inputTimeMax;
	Variable<EntityMemoryId> inputTimeMin;
	Variable<EntityMemoryId> isActive;
	Variable<EntityMemoryId> released;
	Variable<EntityMemoryId> isPerfect;
	Variable<EntityMemoryId> comboChanged;
	Variable<EntityMemoryId> lastSpawn;
	Variable<EntityMemoryId> sfxPlayed;
	Variable<EntityMemoryId> appearTime;

	BlockPointer<EntitySharedMemoryArrayId> line = EntitySharedMemoryArray[judgeline];

	SonolusApi spawnOrder() { return appearTime; }
	SonolusApi shouldSpawn() { return times.now > appearTime; }

	int preprocessOrder = 514;
	SonolusApi preprocess() {
		FUNCBEGIN
		time = time * timeMagic / bpm;
		holdTime = holdTime * timeMagic / bpm;
		IF (!isFake) notes = notes + 1; FI
		isActive = false;
		released = false;
		isPerfect = 0;
		comboChanged = 0;
		inputTimeMax = time + judgment.good;
		inputTimeMin = time - judgment.good;
		isMulti = isMulti && hasSimul;
		lastSpawn = -1;
		maxTime = Max(maxTime, time);
		maxTime = Max(maxTime, time + holdTime);
		sfxPlayed = false;
		var id = EntityDataArray[judgeline].get(0);
		WHILE (id) {
			var deltaFloorPosition = Abs(floorPosition) - EntitySharedMemoryArray[id].get(1);
			IF (deltaFloorPosition <= 10 / 3 / 1) BREAK; FI
			appearTime = EntityDataArray[id].get(0) * timeMagic / bpm + (deltaFloorPosition - 10 / 3 / 1) / EntityDataArray[id].get(2);
			id = EntityDataArray[id].get(3);
		} DONE
		appearTime = Max(0, Min(appearTime, time - 0.5));
		return VOID;
	}

	SonolusApi updateSequential() {
		FUNCBEGIN
		IF (times.now < 0) Return(0); FI
		IF (isAbove) positionY = floorPosition - line.get(5);
		ELSE positionY = floorPosition + line.get(5); FI
		IF (isFake) 
			released = false;
			IF (times.now > time + holdTime) EntityDespawn.set(0, 1); FI
			Return(0); 
		FI
		IF (times.now > time + holdTime) {
			IF (isActive && !released) {
				combo = combo + 1;
				IF (isPerfect) {
					accscore = accscore + score.perfect;
					judgeStatus = Min(judgeStatus, 2); EntityInput.set(0, 1);
					ExportValue(judgeResult, 2);
					ExportValue(judgeTime, times.now - time);
				} ELSE {
					accscore = accscore + score.great;
					judgeStatus = Min(judgeStatus, 1); EntityInput.set(0, 2); 
					ExportValue(judgeResult, 2);
					ExportValue(judgeTime, times.now - time);
				} FI
				SpawnParticleEffect(If(isPerfect, Effects.perfect, Effects.great), 
					effectX1, effectY1, 
					effectX2, effectY2,
					effectX3, effectY3, 
					effectX4, effectY4, effectDurationTime);
			} ELSE {
				IF (!comboChanged) {
					combo = 0;
					judgeStatus = Min(judgeStatus, 0); 
					ExportValue(judgeResult, 0);
					ExportValue(judgeTime, times.now - time);
				} FI
				EntityInput.set(0, 0);
			} FI
			EntityDespawn.set(0, 1);
		} FI
		IF (hasSFX && autoSFX && !sfxPlayed) PlayScheduled(Clips.Note, time / levelSpeed, minSFXDistance); sfxPlayed = true; FI

		// 检测
		IF (isActive && !released && times.now < time + holdTime - holdTailTime) {
			IF (!hasTouch(EntityInfo.get(0))) {
				released = true;
				judgeStatus = Min(judgeStatus, 0); combo = 0;
				ExportValue(judgeResult, 0);
				ExportValue(judgeTime, times.now - time);
				comboChanged = true;
				Return(0);
			} FI
		} FI

		// 画粒子效果
		IF (isActive && !released && times.now <= time + holdTime && times.now - lastSpawn >= 30 / bpm) {
			SpawnParticleEffect(If(isPerfect, Effects.perfect, Effects.great), 
				effectX1, effectY1, 
				effectX2, effectY2, 
				effectX3, effectY3, 
				effectX4, effectY4, effectDurationTime);
			lastSpawn = times.now;
		} FI

		// Claim
		IF (isActive || released) Return(0); FI
		IF (times.now < inputTimeMin) Return(0); FI
		IF (times.now > inputTimeMax) {
			combo = 0;
			judgeStatus = Min(judgeStatus, 0); 
			ExportValue(judgeResult, 0);
			ExportValue(judgeTime, times.now - time);
			ExportValue(judgeResult2, 1);
			isActive = true;
			released = true;
			comboChanged = true;
			Return(0);
		} FI
		ExportValue(accuracy, times.now - time);
		claimStart(EntityInfo.get(0));
		return VOID;
	}

	SonolusApi touch() {
		FUNCBEGIN
		IF (isActive) Return(0); FI
		IF (times.now < inputTimeMin) Return(0); FI
		IF (times.now > inputTimeMax) Return(0); FI
		IF (isFake) Return(0); FI
		let index = getClaimedStart(EntityInfo.get(0));
		IF (index == -1) Return(0); FI

		isActive = true;
		IF (Abs(times.now - time) <= judgment.perfect) isPerfect = 1; FI
		IF (hasSFX && !autoSFX) Play(Clips.Note, minSFXDistance); FI 
		EntityInput.set(1, times.now - time);
		EntityInput.set(2, Buckets.hold);
		EntityInput.set(3, times.now - time);
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
		
		var vec1Length = noteWidth / 2 * If(isMulti, 1062.0 / 989.0, 1), vec1X = vec1Length * Cos(rotate), vec1Y = vec1Length * Sin(rotate);
		var x1 = x - vec1X, y1 = y - vec1Y, x2 = x + vec1X, y2 = y + vec1Y;
		var hx1 = hx - vec1X, hy1 = hy - vec1Y, hx2 = hx + vec1X, hy2 = hy + vec1Y;
		var vec2Length = noteWidth / 2 * If(isMulti, 1062.0 / 989.0, 1) / If(isMulti, hlHoldHeadRatio, holdHeadRatio);
		var vec2X = vec2Length * Cos(rotate + PI / 2), vec2Y = vec2Length * Sin(rotate + PI / 2);
		var vec3X = noteWidth / 2 / hlHoldOffset * Cos(rotate - PI / 2), vec3Y = noteWidth / hlHoldOffset * Sin(rotate - PI / 2);
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
		
		Draw(If(isMulti, Sprites.HLHoldHead, Sprites.NormalHoldHead), x3, y3, x4, y4, x5, y5, x6, y6, 1000 + 1000 - time + EntityInfo.get(0) / 10000, If(released, 0.4, 1));
		Draw(If(isMulti, Sprites.HLHoldBody, Sprites.NormalHoldBody), x4, y4, hx1, hy1, hx2, hy2, x5, y5, 1000 + 1000 - time + EntityInfo.get(0) / 10000, If(released, 0.4, 1));
		return VOID;
	}
};

class FakeHoldNote: public HoldNote {
	public:

	static constexpr const char* name = "Phigros Fake Hold Note";
	bool hasInput = false;
};