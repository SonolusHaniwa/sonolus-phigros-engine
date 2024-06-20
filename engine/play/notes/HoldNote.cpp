class HoldNote: public Archetype {
	public:

	static constexpr const char* name = "Phigros Hold Note";
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
	Variable<EntityMemoryId> effectId;
	Variable<EntityMemoryId> isPerfect;
	Variable<EntityMemoryId> isGood;

	BlockPointer<EntitySharedMemoryArrayId> line = EntitySharedMemoryArray[judgeline];

	SonolusApi spawnOrder() { return 2; }
	SonolusApi shouldSpawn() { return 1; }

	SonolusApi preprocess() {
		FUNCBEGIN
		isActive = false;
		released = false;
		isPerfect = 0;
		inputTimeMax = time + judgment.good;
		inputTimeMin = time - judgment.good;
		effectId = -1;
		return VOID;
	}

	SonolusApi updateSequential() {
		FUNCBEGIN
		IF (times.now < 0) Return(0); FI
		IF (times.now > time + holdTime) {
			IF (isActive && !released) {
				IF (isPerfect) EntityInput.set(0, 1);
				ELSE EntityInput.set(0, 2); FI
				IF (effectId != -1) {
					DestroyParticleEffect(effectId);
				} FI
			} ELSE {
				IF (isGood) EntityInput.set(0, 3);
				ELSE EntityInput.set(0, 0); FI
			} FI
			Debuglog(EntityInput.get(0));
			EntityDespawn.set(0, 1);
		} FI
		IF (isAbove) positionY = floorPosition - line.get(5);
		ELSE positionY = floorPosition + line.get(5); FI

		// 检测
		IF (isActive && !released && times.now < time + holdTime - holdTailTime) {
			IF (!hasTouch(EntityInfo.get(0))) {
				released = true;
				IF (effectId != -1) {
					DestroyParticleEffect(effectId);
					effectId = -1;
				} FI
				Return(0);
			} FI
		} FI
		// Claim
		IF (isActive || released) Return(0); FI
		IF (times.now < inputTimeMin) Return(0); FI
		IF (times.now > inputTimeMax) {
			isActive = true;
			released = true;
			EntityInput.set(1, times.now - time);
			Return(0);
		} FI
		claimStart(EntityInfo.get(0));
		return VOID;
	}

	SonolusApi touch() {
		FUNCBEGIN
		IF (isActive) Return(0); FI
		IF (times.now < inputTimeMin) Return(0); FI
		IF (times.now > inputTimeMax) Return(0); FI
		let index = getClaimedStart(EntityInfo.get(0));
		IF (index == -1) Return(0); FI

		isActive = true;
		IF (Abs(times.now - time) <= judgment.perfect) isPerfect = 1; FI
		IF (Abs(times.now - time) > judgment.great) isGood = 1; released = true; FI
		IF (Abs(times.now - time) <= judgment.great) Play(Clips.Note, minSFXDistance); FI
		EntityInput.set(1, times.now - time);
		return VOID;
	}

	SonolusApi updateParallel() {
		FUNCBEGIN
		IF (times.now < 0) Return(0); FI
		var dx = positionX * stage.w * 0.05625;
		var dy = positionY * stage.h * 0.6;
		var dy2 = dy + speed * holdTime * If(isAbove, 1, -1) * stage.h * 0.6;
		dy = If(isAbove, Max(0, dy), Min(0, dy));
		dy2 = If(isAbove, Max(0, dy2), Min(0, dy2));
		
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
		
		var vec1Length = noteWidth, vec1X = vec1Length * Cos(rotate), vec1Y = vec1Length * Sin(rotate);
		var x1 = x - vec1X, y1 = y - vec1Y, x2 = x + vec1X, y2 = y + vec1Y;
		var hx1 = hx - vec1X, hy1 = hy - vec1Y, hx2 = hx + vec1X, hy2 = hy + vec1Y;
		var vec2Length = noteWidth / If(isMulti, hlHoldHeadRatio, holdHeadRatio);
		var vec2X = vec2Length * Cos(rotate + PI / 2), vec2Y = vec2Length * Sin(rotate + PI / 2);
		var vec3X = noteWidth / hlHoldOffset * Cos(rotate - PI / 2), vec3Y = noteWidth / hlHoldOffset * Sin(rotate - PI / 2);
		var x3 = x1 - vec3X, y3 = y1;
		var x4 = x1 + 2 * vec2X, y4 = y1 + 2 * vec2Y;
		var x5 = x2 + 2 * vec2X, y5 = y2 + 2 * vec2Y;
		var x6 = x2, y6 = y2;
		IF (isMulti) {
			x3 = x3 + vec3X; y3 = y3 + vec3Y;
			x4 = x4 + vec3X; y4 = y4 + vec3Y;
			x5 = x5 + vec3X; y5 = y5 + vec3Y;
			x6 = x6 + vec3X; y6 = y6 + vec3Y;
		} FI
		// 粒子效果不用转
		effectX1 = x0 - noteWidth, effectY1 = y0 - noteWidth;
		effectX2 = x0 - noteWidth, effectY2 = y0 + noteWidth;
		effectX3 = x0 + noteWidth, effectY3 = y0 + noteWidth;
		effectX4 = x0 + noteWidth, effectY4 = y0 - noteWidth;
		// 画粒子效果
		IF (isActive && !released) {
			IF (effectId == -1) {
				effectId = SpawnLoopedParticleEffect(If(isPerfect, Effects.perfect, Effects.great), 
					effectX1, effectY1, 
					effectX2, effectY2, 
					effectX3, effectY3, 
					effectX4, effectY4, effectDurationTime / 2);
			} ELSE {
				MoveParticleEffect(effectId, 
					effectX1, effectY1, 
					effectX2, effectY2, 
					effectX3, effectY3, 
					effectX4, effectY4);
			} FI
		} FI
		
		Draw(If(isMulti, Sprites.HLHoldHead, Sprites.NormalHoldHead), x3, y3, x4, y4, x5, y5, x6, y6, 10000, If(released, 0.4, 1));
		Draw(If(isMulti, Sprites.HLHoldBody, Sprites.NormalHoldBody), x4, y4, hx1, hy1, hx2, hy2, x5, y5, 10000, If(released, 0.4, 1));
		return VOID;
	}
};