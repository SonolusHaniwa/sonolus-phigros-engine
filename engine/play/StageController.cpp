class StageController: public Archetype {
    public:

    static constexpr const char* name = "Phigros Stage Controller";
    bool hasInput = false;

	SonolusApi spawnOrder() { return 1; }
	SonolusApi shouldSpawn() { return 1; }
};