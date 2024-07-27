using namespace std;

namespace watchData {
	const int preprocessId = 0;
	const int spawnTimeId = 1;
	const int despawnTimeId = 2;
	const int initializeId = 3;
	const int updateSequentialId = 4;
	const int updateParallelId = 5;
	const int terminateId = 6;
	const int updateSpawnId = 7;
	const int RuntimeEnvironmentId = 1000;
	const int RuntimeUpdateId = 1001;
	const int RuntimeSkinTransformId = 1002;
	const int RuntimeParticleTransformId = 1003;
	const int RuntimeBackgroundId = 1004;
	const int RuntimeUIId = 1005;
	const int RuntimeUIConfigurationId = 1006;
	const int LevelMemoryId = 2000;
	const int LevelDataId = 2001;
	const int LevelOptionId = 2002;
    const int LevelBucketId = 2003;
	const int LevelScoreId = 2004;
	const int LevelLifeId = 2005;
	const int EngineRomId = 3000;
	const int EntityMemoryId = 4000;
	const int EntityDataId = 4001;
	const int EntitySharedMemoryId = 4002;
	const int EntityInfoId = 4003;
	const int EntityInputId = 4004;
	const int EntityDataArrayId = 4101;
	const int EntitySharedMemoryArrayId = 4102;
	const int EntityInfoArrayId = 4103;
	const int ArchetypeLifeId = 5000;
	const int TemporaryMemoryId = 10000;
	
	BlockPointer<RuntimeEnvironmentId> RuntimeEnvironment;
	BlockPointer<RuntimeUpdateId> RuntimeUpdate;
	BlockPointer<RuntimeSkinTransformId> RuntimeSkinTransform;
	BlockPointer<RuntimeParticleTransformId> RuntimeParticleTransform;
	BlockPointer<RuntimeBackgroundId> RuntimeBackground;
	BlockPointer<RuntimeUIId> RuntimeUI;
	BlockPointer<RuntimeUIConfigurationId> RuntimeUIConfiguration;
	BlockPointer<LevelMemoryId> LevelMemory;
	BlockPointer<LevelDataId> LevelData;
	BlockPointer<LevelOptionId> LevelOption;
	BlockPointer<LevelBucketId> LevelBucket;
	BlockPointer<LevelScoreId> LevelScore;
	BlockPointer<LevelLifeId> LevelLife;
	BlockPointer<EngineRomId> EngineRom;
	BlockPointer<EntityMemoryId> EntityMemory;
	BlockPointer<EntityDataId> EntityData;
	BlockPointer<EntitySharedMemoryId> EntitySharedMemory;
	BlockPointer<EntityInfoId> EntityInfo;
	BlockPointer<EntityInputId> EntityInput;
	BlockArray<EntityDataArrayId> EntityDataArray(32);
	BlockArray<EntitySharedMemoryArrayId> EntitySharedMemoryArray(32);
	BlockArray<EntityInfoArrayId> EntityInfoArray(3);
	BlockPointer<ArchetypeLifeId> ArchetypeLife;
	BlockPointer<TemporaryMemoryId> TemporaryMemory;
	
	bool readMap[10001][8]; 
	bool writeMap[10001][8];
	bool initializeReadWriteTable = [](){
		memset(readMap, 0, sizeof readMap); memset(writeMap, 0, sizeof writeMap);
		writeMap[RuntimeEnvironmentId][preprocessId] = 1; writeMap[RuntimeEnvironmentId][spawnTimeId] = 1; writeMap[RuntimeEnvironmentId][despawnTimeId] = 1; writeMap[RuntimeEnvironmentId][initializeId] = 1; writeMap[RuntimeEnvironmentId][updateSequentialId] = 1; writeMap[RuntimeEnvironmentId][updateParallelId] = 1; writeMap[RuntimeEnvironmentId][terminateId] = 1; writeMap[RuntimeEnvironmentId][updateSpawnId] = 1; 
		readMap[RuntimeEnvironmentId][preprocessId] = 1; 
		writeMap[RuntimeUpdateId][preprocessId] = 1; writeMap[RuntimeUpdateId][spawnTimeId] = 1; writeMap[RuntimeUpdateId][despawnTimeId] = 1; writeMap[RuntimeUpdateId][initializeId] = 1; writeMap[RuntimeUpdateId][updateSequentialId] = 1; writeMap[RuntimeUpdateId][updateParallelId] = 1; writeMap[RuntimeUpdateId][terminateId] = 1; writeMap[RuntimeUpdateId][updateSpawnId] = 1; 
		writeMap[RuntimeSkinTransformId][preprocessId] = 1; writeMap[RuntimeSkinTransformId][spawnTimeId] = 1; writeMap[RuntimeSkinTransformId][despawnTimeId] = 1; writeMap[RuntimeSkinTransformId][initializeId] = 1; writeMap[RuntimeSkinTransformId][updateSequentialId] = 1; writeMap[RuntimeSkinTransformId][updateParallelId] = 1; writeMap[RuntimeSkinTransformId][terminateId] = 1; writeMap[RuntimeSkinTransformId][updateSpawnId] = 1; 
		readMap[RuntimeSkinTransformId][preprocessId] = 1; readMap[RuntimeSkinTransformId][updateSequentialId] = 1; 
		writeMap[RuntimeParticleTransformId][preprocessId] = 1; writeMap[RuntimeParticleTransformId][spawnTimeId] = 1; writeMap[RuntimeParticleTransformId][despawnTimeId] = 1; writeMap[RuntimeParticleTransformId][initializeId] = 1; writeMap[RuntimeParticleTransformId][updateSequentialId] = 1; writeMap[RuntimeParticleTransformId][updateParallelId] = 1; writeMap[RuntimeParticleTransformId][terminateId] = 1; writeMap[RuntimeParticleTransformId][updateSpawnId] = 1; 
		readMap[RuntimeParticleTransformId][preprocessId] = 1; readMap[RuntimeParticleTransformId][updateSequentialId] = 1; 
		writeMap[RuntimeBackgroundId][preprocessId] = 1; writeMap[RuntimeBackgroundId][spawnTimeId] = 1; writeMap[RuntimeBackgroundId][despawnTimeId] = 1; writeMap[RuntimeBackgroundId][initializeId] = 1; writeMap[RuntimeBackgroundId][updateSequentialId] = 1; writeMap[RuntimeBackgroundId][updateParallelId] = 1; writeMap[RuntimeBackgroundId][terminateId] = 1; writeMap[RuntimeBackgroundId][updateSpawnId] = 1; 
		readMap[RuntimeBackgroundId][preprocessId] = 1; readMap[RuntimeBackgroundId][updateSequentialId] = 1; 
		writeMap[RuntimeUIId][preprocessId] = 1; writeMap[RuntimeUIId][spawnTimeId] = 1; writeMap[RuntimeUIId][despawnTimeId] = 1; writeMap[RuntimeUIId][initializeId] = 1; writeMap[RuntimeUIId][updateSequentialId] = 1; writeMap[RuntimeUIId][updateParallelId] = 1; writeMap[RuntimeUIId][terminateId] = 1; writeMap[RuntimeUIId][updateSpawnId] = 1; 
		readMap[RuntimeUIId][preprocessId] = 1; 
		writeMap[RuntimeUIConfigurationId][preprocessId] = 1; writeMap[RuntimeUIConfigurationId][spawnTimeId] = 1; writeMap[RuntimeUIConfigurationId][despawnTimeId] = 1; writeMap[RuntimeUIConfigurationId][initializeId] = 1; writeMap[RuntimeUIConfigurationId][updateSequentialId] = 1; writeMap[RuntimeUIConfigurationId][updateParallelId] = 1; writeMap[RuntimeUIConfigurationId][terminateId] = 1; writeMap[RuntimeUIConfigurationId][updateSpawnId] = 1; 
		readMap[RuntimeUIConfigurationId][preprocessId] = 1; 
		writeMap[LevelMemoryId][preprocessId] = 1; writeMap[LevelMemoryId][spawnTimeId] = 1; writeMap[LevelMemoryId][despawnTimeId] = 1; writeMap[LevelMemoryId][initializeId] = 1; writeMap[LevelMemoryId][updateSequentialId] = 1; writeMap[LevelMemoryId][updateParallelId] = 1; writeMap[LevelMemoryId][terminateId] = 1; writeMap[LevelMemoryId][updateSpawnId] = 1; 
		readMap[LevelMemoryId][preprocessId] = 1; readMap[LevelMemoryId][updateSequentialId] = 1; 
		writeMap[LevelDataId][preprocessId] = 1; writeMap[LevelDataId][spawnTimeId] = 1; writeMap[LevelDataId][despawnTimeId] = 1; writeMap[LevelDataId][initializeId] = 1; writeMap[LevelDataId][updateSequentialId] = 1; writeMap[LevelDataId][updateParallelId] = 1; writeMap[LevelDataId][terminateId] = 1; writeMap[LevelDataId][updateSpawnId] = 1; 
		readMap[LevelDataId][preprocessId] = 1; 
		writeMap[LevelOptionId][preprocessId] = 1; writeMap[LevelOptionId][spawnTimeId] = 1; writeMap[LevelOptionId][despawnTimeId] = 1; writeMap[LevelOptionId][initializeId] = 1; writeMap[LevelOptionId][updateSequentialId] = 1; writeMap[LevelOptionId][updateParallelId] = 1; writeMap[LevelOptionId][terminateId] = 1; writeMap[LevelOptionId][updateSpawnId] = 1; 
		writeMap[LevelScoreId][preprocessId] = 1; writeMap[LevelScoreId][spawnTimeId] = 1; writeMap[LevelScoreId][despawnTimeId] = 1; writeMap[LevelScoreId][initializeId] = 1; writeMap[LevelScoreId][updateSequentialId] = 1; writeMap[LevelScoreId][updateParallelId] = 1; writeMap[LevelScoreId][terminateId] = 1; writeMap[LevelScoreId][updateSpawnId] = 1; 
		readMap[LevelScoreId][preprocessId] = 1; 
		writeMap[LevelLifeId][preprocessId] = 1; writeMap[LevelLifeId][spawnTimeId] = 1; writeMap[LevelLifeId][despawnTimeId] = 1; writeMap[LevelLifeId][initializeId] = 1; writeMap[LevelLifeId][updateSequentialId] = 1; writeMap[LevelLifeId][updateParallelId] = 1; writeMap[LevelLifeId][terminateId] = 1; writeMap[LevelLifeId][updateSpawnId] = 1; 
		readMap[LevelLifeId][preprocessId] = 1; 
		writeMap[EngineRomId][preprocessId] = 1; writeMap[EngineRomId][spawnTimeId] = 1; writeMap[EngineRomId][despawnTimeId] = 1; writeMap[EngineRomId][initializeId] = 1; writeMap[EngineRomId][updateSequentialId] = 1; writeMap[EngineRomId][updateParallelId] = 1; writeMap[EngineRomId][terminateId] = 1; writeMap[EngineRomId][updateSpawnId] = 1; 
		writeMap[EntityMemoryId][preprocessId] = 1; writeMap[EntityMemoryId][spawnTimeId] = 1; writeMap[EntityMemoryId][despawnTimeId] = 1; writeMap[EntityMemoryId][initializeId] = 1; writeMap[EntityMemoryId][updateSequentialId] = 1; writeMap[EntityMemoryId][updateParallelId] = 1; writeMap[EntityMemoryId][terminateId] = 1; 
		readMap[EntityMemoryId][preprocessId] = 1; readMap[EntityMemoryId][spawnTimeId] = 1; readMap[EntityMemoryId][despawnTimeId] = 1; readMap[EntityMemoryId][initializeId] = 1; readMap[EntityMemoryId][updateSequentialId] = 1; readMap[EntityMemoryId][updateParallelId] = 1; readMap[EntityMemoryId][terminateId] = 1; 
		writeMap[EntityDataId][preprocessId] = 1; writeMap[EntityDataId][spawnTimeId] = 1; writeMap[EntityDataId][despawnTimeId] = 1; writeMap[EntityDataId][initializeId] = 1; writeMap[EntityDataId][updateSequentialId] = 1; writeMap[EntityDataId][updateParallelId] = 1; writeMap[EntityDataId][terminateId] = 1; 
		readMap[EntityDataId][preprocessId] = 1; 
		writeMap[EntitySharedMemoryId][preprocessId] = 1; writeMap[EntitySharedMemoryId][spawnTimeId] = 1; writeMap[EntitySharedMemoryId][despawnTimeId] = 1; writeMap[EntitySharedMemoryId][initializeId] = 1; writeMap[EntitySharedMemoryId][updateSequentialId] = 1; writeMap[EntitySharedMemoryId][updateParallelId] = 1; writeMap[EntitySharedMemoryId][terminateId] = 1; 
		readMap[EntitySharedMemoryId][preprocessId] = 1; readMap[EntitySharedMemoryId][updateSequentialId] = 1; 
		writeMap[EntityInfoId][preprocessId] = 1; writeMap[EntityInfoId][spawnTimeId] = 1; writeMap[EntityInfoId][despawnTimeId] = 1; writeMap[EntityInfoId][initializeId] = 1; writeMap[EntityInfoId][updateSequentialId] = 1; writeMap[EntityInfoId][updateParallelId] = 1; writeMap[EntityInfoId][terminateId] = 1; 
		writeMap[EntityInputId][preprocessId] = 1; writeMap[EntityInputId][spawnTimeId] = 1; writeMap[EntityInputId][despawnTimeId] = 1; writeMap[EntityInputId][initializeId] = 1; writeMap[EntityInputId][updateSequentialId] = 1; writeMap[EntityInputId][updateParallelId] = 1; writeMap[EntityInputId][terminateId] = 1; 
		readMap[EntityInputId][preprocessId] = 1; 
		writeMap[EntityDataArrayId][preprocessId] = 1; writeMap[EntityDataArrayId][spawnTimeId] = 1; writeMap[EntityDataArrayId][despawnTimeId] = 1; writeMap[EntityDataArrayId][initializeId] = 1; writeMap[EntityDataArrayId][updateSequentialId] = 1; writeMap[EntityDataArrayId][updateParallelId] = 1; writeMap[EntityDataArrayId][terminateId] = 1; writeMap[EntityDataArrayId][updateSpawnId] = 1; 
		readMap[EntityDataArrayId][preprocessId] = 1; 
		writeMap[EntitySharedMemoryArrayId][preprocessId] = 1; writeMap[EntitySharedMemoryArrayId][spawnTimeId] = 1; writeMap[EntitySharedMemoryArrayId][despawnTimeId] = 1; writeMap[EntitySharedMemoryArrayId][initializeId] = 1; writeMap[EntitySharedMemoryArrayId][updateSequentialId] = 1; writeMap[EntitySharedMemoryArrayId][updateParallelId] = 1; writeMap[EntitySharedMemoryArrayId][terminateId] = 1; writeMap[EntitySharedMemoryArrayId][updateSpawnId] = 1; 
		readMap[EntitySharedMemoryArrayId][preprocessId] = 1; readMap[EntitySharedMemoryArrayId][updateSequentialId] = 1; 
		writeMap[EntityInfoArrayId][preprocessId] = 1; writeMap[EntityInfoArrayId][spawnTimeId] = 1; writeMap[EntityInfoArrayId][despawnTimeId] = 1; writeMap[EntityInfoArrayId][initializeId] = 1; writeMap[EntityInfoArrayId][updateSequentialId] = 1; writeMap[EntityInfoArrayId][updateParallelId] = 1; writeMap[EntityInfoArrayId][terminateId] = 1; writeMap[EntityInfoArrayId][updateSpawnId] = 1; 
		writeMap[ArchetypeLifeId][preprocessId] = 1; writeMap[ArchetypeLifeId][spawnTimeId] = 1; writeMap[ArchetypeLifeId][despawnTimeId] = 1; writeMap[ArchetypeLifeId][initializeId] = 1; writeMap[ArchetypeLifeId][updateSequentialId] = 1; writeMap[ArchetypeLifeId][updateParallelId] = 1; writeMap[ArchetypeLifeId][terminateId] = 1; writeMap[ArchetypeLifeId][updateSpawnId] = 1; 
		readMap[ArchetypeLifeId][preprocessId] = 1; 
		writeMap[TemporaryMemoryId][preprocessId] = 1; writeMap[TemporaryMemoryId][spawnTimeId] = 1; writeMap[TemporaryMemoryId][despawnTimeId] = 1; writeMap[TemporaryMemoryId][initializeId] = 1; writeMap[TemporaryMemoryId][updateSequentialId] = 1; writeMap[TemporaryMemoryId][updateParallelId] = 1; writeMap[TemporaryMemoryId][terminateId] = 1; writeMap[TemporaryMemoryId][updateSpawnId] = 1; 
		readMap[TemporaryMemoryId][preprocessId] = 1; readMap[TemporaryMemoryId][spawnTimeId] = 1; readMap[TemporaryMemoryId][despawnTimeId] = 1; readMap[TemporaryMemoryId][initializeId] = 1; readMap[TemporaryMemoryId][updateSequentialId] = 1; readMap[TemporaryMemoryId][updateParallelId] = 1; readMap[TemporaryMemoryId][terminateId] = 1; readMap[TemporaryMemoryId][updateSpawnId] = 1; 
		return true;
	}();

	class screen {
        public:

        FuncNode aspectRatio = RuntimeEnvironment.get(1);
        FuncNode l = Multiply({-1, aspectRatio});
        FuncNode b = -1;
        FuncNode r = aspectRatio;
        FuncNode t = 1;
        FuncNode w = Multiply({aspectRatio, 2});
        FuncNode h = 2;
    }screen;

    class ui {
        public:

        class layout {
            public: 

            FuncNode offset = 0;
            layout(){};
            layout(FuncNode offset):offset(offset){};

            FuncNode anchorX = RuntimeUI.get(Add({Multiply({offset, 10}), 0}));
            FuncNode anchorY = RuntimeUI.get(Add({Multiply({offset, 10}), 1}));
            FuncNode pivotX = RuntimeUI.get(Add({Multiply({offset, 10}), 2}));
            FuncNode pivotY = RuntimeUI.get(Add({Multiply({offset, 10}), 3}));
            FuncNode width = RuntimeUI.get(Add({Multiply({offset, 10}), 4}));
            FuncNode height = RuntimeUI.get(Add({Multiply({offset, 10}), 5}));
            FuncNode rotation = RuntimeUI.get(Add({Multiply({offset, 10}), 6}));
            FuncNode alpha = RuntimeUI.get(Add({Multiply({offset, 10}), 7}));
            FuncNode horizontalAlign = RuntimeUI.get(Add({Multiply({offset, 10}), 8}));
            FuncNode background = RuntimeUI.get(Add({Multiply({offset, 10}), 9}));


            FuncNode set(FuncNode anchorX, FuncNode anchorY, FuncNode pivotX, FuncNode pivotY, FuncNode width, FuncNode height, FuncNode rotation, FuncNode alpha, FuncNode horizontalAlign, FuncNode background) {
            	FUNCBEGIN
                RuntimeUI.set(Add({Multiply({offset, 10}), 0}), anchorX);
                RuntimeUI.set(Add({Multiply({offset, 10}), 1}), anchorY);
                RuntimeUI.set(Add({Multiply({offset, 10}), 2}), pivotX);
                RuntimeUI.set(Add({Multiply({offset, 10}), 3}), pivotY);
                RuntimeUI.set(Add({Multiply({offset, 10}), 4}), width);
                RuntimeUI.set(Add({Multiply({offset, 10}), 5}), height);
                RuntimeUI.set(Add({Multiply({offset, 10}), 6}), rotation);
                RuntimeUI.set(Add({Multiply({offset, 10}), 7}), alpha);
                RuntimeUI.set(Add({Multiply({offset, 10}), 8}), horizontalAlign);
                RuntimeUI.set(Add({Multiply({offset, 10}), 9}), background);
                return VOID;
            }
        };

        class configuration {
            public:

            FuncNode offset = 0;
            configuration(){};
            configuration(FuncNode offset):offset(offset){};

            FuncNode scale = RuntimeUIConfiguration.get(Add({Multiply({offset, 2}), 0}));
            FuncNode alpha = RuntimeUIConfiguration.get(Add({Multiply({offset, 2}), 1}));
        };

        layout menu = layout(0);
        layout judgment = layout(1);
        layout comboValue = layout(2);
        layout comboText = layout(3);
        layout primaryMetricBar = layout(4);
        layout primaryMetricValue = layout(5);
        layout secondaryMetricBar = layout(6);
        layout secondaryMetricValue = layout(7);
        layout progress = layout(8);

        configuration menuConfiguration = configuration(0);
        configuration judgmentConfiguration = configuration(1);
        configuration comboConfiguration = configuration(2);
        configuration primaryMetricConfiguration = configuration(3);
        configuration secondaryMetricConfiguration = configuration(4);
        configuration progressConfiguration = configuration(5);
    }ui;

    class life {
        public:

        FuncNode offset = 0;
        FuncNode perfect = ArchetypeLife.get(0);
        FuncNode great = ArchetypeLife.get(1);
        FuncNode good = ArchetypeLife.get(2);
        FuncNode miss = ArchetypeLife.get(3);

        life(){};
        life(FuncNode offset):offset(offset){
            perfect = ArchetypeLife.get(Add({Multiply({offset, 4}), 0}));
            great = ArchetypeLife.get(Add({Multiply({offset, 4}), 1}));
            good = ArchetypeLife.get(Add({Multiply({offset, 4}), 2}));
            miss = ArchetypeLife.get(Add({Multiply({offset, 4}), 3}));
        };
        life operator [] (FuncNode offset) {
            return life(offset);
        }
        FuncNode set(FuncNode perfect, FuncNode great, FuncNode good, FuncNode bad) {
        	FUNCBEGIN
            ArchetypeLife.set(Add({Multiply({offset, 4}), 0}), perfect);
            ArchetypeLife.set(Add({Multiply({offset, 4}), 1}), great);
            ArchetypeLife.set(Add({Multiply({offset, 4}), 2}), good);
            ArchetypeLife.set(Add({Multiply({offset, 4}), 3}), bad);
            return VOID;
        }
    }lifes;

    class entityInfo {
        public:

        FuncNode id = EntityInfo.get(0);
        FuncNode archetype = EntityInfo.get(1);
        FuncNode state = EntityInfo.get(2);
    }entityInfo;

    class bucket {
        public:

        FuncNode offset = 0;
        FuncNode minPerfect = LevelBucket.get(0), maxPerfect = LevelBucket.get(1);
        FuncNode minGreat = LevelBucket.get(2), maxGreat = LevelBucket.get(3);
        FuncNode minGood = LevelBucket.get(4), maxGood = LevelBucket.get(5);

        bucket(){}
        bucket(FuncNode offset):offset(offset){
            minPerfect = LevelBucket.get(Add({Multiply({offset, 6}), 0}));
            maxPerfect = LevelBucket.get(Add({Multiply({offset, 6}), 1}));
            minGreat = LevelBucket.get(Add({Multiply({offset, 6}), 2}));
            maxGreat = LevelBucket.get(Add({Multiply({offset, 6}), 3}));
            minGood = LevelBucket.get(Add({Multiply({offset, 6}), 4}));
            maxGood = LevelBucket.get(Add({Multiply({offset, 6}), 5}));
        }
        bucket operator [] (FuncNode offset) {
            return bucket(offset);
        }

        SonolusApi set(var minPerfect, var maxPerfect, var minGreat, var maxGreat, var minGood, var maxGood) {
        	FUNCBEGIN
            LevelBucket.set(Add({Multiply({offset, 6}), 0}), minPerfect);
            LevelBucket.set(Add({Multiply({offset, 6}), 1}), maxPerfect);
            LevelBucket.set(Add({Multiply({offset, 6}), 2}), minGreat);
            LevelBucket.set(Add({Multiply({offset, 6}), 3}), maxGreat);
            LevelBucket.set(Add({Multiply({offset, 6}), 4}), minGood);
            LevelBucket.set(Add({Multiply({offset, 6}), 5}), maxGood);
            return VOID;
        }
    }buckets;
//     int allocatorSize[10001] = {0};
//     
//     template<int identifierId>
//     class Variable {
//         public:
// 
//         int offset;
//         Variable(){
//             offset = allocatorSize[identifierId]++;
//         }
// 
//         FuncNode get() {
//             return Get(identifierId, offset);
//         }
//         FuncNode add(FuncNode value) {
//             return Set(identifierId, offset, Add({Get(identifierId, offset), value}));
//         }
//         FuncNode subtract(FuncNode value) {
//             return Set(identifierId, offset, Subtract({Get(identifierId, offset), value}));
//         }
//         FuncNode multiply(FuncNode value) {
//             return Set(identifierId, offset, Multiply({Get(identifierId, offset), value}));
//         }
//         FuncNode divide(FuncNode value) {
//             return Set(identifierId, offset, Divide({Get(identifierId, offset), value}));
//         }
//         FuncNode mod(FuncNode value) {
//             return Set(identifierId, offset, Mod({Get(identifierId, offset), value}));
//         }
//         FuncNode set(FuncNode value) {
//             return Set(identifierId, offset, value);
//         }
//     };
// 
//     Variable<EntityMemoryId> ForPt[MaxForSize];
// 
//     template<int identifierId>
//     class Array {
//         public:
// 
//         int offset;
//         int capacity;
//         FuncNode size = Get(identifierId, 0);
//         int sizeOffset = 0;
// 
//         Array(){}
//         Array(int capacity):capacity(capacity){
//             sizeOffset = allocatorSize[identifierId];
//             size = Get(identifierId, sizeOffset);
//             offset = allocatorSize[identifierId] + 1;
//             allocatorSize[identifierId] += capacity + 1;
//         };
//         FuncNode operator [] (FuncNode id) {
//             return Get(identifierId, Add({id, offset}));
//         }
//         FuncNode add(FuncNode value) {
//             return Execute({
//                 Set(identifierId, Add({size, offset}), value),
//                 Set(identifierId, sizeOffset, Add({size, 1})),
//             });
//         }
//         FuncNode has(FuncNode value) {
// /*            FuncNode res = false;
//             for (int i = capacity - 1; i >= 0; i--) {
//                 res = If(
//                     Equal(Get(identifierId, Add({i, offset})), value),
//                     true,
//                     res
//                 );
//             } return res;*/
// 			return Block(Execute({
// 				FOR (i, 0, capacity, 1) {
// 					IF (Equal(Get(identifierId, Add({i, offset})), value)) { Break(3, 1) } FI
// 				} DONE,
// 				Break(1, 0)
// 			}));
//         }
//         FuncNode indexOf(FuncNode value) {
// /*            FuncNode res = -1;
//             for (int i = capacity - 1; i >= 0; i--) {
//                 res = If(
//                     Equal(Get(identifierId, Add({i, offset})), value),
//                     i,
//                     res
//                 );
//             } return res;*/
// 			return Block(Execute({
// 				FOR (i, 0, capacity, 1) {
// 					IF (Equal(Get(identifierId, Add({i, offset})), value)) { Break(3, i) } FI
// 				} DONE,
// 				Break(1, -1),
// 			}));
//         }
//         FuncNode clear() {
//             vector<FuncNode> res = {Set(identifierId, sizeOffset, 0)};
//             for (int i = 0; i < capacity; i++) res.push_back(Set(identifierId, Add({i, offset}), 0));
//             return Execute(res);
//         }
//     };
// 
//     Variable<EntityMemoryId> isHighlighted;
//     Variable<EntityMemoryId> playLoopedId;
//     Variable<EntityMemoryId> trackTouchId;

    class HorizontalAlign {
        public:

        int Left = -1;
        int Center = 0;
        int Right = 1;
    }HorizontalAlign;

    class EntityState {
        public:

        int Waiting = 0;
        int Active = 1;
        int Despawned = 2;    
    }EntityState;

    class times {
        public:

        FuncNode now = RuntimeUpdate.get(0);
        FuncNode delta = RuntimeUpdate.get(1);
        FuncNode scaled = RuntimeUpdate.get(2);
        FuncNode skip = RuntimeUpdate.get(3);
    }times;

    FuncNode isReplay = RuntimeEnvironment.get(4);
};
