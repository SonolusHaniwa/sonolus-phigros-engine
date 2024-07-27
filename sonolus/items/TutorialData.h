using namespace std;

namespace tutorialData {
    const int TutorialMemoryId = 2000;
    const int TutorialDataId = 2001;
    const int TutorialInstructionId = 2002;
    const int TemporaryMemoryId = 10000;
    const int RuntimeEnvironmentId = 1000;
    const int RuntimeUpdateId = 1001;
    const int RuntimeSkinTransformId = 1002;
    const int RuntimeParticleTransformId = 1003;
    const int RuntimeBackgroundId = 1004;
    const int RuntimeUIId = 1005;
    const int RuntimeUIConfigurationId = 1006;

    BlockPointer<RuntimeEnvironmentId> RuntimeEnvironment;
    BlockPointer<RuntimeUpdateId> RuntimeUpdate;
    BlockPointer<RuntimeSkinTransformId> RuntimeSkinTransform;
    BlockPointer<RuntimeParticleTransformId> RuntimeParticleTransform;
    BlockPointer<RuntimeBackgroundId> RuntimeBackground;
    BlockPointer<RuntimeUIId> RuntimeUI;
    BlockPointer<RuntimeUIConfigurationId> RuntimeUIConfiguration;
    BlockPointer<TutorialMemoryId> TutorialMemory;
    BlockPointer<TutorialDataId> TutorialData;
    BlockPointer<TutorialInstructionId> TutorialInstruction;
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


            FuncNode set(FuncNode anchorX, FuncNode anchorY, FuncNode pivotX, FuncNode pivotY, FuncNode width, FuncNode height, FuncNode rotation, FuncNode alpha, FuncNode background) {
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
        layout navigationPrevious = layout(1);
        layout navigationNext = layout(2);
        layout instruction = layout(3);

        configuration menuConfiguration = configuration(0);
        configuration navigationConfiguration = configuration(1);
        configuration instructionConfiguration = configuration(2);
    }ui;
    
    Variable<TutorialInstructionId> instruction;
    Variable<TutorialMemoryId> tutorialStartTime;
    Variable<TutorialMemoryId> tutorialNavigation;

    class times {
        public:

        FuncNode now = RuntimeUpdate.get(0);
        FuncNode delta = RuntimeUpdate.get(1);
    }times;

    FuncNode timesNow = Subtract({times.now, tutorialStartTime.get()});

    class navigation {
        public:

        FuncNode direction = RuntimeUpdate.get(2);
    }navigation;
};
