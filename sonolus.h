#pragma GCC system_header

using namespace std;
string currentArchetype = "[Program Initialization]";
string currentCallback = "[Anonymous Callback]";

#include"modules/modules.h"

#include"items/Exception.h"
#include"items/EngineEnums.h"
#include"items/EngineConfiguration.h"
#include"items/EngineData.h"
#include"items/EngineTutorialData.h"
#include"items/EnginePreviewData.h"
#include"items/EngineWatchData.h"

EngineData engineData;
EngineTutorialData engineTutorialData;
EngineConfiguration engineConfiguration;
EnginePreviewData enginePreviewData;
EngineWatchData engineWatchData;

int globalCounter = 0, lastGlobalCounter = 0;
#define R e1e1d3d40573127e9ee0480caf1283d6
#include"blocks/FuncNode.h"
#include"blocks/CustomClass.h"
#include"blocks/Variable.h"
#include"blocks/BlockPointer.h"
#include"blocks/BlockArray.h"
#include"functions/functions.h"
#include"functions/redefine.h"
typedef Variable<10000> var;
typedef FuncNode let;

// Sonolus Api Function Definition
#define SonolusApi FuncNode
#define FUNCBEGIN createNodeContainer();
#define NONFUNCBEGIN createNodeContainer(false);
#define VAR mergeNodeContainer();
#define VOID e1e1d3d40573127e9ee0480caf1283d6(mergeNodeContainer()), 0;

#include"blocks/Archetype.h"
function<FuncNode()> tutorialPreprocess;
function<FuncNode()> tutorialNavigate;
function<FuncNode()> tutorialUpdate;
function<FuncNode()> engineWatchData_updateSpawn;
#include"blocks/Define.h"

// map<EngineDataNode, int> hashMap;
// 双哈希 + 手动哈希表 O(n)
// const int64_t k1 = 23;
// const int64_t k2 = 55331;
// const int64_t p1 = 1e9 + 7;
// const int64_t p2 = 998244353;
// struct hashMap {
//     int *map1, *map2;
//     hashMap() {
//         map1 = new int[1 << 20];
//         map2 = new int[1 << 20];
//         memset(map1, -1, sizeof(map1));
//         memset(map2, -1, sizeof(map2));
//     }
//     int64_t getHash(EngineDataNode v, bool hashType) {
//         string tmp = v.type;
//         if (v.type == "value") tmp += to_string(v.value.value);
//         else {
//             tmp += v.func.func;
//             for (int i = 0; i < v.func.args.size(); i++) tmp += to_string(v.func.args[i]);
//         } int64_t hash = 0;
//         for (int i = 0; i < tmp.size(); i++) 
//             hash *= hashType ? k1 : k2, hash %= hashType ? p1 : p2,
//             hash += tmp[i], hash %= hashType ? p1 : p2;
//         return hash;
//     }
//     void insert(EngineDataNode key, int value) {
//         int64_t hash1 = getHash(key, 0);
//         int64_t hash2 = getHash(key, 1);
//         hash1 %= (1 << 20); hash2 %= (1 << 20);
//         map1[hash1] = value; map2[hash2] = value;
//     }
//     int get(EngineDataNode key) {
//         int64_t hash1 = getHash(key, 0);
//         int64_t hash2 = getHash(key, 1);
//         hash1 %= (1 << 20); hash2 %= (1 << 20);
//         if (map1[hash1] != map2[hash2]) throw runtime_error("Hash Collision!");
//         return map1[hash1];
//     }
//     bool exist(EngineDataNode key) {
//         int64_t hash1 = getHash(key, 0);
//         int64_t hash2 = getHash(key, 1);
//         hash1 %= (1 << 20); hash2 %= (1 << 20);
//         if (map1[hash1] != map2[hash2]) throw runtime_error("Hash Collision!");
//         return map1[hash1] != -1;
//     }
// }hashMap;

/*template<typename T>
int buildScript(FuncNode script, T& nodesContainer, int blockCounter = 0) {
	globalCounter++;
//	if (hashMap.find(script.stringify(0, 1)) != hashMap.end()) return hashMap[script.stringify(0, 5)];
    EngineDataNode res;
    if (script.isValue == true) res = EngineDataValueNode(script.value);
    else {
        // Return 函数判断
        if (script.func == "Return") {
            script.func = "Break"; FuncNode code = script.args[0];
            script.args = {blockCounter, code};
        }
        // 其余函数
        vector<double> args;
        for (int i = 0; i < script.args.size(); i++) 
            args.push_back(buildScript(
                script.args[i],
                nodesContainer,
                blockCounter + (script.func == "Block")));
        res = EngineDataFunctionNode(script.func, args);
    } 
    // if (hashMap.exist(res)) return hashMap.get(res);
    // hashMap.insert(res, nodesContainer.nodes.size()); nodesContainer.nodes.push_back(res);
    // return hashMap.get(res);
    if (hashMap.find(res) != hashMap.end()) return hashMap[res];
    hashMap[res] = nodesContainer.nodes.size(); nodesContainer.nodes.push_back(res);
    return hashMap[res];
	hashMap[script.stringify(0, 1)] = nodesContainer.nodes.size(); nodesContainer.nodes.push_back(res);
	return hashMap[script.stringify(0, 1)];
}*/

/*int buildFuncNode(FuncNode func) {
    return buildScript(func, engineData);
}

int buildFuncNode2(FuncNode func) {
    return buildScript(func, engineTutorialData);
}

int buildFuncNode3(FuncNode func) {
    return buildScript(func, enginePreviewData);
}*/

time_t millitime() {
    return chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
}

vector<FuncNode> preloadElement;

#define compileCallback(name) { newArchetype.name.order = archetype.name##Order; \
	restoreAllocatorBackup(); currentCallback = #name; \
	createNodeContainer(); tmpres = archetype.name(); \
	if (nodesContainer.top().size() == 0) Return(tmpres); \
	newArchetype.name.index = mergeNodeContainer().getNodeId(); }

#define compileCallbackHead(name) { newArchetype.name.order = archetype.name##Order; \
	restoreAllocatorBackup(); currentCallback = #name; \
	createNodeContainer(); nodesContainer.top() = preloadElement; tmpres = archetype.name(); \
	if (nodesContainer.top().size() == preloadElement.size()) Return(tmpres); \
	newArchetype.name.index = mergeNodeContainer().getNodeId(); }

template<typename T, typename... Args> 
void buildArchetype() {
	lastGlobalCounter = globalCounter;
	FuncNode tmpres; 
	if (!T::disableGlobalPreprocess) restoreAllocatorBackup();
	else memset(allocatorSize, 0, sizeof allocatorSize);
	currentArchetype = T::name; currentCallback = "[Anonymous Callback]";
	T archetype = T();
	createAllocatorBackup();
    #ifdef play
    time_t st = millitime();
    EngineDataArchetype newArchetype;
    cout << "Solving Archetype \"" << archetype.name << "\"..." << endl;
    newArchetype.name = archetype.name;
    newArchetype.hasInput = archetype.hasInput;
	if (!T::disableGlobalPreprocess) { compileCallbackHead(preprocess); }
	else { compileCallback(preprocess); }
    compileCallback(spawnOrder);
    compileCallback(shouldSpawn);
    compileCallback(initialize);
    compileCallback(updateSequential);
    compileCallback(touch);
    compileCallback(updateParallel);
    compileCallback(terminate);
    newArchetype.imports = archetype.imports;
    newArchetype.exports = archetype.exports;
    engineData.archetypes.push_back(newArchetype);
    time_t d = millitime() - st;
    cout << "Solved Archetype \"" << archetype.name << "\" in " << d << "ms. Speed: " 
		 << fixed << setprecision(0) 
         << 1.0 * (globalCounter - lastGlobalCounter) / (1.0 * d / 1000) << " nodes/s. Total: " 
		 << (globalCounter - lastGlobalCounter) << " nodes." << endl;
    #elif preview
    time_t st = millitime();
    EnginePreviewDataArchetype newArchetype;
    cout << "Solving Archetype \"" << archetype.name << "\"..." << endl;
    newArchetype.name = archetype.name;
	if (!T::disableGlobalPreprocess) { compileCallbackHead(preprocess); }
	else { compileCallback(preprocess); }
    compileCallback(render);
    newArchetype.imports = archetype.imports;
    enginePreviewData.archetypes.push_back(newArchetype);
    time_t d = millitime() - st;
    cout << "Solved Archetype \"" << archetype.name << "\" in " << d << "ms. Speed: " 
		 << fixed << setprecision(0)
         << 1.0 * (globalCounter - lastGlobalCounter) / (1.0 * d / 1000) << " nodes/s. Total: " 
		 << (globalCounter - lastGlobalCounter) << " nodes." << endl;
	#elif watch
	time_t st = millitime();
    EngineWatchDataArchetype newArchetype;
    cout << "Solving Archetype \"" << archetype.name << "\"..." << endl;
    newArchetype.name = archetype.name;
    newArchetype.hasInput = archetype.hasInput;
	if (!T::disableGlobalPreprocess) { compileCallbackHead(preprocess); }
	else { compileCallback(preprocess); }
    compileCallback(spawnTime);
    compileCallback(despawnTime);
    compileCallback(initialize);
    compileCallback(updateSequential);
    compileCallback(updateParallel);
    compileCallback(terminate);
    newArchetype.imports = archetype.imports;
    engineWatchData.archetypes.push_back(newArchetype);
    time_t d = millitime() - st;
    cout << "Solved Archetype \"" << archetype.name << "\" in " << d << "ms. Speed: " 
   		 << fixed << setprecision(0)
         << 1.0 * (globalCounter - lastGlobalCounter) / (1.0 * d / 1000) << " nodes/s. Total: " 
  		 << (globalCounter - lastGlobalCounter) << " nodes." << endl;
  	#endif
  	deleteAllocatorBackup();
    if constexpr (sizeof...(Args)) buildArchetype<Args...>();
}

int allocatedArchetypeCount = 0;
map<string, int> archetypeId;
template<typename T, typename... Args>
void allocateArchetypeId() {
	archetypeId[typeid(T).name()] = allocatedArchetypeCount++;
	if constexpr (sizeof...(Args)) allocateArchetypeId<Args...>();
}

template<typename... Args>
void build(buffer& configurationBuffer, buffer& dataBuffer) {
	Run(1 + 1 == 2); // 防止空函数爆炸
	preloadElement = nodesContainer.top(); mergeNodeContainer();
	createAllocatorBackup();
    Json::Value configuration = engineConfiguration.toJsonObject();
    configurationBuffer = compress_gzip(json_encode(configuration));
#ifdef play
	allocateArchetypeId<Args...>();
    buildArchetype<Args...>();
	engineData.nodes = dataContainer;
    dataBuffer = compress_gzip(json_encode(engineData.toJsonObject()));
#elif tutorial
    cout << "Solving Archetype \"Sonolus Tutorial Default\"..." << endl;
    time_t st = millitime();
    engineTutorialData.preprocess = Block(tutorialPreprocess()).getNodeId();
    engineTutorialData.navigate = Block(tutorialNavigate()).getNodeId();
    engineTutorialData.update = Block(tutorialUpdate()).getNodeId();
	engineTutorialData.nodes = dataContainer;
    time_t d = millitime() - st;
    cout << "Solved Archetype \"Sonolus Tutorial Default\" in " << d << "ms. Speed: " 
		 << fixed << setprecision(0)
         << 1.0 * (globalCounter - lastGlobalCounter) / (1.0 * d / 1000) << " nodes/s. Total: " 
		 << (globalCounter - lastGlobalCounter) << " nodes." << endl;
    dataBuffer = compress_gzip(json_encode(engineTutorialData.toJsonObject()));
#elif preview
	allocateArchetypeId<Args...>();
    buildArchetype<Args...>();
	enginePreviewData.nodes = dataContainer;
    dataBuffer = compress_gzip(json_encode(enginePreviewData.toJsonObject()));
#elif watch
	allocateArchetypeId<Args...>();
    buildArchetype<Args...>();
    createNodeContainer(); auto tmpres = engineWatchData_updateSpawn();
	if (nodesContainer.top().size() == 0) Return(tmpres);
	engineWatchData.updateSpawn = mergeNodeContainer().getNodeId();
	engineWatchData.nodes = dataContainer;
    dataBuffer = compress_gzip(json_encode(engineWatchData.toJsonObject()));
#endif
}

#define getArchetypeId(T) archetypeId[typeid(T).name()]

// int ForPtIterator = 0;
#define IF(cond) e1e1d3d40573127e9ee0480caf1283d6(If(cond, [&](){ NONFUNCBEGIN
#define ELSE return VAR; }(), [&](){ NONFUNCBEGIN
#define FI return VAR; }()));
#define FOR(i, st, en, step) [&](){ \
	int ForBlockCount = blockCounter.top(); \
	NONFUNCBEGIN var i = st - step; \
	While(i < en - step, [&](){ \
		NONFUNCBEGIN i.set(i + step);
#define WHILE(cond) [&](){ \
	int ForBlockCount = blockCounter.top(); \
	NONFUNCBEGIN \
	While(cond, [&](){ \
		NONFUNCBEGIN
#define DONE return VAR; }()); return VOID; \
}();
    // auto res = __builtin_Block(Execute({\
    //     ForPt[ForPtIterator].set(st - step), \
    //     While(ForPt[ForPtIterator].get() < en - step, __builtin_Block(Execute({\
    //         ForPt[ForPtIterator].set(ForPt[ForPtIterator].get() + step), \
    //         Execute(
// #define DONE )}))) \
//     })); \
// 	blockCounter -= 2; ForPtIterator--; \
// 	return res; \
// }()
#define CONTINUE Break(blockCounter.top() - ForBlockCount - 1, 0)
#define BREAK Break(blockCounter.top() - ForBlockCount, 0)

#include"blocks/Array.h"
#include"blocks/Map.h"
#include"items/PlayData.h"
// #include"items/TutorialData.h"
#include"items/PreviewData.h"
#include"items/WatchData.h"
#include"items/SkinData.h"
#include"items/EffectData.h"

#undef R
