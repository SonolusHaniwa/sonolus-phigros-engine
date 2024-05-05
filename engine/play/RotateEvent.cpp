class RotateEvent: public Archetype {
	public:

	static constexpr const char* name = "Phigros Judgeline Rotate Event";
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
		EntityDespawn.set(0, 1);
		return VOID;
	}
};
