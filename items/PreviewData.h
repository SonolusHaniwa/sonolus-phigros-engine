using namespace std;

namespace previewData {
    const int PreviewDataId = 2000;
    const int PreviewOptionId = 2001;
    const int EntityDataArrayId = 4100;
    const int EntitySharedMemoryArrayId = 4101;
    const int EntityInfoArrayId = 4102;
    const int EntityDataId = 4000;
    const int EntitySharedMemoryId = 4001;
    const int EntityInfoId = 4002;
    const int EngineRomId = 3000;
    const int TemporaryMemoryId = 10000;
    const int RuntimeEnvironmentId = 1000;
    const int RuntimeCanvasId = 1001;
    const int RuntimeSkinTransformId = 1002;
    const int RuntimeUIId = 1003;
    const int RuntimeUIConfigurationId = 1004;

    BlockPointer<RuntimeEnvironmentId> RuntimeEnvironment;
    BlockPointer<RuntimeCanvasId> RuntimeCanvas;
    BlockPointer<RuntimeSkinTransformId> RuntimeSkinTransform;
    BlockPointer<RuntimeUIId> RuntimeUI;
    BlockPointer<RuntimeUIConfigurationId> RuntimeUIConfiguration;
    BlockPointer<PreviewDataId> PreviewData;
    BlockPointer<PreviewOptionId> PreviewOption;
    BlockPointer<EngineRomId> EngineRom;
    BlockArray<EntityDataArrayId> EntityDataArray(32);
    BlockPointer<EntityDataId> EntityData;
    BlockArray<EntitySharedMemoryArrayId> EntitySharedMemoryArray(32);
    BlockPointer<EntitySharedMemoryId> EntitySharedMemory;
    BlockArray<EntityInfoArrayId> EntityInfoArray(2);
    BlockPointer<EntityInfoId> EntityInfo;
    BlockPointer<TemporaryMemoryId> TemporaryMemory;

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

    class canvas {
        public:

        FuncNode scroll = RuntimeCanvas.get(0);
        FuncNode size = RuntimeCanvas.get(1);

        SonolusApi set(FuncNode scroll, FuncNode size) {
        	FUNCBEGIN
            RuntimeCanvas.set(0, scroll);
            RuntimeCanvas.set(1, size);
            return VOID;
        }
    }canvas;

    class ui {
        public:

        class layout {
            public: 

            FuncNode offset = 0;
            layout(){};
            layout(FuncNode offset):offset(offset){};

            FuncNode anchorX = RuntimeUI.get(Add({Multiply({offset, 9}), 0}));
            FuncNode anchorY = RuntimeUI.get(Add({Multiply({offset, 9}), 1}));
            FuncNode pivotX = RuntimeUI.get(Add({Multiply({offset, 9}), 2}));
            FuncNode pivotY = RuntimeUI.get(Add({Multiply({offset, 9}), 3}));
            FuncNode width = RuntimeUI.get(Add({Multiply({offset, 9}), 4}));
            FuncNode height = RuntimeUI.get(Add({Multiply({offset, 9}), 5}));
            FuncNode rotation = RuntimeUI.get(Add({Multiply({offset, 9}), 6}));
            FuncNode alpha = RuntimeUI.get(Add({Multiply({offset, 9}), 7}));
            FuncNode background = RuntimeUI.get(Add({Multiply({offset, 9}), 8}));


            SonolusApi set(FuncNode anchorX, FuncNode anchorY, FuncNode pivotX, FuncNode pivotY, FuncNode width, FuncNode height, FuncNode rotation, FuncNode alpha, FuncNode background) {
                FUNCBEGIN
                RuntimeUI.set(Add({Multiply({offset, 9}), 0}), anchorX);
                RuntimeUI.set(Add({Multiply({offset, 9}), 1}), anchorY);
                RuntimeUI.set(Add({Multiply({offset, 9}), 2}), pivotX);
                RuntimeUI.set(Add({Multiply({offset, 9}), 3}), pivotY);
                RuntimeUI.set(Add({Multiply({offset, 9}), 4}), width);
                RuntimeUI.set(Add({Multiply({offset, 9}), 5}), height);
                RuntimeUI.set(Add({Multiply({offset, 9}), 6}), rotation);
                RuntimeUI.set(Add({Multiply({offset, 9}), 7}), alpha);
                RuntimeUI.set(Add({Multiply({offset, 9}), 8}), background);
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
        layout progress = layout(1);

        configuration menuConfiguration = configuration(0);
        configuration progressConfiguration = configuration(1);
    }ui;

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
// 	Variable<TemporaryMemoryId> ForPt[MaxForSize];
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
//             FuncNode res = false;
//             for (int i = capacity - 1; i >= 0; i--) {
//                 res = If(
//                     Equal(Get(identifierId, Add({i, offset})), value),
//                     true,
//                     res
//                 );
//             } return res;
//         }
//         FuncNode indexOf(FuncNode value) {
//             FuncNode res = -1;
//             for (int i = capacity - 1; i >= 0; i--) {
//                 res = If(
//                     Equal(Get(identifierId, Add({i, offset})), value),
//                     i,
//                     res
//                 );
//             } return res;
//         }
//         FuncNode clear() {
//             vector<FuncNode> args = {Set(identifierId, sizeOffset, 0)};
//             for (int i = 0; i < capacity; i++) args.push_back(Set(identifierId, Add({i, offset}), 0));
//             return Execute(args);
//         }
//     };
    class times {
        public:

        FuncNode now = 0;
        FuncNode delta = 0;
        FuncNode scaled = 0;
    }times;
};
