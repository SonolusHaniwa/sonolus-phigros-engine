class MoveEvent: public Archetype {
	public:

	static constexpr const char* name = "Phigros Judgeline Move Event";
	defineEntityData(startTime);
	defineEntityData(endTime);
	defineEntityData(start);
	defineEntityData(end);
	defineEntityData(start2);
	defineEntityData(end2);
	defineEntityData(next);

	SonolusApi spawnOrder() { return 0; }
	SonolusApi shouldSpawn() { return true; }

	SonolusApi preprocess() {
		FUNCBEGIN
		EntityDespawn.set(0, 1);
		return VOID;
	}
};
