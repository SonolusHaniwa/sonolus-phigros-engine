class Initialization: public Archetype {
    public:

    static constexpr const char* name = "Phigros Initialization";

    SonolusApi spawnOrder() { return 0; }
    SonolusApi shouldSpawn() { return true; }
};