class FlickNote: public Archetype {
	public:

	static constexpr const char* name = "Phigros Flick Note";
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
	defineImports(size);
	defineImports(yOffset);
	defineImports(visibleTime);
	defineImports(alpha);
	defineExports(accuracy);
	defineExports(judgeResult);
	Variable<EntityMemoryId> positionY;
	Variable<EntityMemoryId> played;
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
	Variable<EntityMemoryId> sfxPlayed;
	Variable<EntityMemoryId> appearTime;

	BlockPointer<EntitySharedMemoryArrayId> line = EntitySharedMemoryArray[judgeline];

	SonolusApi spawnOrder() { return appearTime; }
	SonolusApi shouldSpawn() { return times.now > appearTime; }
	
	int preprocessOrder = 514;
	SonolusApi preprocess() {
		FUNCBEGIN
		time = time * timeMagic / bpm;
		IF (!isFake) notes = notes + 1; FI
		played = false;
		inputTimeMax = time + judgment.good;
		inputTimeMin = time - judgment.good;
		isMulti = isMulti && hasSimul;
		maxTime = Max(maxTime, time);
		sfxPlayed = false;
		IF (mirror) positionX = -1 * positionX; FI
		// var id = EntityDataArray[judgeline].get(0);
		// WHILE (id) {
		// 	var deltaFloorPosition = Abs(floorPosition) - EntitySharedMemoryArray[id].get(1);
		// 	IF (deltaFloorPosition <= 10 / 3 / speed * 5.85) BREAK; FI
		// 	appearTime = EntityDataArray[id].get(0) * timeMagic / bpm + (deltaFloorPosition - 10 / 3 / speed * 5.85) / EntityDataArray[id].get(2);
		// 	id = EntityDataArray[id].get(3);
		// } DONE
		// appearTime = Max(0, Min(appearTime, time - 0.5));
		return VOID;
	}

	SonolusApi complete(let hitTime) {
		FUNCBEGIN
		ExportValue(accuracy, hitTime - time);
		IF (Abs(hitTime - time) <= judgment.good) {
			IF (hasSFX && !autoSFX) Play(Clips.Flick, minSFXDistance); FI
			judgeStatus = Min(judgeStatus, 2); combo = combo + 1;
			accscore = accscore + score.perfect;
			SpawnParticleEffect(Effects.perfect, 
				effectX1, effectY1, effectX2, effectY2,
				effectX3, effectY3, effectX4, effectY4,
				effectDurationTime);
			EntityInput.set(0, 1); 
			EntityInput.set(1, hitTime - time);
			EntityInput.set(2, Buckets.flick);
			EntityInput.set(3, (hitTime - time) * 1000);
			ExportValue(judgeResult, 2);
		} FI
		IF (Abs(hitTime - time) > judgment.good) {
			judgeStatus = Min(judgeStatus, 0); combo = 0;
			ExportValue(judgeResult, 0);
		} FI
		EntityDespawn.set(0, 1);
		return VOID;
	}
	int updateSequentialOrder = 1919810;
	SonolusApi updateSequential() {
		FUNCBEGIN
		IF (times.now < 0) Return(0); FI
		IF (isAbove) positionY = floorPosition - line.get(5);
		ELSE positionY = floorPosition + line.get(5); FI
		IF (isFake) 
			IF (times.now > time) EntityDespawn.set(0, 1); FI
			Return(0); 
		FI
		IF (hasSFX && autoSFX && !sfxPlayed) PlayScheduled(Clips.Flick, time / levelSpeed, minSFXDistance); sfxPlayed = true; FI

		// Claim
		IF (played) {
			IF (times.now < time) Return(0); FI
			complete(times.now);
			Return(0);
		} FI
		IF (times.now < inputTimeMin) Return(0); FI
		IF (times.now > inputTimeMax) complete(-1); FI
		flickClaimStart(EntityInfo.get(0));
		return VOID;
	}

	SonolusApi touch() {
		FUNCBEGIN
		IF (played) Return(0); FI
		IF (isFake) Return(0); FI
		IF (times.now < inputTimeMin) Return(0); FI
		let index = flickGetClaimedStart(EntityInfo.get(0));
		IF (index == -1) Return(0); FI
		played = true;
		return VOID;
	}

	SonolusApi updateParallel() {
		FUNCBEGIN
		IF (times.now < 0 || time - times.now > visibleTime) Return(0); FI
		var currentFloorPosition = If(isAbove, positionY, -1 * positionY);
		IF (times.now < time && currentFloorPosition < floorPositionLimit) Return(0); FI
		IF (currentFloorPosition * speed > 10 / 3 * 5.85) Return(0); FI
		var dx = positionX * stage.w * 0.05625;
		var dy = If(isAbove, positionY + yOffset, positionY - yOffset) * speed * stage.h * 0.6;
		
		var rotate = line.get(3);
		var r = Power({dx * dx + dy * dy, 0.5});
		var angle = Arctan2(dy, dx);
		var newAngle = angle + rotate;
		var x = r * Cos(newAngle) + line.get(1), y = r * Sin(newAngle) + line.get(2);
		var x0 = dx * Cos(rotate) + line.get(1), y0 = dx * Sin(rotate) + line.get(2);
		
		var vec1Length = noteWidth * size * If(isMulti, 1089.0 / 989.0, 1), vec1X = vec1Length * Cos(rotate), vec1Y = vec1Length * Sin(rotate);
		var x1 = x - vec1X, y1 = y - vec1Y, x2 = x + vec1X, y2 = y + vec1Y;
		var vec2Length = noteWidth * size * If(isMulti, 1089.0 / 989.0, 1) / If(isMulti, hlFlickRatio, flickRatio);
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
		// IF (x3 >= 0 && x3 <= stage.r && y3 <= 0 && y3 >= stage.b) Debuglog(EntityInfo.get(0)); FI
		
		Draw(If(isMulti, Sprites.HLFlick, Sprites.NormalFlick), x3, y3, x4, y4, x5, y5, x6, y6, 12000 + 1000 - time + EntityInfo.get(0) / 10000, If(times.now > time, Max(1 - (times.now - time) / judgment.great, 0), 1) * alpha);
		return VOID;
	}
};

class FakeFlickNote: public FlickNote {
	public:

	static constexpr const char* name = "Phigros Fake Flick Note";
	bool hasInput = false;
};