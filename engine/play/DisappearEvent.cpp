class DisappearEvent: public Archetype {
	public:

	static constexpr const char* name = "Phigros Judgeline Disappear Event";
	defineEntityData(startTime);
	defineEntityData(endTime);
	defineEntityData(start);
	defineEntityData(end);
	defineEntityData(next);

	SonolusApi spawnOrder() { return 0; }
	SonolusApi shouldSpawn() { return true; }

	SonolusApi preprocess() {
		FUNCBEGIN
		EntityDespawn.set(0, 1);
		return VOID;
	}
};
