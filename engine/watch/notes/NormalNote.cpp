Bucket NormalNoteBucket = defineBucket({
	EngineDataBucketSprite(Sprites.NormalNote, 0, 0.0, 0.0, 4.0, 2.0, 0)
}, Text.MillisecondUnit);

class NormalNote: public Archetype {
	public:

	string name = "Phigros Normal Note";
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
	defineImport(accuracy);
	defineImport(judgeResult);
	var positionY;
	var effectX1;
	var effectY1;
	var effectX2;
	var effectY2;
	var effectX3;
	var effectY3;
	var effectX4;
	var effectY4;
	var appearTime;
	var nextNote = var(EntitySharedMemoryId, 0); // 下一个按键信息
	var judgeTime = var(EntitySharedMemoryId, 1); // 判定时间
	var currentCombo = var(EntitySharedMemoryId, 2); // 当前 Combo 数
	var currentMaxCombo = var(EntitySharedMemoryId, 3); // 当前最大 Combo 数
	var currentJudgeStatus = var(EntitySharedMemoryId, 4); // 当前判定结果
	var currentAccScore = var(EntitySharedMemoryId, 5); // 当前准度得分
	var comboId = var(EntitySharedMemoryId, 6); // Combo 排行

	EntitySharedMemoryArrayGroup line = EntitySharedMemoryArray[judgeline];

	SonolusApi spawnTime() { return appearTime; }
	SonolusApi despawnTime() { return 
		If (
			isFake,
			time,
			If(replay,
				If(Abs(time + accuracy + 1) < 0.001, time + judgment.good, time + accuracy),
				time
			)
		);
	}

	int preprocessOrder = 514;
	SonolusApi preprocess() {
		time = time * timeMagic / bpm / levelSpeed;
		isMulti = isMulti && hasSimul;
		maxTime = Max(maxTime, time);
		if (mirror) positionX = -1 * positionX;
		// var id = EntityDataArray[judgeline].get(0);
		// WHILE (id) {
		// 	var deltaFloorPosition = Abs(floorPosition) - EntitySharedMemoryArray[id].get(1);
		// 	if (deltaFloorPosition <= 10 / 3 / speed * 5.85) BREAK;
		// 	appearTime = EntityDataArray[id].get(0) * timeMagic / bpm + (deltaFloorPosition - 10 / 3 / speed * 5.85) / EntityDataArray[id].get(2);
		// 	id = EntityDataArray[id].get(3);
		// } DONE
		// appearTime = Max(0, Min(appearTime, time - 0.5));
		if (!isFake) {
			notes = notes + 1;
			if (replay) {
				judgeTime = If(Abs(time + accuracy + 1) < 0.001, time + judgment.good, time + accuracy);
				if (judgeResult != 0 && hasSFX && !autoSFX) PlayScheduled(Clips.Note, time + accuracy, minSFXDistance);
				if (autoSFX && hasSFX) PlayScheduled(Clips.Note, time, minSFXDistance);
				Spawn(getAid(UpdateJudgment), {EntityInfo.index});
				input.time = time + accuracy;
				input.bucketIndex = int(NormalNoteBucket);
				input.bucketValue = accuracy * 1000;
			} else {
				judgeTime = time;
				if (hasSFX) PlayScheduled(Clips.Note, time, minSFXDistance); 
				Spawn(getAid(UpdateJudgment), {EntityInfo.index});
				input.time = 0;
				input.bucketIndex = int(NormalNoteBucket);
				input.bucketValue = 0;
			}
		}
	}

	// SonolusApi complete(let hitTime) {
	// 	FUNCBEGIN
	// 	if (Abs(hitTime - time) <= judgment.perfect) {
	// 		accscore = accscore + score.perfect;
	// 		judgeStatus = Min(judgeStatus, 2); combo = combo + 1;
	// 		SpawnParticleEffect(Effects.perfect, 
	// 			effectX1, effectY1, effectX2, effectY2,
	// 			effectX3, effectY3, effectX4, effectY4,
	// 			effectDurationTime);
	// 		EntityInput.set(0, 1); 
	// 		EntityInput.set(1, hitTime - time);
	// 		EntityInput.set(2, Buckets.note);
	// 		EntityInput.set(3, hitTime - time);
	// 	}
	// 	if (Abs(hitTime - time) > judgment.perfect && Abs(hitTime - time) <= judgment.great) {
	// 		accscore = accscore + score.great;
	// 		judgeStatus = Min(judgeStatus, 1); combo = combo + 1;
	// 		SpawnParticleEffect(Effects.great, 
	// 			effectX1, effectY1, effectX2, effectY2,
	// 			effectX3, effectY3, effectX4, effectY4,
	// 			effectDurationTime);
	// 		EntityInput.set(0, 2); 
	// 		EntityInput.set(1, hitTime - time);
	// 		EntityInput.set(2, Buckets.note);
	// 		EntityInput.set(3, hitTime - time);
	// 	}
	// 	if (Abs(hitTime - time) > judgment.great && Abs(hitTime - time) <= judgment.good) {
	// 		judgeStatus = Min(judgeStatus, 0); combo = 0;
	// 		EntityInput.set(0, 3); 
	// 		EntityInput.set(1, hitTime - time);
	// 		EntityInput.set(2, Buckets.note);
	// 		EntityInput.set(3, hitTime - time);
	// 	}
	// 	if (Abs(hitTime - time) > judgment.good) {
	// 		judgeStatus = Min(judgeStatus, 0); combo = 0;
	// 	}
	// 	EntityDespawn.set(0, 1);
	// 	return VOID;
	// }
	int updateSequentialOrder = 1919810;
	SonolusApi updateSequential() {
		if (times.now < 0) return;
		positionY = If(isAbove, floorPosition - line.generic[5], floorPosition + line.generic[5]);
	}

	SonolusApi terminate() {
		if (skip || isFake) return;
		if (replay && judgeResult == 0) return;
		// if (currentCombo == 10)
		// 	Debuglog(effectX1); Debuglog(effectY1);
		//
		SpawnParticleEffect(
			If(judgeResult == 2 || !replay, Effects.Perfect, Effects.Great), 
			{ effectX1, effectY1 }, 
			{ effectX2, effectY2 },
			{ effectX3, effectY3 }, 
			{ effectX4, effectY4 },
			effectDurationTime,
			false
		);
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
			HasSkinSprite(Sprites.NormalNote), 
			If(isMulti, Sprites.HLNote, Sprites.NormalNote),
			Sprites.FallbackNormalNote
		);
		var ratio = If(
			HasSkinSprite(Sprites.NormalNote),
			If(isMulti, hlNoteRatio, noteRatio),
			fallBackNoteRatio
		);
		var vec1Length = noteWidth * size 
			/ If(isMulti && HasSkinSprite(Sprites.NormalNote), 1, 1)
			* If(HasSkinSprite(Sprites.NormalNote), 1, 0.5), vec1X = vec1Length * Cos(rotate), vec1Y = vec1Length * Sin(rotate);
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
		
		Draw(
			sprite, 
			{ x3, y3 }, 
			{ x4, y4 }, 
			{ x5, y5 }, 
			{ x6, y6 }, 
			11000 + 1000 - time + info.index / 10000, 
			If(times.now > time, Max(1 - (times.now - time) / judgment.great, 0), 1) * alpha
		);
	}
};

class FakeNormalNote: public NormalNote {
	public:

	string name = "Phigros Fake Normal Note";
	bool hasInput = false;
};