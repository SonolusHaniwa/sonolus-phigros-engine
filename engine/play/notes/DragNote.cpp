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

	BlockPointer<EntitySharedMemoryArrayId> line = EntitySharedMemoryArray[judgeline];

	SonolusApi spawnOrder() { return 1; }
	SonolusApi shouldSpawn() { return true; }
	

	SonolusApi preprocess() {
		FUNCBEGIN
		played = false;
		inputTimeMax = time + judgment.good;
		inputTimeMin = time - judgment.good;
		return VOID;
	}

	SonolusApi complete(let hitTime) {
		FUNCBEGIN
		IF (Abs(hitTime - time) <= judgment.good) Play(Clips.Drag, minSFXDistance); FI
		IF (Abs(hitTime - time) <= judgment.good) {
			SpawnParticleEffect(Effects.perfect, 
				effectX1, effectY1, effectX2, effectY2,
				effectX3, effectY3, effectX4, effectY4,
				effectDurationTime, 0);
			EntityInput.set(0, 1); 
		} FI
		IF (Abs(hitTime - time) > judgment.good) EntityInput.set(0, 0); FI
		EntityInput.set(1, hitTime - time);
		EntityDespawn.set(0, 1);
		return VOID;
	}
	SonolusApi updateSequential() {
		FUNCBEGIN
		IF (times.now < 0) Return(0); FI
		IF (isAbove) positionY = floorPosition - line.get(5);
		ELSE positionY = floorPosition + line.get(5); FI

		// Claim
		IF (times.now < inputTimeMin) Return(0); FI
		IF (times.now > inputTimeMax) complete(-1); FI
		IF (played) {
			IF (times.now < time) Return(0); FI
			complete(times.now);
			Return(0);
		} FI
		return VOID;
	}

	SonolusApi touch() {
		FUNCBEGIN
		IF (played) Return(0); FI
		IF (times.now < inputTimeMin) Return(0); FI
		IF (hasTouch(EntityInfo.get(0))) played = 1; FI
		return VOID;
	}

	SonolusApi updateParallel() {
		FUNCBEGIN
		IF (times.now < 0) Return(0); FI
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
		effectX1 = x0 - noteWidth, effectY1 = y0 - noteWidth;
		effectX2 = x0 - noteWidth, effectY2 = y0 + noteWidth;
		effectX3 = x0 + noteWidth, effectY3 = y0 + noteWidth;
		effectX4 = x0 + noteWidth, effectY4 = y0 - noteWidth;
		
		Draw(If(isMulti, Sprites.HLDrag, Sprites.NormalDrag), x3, y3, x4, y4, x5, y5, x6, y6, 10000, If(times.now > time, Max(1 - (times.now - time) / judgment.great, 0), 1));
		return VOID;
	}
};