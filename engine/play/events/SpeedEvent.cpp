class SpeedEvent: public Archetype {
	public:

	static constexpr const char* name = "Phigros Judgeline Speed Event";
	defineImports(startTime);
	defineImports(endTime);
	defineImports(value);
	defineImports(next);

	SonolusApi spawnOrder() { return 0; }
	SonolusApi shouldSpawn() { return true; }

	SonolusApi preprocess() {
		FUNCBEGIN
		startTime = startTime / levelSpeed;
		endTime = endTime / levelSpeed;
		value = value * levelSpeed;
		EntityDespawn.set(0, 1);
		return VOID;
	}
};
