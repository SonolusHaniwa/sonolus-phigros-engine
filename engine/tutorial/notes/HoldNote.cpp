Blocked SonolusApi HoldNoteFall(var time, var sum) {
    instruction = -1;
    spawnedEffect = 0;
    var t = stage.t;
    var b = t - 0.8 * stage.h;
    var x = 0, y = Lerp(t, b, time / sum);
    var headWidth = baseNoteWidth, headHeight = headWidth / holdHeadRatio;
    var L = x - headWidth / 2, R = x + headWidth / 2, B = y, T = y + 2 * headHeight / 2;
    Draw(Sprites.NormalHoldHead, { L, B }, { L, T }, { R, T }, { R, B }, 2000, 1);
    var bodyWidth = baseNoteWidth, bodyHeight = 0.8 * stage.h;
    B = T, T = T + bodyHeight;
    Draw(Sprites.NormalHoldBody, { L, B }, { L, T }, { R, T }, { R, B }, 2000, 1);
}

Blocked SonolusApi HoldNoteFrozen(var time, var sum) {
    instruction = int(iTexts.HoldAndFollow);
    spawnedEffect = 0;
    var x = 0, y = stage.t - 0.8 * stage.h;
    var headWidth = baseNoteWidth, headHeight = headWidth / holdHeadRatio;
    var L = x - headWidth / 2, R = x + headWidth / 2, B = y, T = y + 2 * headHeight / 2;
    Draw(Sprites.NormalHoldHead, { L, B }, { L, T }, { R, T }, { R, B }, 2000, 1);
    var bodyWidth = baseNoteWidth, bodyHeight = 0.8 * stage.h;
    B = T, T = T + bodyHeight;
    Draw(Sprites.NormalHoldBody, { L, B }, { L, T }, { R, T }, { R, B }, 2000, 1);
    drawFrozenHand(iIcons.Hand, x, y, time / (sum / 4) % 1);
}

Blocked SonolusApi HoldNoteFall2(var time, var sum) {
    instruction = -1;
    var x = 0, y = stage.t - 0.8 * stage.h;
    var bodyWidth = baseNoteWidth, bodyHeight = 0.8 * stage.h * (1 - time / sum);
    var L = x - bodyWidth / 2, R = x + bodyWidth / 2, B = y, T = y + bodyHeight;
    Draw(Sprites.NormalHoldBody, { L, B }, { L, T }, { R, T }, { R, B }, 2000, 1);
    drawFrozenHand(iIcons.Hand, x, y, 1, false, false);
    if (times.now - spawnedEffect >= 30.0 / 138.0) {
        var x = 0, y = stage.t - 0.8 * stage.h;
        var l = x - effectWidth, r = x + effectWidth, b = y - effectWidth, t = y + effectWidth;
        SpawnParticleEffect(Effects.Perfect, { l, b }, { l, t }, { r, t }, { r, b }, 0.5, false);
        spawnedEffect = times.now;
    }
}

defineTutorialSegment(HoldNoteFall, tutorialFallTime);
defineTutorialSegment(HoldNoteFrozen, tutorialClickTime);
defineTutorialSegment(HoldNoteFall2, tutorialFallTime);
defineTutorialSegmentsGroup(HoldNote);