using namespace std;
#ifdef play
class Archetype {
    public:

	static const bool disableGlobalPreprocess = false;
	static constexpr const char* name = "Default Archetype";
    bool hasInput = false;

    int preprocessOrder = 0;
    SonolusApi preprocess() {
    	FUNCBEGIN
    	Return(0);
    	return VOID;
    }

    int spawnOrderOrder = 0;
	SonolusApi spawnOrder() {
    	FUNCBEGIN
    	Return(0);
    	return VOID;
	}

    int shouldSpawnOrder = 0;
	SonolusApi shouldSpawn() {
    	FUNCBEGIN
    	Return(0);
    	return VOID;
	}

    int initializeOrder = 0;
	SonolusApi initialize() {
    	FUNCBEGIN
    	Return(0);
    	return VOID;
	}

    int updateSequentialOrder = 0;
	SonolusApi updateSequential() {
    	FUNCBEGIN
    	Return(0);
    	return VOID;
	}

    int touchOrder = 0;
	SonolusApi touch() {
    	FUNCBEGIN
    	Return(0);
    	return VOID;
	}

    int updateParallelOrder = 0;
	SonolusApi updateParallel() {
    	FUNCBEGIN
    	Return(0);
    	return VOID;
	}

    int terminateOrder = 0;
	SonolusApi terminate() {
    	FUNCBEGIN
    	Return(0);
    	return VOID;
	}

    vector<pair<string, int> > imports;
    vector<string> exports;
};
#elif preview
class Archetype {
    public:

	static const bool disableGlobalPreprocess = false;
    static constexpr const char* name = "Default Archtype";

    int preprocessOrder = 0;
	SonolusApi preprocess() {
    	FUNCBEGIN
    	Return(0);
    	return VOID;
	}

    int renderOrder = 0;
	SonolusApi render() {
    	FUNCBEGIN
    	Return(0);
    	return VOID;
	}

    vector<pair<string, int> > imports;
};
#elif watch
class Archetype {
    public:

	static const bool disableGlobalPreprocess = false;
    static constexpr const char* name = "Default Archtype";
    bool hasInput = false;

    int preprocessOrder = 0;
	SonolusApi preprocess() {
    	FUNCBEGIN
    	Return(0);
    	return VOID;
	}

    int spawnTimeOrder = 0;
	SonolusApi spawnTime() {
    	FUNCBEGIN
    	Return(0);
    	return VOID;
	}

    int despawnTimeOrder = 0;
	SonolusApi despawnTime() {
    	FUNCBEGIN
    	Return(0);
    	return VOID;
	}

    int initializeOrder = 0;
	SonolusApi initialize() {
    	FUNCBEGIN
    	Return(0);
    	return VOID;
	}

    int updateSequentialOrder = 0;
	SonolusApi updateSequential() {
    	FUNCBEGIN
    	Return(0);
    	return VOID;
	}

    int updateParallelOrder = 0;
	SonolusApi updateParallel() {
    	FUNCBEGIN
    	Return(0);
    	return VOID;
	}

    int terminateOrder = 0;
	SonolusApi terminate() {
    	FUNCBEGIN
    	Return(0);
    	return VOID;
	}

    vector<pair<string, int> > imports;
};
#endif

#define defineImports(name) Variable<EntityDataId> name = Variable<EntityDataId>(0, true); \
	bool unused_##name##_unused = [&](){ \
		name.offset = imports.size(); allocatorSize[EntityDataId]++; \
		imports.push_back({ #name, imports.size() }); return true; }(); 
#define defineExports(name) int name = [&](){ \
		exports.push_back(#name); return exports.size() - 1; }();