Bucket NormalHoldBucket = defineBucket({
	EngineDataBucketSprite(Sprites.NormalHoldHead, 0, -1.95, 0.0, 2.0, 0.1, 270),
	EngineDataBucketSprite(Sprites.NormalHoldBody, 0, 0.04, 0.0, 2.0, 3.9, 270),
}, Text.MillisecondUnit);

class HoldNote: public Archetype {
	public:

	string name = "Phigros Hold Note";
	bool hasInput = true;
	defineImport(time);
	defineImport(positionX);
	defineImport(holdTime);
	defineImport(speed);
	defineImport(floorPosition);
	defineImport(endFloorPosition);
	defineImport(isAbove);
	defineImport(isMulti);
	defineImport(isFake);
	defineImport(judgeline);
	defineImport(bpm);
	defineImport(size);
	defineImport(yOffset);
	defineImport(visibleTime);
	defineImport(alpha);
	defineExport(accuracy);
	defineExport(judgeResult);
	defineExport(judgeTime);
	defineExport(judgeResult2);
	var positionY;
	var effectX1;
	var effectY1;
	var effectX2;
	var effectY2;
	var effectX3;
	var effectY3;
	var effectX4;
	var effectY4;
	var inputTimeMax;
	var inputTimeMin;
	var isActive;
	var released;
	var isPerfect;
	var comboChanged;
	var lastSpawn;
	var sfxPlayed;
	var appearTime;

	EntitySharedMemoryArrayGroup line = EntitySharedMemoryArray[judgeline];

	SonolusApi spawnOrder() { return appearTime; }
	SonolusApi shouldSpawn() { return times.now > appearTime; }

	int preprocessOrder = 514;
	SonolusApi preprocess() {
		// 查找并更正 speed
		var currId = EntityDataArray[judgeline].generic[0];
		while (currId) {
			if (EntityDataArray[currId].generic[1] > time) {
				speed =
					(time - EntityDataArray[currId].generic[0]) /
					(EntityDataArray[currId].generic[1] - EntityDataArray[currId].generic[0]) *
					(EntityDataArray[currId].generic[3] - EntityDataArray[currId].generic[2]) +
					EntityDataArray[currId].generic[2];
				break;
			}
			currId = EntityDataArray[currId].generic[4];
		}
		
		time = time * timeMagic / bpm;
		time /= levelSpeed;
		holdTime = holdTime * timeMagic / bpm;
		holdTime /= levelSpeed;
		if (!isFake) notes = notes + 1;
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
		if (mirror) positionX = -1 * positionX;
		input.accuracy = judgment.good;
		endFloorPosition = If(isAbove, floorPosition + speed * holdTime, floorPosition - speed * holdTime);
		// var id = EntityDataArray[judgeline].get(0);
		// WHILE (id) {
		// 	var deltaFloorPosition = Abs(floorPosition) - EntitySharedMemoryArray[id].get(1);
		// 	if (deltaFloorPosition <= 10 / 3 / 1) BREAK;
		// 	appearTime = EntityDataArray[id].get(0) * timeMagic / bpm + (deltaFloorPosition - 10 / 3 / 1) / EntityDataArray[id].get(2);
		// 	id = EntityDataArray[id].get(3);
		// } DONE
		// appearTime = Max(0, Min(appearTime, time - 0.5));
		if (hasSFX && autoSFX) PlayScheduled(Clips.Note, time, minSFXDistance);
	}

	int updateSequentialOrder = 1919810;
	SonolusApi updateSequential() {
		if (times.now < 0) return;
		if (isAbove) positionY = floorPosition - line.generic[5];
		else positionY = floorPosition + line.generic[5];
		if (isFake) {
			released = false;
			if (times.now > time + holdTime) despawn.despawn = true;
			return;
		}
		if (times.now > time + holdTime) {
			if (isActive && !released) {
				combo = combo + 1;
				if (isPerfect) {
					accscore = accscore + score.perfect;
					judgeStatus = Min(judgeStatus, 2); input.judgment = 1;
					ExportValue(judgeResult, 2);
					ExportValue(judgeTime, times.now - time);
				} else {
					accscore = accscore + score.great;
					judgeStatus = Min(judgeStatus, 1); input.judgment = 2;
					ExportValue(judgeResult, 1);
					ExportValue(judgeTime, times.now - time);
				}
				SpawnParticleEffect(
					If(isPerfect, Effects.Perfect, Effects.Great), 
					{ effectX1, effectY1 }, 
					{ effectX2, effectY2 },
					{ effectX3, effectY3 }, 
					{ effectX4, effectY4 }, 
					effectDurationTime,
					false
				);
			} else {
				if (!comboChanged) {
					combo = 0;
					judgeStatus = Min(judgeStatus, 0); 
					ExportValue(judgeResult, 0);
					ExportValue(judgeTime, times.now - time);
				}
				input.judgment = 0;
			}
			despawn.despawn = true;
		}
		// if (hasSFX && autoSFX && !sfxPlayed && time - times.now <= scheduledSFXTime + Max(0, RuntimeEnvironment.get(2))) PlayScheduled(Clips.Note, time / levelSpeed, minSFXDistance); sfxPlayed = true;

		// 检测
		if (isActive && !released && times.now < time + holdTime - holdTailTime) {
			if (!hasTouch(info.index)) {
				released = true;
				judgeStatus = Min(judgeStatus, 0); combo = 0;
				ExportValue(judgeResult, 0);
				ExportValue(judgeTime, times.now - time);
				comboChanged = true;
				return;
			}
		}

		// 画粒子效果
		if (isActive && !released && times.now <= time + holdTime && times.now - lastSpawn >= 30 / bpm) {
			SpawnParticleEffect(
				If(isPerfect, Effects.Perfect, Effects.Great), 
				{ effectX1, effectY1 }, 
				{ effectX2, effectY2 }, 
				{ effectX3, effectY3 }, 
				{ effectX4, effectY4 }, 
				effectDurationTime,
				false
			);
			lastSpawn = times.now;
		}

		// Claim
		if (isActive || released) return;
		if (times.now < inputTimeMin) return;
		if (times.now > inputTimeMax) {
			combo = 0;
			judgeStatus = Min(judgeStatus, 0); 
			ExportValue(judgeResult, 0);
			ExportValue(judgeTime, times.now - time);
			ExportValue(judgeResult2, 1);
			isActive = true;
			released = true;
			comboChanged = true;
			return;
		}
		ExportValue(accuracy, times.now - time);
		claimStart(info.index);
	}

	SonolusApi touch() {
		if (isActive) return;
		if (times.now < inputTimeMin) return;
		if (times.now > inputTimeMax) return;
		if (isFake) return;
		var index = getClaimedStart(info.index);
		if (index == -1) return;

		isActive = true;
		if (Abs(times.now - time) <= judgment.perfect) isPerfect = 1;
		if (hasSFX && !autoSFX) Play(Clips.Note, minSFXDistance); 
		input.accuracy = times.now - time;
		input.bucketIndex = int(NormalHoldBucket);
		input.bucketValue = (times.now - time) * 1000;
	}

	SonolusApi updateParallel() {
		if (times.now < 0 || time - times.now > visibleTime) return;
		var currentFloorPosition = If(isAbove, positionY, -1 * positionY);
		if (times.now < time && currentFloorPosition < floorPositionLimit) return;
		if (currentFloorPosition * 1 > 10 / 3 * 5.85) return;
		var dx = positionX * stage.w * 0.05625;
		var dy = If(isAbove, positionY + yOffset, positionY - yOffset) * stage.h * 0.6;
		if (times.now > time) dy = If(isAbove, yOffset, -1 * yOffset);
		var dy2 = If(
			time > times.now,
			If(isAbove, endFloorPosition - line.generic[5] + yOffset, endFloorPosition + line.generic[5] - yOffset),
			If(isAbove, speed * (holdTime - times.now + time), -1 * speed * (holdTime - times.now + time))
		) * stage.h * 0.6;
		
		var rotate = line.generic[3];
		var r = Power({dx * dx + dy * dy, 0.5});
		var hr = Power({dx * dx + dy2 * dy2, 0.5});
		var angle = Arctan2(dy, dx);
		var hangle = Arctan2(dy2, dx);
		var newAngle = angle + rotate;
		var hnewAngle = hangle + rotate;
		var x = r * Cos(newAngle) + line.generic[1], y = r * Sin(newAngle) + line.generic[2];
		var hx = hr * Cos(hnewAngle) + line.generic[1], hy = hr * Sin(hnewAngle) + line.generic[2];
		var x0 = dx * Cos(rotate) + line.generic[1], y0 = dx * Sin(rotate) + line.generic[2];
		// if (EntityInfo.get(0) == 3094) Debuglog(positionY); Debuglog(speed);
		
		var vec1Length = noteWidth * size / 2 * If(isMulti, 1062.0 / 989.0, 1), vec1X = vec1Length * Cos(rotate), vec1Y = vec1Length * Sin(rotate);
		var x1 = x - vec1X, y1 = y - vec1Y, x2 = x + vec1X, y2 = y + vec1Y;
		var hx1 = hx - vec1X, hy1 = hy - vec1Y, hx2 = hx + vec1X, hy2 = hy + vec1Y;
		var vec2Length = noteWidth * size / 2 * If(isMulti, 1062.0 / 989.0, 1) / If(isMulti, hlHoldHeadRatio, holdHeadRatio);
		var vec2X = vec2Length * Cos(rotate + PI / 2), vec2Y = vec2Length * Sin(rotate + PI / 2);
		var vec3X = noteWidth * size / 2 / hlHoldOffset * Cos(rotate - PI / 2), vec3Y = noteWidth * size / hlHoldOffset * Sin(rotate - PI / 2);
		var x3 = x1, y3 = y1;
		var x4 = x1 + If(isAbove, 1, -1) * 2 * vec2X, y4 = y1 + If(isAbove, 1, -1) * 2 * vec2Y;
		var x5 = x2 + If(isAbove, 1, -1) * 2 * vec2X, y5 = y2 + If(isAbove, 1, -1) * 2 * vec2Y;
		var x6 = x2, y6 = y2;
		if (isMulti) {
			x3 = x3 + If(isAbove, 1, -1) * vec3X; y3 = y3 + If(isAbove, 1, -1) * vec3Y;
			x4 = x4 + If(isAbove, 1, -1) * vec3X; y4 = y4 + If(isAbove, 1, -1) * vec3Y;
			x5 = x5 + If(isAbove, 1, -1) * vec3X; y5 = y5 + If(isAbove, 1, -1) * vec3Y;
			x6 = x6 + If(isAbove, 1, -1) * vec3X; y6 = y6 + If(isAbove, 1, -1) * vec3Y;
		}
		// 粒子效果不用转
		effectX1 = x0 - effectWidth, effectY1 = y0 - effectWidth;
		effectX2 = x0 - effectWidth, effectY2 = y0 + effectWidth;
		effectX3 = x0 + effectWidth, effectY3 = y0 + effectWidth;
		effectX4 = x0 + effectWidth, effectY4 = y0 - effectWidth;
		
		Draw(
			If(isMulti, Sprites.HLHoldHead, Sprites.NormalHoldHead), 
			{ x3, y3 }, 
			{ x4, y4 }, 
			{ x5, y5 }, 
			{ x6, y6 }, 
			1000 + 1000 - time + info.index / 10000, 
			If(released, 0.4, 1) * alpha
		);
		Draw(
			If(isMulti, Sprites.HLHoldBody, Sprites.NormalHoldBody), 
			{ x4, y4 }, 
			{ hx1, hy1 }, 
			{ hx2, hy2 }, 
			{ x5, y5 }, 
			1000 + 1000 - time + info.index / 10000, 
			If(released, 0.4, 1) * alpha
		);
	}
};

class FakeHoldNote: public HoldNote {
	public:

	string name = "Phigros Fake Hold Note";
	bool hasInput = false;
};