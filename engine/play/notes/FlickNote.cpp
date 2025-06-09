Bucket NormalFlickBucket = defineBucket({
	EngineDataBucketSprite(Sprites.NormalFlick, 0, 0.0, 0.0, 4.0, 2.0, 0)
}, Text.MillisecondUnit);

class FlickNote: public Archetype {
	public:

	string name = "Phigros Flick Note";
	bool hasInput = true;
	defineImport(time);
	defineImport(positionX);
	defineImport(holdTime);
	defineImport(speed);
	defineImport(floorPosition);
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
	var positionY;
	var played;
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
	var sfxPlayed;
	var appearTime;

	EntitySharedMemoryArrayGroup line = EntitySharedMemoryArray[judgeline];

	SonolusApi spawnOrder() { return appearTime; }
	SonolusApi shouldSpawn() { return times.now > appearTime; }
	
	int preprocessOrder = 514;
	SonolusApi preprocess() {
		time = time * timeMagic / bpm;
		time /= levelSpeed;
		if (!isFake) notes = notes + 1;
		played = false;
		inputTimeMax = time + judgment.good;
		inputTimeMin = time - judgment.good;
		isMulti = isMulti && hasSimul;
		maxTime = Max(maxTime, time);
		sfxPlayed = false;
		if (mirror) positionX = -1 * positionX;
		input.accuracy = judgment.good;
		// var id = EntityDataArray[judgeline].generic[0];
		// WHILE (id) {
		// 	var deltaFloorPosition = Abs(floorPosition) - EntitySharedMemoryArray[id].generic[1];
		// 	if (deltaFloorPosition <= 10 / 3 / speed * 5.85) BREAK;
		// 	appearTime = EntityDataArray[id].generic[0] * timeMagic / bpm + (deltaFloorPosition - 10 / 3 / speed * 5.85) / EntityDataArray[id].generic[2];
		// 	id = EntityDataArray[id].generic[3];
		// } DONE
		// appearTime = Max(0, Min(appearTime, time - 0.5));
		if (hasSFX && autoSFX) PlayScheduled(Clips.Flick, time, minSFXDistance);
	}

	SonolusApi complete(var hitTime) {
		ExportValue(accuracy, hitTime - time);
		if (Abs(hitTime - time) <= judgment.good) {
			if (hasSFX && !autoSFX) Play(Clips.Flick, minSFXDistance);
			judgeStatus = Min(judgeStatus, 2); combo = combo + 1;
			accscore = accscore + score.perfect;
			SpawnParticleEffect(
				Effects.Perfect, 
				{ effectX1, effectY1 }, 
				{ effectX2, effectY2 },
				{ effectX3, effectY3 }, 
				{ effectX4, effectY4 },
				effectDurationTime,
				false
			);
			input.judgment = 1;
			input.accuracy = hitTime - time;
			input.bucketIndex = int(NormalFlickBucket);
			input.bucketValue = (hitTime - time) * 1000;
			ExportValue(judgeResult, 2);
		}
		if (Abs(hitTime - time) > judgment.good) {
			judgeStatus = Min(judgeStatus, 0); combo = 0;
			ExportValue(judgeResult, 0);
		}
		despawn.despawn = true;
	}
	int updateSequentialOrder = 1919810;
	SonolusApi updateSequential() {
		if (times.now < 0) return;
		if (isAbove) positionY = floorPosition - line.generic[5];
		else positionY = floorPosition + line.generic[5];
		if (isFake) {
			if (times.now > time) despawn.despawn = true;
			return; 
		}
		// if (hasSFX && autoSFX && !sfxPlayed && time - times.now <= scheduledSFXTime + Max(0, RuntimeEnvironment.generic[2])) PlayScheduled(Clips.Flick, time / levelSpeed, minSFXDistance); sfxPlayed = true;

		// Claim
		if (played) {
			if (times.now < time) return;
			complete(times.now);
			return;
		}
		if (times.now < inputTimeMin) return;
		if (times.now > inputTimeMax) complete(-1);
		flickClaimStart(info.index);
	}

	SonolusApi touch() {
		if (played) return;
		if (isFake) return;
		if (times.now < inputTimeMin) return;
		var index = flickGetClaimedStart(info.index);
		if (index == -1) return;
		played = true;
	}

	SonolusApi updateParallel() {
		if (times.now < 0 || time - times.now > visibleTime) return;
		var currentFloorPosition = If(isAbove, positionY, -1 * positionY);
		if (times.now < time && currentFloorPosition < floorPositionLimit) return;
		if (currentFloorPosition * speed > 10 / 3 * 5.85) return;
		var dx = positionX * stage.w * 0.05625;
		var dy = If(isAbove, positionY + yOffset, positionY - yOffset) * speed * stage.h * 0.6;
		
		var rotate = line.generic[3];
		var r = Power({dx * dx + dy * dy, 0.5});
		var angle = Arctan2(dy, dx);
		var newAngle = angle + rotate;
		var x = r * Cos(newAngle) + line.generic[1], y = r * Sin(newAngle) + line.generic[2];
		var x0 = dx * Cos(rotate) + line.generic[1], y0 = dx * Sin(rotate) + line.generic[2];
		
		var sprite = If(
			HasSkinSprite(Sprites.NormalFlick), 
			If(isMulti, Sprites.HLFlick, Sprites.NormalFlick),
			Sprites.FallbackNormalFlick
		);
		var ratio = If(
			HasSkinSprite(Sprites.NormalFlick),
			If(isMulti, hlFlickRatio, flickRatio),
			fallBackNoteRatio
		);
		var vec1Length = noteWidth * size 
			/ If(isMulti && HasSkinSprite(Sprites.NormalFlick), 1, 1)
			* If(HasSkinSprite(Sprites.NormalFlick), 1, 0.5), vec1X = vec1Length * Cos(rotate), vec1Y = vec1Length * Sin(rotate);
		var x1 = x - vec1X, y1 = y - vec1Y, x2 = x + vec1X, y2 = y + vec1Y;
		var vec2Length = vec1Length / ratio;
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
		// if (x3 >= 0 && x3 <= stage.r && y3 <= 0 && y3 >= stage.b) Debuglog(EntityInfo.generic[0]);
		
		Draw(
			sprite, 
			{ x3, y3 }, 
			{ x4, y4 }, 
			{ x5, y5 }, 
			{ x6, y6 }, 
			12000 + 1000 - time + info.index / 10000, 
			If(times.now > time, Max(1 - (times.now - time) / judgment.great, 0), 1) * alpha
		);
	}
};

class FakeFlickNote: public FlickNote {
	public:

	string name = "Phigros Fake Flick Note";
	bool hasInput = false;
};