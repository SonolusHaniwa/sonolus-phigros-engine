class StageController: public Archetype {
    public:

    string name = "Phigros Stage Controller";
    bool hasInput = false;
    var lastClickMenuTime;

    SonolusApi preprocess() {
        lastClickMenuTime = -10000;

        var entityCount = 0;
        var lastEntity = info.index;
        while (EntityInfoArray[entityCount].index == entityCount) {
            if (EntityInfoArray[entityCount].archetype == getAid(Judgeline)) {
                EntitySharedMemoryArray[lastEntity].generic[31] = entityCount;
                lastEntity = entityCount;
            }
            entityCount++;
        }
    }

	SonolusApi spawnOrder() { return 0; }
	SonolusApi shouldSpawn() { return true; }

    int updateSequentialOrder = -10000;
    SonolusApi updateSequential() {
        maxCombo = Max(maxCombo, combo);

        // 给所有 judgeline 清空数据
        var tmp = sharedMemory[31], cur = tmp;
        while (cur != 0) {
            for (var i = 1; i < 5; i++) EntitySharedMemoryArray[cur].generic[i] = 0;
            cur = EntitySharedMemoryArray[cur].generic[31];
        }
    }

    SonolusApi updateParallel() {
        // 暂停按钮绘制
        if (!sonolusPause && HasSkinSprite(Sprites.PauseButton)) {
            var menuL = stage.l + 0.005 * 3.556;
            var menuR = stage.l + 0.05 * 3.556;
            var menuT = stage.t + 1.0 / 1080.0 * stage.h;
            var menuB = stage.t - 0.095 * stage.h;
            var menuWidth = menuR - menuL;
            var menuHeight = menuT - menuB;
            menuL = menuL + menuWidth * 0.3;
            menuR = menuR - menuWidth * 0.3;
            menuT = menuT - menuHeight * 0.4;
            menuB = menuB + menuHeight * 0.2;
            Draw(
                Sprites.PauseButton, 
                { menuL, menuB }, 
                { menuL, menuT }, 
                { menuR, menuT }, 
                { menuR, menuB }, 
                10000, 
                1
            );
        }

        // 两侧遮挡部分绘制
        var screenL = -10000;
        var screenR = 10000;
        var screenT = 10000;
        var screenB = -10000;
        Draw(Sprites.Blocker, { screenL, screenB }, { screenL, screenT }, { stage.l, screenT }, { stage.l, screenB }, 20000000, 1);
        Draw(Sprites.Blocker, { stage.r, screenB }, { stage.r, screenT }, { screenR, screenT }, { screenR, screenB }, 20000000, 1);
        Draw(Sprites.Blocker, { screenL, screenB }, { screenL, stage.b }, { screenR, stage.b }, { screenR, screenB }, 20000000, 1);
        Draw(Sprites.Blocker, { screenL, stage.t }, { screenL, screenT }, { screenR, screenT }, { screenR, stage.t }, 20000000, 1);

        var numberDistance = 0.0068 * stage.h;
        if (combo > 2 && !sonolusCombo && HasSkinSprite(Sprites.ComboText) && HasSkinSprite(Sprites.Combo0)) {
            // Combo 数量绘制
            var tmpCombo = combo;
            var totalW = 0;
            var comboH = 0.048 * stage.h;
            while(tmpCombo != 0) {
                var digit = tmpCombo % 10;
                totalW = totalW + comboH * Switch(digit, {
                    {0, combo0Ratio}, {1, combo1Ratio}, {2, combo2Ratio},
                    {3, combo3Ratio}, {4, combo4Ratio}, {5, combo5Ratio},
                    {6, combo6Ratio}, {7, combo7Ratio}, {8, combo8Ratio}, {9, combo9Ratio}
                }) + numberDistance;
                tmpCombo = Floor(tmpCombo / 10);
            }
            if (totalW != 0) totalW = totalW - numberDistance;
            var comboR = totalW / 2.0;
            tmpCombo = combo;
            while(tmpCombo != 0) {
                var digit = tmpCombo % 10;
                var digitW = comboH * Switch(digit, {
                    {0, combo0Ratio}, {1, combo1Ratio}, {2, combo2Ratio},
                    {3, combo3Ratio}, {4, combo4Ratio}, {5, combo5Ratio},
                    {6, combo6Ratio}, {7, combo7Ratio}, {8, combo8Ratio}, {9, combo9Ratio}
                });
                var digitL = comboR - digitW;
                var digitR = comboR;
                var digitT = stage.t - 0.03 * stage.h;
                var digitB = digitT - comboH;
                Draw(
                    Switch(digit, {
                        {0, Sprites.Combo0}, {1, Sprites.Combo1}, {2, Sprites.Combo2},
                        {3, Sprites.Combo3}, {4, Sprites.Combo4}, {5, Sprites.Combo5},
                        {6, Sprites.Combo6}, {7, Sprites.Combo7}, {8, Sprites.Combo8}, {9, Sprites.Combo9}
                    }), 
                    { digitL, digitB }, 
                    { digitL, digitT }, 
                    { digitR, digitT }, 
                    { digitR, digitB }, 
                    20000, 
                    1
                );
                comboR = comboR - digitW - numberDistance;
                tmpCombo = Floor(tmpCombo / 10);
            }

            // Combo 文字绘制
            var comboTextHeight = 0.017 * stage.h;
            var comboTextWidth = comboTextHeight * comboTextRatio;
            var comboTextL = -1 * comboTextWidth / 2;
            var comboTextR = comboTextWidth / 2;
            var comboTextT = stage.t - 0.09 * stage.h;
            var comboTextB = comboTextT - comboTextHeight;
            Draw(
                Sprites.ComboText, 
                { comboTextL, comboTextB }, 
                { comboTextL, comboTextT }, 
                { comboTextR, comboTextT }, 
                { comboTextR, comboTextB }, 
                20000, 
                1
            );
        }

        // 分数绘制
        if (!sonolusPrimaryMetric && HasSkinSprite(Sprites.Combo0)) {
            var score = If(strictMode, accscore / notes * 1000000, accscore / notes * 900000 + maxCombo / notes * 100000);
            var scoreH = 0.035 * stage.h;
            var scoreW = -1 * numberDistance;
            for (var i = 0; i < 7; i++) {
                var digit = Floor(score / Power({10, i})) % 10;
                scoreW = scoreW + scoreH * Switch(digit, {
                    {0, combo0Ratio}, {1, combo1Ratio}, {2, combo2Ratio},
                    {3, combo3Ratio}, {4, combo4Ratio}, {5, combo5Ratio},
                    {6, combo6Ratio}, {7, combo7Ratio}, {8, combo8Ratio}, {9, combo9Ratio}
                }) + numberDistance;
            }
            var scoreR = stage.r - 0.0185 * stage.w;
            for (var i = 0; i < 7; i++) {
                var digit = Floor(score / Power({10, i})) % 10;
                var digitW = scoreH * Switch(digit, {
                    {0, combo0Ratio}, {1, combo1Ratio}, {2, combo2Ratio},
                    {3, combo3Ratio}, {4, combo4Ratio}, {5, combo5Ratio},
                    {6, combo6Ratio}, {7, combo7Ratio}, {8, combo8Ratio}, {9, combo9Ratio}
                });
                var digitL = scoreR - digitW;
                var digitR = scoreR;
                var digitT = stage.t - 0.04 * stage.h;
                var digitB = digitT - scoreH;
                Draw(
                    Switch(digit, {
                        {0, Sprites.Combo0}, {1, Sprites.Combo1}, {2, Sprites.Combo2},
                        {3, Sprites.Combo3}, {4, Sprites.Combo4}, {5, Sprites.Combo5},
                        {6, Sprites.Combo6}, {7, Sprites.Combo7}, {8, Sprites.Combo8}, {9, Sprites.Combo9}
                    }), 
                    { digitL, digitB }, 
                    { digitL, digitT },
                    { digitR, digitT }, 
                    { digitR, digitB }, 
                    20000, 
                    1
                );
                scoreR = scoreR - digitW - numberDistance;
            }
        }

        // 背景遮罩绘制
        Draw(
            Sprites.Blocker, 
            { screen.l, screen.b }, 
            { screen.l, screen.t }, 
            { screen.r, screen.t }, 
            { screen.r, screen.b }, 
            -1, 
            1 - background
        );

        // 时间条绘制
        var percent = Min(maxTime, Max(times.now, 0)) / maxTime;
        var timeBarT = stage.t, timeBarB = stage.t - timeBarHeight;
        var timeBarL = stage.l, timeBarR = stage.w * percent + stage.l;
        Draw(
            Sprites.NormalJudgeline, 
            { timeBarL, timeBarB }, 
            { timeBarL, timeBarT }, 
            { timeBarR, timeBarT }, 
            { timeBarR, timeBarB }, 
            100000, 
            1
        );
    }
};