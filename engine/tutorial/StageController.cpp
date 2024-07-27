SonolusApi drawPause(let, let) {
    FUNCBEGIN
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
    return VOID;
}

SonolusApi drawBorder(let, let) {
    FUNCBEGIN
    let screenL = -10000;
    let screenR = 10000;
    let screenT = 10000;
    let screenB = -10000;
    Draw(Sprites.Blocker, screenL, screenB, screenL, screenT, stage.l, screenT, stage.l, screenB, 20000, 1);
    Draw(Sprites.Blocker, stage.r, screenB, stage.r, screenT, screenR, screenT, screenR, screenB, 20000, 1);
    Draw(Sprites.Blocker, screenL, screenB, screenL, stage.b, screenR, stage.b, screenR, screenB, 20000, 1);
    Draw(Sprites.Blocker, screenL, stage.t, screenL, screenT, screenR, screenT, screenR, stage.t, 20000, 1);
    return VOID;
}

SonolusApi drawTimeline(let time, let sum) {
    FUNCBEGIN
    var percent = time / sum;
    var timeBarT = stage.t, timeBarB = stage.t - timeBarHeight;
    var timeBarL = stage.l, timeBarR = stage.w * percent + stage.l;
    Draw(Sprites.NormalJudgeline, timeBarL, timeBarB, timeBarL, timeBarT, timeBarR, timeBarT, timeBarR, timeBarB, 100000, 1);
    return VOID;
}

SonolusApi drawScore(let, let) {
    FUNCBEGIN
    let numberDistance = 0.0068 * stage.h;
    var score = 0;
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
    return VOID;
}

SonolusApi drawMask(let, let) {
    FUNCBEGIN
    Draw(Sprites.Blocker, screen.l, screen.b, screen.l, screen.t, screen.r, screen.t, screen.r, screen.b, -1, 0.5);
    return VOID;
}