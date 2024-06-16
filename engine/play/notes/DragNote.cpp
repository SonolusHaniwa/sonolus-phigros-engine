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

	BlockPointer<EntitySharedMemoryArrayId> line = EntitySharedMemoryArray[judgeline];

	SonolusApi spawnOrder() { return 1; }
	SonolusApi shouldSpawn() { return true; }
	

	SonolusApi preprocess() {
		FUNCBEGIN
		played = false;
		return VOID;
	}

	SonolusApi complete(let time) {
		FUNCBEGIN
		Play(Clips.Drag, minSFXDistance);
		SpawnParticleEffect(Effects.perfect, 
			effectX1, effectY1, effectX2, effectY2,
			effectX3, effectY3, effectX4, effectY4,
			effectDurationTime, 0);
		return VOID;
	}
	SonolusApi updateSequential() {
		FUNCBEGIN
		IF (times.now < 0) Return(0); FI
		IF (times.now > time) complete(times.now); EntityDespawn.set(0, 1); FI
		IF (isAbove) positionY = floorPosition - line.get(5);
		ELSE positionY = floorPosition + line.get(5); FI
		return VOID;
	}

	SonolusApi updateParallel() {
		FUNCBEGIN
		IF (times.now < 0) Return(0); FI
		var dx = positionX * stage.w * 0.05625;
		var dy = positionY * speed * stage.h * 0.6;
		var dy2 = dy + speed * holdTime * If(isAbove, 1, -1) * stage.h * 0.6;
		dy = If(isAbove, Max(0, dy), Min(0, dy));
		dy2 = If(isAbove, Max(0, dy2), Min(0, dy2));
		// dy2 = If(floorPosition > 0, Min(stage.w, dy2), Max(stage.w, dy2));
		
		var rotate = line.get(3);
		var r = Power({dx * dx + dy * dy, 0.5});
		var hr = Power({dx * dx + dy2 * dy2, 0.5});
		var angle = Arctan2(dy, dx);
		var hangle = Arctan2(dy2, dx);
		var newAngle = angle + rotate;
		var hnewAngle = hangle + rotate;
		var x = r * Cos(newAngle) + line.get(1), y = r * Sin(newAngle) + line.get(2);
		var hx = hr * Cos(hnewAngle) + line.get(1), hy = hr * Sin(hnewAngle) + line.get(2);
		
		var vec1Length = noteWidth, vec1X = vec1Length * Cos(rotate), vec1Y = vec1Length * Sin(rotate);
		var x1 = x - vec1X, y1 = y - vec1Y, x2 = x + vec1X, y2 = y + vec1Y;
		var hx1 = hx - vec1X, hy1 = hy - vec1Y, hx2 = hx + vec1X, hy2 = hy + vec1Y;
		var vec2Length = noteHeight;
		var vec2X = vec2Length * Cos(rotate + PI / 2), vec2Y = vec2Length * Sin(rotate + PI / 2);
		var x3 = x1 - vec2X, y3 = y1 - vec2Y;
		var x4 = x1 + vec2X, y4 = y1 + vec2Y;
		var x5 = x2 + vec2X, y5 = y2 + vec2Y;
		var x6 = x2 - vec2X, y6 = y2 - vec2Y;
		var hx3 = x1, hy3 = y1;
		var hx4 = hx1, hy4 = hy1;
		var hx5 = hx2, hy5 = hy2;
		var hx6 = x2, hy6 = y2;
		// 粒子效果不用转
		effectX1 = x - noteWidth, effectY1 = y - noteWidth;
		effectX2 = x - noteWidth, effectY2 = y + noteWidth;
		effectX3 = x + noteWidth, effectY3 = y + noteWidth;
		effectX4 = x + noteWidth, effectY4 = y - noteWidth;
		
		Draw(If(isMulti, Sprites.HLDrag, Sprites.NormalDrag), x3, y3, x4, y4, x5, y5, x6, y6, 10000, 1);
		return VOID;
	}
};