Blocked SonolusApi drawJudgeline(var, var) {
    var x = 0, y = stage.b + stage.h * 0.2;
    var l = stage.l, r = stage.r;
    var b = y - judgelineHeight / 2, t = y + judgelineHeight / 2;
    Draw(
        If(HasSkinSprite(Sprites.AllPerfectJudgeline), Sprites.AllPerfectJudgeline, Sprites.NormalJudgeline), 
        { l, b },
        { l, t }, 
        { r, t }, 
        { r, b }, 
        1000, 
        1
    );
}

defineTutorialStatic(drawJudgeline);