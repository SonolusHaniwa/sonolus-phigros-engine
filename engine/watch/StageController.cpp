class StageController: public Archetype {
    public:

    static constexpr const char* name = "Phigros Stage Controller";
    bool hasInput = false;
    Variable<EntityMemoryId> lastClickMenuTime;

    // Inplemented in Preprocess.cpp
    SonolusApi calcCombo();

    SonolusApi preprocess() {
        FUNCBEGIN
        lastClickMenuTime = -10000;
        calcCombo();
        return VOID;
    }

	SonolusApi spawnTime() { return -999999; }
	SonolusApi despawnTime() { return 999999; }

    int updateSequentialOrder = -10000;
    SonolusApi updateSequential() {
        FUNCBEGIN
        Return(0);
        return VOID;
    }

    SonolusApi updateParallel() {
        FUNCBEGIN
        // 暂停按钮绘制
        IF (!sonolusPause && HasSkinSprite(Sprites.PauseButton)) {
            let menuL = stage.l + 0.005 * stage.w;
            let menuR = stage.l + 0.05 * stage.w;
            let menuT = stage.t + 1.0 / 1080.0 * stage.h;
            let menuB = stage.t - 0.095 * stage.h;
            let menuWidth = menuR - menuL;
            let menuHeight = menuT - menuB;
            menuL = menuL + menuWidth * 0.3;
            menuR = menuR - menuWidth * 0.3;
            menuT = menuT - menuHeight * 0.4;
            menuB = menuB + menuHeight * 0.2;
            Draw(Sprites.PauseButton, menuL, menuB, menuL, menuT, menuR, menuT, menuR, menuB, 10000, 1);
        } FI

        // 两侧遮挡部分绘制
        let screenL = -10000;
        let screenR = 10000;
        let screenT = 10000;
        let screenB = -10000;
        Draw(Sprites.Blocker, screenL, screenB, screenL, screenT, stage.l, screenT, stage.l, screenB, 20000, 1);
        Draw(Sprites.Blocker, stage.r, screenB, stage.r, screenT, screenR, screenT, screenR, screenB, 20000, 1);
        Draw(Sprites.Blocker, screenL, screenB, screenL, stage.b, screenR, stage.b, screenR, screenB, 20000, 1);
        Draw(Sprites.Blocker, screenL, stage.t, screenL, screenT, screenR, screenT, screenR, stage.t, 20000, 1);

        var numberDistance = 0.0068 * stage.h;
        IF (combo > 2 && !sonolusCombo && HasSkinSprite(Sprites.ComboText) && HasSkinSprite(Sprites.Combo0)) {
            // Combo 数量绘制
            var tmpCombo = combo.get();
            var totalW = 0;
            var comboH = 0.048 * stage.h;
            WHILE(tmpCombo != 0) {
                var digit = tmpCombo % 10;
                totalW = totalW + comboH * Switch(digit, {
                    {0, combo0Ratio}, {1, combo1Ratio}, {2, combo2Ratio},
                    {3, combo3Ratio}, {4, combo4Ratio}, {5, combo5Ratio},
                    {6, combo6Ratio}, {7, combo7Ratio}, {8, combo8Ratio}, {9, combo9Ratio}
                }) + numberDistance;
                tmpCombo = Floor(tmpCombo / 10);
            } DONE
            IF (totalW != 0) totalW = totalW - numberDistance; FI
            var comboR = totalW / 2.0;
            tmpCombo = combo.get();
            WHILE(tmpCombo != 0) {
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
                Draw(Switch(digit, {
                    {0, Sprites.Combo0}, {1, Sprites.Combo1}, {2, Sprites.Combo2},
                    {3, Sprites.Combo3}, {4, Sprites.Combo4}, {5, Sprites.Combo5},
                    {6, Sprites.Combo6}, {7, Sprites.Combo7}, {8, Sprites.Combo8}, {9, Sprites.Combo9}
                }), digitL, digitB, digitL, digitT, digitR, digitT, digitR, digitB, 20000, 1);
                comboR = comboR - digitW - numberDistance;
                tmpCombo = Floor(tmpCombo / 10);
            } DONE

            // Combo 文字绘制
            var comboTextHeight = 0.017 * stage.h;
            var comboTextWidth = comboTextHeight * comboTextRatio;
            var comboTextL = -1 * comboTextWidth / 2;
            var comboTextR = comboTextWidth / 2;
            var comboTextT = stage.t - 0.09 * stage.h;
            var comboTextB = comboTextT - comboTextHeight;
            Draw(Sprites.ComboText, comboTextL, comboTextB, comboTextL, comboTextT, comboTextR, comboTextT, comboTextR, comboTextB, 20000, 1);
        } FI

        // 分数绘制
        IF (!sonolusPrimaryMetric && HasSkinSprite(Sprites.Combo0)) {
            var score = If(strictMode, accscore / notes * 1000000, accscore / notes * 900000 + maxCombo / notes * 100000);
            var scoreH = 0.035 * stage.h;
            var scoreW = -1 * numberDistance;
            FOR (i, 0, 7, 1) {
                var digit = Floor(score / Power({10, i})) % 10;
                scoreW = scoreW + scoreH * Switch(digit, {
                    {0, combo0Ratio}, {1, combo1Ratio}, {2, combo2Ratio},
                    {3, combo3Ratio}, {4, combo4Ratio}, {5, combo5Ratio},
                    {6, combo6Ratio}, {7, combo7Ratio}, {8, combo8Ratio}, {9, combo9Ratio}
                }) + numberDistance;
            } DONE
            var scoreR = stage.r - 0.0185 * stage.w;
            FOR (i, 0, 7, 1) {
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
                Draw(Switch(digit, {
                    {0, Sprites.Combo0}, {1, Sprites.Combo1}, {2, Sprites.Combo2},
                    {3, Sprites.Combo3}, {4, Sprites.Combo4}, {5, Sprites.Combo5},
                    {6, Sprites.Combo6}, {7, Sprites.Combo7}, {8, Sprites.Combo8}, {9, Sprites.Combo9}
                }), digitL, digitB, digitL, digitT, digitR, digitT, digitR, digitB, 20000, 1);
                scoreR = scoreR - digitW - numberDistance;
            } DONE
        } FI

        // 背景遮罩绘制
        Draw(Sprites.Blocker, screen.l, screen.b, screen.l, screen.t, screen.r, screen.t, screen.r, screen.b, -1, 1 - background);

        // 时间条绘制
        var percent = Min(maxTime, Max(times.now, 0)) / maxTime;
        var timeBarT = stage.t, timeBarB = stage.t - timeBarHeight;
        var timeBarL = stage.l, timeBarR = stage.w * percent + stage.l;
        Draw(Sprites.NormalJudgeline, timeBarL, timeBarB, timeBarL, timeBarT, timeBarR, timeBarT, timeBarR, timeBarB, 100000, 1);
        return VOID;
    }
};