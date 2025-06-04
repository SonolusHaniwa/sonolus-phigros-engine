Blocked SonolusApi FlickNoteFall(var time, var sum) {
    instruction = -1;
    spawnedEffect = 0;
    var t = stage.t;
    var b = t - 0.8 * stage.h;
    var x = 0, y = Lerp(t, b, time / sum);
    var w = baseNoteWidth * If(HasSkinSprite(Sprites.NormalFlick), 2, 1), 
        h = w / If(HasSkinSprite(Sprites.NormalFlick), flickRatio, fallBackNoteRatio);
    var L = x - w / 2, R = x + w / 2, B = y - h / 2, T = y + h / 2;
    Draw(
        If(HasSkinSprite(Sprites.NormalFlick), Sprites.NormalFlick, Sprites.FallbackNormalFlick), 
        { L, B }, 
        { L, T }, 
        { R, T }, 
        { R, B }, 
        10000, 
        1
    );
}

Blocked SonolusApi FlickNoteFrozen(var time, var sum) {
    instruction = int(iTexts.Flick);
    spawnedEffect = 0;
    var x = 0, y = stage.t - 0.8 * stage.h;
    var w = baseNoteWidth * If(HasSkinSprite(Sprites.NormalFlick), 2, 1), 
        h = w / If(HasSkinSprite(Sprites.NormalFlick), flickRatio, fallBackNoteRatio);
    var L = x - w / 2, R = x + w / 2, B = y - h / 2, T = y + h / 2;
    Draw(
        If(HasSkinSprite(Sprites.NormalFlick), Sprites.NormalFlick, Sprites.FallbackNormalFlick), 
        { L, B }, 
        { L, T }, 
        { R, T }, 
        { R, B }, 
        10000, 
        1
    );
    drawUpperHand(iIcons.Hand, x, y, time / (sum / 4) % 1, handEndAngle);
}

Blocked SonolusApi FlickNoteHit(var time, var sum) {
    instruction = -1;
    if (spawnedEffect == 0) {
        var x = 0, y = stage.t - 0.8 * stage.h;
        var l = x - effectWidth, r = x + effectWidth, b = y - effectWidth, t = y + effectWidth;
        SpawnParticleEffect(Effects.Perfect, { l, b }, { l, t }, { r, t }, { r, b }, 0.5, false);
        spawnedEffect = 1;
    }
}

defineTutorialSegment(FlickNoteFall, tutorialFallTime);
defineTutorialSegment(FlickNoteFrozen, tutorialClickTime);
defineTutorialSegment(FlickNoteHit, tutorialDisappearTime);
defineTutorialSegmentsGroup(FlickNote);