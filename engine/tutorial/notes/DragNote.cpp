SonolusApi DragNoteFall(let time, let sum) {
    FUNCBEGIN
    instruction = -1;
    spawnedEffect = 0;
    let t = stage.t;
    let b = t - 0.8 * stage.h;
    let x = 0, y = Lerp(t, b, time / sum);
    let w = baseNoteWidth * If(HasSkinSprite(Sprites.NormalDrag), 2, 1), 
        h = w / If(HasSkinSprite(Sprites.NormalDrag), dragRatio, fallBackNoteRatio);
    let L = x - w / 2, R = x + w / 2, B = y - h / 2, T = y + h / 2;
    Draw(
        If(HasSkinSprite(Sprites.NormalDrag), Sprites.NormalDrag, Sprites.FallbackNormalDrag), 
        L, B, L, T, R, T, R, B, 10000, 1
    );
    return VOID;
}

SonolusApi DragNoteFrozen(let time, let sum) {
    FUNCBEGIN
    instruction = Texts.Drag;
    spawnedEffect = 0;
    let x = 0, y = stage.t - 0.8 * stage.h;
    let w = baseNoteWidth * If(HasSkinSprite(Sprites.NormalDrag), 2, 1), 
        h = w / If(HasSkinSprite(Sprites.NormalDrag), dragRatio, fallBackNoteRatio);
    let L = x - w / 2, R = x + w / 2, B = y - h / 2, T = y + h / 2;
    Draw(
        If(HasSkinSprite(Sprites.NormalDrag), Sprites.NormalDrag, Sprites.FallbackNormalDrag), 
        L, B, L, T, R, T, R, B, 10000, 1
    );
    drawFrozenHand(Icons.hand, x, y, time / (sum / 4) % 1);
    return VOID;
}

SonolusApi DragNoteHit(let time, let sum) {
    FUNCBEGIN
    instruction = -1;
    IF (spawnedEffect == 0) {
        let x = 0, y = stage.t - 0.8 * stage.h;
        let l = x - effectWidth, r = x + effectWidth, b = y - effectWidth, t = y + effectWidth;
        SpawnParticleEffect(Effects.perfect, l, b, l, t, r, t, r, b, 0.5);
        spawnedEffect = 1;
    } FI
    return VOID;
}