class SpeedEvent: public Archetype {
	public:

	static constexpr const char* name = "Phigros Judgeline Speed Event";
	defineEntityData(startTime);
	defineEntityData(endTime);
	defineEntityData(value);
	defineEntityData(next);

	SonolusApi spawnOrder() { return 0; }
	SonolusApi shouldSpawn() { return true; }

	SonolusApi preprocess() {
		FUNCBEGIN
		EntityDespawn.set(0, 1);
		return VOID;
	}
};
