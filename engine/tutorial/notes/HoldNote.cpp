SonolusApi HoldNoteFall(let time, let sum) {
    FUNCBEGIN
    instruction = -1;
    spawnedEffect = 0;
    let t = stage.t;
    let b = t - 0.8 * stage.h;
    let x = 0, y = Lerp(t, b, time / sum);
    let headWidth = baseNoteWidth, headHeight = headWidth / holdHeadRatio;
    let L = x - headWidth / 2, R = x + headWidth / 2, B = y, T = y + 2 * headHeight / 2;
    Draw(Sprites.NormalHoldHead, L, B, L, T, R, T, R, B, 2000, 1);
    let bodyWidth = baseNoteWidth, bodyHeight = 0.8 * stage.h;
    B = T, T = T + bodyHeight;
    Draw(Sprites.NormalHoldBody, L, B, L, T, R, T, R, B, 2000, 1);
    return VOID;
}

SonolusApi HoldNoteFrozen(let time, let sum) {
    FUNCBEGIN
    instruction = Texts.Hold;
    spawnedEffect = 0;
    let x = 0, y = stage.t - 0.8 * stage.h;
    let headWidth = baseNoteWidth, headHeight = headWidth / holdHeadRatio;
    let L = x - headWidth / 2, R = x + headWidth / 2, B = y, T = y + 2 * headHeight / 2;
    Draw(Sprites.NormalHoldHead, L, B, L, T, R, T, R, B, 2000, 1);
    let bodyWidth = baseNoteWidth, bodyHeight = 0.8 * stage.h;
    B = T, T = T + bodyHeight;
    Draw(Sprites.NormalHoldBody, L, B, L, T, R, T, R, B, 2000, 1);
    drawFrozenHand(Icons.hand, x, y, time / (sum / 4) % 1);
    return VOID;
}

SonolusApi HoldNoteFall2(let time, let sum) {
    FUNCBEGIN
    instruction = -1;
    let x = 0, y = stage.t - 0.8 * stage.h;
    let bodyWidth = baseNoteWidth, bodyHeight = 0.8 * stage.h * (1 - time / sum);
    let L = x - bodyWidth / 2, R = x + bodyWidth / 2, B = y, T = y + bodyHeight;
    Draw(Sprites.NormalHoldBody, L, B, L, T, R, T, R, B, 2000, 1);
    drawFrozenHand(Icons.hand, x, y, 1, false, false);
    IF (times.now - spawnedEffect >= 30.0 / 138.0) {
        let x = 0, y = stage.t - 0.8 * stage.h;
        let l = x - effectWidth, r = x + effectWidth, b = y - effectWidth, t = y + effectWidth;
        SpawnParticleEffect(Effects.perfect, l, b, l, t, r, t, r, b, 0.5);
        spawnedEffect = times.now;
    } FI
    return VOID;
}