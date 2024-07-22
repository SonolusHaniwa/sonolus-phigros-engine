class MoveXEvent: public Archetype {
	public:

	static constexpr const char* name = "Phigros Judgeline Move X Event";
	defineImports(startTime);
	defineImports(endTime);
	defineImports(start);
	defineImports(end);
	defineImports(easing);
	defineImports(next);

	SonolusApi spawnOrder() { return 0; }
	SonolusApi shouldSpawn() { return true; }

	SonolusApi preprocess() {
		FUNCBEGIN
		startTime = startTime / levelSpeed;
		endTime = endTime / levelSpeed;
		EntityDespawn.set(0, 1);
		return VOID;
	}
};

class MoveYEvent: public Archetype {
	public:

	static constexpr const char* name = "Phigros Judgeline Move Y Event";
	defineImports(startTime);
	defineImports(endTime);
	defineImports(start);
	defineImports(end);
	defineImports(easing);
	defineImports(next);

	SonolusApi spawnOrder() { return 0; }
	SonolusApi shouldSpawn() { return true; }

	SonolusApi preprocess() {
		FUNCBEGIN
		startTime = startTime / levelSpeed;
		endTime = endTime / levelSpeed;
		EntityDespawn.set(0, 1);
		return VOID;
	}
};