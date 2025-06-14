Blocked SonolusApi drawPause(var, var) {
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
    Draw(Sprites.PauseButton, { menuL, menuB }, { menuL, menuT }, { menuR, menuT }, { menuR, menuB }, 10000, 1);
}

Blocked SonolusApi drawBorder(var, var) {
    var screenL = -10000;
    var screenR = 10000;
    var screenT = 10000;
    var screenB = -10000;
    Draw(Sprites.Blocker, { screenL, screenB }, { screenL, screenT }, { stage.l, screenT }, { stage.l, screenB }, 20000, 1);
    Draw(Sprites.Blocker, { stage.r, screenB }, { stage.r, screenT }, { screenR, screenT }, { screenR, screenB }, 20000, 1);
    Draw(Sprites.Blocker, { screenL, screenB }, { screenL, stage.b }, { screenR, stage.b }, { screenR, screenB }, 20000, 1);
    Draw(Sprites.Blocker, { screenL, stage.t }, { screenL, screenT }, { screenR, screenT }, { screenR, stage.t }, 20000, 1);
}

Blocked SonolusApi drawTimeline(var time, var sum) {
    var percent = time / sum;
    var timeBarT = stage.t, timeBarB = stage.t - timeBarHeight;
    var timeBarL = stage.l, timeBarR = stage.w * percent + stage.l;
    Draw(Sprites.NormalJudgeline, { timeBarL, timeBarB }, { timeBarL, timeBarT }, { timeBarR, timeBarT }, { timeBarR, timeBarB }, 100000, 1);
}

Blocked SonolusApi drawScore(var, var) {
    var numberDistance = 0.0068 * stage.h;
    var score = 0;
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

Blocked SonolusApi drawMask(var, var) {
    Draw(Sprites.Blocker, { screen.l, screen.b }, { screen.l, screen.t }, { screen.r, screen.t }, { screen.r, screen.b }, -1, 0.5);
}

defineTutorialStatic(drawPause);
defineTutorialStatic(drawBorder);
defineTutorialStatic(drawTimeline);
defineTutorialStatic(drawScore);
defineTutorialStatic(drawMask);